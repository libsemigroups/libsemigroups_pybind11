# -*- coding: utf-8 -*-

# Copyright (c) 2024 R. Cirpons
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.


"""
The full API for the helper functions is given below.
"""

from typing_extensions import Self as _Self

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module
    MinimalRepOrc as _MinimalRepOrc,
    RepOrc as _RepOrc,
    Sims1 as _Sims1,
    Sims2 as _Sims2,
    SimsRefinerFaithful as _SimsRefinerFaithful,
    SimsRefinerIdeals as _SimsRefinerIdeals,
    sims_is_maximal_right_congruence as _is_maximal_right_congruence,
    sims_is_right_congruence as _is_right_congruence,
    sims_is_right_congruence_of_dual as _is_right_congruence_of_dual,
    sims_is_two_sided_congruence as _is_two_sided_congruence,
    sims_poset as _poset,
    sims_right_generating_pairs as _right_generating_pairs,
    sims_two_sided_generating_pairs as _two_sided_generating_pairs,
)

from .detail.cxx_wrapper import (
    wrap_cxx_free_fn as _wrap_cxx_free_fn,
    CxxWrapper as _CxxWrapper,
    to_cxx as _to_cxx,
    copy_cxx_mem_fns as _copy_cxx_mem_fns,
    register_cxx_wrapped_type as _register_cxx_wrapped_type,
)

from .detail.decorators import copydoc as _copydoc

from .presentation import Presentation as _Presentation


class _SimsBase(_CxxWrapper):
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, optional_kwargs=("word",), **kwargs)
        if _to_cxx(self) is not None:
            return
        if len(args) not in (0, 1):
            raise TypeError(
                f"expected 0 or 1 positional arguments but found {len(args)}"
            )
        if len(args) != 0 and len(kwargs) != 0:
            raise TypeError(
                "expected either 1 positional argument or 1"
                f"keyword argument (but not both), but found {len(args)} and {len(kwargs)}"
            )
        if len(args) == 0:
            if "word" not in kwargs:
                raise TypeError(
                    f'expected the keyword argument "word", but found {tuple(kwargs.keys())}'
                )
            # for when Sims has a facade
            # if kwargs["word"] not in (str, list[int]):
            #     raise ValueError(
            #         'the keyword argument "word" must be str or list[int], '
            #         f"but found {kwargs['word']}"
            #     )
            if kwargs["word"] != list[int]:
                raise ValueError(
                    f'the keyword argument "word" must be list[int], but found {kwargs["word"]}'
                )

        if len(args) == 0:
            self.py_template_params = (kwargs["word"],)
        else:
            if isinstance(args[0], _Presentation):
                self.py_template_params = args[0].py_template_params
            else:
                raise TypeError(
                    f"expected the 1st argument to be a Presentation, but found {type(args[0])}"
                )
        self.init_cxx_obj(*args)


########################################################################
# Sims1 wrapper class
########################################################################


class Sims1(_SimsBase):  # pylint: disable=missing-class-docstring
    __doc__ = _Sims1.__doc__

    _py_template_params_to_cxx_type = {
        (list[int],): _Sims1,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_Sims1.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, **kwargs)


_copy_cxx_mem_fns(_Sims1, Sims1)
_register_cxx_wrapped_type(_Sims1, Sims1)

########################################################################
# Sims2 wrapper class
########################################################################


class Sims2(_SimsBase):  # pylint: disable=missing-class-docstring
    __doc__ = _Sims2.__doc__

    _py_template_params_to_cxx_type = {
        (list[int],): _Sims2,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_Sims2.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, **kwargs)


_copy_cxx_mem_fns(_Sims2, Sims2)
_register_cxx_wrapped_type(_Sims2, Sims2)

########################################################################
# RepOrc class wrapper
########################################################################


class RepOrc(_SimsBase):  # pylint: disable=missing-class-docstring
    __doc__ = _RepOrc.__doc__

    _py_template_params_to_cxx_type = {
        (list[int],): _RepOrc,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_RepOrc.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, **kwargs)


