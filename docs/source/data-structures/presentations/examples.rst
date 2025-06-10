..
    Copyright (c) 2022-2024 M. T. Whyte

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11.presentation.examples

Presentations for standard examples
===================================

This page contains the documentation for various examples of presentations for
finitely presented semigroups and monoids.

.. _default-presentations:

Default presentations
---------------------

For some semigroups and monoids presented on this page, there are multiple
presentations. The section below defines some default functions; functions that
should be used when the particular relations aren't important, only the
semigroup or monoid they define.

.. autosummary::
    :signatures: short

    alternating_group
    brauer_monoid
    chinese_monoid
    cyclic_inverse_monoid
    dual_symmetric_inverse_monoid
    fibonacci_semigroup
    full_transformation_monoid
    hypo_plactic_monoid
    monogenic_semigroup
    motzkin_monoid
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
    sigma_plactic_monoid
    singular_brauer_monoid
    special_linear_group_2
    stellar_monoid
    stylic_monoid
    symmetric_group
    symmetric_inverse_monoid
    temperley_lieb_monoid
    uniform_block_bijection_monoid
    zero_rook_monoid

Specific presentations
----------------------

The functions documented below provide specific presentations for various
semigroups and monoids, usually accompanied by a reference to the source of the
presentation. There may be several presentations for any semigroup or monoid.

For each semigroup or monoid, there is a corresponding default function that is
documented in the :ref:`default-presentations` section.

.. autosummary::
    :signatures: short

    alternating_group_Moo97
    brauer_monoid_KM07
    chinese_monoid_CEKNH01
    cyclic_inverse_monoid_Fer22_a
    cyclic_inverse_monoid_Fer22_b
    dual_symmetric_inverse_monoid_EEF07
    fibonacci_semigroup_CRRT94
    full_transformation_monoid_Aiz58
    full_transformation_monoid_II74
    full_transformation_monoid_MW24_a
    full_transformation_monoid_MW24_b
    hypo_plactic_monoid_Nov00
    motzkin_monoid_PHL13
    not_renner_type_B_monoid_Gay18
    not_renner_type_D_monoid_God09
    not_symmetric_group_GKKL08
    order_preserving_cyclic_inverse_monoid_Fer22
    order_preserving_monoid_AR00
    orientation_preserving_monoid_AR00
    orientation_preserving_reversing_monoid_AR00
    partial_brauer_monoid_KM07
    partial_isometries_cycle_graph_monoid_FP22
    partial_transformation_monoid_MW24
    partial_transformation_monoid_Shu60
    partition_monoid_Eas11
    partition_monoid_HR05
    plactic_monoid_Knu70
    rectangular_band_ACOR00
    renner_type_B_monoid_Gay18
    renner_type_D_monoid_Gay18
    sigma_plactic_monoid_AHMNT24
    singular_brauer_monoid_MM07
    special_linear_group_2_CR80
    stellar_monoid_GH19
    stylic_monoid_AR22
    symmetric_group_Bur12
    symmetric_group_Car56
    symmetric_group_Moo97_a
    symmetric_group_Moo97_b
    symmetric_inverse_monoid_Sol04
    symmetric_inverse_monoid_MW24
    symmetric_inverse_monoid_Shu60
    temperley_lieb_monoid_Eas21
    uniform_block_bijection_monoid_Fit03
    zero_rook_monoid_Gay18

Full API
--------

.. automodule:: libsemigroups_pybind11.presentation.examples
    :members:
    :imported-members:
