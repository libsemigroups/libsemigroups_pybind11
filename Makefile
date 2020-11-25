html:
	etc/make-doc-sphinx.sh

black: 
	black setup.py tests/*.py

check:
	tox -- -x tests/
