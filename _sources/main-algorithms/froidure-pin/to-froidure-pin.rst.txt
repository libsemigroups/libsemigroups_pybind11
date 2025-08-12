..
    Copyright (c) 2025 Joseph Edwards

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Converting to a FroidurePin
===========================

This page contains documentation relating to converting
``libsemigroups_pybind11`` objects into :any:`FroidurePin` instances using the
:any:`to` function.

.. seealso::

    :doc:`/data-structures/to-function` for an overview of possible conversions
    between ``libsemigroups_pybind11`` types.

Various uses
------------

Recall that the signature for the :any:`to` function is ``to(*args, Return)``.
In what follows, we explain how different values of *args* and *Return* may be
used to construct :any:`FroidurePin` objects. The following options are
possible:

    - :ref:`congruence-to-froidure-pin`;
    - :ref:`kambites-to-froidure-pin`;
    - :ref:`knuth-bendix-to-froidure-pin`;
    - :ref:`todd-coxeter-to-froidure-pin`; and
    - :ref:`word-graph-to-froidure-pin`.

.. _congruence-to-froidure-pin:

Converting a :any:`Congruence` to a :any:`FroidurePin`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`FroidurePin` from a :any:`Congruence`, specify the
following values for *args*:

    - **cong** (:any:`Congruence`) -- the :any:`Congruence` object being
      converted.

Additionally, specify the following for *Return*:

    - ``(FroidurePin,)`` for constructing a :any:`FroidurePin`.

The :any:`FroidurePin` object returned is isomorphic to the quotient of the
underlying semigroup or monoid of *cong* by the congruence represented by
*cong*.

This function throws a :any:`LibsemigroupsError` if ``cong.kind`` is not
``congruence_kind.twosided``.

.. doctest:: Python

    >>> from libsemigroups_pybind11 import (
    ...     congruence_kind,
    ...     Congruence,
    ...     FroidurePin,
    ...     Presentation,
    ...     presentation,
    ...     to,
    ... )

    >>> p = Presentation([0, 1])
    >>> presentation.add_rule(p, [0, 1], [1, 0])
    >>> presentation.add_rule(p, [0, 0], [0])
    >>> presentation.add_rule(p, [1, 1], [1])

    >>> cong = Congruence(congruence_kind.twosided, p)
    >>> fp = to(cong, rtype=(FroidurePin,))
    >>> fp.run()

    >>> fp.size() == cong.number_of_classes()
    True

.. _kambites-to-froidure-pin:

Converting a :any:`Kambites` to a :any:`FroidurePin`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`FroidurePin` from a :any:`Kambites`, specify the following
values for *args*:

    - **k** (:any:`Kambites`) -- the :any:`Kambites` object being converted.

Additionally, specify the following for *Return*:

    - ``(FroidurePin,)`` for constructing a :any:`FroidurePin`.

The :any:`FroidurePin` object returned is isomorphic to the quotient semigroup
or monoid represented by *k*.

This function throws a :any:`LibsemigroupsError` if the
:any:`Kambites.small_overlap_class` of *k* is not at least :math:`4`.

.. warning::

    The returned :any:`FroidurePin` instance is always infinite, and so any
    calls to any member functions that that trigger a full enumeration will
    never terminate (or they will when your computer kills the process because
    it has run out of memory).

.. doctest:: Python

    >>> from libsemigroups_pybind11 import (
    ...     congruence_kind,
    ...     FroidurePin,
    ...     Kambites,
    ...     Presentation,
    ...     presentation,
    ...     to,
    ... )

    >>> p = Presentation('abcdefg')
    >>> presentation.add_rule(p, 'abcd', 'aaaeaa')
    >>> presentation.add_rule(p, 'ef', 'dg')

    >>> k = Kambites(congruence_kind.twosided, p)
    >>> fp = to(k, rtype=(FroidurePin,))
    >>> fp.enumerate(100)

    >>> fp.current_size() == 8205
    True

.. _knuth-bendix-to-froidure-pin:

Converting a :any:`KnuthBendix` to a :any:`FroidurePin`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`FroidurePin` from a :any:`KnuthBendix`, specify the
following values for *args*:

    - **kb** (:any:`KnuthBendix`) -- the :any:`KnuthBendix` object being
      converted.

Additionally, specify the following for *Return*:

    - ``(FroidurePin,)`` for constructing a :any:`FroidurePin`.

The :any:`FroidurePin` object returned is isomorphic to the quotient semigroup
or monoid represented by *kb*.

This function throws a :any:`LibsemigroupsError` if ``kb.kind()`` is not
``congruence.twosided``.

.. doctest:: Python

    >>> from libsemigroups_pybind11 import (
    ...     congruence_kind,
    ...     FroidurePin,
    ...     KnuthBendix,
    ...     Presentation,
    ...     presentation,
    ...     to,
    ... )

    >>> p = Presentation([0, 1])
    >>> presentation.add_rule(p, [0, 1], [1, 0])
    >>> presentation.add_rule(p, [0, 0], [0])
    >>> presentation.add_rule(p, [1, 1], [1])

    >>> kb = KnuthBendix(congruence_kind.twosided, p)
    >>> fp = to(kb, rtype=(FroidurePin,))
    >>> fp.run()

    >>> fp.size() == kb.number_of_classes()
    True

