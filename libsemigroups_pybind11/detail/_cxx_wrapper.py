# -*- coding: utf-8 -*-

# Copyright (c) 2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-member, protected-access

"""
This package provides some functions and a class CxxWrapper to help wrap
multiple C++ types into a single python type. See:

    * action.py
    * adapters.py
    * froidure_pin.py
    * transf.py

for examples.
"""

import abc
from typing import Any
from typing_extensions import Self

from _libsemigroups_pybind11 import UNDEFINED as _UNDEFINED


def to_cxx(x: Any) -> Any:
    """
    This function returns x._cxx_obj if x is a CxxWrapper, and x o/w.
    """
    if isinstance(x, CxxWrapper):
        return x._cxx_obj
    return x


def to_py(Element: Any, x: Any, *args) -> Any:  # pylint: disable=invalid-name
    """
    This function returns Element(x) if x is not None and type(x) != Element, and x o/w.
    """
    if x is not None and not isinstance(x, Element):
        return Element(x, *args)
    return x


class CxxWrapper(metaclass=abc.ABCMeta):
    # pylint: disable=missing-class-docstring
    # pylint: disable=protected-access, no-member, too-few-public-methods
    def __init__(self: Self, expected_kwargs, **kwargs):
        """See action.py for how to use this"""
        # pylint: disable=invalid-name
        if len(kwargs) != len(expected_kwargs):
            raise TypeError(
                f"expected {len(expected_kwargs)} keyword arguments,"
                f" found {len(kwargs)}"
            )
        for kwarg in expected_kwargs:
            if kwarg not in kwargs:
                raise ValueError(
                    f'required keyword argument "{kwarg}" not found, '
                    f"found {kwargs} instead"
                )
        # the next line ensures we get the values in the same order as in
        # lookup
        values = tuple(kwargs[x] for x in expected_kwargs)
        lookup = self.py_to_cxx_type_dict
        if values in lookup:
            for key, val in kwargs.items():
                setattr(self, key, val)
            self._cxx_obj = None
            return

        raise ValueError(
            f"unexpected keyword argument combination {kwargs.values()}, "
            f"expected one of {lookup.keys()}"
        )

    def __getattr__(self: Self, meth_name: str):
        """
        This method is the fallback for method calls for an instance A of
        CxxWrapper, e.g. A.size(), if there's no such method explicitly
        defined, then we just fall back to calling the method of the same name
        on the underlying self._cxx_obj.

        If self._cxx_obj requires initialisation, then this should be
        implemented in the __getattr__ method of the derived class.
        """

        def cxx_fn_wrapper(*args) -> Any:
            if len(args) == 1 and isinstance(args[0], list):
                args = args[0]
                return getattr(self._cxx_obj, meth_name)([to_cxx(x) for x in args])
            return getattr(self._cxx_obj, meth_name)(*(to_cxx(x) for x in args))

        return cxx_fn_wrapper

    def __repr__(self: Self) -> str:
        if self._cxx_obj is not None:
            return self._cxx_obj.__repr__()
        return ""

    @property
    def py_to_cxx_type_dict(self: Self) -> dict:
        return self.__class__.__lookup

    # TODO type annotations
    @py_to_cxx_type_dict.setter
    def py_to_cxx_type_dict(self: Self, value):
        # TODO check that value is a dict of the correct structure
        self.__class__.__lookup = value

    def _cxx_obj_type_from(self: Self, samples=(), types=()) -> Any:
        py_types = tuple([type(x) for x in samples] + list(types))
        lookup = self.py_to_cxx_type_dict
        if py_types not in lookup:
            raise ValueError(
                f"unexpected keyword argument combination {py_types}, "
                f"expected one of {tuple(lookup.keys())}"
            )
        if not isinstance(lookup[py_types], dict):
            return lookup[py_types]
        lookup = lookup[py_types]
        cpp_types = tuple([type(to_cxx(x)) for x in samples] + list(types))
        if cpp_types not in lookup:
            raise ValueError(
                f"unexpected keyword argument combination {cpp_types}, "
                f"expected one of {lookup.keys()}"
            )
        return lookup[cpp_types]


# Decorators


# Only check that the arg is a list because the checking of the items is done
# by pybind11
def accepts_list(f):
    """TODO"""

    def new_f(self, w):
        # TODO count args = f count_args
        if not isinstance(w, list):
            raise TypeError(f"expect the argument to be a list, but found {type(w)}")

        return f(self, w)

    return new_f


def may_return_undefined(func):
    """
    This function is a decorator for functions/methods that might return
    UNDEFINED (as an integer, since there's no other option in C++), and which
    should return the UNDEFINED object.
    """

    def wrapper(*args):
        result = func(*args)
        if result in (4294967295, 18446744073709551615):
            return _UNDEFINED
        return result

    return wrapper
