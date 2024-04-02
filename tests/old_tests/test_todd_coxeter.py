# -*- coding: utf-8 -*-
# pylint: disable=no-name-in-module, missing-function-docstring, invalid-name

# Copyright (c) 2021, J. D. Mitchell + Maria Tsalakou
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for the ToddCoxeter class.
"""

from datetime import timedelta

import pytest

from libsemigroups_pybind11 import (
    FroidurePin,
    KnuthBendix,
    ReportGuard,
    ToddCoxeter,
    Transf,
    congruence_kind,
    tril,
)

strategy = ToddCoxeter.strategy_options
fpp = ToddCoxeter.froidure_pin_options
order = ToddCoxeter.order


def test_constructors():
    # pylint: disable=too-many-statements
    ToddCoxeter(congruence_kind.left)
    ToddCoxeter(congruence_kind.right)
    ToddCoxeter(congruence_kind.twosided)

    with pytest.raises(TypeError):
        ToddCoxeter(45)
    with pytest.raises(TypeError):
        ToddCoxeter("lft")
    with pytest.raises(TypeError):
        ToddCoxeter(
            congruence_kind.twosided,
            congruence_kind.left,
            congruence_kind.right,
        )

    S = FroidurePin(Transf([0, 0, 1, 2, 3]))
    with pytest.raises(TypeError):
        ToddCoxeter(S)

    ToddCoxeter(congruence_kind.twosided, S)

    K = KnuthBendix()
    with pytest.raises(RuntimeError):
        ToddCoxeter(congruence_kind.left, K)
    K.set_alphabet("a")
    ToddCoxeter(congruence_kind.left, K)

    T = ToddCoxeter(congruence_kind.left)
    ToddCoxeter(congruence_kind.left, T)

    T.set_number_of_generators(1)

    with pytest.raises(RuntimeError):
        ToddCoxeter(congruence_kind.right, T)
    with pytest.raises(RuntimeError):
        ToddCoxeter(congruence_kind.twosided, T)
    ToddCoxeter(congruence_kind.left, T)

    T = ToddCoxeter(congruence_kind.right)
    ToddCoxeter(congruence_kind.right, T)
    T.set_number_of_generators(1)
    with pytest.raises(RuntimeError):
        ToddCoxeter(congruence_kind.left, T)
    with pytest.raises(RuntimeError):
        ToddCoxeter(congruence_kind.twosided, T)

    ToddCoxeter(congruence_kind.right, T)

    T = ToddCoxeter(congruence_kind.twosided)
    ToddCoxeter(congruence_kind.twosided, T)
    T.set_number_of_generators(1)
    ToddCoxeter(congruence_kind.left, T)
    ToddCoxeter(congruence_kind.right, T)
    ToddCoxeter(congruence_kind.twosided, T)

    tc = ToddCoxeter(congruence_kind.left)
    tc.set_number_of_generators(2)
    with pytest.raises(RuntimeError):
        tc.add_pair([0, 0, 1, 0], [1, 1, 2])


def test_attributes():
    ReportGuard(False)
    tc = ToddCoxeter(congruence_kind.left)
    tc.set_number_of_generators(1)
    tc.add_pair([0, 0, 0, 0, 0, 0], [0, 0, 0])
    assert tc.number_of_classes() == 5
    assert tc.contains([0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0])
    assert not tc.contains([0, 0, 0], [0, 0])
    assert tc.const_contains([0, 0, 0], [0, 0]) == tril.false
    assert tc.kind() == congruence_kind.left
    assert not tc.empty()
    assert tc.class_index_to_word(1) == [0, 0]
    assert tc.word_to_class_index([0, 0]) == 1
    assert not tc.has_parent_froidure_pin()
    assert tc.number_of_generators() == 1
    assert tc.number_of_generating_pairs() == 1
    assert tc.less([0], [0, 0])

    S = FroidurePin(Transf([1, 2, 2]), Transf([2, 0, 1]))
    tc = ToddCoxeter(congruence_kind.twosided, S)
    assert tc.number_of_classes() == 24
    assert tc.has_parent_froidure_pin()
    assert tc.parent_froidure_pin().size() == 24
    assert tc.quotient_froidure_pin().size() == 24

    K = KnuthBendix()
    K.set_alphabet("a")
    K.add_rule("aaaa", "aa")
    tc = ToddCoxeter(congruence_kind.left, K)
    assert tc.number_of_classes() == 3

    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(1)
    assert tc.is_quotient_obviously_infinite()
    assert not tc.is_quotient_obviously_finite()
    tc.add_pair([0, 0, 0], [0])
    assert not tc.is_quotient_obviously_infinite()
    tc.run()
    assert tc.is_quotient_obviously_finite()


def test_operators():
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
        lambda: tc.const_contains([0, 0, 0, 0, 0, 0, 0, 0], [0]) == tril.true
    )
    assert tc.stopped_by_predicate()
    assert not tc.finished()

    tc = ToddCoxeter(congruence_kind.left)
    tc.set_number_of_generators(2)
    tc.add_pair([0, 0, 0, 0], [1])
    tc.add_pair([1, 0], [0, 1])
    tc.add_pair([1, 1, 1, 1], [0])
    tc.run_for(timedelta(microseconds=1))
    # If this is run on an incredibly fast computer, this
    # test of timed_out() may not work.
    assert tc.timed_out()
    tc.run()
    assert not tc.timed_out()


def test_settings():
    ReportGuard(False)
    tc = ToddCoxeter(congruence_kind.left)
    tc.set_number_of_generators(1)
    tc.add_pair([0, 0, 0, 0], [0, 0])
    tc.reserve(10)
    tc.run()

    assert not tc.is_standardized()
    tc.standardize(order.lex)
    assert tc.is_standardized()
    tc.standardize(order.shortlex)
    tc.standardize(order.recursive)
    tc.standardize(True)
    tc.standardize(False)
    tc.standardize(1)
    with pytest.raises(TypeError):
        tc.standardize("shooortlex")

    S = FroidurePin(Transf([3, 1, 2, 1, 2]), Transf([1, 1, 1, 2, 2]))
    tc = ToddCoxeter(congruence_kind.twosided, S)
    tc.froidure_pin_policy()
    tc.froidure_pin_policy(fpp.none)
    assert tc.froidure_pin_policy() == fpp.none
    tc.froidure_pin_policy(fpp.use_relations)
    assert tc.froidure_pin_policy() == fpp.use_relations
    tc.froidure_pin_policy(fpp.use_cayley_graph)
    assert tc.froidure_pin_policy() == fpp.use_cayley_graph
    with pytest.raises(TypeError):
        tc.froidure_pin_policy(1)
    with pytest.raises(TypeError):
        tc.froidure_pin_policy("userelations")

    tc = ToddCoxeter(congruence_kind.left)
    tc.set_number_of_generators(2)
    tc.add_pair([0, 0, 0, 0], [0])
    tc.add_pair([1, 1, 1, 1], [0])
    with pytest.raises(TypeError):
        tc.strategy("feelsch")
    with pytest.raises(TypeError):
        tc.strategy(1)
    tc.strategy(strategy.felsch)
    assert tc.strategy() == strategy.felsch
    tc.strategy(strategy.hlt)
    assert tc.strategy() == strategy.hlt
    tc.strategy(strategy.random)
    assert tc.strategy() == strategy.random


def test_000_iterators():
    ReportGuard(False)
    tc = ToddCoxeter(congruence_kind.left)
    tc.set_number_of_generators(2)
    tc.add_pair([0, 0, 0, 0], [0])
    tc.add_pair([1, 1, 1, 1], [1])
    tc.add_pair([0, 1], [1, 0])
    assert list(tc.generating_pairs()) == [
        ([0, 0, 0, 0], [0]),
        ([1, 1, 1, 1], [1]),
        ([0, 1], [1, 0]),
    ]

    assert list(tc.normal_forms()) == [
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
    ]
    S = FroidurePin(Transf([1, 3, 4, 2, 3]), Transf([3, 2, 1, 3, 3]))
    tc = ToddCoxeter(congruence_kind.left, S)
    tc.add_pair(
        S.factorisation(Transf([3, 4, 4, 4, 4])),
        S.factorisation(Transf([3, 1, 3, 3, 3])),
    )
    assert list(tc.non_trivial_classes()) == [
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
    ]


# The following are some tests adapted from the libsemigroups test file


def test_020():
    ReportGuard(False)
    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(1)
    tc.strategy(strategy.hlt)
    assert tc.contains([0, 0], [0, 0])
    assert not tc.contains([0, 0], [0])


def test_021():
    ReportGuard(False)
    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(5)
    with pytest.raises(RuntimeError):
        tc.run()


def test_033():
    ReportGuard(False)
    tc1 = ToddCoxeter(congruence_kind.twosided)
    tc1.set_number_of_generators(2)
    tc1.add_pair([0, 0, 0], [0])
    tc1.add_pair([0], [1, 1])
    assert tc1.number_of_classes() == 5
    tc2 = ToddCoxeter(congruence_kind.left, tc1)
    tc2.next_lookahead(1)
    tc2.report_every(timedelta(microseconds=1))
    assert not tc2.empty()
    tc2.add_pair([0], [0, 0])
    assert tc2.number_of_classes() == 3


def test_036():
    ReportGuard(False)
    S = FroidurePin(Transf([1, 3, 4, 2, 3]), Transf([3, 2, 1, 3, 3]))
    tc = ToddCoxeter(congruence_kind.twosided, S)
    tc.set_number_of_generators(2)
    tc.add_pair([0], [1])
    tc.add_pair([0, 0], [0])
    assert tc.number_of_classes() == 1


def test_096():
    ReportGuard(False)
    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(2)
    tc.add_pair([0], [1])
    tc.add_pair([0, 0], [0])
    tc.strategy(strategy.felsch)
    assert tc.strategy() == strategy.felsch
    assert not tc.complete()
    assert tc.compatible()
    assert tc.number_of_classes() == 1
    assert list(tc.normal_forms()) == [[0]]
    assert tc.complete()
    assert tc.compatible()

    copy = ToddCoxeter(tc)
    assert copy.number_of_generators() == 2
    assert copy.number_of_generating_pairs() == 2
    assert copy.finished()
    assert copy.number_of_classes() == 1
    assert copy.complete()
    assert copy.compatible()


def test_to_gap_string():
    ReportGuard(False)
    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(2)
    tc.add_pair([0, 1], [1, 0])
    assert len(tc.to_gap_string()) > 0

    tc = ToddCoxeter(congruence_kind.left)
    tc.set_number_of_generators(2)
    tc.add_pair([0, 1], [1, 0])
    with pytest.raises(RuntimeError):
        tc.to_gap_string()

    tc = ToddCoxeter(congruence_kind.right)
    tc.set_number_of_generators(2)
    tc.add_pair([0, 1], [1, 0])
    with pytest.raises(RuntimeError):
        tc.to_gap_string()
