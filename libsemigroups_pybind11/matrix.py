# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name

"""
This package provides a the user-facing python part of
``libsemigroups_pybind11`` relating to matrices.
"""

# TODO:
# intercept the return values of
# * operator[x,y];
# * scalar_zero/one
# * operator[i]
# * row_basis?
# so that they returns POSITIVE_INFINITY or NEGATIVE_INFINITY if appropriate

from enum import Enum as _Enum

from _libsemigroups_pybind11 import (
    PositiveInfinity as _PositiveInfinity,
    NegativeInfinity as _NegativeInfinity,
    BMat as _BMat,
    IntMat as _IntMat,
    MaxPlusMat as _MaxPlusMat,
    MinPlusMat as _MinPlusMat,
    ProjMaxPlusMat as _ProjMaxPlusMat,
    MaxPlusTruncMat as _MaxPlusTruncMat,
    MinPlusTruncMat as _MinPlusTruncMat,
    NTPMat as _NTPMat,
    threshold,
    period,
    matrix_row_space_size as row_space_size,
    row_basis,
)


# the underscore prefix stops this from appearing in the doc of the
# "matrix" submodule
class _MatrixKind(_Enum):
    """
    This class is used as the argument to :py:class:`Matrix` to distinguish
    which semiring the matrix should be over.
    """

    Boolean = 0
    Integer = 1
    MaxPlus = 2
    MinPlus = 3
    ProjMaxPlus = 4
    MaxPlusTrunc = 5
    MinPlusTrunc = 6
    NTP = 7


_MatrixKindToCppType = {
    _MatrixKind.Boolean: _BMat,
    _MatrixKind.Integer: _IntMat,
    _MatrixKind.MaxPlus: _MaxPlusMat,
    _MatrixKind.MinPlus: _MinPlusMat,
    _MatrixKind.ProjMaxPlus: _ProjMaxPlusMat,
    _MatrixKind.MaxPlusTrunc: _MaxPlusTruncMat,
    _MatrixKind.MinPlusTrunc: _MinPlusTruncMat,
    _MatrixKind.NTP: _NTPMat,
}


def _convert_matrix_args(*args):
    if not (len(args) == 1 and isinstance(*args, list)):
        return args
    # Convert POSITIVE_INFINITY and NEGATIVE_INFINITY to integers
    return (
        [
            [
                (
                    z.to_int()
                    if isinstance(z, (_PositiveInfinity, _NegativeInfinity))
                    else z
                )
                for z in y
            ]
            for y in args[0]
        ],
    )


# the underscore prefix stops this from appearing in the doc of the
# "matrix" submodule
# TODO could update to use kwargs for threshold and period
def _Matrix(kind: _MatrixKind, *args):
    """
    Constructs a matrix, basically just delegates to
    _libsemigroups_pybind11
    """
    if not isinstance(kind, _MatrixKind):
        raise TypeError("the 1st argument must be a _MatrixKind")
    return _MatrixKindToCppType[kind](*_convert_matrix_args(*args))
