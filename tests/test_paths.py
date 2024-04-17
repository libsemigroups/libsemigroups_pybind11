# Copyright (c) 2024, M. T. Whyte
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This file contains tests for Paths and related functionality in
libsemigroups_pybind11.
"""

# pylint: disable=no-name-in-module, missing-function-docstring, invalid-name,
# pylint: disable=duplicate-code, too-many-lines


import pytest

from libsemigroups_pybind11 import (
    LibsemigroupsError,
    order,
    Paths,
    WordGraph,
)


def test_001():
    w = WordGraph(0)
    n = 100
    w.add_nodes(n)
    w.add_to_out_degree(2)
    for i in range(n - 1):
        w.set_target(i, i % 2, i + 1)

    p = Paths(w)

    with pytest.raises(LibsemigroupsError):
        p.count()

    assert p.order() == order.shortlex
    p.order(order.lex)
    assert p.order() == order.lex

    p.source(0)
    assert p.count() == 100
    assert p.get() == []
    p.next()
    assert p.get() == [0]
    p.next()
    assert p.get() == [0, 1]
    assert not p.at_end()

    p.source(50)
    assert p.count() == 50
    assert p.min_length() == 0

    p.source(10)
    p.target(20)
    assert p.source() == 10
    assert p.target() == 20
    assert p.count() == 1
    p.max_length(5)
    assert p.count() == 0
    p.max_length(11)
    assert p.count() == 1
    p.min_length(11)
    assert p.count() == 0
    p.min_length(0)
