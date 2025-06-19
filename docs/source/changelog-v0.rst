..
    Copyright (c) 2021-2024, J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

Changelog - version 0
=====================

v0.10.1 (released 29/03/2023)
-----------------------------

This release increases the required version of ``libsemigroups`` to v2.7.1,
which contains some bug fixes.

v0.10.0 (released 23/03/2023)
-----------------------------

This is a minor release adding some new functionality from ``libsemigroups``:

- ukkonen: add support for ``Ukkonen`` + helpers by @james-d-mitchell in
  https://github.com/libsemigroups/libsemigroups_pybind11/pull/132
- present: add further manip. funcs by @james-d-mitchell in
  https://github.com/libsemigroups/libsemigroups_pybind11/pull/136

v0.9.2 (released 22/03/2023)
----------------------------

This is a minor release fixing some issues in the last release.

v0.9.1 (released 21/03/2023)
----------------------------

This is a minor release with some minor improvements:

- stephen: make ``accepts/is_left_factor`` call more robust by @james-d-mitchell
  in https://github.com/libsemigroups/libsemigroups_pybind11/pull/129
- tests: add libsemigroups v2.7.0 special case by @james-d-mitchell in
  https://github.com/libsemigroups/libsemigroups_pybind11/pull/130

v0.9.0 (released 06/03/2023)
----------------------------

This is a minor release including two changes and some minor improvements to the
doc:

- Improve ``ActionDigraph`` colours by @MTWhyte in
  https://github.com/libsemigroups/libsemigroups_pybind11/pull/125
- Add support for ``ActionDigraph::cbegin/cend_pstilo`` by @james-d-mitchell in
  https://github.com/libsemigroups/libsemigroups_pybind11/pull/127

v0.8.0 (released 28/02/2023)
----------------------------

This is a minor version that adds all the features from the namespace
``libsemigroups::presentation`` to the package, these are:

- ``add_zero_rules``
- ``are_rules_sorted``
- ``change_alphabet``
- ``letter`` and ``character``
- ``first_unused_letter``
- ``greedy_reduce_length``
- ``longest_rule``, ``shortest_rule``, ``longest_rule_length``, and
  ``shortest_rule_length``
- ``make_semigroup``

v0.7.3 (released 07/02/2023)
----------------------------

This is a minor version with some improvements and adjustments for forthcoming
changes in ``libsemigroups``.

v0.7.2 (released 12/01/2023)
----------------------------

This is a minor change related to some forthcoming changes in ``libsemigroups``.

v0.7.1 (released 11/01/2023)
----------------------------

This is a very minor release updating the required versions of some dependencies
to permit the ``libsemigroups_pybind11`` wheel to be built with python 3.11, and
to adapt for some recent changes in ``libsemigroups``.

v0.7.0 (released 15/12/2022)
----------------------------

This release contains a number of improvements and fixes, and adds support for
the library of finite semigroup and monoid presentations in ``libsemigroups``
and the ``Stephen`` class from ``libsemigroups``.

- Add functionality for the presentation helper ``replace_word`` by @MTWhyte in
  https://github.com/libsemigroups/libsemigroups_pybind11/pull/84
- Out neighbours for action digraphs by @james-d-mitchell in
  https://github.com/libsemigroups/libsemigroups_pybind11/pull/91
- Add functionality for the ``fpsemi-examples`` presentations by @MTWhyte in
  https://github.com/libsemigroups/libsemigroups_pybind11/pull/85
- Stephen by @james-d-mitchell in
  https://github.com/libsemigroups/libsemigroups_pybind11/pull/93

v0.6.0 (released 02/12/2022)
----------------------------

This release contains a number of improvements and fixes, and adds support for
the ``Konieczny`` class from ``libsemigroups``.

- Add missing header include in cong.cpp by @james-d-mitchell in
  https://github.com/libsemigroups/libsemigroups_pybind11/pull/77
- Remove trivial rules by @james-d-mitchell in
  https://github.com/libsemigroups/libsemigroups_pybind11/pull/79
