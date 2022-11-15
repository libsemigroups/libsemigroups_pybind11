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

.. code-block:: python

   kb = KnuthBendix()
   kb.set_alphabet("abc")

   kb.add_rule("aaaa", "a")
   kb.add_rule("bbbb", "b")
   kb.add_rule("cccc", "c")
   kb.add_rule("abab", "aaa")
   kb.add_rule("bcbc", "bbb")

   not kb.confluent()           # True
   kb.run()
   kb.number_of_active_rules()  # 31
   kb.confluent()               # True

.. autosummary::
   :nosignatures:

   ~KnuthBendix
   KnuthBendix.active_rules
   KnuthBendix.add_rule
   KnuthBendix.add_rules
   KnuthBendix.alphabet
   KnuthBendix.char_to_uint
   KnuthBendix.check_confluence_interval
   KnuthBendix.confluent
   KnuthBendix.contains_empty_string
   KnuthBendix.dead
   KnuthBendix.equal_to
   KnuthBendix.finished
   KnuthBendix.froidure_pin
   KnuthBendix.gilman_digraph
   KnuthBendix.has_froidure_pin
   KnuthBendix.identity
   KnuthBendix.inverses
   KnuthBendix.is_obviously_finite
   KnuthBendix.is_obviously_infinite
   KnuthBendix.kill
   KnuthBendix.knuth_bendix_by_overlap_length
   KnuthBendix.max_overlap
   KnuthBendix.max_rules
   KnuthBendix.normal_form
   KnuthBendix.normal_forms
   KnuthBendix.normal_forms_alphabet
   KnuthBendix.number_of_active_rules
   KnuthBendix.number_of_normal_forms
   KnuthBendix.number_of_rules
   KnuthBendix.overlap
   KnuthBendix.overlap_policy
   KnuthBendix.report
   KnuthBendix.report_every
   KnuthBendix.report_why_we_stopped
   KnuthBendix.rules
   KnuthBendix.running
   KnuthBendix.run
   KnuthBendix.run_for
   KnuthBendix.run_until
   KnuthBendix.set_alphabet
   KnuthBendix.set_identity
   KnuthBendix.set_inverses
   KnuthBendix.size
   KnuthBendix.started
   KnuthBendix.stopped_by_predicate
   KnuthBendix.string_to_word
   KnuthBendix.timed_out
   KnuthBendix.to_gap_string
   KnuthBendix.uint_to_char
   KnuthBendix.validate_letter
   KnuthBendix.validate_word
   KnuthBendix.word_to_string

.. autoclass:: KnuthBendix
   :members:
