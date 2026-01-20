## Résumé
Afin de facilement pouvoir configurer les dialogues du jeu, nous utilisons la plugin [FlowGraph](https://github.com/MothCocoon/FlowGraph) de MothCocoon. 
Une présentation sommaire en vidéo de ce dernier est disponible [ici](https://www.youtube.com/watch?v=BAqhccgKx_k).

Le plugin utilise un système de nodes similaire à celui des blueprints, et permet de décrire une suite d'actions à effectuer successivement en reliant la sortie de l'action 1 à l'entrée de l'action 2.

## FlowGraph, FlowNode
Un ***FlowGraph*** est un graphe qui va contenir des ***FlowNodes***, des blocs qui réalisent une action lorsqu'ils sont déclenchés. 
Au sein d'un FlowGraph, on définit un enchainement successif de FlowNodes qui va décrire la logique du jeu.
## Utilisation
### Possibilité 1 - Définir un FlowGraph pour un niveau
Lorsque le niveau s'ouvre, il va lire le FlowGraph et effectuer les actions qu'il contient.
Exemple : On veut que 10 secondes après être arrivé dans le niveau, un dialogue s'affiche pour que Thalia décrive ce qu'elle voit autour d'elle.
Ce FlowGraph peut être défini dans les WorldSettings du niveau, en tant que "Root Flow"

### Possibilité 2 - Définir un FlowGraph pour un acteur
Pour ce faire, il faut attribuer un Flow Component à l'acteur.
Ce component permet deux choses : 
1 - lire le FlowGraph définir comme "RootFlow" au démarrage de l'acteur
2 - Assigner des ***[tags](https://dev.epicgames.com/documentation/en-us/unreal-engine/gameplay-tags?application_version=4.27) d'identité*** à l'acteur, qui vont permettre au FlowGraph (et au jeu de façon générale) de l'identifier

## FlowNodes
### FlowNodes de base du plugin

- ### Subgraph : 
	Lit un graphe et attend que le FlowGraph lu appelle la node "Finish" pour continuer
- ### Notify Actor  : 
	Envoie une notification avec les tags de "Notify Tags" à tous les acteurs qui possèdent un FlowComponent avec les tags "Identity Tags"
- ### On Actor Registered : 
	Attend qu'un acteur avec un FlowComponent et les tags "Identity Tags" soit créé pour déclencher la suite de la logique
- ### On Actor Unregistered :
	Attend qu'un acteur avec un FlowComponent et les tags "Identity Tags" soit supprimé pour déclencher la suite de la logique
- ### On Notify from Actor : 
	Attend de recevoir les tags "Notify Tags" de la part d'un acteur qui possède un FlowComponent avec les tags "Identity Tags" en appelant la fonction "NotifyGraph" dans son blueprint
- ### Play Level Sequence : 
	Joue un Level Sequence
- ### Log :
	Affiche un message de Log (catégorie "LogFlow")
- ### Finish :
	Termine l'exécution du FlowGraph et indique qu'il est terminé (à ne pas oublier de placer quand on a fini d'écrire la logique du Graph pour libérer la place en mémoire et ne pas créer d'erreurs de logique)
### FlowNodes faits maison 
- ### Enable Dialog
	Affiche l'UI de dialogue et désactive les contrôles du joueur
- ### Disable Dialog
	Masque l'UI de dialogue et active les contrôles du joueur (ne pas oublier d'appeler cette node à la fin d'un dialogue)
- ### Display Dialog
	Affiche le texte (compatible RichText) et le nom du personnage qui prononce le texte. Peut afficher des images à droite et gauche pour illustrer le dialogue.
	Possibilité d'activer un effet "machine à écrire" (les lettres apparaissent une par une) et de régler manuellement le délai entre les lettres (avec "Text delay time")
- ### Player Has Tag
	Teste si le joueur a le tag souhaité. Déclenche la sortie Yes ou No selon si le joueur possède le tag.