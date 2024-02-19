# -*- coding: utf-8 -*-

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module provides some tools for libsemigroups_pybind11.
"""
import os
import re

from packaging import version

try:
    import pkgconfig

    PKGCONFIG_IMPORTED = True
except ModuleNotFoundError:
    PKGCONFIG_IMPORTED = False


def minimum_libsemigroups_version():
    """
    Returns the minimum required version of libsemigroups required to make
    this work.
    """
    return "2.7.0"
    # Changed down from 2.7.1 -- do something sensible here


if PKGCONFIG_IMPORTED:

    def libsemigroups_version():
        "Get the version of libsemigroups installed using pkg-config."

        vers = pkgconfig.modversion("libsemigroups")
        if re.search(r"\d+\.\d+\.\d+-\d+-\w{7}", vers):
            # i.e. supplied is of the form: 1.1.0-6-g8b04c08
            vers = re.search(r"\d+\.\d\.+\d+-\d+", vers).group(0)
        return vers


def compare_version_numbers(supplied, required):
    "Returns True if supplied >= required"

    if isinstance(supplied, str) and isinstance(required, str):
        return version.parse(supplied) >= version.parse(required)
    raise TypeError(
        f"expected (string, string), got a ({supplied.__name__}, {required.__name__})"
    )


def extra_link_args() -> str:
    libs_only_L = pkgconfig.libs("libsemigroups")
    # The above pkgconfig query can return an empty string (this also happens on
    # the command line). This happens, for example, using pkg-config version 1.8.0
    # on ArchLinux. CN 27/10/2021

    assert (
        len(libs_only_L) == 0 or libs_only_L[:2] == "-L"
    ), "The first two characters of the library path to the libsemigroups.so etc should be '-L'"

    libs_only_L = [x for x in libs_only_L.split(" ") if x.startswith("-L")]

    if len(libs_only_L) == 0:
        libs_only_L = ["-L/usr/lib"]
    return libs_only_L


def ld_library_path() -> str:
    return ":".join([x[2:] for x in extra_link_args()])
