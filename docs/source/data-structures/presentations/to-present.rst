.. Copyright (c) 2025 Joseph Edwards

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Converting to a PresentationStrings
===================================

This page contains documentation relating to converting
``libsemigroups_pybind11`` objects into :any:`PresentationStrings` instances
using the :any:`to` function.

.. seealso::

    :doc:`/data-structures/to-function` for an overview of possible conversions
    between ``libsemigroups_pybind11`` types.

Various uses
------------

Recall that the signature for the :any:`to` function is ``to(*args, Return)``.
In what follows, we explain how different values of *args* and *Return* may
be used to construct :any:`PresentationStrings` objects. The following options
are possible:

    * :ref:`presentation-to-presentation`;
    * :ref:`presentation-and-function-to-presentation`;
    * :ref:`knuth-bendix-to-presentation`; and
    * :ref:`froidure-pin-to-presentation`.

.. _presentation-to-presentation:

Converting a :any:`PresentationStrings` to a :any:`PresentationStrings`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`PresentationStrings` from a :any:`PresentationStrings`,
specify the following values for *args*:

    * **p** (:any:`PresentationStrings`) -- the :any:`PresentationStrings` to
      convert.

Additionally, specify one of the following for *Return*:

    * ``(Presentation, str)`` for constructing a :any:`PresentationStrings` over
      words of type ``str``.
    * ``(Presentation, List[int])`` for constructing a
      :any:`PresentationStrings` over words of type ``List[int]``.

This function behaves in one of two ways, depending on type of words in *p*, and
the type of words specified in *Return*:

    #. When the type of words in *p* and type of words specified in *Return* are
       not the same, this function returns a :any:`PresentationStrings`
       equivalent to the input :any:`PresentationStrings` *p* but with words a
       different type (for example, can be used to convert from ``str`` to
       ``List[int]``).
    #. When the type of words in *p* and type of words specified in *Return* are
       the same, this function just returns its argument *p*, and is included
       solely for the purpose of simplifying certain client code, where
       presentations must be converted from one type to another sometimes, but
       not other times.

If the alphabet of of *p* is :math:`\{a_0, a_1, \dots a_{n-1}\}`, where each
letter is of type ``str``, then the conversion from one type to another is
:math:`a_i \mapsto` ``human_readable_index(a_i)``. Conversely, if each letter is
of type ``List[int]``, then the conversion from one type to another is
:math:`a_i \mapsto` ``human_readable_letter(a_i)``.

This function throws a :any:`LibsemigroupsError` if the type of words in *p* is
not the same as that specified in *Return*, and ``p.throw_if_bad_alphabet_or_rules()`` throws.

.. seealso::

    * :any:`words.human_readable_index`;
    * :any:`words.human_readable_letter`; and
    * :any:`PresentationStrings.throw_if_bad_alphabet_or_rules`.

.. doctest:: Python

    >>> from typing import List
    >>> from libsemigroups_pybind11 import presentation, Presentation, to

    >>> p = Presentation('abcdef')
    >>> presentation.add_zero_rules(p, 'e')
    >>> p.rules
    ['ae', 'e', 'ea', 'e', 'be', 'e', 'eb', 'e', 'ce', 'e', 'ec', 'e', 'de', 'e', 'ed', 'e', 'ee', 'e', 'fe', 'e', 'ef', 'e']

    >>> p == to(p, Return=(Presentation, str))
    True

    >>> q = to(p, Return=(Presentation, List[int]))
    >>> q.alphabet()
    [0, 1, 2, 3, 4, 5]
    >>> q.rules
    [[0, 4], [4], [4, 0], [4], [1, 4], [4], [4, 1], [4], [2, 4], [4], [4, 2], [4], [3, 4], [4], [4, 3], [4], [4, 4], [4], [5, 4], [4], [4, 5], [4]]

    >>> p == to(q, Return=(Presentation, str))
    True

.. _presentation-and-function-to-presentation:

Converting a :any:`PresentationStrings` to a :any:`PresentationStrings` with a function
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`PresentationStrings` from a :any:`PresentationStrings`
using a custom letter conversion function, specify the following values for
*args*:

    * **p** (:any:`PresentationStrings`) -- the :any:`PresentationStrings` to
      convert.
    * **f** (``Callable[[str | int], int | str]``) -- the function used to
      convert between the different types of letters.

Additionally, specify one of the following for *Return*:

    * ``(Presentation, str)`` for constructing a :any:`PresentationStrings` over
      words of type ``str``.
    * ``(Presentation, List[int])`` for constructing a
      :any:`PresentationStrings` over words of type ``List[int]``.

This function returns a :any:`PresentationStrings` equivalent to the input
:any:`PresentationStrings` *p* but over words with letters of a different type
(for example, can be used to convert from :any:`str` to :any:`int`). The second
parameter *f* specifies how to map the letters of one :any:`PresentationStrings`
to the other.

This function throws a :any:`LibsemigroupsError` if ``p.throw_if_bad_alphabet_or_rules()`` throws, or
if the function specified by *f* does not map letters of the type used in *p* to
letters of the type of word specified in *Return*.

