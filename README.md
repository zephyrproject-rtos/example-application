![Zephyr Project logo](https://github.com/zephyrproject-rtos/zephyr/blob/v1.14-branch/doc/images/Zephyr-Project.png)


# Repository Overview

The Zephyr Project is a scalable real-time operating system (RTOS) supporting
multiple hardware architectures, optimized for resource constrained devices,
and built with security in mind.

The Zephyr build system supports integration of externally maintained
projects called *modules*. This integration is configured through the
settings in a *module.yml* file in a :file:`zephyr/` folder at the root
of the repository.  See the [Zephyr Modules (External projects)][5]
for a high-level overview of the build system module mechanism.


## Scope

The Zephyr eXample Application (ZXA) module repository is supplementary
to the Zephyr repository.  This provides:

1. A reference implementation of using the Zephyr module mechanism.

1. A repository for holding example code and config files outside of the
   Zephyr repository, including:

   - [west][6] manifest (in the default location)
   - projects/samples/tests
   - applications/subsystems/drivers/interface declarations
   - Devicetree source files
   - boards
   - scripts

1. A test case for CI to monitor Zephyr build system support for [modules][5].


   NOTE: The [Zephyr][2] repository is intentionally
   **independent of and unaware of this repository**.


# Getting Started

## Creating a New Repository

This repository is designed to be a boilerplate for other modules simply by:

1. Cloning

1. Stripping out any unnecessary content (e.g. no drivers or subsystems)

  - If you do not have any non-sample, non-test projects:

    1. Remove the `zephyr/projects/` subdirectory
    1. In `zephyr/modules.yml` remove the reference to `zephyr/projects`

  - If you want the Zephyr directory tree structure to be in the repository root:

    1. Move all the content of the `zephyr/` subdirectory to the top level **EXCEPT**
       `zephyr/module.yml`
    1. Edit `zephyr/module.yml`, replacing every ` zephyr/` with ` ./`.


## Creating a Free-Standing Manifest Repository

This example repository is also an appropriate template for creating a
manifest-only repository because, like modules that also contain a
manifest file, they may contain the following:

- Documentation describing the scope and purpose of the workspace
  configured by the manifest

- Configuration settings for CI pipelines

- Scripts unique to the breadth of the workspace

To strip this repository down to be a manifest-only repository:

  1. Remove all files and subdirectories referenced by `zephyr/module.yml`
  1. Remove the `zephyr/include` subdirectory
  1. Remove `zephyr/module.yml`


## Checking Out Code

The Zephyr repository ZXA may be cloned and added to the Zephyr build system
in exactly the same way as any other repository.

Once the Zephyr development environment has been set up (see [Zephyr Getting Started Guide][3]), then follow instructions here to:

- Get the source code with Zephyr repository ZXA as the [west][6]
  manifest repository
- Build and run projects


## Get Zephyr and Install Python Dependencies

Clone Zephyr repository ZXA and its dependencies into a new [west][6]
workspace named :file:`zp`. You'll also install additional Python
dependencies.

### Ubuntu

1. Get the Zephyr source code:

      ```bash
      west init \
        -m https://github.com/zephyrproject-rtos/example-application \
        --mr main \
        ~/zxa
      cd ~/zxa
      west update
      ```

1. Export a `Zephyr CMake package`. This allows CMake to
   automatically load boilerplate code required for building Zephyr
   applications.

      ```bash
      west zephyr-export
      ```

### macOS

1. Get the Zephyr source code:

      ```bash
      west init \
        -m https://github.com/zephyrproject-rtos/example-application \
        --mr main \
        ~/zxa
      cd ~/zxa
      west update
      ```

1. Export a `Zephyr CMake package`. This allows CMake to
   automatically load boilerplate code required for building Zephyr
   applications.

      ```bash
      west zephyr-export
      ```

### Windows

1. Get the Zephyr source code:

      ```bat
      cd %HOMEPATH%
      west init \
        -m https://github.com/zephyrproject-rtos/example-application \
        --mr main \
        %HOMEPATH%/zxa
      cd %HOMEPATH%\zxa
      west update
      ```

1. Export a `Zephyr CMake package`. This allows CMake to
   automatically load boilerplate code required for building Zephyr
   applications.

      ```bat
      west zephyr-export
      ```

## Build and Run the zxa_example_project

Build the `zxa_example_project` with
`west build`, changing ``<your-board-name>``
appropriately for your board:

### Ubuntu

1. Build projects directly from the command line

      ```bash
      cd ~/zxa
      west build -p auto -b reel_board \
        example-application/zephyr/projects/zxa_example_project
      ```

1. Run projects from the command line

      ```bash
      ./build/example-application/zephyr/projects/zxa_example_project/reel_board/zephyr/zephyr.exe
      ```

1. Build and run projects using Zephyr's `twister script`

      ```bash
      zephyr/scripts/twister \
        -T example-application/zephyr/projects/zxa_example_project
      ```

1. Build and run the all projects (including projects and tests)
   with code coverage using Zephyr's `twister script`

      ```bash
      zephyr/scripts/twister --coverage \
        -T example-application/zephyr
      ```

### macOS

1. Build projects directly from the command line

      ```bash
      cd ~/zxa
      west build -p auto -b reel_board \
        example-application/zephyr/projects/zxa_example_project
      ```

1. Run projects from the command line

      ```bash
      ./build/example-application/zephyr/projects/zxa_example_project/reel_board/zephyr/zephyr.exe
      ```

1. Build and run projects using Zephyr's `twister script`

      ```bash
      zephyr/scripts/twister \
        -T example-application/zephyr/projects/zxa_example_project
      ```

1. Build and run the all projects (including projects and tests)
   with code coverage using Zephyr's `twister script`

      ```bash
      zephyr/scripts/twister --coverage \
        -T example-application/zephyr
      ```

### Windows

1. Build projects directly from the command line

      ```bat
      cd %HOMEPATH%\zxa
      west build -p auto -b reel_board \
        example-application\zephyr\projects\zxa_example_project
      ```

1. Run projects from the command line

      ```bat
      .\build\example-application\zephyr\projects\zxa_example_project\reel_board\zephyr\zephyr.exe
      ```

1. Build and run projects using Zephyr's `twister script`

      ```bat
      zephyr\scripts\twister \
        -T example-application\zephyr\projects\zxa_example_project
      ```

1. Build and run the all projects (including projects and tests)
   with code coverage using Zephyr's `twister script`

      ```bat
      zephyr\scripts\twister --coverage \
        -T example-application\zephyr
      ```


# Module Contribution Guidelines

## Design Principles

The ZXA repository is designed around the following principles:

- Keep all Zephyr-specific content within the (required) `zephyr` folder.

  - This enables adding the example on to an existing repository.

- Follow the Zephyr repository structure, mechanisms, and rules
  whenever possible.

  - This minimizes the variance from what users and developers expect,
    simplifies maintenance, and reuses existing solutions.

- Extend the Zephyr repository structure to support an arbitrary number
  of applications and project configurations.

  - The `projects` sub-tree identifies where non-test, non-sample projects
    are expected to be found.  The `projects` folder is tied into the
    Zephyr CI system through `module.yml`.

- Maximize the amount of code automatically verifiable through Zephyr CI.

- Minimize risk of conflicts in global namespaces through use of the
  module abbreviation.

  - Global namespaces exist in many areas, including the following areas.
    Using the module abbreviation as a global symbol prefix reduces
    the chance of collisions as other modules are created.

    - Kconfig, CMake, C preprocessor symbols
    - include file pathnames
    - test and sample names
    - shell commands
    - settings identifiers


# Community Support

Community support is provided via the Zephyr mailing lists and Slack;
see the Resources at the end of the [Zephyr Introduction][2] page for
details.


# Resources

Here's a quick summary of resources to help you find your way around:

* **Help**: [Asking for Help Tips][4]
* **Nightly CI Build Status**: https://lists.zephyrproject.org/g/builds
  The builds@lists.zephyrproject.org mailing list archives the CI
  (shippable) nightly build results.
* **Zephyr Project Website**: [https://zephyrproject.org][1]

[1]: https://docs.zephyrproject.org
[2]: https://docs.zephyrproject.org/latest/introduction/index.html
[3]: https://docs.zephyrproject.org/latest/getting_started/index.html
[4]: https://docs.zephyrproject.org/latest/guides/getting-help.html
[5]: https://docs.zephyrproject.org/latest/guides/modules.html
[6]: https://docs.zephyrproject.org/latest/guides/west.html

