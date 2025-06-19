.. image:: ../pictures/libsemigroups_pybind11_banner.svg
  :width: 75%
  :align: center
  :alt: libsemigroups_pybind11 banner

.. title:: libsemigroups_pybind11

|

What is libsemigroups_?
~~~~~~~~~~~~~~~~~~~~~~~~~~

libsemigroups_ is a C++14 library containing implementations of several
algorithms for computing finite and finitely presented semigroups. Namely:

- the `Froidure-Pin algorithm`_ for computing finite semigroups
- the `Todd-Coxeter algorithm`_ for finitely presented semigroups and monoids;
- the `Knuth-Bendix algorithm`_ for finitely presented semigroups and monoids;
- the `Schreier-Sims algorithm`_ for permutation groups;
- a preliminary implementation of the Konieczny_ and Lallement-McFadden_
  algorithm for computing finite semigroups which act on sets.

``libsemigroups_pybind11`` is a python package exposing much (but not all) of
the functionality of libsemigroups_.

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

.. _libsemigroups: https://libsemigroups.github.io/libsemigroups/index.html 

How to use it
~~~~~~~~~~~~~

See the installation instructions :doc:`install`.

If you encounter any issues with the package or have any suggestions, then
please let us know on the `issue tracker`_.


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

