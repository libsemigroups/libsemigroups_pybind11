.. Copyright (c) 2021, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Matrix
======

This page contains the documentation for functionality in
``libsemigroups_pybind11`` for matrices.

Matrices over various semirings can be constructed using the function
:py:class:`Matrix`.  :py:class:`Matrix` is a function that returns an instance
of one of a number of internal classes. These internal types are optimised in
various ways so that the underlying semiring operations are as fast as
possible.

While :py:class:`Matrix` is not a class the objects returned by
:py:class:`Matrix` have identical methods, and so we document
:py:class:`Matrix` as if it was a class.

MatrixKind
~~~~~~~~~~

.. autoclass:: MatrixKind
   :show-inheritance:

   .. autoattribute:: Boolean

      For matrices over the Boolean semiring.

   .. autoattribute:: Integer

      For matrices over the usual ring of integers.

   .. autoattribute:: MaxPlus

      For matrices over the max-plus semiring of integers.

   .. autoattribute:: MinPlus

      For matrices over the min-plus semiring of integers.

   .. autoattribute:: ProjMaxPlus

      For projective matrices over the max-plus semiring of integers.

   .. autoattribute:: MaxPlusTrunc

      For matrices over the truncated max-plus semiring of integers.

   .. autoattribute:: MinPlusTrunc

      For matrices over the truncated min-plus semiring of integers.

   .. autoattribute:: NTP

      For matrices over the semiring of natural numbers quotiented by
      ``t = t + p``.

The Matrix class
~~~~~~~~~~~~~~~~

