# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

all: install doc

doc:
	etc/make-doc.sh

install:
	pip3 install . --use-feature=in-tree-build

black:
	black setup.py tests/*.py libsemigroups_pybind11/*.py

check:
	pytest -vv tests/test_*.py

lint:
	pylint setup.py tests/*.py libsemigroups_pybind11/*.py
	cpplint src/*.hpp src/*.cpp

coverage:
	@coverage run --source . --omit="tests/*" -m py.test
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
