# -*- coding: utf-8 -*-
# pylint: disable=no-name-in-module, missing-function-docstring, invalid-name

# Copyright (c) 2021-2024 J. D. Mitchell + Maria Tsalakou
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
    ReportGuard,
    ToddCoxeter,
    todd_coxeter,
    Transf,
    congruence_kind,
    tril,
    Order,
    Presentation,
    presentation,
    froidure_pin,
    WordRange,
    word_graph,
)

strategy = ToddCoxeter.options.strategy


def test_constructors():
    with pytest.raises(TypeError):
        ToddCoxeter(congruence_kind.onesided)
    with pytest.raises(TypeError):
        ToddCoxeter(congruence_kind.twosided)

    with pytest.raises(TypeError):
        ToddCoxeter(45)
    with pytest.raises(TypeError):
        ToddCoxeter("lft")
    with pytest.raises(TypeError):
        ToddCoxeter(
            congruence_kind.twosided,
            congruence_kind.onesided,
        )

    S = FroidurePin(Transf([0, 0, 1, 2, 3]))
    with pytest.raises(TypeError):
        ToddCoxeter(S)

    with pytest.raises(TypeError):
        ToddCoxeter(S)
        ToddCoxeter(congruence_kind.twosided, S)

    p = Presentation([0])

    tc = ToddCoxeter(congruence_kind.onesided, p)
    with pytest.raises(RuntimeError):
        tc.add_generating_pair([0, 0, 1, 0], [1, 1, 2])


def test_attributes():
    ReportGuard(False)
    p = Presentation([0])
    presentation.add_rule(p, [0, 0, 0, 0, 0, 0], [0, 0, 0])
    tc = ToddCoxeter(congruence_kind.onesided, p)
    assert tc.number_of_classes() == 5
    assert tc.contains([0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0])
    assert not tc.contains([0, 0, 0], [0, 0])
    assert tc.currently_contains([0, 0, 0], [0, 0]) == tril.false
    assert tc.kind() == congruence_kind.onesided
    assert tc.word_of(1) == [0, 0]
    assert tc.index_of([0, 0]) == 1
    assert tc.number_of_generating_pairs() == 0
    assert tc.generating_pairs() == []

    # TODO(0) use these tests in to_todd_coxeter
    # S = FroidurePin(Transf([1, 2, 2]), Transf([2, 0, 1]))
    # tc = ToddCoxeter(congruence_kind.twosided, S)
    # assert tc.number_of_classes() == 24
    # assert tc.has_parent_froidure_pin()
    # assert tc.parent_froidure_pin().size() == 24
    # assert tc.quotient_froidure_pin().size() == 24

    # K = KnuthBendix()
    # K.set_alphabet("a")
    # K.add_rule("aaaa", "aa")
    # tc = ToddCoxeter(congruence_kind.onesided, K)
    # assert tc.number_of_classes() == 3

    # tc = ToddCoxeter(congruence_kind.twosided, p)
    # assert tc.is_quotient_obviously_infinite()
    # assert not tc.is_quotient_obviously_finite()
    # presentation.add_rule(p, [0, 0, 0], [0])
    # assert not tc.is_quotient_obviously_infinite()
    # tc.run()
    # assert tc.is_quotient_obviously_finite()


