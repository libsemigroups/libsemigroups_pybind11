# -*- coding: utf-8 -*-
# pylint: disable=no-name-in-module, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for FroidurePin
"""

import unittest
from datetime import timedelta
from runner import check_runner

from libsemigroups_pybind11 import (
    ReportGuard,
    Transf16,
    Transf1,
    Transf2,
    Transf4,
    FroidurePinTransf16,
    FroidurePinTransf1,
    FroidurePinTransf2,
    FroidurePinTransf4,
    PPerm16,
    PPerm1,
    PPerm2,
    PPerm4,
    FroidurePinPPerm16,
    FroidurePinPPerm1,
    FroidurePinPPerm2,
    FroidurePinPPerm4,
    FroidurePinTCE,
    ToddCoxeter,
    congruence_kind,
    FroidurePinKBE,
    KnuthBendix,
)

FroidurePin = {
    Transf16: FroidurePinTransf16,
    Transf1: FroidurePinTransf1,
    Transf2: FroidurePinTransf2,
    Transf4: FroidurePinTransf4,
    PPerm16: FroidurePinPPerm16,
    PPerm1: FroidurePinPPerm1,
    PPerm2: FroidurePinPPerm2,
    PPerm4: FroidurePinPPerm4,
}


def check_constructors(self, T, coll):
    ReportGuard(False)
    # default constructor
    S = FroidurePin[T]()
    S.add_generators(coll)

    # copy constructor
    U = FroidurePin[T](S)
    self.assertFalse(S is U)
    self.assertEqual(S.number_of_generators(), U.number_of_generators())
    self.assertEqual(S.current_size(), U.current_size())


def check_generators(self, T, coll):
    ReportGuard(False)
    S = FroidurePin[T]()
    S.add_generators(coll)
    for i, x in enumerate(coll):
        self.assertEqual(S.generator(i), x)

    with self.assertRaises(RuntimeError):
        S.generator(len(coll))

    U = FroidurePin[T]()
    for x in coll:
        U.add_generator(x)
    self.assertEqual(S.number_of_generators(), U.number_of_generators())
    self.assertEqual(S.size(), U.size())

    S.closure(coll)
    self.assertEqual(S.number_of_generators(), len(coll))

    U = S.copy_closure(coll)
    self.assertEqual(U.number_of_generators(), S.number_of_generators())
    self.assertEqual(U.size(), S.size())
    self.assertFalse(U is S)

    U = S.copy_add_generators(coll)
    self.assertEqual(U.number_of_generators(), 2 * S.number_of_generators())
    self.assertEqual(U.size(), S.size())
    self.assertFalse(U is S)


def check_settings(self, S):
    ReportGuard(False)
    self.assertFalse(S.immutable())
    S.immutable(False)
    self.assertEqual(S.batch_size(), 8192)
    S.batch_size(S.batch_size())
    self.assertEqual(S.concurrency_threshold(), 823543)
    S.concurrency_threshold(S.concurrency_threshold())
    S.max_threads(2)
    self.assertEqual(S.max_threads(), 2)
    S.reserve(100)


def check_mem_compare(self, S):
    ReportGuard(False)

    with self.assertRaises(RuntimeError):
        S.current_position([0, 0, 0, 0, 0, 0, 0, S.number_of_generators(), 1])
    with self.assertRaises(RuntimeError):
        S.current_position(S.number_of_generators())

    S.run()
    self.assertTrue(all(S.contains(x) for x in S))

    self.assertEqual([S.position(x) for x in S], list(range(S.size())))
    #  not implemented
    # self.assertEqual(
    #     [S.position(S.factorisation(x)) for x in S], list(range(S.size()))
    # )
    self.assertEqual(
        [S.current_position(S.factorisation(x)) for x in S],
        list(range(S.size())),
    )
    # doesn't work
    # self.assertEqual(
    #     [S.current_position(x) for x in S],
    #     list(range(S.size())),
    # )
    for x in S:
        self.assertEqual(
            S.sorted_position(x), S.position_to_sorted_position(S.position(x))
        )

    for x in S.idempotents():
        w = S.factorisation(x)
        self.assertTrue(S.equal_to(w + w, w))


def check_assessors(self, S):
    ReportGuard(False)
    # current_size
    self.assertTrue(S.current_size(), S.number_of_generators())
    S.run()
    # size
    self.assertTrue(S.current_size(), S.size())

    # at, sorted_at
    for i in range(S.size()):
        self.assertEqual(S.at(i), S.sorted_at(S.position_to_sorted_position(i)))

    # __iter__, sorted
    self.assertEqual(sorted(list(S)), list(S.sorted()))


def check_attributes(self, S):
    ReportGuard(False)

    S.is_monoid()
    self.assertTrue(S.is_finite())
    S.degree()  # just check it doesn't throw


def check_idempotents(self, S):
    ReportGuard(False)

    try:
        self.assertTrue(all((x * x == x for x in S.idempotents())))
    except TypeError:  # no multiplication provides
        pass

    self.assertTrue(
        all(S.fast_product(S.position(x), S.position(x)) == S.position(x))
        for x in S.idempotents()
    )

    self.assertEqual(
        sum(1 for x in range(S.size()) if S.is_idempotent(x)),
        S.number_of_idempotents(),
    )


def check_cayley_graphs(self, S):
    ReportGuard(False)

    g = S.right_cayley_graph()
    self.assertEqual(len(g), S.size())
    self.assertEqual(len(g[0]), S.number_of_generators())
    g = S.left_cayley_graph()
    self.assertEqual(len(g), S.size())
    self.assertEqual(len(g[0]), S.number_of_generators())


def check_factor_prod_rels(self, S):
    ReportGuard(False)

    # current_length
    for i in range(S.number_of_generators()):
        self.assertEqual(S.current_length(i), 1)

    with self.assertRaises(RuntimeError):
        S.current_length(S.number_of_generators())

    # current_max_word_length
    self.assertEqual(S.current_max_word_length(), 1)
    self.assertEqual(S.current_number_of_rules(), 0)

    S.run()
    # (minimal_)factorisation + word_to_element
    for i, x in enumerate(S):
        self.assertEqual(S.word_to_element(S.factorisation(x)), x)
        self.assertEqual(S.word_to_element(S.minimal_factorisation(i)), x)

    # rules, number_of_rules
    self.assertEqual(len(list(S.rules())), S.number_of_rules())

    for l, r in S.rules():
        self.assertEqual(S.current_position(l), S.current_position(r))
        self.assertEqual(S.factorisation(S.current_position(r)), r)

    # product_by_reduction + fast_product
    try:
        for i in range(S.size()):
            for j in range(S.size()):
                self.assertEqual(
                    S.product_by_reduction(i, j), S.position(S.at(i) * S.at(j))
                )
                self.assertEqual(
                    S.fast_product(i, j), S.position(S.at(i) * S.at(j))
                )
    except TypeError:  # no product defined
        pass


def check_prefix_suffix(self, S):
    ReportGuard(False)
    S.run()

    for i in range(S.number_of_generators(), S.size()):
        self.assertEqual(S.fast_product(S.prefix(i), S.final_letter(i)), i)
        self.assertEqual(S.fast_product(S.first_letter(i), S.suffix(i)), i)


def check_froidure_pin_transf1(self, T):
    ReportGuard(False)
    if T is Transf16:
        add = list(range(8, 16))
    else:
        add = []

    S = FroidurePin[T]()

    S.add_generator(T.make([1, 7, 2, 6, 0, 4, 1, 5] + add))
    S.add_generator(T.make([2, 4, 6, 1, 4, 5, 2, 7] + add))
    S.add_generator(T.make([3, 0, 7, 2, 4, 6, 2, 4] + add))
    S.add_generator(T.make([3, 2, 3, 4, 5, 3, 0, 1] + add))
    S.add_generator(T.make([4, 3, 7, 7, 4, 5, 0, 4] + add))
    S.add_generator(T.make([5, 6, 3, 0, 3, 0, 5, 1] + add))
    S.add_generator(T.make([6, 0, 1, 1, 1, 6, 3, 4] + add))
    S.add_generator(T.make([7, 7, 4, 0, 6, 4, 1, 7] + add))
    S.reserve(600000)

    self.assertEqual(S.number_of_generators(), 8)
    self.assertEqual(S.current_size(), 8)
    self.assertEqual(S.current_number_of_rules(), 0)
    S.enumerate(100)
    self.assertEqual(S.current_size(), 8200)
    self.assertEqual(S.current_number_of_rules(), 1049)

    if T == Transf16:
        self.assertEqual(S.degree(), 16)
    else:
        self.assertEqual(S.degree(), 8)


def check_froidure_pin_transf2(self, T):
    add = list(range(3, 16)) if T is Transf16 else []
    gens = [
        T.make([1, 0, 2] + add),
        T.make([1, 2, 0] + add),
    ]
    S = FroidurePin[T]()
    S.add_generators(gens)
    S.run()
    self.assertEqual(
        list(S),
        [
            T.make([1, 0, 2] + add),
            T.make([1, 2, 0] + add),
            T.make([0, 1, 2] + add),
            T.make([2, 1, 0] + add),
            T.make([0, 2, 1] + add),
            T.make([2, 0, 1] + add),
        ],
    )

    self.assertEqual(
        list(S.sorted()),
        [
            T.make([0, 1, 2] + add),
            T.make([0, 2, 1] + add),
            T.make([1, 0, 2] + add),
            T.make([1, 2, 0] + add),
            T.make([2, 0, 1] + add),
            T.make([2, 1, 0] + add),
        ],
    )


class TestFroidurePinTransf(unittest.TestCase):
    def test_froidure_pin_transf(self):

        for T in (Transf16, Transf1, Transf2, Transf4):
            add = list(range(3, 16)) if T is Transf16 else []
            gens = [
                T.make([1, 0, 2] + add),
                T.make([1, 2, 0] + add),
                T.make([0, 0, 1] + add),
            ]
            check_constructors(self, T, gens)
            check_generators(self, T, gens)
            check_settings(self, FroidurePin[T](gens))
            check_mem_compare(self, FroidurePin[T](gens))
            check_assessors(self, FroidurePin[T](gens))
            check_attributes(self, FroidurePin[T](gens))
            check_idempotents(self, FroidurePin[T](gens))
            check_cayley_graphs(self, FroidurePin[T](gens))
            check_factor_prod_rels(self, FroidurePin[T](gens))
            check_prefix_suffix(self, FroidurePin[T](gens))

            check_froidure_pin_transf1(self, T)
            check_froidure_pin_transf2(self, T)

    def test_runner_transf(self):
        for T in (Transf16, Transf1, Transf2, Transf4):
            add = list(range(8, 16)) if T is Transf16 else []
            S = FroidurePin[T]()

            S.add_generator(T.make([1, 7, 2, 6, 0, 4, 1, 5] + add))
            S.add_generator(T.make([2, 4, 6, 1, 4, 5, 2, 7] + add))
            S.add_generator(T.make([3, 0, 7, 2, 4, 6, 2, 4] + add))
            S.add_generator(T.make([3, 2, 3, 4, 5, 3, 0, 1] + add))
            S.add_generator(T.make([4, 3, 7, 7, 4, 5, 0, 4] + add))
            S.add_generator(T.make([5, 6, 3, 0, 3, 0, 5, 1] + add))
            S.add_generator(T.make([6, 0, 1, 1, 1, 6, 3, 4] + add))
            S.add_generator(T.make([7, 7, 4, 0, 6, 4, 1, 7] + add))
            check_runner(self, S, timedelta(microseconds=1000))

    def test_froidure_pin_pperm(self):
        for T in (PPerm16, PPerm1, PPerm2, PPerm4):
            gens = [
                T.make([0, 1, 2], [1, 0, 2], 16),
                T.make([0, 1, 2], [1, 2, 0], 16),
                T.make([0, 1], [0, 1], 16),
            ]
            check_constructors(self, T, gens)
            check_generators(self, T, gens)
            check_settings(self, FroidurePin[T](gens))
            check_mem_compare(self, FroidurePin[T](gens))
            check_assessors(self, FroidurePin[T](gens))
            check_attributes(self, FroidurePin[T](gens))
            check_idempotents(self, FroidurePin[T](gens))
            check_cayley_graphs(self, FroidurePin[T](gens))
            check_factor_prod_rels(self, FroidurePin[T](gens))
            check_prefix_suffix(self, FroidurePin[T](gens))

    def test_runner_pperm(self):
        for T in (PPerm16, PPerm1, PPerm2, PPerm4):
            S = FroidurePin[T]()

            S.add_generator(
                T.make(list(range(9)), [1, 0] + list(range(2, 9)), 16)
            )
            S.add_generator(T.make(list(range(9)), list(range(1, 9)) + [0], 16))
            S.add_generator(T.make([0, 1], [0, 1], 16))
            check_runner(self, S, timedelta(microseconds=1000))

    def test_froidure_pin_tce(self):
        ReportGuard(False)
        tc = ToddCoxeter(congruence_kind.twosided)
        tc.set_number_of_generators(2)
        tc.add_pair([0, 0, 0, 0], [0])
        tc.add_pair([1, 1, 1, 1], [1])
        tc.add_pair([0, 1], [1, 0])

        self.assertEqual(tc.number_of_classes(), 15)

        check_settings(self, FroidurePinTCE(tc.quotient_froidure_pin()))
        check_mem_compare(self, FroidurePinTCE(tc.quotient_froidure_pin()))
        check_assessors(self, FroidurePinTCE(tc.quotient_froidure_pin()))
        check_attributes(self, FroidurePinTCE(tc.quotient_froidure_pin()))
        check_idempotents(self, FroidurePinTCE(tc.quotient_froidure_pin()))
        check_cayley_graphs(self, FroidurePinTCE(tc.quotient_froidure_pin()))
        check_factor_prod_rels(self, FroidurePinTCE(tc.quotient_froidure_pin()))
        check_prefix_suffix(self, FroidurePinTCE(tc.quotient_froidure_pin()))

    def test_froidure_pin_kbe(self):
        ReportGuard(False)
        kb = KnuthBendix()
        kb.set_alphabet(2)
        kb.add_rule([0, 0, 0, 0], [0])
        kb.add_rule([1, 1, 1, 1], [1])
        kb.add_rule([0, 1], [1, 0])

        self.assertEqual(kb.size(), 15)

        check_settings(self, FroidurePinKBE(kb.froidure_pin()))
        check_mem_compare(self, FroidurePinKBE(kb.froidure_pin()))
        check_assessors(self, FroidurePinKBE(kb.froidure_pin()))
        check_attributes(self, FroidurePinKBE(kb.froidure_pin()))
        check_idempotents(self, FroidurePinKBE(kb.froidure_pin()))
        check_cayley_graphs(self, FroidurePinKBE(kb.froidure_pin()))
        check_factor_prod_rels(self, FroidurePinKBE(kb.froidure_pin()))
        check_prefix_suffix(self, FroidurePinKBE(kb.froidure_pin()))
