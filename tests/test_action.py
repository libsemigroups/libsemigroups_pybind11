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

# pylint: disable=missing-function-docstring

import pytest

from libsemigroups_pybind11 import (
    RightAction,
    LeftAction,
    Action,
    PPerm,
    BMat8,
    side,
    Transf,
    ReportGuard,
    LibsemigroupsError,
)
from libsemigroups_pybind11.bmat8 import row_space_basis, col_space_basis


@pytest.fixture(name="right_actions")
def fixture_right_actions():
    seed = row_space_basis(
        BMat8([[1, 1, 1, 0], [1, 1, 0, 0], [0, 1, 0, 1], [0, 1, 0, 0]])
    )
    result = [
        RightAction(
            seeds=[seed],
            generators=[
                BMat8([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]])
            ],
        )
    ]
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

    result.append(
        RightAction(
            seeds=[PPerm.one(16)],
            generators=[
                PPerm(
                    [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
                    [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0],
                    16,
                )
            ],
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

    result.append(
        RightAction(
            seeds=[list(range(16))],
            generators=[
                PPerm(
                    [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
                    [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0],
                    16,
                )
            ],
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

    result.append(
        RightAction(
            seeds=[list(range(16))],
            generators=[
                Transf([0, 0] + list(range(1, 15))),
                Transf([1, 0] + list(range(2, 16))),
                Transf(list(range(1, 16)) + [0]),
            ],
        )
    )

    return result


@pytest.fixture(name="left_actions")
def fixture_left_actions():
    seed = col_space_basis(
        BMat8([[1, 1, 1, 0], [1, 1, 0, 0], [0, 1, 0, 1], [0, 1, 0, 0]])
    )
    result = [
        LeftAction(
            generators=[
                BMat8([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]])
            ],
            seeds=[seed],
        )
    ]
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

    result.append(
        LeftAction(
            seeds=[list(range(16))],
            generators=[
                PPerm(
                    [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
                    [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0],
                    16,
                )
            ],
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


@pytest.fixture(name="extreme_left_actions")
def fixture_extreme_left_actions():
    result = []
    result.append(
        LeftAction(
            seeds=[list(range(16))],
            generators=[
                Transf([0, 0] + list(range(1, 15))),
                Transf([1, 0] + list(range(2, 16))),
                Transf(list(range(1, 16)) + [0]),
            ],
        )
    )
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

    with pytest.raises(LibsemigroupsError):
        right.add_generator(
            PPerm(
                [],
                [],
                15,
            )
        )
    with pytest.raises(TypeError):
        right.add_seed(BMat8(0))

    x = PPerm([], [], 16)
    assert x in right
    assert right.position(x) == 43748
    assert len(right) == 65536

    # Check that adding an existing element as a seed does nothing
    right.add_seed(
        PPerm(
            [],
            [],
            16,
        )
    )

    assert len(right) == 65536


def test_action_pperm2():
    for n in [199, 299, 69999]:
        act = RightAction(
            generators=[PPerm(range(0, n - 1), range(1, n), n)], seeds=[[0]]
        )
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
        Action(generators=PPerm)

    with pytest.raises(TypeError):
        Action(generators=None, seeds=None)

    with pytest.raises(KeyError):
        Action(
            generators=[PPerm], seeds=[BMat8], func=RightAction, side=side.left
        )

    right = RightAction(
        generators=[
            PPerm(
                [0, 1, 2, 3],
                [1, 2, 3, 4],
                16,
            )
        ],
        seeds=[PPerm.one(16)],
    )
    assert right.size() == 6
    right.init()
    assert right.size() == 0
    right.add_generator(
        PPerm(
            [0, 1, 2, 3],
            [1, 2, 3, 4],
            16,
        )
    )
    assert right.size() == 0

    right = RightAction(
        generators=[
            PPerm(
                [0, 1, 2, 3],
                [1, 2, 3, 4],
                5,
            )
        ],
        seeds=[PPerm.one(17)],
    )

    assert repr(right) == "<incomplete right action with 1 generator, 1 point>"
    with pytest.raises(TypeError):
        right.add_generator(BMat8(0))


def test_action_cache_scc_multipliers(right_actions, left_actions):
    for action in right_actions + left_actions:
        assert not action.cache_scc_multipliers()
        action.cache_scc_multipliers(True)
    for action in right_actions + left_actions:
        assert action.cache_scc_multipliers()
        with pytest.raises(TypeError):
            action.cache_scc_multipliers("hello")
        with pytest.raises(TypeError):
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
    assert x in act
    assert right_actions[1].current_size() == 43749


def test_action_empty(right_actions, left_actions):
    for action in right_actions + left_actions:
        assert not action.empty()


def test_action_iterator(right_actions, left_actions):
    for action in right_actions + left_actions:
        for x in action:
            assert x in action


def test_action_multiplier_from_scc_root(right_actions, left_actions):
    for action in right_actions + left_actions:
        action.run()
        gabow = action.scc()
        for i, pt1 in enumerate(action):
            x = action.multiplier_from_scc_root(i)
            pt2 = action[gabow.root_of(i)]
            assert action.apply(pt2, x) == pt1
        break  # only test the first action


def test_action_multiplier_to_scc_root(right_actions, left_actions):
    for action in right_actions + left_actions:
        action.run()
        gabow = action.scc()
        for i, pt1 in enumerate(action):
            x = action.multiplier_to_scc_root(i)
            pt2 = action[gabow.root_of(i)]
            assert action.apply(pt1, x) == pt2
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
            [[1, 1, 1, 0], [1, 1, 0, 0], [0, 1, 0, 1], [0, 1, 0, 0]]
        )
        assert action.root_of_scc(0) == expected
        assert action.root_of_scc(expected) == expected
        with pytest.raises(TypeError):
            action.root_of_scc("banana")
        break  # only test the first action


def test_action_word_graph(right_actions, left_actions):
    for action in right_actions + left_actions:
        wg = action.word_graph()
        assert wg.number_of_nodes() == len(action)
        assert (
            wg.number_of_edges() == len(action) * action.number_of_generators()
        )


def test_action_return_policy(right_actions):
    action = right_actions[0]
    assert action.reserve(10) is action
    x = BMat8([[1, 1, 1, 0], [1, 1, 0, 0], [0, 1, 0, 1], [0, 0, 0, 0]])
    seed = row_space_basis(x)
    assert action.add_seed(seed) is action
    assert action.add_generator(x) is action
    assert action.cache_scc_multipliers(False) is action
    assert action.init() is action
