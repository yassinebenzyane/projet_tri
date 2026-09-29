# Projet de comparaison des algorithmes de tri

Ce dossier contient le squelette du projet.

## Structure attendue
- src/ : code source C des algorithmes
- include/ : en-têtes
- tests/ : scripts et fichiers de validation
- scripts/ : automatisation de compilation et mesures
- docs/ : rapport, synthèses et graphiques

## Contrat de sortie
Le fichier resultats.csv doit respecter le format suivant :

algorithme,type_donnees,n,repetition,temps_ms,comparaisons,echanges

## Règles
- Ne pas modifier le format du CSV sans accord préalable.
- Les mesures doivent être reproductibles.
- Les algorithmes doivent être validés avant benchmarking.
