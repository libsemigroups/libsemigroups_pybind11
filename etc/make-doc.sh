#!/bin/bash
set -e

if [ -x "$(command -v python)" ]; then
  PYTHON="python"
else
  PYTHON="python3"
fi

$PYTHON - <<END
import jinja2
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
    with open(fnam, "w") as file:
        print("Generating %s . . . " % fnam)
        file.write(
            template.render(
                Type=T,
                Underline="=" * (len(TLC) + 1),
                LongNameNoCap=TL,
                LongNameCap=TLC,
                Function=F,
                Subset=S,
            )
        )
END

cd docs/
make html

echo "See: docs/_build/html/index.html"
