# -*- coding: utf-8 -*-

# Copyright (c) 2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, missing-function-docstring, protected-access

"""
This package provides the user-facing python part of libsemigroups_pybind11 for
the libsemigroups::paths namespace from libsemigroups.
"""

from _libsemigroups_pybind11 import Paths, ReversiblePaths, POSITIVE_INFINITY


def count(p: Paths | ReversiblePaths) -> int | type(POSITIVE_INFINITY):
    result = p._count()
    if result == POSITIVE_INFINITY:
        return POSITIVE_INFINITY
    return result


Paths.count = count
doc = Paths._count.__doc__
doc = doc[doc.find("-> int") + 6 :]
Paths.count.__doc__ = doc
ReversiblePaths.count = count
doc = ReversiblePaths._count.__doc__
doc = doc[doc.find("-> int") + 6 :]
ReversiblePaths.count.__doc__ = doc
