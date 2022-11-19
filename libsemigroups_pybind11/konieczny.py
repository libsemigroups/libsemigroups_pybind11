# -*- coding: utf-8 -*-

# Copyright (c) 2022, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name, duplicate-code

"""
This package provides the user-facing python part of libsemigroups_pybind11 for
Konieczny.
"""

from _libsemigroups_pybind11 import (
    Transf16,
    Transf1,
    Transf2,
    Transf4,
    KoniecznyTransf16,
    KoniecznyTransf1,
    KoniecznyTransf2,
    KoniecznyTransf4,
    BMat,
    BMat8,
    KoniecznyBMat8,
    KoniecznyBMat,
    PPerm16,
    PPerm1,
    PPerm2,
    PPerm4,
    KoniecznyPPerm16,
    KoniecznyPPerm1,
    KoniecznyPPerm2,
    KoniecznyPPerm4,
)

_ElementToKonieczny = {
    Transf16: KoniecznyTransf16,
    Transf1: KoniecznyTransf1,
    Transf2: KoniecznyTransf2,
    Transf4: KoniecznyTransf4,
    PPerm16: KoniecznyPPerm16,
    PPerm1: KoniecznyPPerm1,
    PPerm2: KoniecznyPPerm2,
    PPerm4: KoniecznyPPerm4,
    BMat8: KoniecznyBMat8,
    BMat: KoniecznyBMat,
}

_KoniecznyTypes = {
    KoniecznyTransf16: True,
    KoniecznyTransf1: True,
    KoniecznyTransf2: True,
    KoniecznyTransf4: True,
    KoniecznyPPerm16: True,
    KoniecznyPPerm1: True,
    KoniecznyPPerm2: True,
    KoniecznyPPerm4: True,
    KoniecznyBMat8: True,
    KoniecznyBMat: True,
}


def Konieczny(*args):
    """
    Construct a Konieczny instance of the type specified by its generators.
    """
    if len(args) == 0:
        raise ValueError("expected at least 1 argument, found 0")
    if type(args[0]) in _KoniecznyTypes:
        return type(args[0])(args[0])
    if isinstance(args[0], list):
        args_ = args[0]
        type_ = type(args_[0])
    else:
        args_ = args
        type_ = type(args[0])
    return _ElementToKonieczny[type_](args_)
