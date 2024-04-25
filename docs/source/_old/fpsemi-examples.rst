.. Copyright (c) 2022-2024 M. T. Whyte

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Examples
========

This page contains the documentation for examples of finitely presented semigroups.

.. py:class:: author

   The values in this enum class are used to specify the authors of a
   presentation. Where there are different presentations by different
   authors, values of this type can be passed as an argument to the functions
   described on this page to disambiguate which presentation is wanted.

   The author values can be combined via the operator ``+``.

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:func:`symmetric_group`
     - A presentation for the symmetric group.

   * - :py:func:`alternating_group`
     - A presentation for the alternating group.

   * - :py:func:`full_transformation_monoid`
     - A presentation for the full transformation monoid.

   * - :py:func:`partial_transformation_monoid`
     - A presentation for the partial transformation monoid.

   * - :py:func:`symmetric_inverse_monoid`
     - A presentation for the symmetric inverse monoid.

   * - :py:func:`dual_symmetric_inverse_monoid`
     - A presentation for the dual symmetric inverse monoid.

   * - :py:func:`uniform_block_bijection_monoid`
     - A presentation for the uniform block bijection monoid.

   * - :py:func:`partition_monoid`
     - A presentation for the partition monoid.

   * - :py:func:`brauer_monoid`
     - A presentation for the Brauer monoid.

   * - :py:func:`rectangular_band`
     - A presentation for a rectangular band.

   * - :py:func:`stellar_monoid`
     - A presentation for the stellar monoid.

   * - :py:func:`chinese_monoid`
     - A presentation for the Chinese monoid.

   * - :py:func:`monogenic_semigroup`
     - A presentation for a monogenic semigroup.

   * - :py:func:`plactic_monoid`
     - A presentation for the plactic monoid.

   * - :py:func:`stylic_monoid`
     - A presentation for the stylic monoid.

   * - :py:func:`fibonacci_semigroup`
     - A presentation for a Fibonacci semigroup.

   * - :py:func:`temperley_lieb_monoid`
     - A presentation for the Temperley-Lieb monoid.

   * - :py:func:`singular_brauer_monoid`
     - A presentation for the singular part of the Brauer monoid.

   * - :py:func:`orientation_preserving_monoid`
     - A presentation for the monoid of orientation preserving mappings.

   * - :py:func:`orientation_reversing_monoid`
     - A presentation for the monoid of orientation reversing mappings.

Full API
--------

.. py:function:: symmetric_group(n: int, val: author = author.Carmichael) -> List[Tuple[List[int], List[int]]])

   A presentation for the symmetric group.

   Returns a list giving a monoid presentation for the
   symmetric group. The argument ``val`` determines the specific presentation
   which is returned. The options are:

   * ``author.Burnside + author.Miller`` (given on p.464 of `10.1017/CBO9781139237253`_)
   * ``author.Carmichael`` (given in comment 9.5.2 of `10.1007/978-1-84800-281-4`_)
   * ``author.Coxeter + author.Moser`` (see Ch. 3, Prop 1.2 of `hdl.handle.net/10023/2821`_)
   * ``author.Moore`` (given in comment 9.5.3 of `10.1007/978-1-84800-281-4`_)

   The default for ``val`` is ``author.Carmichael``.

   :param n: the degree
   :type n: int
   :param val: the author
   :type val: author

   :returns: List[Tuple[List[int], List[int]]]

   :raises RuntimeError: if ``n < 4``
   :raises RuntimeError: if ``val`` is not one of the combinations of authors listed above (modulo order of author)

   .. _10.1017/CBO9781139237253: https://doi.org/10.1017/CBO9781139237253
   .. _10.1007/978-1-84800-281-4: https://doi.org/10.1007/978-1-84800-281-4
   .. _hdl.handle.net/10023/2821: http://hdl.handle.net/10023/2821

.. py:function:: alternating_group(n: int, val: author) -> List[Tuple[List[int], List[int]]])

   A presentation for the alternating group.

   Returns a list giving a monoid presentation defining the
   alternating group of degree ``n``. The argument ``val`` determines the
   specific presentation which is returned. The options are:

   * ``author.Moore`` (see Ch. 3, Prop 1.3 of `hdl.handle.net/10023/2821`_)

   The default for ``val`` is ``author.Moore``.

   :param n: the degree
   :type n: int
   :param val: the author
   :type val: author

   :returns: List[Tuple[List[int], List[int]]]

   :raises RuntimeError: if ``n < 4``
   :raises RuntimeError: if ``val`` is not one of the combinations of authors listed above (modulo order of author)

   .. _hdl.handle.net/10023/2821: http://hdl.handle.net/10023/2821

