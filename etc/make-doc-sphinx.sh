#!/bin/bash
set -e

UNBUF=
UNBUF_P=

cd docs/
mkdir -p source/_static
$UNBUF make html 2>&1 | $UNBUF_P grep -v --color=always "Citation .* is not referenced"
echo "See: docs/build/html/index.html"
