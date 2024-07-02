.. Copyright (c) 2021-2024 J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Presentations
=============

The ``libsemigroups`` C\ **++** library, and hence the
``libsemigroups_pybind11`` Python package, implements several algorithms for
computing with finitely presented semigroups and monoids. The implementation of
the finite presentations on which these algorithms can be run is described
below.

In ``libsemigroups_pybind11``, a presentation is modelled as a collection of
*rules*, each of which is a pair of *words*. Each word is made up of *letters*.
Presently, for any given presentation, all letters must either be of the type
:any:`str` or :any:`int`. In a presentation where letters are of the type
:any:`str`, words have the type :any:`str`. In a presentation where letters are
of the type :any:`int`, words will be lists of :any:`int` types.

**Once a presentation is constructed, the type of its letters and words cannot
be changed.**

In what follows, we will use the pseudo-types ``Letter`` and ``Word`` instead of
``str | int`` and ``str | List[int]`` to further indicate that two types of
letters and words cannot be interchanged once a presentation is constructed.

All of the classes for finitely presented semigroups and monoids in
``libsemigroups_pybind11`` can be used "interactively", in the sense that they
can be run for a particular amount of time, or until some condition is met; for
further details see, for example,
:py:meth:`.KnuthBendix.run_for` and
:py:meth:`.KnuthBendix.run_until`.

The classes and modules in ``libsemigroups_pybind11`` for finitely presented
semigroups and monoids are:

.. toctree::
   :maxdepth: 1
   
   present
   inverse-present
   present-helpers
   examples
