# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This page contains the documentation for the ``matrix`` subpackage, that
contains helper functions for the :any:`Matrix` class.
"""

from enum import Enum as _Enum
from typing import Union as _Union
from typing_extensions import Self as _Self

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module
    BMat as _BMat,
    IntMat as _IntMat,
    MaxPlusMat as _MaxPlusMat,
    MaxPlusTruncMat as _MaxPlusTruncMat,
    MinPlusMat as _MinPlusMat,
    MinPlusTruncMat as _MinPlusTruncMat,
    NTPMat as _NTPMat,
    NegativeInfinity as _NegativeInfinity,
    PositiveInfinity as _PositiveInfinity,
    ProjMaxPlusMat as _ProjMaxPlusMat,
    matrix_row_space_size as _row_space_size,
    matrix_period as _period,
    matrix_row_basis as _row_basis,
    matrix_threshold as _threshold,
)

from .detail.cxx_wrapper import (
    CxxWrapper as _CxxWrapper,
    to_cxx as _to_cxx,
    copy_cxx_mem_fns as _copy_cxx_mem_fns,
    wrap_cxx_free_fn as _wrap_cxx_free_fn,
)

from .detail.decorators import copydoc as _copydoc


# the underscore prefix stops this from appearing in the doc of the
# "matrix" submodule
class MatrixKind(_Enum):
    # pylint: disable=invalid-name
    """

    This enum contains values that can be used to describe the type of semiring
    over which a :any:`Matrix` is defined.

    .. py:attribute:: MatrixKind.Boolean
        :value: <MatrixKind.Boolean: 0>

        For matrices over the Boolean semiring.

    .. py:attribute:: MatrixKind.Integer
        :value: <MatrixKind.Integer: 1>

        For matrices over the usual ring of integers.

    .. py:attribute:: MatrixKind.MaxPlus
        :value: <MatrixKind.MaxPlus: 2>

        For matrices over the max-plus semiring.

    .. py:attribute:: MatrixKind.MinPlus
        :value: <MatrixKind.MinPlus: 3>

        For matrices over the min-plus semiring.

    .. py:attribute:: MatrixKind.ProjMaxPlus
        :value: <MatrixKind.ProjMaxPlus: 4>

        For projective matrices over the max-plus semiring.

    .. py:attribute:: MatrixKind.MaxPlusTrunc
        :value: <MatrixKind.MaxPlusTrunc: 5>

        For matrices over the truncated max-plus semiring.

    .. py:attribute:: MatrixKind.MinPlusTrunc
        :value: <MatrixKind.MinPlusTrunc: 6>

        For matrices over the truncated min-plus semiring.

    .. py:attribute:: MatrixKind.NTP
        :value: <MatrixKind.NTP: 7>

        For matrices over the semiring of natural numbers quotiented by `t
        = t + p`.
    """

    Boolean = 0
    Integer = 1
    MaxPlus = 2
    MinPlus = 3
    ProjMaxPlus = 4
    MaxPlusTrunc = 5
    MinPlusTrunc = 6
    NTP = 7


########################################################################
# Matrix python class
########################################################################


class Matrix(_CxxWrapper):  # pylint: disable=missing-class-docstring
    __doc__ = _BMat.__doc__

    _py_template_params_to_cxx_type = {
        (MatrixKind.Boolean,): _BMat,
        (MatrixKind.Integer,): _IntMat,
        (MatrixKind.MaxPlus,): _MaxPlusMat,
        (MatrixKind.MinPlus,): _MinPlusMat,
        (MatrixKind.ProjMaxPlus,): _ProjMaxPlusMat,
        (MatrixKind.MaxPlusTrunc,): _MaxPlusTruncMat,
        (MatrixKind.MinPlusTrunc,): _MinPlusTruncMat,
        (MatrixKind.NTP,): _NTPMat,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    # TODO(1) could update to use kwargs for threshold and period
    @_copydoc(_BMat.__init__, _MaxPlusTruncMat.__init__, _NTPMat.__init__)
    def __init__(self: _Self, kind: MatrixKind, *args) -> None:
        super().__init__(
            *args,
            required_kwargs=(),
        )
        if _to_cxx(self) is not None:
            return
        # TODO(1) arg checks?
        if not isinstance(kind, MatrixKind):
            raise TypeError("the 1st argument must be a MatrixKind")
        self.py_template_params = (kind,)
        self.init_cxx_obj(*args)

    def __getitem__(
        self: _Self, *args
    ) -> _Union[int, _Self, _PositiveInfinity, _NegativeInfinity]:
        return _to_cxx(self).__getitem__(*args)

    def __setitem__(self: _Self, *args):
        return _to_cxx(self).__setitem__(*args)

    def __imul__(
        self: _Self,
        that: _Union[_Self, int, _PositiveInfinity, _NegativeInfinity],
    ) -> _Self:
        return _to_cxx(self).__imul__(that)

    def __mul__(
        self: _Self,
        that: _Union[_Self, int, _PositiveInfinity, _NegativeInfinity],
    ) -> _Self:
        return _to_cxx(self).__mul__(that)

    def __iadd__(
        self: _Self,
        that: _Union[_Self, int, _PositiveInfinity, _NegativeInfinity],
    ) -> _Self:
        return _to_cxx(self).__iadd__(that)

    def __add__(
        self: _Self,
        that: _Union[_Self, int, _PositiveInfinity, _NegativeInfinity],
    ) -> _Self:
        return _to_cxx(self).__add__(that)

    def __gt__(self: _Self, that: _Union[_Self, int]) -> bool:
        return _to_cxx(self).__gt__(that)

    def __ge__(self: _Self, that: _Union[_Self, int]) -> bool:
        return _to_cxx(self).__ge__(that)

    def __ne__(self: _Self, that: _Union[_Self, int]) -> bool:
        return _to_cxx(self).__ne__(that)

    def __eq__(self: _Self, that: _Union[_Self, int]) -> bool:
        return _to_cxx(self).__eq__(that)

    def __lt__(self: _Self, that: _Union[_Self, int]) -> bool:
        return _to_cxx(self).__lt__(that)

    def __le__(self: _Self, that: _Union[_Self, int]) -> bool:
        return _to_cxx(self).__le__(that)

    def __len__(self: _Self) -> int:
        return _to_cxx(self).__len__()

    def __pow__(self: _Self, e: int) -> _Self:
        return _to_cxx(self).__pow__(e)

    def __hash__(self: _Self) -> int:
        return _to_cxx(self).__hash__()


_copy_cxx_mem_fns(_NTPMat, Matrix)

########################################################################
# Helper functions
########################################################################

row_space_size = _wrap_cxx_free_fn(_row_space_size)
period = _wrap_cxx_free_fn(_period)
row_basis = _wrap_cxx_free_fn(_row_basis)
threshold = _wrap_cxx_free_fn(_threshold)
