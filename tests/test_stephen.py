# -*- coding: utf-8 -*-

# Copyright (c) 2025, R. Cirpons
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for the libsemigroups_pybind11 functionality
arising from stephen.*pp in libsemigroups.
"""

# pylint: disable=missing-function-docstring, invalid-name, too-many-lines

from itertools import islice
from functools import cmp_to_key

import pytest

from libsemigroups_pybind11 import (
    InversePresentation,
    LibsemigroupsError,
    POSITIVE_INFINITY,
    Presentation,
    ReportGuard,
    Stephen,
    ToWord,
    ToddCoxeter,
    UNDEFINED,
    WordGraph,
    congruence_kind,
    lexicographical_compare,
    presentation,
    stephen,
    todd_coxeter,
    word_graph,
)

from libsemigroups_pybind11.presentation import examples


def check_000(s):
    s.set_word([0]).run()
    assert s.word_graph().number_of_nodes() == 2
    # TODO(1): use UNDEFINED once that works
    assert s.word_graph() == WordGraph(2, [[1, UNDEFINED], [UNDEFINED, 1]])
    assert stephen.number_of_words_accepted(s) == POSITIVE_INFINITY
    assert list(islice(stephen.words_accepted(s), 10)) == [
        [0],
        [0, 1],
        [0, 1, 1],
        [0, 1, 1, 1],
        [0, 1, 1, 1, 1],
        [0, 1, 1, 1, 1, 1],
        [0, 1, 1, 1, 1, 1, 1],
        [0, 1, 1, 1, 1, 1, 1, 1],
        [0, 1, 1, 1, 1, 1, 1, 1, 1],
        [0, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    ]
    assert list(islice(stephen.left_factors(s), 10)) == [
        [],
        [0],
        [0, 1],
        [0, 1, 1],
        [0, 1, 1, 1],
        [0, 1, 1, 1, 1],
        [0, 1, 1, 1, 1, 1],
        [0, 1, 1, 1, 1, 1, 1],
        [0, 1, 1, 1, 1, 1, 1, 1],
        [0, 1, 1, 1, 1, 1, 1, 1, 1],
    ]


# The following is needed to that we can sort lists with a custom order.
# Why does Python not support a comparator directly? Who knows.
# Why does it not automatically convert a boolean comparator to a key function? WHO KNOWS!
def lexicographic_compare_comparator(x, y) -> int:
    if x == y:
        return 0
    if lexicographical_compare(x, y):
        return -1
    return 1


lexicographic_compare_key_func = cmp_to_key(lexicographic_compare_comparator)


def verify_c4_normal_form(p, word, nf):
    S = Stephen(p)
    S.set_word(word).run()

    assert (
        sorted(
            list(stephen.words_accepted(S)), key=lexicographic_compare_key_func
        )[0]
        == nf
    )

    assert all(stephen.accepts(S, w) for w in stephen.words_accepted(S))
    assert stephen.number_of_words_accepted(S) == len(stephen.words_accepted(S))


def verify_c4_equal_to(p, word1, word2):
    S = Stephen(p)
    S.set_word(word1).run()
    assert stephen.accepts(S, word2)
    S.set_word(word2).run()
    assert stephen.accepts(S, word1)


def verify_c4_not_equal_to(p, word1, word2):
    S = Stephen(p)
    S.set_word(word1).run()
    assert not stephen.accepts(S, word2)
    S.set_word(word2).run()
    assert not stephen.accepts(S, word1)


@pytest.mark.quick
def test_stephen_000():
    """basic test 1"""
    ReportGuard(False)
    p = Presentation([0, 1])
    presentation.add_rule(p, [0], [0, 1])
    s = Stephen(p)
    check_000(s)
    s.init(p)
    check_000(s)


@pytest.mark.quick
def test_stephen_001():
    """basic test 2"""
    ReportGuard(False)
    p = Presentation([0, 1])
    presentation.add_rule(p, [0, 0, 0], [0])
    presentation.add_rule(p, [1, 1, 1], [1])
    presentation.add_rule(p, [0, 1, 0, 1], [0, 0])
    s = Stephen(p)
    s.set_word([1, 1, 0, 1]).run()
    assert s.word_graph().number_of_nodes() == 7
    assert s.word_graph() == WordGraph(
        7,
        [
            [UNDEFINED, 1],
            [UNDEFINED, 2],
            [3, 1],
            [4, 5],
            [3, 6],
            [6, 3],
            [5, 4],
        ],
    )
    assert stephen.number_of_words_accepted(s) == POSITIVE_INFINITY

    w = [1, 1, 0, 1]
    assert word_graph.last_node_on_path(s.word_graph(), 0, w)[0] == 5
    w = [1, 1, 0, 0, 1, 0]
    assert word_graph.last_node_on_path(s.word_graph(), 0, w)[0] == 5

    assert stephen.accepts(s, [1, 1, 0, 0, 1, 0])
    assert stephen.accepts(s, [1, 1, 0, 0, 1, 0])
    assert not stephen.accepts(s, [])
    assert not stephen.accepts(s, [0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
    assert not stephen.accepts(s, [1, 1, 1])

    assert list(islice(stephen.words_accepted(s), 10)) == [
        [1, 1, 0, 1],
        [1, 1, 0, 0, 0, 1],
        [1, 1, 0, 0, 1, 0],
        [1, 1, 0, 1, 0, 0],
        [1, 1, 0, 1, 1, 1],
        [1, 1, 1, 1, 0, 1],
        [1, 1, 0, 0, 0, 0, 0, 1],
        [1, 1, 0, 0, 0, 0, 1, 0],
        [1, 1, 0, 0, 0, 1, 0, 0],
        [1, 1, 0, 0, 0, 1, 1, 1],
    ]
    assert list(islice(stephen.left_factors(s), 10)) == [
        [],
        [1],
        [1, 1],
        [1, 1, 0],
        [1, 1, 1],
        [1, 1, 0, 0],
        [1, 1, 0, 1],
        [1, 1, 1, 1],
        [1, 1, 0, 0, 0],
        [1, 1, 0, 0, 1],
    ]
    assert stephen.number_of_left_factors(s) == POSITIVE_INFINITY
    assert all(
        stephen.is_left_factor(s, ww)
        for ww in islice(stephen.left_factors(s), 10)
    )

    s.set_word([0, 0]).run()
    assert s.word_graph().number_of_nodes() == 5
    assert s.word_graph() == WordGraph(
        5, [[1, UNDEFINED], [2, 3], [1, 4], [4, 1], [3, 2]]
    )

    p.rules = []
    presentation.add_rule(p, [0, 0, 0], [0])
    presentation.add_rule(p, [1, 1, 1], [1])
    s.init(p).set_word([0, 0]).run()
    assert s.word() == [0, 0]
    assert s.word_graph() == WordGraph(
        3, [[1, UNDEFINED], [2, UNDEFINED], [1, UNDEFINED]]
    )


@pytest.mark.quick
def test_stephen_002():
    """full transf monoid"""
    ReportGuard(False)
    n = 5
    p = examples.full_transformation_monoid_II74(n)

    s = Stephen(p)
    s.set_word([0, 1, 0, 1, 1, 1, 0, 2, 0, 1, 2, 0]).run()
    assert s.word_graph().number_of_nodes() == 120
    assert s.word_graph() == WordGraph(
        120,
        [
            [1, 2, 3, 4, UNDEFINED],
            [0, 5, 6, 7, UNDEFINED],
            [8, 0, 9, 10, UNDEFINED],
            [11, 12, 0, 13, UNDEFINED],
            [14, 15, 16, 0, UNDEFINED],
            [17, 1, 18, 19, UNDEFINED],
            [20, 21, 1, 22, UNDEFINED],
            [23, 24, 25, 1, UNDEFINED],
            [2, 17, 26, 27, UNDEFINED],
            [28, 29, 2, 30, UNDEFINED],
            [31, 32, 33, 2, UNDEFINED],
            [3, 34, 20, 35, UNDEFINED],
            [36, 3, 29, 37, UNDEFINED],
            [38, 39, 40, 3, UNDEFINED],
            [4, 41, 42, 23, UNDEFINED],
            [43, 4, 44, 32, UNDEFINED],
            [45, 46, 4, 40, UNDEFINED],
            [5, 8, 47, 48, UNDEFINED],
            [49, 50, 5, 51, UNDEFINED],
            [52, 53, 54, 5, UNDEFINED],
            [6, 55, 11, 56, UNDEFINED],
            [57, 6, 50, 58, UNDEFINED],
            [59, 60, 61, 6, UNDEFINED],
            [7, 62, 63, 14, UNDEFINED],
            [64, 7, 65, 53, UNDEFINED],
            [66, 67, 7, 61, UNDEFINED],
            [55, 57, 8, 68, UNDEFINED],
            [62, 64, 69, 8, UNDEFINED],
            [9, 49, 55, 70, UNDEFINED],
            [50, 9, 12, 71, UNDEFINED],
            [72, 73, 74, 9, UNDEFINED],
            [10, 52, 75, 62, UNDEFINED],
            [53, 10, 76, 15, UNDEFINED],
            [77, 78, 10, 74, UNDEFINED],
            [47, 11, 49, 79, UNDEFINED],
            [63, 80, 66, 11, UNDEFINED],
            [12, 47, 57, 81, UNDEFINED],
            [82, 76, 78, 12, UNDEFINED],
            [13, 83, 59, 63, UNDEFINED],
            [84, 13, 73, 76, UNDEFINED],
            [61, 74, 13, 16, UNDEFINED],
            [48, 14, 85, 52, UNDEFINED],
            [56, 86, 14, 59, UNDEFINED],
            [15, 48, 87, 64, UNDEFINED],
            [88, 71, 15, 73, UNDEFINED],
            [16, 89, 56, 66, UNDEFINED],
            [90, 16, 71, 78, UNDEFINED],
            [34, 36, 17, 91, UNDEFINED],
            [41, 43, 92, 17, UNDEFINED],
            [18, 28, 34, 93, UNDEFINED],
            [29, 18, 21, 94, UNDEFINED],
            [95, 96, 97, 18, UNDEFINED],
            [19, 31, 98, 41, UNDEFINED],
            [32, 19, 99, 24, UNDEFINED],
            [100, 101, 19, 97, UNDEFINED],
            [26, 20, 28, 102, UNDEFINED],
            [42, 103, 45, 20, UNDEFINED],
            [21, 26, 36, 104, UNDEFINED],
            [105, 99, 101, 21, UNDEFINED],
            [22, 106, 38, 42, UNDEFINED],
            [107, 22, 96, 99, UNDEFINED],
            [40, 97, 22, 25, UNDEFINED],
            [27, 23, 108, 31, UNDEFINED],
            [35, 109, 23, 38, UNDEFINED],
            [24, 27, 110, 43, UNDEFINED],
            [111, 94, 24, 96, UNDEFINED],
            [25, 112, 35, 45, UNDEFINED],
            [113, 25, 94, 101, UNDEFINED],
            [106, 107, 114, 26, UNDEFINED],
            [112, 113, 27, 114, UNDEFINED],
            [108, 111, 112, 28, UNDEFINED],
            [115, 44, 46, 29, UNDEFINED],
            [30, 95, 106, 108, UNDEFINED],
            [96, 30, 39, 44, UNDEFINED],
            [114, 40, 30, 33, UNDEFINED],
            [102, 105, 31, 106, UNDEFINED],
            [116, 37, 32, 39, UNDEFINED],
            [33, 100, 102, 112, UNDEFINED],
            [101, 33, 37, 46, UNDEFINED],
            [98, 116, 100, 34, UNDEFINED],
            [110, 35, 111, 116, UNDEFINED],
            [109, 110, 113, 36, UNDEFINED],
            [37, 98, 105, 109, UNDEFINED],
            [91, 38, 95, 98, UNDEFINED],
            [39, 91, 107, 110, UNDEFINED],
            [93, 115, 41, 95, UNDEFINED],
            [104, 42, 115, 105, UNDEFINED],
            [103, 104, 43, 107, UNDEFINED],
            [44, 93, 103, 111, UNDEFINED],
            [92, 45, 93, 100, UNDEFINED],
            [46, 92, 104, 113, UNDEFINED],
            [83, 84, 117, 47, UNDEFINED],
            [89, 90, 48, 117, UNDEFINED],
            [85, 88, 89, 49, UNDEFINED],
            [118, 65, 67, 50, UNDEFINED],
            [51, 72, 83, 85, UNDEFINED],
            [73, 51, 60, 65, UNDEFINED],
            [117, 61, 51, 54, UNDEFINED],
            [79, 82, 52, 83, UNDEFINED],
            [119, 58, 53, 60, UNDEFINED],
            [54, 77, 79, 89, UNDEFINED],
            [78, 54, 58, 67, UNDEFINED],
            [75, 119, 77, 55, UNDEFINED],
            [87, 56, 88, 119, UNDEFINED],
            [86, 87, 90, 57, UNDEFINED],
            [58, 75, 82, 86, UNDEFINED],
            [68, 59, 72, 75, UNDEFINED],
            [60, 68, 84, 87, UNDEFINED],
            [70, 118, 62, 72, UNDEFINED],
            [81, 63, 118, 82, UNDEFINED],
            [80, 81, 64, 84, UNDEFINED],
            [65, 70, 80, 88, UNDEFINED],
            [69, 66, 70, 77, UNDEFINED],
            [67, 69, 81, 90, UNDEFINED],
            [74, 117, 68, 69, UNDEFINED],
            [71, 85, 86, 118, UNDEFINED],
            [76, 79, 119, 80, UNDEFINED],
            [97, 114, 91, 92, UNDEFINED],
            [94, 108, 109, 115, UNDEFINED],
            [99, 102, 116, 103, UNDEFINED],
        ],
    )


# TODO(2): add a version of all test cases for std::string once this is
# allowed by Stephen.
@pytest.mark.quick
def test_stephen_003():
    """from step_hen 002"""
    ReportGuard(False)
    to_word = ToWord("ab")
    p = Presentation(to_word("ab"))
    presentation.add_rule(p, to_word("aaa"), to_word("a"))
    presentation.add_rule(p, to_word("bbb"), to_word("b"))
    presentation.add_rule(p, to_word("abab"), to_word("aa"))

    S = Stephen(p)
    S.set_word(to_word("bbab"))

    assert stephen.accepts(S, to_word("bbaaba"))
    assert not stephen.accepts(S, to_word(""))
    assert not stephen.accepts(S, to_word("aaaaaaaaaa"))
    assert not stephen.accepts(S, to_word("bbb"))

    S.set_word(to_word("bba"))
    assert stephen.accepts(S, to_word("bbabb"))
    assert stephen.accepts(S, to_word("bba"))
    assert not stephen.accepts(S, to_word("bbb"))
    assert not stephen.accepts(S, to_word("a"))
    assert not stephen.accepts(S, to_word("ab"))

    S.set_word(to_word("bbaab"))
    assert stephen.accepts(S, to_word("bbaba"))


@pytest.mark.quick
def test_stephen_004():
    """from step_hen 003"""
    ReportGuard(False)
    to_word = ToWord("abcdefg")
    p = Presentation(to_word("abcdefg"))
    presentation.add_rule(p, to_word("aaaeaa"), to_word("abcd"))
    presentation.add_rule(p, to_word("ef"), to_word("dg"))

    S = Stephen(p)

    S.set_word(to_word("abcef")).run()
    assert to_word("abcef") == [0, 1, 2, 4, 5]
    assert S.word_graph() == WordGraph(
        11,
        [
            [
                1,
                UNDEFINED,
                UNDEFINED,
                UNDEFINED,
                UNDEFINED,
                UNDEFINED,
                UNDEFINED,
            ],
            [2, 3],
            [4],
            [UNDEFINED, UNDEFINED, 5],
            [UNDEFINED, UNDEFINED, UNDEFINED, UNDEFINED, 6],
            [UNDEFINED, UNDEFINED, UNDEFINED, 7, 8],
            [9],
            [
                UNDEFINED,
                UNDEFINED,
                UNDEFINED,
                UNDEFINED,
                UNDEFINED,
                UNDEFINED,
                10,
            ],
            [UNDEFINED, UNDEFINED, UNDEFINED, UNDEFINED, UNDEFINED, 10],
            [7],
        ],
    )
    rule = p.rules[0]
    m = word_graph.last_node_on_path(S.word_graph(), 0, rule)[0]
    rule = p.rules[1]
    n = word_graph.last_node_on_path(S.word_graph(), 0, rule)[0]
    assert m != UNDEFINED
    assert n != UNDEFINED
    assert m == n
    assert S.word_graph().number_of_nodes() == 11
    assert stephen.accepts(S, to_word("aaaeaag"))
    assert stephen.number_of_words_accepted(S) == 3
    assert list(stephen.words_accepted(S)) == [
        [0, 1, 2, 3, 6],
        [0, 1, 2, 4, 5],
        [0, 0, 0, 4, 0, 0, 6],
    ]

    S.set_word(to_word("aaaeaaaeaa")).run()

    assert S.word_graph().number_of_nodes() == 15
    assert stephen.number_of_words_accepted(S) == 3
    assert stephen.accepts(S, to_word("aaaeabcd"))
    assert list(stephen.words_accepted(S)) == [
        [0, 0, 0, 4, 0, 1, 2, 3],
        [0, 1, 2, 3, 0, 4, 0, 0],
        [0, 0, 0, 4, 0, 0, 0, 4, 0, 0],
    ]

    S.set_word(to_word("aaaeaag")).run()
    assert S.word_graph().number_of_nodes() == 11
    assert stephen.number_of_words_accepted(S) == 3
    assert list(stephen.words_accepted(S)) == [
        [0, 1, 2, 3, 6],
        [0, 1, 2, 4, 5],
        [0, 0, 0, 4, 0, 0, 6],
    ]


@pytest.mark.quick
def test_stephen_005():
    """from step_hen 004"""
    ReportGuard(False)
    to_word = ToWord("abc")
    p = Presentation(to_word("abc"))
    presentation.add_rule(p, to_word("ab"), to_word("ba"))
    presentation.add_rule(p, to_word("ac"), to_word("cc"))
    presentation.add_rule(p, to_word("ac"), to_word("a"))
    presentation.add_rule(p, to_word("cc"), to_word("a"))
    presentation.add_rule(p, to_word("bc"), to_word("cc"))
    presentation.add_rule(p, to_word("bcc"), to_word("b"))
    presentation.add_rule(p, to_word("bc"), to_word("b"))
    presentation.add_rule(p, to_word("cc"), to_word("b"))
    presentation.add_rule(p, to_word("a"), to_word("b"))

    S = Stephen(p)
    S.set_word(to_word("abcc")).run()
    assert stephen.accepts(S, to_word("baac"))
    assert S.word_graph().number_of_nodes() == 3
    assert stephen.number_of_words_accepted(S) == POSITIVE_INFINITY
    assert list(islice(stephen.words_accepted(S), 10)) == [
        to_word("a"),
        to_word("b"),
        to_word("aa"),
        to_word("ab"),
        to_word("ac"),
        to_word("ba"),
        to_word("bb"),
        to_word("bc"),
        to_word("ca"),
        to_word("cb"),
    ]


@pytest.mark.quick
def test_stephen_006():
    """from step_hen 005"""
    ReportGuard(False)
    to_word = ToWord("abcd")
    p = Presentation(to_word("abcd"))
    presentation.add_rule(p, to_word("bb"), to_word("c"))
    presentation.add_rule(p, to_word("caca"), to_word("abab"))
    presentation.add_rule(p, to_word("bc"), to_word("d"))
    presentation.add_rule(p, to_word("cb"), to_word("d"))
    presentation.add_rule(p, to_word("aa"), to_word("d"))
    presentation.add_rule(p, to_word("ad"), to_word("a"))
    presentation.add_rule(p, to_word("da"), to_word("a"))
    presentation.add_rule(p, to_word("bd"), to_word("b"))
    presentation.add_rule(p, to_word("db"), to_word("b"))
    presentation.add_rule(p, to_word("cd"), to_word("c"))
    presentation.add_rule(p, to_word("dc"), to_word("c"))

    S = Stephen(p)
    S.set_word(to_word("dabdaaadabab")).run()
    assert stephen.accepts(S, to_word("abdadcaca"))
    assert S.word_graph().number_of_nodes() == 25
    assert stephen.number_of_words_accepted(S) == POSITIVE_INFINITY


# TODO(2): uncomment once tests dont automatically run extreme ones
# @pytest.mark.extreme
# def test_stephen_007():
#     """Fibonacci(4, 6)"""
#     # [stephen][extreme]") {
#     ReportGuard(False);
#     S = Stephen(examples.fibonacci_semigroup(4, 6))
#     S.set_word([0, 1, 2, 3]).run_for(timedelta(seconds=10))
#     assert not S.finished()


@pytest.mark.quick
def test_stephen_008():
    """C(4) monoid normal form (test_case_knuth_bendix_055)"""
    ReportGuard(False)
    to_word = ToWord("abcdefg")
    p = Presentation(to_word("abcdefg"))
    presentation.add_rule(p, to_word("abcd"), to_word("ce"))
    presentation.add_rule(p, to_word("df"), to_word("dg"))

    S = Stephen(p)
    S.set_word(to_word("dfabcdf")).run()

    assert S.word_graph().number_of_nodes() == 9
    assert stephen.number_of_words_accepted(S) == 8

    assert list(stephen.words_accepted(S)) == [
        to_word("dfcef"),
        to_word("dfceg"),
        to_word("dgcef"),
        to_word("dgceg"),
        to_word("dfabcdf"),
        to_word("dfabcdg"),
        to_word("dgabcdf"),
        to_word("dgabcdg"),
    ]

    assert sorted(
        list(stephen.words_accepted(S)), key=lexicographic_compare_key_func
    )[0] == to_word("dfabcdf")

    assert all(stephen.accepts(S, w) for w in stephen.words_accepted(S))
    assert stephen.number_of_words_accepted(S) == len(stephen.words_accepted(S))

    S.set_word(to_word("abcdfceg")).run()
    assert stephen.number_of_words_accepted(S) == 16

    assert sorted(
        list(stephen.words_accepted(S)), key=lexicographic_compare_key_func
    ) == [
        to_word("abcdfabcdf"),
        to_word("abcdfabcdg"),
        to_word("abcdfcef"),
        to_word("abcdfceg"),
        to_word("abcdgabcdf"),
        to_word("abcdgabcdg"),
        to_word("abcdgcef"),
        to_word("abcdgceg"),
        to_word("cefabcdf"),
        to_word("cefabcdg"),
        to_word("cefcef"),
        to_word("cefceg"),
        to_word("cegabcdf"),
        to_word("cegabcdg"),
        to_word("cegcef"),
        to_word("cegceg"),
    ]

    assert sorted(
        list(stephen.words_accepted(S)), key=lexicographic_compare_key_func
    )[0] == to_word("abcdfabcdf")
    assert stephen.accepts(S, to_word("abcdfabcdf"))


@pytest.mark.quick
def test_Stephen_009():
    """C(4) monoid normal form (test_case_gap_smalloverlap_85)"""
    # [stephen][quick]") {
    ReportGuard(False)
    to_word = ToWord("abc")
    p = Presentation(to_word("cab"))
    presentation.add_rule(p, to_word("aabc"), to_word("acba"))

    S = Stephen(p)
    S.set_word(to_word("a")).run()
    assert not stephen.accepts(S, to_word("b"))

    S.set_word(to_word("aabcabc")).run()
    assert stephen.accepts(S, to_word("aabccba"))

    S.set_word(to_word("aabccba")).run()
    assert stephen.accepts(S, to_word("aabcabc"))

    S.set_word(to_word("acba")).run()

    assert list(stephen.words_accepted(S)) == [to_word("aabc"), to_word("acba")]

    verify_c4_normal_form(p, to_word("acba"), to_word("aabc"))


@pytest.mark.quick
def test_stephen_010():
    """code coverage"""
    ReportGuard(False)
    to_word = ToWord("abcdefg")
    p = Presentation([])
    # TODO(2): Once we have make<Stephen> check that this error is thrown
    # with pytest.raises(LibsemigroupsError):
    #     Stephen(p)
    p.alphabet(to_word("abcdefg"))

    S = Stephen(p)
    w = to_word("abbbddbcbcbc")
    S.set_word(w)
    S.run()
    assert S.finished()
    S.run()
    # resets
    S.set_word(to_word("abbbddbcbcbc"))
    # S.report_every(timedelta(microseconds=10))

    S.run()

    T = S.copy()
    assert stephen.accepts(T, w)
    assert not stephen.accepts(T, to_word("abbbd"))
    assert stephen.number_of_words_accepted(T) == 1
    assert stephen.number_of_left_factors(T) == len(w) + 1
    assert stephen.accepts(S, w)
    assert not stephen.accepts(S, to_word("abbbd"))
    assert stephen.number_of_words_accepted(S) == 1
    assert stephen.number_of_left_factors(S) == len(w) + 1

    S = T
    assert stephen.accepts(T, w)
    assert not stephen.accepts(T, to_word("abbbd"))
    assert stephen.number_of_words_accepted(T) == 1
    assert stephen.number_of_left_factors(T) == len(w) + 1
    assert stephen.accepts(S, w)
    assert not stephen.accepts(S, to_word("abbbd"))
    assert stephen.number_of_words_accepted(S) == 1
    assert stephen.number_of_left_factors(S) == len(w) + 1


@pytest.mark.quick
def test_Stephen_011():
    """C(4) monoid normal form (test_case_gap_smalloverlap_49)"""
    ReportGuard(False)
    to_word = ToWord("abcdefgh")
    p = Presentation(to_word("abcdefgh"))

    presentation.add_rule(p, to_word("abcd"), to_word("ce"))
    presentation.add_rule(p, to_word("df"), to_word("hd"))
    verify_c4_equal_to(p, to_word("abchd"), to_word("abcdf"))
    verify_c4_equal_to(p, to_word("abchd"), to_word("abchd"))
    verify_c4_equal_to(p, to_word("abchdf"), to_word("abchhd"))
    verify_c4_equal_to(p, to_word("abchd"), to_word("cef"))
    verify_c4_equal_to(p, to_word("cef"), to_word("abchd"))
    verify_c4_not_equal_to(p, to_word("abchf"), to_word("abcdf"))

    verify_c4_equal_to(p, to_word("hdfabce"), to_word("dffababcd"))

    verify_c4_normal_form(p, to_word("hdfabce"), to_word("dffababcd"))


@pytest.mark.quick
def test_Stephen_012():
    """C(4) monoid normal form (test_case_gap_smalloverlap_63)"""
    ReportGuard(False)
    to_word = ToWord("abcdefgh")
    p = Presentation(to_word("abcdefgh"))

    presentation.add_rule(p, to_word("afh"), to_word("bgh"))
    presentation.add_rule(p, to_word("hc"), to_word("d"))
    verify_c4_equal_to(p, to_word("afd"), to_word("bgd"))
    verify_c4_equal_to(p, to_word("bghcafhbgd"), to_word("afdafhafd"))
    verify_c4_normal_form(p, to_word("bghcafhbgd"), to_word("afdafhafd"))


@pytest.mark.quick
def test_Stephen_013():
    """C(4) monoid equal to (test_case_gap_smalloverlap_70)"""
    ReportGuard(False)
    to_word = ToWord("abcdefghij")
    p = Presentation(to_word("abcdefghij"))

    presentation.add_rule(p, to_word("afh"), to_word("bgh"))
    presentation.add_rule(p, to_word("hc"), to_word("de"))
    presentation.add_rule(p, to_word("ei"), to_word("j"))

    verify_c4_equal_to(p, to_word("afdj"), to_word("bgdj"))
    verify_c4_not_equal_to(p, to_word("jjjjjjjjjjjjjjjjjjjjjjj"), to_word("b"))


@pytest.mark.quick
def test_stephen_014():
    """C(4) monoid normal form (test_case_ex_3_13_14)"""
    ReportGuard(False)
    to_word = ToWord("abcd")
    p = Presentation(to_word("abcd"))
    presentation.add_rule(p, to_word("abbba"), to_word("cdc"))
    verify_c4_normal_form(
        p, to_word("cdcdcabbbabbbabbcd"), to_word("abbbadcabbbabbbabbcd")
    )
    verify_c4_equal_to(
        p, to_word("cdcdcabbbabbbabbcd"), to_word("abbbadcabbbabbbabbcd")
    )

    verify_c4_equal_to(p, to_word("abbbadcbbba"), to_word("cdabbbcdc"))
    verify_c4_equal_to(p, to_word("cdabbbcdc"), to_word("cdabbbcdc"))
    verify_c4_normal_form(p, to_word("cdabbbcdc"), to_word("abbbadcbbba"))


@pytest.mark.quick
def test_stephen_015():
    """C(4) monoid normal form (test_case_ex_3_15)"""
    ReportGuard(False)
    to_word = ToWord("abcd")
    p = Presentation(to_word("abcd"))
    presentation.add_rule(p, to_word("aabc"), to_word("acba"))
    original = to_word("cbacbaabcaabcacbacba")
    expected = to_word("cbaabcabcaabcaabcabc")

    verify_c4_equal_to(p, to_word("cbaabcabcaabcaabccba"), original)
    verify_c4_equal_to(p, original, expected)
    verify_c4_equal_to(p, expected, original)
    verify_c4_equal_to(p, to_word("cbaabcabcaabcaabccba"), expected)

    verify_c4_equal_to(p, original, to_word("cbaabcabcaabcaabccba"))

    verify_c4_equal_to(p, expected, to_word("cbaabcabcaabcaabccba"))
    verify_c4_normal_form(p, original, expected)


@pytest.mark.quick
def test_stephen_016():
    """C(4) monoid normal form (test_case_ex_3_16)"""
    ReportGuard(False)
    to_word = ToWord("abcd")
    p = Presentation(to_word("abcd"))
    presentation.add_rule(p, to_word("abcd"), to_word("acca"))
    original = to_word("bbcabcdaccaccabcddd")
    expected = to_word("bbcabcdabcdbcdbcddd")

    verify_c4_equal_to(p, original, expected)
    verify_c4_equal_to(p, expected, original)

    verify_c4_normal_form(p, original, expected)
    verify_c4_normal_form(p, expected, expected)


@pytest.mark.quick
def test_stephen_017():
    """C(4) monoid normal form (test_case_mt_3)"""
    ReportGuard(False)
    to_word = ToWord("abcd")
    p = Presentation(to_word("abcd"))
    presentation.add_rule(p, to_word("abcd"), to_word("accca"))

    verify_c4_normal_form(
        p, to_word("bbcabcdaccaccabcddd"), to_word("bbcabcdaccaccabcddd")
    )
    verify_c4_equal_to(
        p, to_word("bbcabcdaccaccabcddd"), to_word("bbcabcdaccaccabcddd")
    )


@pytest.mark.quick
def test_stephen_018():
    """C(4) monoid normal form (test_case_mt_5)"""
    ReportGuard(False)
    to_word = ToWord("abc")
    p = Presentation(to_word("abc"))
    presentation.add_rule(p, to_word("ac"), to_word("cbbbbc"))

    verify_c4_normal_form(p, to_word("acbbbbc"), to_word("aac"))
    verify_c4_equal_to(p, to_word("acbbbbc"), to_word("aac"))


@pytest.mark.quick
def test_stephen_019():
    """C(4) monoid normal form (test_case_mt_6)"""
    ReportGuard(False)
    to_word = ToWord("abc")
    p = Presentation(to_word("abc"))
    presentation.add_rule(p, to_word("ccab"), to_word("cbac"))

    verify_c4_normal_form(
        p, to_word("bacbaccabccabcbacbac"), to_word("bacbacbaccbaccbacbac")
    )
    verify_c4_equal_to(
        p, to_word("bacbaccabccabcbacbac"), to_word("bacbacbaccbaccbacbac")
    )
    verify_c4_normal_form(p, to_word("ccabcbaccab"), to_word("cbaccbacbac"))
    verify_c4_equal_to(p, to_word("ccabcbaccab"), to_word("cbaccbacbac"))


@pytest.mark.quick
def test_stephen_020():
    """C(4) monoid normal form (test_case_mt_10)"""
    ReportGuard(False)
    to_word = ToWord("abcdefghij")
    p = Presentation(to_word("abcdefghij"))
    presentation.add_rule(p, to_word("afh"), to_word("bgh"))
    presentation.add_rule(p, to_word("hc"), to_word("de"))
    presentation.add_rule(p, to_word("ei"), to_word("j"))

    verify_c4_normal_form(p, to_word("bgdj"), to_word("afdei"))
    verify_c4_equal_to(p, to_word("bgdj"), to_word("afdei"))


@pytest.mark.quick
def test_stephen_021():
    """C(4) monoid normal form (test_case_mt_13)"""
    ReportGuard(False)
    to_word = ToWord("abcd")
    p = Presentation(to_word("abcd"))
    presentation.add_rule(p, to_word("abcd"), to_word("dcba"))

    verify_c4_normal_form(p, to_word("dcbdcba"), to_word("abcdbcd"))
    verify_c4_equal_to(p, to_word("dcbdcba"), to_word("abcdbcd"))


@pytest.mark.quick
def test_stephen_022():
    """C(4) monoid normal form (test_case_mt_14)"""
    ReportGuard(False)
    to_word = ToWord("abcd")
    p = Presentation(to_word("abcd"))
    presentation.add_rule(p, to_word("abca"), to_word("dcbd"))

    verify_c4_normal_form(p, to_word("dcbabca"), to_word("abcacbd"))
    verify_c4_equal_to(p, to_word("dcbabca"), to_word("abcacbd"))


@pytest.mark.quick
def test_stephen_023():
    """C(4) monoid normal form (test_case_mt_15)"""
    ReportGuard(False)
    to_word = ToWord("abcd")
    p = Presentation(to_word("abcd"))
    presentation.add_rule(p, to_word("abcd"), to_word("dcba"))
    presentation.add_rule(p, to_word("adda"), to_word("dbbd"))

    verify_c4_normal_form(p, to_word("dbbabcd"), to_word("addacba"))
    verify_c4_equal_to(p, to_word("dbbabcd"), to_word("addacba"))


@pytest.mark.quick
def test_stephen_024():
    """C(4) monoid normal form (test_case_mt_16)"""
    ReportGuard(False)
    to_word = ToWord("abcdefg")
    p = Presentation(to_word("abcdefg"))
    presentation.add_rule(p, to_word("abcd"), to_word("acca"))
    presentation.add_rule(p, to_word("gf"), to_word("ge"))

    verify_c4_normal_form(p, to_word("accabcdgf"), to_word("abcdbcdge"))
    verify_c4_equal_to(p, to_word("accabcdgf"), to_word("abcdbcdge"))


@pytest.mark.quick
def test_stephen_025():
    """C(4) monoid normal form (test_case_mt_17)"""
    ReportGuard(False)
    to_word = ToWord("abcd")
    p = Presentation(to_word("abcd"))
    presentation.add_rule(
        p, to_word("ababbabbbabbbb"), to_word("abbbbbabbbbbbabbbbbbbabbbbbbbb")
    )
    presentation.add_rule(
        p, to_word("cdcddcdddcdddd"), to_word("cdddddcddddddcdddddddcdddddddd")
    )

    verify_c4_normal_form(
        p,
        to_word("abbbacdddddcddddddcdddddddcdddddddd"),
        to_word("abbbacdcddcdddcdddd"),
    )
    verify_c4_equal_to(
        p,
        to_word("abbbacdddddcddddddcdddddddcdddddddd"),
        to_word("abbbacdcddcdddcdddd"),
    )


@pytest.mark.quick
def test_stephen_026():
    """C(4) monoid normal form (test_case_weak_1)"""
    ReportGuard(False)
    to_word = ToWord("abcd")
    p = Presentation(to_word("abcd"))
    presentation.add_rule(p, to_word("acba"), to_word("aabc"))
    presentation.add_rule(p, to_word("acba"), to_word("dbbbd"))

    verify_c4_equal_to(p, to_word("aaabc"), to_word("adbbbd"))
    verify_c4_equal_to(p, to_word("adbbbd"), to_word("aaabc"))

    verify_c4_equal_to(p, to_word("aaabcadbbbd"), to_word("adbbbdadbbbd"))
    verify_c4_equal_to(p, to_word("aaabcaaabc"), to_word("adbbbdadbbbd"))
    verify_c4_equal_to(p, to_word("acba"), to_word("dbbbd"))
    verify_c4_equal_to(p, to_word("acbabbbd"), to_word("aabcbbbd"))
    verify_c4_equal_to(p, to_word("aabcbbbd"), to_word("acbabbbd"))


@pytest.mark.quick
def test_stephen_027():
    """C(4) monoid normal form (test_case_weak_2)"""
    ReportGuard(False)
    to_word = ToWord("abcd")
    p = Presentation(to_word("abcd"))
    presentation.add_rule(p, to_word("acba"), to_word("aabc"))
    presentation.add_rule(p, to_word("acba"), to_word("adbd"))
    verify_c4_equal_to(p, to_word("acbacba"), to_word("aabcabc"))
    verify_c4_normal_form(p, to_word("acbacba"), to_word("aabcabc"))
    verify_c4_equal_to(p, to_word("aabcabc"), to_word("acbacba"))
    verify_c4_normal_form(p, to_word("aabcabc"), to_word("aabcabc"))


@pytest.mark.quick
def test_stephen_028():
    """C(4) monoid normal form (test_case_weak_3)"""
    ReportGuard(False)
    to_word = ToWord("abcde")
    p = Presentation(to_word("abcde"))
    presentation.add_rule(p, to_word("bceac"), to_word("aeebbc"))
    presentation.add_rule(p, to_word("aeebbc"), to_word("dabcd"))
    verify_c4_normal_form(p, to_word("bceacdabcd"), to_word("aeebbcaeebbc"))
    verify_c4_normal_form(p, to_word("aeebbcaeebbc"), to_word("aeebbcaeebbc"))


@pytest.mark.quick
def test_stephen_029():
    """C(4) monoid normal form (test_case_weak_4)"""
    ReportGuard(False)
    to_word = ToWord("abcd")
    p = Presentation(to_word("abcd"))
    presentation.add_rule(p, to_word("acba"), to_word("aabc"))
    presentation.add_rule(p, to_word("acba"), to_word("dbbd"))
    verify_c4_normal_form(
        p, to_word("bbacbcaaabcbbd"), to_word("bbacbcaaabcbbd")
    )
    verify_c4_normal_form(p, to_word("acbacba"), to_word("aabcabc"))
    verify_c4_normal_form(p, to_word("aabcabc"), to_word("aabcabc"))


@pytest.mark.quick
def test_stephen_030():
    """C(4) monoid normal form (test_case_weak_5)"""
    ReportGuard(False)
    to_word = ToWord("abcd")
    p = Presentation(to_word("abcd"))
    presentation.add_rule(p, to_word("acba"), to_word("aabc"))
    presentation.add_rule(p, to_word("acba"), to_word("adbd"))
    verify_c4_normal_form(p, to_word("acbacba"), to_word("aabcabc"))
    verify_c4_normal_form(p, to_word("aabcabc"), to_word("aabcabc"))


@pytest.mark.quick
def test_stephen_031():
    """Test behaviour when uninitialised"""
    ReportGuard(False)
    p = Presentation([])
    S = Stephen(p)

    with pytest.raises(LibsemigroupsError):
        S.accept_state()
    with pytest.raises(LibsemigroupsError):
        S.word()
    with pytest.raises(LibsemigroupsError):
        S.word_graph()

    with pytest.raises(LibsemigroupsError):
        stephen.is_left_factor(S, [0, 0, 0])
    with pytest.raises(LibsemigroupsError):
        stephen.accepts(S, [0, 0, 0])
    with pytest.raises(LibsemigroupsError):
        stephen.number_of_left_factors(S)
    with pytest.raises(LibsemigroupsError):
        stephen.number_of_words_accepted(S)
    with pytest.raises(LibsemigroupsError):
        S.run()

    with pytest.raises(LibsemigroupsError):
        stephen.words_accepted(S)
    with pytest.raises(LibsemigroupsError):
        stephen.left_factors(S)

    p = Presentation([0, 1])
    presentation.add_rule(p, [0, 1], [1, 0])
    S.init(p)

    with pytest.raises(LibsemigroupsError):
        S.accept_state()
    with pytest.raises(LibsemigroupsError):
        S.word()
    with pytest.raises(LibsemigroupsError):
        S.word_graph()

    with pytest.raises(LibsemigroupsError):
        stephen.is_left_factor(S, [0, 0, 0])
    with pytest.raises(LibsemigroupsError):
        stephen.accepts(S, [0, 0, 0])
    with pytest.raises(LibsemigroupsError):
        stephen.number_of_left_factors(S)
    with pytest.raises(LibsemigroupsError):
        stephen.number_of_words_accepted(S)
    with pytest.raises(LibsemigroupsError):
        S.run()

    with pytest.raises(LibsemigroupsError):
        stephen.words_accepted(S)
    with pytest.raises(LibsemigroupsError):
        stephen.left_factors(S)


@pytest.mark.quick
def test_Stephen_034():
    """(inverse) step_hen test_schutzenbergergraph 001 (string)"""
    ReportGuard(False)
    to_word = ToWord("abcABC")

    p = InversePresentation(to_word("abcABC"))
    p.inverses(to_word("ABCabc"))

    S = Stephen(p)

    S.set_word(to_word("aBcAbC")).run()

    assert S.finished()
    assert S.word_graph().number_of_nodes() == 7
    assert not stephen.accepts(S, to_word("BaAbaBcAbC"))
    assert stephen.accepts(S, to_word("aBcCbBcAbC"))

    S.set_word(to_word("aBcCbBcAbC"))
    assert stephen.accepts(S, to_word("aBcAbC"))

    S.set_word(to_word("BaAbaBcAbC"))
    assert stephen.accepts(S, to_word("aBcAbC"))


@pytest.mark.quick
def test_Stephen_035():
    """(inverse) step_hen test_schutzenbergergraph 001"""
    ReportGuard(False)
    to_word = ToWord("abcABC")
    p = InversePresentation(to_word("abcABC"))
    p.inverses(to_word("ABCabc"))

    S = Stephen(p)
    assert to_word("aBcAbC") == [0, 4, 2, 3, 1, 5]

    S.set_word(to_word("aBcAbC")).run()

    assert S.finished()
    assert S.word_graph().number_of_nodes() == 7
    assert not stephen.accepts(S, to_word("BaAbaBcAbC"))
    assert stephen.accepts(S, to_word("aBcCbBcAbC"))

    S.set_word(to_word("aBcCbBcAbC"))
    assert stephen.accepts(S, to_word("aBcAbC"))

    S.set_word(to_word("BaAbaBcAbC"))
    assert stephen.accepts(S, to_word("aBcAbC"))


@pytest.mark.quick
def test_Stephen_036():
    """(inverse) step_hen test_schutzenbergergraph 002"""
    ReportGuard(False)
    to_word = ToWord("abcABC")
    p = InversePresentation(to_word("abcABC"))
    p.inverses(to_word("ABCabc"))

    S = Stephen(p)
    S.set_word(to_word("aBbcABAabCc")).run()

    assert S.accept_state() == 4
    assert word_graph.follow_path(S.word_graph(), 0, S.word()) == 4
    assert stephen.number_of_words_accepted(S) == POSITIVE_INFINITY


@pytest.mark.quick
def test_Stephen_037():
    """(inverse) step_hen test_schutzenbergergraph 003"""
    ReportGuard(False)
    to_word = ToWord("xyXY")
    p = InversePresentation(to_word("xyXY"))
    p.inverses(to_word("XYxy"))

    S = Stephen(p)
    S.set_word(to_word("xxxyyy")).run()

    assert stephen.accepts(S, to_word("xxxyyyYYYXXXxxxyyy"))
    S.set_word(to_word("xxxyyyYYYXXXxxxyyy"))
    assert stephen.accepts(S, to_word("xxxyyy"))
    assert not stephen.accepts(S, to_word("xxx"))


@pytest.mark.quick
def test_Stephen_038():
    """(inverse) step_hen test_schutzenbergergraph 004"""
    ReportGuard(False)
    to_word = ToWord("xyXY")
    p = InversePresentation(to_word("xyXY"))
    p.inverses(to_word("XYxy"))
    presentation.add_rule(p, to_word("xyXxyX"), to_word("xyX"))

    S = Stephen(p)
    S.set_word(to_word("xyXyy"))
    ys = ""
    for _ in range(10):
        assert stephen.accepts(S, to_word("x" + ys + "Xyy"))
        ys += "y"

    assert not stephen.accepts(S, to_word("xXyx"))
    assert not stephen.accepts(S, to_word("xXxx"))
    assert not stephen.accepts(S, to_word("xXxy"))
    assert not stephen.accepts(S, to_word("xXxX"))
    assert not stephen.accepts(S, to_word("xXyY"))
    assert stephen.accepts(S, to_word("xyXyy"))
    assert stephen.number_of_words_accepted(S) == POSITIVE_INFINITY
    assert S.word_graph().number_of_nodes() == 4
    assert S.word_graph().number_of_edges() == 8

    assert S.word_graph() == WordGraph(
        4,
        [
            [1, 2, UNDEFINED, UNDEFINED],
            [UNDEFINED, 1, 0, 1],
            [UNDEFINED, 3, UNDEFINED, 0],
            [UNDEFINED, UNDEFINED, UNDEFINED, 2],
        ],
    )


@pytest.mark.quick
def test_Stephen_039():
    """(inverse) step_hen test_schutzenbergergraph 005"""
    ReportGuard(False)
    to_word = ToWord("xyXY")
    p = InversePresentation(to_word("xyXY"))
    p.inverses(to_word("XYxy"))
    presentation.add_rule(p, to_word("xyXxyX"), to_word("xyX"))
    presentation.add_rule(p, to_word("xyxy"), to_word("xy"))

    S = Stephen(p)
    S.set_word(to_word("xyXyy"))
    assert stephen.accepts(S, to_word("y"))
    assert stephen.accepts(S, to_word("xxxxxxxxxxxxx"))
    assert stephen.accepts(S, to_word("xyXxyxyxyxyxyXyy"))
    assert S.word_graph().number_of_nodes() == 1


@pytest.mark.quick
def test_Stephen_040():
    """(inverse) step_hen test_schutzenbergergraph 006"""
    ReportGuard(False)
    to_word = ToWord("abcABC")
    p = InversePresentation(to_word("abcABC"))
    p.inverses(to_word("ABCabc"))
    presentation.add_rule(p, to_word("ac"), to_word("ca"))
    presentation.add_rule(p, to_word("ab"), to_word("ba"))
    presentation.add_rule(p, to_word("bc"), to_word("cb"))

    S = Stephen(p)
    S.set_word(to_word("BaAbaBcAbC"))
    S.run()
    assert S.word_graph().number_of_nodes() == 7
    assert S.word_graph() == WordGraph(
        7,
        [
            [1, UNDEFINED, 2, UNDEFINED, 3, UNDEFINED],
            [UNDEFINED, UNDEFINED, UNDEFINED, 0, 4, UNDEFINED],
            [UNDEFINED, UNDEFINED, UNDEFINED, UNDEFINED, 5, 0],
            [4, 0, 5, UNDEFINED, UNDEFINED, UNDEFINED],
            [UNDEFINED, 1, 6, 3, UNDEFINED, UNDEFINED],
            [6, 2, UNDEFINED, UNDEFINED, UNDEFINED, 3],
            [UNDEFINED, UNDEFINED, UNDEFINED, 5, UNDEFINED, 4],
        ],
    )


@pytest.mark.quick
def test_stephen_041():
    """corner case"""
    ReportGuard(False)
    to_word = ToWord("x")

    p = Presentation([])
    p.contains_empty_word(True)

    p.alphabet(to_word("x"))
    presentation.add_rule(p, to_word("xxxx"), to_word("xx"))

    S = Stephen(p)
    S.set_word([])
    S.run()
    assert S.accept_state() == 0
    assert S.word_graph().number_of_nodes() == 1
    assert not stephen.accepts(S, to_word("x"))


@pytest.mark.quick
def test_stephen_042():
    """empty word"""
    ReportGuard(False)
    p = examples.symmetric_inverse_monoid(4)
    assert p.contains_empty_word()
    assert len(p.alphabet()) == 4

    s = Stephen(p)
    s.set_word([]).run()
    assert s.word_graph().number_of_nodes() == 24
    s.set_word([0]).run()
    assert s.word_graph().number_of_nodes() == 24
    s.set_word([1]).run()
    assert s.word_graph().number_of_nodes() == 24
    s.set_word([2]).run()
    assert s.word_graph().number_of_nodes() == 24
    s.set_word([3]).run()
    assert s.word_graph().number_of_nodes() == 48


@pytest.mark.quick
def test_stephen_043():
    """shared_ptr"""
    ReportGuard(False)
    to_word = ToWord("abcABC")
    p = InversePresentation(to_word("abcABC"))
    p.inverses(to_word("ABCabc"))
    presentation.add_rule(p, to_word("ac"), to_word("ca"))
    presentation.add_rule(p, to_word("ab"), to_word("ba"))
    presentation.add_rule(p, to_word("bc"), to_word("cb"))

    S = Stephen(p)
    S.set_word(to_word("BaAbaBcAbC"))
    S.run()
    assert S.word_graph().number_of_nodes() == 7
    assert S.word_graph() == WordGraph(
        7,
        [
            [1, UNDEFINED, 2, UNDEFINED, 3, UNDEFINED],
            [UNDEFINED, UNDEFINED, UNDEFINED, 0, 4, UNDEFINED],
            [UNDEFINED, UNDEFINED, UNDEFINED, UNDEFINED, 5, 0],
            [4, 0, 5, UNDEFINED, UNDEFINED, UNDEFINED],
            [UNDEFINED, 1, 6, 3, UNDEFINED, UNDEFINED],
            [6, 2, UNDEFINED, UNDEFINED, UNDEFINED, 3],
            [UNDEFINED, UNDEFINED, UNDEFINED, 5, UNDEFINED, 4],
        ],
    )


@pytest.mark.quick
def test_stephen_044():
    """inverse presentation -- operator=="""
    ReportGuard(False)
    tc = ToddCoxeter(word=list[int])

    p = examples.symmetric_inverse_monoid(4)
    assert p.contains_empty_word()
    assert len(p.alphabet()) == 4
    tc.init(congruence_kind.twosided, p)

    p = InversePresentation(examples.symmetric_inverse_monoid(4))
    p.inverses([0, 1, 2, 3])

    S = Stephen(p)
    w = [0, 1, 2, 0, 1, 1, 2, 0, 1, 0, 2, 2, 2, 2, 0, 1]
    S.set_word(w)

    assert stephen.number_of_words_accepted(S) == POSITIVE_INFINITY

    index = tc.index_of(w)
    T = Stephen(p)

    for ww in islice(stephen.words_accepted(S), 1024):
        assert tc.index_of(ww) == index
        assert stephen.accepts(T.set_word(ww), ww)


# TODO(2): add test_case_45 once we fix it


@pytest.mark.quick
def test_stephen_046():
    """non-inverse presentation -- operator=="""
    ReportGuard(False)
    p = examples.symmetric_inverse_monoid(4)

    tc = ToddCoxeter(congruence_kind.twosided, p)

    S = Stephen(p)
    w = [0, 1, 2, 0, 1, 1, 2, 0, 1, 0, 2, 2, 2, 2, 0, 1]
    S.set_word(w)
    assert stephen.number_of_words_accepted(S) == POSITIVE_INFINITY

    index = tc.index_of(w)

    for u in islice(stephen.words_accepted(S), 1024):
        assert tc.index_of(u) == index

    for u in islice(todd_coxeter.class_of(tc, w), 1024):
        assert stephen.accepts(S, u)


@pytest.mark.quick
def test_stephen_032():
    """Plactic monoid"""
    ReportGuard(False)
    p = examples.plactic_monoid(4)
    p.contains_empty_word(True)
    s = Stephen(p)
    s.set_word([0, 0, 1, 3, 1, 2, 2]).run()
    assert not stephen.accepts(s, [0, 0, 1, 3, 2, 1, 2])


# TODO(2): uncomment once CI is modified to skip failing tests
# @pytest.mark.fail
# def test_stephen_033():
#     """Whyte's 4-relation full transf monoid 8"""
#     rg = ReportGuard(True);
#     p = Presentation([])
#     p.rules = [
#         [0, 0],
#         [],
#         [1, 1],
#         [],
#         [2, 2],
#         [],
#         [3, 3],
#         [],
#         [4, 4],
#         [],
#         [5, 5],
#         [],
#         [6, 6],
#         [],
#         [0, 1, 0, 1, 0, 1],
#         [],
#         [1, 2, 1, 2, 1, 2],
#         [],
#         [2, 3, 2, 3, 2, 3],
#         [],
#         [3, 4, 3, 4, 3, 4],
#         [],
#         [4, 5, 4, 5, 4, 5],
#         [],
#         [5, 6, 5, 6, 5, 6],
#         [],
#         [6, 0, 6, 0, 6, 0],
#         [],
#         [0, 1, 0, 2, 0, 1, 0, 2],
#         [],
#         [0, 1, 0, 3, 0, 1, 0, 3],
#         [],
#         [0, 1, 0, 4, 0, 1, 0, 4],
#         [],
#         [0, 1, 0, 5, 0, 1, 0, 5],
#         [],
#         [0, 1, 0, 6, 0, 1, 0, 6],
#         [],
#         [1, 2, 1, 0, 1, 2, 1, 0],
#         [],
#         [1, 2, 1, 3, 1, 2, 1, 3],
#         [],
#         [1, 2, 1, 4, 1, 2, 1, 4],
#         [],
#         [1, 2, 1, 5, 1, 2, 1, 5],
#         [],
#         [1, 2, 1, 6, 1, 2, 1, 6],
#         [],
#         [2, 3, 2, 0, 2, 3, 2, 0],
#         [],
#         [2, 3, 2, 1, 2, 3, 2, 1],
#         [],
#         [2, 3, 2, 4, 2, 3, 2, 4],
#         [],
#         [2, 3, 2, 5, 2, 3, 2, 5],
#         [],
#         [2, 3, 2, 6, 2, 3, 2, 6],
#         [],
#         [3, 4, 3, 0, 3, 4, 3, 0],
#         [],
#         [3, 4, 3, 1, 3, 4, 3, 1],
#         [],
#         [3, 4, 3, 2, 3, 4, 3, 2],
#         [],
#         [3, 4, 3, 5, 3, 4, 3, 5],
#         [],
#         [3, 4, 3, 6, 3, 4, 3, 6],
#         [],
#         [4, 5, 4, 0, 4, 5, 4, 0],
#         [],
#         [4, 5, 4, 1, 4, 5, 4, 1],
#         [],
#         [4, 5, 4, 2, 4, 5, 4, 2],
#         [],
#         [4, 5, 4, 3, 4, 5, 4, 3],
#         [],
#         [4, 5, 4, 6, 4, 5, 4, 6],
#         [],
#         [5, 6, 5, 0, 5, 6, 5, 0],
#         [],
#         [5, 6, 5, 1, 5, 6, 5, 1],
#         [],
#         [5, 6, 5, 2, 5, 6, 5, 2],
#         [],
#         [5, 6, 5, 3, 5, 6, 5, 3],
#         [],
#         [5, 6, 5, 4, 5, 6, 5, 4],
#         [],
#         [6, 0, 6, 1, 6, 0, 6, 1],
#         [],
#         [6, 0, 6, 2, 6, 0, 6, 2],
#         [],
#         [6, 0, 6, 3, 6, 0, 6, 3],
#         [],
#         [6, 0, 6, 4, 6, 0, 6, 4],
#         [],
#         [6, 0, 6, 5, 6, 0, 6, 5],
#         [],
#         [0, 7, 1, 6, 5, 4, 3, 2, 1],
#         [1, 6, 5, 4, 3, 2, 1, 7],
#         [2, 1, 7, 1, 2, 1],
#         [1, 7, 1, 7, 1],
#         [0, 1, 0, 2, 7, 2, 0, 1, 0, 7],
#         [7, 0, 1, 0, 2, 7, 0, 1, 0, 2],
#         [1, 0, 7, 0, 1, 7],
#         [7, 0, 1, 0, 7, 0, 1, 0],
#         [1, 2, 1, 7],
#         [7, 1, 2, 1],
#     ]
#     p.alphabet_from_rules()
#     # TODO(2): uncomment once presentation.balance is implemented and change
#     # pytest.mark.standard without the balancing, it takes too long and hence
#     # is marked fail, with balancing only takes about 5s.
#     # presentation.balance(p, [0,1,2,3,4,5,6], [0,1,2,3,4,5,6]);
#
#     s = Stephen(p)
#     s.set_word([1, 2, 1, 7])
#     s.run()
#
#     assert (
#         word_graph.last_node_on_path(s.word_graph(), 0, [1, 2, 1, 7]).first
#         == word_graph.last_node_on_path(s.word_graph(), 0, [7, 1, 2, 1]).first
#     )
#     assert stephen.accepts(s, [7, 1, 2, 1])