.. py:class:: Matrix

      Instances of this class implement matrices.

      .. py:method:: __init__(self: Matrix, x: Matrix) -> None

         Copy a matrix.

         :param x: the matrix to copy.
         :type x: Matrix

      .. py:method:: __init__(self: Matrix, kind: MatrixKind, rows: List[List[int]]) -> None
         :noindex:

         Construct a matrix from rows.

         :param kind: specifies the underlying semiring.
         :type kind: MatrixKind
         :param rows: the rows of the matrix.
         :type rows: List[List[int]]

         :raise RunTimeError: if ``kind`` is
              :py:attr:`MatrixKind.MaxPlusTrunc`,
              :py:attr:`MatrixKind.MinPlusTrunc`, or
              :py:attr:`MatrixKind.NTP`.

      .. py:method:: __init__(self: Matrix, kind: MatrixKind, threshold: int, rows: List[List[int]]) -> None
         :noindex:

         Construct a matrix from rows.

         :param kind: specifies the underlying semiring.
         :type kind: MatrixKind
         :param threshold: the threshold of the underlying semiring.
         :type threshold: int
         :param rows: the rows of the matrix.
         :type rows: List[List[int]]

         .. seealso:: :py:func:`make`

         :raise RunTimeError: if ``kind`` is not
              :py:attr:`MatrixKind.MaxPlusTrunc`, or
              :py:attr:`MatrixKind.MinPlusTrunc`.

      .. py:method:: __init__(self: Matrix, kind: MatrixKind, threshold: int, period: int, rows: List[List[int]]) -> None
         :noindex:

         Construct a matrix from rows.

         :param kind: specifies the underlying semiring.
         :type kind: MatrixKind
         :param threshold: the threshold of the underlying semiring.
         :type threshold: int
         :param period: the period of the underlying semiring.
         :type period: int
         :param rows: the rows of the matrix.
         :type rows: List[List[int]]

         :raise RunTimeError: if ``kind`` is not :py:attr:`MatrixKind.NTP`.

         .. seealso:: :py:func:`make`

      .. py:method:: __init__(self: Matrix, kind: MatrixKind, r: int, c: int) -> None
         :noindex:

         Construct an uninitialized ``r`` by ``c`` matrix.

         :param kind: specifies the underlying semiring.
         :type kind: MatrixKind
         :param r: the number of rows in the matrix
         :type r: int
         :param c: the number of columns in the matrix
         :type c: int

         .. code-block:: cpp

            // construct a 2 x 3 max-plus truncated matrix
            m = Matrix(MatrixKind.MaxPlusTrunc, 11, 2, 3);

         :raise RunTimeError: if ``kind`` is
              :py:attr:`MatrixKind.MaxPlusTrunc`,
              :py:attr:`MatrixKind.MinPlusTrunc`,
              or :py:attr:`MatrixKind.NTP`.

      .. py:method:: __init__(self: Matrix, kind: MatrixKind, threshold: int, r: int, c: int) -> None
         :noindex:

         Construct an uninitialized ``r`` by ``c`` matrix.

         :param kind: specifies the underlying semiring.
         :type kind: MatrixKind
         :param threshold: the threshold of the underlying semiring.
         :type threshold: int
         :param r: the number of rows in the matrix
         :type r: int
         :param c: the number of columns in the matrix
         :type c: int

         .. code-block:: cpp

            // construct a 2 x 3 max-plus truncated matrix
            m = Matrix(MatrixKind.MaxPlusTrunc, 11, 2, 3);

         :raise RunTimeError: if ``kind`` is not :py:attr:`MatrixKind.NTP`.

      .. py:method:: __init__(self: Matrix, kind: MatrixKind, threshold: int, period: int, r: int, c: int) -> None
         :noindex:

         Construct an uninitialized ``r`` by ``c`` matrix.

         :param kind: specifies the underlying semiring.
         :type kind: MatrixKind
         :param threshold: the threshold of the underlying semiring.
         :type threshold: int
         :param period: the period of the underlying semiring.
         :type period: int
         :param r: the number of rows in the matrix
         :type r: int
         :param c: the number of columns in the matrix
         :type c: int

         .. code-block:: cpp

            // construct a 2 x 3 ntp matrix
            m = Matrix(MatrixKind.NTP, 5, 7, 2, 3);

         :raise RunTimeError: if ``kind`` is not :py:attr:`MatrixKind.NTP`.

      .. py:method:: __eq__(self: Matrix, that: Matrix) -> bool

         Equality comparison.

         Returns ``True`` if ``self`` equals ``that``.

         :param that: the matrix for comparison.
         :type that: Matrix

         :returns: A ``bool``.


      .. py:method:: __getitem__(self: Matrix, tup: Tuple) -> int

         Returns value in a given position.

         :param tup: a tuple consisting of the row and column indices.
         :type tup: tuple

         :return: The value in the ``tup``-th position.

      .. py:method:: __iadd__(self: Matrix, that: Matrix) -> Matrix

         In-place matrix addition.

         This method modifies ``self`` so that it contains the sum ``self +
         that``.

         :param that: the matrix to add to ``self``.
         :type that: Matrix

      .. py:method:: __imul__(self: Matrix, a: int) -> Matrix

         In-place scalar multiplication.

         This method modifies ``self`` so that it contains every entry is
         multiplied by the scalar ``a``.

         :param a: the scalar to multiply by.
         :type a: int


      .. py:method:: __lt__(self: Matrix, that: Matrix) -> bool

         Less than comparison.

         Returns ``True`` if ``self`` is less than ``that``.

         :param that: the matrix for comparison.
         :type that: Matrix

         :returns: A ``bool``.

      .. py:method:: __mul__(self: Matrix, that: Matrix) -> Matrix

         Matrix multiplication.

         :param that: the matrix to multiply by.
         :type that: Matrix

         :returns: A new matrix containing the product of ``self`` and ``that``.

         .. seealso:: :py:meth:`product_inplace`.

      .. py:method:: number_of_rows(self: Matrix) -> int

         Returns the number of rows.

         :Parameters: None
         :returns: An integer.

         .. code-block:: python

            from libsemigroups_pybind11 import Matrix, MatrixKind
            x = Matrix(MatrixKind.Integer, [[0, 1], [1, 0]])
            x.number_of_rows() # returns 2

      .. py:method:: number_of_cols(self: Matrix) -> int

         Returns the number of columns.

         :Parameters: None
         :returns: An integer.

         .. code-block:: python

            from libsemigroups_pybind11 import Matrix, MatrixKind
            x = Matrix(MatrixKind.Integer, [[0, 1], [1, 0]])
            x.number_of_cols() # returns 2

      .. py:method:: one(self: Matrix) -> int

        Returns the one of the underlying semiring.

        :parameters: None
        :returns: An ``int``.

      .. py:method:: product_inplace(self: Matrix, x: Matrix, y: Matrix) -> List[Matrix]

         Multiply two matrices and stores the product in ``self``.

         :param x: matrix to multiply.
         :type x: Matrix
         :param y: matrix to multiply.
         :type y: Matrix

         :return: None

      .. py:method:: row(self: Matrix, i: int) -> Matrix

         Returns the specified row.

         :param i: the index of the row.

         :returns: A :py:class:`Matrix`.


      .. py:method:: rows(self: Matrix) -> List[Matrix]

         Returns a list of all rows.

         :Parameters: None
         :returns: A list of the rows.

      .. py:method:: swap(self: Matrix, that: Matrix) -> None

         Swaps the contents of ``self`` with the contents of ``that``.

         :param that: the matrix to swap contents with
         :type that: Matrix

         :returns: (None)

      .. py:method:: transpose(self: Matrix) -> None

         Transposes the matrix in-place.

         :parameters: None
         :returns: None

         .. warning:: This only works for square matrices.

