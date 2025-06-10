..
    Copyright (c) 2022-2024 J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

Presentation helpers
====================

This page contains the documentation for various helper functions for
manipulating presentations.

.. _pseudo_letter_type_helper:

.. _pseudo_word_type_helper:

Types
-----

In what follows, we use the following pseudo-types:

    - ``Letter`` for ``str | int``
    - ``Word`` for ``str | list[int]``

Recall that, once a presentation has been constructed, the type of its letters
and words are fixed.

Contents
--------

.. currentmodule:: libsemigroups_pybind11.presentation

.. autosummary::
    :signatures: short

    add_identity_rules
    add_inverse_rules
    add_rule
    add_rules
    add_zero_rules
    are_rules_sorted
    change_alphabet
    contains_rule
    first_unused_letter
    greedy_reduce_length
    greedy_reduce_length_and_number_of_gens
    is_strongly_compressible
    length
    longest_rule
    longest_rule_length
    longest_subword_reducing_length
    make_semigroup
    normalize_alphabet
    reduce_complements
    reduce_to_2_generators
    remove_duplicate_rules
    remove_redundant_generators
    remove_trivial_rules
    replace_subword
    replace_word
    replace_word_with_new_generator
    reverse
    shortest_rule
    shortest_rule_length
    sort_each_rule
    sort_rules
    strongly_compress
    throw_if_bad_inverses
    to_gap_string

Full API
--------

.. automodule:: libsemigroups_pybind11.presentation
    :members:
    :imported-members:
    :exclude-members: Presentation, InversePresentation
