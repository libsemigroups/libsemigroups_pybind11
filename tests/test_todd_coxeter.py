# -*- coding: utf-8 -*-
# pylint: disable=no-name-in-module, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name

# The following should only be temporary!
# pylint: disable=fixme

# Copyright (c) 2021, J. D. Mitchell + Maria Tsalakou
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for the ToddCoxeter class.
"""

import unittest
from datetime import timedelta

from libsemigroups_pybind11 import (
    ToddCoxeter,
    ReportGuard,
    congruence_kind,
    tril,
    FroidurePinTransf1,
    Transf1,
    KnuthBendix,
)

strategy = ToddCoxeter.strategy_options
fpp = ToddCoxeter.froidure_pin_options
order = ToddCoxeter.order


class TestToddCoxeter(unittest.TestCase):
    def test_constructors(self):
        ToddCoxeter(congruence_kind.left)
        ToddCoxeter(congruence_kind.right)
        ToddCoxeter(congruence_kind.twosided)

        with self.assertRaises(TypeError):
            ToddCoxeter(45)
        with self.assertRaises(TypeError):
            ToddCoxeter("lft")
        with self.assertRaises(TypeError):
            ToddCoxeter(
                congruence_kind.twosided,
                congruence_kind.left,
                congruence_kind.right,
            )

        S = FroidurePinTransf1()
        S.add_generator(Transf1.make([0, 0, 1, 2, 3]))
        with self.assertRaises(TypeError):
            ToddCoxeter(S)

        ToddCoxeter(congruence_kind.twosided, S)

        K = KnuthBendix()
        with self.assertRaises(RuntimeError):
            ToddCoxeter(congruence_kind.left, K)
        K.set_alphabet("a")
        try:
            ToddCoxeter(congruence_kind.left, K)
        except:
            self.fail("unexpected exception thrown")

        T = ToddCoxeter(congruence_kind.left)
        with self.assertRaises(ValueError):
            ToddCoxeter(congruence_kind.left, T)
        T.set_number_of_generators(1)
        with self.assertRaises(RuntimeError):
            ToddCoxeter(congruence_kind.right, T)
        with self.assertRaises(RuntimeError):
            ToddCoxeter(congruence_kind.twosided, T)
        ToddCoxeter(congruence_kind.left, T)

        T = ToddCoxeter(congruence_kind.right)
        with self.assertRaises(ValueError):
            ToddCoxeter(congruence_kind.right, T)
        T.set_number_of_generators(1)
        with self.assertRaises(RuntimeError):
            ToddCoxeter(congruence_kind.left, T)
        with self.assertRaises(RuntimeError):
            ToddCoxeter(congruence_kind.twosided, T)

        ToddCoxeter(congruence_kind.right, T)

        T = ToddCoxeter(congruence_kind.twosided)
        with self.assertRaises(ValueError):
            ToddCoxeter(congruence_kind.twosided, T)
        T.set_number_of_generators(1)
        ToddCoxeter(congruence_kind.left, T)
        ToddCoxeter(congruence_kind.right, T)
        ToddCoxeter(congruence_kind.twosided, T)

        tc = ToddCoxeter(congruence_kind.left)
        tc.set_number_of_generators(2)
        with self.assertRaises(RuntimeError):
            tc.add_pair([0, 0, 1, 0], [1, 1, 2])

    def test_attributes(self):
        ReportGuard(False)
        tc = ToddCoxeter(congruence_kind.left)
        tc.set_number_of_generators(1)
        tc.add_pair([0, 0, 0, 0, 0, 0], [0, 0, 0])
        self.assertEqual(tc.number_of_classes(), 5)
        self.assertTrue(tc.contains([0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0]))
        self.assertFalse(tc.contains([0, 0, 0], [0, 0]))
        self.assertEqual(tc.const_contains([0, 0, 0], [0, 0]), tril.false)
        self.assertEqual(tc.kind(), congruence_kind.left)
        self.assertFalse(tc.empty())
        self.assertEqual(tc.class_index_to_word(1), [0, 0])
        self.assertEqual(tc.word_to_class_index([0, 0]), 1)
        self.assertFalse(tc.has_parent_froidure_pin())
        self.assertEqual(tc.number_of_generators(), 1)
        self.assertEqual(tc.number_of_generating_pairs(), 1)
        self.assertTrue(tc.less([0], [0, 0]))

        S = FroidurePinTransf1()
        S.add_generators([Transf1.make([1, 2, 2]), Transf1.make([2, 0, 1])])
        tc = ToddCoxeter(congruence_kind.twosided, S)
        self.assertEqual(tc.number_of_classes(), 24)
        self.assertTrue(tc.has_parent_froidure_pin())
        self.assertEqual(tc.parent_froidure_pin().size(), 24)
        self.assertEqual(tc.quotient_froidure_pin().size(), 24)

        K = KnuthBendix()
        K.set_alphabet("a")
        K.add_rule("aaaa", "aa")
        tc = ToddCoxeter(congruence_kind.left, K)
        self.assertEqual(tc.number_of_classes(), 3)

        tc = ToddCoxeter(congruence_kind.twosided)
        tc.set_number_of_generators(1)
        self.assertTrue(tc.is_quotient_obviously_infinite())
        self.assertFalse(tc.is_quotient_obviously_finite())
        tc.add_pair([0, 0, 0], [0])
        self.assertFalse(tc.is_quotient_obviously_infinite())
        tc.run()
        self.assertTrue(tc.is_quotient_obviously_finite())

    def test_operators(self):
        ReportGuard(False)
        tc = ToddCoxeter(congruence_kind.left)
        tc.set_number_of_generators(2)
        tc.add_pair([0, 0, 0, 0], [1])
        tc.add_pair([1, 1, 1, 1], [1])
        tc.run()
        tc = ToddCoxeter(congruence_kind.left)
        tc.set_number_of_generators(1)
        tc.add_pair([0, 0, 0], [0])
        tc.run_for(timedelta(microseconds=2))
        tc = ToddCoxeter(congruence_kind.right)
        tc.set_number_of_generators(1)
        tc.add_pair([0, 0, 0, 0, 0, 0, 0, 0], [0])
        tc.run_until(
            lambda: tc.const_contains([0, 0, 0, 0, 0, 0, 0, 0], [0])
            == tril.true
        )
        self.assertTrue(tc.stopped_by_predicate())
        self.assertFalse(tc.finished())

        tc = ToddCoxeter(congruence_kind.left)
        tc.set_number_of_generators(2)
        tc.add_pair([0, 0, 0, 0], [1])
        tc.add_pair([1, 0], [0, 1])
        tc.add_pair([1, 1, 1, 1], [0])
        tc.run_for(timedelta(microseconds=1))
        # If this is run on an incredibly fast computer, this
        # test of timed_out() may not work.
        self.assertTrue(tc.timed_out())
        tc.run()
        self.assertFalse(tc.timed_out())

    def test_settings(self):
        ReportGuard(False)
        tc = ToddCoxeter(congruence_kind.left)
        tc.set_number_of_generators(1)
        tc.add_pair([0, 0, 0, 0], [0, 0])
        tc.reserve(10)
        tc.run()

        self.assertFalse(tc.is_standardized())
        tc.standardize(order.lex)
        self.assertTrue(tc.is_standardized())
        tc.standardize(order.shortlex)
        tc.standardize(order.recursive)
        tc.standardize(True)
        tc.standardize(False)
        tc.standardize(1)
        with self.assertRaises(TypeError):
            tc.standardize("shooortlex")

        S = FroidurePinTransf1()
        S.add_generators(
            [Transf1.make([3, 1, 2, 1, 2]), Transf1.make([1, 1, 1, 2, 2])]
        )
        tc = ToddCoxeter(congruence_kind.twosided, S)
        tc.froidure_pin_policy()
        tc.froidure_pin_policy(fpp.none)
        self.assertEqual(tc.froidure_pin_policy(), fpp.none)
        tc.froidure_pin_policy(fpp.use_relations)
        self.assertEqual(tc.froidure_pin_policy(), fpp.use_relations)
        tc.froidure_pin_policy(fpp.use_cayley_graph)
        self.assertEqual(tc.froidure_pin_policy(), fpp.use_cayley_graph)
        with self.assertRaises(TypeError):
            tc.froidure_pin_policy(1)
        with self.assertRaises(TypeError):
            tc.froidure_pin_policy("userelations")

        tc = ToddCoxeter(congruence_kind.left)
        tc.set_number_of_generators(2)
        tc.add_pair([0, 0, 0, 0], [0])
        tc.add_pair([1, 1, 1, 1], [0])
        with self.assertRaises(TypeError):
            tc.strategy("feelsch")
        with self.assertRaises(TypeError):
            tc.strategy(1)
        tc.strategy(strategy.felsch)
        self.assertEqual(tc.strategy(), strategy.felsch)
        tc.strategy(strategy.hlt)
        self.assertEqual(tc.strategy(), strategy.hlt)
        tc.strategy(strategy.random)
        self.assertEqual(tc.strategy(), strategy.random)

    def test_000_iterators(self):
        ReportGuard(False)
        tc = ToddCoxeter(congruence_kind.left)
        tc.set_number_of_generators(2)
        tc.add_pair([0, 0, 0, 0], [0])
        tc.add_pair([1, 1, 1, 1], [1])
        tc.add_pair([0, 1], [1, 0])
        self.assertEqual(
            list(tc.generating_pairs()),
            [([0, 0, 0, 0], [0]), ([1, 1, 1, 1], [1]), ([0, 1], [1, 0])],
        )
        self.assertEqual(
            list(tc.normal_forms()),
            [
                [0],
                [1],
                [0, 0],
                [1, 0],
                [1, 1],
                [0, 0, 0],
                [1, 0, 0],
                [1, 1, 0],
                [1, 1, 1],
                [1, 0, 0, 0],
                [1, 1, 0, 0],
                [1, 1, 1, 0],
                [1, 1, 0, 0, 0],
                [1, 1, 1, 0, 0],
                [1, 1, 1, 0, 0, 0],
            ],
        )
        S = FroidurePinTransf1()
        S.add_generators(
            [Transf1.make([1, 3, 4, 2, 3]), Transf1.make([3, 2, 1, 3, 3])]
        )
        tc = ToddCoxeter(congruence_kind.left, S)
        tc.add_pair(
            S.factorisation(Transf1.make([3, 4, 4, 4, 4])),
            S.factorisation(Transf1.make([3, 1, 3, 3, 3])),
        )
        self.assertEqual(
            list(tc.non_trivial_classes()),
            [
                [
                    [0, 0, 1],
                    [1, 0, 1],
                    [0, 0, 0, 1],
                    [0, 1, 0, 1],
                    [1, 0, 0, 1],
                    [0, 0, 0, 0, 1],
                    [0, 0, 1, 0, 1],
                    [1, 0, 0, 0, 1],
                    [1, 0, 1, 0, 1],
                    [0, 0, 0, 1, 0, 1],
                    [0, 1, 0, 0, 0, 1],
                    [0, 1, 0, 1, 0, 1],
                    [1, 0, 0, 1, 0, 1],
                    [0, 0, 0, 0, 1, 0, 1],
                    [0, 0, 1, 0, 0, 0, 1],
                    [1, 0, 0, 0, 1, 0, 1],
                    [0, 1, 0, 0, 0, 1, 0, 1],
                    [0, 0, 1, 0, 0, 0, 1, 0, 0],
                    [0, 0, 1, 0, 0, 0, 1, 0, 1],
                    [0, 1, 0, 0, 0, 1, 1, 0, 0],
                ]
            ],
        )

    # The following are some tests adapted from the libsemigroups test file

    def test_020(self):
        ReportGuard(False)
        tc = ToddCoxeter(congruence_kind.twosided)
        tc.set_number_of_generators(1)
        tc.strategy(strategy.hlt)
        self.assertTrue(tc.contains([0, 0], [0, 0]))
        self.assertFalse(tc.contains([0, 0], [0]))

    def test_021(self):
        ReportGuard(False)
        tc = ToddCoxeter(congruence_kind.twosided)
        tc.set_number_of_generators(5)
        with self.assertRaises(RuntimeError):
            tc.run()

    def test_033(self):
        ReportGuard(False)
        tc1 = ToddCoxeter(congruence_kind.twosided)
        tc1.set_number_of_generators(2)
        tc1.add_pair([0, 0, 0], [0])
        tc1.add_pair([0], [1, 1])
        self.assertEqual(tc1.number_of_classes(), 5)
        tc2 = ToddCoxeter(congruence_kind.left, tc1)
        tc2.next_lookahead(1)
        tc2.report_every(timedelta(microseconds=1))
        self.assertFalse(tc2.empty())
        tc2.add_pair([0], [0, 0])
        self.assertEqual(tc2.number_of_classes(), 3)

    def test_036(self):
        ReportGuard(False)
        S = FroidurePinTransf1()
        S.add_generators(
            [Transf1.make([1, 3, 4, 2, 3]), Transf1.make([3, 2, 1, 3, 3])]
        )
        tc = ToddCoxeter(congruence_kind.twosided)
        tc.set_number_of_generators(2)
        tc.add_pair([0], [1])
        tc.add_pair([0, 0], [0])
        self.assertEqual(tc.number_of_classes(), 1)

    def test_096(self):
        ReportGuard(False)
        tc = ToddCoxeter(congruence_kind.twosided)
        tc.set_number_of_generators(2)
        tc.add_pair([0], [1])
        tc.add_pair([0, 0], [0])
        tc.strategy(strategy.felsch)
        self.assertEqual(tc.strategy(), strategy.felsch)
        self.assertFalse(tc.complete())
        self.assertTrue(tc.compatible())
        self.assertEqual(tc.number_of_classes(), 1)
        self.assertEqual(list(tc.normal_forms()), [[0]])
        self.assertTrue(tc.complete())
        self.assertTrue(tc.compatible())

        copy = ToddCoxeter(tc)
        self.assertEqual(copy.number_of_generators(), 2)
        self.assertEqual(copy.number_of_generating_pairs(), 2)
        self.assertTrue(copy.finished())
        self.assertEqual(copy.number_of_classes(), 1)
        self.assertTrue(copy.complete())
        self.assertTrue(copy.compatible())
