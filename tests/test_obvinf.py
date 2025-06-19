# -*- coding: utf-8 -*-
# Copyright (c) 2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for the functions is_obviously_infinite.
"""

# pylint: disable=missing-function-docstring

from libsemigroups_pybind11 import (
    Presentation,
    presentation,
    is_obviously_infinite,
    congruence_kind,
    KnuthBendix,
    ReportGuard,
    to,
)


def test_is_obviously_infinite_presentation():
    p = Presentation([0, 1])
    presentation.add_rule(p, [0], [0, 1, 1])
    presentation.add_rule(p, [1], [1, 0, 0])

    assert not is_obviously_infinite(p)

    p = Presentation([0, 1, 2, 3, 4])
    presentation.add_rule(p, [0], [0] * 2)
    presentation.add_rule(p, [1], [1] * 2)
    presentation.add_rule(p, [0, 1, 4], [4] * 3)
    presentation.add_rule(p, [3, 2], [2])
    presentation.add_rule(p, [2, 2], [3, 3, 3])

    assert is_obviously_infinite(p)

    p = to(p, lambda x: "abcde"[x], rtype=(Presentation, str))
    assert is_obviously_infinite(p)


def test_is_obviously_infinite_knuth_bendix():
    ReportGuard(False)

    p = Presentation("abABe")
    p.contains_empty_word(True)
    presentation.add_identity_rules(p, "e")
    presentation.add_inverse_rules(p, "ABabe", "e")
    presentation.add_rule(p, "aaa", "")
    presentation.add_rule(p, "bbb", "")
    presentation.add_rule(p, "abababab", "")
    presentation.add_rule(p, "aBaBaBaBaB", "")

    assert not is_obviously_infinite(p)

    kb = KnuthBendix(congruence_kind.twosided, p)

    assert not is_obviously_infinite(kb)
    assert kb.number_of_classes() == 1080
    assert not is_obviously_infinite(kb)
