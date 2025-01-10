.. Copyright (c) 2022-2024 M. T. Whyte

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Presentations for standard examples
===================================

This page contains the documentation for various examples of presentations for
finitely presented semigroups and monoids.

.. _pseudo_letter_type_inv_class:

.. _pseudo_word_type_inv_class:

Types
-----
In what follows, we use the following pseudo-types:

   * ``Letter`` for ``str | int``
   * ``Word`` for ``str | List[int]``

Recall that, once a presentation has been constructed, the type of its letters
and words are fixed.

Enums
-----

.. currentmodule:: libsemigroups_pybind11.fpsemigroup

.. autosummary::
   :nosignatures:

   author


Member functions
----------------

.. autosummary::
   :nosignatures:

   alternating_group
   brauer_monoid
   chinese_monoid
   cyclic_inverse_monoid
   dual_symmetric_inverse_monoid
   fibonacci_semigroup
   full_transformation_monoid
   hypo_plactic_monoid
   monogenic_semigroup
   not_renner_type_B_monoid
   not_renner_type_D_monoid
   not_symmetric_group
   order_preserving_cyclic_inverse_monoid
   order_preserving_monoid
   orientation_preserving_monoid
   orientation_preserving_reversing_monoid
   partial_brauer_monoid
   partial_isometries_cycle_graph_monoid
   partial_transformation_monoid
   partition_monoid
   plactic_monoid
   rectangular_band
   renner_type_B_monoid
   renner_type_D_monoid
   sigma_stylic_monoid
   singular_brauer_monoid
   special_linear_group_2
   stellar_monoid
   stylic_monoid
   symmetric_group
   symmetric_inverse_monoid
   temperley_lieb_monoid
   uniform_block_bijection_monoid
   zero_rook_monoid

Enum API
--------

.. automodule:: libsemigroups_pybind11.fpsemigroup
   :members: author

Full Function API
-----------------

.. automodule:: libsemigroups_pybind11.fpsemigroup
   :members:
   :exclude-members: author
   :imported-members:

