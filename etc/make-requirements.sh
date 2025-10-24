#!/bin/bash
set -e
CUSTOM_COMPILE_COMMAND="./etc/$(basename $0)" pip-compile --upgrade --all-build-deps --all-extras --strip-extras pyproject.toml 