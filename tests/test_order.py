# Copyright (c) 2026 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=too-many-lines, invalid-name

"""Tests for order.cpp."""

from copy import copy

import pytest

from libsemigroups_pybind11 import (
    Alphabet,
    LenLexCmp,
    LenWtLexCmp,
    LexCmp,
    LibsemigroupsError,
    Order,
    RevLenLexCmp,
    RevLenWtLexCmp,
    RevLexCmp,
    RevRPOCmp,
    RevWrCmp,
    RevWtLenLexCmp,
    RevWtLexCmp,
    RPOCmp,
    WrCmp,
    WtLenLexCmp,
    WtLexCmp,
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


def _check_simple_copy_and_repr(Cmp, name):
    """Check copies of alphabet-free and alphabet-aware objects."""
    for original, alphabet, replacement, x, y in (
        (Cmp(), None, None, "a", "b"),
        (Cmp(Alphabet("ba")), Alphabet("ba"), Alphabet("ab"), "b", "a"),
        (Cmp(Alphabet([1, 0])), Alphabet([1, 0]), Alphabet([0, 1]), [1], [0]),
    ):
        copies = (
            original.copy(),
            original.__copy__(),  # pylint: disable=unnecessary-dunder-call
            copy(original),
        )
        assert all(isinstance(copied, Cmp) for copied in copies)
        assert all(copied is not original for copied in copies)
        assert all(copied.py_template_params == original.py_template_params for copied in copies)
        assert all(copied(x, y) for copied in copies)
        if alphabet is not None:
            assert all(copied.alphabet() == alphabet for copied in copies)

        if replacement is not None:
            original.init(replacement)
            assert not original(x, y)
            assert all(copied(x, y) for copied in copies)

    assert repr(Cmp()) == f"<{name} object>"
    assert repr(Cmp(Alphabet("ba"))) == (f'<{name} object over <alphabet "ba">>')
    assert repr(Cmp(Alphabet([1, 0]))) == (f"<{name} object over <alphabet [1, 0]>>")


def _check_configured_copy_and_repr(Cmp, name, configuration_name, accessor_name):
    """Check copies of configured alphabet-free and alphabet-aware objects."""
    for original, alphabet, replacement_args in (
        (Cmp([1, 2]), None, ([2, 1],)),
        (Cmp(Alphabet("ab"), [1, 2]), Alphabet("ab"), (Alphabet("ab"), [2, 1])),
        (Cmp(Alphabet([0, 1]), [1, 2]), Alphabet([0, 1]), (Alphabet([0, 1]), [2, 1])),
    ):
        copies = (
            original.copy(),
            original.__copy__(),  # pylint: disable=unnecessary-dunder-call
            copy(original),
        )
        original.init(*replacement_args)
        assert all(isinstance(copied, Cmp) for copied in copies)
        assert all(copied is not original for copied in copies)
        assert all(getattr(copied, accessor_name)() == [1, 2] for copied in copies)
        if alphabet is not None:
            assert all(copied.alphabet() == alphabet for copied in copies)
        assert getattr(original, accessor_name)() == [2, 1]

    assert repr(Cmp([1, 2])) == (f"<{name} object with {configuration_name} [1, 2]>")
    assert repr(Cmp(Alphabet("ab"), [1, 2])) == (
        f'<{name} object over <alphabet "ab"> with {configuration_name} [1, 2]>'
    )
    assert repr(Cmp(Alphabet([0, 1]), [1, 2])) == (
        f"<{name} object over <alphabet [0, 1]> with {configuration_name} [1, 2]>"
    )


def _check_simple_bad_constructor_arguments(Cmp):
    """Check constructor and zero-argument init validation."""
    with pytest.raises(TypeError):
        Cmp("abc")
    with pytest.raises(TypeError):
        Cmp(Alphabet("abc"), None)
    with pytest.raises((AttributeError, TypeError)):
        Cmp().init()


def _check_configured_bad_constructor_arguments(Cmp):
    """Check constructor and zero-argument init validation."""
    with pytest.raises(TypeError):
        Cmp(Alphabet("ab"))
    with pytest.raises(LibsemigroupsError):
        Cmp(Alphabet("ab"), [1])
    with pytest.raises((AttributeError, TypeError)):
        Cmp().init()


###############################################################################
# Lex order
###############################################################################


def test_lex_without_alphabet():
    """Check lexicographic comparison without an alphabet."""
    assert lex_cmp("ab", "ba")
    assert lex_cmp([0, 1], [1, 0])

    cmp = LexCmp()
    assert cmp("a", "b")
    assert cmp([0], [1])
    assert not cmp("b", "aa")


def test_lex_with_alphabet():
    """Check lexicographic comparison with an alphabet."""
    alphabet = Alphabet("cba")
    x = "cba"
    y = "cab"
    assert lex_cmp(alphabet, x, y)

    cmp = LexCmp(alphabet=alphabet)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.init(alphabet) is cmp

    with pytest.raises(LibsemigroupsError):
        lex_cmp(alphabet, x, "d")
    with pytest.raises(LibsemigroupsError):
        cmp(x, "d")

    alphabet = Alphabet([4, 2, 7, 1])
    x = [4, 7]
    y = [2, 4]
    assert lex_cmp(alphabet, x, y)

    cmp = LexCmp(alphabet)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.init(alphabet) is cmp

    with pytest.raises(LibsemigroupsError):
        lex_cmp(alphabet, x, [9])
    with pytest.raises(LibsemigroupsError):
        cmp(x, [9])


def test_lex_object_copy_and_repr():
    """Check copies and human-readable representations."""
    _check_simple_copy_and_repr(LexCmp, "LexCmp")


def test_lex_object_rejects_bad_constructor_arguments():
    """Check constructor validation."""
    _check_simple_bad_constructor_arguments(LexCmp)


###############################################################################
# RevLex order
###############################################################################


def test_rev_lex_without_alphabet():
    """Check reverse lexicographic comparison without an alphabet."""
    assert rev_lex_cmp("ba", "ab")
    assert rev_lex_cmp([1, 0], [0, 1])

    cmp = RevLexCmp()
    assert cmp("a", "b")
    assert cmp([0], [1])
    assert cmp("ba", "ab")


def test_rev_lex_with_alphabet():
    """Check reverse lexicographic comparison with an alphabet."""
    alphabet = Alphabet("qpr")
    x = "rq"
    y = "rp"
    assert rev_lex_cmp(alphabet, x, y)

    cmp = RevLexCmp(alphabet=alphabet)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.init(alphabet) is cmp

    with pytest.raises(LibsemigroupsError):
        rev_lex_cmp(alphabet, x, "s")
    with pytest.raises(LibsemigroupsError):
        cmp(x, "s")

    alphabet = Alphabet([8, 3, 5])
    x = [5, 8]
    y = [5, 3]
    assert rev_lex_cmp(alphabet, x, y)

    cmp = RevLexCmp(alphabet)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.init(alphabet) is cmp

    with pytest.raises(LibsemigroupsError):
        rev_lex_cmp(alphabet, x, [9])
    with pytest.raises(LibsemigroupsError):
        cmp(x, [9])


def test_rev_lex_object_copy_and_repr():
    """Check copies and human-readable representations."""
    _check_simple_copy_and_repr(RevLexCmp, "RevLexCmp")


def test_rev_lex_object_rejects_bad_constructor_arguments():
    """Check constructor validation."""
    _check_simple_bad_constructor_arguments(RevLexCmp)


###############################################################################
# LenLex order
###############################################################################


def test_lenlex_without_alphabet():
    """Check length-lexicographic comparison without an alphabet."""
    assert lenlex_cmp("ba", "aaa")
    assert lenlex_cmp([1, 0], [0, 0, 0])

    cmp = LenLexCmp()
    assert cmp("a", "b")
    assert cmp([0], [1])
    assert not cmp("bb", "a")


def test_lenlex_with_alphabet():
    """Check length-lexicographic comparison with an alphabet."""
    alphabet = Alphabet("yxz")
    x = "zy"
    y = "zx"
    assert lenlex_cmp(alphabet, x, y)

    cmp = LenLexCmp(alphabet=alphabet)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.init(alphabet) is cmp

    with pytest.raises(LibsemigroupsError):
        lenlex_cmp(alphabet, x, "w")
    with pytest.raises(LibsemigroupsError):
        cmp(x, "w")

    alphabet = Alphabet([6, 2, 9])
    x = [9, 6]
    y = [9, 2]
    assert lenlex_cmp(alphabet, x, y)

    cmp = LenLexCmp(alphabet)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.init(alphabet) is cmp

    with pytest.raises(LibsemigroupsError):
        lenlex_cmp(alphabet, x, [4])
    with pytest.raises(LibsemigroupsError):
        cmp(x, [4])


def test_lenlex_object_copy_and_repr():
    """Check copies and human-readable representations."""
    _check_simple_copy_and_repr(LenLexCmp, "LenLexCmp")


def test_lenlex_object_rejects_bad_constructor_arguments():
    """Check constructor validation."""
    _check_simple_bad_constructor_arguments(LenLexCmp)


###############################################################################
# RevLenLex order
###############################################################################


def test_rev_lenlex_without_alphabet():
    """Check reverse length-lexicographic comparison without an alphabet."""
    assert rev_lenlex_cmp("ba", "ab")
    assert rev_lenlex_cmp([1, 0], [0, 1])

    cmp = RevLenLexCmp()
    assert cmp("a", "b")
    assert cmp([0], [1])
    assert cmp("ba", "ab")


def test_rev_lenlex_with_alphabet():
    """Check reverse length-lexicographic comparison with an alphabet."""
    alphabet = Alphabet("mkn")
    x = "nm"
    y = "nk"
    assert rev_lenlex_cmp(alphabet, x, y)

    cmp = RevLenLexCmp(alphabet=alphabet)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.init(alphabet) is cmp

    with pytest.raises(LibsemigroupsError):
        rev_lenlex_cmp(alphabet, x, "p")
    with pytest.raises(LibsemigroupsError):
        cmp(x, "p")

    alphabet = Alphabet([7, 4, 8])
    x = [8, 7]
    y = [8, 4]
    assert rev_lenlex_cmp(alphabet, x, y)

    cmp = RevLenLexCmp(alphabet)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.init(alphabet) is cmp

    with pytest.raises(LibsemigroupsError):
        rev_lenlex_cmp(alphabet, x, [3])
    with pytest.raises(LibsemigroupsError):
        cmp(x, [3])


def test_rev_lenlex_object_copy_and_repr():
    """Check copies and human-readable representations."""
    _check_simple_copy_and_repr(RevLenLexCmp, "RevLenLexCmp")


def test_rev_lenlex_object_rejects_bad_constructor_arguments():
    """Check constructor validation."""
    _check_simple_bad_constructor_arguments(RevLenLexCmp)


###############################################################################
# RPO order
###############################################################################


def test_rpo_without_alphabet():
    """Check recursive-path comparison without an alphabet."""
    assert rpo_cmp("a", "b")
    assert rpo_cmp([0], [1])

    cmp = RPOCmp()
    assert cmp("a", "b")
    assert cmp([0], [1])
    assert not cmp("ab", "ba")


def test_rpo_with_alphabet():
    """Check recursive-path comparison with an alphabet."""
    alphabet = Alphabet("zxy")
    x = "z"
    y = "x"
    assert rpo_cmp(alphabet, x, y)

    cmp = RPOCmp(alphabet=alphabet)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.init(alphabet) is cmp

    with pytest.raises(LibsemigroupsError):
        rpo_cmp(alphabet, x, "w")
    with pytest.raises(LibsemigroupsError):
        cmp(x, "w")

    alphabet = Alphabet([8, 3, 5])
    x = [8]
    y = [3]
    assert rpo_cmp(alphabet, x, y)

    cmp = RPOCmp(alphabet)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.init(alphabet) is cmp

    with pytest.raises(LibsemigroupsError):
        rpo_cmp(alphabet, x, [1])
    with pytest.raises(LibsemigroupsError):
        cmp(x, [1])


def test_rpo_object_copy_and_repr():
    """Check copies and human-readable representations."""
    _check_simple_copy_and_repr(RPOCmp, "RPOCmp")


def test_rpo_object_rejects_bad_constructor_arguments():
    """Check constructor validation."""
    _check_simple_bad_constructor_arguments(RPOCmp)


###############################################################################
# RevRPO order
###############################################################################


def test_rev_rpo_without_alphabet():
    """Check reverse recursive-path comparison without an alphabet."""
    assert rev_rpo_cmp("a", "b")
    assert rev_rpo_cmp([0], [1])

    cmp = RevRPOCmp()
    assert cmp("a", "b")
    assert cmp([0], [1])
    assert not cmp("ba", "ab")


def test_rev_rpo_with_alphabet():
    """Check reverse recursive-path comparison with an alphabet."""
    alphabet = Alphabet("qsr")
    x = "q"
    y = "s"
    assert rev_rpo_cmp(alphabet, x, y)

    cmp = RevRPOCmp(alphabet=alphabet)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.init(alphabet) is cmp

    with pytest.raises(LibsemigroupsError):
        rev_rpo_cmp(alphabet, x, "t")
    with pytest.raises(LibsemigroupsError):
        cmp(x, "t")

    alphabet = Alphabet([6, 4, 9])
    x = [6]
    y = [4]
    assert rev_rpo_cmp(alphabet, x, y)

    cmp = RevRPOCmp(alphabet)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.init(alphabet) is cmp

    with pytest.raises(LibsemigroupsError):
        rev_rpo_cmp(alphabet, x, [2])
    with pytest.raises(LibsemigroupsError):
        cmp(x, [2])


def test_rev_rpo_object_copy_and_repr():
    """Check copies and human-readable representations."""
    _check_simple_copy_and_repr(RevRPOCmp, "RevRPOCmp")


def test_rev_rpo_object_rejects_bad_constructor_arguments():
    """Check constructor validation."""
    _check_simple_bad_constructor_arguments(RevRPOCmp)


###############################################################################
# Wr order
###############################################################################


def test_wr_without_alphabet():
    """Check wreath-product comparison without an alphabet."""
    levels = [0, 0, 1]
    x = [0, 2]
    y = [1, 2]
    assert wr_cmp(levels, x, y)
    assert not wr_cmp(levels, y, x)
    with pytest.raises(LibsemigroupsError):
        wr_cmp([0, 1], [0, 2], [0, 1])

    cmp = WrCmp(levels)
    assert cmp(x, y)
    assert cmp("\x00\x02", "\x01\x02")
    assert cmp.levels() == levels
    assert cmp.init([1, 1, 0]) is cmp
    assert cmp.levels() == [1, 1, 0]
    with pytest.raises(LibsemigroupsError):
        cmp([3], [0])
    with pytest.raises(LibsemigroupsError):
        cmp("\x03", "\x00")
    with pytest.raises(TypeError):
        cmp("\x00", [1])


def test_wr_with_alphabet():
    """Check wreath-product comparison with an alphabet."""
    alphabet = Alphabet("bac")
    levels = [1, 1, 0]
    x = "cbcc"
    y = "ccbc"
    assert wr_cmp(alphabet, levels, x, y)
    assert wr_cmp(alphabet, levels, "ac", "ca")

    cmp = WrCmp(alphabet, levels)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.levels() == levels
    assert cmp.init(alphabet, levels) is cmp

    with pytest.raises(LibsemigroupsError):
        wr_cmp(alphabet, levels, "d", "b")
    with pytest.raises(LibsemigroupsError):
        cmp("d", "b")

    alphabet = Alphabet([9, 4, 7])
    levels = [0, 0, 0]
    x = [9]
    y = [4]
    assert wr_cmp(alphabet, levels, x, y)

    cmp = WrCmp(alphabet, levels)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.levels() == levels
    assert cmp.init(alphabet, levels) is cmp

    with pytest.raises(LibsemigroupsError):
        wr_cmp(alphabet, levels, x, [2])
    with pytest.raises(LibsemigroupsError):
        cmp(x, [2])


def test_wr_object_copy_and_repr():
    """Check copies and human-readable representations."""
    _check_configured_copy_and_repr(WrCmp, "WrCmp", "levels", "levels")


def test_wr_object_rejects_bad_constructor_arguments():
    """Check constructor validation."""
    _check_configured_bad_constructor_arguments(WrCmp)


###############################################################################
# RevWr order
###############################################################################


def test_rev_wr_without_alphabet():
    """Check reverse wreath-product comparison without an alphabet."""
    levels = [0, 0]
    x = [1, 0]
    y = [0, 1]
    assert rev_wr_cmp(levels, x, y)

    cmp = RevWrCmp(levels)
    assert cmp(x, y)
    assert cmp("\x01\x00", "\x00\x01")
    assert cmp.levels() == levels
    assert cmp.init([2, 1]) is cmp
    assert cmp.levels() == [2, 1]

    with pytest.raises(LibsemigroupsError):
        rev_wr_cmp(levels, [2], [0])
    with pytest.raises(LibsemigroupsError):
        cmp([2], [0])
    with pytest.raises(LibsemigroupsError):
        cmp("\x02", "\x00")
    with pytest.raises(TypeError):
        cmp("\x00", [1])


def test_rev_wr_with_alphabet():
    """Check reverse wreath-product comparison with an alphabet."""
    alphabet = Alphabet("cab")
    levels = [0, 0, 0]
    x = "bc"
    y = "ba"
    assert rev_wr_cmp(alphabet, levels, x, y)

    cmp = RevWrCmp(alphabet, levels)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.levels() == levels
    assert cmp.init(alphabet, levels) is cmp

    with pytest.raises(LibsemigroupsError):
        rev_wr_cmp(alphabet, levels, x, "d")
    with pytest.raises(LibsemigroupsError):
        cmp(x, "d")

    alphabet = Alphabet([7, 2, 5])
    levels = [0, 0, 0]
    x = [5, 7]
    y = [5, 2]
    assert rev_wr_cmp(alphabet, levels, x, y)

    cmp = RevWrCmp(alphabet, levels)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.levels() == levels
    assert cmp.init(alphabet, levels) is cmp

    with pytest.raises(LibsemigroupsError):
        rev_wr_cmp(alphabet, levels, x, [9])
    with pytest.raises(LibsemigroupsError):
        cmp(x, [9])


def test_rev_wr_object_copy_and_repr():
    """Check copies and human-readable representations."""
    _check_configured_copy_and_repr(RevWrCmp, "RevWrCmp", "levels", "levels")


def test_rev_wr_object_rejects_bad_constructor_arguments():
    """Check constructor validation."""
    _check_configured_bad_constructor_arguments(RevWrCmp)


###############################################################################
# WtLenLex order
###############################################################################


def test_wt_lenlex_without_alphabet():
    """Check weight-length-lexicographic comparison without an alphabet."""
    weights = [2, 1, 6]
    x = [0, 1]
    y = [2]
    assert wt_lenlex_cmp(weights, x, y)
    assert not wt_lenlex_cmp(weights, y, x)
    assert wt_lenlex_cmp(weights, "\x00\x01", "\x02")
    assert not wt_lenlex_cmp(weights, "\x02", "\x00\x01")

    cmp = WtLenLexCmp(weights)
    assert cmp(x, y)
    assert cmp("\x00\x01", "\x02")
    assert cmp.weights() == weights
    assert cmp.init([1, 2, 6]) is cmp
    assert cmp.weights() == [1, 2, 6]

    with pytest.raises(LibsemigroupsError):
        wt_lenlex_cmp(weights, [3], [0])
    with pytest.raises(LibsemigroupsError):
        wt_lenlex_cmp(weights, "\x00", "\x03")
    with pytest.raises(LibsemigroupsError):
        cmp([3], [0])
    with pytest.raises(LibsemigroupsError):
        cmp("\x00", "\x03")
    with pytest.raises(TypeError):
        cmp("\x00", [1])


def test_wt_lenlex_with_alphabet():
    """Check weight-length-lexicographic comparison with an alphabet."""
    alphabet = Alphabet("cab")
    weights = [4, 1, 3]
    x = "aa"
    y = "b"
    assert wt_lenlex_cmp(alphabet, weights, x, y)

    cmp = WtLenLexCmp(alphabet, weights)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.weights() == weights
    assert cmp.init(alphabet, weights) is cmp

    with pytest.raises(LibsemigroupsError):
        wt_lenlex_cmp(alphabet, weights, x, "d")
    with pytest.raises(LibsemigroupsError):
        wt_lenlex_cmp(alphabet, [1, 2], x, y)
    with pytest.raises(LibsemigroupsError):
        cmp(x, "d")

    alphabet = Alphabet([7, 2, 5])
    weights = [4, 1, 3]
    x = [2, 2]
    y = [5]
    assert wt_lenlex_cmp(alphabet, weights, x, y)

    cmp = WtLenLexCmp(alphabet, weights)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.weights() == weights
    assert cmp.init(alphabet, weights) is cmp

    with pytest.raises(LibsemigroupsError):
        wt_lenlex_cmp(alphabet, weights, x, [9])
    with pytest.raises(LibsemigroupsError):
        cmp(x, [9])


def test_wt_lenlex_object_copy_and_repr():
    """Check copies and human-readable representations."""
    _check_configured_copy_and_repr(WtLenLexCmp, "WtLenLexCmp", "weights", "weights")


def test_wt_lenlex_object_rejects_bad_constructor_arguments():
    """Check constructor validation."""
    _check_configured_bad_constructor_arguments(WtLenLexCmp)


###############################################################################
# RevWtLenLex order
###############################################################################


def test_rev_wt_lenlex_without_alphabet():
    """Check reverse weight-length-lexicographic comparison without an alphabet."""
    weights = [1, 1]
    x = [1, 0]
    y = [0, 1]
    assert rev_wt_lenlex_cmp(weights, x, y)

    cmp = RevWtLenLexCmp(weights)
    assert cmp(x, y)
    assert cmp("\x01\x00", "\x00\x01")
    assert cmp.weights() == weights
    assert cmp.init([2, 1]) is cmp
    assert cmp.weights() == [2, 1]

    assert rev_wt_lenlex_cmp([1, 2], [1], [0, 0])
    with pytest.raises(LibsemigroupsError):
        rev_wt_lenlex_cmp(weights, [2], [0])
    with pytest.raises(LibsemigroupsError):
        cmp([2], [0])
    with pytest.raises(LibsemigroupsError):
        cmp("\x02", "\x00")
    with pytest.raises(TypeError):
        cmp("\x00", [1])


def test_rev_wt_lenlex_with_alphabet():
    """Check reverse weight-length-lexicographic comparison with an alphabet."""
    alphabet = Alphabet("pqr")
    weights = [1, 1, 1]
    x = "rp"
    y = "rq"
    assert rev_wt_lenlex_cmp(alphabet, weights, x, y)

    cmp = RevWtLenLexCmp(alphabet, weights)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.weights() == weights
    assert cmp.init(alphabet, weights) is cmp

    with pytest.raises(LibsemigroupsError):
        rev_wt_lenlex_cmp(alphabet, weights, x, "s")
    with pytest.raises(LibsemigroupsError):
        cmp(x, "s")

    alphabet = Alphabet([6, 1, 8])
    weights = [1, 1, 1]
    x = [8, 6]
    y = [8, 1]
    assert rev_wt_lenlex_cmp(alphabet, weights, x, y)

    cmp = RevWtLenLexCmp(alphabet, weights)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.weights() == weights
    assert cmp.init(alphabet, weights) is cmp

    with pytest.raises(LibsemigroupsError):
        rev_wt_lenlex_cmp(alphabet, weights, x, [4])
    with pytest.raises(LibsemigroupsError):
        cmp(x, [4])


def test_rev_wt_lenlex_object_copy_and_repr():
    """Check copies and human-readable representations."""
    _check_configured_copy_and_repr(RevWtLenLexCmp, "RevWtLenLexCmp", "weights", "weights")


def test_rev_wt_lenlex_object_rejects_bad_constructor_arguments():
    """Check constructor validation."""
    _check_configured_bad_constructor_arguments(RevWtLenLexCmp)


###############################################################################
# WtLex order
###############################################################################


def test_wt_lex_without_alphabet():
    """Check weight-lexicographic comparison without an alphabet."""
    weights = [2, 1, 6]
    x = [0, 1]
    y = [2]
    assert wt_lex_cmp(weights, x, y)
    assert not wt_lex_cmp(weights, y, x)
    assert wt_lex_cmp(weights, "\x00\x01", "\x02")
    assert not wt_lex_cmp(weights, "\x02", "\x00\x01")

    cmp = WtLexCmp(weights)
    assert cmp(x, y)
    assert cmp("\x00\x01", "\x02")
    assert cmp.weights() == weights
    assert cmp.init([1, 2, 6]) is cmp
    assert cmp.weights() == [1, 2, 6]

    assert not wt_lex_cmp([1, 2], [1], [0, 0])
    with pytest.raises(LibsemigroupsError):
        wt_lex_cmp(weights, [3], [0])
    with pytest.raises(LibsemigroupsError):
        wt_lex_cmp(weights, "\x00", "\x03")
    with pytest.raises(LibsemigroupsError):
        cmp([3], [0])
    with pytest.raises(LibsemigroupsError):
        cmp("\x00", "\x03")
    with pytest.raises(TypeError):
        cmp("\x00", [1])


def test_wt_lex_with_alphabet():
    """Check weight-lexicographic comparison with an alphabet."""
    alphabet = Alphabet("dbca")
    weights = [5, 2, 3, 1]
    x = "b"
    y = "aa"
    assert wt_lex_cmp(alphabet, weights, x, y)

    cmp = WtLexCmp(alphabet, weights)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.weights() == weights
    assert cmp.init(alphabet, weights) is cmp

    with pytest.raises(LibsemigroupsError):
        wt_lex_cmp(alphabet, weights, x, "e")
    with pytest.raises(LibsemigroupsError):
        wt_lex_cmp(alphabet, [1, 2, 3], x, y)
    with pytest.raises(LibsemigroupsError):
        cmp(x, "e")

    alphabet = Alphabet([9, 4, 7, 2])
    weights = [5, 2, 3, 1]
    x = [4]
    y = [2, 2]
    assert wt_lex_cmp(alphabet, weights, x, y)

    cmp = WtLexCmp(alphabet, weights)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.weights() == weights
    assert cmp.init(alphabet, weights) is cmp

    with pytest.raises(LibsemigroupsError):
        wt_lex_cmp(alphabet, weights, x, [6])
    with pytest.raises(LibsemigroupsError):
        cmp(x, [6])


def test_wt_lex_object_copy_and_repr():
    """Check copies and human-readable representations."""
    _check_configured_copy_and_repr(WtLexCmp, "WtLexCmp", "weights", "weights")


def test_wt_lex_object_rejects_bad_constructor_arguments():
    """Check constructor validation."""
    _check_configured_bad_constructor_arguments(WtLexCmp)


###############################################################################
# RevWtLex order
###############################################################################


def test_rev_wt_lex_without_alphabet():
    """Check reverse weight-lexicographic comparison without an alphabet."""
    weights = [1, 1]
    x = [1, 0]
    y = [0, 1]
    assert rev_wt_lex_cmp(weights, x, y)

    cmp = RevWtLexCmp(weights)
    assert cmp(x, y)
    assert cmp("\x01\x00", "\x00\x01")
    assert cmp.weights() == weights
    assert cmp.init([2, 1]) is cmp
    assert cmp.weights() == [2, 1]

    assert not rev_wt_lex_cmp([1, 2], [1], [0, 0])
    assert not rev_wt_lex_cmp([100, 1], [0], [1, 1])
    with pytest.raises(LibsemigroupsError):
        rev_wt_lex_cmp(weights, [2], [0])
    with pytest.raises(LibsemigroupsError):
        cmp([2], [0])
    with pytest.raises(LibsemigroupsError):
        cmp("\x02", "\x00")
    with pytest.raises(TypeError):
        cmp("\x00", [1])


def test_rev_wt_lex_with_alphabet():
    """Check reverse weight-lexicographic comparison with an alphabet."""
    alphabet = Alphabet("uvw")
    weights = [1, 1, 1]
    x = "wu"
    y = "wv"
    assert rev_wt_lex_cmp(alphabet, weights, x, y)

    cmp = RevWtLexCmp(alphabet, weights)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.weights() == weights
    assert cmp.init(alphabet, weights) is cmp

    with pytest.raises(LibsemigroupsError):
        rev_wt_lex_cmp(alphabet, weights, x, "x")
    with pytest.raises(LibsemigroupsError):
        cmp(x, "x")

    alphabet = Alphabet([3, 8, 1])
    weights = [1, 1, 1]
    x = [1, 3]
    y = [1, 8]
    assert rev_wt_lex_cmp(alphabet, weights, x, y)

    cmp = RevWtLexCmp(alphabet, weights)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.weights() == weights
    assert cmp.init(alphabet, weights) is cmp

    with pytest.raises(LibsemigroupsError):
        rev_wt_lex_cmp(alphabet, weights, x, [6])
    with pytest.raises(LibsemigroupsError):
        cmp(x, [6])


def test_rev_wt_lex_object_copy_and_repr():
    """Check copies and human-readable representations."""
    _check_configured_copy_and_repr(RevWtLexCmp, "RevWtLexCmp", "weights", "weights")


def test_rev_wt_lex_object_rejects_bad_constructor_arguments():
    """Check constructor validation."""
    _check_configured_bad_constructor_arguments(RevWtLexCmp)


###############################################################################
# LenWtLex order
###############################################################################


def test_len_wt_lex_without_alphabet():
    """Check length-weight-lexicographic comparison without an alphabet."""
    weights = [1, 1]
    x = [0, 1]
    y = [1, 0]
    assert len_wt_lex_cmp(weights, x, y)

    cmp = LenWtLexCmp(weights)
    assert cmp(x, y)
    assert cmp("\x00\x01", "\x01\x00")
    assert cmp.weights() == weights
    assert cmp.init([2, 1]) is cmp
    assert cmp.weights() == [2, 1]

    assert len_wt_lex_cmp([100, 1], [0], [1, 1])
    with pytest.raises(LibsemigroupsError):
        len_wt_lex_cmp(weights, [2], [0])
    with pytest.raises(LibsemigroupsError):
        cmp([2], [0])
    with pytest.raises(LibsemigroupsError):
        cmp("\x02", "\x00")
    with pytest.raises(TypeError):
        cmp("\x00", [1])


def test_len_wt_lex_with_alphabet():
    """Check length-weight-lexicographic comparison with an alphabet."""
    alphabet = Alphabet("acb")
    weights = [2, 1, 3]
    x = "cc"
    y = "aa"
    assert len_wt_lex_cmp(alphabet, weights, x, y)

    cmp = LenWtLexCmp(alphabet, weights)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.weights() == weights
    assert cmp.init(alphabet, weights) is cmp

    with pytest.raises(LibsemigroupsError):
        len_wt_lex_cmp(alphabet, weights, x, "d")
    with pytest.raises(LibsemigroupsError):
        cmp(x, "d")

    alphabet = Alphabet([5, 1, 8])
    weights = [2, 1, 3]
    x = [1, 1]
    y = [5, 5]
    assert len_wt_lex_cmp(alphabet, weights, x, y)

    cmp = LenWtLexCmp(alphabet, weights)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.weights() == weights
    assert cmp.init(alphabet, weights) is cmp

    with pytest.raises(LibsemigroupsError):
        len_wt_lex_cmp(alphabet, weights, x, [9])
    with pytest.raises(LibsemigroupsError):
        cmp(x, [9])


def test_len_wt_lex_object_copy_and_repr():
    """Check copies and human-readable representations."""
    _check_configured_copy_and_repr(LenWtLexCmp, "LenWtLexCmp", "weights", "weights")


def test_len_wt_lex_object_rejects_bad_constructor_arguments():
    """Check constructor validation."""
    _check_configured_bad_constructor_arguments(LenWtLexCmp)


###############################################################################
# RevLenWtLex order
###############################################################################


def test_rev_len_wt_lex_without_alphabet():
    """Check reverse length-weight-lexicographic comparison without an alphabet."""
    weights = [1, 1]
    x = [1, 0]
    y = [0, 1]
    assert rev_len_wt_lex_cmp(weights, x, y)

    cmp = RevLenWtLexCmp(weights)
    assert cmp(x, y)
    assert cmp("\x01\x00", "\x00\x01")
    assert cmp.weights() == weights
    assert cmp.init([2, 1]) is cmp
    assert cmp.weights() == [2, 1]

    assert rev_len_wt_lex_cmp([100, 1], [0], [1, 1])
    with pytest.raises(LibsemigroupsError):
        rev_len_wt_lex_cmp(weights, [2], [0])
    with pytest.raises(LibsemigroupsError):
        cmp([2], [0])
    with pytest.raises(LibsemigroupsError):
        cmp("\x02", "\x00")
    with pytest.raises(TypeError):
        cmp("\x00", [1])


def test_rev_len_wt_lex_with_alphabet():
    """Check reverse length-weight-lexicographic comparison with an alphabet."""
    alphabet = Alphabet("hgf")
    weights = [1, 1, 1]
    x = "fh"
    y = "fg"
    assert rev_len_wt_lex_cmp(alphabet, weights, x, y)

    cmp = RevLenWtLexCmp(alphabet, weights)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.weights() == weights
    assert cmp.init(alphabet, weights) is cmp

    with pytest.raises(LibsemigroupsError):
        rev_len_wt_lex_cmp(alphabet, weights, x, "i")
    with pytest.raises(LibsemigroupsError):
        cmp(x, "i")

    alphabet = Alphabet([4, 6, 2])
    weights = [1, 1, 1]
    x = [2, 4]
    y = [2, 6]
    assert rev_len_wt_lex_cmp(alphabet, weights, x, y)

    cmp = RevLenWtLexCmp(alphabet, weights)
    assert cmp(x, y)
    assert cmp.alphabet() == alphabet
    assert cmp.weights() == weights
    assert cmp.init(alphabet, weights) is cmp

    with pytest.raises(LibsemigroupsError):
        rev_len_wt_lex_cmp(alphabet, weights, x, [7])
    with pytest.raises(LibsemigroupsError):
        cmp(x, [7])


def test_rev_len_wt_lex_object_copy_and_repr():
    """Check copies and human-readable representations."""
    _check_configured_copy_and_repr(RevLenWtLexCmp, "RevLenWtLexCmp", "weights", "weights")


def test_rev_len_wt_lex_object_rejects_bad_constructor_arguments():
    """Check constructor validation."""
    _check_configured_bad_constructor_arguments(RevLenWtLexCmp)


###############################################################################
# Backwards compatibility
###############################################################################


def test_order_values_and_backwards_compatible_aliases():
    """Check enum values and their backwards-compatible aliases."""
    assert Order.shortlex == Order.lenlex
    assert Order.recursive == Order.rev_rpo
    assert Order.rpo != Order.rev_rpo


def test_deprecated_comparisons():
    """Check that legacy comparisons warn and delegate to their replacements."""
    with pytest.deprecated_call():
        result = lexicographical_compare("ac", "ba")
    assert result == lex_cmp("ac", "ba")

    with pytest.deprecated_call():
        result = shortlex_compare("cab", "dddd")
    assert result == lenlex_cmp("cab", "dddd")

    with pytest.deprecated_call():
        result = recursive_path_compare("xy", "yx")
    assert result == rev_rpo_cmp("xy", "yx")
