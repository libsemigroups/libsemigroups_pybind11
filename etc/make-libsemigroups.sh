#!/bin/bash

# If a command fails, exit this script with an error code
set -e
set -o pipefail

VERSION=$(python etc/libsemigroups_version.py)
curl -L -O https://github.com/libsemigroups/libsemigroups/releases/download/v$VERSION/libsemigroups-$VERSION.tar.gz
tar -xf libsemigroups-$VERSION.tar.gz
rm -f libsemigroups-$VERSION.tar.gz
cd libsemigroups-$VERSION
./configure --disable-hpcombi && make -j4
