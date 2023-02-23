.. Copyright (c) 2021, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

Congruence
==========

On this page we describe the functionality of the :py:class:`Congruence` class.
This class can be used for computing a congruence over a semigroup by running
every applicable algorithm (and possibly some variants of the same
algorithm) in parallel. This class is provided for convenience, at present it
is not very customisable, and lacks some of the fine grained control offered by
the classes implementing individual algorithms, such as :py:class:`ToddCoxeter`
and :py:class:`KnuthBendix`.

.. seealso:: :py:class:`congruence_kind` and :py:class:`tril`.

Example
-------

.. doctest::

  >>> from libsemigroups_pybind11 import FpSemigroup, Congruence, congruence_kind
  >>> S = FpSemigroup()
  >>> S.set_alphabet(3)
  >>> S.set_identity(0)
  >>> S.add_rule([1, 2], [0])
  >>> S.is_obviously_infinite()
  True
  >>> C = Congruence(congruence_kind.twosided, S)
  >>> C.add_pair([1, 1, 1], [0])
  >>> C.number_of_classes()
  3

.. autosummary::
   ~Congruence
   Congruence.add_pair
   Congruence.class_index_to_word
   Congruence.const_contains
   Congruence.contains
   Congruence.dead
   Congruence.finished
   Congruence.generating_pairs
   Congruence.has_parent_froidure_pin
   Congruence.has_quotient_froidure_pin
   Congruence.has_todd_coxeter
   Congruence.is_quotient_obviously_finite
   Congruence.is_quotient_obviously_infinite
   Congruence.kill
   Congruence.knuth_bendix
   Congruence.less
   Congruence.non_trivial_classes
   Congruence.number_of_classes
   Congruence.number_of_generating_pairs
   Congruence.number_of_non_trivial_classes
   Congruence.parent_froidure_pin
   Congruence.quotient_froidure_pin
   Congruence.report
   Congruence.report_every
   Congruence.report_why_we_stopped
   Congruence.run_for
   Congruence.run_until
   Congruence.set_number_of_generators
   Congruence.stopped_by_predicate
   Congruence.timed_out
   Congruence.todd_coxeter
   Congruence.word_to_class_index

.. autoclass:: Congruence
   :members:
