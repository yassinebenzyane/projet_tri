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

- Le fichier `src/benchmark.c` permet de mesurer les performances des algorithmes de tri sur différentes tailles et différents types de données. Il vérifie également que le tableau est correctement trié (comparaison avec une copie triée par `qsort`, donc tri **et** permutation des données) et enregistre le temps, les comparaisons et les échanges.
- Le temps est mesuré avec `timespec_get` (C11), plus précis que `clock()` qui n'avance que par pas de 1 ms sous Windows.
- Bubble, Selection et Insertion (O(n²)) sont limités à n ≤ 50 000 ; Shell, Merge et Quick vont jusqu'à 10 000 000.

## Statistiques

- Le fichier `resultats_statistiques.csv` contient, pour chaque algorithme, type de données et taille `n`, la moyenne et l'écart-type des temps obtenus après plusieurs répétitions. Il est utilisé pour l'analyse et la création des graphiques.
- L'écart-type est l'écart-type d'échantillon (division par n − 1).

## Compilation et exécution

```
make check   # tests de validation (6 algorithmes x 4 types x 8 tailles)
make bench   # lance les tests, puis le benchmark si tout est valide
```

Sous Windows avec MinGW, utiliser `mingw32-make` à la place de `make`.
