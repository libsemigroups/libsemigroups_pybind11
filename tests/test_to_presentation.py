# -*- coding: utf-8 -*-
# Copyright (c) 2024 Joseph Edwards
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=missing-function-docstring

"""
This module contains some tests for the to(_inverse)_presentation functions.
"""

import pytest
from libsemigroups_pybind11 import (
    presentation,
    to_inverse_presentation,
    to_presentation,
    Presentation,
    InversePresentation,
)

###############################################################################
# Helper functions
###############################################################################


def sample_to_str(i):
    return "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM9876543210"[i]


def sample_to_int(x):
    return (
        "mnbvcxzlkjhgfdsapoiuytrewqMNBVCXZLKJHGFDSAPOIUYTREWQ5432167890".index(
            x
        )
    )


###############################################################################
# Tests
###############################################################################


def test_to_presentation_000():
    p = Presentation("abcdef")
    presentation.add_zero_rules(p, "e")
    assert p.rules == [
        "ae",
        "e",
        "ea",
        "e",
        "be",
        "e",
        "eb",
        "e",
        "ce",
        "e",
        "ec",
        "e",
        "de",
        "e",
        "ed",
        "e",
        "ee",
        "e",
        "fe",
        "e",
        "ef",
        "e",
    ]
    q = to_presentation(p)
    assert q.alphabet() == [0, 1, 2, 3, 4, 5]
    assert q.rules == [
        [0, 4],
        [4],
        [4, 0],
        [4],
        [1, 4],
        [4],
        [4, 1],
        [4],
        [2, 4],
        [4],
        [4, 2],
        [4],
        [3, 4],
        [4],
        [4, 3],
        [4],
        [4, 4],
        [4],
        [5, 4],
        [4],
        [4, 5],
        [4],
    ]
    assert p == to_presentation(q)


def test_to_presentation_001():
    p = Presentation("abcde")
    presentation.add_zero_rules(p, "e")
    assert p.rules == [
        "ae",
        "e",
        "ea",
        "e",
        "be",
        "e",
        "eb",
        "e",
        "ce",
        "e",
        "ec",
        "e",
        "de",
        "e",
        "ed",
        "e",
        "ee",
        "e",
    ]
    with pytest.raises(TypeError):
        q = to_presentation(p, sample_to_str)

    q = to_presentation(p, sample_to_int)
    assert q.alphabet() == [15, 2, 4, 13, 23]
    assert q.rules == [
        [15, 23],
        [23],
        [23, 15],
        [23],
        [2, 23],
        [23],
        [23, 2],
        [23],
        [4, 23],
        [23],
        [23, 4],
        [23],
        [13, 23],
        [23],
        [23, 13],
        [23],
        [23, 23],
        [23],
    ]

    r = to_presentation(q, sample_to_str)
    assert r.alphabet() == "hetfb"
    assert r.rules == [
        "hb",
        "b",
        "bh",
        "b",
        "eb",
        "b",
        "be",
        "b",
        "tb",
        "b",
        "bt",
        "b",
        "fb",
        "b",
        "bf",
        "b",
        "bb",
        "b",
    ]


def test_to_inverse_presentation_002():
    ip = InversePresentation("abc")
    ip.inverses("cba")
    presentation.add_rule(ip, "aaa", "b")
    presentation.add_rule(ip, "bac", "cab")
    iq = to_inverse_presentation(ip)
    assert iq.alphabet() == [0, 1, 2]
    assert iq.inverses() == [2, 1, 0]
    assert iq.rules == [[0, 0, 0], [1], [1, 0, 2], [2, 0, 1]]

    assert ip == to_presentation(iq)


def test_to_inverse_presentation_003():
    ip = InversePresentation("abc")
    ip.inverses("cba")
    presentation.add_rule(ip, "aaa", "b")
    presentation.add_rule(ip, "bac", "cab")

    iq = to_inverse_presentation(ip, sample_to_int)
    assert iq.alphabet() == [15, 2, 4]
    assert iq.inverses() == [4, 2, 15]
    assert iq.rules == [[15, 15, 15], [2], [2, 15, 4], [4, 15, 2]]

    ir = to_inverse_presentation(iq, sample_to_str)
    assert ir.alphabet() == "het"
    assert ir.inverses() == "teh"
    assert ir.rules == ["hhh", "e", "eht", "the"]


def test_to_inverse_presentation_004():
    p = Presentation("abc")
    presentation.add_rule(p, "aaa", "b")
    presentation.add_rule(p, "bac", "cab")
    ip = to_inverse_presentation(p)
    assert ip.alphabet() == "abcdef"
    assert ip.inverses() == "defabc"
    assert ip.rules == p.rules

    q = to_presentation(p)
    iq = to_inverse_presentation(q)
    assert iq.alphabet() == [0, 1, 2, 3, 4, 5]
    assert iq.inverses() == [3, 4, 5, 0, 1, 2]
    assert iq.rules == q.rules

    assert to_inverse_presentation(
        to_presentation(p)
    ) == to_inverse_presentation(to_inverse_presentation(p))
