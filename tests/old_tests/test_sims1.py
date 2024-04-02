# -*- coding: utf-8 -*-

# Copyright (c) 2022, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for the Sims1 class.
"""

# pylint: disable=duplicate-code
# pylint: disable=missing-function-docstring, invalid-name

import pytest

from libsemigroups_pybind11 import (
    FroidurePin,
    Presentation,
    ReportGuard,
    Sims1,
    Transf,
    action_digraph_helper,
    congruence_kind,
    follow_path,
    presentation,
)

###############################################################################
# Test functions begin
###############################################################################


def test_000():
    ReportGuard(False)
    p = Presentation([0, 1])
    p.contains_empty_word(True)

    presentation.add_rule_and_check(p, [0, 0, 0], [0])
    presentation.add_rule_and_check(p, [1, 1], [1])
    presentation.add_rule_and_check(p, [0, 1, 0, 1], [0])

    S = Sims1(congruence_kind.right)
    assert S.short_rules(p).number_of_threads(2).number_of_congruences(5) == 6

    it = S.iterator(5)

    assert next(it) == action_digraph_helper.make(5, [[0, 0]])
    assert next(it) == action_digraph_helper.make(5, [[1, 0], [1, 1]])

    assert next(it) == action_digraph_helper.make(5, [[1, 1], [1, 1]])
    assert next(it) == action_digraph_helper.make(5, [[1, 2], [1, 1], [1, 2]])
    assert next(it) == action_digraph_helper.make(5, [[1, 2], [1, 1], [2, 2]])
    assert next(it) == action_digraph_helper.make(
        5, [[1, 2], [1, 1], [3, 2], [3, 3]]
    )
    with pytest.raises(StopIteration):
        next(it)

    it = S.iterator(3)
    assert next(it) == action_digraph_helper.make(3, [[0, 0]])

    S = Sims1(congruence_kind.left)
    assert S.short_rules(p).number_of_congruences(5) == 9
    for x in S.iterator(5):
        assert follow_path(x, 0, [1, 0, 1, 0]) == follow_path(x, 0, [0])
    p = Presentation([0, 1, 2])
    presentation.add_rule(p, [0, 0, 0], [0])
    assert len(p.rules) == 2
    presentation.add_rule_and_check(p, [0, 0, 0], [0])
    p.validate()


def test_001():
    ReportGuard(False)
    p = Presentation([0, 1, 2])
    p.contains_empty_word(True)

    presentation.add_rule_and_check(p, [0, 1, 0], [0, 0])
    presentation.add_rule_and_check(p, [2, 2], [0, 0])
    presentation.add_rule_and_check(p, [0, 0, 0], [0, 0])
    presentation.add_rule_and_check(p, [2, 1], [1, 2])
    presentation.add_rule_and_check(p, [2, 0], [0, 0])
    presentation.add_rule_and_check(p, [1, 1], [1])
    presentation.add_rule_and_check(p, [0, 2], [0, 0])

    S = Sims1(congruence_kind.right)
    S.short_rules(p)
    assert S.number_of_congruences(1) == 1
    assert S.number_of_congruences(2) == 3
    assert S.number_of_congruences(3) == 13
    assert S.number_of_congruences(4) == 36
    assert S.number_of_congruences(5) == 82
    assert S.number_of_congruences(6) == 135
    assert S.number_of_congruences(7) == 166
    assert S.number_of_congruences(8) == 175
    assert S.number_of_congruences(9) == 176
    assert S.number_of_congruences(10) == 176

    it = S.iterator(2)
    assert next(it) == action_digraph_helper.make(2, [[0, 0, 0]])
    assert next(it) == action_digraph_helper.make(2, [[1, 0, 1], [1, 1, 1]])
    assert next(it) == action_digraph_helper.make(2, [[1, 1, 1], [1, 1, 1]])
    with pytest.raises(StopIteration):
        next(it)
    S = Sims1(congruence_kind.left)
    S.short_rules(p)
    assert S.number_of_congruences(11) == 176


def test_002():
    ReportGuard(False)
    p = Presentation([0, 1, 2, 3, 4, 5, 6])
    p.contains_empty_word(False)

    presentation.add_identity_rules(p, 6)
    presentation.add_inverse_rules(p, [1, 0, 3, 2, 5, 4, 6], 6)
    presentation.add_rule_and_check(p, [0, 0, 5, 0, 4], [6])
    presentation.add_rule_and_check(p, [0, 4, 2, 2, 1, 5, 2], [6])
    presentation.add_rule_and_check(p, [1, 3, 0, 2, 4, 4, 4], [6])
    S = Sims1(congruence_kind.right)
    S.short_rules(p)
    assert S.number_of_congruences(3) == 14
    assert S.number_of_congruences(4) == 14
    assert S.number_of_congruences(5) == 14


def test_003():
    ReportGuard(False)
    p = Presentation("aAbBcCe")
    p.contains_empty_word(False)
    presentation.add_identity_rules(p, "e")

    presentation.add_inverse_rules(p, "AaBbCce", "e")
    presentation.add_rule_and_check(p, "aaCac", "e")
    presentation.add_rule_and_check(p, "acbbACb", "e")
    presentation.add_rule_and_check(p, "ABabccc", "e")
    S = Sims1(congruence_kind.right)
    S.short_rules(p)
    assert S.number_of_congruences(3) == 14


def test_004():
    ReportGuard(False)
    p = Presentation([0, 1, 2, 3])
    p.contains_empty_word(False)

    presentation.add_identity_rules(p, 0)
    presentation.add_rule_and_check(p, [1, 1], [0])
    presentation.add_rule_and_check(p, [1, 3], [3])
    presentation.add_rule_and_check(p, [2, 2], [2])
    presentation.add_rule_and_check(p, [3, 1], [3])
    presentation.add_rule_and_check(p, [3, 3], [3])
    presentation.add_rule_and_check(p, [2, 3, 2], [2])
    presentation.add_rule_and_check(p, [3, 2, 3], [3])
    presentation.add_rule_and_check(p, [1, 2, 1, 2], [2, 1, 2])
    presentation.add_rule_and_check(p, [2, 1, 2, 1], [2, 1, 2])

    S = Sims1(congruence_kind.right)
    S.short_rules(p)
    assert S.number_of_congruences(2) == 4
    assert S.number_of_congruences(3) == 7
    assert S.number_of_congruences(4) == 14
    assert S.number_of_congruences(5) == 23
    assert S.number_of_congruences(6) == 36
    assert S.number_of_congruences(7) == 51
    assert S.number_of_congruences(8) == 62
    assert S.number_of_congruences(9) == 74
    assert S.number_of_congruences(10) == 86
    assert S.number_of_congruences(11) == 95
    assert S.number_of_congruences(12) == 100
    assert S.number_of_congruences(13) == 102
    assert S.number_of_congruences(14) == 104
    assert S.number_of_congruences(15) == 105
    assert S.number_of_congruences(16) == 105
    assert S.number_of_congruences(17) == 105


def test_006():
    ReportGuard(False)
    S = FroidurePin(Transf([1, 2, 0]), Transf([1, 0, 2]), Transf([0, 1, 0]))
    assert S.size() == 27
    assert S.number_of_generators() == 3
    assert S.number_of_rules() == 16
    C = Sims1(congruence_kind.right)
    C.short_rules(presentation.make(S))
    assert C.number_of_congruences(27) == 287


def test_007():
    ReportGuard(False)
    S = FroidurePin(Transf([1, 2, 0]), Transf([1, 0, 2]), Transf([0, 1, 0]))
    assert S.size() == 27
    assert S.number_of_generators() == 3
    assert S.number_of_rules() == 16
    C = Sims1(congruence_kind.left)
    C.short_rules(presentation.make(S))
    assert C.number_of_congruences(27) == 120


def test_016():
    ReportGuard(False)

    p = Presentation([0, 1])
    p.contains_empty_word(True)

    presentation.add_rule_and_check(p, [0, 0, 0], [0])
    presentation.add_rule_and_check(p, [1, 1], [1])
    presentation.add_rule_and_check(p, [0, 1, 0, 1], [0])

    e = Presentation([0, 1])
    e.contains_empty_word(True)
    presentation.add_rule_and_check(e, [0], [1])
    S = Sims1(congruence_kind.right)
    S.short_rules(p).extra(e)
    assert S.number_of_congruences(5) == 2


def test_021():
    ReportGuard(False)
    p = Presentation([0, 1, 2])
    presentation.add_rule_and_check(p, [0, 1, 0], [0, 0])

    with pytest.raises(RuntimeError):
        Sims1(congruence_kind.twosided)

    e = Presentation([0, 1])

    with pytest.raises(RuntimeError):
        Sims1(congruence_kind.left).short_rules(p).extra(e)
    with pytest.raises(RuntimeError):
        Sims1(congruence_kind.left).short_rules(p).long_rules(e)
    with pytest.raises(RuntimeError):
        Sims1(congruence_kind.left).long_rules(p).extra(e)
    with pytest.raises(RuntimeError):
        Sims1(congruence_kind.left).long_rules(p).short_rules(e)
    with pytest.raises(RuntimeError):
        Sims1(congruence_kind.left).extra(p).short_rules(e)
    with pytest.raises(RuntimeError):
        Sims1(congruence_kind.left).extra(p).long_rules(e)

    S = Sims1(congruence_kind.right)
    with pytest.raises(RuntimeError):
        S.number_of_threads(0)


def test_030():
    ReportGuard(False)
    p = Presentation([0, 1, 2, 3])
    presentation.add_rule(p, [1, 2, 1], [1, 1])
    presentation.add_rule(p, [3, 3], [1, 1])
    presentation.add_rule(p, [1, 1, 1], [1, 1])
    presentation.add_rule(p, [3, 2], [2, 3])
    presentation.add_rule(p, [3, 1], [1, 1])
    presentation.add_rule(p, [2, 2], [2])
    presentation.add_rule(p, [1, 3], [1, 1])
    p.validate()
    C = Sims1(congruence_kind.right)
    C.short_rules(p)
    assert C.number_of_congruences(2) == 67


def test_031():
    ReportGuard(False)
    p = Presentation([1, 2, 3])
    presentation.add_rule(p, [1, 2, 1], [1, 1])
    presentation.add_rule(p, [3, 3], [1, 1])
    presentation.add_rule(p, [1, 1, 1], [1, 1])
    presentation.add_rule(p, [3, 2], [2, 3])
    presentation.add_rule(p, [3, 1], [1, 1])
    presentation.add_rule(p, [2, 2], [2])
    presentation.add_rule(p, [1, 3], [1, 1])
    p.alphabet_from_rules()
    p.validate()

    C = Sims1(congruence_kind.right)
    C.short_rules(p)
    assert C.number_of_congruences(2) == 7
    assert C.stats().max_pending == 0
    assert C.stats().total_pending == 0


def test_070():
    ReportGuard(False)
    p = Presentation([0, 1])
    p.contains_empty_word(True)
    S = Sims1(congruence_kind.right)
    S.short_rules(p)
    assert S.number_of_congruences(4) == 5477
