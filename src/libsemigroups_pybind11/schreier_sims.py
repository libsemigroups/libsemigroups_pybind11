# -*- coding: utf-8 -*-

# Copyright (c) 2024 Joseph Edwards
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This page contains the documentation for the ``schreier_sims`` subpackage, that
contains helper functions for the :any:`SchreierSims` class.
"""

from typing import TypeVar as _TypeVar
from typing_extensions import Self as _Self

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module
    schreier_sims_intersection as _schreier_sims_intersection,
    SchreierSimsPerm1 as _SchreierSimsPerm1,
    SchreierSimsPerm2 as _SchreierSimsPerm2,
    Perm1 as _Perm1,
    Perm2 as _Perm2,
    # Perm4 as _Perm4,
)

from .detail.cxx_wrapper import (
    to_cxx as _to_cxx,
    CxxWrapper as _CxxWrapper,
    copy_cxx_mem_fns as _copy_cxx_mem_fns,
    register_cxx_wrapped_type as _register_cxx_wrapped_type,
    wrap_cxx_free_fn as _wrap_cxx_free_fn,
)

from .detail.decorators import copydoc as _copydoc

Element = _TypeVar("Element")

########################################################################
# ScheierSims python class
########################################################################


class SchreierSims(_CxxWrapper):  # pylint: disable=missing-class-docstring
    __doc__ = _SchreierSimsPerm1.__doc__

    _py_template_params_to_cxx_type = {
        (_Perm1,): _SchreierSimsPerm1,
        (_Perm2,): _SchreierSimsPerm2,
        # (_Perm4,): _SchreierSims,
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

    # TODO(1): This __init__ is identical to the FroidurePin __init__. It would
    # probably be best to make an abstract base class from which all classes
    # that construct using a list of generators inherit.
    @_copydoc(_SchreierSimsPerm1.__init__)
    def __init__(self: _Self, *args) -> None:
        super().__init__(*args)
        if _to_cxx(self) is not None:
            return
        if len(args) == 0:
            raise TypeError("expected at least 1 argument, found 0")

        if isinstance(args[0], list) and len(args) == 1:
            gens = args[0]
        else:
            gens = list(args)
        gens = [_to_cxx(x) for x in gens]
        self.py_template_params = (type(gens[0]),)
        # There's no SchreierSims constructor from std::vector<Element> so just
        # default construct and then add the generators
        self.init_cxx_obj()
        for gen in gens:
            self.add_generator(gen)


########################################################################
# Copy mem fns from sample C++ type and register types
########################################################################

_copy_cxx_mem_fns(_SchreierSimsPerm1, SchreierSims)

_register_cxx_wrapped_type(_SchreierSimsPerm1, SchreierSims)
_register_cxx_wrapped_type(_SchreierSimsPerm2, SchreierSims)

########################################################################
# Helpers -- from schreier-sims.cpp
########################################################################


intersection = _wrap_cxx_free_fn(_schreier_sims_intersection)
