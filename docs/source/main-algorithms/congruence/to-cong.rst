..
    Copyright (c) 2025 Joseph Edwards

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Converting to a Congruence
==========================

This page contains documentation relating to converting
``libsemigroups_pybind11`` objects into :any:`Congruence` instances using the
:any:`to` function.

.. seealso::

    :doc:`/data-structures/to-function` for an overview of possible conversions
    between ``libsemigroups_pybind11`` types.

Various uses
------------

Recall that the signature for the :any:`to` function is ``to(*args, Return)``.
In what follows, we explain how different values of *args* and *Return* may be
used to construct :any:`Congruence` objects. The following options are possible:

    - :ref:`froidure-pin-to-congruence`.

.. _froidure-pin-to-congruence:

Converting a :any:`FroidurePin` to a :any:`Congruence`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`Congruence` from a :any:`FroidurePin`, specify all of the
following values for *args*:

    - **knd** (:any:`congruence_kind`) -- the kind of the congruence being
      construed;
    - **fpb** (:any:`FroidurePin`) -- the :any:`FroidurePin` instance to be
      converted; and
    - **wg** (:any:`WordGraph`) -- the left or right Cayley graph of *fpb*.

Additionally, specify one of the following tuples for *Return*:

    - ``(Congruence, str)`` for constructing a :any:`Congruence` on words of
      type ``str``; or
    - ``(Congruence, list[int])`` for constructing a :any:`Congruence` on words
      of type ``list[int]``.

This function converts the :any:`FroidurePin` object *fpb* into a
:any:`Congruence` object using the :any:`WordGraph` *wg* (which should be either
the :any:`FroidurePin.left_cayley_graph` or the
:any:`FroidurePin.right_cayley_graph` of *fpb*).

This returned :any:`Congruence` object represents the trivial congruence over
the semigroup defined by *fpb*.

This will throw a :any:`LibsemigroupsError` if *wg* is not the
:any:`FroidurePin.left_cayley_graph` or the
:any:`FroidurePin.right_cayley_graph` of *fpb*.

.. doctest:: Python

    >>> from libsemigroups_pybind11 import (
    ...     Bipartition,
    ...     congruence_kind,
    ...     Congruence,
    ...     FroidurePin,
    ...     to,
    ... )

    >>> b1 = Bipartition([[1, -1], [2, -2], [3, -3], [4, -4]])
    >>> b2 = Bipartition([[1, -2], [2, -3], [3, -4], [4, -1]])
    >>> b3 = Bipartition([[1, -2], [2, -1], [3, -3], [4, -4]])
    >>> b4 = Bipartition([[1, 2], [3, -3], [4, -4], [-1, -2]])
    >>> S = FroidurePin(b1, b2, b3, b4)

    >>> cong = to(
    ...     congruence_kind.twosided,   # knd
    ...     S,                          # fpb
    ...     S.right_cayley_graph(),     # wg
    ...     rtype=(Congruence, str),
    ... )

    >>> cong.run()
    >>> S.size() == cong.number_of_classes()
    True
