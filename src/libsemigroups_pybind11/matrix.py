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

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module,unused-import
    BMat as _BMat,
    IntMat as _IntMat,
    MaxPlusMat as _MaxPlusMat,
    MaxPlusTruncMat as _MaxPlusTruncMat,
    MinPlusMat as _MinPlusMat,
    MinPlusTruncMat as _MinPlusTruncMat,
    NEGATIVE_INFINITY as _NEGATIVE_INFINITY,
    NTPMat as _NTPMat,
    NegativeInfinity as _NegativeInfinity,
    POSITIVE_INFINITY as _POSITIVE_INFINITY,
    PositiveInfinity as _PositiveInfinity,
    ProjMaxPlusMat as _ProjMaxPlusMat,
    matrix_row_space_size as row_space_size,
    matrix_period as period,
    matrix_row_basis as row_basis,
    matrix_threshold as threshold,
)


# the underscore prefix stops this from appearing in the doc of the
# "matrix" submodule
class _MatrixKind(_Enum):
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


_MatrixKindToCxxType = {
    _MatrixKind.Boolean: _BMat,
    _MatrixKind.Integer: _IntMat,
    _MatrixKind.MaxPlus: _MaxPlusMat,
    _MatrixKind.MinPlus: _MinPlusMat,
    _MatrixKind.ProjMaxPlus: _ProjMaxPlusMat,
    _MatrixKind.MaxPlusTrunc: _MaxPlusTruncMat,
    _MatrixKind.MinPlusTrunc: _MinPlusTruncMat,
    _MatrixKind.NTP: _NTPMat,
}


# the underscore prefix stops this from appearing in the doc of the
# "matrix" submodule.
# TODO could update to use kwargs for threshold and period
def _Matrix(kind: _MatrixKind, *args):  # pylint: disable=invalid-name
    """
    Documented in docs/source/elements/matrix/matrix.rst
    """
    if not isinstance(kind, _MatrixKind):
        raise TypeError("the 1st argument must be a _MatrixKind")
    return _MatrixKindToCxxType[kind](*args)
