# -*- coding: utf-8 -*-
# pylint: disable=missing-function-docstring, invalid-name

# Copyright (c) 2021, J. D. Mitchell + Maria Tsalakou
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some functions used in tests for wilo.
"""

from libsemigroups_pybind11 import wilo, number_of_words


def test_001():
    u = [0, 0, 0, 0]
    v = [1, 1, 1, 1]
    assert not list(wilo(2, 1, v, u))
    assert not list(wilo(2, 1, u, u))
    w1 = list(wilo(2, 2, [], [1, 1]))
    assert len(w1) == 3
    w2 = [list(x) for x in w1]
    assert w2 == [[], [0], [1]]
    w3 = list(wilo(2, 1, [], [1, 1]))
    assert len(w3) == 1
    assert not list(w3[0])
    w4 = list(wilo(2, 1, [], [0]))
    assert len(w4) == 1
    assert not list(w4[0])


def test_002():
    w = list(wilo(2, 4, [0], [1, 1, 1, 1]))
    u = [list(x) for x in w]
    assert u == [
        [0],
        [0, 0],
        [0, 0, 0],
        [0, 0, 1],
        [0, 1],
        [0, 1, 0],
        [0, 1, 1],
        [1],
        [1, 0],
        [1, 0, 0],
        [1, 0, 1],
        [1, 1],
        [1, 1, 0],
        [1, 1, 1],
    ]
    assert len(u) == 14


def test_003():
    first = [0]
    last = [2, 2, 2, 2]
    w = list(wilo(3, 4, first, last))
    assert len(w) == 39
    assert len(w) == number_of_words(3, 1, 4)
    u = [list(x) for x in w]
    assert u == [
        [0],
        [0, 0],
        [0, 0, 0],
        [0, 0, 1],
        [0, 0, 2],
        [0, 1],
        [0, 1, 0],
        [0, 1, 1],
        [0, 1, 2],
        [0, 2],
        [0, 2, 0],
        [0, 2, 1],
        [0, 2, 2],
        [1],
        [1, 0],
        [1, 0, 0],
        [1, 0, 1],
        [1, 0, 2],
        [1, 1],
        [1, 1, 0],
        [1, 1, 1],
        [1, 1, 2],
        [1, 2],
        [1, 2, 0],
        [1, 2, 1],
        [1, 2, 2],
        [2],
        [2, 0],
        [2, 0, 0],
        [2, 0, 1],
        [2, 0, 2],
        [2, 1],
        [2, 1, 0],
        [2, 1, 1],
        [2, 1, 2],
        [2, 2],
        [2, 2, 0],
        [2, 2, 1],
        [2, 2, 2],
    ]


def test_004():
    first = []
    last = [2, 2, 2, 2, 2, 2, 2, 2, 2, 2]
    w = list(wilo(3, 10, first, last))
    assert len(w) == 29524
    assert len(w) == number_of_words(3, 0, 10)


def test_005():
    first = []
    last = [2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2]
    w = list(wilo(3, 13, first, last))
    assert len(w) == 797161
    assert len(w) == number_of_words(3, 0, 13)


def test_006():
    first = []
    last = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    w = list(wilo(1, 10, first, last))
    assert len(w) == 10
    u = [list(x) for x in w]
    assert u == [
        [],
        [0],
        [0, 0],
        [0, 0, 0],
        [0, 0, 0, 0],
        [0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0],
    ]

    w = list(wilo(0, 0, first, last))
    assert not w
    first = [0, 0]
    last = [0, 0, 0, 0]
    w = list(wilo(1, 4, first, last))
    assert len(w) == 2
    assert list(w[0]) == [0, 0]
    assert list(w[1]) == [0, 0, 0]
    w = list(wilo(1, 1, first, last))
    assert not w
    w = list(wilo(1, 5, [0], [1]))
    assert len(w) == 4
    u = [list(x) for x in w]
    assert u == [[0], [0, 0], [0, 0, 0], [0, 0, 0, 0]]


def test_007():
    first = [0, 1, 2, 3]
    last = [4, 4, 4, 4, 4]
    w = list(wilo(4, 5, first, last))
    assert len(w) == 303


def test_008():
    first = [0, 1]
    last = [1, 1, 1]
    w = list(wilo(2, 3, first, last))
    assert len(w) == 4
    u = [list(x) for x in w]
    assert u == [[0, 1], [1], [1, 0], [1, 1]]
    w = list(wilo(2, 1, first, last))
    assert not w
