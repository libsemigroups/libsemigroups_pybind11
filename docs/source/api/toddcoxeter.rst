.. Copyright (c) 2020, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Todd-Coxeter
============

This class contains the main implementation of the Todd-Coxeter
algorithm for computing left, right, and 2-sided congruences on
semigroups and monoids.

This page contains a summary of the main member functions of the class
``ToddCoxeter``.

In this documentation we use the term "coset enumeration" to mean the
execution of (any version of) the Todd-Coxeter algorithm.

\sa congruence_type and tril.

.. code-block:: python

   from libsemigroups_pybind11 import congruence_type, ToddCoxeter
   tc = ToddCoxeter(congruence_type.left); # construct a left congruence
   tc.set_nr_generators(2);                # 2 generators
   tc.add_pair([0, 0], [0]);               # generator 0 squared is itself
   tc.add_pair([0], [1]);                  # generator 0 equals 1
   tc.strategy(policy::strategy::felsch);  # set the strategy
   tc.nr_classes();                        # calculate number of classes
   tc.contains([0, 0, 0, 0], [0, 0]);      # check if 2 words are equal
   tc.word_to_class_index([0, 0, 0, 0]);   # get the index of a class
   tc.standardize(order::lex);             # standardize to lex order


.. code-block:: python

   from libsemigroups_pybind11 import congruence_type, ToddCoxeter
   tc = ToddCoxeter(congruence_type.twosided)
   tc.set_nr_generators(4)
   tc.add_pair([0, 0], [0])
   tc.add_pair([1, 0], [1])
   tc.add_pair([0, 1], [1])
   tc.add_pair([2, 0], [2])
   tc.add_pair([0, 2], [2])
   tc.add_pair([3, 0], [3])
   tc.add_pair([0, 3], [3])
   tc.add_pair([1, 1], [0])
   tc.add_pair([2, 3], [0])
   tc.add_pair([2, 2, 2], [0])
   tc.add_pair([1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2], [0])
   tc.add_pair([1, 2, 1, 3, 1, 2, 1, 3, 1, 2, 1, 3, 1, 2, 1, 3,
                1, 2, 1, 3, 1, 2, 1, 3, 1, 2, 1, 3, 1, 2, 1, 3],
               [0])
   tc.strategy(ToddCoxeter.strategy_options.hlt).standardize(False)
   tc.lookahead(ToddCoxeter.lookahead_options.partial).save(False)
   tc.nr_classes()  # 10752
   tc.complete()   # true
   tc.compatible() # true
   S = tc.quotient_froidure_pin()  # FroidurePin<TCE>
   S.size()                            # 10752
   S.nr_idempotents()                  # 1
   tc.standardize(ToddCoxeter.order.recursive)
   it = tc.normal_forms()
   [next(it) for _ in range(10)]
   # {{0},
   #  {1},
   #  {2},
   #  {2, 1},
   #  {1, 2},
   #  {1, 2, 1},
   #  {2, 2},
   #  {2, 2, 1},
   #  {2, 1, 2},
   #  {2, 1, 2, 1}};
   tc.standardize(ToddCoxeter.order.lex)
   it = tc.normal_forms()
   [next(it) for _ in range(10)]
   # {{0},
   #  {0, 1},
   #  {0, 1, 2},
   #  {0, 1, 2, 1},
   #  {0, 1, 2, 1, 2},
   #  {0, 1, 2, 1, 2, 1},
   #  {0, 1, 2, 1, 2, 1, 2},
   #  {0, 1, 2, 1, 2, 1, 2, 1},
   #  {0, 1, 2, 1, 2, 1, 2, 1, 2},
   #  {0, 1, 2, 1, 2, 1, 2, 1, 2, 1}};

.. autoclass:: ToddCoxeter
   :members:
