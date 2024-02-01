#!/bin/bash

# Nom de la variable à ajouter dans le .bashrc
variable="ZEPHYR_BASE"

# Valeur à assigner à la variable
valeur="~/zephyrproject/zephyr"

# Chemin vers le fichier .bashrc
bashrc_path="$HOME/.bashrc"

# Vérifier si la variable existe déjà dans le .bashrc avec la même valeur
if grep -q "^export $variable=$valeur$" "$bashrc_path"; then
    echo "La variable $variable avec le chemin $valeur existe déjà dans le fichier $bashrc_path."
else
    # Vérifier si la variable existe déjà avec une autre valeur
    if grep -q "^export $variable=" "$bashrc_path"; then
        echo "La variable $variable existe déjà dans le fichier $bashrc_path avec un autre chemin."
    else
        # Ajouter la ligne d'exportation au .bashrc
        echo "export $variable=$valeur" >> "$bashrc_path"
        echo "La variable $variable a été ajoutée à $bashrc_path avec le chemin $valeur."
    fi
fi
