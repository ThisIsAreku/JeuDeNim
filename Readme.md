# SudokuSolver
## Solveur de sudoku optimisé en C++
[![Build Status](https://travis-ci.org/ThisIsAreku/SudokuSolver.png?branch=master)](https://travis-ci.org/ThisIsAreku/SudokuSolver)

Voici trois implémentations du Solver de sudoku:

* __BasicSudokuSolver__: ne fonctionne pas, ajouté pour respecter le sujet.
* __AdvancedSudokuSolver__: résout les sudoku par la méthode du backtracking
* __UberSudokuSolver__: trouve le maximum de cases à remplir par déduction avant de lancer une phase de backtracking

#### BasicSudokuSolver
Cette implémentation ne fait rien. Le code de résolution est bien présent, mais l'énoncé est trop flou pour pouvoir résoudre correctement les sudoku
Tester toutes les solutions "à l'aveugle" est impossible et prendrait dans certain cas beaucoup trop de temps.

#### AdvancedSudokuSolver
Cette implémentation peut résoudre n'importe quel sudoku en un temps dépendant de la complexité. Pour des sudoku simples, le temps de résolution est inférieur à 1 sec. Pour des sudoku complexes, le temps peut dépasser les 4 sec.

#### UberSudokuSolver
Cette implémentation est la plus complexe des 3. C'est aussi celle qui permet le meilleur temps de complétion. Pour des sudoku de niveau moyens, elle divise le temps nécéssaire à lé résolution, ainsi que le nombre de tentatives par deux. Elle s'appuye sur l'implémentation AdvancedSudokuSolver pour terminer la résolution du sudoku, lorsque toutes les déductions ont été effectués.

## Utilisation
1. Compilez le programme. il est recommanté d'utiliser `clang++`. Le makefile contient toute les instructions nécessaire, vous devez simplement lancer la commande `make`
2. Lancez le programme avec en paramètres une des grilles fournie, ou utilisez le script pour télécharger et résoudre une grille aléatoire

        $ ./bin/Sudoku grilles/level_5.txt
        $ ./download_and_test.sh 2
        
## License

~~~
SudokuSolver

Copyright 2013-2014 Alexandre Boucey <alexandre.boucey@alumni.univ-avignon.fr>

SudokuSolver is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

SudokuSolver is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with SudokuSolver.  If not, see <http://www.gnu.org/licenses/>.
~~~
