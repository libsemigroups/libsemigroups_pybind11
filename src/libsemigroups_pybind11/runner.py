# -*- coding: utf-8 -*-

# Copyright (c) 2025, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.


"""
This package provides a class named Runner that shadows the same class in
libsemigroups. It exists for those python classes in libsemigroups_pybind11
that wrap multiple C++ types.
"""

from abc import ABC as _ABC
from typing_extensions import Self as _Self

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module
    Runner as _Runner,
    Reporter as _Reporter,
)

from .detail.cxx_wrapper import (
    CxxWrapper as _CxxWrapper,
    copy_cxx_mem_fns as _copy_cxx_mem_fns,
    to_cxx as _to_cxx,
)

from .detail.decorators import copydoc as _copydoc


# It is possible to create Reporter objects in C++ but not here due to the way
# that CxxWrapper works.
class Reporter(
    _ABC, _CxxWrapper
):  # pylint: disable=too-few-public-methods, missing-class-docstring
    __doc__ = _Reporter.__doc__


_copy_cxx_mem_fns(_Reporter, Reporter)


# This class exists so that python classes derived from CxxWrapper can
# also be derived from a class call Runner. I.e. Action objects in
# libsemigroups C++ are Runners, Action objects in python are
# CxxWrappers, but should also inherit from Runner, they cannot inherit
# from the C++ runner (because this already has run etc mem fns which
# won't work for python classes) This class is used so
# that classes such as Action have the same interface as Runner C++ objects.
class Runner(Reporter):  # pylint: disable=missing-class-docstring
    __doc__ = _Runner.__doc__


_copy_cxx_mem_fns(_Runner, Runner)
