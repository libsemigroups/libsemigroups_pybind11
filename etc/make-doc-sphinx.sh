#!/bin/bash
set -e

if [[ -x "$(command -v unbuffer)" ]]; then
  UNBUF="unbuffer"
  UNBUF_P="unbuffer -p"
else 
  UNBUF=
  UNBUF_P=
fi






cd docs/
mkdir -p source/_static
$UNBUF make html 2>&1 | $UNBUF_P grep -v --color=always "Citation .* is not referenced"
echo "See: docs/build/html/index.html"
