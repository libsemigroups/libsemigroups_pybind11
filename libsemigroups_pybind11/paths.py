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

from typing import Union

from _libsemigroups_pybind11 import (
    Paths,
    ReversiblePaths,
    POSITIVE_INFINITY,
    PositiveInfinity,
)


def count(p: Union[Paths, ReversiblePaths]) -> Union[int, PositiveInfinity]:
    """
    Get the size of the range. This function returns the number of paths
    remaining in the range (in particular, if :any:`next` is called then the
    return value of :any:`count` decreases by ``1``).

    :returns:
       The number of paths remaining in the range.
    :rtype:
       int | PositiveInfinity

    :raises LibsemigroupsError: if ``source() == UNDEFINED``.
    """
    result = p._count()
    if result == POSITIVE_INFINITY:
        return POSITIVE_INFINITY
    return result


Paths.count = count
ReversiblePaths.count = count
