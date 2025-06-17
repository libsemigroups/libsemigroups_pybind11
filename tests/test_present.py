# -*- coding: utf-8 -*-
# Copyright (c) 2022-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for the Presentation class.
"""

# pylint: disable=missing-function-docstring, invalid-name
# pylint: disable=comparison-with-callable, too-many-lines

from typing import Union

import copy
import pytest
from libsemigroups_pybind11 import (
    UNDEFINED,
    Presentation,
    presentation,
    InversePresentation,
    LibsemigroupsError,
    words,
)

###############################################################################
# Helper functions
###############################################################################


def to_string(w: Union[list[int], int]) -> chr:
    if isinstance(w, int):
        return chr(w + 97)
    return "".join(chr(x + 97) for x in w)


def to_word(w: Union[list[int], int]) -> list[int]:
    return w


###############################################################################
# The functions with prefix check_* are used to test the code for Presentations
# of strings and lists of ints without duplicating code.
###############################################################################


def check_constructors(p):
    p.throw_if_bad_alphabet_or_rules()
    pp = copy.copy(p)
    pp.throw_if_bad_alphabet_or_rules()
    assert pp == p
    assert pp is not p
    assert pp.alphabet() == p.alphabet()
    assert pp.rules == p.rules


def check_inverse_constructors(W):
    p = Presentation(W([0, 1, 2]))
    presentation.add_rule(p, W([0, 0, 0]), W([0]))
    presentation.add_rule(p, W([1, 2, 0]), W([2, 1]))
    ip = InversePresentation(p)

    with pytest.raises(LibsemigroupsError):
        ip.throw_if_bad_alphabet_rules_or_inverses()
    assert ip.alphabet() == p.alphabet()
    assert ip.rules == p.rules
    assert ip.inverses() == W([])


def check_inverses(W):
    ip = InversePresentation(W([0, 1, 2]))
    presentation.add_rule(ip, W([0, 0, 0]), W([0]))
    presentation.add_rule(ip, W([1, 2, 0]), W([2, 1]))
    with pytest.raises(LibsemigroupsError):
        ip.throw_if_bad_alphabet_rules_or_inverses()
    with pytest.raises(LibsemigroupsError):
        ip.inverses(W([1, 2, 0]))
    with pytest.raises(LibsemigroupsError):
        ip.inverses(W([0, 0, 0]))
    with pytest.raises(LibsemigroupsError):
        ip.inverses(W([0, 1, 2, 0]))
    with pytest.raises(LibsemigroupsError):
        ip.inverses(W([0, 1, 3]))
    ip.inverses(W([2, 1, 0]))
    assert ip.inverses() == W([2, 1, 0])
    assert ip.inverse(W(0)) == W(2)
    assert ip.inverse(W(1)) == W(1)
    assert ip.inverse(W(2)) == W(0)


def check_alphabet_letters(W):
    if W is to_word:
        p = Presentation([0, 1, 2])
    elif W is to_string:
        p = Presentation("012")
    else:
        raise RuntimeError("the parameter W must be to_word or to_string")
    with pytest.raises(LibsemigroupsError):
        p.alphabet(W([0, 1, 1]))
    assert int(p.letter(0)) == 0
    assert int(p.letter(1)) == 1
    assert int(p.letter(2)) == 2
    if W is to_word:
        assert p.in_alphabet(0)
        assert p.in_alphabet(1)
        assert p.in_alphabet(2)
        assert not p.in_alphabet(3)
    else:
        assert p.in_alphabet("0")
        assert p.in_alphabet("1")
        assert p.in_alphabet("2")
        assert not p.in_alphabet("3")

    p.alphabet(4)
    p.throw_if_bad_alphabet_or_rules()

    presentation.add_rule(p, W([0, 1, 1, 1]), W([0, 0]))
    presentation.add_rule(p, W([3, 1]), W([0, 1, 1, 1, 1, 1, 1, 1, 1, 1]))
    p.alphabet_from_rules()
    assert p.alphabet() == W([0, 1, 3])
    assert int(p.index(W(0))) == 0
    assert int(p.index(W(1))) == 1
    assert int(p.index(W(3))) == 2


def check_contains_empty_word(W):
    p = Presentation(W([]))
    assert not p.contains_empty_word()
    p.contains_empty_word(True)
    assert p.contains_empty_word()
    p.contains_empty_word(False)
    assert not p.contains_empty_word()


def check_throw_if_bad_rules_throws(W):
    p = Presentation(W([]))
    p.rules = [W([])]
    with pytest.raises(LibsemigroupsError):
        p.throw_if_bad_rules()


def check_throw_if_bad_inverses(W):
    p = Presentation(W([0, 1, 2]))
    with pytest.raises(LibsemigroupsError):
        presentation.throw_if_bad_inverses(p, W([0, 1]))
    with pytest.raises(LibsemigroupsError):
        presentation.throw_if_bad_inverses(p, W([0, 0, 1]))
    with pytest.raises(LibsemigroupsError):
        presentation.throw_if_bad_inverses(p, W([1, 2, 0]))
    presentation.throw_if_bad_inverses(p, W([0, 1, 2]))


def check_add_rules(W):
    p = Presentation(W([]))
    p.alphabet(7)
    presentation.add_rule(p, W([0, 1, 2, 1]), W([0, 0]))
    q = Presentation(W([]))
    q.alphabet(7)
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
    assert presentation.contains_rule(p, W([4, 1]), W([0, 5]))
    assert not presentation.contains_rule(p, W([0, 0]), W([4, 1]))
    p.throw_if_bad_alphabet_or_rules()
    q.throw_if_bad_alphabet_or_rules()


def check_add_identity_rules(W):
    p = Presentation(W([0, 1, 2]))
    presentation.add_rule(p, [0, 1, 2, 1], [0, 0])
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
    p = Presentation(W([0, 1, 2]))
    presentation.add_rule(p, W([0, 1, 2, 1]), W([0, 0]))

    with pytest.raises(LibsemigroupsError):
        presentation.add_inverse_rules(p, W([0, 1, 1]), W(0))
    with pytest.raises(LibsemigroupsError):
        presentation.add_inverse_rules(p, W([1, 2, 0]), W(0))
    p.alphabet(W([0, 1, 2, 3]))
    with pytest.raises(LibsemigroupsError):
        presentation.add_inverse_rules(p, W([0, 2, 3, 1]), W(0))
    with pytest.raises(LibsemigroupsError):
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
    p = Presentation(W([0, 1, 2]))
    presentation.add_rule(p, W([0, 1, 2, 1]), W([0, 0]))
    presentation.add_rule(p, W([0, 0]), W([0, 1, 2, 1]))
    p.alphabet_from_rules()
    assert len(p.rules) == 4
    presentation.remove_duplicate_rules(p)
    assert len(p.rules) == 2


def check_sort_each_rule(W):
    p = Presentation(W([0, 1, 2]))
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
    p = Presentation(W([0, 1, 2]))
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


def check_longest_subword_reducing_length(W):
    p = Presentation(W([0, 1, 2, 3]))
    presentation.add_rule(p, W([0, 1, 2, 1]), W([1, 2, 1]))
    presentation.add_rule(p, W([1, 2, 1]), W([1, 1, 2, 1]))
    presentation.add_rule(p, W([1, 1, 2, 1]), W([1, 1]))
    presentation.add_rule(p, W([1, 1]), W([1, 2, 1]))
    presentation.add_rule(p, W([1, 2, 1]), W([0]))
    assert presentation.longest_subword_reducing_length(p) == W([1, 2, 1])
    presentation.replace_subword(p, W([1, 2, 1]), W([3]))
    presentation.add_rule(p, W([3]), W([1, 2, 1]))
    assert p.rules == [
        W([0, 3]),
        W([3]),
        W([3]),
        W([1, 3]),
        W([1, 3]),
        W([1, 1]),
        W([1, 1]),
        W([3]),
        W([3]),
        W([0]),
        W([3]),
        W([1, 2, 1]),
    ]
    presentation.replace_subword(p, W([1, 2, 1]), W([1]))
    assert p.rules == [
        W([0, 3]),
        W([3]),
        W([3]),
        W([1, 3]),
        W([1, 3]),
        W([1, 1]),
        W([1, 1]),
        W([3]),
        W([3]),
        W([0]),
        W([3]),
        W([1]),
    ]


def check_longest_rule(W):
    p = Presentation(W([0, 1, 2]))
    with pytest.raises(LibsemigroupsError):
        assert presentation.longest_rule(p)
    with pytest.raises(LibsemigroupsError):
        assert presentation.shortest_rule(p)
    p.rules = [W([0, 1, 2, 1])]
    with pytest.raises(LibsemigroupsError):
        assert presentation.longest_rule(p)

    p.rules = [W([0, 1, 2, 1]), W([1, 2, 1])]
    presentation.add_rule(p, W([1, 1, 2, 1]), W([1, 1]))
    presentation.add_rule(p, W([1, 2, 1]), W([0]))
    assert p.rules[presentation.longest_rule(p)] == W([0, 1, 2, 1])
    assert p.rules[presentation.shortest_rule(p)] == W([1, 2, 1])


def check_longest_rule_length(W):
    p = Presentation(W([]))
    with pytest.raises(LibsemigroupsError):
        assert presentation.longest_rule_length(p)
    with pytest.raises(LibsemigroupsError):
        assert presentation.shortest_rule_length(p)
    p.rules = [W([0, 1, 2, 1])]
    with pytest.raises(LibsemigroupsError):
        presentation.longest_rule_length(p)
    p.rules = [W([0, 1, 2, 1]), W([1, 2, 1])]
    p.alphabet_from_rules()
    presentation.add_rule(p, W([1, 1, 2, 1]), W([1, 1]))
    presentation.add_rule(p, W([1, 2, 1]), W([0]))
    assert presentation.longest_rule_length(p) == 7
    assert presentation.shortest_rule_length(p) == 4


def check_make_semigroup(W):
    p = Presentation(W([0, 1, 2, 3]))
    p.contains_empty_word(True)
    presentation.add_rule(p, W([0, 0]), W([]))
    presentation.add_rule(p, W([1, 1]), W([]))
    presentation.add_rule(p, W([2, 2]), W([]))
    presentation.add_rule(p, W([0, 1, 0, 1, 0, 1]), W([]))
    presentation.add_rule(p, W([1, 2, 1, 0, 1, 2, 1, 0]), W([]))
    presentation.add_rule(p, W([2, 0, 2, 1, 2, 0, 2, 1]), W([0, 3]))

    p.alphabet_from_rules()
    e = presentation.make_semigroup(p)
    if e == "e":
        e = 4
    print(type(e))
    assert p.rules == [
        W([0, 0]),
        W([e]),
        W([1, 1]),
        W([e]),
        W([2, 2]),
        W([e]),
        W([0, 1, 0, 1, 0, 1]),
        W([e]),
        W([1, 2, 1, 0, 1, 2, 1, 0]),
        W([e]),
        W([2, 0, 2, 1, 2, 0, 2, 1]),
        W([0, 3]),
        W([0, e]),
        W([0]),
        W([e, 0]),
        W([0]),
        W([1, e]),
        W([1]),
        W([e, 1]),
        W([1]),
        W([2, e]),
        W([2]),
        W([e, 2]),
        W([2]),
        W([3, e]),
        W([3]),
        W([e, 3]),
        W([3]),
        W([e, e]),
        W([e]),
    ]
    e = presentation.make_semigroup(p)
    assert e in (UNDEFINED, "\x7f")
    # TODO allow comparison of UNDEFINED and chars (or strings of length 1),
    # and remove the "or" in the previous line


def check_remove_generator(W):
    if W == to_string:
        letter = words.human_readable_letter
    else:

        def letter(x):
            return x

    p = Presentation(W([0, 1, 2, 3, 4, 5, 6, 7, 8, 9]))
    p.remove_generator(letter(0))
    p.remove_generator(letter(4))
    p.remove_generator(letter(7))
    p.remove_generator(letter(9))
    if W == to_string:
        assert p.alphabet() == "bcdfgi"
    else:
        assert p.alphabet() == W([1, 2, 3, 5, 6, 8])

    assert p.index(letter(1)) == 0
    assert p.index(letter(2)) == 1
    assert p.index(letter(3)) == 2
    assert p.index(letter(5)) == 3
    assert p.index(letter(6)) == 4
    assert p.index(letter(8)) == 5
    assert p.letter(0) == letter(1)
    assert p.letter(1) == letter(2)
    assert p.letter(2) == letter(3)
    assert p.letter(3) == letter(5)
    assert p.letter(4) == letter(6)
    assert p.letter(5) == letter(8)

    with pytest.raises(LibsemigroupsError):
        p.remove_generator(letter(11))


###############################################################################
# Test functions begin
###############################################################################


def test_constructors_000():
    p = Presentation([0, 1, 2])
    presentation.add_rule(p, [0, 0, 0], [0])
    assert len(p.rules) == 2
    presentation.add_rule(p, [0, 0, 0], [0])
    p.throw_if_bad_alphabet_or_rules()
    check_constructors(p)

    p = Presentation("abc")
    presentation.add_rule(p, "aaa", "a")
    assert len(p.rules) == 2
    presentation.add_rule(p, "aaa", "a")
    p.throw_if_bad_alphabet_or_rules()
    check_constructors(p)


def test_strings_001():
    p = Presentation("abc")
    assert p.alphabet() == "abc"
    with pytest.raises(LibsemigroupsError):
        p.alphabet("aa")
    assert p.alphabet() == "abc"
    presentation.add_rule(p, "aaa", "a")
    assert len(p.rules) == 2
    assert p.rules == ["aaa", "a"]
    with pytest.raises(LibsemigroupsError):
        presentation.add_rule(p, "abz", "a")
    with pytest.raises(LibsemigroupsError):
        presentation.add_rule(p, "", "a")
    assert p.rules == ["aaa", "a"]


def test_constructors_word_type_002():
    p = Presentation([0, 1, 2])
    presentation.add_rule(p, [0, 0, 0], [0])
    assert len(p.rules) == 2
    presentation.add_rule(p, [0, 0, 0], [0])
    p.throw_if_bad_alphabet_or_rules()
    check_constructors(p)


def test_constructors_std_string_003():
    p = Presentation("abc")
    presentation.add_rule(p, "aaaa", "aa")
    assert len(p.rules) == 2
    presentation.add_rule(p, "aaa", "aa")
    p.throw_if_bad_alphabet_or_rules()
    check_constructors(p)


def test_alphabet___letters__word_type_004():
    check_alphabet_letters(to_word)
    check_alphabet_letters(to_string)


def test_alphabet__letters__std__string_005():
    p = Presentation("abc")
    assert p.alphabet() == "abc"
    assert p.letter(0) == "a"
    assert p.letter(1) == "b"
    assert p.letter(2) == "c"
    p.alphabet("abcdef")
    assert len(p.alphabet()) == 6
    p.throw_if_bad_alphabet_or_rules()
    with pytest.raises(LibsemigroupsError):
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


def test_contains_empty_word_006():
    check_contains_empty_word(to_word)
    check_contains_empty_word(to_string)


def test_throw_if_bad_rules_throws_007():
    check_throw_if_bad_rules_throws(to_word)
    check_throw_if_bad_rules_throws(to_string)


def test_throw_if_bad_inverses_008():
    check_throw_if_bad_inverses(to_word)
    check_throw_if_bad_inverses(to_string)


def test_helpers_add_rule_s_009():
    check_add_rules(to_word)
    check_add_rules(to_string)


def test_helpers_add_rule_s__std__string__010():
    p = Presentation("abcdef")
    presentation.add_rule(p, "abcb", "aa")
    q = Presentation("abcdef")
    presentation.add_rule(q, "eb", "af")
    presentation.add_rule(q, "eb", "abbbbbbbbb")
    presentation.add_rules(p, q)
    assert p.rules == ["abcb", "aa", "eb", "af", "eb", "abbbbbbbbb"]
    assert q.rules == ["eb", "af", "eb", "abbbbbbbbb"]
    p.throw_if_bad_alphabet_or_rules()
    q.throw_if_bad_alphabet_or_rules()


def test_helpers_add_identity_rules_011():
    check_add_identity_rules(to_word)


def test_helpers_add_identity_rules_std_string_012():
    p = Presentation("abc")
    presentation.add_rule(p, "abcb", "aa")
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
    presentation.add_rule(p, "aaCac", "e")
    presentation.add_rule(p, "acbbACb", "e")
    presentation.add_rule(p, "ABabccc", "e")
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


def test_add_generator_015():
    p = Presentation("ab")
    p.add_generator()
    assert p.alphabet() == "abc"
    p.alphabet("ac")
    p.add_generator()
    assert p.alphabet() == "acb"
    p.alphabet("ac")
    p.add_generator("b")
    assert p.alphabet() == "acb"
    p.alphabet("ac")
    with pytest.raises(LibsemigroupsError):
        p.add_generator("c")


def test_helpers_remove_duplicate_rules_016():
    check_remove_duplicate_rules(to_word)
    check_remove_duplicate_rules(to_string)


def test_helpers_reduce_complements_017():
    p = Presentation([0, 1, 2])
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

    p = Presentation("abc")
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
    assert p.letter(0) == "a"
    assert p.letter(1) == "b"
    assert p.letter(2) == "c"

    p.throw_if_bad_alphabet_or_rules()


def test_helpers_sort_each_rule_018():
    check_sort_each_rule(to_word)
    check_sort_each_rule(to_string)


def test_helpers_sort_rules_019():
    check_sort_rules(to_word)
    check_sort_rules(to_string)


def test_helpers_longest_subword_reducing_length_020():
    check_longest_subword_reducing_length(to_word)
    check_longest_subword_reducing_length(to_string)


def test_helpers_remove_trivial_rules_021():
    p = Presentation([0, 1])
    presentation.add_rule(p, [0], [0])
    presentation.add_rule(p, [1], [1])
    presentation.add_rule(p, [1, 0, 1], [1])
    p.alphabet_from_rules()
    p.throw_if_bad_alphabet_or_rules()

    presentation.remove_trivial_rules(p)

    assert p.rules == [[1, 0, 1], [1]]


def test_helpers_remove_redundant_generators_022():
    p = Presentation([0, 1])
    presentation.add_rule(p, [0], [1])
    presentation.add_rule(p, [1], [1])
    presentation.add_rule(p, [1, 0, 1], [1])
    p.alphabet_from_rules()
    p.throw_if_bad_alphabet_or_rules()

    presentation.remove_redundant_generators(p)
    assert p.rules == [[0, 0, 0], [0]]
    p.rules = p.rules + [[1, 1, 1], [0]]
    presentation.remove_redundant_generators(p)
    assert p.rules == [[1, 1, 1, 1, 1, 1, 1, 1, 1], [1, 1, 1]]

    p = Presentation("abcdefghi")
    presentation.add_rule(p, "d", "ffg")
    presentation.add_rule(p, "bcbc", "cc")
    presentation.add_rule(p, "bbb", "d")
    presentation.add_rule(p, "biib", "e")
    presentation.add_rule(p, "iii", "h")
    presentation.add_rule(p, "h", "gg")
    presentation.add_rule(p, "d", "iii")

    presentation.remove_redundant_generators(p)
    assert p.alphabet() == "abcfgi"
    assert p.rules == ["bcbc", "cc", "bbb", "ffg", "iii", "gg", "ffg", "iii"]


def test_helpers_replace_word_023():
    p = Presentation([0])
    p.contains_empty_word(True)
    presentation.add_rule(p, [0, 0, 0, 0, 0], [])
    p.alphabet_from_rules()
    p.throw_if_bad_alphabet_or_rules()

    presentation.replace_word(p, [], [1])
    p.alphabet_from_rules()
    p.throw_if_bad_alphabet_or_rules()


def test_helpers_add_zero_rules_024():
    p = Presentation("abc")
    presentation.add_rule(p, "abcb", "aa")

    with pytest.raises(LibsemigroupsError):
        presentation.add_zero_rules(p, "0")
    p.alphabet("abc0")
    presentation.add_zero_rules(p, "0")
    assert p.rules == [
        "abcb",
        "aa",
        "a0",
        "0",
        "0a",
        "0",
        "b0",
        "0",
        "0b",
        "0",
        "c0",
        "0",
        "0c",
        "0",
        "00",
        "0",
    ]


def test_helpers_are_rules_sorted_025():
    p = Presentation("")
    p.contains_empty_word(False)
    p.alphabet("aAbBcCe")
    presentation.add_inverse_rules(p, "AaBbCce", "e")
    assert p.rules == [
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
    ]

    assert not presentation.are_rules_sorted(p)
    presentation.sort_rules(p)
    assert presentation.are_rules_sorted(p)


def test_change_alphabet__026():
    p = Presentation("")
    p.alphabet("ab")
    presentation.add_rule(p, "ba", "abaaabaa")
    presentation.replace_word_with_new_generator(p, "ba")
    presentation.change_alphabet(p, "abc")
    assert p.rules == ["c", "acaaca", "c", "ba"]
    assert p.alphabet() == "abc"
    p.throw_if_bad_alphabet_or_rules()
    # Alphabet wrong size
    with pytest.raises(LibsemigroupsError):
        presentation.change_alphabet(p, "ab")
    with pytest.raises(LibsemigroupsError):
        presentation.change_alphabet(p, "aab")
    assert p.alphabet() == "abc"
    assert p.rules == ["c", "acaaca", "c", "ba"]
    presentation.change_alphabet(p, "bac")
    assert p.rules == ["c", "bcbbcb", "c", "ab"]
    assert p.alphabet() == "bac"

    presentation.change_alphabet(p, "xyz")
    assert p.rules == ["z", "xzxxzx", "z", "yx"]
    assert p.alphabet() == "xyz"

    presentation.change_alphabet(p, "xyt")
    assert p.rules == ["t", "xtxxtx", "t", "yx"]
    assert p.alphabet() == "xyt"


def test_first_unused_letter_028():
    p = Presentation("ab")

    presentation.add_rule(p, "baabaa", "ababa")
    assert presentation.first_unused_letter(p) == "c"
    p.alphabet("abcdefghijklmnopq")
    assert presentation.first_unused_letter(p) == "r"
    p.alphabet("abcdefghijklmnopqrstuvwxyz")
    assert presentation.first_unused_letter(p) == "A"
    p.alphabet("abcdefgijklmnopqrstuvwxyz")
    assert presentation.first_unused_letter(p) == "h"
    p.alphabet("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")
    assert presentation.first_unused_letter(p) == "0"
    p.alphabet("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ02")
    assert presentation.first_unused_letter(p) == "1"
    letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
    for i, c in enumerate(letters):
        assert c == words.human_readable_letter(i)

    p.alphabet(256)
    with pytest.raises(LibsemigroupsError):
        assert presentation.first_unused_letter(p)
    with pytest.raises(LibsemigroupsError):
        assert p.alphabet(257)


def test_greedy_reduce_length_029():
    p = Presentation("ab")
    presentation.add_rule(p, "aaaaaaaaaaaaaaaa", "a")
    presentation.add_rule(p, "bbbbbbbbbbbbbbbb", "b")
    presentation.add_rule(p, "abb", "baa")
    assert presentation.length(p) == 40
    presentation.greedy_reduce_length(p)
    assert presentation.length(p) == 26
    assert p.rules == [
        "dddd",
        "a",
        "cccc",
        "b",
        "abb",
        "baa",
        "c",
        "bbbb",
        "d",
        "aaaa",
    ]
    assert presentation.longest_subword_reducing_length(p) == ""


def test_greedy_reduce_length_and_number_of_gens_030():
    p1 = Presentation("ab")
    presentation.add_rule(p1, "aaaaaa", "a")
    presentation.add_rule(p1, "bbbbbb", "b")
    presentation.add_rule(p1, "abb", "baa")
    q1 = Presentation("ab")
    presentation.add_rules(q1, p1)

    assert presentation.length(p1) == 20
    assert len(p1.alphabet()) == 2

    presentation.greedy_reduce_length_and_number_of_gens(p1)
    assert presentation.length(p1) == 20
    assert len(p1.alphabet()) == 2
    assert presentation.longest_subword_reducing_length(p1) != ""

    presentation.greedy_reduce_length(q1)
    assert presentation.length(q1) == 18
    assert len(q1.alphabet()) == 4
    assert presentation.longest_subword_reducing_length(q1) == ""
    assert p1 != q1

    p2 = Presentation("ab")
    presentation.add_rule(p2, "aaaaaaaaaaaaaaaa", "a")
    presentation.add_rule(p2, "bbbbbbbbbbbbbbbb", "b")
    presentation.add_rule(p2, "abb", "baa")
    q2 = Presentation("ab")
    presentation.add_rules(q2, p2)
    presentation.greedy_reduce_length(p2)
    presentation.greedy_reduce_length_and_number_of_gens(q2)
    assert p2 == q2


def test_longest_shortest_rule_031():
    check_longest_rule(to_word)
    check_longest_rule(to_string)
    check_longest_rule_length(to_word)
    check_longest_rule_length(to_string)


def test_make_semigroup_032():
    check_make_semigroup(to_word)
    check_make_semigroup(to_string)


def test_033():
    p = Presentation("ab")
    p.alphabet("ab")
    presentation.add_rule(p, "aaaaaaaab", "aaaaaaaaab")
    assert presentation.is_strongly_compressible(p)
    assert presentation.strongly_compress(p)
    assert p.rules == ["a", "aa"]

    p.rules = ["adadnadnasnamdnamdna", "akdjskadjksajdaldja"]
    p.alphabet_from_rules()

    assert presentation.is_strongly_compressible(p)
    assert presentation.strongly_compress(p)
    assert presentation.reduce_to_2_generators(p)
    assert p.rules == ["aaaaaaaaaaaaaaaaaaa", "baaaaaaaaaaaaaaaaa"]

    # Only works for 1-relation monoids at present
    p.alphabet("ab")
    presentation.add_rule(p, "aaaaaaaab", "aaaaaaaaab")
    presentation.add_rule(p, "aaaaaaaab", "aaaaaaaaab")
    assert not presentation.is_strongly_compressible(p)
    assert not presentation.strongly_compress(p)


def test_034():
    p = Presentation("ab")
    presentation.add_rule(p, "abaaaabab", "abbabaaaab")
    assert presentation.strongly_compress(p)
    assert p.rules == ["abccdae", "fgeabccd"]

    q = copy.copy(p)
    assert presentation.reduce_to_2_generators(q)
    assert q.rules == ["aaaaaaa", "baaaaaaa"]

    q = copy.copy(p)
    assert presentation.reduce_to_2_generators(q, 1)
    assert q.rules == ["abbbbab", "bbbabbbb"]


def test_035():
    p = Presentation("ab")
    presentation.add_rule(p, "aabb", "aaabaaab")
    assert presentation.is_strongly_compressible(p)
    assert presentation.strongly_compress(p)
    presentation.reverse(p)
    assert p.rules == ["cba", "baadbaa"]

    q = copy.copy(p)
    assert presentation.reduce_to_2_generators(q)
    assert q.rules == ["aba", "baaabaa"]
    assert p.rules == ["cba", "baadbaa"]

    q = copy.copy(p)
    assert presentation.reduce_to_2_generators(q, 1)
    assert q.rules == ["abb", "bbbbbbb"]

    with pytest.raises(LibsemigroupsError):
        presentation.reduce_to_2_generators(q, 2)

    q = copy.copy(p)
    presentation.add_rule(q, "aabb", "aaabaaab")

    assert not presentation.reduce_to_2_generators(q, 1)

    q.rules = ["aaaaa", "a"]
    assert not presentation.reduce_to_2_generators(q)

    q.rules = ["aaaaa", ""]
    assert not presentation.reduce_to_2_generators(q)

    q.rules = ["abcacbabab", ""]
    assert not presentation.reduce_to_2_generators(q)


def test_036():
    p = Presentation("ab")
    p.rules = ["aabb", "aaabaab"]
    assert presentation.is_strongly_compressible(p)
    assert presentation.strongly_compress(p)
    assert p.rules == ["abc", "aabdab"]
    assert not presentation.reduce_to_2_generators(p)
    presentation.reverse(p)
    assert presentation.reduce_to_2_generators(p)
    assert p.rules == ["aba", "baabaa"]


def test_inverse_presentation_constructors_037():
    ip = InversePresentation([0, 1, 2])
    presentation.add_rule(ip, [0, 0, 0], [0])
    assert len(ip.rules) == 2
    presentation.add_rule(ip, [0, 0, 0], [0])
    with pytest.raises(LibsemigroupsError):
        ip.throw_if_bad_alphabet_rules_or_inverses()
    ip.inverses([2, 1, 0])
    check_constructors(ip)

    ip = InversePresentation("abc")
    presentation.add_rule(ip, "aaa", "a")
    assert len(ip.rules) == 2
    presentation.add_rule(ip, "aaa", "a")
    with pytest.raises(LibsemigroupsError):
        ip.throw_if_bad_alphabet_rules_or_inverses()
    ip.inverses("cba")
    check_constructors(ip)


def test_inverse_constructors_038():
    check_inverse_constructors(to_string)
    check_inverse_constructors(to_word)


def test_inverses_039():
    check_inverses(to_string)
    check_inverses(to_word)


def test_remove_generator_040():
    check_remove_generator(to_string)
    check_remove_generator(to_word)


def test_presentation_return_policy():
    p = Presentation([0, 1, 2])
    presentation.add_rule(p, [0, 0, 0], [0])

    assert p.copy() is not p
    assert p.alphabet() is not p.alphabet()
    assert p.alphabet(5) is p
    assert p.alphabet([1]) is p
    assert p.alphabet_from_rules() is p
    assert p.contains_empty_word(False) is p
    assert p.init() is p
    assert p.add_generator(2) is p
    assert p.remove_generator(2) is p

    p = Presentation("abc")
    assert p.copy() is not p
    assert p.alphabet() is not p.alphabet()
    assert p.alphabet(5) is p
    assert p.alphabet_from_rules() is p
    assert p.alphabet("a") is p
    assert p.contains_empty_word(False) is p
    assert p.init() is p
    assert p.add_generator("b") is p
    assert p.remove_generator("b") is p
