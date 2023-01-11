# -*- coding: utf-8 -*-

# Copyright (c) 2021, J. D. Mitchell + Maria Tsalakou
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This package provides a python interface to the C++ library
libsemigroups (libsemigroups.rtfd.io).
"""

import glob
import json
import os
import sys
import pkgconfig

from packaging import version
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup, find_packages

__dir__ = os.path.abspath(os.path.dirname(__file__))
sys.path.insert(0, __dir__ + "/libsemigroups_pybind11")

from tools import (  # pylint: disable=import-error, wrong-import-position
    libsemigroups_version,
    minimum_libsemigroups_version,
)

__version__ = "0.7.1"


def compare_version_numbers(supplied, required):
    "Returns True if supplied >= required"

    if isinstance(supplied, str) and isinstance(required, str):
        return version.parse(supplied) >= version.parse(required)
    raise TypeError(
        "expected a (string, string), got a ("
        + supplied.__name__
        + ", "
        + required.__name__
        + ")"
    )


if "PKG_CONFIG_PATH" not in os.environ:
    os.environ["PKG_CONFIG_PATH"] = ""

pkg_config_path = os.environ["PKG_CONFIG_PATH"].split(":")

if "CONDA_PREFIX" in os.environ:
    conda_env_pkg_config = os.path.join(
        os.environ["CONDA_PREFIX"], "lib", "pkgconfig"
    )
    if (
        os.path.exists(conda_env_pkg_config)
        and not conda_env_pkg_config in pkg_config_path
    ):
        os.environ["PKG_CONFIG_PATH"] += ":" + conda_env_pkg_config

if "CONDA_DEFAULT_ENV" in os.environ and "CONDA_ENVS_PATH" in os.environ:
    conda_env_pkg_config = os.path.join(
        os.environ["CONDA_ENVS_PATH"],
        os.environ["CONDA_DEFAULT_ENV"],
        "lib",
        "pkgconfig",
    )
    if (
        os.path.exists(conda_env_pkg_config)
        and not conda_env_pkg_config in pkg_config_path
    ):
        os.environ["PKG_CONFIG_PATH"] += ":" + conda_env_pkg_config

if "/usr/local/lib/pkgconfig" not in pkg_config_path:
    if (
        "PKG_CONFIG_PATH" in os.environ
        and not len(os.environ["PKG_CONFIG_PATH"]) == 0
    ):
        os.environ["PKG_CONFIG_PATH"] += ":/usr/local/lib/pkgconfig"
    else:
        os.environ["PKG_CONFIG_PATH"] = "/usr/local/lib/pkgconfig"

if not pkgconfig.exists("libsemigroups"):
    print(json.dumps(dict(os.environ), sort_keys=True, indent=4))
    raise ImportError("cannot locate libsemigroups library")

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


LIBRARY_PATH = pkgconfig.pkgconfig._query(  # pylint: disable=protected-access
    "libsemigroups", "--libs-only-L"
)

# The above pkgconfig query can return an empty string (this also happens on
# the command line). This happens, for example, using pkg-config version 1.8.0
# on ArchLinux. CN 27/10/2021

assert (
    len(LIBRARY_PATH) == 0 or LIBRARY_PATH[:2] == "-L"
), "The first two characters of the library path to the libsemigroups.so etc should be '-L'"

# Try to use pkg-config to add the path to libsemigroups.so etc to
# LD_LIBRARY_PATH so that python can find it.

if len(LIBRARY_PATH) != 0:
    LIBRARY_PATH_NO_L = LIBRARY_PATH[2:]
else:
    LIBRARY_PATH_NO_L = "/usr/lib"
    LIBRARY_PATH = "-L/usr/lib"

if os.path.exists(LIBRARY_PATH_NO_L):
    if (
        "LD_LIBRARY_PATH" in os.environ
        and len(os.environ["LD_LIBRARY_PATH"]) != 0
    ):
        LD_LIBRARY_PATH = os.environ["LD_LIBRARY_PATH"]
        if LD_LIBRARY_PATH.find(LIBRARY_PATH_NO_L) == -1:
            PREFIX = "" if LD_LIBRARY_PATH[-1] == ":" else ":"
            os.environ["LD_LIBRARY_PATH"] += PREFIX + LIBRARY_PATH_NO_L
    else:
        os.environ["LD_LIBRARY_PATH"] = LIBRARY_PATH_NO_L
    print(os.environ["LD_LIBRARY_PATH"])


include_path = [
    GetPyBind11Include(),
    GetPyBind11Include(user=True),
    "/usr/local/include",
    "/usr/local/include/libsemigroups",
]

if "CONDA_PREFIX" in os.environ:
    include_path.append(os.path.join(os.environ["CONDA_PREFIX"], "include"))
    include_path.append(
        os.path.join(os.environ["CONDA_PREFIX"], "include", "eigen3")
    )

if "CONDA_DEFAULT_ENV" in os.environ and "CONDA_ENVS_PATH" in os.environ:
    include_path.append(
        os.path.join(
            os.environ["CONDA_ENVS_PATH"],
            os.environ["CONDA_DEFAULT_ENV"],
            "include",
        )
    )
    include_path.append(
        os.path.join(
            os.environ["CONDA_ENVS_PATH"],
            os.environ["CONDA_DEFAULT_ENV"],
            "include",
            "eigen3",
        )
    )

ext_modules = [
    Pybind11Extension(
        "_libsemigroups_pybind11",
        glob.glob("src/*.cpp"),
        include_dirs=include_path,
        language="c++",
        libraries=["semigroups"],
        extra_link_args=[LIBRARY_PATH, "-L/usr/local/lib"],
    ),
]

with open(os.path.join(__dir__, "README.md"), "r", encoding="utf-8") as readme:
    long_description = readme.read()

setup(
    name="libsemigroups_pybind11",
    version=__version__,
    author="James D. Mitchell",
    author_email="jdm3@st-andrews.ac.uk",
    url="https://github.com/libsemigroups/libsemigroups_pybind11",
    description="A python package for the libsemigroups C++ library",
    long_description=long_description,
    long_description_content_type="text/markdown",
    ext_modules=ext_modules,
    packages=find_packages(),
    setup_requires=["pkgconfig>=1.5.0"],
    install_requires=[
        "pybind11>=2.10.1",
        "packaging>=20.4",
        "pkgconfig>=1.5.0",
    ],
    tests_require=["pytest==6.2.4"],
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
)