@pytest.mark.quick
def test_stephen_045():
    """Munn tree products"""
    ReportGuard(False)
    to_word = ToWord("abcABC")

    p = InversePresentation(to_word("abcABC"))
    p.inverses(to_word("ABCabc"))

    S = Stephen(p)
    T = Stephen(p)

    S.set_word(to_word("aBbcaABAabCc"))
    T.set_word(to_word("aBbcaABAabCc"))
    S.run()
    assert S.word_graph().number_of_nodes() == 7

    T.run()
    assert T.word_graph().number_of_nodes() == 7
    S *= T
    assert S.word_graph().number_of_nodes() == 14
    assert not S.finished()
    S.run()
    assert S.finished()
    assert S.word_graph().number_of_nodes() == 12
    assert stephen.accepts(S, T.word() * 2)
    T.set_word(T.word() * 2)
    T.run()
    assert S == T
    assert stephen.words_accepted(S).get() == [0, 2, 0, 2]
    assert stephen.number_of_left_factors(S) == POSITIVE_INFINITY
    S.set_word(to_word("aBbcaABAabCc"))
    T.set_word(to_word("CcBAabaACBbA"))
    S *= T

    q = InversePresentation(to_word("abcABC"))
    q.inverses(to_word("ABCabc"))
    presentation.add_rule(q, to_word("ab"), to_word("ba"))
    T.init(q)
    T.set_word(to_word("CcBAabaACBbA"))
    T.run()
    with pytest.raises(LibsemigroupsError):
        S *= T


