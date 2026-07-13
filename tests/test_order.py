# Copyright (c) 2026 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""Tests for order.cpp."""

import pytest

from libsemigroups_pybind11 import (
    Alphabet,
    LibsemigroupsError,
    Order,
    lenlex_cmp,
    lex_cmp,
    lexicographical_compare,
    recursive_path_compare,
    rev_rpo_cmp,
    rpo_cmp,
    shortlex_compare,
)


@pytest.mark.parametrize(
    ("compare", "x", "y"),
    [
        (lex_cmp, "ab", "ba"),
        (lex_cmp, [0, 1], [1, 0]),
        (lenlex_cmp, "ba", "aaa"),
        (lenlex_cmp, [1, 0], [0, 0, 0]),
        (rpo_cmp, "ab", "ba"),
        (rpo_cmp, [0, 1], [1, 0]),
        (rev_rpo_cmp, "ab", "ba"),
        (rev_rpo_cmp, [0, 1], [1, 0]),
    ],
)
def test_compare_without_alphabet(compare, x, y):
    """Check that each comparison function works without an alphabet."""
    assert isinstance(compare(x, y), bool)


@pytest.mark.parametrize("compare", [lex_cmp, lenlex_cmp, rpo_cmp, rev_rpo_cmp])
@pytest.mark.parametrize(
    ("alphabet", "x", "y", "missing"),
    [(Alphabet("ba"), "ba", "ab", "c"), (Alphabet([1, 0]), [1, 0], [0, 1], [2])],
)
def test_compare_with_alphabet(compare, alphabet, x, y, missing):
    """Check alphabet-aware comparisons and reject unknown letters."""
    assert isinstance(compare(alphabet, x, y), bool)

    with pytest.raises(LibsemigroupsError):
        compare(alphabet, x, x + missing)


def test_order_values_and_backwards_compatible_aliases():
    """Check the new order values and their backwards-compatible aliases."""
    assert Order.shortlex == Order.lenlex
    assert Order.recursive == Order.rev_rpo
    assert Order.rpo != Order.rev_rpo


@pytest.mark.parametrize(
    ("old_compare", "new_compare"),
    [
        (lexicographical_compare, lex_cmp),
        (shortlex_compare, lenlex_cmp),
        (recursive_path_compare, rev_rpo_cmp),
    ],
)
def test_deprecated_comparisons(old_compare, new_compare):
    """Check that legacy comparisons warn and delegate to their replacements."""
    with pytest.deprecated_call():
        result = old_compare("ab", "ba")
    assert result == new_compare("ab", "ba")
