# -*- coding: utf-8 -*-

# Copyright (c) 2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name, missing-function-docstring
# pylint: disable=unused-import, missing-module-docstring, protected-access
# pylint: disable=duplicate-code

from typing import Union, List, Iterator, Self

from _libsemigroups_pybind11 import (
    PositiveInfinity,
    PresentationStrings as _PresentationStrings,
    PresentationWords as _PresentationWords,
    ToddCoxeterImpl as _ToddCoxeterImpl,
    ToddCoxeterString as _ToddCoxeterString,
    ToddCoxeterWord as _ToddCoxeterWord,
    Undefined,
    WordGraph as _WordGraph,
    class_by_index,
    class_of,
    congruence_kind as _congruence_kind,
    todd_coxeter_is_non_trivial as is_non_trivial,
    todd_coxeter_non_trivial_classes as non_trivial_classes,
    todd_coxeter_normal_forms as normal_forms,
    todd_coxeter_partition as partition,
    todd_coxeter_redundant_rule as redundant_rule,
)

from .detail.decorators import (
    may_return_positive_infinity as _may_return_positive_infinity,
    may_return_undefined as _may_return_undefined,
    template_params_as_kwargs as _template_params_as_kwargs,
)

from .detail.cxx_wrapper import (
    CxxWrapper,
)

from .detail import cong_intf


def ToddCoxeter(*args, **kwargs):
    """
    Function pretending to be a class, just dispatches to the corresponding
    constructor of the appropriate type based on the arguments.
    """

    cong_intf.raise_if_bad_args(*args, **kwargs)

    if len(args) == 0:
        Word = kwargs["Word"]
    else:
        assert len(args) == 2
        if isinstance(args[1], (_PresentationStrings, _ToddCoxeterString)):
            Word = str
        elif isinstance(
            args[1],
            (
                _PresentationWords,
                _ToddCoxeterWord,
                _WordGraph,
            ),
        ):
            Word = List[int]
        else:
            raise TypeError(
                "expected the 2nd argument to be Presentation, ToddCoxeter, "
                f"or WordGraph but found {type(args[1])}"
            )
    cpp_type = {
        List[int]: _ToddCoxeterWord,
        str: _ToddCoxeterString,
    }
    return cpp_type[Word](*args)


ToddCoxeter.options = _ToddCoxeterImpl.options

for _ToddCoxeter in [_ToddCoxeterWord, _ToddCoxeterString]:
    _ToddCoxeter.number_of_classes = _may_return_positive_infinity(
        _ToddCoxeter._number_of_classes
    )

    _ToddCoxeter.current_index_of = _may_return_undefined(
        _ToddCoxeter._current_index_of
    )

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