.. seealso::

    * :any:`PresentationStrings.throw_if_bad_alphabet_or_rules`.

.. doctest:: Python

    >>> from typing import List
    >>> from libsemigroups_pybind11 import presentation, Presentation, to

    >>> p = Presentation([0, 2, 4, 6])
    >>> presentation.add_zero_rules(p, 4)
    >>> p.rules
    [[0, 4], [4], [4, 0], [4], [2, 4], [4], [4, 2], [4], [4, 4], [4], [6, 4], [4], [4, 6], [4]]

    >>> double = lambda x: 2*x

    >>> q = to(
    ...    p,                               # p
    ...    double,                          # f
    ...    Return=(Presentation, List[int])
    ... )
    >>> q.alphabet()
    [0, 4, 8, 12]
    >>> q.rules
    [[0, 8], [8], [8, 0], [8], [4, 8], [8], [8, 4], [8], [8, 8], [8], [12, 8], [8], [8, 12], [8]]

.. _knuth-bendix-to-presentation:

Converting a :any:`KnuthBendixStringRewriteTrie` to a :any:`PresentationStrings`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`PresentationStrings` from a
:any:`KnuthBendixStringRewriteTrie`, specify the following values for
*args*:

    * **kb** (:any:`KnuthBendixStringRewriteTrie`) -- the
      :any:`KnuthBendixStringRewriteTrie` from which to obtain the rules.

Additionally, specify the following for *Return*:

    * ``Presentation`` for constructing a :any:`PresentationStrings` over
      words of the same type as that in *kb*.

This function constructs and returns a :any:`PresentationStrings` object using
the currently active rules of *kb*.

No enumeration of the argument *kb* is performed, so it might be the case that
the resulting :any:`PresentationStrings` does not define the same
semigroup/monoid as *kb*. To ensure that the resulting
:any:`PresentationStrings` defines the same semigroup/monoid as *kb*, run
``kb.run()`` (or any other function that fully enumerates *kb*) prior to calling
this function.

.. seealso::

    * :any:`KnuthBendixStringRewriteTrie.active_rules`; and
    * :any:`Runner.run`.

.. doctest:: Python

    >>> from libsemigroups_pybind11 import (
    ...     congruence_kind,
    ...     KnuthBendix,
    ...     Presentation,
    ...     presentation,
    ...     to,
    ... )

    >>> p1 = Presentation([0, 1])
    >>> presentation.add_rule(p1, [0, 1], [1, 0])
    >>> presentation.add_rule(p1, [0, 0], [0])
    >>> presentation.add_rule(p1, [1, 1], [1])

    >>> kb = KnuthBendix(congruence_kind.twosided, p1)
    >>> kb.run()
    >>> p2 = to(kb, Return=Presentation)
    >>> for p in [p1, p2]:
    ...     presentation.sort_each_rule(p)
    ...     presentation.sort_rules(p)

    >>> p1 == p2
    True

.. _froidure-pin-to-presentation:

Converting a :any:`FroidurePinPBR` to a :any:`PresentationStrings`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`PresentationStrings` from a
:any:`FroidurePinPBR`, specify the following values for
*args*:

    * **fp** (:any:`FroidurePinPBR`) -- the :any:`FroidurePinPBR` from which to
      obtain the rules.

Additionally, specify the following for *Return*:

    * ``(Presentation, str)`` for constructing a :any:`PresentationStrings` over
      words of type ``str``.
    * ``(Presentation, List[int])`` for constructing a
      :any:`PresentationStrings` over words of type ``List[int]``.

This function constructs and returns a :any:`PresentationStrings` object using
the rules of a :any:`FroidurePinPBR` object.

No enumeration of the argument *fp* is performed, so it might be the case that
the resulting :any:`PresentationStrings` does not define the same semigroup as
*fp*. To ensure that the resulting :any:`PresentationStrings` defines the same
semigroup as *fp*, run ``fp.run()`` (or any other function that fully enumerates
*fp*) prior to calling this function.

.. seealso::

    * :any:`froidure_pin.rules`; and
    * :any:`Runner.run`.

.. doctest:: Python

    >>> from libsemigroups_pybind11 import (
    ...     Bipartition,
    ...     FroidurePin,
    ...     Presentation,
    ...     presentation,
    ...     to,
    ... )

    >>> b1 = Bipartition([[1, -1], [2, -2], [3, -3], [4, -4]])
    >>> b2 = Bipartition([[1, -2], [2, -3], [3, -4], [4, -1]])
    >>> b3 = Bipartition([[1, -2], [2, -1], [3, -3], [4, -4]])
    >>> b4 = Bipartition([[1, 2], [3, -3], [4, -4], [-1, -2]])
    >>> S = FroidurePin(b1, b2, b3, b4)

    >>> p = to(S, Return=(Presentation, str))
    >>> len(p.alphabet()) == 4
    True

.. py:class:: Thing(arg)
