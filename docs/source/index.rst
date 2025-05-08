libsemigroups_pybind11 - Version 1.0.0
======================================

python bindings for the C++ library libsemigroups
-------------------------------------------------

What is ``libsemigroups``?
~~~~~~~~~~~~~~~~~~~~~~~~~~

``libsemigroups`` is a C++14 library containing implementations of several
algorithms for computing finite and finitely presented semigroups. Namely:

- the `Froidure-Pin algorithm`_ for computing finite semigroups
- the `Todd-Coxeter algorithm`_ for finitely presented semigroups and monoids;
- the `Knuth-Bendix algorithm`_ for finitely presented semigroups and monoids;
- the `Schreier-Sims algorithm`_ for permutation groups;
- a preliminary implementation of the Konieczny_ and Lallement-McFadden_
  algorithm for computing finite semigroups which act on sets.

``libsemigroups_pybind11`` is a python package exposing much (but not all) of
the functionality of ``libsemigroups``.

The development version of ``libsemigroups_pybind11`` is available on github_,
and some related projects are here_.

.. _froidure-pin algorithm: https://www.irif.fr/~jep/PDF/Rio.pdf

.. _github: https://github.com/libsemigroups/libsemigroups_pybind11

.. _here: https://github.com/libsemigroups

.. _knuth-bendix algorithm: https://en.wikipedia.org/wiki/Knuth%E2%80%93Bendix_completion_algorithm

.. _konieczny: https://link.springer.com/article/10.1007/BF02573672

.. _lallement-mcfadden: https://www.sciencedirect.com/science/article/pii/S0747717108800570

.. _schreier-sims algorithm: https://en.wikipedia.org/wiki/Schreier%E2%80%93Sims_algorithm

.. _todd-coxeter algorithm: https://en.wikipedia.org/wiki/Todd%E2%80%93Coxeter_algorithm

How to use it
~~~~~~~~~~~~~

See the installation instructions:

.. toctree::
    :maxdepth: 1

    install
    changelog

.. toctree::
    :caption: Data Structures
    :hidden:

    data-structures/adapters/index
    data-structures/elements/index
    data-structures/misc/index
    data-structures/order/index
    data-structures/presentations/index
    data-structures/suffix-trees/index
    data-structures/tries/index
    data-structures/to-function
    data-structures/visualisation/index
    data-structures/word-graph/index
    data-structures/words/index

.. toctree::
    :caption: Main Algorithms
    :hidden:

    main-algorithms/action/index
    main-algorithms/congruence/index
    main-algorithms/froidure-pin/index
    main-algorithms/kambites/index
    main-algorithms/knuth-bendix/index
    main-algorithms/konieczny/index
    main-algorithms/low-index/index
    main-algorithms/radoszewski-rytter/index
    main-algorithms/schreier-sims/index
    main-algorithms/stephen/index
    main-algorithms/todd-coxeter/index

.. toctree::
    :caption: Bibliography
    :hidden:

    biblio

.. toctree::
    :caption: Further info
    :maxdepth: 1

    authors
