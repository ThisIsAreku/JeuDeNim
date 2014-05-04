# JeuDeNim
## Un mélange entre Puissance 4 et le Morpion

### Touches
P: placer un jeton
R: rotation antihoraire
T: rotation horaire
D: Supprimer un jeton

O,K,L,M: Déplacer le plateau

En ligne de commande, -debug active les informations de debogage dans stderr
```./bin/JeuDeNim -debug 2>debug.txt```

### Note
Ncurses ne libère pas toute la mémoire, entre 300 et 500 blocs ne sont psa libéré ("still reachable" dans valgrind).
D'après la documentation Ncurses: 
~~~
Any implementation of curses must not free the memory associated with
a screen, since (even after calling endwin()), it must be available
for use in the next call to refresh().  There are also chunks of
memory held for performance reasons.
~~~

### Sites web utilisés:
* http://tldp.org/HOWTO/text/NCURSES-Programming-HOWTO
* http://pubs.opengroup.org/onlinepubs/7990989775/xcurses/curses.h.html
        
## License

~~~
JeuDeNim

Copyright 2013-2014 Alexandre Boucey <alexandre.boucey@alumni.univ-avignon.fr>

JeuDeNim is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

JeuDeNim is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with JeuDeNim.  If not, see <http://www.gnu.org/licenses/>.
~~~
