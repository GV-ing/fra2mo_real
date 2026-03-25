#!/bin/bash

# Definisci il nome dell'immagine
IMAGE_NAME="fra2mo_real_image"

echo "Inizio la build dell'immagine Docker: $IMAGE_NAME"

# Esegue la build. L'istruzione 'if' valuta l'exit code (0 = successo, altro = errore)
if docker build -t $IMAGE_NAME -f Dockerfile .; then
    # Se il comando va a buon fine:
    echo -e "\n✅ Build di $IMAGE_NAME completata"
else
    # Se il comando fallisce:
    echo -e "\n❌ ERRORE CRITICO: La build di $IMAGE_NAME è fallita."
  
fi