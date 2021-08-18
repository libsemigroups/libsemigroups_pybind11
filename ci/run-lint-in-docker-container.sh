#!/bin/bash
set -e

# This entire script should be executed in the libsemigroups container

cd /home/libsemigroups_pybind11
ci/install-in-docker-container.sh
sudo -H pip3 install pylint
make lint
