#!/bin/bash

if [[ $# -ne 0 ]]; then
    bold "error expected 0 arguments, got $#!"
    exit 1
fi

exit_code=0
python_files="setup.py tests/*.py src/libsemigroups_pybind11/*.py src/libsemigroups_pybind11/**/*.py docs/source/*.py docs/source/**/*.py"

echo "Linting with ruff . . ."
ruff check $python_files || ((exit_code = 1))

echo "Linting with pylint . . ."
pylint $python_files || ((exit_code = 1))

echo "Linting with cpplint . . ."
cpplint src/*.hpp src/*.cpp || ((exit_code = 1))

exit $exit_code
