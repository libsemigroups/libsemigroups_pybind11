#!/bin/bash
set -e

if [ -x "$(command -v python)" ]; then
  PYTHON="python"
else
  PYTHON="python3"
fi

$PYTHON etc/check-docstring-indentation.py

$PYTHON - <<END
import re
from jinja2 import Environment, FileSystemLoader, select_autoescape

env = Environment(
    loader=FileSystemLoader("docs/templates/api"),
    autoescape=select_autoescape(),
)
# template = env.get_template("matrix.rst")
# for T in [
#     "BMat",
#     "IntMat",
#     "MaxPlusMat",
#     "MinPlusMat",
#     "ProjMaxPlusMat",
#     "MaxPlusTruncMat",
#     "MinPlusTruncMat",
#     "NTPMat",
# ]:
#     fnam = "docs/source/api/%s.rst" % T
#     with open(fnam, "w") as file:
#         print("Generating %s . . . " % fnam)
#         file.write(template.render(Mat=T, underline="=" * len(T)))

template = env.get_template("transf.rst")
for T, TL, TLC, F, S in [
    ("Transf", "transformation", "Transformation", "", "the whole"),
    ("PPerm", "partial perm", "Partial perm", "n injective", "any subset"),
    ("Perm", "permutation", "Permutation", " bijective", "the whole"),
]:
    fnam = "docs/source/api/%s.rst" % T
    trailing_ws = re.compile(r"(\s+\n|\s+$)")
    with open(fnam, "w") as file:
        print("Generating %s . . . " % fnam)
        content = template.render(
                     Type=T,
                     Underline="=" * (len(TLC) + 1),
                     LongNameNoCap=TL,
                     LongNameCap=TLC,
                     Function=F,
                     Subset=S,
                  )
        content = re.sub(trailing_ws, r"\n\n", content)
        file.write(content)
END

cd docs/
make html

cd ..
$PYTHON etc/replace-strings-in-doc.py

echo "See: docs/_build/html/index.html"
