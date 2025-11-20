#!/bin/bash
set -e

if [ -x "$(command -v python)" ]; then
  PYTHON="python"
else
  PYTHON="python3"
fi

echo "Checking whether to build the to-table . . ."
if command -v pdflatex && command -v inkscape 2>&1 >/dev/null; then
  cd docs/pictures
  echo "Building to-table . . ."
  pdflatex to-table.tex
  inkscape --pdf-poppler --export-type="svg" -o to-table.svg to-table.pdf
  if [ $? -eq 0 ]; then
    echo Successfully created to-table.svg
  else
    echo Warning: there was an issue with inkscape when creating to-table.svg
    echo Please check the state of to-table.svg
  fi
  cd ../..
else
  echo "Not building to-table"
fi

$PYTHON etc/check-docstring-indentation.py

cd docs/
make html

cd ..
$PYTHON etc/replace-strings-in-doc.py $1
$PYTHON etc/check-params.py $1

echo "See: docs/_build/html/index.html"
