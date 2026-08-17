# Copyright (c) 2026 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""Tests for order.cpp."""

from copy import copy

import pytest

from libsemigroups_pybind11 import (
    Alphabet,
    LenLexCmp,
    LexCmp,
    LibsemigroupsError,
    Order,
    RevRPOCmp,
    RPOCmp,
    len_wt_lex_cmp,
    lenlex_cmp,
    lex_cmp,
    lexicographical_compare,
    recursive_path_compare,
    rev_len_wt_lex_cmp,
    rev_lenlex_cmp,
    rev_lex_cmp,
    rev_rpo_cmp,
    rev_wr_cmp,
    rev_wt_lenlex_cmp,
    rev_wt_lex_cmp,
    rpo_cmp,
    shortlex_compare,
    wr_cmp,
    wt_lenlex_cmp,
    wt_lex_cmp,
)


def test_lex_cmp_object_without_alphabet():
    """Check the stateless comparison object with both word types."""
    compare = LexCmp()
    assert compare("a", "b")
    assert compare([0], [1])
    assert not compare("b", "aa")


@pytest.mark.parametrize(
    ("alphabet", "x", "y", "missing"),
    [(Alphabet("ba"), "b", "a", "c"), (Alphabet([1, 0]), [1], [0], [2])],
)
def test_lex_cmp_object_with_alphabet(alphabet, x, y, missing):
    """Check that the comparison object stores and uses its alphabet."""
    compare = LexCmp(alphabet=alphabet)
    assert compare(x, y)
    assert compare.alphabet() == alphabet
    assert compare.init(alphabet) is compare

    with pytest.raises(LibsemigroupsError):
        compare(x, missing)


def test_lex_cmp_object_rejects_bad_constructors():
    """Check that only zero arguments or one Alphabet are accepted."""
    with pytest.raises(TypeError):
        LexCmp("abc")
    with pytest.raises(TypeError):
        LexCmp(Alphabet("abc"), Alphabet("abc"))


def test_lenlex_cmp_object_without_alphabet():
    """Check the stateless comparison object with both word types."""
    compare = LenLexCmp()
    assert compare("a", "b")
    assert compare([0], [1])
    assert not compare("bb", "a")


@pytest.mark.parametrize(
    ("alphabet", "x", "y", "missing"),
    [(Alphabet("ba"), "b", "a", "c"), (Alphabet([1, 0]), [1], [0], [2])],
)
def test_lenlex_cmp_object_with_alphabet(alphabet, x, y, missing):
    """Check that the comparison object stores and uses its alphabet."""
    compare = LenLexCmp(alphabet=alphabet)
    assert compare(x, y)
    assert compare.alphabet() == alphabet
    assert compare.init(alphabet) is compare

    with pytest.raises(LibsemigroupsError):
        compare(x, missing)


def test_lenlex_cmp_object_rejects_bad_constructors():
    """Check that only zero arguments or one Alphabet are accepted."""
    with pytest.raises(TypeError):
        LenLexCmp("abc")
    with pytest.raises(TypeError):
        LenLexCmp(Alphabet("abc"), Alphabet("abc"))


def test_rpo_cmp_object_without_alphabet():
    """Check the stateless comparison object with both word types."""
    compare = RPOCmp()
    assert compare("a", "b")
    assert compare([0], [1])
    assert not compare("ab", "ba")


@pytest.mark.parametrize(
    ("alphabet", "x", "y", "missing"),
    [(Alphabet("ba"), "b", "a", "c"), (Alphabet([1, 0]), [1], [0], [2])],
)
def test_rpo_cmp_object_with_alphabet(alphabet, x, y, missing):
    """Check that the comparison object stores and uses its alphabet."""
    compare = RPOCmp(alphabet=alphabet)
    assert compare(x, y)
    assert compare.alphabet() == alphabet
    assert compare.init(alphabet) is compare

    with pytest.raises(LibsemigroupsError):
        compare(x, missing)


def test_rpo_cmp_object_rejects_bad_constructors():
    """Check that only zero arguments or one Alphabet are accepted."""
    with pytest.raises(TypeError):
        RPOCmp("abc")
    with pytest.raises(TypeError):
        RPOCmp(Alphabet("abc"), Alphabet("abc"))


def test_rev_rpo_cmp_object_without_alphabet():
    """Check the stateless comparison object with both word types."""
    compare = RevRPOCmp()
    assert compare("a", "b")
    assert compare([0], [1])
    assert not compare("ba", "ab")


@pytest.mark.parametrize(
    ("alphabet", "x", "y", "missing"),
    [(Alphabet("ba"), "b", "a", "c"), (Alphabet([1, 0]), [1], [0], [2])],
)
def test_rev_rpo_cmp_object_with_alphabet(alphabet, x, y, missing):
    """Check that the comparison object stores and uses its alphabet."""
    compare = RevRPOCmp(alphabet=alphabet)
    assert compare(x, y)
    assert compare.alphabet() == alphabet
    assert compare.init(alphabet) is compare

    with pytest.raises(LibsemigroupsError):
        compare(x, missing)


def test_rev_rpo_cmp_object_rejects_bad_constructors():
    """Check that only zero arguments or one Alphabet are accepted."""
    with pytest.raises(TypeError):
        RevRPOCmp("abc")
    with pytest.raises(TypeError):
        RevRPOCmp(Alphabet("abc"), Alphabet("abc"))


@pytest.mark.parametrize("comparison_type", [LexCmp, LenLexCmp, RPOCmp, RevRPOCmp])
@pytest.mark.parametrize(
    ("alphabet", "replacement", "x", "y"),
    [
        (None, None, "a", "b"),
        (Alphabet("ba"), Alphabet("ab"), "b", "a"),
        (Alphabet([1, 0]), Alphabet([0, 1]), [1], [0]),
    ],
)
def test_comparison_object_copy(comparison_type, alphabet, replacement, x, y):
    """Check that both copy APIs make independent copies of the C++ object."""
    original = comparison_type() if alphabet is None else comparison_type(alphabet)
    copies = (
        original.copy(),
        original.__copy__(),  # pylint: disable=unnecessary-dunder-call
        copy(original),
    )

    for copied in copies:
        assert isinstance(copied, comparison_type)
        assert copied is not original
        assert copied.py_template_params == original.py_template_params
        assert copied(x, y)
        if alphabet is not None:
            assert copied.alphabet() == alphabet

    if replacement is not None:
        original.init(replacement)
        assert not original(x, y)
        assert all(copied(x, y) for copied in copies)


@pytest.mark.parametrize(
    ("comparison_type", "name"),
    [(LexCmp, "LexCmp"), (LenLexCmp, "LenLexCmp"), (RPOCmp, "RPOCmp"), (RevRPOCmp, "RevRPOCmp")],
)
def test_comparison_object_repr(comparison_type, name):
    """Check human-readable representations with and without alphabets."""
    assert repr(comparison_type()) == f"<{name} object>"
    assert repr(comparison_type(Alphabet("ba"))) == f'<{name} object over <alphabet "ba">>'
    assert repr(comparison_type(Alphabet([1, 0]))) == f"<{name} object over <alphabet [1, 0]>>"


@pytest.mark.parametrize(
    ("compare", "x", "y"),
    [
        (lex_cmp, "ab", "ba"),
        (lex_cmp, [0, 1], [1, 0]),
        (lenlex_cmp, "ba", "aaa"),
        (lenlex_cmp, [1, 0], [0, 0, 0]),
        (rev_lex_cmp, "ab", "ba"),
        (rev_lex_cmp, [0, 1], [1, 0]),
        (rev_lenlex_cmp, "ba", "aaa"),
        (rev_lenlex_cmp, [1, 0], [0, 0, 0]),
        (rpo_cmp, "ab", "ba"),
        (rpo_cmp, [0, 1], [1, 0]),
        (rev_rpo_cmp, "ab", "ba"),
        (rev_rpo_cmp, [0, 1], [1, 0]),
    ],
)
def test_compare_without_alphabet(compare, x, y):
    """Check that each comparison function works without an alphabet."""
    assert isinstance(compare(x, y), bool)


@pytest.mark.parametrize(
    "compare", [lex_cmp, lenlex_cmp, rev_lex_cmp, rev_lenlex_cmp, rpo_cmp, rev_rpo_cmp]
)
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


def test_wr_cmp_for_integer_words():
    """Check wreath-product comparison, validation, and its unchecked form."""
    levels = [0, 0, 1]
    x = [0, 2]
    y = [1, 2]

    assert wr_cmp(levels, x, y)
    assert not wr_cmp(levels, y, x)

    with pytest.raises(LibsemigroupsError):
        wr_cmp([0, 1], [0, 2], [0, 1])


def test_wr_cmp_with_alphabet():
    """Check wreath-product comparison over an explicitly ordered alphabet."""
    alphabet = Alphabet("bac")
    levels = [1, 1, 0]

    assert wr_cmp(alphabet, levels, "cbcc", "ccbc")
    assert wr_cmp(alphabet, levels, "ac", "ca")

    word_alphabet = Alphabet([1, 0])
    assert wr_cmp(word_alphabet, [0, 0], [1], [0])

    with pytest.raises(LibsemigroupsError):
        wr_cmp(alphabet, levels, "d", "b")


@pytest.mark.parametrize("compare", [wt_lenlex_cmp, wt_lex_cmp])
def test_weighted_comparisons_for_integer_words(compare):
    """Check weighted comparisons and validation for integer words."""
    weights = [2, 1, 6]

    assert compare(weights, [0, 1], [2])
    assert not compare(weights, [2], [0, 1])

    with pytest.raises(LibsemigroupsError):
        compare(weights, [0], [3])

    assert compare(weights, chr(0) + chr(1), chr(2))
    assert not compare(weights, chr(2), chr(0) + chr(1))

    with pytest.raises(LibsemigroupsError):
        compare(weights, chr(0), chr(3))


def test_weighted_comparisons_use_different_tie_breakers():
    """Check the length and lexicographic tie breakers differ."""
    weights = [1, 2]

    assert wt_lenlex_cmp(weights, [1], [0, 0])
    assert not wt_lex_cmp(weights, [1], [0, 0])


@pytest.mark.parametrize("compare", [wt_lenlex_cmp, wt_lex_cmp])
def test_weighted_comparisons_with_alphabet(compare):
    """Check weighted comparisons over an explicitly ordered alphabet."""
    alphabet = Alphabet("ba")

    assert compare(alphabet, [10, 1], "a", "b")
    assert compare(alphabet, [1, 1], "b", "a")

    word_alphabet = Alphabet([1, 0])
    assert compare(word_alphabet, [1, 1], [1], [0])

    with pytest.raises(LibsemigroupsError):
        compare(alphabet, [1, 1], "c", "a")

    with pytest.raises(LibsemigroupsError):
        compare(alphabet, [1], "a", "b")


@pytest.mark.parametrize(
    ("compare", "parameters"),
    [
        (rev_wr_cmp, [0, 0]),
        (rev_wt_lenlex_cmp, [1, 1]),
        (rev_wt_lex_cmp, [1, 1]),
        (rev_len_wt_lex_cmp, [1, 1]),
    ],
)
def test_parameterized_reverse_comparisons(compare, parameters):
    """Check parameterized reverse orders for both supported word types."""
    assert compare(parameters, [1, 0], [0, 1])
    assert compare(Alphabet("ab"), parameters, "ba", "ab")

    with pytest.raises(LibsemigroupsError):
        compare(parameters, [2], [0, 1])

    with pytest.raises(LibsemigroupsError):
        compare(Alphabet("ab"), parameters, "c", "ab")


def test_reverse_weighted_comparisons_use_the_expected_priorities():
    """Check the weight and length priorities of the reverse weighted orders."""
    weights = [1, 2]

    assert rev_wt_lenlex_cmp(weights, [1], [0, 0])
    assert not rev_wt_lex_cmp(weights, [1], [0, 0])

    weights = [100, 1]
    assert rev_len_wt_lex_cmp(weights, [0], [1, 1])
    assert not rev_wt_lex_cmp(weights, [0], [1, 1])


def test_len_wt_lex_cmp():
    """Check length-before-weighted-lex ordering and validation."""
    weights = [1, 1]

    assert len_wt_lex_cmp(weights, [0, 1], [1, 0])
    assert len_wt_lex_cmp(Alphabet("ab"), weights, "ab", "ba")

    with pytest.raises(LibsemigroupsError):
        len_wt_lex_cmp(weights, [2], [0, 1])

    with pytest.raises(LibsemigroupsError):
        len_wt_lex_cmp(Alphabet("ab"), weights, "c", "ab")

    weights = [100, 1]
    assert len_wt_lex_cmp(weights, [0], [1, 1])
