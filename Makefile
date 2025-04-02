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
	black setup.py tests/*.py libsemigroups_pybind11/*.py docs/source/conf.py

check: doctest
	pytest -vv tests/test_*.py

lint:
	ruff check --exit-zero setup.py tests/*.py libsemigroups_pybind11/*.py docs/source/conf.py
	pylint --exit-zero setup.py tests/*.py libsemigroups_pybind11/*.py docs/source/conf.py
	cpplint src/*.hpp src/*.cpp

coverage:
	@coverage run --source . --omit="tests/*" -m pytest tests/test_*.py
	@coverage html
	@echo "See: htmlcov/index.html"


clean-doc:
	rm -rf docs/_build

clean: clean-doc
	rm -rf __pycache__ libsemigroups_pybind11.egg-info
	rm -rf tests/__pycache__ libsemigroups_pybind11/__pycache__
	rm -f *.whl
	rm -rf build/

superclean: clean
	git clean -xdf --exclude *.swp --exclude *.swo
