.. image:: ../pictures/libsemigroups_pybind11_banner.svg
  :width: 75%
  :align: center
  :alt: libsemigroups_pybind11 banner

.. title:: libsemigroups_pybind11

|

What is libsemigroups_?
~~~~~~~~~~~~~~~~~~~~~~~

Before explaining what ``libsmigroups_pybind11`` is, it is first necessary to
explain `libsemigroups`_. `libsemigroups`_ is a C++17 library containing
implementations of several algorithms for computing finite, and finitely
presented, semigroups and monoids. The main algorithms implemented in
`libsemigroups`_ are:

- the `Froidure-Pin algorithm`_ for computing semigroups and monoids defined
  by a generating set consisting of elements whose multiplication and equality
  is decidable (such as transformations, partial permutations, permutations,
  bipartitions, and matrices over a semiring);
- Kambites' algorithm for solving the word problem in small overlap monoids
  from
  `Small overlap monoids I: The word problem <https://doi.org/10.1016/j.jalgebra.2008.09.038>`_,
  and the algorithm from
  `An explicit algorithm for normal forms in small overlap monoids`_;
- the `Knuth-Bendix algorithm`_ for finitely presented semigroups and monoids;
  a version of Sims' low index subgroup algorithm for computing congruences of a
  semigroup or monoid from
  `Computing finite index congruences of finitely presented semigroups and monoids`_
  in the classes;
- a generalized version of the algorithms described in
  `Green's equivalences in finite semigroups of binary relations`_ by
  Konieczny, and
  `On the determination of Green's relations in finite transformation semigroups`_
  by Lallement and Mcfadden for computing finite semigroups and monoids
  admitting a pair of actions with particular properties;
- the algorithm from
  `Efficient Testing of Equivalence of Words in a Free Idempotent Semigroup`_
  by Radoszewski and Rytter;
- a non-random version of the `Schreier-Sims algorithm`_ for permutation groups;
- a version of Stephen's procedure from
  `Applications of automata theory to presentations of monoids and inverse monoids`_
  for finitely presented inverse semigroups and monoids (for a given word ``w``
  this procedure is for determining words equivalent to ``w`` or that are
  left divisors of ``w``);
- the `Todd-Coxeter algorithm`_ for finitely presented semigroups and monoids;
  see also `The Todd-Coxeter algorithm for semigroups and monoids`_.

`libsemigroups`_ is partly based on
`Algorithms for computing finite semigroups <https://www.irif.fr/~jep/PDF/Rio.pdf>`_,
`Expository Slides`_, and `Semigroupe 2.01`_ by `Jean-Eric Pin`_.

.. _Froidure-Pin algorithm: https://www.irif.fr/~jep/PDF/Rio.pdf
.. _An explicit algorithm for normal forms in small overlap monoids: https://doi.org/10.1016/j.jalgebra.2023.04.019
.. _Knuth-Bendix algorithm: https://en.wikipedia.org/wiki/Knuth%E2%80%93Bendix_completion_algorithm
.. _Computing finite index congruences of finitely presented semigroups and monoids: https://arxiv.org/abs/2302.06295
.. _Green's equivalences in finite semigroups of binary relations: https://link.springer.com/article/10.1007/BF02573672
.. _On the determination of Green's relations in finite transformation semigroups: https://www.sciencedirect.com/science/article/pii/S0747717108800570
.. _Efficient Testing of Equivalence of Words in a Free Idempotent Semigroup: https://link.springer.com/chapter/10.1007/978-3-642-11266-9_55
.. _Applications of automata theory to presentations of monoids and inverse monoids: https://digitalcommons.unl.edu/dissertations/AAI8803771/
.. _Todd-Coxeter algorithm: https://en.wikipedia.org/wiki/Todd%E2%80%93Coxeter_algorithm
.. _The Todd-Coxeter algorithm for semigroups and monoids: https://doi.org/10.1007/s00233-024-10431-z
.. _Schreier-Sims algorithm: https://en.wikipedia.org/wiki/Schreier%E2%80%93Sims_algorithm
.. _Expository Slides: https://www.irif.fr/~jep/PDF/Exposes/StAndrews.pdf
.. _Semigroupe 2.01: https://www.irif.fr/~jep/Logiciels/Semigroupe2.0/semigroupe2.html
.. _Jean-Eric Pin: https://www.irif.fr/~jep/

What is ``libsemigroups_pybind11``?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

``libsemigroups_pybind11`` is a package for Python 3.9+ exposing much
(but not all) of the functionality of libsemigroups_. It is built with the help
of the excellent library pybind11_, for which we are very grateful. A more
detailed description of the structure of this package, along with some
associated quirks, is described on the
:doc:`exceptions page<libsemigroups-error>`.

The development version of ``libsemigroups_pybind11`` is available on github_,
and some related projects are here_.


.. _github: https://github.com/libsemigroups/libsemigroups_pybind11
.. _here: https://github.com/libsemigroups

How to install ``libsemigroups_pybind11``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To see the different ways ``libsemigroups_pybind11`` can be installed, see the
:doc:`installation page<install>`.

Issues
~~~~~~

If you find any problems with ``libsemigroups_pybind11``, or have any
suggestions for features that you'd like to see, please use the
`issue tracker`_.

Acknowledgements
~~~~~~~~~~~~~~~~

In addition to `libsemigroups`_, there are several excellent projects that are
utilised in the development of ``libsemigroups_pybind11``, specifically:

* `codespell`_, `cpplint`_, `Pylint`_ and `Ruff`_ for code quality;
* `Graphviz`_ for graph visualisation;
* pybind11_ for assistance in the binding of `libsemigroups`_;
* `pytest`_ for testing; and
* `Sphinx`_ for the documentation.

We would like to thank the authors and contributors of these projects!


.. _codespell: https://github.com/codespell-project/codespell
.. _cpplint: https://github.com/cpplint/cpplint
.. _Pylint: https://pylint.readthedocs.io/en/latest/
.. _Ruff: https://docs.astral.sh/ruff
.. _pytest: https://docs.pytest.org/en/stable/
.. _Sphinx: https://www.sphinx-doc.org/en/master/

.. toctree::
    :caption: Package Info
    :maxdepth: 1
    :hidden:

    authors
    biblio
    changelog-v1
    changelog-v0
    libsemigroups-error
    install

.. toctree::
    :caption: Data Structures
    :hidden:

    data-structures/adapters/index
    data-structures/constants/index
    data-structures/elements/index
    data-structures/enums/index
    data-structures/order/index
    data-structures/presentations/index
    data-structures/suffix-trees/index
    data-structures/to-function
    data-structures/tries/index
    data-structures/visualisation/index
    data-structures/word-graph/index
    data-structures/words/index

.. toctree::
    :caption: Main Algorithms
    :hidden:

    main-algorithms/action/index
    main-algorithms/congruence/index
    main-algorithms/core-classes/index
    main-algorithms/froidure-pin/index
    main-algorithms/kambites/index
    main-algorithms/knuth-bendix/index
    main-algorithms/konieczny/index
    main-algorithms/low-index/index
    main-algorithms/radoszewski-rytter/index
    main-algorithms/schreier-sims/index
    main-algorithms/stephen/index
    main-algorithms/todd-coxeter/index

