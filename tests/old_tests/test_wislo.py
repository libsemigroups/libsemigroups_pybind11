# -*- coding: utf-8 -*-
# pylint: disable=missing-function-docstring, missing-class-docstring, invalid-name

# Copyright (c) 2021, J. D. Mitchell + Maria Tsalakou
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some functions used in tests for wislo.
"""

from libsemigroups_pybind11 import wislo, number_of_words


def test_000():
    first = [0]
    last = [0, 0, 0, 0]
    w = list(wislo(2, first, last))
    assert len(w) == 14
    u = [list(x) for x in w]
    assert u == [
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


def test_001():
    first = [0, 0, 0, 0]
    last = [0, 0, 0, 0, 0]
    w1 = list(wislo(2, last, first))
    assert not w1
    w2 = list(wislo(2, last, last))
    assert not w2
    w3 = list(wislo(2, last, [0, 0, 0, 0, 0, 0]))
    assert len(w3) == 32
    u = [list(x) for x in w3]
    assert u == [
        [0, 0, 0, 0, 0],
        [0, 0, 0, 0, 1],
        [0, 0, 0, 1, 0],
        [0, 0, 0, 1, 1],
        [0, 0, 1, 0, 0],
        [0, 0, 1, 0, 1],
        [0, 0, 1, 1, 0],
        [0, 0, 1, 1, 1],
        [0, 1, 0, 0, 0],
        [0, 1, 0, 0, 1],
        [0, 1, 0, 1, 0],
        [0, 1, 0, 1, 1],
        [0, 1, 1, 0, 0],
        [0, 1, 1, 0, 1],
        [0, 1, 1, 1, 0],
        [0, 1, 1, 1, 1],
        [1, 0, 0, 0, 0],
        [1, 0, 0, 0, 1],
        [1, 0, 0, 1, 0],
        [1, 0, 0, 1, 1],
        [1, 0, 1, 0, 0],
        [1, 0, 1, 0, 1],
        [1, 0, 1, 1, 0],
        [1, 0, 1, 1, 1],
        [1, 1, 0, 0, 0],
        [1, 1, 0, 0, 1],
        [1, 1, 0, 1, 0],
        [1, 1, 0, 1, 1],
        [1, 1, 1, 0, 0],
        [1, 1, 1, 0, 1],
        [1, 1, 1, 1, 0],
        [1, 1, 1, 1, 1],
    ]


def test_002():
    first = []
    last = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    w = list(wislo(3, first, last))
    assert len(w) == 29524
    assert len(w) == number_of_words(3, 0, 10)


def test_003():

    first = []
    last = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    w = list(wislo(3, first, last))
    assert len(w) == 797161
    assert len(w) == number_of_words(3, 0, 13)
