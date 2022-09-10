#!/bin/bash
set -e

PREFIX="/home/libsemigroups_pybind11"
echo -e "Using PREFIX = '$PREFIX'"

if [ "$GH_ACTIONS_SUITE" == "test" ]; then
  GH_ACTIONS_SCRIPT="$PREFIX/ci/run-tests-in-docker-container.sh"
elif [ "$GH_ACTIONS_SUITE" == "lint" ]; then
  GH_ACTIONS_SCRIPT="$PREFIX/ci/run-lint-in-docker-container.sh"
else
  echo -e "Error, unrecognised suite, expect one of \"test\", or \"lint\", got \"$GH_ACTIONS_SUITE\""
  exit 1
fi

echo -e "Using GH_ACTIONS_SUITE = '$GH_ACTIONS_SUITE'"

if [ "x$PYTHON" != x ] ; then
    :
elif command -v python >/dev/null 2>&1; then
    PYTHON=python
elif command -v python3 >/dev/null 2>&1; then
    PYTHON=python3
else
    echo -e "Python not found, please install it and/or set the PYTHON environment variable to the path of a python executable"
    exit 1
fi

echo -e "Using PYTHON = '$PYTHON'"

LIBSEMIGROUPS_VERSION=$($PYTHON etc/libsemigroups_version.py)
echo -e "Using LIBSEMIGROUPS_VERSION = '$LIBSEMIGROUPS_VERSION'"

CONTAINER_NAME="libsemigroups/libsemigroups-docker:version-$LIBSEMIGROUPS_VERSION"
echo -e "Using CONTAINER_NAME = '$CONTAINER_NAME'"

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
