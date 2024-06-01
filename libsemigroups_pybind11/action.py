# -*- coding: utf-8 -*-

# Copyright (c) 2024, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint:disable=no-name-in-module, unused-import, protected-access,
# pylint:disable=missing-function-docstring

"""
This package provides the user-facing python part of libsemigroups_pybind11 for
the Action class from libsemigroups.
"""

from typing import Any, Union, Iterator
from typing_extensions import Self

from _libsemigroups_pybind11 import (
    RightActionBMat8BMat8 as _RightActionBMat8BMat8,
    LeftActionBMat8BMat8 as _LeftActionBMat8BMat8,
    RightActionPPerm16List as _RightActionPPerm16List,
    RightActionPPerm1List as _RightActionPPerm1List,
    RightActionPPerm2List as _RightActionPPerm2List,
    RightActionPPerm4List as _RightActionPPerm4List,
    RightActionPPerm16PPerm16 as _RightActionPPerm16PPerm16,
    LeftActionPPerm16List as _LeftActionPPerm16List,
    LeftActionPPerm1List as _LeftActionPPerm1List,
    LeftActionPPerm2List as _LeftActionPPerm2List,
    LeftActionPPerm4List as _LeftActionPPerm4List,
    LeftActionPPerm16PPerm16 as _LeftActionPPerm16PPerm16,
    RightActionTransf16List as _RightActionTransf16List,
    RightActionTransf1List as _RightActionTransf1List,
    RightActionTransf2List as _RightActionTransf2List,
    RightActionTransf4List as _RightActionTransf4List,
    LeftActionTransf16List as _LeftActionTransf16List,
    LeftActionTransf1List as _LeftActionTransf1List,
    LeftActionTransf2List as _LeftActionTransf2List,
    LeftActionTransf4List as _LeftActionTransf4List,
    StaticPPerm16 as _StaticPPerm16,
    PPerm1 as _PPerm1,
    PPerm2 as _PPerm2,
    PPerm4 as _PPerm4,
    StaticTransf16 as _StaticTransf16,
    Transf1 as _Transf1,
    Transf2 as _Transf2,
    Transf4 as _Transf4,
)

from _libsemigroups_pybind11 import BMat8, side, UNDEFINED

from .adapters import ImageRightAction, ImageLeftAction
from .py_wrappers import to_cpp, to_py, pass_thru_methods
from .runner import Runner
from .transf import PPerm, Transf


