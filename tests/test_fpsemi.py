# -*- coding: utf-8 -*-

# Copyright (c) 2021, J. D. Mitchell + Maria Tsalakou
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This file contains tests for FpSemigroup from libsemigroups_pybind11.
"""

# pylint: disable=no-name-in-module, missing-function-docstring, invalid-name

from fpsemi_intf import (
    check_attributes,
    check_converters,
    check_initialisation,
    check_operators,
    check_running_and_state,
    check_validation,
)
from libsemigroups_pybind11 import (
    POSITIVE_INFINITY,
    FpSemigroup,
    FroidurePin,
    ReportGuard,
    Transf,
)


def test_running_and_state():
    check_running_and_state(FpSemigroup)


def test_all():
    ReportGuard(False)
    S = FpSemigroup()
    S.set_alphabet("abcde")
    S.set_identity("e")
    S.add_rule("cacac", "aacaa")
    S.add_rule("acaca", "ccacc")
    S.add_rule("ada", "bbcbb")
    S.add_rule("bcb", "aadaa")
    S.add_rule("aaaa", "e")
    S.add_rule("ab", "e")
    S.add_rule("ba", "e")
    S.add_rule("cd", "e")
    S.add_rule("dc", "e")
    S.run()

    assert S.number_of_rules() == 18
    assert S.equal_to(
        "abbbbbbbbbbbadddddddddddddddacccccccccccc",
        "aaccccccccccccccccccccccccccc",
    )

    assert not S.equal_to("abbbbbbbbbbbadddddddddddddddacccccccccccc", "a")


def test_constructors():
    S = FroidurePin(Transf([1, 0, 1]), Transf([0, 0, 0]))
    T = FpSemigroup(S)
    assert not T.has_froidure_pin()
    assert S.number_of_rules() == T.number_of_rules()
    assert list(S.rules()) == [
        ([0, 1], [1]),
        ([1, 1], [1]),
        ([0, 0, 0], [0]),
        ([1, 0, 0], [1]),
    ]
    assert list(T.rules()) == [
        ("\x01\x02", "\x02"),
        ("\x02\x02", "\x02"),
        ("\x01\x01\x01", "\x01"),
        ("\x02\x01\x01", "\x02"),
    ]

    assert T.size() == S.size()


# Common tests for FpSemigroupInterface derived classes


def test_validation():
    check_validation(FpSemigroup)


def test_converters():
    check_converters(FpSemigroup)


def test_initialisation():
    check_initialisation(FpSemigroup)


def test_attributes():
    check_attributes(FpSemigroup)


def test_operators():
    check_operators(FpSemigroup)


def test_related_algorithms():
    ReportGuard(False)
    S = FpSemigroup()
    S.set_alphabet("abcde")
    S.set_identity("e")
    S.add_rule("cacac", "aacaa")
    S.add_rule("acaca", "ccacc")
    S.add_rule("ada", "bbcbb")
    S.add_rule("bcb", "aadaa")
    S.add_rule("aaaa", "e")
    S.add_rule("ab", "e")
    S.add_rule("ba", "e")
    S.add_rule("cd", "e")
    S.add_rule("dc", "e")

    assert S.size() == POSITIVE_INFINITY
    assert S.has_knuth_bendix()
    assert not S.has_todd_coxeter()
    #  S is infinite and so it always has a KnuthBendix
    S.knuth_bendix().run()
    assert S.knuth_bendix().finished()
    assert S.knuth_bendix().active_rules() == [
        ("ab", "e"),
        ("ae", "a"),
        ("ba", "e"),
        ("bb", "aa"),
        ("be", "b"),
        ("ca", "bd"),
        ("cb", "ad"),
        ("cd", "e"),
        ("ce", "c"),
        ("da", "bc"),
        ("db", "ac"),
        ("dc", "e"),
        ("de", "d"),
        ("ea", "a"),
        ("eb", "b"),
        ("ec", "c"),
        ("ed", "d"),
        ("ee", "e"),
        ("aaa", "b"),
    ]
    assert list(S.rules()) == list(S.knuth_bendix().rules())

    S = FpSemigroup()
    S.set_alphabet("01")

    S.add_rule("000", "0")
    S.add_rule("1111", "1")
    S.add_rule("01110", "00")
    S.add_rule("1001", "11")
    S.add_rule("001010101010", "00")

    assert S.size() == 240
    assert not S.has_froidure_pin()
    S.froidure_pin()
