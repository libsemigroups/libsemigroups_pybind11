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


def copydoc(original, *args):
    """
    Decorator that can be used to copy the doc from one function to another,
    for example:

    @copydoc(Transf1.__init__)
    def __init___(self) -> None:
       pass
    """

    original_doc = original.__doc__
    patterns = {"Overloaded function.": "", "__init__(*args, **kwargs)": ""}
    for arg in args:
        doc = arg.__doc__
        for old, new in patterns.items():
            doc = doc.replace(old, new)
        original_doc += doc

    def wrapper(target):
        target.__doc__ = original_doc
        return target

    return wrapper
