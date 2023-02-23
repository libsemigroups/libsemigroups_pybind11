.. Copyright (c) 2021, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

Todd-Coxeter
============

This class contains the main implementation of the Todd-Coxeter algorithm for
computing left, right, and 2-sided congruences on semigroups and monoids.

This page contains a summary of the main methods of the class ``ToddCoxeter``.

In this documentation we use the term "coset enumeration" to mean the execution
of (any version of) the Todd-Coxeter algorithm.

.. seealso:: :py:class:`congruence_kind` and :py:class:`tril`.

.. doctest::

   >>> from libsemigroups_pybind11 import congruence_kind, ToddCoxeter
   >>> tc = ToddCoxeter(congruence_kind.left)            # construct a left congruence
   >>> tc.set_number_of_generators(2)                    # 2 generators
   >>> tc.add_pair([0, 0], [0])                          # generator 0 squared is itself
   >>> tc.add_pair([0], [1])                             # generator 0 equals 1
   >>> tc.strategy(ToddCoxeter.strategy_options.felsch)  # set the strategy
   <ToddCoxeter object with 2 generators and 2 pairs>
   >>> tc.number_of_classes()                            # calculate number of classes
   1
   >>> tc.contains([0, 0, 0, 0], [0, 0])                 # check if 2 words are equal
   True
   >>> tc.word_to_class_index([0, 0, 0, 0])              # get the index of a class
   0
   >>> tc.standardize(ToddCoxeter.order.lex)             # standardize to lex order
   True


.. doctest::

   >>> from libsemigroups_pybind11 import congruence_kind, ToddCoxeter
   >>> tc = ToddCoxeter(congruence_kind.twosided)
   >>> tc.set_number_of_generators(4)
   >>> tc.add_pair([0, 0], [0])
   >>> tc.add_pair([1, 0], [1])
   >>> tc.add_pair([0, 1], [1])
   >>> tc.add_pair([2, 0], [2])
   >>> tc.add_pair([0, 2], [2])
   >>> tc.add_pair([3, 0], [3])
   >>> tc.add_pair([0, 3], [3])
   >>> tc.add_pair([1, 1], [0])
   >>> tc.add_pair([2, 3], [0])
   >>> tc.add_pair([2, 2, 2], [0])
   >>> tc.add_pair([1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2], [0])
   >>> tc.add_pair([1, 2, 1, 3, 1, 2, 1, 3, 1, 2, 1, 3, 1, 2, 1, 3, 
   ... 1, 2, 1, 3, 1, 2, 1, 3, 1, 2, 1, 3, 1, 2, 1, 3], [0])
   >>> tc.strategy(ToddCoxeter.strategy_options.hlt).standardize(False)
   <ToddCoxeter object with 4 generators and 12 pairs>
   >>> tc.lookahead(ToddCoxeter.lookahead_options.partial).save(False)
   <ToddCoxeter object with 4 generators and 12 pairs>
   >>> tc.number_of_classes()
   10752
   >>> tc.complete()
   True
   >>> tc.compatible()
   True
   >>> S = tc.quotient_froidure_pin()             # quotient semigroup
   >>> S.size()
   10752
   >>> S.number_of_idempotents()
   1
   >>> tc.standardize(ToddCoxeter.order.recursive)
   True
   >>> it = tc.normal_forms()
   >>> [next(it) for _ in range(10)]
   [[0], [1], [2], [2, 1], [1, 2], [1, 2, 1], [2, 2], [2, 2, 1], [2, 1, 2], [2, 1, 2, 1]]
   >>> tc.standardize(ToddCoxeter.order.lex)
   True
   >>> it = tc.normal_forms()
   >>> [next(it) for _ in range(10)]
   [[0], [0, 1], [0, 1, 2], [0, 1, 2, 1], [0, 1, 2, 1, 2], [0, 1, 2, 1, 2, 1], [0, 1, 2, 1, 2, 1, 2], [0, 1, 2, 1, 2, 1, 2, 1], [0, 1, 2, 1, 2, 1, 2, 1, 2], [0, 1, 2, 1, 2, 1, 2, 1, 2, 1]]

.. autosummary::
   ~ToddCoxeter
   ToddCoxeter.add_pair
   ToddCoxeter.class_index_to_word
   ToddCoxeter.compatible
   ToddCoxeter.complete
   ToddCoxeter.const_contains
   ToddCoxeter.contains
   ToddCoxeter.dead
   ToddCoxeter.empty
   ToddCoxeter.finished
   ToddCoxeter.froidure_pin_options
   ToddCoxeter.froidure_pin_policy
   ToddCoxeter.generating_pairs
   ToddCoxeter.has_parent_froidure_pin
   ToddCoxeter.has_quotient_froidure_pin
   ToddCoxeter.is_quotient_obviously_finite
   ToddCoxeter.is_quotient_obviously_infinite
   ToddCoxeter.is_standardized
   ToddCoxeter.kill
   ToddCoxeter.kind
   ToddCoxeter.less
   ToddCoxeter.lookahead
   ToddCoxeter.lookahead_options
   ToddCoxeter.lower_bound
   ToddCoxeter.next_lookahead
   ToddCoxeter.non_trivial_classes
   ToddCoxeter.normal_forms
   ToddCoxeter.number_of_classes
   ToddCoxeter.number_of_generating_pairs
   ToddCoxeter.number_of_generators
   ToddCoxeter.number_of_non_trivial_classes
   ToddCoxeter.order
   ToddCoxeter.parent_froidure_pin
   ToddCoxeter.quotient_froidure_pin
   ToddCoxeter.random_interval
   ToddCoxeter.random_shuffle_generating_pairs
   ToddCoxeter.report
   ToddCoxeter.report_every
   ToddCoxeter.report_why_we_stopped
   ToddCoxeter.reserve
   ToddCoxeter.run
   ToddCoxeter.run_for
   ToddCoxeter.run_until
   ToddCoxeter.save
   ToddCoxeter.set_number_of_generators
   ToddCoxeter.shrink_to_fit
   ToddCoxeter.sort_generating_pairs
   ToddCoxeter.standardize
   ToddCoxeter.stopped_by_predicate
   ToddCoxeter.strategy
   ToddCoxeter.strategy_options
   ToddCoxeter.timed_out
   ToddCoxeter.word_to_class_index
   ToddCoxeter.to_gap_string

.. autoclass:: ToddCoxeter
   :members:
