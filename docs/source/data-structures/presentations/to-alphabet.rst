..
    Copyright (c) 2026 J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Converting to an Alphabet
=========================

This page contains documentation relating to converting
``libsemigroups_pybind11`` objects into :any:`Alphabet` instances using the
:any:`to` function.

.. seealso::

    :doc:`/data-structures/to-function` for an overview of possible conversions
    between ``libsemigroups_pybind11`` types.

Various uses
------------

Recall that the signature for the :any:`to` function is ``to(*args, rtype)``.
In what follows, we explain how different values of *args* and *rtype* may be
used to construct :any:`Alphabet` objects.

.. _alphabet-to-alphabet:

Converting an :any:`Alphabet` to an :any:`Alphabet`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct an :any:`Alphabet` from an :any:`Alphabet`, specify the following
values for *args*:

    - **alphabet** (:any:`Alphabet`) -- the :any:`Alphabet` to convert.

Additionally, specify one of the following for *rtype*:

    - ``(Alphabet, str)`` for constructing an :any:`Alphabet` over words of type
      ``str``.
    - ``(Alphabet, list[int])`` for constructing an :any:`Alphabet` over words
      of type ``list[int]``.

This function behaves in one of two ways, depending on the type of words in
*alphabet*, and the type of words specified in *rtype*:

    1. When the type of words in *alphabet* and type of words specified in
       *rtype* are not the same, this function returns an :any:`Alphabet`
       equivalent to the input :any:`Alphabet` *alphabet* but with words a
       different type (for example, can be used to convert from ``str`` to
       ``list[int]``).
    2. When the type of words in *alphabet* and type of words specified in
       *rtype* are the same, this function effectively just returns its argument
       *alphabet*, and is included solely for the purpose of simplifying certain
       client code, where alphabets must be converted from one type to another
       sometimes, but not other times.

If *alphabet* has letters :math:`\{a_0, a_1, \dots a_{n-1}\}`, where each
letter is of type ``str``, then the conversion from one type to another is
:math:`a_i \mapsto` ``human_readable_index(a_i)``. Conversely, if each letter is
of type ``int``, then the conversion from one type to another is
:math:`a_i \mapsto` ``human_readable_letter(a_i)``.

.. seealso::

    - :any:`words.human_readable_index`;
    - :any:`words.human_readable_letter`; and
    - :any:`Alphabet.throw_if_duplicate_letters`.

.. doctest:: Python

    >>> from libsemigroups_pybind11 import Alphabet, to

    >>> a = Alphabet("abcdef")
    >>> a == to(a, rtype=(Alphabet, str))
    True

    >>> b = to(a, rtype=(Alphabet, list[int]))
    >>> b.letters()
    [0, 1, 2, 3, 4, 5]

    >>> to(b, rtype=(Alphabet, str)) == a
    True