_copy_cxx_mem_fns(_RepOrc, RepOrc)
_register_cxx_wrapped_type(_RepOrc, RepOrc)

########################################################################
# MinimalRepOrc class wrapper
########################################################################


class MinimalRepOrc(_SimsBase):  # pylint: disable=missing-class-docstring
    __doc__ = _MinimalRepOrc.__doc__

    _py_template_params_to_cxx_type = {
        (list[int],): _MinimalRepOrc,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_MinimalRepOrc.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, **kwargs)


_copy_cxx_mem_fns(_MinimalRepOrc, MinimalRepOrc)
_register_cxx_wrapped_type(_MinimalRepOrc, MinimalRepOrc)


########################################################################
# SimsRefinerIdeals class wrapper
########################################################################


class SimsRefinerIdeals(_SimsBase):  # pylint: disable=missing-class-docstring
    __doc__ = _SimsRefinerIdeals.__doc__

    _py_template_params_to_cxx_type = {
        (list[int],): _SimsRefinerIdeals,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_SimsRefinerIdeals.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, **kwargs)

    @_copydoc(_SimsRefinerIdeals.__call__)
    def __call__(self: _Self, *args, **kwargs) -> bool:
        return super().__call__(*args, **kwargs)


_copy_cxx_mem_fns(_SimsRefinerIdeals, SimsRefinerIdeals)
_register_cxx_wrapped_type(_SimsRefinerIdeals, SimsRefinerIdeals)

########################################################################
# SimsRefinerFaithful class wrapper
########################################################################


class SimsRefinerFaithful(
    _CxxWrapper
):  # pylint: disable=missing-class-docstring
    __doc__ = _SimsRefinerFaithful.__doc__

    _py_template_params_to_cxx_type = {
        (list[int],): _SimsRefinerFaithful,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_SimsRefinerFaithful.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, optional_kwargs=("word",), **kwargs)
        if _to_cxx(self) is not None:
            return

        if len(args) not in (0, 1):
            raise TypeError(
                f"expected 0 or 1 positional arguments but found {len(args)}"
            )
        if len(args) != 0 and len(kwargs) != 0:
            raise TypeError(
                "expected either 1 positional argument or 1"
                f"keyword argument (but not both), but found {len(args)} and {len(kwargs)}"
            )
        if len(args) == 0:
            if "word" not in kwargs:
                raise TypeError(
                    f'expected the keyword argument "word", but found {tuple(kwargs.keys())}'
                )
            if kwargs["word"] != list[int]:
                raise ValueError(
                    f'the keyword argument "word" must be list[int], but found {kwargs["word"]}'
                )
        if len(args) == 0:
            self.py_template_params = (kwargs["word"],)
        else:
            if (
                isinstance(args[0], list)
                and all(isinstance(x, list) for x in args[0])
                and all(isinstance(y, int) for x in args[0] for y in x)
            ):
                self.py_template_params = (list[int],)
            else:
                raise TypeError(
                    "expected the 1st argument to be a list[list[int]]"
                )
        self.init_cxx_obj(*args)

    @_copydoc(_SimsRefinerFaithful.__call__)
    def __call__(self: _Self, *args, **kwargs) -> bool:
        return super().__call__(*args, **kwargs)


_copy_cxx_mem_fns(_SimsRefinerFaithful, SimsRefinerFaithful)
_register_cxx_wrapped_type(_SimsRefinerFaithful, SimsRefinerFaithful)

########################################################################
# Helper functions
########################################################################

right_generating_pairs = _wrap_cxx_free_fn(_right_generating_pairs)
two_sided_generating_pairs = _wrap_cxx_free_fn(_two_sided_generating_pairs)
is_right_congruence = _wrap_cxx_free_fn(_is_right_congruence)
is_right_congruence_of_dual = _wrap_cxx_free_fn(_is_right_congruence_of_dual)
is_two_sided_congruence = _wrap_cxx_free_fn(_is_two_sided_congruence)
is_maximal_right_congruence = _wrap_cxx_free_fn(_is_maximal_right_congruence)
poset = _wrap_cxx_free_fn(_poset)
