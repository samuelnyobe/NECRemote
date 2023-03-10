# NECRemote

Contrôle de vidéoprojecteurs. 

Cette application permet de contôler deux vidéo-projecteurs de marque NEC modèle PX1005QL.
Deux utilisateurs manipulent les vidéoprojecteurs : 
- L'opérateur qui peut changer de port d'entrée des vidéoprojecteurs à tout moment
- L'administrateur qui à l'aide d'un mot de passe autorise le changement de port à l'opérateur sur les ports de son choix

Pour mieux fonctionner, veillez suivre les étapes suivantes : 

1 - Ouvrir le fichier ./utils.txt, il contient une seule ligne sou le format xxxx-zzz.zzz.zzz.zzz-yyy.yyy.yyy.yyy, avec \newline
      xxxx mot de passe de l'administrateur\newline
      zzz.zzz.zzz.zzz l'adresse IP du premier videoprojecteur\newline
      yyy.yyy.yyy.yyy l'adresse du second vidéoprojecteur\newline
      
2 - Lancer la télécommande. Celle-ci va tenter une connexion avec chaque videoprojecteur et ce à tout momemnt même si on perd la connexion,
      la télécommande est capable de détecter une nouvelle connexion
      
 COMPILATION : \newline
 
 $make clean\newline
 $qmake\newline
 $make\newline
 
 OUVERTURE : \newline
 
 $./NECRemote


Résultats\newline

![alt text](https://github.com/samuelnyobe/NECRemote/blob/master/img/remote1.png?raw=true)

![alt text](https://github.com/samuelnyobe/NECRemote/blob/master/img/remote2.png?raw=true)

![alt text](https://github.com/samuelnyobe/NECRemote/blob/master/img/remote3.png?raw=true)

