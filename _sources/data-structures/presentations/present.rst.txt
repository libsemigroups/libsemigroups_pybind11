..
    Copyright (c) 2022-2024 J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

The Presentation class
======================

.. autoclass:: Presentation
    :doc-only:

.. _pseudo_letter_type_class:

.. _pseudo_word_type_class:

Types
-----

In what follows, we use the following pseudo-types:

    - ``Letter`` for ``str | int``
    - ``Word`` for ``str | list[int]``

Recall that, once a presentation has been constructed, the type of its letters
and words are fixed.

Contents
--------

.. autosummary::
    :signatures: short

    ~Presentation
    Presentation.add_generator
    Presentation.alphabet
    Presentation.alphabet_from_rules
    Presentation.contains_empty_word
    Presentation.copy
    Presentation.in_alphabet
    Presentation.index
    Presentation.init
    Presentation.letter
    Presentation.remove_generator
    Presentation.rules
    Presentation.throw_if_alphabet_has_duplicates
    Presentation.throw_if_bad_alphabet_or_rules
    Presentation.throw_if_bad_rules
    Presentation.throw_if_letter_not_in_alphabet

Full API
--------

.. autoclass:: Presentation
    :class-doc-from: init
    :members:
