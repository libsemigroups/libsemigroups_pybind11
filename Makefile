# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

all: install doc

doc:
	cd docs && make html 

install: 
	pip install . --use-feature=in-tree-build

black: 
	black setup.py tests/*.py

check:
	pytest -vv tests/test_*.py

lint: 
	pylint setup.py tests/*.py
	cpplint src/*.hpp src/*.cpp
