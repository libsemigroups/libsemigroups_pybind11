# -*- coding: utf-8 -*-

# Copyright (c) 2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This page contains the documentation for various helper functions for
manipulating :any:`Congruence` objects. All such functions are contained in
the submodule ``congruence``.
"""

from typing import Union as _Union
from typing_extensions import Self as _Self

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module
    CongruenceString as _CongruenceString,
    CongruenceWord as _CongruenceWord,
    congruence_non_trivial_classes as _congruence_non_trivial_classes,
    congruence_partition as _congruence_partition,
)

from .kambites import Kambites as _Kambites
from .knuth_bendix import KnuthBendix as _KnuthBendix
from .todd_coxeter import ToddCoxeter as _ToddCoxeter
from .presentation import Presentation as _Presentation

from .detail.decorators import copydoc as _copydoc

from .detail.congruence_common import CongruenceCommon as _CongruenceCommon

from .detail.cxx_wrapper import (
    copy_cxx_mem_fns as _copy_cxx_mem_fns,
    to_cxx as _to_cxx,
    wrap_cxx_free_fn as _wrap_cxx_free_fn,
    register_cxx_wrapped_type as _register_cxx_wrapped_type,
)


class Congruence(_CongruenceCommon):  # pylint: disable=missing-class-docstring
    __doc__ = _CongruenceWord.__doc__

    _py_template_params_to_cxx_type = {
        (list[int],): _CongruenceWord,
        (str,): _CongruenceString,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(_CongruenceWord.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, **kwargs)
        if _to_cxx(self) is not None:
            return

        if len(args) == 2:
            if isinstance(args[1], _Presentation):
                self.py_template_params = args[1].py_template_params
            else:
                raise TypeError(
                    f"expected the 2nd argument to be a Presentation, but found {type(args[1])}"
                )
        self.init_cxx_obj(*args)

    def get(
        self: _Self, t: type
    ) -> _Union[_Kambites, _KnuthBendix, _ToddCoxeter]:
        """
        :sig=(self: Congruence, t: type) -> Kambites | KnuthBendix | ToddCoxeter:
        Returns the *t* instance used to compute the congruence (if any).

        This function returns (a copy of) the object of type *t* used to
        compute the congruence (if any). If there is no such object, then an
        exception is raised.

        :param t:
            The type of object being sought (must be
            :any:`Kambites`, :any:`KnuthBendix`, or :any:`ToddCoxeter`).
        :type t: type

        :returns: A copy of the instance of type *t*.
        :rtype: Kambites | KnuthBendix | ToddCoxeter

        :raises LibsemigroupsError:
            if there is no object of type *t* within *self*.
        """
        if t is _KnuthBendix:
            return _KnuthBendix(self._get_knuth_bendix())
        if t is _ToddCoxeter:
            return _ToddCoxeter(self._get_todd_coxeter())
        if t is _Kambites:
            return _Kambites(self._get_kambites())
        raise ValueError(
            "expected the 1st argument to be one of Kambites, KnuthBendix, or ToddCoxeter"
            f", but found {t}"
        )

    def has(self: _Self, t: type) -> bool:
        """
        :sig=(self: Congruence, t: type) -> bool:

        Returns whether or not there is an instance of type *t* within the
        congruence.

        This function returns ``True`` if *self* contains an instance of type
        *t*, and ``False`` if not.

        :param t:
            The type of object being sought (must be :any:`Kambites`,
            :any:`KnuthBendix`, or :any:`ToddCoxeter`).
        :type t: type

        :returns: Whether or not there is an object of type *t* in *self*.
        :rtype: bool
        """
        if t is _KnuthBendix:
            return self._has_knuth_bendix()
        if t is _ToddCoxeter:
            return self._has_todd_coxeter()
        if t is _Kambites:
            return self._has_kambites()
        return False


_copy_cxx_mem_fns(_CongruenceWord, Congruence)
_register_cxx_wrapped_type(_CongruenceWord, Congruence)
_register_cxx_wrapped_type(_CongruenceString, Congruence)

partition = _wrap_cxx_free_fn(_congruence_partition)
non_trivial_classes = _wrap_cxx_free_fn(_congruence_non_trivial_classes)
