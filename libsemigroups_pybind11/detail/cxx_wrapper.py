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
from types import MethodType
from typing import Any, Callable
from typing_extensions import Self
from inspect import signature
from functools import update_wrapper, wraps

from _libsemigroups_pybind11 import UNDEFINED as _UNDEFINED

pybind11_type = type(_UNDEFINED)


def to_cxx(x: Any) -> Any:
    """
    This function returns x._cxx_obj if x is a CxxWrapper, and x o/w.
    """
    if isinstance(x, CxxWrapper):
        return x._cxx_obj
    return x


# TODO rm
def to_py(Element: Any, x: Any, *args) -> Any:  # pylint: disable=invalid-name
    """
    This function returns Element(x) if x is not None and type(x) != Element, and x o/w.
    """
    if x is not None and not isinstance(x, Element):
        return Element(x, *args)
    return x


_cxx_wrapped_type_to_py_type = {}


def to_py_new(x: Any, *args) -> Any:
    """
    This function returns Element(x) if x is not None and type(x) != Element, and x o/w.
    """
    global _cxx_wrapped_type_to_py_type
    if type(x) in _cxx_wrapped_type_to_py_type:
        return _cxx_wrapped_type_to_py_type[type(x)](x, *args)
    return x


def register_cxx_wrapped_type(cxx_type: pybind11_type, py_type: type) -> None:
    global _cxx_wrapped_type_to_py_type
    assert cxx_type not in _cxx_wrapped_type_to_py_type
    _cxx_wrapped_type_to_py_type[cxx_type] = py_type


class CxxWrapper(metaclass=abc.ABCMeta):
    # pylint: disable=missing-class-docstring
    # pylint: disable=protected-access, no-member
    def __init__(
        self: Self,
        *args,
        required_kwargs=(),
        optional_kwargs=(),
        **kwargs,
    ):
        if (
            len(args) == 1
            and len(kwargs) == 0
            and type(args[0]) in self._all_wrapped_cxx_types
        ):
            # Copy constructor like construction directly from cxx object
            self._cxx_obj = args[0]
            self.py_template_params = self.py_template_params_from_cxx_obj()
            return

        if (
            not len(required_kwargs)
            <= len(kwargs)
            <= len(required_kwargs) + len(optional_kwargs)
        ):
            raise TypeError(
                f"expected between {len(required_kwargs)} and {len(required_kwargs) + len(optional_kwargs)} "
                f"keyword arguments, found {len(kwargs)}"
            )

        for kwarg in required_kwargs:
            if kwarg not in kwargs:
                raise ValueError(
                    f'required keyword argument "{kwarg}" not found, '
                    f"found {tuple(kwargs.keys())} instead"
                )
        for kwarg in kwargs:
            if kwarg not in required_kwargs and kwarg not in optional_kwargs:
                raise ValueError(
                    f'unexpected keyword argument "{kwarg}", '
                    f"required keyword arguments are {required_kwargs} and optional keyword arguments are {optional_kwargs}"
                )

        self._cxx_obj = None
        self.py_template_params = None

    def __getattr__(self: Self, name: str):
        """
        This method is the fallback for method calls for an instance A of
        CxxWrapper, e.g. A.size(), if there's no such method explicitly
        defined, then we just fall back to calling the method of the same name
        on the underlying self._cxx_obj.

        If self._cxx_obj requires initialisation, then this should be
        implemented in the __getattr__ method of the derived class.
        """
        if type(getattr(self._cxx_obj, name)) is MethodType:

            def cxx_fn_wrapper(*args) -> Any:
                if len(args) == 1 and isinstance(args[0], list):
                    args = args[0]
                    return getattr(self._cxx_obj, name)(
                        [to_cxx(x) for x in args]
                    )
                return getattr(self._cxx_obj, name)(*(to_cxx(x) for x in args))

            return cxx_fn_wrapper
        else:
            return getattr(self._cxx_obj, name)

    def __repr__(self: Self) -> str:
        if self._cxx_obj is not None:
            return self._cxx_obj.__repr__()
        return ""

    def __copy__(self: Self) -> Self:
        global _cxx_wrapped_type_to_py_type
        if self._cxx_obj is not None:
            if hasattr(self._cxx_obj, "__copy__"):
                return _cxx_wrapped_type_to_py_type[type(self._cxx_obj)](
                    self._cxx_obj.__copy__()
                )
            raise NotImplementedError(
                f"{type(self._cxx_obj)} has no member named __copy__"
            )
        raise NameError("_cxx_obj has not been defined")

    def __eq__(self: Self, that) -> bool:
        if self._cxx_obj is not None:
            if hasattr(self._cxx_obj, "__eq__"):
                return self._cxx_obj.__eq__(that._cxx_obj)
            raise NotImplementedError(
                f"{type(self._cxx_obj)} has no member named __eq__"
            )
        raise NameError("_cxx_obj has not been defined")

    def _cxx_obj_type_from(self: Self, samples=(), types=()) -> Any:
        py_types = tuple([type(x) for x in samples] + list(types))
        lookup = self._py_template_params_to_cxx_type
        if py_types not in lookup:
            raise ValueError(
                f"unexpected keyword argument combination {py_types}, "
                f"expected one of {tuple(lookup.keys())}"
            )
        if not isinstance(lookup[py_types], dict):
            return lookup[py_types]
        lookup = lookup[py_types]
        cxx_types = tuple([type(to_cxx(x)) for x in samples] + list(types))
        if cxx_types not in lookup:
            raise ValueError(
                f"unexpected keyword argument combination {cxx_types}, "
                f"expected one of {lookup.keys()}"
            )
        return lookup[cxx_types]

    def py_template_params_from_cxx_obj(self: Self) -> tuple:
        assert self._cxx_obj is not None
        if type(self._cxx_obj) in self._cxx_type_to_py_template_params:
            return self._cxx_type_to_py_template_params[type(self._cxx_obj)]
        return None

    def init_cxx_obj(self: Self, *args) -> None:
        assert self.py_template_params is not None
        self._cxx_obj = self._py_template_params_to_cxx_type[
            self.py_template_params
        ](*(to_cxx(x) for x in args))


# TODO proper annotations
def wrap_cxx_mem_fn(cxx_mem_fn: pybind11_type) -> Callable:
    global _cxx_wrapped_type_to_py_type

    def cxx_mem_fn_wrapper(self, *args):
        # TODO move the first if-clause into to_cxx
        if len(args) == 1 and isinstance(args[0], list):
            args = [[to_cxx(x) for x in args[0]]]
        result = getattr(self._cxx_obj, cxx_mem_fn.__name__)(
            *(to_cxx(x) for x in args)
        )
        if result is self._cxx_obj:
            return self
        if type(result) in _cxx_wrapped_type_to_py_type:
            cached_val = f"_cached_return_value_{cxx_mem_fn.__name__}"
            # TODO use args too in cached_val?
            if (
                hasattr(self, cached_val)
                and result is getattr(self, cached_val)._cxx_obj
            ):
                return getattr(self, cached_val)
            result = _cxx_wrapped_type_to_py_type[type(result)](result)
            setattr(self, cached_val, result)
            return result

        return result

    update_wrapper(cxx_mem_fn_wrapper, cxx_mem_fn)
    return cxx_mem_fn_wrapper


# TODO proper annotations
def wrap_cxx_free_fn(cxx_free_fn: pybind11_type) -> Callable:
    global _cxx_wrapped_type_to_py_type

    def cxx_free_fn_wrapper(*args):
        result = cxx_free_fn(*(to_cxx(x) for x in args))
        if type(result) in _cxx_wrapped_type_to_py_type:
            return _cxx_wrapped_type_to_py_type[type(result)](result)
        return result

    update_wrapper(cxx_free_fn_wrapper, cxx_free_fn)
    return cxx_free_fn_wrapper


# TODO proper annotations
# TODO remove py_return_type, just use _cxx_wrapped_type_to_py_type
def unwrap_cxx_free_fn(
    cxx_free_fn: pybind11_type, py_return_type: type
) -> Callable:
    def cxx_free_fn_unwrapper(*args):
        return py_return_type(cxx_free_fn(*args))

    update_wrapper(cxx_free_fn_unwrapper, cxx_free_fn)
    return cxx_free_fn_unwrapper


def copy_cxx_mem_fns(cxx_class: pybind11_type, py_class: CxxWrapper) -> None:
    """
    Copy all the non-special methods of *cxx_class* into methods of *py_class*
    that call the cxx member function on the _cxx_obj.
    """
    for py_meth_name in dir(cxx_class):
        if (
            (not py_meth_name.startswith("_"))
            and py_meth_name not in dir(py_class)
            # and type(getattr(cxx_class, py_meth_name)) is MethodType
        ):
            setattr(
                py_class,
                py_meth_name,
                wrap_cxx_mem_fn(getattr(cxx_class, py_meth_name)),
            )


########################################################################
# Decorators
########################################################################


def may_return_wrapped_cxx_obj(method):
    @wraps(method)
    def wrapper(self, *args):
        result = method(self, *args)
        if type(result) in _cxx_wrapped_type_to_py_type:
            return _cxx_wrapped_type_to_py_type[type(result)](result)
        return result

    return wrapper
