## Résumé
Pour configurer facilement les dialogues du jeu, nous utilisons le **plugin** [FlowGraph](https://github.com/MothCocoon/FlowGraph) de MothCocoon. Une présentation vidéo de ce plugin est disponible [ici](https://www.youtube.com/watch?v=BAqhccgKx_k).

Le plugin utilise un système de **nodes** similaire à celui des **blueprints**. Il permet de décrire une suite d'actions à effectuer successivement en reliant la sortie de l'action 1 à l'entrée de l'action 2.

## FlowGraph, FlowNode
Un **FlowGraph** est un **graphe** qui va contenir des **FlowNodes**, des blocs qui réalisent une action lorsqu'ils sont déclenchés. Au sein d'un **FlowGraph**, on définit un enchainement successif de **FlowNodes** qui va décrire la logique du jeu.

## Utilisation
### Possibilité 1 - Définir un FlowGraph pour un niveau
➡️Lorsque le niveau s'ouvre, il va lire le **FlowGraph** et effectuer les actions qu'il contient.

Ce **FlowGraph** peut être défini dans les **WorldSettings** du niveau, en tant que **"Root Flow"**

Exemple : On veut que 10 secondes après être arrivé dans le niveau, un dialogue s'affiche pour que Thalia décrive ce qu'elle voit autour d'elle.

### Possibilité 2 - Définir un FlowGraph pour un acteur
➡️ Permet deux choses :

- Le jeu lira le **FlowGraph** défini par la variable **"RootFlow"** au démarrage de l'acteur (ce qui permet que ce **FlowGraph** se lance au démarrage de l’acteur)
- On peut assigner des [**tags](https://dev.epicgames.com/documentation/en-us/unreal-engine/gameplay-tags?application_version=4.27) **d’identité** à l'acteur, qui vont permettre au **FlowGraph** (et au jeu de façon générale) de l'identifier facilement

Pour ce faire, il faut attribuer un **FlowComponent** à l'acteur.
## FlowNodes

### FlowNodes de base du plugin
- ==Subgraph== : Lit un graphe et attend que le **FlowGraph** lu appelle sa node "Finish" pour continuer
- ==Notify Actor== : Envoie une notification avec les tags définis dans le paramètre **"Notify Tags"** à tous les acteurs qui possèdent un **FlowComponent** avec les **tags d’identité** **"Identity Tags"**
- ==On Actor Registered== : Attend qu'un acteur avec un **FlowComponent** et les **tags d’identité** **"Identity Tags"** soit créé pour déclencher la suite du graphe
- ==On Actor Unregistered== : Attend qu'un acteur avec un **FlowComponent** et les **tags d’identité** **"Identity Tags"** soit supprimé pour déclencher la suite de la logique
- ==On Notify from Actor== : Attend de recevoir une notification avec les **tags "Notify Tags"** de la part d'un acteur qui possède un **FlowComponent** avec les **tags d’identité** **"Identity Tags"** en appelant la fonction **"NotifyGraph"** dans son **Blueprint**
- ==Play Level Sequence== : Joue un **Level Sequence**
- ==Log== : Affiche un message de **Log** (catégorie **"LogFlow"**)
- ==Finish== : Termine l'exécution du **FlowGraph** et indique qu'il est terminé (à ne pas oublier de placer quand on a fini d'écrire la logique du Graph pour libérer la place en mémoire et ne pas créer d'erreurs de logique)
- ==Timer== : Attend le nombre de secondes défini dans **“Completion Time”** puis lit la **FlowNode** suivante

### FlowNodes faits maison
- ==EnableDialog== Affiche l'**UI de dialogue** et désactive les contrôles du joueur
- ==DisableDialog== Masque l'**UI de dialogue** et active les contrôles du joueur (ne pas oublier d'appeler cette node à la fin d'un dialogue)
- ==DisplayDialog== Affiche le texte **“Dialog Text”** (compatible **RichText**) et le nom du personnage qui prononce le texte **“Character Name”** . Affiche des images au dessus du dialogue à droite et gauche, **“Left Image”** et **“Right Image”** pour illustrer le dialogue. Possibilité d'activer un **effet "machine à écrire"** (les lettres apparaissent une par une) en cochant **“Delay Text Appearance”** et de régler manuellement le délai entre les lettres avec **"Text delay time"**. Lorsque le joueur appuie sur le **bouton** sur la bulle de dialogue, cette **FlowNode** déclenche la suite du **FlowGraph**
- ==PlayerHasTag== Teste si le joueur a le tag souhaité. Déclenche la sortie Yes ou No selon si le joueur possède le tag.