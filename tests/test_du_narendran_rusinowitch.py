# Copyright (c) 2026 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""Tests for du_narendran_rusinowitch."""

# pylint: disable=missing-function-docstring

import pytest

from libsemigroups_pybind11 import (
    Alphabet,
    LibsemigroupsError,
    Presentation,
    du_narendran_rusinowitch,
    rev_rpo_cmp,
)


@pytest.mark.parametrize(
    ("alphabet", "rules", "expected"),
    [
        ("abcde", ["dbcbace", "cbbaec", "bcbad", "badbc"], "edcab"),
        (
            [0, 1, 2, 3, 4],
            [[3, 1, 2, 1, 0, 2, 4], [2, 1, 1, 0, 4, 2], [1, 2, 1, 0, 3], [1, 0, 3, 1, 2]],
            [4, 3, 2, 0, 1],
        ),
    ],
)
def test_du_narendran_rusinowitch(alphabet, rules, expected):
    p = Presentation(alphabet)
    p.rules = rules

    assert du_narendran_rusinowitch(p) == expected
    for lhs, rhs in list(zip(rules[::2], rules[1::2], strict=True)):
        assert rev_rpo_cmp(Alphabet(expected), rhs, lhs)


def test_du_narendran_rusinowitch_no_order_exists():
    p = Presentation("abcd")
    p.rules = ["a", "b", "b", "c", "c", "d", "d", "a"]

    assert du_narendran_rusinowitch(p) == ""


def test_du_narendran_rusinowitch_empty_rules():
    p = Presentation([2, 0, 1])

    assert du_narendran_rusinowitch(p) == [2, 0, 1]


def test_du_narendran_rusinowitch_invalid_presentation():
    p = Presentation("ab")
    p.rules = ["a", "c"]

    with pytest.raises(LibsemigroupsError):
        du_narendran_rusinowitch(p)
