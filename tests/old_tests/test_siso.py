# -*- coding: utf-8 -*-

# Copyright (c) 2021, J. D. Mitchell + Maria Tsalakou
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, missing-function-docstring, invalid-name

"""
This module contains some tests for silo/siso.
"""

from libsemigroups_pybind11 import silo, sislo, number_of_words


def test_001():
    v = "aaaaaaaaaa"
    w = list(silo("a", 10, "", "aaaaaaaaaa"))
    assert len(w) == 10
    assert w == [
        "",
        "a",
        "aa",
        "aaa",
        "aaaa",
        "aaaaa",
        "aaaaaa",
        "aaaaaaa",
        "aaaaaaaa",
        "aaaaaaaaa",
    ]
    w = list(silo("", 10, "", "aaaaaaaaaa"))
    assert len(w) == 1
    w = list(silo("a", 4, "aa", v))
    assert w == ["aa", "aaa"]


def test_002():
    u = ""
    v = "aaaaaaaaaa"
    w1 = list(silo("ab", 4, v, u))
    assert not w1
    w2 = list(silo("ab", 4, u, u))
    assert not w2
    w3 = list(silo("ab", 2, "a", "bb"))
    assert w3 == ["a", "b"]
    w4 = list(silo("ab", 1, u, v))
    assert w4 == [""]


def test_003():
    w = list(silo("ab", 4, "a", "bbbbb"))
    assert len(w) == 14
    assert w == [
        "a",
        "aa",
        "aaa",
        "aab",
        "ab",
        "aba",
        "abb",
        "b",
        "ba",
        "baa",
        "bab",
        "bb",
        "bba",
        "bbb",
    ]

    w = list(silo("ba", 4, "b", "aaaaa"))
    assert w == [
        "b",
        "bb",
        "bbb",
        "bba",
        "ba",
        "bab",
        "baa",
        "a",
        "ab",
        "abb",
        "aba",
        "aa",
        "aab",
        "aaa",
    ]


def test_004():
    w = list(silo("abc", 13, "", "ccccccccccccc"))
    assert len(w) == 797161
    assert len(w) == number_of_words(3, 0, 13)


def test_005():
    first = ""
    last = "aaaaaaaaaa"
    w = list(sislo("a", first, last))
    assert len(w) == 10
    assert w == [
        "",
        "a",
        "aa",
        "aaa",
        "aaaa",
        "aaaaa",
        "aaaaaa",
        "aaaaaaa",
        "aaaaaaaa",
        "aaaaaaaaa",
    ]
    w = list(sislo("a", "aa", "aaaa"))
    assert w == ["aa", "aaa"]


def test_006():
    first = ""
    last = "bbaaab"
    w1 = list(sislo("ab", last, first))
    assert not w1
    w2 = list(sislo("ab", first, first))
    assert not w2
    w3 = list(sislo("ab", "a", "aa"))
    assert w3 == ["a", "b"]
    w4 = list(sislo("ab", first, last))
    assert len(w4) == 112
    assert w4 == [
        "",
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
        "aaaa",
        "aaab",
        "aaba",
        "aabb",
        "abaa",
        "abab",
        "abba",
        "abbb",
        "baaa",
        "baab",
        "baba",
        "babb",
        "bbaa",
        "bbab",
        "bbba",
        "bbbb",
        "aaaaa",
        "aaaab",
        "aaaba",
        "aaabb",
        "aabaa",
        "aabab",
        "aabba",
        "aabbb",
        "abaaa",
        "abaab",
        "ababa",
        "ababb",
        "abbaa",
        "abbab",
        "abbba",
        "abbbb",
        "baaaa",
        "baaab",
        "baaba",
        "baabb",
        "babaa",
        "babab",
        "babba",
        "babbb",
        "bbaaa",
        "bbaab",
        "bbaba",
        "bbabb",
        "bbbaa",
        "bbbab",
        "bbbba",
        "bbbbb",
        "aaaaaa",
        "aaaaab",
        "aaaaba",
        "aaaabb",
        "aaabaa",
        "aaabab",
        "aaabba",
        "aaabbb",
        "aabaaa",
        "aabaab",
        "aababa",
        "aababb",
        "aabbaa",
        "aabbab",
        "aabbba",
        "aabbbb",
        "abaaaa",
        "abaaab",
        "abaaba",
        "abaabb",
        "ababaa",
        "ababab",
        "ababba",
        "ababbb",
        "abbaaa",
        "abbaab",
        "abbaba",
        "abbabb",
        "abbbaa",
        "abbbab",
        "abbbba",
        "abbbbb",
        "baaaaa",
        "baaaab",
        "baaaba",
        "baaabb",
        "baabaa",
        "baabab",
        "baabba",
        "baabbb",
        "babaaa",
        "babaab",
        "bababa",
        "bababb",
        "babbaa",
        "babbab",
        "babbba",
        "babbbb",
        "bbaaaa",
    ]


def test_007():
    w = list(sislo("ab", "a", "aaaa"))
    assert w == [
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
    assert len(w) == 14
    w = list(sislo("ba", "b", "bbbb"))
    assert w == [
        "b",
        "a",
        "bb",
        "ba",
        "ab",
        "aa",
        "bbb",
        "bba",
        "bab",
        "baa",
        "abb",
        "aba",
        "aab",
        "aaa",
    ]


def test_008():
    w = list(sislo("abc", "", "aaaaaaaaaaaaa"))
    assert len(w) == 797161
    assert len(w) == number_of_words(3, 0, 13)
