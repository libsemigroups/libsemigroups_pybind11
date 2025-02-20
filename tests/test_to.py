# -*- coding: utf-8 -*-
# Copyright (c) 2025 Joseph Edwards
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=missing-function-docstring

"""
This module contains some tests for the to function.
"""

import pytest
from libsemigroups_pybind11 import (
    congruence_kind,
    Congruence,
    FroidurePin,
    Kambites,
    KnuthBendix,
    Presentation,
    presentation,
    to,
    ToddCoxeter,
    WordGraph,
    ReportGuard,
)

ReportGuard(False)


def construct_from_pres(ReturnType, Word):
    if Word != str:
        p = Presentation([0, 1])
        presentation.add_rule(p, [0, 1], [1, 0])
        presentation.add_rule(p, [0, 0], [0])
        presentation.add_rule(p, [1, 1], [1])
        return ReturnType(congruence_kind.twosided, p)
    else:
        p = Presentation("ab")
        presentation.add_rule(p, "ab", "ba")
        presentation.add_rule(p, "aa", "a")
        presentation.add_rule(p, "bb", "b")
        return ReturnType(congruence_kind.twosided, p)


def check_cong_to_froidure_pin(Type, Word):
    thing = construct_from_pres(Type, Word)
    fp = to(thing, return_type=FroidurePin)
    fp.run()
    assert fp.is_finite()
    assert fp.number_of_idempotents() == 3


def test_to_000():
    check_cong_to_froidure_pin(KnuthBendix, str)


def test_to_001():
    check_cong_to_froidure_pin(KnuthBendix, int)


def test_to_002():
    check_cong_to_froidure_pin(ToddCoxeter, str)


def test_to_003():
    check_cong_to_froidure_pin(ToddCoxeter, int)


def test_to_004():
    check_cong_to_froidure_pin(Congruence, str)


def test_to_005():
    check_cong_to_froidure_pin(Congruence, int)


# TODO why does this cause a segfault?
# def test_to_006():
#     k = Kambites(Word=str)
#     assert isinstance(to(k, return_type=FroidurePin), FroidurePin)


def test_to_007():
    p = Presentation("abcdefg")
    presentation.add_rule(p, "abcd", "aaaeaa")
    presentation.add_rule(p, "ef", "dg")
    k = Kambites(congruence_kind.twosided, p)

    fp = to(k, return_type=FroidurePin)
    fp.enumerate(100)
    assert fp.current_size() == 8205


def test_to_008():
    p = Presentation([0, 1, 2, 3, 4, 5, 6])
    presentation.add_rule(p, [0, 1, 2, 3], [0, 0, 0, 4, 0, 0])
    presentation.add_rule(p, [4, 5], [3, 6])
    k = Kambites(congruence_kind.twosided, p)

    fp = to(k, return_type=FroidurePin)
    fp.enumerate(100)
    assert fp.current_size() == 8205


def test_to_009():
    p = Presentation("abcdefg")
    presentation.add_rule(p, "abcd", "aaaeaa")
    presentation.add_rule(p, "ef", "dg")
    # Kambites wins here, so this checks FroidurePinKEString
    c = Congruence(congruence_kind.twosided, p)

    fp = to(c, return_type=FroidurePin)
    fp.enumerate(100)
    assert fp.current_size() == 8205


def test_to_010():
    w = WordGraph(3, 1)
    w.target(0, 0, 1)
    w.target(1, 0, 1)
    w.target(2, 0, 1)

    fp = to(w, return_type=FroidurePin)
    fp.run()
    assert fp.number_of_rules() == 1


def test_to_010():
    w = WordGraph(3, 1)
    w.target(0, 0, 1)
    w.target(1, 0, 1)
    w.target(2, 0, 1)

    fp = to(w, return_type=FroidurePin)
    fp.run()
    assert fp.number_of_rules() == 1


def test_to_011():
    w = WordGraph(3, 1)
    w.target(0, 0, 1)
    w.target(1, 0, 1)
    w.target(2, 0, 1)

    fp = to(w, 1, 2, return_type=FroidurePin)
    fp.run()
    assert fp.number_of_rules() == 1


def test_to_999():
    x = 10
    with pytest.raises(TypeError):
        to(x, return_type=FroidurePin)
    with pytest.raises(TypeError):
        to(x, return_type=str)
