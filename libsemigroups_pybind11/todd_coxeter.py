# -*- coding: utf-8 -*-

# Copyright (c) 2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name, missing-function-docstring
# pylint: disable=unused-import, missing-module-docstring, protected-access

from typing import Union, List, Iterator

from _libsemigroups_pybind11 import (
    ToddCoxeter,
    PositiveInfinity,
    todd_coxeter_str_normal_forms as _str_normal_forms,
    todd_coxeter_word_normal_forms as _word_normal_forms,
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

ToddCoxeter.current_index_of = _may_return_undefined(ToddCoxeter._current_index_of)
ToddCoxeter.current_index_of.__doc__ = "\n".join(
    ToddCoxeter._current_index_of.__doc__.split("\n")[1:]
)


# The next function (normal_forms) is documented here not in the cpp
# file because we add the additional kwarg Word.
def normal_forms(kb: ToddCoxeter, **kwargs) -> Iterator[str | List[int]]:
    r"""
    Returns an iterator yielding normal forms.

    This function returns an iterator yielding normal forms of the classes of
    the congruence represented by an instance of :any:`ToddCoxeter`. The order of
    the classes, and the normal forms, that are returned are controlled by
    :any:`ToddCoxeter.standardize`. This function triggers a full enumeration of
    ``tc``.

    :param tc: the ToddCoxeter instance.
    :type tc: ToddCoxeter

    :Keyword Arguments:
        * *Word* (``type``) -- type of the output words (must be ``str`` or ``List[int]``).

    :returns: An iterator yielding normal forms.
    :rtype: Iterator[str | List[int]]

    :raises TypeError:
        if the keyword argument *Word* is not present, any other keyword
        argument is present, or is present but has value other than ``str`` or
        ``List[int]``.
    """
    if len(kwargs) != 1:
        raise TypeError(f"expected 1 keyword argument, but found {len(kwargs)}")
    if "Word" not in kwargs:
        raise TypeError(
            f'expected keyword argument "Word", but found "{next(iter(kwargs))}"'
        )
    if kwargs["Word"] is List[int]:
        return _word_normal_forms(kb)
    if kwargs["Word"] is str:
        return _str_normal_forms(kb)

    val = kwargs["Word"]
    val = f'"{val}"' if isinstance(val, str) else val

    raise TypeError(
        'expected the value of the keyword argument "Word" to be '
        f"List[int] or str, but found {val}"
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
