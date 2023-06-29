#!/bin/bash

set -e

create_tag=false
new_version=""

while (( "$#" )); do
  case "$1" in
    -t|--create-tag)
      create_tag=true
      shift
      ;;
    -n|--new-version)
      if [ -n "$2" ] && [ ${2:0:1} != "-" ]; then
        new_version="$2"
        shift 2
      else
        echo "Error: The argument for '--new-version' is missing" >&2
        exit 1
      fi
      ;;
    *)
      echo "Usage: $0 [--create-tag] [--new-version vx.x.x]"
      shift
      exit 0
      ;;
  esac
done

cleanup() {
  rm -f CHANGELOG.md.part1 CHANGELOG.md.part2
}

trap cleanup EXIT

last_version=$(bash "$(dirname "$0")/get_latest_version.sh")
echo "Last version: $last_version"

if [[ -z "$new_version" ]]; then
  # Increment the patch version by 1 if new_version is not provided
  if [[ "$last_version" == "v0.0.0" ]]; then
    new_version="v0.0.1"
  else
    new_version=$(echo $last_version | awk -F. '{$NF++; print}' OFS=.)
  fi
fi

echo "Generating CHANGELOG.md using commit history from $last_version till $new_version"
git_changelog=$(git-chglog --next-tag "$new_version" "$new_version")

split_position=$(grep -n -m 1 "^## " CHANGELOG.md | cut -d: -f1)
head -n $(($split_position - 2)) CHANGELOG.md > CHANGELOG.md.part1
tail -n +$split_position CHANGELOG.md > CHANGELOG.md.part2

cat CHANGELOG.md.part1 > CHANGELOG.md
echo "$git_changelog" >> CHANGELOG.md
echo "" >> CHANGELOG.md
cat CHANGELOG.md.part2 >> CHANGELOG.md

if $create_tag; then
  echo "Committing the updated CHANGELOG.md"
  git add CHANGELOG.md
  git commit -m "release: $new_version"

  echo "Creating a new tag for the release"
  git tag "$new_version"
else
  echo "Use 'git diff CHANGELOG.md' to display"
fi
