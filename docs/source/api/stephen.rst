.. Copyright (c) 2022, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

Stephen
=======

On this page we describe the functionality in ``libsemigroups_pybind11``
relating to Stephen’s procedure for finitely presented semigroups. This class
implements Stephen’s procedure for (possibly) constructing the word graph
(:py:class:`ActionDigraph`) corresponding to the left factors of a word in a
finitely presented semigroup. The algorithm implemented in this class is
closely related to the Todd-Coxeter algorithm (as implemented in
:py:class:`ToddCoxeter`) and originates in `Applications of automata theory to
presentations of monoids and inverse monoids`_ by J. B. Stephen.

.. _Applications of automata theory to presentations of monoids and inverse monoids: https://rb.gy/brsuvc

.. doctest::

   >>> from libsemigroups_pybind11 import Presentation, presentation, Stephen, action_digraph_helper, UNDEFINED
   >>> p = Presentation([0, 1])
   >>> presentation.add_rule_and_check(p, [0, 0, 0], [0])
   >>> presentation.add_rule_and_check(p, [1, 1, 1], [1])
   >>> presentation.add_rule_and_check(p, [0, 1, 0, 1], [0, 0])
   >>> s = Stephen(p)
   >>> s.set_word([1, 1, 0, 1]).run()
   >>> s.word_graph().number_of_nodes()
   7
   >>> s.word_graph() == action_digraph_helper.make(
   ... 7,
   ... [
   ...   [UNDEFINED, 1],
   ...   [UNDEFINED, 2],
   ...   [3, 1],
   ...   [4, 5],
   ...   [3, 6],
   ...   [6, 3],
   ...   [5, 4],
   ... ])
   True

Methods
~~~~~~~

.. autosummary::
   :nosignatures:

   ~Stephen
   Stephen.init
   Stephen.accept_state
   Stephen.presentation
   Stephen.set_word
   Stephen.word
   Stephen.word_graph

Methods inherited from Runner
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. autosummary::
   :nosignatures:

   Stephen.dead
   Stephen.finished
   Stephen.kill
   Stephen.report
   Stephen.report_every
   Stephen.report_why_we_stopped
   Stephen.run
   Stephen.run_for
   Stephen.run_until
   Stephen.running
   Stephen.started
   Stephen.stopped
   Stephen.stopped_by_predicate
   Stephen.timed_out


.. autoclass:: Stephen
   :members:
