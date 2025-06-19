# -*- coding: utf-8 -*-

# Copyright (c) 2025 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This submodule collects some common aspects of the classes Congruence,
Kambites, KnuthBendix, and ToddCoxeter.
"""

from typing_extensions import Self

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module
    congruence_kind as _congruence_kind,
)

from libsemigroups_pybind11.presentation import Presentation as _Presentation

from .cxx_wrapper import CxxWrapper as _CxxWrapper, to_cxx as _to_cxx


class CongruenceCommon(_CxxWrapper):
    """
    A base class for Congruence, Kambites, KnuthBendix, and ToddCoxeter,
    collecting the common behaviour required by __init__
    """

    def __init__(self: Self, *args, wrong_num_args_msg="", **kwargs) -> None:
        # super().__init__ checks if there are unexpected kwargs, sets up
        # _cxx_type_to_py_template_params, and sets _cxx_obj if
        # the unique argument is a cxx_obj of type in _all_wrapped_cxx_types
        super().__init__(*args, optional_kwargs=("word",), **kwargs)
        if _to_cxx(self) is not None:
            # constructed from args[0] of type belonging to _all_wrapped_cxx_types
            return

        # checks that we have 0 args and 1 kwarg or 2 args and 0 kwargs, and
        # that the types of these are correct
        if len(args) not in (0, 2):
            raise TypeError(f"expected 0 or 2 positional arguments, found {len(args)}")
        if len(args) != 0 and len(kwargs) != 0:
            if len(wrong_num_args_msg) == 0:
                wrong_num_args_msg = (
                    'expected 2 positional arguments or 1 keyword arguments ("word",) '
                    f"but found {len(args)} and {len(kwargs)}"
                )
            raise TypeError(wrong_num_args_msg)
        if len(args) == 0:
            if kwargs["word"] not in (str, list[int]):
                raise ValueError(
                    'expected the keyword argument "word" to be "str" or "list[int]" '
                    f"but found {kwargs['word']}"
                )
            self.py_template_params = (kwargs["word"],)
        elif not isinstance(args[0], _congruence_kind):
            raise TypeError(
                f"expected the 1st argument to be congruence_kind but found {type(args[0])}"
            )
