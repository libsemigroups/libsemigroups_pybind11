# -*- coding: utf-8 -*-

# Copyright (c) 2022, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for the Stephen class.
"""

# pylint: disable=duplicate-code
# pylint: disable=missing-function-docstring, invalid-name
import itertools

from libsemigroups_pybind11 import (
    POSITIVE_INFINITY,
    UNDEFINED,
    Presentation,
    Stephen,
    action_digraph_helper,
    presentation,
    stephen,
)

###############################################################################
# Test functions begin
###############################################################################


def test_000():

    p = Presentation([0, 1])
    presentation.add_rule_and_check(p, [0, 0, 0], [0])
    presentation.add_rule_and_check(p, [1, 1, 1], [1])
    presentation.add_rule_and_check(p, [0, 1, 0, 1], [0, 0])
    s = Stephen(p)
    s.set_word([1, 1, 0, 1]).run()
    assert s.word_graph().number_of_nodes() == 7
    assert s.word_graph() == action_digraph_helper.make(
        7,
        [
            [UNDEFINED, 1],
            [UNDEFINED, 2],
            [3, 1],
            [4, 5],
            [3, 6],
            [6, 3],
            [5, 4],
        ],
    )
    assert stephen.number_of_words_accepted(s) == POSITIVE_INFINITY

    w = [1, 1, 0, 1]

    assert stephen.accepts(s, [1, 1, 0, 0, 1, 0])
    assert stephen.accepts(s, [1, 1, 0, 0, 1, 0])
    assert not stephen.accepts(s, [])
    assert not stephen.accepts(s, [0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
    assert not stephen.accepts(s, [1, 1, 1])
    words = list(itertools.islice(stephen.words_accepted(s), 10))
    assert words == [
        [1, 1, 0, 1],
        [1, 1, 0, 0, 0, 1],
        [1, 1, 0, 0, 1, 0],
        [1, 1, 0, 1, 0, 0],
        [1, 1, 0, 1, 1, 1],
        [1, 1, 1, 1, 0, 1],
        [1, 1, 0, 0, 0, 0, 0, 1],
        [1, 1, 0, 0, 0, 0, 1, 0],
        [1, 1, 0, 0, 0, 1, 0, 0],
        [1, 1, 0, 0, 0, 1, 1, 1],
    ]

    left_factors = list(itertools.islice(stephen.left_factors(s), 10))

    assert left_factors == [
        [],
        [1],
        [1, 1],
        [1, 1, 0],
        [1, 1, 1],
        [1, 1, 0, 0],
        [1, 1, 0, 1],
        [1, 1, 1, 1],
        [1, 1, 0, 0, 0],
        [1, 1, 0, 0, 1],
    ]
    assert stephen.number_of_left_factors(s) == POSITIVE_INFINITY
    for w in left_factors:
        assert stephen.is_left_factor(s, w)

    s.set_word([0, 0]).run()
    assert s.word_graph().number_of_nodes() == 5
    assert s.word_graph() == action_digraph_helper.make(
        5, [[1, UNDEFINED], [2, 3], [1, 4], [4, 1], [3, 2]]
    )

    p.rules = []
    presentation.add_rule_and_check(p, [0, 0, 0], [0])
    presentation.add_rule_and_check(p, [1, 1, 1], [1])
    s.init(p).set_word([0, 0]).run()
    assert s.word_graph().number_of_nodes() == 3
    assert s.word_graph() == action_digraph_helper.make(
        3, [[1, UNDEFINED], [2, UNDEFINED], [1, UNDEFINED]]
    )