class Action(
    Runner
):  # pylint: disable=invalid-name, too-many-instance-attributes, no-member
    """
    The documentation for this class is taken from RightActionPPerm16List in
    src/action.cpp!
    """

    _CppObjWrapper__lookup = {
        (BMat8, BMat8, ImageRightAction, side.right): _RightActionBMat8BMat8,
        (BMat8, BMat8, ImageLeftAction, side.left): _LeftActionBMat8BMat8,
        (PPerm, PPerm, ImageRightAction, side.right): {
            (
                _StaticPPerm16,
                _StaticPPerm16,
                ImageRightAction,
                side.right,
            ): _RightActionPPerm16PPerm16,
        },
        (PPerm, PPerm, ImageLeftAction, side.left): {
            (
                _StaticPPerm16,
                _StaticPPerm16,
                ImageLeftAction,
                side.left,
            ): _LeftActionPPerm16PPerm16,
        },
        (PPerm, list, ImageRightAction, side.right): {
            (
                _StaticPPerm16,
                list,
                ImageRightAction,
                side.right,
            ): _RightActionPPerm16List,
            (
                _PPerm1,
                list,
                ImageRightAction,
                side.right,
            ): _RightActionPPerm1List,
            (
                _PPerm2,
                list,
                ImageRightAction,
                side.right,
            ): _RightActionPPerm2List,
            (
                _PPerm4,
                list,
                ImageRightAction,
                side.right,
            ): _RightActionPPerm4List,
        },
        (PPerm, list, ImageLeftAction, side.left): {
            (
                _StaticPPerm16,
                list,
                ImageLeftAction,
                side.left,
            ): _LeftActionPPerm16List,
            (
                _PPerm1,
                list,
                ImageLeftAction,
                side.right,
            ): _LeftActionPPerm1List,
            (
                _PPerm2,
                list,
                ImageLeftAction,
                side.right,
            ): _LeftActionPPerm2List,
            (
                _PPerm4,
                list,
                ImageLeftAction,
                side.right,
            ): _LeftActionPPerm4List,
        },
        (Transf, list, ImageRightAction, side.right): {
            (
                _StaticTransf16,
                list,
                ImageRightAction,
                side.right,
            ): _RightActionTransf16List,
            (
                _Transf1,
                list,
                ImageRightAction,
                side.right,
            ): _RightActionTransf1List,
            (
                _Transf2,
                list,
                ImageRightAction,
                side.right,
            ): _RightActionTransf2List,
            (
                _Transf4,
                list,
                ImageRightAction,
                side.right,
            ): _RightActionTransf4List,
        },
        (Transf, list, ImageLeftAction, side.left): {
            (
                _StaticTransf16,
                list,
                ImageLeftAction,
                side.left,
            ): _LeftActionTransf16List,
            (
                _Transf1,
                list,
                ImageLeftAction,
                side.right,
            ): _LeftActionTransf1List,
            (
                _Transf2,
                list,
                ImageLeftAction,
                side.right,
            ): _LeftActionTransf2List,
            (
                _Transf4,
                list,
                ImageLeftAction,
                side.right,
            ): _LeftActionTransf4List,
        },
    }

    def __init__(self: Self, **kwargs):
        super().__init__(("Element", "Point", "Func", "Side"), **kwargs)
        self.init()

    def _init_cpp_obj(self: Self) -> None:
        # pylint: disable=attribute-defined-outside-init
        if self._cpp_obj is not None:
            return

        if len(self._generators) == 0:
            raise ValueError(
                "no generators have been specified, please add generators "
                "using Action.add_generator"
            )
        if len(self._seeds) == 0:
            raise ValueError(
                "no seeds have been specified, please add seeds using "
                "Action.add_seed"
            )

        cpp_obj_t = self._cpp_obj_type_from(
            samples=(self._generators[0], self._seeds[0]),
            types=(self.Func, self.Side),
        )

        self._cpp_obj = cpp_obj_t()

        for x in self._generators:
            self._cpp_obj.add_generator(to_cpp(x))
        for x in self._seeds:
            self._cpp_obj.add_seed(to_cpp(x))
        self._cpp_obj.cache_scc_multipliers(self._cache_scc_multipliers)
        self._cpp_obj.reserve(self._reserve)

    def __repr__(self: Self) -> str:
        result = super().__repr__()
        if result:
            return result
        # TODO add side
        return (
            f"<incomplete action with {len(self._generators)} generators, "
            + f"{len(self._seeds)} points>"
        )

    def __getitem__(self: Self, pos: int) -> Any:
        self._init_cpp_obj()
        return to_py(self.Point, self._cpp_obj[pos])

    def add_generator(self: Self, x: Any) -> Self:
        if not isinstance(x, self.Element):
            raise ValueError(
                "the argument (generator) has incorrect type, expected "
                f"{self.Element} but found {type(x)}"
            )
        if (
            len(self._generators) != 0
            and hasattr(x, "degree")
            and x.degree() != self._generators[0].degree()
        ):
            raise ValueError(
                "the argument (generator) has incorrect degree, expected "
                f"{self._generators[0].degree()} but found {x.degree()}"
            )

        if self._cpp_obj is not None:
            self._cpp_obj.add_generator(to_cpp(x))
        else:
            self._generators.append(x)
        return self

    def add_seed(self: Self, x: Any) -> Self:
        if not isinstance(x, self.Point):
            raise ValueError(
                "the argument (seed) has incorrect type, expected "
                f"{self.Element} but found {type(x)}"
            )
        if self._cpp_obj is not None:
            self._cpp_obj.add_seed(to_cpp(x))
        else:
            # TODO check compatibility of x with things already in _seeds,
            # i.e. degree. Not currently sure how to do this
            self._seeds.append(x)
        return self

    def cache_scc_multipliers(self: Self, *args) -> Union[Self, bool]:
        # pylint: disable=attribute-defined-outside-init
        if self._cpp_obj is not None:
            return self._cpp_obj.cache_scc_multipliers(*args)
        if len(args) == 0:
            return self._cache_scc_multipliers
        if len(args) == 1 and isinstance(args[0], bool):
            self._cache_scc_multipliers = args[0]
            return self
        raise ValueError(
            f"expected 0 arguments or 1 argument (a bool), found {len(args)} "
            "arguments"
        )

    def current_size(self: Self) -> int:
        if self._cpp_obj is not None:
            return self._cpp_obj.current_size()
        return len(self._seeds)

    def empty(self: Self) -> bool:
        return self.current_size() != 0

    def init(self: Self) -> Self:
        self._generators = []
        self._seeds = []
        self._cache_scc_multipliers = False
        self._reserve = 0
        self._cpp_obj = None
        return self

    def multiplier_from_scc_root(self: Self, pos: int) -> Any:
        self._init_cpp_obj()
        return to_py(self.Element, self._cpp_obj.multiplier_from_scc_root(pos))

    def multiplier_to_scc_root(self: Self, pos: int) -> Any:
        self._init_cpp_obj()
        return to_py(self.Element, self._cpp_obj.multiplier_to_scc_root(pos))

    def number_of_generators(self: Self) -> int:
        if self._cpp_obj is None:
            return len(self._generators)

        return self._cpp_obj.number_of_generators()

    def __contains__(self: Self, pt: Any) -> bool:
        return self.index(pt) != UNDEFINED

    def reserve(self: Self, val: int) -> Self:
        # pylint: disable=attribute-defined-outside-init
        if self._cpp_obj is None:
            self._reserve = val
        else:
            self._cpp_obj.reserve(val)
        return self

    def root_of_scc(self: Self, x: Any) -> Any:
        if not isinstance(x, self.Element) and not isinstance(x, int):
            raise ValueError(
                "the 1st argument has incorrect type expected "
                f"{self.Element} or int, found {type(x)}"
            )

        self.run()
        if isinstance(x, self.Element):
            x = to_cpp(x)
        return self._cpp_obj.root_of_scc(x)


