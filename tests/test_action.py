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


from libsemigroups_pybind11 import RightAction, LeftAction, BMat8


def test_action_001():
    rows = RightAction(Point=BMat8, Element=BMat8)
    rows.add_seed(
        BMat8(
            [[1, 1, 1, 0], [1, 1, 0, 0], [0, 1, 0, 1], [0, 1, 0, 0]]
        ).row_space_basis()
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

    assert rows.size() == 553
    rows.init()

    rows.add_seed(
        BMat8(
            [[1, 1, 1, 0], [1, 1, 0, 0], [0, 1, 0, 1], [0, 1, 0, 0]]
        ).row_space_basis()
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

    assert rows.size() == 553

    cols = LeftAction(Point=BMat8, Element=BMat8)
    cols.add_seed(
        BMat8(
            [[1, 1, 1, 0], [1, 1, 0, 0], [0, 1, 0, 1], [0, 1, 0, 0]]
        ).col_space_basis()
    )

    cols.add_generator(
        BMat8([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]])
    )
    cols.add_generator(
        BMat8([[0, 1, 0, 0], [1, 0, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]])
    )
    cols.add_generator(
        BMat8([[0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1], [1, 0, 0, 0]])
    )
    cols.add_generator(
        BMat8([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [1, 0, 0, 1]])
    )
    cols.add_generator(
        BMat8([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 0]])
    )

    assert cols.size() == 553
    assert cols != rows
