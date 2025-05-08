#!/bin/bash
set -e

find docs/source -type f *.rst -exec docstrfmt {} \;

