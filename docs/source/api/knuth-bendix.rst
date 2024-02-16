.. Copyright (c) 2021, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

Knuth-Bendix
============

On this page we describe the functionality relating to the Knuth-Bendix
algorithm for semigroups and monoids that is available in
``libsemigroups_pybind11``. This page contains a details of the methods
of the class :py:class:`KnuthBendix`.  This class is used to represent a
`string rewriting system <https://w.wiki/9Re>`_ defining a finitely presented
monoid or semigroup.

.. doctest::

   >>> from libsemigroups_pybind11 import KnuthBendix
   >>> kb = KnuthBendix()
   >>> kb.set_alphabet("abc")
   >>> kb.add_rule("aaaa", "a")
   >>> kb.add_rule("bbbb", "b")
   >>> kb.add_rule("cccc", "c")
   >>> kb.add_rule("abab", "aaa")
   >>> kb.add_rule("bcbc", "bbb")
   >>> not kb.confluent()
   True
   >>> kb.run()
   >>> kb.number_of_active_rules()
   31
   >>> kb.confluent()
   True

.. autosummary::
   :nosignatures:

   ~KnuthBendixRewriteTrie
   KnuthBendixRewriteTrie.active_rules
   KnuthBendixRewriteTrie.add_rule
   KnuthBendixRewriteTrie.add_rules
   KnuthBendixRewriteTrie.alphabet
   KnuthBendixRewriteTrie.char_to_uint
   KnuthBendixRewriteTrie.check_confluence_interval
   KnuthBendixRewriteTrie.confluent
   KnuthBendixRewriteTrie.contains_empty_string
   KnuthBendixRewriteTrie.dead
   KnuthBendixRewriteTrie.equal_to
   KnuthBendixRewriteTrie.finished
   KnuthBendixRewriteTrie.froidure_pin
   KnuthBendixRewriteTrie.gilman_digraph
   KnuthBendixRewriteTrie.has_froidure_pin
   KnuthBendixRewriteTrie.identity
   KnuthBendixRewriteTrie.inverses
   KnuthBendixRewriteTrie.is_obviously_finite
   KnuthBendixRewriteTrie.is_obviously_infinite
   KnuthBendixRewriteTrie.kill
   KnuthBendixRewriteTrie.knuth_bendix_by_overlap_length
   KnuthBendixRewriteTrie.max_overlap
   KnuthBendixRewriteTrie.max_rules
   KnuthBendixRewriteTrie.normal_form
   KnuthBendixRewriteTrie.normal_forms
   KnuthBendixRewriteTrie.normal_forms_alphabet
   KnuthBendixRewriteTrie.number_of_active_rules
   KnuthBendixRewriteTrie.number_of_normal_forms
   KnuthBendixRewriteTrie.number_of_rules
   KnuthBendixRewriteTrie.overlap
   KnuthBendixRewriteTrie.overlap_policy
   KnuthBendixRewriteTrie.report
   KnuthBendixRewriteTrie.report_every
   KnuthBendixRewriteTrie.report_why_we_stopped
   KnuthBendixRewriteTrie.rules
   KnuthBendixRewriteTrie.running
   KnuthBendixRewriteTrie.run
   KnuthBendixRewriteTrie.run_for
   KnuthBendixRewriteTrie.run_until
   KnuthBendixRewriteTrie.set_alphabet
   KnuthBendixRewriteTrie.set_identity
   KnuthBendixRewriteTrie.set_inverses
   KnuthBendixRewriteTrie.size
   KnuthBendixRewriteTrie.started
   KnuthBendixRewriteTrie.stopped_by_predicate
   KnuthBendixRewriteTrie.string_to_word
   KnuthBendixRewriteTrie.timed_out
   KnuthBendixRewriteTrie.to_gap_string
   KnuthBendixRewriteTrie.uint_to_char
   KnuthBendixRewriteTrie.validate_letter
   KnuthBendixRewriteTrie.validate_word
   KnuthBendixRewriteTrie.word_to_string

.. autoclass:: KnuthBendixRewriteTrie
   :members:
