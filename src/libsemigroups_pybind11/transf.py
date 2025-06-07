# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This page contains the documentation for the ``transf`` subpackage, that
contains helper functions for the :any:`Transf`, :any:`Perm`, :any:`PPerm`
classes.
"""

import abc

from typing import Any as _Any, Union
from typing_extensions import Self

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module
    PPerm1 as _PPerm1,
    PPerm2 as _PPerm2,
    PPerm4 as _PPerm4,
    Perm1 as _Perm1,
    Perm2 as _Perm2,
    Perm4 as _Perm4,
    Transf1 as _Transf1,
    Transf2 as _Transf2,
    Transf4 as _Transf4,
    transf_domain as _transf_domain,
    transf_image as _transf_image,
    transf_inverse as _transf_inverse,
    transf_left_one as _transf_left_one,
    transf_one as _transf_one,
    transf_right_one as _transf_right_one,
    Undefined as _Undefined,
)

from .detail.decorators import copydoc as _copydoc
from .detail.cxx_wrapper import (
    CxxWrapper as _CxxWrapper,
    copy_cxx_mem_fns as _copy_cxx_mem_fns,
    register_cxx_wrapped_type as _register_cxx_wrapped_type,
    to_cxx as _to_cxx,
    to_py as _to_py,
    wrap_cxx_free_fn as _wrap_cxx_free_fn,
)

########################################################################
# PTransfBase protected python class
########################################################################


class _PTransfBase(_CxxWrapper):
    """
    Subclasses must implement:
    * __str__                   (function)
    * __repr__                  (function)
    * one                       (static method)
    """

    @abc.abstractmethod
    def __str__(self: Self) -> str:
        return ""  # pragma: no cover

    @abc.abstractmethod
    def __repr__(self: Self) -> str:
        return ""  # pragma: no cover

    @staticmethod
    def _py_template_params_from_degree(n: int) -> tuple[int]:
        if n < 2**8:
            return (2**8,)
        if n < 2**16:
            return (2**16,)
        assert n <= 2**32
        return (2**32,)

    def _set_py_template_params_from_degree(self: Self, n: int) -> None:
        self.py_template_params = self._py_template_params_from_degree(n)

    def _cxx_type_change_required(self: Self, n: int) -> bool:
        assert n <= 2**32
        return n > self.py_template_params[0]

    def __getitem__(self: Self, i: int) -> Union[int, _Undefined]:
        return _to_cxx(self)[i]

    def __hash__(self: Self) -> int:
        return _to_cxx(self).__hash__()

    def __new__(cls, *_):
        return super(_PTransfBase, cls).__new__(cls)

    def __init__(self: Self, arg) -> None:
        super().__init__(arg)
        if _to_cxx(self) is not None:
            return

        images = arg

        if len(images) > 2**32:
            raise ValueError(  # pragma: no cover
                f"the argument (a list) must have length at most {2**32}, found {len(images)}"
            )
        if not isinstance(images, list):
            images = list(images)

        self._set_py_template_params_from_degree(len(images))
        self.init_cxx_obj(images)

    def __eq__(self: Self, other) -> bool:
        if not isinstance(_to_cxx(other), type(_to_cxx(self))):
            return False
        return _to_cxx(self) == _to_cxx(other)

    def __lt__(self: Self, other) -> bool:
        return _to_cxx(self) < _to_cxx(other)

    def __le__(self: Self, other) -> bool:
        return self < other or self == other

    def __gt__(self: Self, other) -> bool:
        return other < self

    def __ge__(self: Self, other) -> bool:
        return other < self or self == other

    def __mul__(self: Self, other: Self):
        result = one(self)
        result.product_inplace(self, other)
        return result

    @staticmethod
    @abc.abstractmethod
    def one(n: int) -> Self:  # pylint: disable=missing-function-docstring
        pass  # pragma: no cover

    def increase_degree_by(  # pylint: disable=missing-function-docstring
        self: Self, n: int
    ) -> Self:
        new_degree = n + self.degree()
        if new_degree > 2**32:
            raise ValueError(
                "the 1st argument (int) is too large, partial "
                "transformations of degree > 2 ** 32 are not supported, "
                f"expected at most {2**32 - self.degree()} but found {n}"
            )
        if self._cxx_type_change_required(new_degree):
            images = list(self.images())
            images.extend(range(self.degree(), new_degree))
            self._set_py_template_params_from_degree(len(images))
            self.init_cxx_obj(images)

        else:
            _to_cxx(self).increase_degree_by(n)
        return self


########################################################################
# Transf python class
########################################################################


class Transf(_PTransfBase):  # pylint: disable=missing-class-docstring
    __doc__ = _Transf1.__doc__

    _py_template_params_to_cxx_type = {
        (2**8,): _Transf1,
        (2**16,): _Transf2,
        (2**32,): _Transf4,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    )

    _all_wrapped_cxx_types = {_Transf1, _Transf2, _Transf4}

    # This method only exists to copy the doc. . .
    @_copydoc(_Transf1.__init__)
    def __init__(self: Self, imgs: list[int]) -> None:
        super().__init__(imgs)

    # We retain a separate __repr__ so that we can distinguish the cxx objects
    # and their python counterparts.
    def __repr__(self: Self) -> str:
        result = str(self)
        if len(result) < 72:
            return result
        return (
            f"<transformation of degree {self.degree()} and rank {self.rank()}>"
        )

    # We retain a separate __repr__ so that we can distinguish the cxx objects
    # and their python counterparts.
    def __str__(self: Self) -> str:
        return f"Transf({list(self.images())})"

    # This method only exists to copy the doc. . .
    @_copydoc(_Transf1.increase_degree_by)
    def increase_degree_by(self: Self, n: int) -> Self:
        # pylint: disable=missing-function-docstring
        _PTransfBase.increase_degree_by(self, n)
        return self

    @staticmethod
    @_copydoc(_Transf1.one)
    def one(n: int) -> Self:
        result_type = Transf._py_template_params_to_cxx_type[
            Transf._py_template_params_from_degree(n)
        ]
        return _to_py(result_type.one(n))


_copy_cxx_mem_fns(_Transf1, Transf)
_register_cxx_wrapped_type(_Transf1, Transf)
_register_cxx_wrapped_type(_Transf2, Transf)
_register_cxx_wrapped_type(_Transf4, Transf)

########################################################################
# PPerm python class
########################################################################


class PPerm(_PTransfBase):  # pylint: disable=missing-class-docstring
    __doc__ = _PPerm1.__doc__

    _py_template_params_to_cxx_type = {
        (2**8,): _PPerm1,
        (2**16,): _PPerm2,
        (2**32,): _PPerm4,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    )

    _all_wrapped_cxx_types = {_PPerm1, _PPerm2, _PPerm4}

    @staticmethod
    def _cxx_type_from_degree(n: int):
        if n <= 2**8:
            return _PPerm1
        if n <= 2**16:
            return _PPerm2
        return _PPerm4

    @_copydoc(_PPerm1.__init__)
    def __init__(self: Self, *args) -> None:
        if len(args) < 3:
            super().__init__(*args)
            return
        if len(args) != 3:
            raise TypeError(f"expected 1 or 3 arguments, found {len(args)}")
        dom, im, deg = args
        self._set_py_template_params_from_degree(deg)
        self.init_cxx_obj(dom, im, deg)

    # We retain a separate __repr__ so that we can distinguish the cxx objects
    # and their python counterparts.
    def __repr__(self: Self) -> str:
        result = str(self)
        if len(result) < 72:
            return result
        return (
            f"<partial perm of degree {self.degree()} and rank {self.rank()}>"
        )

    # We retain a separate __str__ so that we can distinguish the cxx objects
    # and their python counterparts.
    def __str__(self: Self) -> str:
        return f"PPerm({domain(self)}, {[self[i] for i in domain(self)]}, {self.degree()})"

    # This method only exists to copy the doc. . .
    @_copydoc(_PPerm1.increase_degree_by)
    def increase_degree_by(self: Self, n: int) -> Self:
        # pylint: disable=missing-function-docstring
        _PTransfBase.increase_degree_by(self, n)
        return self

    @staticmethod
    @_copydoc(_PPerm1.one)
    def one(n: int) -> Self:
        result_type = PPerm._py_template_params_to_cxx_type[
            PPerm._py_template_params_from_degree(n)
        ]
        return _to_py(result_type.one(n))


_copy_cxx_mem_fns(_PPerm1, PPerm)

_register_cxx_wrapped_type(_PPerm1, PPerm)
_register_cxx_wrapped_type(_PPerm2, PPerm)
_register_cxx_wrapped_type(_PPerm4, PPerm)


########################################################################
# Perm python class
########################################################################


# Deriving from Transf messes up the documentation
class Perm(_PTransfBase):  # pylint: disable=missing-class-docstring
    __doc__ = _Perm1.__doc__

    _py_template_params_to_cxx_type = {
        (2**8,): _Perm1,
        (2**16,): _Perm2,
        (2**32,): _Perm4,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    )

    _all_wrapped_cxx_types = {_Perm1, _Perm2, _Perm4}

    # This method only exists to copy the doc. . .
    @_copydoc(_Perm1.__init__)
    def __init__(self: Self, imgs: list[int]) -> None:
        super().__init__(imgs)

    # We retain a separate __repr__ so that we can distinguish the cxx objects
    # and their python counterparts.
    def __repr__(self: Self) -> str:
        result = str(self)
        if len(result) < 72:
            return result
        return f"<permutation of degree {self.degree()}>"

    # We retain a separate __repr__ so that we can distinguish the cxx objects
    # and their python counterparts.
    def __str__(self: Self) -> str:
        return f"Perm({list(self.images())})"

    # This method only exists to copy the doc. . .
    @_copydoc(_Perm1.increase_degree_by)
    def increase_degree_by(self: Self, n: int) -> Self:
        # pylint: disable=missing-function-docstring
        _PTransfBase.increase_degree_by(self, n)
        return self

    @staticmethod
    @_copydoc(_Perm1.one)
    def one(n: int) -> Self:
        result_type = Perm._py_template_params_to_cxx_type[
            Perm._py_template_params_from_degree(n)
        ]
        return _to_py(result_type.one(n))


_copy_cxx_mem_fns(_Perm1, Perm)

_register_cxx_wrapped_type(_Perm1, Perm)
_register_cxx_wrapped_type(_Perm2, Perm)
_register_cxx_wrapped_type(_Perm4, Perm)

########################################################################
# Helper functions
########################################################################

one = _wrap_cxx_free_fn(_transf_one)
inverse = _wrap_cxx_free_fn(_transf_inverse)
right_one = _wrap_cxx_free_fn(_transf_right_one)
left_one = _wrap_cxx_free_fn(_transf_left_one)
image = _wrap_cxx_free_fn(_transf_image)
domain = _wrap_cxx_free_fn(_transf_domain)
