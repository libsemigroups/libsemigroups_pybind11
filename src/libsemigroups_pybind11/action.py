# -*- coding: utf-8 -*-

# Copyright (c) 2024, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This page contains the documentation for the ``action`` subpackage, that
contains helper functions for the :any:`Action` class.
"""

from collections.abc import Iterator
from typing import TypeVar as _TypeVar
from typing_extensions import Self as _Self

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module
    BMat8 as _BMat8,
    LeftActionBMat8BMat8 as _LeftActionBMat8BMat8,
    LeftActionPPerm1List as _LeftActionPPerm1List,
    LeftActionPPerm1PPerm1 as _LeftActionPPerm1PPerm1,
    LeftActionPPerm2List as _LeftActionPPerm2List,
    LeftActionPPerm4List as _LeftActionPPerm4List,
    LeftActionTransf1List as _LeftActionTransf1List,
    LeftActionTransf2List as _LeftActionTransf2List,
    LeftActionTransf4List as _LeftActionTransf4List,
    PPerm1 as _PPerm1,
    PPerm2 as _PPerm2,
    PPerm4 as _PPerm4,
    RightActionBMat8BMat8 as _RightActionBMat8BMat8,
    RightActionPPerm1List as _RightActionPPerm1List,
    RightActionPPerm1PPerm1 as _RightActionPPerm1PPerm1,
    RightActionPPerm2List as _RightActionPPerm2List,
    RightActionPPerm4List as _RightActionPPerm4List,
    RightActionTransf1List as _RightActionTransf1List,
    RightActionTransf2List as _RightActionTransf2List,
    RightActionTransf4List as _RightActionTransf4List,
    Transf1 as _Transf1,
    Transf2 as _Transf2,
    Transf4 as _Transf4,
    UNDEFINED as _UNDEFINED,
    side as _side,
)

from .adapters import (
    ImageLeftAction as _ImageLeftAction,
    ImageRightAction as _ImageRightAction,
)
from .transf import PPerm as _PPerm, Transf as _Transf

from .detail.cxx_wrapper import (
    CxxWrapper as _CxxWrapper,
    copy_cxx_mem_fns as _copy_cxx_mem_fns,
    register_cxx_wrapped_type as _register_cxx_wrapped_type,
    to_cxx as _to_cxx,
    to_py as _to_py,
)

from .detail.decorators import copydoc as _copydoc


########################################################################
# Action python class
########################################################################


class Action(_CxxWrapper):  # pylint: disable=missing-class-docstring
    __doc__ = _RightActionPPerm1List.__doc__

    Element = _TypeVar("Element")
    Point = _TypeVar("Point")
    Func = _TypeVar("Func")
    Side = _TypeVar("Side")

    _py_template_params_to_cxx_type = {
        (
            _BMat8,
            _BMat8,
            _ImageRightAction,
            _side.right,
        ): _RightActionBMat8BMat8,
        (_BMat8, _BMat8, _ImageLeftAction, _side.left): _LeftActionBMat8BMat8,
        (
            _PPerm1,
            _PPerm1,
            _ImageRightAction,
            _side.right,
        ): _RightActionPPerm1PPerm1,
        (
            _PPerm1,
            _PPerm1,
            _ImageLeftAction,
            _side.left,
        ): _LeftActionPPerm1PPerm1,
        (
            _PPerm1,
            list,
            _ImageRightAction,
            _side.right,
        ): _RightActionPPerm1List,
        (
            _PPerm2,
            list,
            _ImageRightAction,
            _side.right,
        ): _RightActionPPerm2List,
        (
            _PPerm4,
            list,
            _ImageRightAction,
            _side.right,
        ): _RightActionPPerm4List,
        (
            _PPerm1,
            list,
            _ImageLeftAction,
            _side.left,
        ): _LeftActionPPerm1List,
        (
            _PPerm2,
            list,
            _ImageLeftAction,
            _side.left,
        ): _LeftActionPPerm2List,
        (
            _PPerm4,
            list,
            _ImageLeftAction,
            _side.left,
        ): _LeftActionPPerm4List,
        (
            _Transf1,
            list,
            _ImageRightAction,
            _side.right,
        ): _RightActionTransf1List,
        (
            _Transf2,
            list,
            _ImageRightAction,
            _side.right,
        ): _RightActionTransf2List,
        (
            _Transf4,
            list,
            _ImageRightAction,
            _side.right,
        ): _RightActionTransf4List,
        (
            _Transf1,
            list,
            _ImageLeftAction,
            _side.left,
        ): _LeftActionTransf1List,
        (
            _Transf2,
            list,
            _ImageLeftAction,
            _side.left,
        ): _LeftActionTransf2List,
        (
            _Transf4,
            list,
            _ImageLeftAction,
            _side.left,
        ): _LeftActionTransf4List,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    ########################################################################
    # Special methods
    ########################################################################

    # pylint: disable=redefined-outer-name, line-too-long
    def __init__(
        self: _Self, *args, generators=None, seeds=None, func=None, side=None
    ) -> None:
        """
        :sig=(self: Action, generators:list[Element] = None, seeds:list[Point] = None, func:collections.abc.Callable[[Point, Element], Point] = None, side:side = None) -> None:

        Construct an action from generators, seeds, function, and side.

        :Keyword Arguments:

            * **generators** (*list[Element]*)-- at least one generator for the action.
            * **seeds** (*list[Point]*) -- at least one seed point for the action.
            * **func** (*collections.abc.Callable[[Point, Element], Point]*) -- the function defining the action.
            * **side** (:any:`side`)-- the side of the action.

        :raises TypeError:
            if *generators* or *seeds* is not a list.
        :raises ValueError:
            if *generators* or *seeds* has length ``0``.
        :raises KeyError:
            if the action defined by the arguments is not implemented.
        """  # noqa: E501

        super().__init__(
            *args,
            required_kwargs=("func", "side", "seeds", "generators"),
            generators=generators,
            seeds=seeds,
            func=func,
            side=side,
        )
        if _to_cxx(self) is not None:
            return
        if len(args) != 0:
            raise TypeError(
                f"expected 0 positional arguments, but found {len(args)}"
            )
        if not isinstance(generators, list):
            raise TypeError(
                "expected the keyword argument 'generators' to be "
                f"a list but found {type(generators)}"
            )
        if not isinstance(seeds, list):
            raise TypeError(
                f"expected the keyword argument 'seeds' to be a list but found {type(seeds)}"
            )
        if len(generators) == 0:
            raise ValueError(
                "no generators have been specified, please specify at least one generator, "
                "more generators can be added later using Action.add_generator"
            )
        if len(seeds) == 0:
            raise ValueError(
                "no seeds have been specified, please specify at least one "
                "seed, more seeds can be added later using Action.add_seed"
            )

        generators = [_to_cxx(x) for x in generators]
        seeds = [_to_cxx(x) for x in seeds]
        self.py_template_params = (
            type(generators[0]),
            type(seeds[0]),
            func,
            side,
        )
        self.init_cxx_obj()
        for x in generators:
            self.add_generator(x)
        for x in seeds:
            self.add_seed(x)

    def __getitem__(self: _Self, i: int) -> Point:
        return _to_py(_to_cxx(self)[i])

    def __len__(self: _Self) -> int:
        return _to_cxx(self).size()

    def __contains__(self: _Self, pt: Point) -> bool:
        return self.position(pt) != _UNDEFINED

    ########################################################################
    # Iterators
    ########################################################################

    @_copydoc(_RightActionPPerm1PPerm1.generators)
    def generators(self: _Self) -> Iterator[Element]:
        # pylint: disable=missing-function-docstring
        return map(
            _to_py,
            _to_cxx(self).generators(),
        )


########################################################################
# Copy mem fns from sample C++ type and register types
########################################################################

_copy_cxx_mem_fns(_RightActionPPerm1PPerm1, Action)

for (
    _type
) in (
    Action._py_template_params_to_cxx_type.values()  # pylint: disable=protected-access
):
    _register_cxx_wrapped_type(_type, Action)

########################################################################
# RightAction python class
########################################################################


class RightAction(Action):
    """
    Class representing a right action of a semigroup or monoid on a set.

    This page contains the documentation for the class :any:`RightAction`,
    which just calls :any:`Action` with the keyword arguments *func* given by
    :any:`ImageRightAction` and *side* given by :any:`side.right`.
    """

    def __init__(self: _Self, *args, generators=None, seeds=None) -> None:
        """
        :sig=(self: RightAction, generators: list[Element]=None, seeds: list[Point]=None) -> None:

        Construct an :any:`Action` from generators and seeds,
        :any:`ImageRightAction` and :any:`side.right`.

        :Keyword Arguments:

            * **generators** (*list[Element]*)-- at least one generator for the action.
            * **seeds** (*list[Point]*) -- at least one seed point for the action.

        :raises TypeError:
            if *generators* or *seeds* is not a list.
        :raises ValueError:
            if *generators* or *seeds* has length ``0``.
        :raises KeyError:
            if the action given by the arguments is not yet implemented.

        .. |name| replace:: :any:`RightAction`

        .. include:: ../../_static/runner_non_inherit.rst
        """
        super().__init__(
            *args,
            generators=generators,
            seeds=seeds,
            side=_side.right,
            func=_ImageRightAction,
        )


########################################################################
# LeftAction python class
########################################################################


class LeftAction(Action):
    """
    Class representing a left action of a semigroup or monoid on a set.

    This page contains the documentation for the class ``LeftAction``, which
    just calls :any:`Action` with the keyword arguments *func* given by
    :any:`ImageLeftAction` and *side* given by :any:`side.left`.
    """

    def __init__(self: _Self, *args, generators=None, seeds=None) -> None:
        """
        :sig=(self: LeftAction, generators: list[Element]=None, seeds: list[Point]=None) -> None:

        Construct an :any:`Action` from generators and seeds,
        :any:`ImageLeftAction` and :any:`side.left`.

        :Keyword Arguments:

            * **generators** (*list[Element]*)-- at least one generator for the action.
            * **seeds** (*list[Point]*) -- at least one seed point for the action.

        :raises TypeError:
            if *generators* or *seeds* is not a list.
        :raises ValueError:
            if *generators* or *seeds* has length ``0``.
        :raises KeyError:
            if the action given by the arguments is not yet implemented.

        .. |name| replace:: :any:`LeftAction`

        .. include:: ../../_static/runner_non_inherit.rst
        """
        super().__init__(
            generators=generators,
            seeds=seeds,
            side=_side.left,
            func=_ImageLeftAction,
        )
