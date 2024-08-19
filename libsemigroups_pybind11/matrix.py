# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name

"""
This package provides a the user-facing python part of libsemigroups_pybind11
relating to matrices.
"""

from enum import Enum

# TODO add underscores

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
)


class MatrixKind(Enum):
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


_Matrix = {
    MatrixKind.Boolean: _BMat,
    MatrixKind.Integer: _IntMat,
    MatrixKind.MaxPlus: _MaxPlusMat,
    MatrixKind.MinPlus: _MinPlusMat,
    MatrixKind.ProjMaxPlus: _ProjMaxPlusMat,
    MatrixKind.MaxPlusTrunc: _MaxPlusTruncMat,
    MatrixKind.MinPlusTrunc: _MinPlusTruncMat,
    MatrixKind.NTP: _NTPMat,
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


# TODO could update to use kwargs for threshold and period
def Matrix(kind: MatrixKind, *args):
    """
    Constructs a matrix, basically just delegates to
    _libsemigroups_pybind11
    """
    if not isinstance(kind, MatrixKind):
        raise TypeError("the 1st argument must be a MatrixKind")
    return _Matrix[kind](*_convert_matrix_args(*args))
