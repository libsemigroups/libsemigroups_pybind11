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


def max_method(*args):
    """
    max(*args, **kwargs)

        Overloaded function.

        .. py:method:: max(self: Paths) -> int | POSITIVE_INFINITY
            :noindex:

            Get the maximum length of path in the range. This function returns the
            current maximum length of paths in the range. The initial value is
            :any:`POSITIVE_INFINITY`.

            :returns:
               The maximum length of paths in the range.

            :rtype:
               int | PositiveInfinity

        .. py:method:: max(self: Paths, val: int | POSITIVE_INFINITY) -> Paths
            :noindex:

            Set the maximum length of path in the range.

            This function can be used to set the maximum length of path that will be
            contained in the range. If this function is not called, then the range will
            contain paths of unbounded length (possibly infinitely many).

            :param val: the maximum path length.
            :type val: int

            :returns: ``self``.
            :rtype: Paths
    """
    if len(args) == 1:
        result = args[0]._max()
        if result == POSITIVE_INFINITY:
            return POSITIVE_INFINITY
        return result
    if len(args) > 1:
        return args[0]._max(*args[1:])
    raise TypeError


Paths.count = count
Paths.max = max_method
ReversiblePaths.count = count
ReversiblePaths.max = max_method