def test_operators():
    ReportGuard(False)
    p = Presentation([0, 1])
    presentation.add_rule(p, [0, 0, 0, 0], [1])
    presentation.add_rule(p, [1, 1, 1, 1], [1])

    tc = ToddCoxeter(congruence_kind.onesided, p)
    tc.run()
    p = Presentation([0])
    presentation.add_rule(p, [0, 0, 0], [0])
    tc.init(congruence_kind.onesided, p)
    tc.run_for(timedelta(microseconds=2))

    p = Presentation([0])
    presentation.add_rule(p, [0, 0, 0, 0, 0, 0, 0, 0], [0])
    tc = ToddCoxeter(congruence_kind.onesided, p)
    tc.run_until(
        lambda: tc.currently_contains([0, 0, 0, 0, 0, 0, 0, 0], [0]) == tril.true
    )
    assert tc.stopped_by_predicate()
    assert not tc.finished()
    p.alphabet([0, 1])
    presentation.add_rule(p, [1, 1, 1, 1], [0])
    tc.init(congruence_kind.onesided, p)
    tc.run_for(timedelta(microseconds=1))

    # If this is run on an incredibly fast computer, this
    # test of timed_out() may not work.
    assert tc.timed_out()
    tc.run()
    assert not tc.timed_out()


def test_settings():
    ReportGuard(False)
    p = Presentation([0])
    presentation.add_rule(p, [0, 0, 0, 0], [0, 0])
    tc = ToddCoxeter(congruence_kind.onesided, p)
    # tc.reserve(10)
    tc.run()

    assert not tc.is_standardized()
    tc.standardize(Order.lex)
    assert tc.is_standardized()
    tc.standardize(Order.shortlex)
    tc.standardize(Order.recursive)
    with pytest.raises(TypeError):
        tc.standardize("shooortlex")

    S = FroidurePin(Transf([3, 1, 2, 1, 2]), Transf([1, 1, 1, 2, 2]))
    tc = ToddCoxeter(congruence_kind.twosided, S.right_cayley_graph())
    assert tc.number_of_classes() == 4

    p = Presentation([0, 1])
    presentation.add_rule(p, [0, 0, 0, 0], [0])
    presentation.add_rule(p, [1, 1, 1, 1], [0])
    tc = ToddCoxeter(congruence_kind.onesided, p)
    with pytest.raises(TypeError):
        tc.strategy("feelsch")
    with pytest.raises(TypeError):
        tc.strategy(1)
    tc.strategy(strategy.felsch)
    assert tc.strategy() == strategy.felsch
    tc.strategy(strategy.hlt)
    assert tc.strategy() == strategy.hlt


def test_000_iterators():
    ReportGuard(False)
    p = Presentation([0, 1])
    presentation.add_rule(p, [0, 0, 0, 0], [0])
    presentation.add_rule(p, [1, 1, 1, 1], [1])
    presentation.add_rule(p, [0, 1], [1, 0])
    tc = ToddCoxeter(congruence_kind.onesided, p)
    assert tc.presentation().rules == [
        [0, 0, 0, 0],
        [0],
        [1, 1, 1, 1],
        [1],
        [0, 1],
        [1, 0],
    ]

    assert list(todd_coxeter.word_normal_forms(tc)) == [
        [0],
        [1],
        [0, 0],
        [0, 1],
        [1, 1],
        [0, 0, 0],
        [0, 0, 1],
        [0, 1, 1],
        [1, 1, 1],
        [0, 0, 0, 1],
        [0, 0, 1, 1],
        [0, 1, 1, 1],
        [0, 0, 0, 1, 1],
        [0, 0, 1, 1, 1],
        [0, 0, 0, 1, 1, 1],
    ]
    S = FroidurePin(Transf([1, 3, 4, 2, 3]), Transf([3, 2, 1, 3, 3]))
    tc = ToddCoxeter(congruence_kind.onesided, S.right_cayley_graph())
    tc.add_generating_pair(
        froidure_pin.factorisation(S, Transf([3, 4, 4, 4, 4])),
        froidure_pin.factorisation(S, Transf([3, 1, 3, 3, 3])),
    )
    assert tc.generating_pairs() == [
        [0, 1, 0, 0, 0, 1, 1, 0, 0],
        [1, 0, 0, 0, 1],
    ]
    words = WordRange().alphabet_size(2).min(1).max(5)
    assert todd_coxeter.non_trivial_classes(tc, list(words)) == [
        [[0], [0, 0, 0, 0]],
        [[1], [1, 1, 1]],
        [[0, 1], [0, 1, 1, 1]],
        [[1, 0], [1, 1, 1, 0]],
        [[1, 1], [1, 1, 1, 1]],
        [[1, 0, 1], [1, 1, 0, 1]],
        [[0, 0, 0, 1], [1, 0, 0, 1]],
    ]


