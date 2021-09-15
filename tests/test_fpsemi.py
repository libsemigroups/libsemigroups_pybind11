# -*- coding: utf-8 -*-
# pylint: disable=no-name-in-module, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name

# Copyright (c) 2021, J. D. Mitchell + Maria Tsalakou
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This file contains tests for FpSemigroup from libsemigroups_pybind11.
"""

import unittest
from fpsemi_intf import (
    check_validation,
    check_converters,
    check_initialisation,
    check_attributes,
    check_operators,
    check_running_and_state,
)
from libsemigroups_pybind11 import (
    FpSemigroup,
    ReportGuard,
    Transf,
    FroidurePin,
)


class TestFpSemigroup(unittest.TestCase):
    def test_all(self):
        ReportGuard(False)
        S = FpSemigroup()
        S.set_alphabet("abcde")
        S.set_identity("e")
        S.add_rule("cacac", "aacaa")
        S.add_rule("acaca", "ccacc")
        S.add_rule("ada", "bbcbb")
        S.add_rule("bcb", "aadaa")
        S.add_rule("aaaa", "e")
        S.add_rule("ab", "e")
        S.add_rule("ba", "e")
        S.add_rule("cd", "e")
        S.add_rule("dc", "e")
        S.run()

        self.assertEqual(S.number_of_rules(), 18)
        self.assertTrue(
            S.equal_to(
                "abbbbbbbbbbbadddddddddddddddacccccccccccc",
                "aaccccccccccccccccccccccccccc",
            )
        )

        self.assertFalse(
            S.equal_to("abbbbbbbbbbbadddddddddddddddacccccccccccc", "a")
        )

    def test_constructors(self):
        S = FroidurePin(Transf([1, 0, 1]), Transf([0, 0, 0]))
        T = FpSemigroup(S)
        self.assertFalse(T.has_froidure_pin())
        self.assertEqual(S.number_of_rules(), T.number_of_rules())
        self.assertEqual(
            list(S.rules()),
            [
                ([0, 1], [1]),
                ([1, 1], [1]),
                ([0, 0, 0], [0]),
                ([1, 0, 0], [1]),
            ],
        )
        self.assertEqual(
            list(T.rules()),
            [
                ("\x01\x02", "\x02"),
                ("\x02\x02", "\x02"),
                ("\x01\x01\x01", "\x01"),
                ("\x02\x01\x01", "\x02"),
            ],
        )

        self.assertEqual(T.size(), S.size())

    # Common tests for FpSemigroupInterface derived classes

    def test_validation(self):
        check_validation(self, FpSemigroup)

    def test_converters(self):
        check_converters(self, FpSemigroup)

    def test_initialisation(self):
        check_initialisation(self, FpSemigroup)

    def test_attributes(self):
        check_attributes(self, FpSemigroup)

    def test_operators(self):
        check_operators(self, FpSemigroup)

    def test_running_and_state(self):
        check_running_and_state(FpSemigroup)

    def test_related_algorithms(self):
        ReportGuard(False)
        S = FpSemigroup()
        S.set_alphabet("abcde")
        S.set_identity("e")
        S.add_rule("cacac", "aacaa")
        S.add_rule("acaca", "ccacc")
        S.add_rule("ada", "bbcbb")
        S.add_rule("bcb", "aadaa")
        S.add_rule("aaaa", "e")
        S.add_rule("ab", "e")
        S.add_rule("ba", "e")
        S.add_rule("cd", "e")
        S.add_rule("dc", "e")

        self.assertTrue(S.has_knuth_bendix())
        self.assertFalse(S.has_todd_coxeter())
        #  S is infinite and so it always has a KnuthBendix
        S.knuth_bendix().run()
        self.assertTrue(S.knuth_bendix().finished())
        self.assertEqual(
            S.knuth_bendix().active_rules(),
            [
                ("ab", "e"),
                ("ae", "a"),
                ("ba", "e"),
                ("bb", "aa"),
                ("be", "b"),
                ("ca", "bd"),
                ("cb", "ad"),
                ("cd", "e"),
                ("ce", "c"),
                ("da", "bc"),
                ("db", "ac"),
                ("dc", "e"),
                ("de", "d"),
                ("ea", "a"),
                ("eb", "b"),
                ("ec", "c"),
                ("ed", "d"),
                ("ee", "e"),
                ("aaa", "b"),
            ],
        )
        self.assertEqual(list(S.rules()), list(S.knuth_bendix().rules()))

        S = FpSemigroup()
        S.set_alphabet("01")

        S.add_rule("000", "0")
        S.add_rule("1111", "1")
        S.add_rule("01110", "00")
        S.add_rule("1001", "11")
        S.add_rule("001010101010", "00")

        self.assertEqual(S.size(), 240)
        self.assertFalse(S.has_froidure_pin())
        S.froidure_pin()
