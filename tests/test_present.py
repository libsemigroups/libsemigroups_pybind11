# -*- coding: utf-8 -*-
# Copyright (c) 2022, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for the Presentation class.
"""

# pylint: disable=fixme, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name


from datetime import timedelta
from typing import List, Union

import pytest
from libsemigroups_pybind11 import Presentation, ReportGuard, presentation

###############################################################################
# Helper functions
###############################################################################


def to_string(w: Union[List[int], int]) -> str:
    if isinstance(w, int):
        return str(w)
    return "".join(str(x) for x in w)


def to_word(w: List[int]) -> List[int]:
    return w


###############################################################################
# The functions with prefix check_* are used to test the code for Presentations
# of strings and lists of ints without duplicating code.
###############################################################################


def check_constructors(p):
    p.validate()
    pp = Presentation(p)
    pp.validate()
    assert pp.alphabet() == p.alphabet()
    assert pp.rules == p.rules


def check_alphabet_letters(W):
    if W is to_word:
        p = Presentation([0, 1, 2])
    elif W is to_string:
        p = Presentation("012")
    else:
        raise RuntimeError("the parameter W must be to_word or to_string")
    with pytest.raises(RuntimeError):
        p.alphabet(W([0, 1, 1]))
    assert int(p.letter(0)) == 0
    assert int(p.letter(1)) == 1
    assert int(p.letter(2)) == 2
    p.alphabet(4)
    p.validate()

    presentation.add_rule(p, W([0, 1, 2, 1]), W([0, 0]))
    presentation.add_rule(p, W([4, 1]), W([0, 5]))
    presentation.add_rule(p, W([4, 1]), W([0, 1, 1, 1, 1, 1, 1, 1, 1, 1]))
    p.alphabet_from_rules()
    assert [int(x) for x in p.alphabet()] == [0, 1, 2, 4, 5]
    assert int(p.index(W(0))) == 0
    assert int(p.index(W(1))) == 1
    assert int(p.index(W(2))) == 2
    assert int(p.index(W(4))) == 3
    assert int(p.index(W(5))) == 4


def check_contains_empty_word(W):
    p = Presentation(W([]))
    assert not p.contains_empty_word()
    p.contains_empty_word(True)
    assert p.contains_empty_word()
    p.contains_empty_word(False)
    assert not p.contains_empty_word()


def check_validate_rules_throws(W):
    p = Presentation(W([]))
    p.rules = [W([])]
    with pytest.raises(RuntimeError):
        p.validate_rules()


def check_add_rules(W):
    p = Presentation(W([]))
    presentation.add_rule(p, W([0, 1, 2, 1]), W([0, 0]))
    q = Presentation(W([]))
    presentation.add_rule(q, W([4, 1]), W([0, 5]))
    presentation.add_rule(q, W([4, 1]), W([0, 1, 1, 1, 1, 1, 1, 1, 1, 1]))
    presentation.add_rules(p, q)
    assert p.rules == [
        W([0, 1, 2, 1]),
        W([0, 0]),
        W([4, 1]),
        W([0, 5]),
        W([4, 1]),
        W([0, 1, 1, 1, 1, 1, 1, 1, 1, 1]),
    ]
    assert q.rules == [
        W([4, 1]),
        W([0, 5]),
        W([4, 1]),
        W([0, 1, 1, 1, 1, 1, 1, 1, 1, 1]),
    ]
    with pytest.raises(RuntimeError):
        p.validate()
    with pytest.raises(RuntimeError):
        q.validate()


def check_add_identity_rules(W):
    p = Presentation(W([]))
    presentation.add_rule(p, [0, 1, 2, 1], [0, 0])
    with pytest.raises(RuntimeError):
        presentation.add_identity_rules(p, 0)
    p.alphabet_from_rules()
    presentation.add_identity_rules(p, 0)
    assert p.rules == [
        [0, 1, 2, 1],
        [0, 0],
        [0, 0],
        [0],
        [1, 0],
        [1],
        [0, 1],
        [1],
        [2, 0],
        [2],
        [0, 2],
        [2],
    ]


def check_add_inverse_rules(W):
    p = Presentation(W([]))
    presentation.add_rule(p, W([0, 1, 2, 1]), W([0, 0]))
    p.alphabet_from_rules()

    with pytest.raises(RuntimeError):
        presentation.add_inverse_rules(p, W([0, 1, 1]), W(0))
    with pytest.raises(RuntimeError):
        presentation.add_inverse_rules(p, W([1, 2, 0]), W(0))
    p.alphabet(W([0, 1, 2, 3]))
    with pytest.raises(RuntimeError):
        presentation.add_inverse_rules(p, W([0, 2, 3, 1]), W(0))
    with pytest.raises(RuntimeError):
        presentation.add_inverse_rules(p, W([0, 2, 1]), W(0))
    p.alphabet(W([0, 1, 2]))
    presentation.add_inverse_rules(p, W([0, 2, 1]), W(0))

    assert p.rules == [
        W([0, 1, 2, 1]),
        W([0, 0]),
        W([1, 2]),
        W([0]),
        W([2, 1]),
        W([0]),
    ]


def check_remove_duplicate_rules(W):
    p = Presentation(W([]))
    presentation.add_rule(p, W([0, 1, 2, 1]), W([0, 0]))
    presentation.add_rule(p, W([0, 0]), W([0, 1, 2, 1]))
    p.alphabet_from_rules()
    assert len(p.rules) == 4
    presentation.remove_duplicate_rules(p)
    assert len(p.rules) == 2


def check_sort_each_rule(W):
    p = Presentation(W([]))
    presentation.add_rule(p, W([0, 1, 2, 1]), W([1, 2, 1]))
    presentation.add_rule(p, W([1, 2, 1]), W([1, 1, 2, 1]))
    presentation.add_rule(p, W([1, 1, 2, 1]), W([1, 1]))
    presentation.add_rule(p, W([1, 1]), W([1, 2, 1]))
    presentation.add_rule(p, W([1, 2, 1]), W([0]))
    p.alphabet_from_rules()
    presentation.sort_each_rule(p)
    assert p.rules == [
        W([0, 1, 2, 1]),
        W([1, 2, 1]),
        W([1, 1, 2, 1]),
        W([1, 2, 1]),
        W([1, 1, 2, 1]),
        W([1, 1]),
        W([1, 2, 1]),
        W([1, 1]),
        W([1, 2, 1]),
        W([0]),
    ]


def check_sort_rules(W):
    p = Presentation(W([]))
    presentation.add_rule(p, W([0, 1, 2, 1]), W([1, 2, 1]))
    presentation.add_rule(p, W([1, 2, 1]), W([1, 1, 2, 1]))
    presentation.add_rule(p, W([1, 1, 2, 1]), W([1, 1]))
    presentation.add_rule(p, W([1, 1]), W([1, 2, 1]))
    presentation.add_rule(p, W([1, 2, 1]), W([0]))
    p.alphabet_from_rules()
    presentation.sort_rules(p)
    assert p.rules == [
        W([1, 2, 1]),
        W([0]),
        W([1, 1]),
        W([1, 2, 1]),
        W([1, 1, 2, 1]),
        W([1, 1]),
        W([0, 1, 2, 1]),
        W([1, 2, 1]),
        W([1, 2, 1]),
        W([1, 1, 2, 1]),
    ]


def check_longest_common_subword(W):
    p = Presentation(W([]))
    presentation.add_rule(p, W([0, 1, 2, 1]), W([1, 2, 1]))
    presentation.add_rule(p, W([1, 2, 1]), W([1, 1, 2, 1]))
    presentation.add_rule(p, W([1, 1, 2, 1]), W([1, 1]))
    presentation.add_rule(p, W([1, 1]), W([1, 2, 1]))
    presentation.add_rule(p, W([1, 2, 1]), W([0]))
    p.alphabet_from_rules()
    assert presentation.longest_common_subword(p) == W([1, 2, 1])
    presentation.replace_subword(p, W([1, 2, 1]))
    assert p.rules == [
        W([0, p.letter(3)]),
        W([p.letter(3)]),
        W([p.letter(3)]),
        W([1, p.letter(3)]),
        W([1, p.letter(3)]),
        W([1, 1]),
        W([1, 1]),
        W([p.letter(3)]),
        W([p.letter(3)]),
        W([0]),
        W([p.letter(3)]),
        W([1, 2, 1]),
    ]
    presentation.replace_subword(p, W([1, 2, 1]), W([1]))
    assert p.rules == [
        W([0, p.letter(3)]),
        W([p.letter(3)]),
        W([p.letter(3)]),
        W([1, p.letter(3)]),
        W([1, p.letter(3)]),
        W([1, 1]),
        W([1, 1]),
        W([p.letter(3)]),
        W([p.letter(3)]),
        W([0]),
        W([p.letter(3)]),
        W([1]),
    ]


def check_redundant_rule(W):
    ReportGuard(False)
    p = Presentation(W([0, 1, 2, 3]))
    p.rules = [
        W([0, 0]),
        W([0]),
        W([0, 1]),
        W([1]),
        W([0, 2]),
        W([2]),
        W([0, 3]),
        W([3]),
        W([1, 0]),
        W([1]),
        W([2, 0]),
        W([2]),
        W([2, 2]),
        W([0]),
        W([2, 3]),
        W([3]),
        W([3, 0]),
        W([3]),
        W([3, 2]),
        W([3]),
        W([3, 3]),
        W([3]),
        W([1, 1, 1, 1]),
        W([0]),
        W([2, 1, 1, 1]),
        W([1, 2, 1, 2]),
        W([2, 1, 2, 1]),
        W([1, 1, 1, 2]),
        W([1, 1, 3, 1, 3]),
        W([2, 1, 1, 3]),
        W([2, 1, 3, 1, 3]),
        W([1, 1, 2, 1, 3]),
        W([3, 1, 1, 1, 2]),
        W([3, 1, 2, 1]),
        W([3, 1, 1, 1, 3]),
        W([3, 1, 3]),
        W([3, 1, 2, 1, 2]),
        W([3, 1, 1, 1]),
        W([3, 1, 2, 1, 3]),
        W([3, 1, 3]),
        W([3, 1, 3, 1, 1]),
        W([3, 1, 1, 2]),
        W([3, 1, 3, 1, 2]),
        W([3, 1, 2, 1, 1]),
        W([3, 1, 3, 1, 3]),
        W([3]),
        W([1, 1, 2, 1, 1, 3]),
        W([3, 1, 3]),
        W([1, 1, 3, 1, 1, 3]),
        W([3, 1, 1, 3]),
        W([1, 2, 1, 1, 2, 1]),
        W([2, 1, 1, 2]),
        W([2, 1, 1, 2, 1, 1]),
        W([1, 1, 2, 1, 1, 2]),
        W([2, 1, 1, 2, 1, 2]),
        W([1, 1, 1, 2, 1, 1]),
        W([2, 1, 1, 2, 1, 3]),
        W([1, 3, 1, 3]),
        W([2, 1, 1, 3, 1, 1]),
        W([1, 1, 3, 1, 1, 2]),
        W([3, 1, 1, 2, 1, 1]),
        W([3, 1, 3]),
        W([3, 1, 1, 2, 1, 3]),
        W([3]),
        W([3, 1, 1, 3, 1, 1]),
        W([3, 1, 1, 3]),
        W([3, 1, 2, 1, 1, 2]),
        W([3, 1, 3, 1]),
        W([3, 1, 2, 1, 1, 3]),
        W([3]),
        W([1, 1, 1, 2, 1, 1, 2]),
        W([2, 1, 1, 2, 1]),
        W([1, 1, 2, 1, 3, 1, 1]),
        W([2, 1, 3, 1, 1, 2]),
        W([1, 1, 3, 1, 2, 1, 1]),
        W([2, 1, 1, 3, 1, 2]),
        W([1, 2, 1, 3, 1, 1, 3]),
        W([2, 1, 3, 1, 1, 3]),
        W([2, 1, 3, 1, 2, 1, 1]),
        W([1, 1, 2, 1, 3, 1, 2]),
        W([3, 1, 1, 3, 1, 2, 1]),
        W([3, 1, 1, 3, 1, 2]),
        W([1, 1, 3, 1, 1, 2, 1, 2]),
        W([2, 1, 1, 3, 1, 2, 1]),
        W([2, 1, 3, 1, 1, 2, 1, 2]),
        W([1, 1, 2, 1, 3, 1, 2, 1]),
    ]
    assert presentation.length(p) == 359
    presentation.remove_duplicate_rules(p)
    assert presentation.length(p) == 359
    presentation.reduce_complements(p)
    assert presentation.length(p) == 359
    presentation.sort_each_rule(p)
    presentation.sort_rules(p)

    assert presentation.length(p) == 359
    assert len(p.rules) == 86

    p.alphabet_from_rules()
    i = presentation.redundant_rule(p, timedelta(milliseconds=100))
    assert p.rules[i] == W([2, 1, 3, 1, 1, 2, 1, 2])
    assert p.rules[i + 1] == W([1, 1, 2, 1, 3, 1, 2, 1])
    p.rules = p.rules[:i] + p.rules[i + 2 :]
    p.validate()
    assert presentation.length(p) == 343
    assert len(p.rules) == 84


###############################################################################
# Test functions begin
###############################################################################


def test_constructors_000():
    p = Presentation([0, 1, 2])
    presentation.add_rule(p, [0, 0, 0], [0])
    assert len(p.rules) == 2
    presentation.add_rule_and_check(p, [0, 0, 0], [0])
    p.validate()
    check_constructors(p)

    p = Presentation("abc")
    presentation.add_rule(p, "aaa", "a")
    assert len(p.rules) == 2
    presentation.add_rule_and_check(p, "aaa", "a")
    p.validate()
    check_constructors(p)


def test_strings_001():
    p = Presentation("abc")
    assert p.alphabet() == "abc"
    with pytest.raises(RuntimeError):
        p.alphabet("aa")
    assert p.alphabet() == "abc"
    presentation.add_rule(p, "aaa", "a")
    assert len(p.rules) == 2
    assert p.rules == ["aaa", "a"]
    with pytest.raises(RuntimeError):
        presentation.add_rule_and_check(p, "abz", "a")
    with pytest.raises(RuntimeError):
        presentation.add_rule_and_check(p, "", "a")
    assert p.rules == ["aaa", "a"]


def test_constructors_word_type_002():
    p = Presentation([0, 1, 2])
    presentation.add_rule(p, [0, 0, 0], [0])
    assert len(p.rules) == 2
    presentation.add_rule_and_check(p, [0, 0, 0], [0])
    p.validate()
    check_constructors(p)


def test_constructors_std_string_004():
    p = Presentation("abc")
    presentation.add_rule(p, "aaaa", "aa")
    assert len(p.rules) == 2
    presentation.add_rule_and_check(p, "aaa", "aa")
    p.validate()
    check_constructors(p)


def test_alphabet___letters__word_type_005():
    check_alphabet_letters(to_word)
    check_alphabet_letters(to_string)


def test_alphabet__letters__std__string_006():
    p = Presentation("abc")
    assert p.alphabet() == "abc"
    assert p.letter(0) == "a"
    assert p.letter(1) == "b"
    assert p.letter(2) == "c"
    p.alphabet(4)
    assert len(p.alphabet()) == 4
    p.validate()
    with pytest.raises(RuntimeError):
        p.alphabet("abb")

    presentation.add_rule(p, "abca", "aa")
    presentation.add_rule(p, "eb", "af")
    presentation.add_rule(p, "eb", "abbbbbb")
    p.alphabet_from_rules()
    assert p.alphabet() == "abcef"
    assert p.index("a") == 0
    assert p.index("b") == 1
    assert p.index("c") == 2
    assert p.index("e") == 3
    assert p.index("f") == 4


def test_contains_empty_word_007():
    check_contains_empty_word(to_word)
    check_contains_empty_word(to_string)


def test_validate_rules_throws_008():
    check_validate_rules_throws(to_word)
    check_validate_rules_throws(to_string)


def test_helpers_add_rule_s_009():
    check_add_rules(to_word)
    check_add_rules(to_string)


def test_helpers_add_rule_s__std__string__010():
    p = Presentation("")
    presentation.add_rule(p, "abcb", "aa")
    q = Presentation("")
    presentation.add_rule(q, "eb", "af")
    presentation.add_rule(q, "eb", "abbbbbbbbb")
    presentation.add_rules(p, q)
    assert p.rules == ["abcb", "aa", "eb", "af", "eb", "abbbbbbbbb"]
    assert q.rules == ["eb", "af", "eb", "abbbbbbbbb"]
    with pytest.raises(RuntimeError):
        p.validate()
    with pytest.raises(RuntimeError):
        q.validate()


def test_helpers_add_identity_rules_011():
    check_add_identity_rules(to_word)


def test_helpers_add_identity_rules_std_string_012():
    p = Presentation("")
    presentation.add_rule(p, "abcb", "aa")
    with pytest.raises(RuntimeError):
        presentation.add_identity_rules(p, "a")
    p.alphabet_from_rules()
    presentation.add_identity_rules(p, "a")
    assert p.rules == [
        "abcb",
        "aa",
        "aa",
        "a",
        "ba",
        "b",
        "ab",
        "b",
        "ca",
        "c",
        "ac",
        "c",
    ]


def test_helpers_add_inverse_rules_013():
    check_add_inverse_rules(to_word)
    check_add_inverse_rules(to_string)


def test_helpers_add_inverse_rules_std_string_014():
    p = Presentation("")
    p.contains_empty_word(False)

    p.alphabet("aAbBcCe")
    presentation.add_identity_rules(p, "e")

    presentation.add_inverse_rules(p, "AaBbCce", "e")
    presentation.add_rule_and_check(p, "aaCac", "e")
    presentation.add_rule_and_check(p, "acbbACb", "e")
    presentation.add_rule_and_check(p, "ABabccc", "e")
    assert p.rules == [
        "ae",
        "a",
        "ea",
        "a",
        "Ae",
        "A",
        "eA",
        "A",
        "be",
        "b",
        "eb",
        "b",
        "Be",
        "B",
        "eB",
        "B",
        "ce",
        "c",
        "ec",
        "c",
        "Ce",
        "C",
        "eC",
        "C",
        "ee",
        "e",
        "aA",
        "e",
        "Aa",
        "e",
        "bB",
        "e",
        "Bb",
        "e",
        "cC",
        "e",
        "Cc",
        "e",
        "aaCac",
        "e",
        "acbbACb",
        "e",
        "ABabccc",
        "e",
    ]


def test_helpers_remove_duplicate_rules_015():
    check_remove_duplicate_rules(to_word)
    check_remove_duplicate_rules(to_string)


def test_helpers_reduce_complements_016():
    p = Presentation([])
    presentation.add_rule(p, [0, 1, 2, 1], [1, 2, 1])
    presentation.add_rule(p, [1, 2, 1], [1, 1, 2, 1])
    presentation.add_rule(p, [1, 1, 2, 1], [1, 1])
    presentation.add_rule(p, [1, 1], [1, 2, 1])
    presentation.add_rule(p, [1, 2, 1], [0])
    p.alphabet_from_rules()
    presentation.reduce_complements(p)
    presentation.sort_each_rule(p)
    presentation.sort_rules(p)
    assert p.rules == [
        [1, 1],
        [0],
        [1, 2, 1],
        [0],
        [0, 1, 2, 1],
        [0],
        [1, 1, 2, 1],
        [0],
    ]

    p = Presentation("")
    presentation.add_rule(p, "abcb", "bcb")
    presentation.add_rule(p, "bcb", "bbcb")
    presentation.add_rule(p, "bbcb", "bb")
    presentation.add_rule(p, "bb", "bcb")
    presentation.add_rule(p, "bcb", "a")
    p.alphabet_from_rules()
    presentation.reduce_complements(p)
    presentation.sort_each_rule(p)
    presentation.sort_rules(p)
    assert p.rules == ["bb", "a", "bcb", "a", "abcb", "a", "bbcb", "a"]
    assert p.alphabet() == "abc"
    presentation.normalize_alphabet(p)
    assert p.letter(0) == "\x00"
    assert p.letter(1) == "\x01"
    assert p.letter(2) == "\x02"
    p.validate()


def test_helpers_sort_each_rule_017():
    check_sort_each_rule(to_word)
    check_sort_each_rule(to_string)


def test_helpers_sort_rules_018():
    check_sort_rules(to_word)
    check_sort_rules(to_string)


def test_helpers_longest_common_subword_019():
    check_longest_common_subword(to_word)
    check_longest_common_subword(to_string)


def test_redundant_rule_020():
    check_redundant_rule(to_word)
    check_redundant_rule(to_string)


def test_make_021():
    p = Presentation([])
    presentation.add_rule(p, [0, 1, 2, 1], [1, 2, 1])
    presentation.add_rule(p, [1, 2, 1], [1, 1, 2, 1])
    presentation.add_rule(p, [1, 1, 2, 1], [1, 1])
    presentation.add_rule(p, [1, 1], [1, 2, 1])
    presentation.add_rule(p, [1, 2, 1], [0])
    p.alphabet_from_rules()
    p = presentation.make(p, "abc")
    assert p.rules == [
        "abcb",
        "bcb",
        "bcb",
        "bbcb",
        "bbcb",
        "bb",
        "bb",
        "bcb",
        "bcb",
        "a",
    ]

    p = presentation.make(p)
    assert p.rules == [
        [0, 1, 2, 1],
        [1, 2, 1],
        [1, 2, 1],
        [1, 1, 2, 1],
        [1, 1, 2, 1],
        [1, 1],
        [1, 1],
        [1, 2, 1],
        [1, 2, 1],
        [0],
    ]


def test_issue_52_022():
    p = Presentation([])
    presentation.add_rule(p, [0, 1], [])
    p.alphabet_from_rules()
    p.validate()

    assert presentation.redundant_rule(p, timedelta(seconds=1)) == len(p.rules)


def test_helpers_remove_trivial_rules():
    p = Presentation([])
    presentation.add_rule(p, [0], [0])
    presentation.add_rule(p, [1], [1])
    presentation.add_rule(p, [1, 0, 1], [1])
    p.alphabet_from_rules()
    p.validate()

    presentation.remove_trivial_rules(p)

    assert p.rules == [[1, 0, 1], [1]]


def test_helpers_remove_redundant_generators():
    p = Presentation([])
    presentation.add_rule(p, [0], [1])
    presentation.add_rule(p, [1], [1])
    presentation.add_rule(p, [1, 0, 1], [1])
    p.alphabet_from_rules()
    p.validate()

    presentation.remove_redundant_generators(p)
    assert p.rules == [[0, 0, 0], [0]]
    p.rules = p.rules + [[1, 1, 1], [0]]
    presentation.remove_redundant_generators(p)
    assert p.rules == [[1, 1, 1, 1, 1, 1, 1, 1, 1], [1, 1, 1]]