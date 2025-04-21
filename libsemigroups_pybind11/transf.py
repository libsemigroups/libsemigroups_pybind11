# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name
# pylint: disable=bad-option-value, protected-access

"""
This package provides a the user-facing python part of libsemigroups_pybind11
relating to transformations.
"""

import abc
import copy

from typing import Any, Union
from typing_extensions import Self

from _libsemigroups_pybind11 import (
    Transf1 as _Transf1,
    Transf2 as _Transf2,
    Transf4 as _Transf4,
    PPerm1 as _PPerm1,
    PPerm2 as _PPerm2,
    PPerm4 as _PPerm4,
    Perm1 as _Perm1,
    Perm2 as _Perm2,
    Perm4 as _Perm4,
    one as _one,
    inverse as _inverse,
    image as _image,
    domain as _domain,
    right_one as _right_one,
    left_one as _left_one,
)

from .detail.decorators import copydoc
from .detail.cxx_wrapper import (
    CxxWrapper as _CxxWrapper,
    register_cxx_wrapped_type as _register_cxx_wrapped_type,
)

pybind11_type = type(_Transf1)


class PTransfBase(_CxxWrapper):
    """
    Subclasses must implement:
    * _cxx_type_from_degree     (static method)
    * _cxx_type_change_required (function)
    * __str__                   (function)
    * __repr__                  (function)
    * one                       (static method)
    """

    @staticmethod
    @abc.abstractmethod
    def _cxx_type_from_degree(n: int) -> Any:
        return  # pragma: no cover

    @abc.abstractmethod
    def _cxx_type_change_required(self: Self, n: int) -> bool:
        return  # pragma: no cover

    @abc.abstractmethod
    def __str__(self: Self) -> str:
        return ""  # pragma: no cover

    @abc.abstractmethod
    def __repr__(self: Self) -> str:
        return ""  # pragma: no cover

    @staticmethod
    @abc.abstractmethod
    def one(N: int) -> Any:  # pylint: disable=missing-function-docstring
        return None  # pragma: no cover

    def __getitem__(self: Self, i: int) -> int:
        return self._cxx_obj[i]

    def __hash__(self: Self) -> int:
        return self._cxx_obj.__hash__()

    def __new__(cls, *_):
        return super(PTransfBase, cls).__new__(cls)

    def __init__(self: Self, arg) -> None:
        # pylint: disable=not-callable, super-init-not-called
        # TODO this can't be right, pybind11_type is _Transf1, so this will
        # almost never invoked
        if isinstance(type(arg), pybind11_type):
            self._cxx_obj = arg
            return

        images = arg

        if len(images) > 2**32:
            raise ValueError(  # pragma: no cover
                f"the argument (a list) must have length at most {2**32}, found {len(images)}"
            )

        if not isinstance(images, list):
            images = list(images)

        cxx_type = self._cxx_type_from_degree(len(images))
        self._cxx_obj = cxx_type(images)

    def __eq__(self: Self, other) -> bool:
        if not isinstance(other._cxx_obj, type(self._cxx_obj)):
            return False
        return self._cxx_obj == other._cxx_obj

    def __lt__(self: Self, other) -> bool:
        return self._cxx_obj < other._cxx_obj

    def __le__(self: Self, other) -> bool:
        return self < other or self == other

    def __gt__(self: Self, other) -> bool:
        return other < self

    def __ge__(self: Self, other) -> bool:
        return other < self or self == other

    def __mul__(self: Self, other: Self):
        # pylint: disable=no-member
        result = one(self)
        result.product_inplace(self, other)
        return result

    def __copy__(self: Self):
        # Use _cxx_obj here so that we get the trailing undef's/id func in the
        # case that _cxx_obj is static
        result = type(self)(list(self._cxx_obj.images()))
        return result

    def copy(self: Self) -> Self:  # pylint: disable=missing-function-docstring
        return copy.copy(self)

    def increase_degree_by(  # pylint: disable=missing-function-docstring
        self: Self, N: int
    ) -> Self:
        # pylint: disable=not-callable
        new_degree = N + self.degree()
        if new_degree > 2**32:
            raise ValueError(
                "the 1st argument (int) is too large, partial "
                "transformations of degree > 2 ** 32 are not supported, "
                f"expected at most {2**32 - self.degree()} but found {N}"
            )
        if self._cxx_type_change_required(new_degree):
            imgs = list(self.images())
            imgs.extend(range(self.degree(), new_degree))
            new_cxx_type = type(self)._cxx_type_from_degree(new_degree)
            if isinstance(self, PPerm):
                old_cxx_type = type(self._cxx_obj)
                imgs = [
                    new_cxx_type.undef() if x == old_cxx_type.undef() else x
                    for x in imgs
                ]
            self._cxx_obj = new_cxx_type(imgs)
        else:
            self._cxx_obj.increase_degree_by(N)
        return self

    @staticmethod
    def _one(  # pylint: disable=missing-function-docstring
        N: int, subclass: Any
    ) -> Any:
        cxx_type = subclass._cxx_type_from_degree(N)
        M = N
        result = subclass.__new__(subclass)
        result._cxx_obj = cxx_type.one(M)
        return result

    def swap(  # pylint: disable=missing-function-docstring
        self: Self, other: Self
    ) -> None:
        # TODO maybe improve this with exception message or something, in case
        # that self._cxx_obj type is not the same as other._cxx_obj type
        self._cxx_obj.swap(other._cxx_obj)


