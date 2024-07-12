.. Copyright (c) 2022-2024 J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

The Presentation class
======================

.. autoclass:: PresentationStrings
   :doc-only:
   :class-doc-from: class

.. _pseudo_letter_type_class:

.. _pseudo_word_type_class:

Types
-----
In what follows, we use the following pseudo-types:

   * ``Letter`` for ``str | int``
   * ``Word`` for ``str | List[int]``

Recall that, once a presentation has been constructed, the type of its letters
and words are fixed.

Contents
--------

.. autosummary::
   :nosignatures:
   
   PresentationStrings.add_generator
   PresentationStrings.alphabet
   PresentationStrings.alphabet_from_rules
   PresentationStrings.contains_empty_word
   PresentationStrings.in_alphabet
   PresentationStrings.index
   PresentationStrings.init
   PresentationStrings.letter
   PresentationStrings.remove_generator
   PresentationStrings.rules
   PresentationStrings.validate
   PresentationStrings.validate_alphabet
   PresentationStrings.validate_letter
   PresentationStrings.validate_rules

Full API
--------

.. autoclass:: PresentationStrings
   :members:
   :class-doc-from: init
