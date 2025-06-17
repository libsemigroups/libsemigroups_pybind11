# -*- coding: utf-8 -*-

# Copyright (c) 2025, James D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=missing-function-docstring

"""
This module contains some tests for the libsemigroups_pybind11 functionality
arising from forest.*pp in libsemigroups.
"""

import pytest

from libsemigroups_pybind11 import Forest, UNDEFINED


@pytest.fixture(name="f")
def forest_fixture():
    return Forest([UNDEFINED, 0, 1, 2, 3], [UNDEFINED, 0, 0, 0, 0])


def test_forest_return_policy(f):
    assert f.add_nodes(2) is f
    assert f.init(2) is f
    assert f.labels() is not f.labels()
    assert f.parents() is not f.parents()
    assert f.labels() is not f.labels()
    assert f.set_parent_and_label(1, 0, 10) is f
