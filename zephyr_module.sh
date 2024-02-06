#!/bin/bash

# Récupérer le chemin du répertoire actuel
current_directory=$(pwd)
fichier_temporaire=$(mktemp)

# Nom de la variable
variable_name="EXTRA_ZEPHYR_MODULES"


#!/bin/bash

# Valeur à rechercher
valeur_a_chercher="export $variable_name=$current_directory"

# Chemin du fichier
chemin_du_fichier="$HOME/.bashrc"

# Vérifier si la valeur existe dans le fichier
if grep -i "$valeur_a_chercher" "$chemin_du_fichier" > /dev/null ; then
    echo "La valeur '$valeur_a_chercher' existe dans le fichier .bashrc"
    read -p "Voulez-vous la supprimer (y/n)? " erase
    if [ "$erase" = "y" ]; then
    	# Lecture du fichier ligne par ligne
	while IFS= read -r ligne
	do
	    # Vérifier si la ligne contient le motif
	    if [[ ! "$ligne" =~ "$valeur_a_chercher" ]]; then
		# Si la ligne ne contient pas le motif, l'ajouter au fichier temporaire
		echo "$ligne" >> "$fichier_temporaire"
	    fi
	done < "$chemin_du_fichier"

	# Remplacer le fichier d'origine par le fichier temporaire
	mv "$fichier_temporaire" "$chemin_du_fichier"

	# Supprimer le fichier temporaire
	rm -f "$fichier_temporaire"
    else
    	echo "Opération annulée. Aucune modification effectuée."
        exit 0
    fi    	
else
    echo "La valeur '$valeur_a_chercher' n'existe pas dans le fichier."
    # Définir la variable système avec le chemin du répertoire actuel
    echo "export $variable_name=$current_directory" >> $chemin_du_fichier
    echo "La valeur '$valeur_a_chercher' a été ajoutée dans .bashrc"
    exit 0
fi


