VERSION TEST DU MANUEL
-> pdf à venir

I - Setup

    I.1 : Windows

        2 choix : 
        - Définir des variables d'environnement pour les paths vers :
            - Dossier include de wxWidgets
            - Dossier contenant wx/setup.h de wxWidgets
            - Dossier de librairies de wxWidgets

            - Dossier include de openCV
            - Dossier de librairies de openCV

        - Copier les libs (dll) nécessaires dans un dossier libs du projet

    I.2 : Linux
        On utilise pkg-config et wx-config donc géré automatiquement

II - Fonctionnement

1 - Sélection du type de calibrage (perspective / omnidirectionnel)
1.bis - Définition des préférences de base (paramètres de mire, render/search size)
2 - Chargement d'images (au moins 3, de même dimensions)

2.bis - On peut choisir ici ou après EGC les paramètres à prendre en compte

3 - EGC -> surligne les coins détectés et à la fin, doit confirmer si ok ou non
    A VOIR : modèle "tout ou rien complet" (soit tout bon, soit tout à refaire)
             modèle "tout ou rien" (soit tout bon, soit on ignore les pas bons)
             modèle autre ?

4 - calibration -> fait le calcul
5 -> choix entre results et voir reprojection
6 -> pareil
Possibilité de save
Possibilité de load