html:
	pip install .
	cd docs && make html

black: 
	black setup.py tests/*.py

check:
	tox -- -x tests/
