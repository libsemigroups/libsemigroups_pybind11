# -*- coding: utf-8 -*-

# Copyright (c) 2021, J. D. Mitchell + Maria Tsalakou
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for KnuthBendix.
"""

# pylint: disable=no-name-in-module, missing-function-docstring, invalid-name

import pytest

from fpsemi_intf import (
    check_validation,
    check_converters,
    check_initialisation,
    check_attributes,
    check_operators,
    check_running_and_state,
)
from libsemigroups_pybind11 import ReportGuard, KnuthBendix


def test_validation_other():
    ReportGuard(False)
    kb = KnuthBendix()
    kb.set_alphabet("ab")

    with pytest.raises(RuntimeError):
        kb.validate_letter("c")
    kb.validate_letter("a")

    with pytest.raises(RuntimeError):
        kb.validate_letter(3)
    kb.validate_letter(0)

    with pytest.raises(RuntimeError):
        kb.validate_word("abc")
    kb.validate_word("abab")

    with pytest.raises(RuntimeError):
        kb.validate_word([0, 1, 2])
    kb.validate_word([0, 1, 0, 1])


def test_validation():
    check_validation(KnuthBendix)


def test_converters():
    check_converters(KnuthBendix)


def test_initialisation():
    check_initialisation(KnuthBendix)


def test_attributes():
    check_attributes(KnuthBendix)


def test_operators():
    check_operators(KnuthBendix)


# Tests from libsemigroups
def test_006():
    kb = KnuthBendix()
    kb.set_alphabet("ab")
    kb.add_rule("aa", "")
    kb.add_rule("bbb", "")
    kb.add_rule("ababab", "")
    assert not kb.confluent()
    kb.run()
    assert kb.size() == 12
    assert kb.confluent()


def test_009():
    kb = KnuthBendix()
    kb.set_alphabet("012")
    kb.add_rule("000", "2")
    kb.add_rule("111", "2")
    kb.add_rule("010101", "2")
    kb.set_identity("2")
    assert kb.alphabet() == "012"
    assert not kb.confluent()
    kb.run()
    assert kb.confluent()
    assert kb.number_of_active_rules() == 9
    ad = kb.gilman_digraph()
    assert ad.number_of_nodes() == 9
    assert ad.number_of_edges() == 13


def test_022():
    kb = KnuthBendix()
    kb.set_alphabet("aAbBcC")
    kb.set_identity("")
    kb.set_inverses("AaBbCc")
    kb.add_rule("Aba", "bb")
    kb.add_rule("Bcb", "cc")
    kb.add_rule("Cac", "aa")
    assert not kb.confluent()
    kb.run()
    assert kb.confluent()
    assert kb.equal_to("Aba", "bb")
    assert kb.equal_to("Bcb", "cc")
    assert kb.equal_to("Cac", "aa")
    assert kb.size() == 1


def test_024():
    kb = KnuthBendix()
    kb.set_alphabet("abAB")
    kb.set_identity("")
    kb.set_inverses("ABab")
    kb.add_rule("aaa", "")
    kb.add_rule("bbb", "")
    kb.add_rule("abababab", "")
    kb.add_rule("aBaBaBaBaB", "")
    assert not kb.confluent()
    kb.run()
    assert kb.confluent()
    assert kb.number_of_active_rules() == 183
    assert kb.equal_to("aaa", "")
    assert kb.equal_to("bbb", "")
    assert kb.equal_to("BaBaBaBaB", "aa")
    assert kb.equal_to("bababa", "aabb")
    assert kb.equal_to("ababab", "bbaa")
    assert kb.equal_to("aabbaa", "babab")
    assert kb.equal_to("bbaabb", "ababa")
    assert kb.equal_to("bababbabab", "aabbabbaa")
    assert kb.equal_to("ababaababa", "bbaabaabb")
    assert kb.equal_to("bababbabaababa", "aabbabbaabaabb")
    assert kb.equal_to("bbaabaabbabbaa", "ababaababbabab")


def test_025():
    kb = KnuthBendix()
    kb.set_alphabet("aAbB")
    kb.set_identity("")
    kb.set_inverses("AaBb")
    assert kb.confluent()
    kb.run()
    assert kb.confluent()
    assert kb.is_obviously_infinite()


def test_027():
    kb = KnuthBendix()
    kb.set_alphabet("abB")
    kb.add_rule("bb", "B")
    kb.add_rule("BaB", "aba")
    assert not kb.confluent()
    kb.run()
    assert kb.confluent()
    assert kb.number_of_active_rules() == 6
    assert kb.active_rules() == [
        ("Bb", "bB"),
        ("bb", "B"),
        ("BaB", "aba"),
        ("BabB", "abab"),
        ("Baaba", "abaaB"),
        ("Bababa", "ababaB"),
    ]


def test_running_and_state():
    check_running_and_state(KnuthBendix)
