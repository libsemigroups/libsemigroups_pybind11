#!/bin/bash

if [[ $# -ne 0 ]]; then
    bold "error expected 0 arguments, got $#!"
    exit 1
fi

exit_code=0

echo "Linting with ruff . . ."
ruff check || ((exit_code = 1))

echo "Linting with pylint . . ."
pylint $(ruff check --show-files | grep "\.py$") || ((exit_code = 1))

echo "Linting with cpplint . . ."
cpplint src/*.hpp src/*.cpp || ((exit_code = 1))

exit $exit_code
