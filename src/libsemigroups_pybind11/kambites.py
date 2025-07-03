# -*- coding: utf-8 -*-

# Copyright (c) 2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This page contains the documentation for the ``kambites`` subpackage, that
contains helper functions for the :any:`Kambites` class.
"""

from typing_extensions import Self as _Self

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module
    KambitesMultiViewString as _KambitesMultiViewString,
    KambitesWord as _KambitesWord,
    KambitesString as _KambitesString,
    congruence_kind as _congruence_kind,
    kambites_non_trivial_classes as _kambites_non_trivial_classes,
    kambites_normal_forms as _kambites_normal_forms,
    kambites_partition as _kambites_partition,
)

from .detail.decorators import (
    copydoc as _copydoc,
)

from .detail.congruence_common import CongruenceCommon as _CongruenceCommon


from .detail.cxx_wrapper import (
    copy_cxx_mem_fns as _copy_cxx_mem_fns,
    to_cxx as _to_cxx,
    wrap_cxx_free_fn as _wrap_cxx_free_fn,
    register_cxx_wrapped_type as _register_cxx_wrapped_type,
)

from .presentation import Presentation as _Presentation


class Kambites(_CongruenceCommon):  # pylint: disable=missing-class-docstring
    __doc__ = _KambitesWord.__doc__

    _py_template_params_to_cxx_type = {
        (list[int],): _KambitesWord,
        (str,): _KambitesMultiViewString,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    ) | {_KambitesString: (str,)}

    _all_wrapped_cxx_types = {*_cxx_type_to_py_template_params.keys()}

    @_copydoc(_KambitesWord.__init__)
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


_copy_cxx_mem_fns(_KambitesWord, Kambites)

_register_cxx_wrapped_type(_KambitesWord, Kambites)
_register_cxx_wrapped_type(_KambitesMultiViewString, Kambites)
_register_cxx_wrapped_type(_KambitesString, Kambites)

partition = _wrap_cxx_free_fn(_kambites_partition)
non_trivial_classes = _wrap_cxx_free_fn(_kambites_non_trivial_classes)
normal_forms = _wrap_cxx_free_fn(_kambites_normal_forms)
