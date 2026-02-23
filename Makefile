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

format:
# ruff format can't yet sort imports, so we require the ruff check line too
# See https://github.com/astral-sh/ruff/issues/8232 for updates.
	ruff check --select I --fix-only
	ruff format
	find src -name "*.*pp" -type f -exec clang-format-15 -i --verbose {} +

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
