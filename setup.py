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
from pprint import pprint

import pkgconfig
from pybind11.setup_helpers import (
    ParallelCompile,
    Pybind11Extension,
    build_ext,
    naive_recompile,
)
from setuptools import find_packages, setup

ParallelCompile("NPY_NUM_BUILD_JOBS", needs_recompile=naive_recompile).install()

__dir__ = os.path.abspath(os.path.dirname(__file__))
sys.path.insert(0, __dir__ + "/libsemigroups_pybind11")

from tools import (  # pylint: disable=import-error, wrong-import-position
    compare_version_numbers,
    extra_link_args,
    libsemigroups_version,
    minimum_libsemigroups_version,
)

__version__ = "1.0.0"

DISCLAIMER = """(You should not see this message unless you are installing
libsemigroups_pybind11 from its sources. If you are not installing from the
sources, please raise an issue at:
https://github.com/libsemigroups/libsemigroups_pybind11)"""

if not pkgconfig.exists("libsemigroups"):
    raise ImportError(
        f"""cannot locate the libsemigroups library.
For more information about installing the libsemigroups library see
https://libsemigroups.github.io/libsemigroups_pybind11/install.html.

If libsemigroups is installed, then it cannot be located by pkg-config, perhaps
you should add the directory containing `libsemigroups.pc' to the
\"PKG_CONFIG_PATH\". The file `libsemigroups.pc' might be in one of the
following directories:
* /usr/local/lib/pkgconfig
* $CONDA_PREFIX/lib/pkgconfig if your active conda environment has pkgconfig
  installed, and libsemigroups was installed with conda/mamba in this
  environment.
{DISCLAIMER}"""
    )

if not compare_version_numbers(
    libsemigroups_version(), minimum_libsemigroups_version()
):
    raise ImportError(
        f"libsemigroups version at least {minimum_libsemigroups_version()}"
        + f" is required, found {libsemigroups_version()}"
    )


class GetPyBind11Include:  # pylint: disable=too-few-public-methods
    """
    Helper class to determine the pybind11 include path

    The purpose of this class is to postpone importing pybind11
    until it is actually installed, so that the ``get_include()``
    method can be invoked.
    """

    def __init__(self, user=False):
        self.user = user

    def __str__(self):
        import pybind11  # pylint: disable=import-outside-toplevel

        return pybind11.get_include(self.user)


include_path = [
    GetPyBind11Include(),
    GetPyBind11Include(user=True),
]

for lib in ("libsemigroups", "eigen3", "fmt"):
    try:
        cflags_only_I = pkgconfig.cflags(lib)  # pylint: disable=invalid-name
    except pkgconfig.pkgconfig.PackageNotFoundError:
        continue

    cflags_only_I = [
        x[2:] for x in cflags_only_I.split(" ") if x.startswith("-I")
    ]

    include_path.extend(cflags_only_I)
    if lib == "libsemigroups":
        include_path.extend(
            [os.path.join(x, "libsemigroups") for x in cflags_only_I]
        )

print("Include directories are:")
pprint(include_path)

print("Extra link args are:")
pprint(extra_link_args())

ext_modules = [
    Pybind11Extension(
        "_libsemigroups_pybind11",
        glob.glob("src/*.cpp"),
        include_dirs=include_path,
        language="c++",
        libraries=["semigroups"],
        extra_link_args=extra_link_args(),
    ),
]

with open(os.path.join(__dir__, "README.md"), "r", encoding="utf-8") as readme:
    long_description = readme.read()

setup(
    name="libsemigroups_pybind11",
    version=__version__,
    author="Joseph Edwards, James Mitchell, Maria Tsalakou, Murray Whyte",
    author_email="jdm3@st-andrews.ac.uk",
    url="https://github.com/libsemigroups/libsemigroups_pybind11",
    description="A python package for the libsemigroups C++ library",
    long_description=long_description,
    long_description_content_type="text/markdown",
    ext_modules=ext_modules,
    packages=find_packages(),
    setup_requires=["pkgconfig>=1.5.5"],
    install_requires=[
        "packaging>=20.4",
        "pkgconfig>=1.5.5",
        "pybind11>=2.10.1",
    ],
    tests_require=["pytest==8.0.0"],
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.7.0",
)
