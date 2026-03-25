#!/bin/bash

CONTAINER_NAME="fra2mo_real_container"

echo "Connessione a un nuovo terminale nel container: $CONTAINER_NAME"

# Esegue una nuova sessione bash interattiva nel container in esecuzione
docker exec -it $CONTAINER_NAME bash