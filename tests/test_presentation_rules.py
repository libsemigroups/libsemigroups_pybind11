# Copyright (c) 2026, James D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.
# AI assistance: OpenAI Codex helped implement these regression tests.

# pylint: disable=missing-function-docstring

"""Mutable presentation rules and their interaction with ordinary vector bindings."""

import gc
import operator
import sys

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


class _Index:  # pylint: disable=too-few-public-methods
    """Exercise the integer index protocol without inheriting from int."""

    def __init__(self, value):
        self.value = value

    def __index__(self):
        return self.value


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


@pytest.mark.parametrize("other_word", ["a", [0, 1], None, object(), 0, [object()]])
def test_rules_comparison_with_incompatible_elements(p, words, other_word):
    p.rules = words[:1]
    other = [other_word]
    assert (p.rules == other) == (words[:1] == other)
    assert (other == p.rules) == (other == words[:1])
    assert (p.rules != other) == (words[:1] != other)
    assert (other != p.rules) == (other != words[:1])
    assert p.rules == words[:1]


def test_rules_comparison_with_incompatible_views(presentation_type):
    strings = presentation_type("ab")
    strings.rules = ["a"]
    integers = presentation_type([0, 1])
    integers.rules = [[0, 1]]
    assert not strings.rules == integers.rules  # noqa: SIM201 - exercise __eq__
    assert not integers.rules == strings.rules  # noqa: SIM201 - exercise __eq__
    assert strings.rules != integers.rules
    assert integers.rules != strings.rules


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


@pytest.mark.parametrize(
    "index", [-sys.maxsize - 1, -100, -5, -4, -1, 0, 1, 4, 5, 100, sys.maxsize]
)
@pytest.mark.parametrize("initially_empty", [False, True])
def test_rules_insert_matches_lists(p, words, index, initially_empty):
    expected = [] if initially_empty else list(words)
    p.rules = expected
    rules = p.rules
    expected.insert(index, words[2])
    assert rules.insert(index, words[2]) is None
    assert rules == expected
    assert p.rules == expected


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


def test_rules_copy_and_reverse(p, words):
    rules = p.rules
    copied = rules.copy()
    assert isinstance(copied, list)
    assert copied == words
    assert rules.reverse() is None
    assert p.rules == words[::-1]
    assert copied == words
    copied.clear()
    assert p.rules == words[::-1]
    rules.clear()
    assert rules.reverse() is None
    assert rules.sort() is None
    assert rules.copy() == []


@pytest.mark.parametrize(
    "bounds",
    [
        (),
        (1,),
        (-3,),
        (0, 0),
        (1, 3),
        (2, 5),
        (-100, 100),
        (100,),
        (10**100,),
        (-(10**100), 10**100),
        (_Index(2), _Index(8)),
        (None,),
        (1.5,),
        (0, None),
    ],
)
def test_rules_index_matches_lists(p, words, bounds):
    expected = words * 2
    p.rules = expected
    try:
        result = expected.index(words[1], *bounds)
    except (ValueError, TypeError) as error:
        with pytest.raises(type(error)):
            p.rules.index(words[1], *bounds)
    else:
        assert p.rules.index(words[1], *bounds) == result
    assert p.rules == expected


def test_rules_index_missing_value_and_keywords(p):
    with pytest.raises(ValueError):
        p.rules.index(object())
    with pytest.raises(TypeError):
        p.rules.index(value=p.rules[0])
    with pytest.raises(TypeError):
        p.rules.index(p.rules[0], start=0)


@pytest.mark.parametrize("key", [None, len])
@pytest.mark.parametrize("reverse", [False, True])
def test_rules_sort_matches_lists(p, words, key, reverse):
    expected = words * 2
    p.rules = expected
    rules = p.rules
    expected.sort(key=key, reverse=reverse)
    assert rules.sort(key=key, reverse=reverse) is None
    assert rules == expected
    assert p.rules == expected


def test_rules_sort_calls_key_once_per_word(p, words):
    seen = []

    def key(word):
        seen.append(word)
        return len(word)

    p.rules.sort(key=key)
    assert seen == words
    assert p.rules == sorted(words, key=len)


def test_rules_sort_errors(p, words):
    def fail(_):
        raise RuntimeError("key failed")

    with pytest.raises(RuntimeError, match="key failed"):
        p.rules.sort(key=fail)
    with pytest.raises(TypeError):
        p.rules.sort(key=lambda _: object())
    with pytest.raises(TypeError):
        p.rules.sort(len)
    assert p.rules == words


def test_rules_sort_detects_callback_mutation(p, words):
    def key(word):
        if len(p.rules) == len(words):
            p.rules.append(words[0])
        return len(word)

    with pytest.raises(ValueError, match="modified during sort"):
        p.rules.sort(key=key)
    assert p.rules == words + words[:1]


@pytest.mark.parametrize("compare", [operator.lt, operator.le, operator.gt, operator.ge])
@pytest.mark.parametrize("selection", [slice(None), slice(1), slice(None, None, -1)])
def test_rules_ordering_matches_lists(p, words, presentation_type, compare, selection):
    other = words[selection]
    assert compare(p.rules, other) == compare(words, other)
    assert compare(other, p.rules) == compare(other, words)
    q = presentation_type(p.alphabet())
    q.rules = other
    assert compare(p.rules, q.rules) == compare(words, other)
    assert compare(q.rules, p.rules) == compare(other, words)
    assert compare(p.rules, p.rules) == compare(words, words)
    with pytest.raises(TypeError):
        compare(p.rules, tuple(other))
    with pytest.raises(TypeError):
        compare(tuple(other), p.rules)


@pytest.mark.parametrize("compare", [operator.lt, operator.le, operator.gt, operator.ge])
def test_rules_ordering_incompatible_words(presentation_type, compare):
    strings = presentation_type("ab")
    strings.rules = ["a"]
    integers = presentation_type([0, 1])
    integers.rules = [[0]]
    with pytest.raises(TypeError):
        compare(strings.rules, integers.rules)
    with pytest.raises(TypeError):
        compare(integers.rules, strings.rules)
    strings.rules.clear()
    assert compare(strings.rules, integers.rules) == compare([], [[0]])


def test_rules_inplace_add_keeps_the_view(p, words):
    rules = p.rules
    original = rules
    other_view = p.rules
    rules += rules
    assert rules is original
    assert other_view == words * 2
    rules += iter(other_view)
    assert rules is original
    assert p.rules == words * 4
    p.rules += tuple(words)
    assert other_view == words * 5
    with pytest.raises(RuntimeError):
        rules += [words[0], object()]
    assert p.rules == words * 5


@pytest.mark.parametrize("count", [-3, 0, 1, 2, False, True, _Index(3)])
@pytest.mark.parametrize("initially_empty", [False, True])
def test_rules_repetition_matches_lists(p, words, count, initially_empty):
    expected = [] if initially_empty else list(words)
    p.rules = expected
    rules = p.rules
    original = rules
    for result in (rules * count, count * rules):
        assert isinstance(result, list)
        assert result == expected * count
        result.append(words[0])
        assert p.rules == expected
    rules *= count
    assert rules is original
    assert p.rules == expected * count
    p.rules *= 2
    assert rules == expected * count * 2


@pytest.mark.parametrize(
    "count, error_type",
    [
        (1.5, TypeError),
        (None, TypeError),
        (10**100, OverflowError),
        (-(10**100), OverflowError),
        (sys.maxsize, MemoryError),
    ],
)
def test_rules_repetition_errors(p, words, count, error_type):
    rules = p.rules
    with pytest.raises(error_type):
        _ = rules * count
    with pytest.raises(error_type):
        _ = count * rules
    with pytest.raises(error_type):
        rules *= count
    assert p.rules == words


def test_rules_inplace_operations_keep_presentation_alive(presentation_type, words):
    p = presentation_type("ab" if isinstance(words[0], str) else [0, 1])
    p.rules = words
    rules = p.rules
    del p
    gc.collect()
    rules += rules
    rules *= 2
    assert rules == words * 4
    rules.reverse()
    rules.sort()
    assert rules == sorted(words * 4)


def test_rules_provide_all_named_list_methods(p):
    methods = {name for name in dir(list) if not name.startswith("_")}
    assert methods <= set(dir(p.rules))


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
    with pytest.raises(ValueError, match="sequence of size 0 to extended slice of size 2"):
        rules[1::2] = []
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
        slice(1, 3),
        slice(1, 3, 1),
        slice(2, 2),
        slice(3, 1),
        slice(-3, -1),
        slice(-100, 100),
        slice(100, 200),
        slice(-100, -50),
    ],
)
@pytest.mark.parametrize("replacement_size", [0, 1, 6])
@pytest.mark.parametrize("initially_empty", [False, True])
def test_rules_slice_assignment_resizes_like_lists(
    p, words, selection, replacement_size, initially_empty
):
    expected = [] if initially_empty else list(words)
    p.rules = expected
    rules = p.rules
    replacement = (words * 2)[:replacement_size]
    expected[selection] = replacement
    rules[selection] = replacement
    assert rules == expected
    assert p.rules == expected


@pytest.mark.parametrize("selection", [slice(1, 2), slice(2, 2), slice(3, 1)])
@pytest.mark.parametrize("replacement_type", [list, tuple])
def test_rules_slice_assignment_resizes_from_a_sequence(p, words, selection, replacement_type):
    expected = list(words)
    expected[selection] = expected
    rules = p.rules
    rules[selection] = replacement_type(p.rules)
    assert rules == expected
    assert p.rules == expected


def test_rules_slice_assignment_resizes_from_a_view(p, words):
    expected = list(words)
    expected[1:2] = expected
    p.rules[1:2] = p.rules
    assert p.rules == expected


@pytest.mark.parametrize("selection", [slice(None, None, 2), slice(None, None, -1), slice(2, 2, 2)])
def test_rules_extended_slice_assignment_rejects_size_mismatch(p, words, selection):
    expected = list(words)
    with pytest.raises(ValueError) as list_error:
        expected[selection] = words[:1]
    with pytest.raises(ValueError) as rules_error:
        p.rules[selection] = words[:1]
    assert str(rules_error.value) == str(list_error.value)
    assert p.rules == expected


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
        slice(3, 3, -(10**100)),
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


@pytest.mark.parametrize(
    "selection",
    [
        slice(None, None, 2),
        slice(None, None, -2),
        slice(2, 13, 3),
        slice(13, 2, -3),
        slice(3, 12, 2),
        slice(11, 2, -2),
        slice(-2, None, -3),
        slice(2, 3, 5),
        slice(2, None, 10**100),
        slice(17, None, -(10**100)),
    ],
)
def test_rules_stepped_deletion_preserves_prefix_and_suffix(p, words, selection):
    expected = [words[0] * i for i in range(20)]
    p.rules = expected
    rules = p.rules
    del expected[selection]
    del rules[selection]
    assert rules == expected
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
    with pytest.raises(TypeError):
        rules[:1] = [words[1], object()]
    with pytest.raises(RuntimeError):
        rules.extend([words[0], object()])
    assert p.rules == words
    with pytest.raises(TypeError):
        rules.insert(5, object())
    with pytest.raises(ValueError):
        _ = rules[::0]
    with pytest.raises(ValueError):
        rules[::0] = words
    assert p.rules == words
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
