# architecture_syst-me

o Notice d’utilisation du logiciel :

Pour juste le lancer : ./planificateur
On obtiens la notice :

Utilisation du planificateur: ./planificateur <iterations> <delai (seconde)> <"commande"> <date (facultatif)>

Options :
infini : <itération> = i
date : <date> = JJ/MM/AAAA-HH:MM

Pour arreter le plannificateur de taches : Ctrl+C (sauf pour infini : kill le terminal)

o Cas d’utilisation :

normal = ./planificateur 2 5 "ls"

chevauchement de 2 commandes = ./planificateur 2 5 "echo 'bjr'; sleep 10; echo 'bye';"

infini = ./planificateur i 5 "ls"

date = ./planificateur 2 5 "ls" 11/04/2024-09:00

date + infini + chevauchement = ./planificateur i 3 "echo 'bjr'; sleep 10; echo 'bye';" 11/04/2024-09:00
