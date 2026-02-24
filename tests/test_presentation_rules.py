# Copyright (c) 2026, James D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.
# AI assistance: OpenAI Codex helped implement these regression tests.

# pylint: disable=missing-function-docstring

"""Mutable presentation rules and their interaction with ordinary vector bindings."""

import gc

import pytest

from libsemigroups_pybind11 import (
    UNDEFINED,
    Forest,
    InversePresentation,
    Presentation,
    SimsRefinerFaithful,
    forest,
    presentation,
)

pytestmark = pytest.mark.quick


@pytest.fixture(name="presentation_type", params=[Presentation, InversePresentation])
def presentation_type_fixture(request):
    return request.param


@pytest.fixture(name="words", params=[str, list])
def words_fixture(request):
    if request.param is str:
        return ["a", "b", "ab", ""]
    return [[0], [1], [0, 1], []]


@pytest.fixture(name="p")
def presentation_fixture(presentation_type, words):
    result = presentation_type("ab" if isinstance(words[0], str) else [0, 1])
    result.rules = words
    return result


def test_rules_access_and_comparison(p, words):
    rules = p.rules
    assert len(rules) == 4
    assert rules
    assert list(rules) == words
    assert rules[0] == words[0]
    assert rules[-1] == words[-1]
    assert rules == words
    assert rules == tuple(words)
    if isinstance(words[0], list):
        assert rules == tuple(tuple(word) for word in words)
    assert rules == p.rules
    assert not rules != p.rules  # noqa: SIM202 - exercise __ne__ separately
    assert rules != words[:-1]
    assert rules != object()
    assert words[0] in rules
    assert rules.count(words[0]) == 1
    assert repr(rules) == repr(words)


def test_rules_mutation(p, words):
    rules = p.rules
    rules[0] = words[1]
    rules[-1] = words[2]
    assert p.rules == [words[1], words[1], words[2], words[2]]
    rules.append(words[0])
    rules.insert(0, words[3])
    rules.insert(-1, words[3])
    rules.insert(len(rules), words[1])
    assert p.rules == [
        words[3],
        words[1],
        words[1],
        words[2],
        words[2],
        words[3],
        words[0],
        words[1],
    ]
    assert rules.pop() == words[1]
    assert rules.pop(-2) == words[3]
    assert rules.pop(0) == words[3]
    rules.remove(words[1])
    del rules[-1]
    assert p.rules == [words[1], words[2], words[2]]
    assert rules.count(words[2]) == 2
    assert words[3] not in rules
    with pytest.raises(ValueError):
        rules.remove(words[3])
    rules.clear()
    assert not rules
    assert p.rules == []


def test_rules_extend(p, words):
    rules = p.rules
    rules.extend(words)
    rules.extend(iter(words))
    rules.extend(p.rules)
    assert p.rules == words * 6
    rules.extend(iter(rules))
    assert p.rules == words * 12


def test_rules_copies_and_slices_are_lists(p, words):
    rules = p.rules
    for independent in (list(rules), rules[:], rules[::-1]):
        assert isinstance(independent, list)
        expected = list(independent)
        independent.append(words[0])
        assert list(independent) == expected + [words[0]]
        assert p.rules == words
    assert rules[1::2] == words[1::2]
    assert rules[::-1] == words[::-1]
    assert rules[1:1] == []


def test_rules_cannot_be_constructed_independently(p, words):
    rules_type = type(p.rules)
    for args in ((), (words,), (p.rules,)):
        with pytest.raises(TypeError):
            rules_type(*args)


def test_rules_slice_assignment_and_deletion(p, words):
    rules = p.rules
    rules[1:3] = [words[0], words[1]]
    assert p.rules == [words[0], words[0], words[1], words[3]]
    rules[::-2] = (words[2], words[3])
    assert p.rules == [words[0], words[3], words[1], words[2]]
    with pytest.raises(RuntimeError, match="different sizes"):
        rules[1:2] = []
    with pytest.raises(TypeError):
        rules[1:2] = [object()]
    del rules[1::2]
    assert p.rules == [words[0], words[1]]
    del rules[::-1]
    assert p.rules == []


def test_rules_slice_assignment_from_an_alias(p, words):
    p.rules[::-1] = p.rules
    assert p.rules == words[::-1]


