## 👹 Matt-Deamon

Le projet est de lancé un serveur où des utilisateurs peuvent se connecter et envoyer la commande "quit" pour arrêter le "deamon".

Un deamon est un programme en arrière plan. Le projet est donc un serveur qui permet de faire d'autres commandes que celle demandée et de communiquer avec les différents clients.

Il y a aussi un logger, toutes les commandes et actions sont enregistrées dans un fichier .log.

# 🤖 Project

Pour lancer le projet :

```make```

Puis on lance l'exécutable :

```./MattDaemon```

Il faut être root pour qu'il se lance.

---

# 🚀 Lancement

Une fois lancé, le programme :

- Se détache du terminal

- Devient un vrai daemon Unix

- Ouvre un port pour accepter des connexions entrantes

- Commence à logger toutes les actions dans un fichier .log

# 📡 Fonctionnement général

Pour se connecter, ici le port est 4242:

```
nc 127.0.0.1 <port_du_daemon>
```

---

# 🗒️ Commandes supplémentaires :

Si on ne met pas de / avant un mot, alors on envoie un message à tous les clients connectés.

| Commandes   | Dêfinitions |                
|------------|-------------|
| **/login <name> <password>**   | Permet de se connecter |
| **/quit ou quit**   | Éteint le serveur et ferme le deamon |
| **/leave**   | Déconnecte le client |
| **/list (-a)**| Affiche tous les clients (-a affiche tous les noms dans la base de données) |
| **/msg <name> : <msg>**   | Envoie un message à un client précis |
| **/help**   | Affiche toutes les commandes |
---

Les logs contiennent les actions faites par chaque client.

Exemple :

<img width="599" height="518" alt="Screenshot from 2025-11-18 16-13-33" src="https://github.com/user-attachments/assets/65ac3003-903d-4332-8259-ae08beac0312" />

Deux utilisateurs se sont connectés et on fait plusieurs commandes.

# 🧷 Notes techniques

Le deamon ne peut être lancé qu'une seule fois, impossible d'avoir deux fois le programme en arrière plan.

Un .lock est créé, ce qui permet au code de vérifier si il est déjà lancé ou non, si il l'est, une erreur sera envoyée dans les logs et le second ne se lance pas.

Pour détacher un programme du terminal, il faut créer un sous processus (fork()), changer l'id (setsid()), puis refaire un sous processus. Le programme est détaché du terminal.

# ⭐ Bonus :

Pour lancer le projet :

```make bonus```

Puis on lance l'exécutable :

```./Ben-AFK <ip_du_daemon> <port_du_daemon>```

---

Le terminal prend une autre apparence, c'est un client graphique.

---

Il y a une base de données pour se connecter.

Les mots de passe sont "Hachés".

---

Un petit script (test.sh) permet de se connecter, faire des commandes et arrêter le deamon. Il permet de générer et vérifier les logs rapidement. 
