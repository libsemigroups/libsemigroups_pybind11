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
import itertools
import copy
from typing import Iterator, Any, Union
from typing_extensions import Self

from _libsemigroups_pybind11 import (
    StaticTransf16 as _StaticTransf16,
    Transf1 as _Transf1,
    Transf2 as _Transf2,
    Transf4 as _Transf4,
    StaticPPerm16 as _StaticPPerm16,
    PPerm1 as _PPerm1,
    PPerm2 as _PPerm2,
    PPerm4 as _PPerm4,
    StaticPerm16 as _StaticPerm16,
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

from .tools import copydoc
from .py_wrappers import CppObjWrapper, pass_thru_methods


class PTransfBase(CppObjWrapper):
    """
    Subclasses must implement:
    * _cpp_type_from_degree     (static method)
    * _cpp_type_change_required (function)
    * __str__                   (function)
    * __repr__                  (function)
    * one                       (static method)
    """

    @staticmethod
    @abc.abstractmethod
    def _cpp_type_from_degree(n: int) -> Any:
        return  # pragma: no cover

    @abc.abstractmethod
    def _cpp_type_change_required(self: Self, n: int) -> bool:
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

    @staticmethod
    def _is_static(cpp_type: Any) -> bool:
        return cpp_type in (_StaticTransf16, _StaticPPerm16, _StaticPerm16)

    def degree(self: Self) -> int:  # pylint: disable=missing-function-docstring
        return self._degree

    def __new__(cls, *_):
        return super(PTransfBase, cls).__new__(cls)

    def __init__(self: Self, arg: Any) -> None:
        # pylint: disable=not-callable, super-init-not-called
        if isinstance(type(arg), type(_StaticTransf16)):
            # check if arg is a pybind11 built in type
            self._cpp_obj = arg
            self._degree = arg.degree()
            return
        images = arg

        if len(images) > 2**32:
            raise ValueError(  # pragma: no cover
                f"the argument (a list) must have length at most {2**32}, found {len(images)}"
            )

        if not isinstance(images, list):
            images = list(images)

        self._degree = len(images)
        cpp_type = self._cpp_type_from_degree(len(images))
        if len(images) <= 16:
            if cpp_type is not _StaticPPerm16:
                images += range(len(images), 16)
            else:
                images += [cpp_type.undef()] * (16 - len(images))

        self._cpp_obj = cpp_type(images)

    def __eq__(self: Self, other) -> bool:
        if self._degree != other._degree:
            return False
        assert isinstance(other._cpp_obj, type(self._cpp_obj))
        return self._cpp_obj == other._cpp_obj

    def __lt__(self: Self, other) -> bool:
        if self._degree != other._degree:
            return self._degree < other._degree
        assert isinstance(other._cpp_obj, type(self._cpp_obj))
        return self._cpp_obj < other._cpp_obj

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
        # Use _cpp_obj here so that we get the trailing undef's/id func in the
        # case that _cpp_obj is static
        result = type(self)(list(self._cpp_obj.images()))
        result._degree = self._degree
        return result

    def copy(self: Self) -> Self:  # pylint: disable=missing-function-docstring
        return copy.copy(self)

    def images(  # pylint: disable=missing-function-docstring
        self: Self,
    ) -> Iterator:
        it = self._cpp_obj.images()
        if PTransfBase._is_static(type(self._cpp_obj)):
            return itertools.islice(it, self._degree)
        return it

    def increase_degree_by(  # pylint: disable=missing-function-docstring
        self: Self, N: int
    ) -> Self:
        # pylint: disable=not-callable
        new_degree = N + self._degree
        if new_degree > 2**32:
            raise ValueError(
                "the 1st argument (int) is too large, partial "
                "transformations of degree > 2 ** 32 are not supported, "
                f"expected at most {2 **32 - self._degree} but found {N}"
            )
        if self._cpp_type_change_required(new_degree):
            imgs = list(self.images())
            imgs.extend(range(self._degree, new_degree))
            new_cpp_type = type(self)._cpp_type_from_degree(new_degree)
            if isinstance(self, PPerm):
                old_cpp_type = type(self._cpp_obj)
                imgs = [
                    new_cpp_type.undef() if x == old_cpp_type.undef() else x
                    for x in imgs
                ]
            self._cpp_obj = new_cpp_type(imgs)
        elif not PTransfBase._is_static(type(self._cpp_obj)):
            self._cpp_obj.increase_degree_by(N)
        self._degree = new_degree
        return self

    @staticmethod
    def _one(  # pylint: disable=missing-function-docstring
        N: int, subclass: Any
    ) -> Any:
        cpp_type = subclass._cpp_type_from_degree(N)
        if PTransfBase._is_static(cpp_type):
            if subclass is PPerm:
                return PPerm(range(N))
            M = 16
        else:
            M = N
        result = subclass.__new__(subclass)
        result._cpp_obj = cpp_type.one(M)
        result._degree = N
        return result

    def rank(self: Self) -> None:  # pylint: disable=missing-function-docstring
        if PTransfBase._is_static(type(self._cpp_obj)) and not isinstance(
            self, PPerm
        ):
            return self._cpp_obj.rank() - (16 - self._degree)
        return self._cpp_obj.rank()

    def swap(  # pylint: disable=missing-function-docstring
        self: Self, other: Self
    ) -> None:
        # TODO maybe improve this with exception message or something, in case
        # that self._cpp_obj type is not the same as other._cpp_obj type
        self._cpp_obj.swap(other._cpp_obj)

        self._degree, other._degree = (
            other._degree,
            self._degree,
        )


pass_thru_methods(PTransfBase, "product_inplace", "__getitem__", "__hash__")


class Transf(PTransfBase):  # pylint: disable=missing-class-docstring
    __doc__ = _StaticTransf16.__doc__

    @staticmethod
    def _cpp_type_from_degree(n: int):
        if n <= 16:
            return _StaticTransf16
        if n <= 2**8:
            return _Transf1
        if n <= 2**16:
            return _Transf2
        return _Transf4

    def _cpp_type_change_required(self: Self, n: int) -> bool:
        if isinstance(self._cpp_obj, _StaticTransf16):
            return n > 16
        if isinstance(self._cpp_obj, _Transf1):
            return n > 2**8
        if isinstance(self._cpp_obj, _Transf2):
            return n > 2**16
        raise RuntimeError("this should never happen")  # pragma: no cover

    def __repr__(self: Self) -> str:
        if self._degree < 32:
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
    __doc__ = _StaticPPerm16.__doc__

    @staticmethod
    def _cpp_type_from_degree(n: int):
        if n <= 16:
            return _StaticPPerm16
        if n <= 2**8:
            return _PPerm1
        if n <= 2**16:
            return _PPerm2
        return _PPerm4

    def _cpp_type_change_required(self: Self, n: int) -> bool:
        if isinstance(self._cpp_obj, _StaticPPerm16):
            return n > 16
        if isinstance(self._cpp_obj, _PPerm1):
            return n > 2**8
        if isinstance(self._cpp_obj, _PPerm2):
            return n > 2**16
        raise RuntimeError("this should never happen")  # pragma: no cover

    def __init__(self: Self, *args):
        if len(args) == 1:
            super().__init__(*args)
            return
        if len(args) != 3:
            raise TypeError(f"expected 1 or 3 arguments, found {len(args)}")

        self._degree = args[2]
        args = list(args)
        if args[2] < 16:
            args[2] = 16
        self._cpp_obj = self._cpp_type_from_degree(args[2])(*args)

    def __repr__(self: Self) -> str:
        if self._degree < 32:
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
        return self._cpp_obj.undef()


class Perm(Transf):  # pylint: disable=missing-class-docstring
    __doc__ = _StaticPerm16.__doc__

    @staticmethod
    def _cpp_type_from_degree(n: int):
        if n <= 16:
            return _StaticPerm16
        if n <= 2**8:
            return _Perm1
        if n <= 2**16:
            return _Perm2
        return _Perm4

    def _cpp_type_change_required(self: Self, n: int) -> bool:
        if isinstance(self._cpp_obj, _StaticPerm16):
            return n > 16
        if isinstance(self._cpp_obj, _Perm1):
            return n > 2**8
        if isinstance(self._cpp_obj, _Perm2):
            return n > 2**16
        raise RuntimeError("this should never happen")  # pragma: no cover

    def __repr__(self: Self) -> str:
        if self._degree < 32:
            return str(self)
        return f"<permutation of degree {self.degree()}>"

    def __str__(self: Self) -> str:
        return f"Perm({list(self.images())})"

    @staticmethod
    def one(N: int):  # pylint: disable=arguments-differ
        return PTransfBase._one(N, Perm)


########################################################################
# Helper functions
########################################################################


@copydoc(_one)
def one(  # pylint: disable=missing-function-docstring
    x: Union[Transf, PPerm, Perm]
) -> Union[Transf, PPerm, Perm]:
    return type(x).one(x.degree())


@copydoc(_inverse)
def inverse(  # pylint: disable=missing-function-docstring
    x: Union[PPerm, Perm]
) -> Union[PPerm, Perm]:
    subclass = type(x)
    result = subclass.__new__(subclass)
    result._cpp_obj = _inverse(x._cpp_obj)
    result._degree = x._degree
    return result


@copydoc(_right_one)
def right_one(x: PPerm) -> PPerm:  # pylint: disable=missing-function-docstring
    result = PPerm.__new__(PPerm)
    result._cpp_obj = _right_one(x._cpp_obj)
    result._degree = x._degree
    return result


@copydoc(_left_one)
def left_one(x: PPerm) -> PPerm:  # pylint: disable=missing-function-docstring
    result = PPerm.__new__(PPerm)
    result._cpp_obj = _left_one(x._cpp_obj)
    result._degree = x._degree
    return result


@copydoc(_image)  # pylint: disable-next=missing-function-docstring
def image(x: Union[Transf, PPerm, Perm]) -> Union[Transf, PPerm, Perm]:
    result = _image(x._cpp_obj)
    if type(x)._is_static(type(x._cpp_obj)):
        result = result[: x.degree()]
    return result


@copydoc(_domain)  # pylint: disable-next=missing-function-docstring
def domain(x: Union[Transf, PPerm, Perm]) -> Union[Transf, PPerm, Perm]:
    result = _domain(x._cpp_obj)
    if type(x)._is_static(type(x._cpp_obj)):
        result = result[: x.degree()]
    return result