# The following are some tests adapted from the libsemigroups test file


def test_020():
    ReportGuard(False)
    p = Presentation([0])
    tc = ToddCoxeter(congruence_kind.twosided, p)
    tc.strategy(strategy.hlt)
    assert tc.contains([0, 0], [0, 0])
    assert not tc.contains([0, 0], [0])


def test_021():
    ReportGuard(False)
    p = Presentation([0, 1, 2, 3, 4])
    tc = ToddCoxeter(congruence_kind.twosided, p)
    with pytest.raises(RuntimeError):
        tc.run()


def test_033():
    ReportGuard(False)
    p = Presentation([0, 1])
    presentation.add_rule(p, [0, 0, 0], [0])
    presentation.add_rule(p, [0], [1, 1])
    tc1 = ToddCoxeter(congruence_kind.twosided, p)
    assert tc1.number_of_classes() == 5
    tc2 = ToddCoxeter(congruence_kind.onesided, tc1)
    tc2.add_generating_pair([0], [0, 0])
    tc2.lookahead_next(1)
    # TODO(0) Uncommenting the next line triggers a bad_alloc in the ubuntu ci
    # jobs.
    # tc2.report_every(timedelta(seconds=1))
    assert tc2.number_of_classes() == 3


def test_036():
    ReportGuard(False)
    S = FroidurePin(Transf([1, 3, 4, 2, 3]), Transf([3, 2, 1, 3, 3]))
    tc = ToddCoxeter(congruence_kind.twosided, S.right_cayley_graph())
    tc.add_generating_pair([0], [1])
    tc.add_generating_pair([0, 0], [0])
    assert tc.number_of_classes() == 1


def test_096():
    ReportGuard(False)
    p = Presentation([0, 1])
    presentation.add_rule(p, [0], [1])
    presentation.add_rule(p, [0, 0], [0])
    tc = ToddCoxeter(congruence_kind.twosided, p)
    tc.strategy(strategy.felsch)
    assert tc.strategy() == strategy.felsch
    wg = tc.current_word_graph()
    assert not word_graph.is_complete(wg)
    for lhs, rhs in ((p.rules[i], p.rules[i + 1]) for i in range(0, len(p.rules), 2)):
        assert word_graph.is_compatible(wg, 0, wg.number_of_nodes(), lhs, rhs)
    assert tc.number_of_classes() == 1
    tc.shrink_to_fit()
    assert list(todd_coxeter.word_normal_forms(tc)) == [[0]]
    assert word_graph.is_complete(tc.current_word_graph())
    for lhs, rhs in ((p.rules[i], p.rules[i + 1]) for i in range(0, len(p.rules), 2)):
        assert word_graph.is_compatible(wg, 0, wg.number_of_nodes(), lhs, rhs)

    copy = tc.copy()
    assert tc is not copy
    assert len(copy.presentation().alphabet()) == 2
    assert copy.number_of_generating_pairs() == 0
    assert copy.finished()
    assert copy.number_of_classes() == 1
    wg = copy.current_word_graph()
    assert word_graph.is_complete(wg)
    for lhs, rhs in ((p.rules[i], p.rules[i + 1]) for i in range(0, len(p.rules), 2)):
        assert word_graph.is_compatible(wg, 0, wg.number_of_nodes(), lhs, rhs)


def test_redundant_rule():
    p = Presentation("ab")
    p.rules = ["aaa", "a", "bbbb", "b", "abab", "aaaaaa"]

    assert todd_coxeter.redundant_rule(p, timedelta(milliseconds=10)) is None
