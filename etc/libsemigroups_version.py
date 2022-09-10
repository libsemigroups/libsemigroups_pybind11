#!/usr/bin/env python3
import os
import sys

__dir__ = os.path.abspath(os.path.dirname(__file__))
sys.path.insert(0, __dir__ + "/../libsemigroups_pybind11")

from tools import minimum_libsemigroups_version

print(minimum_libsemigroups_version())