.. _todd-coxeter-to-froidure-pin:

Converting a :any:`ToddCoxeter` to a :any:`FroidurePin`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`FroidurePin` from a :any:`ToddCoxeter`, specify the
following values for *args*:

    - **tc** (:any:`ToddCoxeter`) -- the :any:`ToddCoxeter` object being
      converted.

Additionally, specify the following for *Return*:

    - ``(FroidurePin,)`` for constructing a :any:`FroidurePin`.

The :any:`FroidurePin` object returned is isomorphic to the quotient semigroup
or monoid represented by *tc*.

This function throws a :any:`LibsemigroupsError` if ``tc.kind()`` is not
``congruence.twosided``.

.. doctest:: Python

    >>> from libsemigroups_pybind11 import (
    ...     congruence_kind,
    ...     FroidurePin,
    ...     Presentation,
    ...     presentation,
    ...     to,
    ...     ToddCoxeter,
    ... )

    >>> p = Presentation([0, 1])
    >>> presentation.add_rule(p, [0, 1], [1, 0])
    >>> presentation.add_rule(p, [0, 0], [0])
    >>> presentation.add_rule(p, [1, 1], [1])

    >>> tc = ToddCoxeter(congruence_kind.twosided, p)
    >>> fp = to(tc, rtype=(FroidurePin,))
    >>> fp.run()

    >>> fp.size() == tc.number_of_classes()
    True

.. _word-graph-to-froidure-pin:

Converting a :any:`WordGraph` to a :any:`FroidurePin`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`FroidurePin` from a :any:`WordGraph`, specify the following
values for *args*:

    - **wg** (:any:`WordGraph`) -- the :any:`WordGraph` object being used to
      construct the :any:`FroidurePin` object.

or

    - **wg** (:any:`WordGraph`) -- the :any:`WordGraph` object being used to
      construct the :any:`FroidurePin` object.
    - **first** (:any:`int`) -- the value of :math:`a` in the following
      discussion.
    - **last** (:any:`int`) -- the value of :math:`b` in the following
      discussion.

Additionally, specify the following for *Return*:

    - ``(FroidurePin,)`` for constructing a :any:`FroidurePin`.

Each label ``n`` in the :any:`WordGraph` will correspond to a generator ``f`` in
the output :any:`FroidurePin` such that ``f[s] = t`` whenever there is an edge
from ``s`` to ``t`` in *wg* labelled ``n``.

More precisely, if :math:`a` and :math:`b` are the parameters *first* (or ``0``
if not specified) and *last* (or ``wg.number_of_nodes()`` if not specified),
respectively, :math:`m` is the number of nodes in the WordGraph *wg*, :math:`0
\leq a, b< m`, and :math:`n` is an edge label, then we define :math:`f: \{a,
\ldots, b - 1\} \to \{0, \ldots, m - 1\}` so that :math:`(x)f` equals the target
of the edge starting at node :math:`x` with label :math:`n`. In this way, every
edge label in a :any:`WordGraph` corresponds to a transformation of the nodes of
the digraph. If :math:`\{a, \ldots, b - 1\}f \subseteq \{a, \ldots, b - 1\}`,
then :math:`f` is a transformation in the sense of :any:`Transf`. Assuming that
for every edge label of the :any:`WordGraph` the corresponding :math:`f`
satisfies :math:`\{a, \ldots, b - 1\}f \subseteq \{a, \ldots, b - 1\}`, then
this function returns the :any:`FroidurePin` object corresponding to the
semigroup or monoid generated by the set of all such transformations.

This function throws a :any:`LibsemigroupsError` if
``throw_if_image_value_out_of_range`` throws for any of the constructed
generators. This can happen if, for example, the :any:`WordGraph` is not
complete (i.e. there exists an edge label and node for which there is no edge
with the given label and given source) or if there is an edge label such that
:math:`\{a, \ldots, b - 1\}f \not\subseteq \{a, \ldots, b - 1\}` for the
corresponding :math:`f`.

.. doctest:: Python

    >>> from libsemigroups_pybind11 import (
    ...     congruence_kind,
    ...     FroidurePin,
    ...     to,
    ...     WordGraph,
    ... )

    >>> w = WordGraph(3, 1)
    >>> w.target(0, 0, 1)
    <WordGraph with 3 nodes, 1 edges, & out-degree 1>
    >>> w.target(1, 0, 1)
    <WordGraph with 3 nodes, 2 edges, & out-degree 1>
    >>> w.target(2, 0, 1)
    <WordGraph with 3 nodes, 3 edges, & out-degree 1>

    >>> fp = to(
    ...     w,                  # wg
    ...     1,                  # first
    ...     2,                  # last
    ...     rtype=(FroidurePin,)
    ... )
    >>> fp.run()

    >>> fp.number_of_rules() == 1
    True
