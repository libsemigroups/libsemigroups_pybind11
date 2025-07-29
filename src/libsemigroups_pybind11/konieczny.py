# -*- coding: utf-8 -*-

# Copyright (c) 2022-2025 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This page contains the documentation for the ``konieczny`` subpackage, that
contains helper functions for the :any:`Konieczny` class.
"""

from collections.abc import Iterator as _Iterator
from typing import TypeVar as _TypeVar
from typing_extensions import Self as _Self

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module
    BMat as _BMat,
    BMat8 as _BMat8,
    KoniecznyBMat as _KoniecznyBMat,
    KoniecznyBMat8 as _KoniecznyBMat8,
    KoniecznyPPerm1 as _KoniecznyPPerm1,
    KoniecznyPPerm2 as _KoniecznyPPerm2,
    KoniecznyPPerm4 as _KoniecznyPPerm4,
    KoniecznyTransf1 as _KoniecznyTransf1,
    KoniecznyTransf2 as _KoniecznyTransf2,
    KoniecznyTransf4 as _KoniecznyTransf4,
    KoniecznyBMatDClass as _KoniecznyBMatDClass,
    KoniecznyBMat8DClass as _KoniecznyBMat8DClass,
    KoniecznyPPerm1DClass as _KoniecznyPPerm1DClass,
    KoniecznyPPerm2DClass as _KoniecznyPPerm2DClass,
    KoniecznyPPerm4DClass as _KoniecznyPPerm4DClass,
    KoniecznyTransf1DClass as _KoniecznyTransf1DClass,
    KoniecznyTransf2DClass as _KoniecznyTransf2DClass,
    KoniecznyTransf4DClass as _KoniecznyTransf4DClass,
    PPerm1 as _PPerm1,
    PPerm2 as _PPerm2,
    PPerm4 as _PPerm4,
    Transf1 as _Transf1,
    Transf2 as _Transf2,
    Transf4 as _Transf4,
)

from .detail.cxx_wrapper import (
    to_cxx as _to_cxx,
    to_py as _to_py,
    CxxWrapper as _CxxWrapper,
    register_cxx_wrapped_type as _register_cxx_wrapped_type,
    copy_cxx_mem_fns as _copy_cxx_mem_fns,
)
from .detail.decorators import copydoc as _copydoc


########################################################################
# Konieczny python class
########################################################################


class Konieczny(_CxxWrapper):  # pylint: disable=missing-class-docstring
    Element = _TypeVar("Element")
    __doc__ = _KoniecznyBMat.__doc__

    _py_template_params_to_cxx_type = {
        (_BMat,): _KoniecznyBMat,
        (_BMat8,): _KoniecznyBMat8,
        (_PPerm1,): _KoniecznyPPerm1,
        (_PPerm2,): _KoniecznyPPerm2,
        (_PPerm4,): _KoniecznyPPerm4,
        (_Transf1,): _KoniecznyTransf1,
        (_Transf2,): _KoniecznyTransf2,
        (_Transf4,): _KoniecznyTransf4,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    ########################################################################
    # Konieczny nested classes
    ########################################################################

    class DClass(_CxxWrapper):  # pylint: disable=missing-class-docstring
        __doc__ = _KoniecznyBMat8DClass.__doc__

        Element = _TypeVar("Element")

        _py_template_params_to_cxx_type = {
            (_BMat,): _KoniecznyBMatDClass,
            (_BMat8,): _KoniecznyBMat8DClass,
            (_PPerm1,): _KoniecznyPPerm1DClass,
            (_PPerm2,): _KoniecznyPPerm2DClass,
            (_PPerm4,): _KoniecznyPPerm4DClass,
            (_Transf1,): _KoniecznyTransf1DClass,
            (_Transf2,): _KoniecznyTransf2DClass,
            (_Transf4,): _KoniecznyTransf4DClass,
        }

        _cxx_type_to_py_template_params = dict(
            zip(
                _py_template_params_to_cxx_type.values(),
                _py_template_params_to_cxx_type.keys(),
            )
        )

        _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

        def __contains__(self: _Self, x: Element) -> bool:
            return _to_cxx(self).contains(_to_cxx(x))

        @_copydoc(_KoniecznyBMat8DClass.__init__)
        def __init__(self: _Self, *args) -> None:
            super().__init__(*args)
            assert _to_cxx(self) is not None

    ########################################################################
    # Konieczny special methods
    ########################################################################

    # TODO(1) Add a keyword argument for element type to the __init__ function,
    # so that we know which Konieczny type to construct based on the element
    # and/or the underlying cxx type.
    @_copydoc(_KoniecznyBMat.__init__)
    def __init__(self: _Self, *args) -> None:
        super().__init__(*args)
        if _to_cxx(self) is not None:
            return
        if len(args) == 0:
            raise TypeError("expected at least 1 argument, found 0")

        if isinstance(args[0], list) and len(args) == 1:
            gens = args[0]
        else:
            gens = list(args)
        gens = [_to_cxx(x) for x in gens]
        self.py_template_params = (type(gens[0]),)
        self.init_cxx_obj(gens)

    def __contains__(self: _Self, x: Element) -> bool:
        return _to_cxx(self).contains(_to_cxx(x))

    ########################################################################
    # Iterators
    ########################################################################

    @_copydoc(_KoniecznyBMat.generators)
    def generators(self: _Self) -> _Iterator[Element]:
        # pylint: disable=missing-function-docstring
        return map(
            _to_py,
            _to_cxx(self).generators(),
        )

    @_copydoc(_KoniecznyBMat.current_D_classes)
    def current_D_classes(self: _Self) -> _Iterator[DClass]:
        # pylint: disable=missing-function-docstring,invalid-name
        return map(
            _to_py,
            _to_cxx(self).current_D_classes(),
        )

    @_copydoc(_KoniecznyBMat.D_classes)
    def D_classes(self: _Self) -> _Iterator[DClass]:
        # pylint: disable=missing-function-docstring,invalid-name
        return map(
            _to_py,
            _to_cxx(self).D_classes(),
        )


########################################################################
# Copy mem fns from sample C++ type and register types
########################################################################

_copy_cxx_mem_fns(_KoniecznyBMat8, Konieczny)
_copy_cxx_mem_fns(_KoniecznyBMat8DClass, Konieczny.DClass)

for (
    _type
) in (
    Konieczny._py_template_params_to_cxx_type.values()  # pylint: disable=protected-access
):
    _register_cxx_wrapped_type(_type, Konieczny)

for (
    _type
) in (
    Konieczny.DClass._py_template_params_to_cxx_type.values()  # pylint: disable=protected-access
):
    _register_cxx_wrapped_type(_type, Konieczny.DClass)
