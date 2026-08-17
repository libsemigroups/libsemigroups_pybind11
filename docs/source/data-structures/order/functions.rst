..
    Copyright (c) 2024-2026 Joseph Edwards, J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Functions for orders
====================

This page contains the documentation for functions that compare words or
strings with respect to certain reduction orderings.

.. important:: 

   The following functions are deprecated in v1.5.0 and will be removed v2.0.0:

    * ``lexicographical_compare``, please use :any:`lex_cmp` instead.
    * ``recursive_path_compare`` please use :any:`rev_rpo_cmp` instead.
    * ``shortlex_compare`` please use :any:`lenlex_cmp` instead.

Contents
--------

.. autosummary::
    :signatures: short

    len_wt_lex_cmp
    lenlex_cmp
    lex_cmp
    rev_len_wt_lex_cmp
    rev_lenlex_cmp
    rev_lex_cmp
    rev_rpo_cmp
    rev_wr_cmp
    rev_wt_lenlex_cmp
    rev_wt_lex_cmp
    rpo_cmp
    wr_cmp
    wt_lenlex_cmp
    wt_lex_cmp

Full API
--------

.. autofunction:: len_wt_lex_cmp

.. autofunction:: lenlex_cmp

.. autofunction:: lex_cmp

.. autofunction:: rev_len_wt_lex_cmp

.. autofunction:: rev_lenlex_cmp

.. autofunction:: rev_lex_cmp

.. autofunction:: rev_rpo_cmp

.. autofunction:: rev_wr_cmp

.. autofunction:: rev_wt_lenlex_cmp

.. autofunction:: rev_wt_lex_cmp

.. autofunction:: rpo_cmp

.. autofunction:: wr_cmp

.. autofunction:: wt_lenlex_cmp

.. autofunction:: wt_lex_cmp
