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
    LenWtLexCmpDefault as _LenWtLexCmpDefault,
    LenWtLexCmpString as _LenWtLexCmpString,
    LenWtLexCmpWord as _LenWtLexCmpWord,
    LexCmpDefault as _LexCmpDefault,
    LexCmpString as _LexCmpString,
    LexCmpWord as _LexCmpWord,
    RevLenLexCmpDefault as _RevLenLexCmpDefault,
    RevLenLexCmpString as _RevLenLexCmpString,
    RevLenLexCmpWord as _RevLenLexCmpWord,
    RevLenWtLexCmpDefault as _RevLenWtLexCmpDefault,
    RevLenWtLexCmpString as _RevLenWtLexCmpString,
    RevLenWtLexCmpWord as _RevLenWtLexCmpWord,
    RevLexCmpDefault as _RevLexCmpDefault,
    RevLexCmpString as _RevLexCmpString,
    RevLexCmpWord as _RevLexCmpWord,
    RevRPOCmpDefault as _RevRPOCmpDefault,
    RevRPOCmpString as _RevRPOCmpString,
    RevRPOCmpWord as _RevRPOCmpWord,
    RevWrCmpDefault as _RevWrCmpDefault,
    RevWrCmpString as _RevWrCmpString,
    RevWrCmpWord as _RevWrCmpWord,
    RevWtLenLexCmpDefault as _RevWtLenLexCmpDefault,
    RevWtLenLexCmpString as _RevWtLenLexCmpString,
    RevWtLenLexCmpWord as _RevWtLenLexCmpWord,
    RevWtLexCmpDefault as _RevWtLexCmpDefault,
    RevWtLexCmpString as _RevWtLexCmpString,
    RevWtLexCmpWord as _RevWtLexCmpWord,
    RPOCmpDefault as _RPOCmpDefault,
    RPOCmpString as _RPOCmpString,
    RPOCmpWord as _RPOCmpWord,
    WrCmpDefault as _WrCmpDefault,
    WrCmpString as _WrCmpString,
    WrCmpWord as _WrCmpWord,
    WtLenLexCmpDefault as _WtLenLexCmpDefault,
    WtLenLexCmpString as _WtLenLexCmpString,
    WtLenLexCmpWord as _WtLenLexCmpWord,
    WtLexCmpDefault as _WtLexCmpDefault,
    WtLexCmpString as _WtLexCmpString,
    WtLexCmpWord as _WtLexCmpWord,
)

from .alphabet import Alphabet as _Alphabet
from .detail.cxx_wrapper import (
    CxxWrapper as _CxxWrapper,
    copy_cxx_mem_fns as _copy_cxx_mem_fns,
    register_cxx_wrapped_type as _register_cxx_wrapped_type,
    to_cxx as _to_cxx,
)
from .detail.decorators import copydoc as _copydoc


class _ConfiguredCmp(_CxxWrapper):
    _configuration_name: str

    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, optional_kwargs=("alphabet", self._configuration_name), **kwargs)
        if _to_cxx(self) is not None:
            return

        if len(args) > 2:
            raise TypeError(f"expected at most 2 positional arguments, found {len(args)}")
        if args and kwargs:
            raise TypeError("expected positional or keyword arguments, but found both")

        if kwargs:
            alphabet = kwargs.get("alphabet")
            configuration = kwargs.get(self._configuration_name)
            has_alphabet = "alphabet" in kwargs
            has_configuration = self._configuration_name in kwargs
        else:
            alphabet = args[0] if len(args) == 2 else None
            configuration = args[-1] if args else None
            has_alphabet = len(args) == 2
            has_configuration = bool(args)

        if not has_alphabet and not has_configuration:
            self.py_template_params = ()
            self.init_cxx_obj()
        elif not has_alphabet and isinstance(configuration, list):
            self.py_template_params = ()
            self.init_cxx_obj(configuration)
        elif (
            has_alphabet
            and has_configuration
            and isinstance(alphabet, _Alphabet)
            and isinstance(configuration, list)
        ):
            self.py_template_params = alphabet.py_template_params
            self.init_cxx_obj(alphabet, configuration)
        else:
            raise TypeError(
                f"expected either {self._configuration_name}: list[int], or alphabet: "
                f"Alphabet and {self._configuration_name}: list[int]"
            )


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
            raise TypeError(f"expected at most 1 positional argument, found {len(args)}")

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
            raise TypeError(f"expected the argument to be an Alphabet, but found {type(alphabet)}")

    @_copydoc(_LexCmpString.__call__)
    def __call__(self: _Self, x: str | list[int], y: str | list[int]) -> bool:
        return super().__call__(x, y)


