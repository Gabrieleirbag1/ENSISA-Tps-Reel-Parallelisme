# COMPTE RENDU

Le code est regroupé dans le dossier [`src/`](./src/) :

- [`main.cpp`](./src/main.cpp) contient la génération des graphes, les algorithmes et les mesures de temps
- [`summit.hpp`](./src/summit.hpp) déclare la classe Summit
- [`summit.tpp`](./src/summit.tpp) contient l'implémentation de Summit

## Étape 1 : génération du graphe

J'ai commencé par créer une classe simple représentant un sommet. Un graphe orienté pondéré est ensuite généré aléatoirement sous forme de vector. Chaque élément reçoit un poids compris entre 1 et 20.

Le vector est ensuite convertie en matrice des distances. La diagonale est initialisée à zéro et les cases sans arête sont initialisées avec la valeur INF.

Pour tester l'influence de la taille du graphe, le programme utilise plusieurs valeurs de N (étape 4) mais au départ il y avait une valeur MAX_SUMMIT qui était égale à 1000.

## Étape 2 : Roy-Floyd-Warshall séquentiel

J'ai implémenté l'algorithme de Roy-Floyd-Warshall (j'ai du chercher ce que c'était) pour calculer les chemins les plus courts entre toutes les paires de sommets.

L'algorithme parcourt successivement chaque sommet intermédiaire et met à jour les distances de la matrice. Je l'utilise comme première référence pour les comparaisons de temps et de résultats affichées dans le terminal.

## Étape 3 : version avec `std::thread`

Pour paralléliser Roy-Floyd-Warshall, j'ai du répartir les lignes de la matrice entre plusieurs threads et chaque thread traite une partie différente des sommets sources.

Les threads sont synchronisés à chaque sommet intermédiaire avec un join() ce qui est nécessaire avant de passer au sommet intermédiaire suivant, car sinon les calculs sont faux et ne correspondent pas aux bons indexs.

## Étape 4 : version OpenMP

J'ai repris la même répartition des lignes avec OpenMP. Une section parallèle est créée pour chaque sommet intermédiaire et chaque thread traite un intervalle de lignes.

C'est à cette étape que j'ai remplacé MAX_SUMMIT par N, et j'ai simplement mis une boucle dans main et changé les paramètres des fonctions qui prenaient la constante globale MAX_SUMMIT.

J'ai mis 4 threads pour chaque test. Les deux versions parallèles sont comparées à la version séquentielle avec ce calcul pour le speedup :

```text
speedup = temps séquentiel / temps parallèle
```

Enfin on voit à la fin les temps et les speedups affichés séparément pour chaque valeur de N.

## Étape 5 : BFS

Comme extension, j'ai ajouté le BFS c'est à dire le parcours en largeur. Il part du premier sommet et utilise une queue pour visiter chaque sommet atteignable, et le BFS mémorise le nombre d'éléments nécessaires pour l'atteindre. Enfin je mesure le temps d'exécution et affiche le nombre de sommets atteignables.

## Compilation et exécution

J'ai créé le fichier [`Makefile`](./Makefile) pour la compilation.
L'exécutable est généré dans dist/main.

Pour compiler le projet :

```bash
make
```

Pour compiler puis lancer le programme :

```bash
make run
```

Pour supprimer les fichiers compilés :

```bash
make clean
```

## Résultats et conclusion

J'ai réalisé des essais avec le même type de graphe pour les différentes tailles de N, et les versions std::thread et OpenMP permettent de comparer l'effet du parallélisme sur le temps de calcul de Roy-Floyd-Warshall via le speedup.

On voit que le gain dépend de la taille du graphe : pour les petites tailles, le coût de création et de synchronisation des threads peut limiter le speedup. Pour les graphes plus grands, on voit que ça a un réel impact. Également, le BFS est beaucoup plus rapide que Roy-Floyd-Warshall lorsqu'il est lancé depuis une seule source, car il ne cherche pas les distances entre toutes les paires de sommets.

Voir  [`figure_1.png`](./figure_1.png) pour voir mes résultats.