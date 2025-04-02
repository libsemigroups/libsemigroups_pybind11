# -*- coding: utf-8 -*-

# Copyright (c) 2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint:disable=no-name-in-module

"""
This submodule provides some helper functions for congruence-like objects in
libsemigroups_pybind11.
"""

from typing import List
from _libsemigroups_pybind11 import congruence_kind as _congruence_kind


def raise_if_bad_args(*args, msg2="", **kwargs):
    """Raise an error if *args* are not valid"""
    if len(args) not in (0, 2):
        raise TypeError(
            f"expected 0 or 2 positional arguments, found {len(args)}"
        )
    if len(args) != 0 and len(kwargs) != 0:
        if msg2 == "":
            msg2 = (
                'expected 2 positional arguments or 1 keyword arguments ("Word") '
                + f"but found {len(args)} and {len(kwargs)}"
            )
        raise TypeError(msg2)
    if len(args) == 0:
        if "Word" not in kwargs or len(kwargs) > 1:
            raise TypeError(
                f'expected keyword argument "Word", but found {list(kwargs.keys())}'
            )
        if kwargs["Word"] not in (str, List[int]):
            raise ValueError(
                'expected the keyword argument "Word" to be str or List[int] '
                f"but found {kwargs['Word']}"
            )
    elif not isinstance(args[0], _congruence_kind):
        raise TypeError(
            f"expected the 1st argument to be congruence_kind but found {type(args[0])}"
        )
