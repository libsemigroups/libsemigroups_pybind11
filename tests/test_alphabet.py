# Copyright (c) 2026 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""This module contains some tests for the Alphabet class."""

# pylint: disable=missing-function-docstring

import pytest

from libsemigroups_pybind11 import Alphabet, LibsemigroupsError, alphabet, to


def test_string_alphabet():
    a = Alphabet("abc")

    assert len(a) == 3
    assert not a.empty()
    assert "a" in a
    assert "d" not in a
    assert a.letters() == "abc"
    assert a.index("b") == 1
    assert a.letter(2) == "c"
    assert alphabet.first_unused_letter(a) == "d"
    assert alphabet.validate(a) is None

    with pytest.raises(LibsemigroupsError):
        Alphabet("aa")


def test_word_alphabet():
    a = Alphabet([0, 2])

    assert len(a) == 2
    assert not a.empty()
    assert 0 in a
    assert 1 not in a
    assert a.letters() == [0, 2]
    assert a.index(2) == 1
    assert a.letter(0) == 0
    assert alphabet.first_unused_letter(a) == 1
    assert alphabet.validate(a) is None

    with pytest.raises(LibsemigroupsError):
        Alphabet([0, 0])


def test_ambiguous_constructors():
    with pytest.raises(TypeError):
        Alphabet()

    assert len(Alphabet(word=str)) == 0
    assert len(Alphabet(word=list[int])) == 0
    assert Alphabet(3, word=str).letters() == "abc"
    assert Alphabet(3, word=list[int]).letters() == [0, 1, 2]

    with pytest.raises(TypeError):
        Alphabet(3)
    with pytest.raises(ValueError):
        Alphabet(word=tuple)


def test_add_letter():
    a = Alphabet("abc")

    with pytest.raises(LibsemigroupsError):
        a.add_letter("a")
    with pytest.raises(TypeError):
        a.add_letter(10)
    assert a.add_letter("e") is a

    a = Alphabet([0, 3])

    with pytest.raises(LibsemigroupsError):
        a.add_letter(0)
    with pytest.raises(TypeError):
        a.add_letter("a")
    assert a.add_letter(1) is a


def test_contains():
    a = Alphabet("abc")

    assert "a" in a
    assert a.contains("a")

    with pytest.raises(TypeError):
        a.contains(0)

    a = Alphabet([0, 1, 2])

    assert 1 in a
    assert a.contains(1)

    with pytest.raises(TypeError):
        a.contains("a")


def test_empty():
    a = Alphabet("abc")
    assert not a.empty()
    a.init()
    assert a.empty()

    a = Alphabet(3, word=list[int])
    assert not a.empty()
    a.init()
    assert a.empty()


def test_iter():
    a = Alphabet("abc")
    assert list(a) == list(a.letters())

    a = Alphabet([0, 1, 11])
    assert list(a) == a.letters()


def test_index():
    a = Alphabet("abc")
    assert [a.index(x) for x in a] == [0, 1, 2]

    with pytest.raises(LibsemigroupsError):
        a.index("d")
    with pytest.raises(TypeError):
        a.index(0)

    a = Alphabet([11, 0, 1])
    assert [a.index(x) for x in a] == [0, 1, 2]

    with pytest.raises(LibsemigroupsError):
        a.index(2)
    with pytest.raises(TypeError):
        a.index("a")


def test_copy():
    a = Alphabet("abc")
    b = a.copy()

    assert b == a
    assert b is not a
    b.add_letter("d")
    assert b != a
    assert a.letters() == "abc"
    assert b.letters() == "abcd"

    a = Alphabet([0, 2])
    b = a.copy()

    assert b == a
    assert b is not a
    b.add_letter(1)
    assert b != a
    assert a.letters() == [0, 2]
    assert b.letters() == [0, 2, 1]


def test_copy_constructor():
    a = Alphabet("abc")
    b = a.copy()

    assert b == a
    assert b is not a
    assert b.letters() == "abc"

    a = Alphabet([0, 2])
    b = a.copy()

    assert b == a
    assert b is not a
    assert b.letters() == [0, 2]


def test_repr_and_str():
    assert repr(Alphabet("abc")) == '<alphabet "abc">'
    assert str(Alphabet("abc")) == 'Alphabet("abc")'
    assert repr(Alphabet([0, 1, 2])) == "<alphabet [0, 1, 2]>"
    assert str(Alphabet([0, 1, 2])) == "Alphabet([0, 1, 2])"


def test_equality():
    assert Alphabet("abc") == Alphabet("abc")
    assert Alphabet("abc") != Alphabet("acb")
    assert Alphabet([0, 1, 2]) == Alphabet([0, 1, 2])
    assert Alphabet([0, 1, 2]) != Alphabet([0, 2, 1])


def test_init_from_letters():
    a = Alphabet("abc")

    assert a.init("de") is a
    assert a.letters() == "de"
    with pytest.raises(LibsemigroupsError):
        a.init("dd")
    with pytest.raises(TypeError):
        a.init([0, 1])

    a = Alphabet([0, 1])

    assert a.init([3, 2]) is a
    assert a.letters() == [3, 2]
    with pytest.raises(LibsemigroupsError):
        a.init([3, 3])
    with pytest.raises(TypeError):
        a.init("ab")


def test_init_from_size():
    a = Alphabet("abc")

    assert a.init(2) is a
    assert a.letters() == "ab"

    a = Alphabet([10, 11])

    assert a.init(3) is a
    assert a.letters() == [0, 1, 2]


def test_letter():
    a = Alphabet("abc")

    assert [a.letter(i) for i in range(len(a))] == ["a", "b", "c"]
    with pytest.raises(LibsemigroupsError):
        a.letter(3)
    with pytest.raises(TypeError):
        a.letter("a")

    a = Alphabet([11, 0, 1])

    assert [a.letter(i) for i in range(len(a))] == [11, 0, 1]
    with pytest.raises(LibsemigroupsError):
        a.letter(3)
    with pytest.raises(TypeError):
        a.letter("a")


def test_letters_after_mutation():
    a = Alphabet("ab")

    a.add_letter("c")
    assert a.letters() == "abc"
    a.remove_letter("b")
    assert a.letters() == "ac"

    a = Alphabet([0, 1])

    a.add_letter(3)
    assert a.letters() == [0, 1, 3]
    a.remove_letter(1)
    assert a.letters() == [0, 3]


def test_remove_letter():
    a = Alphabet("abc")

    assert a.remove_letter("b") is a
    assert a.letters() == "ac"
    with pytest.raises(LibsemigroupsError):
        a.remove_letter("b")
    with pytest.raises(TypeError):
        a.remove_letter(0)

    a = Alphabet([0, 1, 2])

    assert a.remove_letter(1) is a
    assert a.letters() == [0, 2]
    with pytest.raises(LibsemigroupsError):
        a.remove_letter(1)
    with pytest.raises(TypeError):
        a.remove_letter("a")


def test_throw_if_duplicate_letters():
    assert Alphabet("abc").throw_if_duplicate_letters() is None
    assert Alphabet([0, 1, 2]).throw_if_duplicate_letters() is None


def test_throw_if_letter_not_in_alphabet():
    a = Alphabet("abc")

    assert a.throw_if_letter_not_in_alphabet("a") is None
    with pytest.raises(LibsemigroupsError):
        a.throw_if_letter_not_in_alphabet("d")
    with pytest.raises(TypeError):
        a.throw_if_letter_not_in_alphabet(0)

    a = Alphabet([0, 1, 2])

    assert a.throw_if_letter_not_in_alphabet(1) is None
    with pytest.raises(LibsemigroupsError):
        a.throw_if_letter_not_in_alphabet(3)
    with pytest.raises(TypeError):
        a.throw_if_letter_not_in_alphabet("a")


def test_validate():
    assert alphabet.validate(Alphabet("abc")) is None
    assert alphabet.validate(Alphabet([0, 1, 2])) is None
    with pytest.raises(TypeError):
        alphabet.validate("abc")


def test_to_alphabet():
    a = Alphabet("abc")
    assert a.letters() == "abc"

    b = to(a, rtype=(Alphabet, str))
    assert a == b

    # assert b is a

    c = to(a, rtype=(Alphabet, list[int]))
    assert c.letters() == [0, 1, 2]

    assert to(c, rtype=(Alphabet, str)) == a
