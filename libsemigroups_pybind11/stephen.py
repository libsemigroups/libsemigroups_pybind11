# -*- coding: utf-8 -*-

# Copyright (c) 2022-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name, unused-import, fixme

"""
This page contains the documentation for various helper functions for
manipulating :any:`Stephen` objects. All such functions
are contained in the submodule ``stephen``.
"""

from typing_extensions import Self

from _libsemigroups_pybind11 import (
    PresentationWords as _PresentationWords,
    InversePresentationWords as _InversePresentationWords,
    StephenPresentationWords as _StephenPresentationWords,
    StephenInversePresentationWords as _StephenInversePresentationWords,
    # TODO rename with stephen_ prefix
    accepts as _accepts,
    dot as _dot,
    is_left_factor as _is_left_factor,
    left_factors as _left_factors,
    number_of_left_factors as _number_of_left_factors,
    number_of_words_accepted as _number_of_words_accepted,
    words_accepted as _words_accepted,
)

from libsemigroups_pybind11 import (
    Presentation as _Presentation,
    InversePresentation as _InversePresentation,
)

from .detail.decorators import copydoc as _copydoc
from .detail.cxx_wrapper import (
    CxxWrapper as _CxxWrapper,
    to_cxx as _to_cxx,
    copy_cxx_mem_fns as _copy_cxx_mem_fns,
    wrap_cxx_free_fn as _wrap_cxx_free_fn,
    register_cxx_wrapped_type as _register_cxx_wrapped_type,
)

########################################################################
# The Stephen python class
########################################################################


# TODO(2): Make this work with string presentations once it works
class Stephen(_CxxWrapper):
    __doc__ = _StephenPresentationWords.__doc__

    _py_template_params_to_cxx_type = {
        (_PresentationWords,): _StephenPresentationWords,
        (_InversePresentationWords,): _StephenInversePresentationWords,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_StephenPresentationWords.__init__)
    def __init__(self: Self, *args, **kwargs) -> None:
        # TODO remove the doc string
        """Construct a Stephen instance of the type specified by its arguments.

        Options for calling this function are:
        1  Stephen(presentation: PresentationWords)
        2  Stephen(presentation: InversePresentationWords)
        3  Stephen(presentation: StephenPresentationWords)
        4  Stephen(presentation: StephenInversePresentationWords)

        In cases 1 and 2 a new Stephen object is constructed with the given
        presentation. In cases 3 and 4 the Stephen object is constructed by copying
        an existing Stephen object. In cases 1 and 3 a StephenPresentationWords
        object is returned. In cases 2 and 4 a StephenInversePresentationWords
        object is returned.
        """
        super().__init__(*args, **kwargs)
        if _to_cxx(self) is not None:
            return

        if len(args) != 1:
            raise ValueError(f"expected 1 argument, but got {len(args)}")

        if isinstance(args[0], (_Presentation, _InversePresentation)):
            self.py_template_params = (type(_to_cxx(args[0])),)
        else:
            raise TypeError(
                "expected the 1st argument to have type 'Presentation', "
                f" or 'InversePresentation', but found {type(args[0])}",
            )
        self.init_cxx_obj(*args)

    def __imul__(self: Self, other: Self) -> Self:
        x = _to_cxx(self)
        x *= _to_cxx(other)
        return self


########################################################################
# Copy mem fns from sample C++ type and register types
########################################################################

_copy_cxx_mem_fns(_StephenPresentationWords, Stephen)
_register_cxx_wrapped_type(_StephenPresentationWords, Stephen)
_register_cxx_wrapped_type(_StephenInversePresentationWords, Stephen)

########################################################################
# Helpers
########################################################################

accepts = _wrap_cxx_free_fn(_accepts)
dot = _wrap_cxx_free_fn(_dot)
is_left_factor = _wrap_cxx_free_fn(_is_left_factor)
left_factors = _wrap_cxx_free_fn(_left_factors)
number_of_left_factors = _wrap_cxx_free_fn(_number_of_left_factors)
number_of_words_accepted = _wrap_cxx_free_fn(_number_of_words_accepted)
words_accepted = _wrap_cxx_free_fn(_words_accepted)
