# NECRemote

Contrôle de vidéoprojecteurs.

Cette application permet de contôler deux vidéo-projecteurs de marque NEC modèle PX1005QL. Deux utilisateurs manipulent les vidéoprojecteurs : - L'opérateur qui peut changer de port d'entrée des vidéoprojecteurs à tout moment - L'administrateur qui à l'aide d'un mot de passe autorise le changement de port à l'opérateur sur les ports de son choix

Pour mieux fonctionner, veillez suivre les étapes suivantes :

1 - Ouvrir le fichier ./utils.txt, il contient une seule ligne sous le format xxxx-zzz.zzz.zzz.zzz-yyy.yyy.yyy.yyy, avec :<br /> xxxx mot de passe de l'administrateur<br /> zzz.zzz.zzz.zzz l'adresse IP du premier videoprojecteur<br /> yyy.yyy.yyy.yyy l'adresse du second vidéoprojecteur<br />

2 - Il faut lancer l'émulateur du vidéoprojecteur qui se trouve dans le répertoire ./NECVideoProjectorEmulator.

Celui-ci a pour rôle de simuler le comportement d'un vidéoprojecteur, il affiche son adresse et le port sur lequel il écoute. Cette adresse doit correspondre à ce qui est marqué dans le ficher ./utils.txt. <br />

3 - Lancer la télécommande. Celle-ci va tenter une connexion avec chaque videoprojecteur et ce à tout momemnt même si on perd la connexion, la télécommande est capable de détecter une nouvelle connexion

COMPILATION : (Valable pour la télécommande et pour l'émulateur du vidéoprojecteur) <br />

\$qmake6<br /> \$make<br />

Ou alors ouvrir le fichier .pro à partir de Qt Creator et le compiler directement de là. La version de Qt utilisé est la version 6.

OUVERTURE : <br />

\$./NECRemote

Résultats<br />

![alt text](https://github.com/samuelnyobe/NECRemote/blob/master/img/remote1.png?raw=true)

![alt text](https://github.com/samuelnyobe/NECRemote/blob/master/img/remote2.png?raw=true)

![alt text](https://github.com/samuelnyobe/NECRemote/blob/master/img/remote3.png?raw=true)
