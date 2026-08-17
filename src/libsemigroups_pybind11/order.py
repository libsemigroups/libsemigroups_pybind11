# Copyright (c) 2026 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""Python wrappers for word-ordering objects and comparison functions."""

from typing_extensions import Self as _Self

from _libsemigroups_pybind11 import (
    LenLexCmpDefault as _LenLexCmpDefault,
    LenLexCmpString as _LenLexCmpString,
    LenLexCmpWord as _LenLexCmpWord,
    LexCmpDefault as _LexCmpDefault,
    LexCmpString as _LexCmpString,
    LexCmpWord as _LexCmpWord,
    wr_cmp as _wr_cmp,
)

from .alphabet import Alphabet as _Alphabet
from .detail.cxx_wrapper import (
    CxxWrapper as _CxxWrapper,
    copy_cxx_mem_fns as _copy_cxx_mem_fns,
    register_cxx_wrapped_type as _register_cxx_wrapped_type,
    to_cxx as _to_cxx,
    wrap_cxx_free_fn as _wrap_cxx_free_fn,
)
from .detail.decorators import copydoc as _copydoc


class LexCmp(_CxxWrapper):
    __doc__ = _LexCmpString.__doc__

    _py_template_params_to_cxx_type = {
        (): _LexCmpDefault,
        (str,): _LexCmpString,
        (list[int],): _LexCmpWord,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
            strict=True,
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_LexCmpString.__init__, _LexCmpDefault.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, optional_kwargs=("alphabet",), **kwargs)
        if _to_cxx(self) is not None:
            return

        if len(args) > 1:
            raise TypeError(
                f"expected at most 1 positional argument, found {len(args)}"
            )

        if len(args) == 1 and kwargs:
            raise TypeError(
                'expected either 1 positional argument or the keyword argument "alphabet", '
                "but found both"
            )

        if len(args) == 1:
            alphabet = args[0]
        else:
            alphabet = kwargs.get("alphabet")

        if len(args) == 0 and "alphabet" not in kwargs:
            self.py_template_params = ()
            self.init_cxx_obj()
        elif isinstance(alphabet, _Alphabet):
            self.py_template_params = alphabet.py_template_params
            self.init_cxx_obj(alphabet)
        else:
            raise TypeError(
                f"expected the argument to be an Alphabet, but found {type(alphabet)}"
            )

    @_copydoc(_LexCmpString.__call__)
    def __call__(self: _Self, x: str | list[int], y: str | list[int]) -> bool:
        return super().__call__(x, y)


_copy_cxx_mem_fns(_LexCmpString, LexCmp)
_register_cxx_wrapped_type(_LexCmpDefault, LexCmp)
_register_cxx_wrapped_type(_LexCmpString, LexCmp)
_register_cxx_wrapped_type(_LexCmpWord, LexCmp)


class LenLexCmp(_CxxWrapper):
    __doc__ = _LenLexCmpString.__doc__

    _py_template_params_to_cxx_type = {
        (): _LenLexCmpDefault,
        (str,): _LenLexCmpString,
        (list[int],): _LenLexCmpWord,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
            strict=True,
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_LenLexCmpString.__init__, _LenLexCmpDefault.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, optional_kwargs=("alphabet",), **kwargs)
        if _to_cxx(self) is not None:
            return

        if len(args) > 1:
            raise TypeError(
                f"expected at most 1 positional argument, found {len(args)}"
            )

        if len(args) == 1 and kwargs:
            raise TypeError(
                'expected either 1 positional argument or the keyword argument "alphabet", '
                "but found both"
            )

        if len(args) == 1:
            alphabet = args[0]
        else:
            alphabet = kwargs.get("alphabet")

        if len(args) == 0 and "alphabet" not in kwargs:
            self.py_template_params = ()
            self.init_cxx_obj()
        elif isinstance(alphabet, _Alphabet):
            self.py_template_params = alphabet.py_template_params
            self.init_cxx_obj(alphabet)
        else:
            raise TypeError(
                f"expected the argument to be an Alphabet, but found {type(alphabet)}"
            )

    @_copydoc(_LenLexCmpString.__call__)
    def __call__(self: _Self, x: str | list[int], y: str | list[int]) -> bool:
        return super().__call__(x, y)


_copy_cxx_mem_fns(_LenLexCmpString, LenLexCmp)
_register_cxx_wrapped_type(_LenLexCmpDefault, LenLexCmp)
_register_cxx_wrapped_type(_LenLexCmpString, LenLexCmp)
_register_cxx_wrapped_type(_LenLexCmpWord, LenLexCmp)

wr_cmp = _wrap_cxx_free_fn(_wr_cmp)

__all__ = ["LenLexCmp", "LexCmp", "wr_cmp"]
