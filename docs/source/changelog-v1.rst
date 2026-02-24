..
    Copyright (c) 2025-2026, J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

Changelog - version 1
=====================

v1.4.0 (released 25/02/2026)
----------------------------

This is a minor release including some improvements and new features:

* Update to libsemigroups v3.5.0 and add the new features it contains.
* transf: future proof ``__getitem__`` by @james-d-mitchell in https://github.com/libsemigroups/libsemigroups_pybind11/pull/382
* todd-coxeter: update for changes in libsemigroups by @james-d-mitchell in https://github.com/libsemigroups/libsemigroups_pybind11/pull/388


v1.3.0 (released 18/01/2026)
----------------------------

* Update to libsemigroups v3.4.0 and add features introduced there by
  @james-d-mitchell in
  https://github.com/libsemigroups/libsemigroups_pybind11/pull/380

v1.2.0 (released 16/12/2025)
----------------------------

This is a minor release incorporating a number of bugfixes/improvements in
libsemigroups_. Support for HPCombi_ transformation types is also added for the
first time!

* Drop support for Python 3.9 support Python 3.14 by @Joseph-Edwards in https://github.com/libsemigroups/libsemigroups_pybind11/pull/335
* Remove ``Union`` by @james-d-mitchell in https://github.com/libsemigroups/libsemigroups_pybind11/pull/339
* Import file modules in ``__init__`` by @james-d-mitchell in https://github.com/libsemigroups/libsemigroups_pybind11/pull/338
* Sync aho corasick by @james-d-mitchell in https://github.com/libsemigroups/libsemigroups_pybind11/pull/340
* konieczny: add init mem fn by @james-d-mitchell in https://github.com/libsemigroups/libsemigroups_pybind11/pull/349
* word-graph: fix ``follow_path`` return type by @james-d-mitchell in https://github.com/libsemigroups/libsemigroups_pybind11/pull/348
* paths: add helpers by @james-d-mitchell in https://github.com/libsemigroups/libsemigroups_pybind11/pull/350
* Sync ``Forest`` and its helpers with recent changes in libsemigroups_ by @james-d-mitchell in https://github.com/libsemigroups/libsemigroups_pybind11/pull/347
* Sync presentation by @james-d-mitchell in https://github.com/libsemigroups/libsemigroups_pybind11/pull/351
* Sync examples by @james-d-mitchell in https://github.com/libsemigroups/libsemigroups_pybind11/pull/353
* Sync ``to<Presentation>`` upstream enhancements by @jswent in https://github.com/libsemigroups/libsemigroups_pybind11/pull/358
* hpcombi: support ``Vect16`` + ``PTransf16`` + subclasses by @james-d-mitchell in https://github.com/libsemigroups/libsemigroups_pybind11/pull/355
* paths: update for fixes in libsemigroups_ by @james-d-mitchell in https://github.com/libsemigroups/libsemigroups_pybind11/pull/363

New Contributors
~~~~~~~~~~~~~~~~

* @jswent made their first contribution in https://github.com/libsemigroups/libsemigroups_pybind11/pull/359

v1.1.0 (released 30/09/2025)
----------------------------

This is a minor release with a few new features from libsemigroups_ and some bugfixes:

* Throw if bad alphabet or rules by @james-d-mitchell in https://github.com/libsemigroups/libsemigroups_pybind11/pull/317
* Add some missing and features from the ``presentation`` namespace by @james-d-mitchell in https://github.com/libsemigroups/libsemigroups_pybind11/pull/318
* Test default values and ``datetime`` types by @Joseph-Edwards in https://github.com/libsemigroups/libsemigroups_pybind11/pull/324
* Todd coxeter additions by @james-d-mitchell in https://github.com/libsemigroups/libsemigroups_pybind11/pull/328

v1.0.1 (released 13/08/2025)
----------------------------

This is a minor release that adds some missing files to the PyPI source.

v1.0.0 (released 12/08/2025)
----------------------------

This is a major release that significantly expands the API of
``libsemigroups_pybind11`` to expose much of the functionality implemented in
version 3 of libsemigroups_. The extent of the changes made in this release
means that it is likely that any code written with earlier versions of
``libsemigroups_pybin11`` will no longer work.

The structure of ``libsemigroups_pybind11`` is very tightly linked to the the
structure of libsemigroups_. Therefore, some of the differences between
v0.10.1 and v1.0.0 of ``libsemigroups_pyind11`` will be related to the
differences between v2 and v3 of libsemigroups_, such as changes to class names
and interfaces. These changes can be found in the
`libsemigroups changelog <https://libsemigroups.github.io/libsemigroups/md_changelog-v3.html>`_.