- Fix string encoding in ``KnuthBendix`` by @james-d-mitchell in
  https://github.com/libsemigroups/libsemigroups_pybind11/pull/82
- Add support for ``Konieczny`` from ``libsemigroups`` by @james-d-mitchell in
  https://github.com/libsemigroups/libsemigroups_pybind11/pull/80
- Better ``__repr__`` for ``KnuthBendix`` by @james-d-mitchell in
  https://github.com/libsemigroups/libsemigroups_pybind11/pull/86

v0.5.0 (released 16/11/2022)
----------------------------

This is a minor release adding support for the ``libsemigroups`` class
``Kambites`` for computing small overlap monoids.

v0.4.3 (released 09/11/2022)
----------------------------

This is a very minor release which contains no functional changes, or bugfixes,
but exists to allow zenodo.org to pick up the release and generate a doi for the
package.

v0.4.2 (released 28/10/2022)
----------------------------

Update for version 2.3.2 of ``libsemigroups`` which contains some bugfixes.

v0.4.1 (released 11/10/2022)
----------------------------

Update for version 2.3.1 of ``libsemigroups`` which contains some bugfixes in
the ``Sims1`` class, the ``Presentation`` class and its helper functions.

v0.4.0 (released 04/10/2022)
----------------------------

This is a minor release with a couple of new features added:

- ``libsemigroups`` constants ``POSITIVE_INFINITY``, ``NEGATIVE_INFINITY``, and
  ``UNDEFINED`` are properly supported;
- ``libsemigroups::matrix_helper::pow`` is added as a method for ``__pow__`` for
  some types of matrices (those not defined over a runtime semiring);

and some minor improvements (the tests now use ``pytest`` exclusively).

v0.3.0 (released 29/09/2022)
----------------------------

This is a minor release adding support for the ``libsemigroups`` class ``Sims1``
for computing low index congruences, and some further minor changes to the
``Presentation`` class.

v0.2.2 (released 16/09/2022)
----------------------------

A very minor release to futureproof some tests against new versions of
``libsemigroups``.

v0.2.1 (released 12/09/2022)
----------------------------

A very minor release trying to fix an issue in the release process.

v0.2.0 (released 10/09/2022)
----------------------------

This is a minor release adding support for the ``libsemigroups`` class template
``Presentation`` by @MTWhyte and @james-d-mitchell in:

https://github.com/libsemigroups/libsemigroups_pybind11/pull/49

v0.1.8 (released 27/01/2022)
----------------------------

This is a very minor release which contains no functional changes, or bugfixes,
but exists to fix an issue related to the wheel created for pypi on mac os x arm
machines.

v0.1.7 (released 27/01/2022)
----------------------------

This is a very minor release which contains no functional changes, or bugfixes,
but exists to fix an issue related to the wheel created for pypi on mac os x arm
machines.

v0.1.6 (released 02/12/2021)
----------------------------

This is a very minor release which contains no functional changes, or bugfixes,
but exists to test whether or not the github actions job for creating wheels,
and uploading to pypi works properly. These changes were made by Chinmaya
Nagpal.

v0.1.5 (released 02/12/2021)
----------------------------

This is a minor release fixing (hopefully) the required version of
``pkgconfig``.

v0.1.4 (released 12/11/2021)
----------------------------

Yet another minor release updating the C++ code for some forthcoming changes in
``libsemigroups``.

v0.1.3 (released 11/11/2021)
----------------------------

A minor release updating the C++ code for some forthcoming changes in
``libsemigroups``.

v0.1.2 (released 11/11/2021)
----------------------------

A minor release fixing some issues and adding some more tests.

v0.1.1 (released 08/10/2021)
----------------------------

An exceedingly minor release to resolve an issue in the previous release.

v0.1.0 (released 08/10/2021)
----------------------------

Some minor issues were resolved and the function ``follow_path`` was added for
``ActionDigraphs``.

v0.0.0 (released 24/09/2021)
----------------------------

First release of the package, some functionality of ``libsemigroups`` is not yet
available.
