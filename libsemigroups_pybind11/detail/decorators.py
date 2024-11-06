# -*- coding: utf-8 -*-

# Copyright (c) 2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-member, protected-access

"""
This package provides decorators for the implementation of
libsemigroups_pybind11.
"""

<<<<<<< HEAD
from functools import wraps

from _libsemigroups_pybind11 import (
    UNDEFINED as _UNDEFINED,
    POSITIVE_INFINITY as _POSITIVE_INFINITY,
)
=======
from _libsemigroups_pybind11 import UNDEFINED as _UNDEFINED
>>>>>>> cb4ac5b (Refactor a bit)


def copydoc(original):
    """
    Decorator that can be used to copy the doc from one function to another,
    for example:

    @copydoc(Transf1.__init__)
    def __init___(self):
       pass
    """

    original_doc = original.__doc__

    if original_doc.startswith(original.__name__):
        original_doc = "\n".join(original_doc.split("\n")[2:])

    def wrapper(target):
        target.__doc__ = original_doc
        return target

    return wrapper


def may_return_undefined(func):
    """
    This function is a decorator for functions/methods that might return
    UNDEFINED (as an integer, since there's no other option in C++), and which
    should return the UNDEFINED object.
    """
    vals = tuple(2 ** (2**n) - 1 for n in range(3, 7))

    @wraps(func)
    def wrapper(*args, **kwargs):
        result = func(*args, **kwargs)
        if result in vals:
            return _UNDEFINED
        return result

    return wrapper


def may_return_positive_infinity(func):
    """
    This function is a decorator for functions/methods that might return
    POSITIVE_INFINITY (as an integer, since there's no other option in C++), and which
    should return the POSITIVE_INFINITY object.
    """
    vals = tuple(2 ** (2**n) - 2 for n in range(3, 7))

    @wraps(func)
    def wrapper(self, *args, **kwargs):
        result = func(self, *args, **kwargs)
        if result in vals:
            return _POSITIVE_INFINITY
        return result

    return wrapper
