#!/bin/bash
set -e

PREFIX="/home/libsemigroups_pybind11/ci/"

if [ "$GH_ACTIONS_SUITE" == "test" ]; then
  GH_ACTIONS_SCRIPT="$PREFIX/run-tests-in-docker-container.sh"
elif [ "$GH_ACTIONS_SUITE" == "lint" ]; then
  GH_ACTIONS_SCRIPT="$PREFIX/run-lint-in-docker-container.sh"
else
  echo -e "Error, unrecognised suite, expect one of \"test\", or \"lint\", got \"$GITHUB_ACTIONS_SUITE\""
  exit 1
fi

CONTAINER_NAME="libsemigroups/libsemigroups-docker:version-2.0.0"

# Pull the docker container
docker pull "$CONTAINER_NAME" 

# Start the docker container detached
ID=$(docker run --rm -i -d "$CONTAINER_NAME")

# Copy the libsemigroups directory into the container
docker cp . "$ID:/home/libsemigroups_pybind11"

# Run the $GH_ACTIONS_SCRIPT in the running container
docker exec -i "$ID" "$GH_ACTIONS_SCRIPT" ; exit

# Attach to the container
docker attach "$ID"
