# -*- coding: utf-8 -*-

# Copyright (c) 2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name, missing-function-docstring
# pylint: disable=unused-import, missing-module-docstring, protected-access

from typing import Union

from _libsemigroups_pybind11 import (
    ToddCoxeter,
    PositiveInfinity,
    todd_coxeter_str_normal_forms as str_normal_forms,
    todd_coxeter_word_normal_forms as word_normal_forms,
    word_class_by_index,
    str_class_by_index,
    class_of,
    todd_coxeter_is_non_trivial as is_non_trivial,
    todd_coxeter_partition as partition,
    todd_coxeter_non_trivial_classes as non_trivial_classes,
    todd_coxeter_redundant_rule as redundant_rule,
    Dot as _Dot,
)

from .detail.decorators import (
    may_return_positive_infinity as _may_return_positive_infinity,
    may_return_undefined as _may_return_undefined,
)

ToddCoxeter.number_of_classes = _may_return_positive_infinity(
    ToddCoxeter._number_of_classes
)
ToddCoxeter.number_of_classes.__doc__ = "\n".join(
    ToddCoxeter._number_of_classes.__doc__.split("\n")[1:]
)

ToddCoxeter.current_index_of = _may_return_undefined(
    ToddCoxeter._current_index_of
)
ToddCoxeter.current_index_of.__doc__ = "\n".join(
    ToddCoxeter._current_index_of.__doc__.split("\n")[1:]
)


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
