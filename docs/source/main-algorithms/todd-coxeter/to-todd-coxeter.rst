.. Copyright (c) 2025 Joseph Edwards

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Converting to a ToddCoxeterWord
===============================

This page contains documentation relating to converting
``libsemigroups_pybind11`` objects into :any:`ToddCoxeterWord` instances using
the :any:`to` function.

.. seealso::

    :doc:`/data-structures/to-function` for an overview of possible conversions
    between ``libsemigroups_pybind11`` types.

Various uses
------------

Recall that the signature for the :any:`to` function is ``to(*args, Return)``.
In what follows, we explain how different values of *args* and *Return* may
be used to construct :any:`ToddCoxeterWord` objects. The following options are
possible:

    * :ref:`froidure-pin-to-todd-coxeter`
    * :ref:`knuth-bendix-to-todd-coxeter`

.. _froidure-pin-to-todd-coxeter:

Converting a :any:`FroidurePinPBR` to a :any:`ToddCoxeterWord`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`ToddCoxeterWord` from a :any:`FroidurePinPBR`, specify all
of the following values for *args*:

    * **knd** (:any:`congruence_kind`) -- the kind of the congruence being
      construed;
    * **fpb** (:any:`FroidurePinBase`) -- the :any:`FroidurePinPBR` instance to
      be converted; and
    * **wg** (:any:`WordGraph`) -- the left or right Cayley graph of *fpb*.

Additionally, specify one of the following for *Return*:

    * ``(ToddCoxeter, str)`` for constructing a :any:`ToddCoxeterWord` on words
      with type ``str``.
    * ``(ToddCoxeter, List[int])`` for constructing a :any:`ToddCoxeterWord` on
      words with type ``List[int]``.

This function converts the :any:`FroidurePinPBR` object *fpb* into a
:any:`ToddCoxeterWord` object using the :any:`WordGraph` *wg* (which should be
either the :any:`FroidurePinBase.left_cayley_graph` or the
:any:`FroidurePinBase.right_cayley_graph` of *fpb*).

This returned :any:`ToddCoxeterWord` object represents the trivial congruence
over the semigroup defined by *fpb*.

This will throw a :any:`LibsemigroupsError` if *wg* is not the
:any:`FroidurePinBase.left_cayley_graph` or the
:any:`FroidurePinBase.right_cayley_graph` of *fpb*.

.. doctest:: Python

    >>> from libsemigroups_pybind11 import (
    ...     Bipartition,
    ...     congruence_kind,
    ...     FroidurePin,
    ...     to,
    ...     ToddCoxeter,
    ... )

    >>> b1 = Bipartition([[1, -1], [2, -2], [3, -3], [4, -4]])
    >>> b2 = Bipartition([[1, -2], [2, -3], [3, -4], [4, -1]])
    >>> b3 = Bipartition([[1, -2], [2, -1], [3, -3], [4, -4]])
    >>> b4 = Bipartition([[1, 2], [3, -3], [4, -4], [-1, -2]])
    >>> S = FroidurePin(b1, b2, b3, b4)

    >>> tc = to(
    ...     congruence_kind.twosided,   # knd
    ...     S,                          # fpb
    ...     S.right_cayley_graph(),     # wg
    ...     Return=(ToddCoxeter, str),
    ... )

    >>> tc.run()
    >>> S.size() == tc.number_of_classes()
    True

.. _knuth-bendix-to-todd-coxeter:

Converting a :any:`KnuthBendixStringRewriteTrie` to a :any:`ToddCoxeterWord`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`ToddCoxeterWord` from a
:any:`KnuthBendixStringRewriteTrie` specify all of the following values for
*args*:

    * **knd** (:any:`congruence_kind`) -- the kind of the congruence being
      constructed.
    * **kb** (:any:`KnuthBendixStringRewriteTrie`) -- the
      :any:`KnuthBendixStringRewriteTrie` object being converted.

Additionally, specify the following for *Return*:

    * ``ToddCoxeter`` for constructing a :any:`ToddCoxeterWord`.

This function converts the :any:`KnuthBendixStringRewriteTrie` object *kb* into
a :any:`ToddCoxeterWord` object using the right Cayley graph of the semigroup
represented by *kb*.

This returned :any:`ToddCoxeterWord` object represents the trivial
congruence over the semigroup defined by *kb*.

This will throw a :any:`LibsemigroupsError` if either:

    * ``kb.kind()`` is not ``congruence_kind.twosided``; or
    * ``kb.number_of_classes()`` is not finite. In this case, use 
      ``ToddCoxeter(knd, kb.presentation())`` instead.

.. doctest:: Python

    >>> from libsemigroups_pybind11 import (
    ...     congruence_kind,
    ...     to,
    ...     KnuthBendix,
    ...     Presentation,
    ...     presentation,
    ...     ToddCoxeter,
    ... )

    >>> p = Presentation("ab")
    >>> presentation.add_rule(p, "ab", "ba")
    >>> presentation.add_rule(p, "aa", "a")
    >>> presentation.add_rule(p, "bb", "b")

    >>> kb = KnuthBendix(congruence_kind.twosided, p)

    >>> tc = to(
    ...     congruence_kind.twosided,   # knd
    ...     kb,                         # kb
    ...     Return=ToddCoxeter
    ... )
    >>> tc.run()

    >>> tc.number_of_classes() == kb.number_of_classes()
    True
