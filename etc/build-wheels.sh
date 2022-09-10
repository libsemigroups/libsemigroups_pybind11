#!/usr/bin/env bash

# If a command fails, exit this script with an error code
set -e
set -o pipefail

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

LIBSEMIGROUPS_PYBIND11_VERSION="$($PYTHON etc/version.py)"
MACOSX_VERSION="$($PYTHON etc/mac_os_version.py)"
echo -e "Building wheels for libsemigroups_pybind11 v$LIBSEMIGROUPS_PYBIND11_VERSION ($MACOSX_VERSION)"
exit 1

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
