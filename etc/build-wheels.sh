#!/usr/bin/env bash

# If a command fails, exit this script with an error code
set -e
set -o pipefail

# This script requires that python 3.8, 3.9, and 3.10 are installed by homebrew. 
# If you've updated your OS recently, then it's also necessary to update the
# installations of python so that they are built against the same version of
# the OS. Building the wheels will fail if this doesn't happen. 

PYTHON_VERSIONS=( "3.8" "3.9" "3.10" )

if ! [[ "$PWD" == *libsemigroups_pybind11 ]] ; then
  echo -e "This script must be run in the libsemigroups_pybind11 directory! Aborting!"
  exit 1
elif [[ -n $(git status -s) ]] ; then
  echo -e "There are untracked files, or uncommitted changes! Aborting!"
  exit 1
fi

make superclean
mkdir -p wheelhouse

for PYTHON_VERSION in "${PYTHON_VERSIONS[@]}"; do
  PY="/opt/homebrew/opt/python@$PYTHON_VERSION/bin/python$PYTHON_VERSION"
  LIBSEMIGROUPS_PYBIND11_VERSION="$($PY etc/version.py)"
  MACOSX_VERSION="$($PY etc/mac_os_version.py)"
  WHEEL_PY_VERSION=$(echo "cp$PYTHON_VERSION" | sed 's/\.//')
  WHEEL_NAME="libsemigroups_pybind11-$LIBSEMIGROUPS_PYBIND11_VERSION-$WHEEL_PY_VERSION-$WHEEL_PY_VERSION-$MACOSX_VERSION.whl"
  echo -e "\033[1mBuilding wheel $WHEEL_NAME ...\033[0m"
  "$PY" -m pip wheel .
  delocate-wheel --require-archs arm64 -w wheelhouse -v "$WHEEL_NAME"
  echo -e "\033[1mSuccess for $WHEEL_NAME !!!\033[0m"
done

for WHEEL in wheelhouse/*; do
  echo -e "twine upload --repository pypi wheelhouse/$WHEEL"
  twine upload --repository pypi wheelhouse/$WHEEL
done
