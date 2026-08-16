#!/bin/bash

CONFIGFILE=docker/config/tools.sh
source $CONFIGFILE

docker build \
    --network=host \
    -f $DOCKERFILE \
    -t $IMAGE_NAME:$IMAGE_TAG \
    --rm \
    .

# Use Bash as the shell for this script.
# --network=host \ -> Use the host's network, not creating a separate network stack for the Docker container.

# -f $DOCKERFILE \ -> Specify the Dockerfile to use, named 'docker/robot.dockerfile' (defined into tools.sh).

# -t $IMAGE_NAME:$IMAGE_TAG \ -> Tag the built image as 'ros-jazzy:ros-jazzy' (defined into tools.sh).

# --rm \ -> Remove intermediate containers after a successful build.

# Build a Docker image from a Dockerfile located in the current directory.
