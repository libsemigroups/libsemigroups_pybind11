# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name, protected-access, unused-import

"""
This package provides a the user-facing python part of
``libsemigroups_pybind11`` relating to matrices.
"""

from enum import Enum as _Enum
from typing import Union, List

from _libsemigroups_pybind11 import (
    NEGATIVE_INFINITY as _NEGATIVE_INFINITY,
    POSITIVE_INFINITY as _POSITIVE_INFINITY,
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
    row_basis as _row_basis,
)


# the underscore prefix stops this from appearing in the doc of the
# "matrix" submodule
class _MatrixKind(_Enum):
    """
    Documented in docs/source/elements/matrix/matrix.rst
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
    # Convert POSITIVE_INFINITY and NEGATIVE_INFINITY to integers
    if len(args) == 0 or not isinstance(args[-1], list):
        return args
    return (
        *args[:-1],
        [
            [
                (
                    z.to_int()
                    if isinstance(z, (_PositiveInfinity, _NegativeInfinity))
                    else z
                )
                for z in y
            ]
            for y in args[-1]
        ],
    )


def _convert_cpp_entry_to_py(
    val: int,
) -> Union[int, _PositiveInfinity, _NegativeInfinity]:
    # Convert from integers to _POSITIVE_INFINITY and _NEGATIVE_INFINITY

    if val == _POSITIVE_INFINITY:
        return _POSITIVE_INFINITY
    if val == _NEGATIVE_INFINITY:
        return _NEGATIVE_INFINITY
    return val


def _convert_cpp_row_to_py(
    row: List[int],
) -> List[Union[int, _PositiveInfinity, _NegativeInfinity]]:
    for i, val in enumerate(row):
        row[i] = _convert_cpp_entry_to_py(val)
    return row


def _convert_cpp_rows_to_py(
    rows: List[int],
) -> List[List[Union[int, _PositiveInfinity, _NegativeInfinity]]]:
    for i, val in enumerate(rows):
        rows[i] = _convert_cpp_row_to_py(val)
    return rows


def _at(self, arg):
    if isinstance(arg, tuple) and len(arg) == 2:
        return _convert_cpp_entry_to_py(self._at(arg))
    if isinstance(arg, int) and arg >= 0:
        return _convert_cpp_row_to_py(self._at(arg))
    raise NotImplementedError


def _scalar_zero(self) -> Union[int, _PositiveInfinity, _NegativeInfinity]:
    return _convert_cpp_entry_to_py(self._scalar_zero())


def row_basis(x):
    """
    Returns a row space basis of a matrix as a list of lists. The matrix *x* which
    must be one of:

    * :any:`MatrixKind.Boolean`
    * :any:`MatrixKind.MaxPlusTrunc`

    This function returns a row space basis of the matrix *x* as a list of lists
    of rows.

    :param x: the matrix.
    :type x: Matrix

    :complexity:
      :math:`O(r ^ 2 c)` where :math:`r` is the number of rows in ``x``
      and :math:`c` is the number of columns in ``x``.

    :returns: A basis for the row space of *x*.
    :rtype: List[List[int | POSITIVE_INFINITY | NEGATIVE_INFINITY]]
    """
    return _convert_cpp_rows_to_py(_row_basis(x))


for Mat in _MatrixKindToCppType.values():
    Mat.__getitem__ = _at
    Mat.scalar_zero = _scalar_zero


# the underscore prefix stops this from appearing in the doc of the
# "matrix" submodule.
# TODO could update to use kwargs for threshold and period
def _Matrix(kind: _MatrixKind, *args):
    """
    Documented in docs/source/elements/matrix/matrix.rst
    """
    if not isinstance(kind, _MatrixKind):
        raise TypeError("the 1st argument must be a _MatrixKind")
    return _MatrixKindToCppType[kind](*_convert_matrix_args(*args))
