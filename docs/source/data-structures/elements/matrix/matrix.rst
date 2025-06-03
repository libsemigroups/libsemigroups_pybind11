..
    Copyright (c) 2021-2024, J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

..
    TODO check layout is the same as other parts of the doc.

Matrix
======

This page contains the documentation for functionality in
``libsemigroups_pybind11`` for matrices.

Matrices over various semirings can be constructed using the function
:py:class:`Matrix`. :py:class:`Matrix` is a function that returns an instance of
one of a number of internal classes. These internal types are optimised in
various ways so that the underlying semiring operations are as fast as possible.

While :py:class:`Matrix` is not a class the objects returned by
:py:class:`Matrix` have identical methods, and so we document :py:class:`Matrix`
as if it was a class.

Some helper functions for :py:class:`Matrix` objects are documented in the
submodule ``matrix``.

.. toctree::
    :maxdepth: 1

    matrix-helpers

.. doctest::

    >>> from libsemigroups_pybind11 import Matrix, MatrixKind
    >>> x = Matrix(MatrixKind.Integer, [[2]])
    >>> x ** 64
    Matrix(MatrixKind.Integer, [[0]])
    >>> x = Matrix(MatrixKind.Integer, [[0, 1, 1], [1, 2, 3], [-1, 0, -1]])
    >>> x[0, 0]
    0
    >>> x[0, 1]
    1
    >>> x[1]
    [1, 2, 3]
    >>> x[0, 0] = 666
    >>> x
    Matrix(MatrixKind.Integer, [[666,   1,   1],
                                [  1,   2,   3],
                                [ -1,   0,  -1]])

    >>> x[0] = [0, 1, 1]
    >>> x
    Matrix(MatrixKind.Integer, [[ 0,  1,  1],
                                [ 1,  2,  3],
                                [-1,  0, -1]])
    >>> x += 1
    >>> x
    Matrix(MatrixKind.Integer, [[1, 2, 2],
                                [2, 3, 4],
                                [0, 1, 0]])
    >>> x *= 2
    >>> x
    Matrix(MatrixKind.Integer, [[2, 4, 4],
                                [4, 6, 8],
                                [0, 2, 0]])
    >>> x += x
    >>> x
    Matrix(MatrixKind.Integer, [[ 4,  8,  8],
                                [ 8, 12, 16],
                                [ 0,  4,  0]])
    >>> x + x
    Matrix(MatrixKind.Integer, [[ 8, 16, 16],
                                [16, 24, 32],
                                [ 0,  8,  0]])
    >>> x * x
    Matrix(MatrixKind.Integer, [[ 80, 160, 160],
                                [128, 272, 256],
                                [ 32,  48,  64]])
    >>> y = x.one()
    >>> y
    Matrix(MatrixKind.Integer, [[1, 0, 0],
                                [0, 1, 0],
                                [0, 0, 1]])
    >>> x.one(2)
    Matrix(MatrixKind.Integer, [[1, 0],
                                [0, 1]])
    >>> x.swap(y)
    >>> x
    Matrix(MatrixKind.Integer, [[1, 0, 0],
                                [0, 1, 0],
                                [0, 0, 1]])
    >>> y
    Matrix(MatrixKind.Integer, [[ 4,  8,  8],
                                [ 8, 12, 16],
                                [ 0,  4,  0]])
    >>> x.number_of_rows()
    3
    >>> x.number_of_cols()
    3
    >>> y = x.copy()
    >>> x is not y
    True
    >>> x == y
    True
    >>> x != y
    False
    >>> x < y
    False
    >>> x != y
    False
    >>> x > y
    False
    >>> x >= y
    True
    >>> x <= y
    True
    >>> x ** 10 == y
    True
    >>> len(x)
    3
    >>> list(x)
    [[1, 0, 0], [0, 1, 0], [0, 0, 1]]
    >>> x + 2 == 2 + x
    True
    >>> x * 2 == 2 * x
    True
    >>> import copy
    >>> z = copy.copy(y)
    >>> z *= 0
    >>> z
    Matrix(MatrixKind.Integer, [[0, 0, 0],
                                [0, 0, 0],
                                [0, 0, 0]])
    >>> z = Matrix(MatrixKind.Integer, 4, 4)
    >>> z
    Matrix(MatrixKind.Integer, [[0, 0, 0, 0],
                                [0, 0, 0, 0],
                                [0, 0, 0, 0],
                                [0, 0, 0, 0]])
    >>> d = {z: True}
    >>> z in d
    True

.. warning::

    The entries in a ``libsemigroups_pybind11`` matrix are stored internally as
    64-bit signed integers, and there are no checks that the multiplication does
    not overflow.

MatrixKind
----------