class RevLexCmp(_CxxWrapper):
    __doc__ = _RevLexCmpString.__doc__

    _py_template_params_to_cxx_type = {
        (): _RevLexCmpDefault,
        (str,): _RevLexCmpString,
        (list[int],): _RevLexCmpWord,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
            strict=True,
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_RevLexCmpString.__init__, _RevLexCmpDefault.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, optional_kwargs=("alphabet",), **kwargs)
        if _to_cxx(self) is not None:
            return

        if len(args) > 1:
            raise TypeError(f"expected at most 1 positional argument, found {len(args)}")

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
            raise TypeError(f"expected the argument to be an Alphabet, but found {type(alphabet)}")

    @_copydoc(_RevLexCmpString.__call__)
    def __call__(self: _Self, x: str | list[int], y: str | list[int]) -> bool:
        return super().__call__(x, y)


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
            raise TypeError(f"expected at most 1 positional argument, found {len(args)}")

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
            raise TypeError(f"expected the argument to be an Alphabet, but found {type(alphabet)}")

    @_copydoc(_LenLexCmpString.__call__)
    def __call__(self: _Self, x: str | list[int], y: str | list[int]) -> bool:
        return super().__call__(x, y)


class RevLenLexCmp(_CxxWrapper):
    __doc__ = _RevLenLexCmpString.__doc__

    _py_template_params_to_cxx_type = {
        (): _RevLenLexCmpDefault,
        (str,): _RevLenLexCmpString,
        (list[int],): _RevLenLexCmpWord,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
            strict=True,
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_RevLenLexCmpString.__init__, _RevLenLexCmpDefault.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, optional_kwargs=("alphabet",), **kwargs)
        if _to_cxx(self) is not None:
            return

        if len(args) > 1:
            raise TypeError(f"expected at most 1 positional argument, found {len(args)}")

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
            raise TypeError(f"expected the argument to be an Alphabet, but found {type(alphabet)}")

    @_copydoc(_RevLenLexCmpString.__call__)
    def __call__(self: _Self, x: str | list[int], y: str | list[int]) -> bool:
        return super().__call__(x, y)


class RPOCmp(_CxxWrapper):
    __doc__ = _RPOCmpString.__doc__

    _py_template_params_to_cxx_type = {
        (): _RPOCmpDefault,
        (str,): _RPOCmpString,
        (list[int],): _RPOCmpWord,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
            strict=True,
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_RPOCmpString.__init__, _RPOCmpDefault.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, optional_kwargs=("alphabet",), **kwargs)
        if _to_cxx(self) is not None:
            return

        if len(args) > 1:
            raise TypeError(f"expected at most 1 positional argument, found {len(args)}")

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
            raise TypeError(f"expected the argument to be an Alphabet, but found {type(alphabet)}")

    @_copydoc(_RPOCmpString.__call__)
    def __call__(self: _Self, x: str | list[int], y: str | list[int]) -> bool:
        return super().__call__(x, y)


class RevRPOCmp(_CxxWrapper):
    __doc__ = _RevRPOCmpString.__doc__

    _py_template_params_to_cxx_type = {
        (): _RevRPOCmpDefault,
        (str,): _RevRPOCmpString,
        (list[int],): _RevRPOCmpWord,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
            strict=True,
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_RevRPOCmpString.__init__, _RevRPOCmpDefault.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, optional_kwargs=("alphabet",), **kwargs)
        if _to_cxx(self) is not None:
            return

        if len(args) > 1:
            raise TypeError(f"expected at most 1 positional argument, found {len(args)}")

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
            raise TypeError(f"expected the argument to be an Alphabet, but found {type(alphabet)}")

    @_copydoc(_RevRPOCmpString.__call__)
    def __call__(self: _Self, x: str | list[int], y: str | list[int]) -> bool:
        return super().__call__(x, y)


class WrCmp(_ConfiguredCmp):
    __doc__ = _WrCmpString.__doc__
    _configuration_name = "levels"

    _py_template_params_to_cxx_type = {
        (): _WrCmpDefault,
        (str,): _WrCmpString,
        (list[int],): _WrCmpWord,
    }
    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
            strict=True,
        )
    )
    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_WrCmpString.__init__, _WrCmpDefault.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, **kwargs)

    @_copydoc(_WrCmpString.__call__)
    def __call__(self: _Self, x: str | list[int], y: str | list[int]) -> bool:
        return super().__call__(x, y)


class RevWrCmp(_ConfiguredCmp):
    __doc__ = _RevWrCmpString.__doc__
    _configuration_name = "levels"

    _py_template_params_to_cxx_type = {
        (): _RevWrCmpDefault,
        (str,): _RevWrCmpString,
        (list[int],): _RevWrCmpWord,
    }
    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
            strict=True,
        )
    )
    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_RevWrCmpString.__init__, _RevWrCmpDefault.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, **kwargs)

    @_copydoc(_RevWrCmpString.__call__)
    def __call__(self: _Self, x: str | list[int], y: str | list[int]) -> bool:
        return super().__call__(x, y)


