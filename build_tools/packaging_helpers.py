# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This submodule provides some more tools for building libsemigroups_pybind11.
These tools rely on some non-built-in packages, so they have been placed in
their own submodule to separate from the functions that need to be run in a bare
Python environment.
"""

import re
from packaging import version
import pkgconfig
from . import minimum_libsemigroups_version


def libsemigroups_version():
    "Get the version of libsemigroups installed using pkg-config."

    vers = pkgconfig.modversion("libsemigroups")
    if re.search(r"\d+\.\d+\.\d+-\d+-\w{7}", vers):
        # i.e. supplied is of the form: 1.1.0-6-g8b04c08
        vers = re.search(r"\d+\.\d\.+\d+-\d+", vers).group(0)
    return vers


def compare_version_numbers(supplied, required):
    """Returns True if supplied >= required"""

    if isinstance(supplied, str) and isinstance(required, str):
        if "dev" in supplied:
            print(
                (
                    "\033[93mWarning: You are using a development version of libsemigroups. This "
                    "may cause undocumented behaviour\033[0m"
                )
            )
            return True
        return version.parse(supplied) >= version.parse(required)
    raise TypeError(f"expected (string, string), got a ({supplied.__name__}, {required.__name__})")


def extra_link_args() -> str:
    """Find extra link args"""
    libs_only_L = pkgconfig.libs(  # pylint: disable=invalid-name
        "libsemigroups"
    )
    # The above pkgconfig query can return an empty string (this also happens on
    # the command line). This happens, for example, using pkg-config version 1.8.0
    # on ArchLinux. CN 27/10/2021

    assert len(libs_only_L) == 0 or libs_only_L[:2] == "-L", (
        "The first two characters of the library path to the libsemigroups.so etc should be '-L'"
    )

    libs_only_L = [  # pylint: disable=invalid-name
        x for x in libs_only_L.split(" ") if x.startswith("-L")
    ]

    if len(libs_only_L) == 0:
        libs_only_L = ["-L/usr/lib"]  # pylint: disable=invalid-name
    return libs_only_L


def ld_library_path() -> str:
    """Construct the LD_LIBRARY_PATH"""
    return ":".join([x[2:] for x in extra_link_args()])


def validate_libsemigroups():
    DISCLAIMER = """
    (You should not see this message unless you are installing
    libsemigroups_pybind11 from its sources. If you are not installing from the
    sources, please raise an issue at:
    https://github.com/libsemigroups/libsemigroups_pybind11)"""

    if not pkgconfig.exists("libsemigroups"):
        raise ImportError(
            f"""cannot locate the libsemigroups library.
    For more information about installing the libsemigroups library see
    https://libsemigroups.github.io/libsemigroups_pybind11/install.html.

    If libsemigroups is installed, then it cannot be located by pkg-config,
    perhaps you should add the directory containing `libsemigroups.pc' to the
    \"PKG_CONFIG_PATH\". The file `libsemigroups.pc' might be in one of the
    following directories:
    * /usr/local/lib/pkgconfig
    * $CONDA_PREFIX/lib/pkgconfig if your active conda environment has pkgconfig
    installed, and libsemigroups was installed with conda/mamba in this
    environment.
    {DISCLAIMER}"""
        )

    if not compare_version_numbers(libsemigroups_version(), minimum_libsemigroups_version()):
        raise ImportError(
            f"libsemigroups version at least {minimum_libsemigroups_version()}"
            + f" is required, found {libsemigroups_version()}"
        )
