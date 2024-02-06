@echo off
setlocal enabledelayedexpansion


REM Construire le chemin complet
set "chemin_complet=%cd%%"

REM Créer la variable d'environnement EXTRA_ZEPHYR_MODULES s'il n'existe pas
if not defined EXTRA_ZEPHYR_MODULES (
    setx EXTRA_ZEPHYR_MODULES "%chemin_complet%"
    echo La variable d'environnement EXTRA_ZEPHYR_MODULES a été créée avec la valeur : %chemin_complet%
) else (
    REM Vérifier si la valeur existe dans la variable
    set "valeur_existe="
    for %%a in (!EXTRA_ZEPHYR_MODULES!) do (
        if /i "%%a"=="%chemin_complet%" (
            set "valeur_existe=1"
        )
    )

    REM Si la valeur n'existe pas, l'ajouter à la variable
    if not defined valeur_existe (
        setx EXTRA_ZEPHYR_MODULES "!EXTRA_ZEPHYR_MODULES!;%chemin_complet%"
        echo La valeur %chemin_complet% a été ajoutée à la variable EXTRA_ZEPHYR_MODULES.
    ) else (
        echo La valeur %chemin_complet% existe déjà dans la variable EXTRA_ZEPHYR_MODULES.
    )
)

REM Attendre que l'utilisateur appuie sur Entrée pour finir l'exécution du script
pause
