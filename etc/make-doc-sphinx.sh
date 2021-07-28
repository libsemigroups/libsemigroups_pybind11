#!/bin/bash
set -e

if [[ -x "$(command -v unbuffer)" ]]; then
  UNBUF="unbuffer"
  UNBUF_P="unbuffer -p"
else 
  UNBUF=
  UNBUF_P=
fi

on_rtd = os.environ.get("READTHEDOCS", None) == "True"
if on_rtd:
	subprocess.call("cd .. && mkdir -p build && cd .. && etc/make-doc-sphinx.sh",shell=True,)


cd docs/
view:
  firefox _build/html/index.html
mkdir -p source/_static
$UNBUF make html 2>&1 | $UNBUF_P grep -v --color=always "Citation .* is not referenced"
echo "See: docs/build/html/index.html"
