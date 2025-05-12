#!/bin/bash

if [[ $# -ne 0 ]]; then
    bold "error expected 0 arguments, got $#!"
    exit 1
fi

exit_code=0

echo "Linting with ruff . . ."
ruff check setup.py tests/*.py src/libsemigroups_pybind11/*.py docs/source/*.py || ((exit_code = 1))

echo "Linting with pylint . . ."
pylint setup.py tests/*.py src/libsemigroups_pybind11/*.py src/libsemigroups_pybind11/**/*.py || ((exit_code = 1))

echo "Linting with cpplint . . ."
cpplint src/*.hpp src/*.cpp || ((exit_code = 1))

exit $exit_code
