#!/bin/bash
set -e

if [ -x "$(command -v python)" ]; then
  PYTHON="python"
else
  PYTHON="python3"
fi

$PYTHON etc/check-docstring-indentation.py

cd docs/
make html

cd ..
$PYTHON etc/replace-strings-in-doc.py $1
$PYTHON etc/check-params.py $1

echo "See: docs/_build/html/index.html"
