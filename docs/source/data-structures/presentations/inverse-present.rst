.. Copyright (c) 2022-2024 J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

The InversePresentation class
=============================

.. autoclass:: InversePresentationStrings
   :doc-only:
   :class-doc-from: class

.. _pseudo_letter_type_inv_class:

.. _pseudo_word_type_inv_class:

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
   :signatures: short
   
   libsemigroups_pybind11.InversePresentation
   InversePresentationStrings.inverse
   InversePresentationStrings.inverses
   InversePresentationStrings.throw_if_bad_alphabet_rules_or_inverses

Full API
--------
.. currentmodule:: libsemigroups_pybind11

.. autofunction:: InversePresentation

.. currentmodule:: _libsemigroups_pybind11

.. autoclass:: InversePresentationStrings
   :members:
   :class-doc-from: class
   :no-doc:
