# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell + Maria Tsalakou
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for KnuthBendix.
"""

# pylint: disable=missing-function-docstring

from datetime import timedelta
import pytest

from _libsemigroups_pybind11 import Runner  # pylint: disable=no-name-in-module

from libsemigroups_pybind11 import (
    KnuthBendix,
    LIMIT_MAX,
    LibsemigroupsError,
    POSITIVE_INFINITY,
    Presentation,
    ReportGuard,
    StringRange,
    congruence_kind,
    is_obviously_infinite,
    knuth_bendix,
    presentation,
)


from .runner import check_runner
from .cong_common import check_congruence_common_return_policy


def check_initialisation(*args):
    for rewriter in ["RewriteFromLeft", "RewriteTrie"]:
        kb = KnuthBendix(*args, rewriter=rewriter)
        kb.run()


def test_initialisation():
    ReportGuard(False)
    kinds = [congruence_kind.twosided, congruence_kind.onesided]

    p = Presentation("ba")
    presentation.add_rule(p, "ba", "ab")
    presentation.add_rule(p, "aa", "a")
    presentation.add_rule(p, "bb", "b")
    for kind in kinds:
        check_initialisation(kind, p)

    p = Presentation([0, 1])
    presentation.add_rule(p, [0, 1], [1, 0])
    presentation.add_rule(p, [0, 0], [0])
    presentation.add_rule(p, [1, 1], [1])
    for kind in kinds:
        check_initialisation(kind, p)

    for kind in kinds:
        kb = KnuthBendix(kind, p)
        kb2 = kb.copy()
        kb2.run()

        with pytest.raises(TypeError):
            KnuthBendix(kb, rewriter="RewriteFromLeft")

        kb = KnuthBendix(kind, p, rewriter="RewriteFromLeft")


def test_attributes():
    ReportGuard(False)
    p = Presentation("abBe")
    presentation.add_identity_rules(p, "e")
    presentation.add_inverse_rules(p, "aBbe", "e")
    presentation.add_rule(p, "bb", "B")
    presentation.add_rule(p, "BaBa", "abab")

    kb = KnuthBendix(congruence_kind.twosided, p)

    assert sorted(list(kb.active_rules())) == sorted(
        [
            ("ae", "a"),
            ("ea", "a"),
            ("be", "b"),
            ("eb", "b"),
            ("Be", "B"),
            ("eB", "B"),
            ("ee", "e"),
            ("aa", "e"),
            ("bB", "e"),
            ("Bb", "e"),
            ("bb", "B"),
            ("BaBa", "abab"),
        ]
    )
    assert kb.max_pending_rules() == 128
    assert kb.check_confluence_interval() == 4096
    assert kb.max_overlap() == POSITIVE_INFINITY
    assert kb.max_rules() == POSITIVE_INFINITY
    assert kb.overlap_policy() == kb.options.overlap.ABC
    assert kb.presentation().alphabet() == "abBe"
    assert kb.number_of_active_rules() == 12
    assert kb.number_of_inactive_rules() == 0
    assert kb.total_rules() == 12


def test_operators():
    ReportGuard(False)
    p = Presentation("abBe")
    presentation.add_identity_rules(p, "e")
    presentation.add_inverse_rules(p, "aBbe", "e")
    presentation.add_rule(p, "bb", "B")
    presentation.add_rule(p, "BaBa", "abab")

    kb = KnuthBendix(congruence_kind.twosided, p)
    assert kb.contains("bb", "B")
    # REVIEW Should this be allowed
    # assert kb.contains([1, 1], [2])

    with pytest.raises(LibsemigroupsError):
        kb.contains("aa", "z")

    assert kb.reduce_no_run("bb") == "B"
    assert kb.reduce_no_run("B") == "B"

    with pytest.raises(LibsemigroupsError):
        kb.reduce_no_run("z")

    assert kb.reduce_no_run("aa") == "e"
    assert kb.reduce_no_run("bb") == "B"

    with pytest.raises(LibsemigroupsError):
        kb.reduce_no_run("z")


def test_running_state():
    ReportGuard(False)

    p = Presentation("abce")
    presentation.add_identity_rules(p, "e")
    presentation.add_rule(p, "aa", "e")
    presentation.add_rule(p, "bc", "e")
    presentation.add_rule(p, "bbb", "e")
    presentation.add_rule(p, "ababababababab", "e")
    presentation.add_rule(p, "abacabacabacabacabacabacabacabac", "e")

    kb = KnuthBendix(congruence_kind.twosided, p)
    check_runner(kb, timedelta(microseconds=1000))


# Tests from libsemigroups
def test_001():
    p = Presentation("ab")
    p.contains_empty_word(True)
    presentation.add_rule(p, "aa", "")
    presentation.add_rule(p, "bbb", "")
    presentation.add_rule(p, "ababab", "")

    kb = KnuthBendix(congruence_kind.twosided, p)
    assert not kb.confluent()
    kb.run()
    assert kb.confluent()


def test_002():
    p = Presentation("012")
    presentation.add_rule(p, "000", "2")
    presentation.add_rule(p, "111", "2")
    presentation.add_rule(p, "010101", "2")
    presentation.add_identity_rules(p, "2")

    kb = KnuthBendix(congruence_kind.twosided, p)

    assert kb.presentation().alphabet() == "012"
    assert not kb.confluent()
    kb.run()
    assert kb.confluent()
    assert kb.number_of_active_rules() == 9
    ad = kb.gilman_graph()
    assert ad.number_of_nodes() == 9
    assert ad.number_of_edges() == 13


def test_003():
    p = Presentation("aAbBcC")
    p.contains_empty_word(True)
    presentation.add_inverse_rules(p, "AaBbCc")
    presentation.add_rule(p, "Aba", "bb")
    presentation.add_rule(p, "Bcb", "cc")
    presentation.add_rule(p, "Cac", "aa")

    kb = KnuthBendix(congruence_kind.twosided, p)
    assert not kb.confluent()
    kb.run()
    assert kb.confluent()
    assert kb.contains("Aba", "bb")
    assert kb.contains("Bcb", "cc")
    assert kb.contains("Cac", "aa")


def test_003_b():
    p = Presentation("aAbBcCe")
    presentation.add_identity_rules(p, "e")
    presentation.add_inverse_rules(p, "AaBbCce", "e")
    presentation.add_rule(p, "Aba", "bb")
    presentation.add_rule(p, "Bcb", "cc")
    presentation.add_rule(p, "Cac", "aa")

    kb = KnuthBendix(congruence_kind.twosided, p)
    assert not kb.confluent()
    kb.run()
    assert kb.confluent()
    assert kb.contains("Aba", "bb")
    assert kb.contains("Bcb", "cc")
    assert kb.contains("Cac", "aa")


def test_004():
    p = Presentation("abAB")
    p.contains_empty_word(True)
    presentation.add_inverse_rules(p, "ABab")
    presentation.add_rule(p, "aaa", "")
    presentation.add_rule(p, "bbb", "")
    presentation.add_rule(p, "abababab", "")
    presentation.add_rule(p, "aBaBaBaBaB", "")

    kb = KnuthBendix(congruence_kind.twosided, p)
    assert not kb.confluent()
    kb.run()
    assert kb.confluent()
    assert kb.number_of_active_rules() == 183
    assert kb.contains("aaa", "")
    assert kb.contains("bbb", "")
    assert kb.contains("BaBaBaBaB", "aa")
    assert kb.contains("bababa", "aabb")
    assert kb.contains("ababab", "bbaa")
    assert kb.contains("aabbaa", "babab")
    assert kb.contains("bbaabb", "ababa")
    assert kb.contains("bababbabab", "aabbabbaa")
    assert kb.contains("ababaababa", "bbaabaabb")
    assert kb.contains("bababbabaababa", "aabbabbaabaabb")
    assert kb.contains("bbaabaabbabbaa", "ababaababbabab")


def test_004_b():
    p = Presentation("abABe")
    p.contains_empty_word(True)
    presentation.add_identity_rules(p, "e")
    presentation.add_inverse_rules(p, "ABabe", "e")
    presentation.add_rule(p, "aaa", "")
    presentation.add_rule(p, "bbb", "")
    presentation.add_rule(p, "abababab", "")
    presentation.add_rule(p, "aBaBaBaBaB", "")

    kb = KnuthBendix(congruence_kind.twosided, p)
    assert not kb.confluent()
    kb.run()
    assert kb.confluent()
    assert kb.number_of_active_rules() == 184  # Was 183. 1 more for ee=e
    assert kb.contains("aaa", "")
    assert kb.contains("bbb", "")
    assert kb.contains("BaBaBaBaB", "aa")
    assert kb.contains("bababa", "aabb")
    assert kb.contains("ababab", "bbaa")
    assert kb.contains("aabbaa", "babab")
    assert kb.contains("bbaabb", "ababa")
    assert kb.contains("bababbabab", "aabbabbaa")
    assert kb.contains("ababaababa", "bbaabaabb")
    assert kb.contains("bababbabaababa", "aabbabbaabaabb")
    assert kb.contains("bbaabaabbabbaa", "ababaababbabab")


def test_005():
    p = Presentation("aAbB")
    p.contains_empty_word(True)
    presentation.add_inverse_rules(p, "AaBb")
    kb = KnuthBendix(congruence_kind.twosided, p)
    assert kb.confluent()
    kb.run()
    assert kb.confluent()
    assert is_obviously_infinite(kb)


def test_006():
    p = Presentation("abB")
    presentation.add_rule(p, "bb", "B")
    presentation.add_rule(p, "BaB", "aba")

    kb = KnuthBendix(congruence_kind.twosided, p)
    assert not kb.confluent()
    kb.run()
    assert kb.confluent()
    assert kb.number_of_active_rules() == 6
    assert sorted(list(kb.active_rules())) == sorted(
        [
            ("Bb", "bB"),
            ("bb", "B"),
            ("BaB", "aba"),
            ("BabB", "abab"),
            ("Baaba", "abaaB"),
            ("Bababa", "ababaB"),
        ]
    )


def test_non_trivial_classes():
    p = Presentation("ab")
    presentation.add_rule(p, "ba", "ababbb")
    presentation.add_rule(p, "abab", "a" * 6)
    kb = KnuthBendix(congruence_kind.twosided, p)
    strings = StringRange().alphabet("ab").first("").last("a" * 6)
    assert strings.count() == 63
    assert knuth_bendix.non_trivial_classes(kb, kb) == []
    assert knuth_bendix.non_trivial_classes(kb, list(strings)) == [
        [
            "aba",
            "baa",
        ],
        [
            "bab",
            "bba",
        ],
        [
            "aaba",
            "abaa",
            "baaa",
        ],
        [
            "abab",
            "abba",
            "baab",
            "baba",
            "bbaa",
        ],
        [
            "babb",
            "bbab",
            "bbba",
        ],
        [
            "aaaba",
            "aabaa",
            "abaaa",
            "baaaa",
        ],
        [
            "aabab",
            "aabba",
            "abaab",
            "ababa",
            "abbaa",
            "baaab",
            "baaba",
            "babaa",
            "bbaaa",
        ],
        [
            "ababb",
            "abbab",
            "abbba",
            "baabb",
            "babab",
            "babba",
            "bbaab",
            "bbaba",
            "bbbaa",
        ],
        [
            "babbb",
            "bbabb",
            "bbbab",
            "bbbba",
        ],
    ]


def test_knuth_bendix_return_policy():
    kb = check_congruence_common_return_policy(KnuthBendix)
    assert kb.max_pending_rules(10) is kb.max_pending_rules(10)
    assert kb.check_confluence_interval(
        LIMIT_MAX
    ) is kb.check_confluence_interval(LIMIT_MAX)
    assert kb.max_overlap(POSITIVE_INFINITY) is kb.max_overlap(
        POSITIVE_INFINITY
    )
    assert kb.max_rules(POSITIVE_INFINITY) is kb.max_rules(POSITIVE_INFINITY)
    assert kb.overlap_policy() is not kb.overlap_policy()
    assert kb.overlap_policy(kb.options.overlap.ABC) is kb.overlap_policy(
        kb.options.overlap.ABC
    )
    assert kb.gilman_graph() is kb.gilman_graph()
    assert kb.gilman_graph_node_labels() is not kb.gilman_graph_node_labels()


def test_knuth_bendix_runner_state():
    p = Presentation([0, 1])
    kb = KnuthBendix(congruence_kind.twosided, p)
    assert isinstance(kb.state(0), Runner.state)


# TODO(0) Does the alphabet bug persist? YES: the test fails
# def test_alphabet_bug():
#     p = Presentation("".join(chr(i) for i in range(-126, 128)))
#     p.throw_if_bad_alphabet_or_rules()
#     presentation.add_rule(p, str(p.alphabet()[-1]), "a")
#     p.throw_if_bad_alphabet_or_rules()
#
#     expected = "\x7f"
#
#     assert str(p.alphabet()[-1]) == expected
#     assert p.rules == [
#         expected,
#         "a",
#     ]
#
#     k = KnuthBendix(congruence_kind.twosided, p)
#     assert k.presentation().alphabet() == p.alphabet()
#     assert k.presentation().rules == p.rules
#     assert list(k.active_rules()) == [(expected, "a")]
