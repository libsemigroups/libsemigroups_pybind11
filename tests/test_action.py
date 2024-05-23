# -*- coding: utf-8 -*-

# Copyright (c) 2024, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for the libsemigroups_pybind11 functionality
arising from action.*pp in libsemigroups.
"""

# pylint: disable=no-name-in-module, missing-function-docstring, invalid-name
# pylint: disable=redefined-outer-name, no-member


import pytest

from libsemigroups_pybind11 import (
    RightAction,
    LeftAction,
    Action,
    ImageRightAction,
    PPerm,
    BMat8,
    side,
    Transf,
    ReportGuard,
)
from libsemigroups_pybind11.bmat8 import row_space_basis, col_space_basis


@pytest.fixture
def right_actions():
    result = [RightAction(Point=BMat8, Element=BMat8)]
    result[0].add_seed(
        row_space_basis(
            BMat8([[1, 1, 1, 0], [1, 1, 0, 0], [0, 1, 0, 1], [0, 1, 0, 0]])
        )
    )

    result[0].add_generator(
        BMat8([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]])
    )
    result[0].add_generator(
        BMat8([[0, 1, 0, 0], [1, 0, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]])
    )
    result[0].add_generator(
        BMat8([[0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1], [1, 0, 0, 0]])
    )
    result[0].add_generator(
        BMat8([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [1, 0, 0, 1]])
    )
    result[0].add_generator(
        BMat8([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 0]])
    )

    result.append(RightAction(Point=PPerm, Element=PPerm))
    result[-1].add_seed(PPerm.one(16))

    result[-1].add_generator(
        PPerm(
            [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
            [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0],
            16,
        )
    )
    result[-1].add_generator(
        PPerm(
            [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
            [1, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
            16,
        )
    )
    result[-1].add_generator(
        PPerm(
            [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
            [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14],
            16,
        )
    )
    result[-1].add_generator(
        PPerm(
            [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14],
            [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
            16,
        )
    )
    result[-1].reserve(70000)

    result.append(RightAction(Point=list, Element=PPerm))
    result[-1].add_seed(list(range(16)))

    result[-1].add_generator(
        PPerm(
            [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
            [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0],
            16,
        )
    )
    result[-1].add_generator(
        PPerm(
            [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
            [1, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
            16,
        )
    )
    result[-1].add_generator(
        PPerm(
            [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
            [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14],
            16,
        )
    )
    result[-1].add_generator(
        PPerm(
            [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14],
            [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
            16,
        )
    )
    result[-1].reserve(70000)

    result.append(RightAction(Point=list, Element=Transf))
    result[-1].add_seed(list(range(16)))
    result[-1].add_generator(Transf([0, 0] + list(range(1, 15))))
    result[-1].add_generator(Transf([1, 0] + list(range(2, 16))))
    result[-1].add_generator(Transf(list(range(1, 16)) + [0]))

    return result


@pytest.fixture
def left_actions():
    result = [LeftAction(Point=BMat8, Element=BMat8)]
    result[-1].add_seed(
        col_space_basis(
            BMat8([[1, 1, 1, 0], [1, 1, 0, 0], [0, 1, 0, 1], [0, 1, 0, 0]])
        )
    )

    result[-1].add_generator(
        BMat8([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]])
    )
    result[-1].add_generator(
        BMat8([[0, 1, 0, 0], [1, 0, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]])
    )
    result[-1].add_generator(
        BMat8([[0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1], [1, 0, 0, 0]])
    )
    result[-1].add_generator(
        BMat8([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [1, 0, 0, 1]])
    )
    result[-1].add_generator(
        BMat8([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 0]])
    )

    result.append(LeftAction(Point=list, Element=PPerm))
    result[-1].add_seed(list(range(16)))

    result[-1].add_generator(
        PPerm(
            [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
            [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0],
            16,
        )
    )
    result[-1].add_generator(
        PPerm(
            [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
            [1, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
            16,
        )
    )
    result[-1].add_generator(
        PPerm(
            [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
            [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14],
            16,
        )
    )
    result[-1].add_generator(
        PPerm(
            [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14],
            [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
            16,
        )
    )
    result[-1].reserve(70000)
    return result


@pytest.fixture
def extreme_left_actions():
    result = []
    result.append(LeftAction(Point=list, Element=Transf))
    result[-1].add_seed(list(range(16)))
    result[-1].add_generator(Transf([0, 0] + list(range(1, 15))))
    result[-1].add_generator(Transf([1, 0] + list(range(2, 16))))
    result[-1].add_generator(Transf(list(range(1, 16)) + [0]))
    return result


def test_action_001(right_actions, left_actions):
    rows = right_actions[0]
    assert len(rows) == 553
    rows.init()

    rows.add_seed(
        row_space_basis(
            BMat8([[1, 1, 1, 0], [1, 1, 0, 0], [0, 1, 0, 1], [0, 1, 0, 0]])
        )
    )

    rows.add_generator(
        BMat8([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]])
    )
    rows.add_generator(
        BMat8([[0, 1, 0, 0], [1, 0, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]])
    )
    rows.add_generator(
        BMat8([[0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1], [1, 0, 0, 0]])
    )
    rows.add_generator(
        BMat8([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [1, 0, 0, 1]])
    )
    rows.add_generator(
        BMat8([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 0]])
    )

    assert len(rows) == 553

    cols = left_actions[0]

    assert len(cols) == 553
    assert cols != rows


def test_action_pperm(right_actions):
    right = right_actions[1]
    right.reserve(70000)
    assert len(right) == 65536
    assert (
        repr(right) == "<complete right action with 4 generators, 65536 points>"
    )
    assert right[666] == PPerm(
        [0, 2, 7, 8, 9, 10, 11, 12, 13, 14],
        [0, 2, 7, 8, 9, 10, 11, 12, 13, 14],
        16,
    )

    right.add_generator(
        PPerm(
            [],
            [],
            16,
        )
    )

    with pytest.raises(ValueError):
        right.add_generator(
            PPerm(
                [],
                [],
                15,
            )
        )
    with pytest.raises(ValueError):
        right.add_seed(BMat8(0))

    x = PPerm([], [], 16)
    assert x in right
    assert right.index(x) == 43748
    right.add_seed(
        PPerm(
            [],
            [],
            16,
        )
    )

    # TODO adding another seed that's already in the action should maybe not
    # increase the size by 1
    assert len(right) == 65537


def test_action_pperm2():
    act = RightAction(Element=PPerm, Point=list)
    for n in [199, 299, 69999]:
        act.init()
        act.add_seed([0])
        act.add_generator(PPerm(range(0, n - 1), range(1, n), n))
        assert len(act) == n + 1


def test_action_transf(right_actions, extreme_left_actions):
    act = right_actions[3]
    assert len(act) == 65535

    ReportGuard(True)
    act = extreme_left_actions[0]
    act.run_until(lambda: act.current_size() > 5 * 10**5)
    assert act.current_size() > 5 * 10**5


def test_action_coverage():
    with pytest.raises(TypeError):
        Action(Element=PPerm)

    with pytest.raises(ValueError):
        Action(BadKeywordArg=PPerm, Element=None, Point=None, Help=None)

    with pytest.raises(ValueError):
        Action(Point=PPerm, Element=BMat8, Func=RightAction, Side=side.left)
        Action(BadKeywordArg=PPerm, Element=None, Point=None, Help=None)

    right = RightAction(Point=PPerm, Element=PPerm)
    right.add_seed(PPerm.one(16))

    right.add_generator(
        PPerm(
            [0, 1, 2, 3],
            [1, 2, 3, 4],
            16,
        )
    )
    right.run()
    right.run()  # tests that _before_run returns correctly on duplicate call
    right.init()
    with pytest.raises(ValueError):
        right.run()  # No generators
    right.add_generator(
        PPerm(
            [0, 1, 2, 3],
            [1, 2, 3, 4],
            16,
        )
    )
    with pytest.raises(ValueError):
        right.run()  # No seeds

    right = Action(
        Point=PPerm, Element=PPerm, Func=ImageRightAction, Side=side.right
    )
    right.add_seed(PPerm.one(17))
    right.add_generator(
        PPerm(
            [0, 1, 2, 3],
            [1, 2, 3, 4],
            5,
        )
    )
    with pytest.raises(ValueError):
        right.run()

    assert repr(right) == "<incomplete action with 1 generators, 1 points>"
    with pytest.raises(ValueError):
        right.add_generator(BMat8(0))


def test_action_cache_scc_multipliers(right_actions, left_actions):
    for action in right_actions + left_actions:
        assert not action.cache_scc_multipliers()
        action.cache_scc_multipliers(True)
    for action in right_actions + left_actions:
        assert action.cache_scc_multipliers()
        with pytest.raises(ValueError):
            action.cache_scc_multipliers("hello")
        with pytest.raises(ValueError):
            action.cache_scc_multipliers(1, 2)
    for action in right_actions + left_actions:
        action.run()
        assert action.cache_scc_multipliers()


def test_action_current_size(right_actions, left_actions):
    for action in right_actions + left_actions:
        assert action.current_size() == 1
    x = PPerm(
        [],
        [],
        16,
    )
    act = right_actions[1]
    right_actions[1].run_until(lambda: x in act)
    assert right_actions[1].current_size() == 43749


def test_action_empty(right_actions, left_actions):
    for action in right_actions + left_actions:
        assert action.empty()


def test_action_iterator(right_actions, left_actions):
    for action in right_actions + left_actions:
        for x in action.iterator():
            assert x in action


def test_action_multiplier_from_scc_root(right_actions, left_actions):
    for action in right_actions + left_actions:
        action.run()
        gabow = action.scc()
        func = action.Func(Element=action.Element, Point=action.Point)
        for i, pt1 in enumerate(action):
            x = action.multiplier_from_scc_root(i)
            pt2 = action[gabow.root_of(i)]
            assert func(pt2, x) == pt1
        break  # only test the first action


def test_action_multiplier_to_scc_root(right_actions, left_actions):
    for action in right_actions + left_actions:
        action.run()
        gabow = action.scc()
        func = action.Func(Element=action.Element, Point=action.Point)
        for i, pt1 in enumerate(action):
            x = action.multiplier_to_scc_root(i)
            pt2 = action[gabow.root_of(i)]
            assert func(pt1, x) == pt2
        break  # only test the first action


def test_action_number_of_generators(right_actions, left_actions):
    for action in right_actions + left_actions:
        num = action.number_of_generators()
        assert num > 2
        action.run()
        assert action.number_of_generators() == num


def test_action_reserve(right_actions, left_actions):
    for action in right_actions + left_actions:
        action.reserve(10)
        action.run()
        action.reserve(10)


def test_action_root_of_scc(right_actions, left_actions):
    for action in right_actions + left_actions:
        expected = BMat8(
            [[1, 1, 0, 1], [1, 1, 0, 0], [0, 1, 1, 0], [0, 1, 0, 0]]
        )
        assert action.root_of_scc(0) == expected
        assert action.root_of_scc(expected) == expected
        with pytest.raises(ValueError):
            action.root_of_scc("banana")
        break  # only test the first action


def test_action_word_graph(right_actions, left_actions):
    for action in right_actions + left_actions:
        wg = action.word_graph()
        assert wg.number_of_nodes() == len(action)
        assert (
            wg.number_of_edges() == len(action) * action.number_of_generators()
        )
