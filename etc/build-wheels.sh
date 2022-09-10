#!/usr/bin/env bash

# If a command fails, exit this script with an error code
set -e
set -o pipefail

# TODO:
# * Get macosx_11_0_arm64 from the system
# * LIBSEMIGROUPS_PYBIND11_VERSION should be an argument or obtained from git

LIBSEMIGROUPS_PYBIND11_VERSION="0.2.0"
MACOSX_VERSION="macosx_11_0_arm64"

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
  WHEEL_PY_VERSION=$(echo "cp$PYTHON_VERSION" | sed 's/\.//')
  WHEEL_NAME="libsemigroups_pybind11-$LIBSEMIGROUPS_PYBIND11_VERSION-$WHEEL_PY_VERSION-$WHEEL_PY_VERSION-$MACOSX_VERSION.whl"
  "$PY" -m pip wheel .
  delocate-wheel --require-archs arm64 -w wheelhouse -v "$WHEEL_NAME"
done

for WHEEL in wheelhouse/*; do
  if [ -f "wheelhouse/$WHEEL" ]; then
    echo "twine upload --repository pypi wheelhouse/$WHEEL"
    twine upload --repository pypi wheelhouse/$WHEEL
  fi
done
