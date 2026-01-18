..
    Copyright (c) 2025 Joseph Edwards

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Converting to a Presentation
============================

This page contains documentation relating to converting
``libsemigroups_pybind11`` objects into :any:`Presentation` instances using the
:any:`to` function.

.. seealso::

    :doc:`/data-structures/to-function` for an overview of possible conversions
    between ``libsemigroups_pybind11`` types.

Various uses
------------

Recall that the signature for the :any:`to` function is ``to(*args, rtype)``.
In what follows, we explain how different values of *args* and *rtype* may be
used to construct :any:`Presentation` objects. The following options are
possible:

    - :ref:`presentation-to-presentation`;
    - :ref:`presentation-and-function-to-presentation`;
    - :ref:`knuth-bendix-to-presentation`;
    - :ref:`froidure-pin-to-presentation`;
    - :ref:`kambites-to-presentation`;
    - :ref:`todd-coxeter-to-presentation`;
    - :ref:`congruence-to-presentation`; and
    - :ref:`stephen-to-presentation`.

.. _presentation-to-presentation:

Converting a :any:`Presentation` to a :any:`Presentation`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`Presentation` from a :any:`Presentation`, specify the
following values for *args*:

    - **p** (:any:`Presentation`) -- the :any:`Presentation` to convert.

Additionally, specify one of the following for *rtype*:

    - ``(Presentation, str)`` for constructing a :any:`Presentation` over words
      of type ``str``.
    - ``(Presentation, list[int])`` for constructing a :any:`Presentation` over
      words of type ``list[int]``.

This function behaves in one of two ways, depending on type of words in *p*, and
the type of words specified in *rtype*:

    1. When the type of words in *p* and type of words specified in *rtype* are
       not the same, this function returns a :any:`Presentation` equivalent to
       the input :any:`Presentation` *p* but with words a different type (for
       example, can be used to convert from ``str`` to ``list[int]``).
    2. When the type of words in *p* and type of words specified in *rtype* are
       the same, this function just returns its argument *p*, and is included
       solely for the purpose of simplifying certain client code, where
       presentations must be converted from one type to another sometimes, but
       not other times.

If the alphabet of of *p* is :math:`\{a_0, a_1, \dots a_{n-1}\}`, where each
letter is of type ``str``, then the conversion from one type to another is
:math:`a_i \mapsto` ``human_readable_index(a_i)``. Conversely, if each letter is
of type ``list[int]``, then the conversion from one type to another is
:math:`a_i \mapsto` ``human_readable_letter(a_i)``.

This function throws a :any:`LibsemigroupsError` if the type of words in *p* is
not the same as that specified in *rtype*, and
``p.throw_if_bad_alphabet_or_rules()`` throws.

.. seealso::

    - :any:`words.human_readable_index`;
    - :any:`words.human_readable_letter`; and
    - :any:`Presentation.throw_if_bad_alphabet_or_rules`.

.. doctest:: Python

    >>> from libsemigroups_pybind11 import presentation, Presentation, to

    >>> p = Presentation('abcdef')
    >>> presentation.add_zero_rules(p, 'e')
    >>> p.rules
    ['ae', 'e', 'ea', 'e', 'be', 'e', 'eb', 'e', 'ce', 'e', 'ec', 'e', 'de', 'e', 'ed', 'e', 'ee', 'e', 'fe', 'e', 'ef', 'e']

    >>> p == to(p, rtype=(Presentation, str))
    True

    >>> q = to(p, rtype=(Presentation, list[int]))
    >>> q.alphabet()
    [0, 1, 2, 3, 4, 5]
    >>> q.rules
    [[0, 4], [4], [4, 0], [4], [1, 4], [4], [4, 1], [4], [2, 4], [4], [4, 2], [4], [3, 4], [4], [4, 3], [4], [4, 4], [4], [5, 4], [4], [4, 5], [4]]

    >>> p == to(q, rtype=(Presentation, str))
    True

.. _presentation-and-function-to-presentation:

Converting a :any:`Presentation` to a :any:`Presentation` with a function
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`Presentation` from a :any:`Presentation` using a custom
letter conversion function, specify the following values for *args*:

    - **p** (:any:`Presentation`) -- the :any:`Presentation` to convert.
    - **f** (``Callable[[str | int], int | str]``) -- the function used to
      convert between the different types of letters.

