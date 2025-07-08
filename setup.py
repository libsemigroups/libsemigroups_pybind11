# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell + Maria Tsalakou
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This package provides a python interface to the C++ library
libsemigroups (libsemigroups.rtfd.io).
"""

import glob
import os
import sys

import pkgconfig
from pybind11.setup_helpers import (
    ParallelCompile,
    Pybind11Extension,
    build_ext,
    naive_recompile,
)
from setuptools import setup

ParallelCompile("NPY_NUM_BUILD_JOBS", needs_recompile=naive_recompile).install()

sys.path.append(os.path.abspath(os.path.dirname(__file__)))

from build_tools.packaging_helpers import (  # pylint: disable=wrong-import-position
    validate_libsemigroups,
)

validate_libsemigroups()

libsemigroups_info = pkgconfig.parse("libsemigroups")
extra_libs = set(libsemigroups_info["libraries"])
extra_libs.remove("semigroups")
include_dirs = set(libsemigroups_info["include_dirs"])
for lib in extra_libs:
    include_dirs.update(set(pkgconfig.parse(lib)["include_dirs"]))
include_dirs = sorted(list(include_dirs))

print("Include directories are:")
print(include_dirs)

print("Library directories args are:")
print(libsemigroups_info["library_dirs"])

ext_modules = [
    Pybind11Extension(
        "_libsemigroups_pybind11",
        glob.glob("src/*.cpp"),
        include_dirs=include_dirs,
        library_dirs=libsemigroups_info["library_dirs"],
        language="c++",
        libraries=["semigroups"],
    ),
]

setup(
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
)