.. autoclass:: MatrixKind
    :show-inheritance:

    .. autoattribute:: Boolean

        For matrices over the Boolean semiring.

    .. autoattribute:: Integer

        For matrices over the usual ring of integers.

    .. autoattribute:: MaxPlus

        For matrices over the max-plus semiring.

    .. autoattribute:: MinPlus

        For matrices over the min-plus semiring.

    .. autoattribute:: ProjMaxPlus

        For projective matrices over the max-plus semiring.

    .. autoattribute:: MaxPlusTrunc

        For matrices over the truncated max-plus semiring.

    .. autoattribute:: MinPlusTrunc

        For matrices over the truncated min-plus semiring.

    .. autoattribute:: NTP

        For matrices over the semiring of natural numbers quotiented by `t = t +
        p`.

..
    TODO later summary

The Matrix class
----------------

.. py:class:: Matrix

    Instances of this class implement matrices over the semirings listed
    above in :any:`MatrixKind`.

    .. py:method:: __init__(self: Matrix, kind: MatrixKind, rows: list[list[int | POSITIVE_INFINITY | NEGATIVE_INFINITY]]) -> None
       :noindex:

       Construct a matrix from rows.

       :param kind: specifies the underlying semiring.
       :type kind: MatrixKind

       :param rows: the rows of the matrix.
       :type rows: list[list[int | POSITIVE_INFINITY | NEGATIVE_INFINITY]]

       :raise RunTimeError: if *kind* is
            :py:attr:`MatrixKind.MaxPlusTrunc`,
            :py:attr:`MatrixKind.MinPlusTrunc`, or
            :py:attr:`MatrixKind.NTP`.

       :raise LibsemigroupsError:
         if the entries in *rows* are not of equal length.

       :raise LibsemigroupsError:
         if any of the entries of the lists in *rows* do not belong to
         the underlying semiring.


    .. py:method:: __init__(self: Matrix, kind: MatrixKind, threshold: int, rows: list[list[int | POSITIVE_INFINITY | NEGATIVE_INFINITY]]) -> None
       :noindex:

       Construct a matrix from threshold and rows.

       :param kind: specifies the underlying semiring.
       :type kind: MatrixKind

       :param threshold: the threshold of the underlying semiring.
       :type threshold: int

       :param rows: the rows of the matrix.
       :type rows: list[list[int | POSITIVE_INFINITY | NEGATIVE_INFINITY]]

       :raise RunTimeError: if *kind* is not
            :py:attr:`MatrixKind.MaxPlusTrunc`, or
            :py:attr:`MatrixKind.MinPlusTrunc`.

       :raise LibsemigroupsError:
         if the entries in *rows* are not of equal length.

       :raise LibsemigroupsError:
         if any of the entries of the lists in *rows* do not belong to
         the underlying semiring.


    .. py:method:: __init__(self: Matrix, kind: MatrixKind, threshold: int, period: int, rows: list[list[int | POSITIVE_INFINITY | NEGATIVE_INFINITY]]) -> None
       :noindex:

       Construct a matrix from rows.

       :param kind: specifies the underlying semiring.
       :type kind: MatrixKind

       :param threshold: the threshold of the underlying semiring.
       :type threshold: int

       :param period: the period of the underlying semiring.
       :type period: int

       :param rows: the rows of the matrix.
       :type rows: list[list[int | POSITIVE_INFINITY | NEGATIVE_INFINITY]]

       :raise RunTimeError: if *kind* is not :py:attr:`MatrixKind.NTP`.

       :raise LibsemigroupsError:
         if the entries in *rows* are not of equal length.

       :raise LibsemigroupsError:
         if any of the entries of the lists in *rows* do not belong to
         the underlying semiring.


    .. py:method:: __init__(self: Matrix, kind: MatrixKind, r: int, c: int) -> None
       :noindex:

       Construct an uninitialized *r* by *c* matrix.

       :param kind: specifies the underlying semiring.
       :type kind: MatrixKind

       :param r: the number of rows in the matrix
       :type r: int

       :param c: the number of columns in the matrix
       :type c: int

       :raise RunTimeError: if *kind* is
            :py:attr:`MatrixKind.MaxPlusTrunc`,
            :py:attr:`MatrixKind.MinPlusTrunc`,
            or :py:attr:`MatrixKind.NTP`.

       .. doctest::

          >>> from libsemigroups_pybind11 import Matrix, MatrixKind
          >>> # construct a 2 x 3 boolean matrix
          >>> Matrix(MatrixKind.Boolean, 2, 3)
          Matrix(MatrixKind.Boolean, [[0, 0, 0],
                                      [0, 0, 0]])


    .. py:method:: __init__(self: Matrix, kind: MatrixKind, threshold: int, r: int, c: int) -> None
       :noindex:

       Construct an uninitialized `r` by `c` matrix.

       :param kind: specifies the underlying semiring.
       :type kind: MatrixKind

       :param threshold: the threshold of the underlying semiring.
       :type threshold: int

       :param r: the number of rows in the matrix
       :type r: int

       :param c: the number of columns in the matrix
       :type c: int

       :raise RunTimeError:
         if *kind* is not :py:attr:`MatrixKind.MaxPlusTrunc` or
         :py:attr:`MatrixKind.MinPlusTrunc`.

       .. doctest::

          >>> from libsemigroups_pybind11 import Matrix, MatrixKind
          >>> # construct a 2 x 3 max-plus truncated matrix
          >>> Matrix(MatrixKind.MaxPlusTrunc, 11, 2, 3)
          Matrix(MatrixKind.MaxPlusTrunc, 11, [[0, 0, 0],
                                               [0, 0, 0]])


    .. py:method:: __init__(self: Matrix, kind: MatrixKind, threshold: int, period: int, r: int, c: int) -> None
       :noindex:

       Construct an uninitialized `r` by `c` matrix.

       :param kind: specifies the underlying semiring.
       :type kind: MatrixKind

       :param threshold: the threshold of the underlying semiring.
       :type threshold: int

       :param period: the period of the underlying semiring.
       :type period: int

       :param r: the number of rows in the matrix.
       :type r: int

       :param c: the number of columns in the matrix.
       :type c: int

       :raise RunTimeError: if *kind* is not :py:attr:`MatrixKind.NTP`.

       .. doctest::

          >>> from libsemigroups_pybind11 import Matrix, MatrixKind
          >>> # construct a 2 x 3 ntp matrix
          >>> Matrix(MatrixKind.NTP, 5, 7, 2, 3)
          Matrix(MatrixKind.NTP, 5, 7, [[0, 0, 0],
                                        [0, 0, 0]])


    .. py:method:: number_of_cols(self: Matrix) -> int

       Returns the number of columns.

       :returns: The number of columns in the matrix.
       :rtype: int

       .. doctest::

          >>> from libsemigroups_pybind11 import Matrix, MatrixKind
          >>> x = Matrix(MatrixKind.Integer, [[0, 1], [1, 0]])
          >>> x.number_of_cols()
          2


    .. py:method:: number_of_rows(self: Matrix) -> int

       Returns the number of rows.

       :returns: The number of rows in the matrix.
       :rtype: int

       .. doctest::

          >>> from libsemigroups_pybind11 import Matrix, MatrixKind
          >>> x = Matrix(MatrixKind.Integer, [[0, 1], [1, 0]])
          >>> x.number_of_rows()
          2


    .. py:method:: one(self: Matrix, n: int) -> Matrix

       Construct the :math:`n \times n` identity matrix. The second argument
       is optional and if not specified then ``x.number_of_rows()`` is used).

       :param n: the dimension of the matrix (optional).
       :type n: int

                               :returns: An identity matrix.
       :rtype: Matrix


    .. py:method:: product_inplace(self: Matrix, x: Matrix, y: Matrix) -> None

       Multiply two matrices and stores the product in *self*.

       :param x: first matrix to multiply.
       :type x: Matrix
       :param y: second matrix to multiply.
       :type y: Matrix

       :raises LibsemigroupsError:
         if *x* and *y* are not square, or do not have the same number of rows.

       :raises RunTimeError:
         if *x* and *y* are not defined over the same semiring.


    .. py:method:: row(self: Matrix, i: int) -> Matrix

       Returns the specified row.

       :param i: the index of the row.
       :type i: int

       :returns: A :py:class:`Matrix`.

       :raises LibsemigroupsError:
         if *i* is greater than or equal to :any:`number_of_rows`.


    .. py:method:: rows(self: Matrix) -> list[Matrix]

       Returns a list of all rows of a matrix.

       :returns: A list of the rows.
       :rtype: list[Matrix]


    .. py:method:: scalar_one(self: Matrix) -> int

      Returns the multiplicative identity of the underlying semiring of a
      matrix.

      :returns: The multiplicative identity of the underlying semiring.
      :rtype: int

      .. doctest::

          >>> from libsemigroups_pybind11 import Matrix, MatrixKind
          >>> x = Matrix(MatrixKind.MinPlusTrunc, 11 ,[[0, 1, 1], [0] * 3, [1] * 3])
          >>> x.scalar_one()
          0


    .. py:method:: scalar_zero(self: Matrix) -> int | POSITIVE_INFINITY | NEGATIVE_INFINITY

       Returns the additive identity of the underlying semiring of a
       matrix.

       :returns: The additive identity of the underlying semiring.
       :rtype: int | POSITIVE_INFINITY | NEGATIVE_INFINITY

       .. doctest::

           >>> from libsemigroups_pybind11 import Matrix, MatrixKind, POSITIVE_INFINITY
           >>> x = Matrix(MatrixKind.MinPlusTrunc, 11 ,[[0, 1, 1], [0] * 3, [1] * 3])
           >>> x.scalar_zero() == POSITIVE_INFINITY
           True


    .. py:method:: swap(self: Matrix, that: Matrix) -> None

       Swaps the contents of *self* with the contents of *that*.

       :param that: the matrix to swap contents with
       :type that: Matrix


    .. py:method:: transpose(self: Matrix) -> None

       Transposes the matrix in-place.

       :raises LibsemigroupsError:
         if *self* is not a square matrix.
