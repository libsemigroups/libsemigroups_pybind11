.. Copyright (c) 2022, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

Kambites
========

On this page we describe the functionality relating to the algorithms for small
overlap monoids by Kambites_ and the authors of ``libsemigroups``.

.. _Kambites: https://doi.org/10.1016/j.jalgebra.2008.09.038

.. doctest::

   >>> from libsemigroups_pybind11 import Kambites, POSITIVE_INFINITY
   >>> k = Kambites()
   >>> k.set_alphabet("abcd")
   >>> k.add_rule("abcd", "accca");
   >>> k.number_of_pieces(0) == POSITIVE_INFINITY
   True
   >>> k.number_of_pieces(1)
   4
   >>> k.small_overlap_class()
   4
   >>> k.normal_form("bbcabcdaccaccabcddd")
   'bbcabcdaccaccabcddd'
   >>> k.equal_to("bbcabcdaccaccabcddd", "bbcabcdaccaccabcddd")
   True

.. .. autosummary::
..    :nosignatures:

..    ~Kambites
..    Kambites.add_rule
..    Kambites.add_rules
..    Kambites.alphabet
..    Kambites.char_to_uint
..    Kambites.dead
..    Kambites.equal_to
..    Kambites.finished
..    Kambites.froidure_pin
..    Kambites.has_froidure_pin
..    Kambites.has_identity
..    Kambites.identity
..    Kambites.inverses
..    Kambites.is_obviously_finite
..    Kambites.is_obviously_infinite
..    Kambites.kill
..    Kambites.normal_form
..    Kambites.number_of_normal_forms
..    Kambites.number_of_pieces
..    Kambites.number_of_rules
..    Kambites.report
..    Kambites.report_every
..    Kambites.report_why_we_stopped
..    Kambites.rules
..    Kambites.run
..    Kambites.run_for
..    Kambites.run_until
..    Kambites.running
..    Kambites.set_alphabet
..    Kambites.set_identity
..    Kambites.set_inverses
..    Kambites.size
..    Kambites.small_overlap_class
..    Kambites.started
..    Kambites.stopped
..    Kambites.stopped_by_predicate
..    Kambites.string_to_word
..    Kambites.timed_out
..    Kambites.uint_to_char
..    Kambites.validate_letter
..    Kambites.validate_word
..    Kambites.word_to_string

.. .. autoclass:: Kambites
..    :members:
