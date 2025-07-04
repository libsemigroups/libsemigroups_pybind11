# -*- coding: utf-8 -*-

# Copyright (c) 2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This package provides some functions and a class CxxWrapper to help wrap
multiple C++ types into a single python type. I.e. to wrap a C++ class template
with various template parameters into a single python type.
"""

from functools import update_wrapper
from inspect import isclass
from types import MethodType

from typing import Any, Callable
from typing_extensions import Self

# We would really like Pybind11Type to point to pybind11_builtins.pybind11_type.
# However, this is not easily importable into python; see the discussions in
# https://github.com/pybind/pybind11/issues/3945 and
# https://github.com/pybind/cmake_example/pull/164.
# A proposed workaround would be to do the following:
# Pybind11Type = TypeVar('Pybind11Type', bound=type(Undefined))
# but that isn't allowed, since function calls aren't permitted in TypeVar
# expressions. Hence, the next best thing is to use the parent class of
# pybind11_builtins.pybind11_type, which is simply type. Further runtime checks
# can be added latter if required.
Pybind11Type = type


def to_cxx(x: Any) -> Any:
    """
    This function returns x._cxx_obj if x is a CxxWrapper, and x o/w.
    """
    if isinstance(x, CxxWrapper):
        return x._cxx_obj  # pylint: disable=protected-access
    return x


def to_py(x: Any, *args) -> Any:
    """
    This function returns the CxxWrapper type wrapping an instance of <x> if
    <x> is an instance of CxxWrapper type, and <x> o/w.
    """
    if type(x) in _CXX_WRAPPED_TYPE_TO_PY_TYPE:
        return _CXX_WRAPPED_TYPE_TO_PY_TYPE[type(x)](x, *args)
    return x


# Dict containing every single C++ type wrapped by CxxWrapper
_CXX_WRAPPED_TYPE_TO_PY_TYPE = {}


def register_cxx_wrapped_type(cxx_type: Pybind11Type, py_type: type) -> None:
    """
    Function for adding to the _CXX_WRAPPED_TYPE_TO_PY_TYPE dictionary.
    """
    assert cxx_type not in _CXX_WRAPPED_TYPE_TO_PY_TYPE
    _CXX_WRAPPED_TYPE_TO_PY_TYPE[cxx_type] = py_type


class CxxWrapper:
    # pylint: disable=missing-class-docstring
    def __init__(
        self: Self,
        *args,
        required_kwargs=(),
        optional_kwargs=(),
        **kwargs,
    ) -> None:
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
                f"expected between {len(required_kwargs)} and "
                f"{len(required_kwargs) + len(optional_kwargs)} "
                f"keyword arguments, found {len(kwargs)}"
            )

        for kwarg in required_kwargs:
            if kwarg not in kwargs:
                raise TypeError(
                    f'required keyword argument "{kwarg}" not found, '
                    f"found {tuple(kwargs.keys())} instead"
                )
        for kwarg in kwargs:
            if kwarg not in required_kwargs and kwarg not in optional_kwargs:
                raise TypeError(
                    f'unexpected keyword argument "{kwarg}", '
                    f"required keyword arguments are {required_kwargs} "
                    f"and optional keyword arguments are {optional_kwargs}"
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
        if isinstance(getattr(self._cxx_obj, name), MethodType):

            def cxx_fn_wrapper(*args) -> Any:
                if len(args) == 1 and isinstance(args[0], list):
                    args = args[0]
                    return getattr(self._cxx_obj, name)(
                        [to_cxx(x) for x in args]
                    )
                return getattr(self._cxx_obj, name)(*(to_cxx(x) for x in args))

            return cxx_fn_wrapper
        return getattr(self._cxx_obj, name)

    def __repr__(self: Self) -> str:
        if self._cxx_obj is not None:
            return self._cxx_obj.__repr__()
        return ""

    def __copy__(self: Self) -> Self:
        if self._cxx_obj is not None:
            if hasattr(self._cxx_obj, "__copy__"):
                return to_py(self._cxx_obj.__copy__())
            raise NotImplementedError(
                f"{type(self._cxx_obj)} has no member named __copy__"
            )
        raise NameError("_cxx_obj has not been defined")

    def __eq__(self: Self, that: Self) -> bool:
        if self._cxx_obj is not None:
            if hasattr(self._cxx_obj, "__eq__"):
                return self._cxx_obj.__eq__(that._cxx_obj)
            raise NotImplementedError(
                f"{type(self._cxx_obj)} has no member named __eq__"
            )
        raise NameError("_cxx_obj has not been defined")

    def __call__(self: Self, *args) -> Any:
        if self._cxx_obj is not None:
            if callable(self._cxx_obj):
                return self._cxx_obj.__call__(*args)
            raise NotImplementedError(
                f"{type(self._cxx_obj)} has no member named __call__"
            )
        raise NameError("_cxx_obj has not been defined")

    def py_template_params_from_cxx_obj(self: Self) -> tuple:
        """
        Get the py_template_params from _cxx_obj. Requires
        _cxx_type_to_py_template_params to be defined.
        """
        assert self._cxx_obj is not None
        if type(self._cxx_obj) in self._cxx_type_to_py_template_params:
            return self._cxx_type_to_py_template_params[type(self._cxx_obj)]
        return None

    def init_cxx_obj(self: Self, *args) -> None:
        """
        Initialize _cxx_obj from args. Requires py_template_params to be
        defined.
        """
        assert self.py_template_params is not None
        self._cxx_obj = self._py_template_params_to_cxx_type[
            self.py_template_params
        ](*(to_cxx(x) for x in args))


# TODO(1) proper annotations
def wrap_cxx_mem_fn(cxx_mem_fn: Pybind11Type) -> Callable:
    """
    This function creates a wrapper around the pybind11 c++ member function
    <cxx_mem_fn> that automatically wraps and unwraps CxxWrapper types, and
    caches the output. The documentation + annotations etc are also copied from
    <cxx_mem_fn> to the returned function.
    """

    def cxx_mem_fn_wrapper(self, *args):
        # TODO(1) move the first if-clause into to_cxx?
        if len(args) == 1 and isinstance(args[0], list):
            args = [[to_cxx(x) for x in args[0]]]
        result = getattr(to_cxx(self), cxx_mem_fn.__name__)(
            *(to_cxx(x) for x in args)
        )
        if result is to_cxx(self):
            return self
        if type(result) in _CXX_WRAPPED_TYPE_TO_PY_TYPE:
            cached_val = f"_cached_return_value_{cxx_mem_fn.__name__}"
            # TODO(1) use args too in cached_val?
            if hasattr(self, cached_val) and result is to_cxx(
                getattr(self, cached_val)
            ):
                return getattr(self, cached_val)
            result = _CXX_WRAPPED_TYPE_TO_PY_TYPE[type(result)](result)
            setattr(self, cached_val, result)
            return result

        return result

    update_wrapper(cxx_mem_fn_wrapper, cxx_mem_fn)
    return cxx_mem_fn_wrapper


# TODO(1) proper annotations
def wrap_cxx_free_fn(cxx_free_fn: Pybind11Type) -> Callable:
    """
    This function creates a wrapper around the pybind11 c++ free function
    <cxx_free_fn> that automatically wraps and unwraps CxxWrapper types. The
    documentation + annotations etc are also copied from <cxx_mem_fn> to the
    returned function.
    """

    def cxx_free_fn_wrapper(*args):
        return to_py(cxx_free_fn(*(to_cxx(x) for x in args)))

    update_wrapper(cxx_free_fn_wrapper, cxx_free_fn)
    return cxx_free_fn_wrapper


def copy_cxx_mem_fns(cxx_class: Pybind11Type, py_class: CxxWrapper) -> None:
    """
    Copy all the non-special methods of *cxx_class* into methods of *py_class*
    that call the cxx member function on the _cxx_obj.
    """
    for py_meth_name in dir(cxx_class):
        if (not py_meth_name.startswith("_")) and py_meth_name not in dir(
            py_class
        ):
            if not isclass(getattr(cxx_class, py_meth_name)):
                setattr(
                    py_class,
                    py_meth_name,
                    wrap_cxx_mem_fn(getattr(cxx_class, py_meth_name)),
                )
            else:
                setattr(
                    py_class, py_meth_name, getattr(cxx_class, py_meth_name)
                )
