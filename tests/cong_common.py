# -*- coding: utf-8 -*-
# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some functions used in tests for derived classes of
CongruenceCommon.
"""

from libsemigroups_pybind11 import (
    Presentation,
    congruence_kind,
    presentation,
)


def check_congruence_common_return_policy(
    TestType,
):  # pylint: disable=missing-function-docstring, invalid-name
    p = Presentation("ab")
    presentation.add_rule(p, "abab", "a" * 6)
    presentation.add_rule(p, "ba", "ababbb")

    c = TestType(congruence_kind.twosided, p)
    assert c.init() is c
    assert c.init(congruence_kind.twosided, p) is c
    assert c.add_generating_pair("a", "b") is c

    assert c.presentation() is c.presentation()
    assert c.generating_pairs() is not c.generating_pairs()
    return c
