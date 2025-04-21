# -*- coding: utf-8 -*-

# Copyright (c) 2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This package provides decorators for the implementation of
libsemigroups_pybind11.
"""


def copydoc(original):
    """
    Decorator that can be used to copy the doc from one function to another,
    for example:

    @copydoc(Transf1.__init__)
    def __init___(self):
       pass
    """

    original_doc = original.__doc__

    def wrapper(target):
        target.__doc__ = original_doc
        return target

    return wrapper
