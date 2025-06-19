# -*- coding: utf-8 -*-

# Copyright (c) 2024, R. Cirpons
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for the libsemigroups_pybind11 functionality
arising from sims.*pp in libsemigroups.
"""

# pylint: disable=missing-function-docstring, invalid-name

import os

import pytest

from libsemigroups_pybind11 import (
    LibsemigroupsError,
    Matrix,
    MatrixKind,
    MinimalRepOrc,
    Order,
    Presentation,
    ReportGuard,
    RepOrc,
    Sims1,
    Sims2,
    SimsRefinerFaithful,
    SimsRefinerIdeals,
    ToddCoxeter,
    congruence_kind,
    presentation,
    sims,
    to,
    word_graph,
    WordGraph,
)


def check_right_generating_pairs(s, wg):
    tc = ToddCoxeter(congruence_kind.onesided, s.presentation())

    for u, v in sims.right_generating_pairs(wg):
        tc.add_generating_pair(u, v)
    tc.run()

    tc.standardize(Order.shortlex)
    expected = tc.word_graph()
    result = wg.copy()

    num = word_graph.number_of_nodes_reachable_from(result, 0)

    assert expected.number_of_nodes() == num

    expected = expected.induced_subgraph(0, num)
    result = result.induced_subgraph(0, num)
    assert result == expected

    tc.init(congruence_kind.onesided, s.presentation())
    for u, v in sims.right_generating_pairs(s.presentation(), wg):
        tc.add_generating_pair(u, v)
    tc.run()
    tc.standardize(Order.shortlex)
    expected = tc.word_graph()

    num = word_graph.number_of_nodes_reachable_from(result, 0)
    assert expected.number_of_nodes() == num
    expected = expected.induced_subgraph(0, num)
    assert result == expected


def check_meets_and_joins(_):
    # TODO(1): Implement correct function once issue in libsemigroups is resolved.
    assert True


@pytest.mark.quick
def test_sims1_000():
    """fp example 1"""
    ReportGuard(False)

    p = Presentation([0, 1])
    p.contains_empty_word(True)
    presentation.add_rule(p, [0, 0, 0], [0])
    presentation.add_rule(p, [1, 1], [1])
    presentation.add_rule(p, [0, 1, 0, 1], [0])

    S = Sims1(word=list[int])
    assert S.presentation(p).number_of_threads(2).number_of_congruences(5) == 6
    with pytest.raises(LibsemigroupsError):
        S.number_of_congruences(0)
    with pytest.raises(LibsemigroupsError):
        S.find_if(0, lambda: False)
    with pytest.raises(LibsemigroupsError):
        S.for_each(0, lambda: False)
    with pytest.raises(LibsemigroupsError):
        S.iterator(0)
    assert S.number_of_congruences(1) == 1

    it = S.iterator(1)
    wg = next(it)
    assert wg == WordGraph(1, [[0, 0]])
    assert isinstance(wg, WordGraph)

    it = S.iterator(5)
    assert next(it) == WordGraph(5, [[0, 0]])
    assert next(it) == WordGraph(5, [[1, 0], [1, 1]])
    assert next(it) == WordGraph(5, [[1, 1], [1, 1]])
    assert next(it) == WordGraph(5, [[1, 2], [1, 1], [1, 2]])
    assert next(it) == WordGraph(5, [[1, 2], [1, 1], [2, 2]])
    assert next(it) == WordGraph(5, [[1, 2], [1, 1], [3, 2], [3, 3]])
    with pytest.raises(StopIteration):
        next(it)
    with pytest.raises(StopIteration):
        next(it)

    it = S.iterator(3)
    assert next(it) == WordGraph(3, [[0, 0]])
    S.number_of_threads(1).for_each(
        5, lambda wg: check_right_generating_pairs(S, wg)
    )
    presentation.reverse(p)
    S = Sims1(word=list[int])
    assert S.presentation(p).number_of_congruences(5) == 9
    for wg in S.iterator(5):
        assert word_graph.follow_path(
            wg, 0, [1, 0, 1, 0]
        ) == word_graph.follow_path(wg, 0, [0])
    S.for_each(5, lambda wg: check_right_generating_pairs(S, wg))
    mat = sims.poset(S, 5)
    assert mat == Matrix(
        MatrixKind.Boolean,
        [
            [0, 0, 0, 0, 0, 0, 0, 0, 0],
            [1, 0, 0, 0, 0, 0, 0, 0, 0],
            [1, 0, 0, 0, 0, 0, 0, 0, 0],
            [1, 0, 0, 0, 0, 0, 0, 0, 0],
            [0, 0, 1, 1, 0, 0, 0, 0, 0],
            [1, 0, 0, 0, 0, 0, 0, 0, 0],
            [0, 0, 0, 1, 0, 1, 0, 0, 0],
            [0, 1, 1, 0, 0, 1, 0, 0, 0],
            [0, 0, 0, 0, 1, 0, 1, 1, 0],
        ],
    )
    check_meets_and_joins(S.iterator(5))


@pytest.mark.quick
def test_sims1_001():
    """fp example 2"""
    ReportGuard(False)
    p = Presentation([0, 1, 2])
    p.contains_empty_word(True)

    presentation.add_rule(p, [0, 1, 0], [0, 0])
    presentation.add_rule(p, [2, 2], [0, 0])
    presentation.add_rule(p, [0, 0, 0], [0, 0])
    presentation.add_rule(p, [2, 1], [1, 2])
    presentation.add_rule(p, [2, 0], [0, 0])
    presentation.add_rule(p, [1, 1], [1])
    presentation.add_rule(p, [0, 2], [0, 0])

    S = Sims1(word=list[int])
    S.presentation(p)
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
    assert next(it) == WordGraph(2, [[0, 0, 0]])
    assert next(it) == WordGraph(2, [[1, 0, 1], [1, 1, 1]])
    assert next(it) == WordGraph(2, [[1, 1, 1], [1, 1, 1]])
    with pytest.raises(StopIteration):
        next(it)
    with pytest.raises(StopIteration):
        next(it)

    presentation.reverse(p)
    S.init(p)
    assert S.number_of_congruences(11) == 176


@pytest.mark.quick
def test_sims1_002():
    """ToddCoxeter failing example (word_type)"""
    ReportGuard(False)
    p = Presentation([0, 1, 2, 3, 4, 5, 6])
    p.contains_empty_word(False)

    presentation.add_identity_rules(p, 6)
    presentation.add_inverse_rules(p, [1, 0, 3, 2, 5, 4, 6], 6)
    presentation.add_rule(p, [0, 0, 5, 0, 4], [6])
    presentation.add_rule(p, [0, 4, 2, 2, 1, 5, 2], [6])
    presentation.add_rule(p, [1, 3, 0, 2, 4, 4, 4], [6])
    S = Sims1(word=list[int])
    S.presentation(p)

    S.for_each(3, lambda wg: check_right_generating_pairs(S, wg))

    it = S.iterator(3)
    wg = next(it)
    assert list(sims.right_generating_pairs(wg)) == [
        ([1], [0]),
        ([2], [0]),
        ([3], [0]),
        ([4], [0]),
        ([5], [0]),
        ([6], [0]),
        ([0, 0], [0]),
        ([0, 1], [0]),
        ([0, 2], [0]),
        ([0, 3], [0]),
        ([0, 4], [0]),
        ([0, 5], [0]),
        ([0, 6], [0]),
    ]
    assert list(sims.right_generating_pairs(p, wg)) == [
        ([1], [0]),
        ([2], [0]),
        ([3], [0]),
        ([4], [0]),
        ([5], [0]),
        ([6], [0]),
    ]

    check_right_generating_pairs(S, wg)

    wg = next(it)
    check_right_generating_pairs(S, wg)
    assert list(sims.right_generating_pairs(wg)) == [
        ([1], [0]),
        ([2], [0]),
        ([3], [0]),
        ([6], [0]),
        ([0, 0], [0]),
        ([0, 1], [0]),
        ([0, 2], [0]),
        ([0, 3], [0]),
        ([0, 4], [4]),
        ([0, 5], [5]),
        ([0, 6], [0]),
        ([4, 0], [4]),
        ([4, 1], [4]),
        ([4, 2], [4]),
        ([4, 3], [4]),
        ([4, 4], [5]),
        ([4, 5], [0]),
        ([4, 6], [4]),
        ([5, 0], [5]),
        ([5, 1], [5]),
        ([5, 2], [5]),
        ([5, 3], [5]),
        ([5, 4], [0]),
        ([5, 5], [4]),
        ([5, 6], [5]),
    ]
    assert list(sims.right_generating_pairs(p, wg)) == [
        ([1], [0]),
        ([2], [0]),
        ([3], [0]),
        ([6], [0]),
        ([4, 0], [4]),
        ([4, 2], [4]),
        ([4, 4], [5]),
        ([5, 0], [5]),
    ]


@pytest.mark.quick
def test_sims1_003():
    """ToddCoxeter failing example (std::string)"""
    ReportGuard(False)
    p = Presentation("aAbBcCe")
    p.contains_empty_word(False)

    presentation.add_identity_rules(p, "e")

    presentation.add_inverse_rules(p, "AaBbCce", "e")
    presentation.add_rule(p, "aaCac", "e")
    presentation.add_rule(p, "acbbACb", "e")
    presentation.add_rule(p, "ABabccc", "e")
    S = Sims1(word=list[int])
    S.presentation(to(p, rtype=(Presentation, list[int])))
    assert S.number_of_congruences(3) == 14


@pytest.mark.quick
def test_sims1_004():
    """partition_monoid(2) right"""
    ReportGuard(False)
    p = Presentation([0, 1, 2, 3])
    p.contains_empty_word(False)

    presentation.add_identity_rules(p, 0)
    presentation.add_rule(p, [1, 1], [0])
    presentation.add_rule(p, [1, 3], [3])
    presentation.add_rule(p, [2, 2], [2])
    presentation.add_rule(p, [3, 1], [3])
    presentation.add_rule(p, [3, 3], [3])
    presentation.add_rule(p, [2, 3, 2], [2])
    presentation.add_rule(p, [3, 2, 3], [3])
    presentation.add_rule(p, [1, 2, 1, 2], [2, 1, 2])
    presentation.add_rule(p, [2, 1, 2, 1], [2, 1, 2])

    S = Sims1(p)
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

    T = Sims2(word=list[int])
    T.presentation(p)
    T.number_of_threads(2)
    assert T.number_of_congruences(16) == 13

    orc = MinimalRepOrc(word=list[int])
    d = (
        orc.presentation(p)
        .target_size(15)
        .number_of_threads(os.cpu_count())
        .word_graph()
    )

    assert d.number_of_nodes() == 7


@pytest.mark.quick
def test_sims_refiner_faithful_128():
    ReportGuard(True)
    p = Presentation([0, 1])
    p.contains_empty_word(True)
    presentation.add_rule(p, [0, 0, 0], [0])
    presentation.add_rule(p, [1, 1, 1], [])
    presentation.add_rule(p, [0, 1, 1], [1, 0])

    forbid = [[0], [0, 1], [0, 0], []]
    pruno = SimsRefinerFaithful(forbid)

    S = Sims1(word=list[int])
    S.presentation(p)
    S.add_pruner(pruno)
    assert (
        S.number_of_threads(2).number_of_congruences(9) == 4
    )  # Verified with GAP

    it = S.iterator(9)

    assert next(it) == WordGraph(
        9, [[1, 2], [1, 3], [4, 5], [4, 4], [3, 1], [3, 0]]
    )
    assert next(it) == WordGraph(
        9, [[1, 2], [3, 3], [4, 5], [1, 4], [4, 1], [3, 0]]
    )
    assert next(it) == WordGraph(
        9, [[1, 2], [3, 4], [3, 5], [1, 1], [4, 3], [4, 0]]
    )
    assert next(it) == WordGraph(
        9,
        [
            [1, 2],
            [3, 4],
            [5, 6],
            [1, 7],
            [8, 5],
            [7, 1],
            [4, 0],
            [5, 8],
            [4, 3],
        ],
    )
    with pytest.raises(StopIteration):
        next(it)


def test_sims1_901():
    ReportGuard(False)
    p = Presentation(list(range(4)))
    presentation.add_rule(p, [1, 2, 1], [1, 1])
    presentation.add_rule(p, [3, 3], [1, 1])
    presentation.add_rule(p, [1, 1, 1], [1, 1])
    presentation.add_rule(p, [3, 2], [2, 3])
    presentation.add_rule(p, [3, 1], [1, 1])
    presentation.add_rule(p, [2, 2], [2])
    presentation.add_rule(p, [1, 3], [1, 1])
    s = Sims1(p)
    assert s.number_of_congruences(2) == 67
    assert s.number_of_threads(2).number_of_congruences(2) == 67
    assert s.number_of_threads(4).number_of_congruences(2) == 67
    assert s.number_of_threads(8).number_of_congruences(2) == 67


def test_sims1_902():
    ReportGuard(False)
    p = Presentation("abcd")
    presentation.add_rule(p, "bcb", "bb")
    presentation.add_rule(p, "dd", "bb")
    presentation.add_rule(p, "bbb", "bb")
    presentation.add_rule(p, "dc", "cd")
    presentation.add_rule(p, "db", "bb")
    presentation.add_rule(p, "cc", "c")
    presentation.add_rule(p, "bd", "bb")
    s = Sims1(to(p, rtype=(Presentation, list[int])))
    assert s.number_of_congruences(2) == 67
    assert s.number_of_threads(2).number_of_congruences(2) == 67
    assert s.number_of_threads(4).number_of_congruences(2) == 67
    assert s.number_of_threads(8).number_of_congruences(2) == 67


def test_sims2_901():
    ReportGuard(False)
    p = Presentation([0, 1])
    presentation.add_rule(p, [0, 1], [1, 0])

    s = Sims2(p)
    assert s.number_of_congruences(1) == 1
    assert s.number_of_congruences(2) == 9
    assert s.number_of_congruences(3) == 37
    assert s.number_of_congruences(4) == 117
    assert s.number_of_congruences(5) == 301
    assert s.number_of_congruences(6) == 699
    assert s.number_of_congruences(7) == 1484
    assert s.number_of_congruences(8) == 2947
    assert s.number_of_congruences(9) == 5607
    assert s.number_of_congruences(10) == 10168
    assert s.number_of_congruences(11) == 17942
    assert s.number_of_congruences(12) == 30650
    assert s.number_of_congruences(13) == 51292
    assert s.number_of_threads(2).number_of_congruences(4) == 117
    assert s.number_of_threads(4).number_of_congruences(4) == 117
    assert s.number_of_threads(8).number_of_congruences(4) == 117


def test_sims2_902():
    ReportGuard(False)
    p = Presentation("ab")
    presentation.add_rule(p, "ab", "ba")

    s = Sims2(to(p, rtype=(Presentation, list[int])))
    assert s.number_of_congruences(1) == 1
    assert s.number_of_congruences(2) == 9
    assert s.number_of_congruences(3) == 37
    assert s.number_of_congruences(4) == 117
    assert s.number_of_congruences(5) == 301
    assert s.number_of_congruences(6) == 699
    assert s.number_of_congruences(7) == 1484
    assert s.number_of_congruences(8) == 2947
    assert s.number_of_congruences(9) == 5607
    assert s.number_of_congruences(10) == 10168
    assert s.number_of_congruences(11) == 17942
    assert s.number_of_congruences(12) == 30650
    assert s.number_of_congruences(13) == 51292
    assert s.number_of_threads(2).number_of_congruences(4) == 117
    assert s.number_of_threads(4).number_of_congruences(4) == 117
    assert s.number_of_threads(8).number_of_congruences(4) == 117


def test_sims_refiner_ideals_901():
    p = Presentation([0, 1, 2])
    p.contains_empty_word(True)
    presentation.add_rule(p, [0, 0], [])
    presentation.add_rule(p, [0, 2], [2])
    presentation.add_rule(p, [1, 1], [1])
    presentation.add_rule(p, [2, 0], [2])
    presentation.add_rule(p, [2, 2], [2])
    presentation.add_rule(p, [1, 2, 1], [1])
    presentation.add_rule(p, [2, 1, 2], [2])
    presentation.add_rule(p, [0, 1, 0, 1], [1, 0, 1])
    presentation.add_rule(p, [1, 0, 1, 0], [1, 0, 1])

    ip = SimsRefinerIdeals(p)

    s = Sims1(p)
    s.add_pruner(ip)
    assert s.number_of_congruences(15) == 15
    assert s.number_of_threads(2).number_of_congruences(15) == 15
    assert s.number_of_threads(4).number_of_congruences(15) == 15
    assert s.number_of_threads(8).number_of_congruences(15) == 15


def test_sims_refiner_ideals_902():
    p = Presentation([0, 1])
    presentation.add_rule(p, [0, 0, 0], [1, 1])
    presentation.add_rule(p, [0, 0, 1], [1, 0])

    s = Sims2(p)
    pruner = SimsRefinerIdeals(s.presentation())
    s.add_pruner(pruner)
    assert s.number_of_congruences(1) == 1
    assert s.number_of_congruences(2) == 3
    assert s.number_of_congruences(3) == 5
    assert s.number_of_congruences(4) == 7
    assert s.number_of_congruences(5) == 9
    assert s.number_of_congruences(6) == 11
    assert s.number_of_congruences(7) == 12
    for n in range(8, 20):
        assert s.number_of_congruences(n) == 12
    assert s.number_of_threads(2).number_of_congruences(7) == 12
    assert s.number_of_threads(4).number_of_congruences(7) == 12
    assert s.number_of_threads(8).number_of_congruences(7) == 12


def test_sims1_return_policy():
    p = Presentation("ab")
    p.rules = ["a" * 5, "a", "b" * 4, "b", "ab", "ba"]
    q = to(p, rtype=(Presentation, list[int]))
    s = Sims1(q)

    assert s.presentation() is s.presentation()
    assert s.presentation(q) is s
    assert s.number_of_threads(2) is s
    assert s.first_long_rule_position(0) is s
    assert s.clear_long_rules() is s
    assert s.long_rule_length(0) is s
    assert s.pruners() is not s.pruners()
    assert s.add_pruner(lambda wg: True) is s
    assert s.pruners() is not s.pruners()
    assert s.clear_pruners() is s
    assert s.included_pairs() is not s.included_pairs()
    assert s.add_included_pair([0, 1], [0]) is s
    assert s.clear_included_pairs() is s
    assert s.excluded_pairs() is not s.excluded_pairs()
    assert s.add_excluded_pair([0, 1], [0]) is s
    assert s.clear_excluded_pairs() is s
    assert s.idle_thread_restarts(2) is s
    assert s.init(q) is s
    assert s.copy() is not s


def test_sims2_return_policy():
    p = Presentation("ab")
    p.rules = ["a" * 5, "a", "b" * 4, "b", "ab", "ba"]
    q = to(p, rtype=(Presentation, list[int]))
    s = Sims2(q)

    assert s.presentation() is s.presentation()
    assert s.presentation(q) is s
    assert s.number_of_threads(2) is s
    assert s.first_long_rule_position(0) is s
    assert s.clear_long_rules() is s
    assert s.long_rule_length(0) is s
    assert s.pruners() is not s.pruners()
    assert s.add_pruner(lambda wg: True) is s
    assert s.pruners() is not s.pruners()
    assert s.clear_pruners() is s
    assert s.included_pairs() is not s.included_pairs()
    assert s.add_included_pair([0, 1], [0]) is s
    assert s.clear_included_pairs() is s
    assert s.excluded_pairs() is not s.excluded_pairs()
    assert s.add_excluded_pair([0, 1], [0]) is s
    assert s.clear_excluded_pairs() is s
    assert s.idle_thread_restarts(2) is s
    assert s.init(q) is s
    assert s.copy() is not s


def test_rep_orc_return_policy():
    ro = RepOrc(word=list[int])
    assert ro.init() is ro
    assert ro.max_nodes(10) is ro
    assert ro.min_nodes(9) is ro
    assert ro.target_size(27) is ro


def test_min_rep_orc_return_policy():
    ro = MinimalRepOrc(word=list[int])
    assert ro.init() is ro
    assert ro.target_size(27) is ro


def test_sims_refiner_faithful_return_policy():
    srf = SimsRefinerFaithful(word=list[int])

    assert srf.init() is srf
    assert srf.init([[0, 1], [0]]) is srf


def test_sims_refiner_faithful_call():
    srf = SimsRefinerFaithful(word=list[int])
    wg = WordGraph(2, [[0, 1]])
    assert srf(wg)


def test_sims_refiner_ideals_return_policy():
    sri = SimsRefinerIdeals(word=list[int])
    assert sri.presentation() is sri.presentation()
    assert sri.init() is sri
    p = Presentation("ab")
    p.rules = ["a" * 5, "a", "b" * 4, "b", "ab", "ba"]
    q = to(p, rtype=(Presentation, list[int]))
    assert sri.init(q) is sri
