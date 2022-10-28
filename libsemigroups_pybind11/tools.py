# -*- coding: utf-8 -*-

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module provides some tools for libsemigroups_pybind11.
"""

import re

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
    return "2.3.2"


if PKGCONFIG_IMPORTED:

    def libsemigroups_version():
        "Get the version of libsemigroups installed using pkg-config."

        # the try-except is require pkgconfig v1.5.0 which is very recent, and
        # hence not on conda at time of writing.
        try:
            vers = pkgconfig.modversion("libsemigroups")
        except AttributeError:
            # this is just the guts of the modversion method in pkgconfig v1.5.1
            vers = (
                pkgconfig.pkgconfig._query(  # pylint: disable=protected-access
                    "libsemigroups", "--modversion"
                )
            )
        if re.search(r"\d+\.\d+\.\d+-\d+-\w{7}", vers):
            # i.e. supplied is of the form: 1.1.0-6-g8b04c08
            vers = re.search(r"\d+\.\d\.+\d+-\d+", vers).group(0)
        return vers
