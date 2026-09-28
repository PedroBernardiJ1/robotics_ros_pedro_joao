#!/bin/bash

CONFIGFILE=docker/config/tools.sh
source $CONFIGFILE

xhost +local:docker

# Garante que a pasta exista no host, já com o dono certo, antes do primeiro mount abaixo
mkdir -p ~/.vscode-server-ros

# Update to not use -v for devices
docker run -it --rm \
    -e QT_X11_NO_MITSHM=1 \
    --network=host \
    --ipc=host \
    -v /dev:/dev \
    -v /dev/dri:/dev/dri \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -e DISPLAY=$DISPLAY \
    -v $WORKSPACE_HOST:$WORKSPACE_CONTAINER \
    -v ~/.zsh_history_ros:/home/host/.zsh_history \
    -v ~/.vscode-server-ros:/home/host/.vscode-server \
    --privileged \
    --name $CONTAINER_NAME \
    $IMAGE_NAME:$IMAGE_TAG
    
