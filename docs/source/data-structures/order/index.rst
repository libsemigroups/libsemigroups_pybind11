..
    Copyright (c) 2024 Joseph Edwards

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Orders
======

This page contains the documentation for several functions for comparing words
or strings with respect to certain reduction orderings.

.. seealso::
    
    - :any:`Order`
    - :any:`Alphabet`

.. important:: 

   The following functions are deprecated in v1.5.0 and will be removed v2.0.0:

    * ``lexicographical_compare``, please use :any:`lex_cmp` instead.
    * ``recursive_path_compare`` please use :any:`rev_rpo_cmp` instead.
    * ``shortlex_compare`` please use :any:`lenlex_cmp` instead.

Contents
--------

.. autosummary::
    :signatures: short

    lenlex_cmp
    lex_cmp
    rev_rpo_cmp
    rpo_cmp
    wr_cmp

Full API
--------

.. autofunction:: lenlex_cmp

.. autofunction:: lex_cmp

.. autofunction:: rev_rpo_cmp

.. autofunction:: rpo_cmp

.. autofunction:: wr_cmp
