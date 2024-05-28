.. Copyright (c) 2024 Joseph Edwards

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.


Aho-Corasick helper functions
=============================

This module contains various helper functions for the class :any:`AhoCorasick`.
These functions could be functions of :any:`AhoCorasick` but they only use
public member functions of :any:`AhoCorasick`, and so they are declared
as free functions instead.


.. doctest::

   >>> from libsemigroups_pybind11 import AhoCorasick, aho_corasick
   >>> # Construct an empty AhoCorasick
   >>> ac = AhoCorasick()

   >>> # Add words
   >>> aho_corasick.add_word(ac, [0, 1, 0, 1])
   4
   >>> aho_corasick.add_word(ac, [0, 1, 1, 0])
   6
   >>> aho_corasick.add_word(ac, [0, 1, 1, 0, 1])
   7
   >>> aho_corasick.add_word(ac, [0, 1, 1, 0, 0])
   8

   >>> # Can't add a word that already exists
   >>> aho_corasick.add_word(ac, [0, 1, 1, 0, 0])
   Traceback (most recent call last):
      ...
   LibsemigroupsError: the word [0, 1, 1, 0, 0] given by the arguments [first, last) already belongs to the trie

   >>> # Remove words
   >>> aho_corasick.rm_word(ac, [0, 1, 0, 1])
   4

   >>> # Can't remove a word that is not a terminal node in the trie
   >>> aho_corasick.rm_word(ac, [0, 1, 0, 1])
   Traceback (most recent call last):
      ...
   LibsemigroupsError: cannot remove the word [0, 1, 0, 1] given by the arguments [first, last), as it does not correspond to a node in the trie

   >>> # Traverse
   >>> aho_corasick.traverse_word(ac, 5, [0, 1])
   7
   >>> aho_corasick.traverse_word(ac, [0, 1, 0, 1, 1, 0])
   6


Contents
--------
.. currentmodule:: libsemigroups_pybind11.aho_corasick

.. autosummary::
   :nosignatures:

   add_word
   rm_word
   traverse_word

Full API
--------

.. automodule:: libsemigroups_pybind11.aho_corasick
   :members:
   :imported-members: