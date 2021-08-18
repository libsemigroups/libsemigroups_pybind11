#!/bin/bash
set -e

# This entire script should be executed in the libsemigroups container

export PATH=/home/libsemigroups/.local/bin:$PATH

sudo apt-get --yes update
sudo apt-get install python3 --yes 
sudo apt-get install python3-pip --yes 

sudo chown 1000:1000 -R /home/libsemigroups_pybind11/

cd /home/libsemigroups_pybind11

sudo -H pip3 install -r requirements.txt
sudo -H pip3 install .
make check
