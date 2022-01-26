#!/usr/bin/env bash

# If a command fails, exit this script with an error code
set -e
set -o pipefail

# Don't build wheels on gh actions

# TODO:
# 1) check we are in the libsemigroups_pybind11 dir
# 2) Get macosx_11_0_arm64 from the system
# 3) LIBSEMIGROUPS_PYBIND11_VERSION should be an argument or obtained from git

LIBSEMIGROUPS_PYBIND11_VERSION="0.1.6"
PYTHON_VERSIONS=( "3.8" "3.9" "3.10" )
MACOSX_VERSION="macosx_11_0_arm64"

mkdir -p wheelhouse

# TODO: fail if wheelhouse exists and is non-empty

for PYTHON_VERSION in "${PYTHON_VERSIONS[@]}"; do
  PY="/opt/homebrew/opt/python@$PYTHON_VERSION/bin/python$PYTHON_VERSION"
  WHEEL_PY_VERSION=$(echo "cp$PYTHON_VERSION" | sed 's/\.//')
  WHEEL_NAME="libsemigroups_pybind11-$LIBSEMIGROUPS_PYBIND11_VERSION-$WHEEL_PY_VERSION-$WHEEL_PY_VERSION-$MACOSX_VERSION.whl"
  #if ! [ -x "$PY" ]; then
  #  echo -e "This script requires $PY! Aborting!"
  #  exit 1
  #fi
  $PY -m pip wheel .
  delocate-wheel --require-archs arm64 -w wheelhouse "$WHEEL_NAME"
done

for WHEEL in wheelhouse/*; do 
  if [ -f "$WHEEL" ]; then 
    echo "twine upload --repository pypi $WHEEL" 
    twine upload --repository pypi
  fi 
done
