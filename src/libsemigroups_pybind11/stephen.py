# Copyright (c) 2022-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.


"""This page contains the documentation for various helper functions for
manipulating :any:`Stephen` objects. All such functions
are contained in the submodule ``stephen``.
"""

from typing_extensions import Self as _Self

from _libsemigroups_pybind11 import (
    InversePresentationWord as _InversePresentationWord,
    PresentationWord as _PresentationWord,
    StephenInversePresentationWord as _StephenInversePresentationWord,
    StephenPresentationWord as _StephenPresentationWord,
    stephen_accepts as _stephen_accepts,
    stephen_dot as _stephen_dot,
    stephen_is_left_factor as _stephen_is_left_factor,
    stephen_left_factors as _stephen_left_factors,
    stephen_number_of_left_factors as _stephen_number_of_left_factors,
    stephen_number_of_words_accepted as _stephen_number_of_words_accepted,
    stephen_words_accepted as _stephen_words_accepted,
)

from .detail.cxx_wrapper import (
    CxxWrapper as _CxxWrapper,
    copy_cxx_mem_fns as _copy_cxx_mem_fns,
    register_cxx_wrapped_type as _register_cxx_wrapped_type,
    to_cxx as _to_cxx,
    wrap_cxx_free_fn as _wrap_cxx_free_fn,
)
from .detail.decorators import copydoc as _copydoc
from .presentation import InversePresentation as _InversePresentation, Presentation as _Presentation

########################################################################
# The Stephen python class
########################################################################


# TODO(2): Make this work with string presentations once it works
class Stephen(_CxxWrapper):
    __doc__ = _StephenPresentationWord.__doc__

    _py_template_params_to_cxx_type = {
        (_PresentationWord,): _StephenPresentationWord,
        (_InversePresentationWord,): _StephenInversePresentationWord,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
            strict=True,
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_StephenPresentationWord.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, **kwargs)
        if _to_cxx(self) is not None:
            return

        if len(args) != 1:
            raise TypeError(f"expected 1 argument, but got {len(args)}")

        if isinstance(args[0], (_Presentation, _InversePresentation)):
            self.py_template_params = (type(_to_cxx(args[0])),)
        else:
            raise TypeError(
                "expected the 1st argument to have type 'Presentation', "
                f" or 'InversePresentation', but found {type(args[0])}"
            )
        self.init_cxx_obj(*args)

    def __imul__(self: _Self, other: _Self) -> _Self:
        x = _to_cxx(self)
        x *= _to_cxx(other)
        return self


########################################################################
# Copy mem fns from sample C++ type and register types
########################################################################

_copy_cxx_mem_fns(_StephenPresentationWord, Stephen)
_register_cxx_wrapped_type(_StephenPresentationWord, Stephen)
_register_cxx_wrapped_type(_StephenInversePresentationWord, Stephen)

########################################################################
# Helpers
########################################################################

accepts = _wrap_cxx_free_fn(_stephen_accepts)
dot = _wrap_cxx_free_fn(_stephen_dot)
is_left_factor = _wrap_cxx_free_fn(_stephen_is_left_factor)
left_factors = _wrap_cxx_free_fn(_stephen_left_factors)
number_of_left_factors = _wrap_cxx_free_fn(_stephen_number_of_left_factors)
number_of_words_accepted = _wrap_cxx_free_fn(_stephen_number_of_words_accepted)
words_accepted = _wrap_cxx_free_fn(_stephen_words_accepted)
