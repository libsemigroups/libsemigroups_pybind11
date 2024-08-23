# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell + Maria Tsalakou
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=missing-function-docstring

"""
This module contains some tests for the functionality in words.*pp.
"""

import pytest
from libsemigroups_pybind11 import (
    number_of_words,
    random_word,
    WordRange,
    StringRange,
    ToWord,
    LibsemigroupsError,
)
from libsemigroups_pybind11.words import parse_relations, human_readable_letter


def test_number_of_words():
    assert number_of_words(3, 1, 4) == 39
    assert number_of_words(3, 0, 10) == 29524
    assert number_of_words(2, 5, 6) == 32
    assert number_of_words(3, 0, 13) == 797161
    assert number_of_words(2, 4, 1) == 0
    assert number_of_words(2, 4, 4) == 0
    assert number_of_words(2, 4, 2) == 0


def test_random_word():
    w = random_word(10, 3)
    assert len(w) == 10
    with pytest.raises(LibsemigroupsError):
        random_word(10, 0)


def test_Words_000():  # pylint: disable=invalid-name
    first = [0]
    last = [0, 0, 0, 0]
    words = WordRange()
    words.first(first).last(last)
    assert words.count() == 0
    words.alphabet_size(2)
    assert words.count() == 14
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


def test_Strings_000():  # pylint: disable=invalid-name
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


def test_to_word():
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


def test_ToWord():  # pylint: disable=invalid-name
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

    human_readable_letter(0) == "a"
    human_readable_letter(255) == "\377"
