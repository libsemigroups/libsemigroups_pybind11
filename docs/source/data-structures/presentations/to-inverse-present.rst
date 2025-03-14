.. Copyright (c) 2025 Joseph Edwards

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Converting to an InversePresentationStrings
===========================================

This page contains documentation relating to converting
``libsemigroups_pybind11`` objects into :any:`InversePresentationStrings`
instances using the :any:`to` function.

.. seealso::

    :doc:`/data-structures/to-function` for an overview of possible conversions
    between ``libsemigroups_pybind11`` types.

Various uses
------------

Recall that the signature for the :any:`to` function is ``to(*args, Return)``.
In what follows, we explain how different values of *args* and *Return* may
be used to construct :any:`InversePresentationStrings` objects. The following
options are possible:

    * :ref:`presentation-to-inverse-presentation`;
    * :ref:`inverse-presentation-to-inverse-presentation`; and
    * :ref:`inverse-presentation-and-function-to-inverse-presentation`.

.. _presentation-to-inverse-presentation:

Converting a :any:`PresentationStrings` to an :any:`InversePresentationStrings`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct an :any:`InversePresentationStrings` from a
:any:`PresentationStrings`, specify the following values for *args*:

    * **p** (:any:`PresentationStrings`) -- the :any:`PresentationStrings` to
      convert.

Additionally, specify the following for *Return*:

    * ``InversePresentation`` for constructing an
      :any:`InversePresentationStrings` over words of the same type as those in
      *p*.

This function returns an :any:`InversePresentationStrings` with rules equivalent
to those of the input :any:`PresentationStrings` *p*, but over a normalised
alphabet. If the alphabet of *p* is :math:`\{a_0, a_1, \dots, a_{n-1}\}`, then
the alphabet of the returned :any:`InversePresentationStrings` will be
:math:`\{0, 1, \dots, n-1, n, \dots, 2n-1\}`, where the inverse of letter
:math:`i` is the letter :math:`i + n\, (\text{mod }2n)`.

This function throws a :any:`LibsemigroupsError` if
``p.throw_if_bad_alphabet_or_rules()`` throws.

.. seealso::

    * :any:`PresentationStrings.throw_if_bad_alphabet_or_rules`.

.. doctest:: Python

    >>> from libsemigroups_pybind11 import (
    ...     InversePresentation,
    ...     presentation,
    ...     Presentation,
    ...     to,
    ... )

    >>> p = Presentation('abc')
    >>> presentation.add_rule(p, 'aaa', 'b')
    >>> presentation.add_rule(p, 'bac', 'cab')

    >>> ip = to(p, Return=InversePresentation)
    >>> ip.alphabet()
    'abcdef'
    >>> ip.inverses()
    'defabc'
    >>> ip.rules == p.rules
    True

.. _inverse-presentation-to-inverse-presentation:

Converting an :any:`InversePresentationStrings` to an :any:`InversePresentationStrings`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct an :any:`InversePresentationStrings` from an
:any:`InversePresentationStrings`, specify the following values for *args*:

    * **ip** (:any:`InversePresentationStrings`) -- the
      :any:`InversePresentationStrings` to convert.

Additionally, specify one of the following for *Return*:

    * ``(InversePresentation, str)`` for constructing an
    * :any:`InversePresentationStrings` over words of type ``str``.
    * ``(InversePresentation, List[int])`` for constructing an
      :any:`InversePresentationStrings` over words of type ``List[int]``.

This function behaves in one of two ways, depending on type of words in *p*, and
the type of words specified in *Return*:

    #. When the type of words in *ip* and type of words specified in *Return*
       are not the same, this function returns an
       :any:`InversePresentationStrings` equivalent to the input
       :any:`InversePresentationStrings` *ip* but with words a different type
       (for example, can be used to convert from ``str`` to ``List[int]``).
    #. When the type of words in *ip* and type of words specified in *Return*
       are the same, this function just returns its argument *ip*, and is
       included solely for the purpose of simplifying certain client code, where
       objects of type :any:`InversePresentationStrings` must be converted from
       one type to another sometimes, but not other times.

