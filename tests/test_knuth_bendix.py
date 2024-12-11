# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell + Maria Tsalakou
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for KnuthBendix.
"""

# pylint: disable=no-name-in-module, missing-function-docstring, invalid-name

from datetime import timedelta
import pytest
from runner import check_runner
from libsemigroups_pybind11 import (
    KnuthBendix,
    congruence_kind,
    ReportGuard,
    Presentation,
    presentation,
    LibsemigroupsError,
    POSITIVE_INFINITY,
    overlap,
    is_obviously_infinite,
)

# TODO should this be for presentation?
# def test_validation_other():
#     ReportGuard(False)
#     p = Presentation("abc")
#     kb = KnuthBendix(congruence_kind.twosided, p)

#     with pytest.raises(LibsemigroupsError):
#         kb.validate_letter("c")
#     kb.validate_letter("a")

#     with pytest.raises(LibsemigroupsError):
#         kb.validate_letter(3)
#     kb.validate_letter(0)

#     with pytest.raises(LibsemigroupsError):
#         kb.validate_word("abc")
#     kb.validate_word("abab")

#     with pytest.raises(LibsemigroupsError):
#         kb.validate_word([0, 1, 2])
#     kb.validate_word([0, 1, 0, 1])


# def test_validation():
#     check_validation(KnuthBendix)


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
    assert isinstance(kb.overlap_policy(), overlap)
    assert kb.overlap_policy() == overlap.ABC
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


# TODO Decided what to do with this. Does the alphabet bug persist? Either way, this seems like a
# test for presentation
# def test_alphabet_bug():
#     k = KnuthBendix()
#     k.set_alphabet(128)
#     assert list(k.alphabet()) == [
#         "\x01",
#         "\x02",
#         "\x03",
#         "\x04",
#         "\x05",
#         "\x06",
#         "\x07",
#         "\x08",
#         "\t",
#         "\n",
#         "\x0b",
#         "\x0c",
#         "\r",
#         "\x0e",
#         "\x0f",
#         "\x10",
#         "\x11",
#         "\x12",
#         "\x13",
#         "\x14",
#         "\x15",
#         "\x16",
#         "\x17",
#         "\x18",
#         "\x19",
#         "\x1a",
#         "\x1b",
#         "\x1c",
#         "\x1d",
#         "\x1e",
#         "\x1f",
#         " ",
#         "!",
#         '"',
#         "#",
#         "$",
#         "%",
#         "&",
#         "'",
#         "(",
#         ")",
#         "*",
#         "+",
#         ",",
#         "-",
#         ".",
#         "/",
#         "0",
#         "1",
#         "2",
#         "3",
#         "4",
#         "5",
#         "6",
#         "7",
#         "8",
#         "9",
#         ":",
#         ";",
#         "<",
#         "=",
#         ">",
#         "?",
#         "@",
#         "A",
#         "B",
#         "C",
#         "D",
#         "E",
#         "F",
#         "G",
#         "H",
#         "I",
#         "J",
#         "K",
#         "L",
#         "M",
#         "N",
#         "O",
#         "P",
#         "Q",
#         "R",
#         "S",
#         "T",
#         "U",
#         "V",
#         "W",
#         "X",
#         "Y",
#         "Z",
#         "[",
#         "\\",
#         "]",
#         "^",
#         "_",
#         "`",
#         "a",
#         "b",
#         "c",
#         "d",
#         "e",
#         "f",
#         "g",
#         "h",
#         "i",
#         "j",
#         "k",
#         "l",
#         "m",
#         "n",
#         "o",
#         "p",
#         "q",
#         "r",
#         "s",
#         "t",
#         "u",
#         "v",
#         "w",
#         "x",
#         "y",
#         "z",
#         "{",
#         "|",
#         "}",
#         "~",
#         "\x7f",
#         "\x80",
#     ]
#     k.add_rule(k.alphabet(127), "a")
#     kk = KnuthBendix()
#     kk.set_alphabet(k.alphabet())
#     assert kk.alphabet() == k.alphabet()
#     assert k.active_rules() == [("a", "\x80")]
#     k.set_identity(k.alphabet(127))
#     kk.set_identity(127)
#     assert k.identity() == "\x80"
#     assert kk.identity() == "\x80"
#     k.set_inverses(k.alphabet())
#     assert k.inverses() == k.alphabet()
#     assert k.contains("\x80", "a")
#     assert k.reduce_no_run("\x80") == "\x80"
#     assert k.reduce_no_run("a") == "\x80"
#     k.validate_letter("\x80")
#     k.validate_word("\x80")
#     assert k.char_to_uint("\x80") == 127
#     assert k.uint_to_char(127) == "\x80"
#     assert k.string_to_word("\x80") == [127]
#     assert k.word_to_string([127]) == "\x80"
