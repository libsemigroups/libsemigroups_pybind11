html:
	etc/make-doc-sphinx.sh

black: 
	black setup.py tests/*.py

check:
	pytest -vv tests/test_*.py

lint: 
	pylint setup.py tests/*.py
