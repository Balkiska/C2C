### **Partie 1 : Préparation**

- [x]  Lire et comprendre les conditions de l'examen.
- [x]  Choisir un contexte dans ceux proposés ( cacher un fichier )
- [x]  Déterminer l’objectif du Bonus (scénario→ exfiltration de fichier)
-  Renseignements et Documentation techniques sur :
    - [x] LD_Preload
    - [x] linker
    - [x] types de threads et leur utilisation en c
    - [x] memory ordering
- [x]  Renseignement sur Valgrind,Thread Sanitizer et l’exfiltration de fichiers sur linux
- [x]  DOC PART 1

### **Partie 2 : Développement technique**

**Configurer l'environnement ( à ajouter au readme) :**

- Installer et configurer les outils nécessaires:
    - [x]  GCC (version du compilateur)
    - [x]  Valgrind
    - [x]  Thread Sanitizer
- [ ]  Préparer un espace de travail structuré avec des fichiers `.c` séparés pour chaque module. + les définir
- [ ]  Initialiser un dépôt Git.
- [ ]  Ajouter un Makefile pour compiler et organiser facilement le code + flags de compilation .

**Module de création du malware:** 

- [ ]  Diviser le projet en modules :
    - [ ]  Module pour cacher le fichier.
    - [ ]  Module pour le bonus (exfiltration de fichier)
- [ ]  Fonctionnement en User par défault et root
- [ ]  Exfiltration de clé ssh via hook via des fonctions open ssl

**Module pour masquer le malware :**

- [ ]  Utiliser `LD_PRELOAD` ou des hooks système pour cacher le malware
- [ ]  Utilisation de ptrace pour éviter de le débugger

**Créer un serveur multi-threaded :**

- [ ]  Implémenter du port knoking
- [ ]  Utiliser les bibliothèques standard de C (`pthread`, `sys/socket.h`).
- [ ]  Implémenter un serveur TCP basique qui écoute sur un port spécifié.
- [ ]  Chaque connexion client est gérée par un thread distinct.
- [ ]  Créer une socket et l’attacher à une adresse IP/port.
- [ ]  Accepter les connexions entrantes avec `accept()`.
- [ ]  Assurer une gestion multi-thread avec `pthread_create()` pour chaque client.
- [ ]  Créer une structure pour stocker les informations du client (e.g., IP, port, clés SSH)

**Fonctions d'API :**

- [ ]  Ajouter un client à une liste ou une base de données en mémoire.
- [ ]  Associer une clé SSH à un ID client.

**Tests techniques :**

- [ ]  Utiliser Valgrind pour vérifier les fuites mémoire.
- [ ]  Vérifier que le fichier reste caché même après un redémarrage ou des accès multiples.

**Validation du scénario :**

- [ ]  DOC PART 2 + démontrer le déroulement du scénario avec logs ou visualisation.