If the alphabet of of *ip* is :math:`\{a_0, a_1, \dots a_{n-1}\}`, where each
letter is of type ``str``, then the conversion from one type to another is
:math:`a_i \mapsto` ``human_readable_index(a_i)``. Conversely, if each letter is
of type ``List[int]``, then the conversion from one type to another is
:math:`a_i \mapsto` ``human_readable_letter(a_i)``.

This function throws a :any:`LibsemigroupsError` if the type of words in *ip* is
not the same as that specified in *Return* and
``p.throw_if_bad_alphabet_rules_or_inverses()`` throws.

.. seealso::

    * :any:`words.human_readable_index`;
    * :any:`words.human_readable_letter`; and
    * :any:`InversePresentationStrings.throw_if_bad_alphabet_rules_or_inverses`.

.. doctest:: Python

    >>> from typing import List
    >>> from libsemigroups_pybind11 import presentation, Presentation, to

    >>> ip = InversePresentation('abc')
    >>> ip.inverses('cba')
    <inverse semigroup presentation with 3 letters, 0 rules, and length 0>
    >>> presentation.add_rule(ip, 'aaa', 'b')
    >>> presentation.add_rule(ip, 'bac', 'cab')
        
    >>> ip == to(ip, Return=(InversePresentation, str))
    True

    >>> iq = to(ip, Return=(InversePresentation, List[int]))
    >>> iq.alphabet()
    [0, 1, 2]
    >>> iq.inverses()
    [2, 1, 0]
    >>> iq.rules
    [[0, 0, 0], [1], [1, 0, 2], [2, 0, 1]]

.. _inverse-presentation-and-function-to-inverse-presentation:

Converting an :any:`InversePresentationStrings` to n :any:`InversePresentationStrings` with a function
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`InversePresentationStrings` from a
:any:`InversePresentationStrings` using a custom letter conversion function,
specify the following values for *args*:

    * **ip** (:any:`InversePresentationStrings`) -- the
      :any:`InversePresentationStrings` to convert.
    * **f** (``Callable[[str | int], int | str]``) -- the function used to
      convert between the different types of letters.

Additionally, specify one of the following for *Return*:

    * ``(InversePresentation, str)`` for constructing an
      :any:`InversePresentationStrings` over words of type ``str``.
    * ``(InversePresentation, List[int])`` for constructing a
      :any:`InversePresentationStrings` over words of type ``List[int]``.

This function returns an :any:`InversePresentationStrings` equivalent to the
input :any:`InversePresentationStrings` *ip* but over words with letters of a
different type (for example, can be used to convert from :any:`str` to
:any:`int`). The second parameter *f* specifies how to map the letters of one
:any:`InversePresentationStrings` to the other.

This function throws a :any:`LibsemigroupsError` if
``ip.throw_if_bad_alphabet_rules_or_inverses()`` throws, or if the function
specified by *f* does not map letters of the type used in *ip* to letters of
the type of word specified in *Return*.

.. seealso::

    * :any:`InversePresentationStrings.throw_if_bad_alphabet_rules_or_inverses`.

.. doctest:: Python

    >>> from libsemigroups_pybind11 import (
    ...     InversePresentation,
    ...     presentation,
    ...     Presentation,
    ...     to,
    ... )

    >>> ip = InversePresentation('abc')
    >>> ip.inverses('cba')
    <inverse semigroup presentation with 3 letters, 0 rules, and length 0>
    >>> presentation.add_rule(ip, 'aaa', 'b')
    >>> presentation.add_rule(ip, 'bac', 'cab')

    >>> iq = to(
    ...     ip,                                 # ip
    ...     lambda x: chr(ord(x) + 11),         # f
    ...     Return=(InversePresentation, str)
    ... )
    >>> iq.alphabet()
    'lmn'
    >>> iq.inverses()
    'nml'
    >>> iq.rules
    ['lll', 'm', 'mln', 'nlm']
