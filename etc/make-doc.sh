#!/bin/bash
set -e

python3 - <<END
import jinja2
from jinja2 import Environment, FileSystemLoader, select_autoescape
env = Environment(
          loader=FileSystemLoader("docs/templates/api"),
          autoescape=select_autoescape()
      )
template = env.get_template("mat-compile.rst")
for T in ["BMat", "IntMat", "MaxPlusMat", "MinPlusMat", "ProjMaxPlusMat"]:
   fnam = "docs/source/api/%s.rst" % T
   with open(fnam, "w") as file:
     print("Generating %s . . . " % fnam)
     file.write(template.render(Mat=T, underline="=" * len(T)))
END

cd docs/
make html 

echo "See: docs/_build/html/index.html"