Additionally, specify one of the following for *rtype*:

    - ``(Presentation, str)`` for constructing a :any:`Presentation` over words
      of type ``str``.
    - ``(Presentation, list[int])`` for constructing a :any:`Presentation` over
      words of type ``list[int]``.

This function returns a :any:`Presentation` equivalent to the input
:any:`Presentation` *p* but over words with letters of a different type (for
example, can be used to convert from :any:`str` to :any:`int`). The second
parameter *f* specifies how to map the letters of one :any:`Presentation` to the
other.

This function throws a :any:`LibsemigroupsError` if
``p.throw_if_bad_alphabet_or_rules()`` throws, or if the function specified by
*f* does not map letters of the type used in *p* to letters of the type of word
specified in *rtype*.

.. seealso::

    - :any:`Presentation.throw_if_bad_alphabet_or_rules`.

.. doctest:: Python

    >>> from libsemigroups_pybind11 import presentation, Presentation, to

    >>> p = Presentation([0, 2, 4, 6])
    >>> presentation.add_zero_rules(p, 4)
    >>> p.rules
    [[0, 4], [4], [4, 0], [4], [2, 4], [4], [4, 2], [4], [4, 4], [4], [6, 4], [4], [4, 6], [4]]

    >>> double = lambda x: 2*x

    >>> q = to(
    ...    p,                               # p
    ...    double,                          # f
    ...    rtype=(Presentation, list[int])
    ... )
    >>> q.alphabet()
    [0, 4, 8, 12]
    >>> q.rules
    [[0, 8], [8], [8, 0], [8], [4, 8], [8], [8, 4], [8], [8, 8], [8], [12, 8], [8], [8, 12], [8]]

.. _knuth-bendix-to-presentation:

Converting a :any:`KnuthBendix` to a :any:`Presentation`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`Presentation` from a :any:`KnuthBendix`, specify the
following values for *args*:

    - **kb** (:any:`KnuthBendix`) -- the :any:`KnuthBendix` from which to obtain
      the rules.

