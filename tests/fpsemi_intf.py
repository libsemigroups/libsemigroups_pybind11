# -*- coding: utf-8 -*-

# Copyright (c) 2021, J. D. Mitchell + Maria Tsalakou
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some functions used in tests for FpSemigroupInterface
derived classes, i.e. KnuthBendix, FpSemigroup, etc.
"""

# pylint: disable=no-name-in-module, missing-function-docstring, invalid-name

from datetime import timedelta
import pytest
from runner import check_runner
from libsemigroups_pybind11 import ReportGuard #, FroidurePin, Transf


def check_validation(t):
    ReportGuard(False)
    x = t()
    x.set_alphabet("ab")

    with pytest.raises(RuntimeError):
        x.validate_letter("c")
    try:
        x.validate_letter("a")
    except RuntimeError as e:
        pytest.fail(
            "unexpected exception raised for FpSemigroupInterface::validate_letter: "
            + e
        )

    with pytest.raises(RuntimeError):
        x.validate_letter(3)
    try:
        x.validate_letter(0)
    except RuntimeError as e:
        pytest.fail(
            "unexpected exception raised for FpSemigroupInterface::validate_letter: "
            + e
        )
    with pytest.raises(RuntimeError):
        x.validate_word("abc")
    try:
        x.validate_word("abab")
    except RuntimeError as e:
        pytest.fail(
            "unexpected exception raised for FpSemigroupInterface::validate_letter: "
            + e
        )

    with pytest.raises(RuntimeError):
        x.validate_word([0, 1, 2])

    try:
        x.validate_word([0, 1, 0, 1])
    except RuntimeError as e:
        pytest.fail(
            "unexpected exception raised for FpSemigroupInterface::validate_letter: "
            + e
        )


def check_converters(t):
    ReportGuard(False)
    x = t()
    x.set_alphabet("ba")
    assert x.char_to_uint("a") == 1
    assert x.char_to_uint("b") == 0

    assert x.string_to_word("ab") == [1, 0]
    assert x.string_to_word("aaaaaa") == [1] * 6
    with pytest.raises(RuntimeError):
        x.string_to_word("c")

    assert x.uint_to_char(0) == "b"
    assert x.uint_to_char(1) == "a"
    with pytest.raises(RuntimeError):
        x.uint_to_char(2)

    assert x.word_to_string([1, 0]) == "ab"
    assert x.word_to_string([1] * 6) == "a" * 6
    with pytest.raises(RuntimeError):
        x.word_to_string([2])


def check_initialisation(t):
    ReportGuard(False)
    x = t()
    x.set_alphabet("ba")
    x.add_rule([0, 1], [1, 0])

    with pytest.raises(RuntimeError):
        x.add_rule([0, 1], [2])

    S = FroidurePin([Transf([1, 2, 0]), Transf([1, 0, 2])])
    S.run()
    x.add_rules(S)
    assert x.size() == 2

    x = t()
    x.set_alphabet("abBe")
    x.set_identity("e")

    x.set_inverses("aBbe")

    x.add_rule("bb", "B")
    x.add_rule("BaBa", "abab")
    assert x.size() == 24

    x = t()
    x.set_alphabet(1)
    x.set_identity(0)
    assert x.size() == 1


def check_attributes(t):
    ReportGuard(False)
    x = t()
    x.set_alphabet("abBe")
    x.set_identity("e")
    x.set_inverses("aBbe")

    x.add_rule("bb", "B")
    x.add_rule("BaBa", "abab")
    x.run()

    assert list(x.rules()) == [
        ("ae", "a"),
        ("ea", "a"),
        ("be", "b"),
        ("eb", "b"),
        ("Be", "B"),
        ("eB", "B"),
        ("ee", "e"),
        ("aa", "e"),
        ("bB", "e"),
        ("Bb", "e"),
        ("bb", "B"),
        ("BaBa", "abab"),
    ]
    assert x.number_of_rules() == 12

    assert x.alphabet() == "abBe"
    assert not x.has_froidure_pin()
    assert x.froidure_pin().size() == 24
    assert x.identity() == "e"
    assert x.inverses() == "aBbe"
    assert not x.is_obviously_infinite()
    assert x.is_obviously_finite()
    assert x.size() == 24


def check_operators(t):
    x = t()
    x.set_alphabet("abBe")
    x.set_identity("e")
    x.set_inverses("aBbe")

    x.add_rule("bb", "B")
    x.add_rule("BaBa", "abab")
    x.run()
    assert x.equal_to("bb", "B")
    assert x.equal_to([1, 1], [2])

    with pytest.raises(RuntimeError):
        x.equal_to([1, 1], [5])

    with pytest.raises(RuntimeError):
        x.equal_to("aa", "z")

    assert x.normal_form("bb") == "B"
    assert x.normal_form("B") == "B"
    assert x.normal_form([1, 1]) == [2]
    assert x.normal_form([0, 0]) == [3]

    with pytest.raises(RuntimeError):
        x.normal_form([1, 5])
    with pytest.raises(RuntimeError):
        x.normal_form("z")

    if hasattr(t, "rewrite"):
        assert x.rewrite("aa") == "e"
        assert x.rewrite("bb") == "B"

        with pytest.raises(RuntimeError):
            x.rewrite("z")


def check_running_and_state(T):
    ReportGuard(False)
    x = T()
    x.set_alphabet("abce")
    x.set_identity("e")
    x.add_rule("aa", "e")
    x.add_rule("bc", "e")
    x.add_rule("bbb", "e")
    x.add_rule("ababababababab", "e")
    x.add_rule("abacabacabacabacabacabacabacabac", "e")
    check_runner(x, timedelta(microseconds=1000))
