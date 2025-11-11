#!/usr/bin/env python3
import os
import sys

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))
from build_tools import minimum_libsemigroups_version

print(minimum_libsemigroups_version())
