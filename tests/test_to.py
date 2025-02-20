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
    Transf,
    WordGraph,
    ReportGuard,
)

ReportGuard(False)


def construct_from_pres(ReturnType, Word, **kwargs):
    if Word != str:
        p = Presentation([0, 1])
        presentation.add_rule(p, [0, 1], [1, 0])
        presentation.add_rule(p, [0, 0], [0])
        presentation.add_rule(p, [1, 1], [1])
        return ReturnType(congruence_kind.twosided, p, **kwargs)
    else:
        p = Presentation("ab")
        presentation.add_rule(p, "ab", "ba")
        presentation.add_rule(p, "aa", "a")
        presentation.add_rule(p, "bb", "b")
        return ReturnType(congruence_kind.twosided, p, **kwargs)


def check_cong_to_froidure_pin(Type, Word, **kwargs):
    thing = construct_from_pres(Type, Word, **kwargs)
    fp = to(thing, return_type=FroidurePin)
    fp.run()
    assert fp.is_finite()
    assert fp.number_of_idempotents() == 3


def check_cong_to_todd_coxeter(Type, Word, **kwargs):
    thing = construct_from_pres(Type, Word, **kwargs)
    tc = to(congruence_kind.twosided, thing, return_type=ToddCoxeter)
    tc.run()
    assert tc.number_of_classes() == 3


def test_to_000():
    check_cong_to_froidure_pin(KnuthBendix, str, Rewriter="RewriteFromLeft")


def test_to_001():
    check_cong_to_froidure_pin(KnuthBendix, str, Rewriter="RewriteTrie")


def test_to_002():
    check_cong_to_froidure_pin(KnuthBendix, int, Rewriter="RewriteFromLeft")


def test_to_003():
    check_cong_to_froidure_pin(KnuthBendix, int, Rewriter="RewriteTrie")


def test_to_004():
    check_cong_to_froidure_pin(ToddCoxeter, str)


def test_to_005():
    check_cong_to_froidure_pin(ToddCoxeter, int)


def test_to_006():
    check_cong_to_froidure_pin(Congruence, str)


def test_to_007():
    check_cong_to_froidure_pin(Congruence, int)


# TODO why does this cause a segfault?
# def test_to_008():
#     k = Kambites(Word=str)
#     assert isinstance(to(k, return_type=FroidurePin), FroidurePin)


def test_to_009():
    p = Presentation("abcdefg")
    presentation.add_rule(p, "abcd", "aaaeaa")
    presentation.add_rule(p, "ef", "dg")
    k = Kambites(congruence_kind.twosided, p)

    fp = to(k, return_type=FroidurePin)
    fp.enumerate(100)
    assert fp.current_size() == 8205


def test_to_010():
    p = Presentation([0, 1, 2, 3, 4, 5, 6])
    presentation.add_rule(p, [0, 1, 2, 3], [0, 0, 0, 4, 0, 0])
    presentation.add_rule(p, [4, 5], [3, 6])
    k = Kambites(congruence_kind.twosided, p)

    fp = to(k, return_type=FroidurePin)
    fp.enumerate(100)
    assert fp.current_size() == 8205


def test_to_011():
    p = Presentation("abcdefg")
    presentation.add_rule(p, "abcd", "aaaeaa")
    presentation.add_rule(p, "ef", "dg")
    # Kambites wins here, so this checks FroidurePinKEString
    c = Congruence(congruence_kind.twosided, p)

    fp = to(c, return_type=FroidurePin)
    fp.enumerate(100)
    assert fp.current_size() == 8205


def test_to_012():
    w = WordGraph(3, 1)
    w.target(0, 0, 1)
    w.target(1, 0, 1)
    w.target(2, 0, 1)

    fp = to(w, return_type=FroidurePin)
    fp.run()
    assert fp.number_of_rules() == 1


def test_to_013():
    w = WordGraph(3, 1)
    w.target(0, 0, 1)
    w.target(1, 0, 1)
    w.target(2, 0, 1)

    fp = to(w, return_type=FroidurePin)
    fp.run()
    assert fp.number_of_rules() == 1


def test_to_014():
    w = WordGraph(3, 1)
    w.target(0, 0, 1)
    w.target(1, 0, 1)
    w.target(2, 0, 1)

    fp = to(w, 1, 2, return_type=FroidurePin)
    fp.run()
    assert fp.number_of_rules() == 1


def test_to_015():
    check_cong_to_todd_coxeter(KnuthBendix, str, Rewriter="RewriteFromLeft")


def test_to_016():
    check_cong_to_todd_coxeter(KnuthBendix, str, Rewriter="RewriteTrie")


def test_to_017():
    check_cong_to_todd_coxeter(KnuthBendix, int, Rewriter="RewriteFromLeft")


def test_to_018():
    check_cong_to_todd_coxeter(KnuthBendix, int, Rewriter="RewriteTrie")


# TODO make this work
# def test_to_019():
#     S = FroidurePin(Transf([1, 3, 4, 2, 3]), Transf([3, 2, 1, 3, 3]))
#     tc = to(
#         congruence_kind.twosided,
#         S,
#         S.right_cayley_graph(),
#         return_type=ToddCoxeter,
#     )
#     assert tc.current_word_graph().number_of_nodes() == S.size() + 1


def test_to_999():
    x = 10
    with pytest.raises(TypeError):
        to(x, return_type=FroidurePin)
    with pytest.raises(TypeError):
        to(x, return_type=ToddCoxeter)
    with pytest.raises(TypeError):
        to(x, return_type=str)