@pytest.mark.parametrize(
    "selection",
    [
        slice(None),
        slice(None, None, -1),
        slice(1, None, 2),
        slice(None, None, -2),
        slice(-3, -1),
        slice(3, 0, -1),
        slice(0, 3, -1),
        slice(-100, 100, 3),
        slice(100, -100, -3),
        slice(None, None, 10**100),
        slice(None, None, -(10**100)),
    ],
)
def test_rules_slices_match_lists(p, words, selection):
    expected = list(words)
    assert p.rules[selection] == expected[selection]
    replacement = list(reversed(expected[selection]))
    p.rules[selection] = replacement
    expected[selection] = replacement
    assert p.rules == expected
    del expected[selection]
    del p.rules[selection]
    assert p.rules == expected


def test_rules_concatenation(p, words):
    rules = p.rules
    for other in (words, tuple(words), p.rules):
        combined = rules + other
        assert isinstance(combined, list)
        assert combined == words * 2
        combined.clear()
        assert p.rules == words
    p.rules += words
    assert rules == words * 2
    with pytest.raises(TypeError):
        _ = rules + 1


def test_rules_views_follow_replacement_and_cpp_mutation(p, words):
    rules = p.rules
    other_view = p.rules
    p.rules = iter(words[:2])
    assert rules == words[:2]
    assert other_view == words[:2]
    p.contains_empty_word(True)
    presentation.add_rule(p, words[2], words[3])
    assert rules == words
    p.rules = rules
    assert rules == words
    p.init()
    assert rules == []
    rules.append(words[0])
    assert p.rules == words[:1]


def test_rules_view_keeps_presentation_alive(presentation_type, words):
    p = presentation_type("ab" if isinstance(words[0], str) else [0, 1])
    p.rules = words
    rules = p.rules
    del p
    gc.collect()
    assert rules == words
    rules.append(words[0])
    assert rules == words + words[:1]


def test_rules_iterator_keeps_presentation_alive(presentation_type, words):
    p = presentation_type("ab" if isinstance(words[0], str) else [0, 1])
    p.rules = words
    iterator = iter(p.rules)
    del p
    gc.collect()
    assert list(iterator) == words


def test_rules_iterator_survives_replacement(p, words):
    iterator = iter(p.rules)
    assert next(iterator) == words[0]
    p.rules = words * 20
    assert list(iterator) == (words * 20)[1:]
    iterator = iter(p.rules)
    p.rules.clear()
    with pytest.raises(StopIteration):
        next(iterator)


@pytest.mark.parametrize("index", [-5, 4])
def test_rules_invalid_indices(p, words, index):
    rules = p.rules
    with pytest.raises(IndexError):
        _ = rules[index]
    with pytest.raises(IndexError):
        rules[index] = words[0]
    with pytest.raises(IndexError):
        del rules[index]
    with pytest.raises(IndexError):
        rules.pop(index)
    assert p.rules == words


def test_rules_invalid_values(p, words):
    rules = p.rules
    with pytest.raises(TypeError):
        rules.append(object())
    with pytest.raises(TypeError):
        rules[0] = object()
    with pytest.raises(TypeError):
        p.rules = [object()]
    with pytest.raises(TypeError):
        rules[:2] = [words[1], object()]
    with pytest.raises(RuntimeError):
        rules.extend([words[0], object()])
    assert p.rules == words
    with pytest.raises(IndexError):
        rules.insert(5, words[0])
    with pytest.raises(ValueError):
        _ = rules[::0]
    rules.clear()
    with pytest.raises(IndexError):
        rules.pop()


def test_integer_rule_elements_are_copies():
    p = Presentation([0, 1])
    p.rules = [[0], [1]]
    word = p.rules[0]
    word.append(1)
    assert p.rules == [[0], [1]]
    p.rules[0] = word
    assert p.rules == [[0, 1], [1]]


def test_other_vector_bindings_still_convert_lists():
    tree = Forest([UNDEFINED, 0], [UNDEFINED, 0])
    assert "a" in str(forest.dot(tree, ["a"]))
    forbidden = [[0], [0, 1]]
    refiner = SimsRefinerFaithful(forbidden)
    result = refiner.forbid()
    assert isinstance(result, list)
    assert result == forbidden
    result.clear()
    assert refiner.forbid() == forbidden
    refiner.init([[1], []])
    assert refiner.forbid() == [[1], []]