Additionally, specify one of the following for *rtype*:

    - ``(Presentation,)`` for constructing a :any:`Presentation` over words of 
      the same type as that in *kb*.
    - ``(Presentation, str)`` for constructing a :any:`Presentation` over words
      of type ``str``.
    - ``(Presentation, list[int]`` for constructing a :any:`Presentation` over
      words of type ``list[int]``.

This function constructs and returns a :any:`Presentation` object using the
currently active rules of *kb*.

No enumeration of the argument *kb* is performed, so it might be the case that
the resulting :any:`Presentation` does not define the same semigroup/monoid as
*kb*. To ensure that the resulting :any:`Presentation` defines the same
semigroup/monoid as *kb*, run ``kb.run()`` (or any other function that fully
enumerates *kb*) prior to calling this function.

.. seealso::

    - :any:`KnuthBendix.active_rules`; and
    - :any:`Runner.run`.

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
    >>> p2 = to(kb, rtype=(Presentation,))
    >>> for p in [p1, p2]:
    ...     # rtypes whether any changes have been made
    ...     presentation.sort_each_rule(p)
    ...     presentation.sort_rules(p)
    True
    False
    >>> p1 == p2
    True

.. _froidure-pin-to-presentation:

Converting a :any:`FroidurePin` to a :any:`Presentation`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`Presentation` from a :any:`FroidurePin`, specify the
following values for *args*:

    - **fp** (:any:`FroidurePin`) -- the :any:`FroidurePin` from which to obtain
      the rules.

Additionally, specify the following for *rtype*:

    - ``(Presentation, str)`` for constructing a :any:`Presentation` over words
      of type ``str``.
    - ``(Presentation, list[int])`` for constructing a :any:`Presentation` over
      words of type ``list[int]``.

This function constructs and returns a :any:`Presentation` object using the
rules of a :any:`FroidurePin` object.

No enumeration of the argument *fp* is performed, so it might be the case that
the resulting :any:`Presentation` does not define the same semigroup as *fp*. To
ensure that the resulting :any:`Presentation` defines the same semigroup as
*fp*, run ``fp.run()`` (or any other function that fully enumerates *fp*) prior
to calling this function.

.. seealso::

    - :any:`froidure_pin.rules`; and
    - :any:`Runner.run`.

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

    >>> p = to(S, rtype=(Presentation, str))
    >>> len(p.alphabet()) == 4
    True

.. _kambites-to-presentation:

Converting a :any:`Kambites` to a :any:`Presentation`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`Presentation` from a :any:`Kambites`, specify the
following values for *args*:

    - **k** (:any:`Kambites`) -- the :any:`Kambites` from which to obtain
      the presentation.

Additionally, specify one of the following for *rtype*:

    - ``(Presentation,)`` for constructing a :any:`Presentation` over words of
      the same type as that in *k*.
    - ``(Presentation, str)`` for constructing a :any:`Presentation` over words
      of type ``str``.
    - ``(Presentation, list[int])`` for constructing a :any:`Presentation` over
      words of type ``list[int]``.

This function constructs and returns a :any:`Presentation` object that is
equivalent to the presentation used to construct the :any:`Kambites` object.
When the word type matches, this function returns a reference to ``k.presentation()``.
When the word types differ, a converted copy is returned.

No computation is performed by this function; it simply retrieves the
presentation that was used to define the :any:`Kambites` object.

.. seealso::

    - :any:`Kambites.presentation`.

.. doctest:: Python

    >>> from libsemigroups_pybind11 import (
    ...     congruence_kind,
    ...     Kambites,
    ...     Presentation,
    ...     presentation,
    ...     to,
    ... )

    >>> p = Presentation([5, 6, 7, 8, 9])
    >>> presentation.add_rule(p, [5, 6], [7])
    >>> presentation.add_rule(p, [6, 7], [8])
    >>> presentation.add_rule(p, [7, 8], [9])
    >>> presentation.add_rule(p, [8, 9], [5])
    >>> presentation.add_rule(p, [9, 5], [6])

    >>> k = Kambites(congruence_kind.twosided, p)
    >>> result = to(k, rtype=(Presentation, list[int]))
    >>> result == p
    True

    >>> result_str = to(k, rtype=(Presentation, str))
    >>> result_str.alphabet()
    'abcde'
    >>> result_str.rules
    ['ab', 'c', 'bc', 'd', 'cd', 'e', 'de', 'a', 'ea', 'b']

.. _todd-coxeter-to-presentation:

Converting a :any:`ToddCoxeter` to a :any:`Presentation`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`Presentation` from a :any:`ToddCoxeter`, specify the
following values for *args*:

    - **tc** (:any:`ToddCoxeter`) -- the :any:`ToddCoxeter` from which to obtain
      the presentation.

Additionally, specify one of the following for *rtype*:

    - ``(Presentation,)`` for constructing a :any:`Presentation` over words of
      the same type as that in *tc*.
    - ``(Presentation, str)`` for constructing a :any:`Presentation` over words
      of type ``str``.
    - ``(Presentation, list[int])`` for constructing a :any:`Presentation` over
      words of type ``list[int]``.

This function constructs and returns a :any:`Presentation` object that is
equivalent to the presentation used to construct the :any:`ToddCoxeter` object.
When the word type matches, this function returns a reference to ``tc.presentation()``.
When the word types differ, a converted copy is returned.

No computation is performed by this function; it simply retrieves the
presentation that was used to define the :any:`ToddCoxeter` object.

.. seealso::

    - :any:`ToddCoxeter.presentation`.

.. doctest:: Python

    >>> from libsemigroups_pybind11 import (
    ...     congruence_kind,
    ...     ToddCoxeter,
    ...     Presentation,
    ...     presentation,
    ...     to,
    ... )

    >>> p = Presentation([5, 6, 7, 8, 9])
    >>> presentation.add_rule(p, [5, 6], [7])
    >>> presentation.add_rule(p, [6, 7], [8])
    >>> presentation.add_rule(p, [7, 8], [9])
    >>> presentation.add_rule(p, [8, 9], [5])
    >>> presentation.add_rule(p, [9, 5], [6])

    >>> tc = ToddCoxeter(congruence_kind.twosided, p)
    >>> result = to(tc, rtype=(Presentation, list[int]))
    >>> result == p
    True

    >>> result_str = to(tc, rtype=(Presentation, str))
    >>> result_str.alphabet()
    'abcde'
    >>> result_str.rules
    ['ab', 'c', 'bc', 'd', 'cd', 'e', 'de', 'a', 'ea', 'b']

.. _congruence-to-presentation:

Converting a :any:`Congruence` to a :any:`Presentation`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`Presentation` from a :any:`Congruence`, specify the
following values for *args*:

    - **c** (:any:`Congruence`) -- the :any:`Congruence` from which to obtain
      the presentation.

Additionally, specify one of the following for *rtype*:

    - ``(Presentation,)`` for constructing a :any:`Presentation` over words of
      the same type as that in *c*.
    - ``(Presentation, str)`` for constructing a :any:`Presentation` over words
      of type ``str``.
    - ``(Presentation, list[int])`` for constructing a :any:`Presentation` over
      words of type ``list[int]``.

This function constructs and returns a :any:`Presentation` object that is
equivalent to the presentation used to construct the :any:`Congruence` object.
When the word type matches, this function returns a reference to ``c.presentation()``.
When the word types differ, a converted copy is returned.

No computation is performed by this function; it simply retrieves the
presentation that was used to define the :any:`Congruence` object. Note that
any generating pairs added via :any:`Congruence.add_generating_pair`
are not included in the returned presentation.

.. seealso::

    - :any:`Congruence.presentation`; and
    - :any:`Congruence.add_generating_pair`.

.. doctest:: Python

    >>> from libsemigroups_pybind11 import (
    ...     congruence_kind,
    ...     Congruence,
    ...     Presentation,
    ...     presentation,
    ...     to,
    ... )

    >>> p = Presentation("abc")
    >>> presentation.add_rule(p, "aa", "b")
    >>> presentation.add_rule(p, "bb", "c")
    >>> presentation.add_rule(p, "cc", "a")

    >>> c = Congruence(congruence_kind.twosided, p)
    >>> result = to(c, rtype=(Presentation, str))
    >>> result == p
    True

    >>> result_word = to(c, rtype=(Presentation, list[int]))
    >>> result_word.alphabet()
    [0, 1, 2]
    >>> result_word.rules
    [[0, 0], [1], [1, 1], [2], [2, 2], [0]]

.. _stephen-to-presentation:

Converting a :any:`Stephen` to a :any:`Presentation`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`Presentation` from a :any:`Stephen`, specify the
following values for *args*:

    - **s** (:any:`Stephen`) -- the :any:`Stephen` from which to obtain
      the presentation.

Additionally, specify one of the following for *rtype*:

    - ``(Presentation,)`` for constructing a :any:`Presentation` over words of
      the same type as that in *s*.
    - ``(Presentation, str)`` for constructing a :any:`Presentation` over words
      of type ``str``.
    - ``(Presentation, list[int])`` for constructing a :any:`Presentation` over
      words of type ``list[int]``.

This function constructs and returns a :any:`Presentation` object that is
equivalent to the presentation used to construct the :any:`Stephen` object.
A converted copy is always returned (even when word types match).

No computation is performed by this function; it simply retrieves the
presentation that was used to define the :any:`Stephen` object. Note that
:any:`Stephen` can be constructed from either a :any:`Presentation` or an
:any:`InversePresentation`; in both cases, this function returns a
:any:`Presentation`.

.. seealso::

    - :any:`Stephen.presentation`.

.. doctest:: Python

    >>> from libsemigroups_pybind11 import (
    ...     Stephen,
    ...     Presentation,
    ...     presentation,
    ...     to,
    ... )

    >>> p = Presentation([5, 6, 7, 8, 9])
    >>> presentation.add_rule(p, [5, 6], [7])
    >>> presentation.add_rule(p, [6, 7], [8])
    >>> presentation.add_rule(p, [7, 8], [9])
    >>> presentation.add_rule(p, [8, 9], [5])
    >>> presentation.add_rule(p, [9, 5], [6])

    >>> s = Stephen(p)
    >>> result = to(s, rtype=(Presentation, list[int]))
    >>> result == p
    True

    >>> result_str = to(s, rtype=(Presentation, str))
    >>> result_str.alphabet()
    'abcde'
    >>> result_str.rules
    ['ab', 'c', 'bc', 'd', 'cd', 'e', 'de', 'a', 'ea', 'b']