class Transf(PTransfBase):  # pylint: disable=missing-class-docstring
    __doc__ = _Transf1.__doc__

    @staticmethod
    def _cxx_type_from_degree(n: int):
        if n <= 2**8:
            return _Transf1
        if n <= 2**16:
            return _Transf2
        return _Transf4

    def _cxx_type_change_required(self: Self, n: int) -> bool:
        if isinstance(self._cxx_obj, _Transf1):
            return n > 2**8
        if isinstance(self._cxx_obj, _Transf2):
            return n > 2**16
        raise RuntimeError("this should never happen")  # pragma: no cover

    def __repr__(self: Self) -> str:
        if self.degree() < 32:
            return str(self)
        return (
            f"<transformation of degree {self.degree()} and rank {self.rank()}>"
        )

    def __str__(self: Self) -> str:
        return f"Transf({list(self.images())})"

    @staticmethod
    def one(N: int):  # pylint: disable=arguments-differ
        return PTransfBase._one(N, Transf)


class PPerm(PTransfBase):  # pylint: disable=missing-class-docstring
    __doc__ = _PPerm1.__doc__

    @staticmethod
    def _cxx_type_from_degree(n: int):
        if n <= 2**8:
            return _PPerm1
        if n <= 2**16:
            return _PPerm2
        return _PPerm4

    def _cxx_type_change_required(self: Self, n: int) -> bool:
        if isinstance(self._cxx_obj, _PPerm1):
            return n > 2**8
        if isinstance(self._cxx_obj, _PPerm2):
            return n > 2**16
        raise RuntimeError("this should never happen")  # pragma: no cover

    def __init__(self: Self, *args):
        if len(args) < 3:
            super().__init__(*args)
            return
        if len(args) != 3:
            raise TypeError(f"expected 1 or 3 arguments, found {len(args)}")

        args = list(args)
        self._cxx_obj = self._cxx_type_from_degree(args[2])(*args)

    def __repr__(self: Self) -> str:
        if self.degree() < 32:
            return str(self)
        return (
            f"<partial perm of degree {self.degree()} and rank {self.rank()}>"
        )

    def __str__(self: Self) -> str:
        # pylint: disable-next=unsubscriptable-object
        return f"PPerm({domain(self)}, {[self[i] for i in domain(self)]}, {self.degree()})"

    @staticmethod
    def one(N: int):  # pylint: disable=arguments-differ
        return PTransfBase._one(N, PPerm)

    def undef(self: Self) -> int:  # pylint: disable=missing-function-docstring
        return self._cxx_obj.undef()


class Perm(Transf):  # pylint: disable=missing-class-docstring
    __doc__ = _Perm1.__doc__

    @staticmethod
    def _cxx_type_from_degree(n: int):
        if n <= 2**8:
            return _Perm1
        if n <= 2**16:
            return _Perm2
        return _Perm4

    def _cxx_type_change_required(self: Self, n: int) -> bool:
        if isinstance(self._cxx_obj, _Perm1):
            return n > 2**8
        if isinstance(self._cxx_obj, _Perm2):
            return n > 2**16
        raise RuntimeError("this should never happen")  # pragma: no cover

    def __repr__(self: Self) -> str:
        if self.degree() < 32:
            return str(self)
        return f"<permutation of degree {self.degree()}>"

    def __str__(self: Self) -> str:
        return f"Perm({list(self.images())})"

    @staticmethod
    def one(N: int):  # pylint: disable=arguments-differ
        return PTransfBase._one(N, Perm)


_register_cxx_wrapped_type(_Transf1, Transf)
_register_cxx_wrapped_type(_Transf2, Transf)
_register_cxx_wrapped_type(_Transf4, Transf)
_register_cxx_wrapped_type(_PPerm1, PPerm)
_register_cxx_wrapped_type(_PPerm2, PPerm)
_register_cxx_wrapped_type(_PPerm4, PPerm)
_register_cxx_wrapped_type(_Perm1, Perm)
_register_cxx_wrapped_type(_Perm2, Perm)
_register_cxx_wrapped_type(_Perm4, Perm)

########################################################################
# Helper functions
########################################################################


@copydoc(_one)
def one(  # pylint: disable=missing-function-docstring
    x: Union[Transf, PPerm, Perm],
) -> Union[Transf, PPerm, Perm]:
    return type(x).one(x.degree())


@copydoc(_inverse)
def inverse(  # pylint: disable=missing-function-docstring
    x: Union[PPerm, Perm],
) -> Union[PPerm, Perm]:
    subclass = type(x)
    result = subclass.__new__(subclass)
    result._cxx_obj = _inverse(x._cxx_obj)
    return result


@copydoc(_right_one)
def right_one(x: PPerm) -> PPerm:  # pylint: disable=missing-function-docstring
    result = PPerm.__new__(PPerm)
    result._cxx_obj = _right_one(x._cxx_obj)
    return result


@copydoc(_left_one)
def left_one(x: PPerm) -> PPerm:  # pylint: disable=missing-function-docstring
    result = PPerm.__new__(PPerm)
    result._cxx_obj = _left_one(x._cxx_obj)
    return result


@copydoc(_image)  # pylint: disable-next=missing-function-docstring
def image(x: Union[Transf, PPerm, Perm]) -> Union[Transf, PPerm, Perm]:
    return _image(x._cxx_obj)


@copydoc(_domain)  # pylint: disable-next=missing-function-docstring
def domain(x: Union[Transf, PPerm, Perm]) -> Union[Transf, PPerm, Perm]:
    return _domain(x._cxx_obj)
