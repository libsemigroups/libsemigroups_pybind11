#!/bin/bash
set -e

CONTAINER_NAME="libsemigroups/libsemigroups-docker:version-2.0.0"

# Pull the docker container
docker pull "$CONTAINER_NAME" 

# Start the docker container detached
ID=$(docker run --rm -i -d "$CONTAINER_NAME")

# Copy the libsemigroups directory into the container
docker cp . "$ID:/home/libsemigroups_pybind11"

# Run the ci/docker-gap.sh script in the running container
docker exec -i "$ID" /home/libsemigroups_pybind11/ci/run-tests-in-docker-container.sh ; exit

# Attach to the container
docker attach "$ID"