Additional functionality
~~~~~~~~~~~~~~~~~~~~~~~~

.. py:function:: make_identity(kind: MatrixKind, n: int) -> Matrix

    Construct the :math:`n \times n` identity matrix.

    :param kind: specifies the underlying semiring.
    :type kind: MatrixKind
    :param n: the dimension
    :type n: int

    :returns: The :math:`n \times n` identity matrix.

    :raise RunTimeError: if ``kind`` is
         :py:attr:`MatrixKind.MaxPlusTrunc`,
         :py:attr:`MatrixKind.MinPlusTrunc`, or
         :py:attr:`MatrixKind.NTP`.

.. py:function:: make_identity(kind: MatrixKind, threshold: int, n: int) -> Matrix
    :noindex:

    Construct the :math:`n \times n` identity matrix.

    :param kind: specifies the underlying semiring.
    :type kind: MatrixKind
    :param threshold: the threshold of the underlying semiring.
    :type threshold: int
    :param n: the dimension
    :type n: int

    :returns: The :math:`n \times n` identity matrix.

    :raise RunTimeError: if ``kind`` is not
         :py:attr:`MatrixKind.MaxPlusTrunc` or
         :py:attr:`MatrixKind.MinPlusTrunc`.


.. py:function:: make_identity(kind: MatrixKind, threshold: int, period: int, n: int) -> Matrix
    :noindex:

    Construct the :math:`n \times n` identity matrix.

    :param kind: specifies the underlying semiring.
    :type kind: MatrixKind
    :param threshold: the threshold of the underlying semiring.
    :type threshold: int
    :param period: the period of the underlying semiring.
    :type period: int
    :param n: the dimension
    :type n: int

    :returns: The :math:`n \times n` identity matrix.

    :raise RunTimeError: if ``kind`` is not :py:attr:`MatrixKind.NTP`.


.. py:function:: make(kind: MatrixKind, rows: List[List[int]]) -> Matrix

    Construct and validate a matrix from rows.

    :param kind: specifies the underlying semiring.
    :type kind: MatrixKind
    :param rows: the rows of the matrix.
    :type rows: List[List[int]]

    :returns: A matrix.

    :raise RunTimeError: if ``kind`` is
         :py:attr:`MatrixKind.MaxPlusTrunc`,
         :py:attr:`MatrixKind.MinPlusTrunc`, or
         :py:attr:`MatrixKind.NTP`.

    :raises RunTimeError: if ``rows`` contains any invalid values.


.. py:function:: make(kind: MatrixKind, threshold: int, rows: List[List[int]]) -> Matrix
    :noindex:

    Construct and validate a matrix from rows.

    :param kind: specifies the underlying semiring.
    :type kind: MatrixKind
    :param threshold: the threshold of the underlying semiring.
    :type threshold: int
    :param rows: the rows of the matrix.
    :type rows: List[List[int]]

    :returns: A matrix.

    :raise RunTimeError: if ``kind`` is not
         :py:attr:`MatrixKind.MaxPlusTrunc` or
         :py:attr:`MatrixKind.MinPlusTrunc`.

    :raises RunTimeError: if ``rows`` contains any invalid values.


.. py:function:: make(kind: MatrixKind, threshold: int, period: int, rows: List[List[int]]) -> Matrix
    :noindex:

    Construct and validate a matrix from rows.

    :param kind: specifies the underlying semiring.
    :type kind: MatrixKind
    :param threshold: the threshold of the underlying semiring.
    :type threshold: int
    :param period: the period of the underlying semiring.
    :type period: int
    :param rows: the rows of the matrix.
    :type rows: List[List[int]]

    :returns: A matrix.

    :raise RunTimeError: if ``kind`` is not :py:attr:`MatrixKind.NTP`.

    :raises RunTimeError: if ``rows`` contains any invalid values.