.. py:function:: full_transformation_monoid(n: int, val: author) -> List[Tuple[List[int], List[int]]])

   A presentation for the full transformation monoid.

   Returns a list giving a monoid presentation defining the
   full transformation monoid. The argument ``val`` determines the specific
   presentation which is returned. The options are:

   * ``author.Aizenstat`` (see Ch. 3, Prop 1.7 of `hdl.handle.net/10023/2821`_)
   * ``author.Iwahori`` (see Theorem 9.3.1 of `10.1007/978-1-84800-281-4`_)

   The default for ``val`` is ``author.Iwahori``.

   :param n: the degree
   :type n: int
   :param val: the author
   :type val: author

   :returns: List[Tuple[List[int], List[int]]]

   :raises RuntimeError: if ``n < 4``
   :raises RuntimeError: if ``val`` is not one of the combinations of authors listed above (modulo order of author)

   .. _hdl.handle.net/10023/2821: http://hdl.handle.net/10023/2821
   .. _10.1007/978-1-84800-281-4: https://doi.org/10.1007/978-1-84800-281-4

.. py:function:: partial_transformation_monoid(n: int, val: author) -> List[Tuple[List[int], List[int]]])

   A presentation for the partial transformation monoid.

   Returns a list giving a monoid presentation defining the
   partial transformation monoid. The argument ``val`` determines the
   specific presentation which is returned. The options are:

   * ``author.Machine``
   * ``author.Sutov`` (see Theorem 9.4.1 of `10.1007/978-1-84800-281-4`_)

   The default for ``val`` is ``author.Sutov``.

   :param n: the degree
   :type n: int
   :param val: the author
   :type val: author

   :returns: List[Tuple[List[int], List[int]]]

   :raises RuntimeError: if ``n < 4``
   :raises RuntimeError: if ``val`` is not one of the combinations of authors listed above (modulo order of author)

   .. _10.1007/978-1-84800-281-4: https://doi.org/10.1007/978-1-84800-281-4

.. py:function:: symmetric_inverse_monoid(n: int, val: author) -> List[Tuple[List[int], List[int]]])

   A presentation for the symmetric inverse monoid.

   Returns a list giving a monoid presentation defining the
   symmetric inverse monoid. The argument ``val`` determines the specific
   presentation which is returned. The options are:

   * ``author.Sutov`` (see Theorem 9.2.2 of `10.1007/978-1-84800-281-4`_)

   The default for ``val`` is the only option above.

   :param n: the degree
   :type n: int
   :param val: the author
   :type val: author

   :returns: List[Tuple[List[int], List[int]]]

   :raises RuntimeError: if ``n < 4``
   :raises RuntimeError: if ``val`` is not one of the combinations of authors listed above (modulo order of author)

   .. _10.1007/978-1-84800-281-4: https://doi.org/10.1007/978-1-84800-281-4

.. py:function:: dual_symmetric_inverse_monoid(n: int, val: author) -> List[Tuple[List[int], List[int]]])

   A presentation for the dual symmetric inverse monoid.

   Returns a list giving a semigroup presentation defining
   the dual symmetric inverse monoid of degree ``n``. The argument ``val``
   determines the specific presentation which is returned. The options are:

   * ``author.Easdown + author.East + author.FitzGerald`` (from Section 3 of `10.48550/arxiv.0707.2439`_)

   The default for ``val`` is the only option above.

   :param n: the degree
   :type n: int
   :param val: the author
   :type val: author

   :returns: List[Tuple[List[int], List[int]]]

   :raises RuntimeError: if ``n < 3``
   :raises RuntimeError: if ``val`` is not one of the combinations of authors listed above (modulo order of author)

   .. _10.48550/arxiv.0707.2439: https://doi.org/10.48550/arxiv.0707.2439

.. py:function:: uniform_block_bijection_monoid(n: int, val: author) -> List[Tuple[List[int], List[int]]])

   A presentation for the uniform block bijection monoid.

   Returns a list giving a semigroup presentation defining
   the uniform block bijection monoid of degree ``n``. The argument ``val``
   determines the specific presentation which is returned. The only option
   is:

   * ``author.FitzGerald`` (see `10.1017/s0004972700037692`_)

   The default for ``val`` is the only option above.

   :param n: the degree
   :type n: int
   :param val: the author
   :type val: author

   :returns: List[Tuple[List[int], List[int]]]

   :raises RuntimeError: if ``n < 3``
   :raises RuntimeError: if ``val`` is not one of the combinations of authors listed above (modulo order of author)

   .. _10.1017/s0004972700037692: https://doi.org/10.1017/s0004972700037692

.. py:function:: partition_monoid(n: int, val: author) -> List[Tuple[List[int], List[int]]])

   A presentation for the partition monoid.

   Returns a list giving a semigroup presentation defining
   the partition monoid of degree ``n``. The argument ``val`` determines the
   specific presentation which is returned. The options are:

   * ``author.Machine``
   * ``author.East`` (see Theorem 41 of `10.1016/j.jalgebra.2011.04.008`_)

   The default for ``val`` is ``author.East``.

   :param n: the degree
   :type n: int
   :param val: the author
   :type val: author

   :returns: List[Tuple[List[int], List[int]]]

   :raises RuntimeError: if ``n < 4`` and ``val = author.East``
   :raises RuntimeError: if ``n != 3`` and ``val = author.Machine``
   :raises RuntimeError: if ``val`` is not one of the combinations of authors listed above (modulo order of author)

   .. _10.1016/j.jalgebra.2011.04.008: https://doi.org/10.1016/j.jalgebra.2011.04.008

.. py:function:: brauer_monoid(n: int) -> List[Tuple[List[int], List[int]]])

   A presentation for the Brauer monoid.

   Returns a list giving a semigroup presentation defining
   the Brauer monoid of degree ``n``, as described in Theorem 3.1 of the
   paper `10.2478/s11533-006-0017-6`_.

   :param n: the degree
   :type n: int

   :returns: List[Tuple[List[int], List[int]]]

   .. _`10.2478/s11533-006-0017-6`: https://doi.org/10.2478/s11533-006-0017-6

.. py:function:: rectangular_band(m: int, n: int) -> List[Tuple[List[int], List[int]]])

   A presentation for a rectangular band.

   Returns a list giving a semigroup presentation defining
   the ``m`` by ``n`` rectangular band, as given in Proposition 4.2 of
   `10.1007/s002339910016`_.


   :param m: the number of rows
   :type m: int
   :param n: the number of columns
   :type n: int

   :returns: List[Tuple[List[int], List[int]]]

   :raises RuntimeError: if ``m = 0``
   :raises RuntimeError: if ``n = 0``

   .. _`10.1007/s002339910016`: https://doi.org/10.1007/s002339910016

.. py:function:: stellar_monoid(l: int) -> List[Tuple[List[int], List[int]]])

   A presentation for the stellar monoid.

   Returns a list giving a semigroup presentation defining
   the stellar monoid with ``l`` generators, as in Theorem 4.39 of
   `10.48550/arXiv.1910.11740`_.

   :param l: the number of generators
   :type l: int

   :returns: List[Tuple[List[int], List[int]]

   :raises RuntimeError: if ``l < 2``

   .. _`10.48550/arXiv.1910.11740`: https://doi.org/10.48550/arXiv.1910.11740

.. py:function:: chinese_monoid(l: int) -> List[Tuple[List[int], List[int]]])

   A presentation for the Chinese monoid.

   Returns a list giving a semigroup presentation defining
   the Chinese monoid with ``n`` generators, as described in `10.1142/S0218196701000425`_.

   :param n: the number of generators
   :type n: int

   :returns: List[Tuple[List[int], List[int]]

   :raises RuntimeError: if ``n < 2``

   .. _`10.1142/S0218196701000425`: https://doi.org/10.1142/S0218196701000425

.. py:function:: monogenic_semigroup(m: int, r: int) -> List[Tuple[List[int], List[int]]])

   A presentation for a monogenic semigroup.

   Returns a list giving a semigroup presentation defining
   the monogenic semigroup defined by the presentation
   :math:`\langle a \mid a^{m + r} = a^m \rangle`.

   :param m: the index
   :type m: int
   :param r: the period
   :type r: int

   :returns: List[Tuple[List[int], List[int]]]

   :raises RuntimeError: if ``r = 0``

