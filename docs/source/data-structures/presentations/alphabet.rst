..
    Copyright (c) 2026 J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

The Alphabet class
==================

.. autoclass:: Alphabet
    :doc-only:

.. Types

    -----

    In what follows, we use the following pseudo-types:

        - ``Letter`` for ``str | int``
        - ``Word`` for ``str | list[int]``

    Recall that, once an alphabet has been constructed, the type of its letters and
    words are fixed.

Contents
--------

.. autosummary::
    :signatures: short

    ~Alphabet
    Alphabet.add_letter
    Alphabet.copy
    Alphabet.contains
    Alphabet.empty
    Alphabet.index
    Alphabet.init
    Alphabet.letter
    Alphabet.letters
    Alphabet.remove_letter
    Alphabet.throw_if_duplicate_letters
    Alphabet.throw_if_letter_not_in_alphabet

Full API
--------

.. autoclass:: Alphabet
    :class-doc-from: init
    :members:

