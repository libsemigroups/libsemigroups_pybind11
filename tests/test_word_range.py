# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell + Maria Tsalakou
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=missing-function-docstring, invalid-name

"""
This module contains some tests for the functionality in words-range.*pp.
"""

import pytest
from libsemigroups_pybind11 import (
    number_of_words,
    random_word,
    random_string,
    random_strings,
    WordRange,
    StringRange,
    ToString,
    ToWord,
    LibsemigroupsError,
    Order,
)
from libsemigroups_pybind11.words import (
    parse_relations,
    human_readable_letter,
    human_readable_index,
)


def test_number_of_words():
    assert number_of_words(3, 1, 4) == 39
    assert number_of_words(3, 0, 10) == 29524
    assert number_of_words(2, 5, 6) == 32
    assert number_of_words(3, 0, 13) == 797161
    assert number_of_words(2, 4, 1) == 0
    assert number_of_words(2, 4, 4) == 0
    assert number_of_words(2, 4, 2) == 0


def test_alphabet_size():
    wr = WordRange()
    for i in range(1000):
        wr.alphabet_size(i)
        assert wr.alphabet_size() == i


def test_random_word():
    w = random_word(10, 3)
    assert len(w) == 10
    with pytest.raises(LibsemigroupsError):
        random_word(10, 0)


def test_random_string():
    s = random_string("abc", 10)
    assert len(s) == 10


def test_random_strings():
    rs = random_strings("abc", 10, 5, 6)
    assert len(list(rs)) == 10
    for s in rs:
        assert len(s) == 5


def test_range_lex():
    wr, sr = WordRange(), StringRange()
    wr.alphabet_size(4).last([3, 3, 3]).upper_bound(4).order(Order.lex)
    sr.alphabet("abcd").last("ddd").upper_bound(4).order(Order.lex)

    assert wr.get() == []
    assert sr.get() == ""

    n = 84
    for i in range(n):
        assert not wr.at_end()
        assert not sr.at_end()
        assert wr.count() == n - i
        assert sr.count() == n - i
        wr.next()
        sr.next()
    assert wr.at_end()
    assert sr.at_end()


def test_range_shortlex():
    wr, sr = WordRange(), StringRange()
    wr.alphabet_size(4).min(2).max(10)
    sr.alphabet("abcd").min(2).max(10)

    assert wr.get() == [0, 0]
    assert sr.get() == "aa"

    n = number_of_words(4, 2, 10)
    for i in range(n):
        assert not wr.at_end()
        assert not sr.at_end()
        assert wr.count() == wr.size_hint() == n - i
        assert sr.count() == sr.size_hint() == n - i
        wr.next()
        sr.next()
    assert wr.at_end()
    assert sr.at_end()


def test_init():
    wr, sr = WordRange(), StringRange()
    wr.alphabet_size(4).min(0).max(5).order(Order.lex).upper_bound(7)
    sr.alphabet("abcd").min(0).max(5).order(Order.lex).upper_bound(7)
    wr.init()
    sr.init()
    assert wr.alphabet_size() == len(sr.alphabet()) == 0
    assert sr.first() == ""
    assert sr.last() == ""
    assert wr.first() == []
    assert wr.last() == []
    assert sr.order() == wr.order() == Order.shortlex
    assert sr.upper_bound() == wr.upper_bound() == 0


def test_ToString():
    wr = WordRange()
    wr.alphabet_size(2).min(0).max(3)
    with pytest.raises(LibsemigroupsError):
        wr | ToString("b")  # pylint: disable=expression-not-assigned
    assert list(wr | ToString("dc")) == ["", "d", "c", "dd", "dc", "cd", "cc"]
    tstr = ToString("zxy")
    assert tstr.alphabet() == "zxy"
    assert tstr.can_convert_letter(0)
    assert tstr.can_convert_letter(1)
    assert tstr.can_convert_letter(2)
    assert not tstr.can_convert_letter(3)
    assert not tstr.empty()

    tstr.init("abcd")
    assert tstr.alphabet() == "abcd"
    assert tstr.can_convert_letter(0)
    assert tstr.can_convert_letter(1)
    assert tstr.can_convert_letter(2)
    assert tstr.can_convert_letter(3)
    assert not tstr.can_convert_letter(4)
    assert not tstr.empty()

    tstr.init()
    assert tstr.alphabet() == ""
    assert not tstr.can_convert_letter(0)
    assert tstr.empty()


def test_ToWord():
    sr = StringRange()
    sr.alphabet("cd").min(0).max(3)
    with pytest.raises(LibsemigroupsError):
        sr | ToWord("b")  # pylint: disable=expression-not-assigned
    with pytest.raises(LibsemigroupsError):
        sr | ToWord("bd")  # pylint: disable=expression-not-assigned
    assert list(sr | ToWord("edc")) == [
        [],
        [2],
        [1],
        [2, 2],
        [2, 1],
        [1, 2],
        [1, 1],
    ]
    twrd = ToWord("zxy")
    assert twrd.alphabet() == "zxy"
    assert twrd.can_convert_letter("z")
    assert twrd.can_convert_letter("x")
    assert twrd.can_convert_letter("y")
    assert not twrd.can_convert_letter("a")
    assert not twrd.empty()

    twrd.init("abcd")
    assert twrd.alphabet() == "abcd"
    assert twrd.can_convert_letter("a")
    assert twrd.can_convert_letter("b")
    assert twrd.can_convert_letter("c")
    assert twrd.can_convert_letter("d")
    assert not twrd.can_convert_letter("e")
    assert not twrd.empty()

    twrd.init()
    assert twrd.alphabet() == ""
    assert not twrd.can_convert_letter("a")
    assert twrd.empty()


def test_WordRange_000():
    first = [0]
    last = [0, 0, 0, 0]
    words = WordRange()
    words.first(first).last(last)
    assert words.first() == first
    assert words.last() == last
    assert words.count() == 0
    words.alphabet_size(2)
    assert words.count() == 14
    assert words.get() == first
    assert list(words) == [
        [0],
        [1],
        [0, 0],
        [0, 1],
        [1, 0],
        [1, 1],
        [0, 0, 0],
        [0, 0, 1],
        [0, 1, 0],
        [0, 1, 1],
        [1, 0, 0],
        [1, 0, 1],
        [1, 1, 0],
        [1, 1, 1],
    ]


def test_StringRange_000():
    strings = StringRange()
    strings.alphabet("ab").first("a").last("aaaa")
    assert strings.count() == 14
    assert list(strings) == [
        "a",
        "b",
        "aa",
        "ab",
        "ba",
        "bb",
        "aaa",
        "aab",
        "aba",
        "abb",
        "baa",
        "bab",
        "bba",
        "bbb",
    ]


def test_parse_relations():
    assert parse_relations("cd(ab)^2ef") == "cdababef"
    assert parse_relations("cd((ab)^2)^4ef") == "cdababababababababef"
    assert parse_relations("cd((ab)^2)^4(ef)^2") == "cdababababababababefef"
    assert parse_relations("a^16") == "aaaaaaaaaaaaaaaa"
    assert (
        parse_relations("a^16cd^10((ab)^2)^4(ef)^2")
        == "aaaaaaaaaaaaaaaacddddddddddababababababababefef"
    )
    assert parse_relations("X^3(yx^2)") == "XXXyxx"
    assert parse_relations("b(aX)^3x") == "baXaXaXx"
    assert (
        parse_relations("((a)b^2y)^10")
        == "abbyabbyabbyabbyabbyabbyabbyabbyabbyabby"
    )

    assert parse_relations("()") == ""
    assert parse_relations("y^0") == ""
    assert parse_relations("") == ""
    assert parse_relations("a") == "a"

    assert parse_relations("           ") == ""


def test_ToWord_1():
    to_word = ToWord()
    assert to_word(parse_relations("cd(ab)^2ef")) == [2, 3, 0, 1, 0, 1, 4, 5]
    assert to_word(parse_relations("cd((ab)^2)^4ef")) == [
        2,
        3,
        0,
        1,
        0,
        1,
        0,
        1,
        0,
        1,
        0,
        1,
        0,
        1,
        0,
        1,
        0,
        1,
        4,
        5,
    ]
    assert to_word(parse_relations("cd((ab)^2)^4(ef)^2")) == [
        2,
        3,
        0,
        1,
        0,
        1,
        0,
        1,
        0,
        1,
        0,
        1,
        0,
        1,
        0,
        1,
        0,
        1,
        4,
        5,
        4,
        5,
    ]


def test_ToWord_2():
    toword = ToWord("BCA")
    assert not toword.empty()
    assert toword("BCABACB") == [0, 1, 2, 0, 2, 1, 0]
    assert toword("B") == [0]
    assert toword("C") == [1]
    assert toword("A") == [2]

    with pytest.raises(LibsemigroupsError):
        toword.init("aa")
    with pytest.raises(LibsemigroupsError):
        toword.init("XX")

    assert toword("BCABACB") == [0, 1, 2, 0, 2, 1, 0]
    assert toword("B") == [0]
    assert toword("C") == [1]
    assert toword("A") == [2]
    with pytest.raises(LibsemigroupsError):
        assert toword("z")


def test_human_readable_letter():
    with pytest.raises(LibsemigroupsError):
        human_readable_letter(256)

    assert human_readable_letter(0) == "a"
    assert human_readable_letter(255) == "\377"


def test_human_readable_index():
    assert human_readable_index("\377") == 255
    with pytest.raises(ValueError):
        human_readable_index("Ā")  # this is character 256


def test_ranges_return_policy():
    for TestType in (WordRange, StringRange):
        r = TestType()
        assert r.copy() is not r
        assert r.init() is r
        assert r.max(2) is r
        assert r.min(2) is r
        assert r.order(Order.shortlex) is r
        assert r.upper_bound(10) is r

    r = WordRange()
    assert r.alphabet_size(2) is r
    assert r.first([0] * 2) is r
    assert r.last([1] * 2) is r

    r = StringRange()
    assert r.alphabet("ab") is r
    assert r.first("a" * 2) is r
    assert r.last("b" * 2) is r


def test_to_return_policy():
    for TestType in (ToWord, ToString):
        t = TestType()
        assert t.copy() is not t
        assert t.init() is t
        assert t.init("bc") is t

    t = ToWord()

    t = ToString()