pass_thru_methods(
    Action,
    "word_graph",
    "scc",
    ("__len__", "size"),
    ("index", "position"),
    "iterator",
)


def RightAction(
    Func=ImageRightAction, **kwargs
):  # pylint: disable=invalid-name
    """
    Construct a right :any:`RightActionPPerm16List` instance.

    :Keyword Arguments:
        * *Element* -- the type of the elements in the action
        * *Point* -- the type of the points acted on
        * *Func* -- the function defining the action (defaults to
          :any:`ImageRightActionPPerm16PPerm16`)
    """
    # TODO probably this will generate unhelpful error messages
    return Action(
        Func=Func,
        Element=kwargs["Element"],
        Point=kwargs["Point"],
        Side=side.right,
    )


def LeftAction(Func=ImageLeftAction, **kwargs):  # pylint: disable=invalid-name
    """
    Construct a left :any:`RightActionPPerm16List` instance.

    :Keyword Arguments:
        * *Element* -- the type of the elements in the action
        * *Point* -- the type of the points acted on
        * *Func* -- the function defining the action (defaults to
          :any:`ImageLeftActionPPerm16PPerm16`)

    """
    # TODO probably this will generate unhelpful error messages
    return Action(
        Func=Func,
        Element=kwargs["Element"],
        Point=kwargs["Point"],
        Side=side.left,
    )
