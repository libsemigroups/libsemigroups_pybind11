.. Copyright (c) 2025 Joseph Edwards

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Converting to a KnuthBendixStringRewriteTrie
============================================

This page contains documentation relating to converting
``libsemigroups_pybind11`` objects into :any:`KnuthBendixStringRewriteTrie`
instances using the :any:`to` function.

.. seealso::

    :doc:`/data-structures/to-function` for an overview of possible conversions
    between ``libsemigroups_pybind11`` types.

Various uses
------------

Recall that the signature for the :any:`to` function is ``to(*args, Return)``.
In what follows, we explain how different values of *args* and *Return* may
be used to construct :any:`KnuthBendixStringRewriteTrie` objects. The following
options are possible:

  * :ref:`todd-coxeter-to-knuth-bendix-default`
  * :ref:`todd-coxeter-to-knuth-bendix`
  * :ref:`froidure-pin-to-knuth-bendix`

.. _todd-coxeter-to-knuth-bendix-default:

Converting a :any:`ToddCoxeterWord` to a :any:`KnuthBendixStringRewriteTrie` (default rewriter)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`KnuthBendixStringRewriteTrie` from a
:any:`ToddCoxeterWord` using the default rewriter, specify all of the following
values for *args*:

    * **knd** (:any:`congruence_kind`) -- the kind of the congruence being
      constructed.
    * **tc** (:any:`ToddCoxeterWord`) -- the :any:`ToddCoxeterWord` object
      being converted.

Additionally, specify the following for *Return*:

    * ``KnuthBendix`` for constructing a :any:`KnuthBendixStringRewriteTrie`
      with the default rewriter.

This function converts a :any:`ToddCoxeterWord` object *tc* to a
:any:`KnuthBendixStringRewriteTrie` object using
:any:`ToddCoxeterWord.presentation`. This is equivalent to specifying
``(KnuthBendix, "RewriteTrie")`` as described below.

This returned :any:`KnuthBendixStringRewriteTrie` object represents the trivial
congruence over the semigroup defined by *tc*.

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

    >>> tc = ToddCoxeter(congruence_kind.twosided, p)

    >>> kb = to(
    ...     congruence_kind.twosided,   # knd
    ...     tc,                         # tc
    ...     Return=KnuthBendix
    ... )
    >>> kb.run()

    >>> kb.number_of_classes() == tc.number_of_classes()
    True

.. _todd-coxeter-to-knuth-bendix:

Converting a :any:`ToddCoxeterWord` to a :any:`KnuthBendixStringRewriteTrie`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`KnuthBendixStringRewriteTrie` from a
:any:`ToddCoxeterWord`, specify all of the following values for *args*:

    * **knd** (:any:`congruence_kind`) -- the kind of the congruence being
      constructed.
    * **tc** (:any:`ToddCoxeterWord`) -- the :any:`ToddCoxeterWord` object
      being converted.

Additionally, specify one of the following for *Return*:

    * ``(KnuthBendix, "RewriteTrie")`` for constructing a
      :any:`KnuthBendixStringRewriteTrie` with the the ``RewriteTrie"``
      rewriter.
    * ``(KnuthBendix, "RewriteFromLeft")`` for constructing a
      :any:`KnuthBendixStringRewriteTrie` with the the ``RewriteFromLeft"``
      rewriter.

This function converts a :any:`ToddCoxeterWord` object *tc* to a
:any:`KnuthBendixStringRewriteTrie` object with the rewriter as specified above,
using :any:`ToddCoxeterWord.presentation`.

This returned :any:`KnuthBendixStringRewriteTrie` object represents the trivial
congruence over the semigroup defined by *tc*.

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

    >>> tc = ToddCoxeter(congruence_kind.twosided, p)

    >>> kb = to(
    ...     congruence_kind.twosided,               # knd
    ...     tc,                                     # tc
    ...     Return=(KnuthBendix, "RewriteFromLeft")
    ... )
    >>> kb.run()

    >>> kb.number_of_classes() == tc.number_of_classes()
    True

.. _froidure-pin-to-knuth-bendix:

Converting a :any:`FroidurePinPBR` to a :any:`KnuthBendixStringRewriteTrie`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`KnuthBendixStringRewriteTrie` from a
:any:`FroidurePinPBR`, specify all of the following values for *args*:

    * **knd** (:any:`congruence_kind`) -- the kind of the congruence being
      constructed.
    * **fpb** (:any:`FroidurePinBase`) -- the :any:`FroidurePinPBR` object
      being converted.

Additionally, specify one of the following for *Return*:

    * ``(KnuthBendix, str, "RewriteTrie")`` for constructing a
      :any:`KnuthBendixStringRewriteTrie` on words with type ``str`` using the
      ``RewriteTrie"`` rewriter.
    * ``(KnuthBendix, List[int], "RewriteTrie")`` for constructing a
      :any:`KnuthBendixStringRewriteTrie` on words with type ``List[int]`` using
      the ``RewriteTrie"`` rewriter.
    * ``(KnuthBendix, str, "RewriteFromLeft")`` for constructing a
      :any:`KnuthBendixStringRewriteTrie` on words with type ``str`` using the
      ``RewriteFromLeft"`` rewriter.
    * ``(KnuthBendix, List[int], "RewriteFromLeft")`` for constructing a
      :any:`KnuthBendixStringRewriteTrie` on words with type ``List[int]`` using
      the ``RewriteFromLeft"`` rewriter.


This function converts a :any:`FroidurePinPBR` object *fpb* to a
:any:`KnuthBendixStringRewriteTrie` object with the word type and rewriter as
specified above. This is done using the presentation obtained from
``to(fpb, Return=(Presentation, Word)`` where ``Word`` is either ``str`` or
``List[int]``.

This returned :any:`KnuthBendixStringRewriteTrie` object represents the trivial
congruence over the semigroup defined by *fpb*.

.. doctest:: Python

    >>> from typing import List 

    >>> from libsemigroups_pybind11 import (
    ...     Bipartition,
    ...     congruence_kind,
    ...     FroidurePin,
    ...     to,
    ...     KnuthBendix,
    ...     Presentation,
    ...     presentation,
    ... )

    >>> b1 = Bipartition([[1, -1], [2, -2], [3, -3], [4, -4]])
    >>> b2 = Bipartition([[1, -2], [2, -3], [3, -4], [4, -1]])
    >>> b3 = Bipartition([[1, -2], [2, -1], [3, -3], [4, -4]])
    >>> b4 = Bipartition([[1, 2], [3, -3], [4, -4], [-1, -2]])
    >>> S = FroidurePin(b1, b2, b3, b4)
    
    >>> kb = to(
    ...     congruence_kind.twosided,                           # knd
    ...     S,                                                  # tc
    ...     Return=(KnuthBendix, List[int], "RewriteFromLeft")
    ... )
    >>> kb.run()

    >>> kb.number_of_classes() == S.size()
    True
