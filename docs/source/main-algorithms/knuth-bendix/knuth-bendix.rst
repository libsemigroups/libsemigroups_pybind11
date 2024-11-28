.. Copyright (c) 2021-2024 J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

Knuth-Bendix
============

.. On this page we describe the functionality relating to the Knuth-Bendix
   algorithm for semigroups and monoids that is available in
   ``libsemigroups_pybind11``. This page contains a details of the methods
   of the class :py:class:`KnuthBendix`.  This class is used to represent a
   `string rewriting system <https://w.wiki/9Re>`_ defining a finitely presented
   monoid or semigroup.

   .. doctest::

      >>> from libsemigroups_pybind11 import KnuthBendix, Presentation, presentation, congruence_kind
      >>> p = Presentation("abc")
      >>> presentation.add_rule(p, "aaaa", "a")
      >>> presentation.add_rule(p, "bbbb", "b")
      >>> presentation.add_rule(p, "cccc", "c")
      >>> presentation.add_rule(p, "abab", "aaa")
      >>> presentation.add_rule(p, "bcbc", "bbb")
      >>> kb = KnuthBendix(congruence_kind.twosided, p)
      >>> not kb.confluent()
      True
      >>> kb.run()
      >>> kb.number_of_active_rules()
      31
      >>> kb.confluent()
      True

   Contents
   --------
   .. autosummary::
      :nosignatures:

      ~KnuthBendixRewriteTrie
      KnuthBendixRewriteTrie.active_rules
      KnuthBendixRewriteTrie.add_pair
      KnuthBendixRewriteTrie.batch_size
      KnuthBendixRewriteTrie.check_confluence_interval
      KnuthBendixRewriteTrie.confluent
      KnuthBendixRewriteTrie.confluent_known
      KnuthBendixRewriteTrie.contains
      KnuthBendixRewriteTrie.current_state
      KnuthBendixRewriteTrie.equal_to
      KnuthBendixRewriteTrie.generating_pairs
      KnuthBendixRewriteTrie.gilman_graph
      KnuthBendixRewriteTrie.gilman_graph_node_labels
      KnuthBendixRewriteTrie.kind
      KnuthBendixRewriteTrie.max_overlap
      KnuthBendixRewriteTrie.max_rules
      KnuthBendixRewriteTrie.normal_form
      KnuthBendixRewriteTrie.number_of_active_rules
      KnuthBendixRewriteTrie.number_of_classes
      KnuthBendixRewriteTrie.number_of_generating_pairs
      KnuthBendixRewriteTrie.number_of_inactive_rules
      KnuthBendixRewriteTrie.overlap_policy
      KnuthBendixRewriteTrie.presentation
      KnuthBendixRewriteTrie.rewrite
      KnuthBendixRewriteTrie.total_rules

   Full API
   --------

   .. autoclass:: KnuthBendixRewriteTrie
      :members:

   Methods inherited from Runner
   -----------------------------

   .. autoclass:: Runner
      :members:
      :noindex:
