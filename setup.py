# Copyright (c) 2021-2024 J. D. Mitchell + Maria Tsalakou
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.
# pylint: disable=import-error

"""This package provides a python interface to the C++ library libsemigroups
(libsemigroups.rtfd.io).
"""

import glob
import os
import platform
import sys

import pkgconfig
from pybind11.setup_helpers import (
    ParallelCompile,
    Pybind11Extension,
    build_ext,
    has_flag,
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


def get_arch():
    """Simple function to return the architecture, namely, x86 or not"""
    arch = platform.machine().lower()
    if arch in {"x86_64", "amd64", "i386", "i686"}:
        return "x86"
    if arch in {"arm64", "aarch64", "armv7l", "armv6l"}:
        return "arm"
    return arch


class LibsemigroupsBuildExt(build_ext):
    # pylint: disable=too-few-public-methods
    """Class conditionally add compile flags"""

    def build_extensions(self):
        """Adds compile flags before calling build_extensions in build_ext"""
        compiler = self.compiler

        if has_flag(compiler, "-mavx"):
            print("Compiler supports '-mavx' flag, adding it to 'extra_compile_args'")
            for ext in self.extensions:
                ext.extra_compile_args += ["-mavx"]
        else:
            print("Compiler does not support '-mavx' flag, not adding it to 'extra_compile_args'")
        if get_arch() == "arm" and (
            any(x.startswith("gcc") for x in compiler.compiler)
            or any(x.startswith("g++") for x in compiler.compiler_cxx)
        ):
            print(
                "Compiler is gcc, and architecture is arm, adding '-fpermissive' to "
                "'extra_compile_args'"
            )
            for ext in self.extensions:
                ext.extra_compile_args += ["-fpermissive"]

        for ext in self.extensions:
            print(f"'extra_compile_args' for '{ext.name}' are:")
            print(ext.extra_compile_args)

        super().build_extensions()


ext_modules = [
    Pybind11Extension(
        "_libsemigroups_pybind11",
        glob.glob("src/*.cpp"),
        include_dirs=include_dirs,
        library_dirs=libsemigroups_info["library_dirs"],
        language="c++",
        libraries=["semigroups"],
        extra_compile_args=["-flax-vector-conversions"],
    )
]

setup(ext_modules=ext_modules, cmdclass={"build_ext": LibsemigroupsBuildExt})
