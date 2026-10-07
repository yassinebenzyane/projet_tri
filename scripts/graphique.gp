# Graphique : temps moyen = f(n), donnees aleatoires, une courbe par algorithme.
# A lancer depuis la racine du projet :  gnuplot scripts/graphique.gp
# Entree : resultats_statistiques.csv   Sortie : docs/temps_random.png

fichier = "resultats_statistiques.csv"
type    = "random"
algos   = "Bubble Selection Insertion Shell Merge Quick"

# 1) Filtrage : pour chaque algorithme, on copie ses lignes (n, moyenne_ms)
#    dans un bloc memoire $D1 ... $D6. Les lignes d'un meme algorithme ne se
#    suivent pas dans le CSV : sans ce filtrage, les courbes seraient coupees.
set datafile separator ","
garder(i) = strcol(1) eq word(algos, i) && strcol(2) eq type

do for [i = 1:words(algos)] {
    eval sprintf('set table $D%d', i)
    plot fichier skip 1 using 3:4 with table if (garder(i))
    unset table
}
set datafile separator whitespace

# 2) Mise en forme
set terminal pngcairo size 1000,650 font "Segoe UI,11"
set output "docs/temps_random.png"

set title "Temps de tri en fonction de n (donnees aleatoires)"
set xlabel "n (taille du tableau)"
set ylabel "temps moyen (ms)"

# Axes logarithmiques : n va de 10^3 a 10^7, le temps de 0,1 ms a plusieurs secondes
set logscale xy
set xrange [500:2e7]
set format x "10^{%L}"
set format y "10^{%L}"
set grid xtics ytics lc rgb "#e1e0d9"
set key top left box opaque

# Une couleur et une forme de point par algorithme (lisible aussi en noir et blanc)
couleur(i) = word("#2a78d6 #eb6834 #1baf7a #eda100 #e87ba4 #008300", i)

# 3) Trace
plot for [i = 1:words(algos)] sprintf('$D%d', i) using 1:2 \
     with linespoints lw 2 pt (i + 4) ps 1.2 lc rgb couleur(i) \
     title word(algos, i)
