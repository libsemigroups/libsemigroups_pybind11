# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

all: install doc

doc:
	etc/make-doc.sh

fresh-doc: clean-doc
	etc/make-doc.sh --check-unused

doctest:
	cd docs && make doctest

install:
	pip3 install . --use-feature=in-tree-build

black:
	black setup.py tests/*.py src/libsemigroups_pybind11/*.py docs/source/conf.py

check: doctest
	pytest -vv tests/test_*.py

lint:
	etc/make-lint.sh

coverage:
	@coverage run --source libsemigroups_pybind11 --omit="tests/*" -m pytest tests/test_*.py
	@coverage html
	@echo "See: htmlcov/index.html"

clean-doc:
	rm -rf docs/_build

clean: clean-doc
	find . -type d -name __pycache__ -prune -exec rm -rf {} \;
	rm -rf libsemigroups_pybind11.egg-info
	rm -f *.whl
	rm -rf build/

superclean: clean
	git clean -xdf --exclude *.swp --exclude *.swo
