#!/bin/bash
set -e

printf "\033[0;32mDeploying updates to GitHub...\033[0m\n"

make clean-doc # to remove old doc files
make doc
cp -r docs/_build/html/* gh-pages
cd gh-pages
git add .
msg="rebuilding site $(date)"
if [ -n "$*" ]; then
	msg="$*"
fi
git commit -m "$msg"
git push origin gh-pages
