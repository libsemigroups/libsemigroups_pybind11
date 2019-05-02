# -*- coding: utf-8 -*-
# pylint: disable=no-name-in-module, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This file contains tests for Congruence from libsemigroups_pybind11.
"""

import unittest
from libsemigroups_pybind11 import (
    FpSemigroup,
    Congruence,
    congruence_kind,
    ReportGuard,
)


class TestFpSemigroup(unittest.TestCase):
    def test_018(self):
        ReportGuard(False)
        twosided = congruence_kind.twosided
        S = FpSemigroup()
        S.set_alphabet(3)
        S.add_rule([0, 0], [0, 0])
        S.add_rule([0, 1], [1, 0])
        S.add_rule([0, 2], [2, 0])
        S.add_rule([0, 0], [0])
        S.add_rule([0, 2], [0])
        S.add_rule([2, 0], [0])
        S.add_rule([1, 0], [0, 1])
        S.add_rule([1, 1], [1, 1])
        S.add_rule([1, 2], [2, 1])
        S.add_rule([1, 1, 1], [1])
        S.add_rule([1, 2], [1])
        S.add_rule([2, 1], [1])

        self.assertTrue(S.is_obviously_infinite())

        cong = Congruence(twosided, S)
        cong.add_pair([0], [1])
        self.assertFalse(cong.finished())
        self.assertEqual(cong.number_of_non_trivial_classes(), 1)
        self.assertTrue(cong.finished())
        self.assertEqual(
            cong.non_trivial_classes(0), [[0], [1], [0, 1], [1, 1], [0, 1, 1]]
        )
