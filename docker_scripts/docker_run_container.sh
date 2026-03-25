#!/bin/bash

# Definisci i nomi
IMAGE_NAME="fra2mo_real_image"
CONTAINER_NAME="fra2mo_real_container"

echo "Avvio del container: $CONTAINER_NAME"

# Abilita le connessioni al server grafico X dell'host (utile dato che usi Ubuntu Desktop sul Pi)
xhost +local:root

# Lancia il container con tutte le flag necessarie
docker run -it \
    --name $CONTAINER_NAME \
    --rm \
    --network host \
    --privileged \
    -e DISPLAY=$DISPLAY \
    -e QT_X11_NO_MITSHM=1 \
    -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
    -v /dev:/dev:rw \
    -v "$(pwd)/src:/root/ros2_ws/src:rw" \
    $IMAGE_NAME