.. Copyright (c) 2021, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Finitely presented semigroups 
=============================

``libsemigroups`` implements serveral algorithms for computing finitely
presented semigroups and monoids. The main algorithms implemented are
Todd-Coxeter, Knuth-Bendix, and brute-force enumeration; see the links below
for further details. As a convenience, ``libsemigroups_pybind11`` also has a class
:py:class:`FpSemigroup` that runs some predetermined variants of Todd-Coxeter,
Knuth-Bendix, and the brute-force enumeration in parallel.  This class is, at
present, not very customisable, and lacks some of the fine grained control
offered by the classes implementing individual algorithms, such as
:py:class:`KnuthBendix`.  

At present some of the features for finitely presented semigroups in
``libsemigroups`` are not present in ``libsemigroups_pybind11``. We plan to add
the missing features in a later release. 

All of the classes for finitely presented semigroups and monoids in
``libsemigroups_pybind11`` can be used "interactively", in the sense that they
can be run for a particular amount of time, or until some condition is met; for
further details see, for example,
:py:meth:`.KnuthBendix.run_for` and
:py:meth:`.KnuthBendix.run_until`. 

The classes in ``libsemigroups_pybind11`` for finitely presented semigroups and
monoids are:

.. toctree::
   :maxdepth: 2
  
   api/fpsemi
   api/knuth-bendix