@pytest.mark.quick
def test_stephen_048():
    """chinese monoid"""
    ReportGuard(False)
    p = examples.chinese_monoid(3)

    S = Stephen(p)
    S.set_word([0, 1, 1, 0]).run()
    assert S.word_graph().number_of_nodes() == 8
    assert list(stephen.words_accepted(S)) == [
        [0, 1, 0, 1],
        [0, 1, 1, 0],
        [1, 0, 0, 1],
    ]


@pytest.mark.quick
def test_stephen_049():
    """to_human_readable_repr"""
    ReportGuard(False)
    p = Presentation([0, 1])
    p.contains_empty_word(True)
    presentation.add_rule(p, [0, 0, 0], [1, 1])
    presentation.add_rule(p, [0, 0, 1], [1, 0])

    S = Stephen(Presentation([]))
    assert (
        repr(S)
        == f"<Stephen object over {repr(S.presentation())} with no word set>"
    )
    S.init(p)
    assert (
        repr(S)
        == f"<Stephen object over {repr(S.presentation())} with no word set>"
    )
    S.set_word([0, 1, 1, 0])
    assert (
        repr(S)
        == f"<Stephen object over {repr(S.presentation())} for word [0, 1, 1, 0] "
        "with 5 nodes and 4 edges>"
    )
    S.run()
    assert (
        repr(S)
        == f"<Stephen object over {repr(S.presentation())} for word [0, 1, 1, 0] "
        "with 13 nodes and 26 edges>"
    )
    S.set_word([0, 1, 1, 0, 0, 1, 1, 0, 0])
    assert (
        repr(S)
        == f"<Stephen object over {repr(S.presentation())} for word [0, 1, 1, 0, 0, 1, 1, 0, 0] "
        "with 10 nodes and 9 edges>"
    )
    S.run()
    assert (
        repr(S)
        == f"<Stephen object over {repr(S.presentation())} for word [0, 1, 1, 0, 0, 1, 1, 0, 0] "
        "with 13 nodes and 26 edges>"
    )
    S.set_word([0, 1, 1, 0, 0, 1, 1, 0, 0, 1])
    S.run()
    assert (
        repr(S)
        == f"<Stephen object over {repr(S.presentation())} for 10 letter word "
        "with 13 nodes and 26 edges>"
    )
    S.set_word([0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0])
    S.run()
    assert (
        repr(S)
        == f"<Stephen object over {repr(S.presentation())} for 11 letter word "
        "with 13 nodes and 26 edges>"
    )
    S.init(p)
    assert (
        repr(S)
        == f"<Stephen object over {repr(S.presentation())} with no word set>"
    )

    to_word = ToWord("abcABC")
    pi = InversePresentation(to_word("abcABC"))
    pi.inverses(to_word("ABCabc"))
    presentation.add_rule(pi, to_word("ac"), to_word("ca"))
    presentation.add_rule(pi, to_word("ab"), to_word("ba"))
    presentation.add_rule(pi, to_word("bc"), to_word("cb"))

    IS = Stephen(InversePresentation([]))
    assert (
        repr(IS)
        == f"<Stephen object over {repr(IS.presentation())} with no word set>"
    )
    IS.init(pi)
    assert (
        repr(IS)
        == f"<Stephen object over {repr(IS.presentation())} with no word set>"
    )
    IS.set_word(to_word("BaAbaBcAb"))
    assert (
        repr(IS)
        == f"<Stephen object over {repr(IS.presentation())} for word [4, 0, 3, 1, 0, 4, 2, 3, 1] "
        "with 8 nodes and 14 edges>"
    )
    IS.run()
    assert (
        repr(IS)
        == f"<Stephen object over {repr(IS.presentation())} for word [4, 0, 3, 1, 0, 4, 2, 3, 1] "
        "with 7 nodes and 18 edges>"
    )


@pytest.mark.quick
def test_stephen_051():
    """Incomplete Munn tree products"""
    ReportGuard(False)
    to_word = ToWord("abcABC")

    p = InversePresentation(to_word("abcABC"))
    p.inverses(to_word("ABCabc"))

    S = Stephen(p)
    Si = Stephen(p)
    T = Stephen(p)
    Ti = Stephen(p)

    S.set_word(to_word("aBbcaABAabCc")).run()
    T.set_word(to_word("aBbcaABAabCc")).run()
    S *= T
    S.run()

    Si.set_word(to_word("aBbcaABAabCc"))
    Si *= T
    Si.run()
    assert Si == S

    Si.set_word(to_word("aBbcaABAabCc"))
    Ti.set_word(to_word("aBbcaABAabCc"))
    Si.run()
    Si *= Ti
    Si.run()
    assert Si == S


@pytest.mark.quick
def test_stephen_return_policy():
    ReportGuard(False)
    to_word = ToWord("abcABC")

    p = InversePresentation(to_word("abcABC"))
    p.inverses(to_word("ABCabc"))

    S = Stephen(p)

    assert S.copy() is not S
    assert S.init(p) is S
    assert S.set_word([0, 1]) is S
    assert S.word_graph() is S.word_graph()
