# -*- coding: utf-8 -*-

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name

"""
This package provides the user-facing python part of libsemigroups_pybind11 for
FroidurePin.
"""

from _libsemigroups_pybind11 import (
    Transf16,
    Transf1,
    Transf2,
    Transf4,
    FroidurePinTransf16,
    FroidurePinTransf1,
    FroidurePinTransf2,
    FroidurePinTransf4,
    PPerm16,
    PPerm1,
    PPerm2,
    PPerm4,
    FroidurePinPPerm16,
    FroidurePinPPerm1,
    FroidurePinPPerm2,
    FroidurePinPPerm4,
    Perm16,
    Perm1,
    Perm2,
    Perm4,
    FroidurePinPerm16,
    FroidurePinPerm1,
    FroidurePinPerm2,
    FroidurePinPerm4,
    FroidurePinTCE,
    FroidurePinKBE,
    Bipartition,
    FroidurePinBipartition,
    PBR,
    FroidurePinPBR,
    BMat8,
    FroidurePinBMat8,
    BMat,
    FroidurePinBMat,
    IntMat,
    FroidurePinIntMat,
    MaxPlusMat,
    FroidurePinMaxPlusMat,
    MinPlusMat,
    FroidurePinMinPlusMat,
    ProjMaxPlusMat,
    FroidurePinProjMaxPlusMat,
    MaxPlusTruncMat,
    FroidurePinMaxPlusTruncMat,
    MinPlusTruncMat,
    FroidurePinMinPlusTruncMat,
    NTPMat,
    FroidurePinNTPMat,
)

_ElementToFroidurePin = {
    Transf16: FroidurePinTransf16,
    Transf1: FroidurePinTransf1,
    Transf2: FroidurePinTransf2,
    Transf4: FroidurePinTransf4,
    PPerm16: FroidurePinPPerm16,
    PPerm1: FroidurePinPPerm1,
    PPerm2: FroidurePinPPerm2,
    PPerm4: FroidurePinPPerm4,
    Perm16: FroidurePinPerm16,
    Perm1: FroidurePinPerm1,
    Perm2: FroidurePinPerm2,
    Perm4: FroidurePinPerm4,
    Bipartition: FroidurePinBipartition,
    PBR: FroidurePinPBR,
    BMat8: FroidurePinBMat8,
    BMat: FroidurePinBMat,
    IntMat: FroidurePinIntMat,
    MaxPlusMat: FroidurePinMaxPlusMat,
    MinPlusMat: FroidurePinMinPlusMat,
    ProjMaxPlusMat: FroidurePinProjMaxPlusMat,
    MaxPlusTruncMat: FroidurePinMaxPlusTruncMat,
    MinPlusTruncMat: FroidurePinMinPlusTruncMat,
    NTPMat: FroidurePinNTPMat,
}

_FroidurePinTypes = {
    FroidurePinTransf16: True,
    FroidurePinTransf1: True,
    FroidurePinTransf2: True,
    FroidurePinTransf4: True,
    FroidurePinPPerm16: True,
    FroidurePinPPerm1: True,
    FroidurePinPPerm2: True,
    FroidurePinPPerm4: True,
    FroidurePinPerm16: True,
    FroidurePinPerm1: True,
    FroidurePinPerm2: True,
    FroidurePinPerm4: True,
    FroidurePinBipartition: True,
    FroidurePinPBR: True,
    FroidurePinBMat8: True,
    FroidurePinBMat: True,
    FroidurePinIntMat: True,
    FroidurePinMaxPlusMat: True,
    FroidurePinMinPlusMat: True,
    FroidurePinProjMaxPlusMat: True,
    FroidurePinMaxPlusTruncMat: True,
    FroidurePinMinPlusTruncMat: True,
    FroidurePinNTPMat: True,
    FroidurePinKBE: True,
    FroidurePinTCE: True,
}


def FroidurePin(*args):
    """
    Construct a FroidurePin instance of the type specified by its generators.
    """
    if len(args) == 0:
        raise ValueError("expected at least 1 argument, found 0")
    if type(args[0]) in _FroidurePinTypes:
        return type(args[0])(args[0])
    if isinstance(args[0], list):
        args_ = args[0]
        type_ = type(args_[0])
    else:
        args_ = args
        type_ = type(args[0])
    return _ElementToFroidurePin[type_](args_)
