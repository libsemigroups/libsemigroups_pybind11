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

import re as _re


def _get_overloaded_doc(func):
    """Return the docstring of a function as if that function was overloaded"""
    doc = func.__doc__
    if "Overloaded function." in doc:
        return doc
    return "1. " + doc


def _correct_overloads(target, base_func, extra_funcs):
    """
    Fix the docstring of copied overloaded functions

    When functions are bound, pybind11 adds some decorations to the docstring
    (unless this is explicitly turned off using
    py::options::disable_function_signatures()). When a function is overloaded,
    this decoration includes the string 'Overloaded function.', a new signature
    with parameters *args and **kwargs, and some enumeration of the different
    overloads.

    To combine multiple docstrings of functions, it is necessary to:
     * remove all but the first occurrence of the pybind11 inserted strings;
     * ensure every function is documented as if it was overloaded; and
     * number the overloads 1, 2, ..., N.

    This function does these things.
    """
    target_name = target.__name__
    old_names = set(fn.__name__ for fn in extra_funcs) | {base_func.__name__}
    base_doc = _get_overloaded_doc(base_func)
    extra_doc = "".join([_get_overloaded_doc(func) for func in extra_funcs])

    # Remove pybind11 inserted strings
    replacements = {
        f"{old_name}(*args, **kwargs)\n": "" for old_name in old_names
    }
    replacements["Overloaded function.\n"] = ""
    for old, new in replacements.items():
        extra_doc = extra_doc.replace(old, new)

    # Fix overload numbering
    overload_counter = 1
    doc_blocks = _re.split(
        rf"\d+\. (?:{'|'.join(old_names)})", base_doc + extra_doc
    )
    new_doc = doc_blocks[0]
    for doc_block in doc_blocks[1:]:
        new_doc += f"{overload_counter}. {target_name}" + doc_block
        overload_counter += 1
    return new_doc


def copydoc(func, *extra_funcs):
    """
    Decorator that can be used to copy the doc from one function to another,
    for example:

    @copydoc(Transf1.__init__)
    def __init___(self) -> None:
       pass
    """
    new_doc = func.__doc__

    def wrapper(target):
        if extra_funcs:
            target.__doc__ = _correct_overloads(target, func, extra_funcs)
        else:
            target.__doc__ = new_doc
        return target

    return wrapper
