# NECRemote

Contrôle de vidéoprojecteurs. 

Cette application permet de contôler deux vidéo-projecteurs de marque NEC modèle PX1005QL.
Deux utilisateurs manipulent les vidéoprojecteurs : 
- L'opérateur qui peut changer de port d'entrée des vidéoprojecteurs à tout moment
- L'administrateur qui à l'aide d'un mot de passe autorise le changement de port à l'opérateur sur les ports de son choix

Pour mieux fonctionner, veillez suivre les étapes suivantes : 

1 - Ouvrir le fichier ./utils.txt, il contient une seule ligne sou le format xxxx-zzz.zzz.zzz.zzz-yyy.yyy.yyy.yyy,avec 
      xxxx mot de passe de l'administrateur
      zzz.zzz.zzz.zzz l'adresse IP du premier videoprojecteur
      yyy.yyy.yyy.yyy l'adresse du second vidéoprojecteur
      
2 - Lancer la télécommande. Celle-ci va tenter une connexion avec chaque videoprojecteur et ce à tout momemnt même si on perd la connexion,
      la télécommande est capable de détecter une nouvelle connexion
      
 COMPILATION : 
 
 $make clean
 $qmake
 $make
 
 OUVERTURE : 
 
 $./NECRemote
