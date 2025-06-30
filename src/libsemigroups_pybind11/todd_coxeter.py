# -*- coding: utf-8 -*-

# Copyright (c) 2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This page contains the documentation for various helper functions for
manipulating :any:`ToddCoxeter` objects. All such functions
are contained in the subpackage ``todd_coxeter``.
"""

from typing_extensions import Self as _Self

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module
    ToddCoxeterImpl as _ToddCoxeterImpl,
    ToddCoxeterString as _ToddCoxeterString,
    ToddCoxeterWord as _ToddCoxeterWord,
    WordGraph as _WordGraph,
    todd_coxeter_class_by_index as _todd_coxeter_class_by_index,
    todd_coxeter_class_of as _todd_coxeter_class_of,
    todd_coxeter_is_non_trivial as _todd_coxeter_is_non_trivial,
    todd_coxeter_non_trivial_classes as _todd_coxeter_non_trivial_classes,
    todd_coxeter_normal_forms as _todd_coxeter_normal_forms,
    todd_coxeter_partition as _todd_coxeter_partition,
    todd_coxeter_perform_lookbehind as _todd_coxeter_perform_lookbehind,
    todd_coxeter_redundant_rule as _todd_coxeter_redundant_rule,
)

from .detail.cxx_wrapper import (
    to_cxx as _to_cxx,
    copy_cxx_mem_fns as _copy_cxx_mem_fns,
    wrap_cxx_free_fn as _wrap_cxx_free_fn,
    register_cxx_wrapped_type as _register_cxx_wrapped_type,
)

from .detail.decorators import copydoc as _copydoc

from .detail.congruence_common import CongruenceCommon as _CongruenceCommon

from .presentation import Presentation as _Presentation


class ToddCoxeter(_CongruenceCommon):  # pylint: disable=missing-class-docstring
    __doc__ = _ToddCoxeterWord.__doc__

    _py_template_params_to_cxx_type = {
        (list[int],): _ToddCoxeterWord,
        (str,): _ToddCoxeterString,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    options = _ToddCoxeterImpl.options

    @_copydoc(_ToddCoxeterWord.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        super().__init__(*args, **kwargs)
        if _to_cxx(self) is not None:
            return
        if len(args) == 2:
            if isinstance(args[1], _Presentation):
                self.py_template_params = args[1].py_template_params
            elif isinstance(args[1], _WordGraph):
                self.py_template_params = (list[int],)
            elif isinstance(args[1], ToddCoxeter):
                self.py_template_params = args[1].py_template_params
            else:
                raise TypeError(
                    "expected the 2nd argument to be Presentation, ToddCoxeter, "
                    f"or WordGraph but found {type(args[1])}"
                )

        self.init_cxx_obj(*args)


_copy_cxx_mem_fns(_ToddCoxeterWord, ToddCoxeter)
_register_cxx_wrapped_type(_ToddCoxeterString, ToddCoxeter)
_register_cxx_wrapped_type(_ToddCoxeterWord, ToddCoxeter)

class_by_index = _wrap_cxx_free_fn(_todd_coxeter_class_by_index)
class_of = _wrap_cxx_free_fn(_todd_coxeter_class_of)
is_non_trivial = _wrap_cxx_free_fn(_todd_coxeter_is_non_trivial)
non_trivial_classes = _wrap_cxx_free_fn(_todd_coxeter_non_trivial_classes)
normal_forms = _wrap_cxx_free_fn(_todd_coxeter_normal_forms)
partition = _wrap_cxx_free_fn(_todd_coxeter_partition)
perform_lookbehind = _wrap_cxx_free_fn(_todd_coxeter_perform_lookbehind)
redundant_rule = _wrap_cxx_free_fn(_todd_coxeter_redundant_rule)

########################################################################
# Helper functions
########################################################################

# def fancy_dot(tc: ToddCoxeter) -> _Dot:
#    dot = word_graph.dot(tc.word_graph())
#    offset = 0 if tc.presentation().contains_empty_word() else 1
#    dot.node("0").add_attr("label", "ε")
#    tree = tc.spanning_tree()
#    for i in range(1, tc.number_of_classes() + offset):
#        w = tc.word_of(i - offset)
#        dot.node(str(i)).add_attr("label", "".join([chr(x) for x in w]))
#        if len(w) == 1:
#            dot.node(str(i)).add_attr(
#                "color", dot.edge("0", str(i)).attrs["color"]
#            ).add_attr("style", "filled")
#        if tree.parent(i) != 18446744073709551615:
#            dot.edge(str(tree.parent(i)), str(i)).add_attr(
#                "style", "dashed,bold"
#            )
#
#    return dot