.. py:function:: plactic_monoid(n: int) -> List[Tuple[List[int], List[int]]])

   A presentation for the plactic monoid.

   Returns a list giving a semigroup presentation defining
   the plactic monoid with ``n`` generators (see Section 3 of
   `10.1007/s00233-022-10285-3`_).

   :param n: the number of generators
   :type n: int

   :returns: List[Tuple[List[int], List[int]]]

   :raises RuntimeError: if ``n < 2``

   .. _`10.1007/s00233-022-10285-3`: https://doi.org/10.1007/s00233-022-10285-3

.. py:function:: stylic_monoid(n: int) -> List[Tuple[List[int], List[int]]])

   A presentation for the stylic monoid.

   Returns a list giving a semigroup presentation defining
   the stylic monoid with ``n`` generators (see Theorem 8.1 of
   `10.1007/s00233-022-10285-3`_).

   :param n: the number of generators
   :type n: int

   :returns: List[Tuple[List[int], List[int]]]

   :raises RuntimeError: if ``n < 2``

   .. _`10.1007/s00233-022-10285-3`: https://doi.org/10.1007/s00233-022-10285-3

.. py:function:: fibonacci_semigroup(r: int, n: int) -> List[Tuple[List[int], List[int]]])

   A presentation for a Fibonacci semigroup.

   Returns a list giving a semigroup presentation defining
   the Fibonacci semigroup :math:`F(r, n)`, as described in the paper
   `10.1016/0022-4049(94)90005-1`_.

   :param r: the length of the left hand sides of the relations
   :type r: int
   :param n: the number of generators
   :type n: int

   :returns: List[Tuple[List[int], List[int]]]

   :raises RuntimeError: if ``n = 0``
   :raises RuntimeError: if ``r = 0``

   .. _`10.1016/0022-4049(94)90005-1`: https://doi.org/10.1016/0022-4049(94)90005-1

.. py:function:: temperley_lieb_monoid(n: int) -> List[Tuple[List[int], List[int]]])

   A presentation for the Temperley-Lieb monoid.

   Returns a list giving a semigroup presentation defining
   the Temperley-Lieb monoid with ``n`` generators, as described in
   Theorem 2.2 of the paper `10.1093/qmath/haab001`_.

   :param n: the number of generators
   :type n: int

   :returns: List[Tuple[List[int], List[int]]]

   :raises RuntimeError: if ``n < 3``

   .. _10.1093/qmath/haab001: https://doi.org/10.1093/qmath/haab001

.. py:function:: singular_brauer_monoid(n: int) -> List[Tuple[List[int], List[int]]])

   A presentation for the singular part of the Brauer monoid.

   Returns a list giving a semigroup presentation for the
   singular part of the Brauer monoid of degree ``n``, as in Theorem 5 of
   the paper `10.21136/MB.2007.134125`_).

   :param n: the degree
   :type n: int

   :returns: List[Tuple[List[int], List[int]]]

   :raises RuntimeError: if ``n < 3``

   .. _`10.21136/MB.2007.134125`: https://doi.org/10.21136/MB.2007.134125

.. py:function:: orientation_preserving_monoid(n: int) -> List[Tuple[List[int], List[int]]])

   A presentation for the monoid of orientation preserving
   mappings.

   Returns a list giving a semigroup presentation defining
   the monoid of orientation preserving mappings on a finite chain of order
   ``n``, as described in the paper `10.1007/s10012-000-0001-1`_.

   :param n: the order of the chain
   :type n: int

   :returns: List[Tuple[List[int], List[int]]]

   :raises RuntimeError: if ``n < 3``

   .. _`10.1007/s10012-000-0001-1`: https://doi.org/10.1007/s10012-000-0001-1`

.. py:function:: orientation_reversing_monoid(n: int) -> List[Tuple[List[int], List[int]]])

   A presentation for the monoid of orientation preserving
   mappings.

   Returns a list giving a semigroup presentation defining
   the monoid of orientation reversing mappings on a finite chain of order
   ``n``, as described in the paper `10.1007/s10012-000-0001-1`_.

   :param n: the order of the chain
   :type n: int

   :returns: List[Tuple[List[int], List[int]]]

   :raises RuntimeError: if ``n < 3``

   .. _`10.1007/s10012-000-0001-1`: https://doi.org/10.1007/s10012-000-0001-1`
