# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module provides some tools for libsemigroups_pybind11.
"""
import re

from packaging import version

try:
    import pkgconfig

    PKGCONFIG_IMPORTED = True
except ModuleNotFoundError:
    PKGCONFIG_IMPORTED = False


def ordinal(n: int):
    """
    Returns 1st from 1, 2nd from 2 etc.
    From https://stackoverflow.com/questions/9647202/
    """
    if 11 <= (n % 100) <= 13:
        suffix = "th"
    else:
        suffix = ["th", "st", "nd", "rd", "th"][min(n % 10, 4)]
    return str(n) + suffix


def copydoc(original):
    """
    Decorator that can be used to copy the doc from one function to another,
    for example:

    @copydoc(StaticTransf16.__init__)
    def __init___(self):
       pass
    """

    def wrapper(target):
        target.__doc__ = original.__doc__
        return target

    return wrapper


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
    raise TypeError(
        f"expected (string, string), got a ({supplied.__name__}, {required.__name__})"
    )


def extra_link_args() -> str:
    """Find extra link args"""
    libs_only_L = pkgconfig.libs(  # pylint: disable=invalid-name
        "libsemigroups"
    )
    # The above pkgconfig query can return an empty string (this also happens on
    # the command line). This happens, for example, using pkg-config version 1.8.0
    # on ArchLinux. CN 27/10/2021

    assert (
        len(libs_only_L) == 0 or libs_only_L[:2] == "-L"
    ), "The first two characters of the library path to the libsemigroups.so etc should be '-L'"

    libs_only_L = [  # pylint: disable=invalid-name
        x for x in libs_only_L.split(" ") if x.startswith("-L")
    ]

    if len(libs_only_L) == 0:
        libs_only_L = ["-L/usr/lib"]  # pylint: disable=invalid-name
    return libs_only_L


def ld_library_path() -> str:
    """Construct the LD_LIBRARY_PATH"""
    return ":".join([x[2:] for x in extra_link_args()])
