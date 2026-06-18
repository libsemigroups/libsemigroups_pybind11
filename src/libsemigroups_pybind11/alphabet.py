# Copyright (c) 2026 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# No doc string, this file is documented in alphabet-helpers.rst

# pylint: disable=missing-module-docstring

import collections

from typing_extensions import Self as _Self

from _libsemigroups_pybind11 import (
    AlphabetString as _AlphabetString,
    AlphabetWord as _AlphabetWord,
    alphabet_first_unused_letter as _alphabet_first_unused_letter,
    validate as _validate,
)

from .detail.cxx_wrapper import (
    CxxWrapper as _CxxWrapper,
    copy_cxx_mem_fns as _copy_cxx_mem_fns,
    register_cxx_wrapped_type as _register_cxx_wrapped_type,
    to_cxx as _to_cxx,
    wrap_cxx_free_fn as _wrap_cxx_free_fn,
)
from .detail.decorators import copydoc as _copydoc


class Alphabet(_CxxWrapper):
    __doc__ = _AlphabetString.__doc__

    _py_template_params_to_cxx_type = {(str,): _AlphabetString, (list[int],): _AlphabetWord}

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
            strict=True,
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @staticmethod
    def _validate_word_type(word: type) -> None:
        if word not in (str, list[int]):
            raise ValueError(
                f'the keyword argument "word" must be str or list[int], but found {word}'
            )

    @_copydoc(_AlphabetString.__init__, _AlphabetWord.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        """__init__(self: Alphabet, *, word: type) -> None

        Construct an empty alphabet.

        :Keyword Arguments:
            * **word** (*type*) -- the type of words to use. Must be either
              ``str`` or ``list[int]``.
        """
        super().__init__(*args, optional_kwargs=("word",), **kwargs)
        if _to_cxx(self) is not None:
            return

        if len(args) > 1:
            raise TypeError(f"expected at most 1 positional argument, found {len(args)}")

        if len(args) == 1 and "word" in kwargs and not isinstance(args[0], int):
            raise TypeError(
                'expected either 1 positional argument or the keyword argument "word" '
                f"but found both, with argument type {type(args[0])}"
            )

        if len(args) == 0:
            if "word" not in kwargs:
                raise TypeError(
                    f'expected the keyword argument "word", but found {tuple(kwargs.keys())}'
                )
            self._validate_word_type(kwargs["word"])
            self.py_template_params = (kwargs["word"],)
            self.init_cxx_obj()
            return

        if isinstance(args[0], Alphabet):
            self.py_template_params = args[0].py_template_params
        elif isinstance(args[0], str):
            self.py_template_params = (str,)
        elif isinstance(args[0], list):
            if not all(isinstance(x, int) for x in args[0]):
                raise TypeError("expected the argument to consist of int values")
            self.py_template_params = (list[int],)
        elif isinstance(args[0], int):
            if "word" not in kwargs:
                raise TypeError('expected the keyword argument "word" when constructing by size')
            self._validate_word_type(kwargs["word"])
            self.py_template_params = (kwargs["word"],)
        else:
            raise TypeError(
                f"expected the argument to have type one of (str, list[int], int) "
                f"but found {type(args[0])}"
            )

        self.init_cxx_obj(*args)

    def __contains__(self: _Self, val: str | int) -> bool:
        return _to_cxx(self).contains(val)

    def __eq__(self: _Self, other: _Self) -> bool:
        return _to_cxx(self) == _to_cxx(other)

    def __len__(self: _Self) -> int:
        return len(_to_cxx(self))

    def __str__(self: _Self) -> str:
        return str(_to_cxx(self))

    def __iter__(self: _Self) -> collections.abc.Iterator[int | str]:
        return iter(_to_cxx(self))


_copy_cxx_mem_fns(_AlphabetString, Alphabet)
_register_cxx_wrapped_type(_AlphabetString, Alphabet)
_register_cxx_wrapped_type(_AlphabetWord, Alphabet)

first_unused_letter = _wrap_cxx_free_fn(_alphabet_first_unused_letter)
validate = _wrap_cxx_free_fn(_validate)

__all__ = ["Alphabet", "first_unused_letter", "validate"]