class WtLenLexCmp(_ConfiguredCmp):
    __doc__ = _WtLenLexCmpString.__doc__
    _configuration_name = "weights"

    _py_template_params_to_cxx_type = {
        (): _WtLenLexCmpDefault,
        (str,): _WtLenLexCmpString,
        (list[int],): _WtLenLexCmpWord,
    }
    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
            strict=True,
        )
    )
    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_WtLenLexCmpString.__init__, _WtLenLexCmpDefault.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, **kwargs)

    @_copydoc(_WtLenLexCmpString.__call__)
    def __call__(self: _Self, x: str | list[int], y: str | list[int]) -> bool:
        return super().__call__(x, y)


class RevWtLenLexCmp(_ConfiguredCmp):
    __doc__ = _RevWtLenLexCmpString.__doc__
    _configuration_name = "weights"

    _py_template_params_to_cxx_type = {
        (): _RevWtLenLexCmpDefault,
        (str,): _RevWtLenLexCmpString,
        (list[int],): _RevWtLenLexCmpWord,
    }
    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
            strict=True,
        )
    )
    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_RevWtLenLexCmpString.__init__, _RevWtLenLexCmpDefault.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, **kwargs)

    @_copydoc(_RevWtLenLexCmpString.__call__)
    def __call__(self: _Self, x: str | list[int], y: str | list[int]) -> bool:
        return super().__call__(x, y)


class WtLexCmp(_ConfiguredCmp):
    __doc__ = _WtLexCmpString.__doc__
    _configuration_name = "weights"

    _py_template_params_to_cxx_type = {
        (): _WtLexCmpDefault,
        (str,): _WtLexCmpString,
        (list[int],): _WtLexCmpWord,
    }
    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
            strict=True,
        )
    )
    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_WtLexCmpString.__init__, _WtLexCmpDefault.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, **kwargs)

    @_copydoc(_WtLexCmpString.__call__)
    def __call__(self: _Self, x: str | list[int], y: str | list[int]) -> bool:
        return super().__call__(x, y)


_copy_cxx_mem_fns(_LenLexCmpString, LenLexCmp)
_register_cxx_wrapped_type(_LenLexCmpDefault, LenLexCmp)
_register_cxx_wrapped_type(_LenLexCmpString, LenLexCmp)
_register_cxx_wrapped_type(_LenLexCmpWord, LenLexCmp)

_copy_cxx_mem_fns(_RevLenLexCmpString, RevLenLexCmp)
_register_cxx_wrapped_type(_RevLenLexCmpDefault, RevLenLexCmp)
_register_cxx_wrapped_type(_RevLenLexCmpString, RevLenLexCmp)
_register_cxx_wrapped_type(_RevLenLexCmpWord, RevLenLexCmp)

_copy_cxx_mem_fns(_LexCmpString, LexCmp)
_register_cxx_wrapped_type(_LexCmpDefault, LexCmp)
_register_cxx_wrapped_type(_LexCmpString, LexCmp)
_register_cxx_wrapped_type(_LexCmpWord, LexCmp)

_copy_cxx_mem_fns(_RevLexCmpString, RevLexCmp)
_register_cxx_wrapped_type(_RevLexCmpDefault, RevLexCmp)
_register_cxx_wrapped_type(_RevLexCmpString, RevLexCmp)
_register_cxx_wrapped_type(_RevLexCmpWord, RevLexCmp)

_copy_cxx_mem_fns(_RPOCmpString, RPOCmp)
_register_cxx_wrapped_type(_RPOCmpDefault, RPOCmp)
_register_cxx_wrapped_type(_RPOCmpString, RPOCmp)
_register_cxx_wrapped_type(_RPOCmpWord, RPOCmp)

_copy_cxx_mem_fns(_RevRPOCmpString, RevRPOCmp)
_register_cxx_wrapped_type(_RevRPOCmpDefault, RevRPOCmp)
_register_cxx_wrapped_type(_RevRPOCmpString, RevRPOCmp)
_register_cxx_wrapped_type(_RevRPOCmpWord, RevRPOCmp)

_copy_cxx_mem_fns(_WrCmpString, WrCmp)
_register_cxx_wrapped_type(_WrCmpDefault, WrCmp)
_register_cxx_wrapped_type(_WrCmpString, WrCmp)
_register_cxx_wrapped_type(_WrCmpWord, WrCmp)


_copy_cxx_mem_fns(_RevWrCmpString, RevWrCmp)
_register_cxx_wrapped_type(_RevWrCmpDefault, RevWrCmp)
_register_cxx_wrapped_type(_RevWrCmpString, RevWrCmp)
_register_cxx_wrapped_type(_RevWrCmpWord, RevWrCmp)

_copy_cxx_mem_fns(_WtLenLexCmpString, WtLenLexCmp)
_register_cxx_wrapped_type(_WtLenLexCmpDefault, WtLenLexCmp)
_register_cxx_wrapped_type(_WtLenLexCmpString, WtLenLexCmp)
_register_cxx_wrapped_type(_WtLenLexCmpWord, WtLenLexCmp)


_copy_cxx_mem_fns(_RevWtLenLexCmpString, RevWtLenLexCmp)
_register_cxx_wrapped_type(_RevWtLenLexCmpDefault, RevWtLenLexCmp)
_register_cxx_wrapped_type(_RevWtLenLexCmpString, RevWtLenLexCmp)
_register_cxx_wrapped_type(_RevWtLenLexCmpWord, RevWtLenLexCmp)


_copy_cxx_mem_fns(_WtLexCmpString, WtLexCmp)
_register_cxx_wrapped_type(_WtLexCmpDefault, WtLexCmp)
_register_cxx_wrapped_type(_WtLexCmpString, WtLexCmp)
_register_cxx_wrapped_type(_WtLexCmpWord, WtLexCmp)


class RevWtLexCmp(_ConfiguredCmp):
    __doc__ = _RevWtLexCmpString.__doc__
    _configuration_name = "weights"

    _py_template_params_to_cxx_type = {
        (): _RevWtLexCmpDefault,
        (str,): _RevWtLexCmpString,
        (list[int],): _RevWtLexCmpWord,
    }
    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
            strict=True,
        )
    )
    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_RevWtLexCmpString.__init__, _RevWtLexCmpDefault.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, **kwargs)

    @_copydoc(_RevWtLexCmpString.__call__)
    def __call__(self: _Self, x: str | list[int], y: str | list[int]) -> bool:
        return super().__call__(x, y)


_copy_cxx_mem_fns(_RevWtLexCmpString, RevWtLexCmp)
_register_cxx_wrapped_type(_RevWtLexCmpDefault, RevWtLexCmp)
_register_cxx_wrapped_type(_RevWtLexCmpString, RevWtLexCmp)
_register_cxx_wrapped_type(_RevWtLexCmpWord, RevWtLexCmp)


class LenWtLexCmp(_ConfiguredCmp):
    __doc__ = _LenWtLexCmpString.__doc__
    _configuration_name = "weights"

    _py_template_params_to_cxx_type = {
        (): _LenWtLexCmpDefault,
        (str,): _LenWtLexCmpString,
        (list[int],): _LenWtLexCmpWord,
    }
    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
            strict=True,
        )
    )
    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_LenWtLexCmpString.__init__, _LenWtLexCmpDefault.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, **kwargs)

    @_copydoc(_LenWtLexCmpString.__call__)
    def __call__(self: _Self, x: str | list[int], y: str | list[int]) -> bool:
        return super().__call__(x, y)


_copy_cxx_mem_fns(_LenWtLexCmpString, LenWtLexCmp)
_register_cxx_wrapped_type(_LenWtLexCmpDefault, LenWtLexCmp)
_register_cxx_wrapped_type(_LenWtLexCmpString, LenWtLexCmp)
_register_cxx_wrapped_type(_LenWtLexCmpWord, LenWtLexCmp)


class RevLenWtLexCmp(_ConfiguredCmp):
    __doc__ = _RevLenWtLexCmpString.__doc__
    _configuration_name = "weights"

    _py_template_params_to_cxx_type = {
        (): _RevLenWtLexCmpDefault,
        (str,): _RevLenWtLexCmpString,
        (list[int],): _RevLenWtLexCmpWord,
    }
    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
            strict=True,
        )
    )
    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_RevLenWtLexCmpString.__init__, _RevLenWtLexCmpDefault.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, **kwargs)

    @_copydoc(_RevLenWtLexCmpString.__call__)
    def __call__(self: _Self, x: str | list[int], y: str | list[int]) -> bool:
        return super().__call__(x, y)


_copy_cxx_mem_fns(_RevLenWtLexCmpString, RevLenWtLexCmp)
_register_cxx_wrapped_type(_RevLenWtLexCmpDefault, RevLenWtLexCmp)
_register_cxx_wrapped_type(_RevLenWtLexCmpString, RevLenWtLexCmp)
_register_cxx_wrapped_type(_RevLenWtLexCmpWord, RevLenWtLexCmp)


__all__ = [
    "LenLexCmp",
    "LenWtLexCmp",
    "LexCmp",
    "RPOCmp",
    "RevLenLexCmp",
    "RevLenWtLexCmp",
    "RevLexCmp",
    "RevRPOCmp",
    "RevWrCmp",
    "RevWtLenLexCmp",
    "RevWtLexCmp",
    "WrCmp",
    "WtLenLexCmp",
    "WtLexCmp",
]
