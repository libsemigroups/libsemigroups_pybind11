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

    * :doc:`install`
    * :doc:`changelog`

The structure of the module
~~~~~~~~~~~~~~~~~~~~~~~~~~~
The Python module ``libsemigroups_pybind11`` was designed to mirror the C++
library ``libsemigroups`` as closely as possible, whilst navigating the
fundamental difference between Python and C++. This is done with the help of the
tool `pybind11 <https://pybind11.readthedocs.io/en/stable/>`_.

For various implementational reasons (mostly related to Python's lack of an
analogue for C++'s templating system), the Python code exposed by ``pybind11``
is less streamlined and concise than the C++ library. To try and address this,
the authors of ``libsemigroups_pybind11`` have further wrapped the Python code
produced by ``pybind11`` to make the Python and C++ user experience as similar
as possible.

The Python bindings of the ``libsemigroups`` code produced by ``pybind11``
reside in an intermediate module called ``_libsemigroups_pybind11`` (note the
leading underscore), and the public-facing nicely wrapped code resides in this
module — ``libsemigroups_pybind11``.

Should this impact the way you, the user, use ``libsemigroups_pybind11``? For
the most part, no. It should be possible to use ``libsemigroups_pybind11`` in
the ways documented on this site without the knowledge that
``_libsemigroups_pybind11`` even exists. The notable exceptions to this relate
to type names and error messages. A lot of the code in
``libsemigroups_pybind11`` has been imported from ``_libsemigroups_pybind11``,
and this is visible in qualified type names. For example:

.. doctest:: python

    >>> from libsemigroups_pybind11 import WordGraph
    >>> WordGraph
    <class '_libsemigroups_pybind11.WordGraph'>

Additionally, some functions or classes in the ``_libsemigroups_pybind11``
module have additional prefixes and suffixes relative to their
``libsemigroups_pybind11`` counterpart. These usually relate to the module that
the function or class is contained within or a type the function or class is
defined upon. These may appear in error messages. For example:

.. doctest:: python

    >>> from libsemigroups_pybind11 import aho_corasick, AhoCorasick
    >>> ac = AhoCorasick()
    >>> aho_corasick.add_word(ac, False)
    Traceback (most recent call last):
        ...
    TypeError: aho_corasick_add_word(): incompatible function arguments. The following argument types are supported:
        1. (ac: _libsemigroups_pybind11.AhoCorasick, w: list[int]) -> int
        2. (ac: _libsemigroups_pybind11.AhoCorasick, w: str) -> int
    <BLANKLINE>
    Invoked with: <AhoCorasick with 1 node>, False

The :doc:`authors <authors>` of ``libsemigroups_pybind11`` have gone to a lot of
effort to try and make error messages clear, specific and intelligible; however,
if there you encounter any errors with unclear messages, please raise this on
the
`issue tracker <https://github.com/libsemigroups/libsemigroups_pybind11/issues>`_.

Further Info
------------

For additional information about ``libsemigroups_pybind11``, see the following:

    * :doc:`authors`
    * :doc:`biblio`

.. toctree::
    :caption: Package Info
    :maxdepth: 1
    :hidden:

    install
    changelog
    authors
    biblio

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

