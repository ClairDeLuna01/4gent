# Stratégie

Pour qu'un agent détermine le meilleur coup à jouer, il faut que le coup maximise le potentiel des prochains coups de l'agent, mais aussi qu'il minimise les options du joueur adverse.

## Exemple:

Si l'agent joue en premier, le meilleur coup à jouer est mettre son jeton au milieu car le coup maximise le potentiel en permettant de gagner à droite et à gauche.
Si l'agent joue en deuxième, le meilleur coup à jouer est mettre son jeton sur le pion adverse, pour minimiser les options du joueur adverse.

## Comment calculer le facteur de maximisation/minimisation

- Combien de combinaisons de 4 pourrait-on former depuis l'emplacement que l'on visite ? -> L'agent devrait privilégier les coups qui créent des combinaisons de 2 transformables en combinaisons de 4.
- Comment cette action pourrait bloquer le joueur adverse ? -> L'agent devrait privilégier les coups qui bloquent les combinaisons de 3.

- Parcours des coups possibles en profondeur limitée, en supposant que le joueur adverse va réaliser son meilleur coup possible. Largeur limitée? (à voir si la complexité est assez petite pour que l'agent solve le jeu directement).
- Déterminer le coup qui amènerait a la victoire la plus rapide? (possiblement prendre le coup qui donne les branches qui donnent les coups qui donnent une victoire le plus rapidement).
- Si la victoire n'est pas possible, déterminer le coup qui amènera à coup sur au draw (possiblement prendre le coup qui donne le plus de branche qui mène vers le draw/le moins de branche qui mène vers une défaite).