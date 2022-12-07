# -*- coding: utf-8 -*-

# Copyright (c) 2022, M. T. Whyte
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name, unused-import, fixme

"""
This package provides the user-facing python part of libsemigroups_pybind11 for
various examples of presentations for finitely presented semigroups and monoids.
"""

from _libsemigroups_pybind11 import (
    author,
    symmetric_group,
    alternating_group,
    full_transformation_monoid,
    partial_transformation_monoid,
    symmetric_inverse_monoid,
    dual_symmetric_inverse_monoid,
    uniform_block_bijection_monoid,
    partition_monoid,
    brauer_monoid,
    rectangular_band,
    stellar_monoid,
    chinese_monoid,
    monogenic_semigroup,
    plactic_monoid,
    stylic_monoid,
    fibonacci_semigroup,
    temperley_lieb_monoid,
    singular_brauer_monoid,
    orientation_preserving_monoid,
    orientation_reversing_monoid
)

from _libsemigroups_pybind11 import make_presentation as make
