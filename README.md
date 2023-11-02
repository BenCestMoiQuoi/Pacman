################################################
##                                            ##
##                Projet Pacman               ##
##                                            ##
################################################

Projet C++ S8 - INSA_CVL - M_Mille

Réalisation :   Aurore Le Mercier
                Ian Menau
                Corentin Roche

Version :       7

Date :          12/04/2023

Language :      C++
Compilateur :   MinGw

Fichiers :      main.cpp
                pacmanwindow.cpp
                pacmanwindow.hpp
                jeu.cpp
                jeu.hpp
                Makefile

Librairies utilisé : 
                qtbase-small-5.15.2-x64 -> Fournis par les enseignants

Petites explications du projet :
    Le jeu est similaire au jeu Pacman, il peut y avoir jusqu'à 4 Pacmans contre des fantomes (nombre indéterminé à l'avance)
    Les pacmans doivent manger l'intégralité des PacGums pour gagner. Les pacmans peuvent manger des SuperGums pour devenir invincible.
    Lorsque un Pacman se superpose avec un fantome, le Pacman en question est mangé par le fantome.
    Lorsque un Pacman mange une SuperGum, dans une durée de 5s, l'ensemble des pacmans est supérieur aux fantomes.
    Dans ce même cas, le Pacman mangeant la SuperGum à la possibilité de manger les autres joueurs dans le temps de 5s.
    La partie se termine lorsque les pacmans ont mangé l'intégralité des PacGums ou lorsque les pacmans se sont tous fait manger.
    Le score se cumule et s'ajoute pour chaque PacGum mangé en fonction du nombre de fantomes encore sur le plateau.
    Le jeu est modifiable grâce à un fichier .txt qui est lu puis interprété par le programme.


Problèmes rencontrés : 
    Nous avons essayé de prendre en compte le son dans notre jeu.
    Avec plusieurs sons comme : 
        Début du jeu
        Fin du jeu
        Pacman mange un PacGum
        Pacman mange un SUperGum
        La mort d'un des Pacman
        Pacman mange un Fantome
    Mais des problèmes de compréhension sur la bibliothèque FMOD nous ont fait revenir 
    sur cette idée.


Pour toutes questions liées au jeu, merci de contacter par mail l'un des auteurs.

Bon jeu et que la course aux PacGums commence.
