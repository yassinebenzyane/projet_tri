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


## Benchmark

- Le fichier `src/benchmark.c` permet de mesurer les performances des algorithmes de tri sur différentes tailles et différents types de données. Il vérifie également que le tableau est correctement trié et enregistre le temps, les comparaisons et les échanges.

## Statistiques

- Le fichier `resultats_statistiques.csv` contient, pour chaque algorithme, type de données et taille `n`, la moyenne et l'écart-type des temps obtenus après plusieurs répétitions. Il est utilisé pour l'analyse et la création des graphiques.
