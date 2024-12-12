# -*- coding: utf-8 -*-

# Copyright (c) 2024, J. D. Mitchell, Joseph Edwards
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, unused-import, protected-access,
# pylint: disable=missing-function-docstring, line-too-long

"""
This package provides the user-facing python part of libsemigroups_pybind11 for
the KnuthBendix class from libsemigroups.
"""

from typing import List, Iterator, Union

from _libsemigroups_pybind11 import (
    KnuthBendixRewriteFromLeft as _KnuthBendixRewriteFromLeft,
    KnuthBendixRewriteTrie as _KnuthBendixRewriteTrie,
    PresentationStrings as _PresentationStrings,
    PresentationWords as _PresentationWords,
    congruence_kind as _congruence_kind,
    knuth_bendix_by_overlap_length as by_overlap_length,
    knuth_bendix_str_normal_forms as _knuth_bendix_str_normal_forms,
    knuth_bendix_word_normal_forms as _knuth_bendix_word_normal_forms,
    knuth_bendix_str_non_trivial_classes as _knuth_bendix_str_non_trivial_classes,
    knuth_bendix_word_non_trivial_classes as _knuth_bendix_word_non_trivial_classes,
    knuth_bendix_redundant_rule as redundant_rule,
    knuth_bendix_is_reduced as is_reduced,
)

from .detail.decorators import (
    may_return_positive_infinity as _may_return_positive_infinity,
    template_params_as_kwargs as _template_params_as_kwargs,
)

_Presentation = (_PresentationStrings, _PresentationWords)
_KnuthBendix = (_KnuthBendixRewriteFromLeft, _KnuthBendixRewriteTrie)

for KB in _KnuthBendix:
    KB.number_of_classes = _may_return_positive_infinity(KB._number_of_classes)
    KB.number_of_classes.__doc__ = "\n".join(
        KB._number_of_classes.__doc__.split("\n")[1:]
    )


def KnuthBendix(*args, rewriter="RewriteTrie"):  # pylint: disable=invalid-name
    """
    Construct a KnuthBendix instance of the type specified by its arguments
    """
    if len(args) not in (0, 2):
        raise TypeError(f"expected 0 or 2 positional arguments, found {len(args)}")

    if len(args) == 2 and not isinstance(args[0], _congruence_kind):
        raise TypeError(
            (
                "the 1st positional argument must be congruence_kind, "
                f"found ({type(args[0])})"
            )
        )

    if len(args) == 2 and not isinstance(args[1], _Presentation):
        raise TypeError(
            (
                "the 2nd positional argument must be presentation, "
                f"found ({type(args[1])})"
            )
        )

    if rewriter == "RewriteFromLeft":
        result = _KnuthBendixRewriteFromLeft(*args)
    elif rewriter == "RewriteTrie":
        result = _KnuthBendixRewriteTrie(*args)
    else:
        raise TypeError(
            (
                f'expected the rewriter kwarg to be either "RewriteTrie" or '
                f'"RewriteFromLeft", but found {rewriter}'
            )
        )

    return result


KnuthBendix.options = _KnuthBendixRewriteTrie.options


# The next function (non_trivial_classes) is documented here not in the cpp
# file because we add the additional kwarg Word.
@_template_params_as_kwargs(
    Word={
        str: _knuth_bendix_str_non_trivial_classes,
        List[int]: _knuth_bendix_word_non_trivial_classes,
    }
)
def non_trivial_classes(
    kb1: KnuthBendix,
    kb2: KnuthBendix,
    **kwargs,  # pylint: disable=unused-argument
) -> List[List[Union[str, List[int]]]]:
    r"""
    Find the non-trivial classes of the quotient of one KnuthBendix instance in
    another.

    This function returns the classes with size at least :math:`2` in the normal
    forms of *kb1* in *kb2* (the greater congruence, with fewer classes). This
    function triggers a full enumeration of both *kb1* and *kb2*. Note that this
    function does **not** compute the normal forms of *kb1* and try to compute the
    partition of these induced by *kb2*, before filtering out the classes of
    size :math:`1`. In particular, it is possible to compute the non-trivial
    classes of *kb2* in *kb1* if there are only finitely many finite such
    classes, regardless of whether or not *kb1* or *kb2* has infinitely many
    classes.

    :param kb1: the first KnuthBendix instance.
    :type kb1: KnuthBendixRewriteTrie

    :param kb2: the second KnuthBendix instance.
    :type kb2: KnuthBendixRewriteTrie

    :Keyword Arguments:
        * *Word* (``type``) -- type of the output words (must be ``str`` or ``List[int]``).

    :returns: The non-trivial classes of *kb1* in *kb2*.
    :rtype: List[List[str | List[int]]]

    :raises LibsemigroupsError:
        if *kb2* has infinitely many classes and *kb1* has finitely many
        classes (so that there is at least one infinite non-trivial class).

    :raises LibsemigroupsError:
        if the alphabets of the presentations of *kb1* and *kb2* are not equal.

    :raises LibsemigroupsError:
        if the :any:`KnuthBendixRewriteTrie.gilman_graph` of *kb2* has fewer nodes
        than that of *kb1*.

    :raises TypeError:
        if the keyword argument *Word* is not present, any other keyword
        argument is present, or is present but has value other than ``str`` or
        ``List[int]``.

    .. doctest::

        >>> from libsemigroups_pybind11 import (knuth_bendix, presentation,
        ... Presentation, congruence_kind, KnuthBendix)
        >>> from typing import List
        >>> p = Presentation("abc")
        >>> p.rules = ["ab", "ba", "ac", "ca", "aa", "a", "ac", "a", "ca",
        ... "a", "bc", "cb", "bbb", "b", "bc", "b", "cb", "b"]
        >>> kb1 = KnuthBendix(congruence_kind.twosided, p)
        >>> presentation.add_rule(p, "a", "b")
        >>> kb2 = KnuthBendix(congruence_kind.twosided, p)
        >>> knuth_bendix.non_trivial_classes(kb1, kb2, Word=str)
        [['b', 'ab', 'bb', 'abb', 'a']]
        >>> knuth_bendix.non_trivial_classes(kb1, kb2, Word=List[int])
        [[[98], [97, 98], [98, 98], [97, 98, 98], [97]]]
        >>> p = Presentation([0, 1, 2])
        >>> p.rules = [[0, 1], [1, 0], [0, 2], [2, 0], [0, 0], [0], [0, 2], [0], [2, 0],
        ... [0], [1, 2], [2, 1], [1, 1, 1], [1], [1, 2], [1], [2, 1], [1]]
        >>> kb1 = KnuthBendix(congruence_kind.twosided, p)
        >>> presentation.add_rule(p, [0], [1])
        >>> kb2 = KnuthBendix(congruence_kind.twosided, p)
        >>> knuth_bendix.non_trivial_classes(kb1, kb2, Word=List[int])
        [[[1], [0, 1], [1, 1], [0, 1, 1], [0]]]
    """


# The next function (normal_forms) is documented here not in the cpp
# file because we add the additional kwarg Word.
@_template_params_as_kwargs(
    Word={
        str: _knuth_bendix_str_normal_forms,
        List[int]: _knuth_bendix_word_normal_forms,
    }
)
def normal_forms(kb: KnuthBendix, **kwargs) -> Iterator[Union[str, List[int]]]:  # pylint: disable=unused-argument
    r"""
    Returns an iterator yielding normal forms.

    This function returns an iterator yielding normal forms of the classes of
    the congruence represented by an instance of :any:`KnuthBendixRewriteTrie`. The
    order of the classes, and the normal form that is returned, are controlled by
    the reduction order used to construct *kb*. This function triggers a full
    enumeration of *kb*.

    :param kb: the KnuthBendix instance.
    :type kb: KnuthBendixRewriteTrie

    :Keyword Arguments:
        * *Word* (``type``) -- type of the output words (must be ``str`` or ``List[int]``).

    :returns: An iterator.
    :rtype: Iterator[str | List[int]]

    :raises TypeError:
        if the keyword argument *Word* is not present, any other keyword
        argument is present, or is present but has value other than ``str`` or
        ``List[int]``.

    .. doctest::

        >>> from libsemigroups_pybind11 import (KnuthBendix, Presentation,
        ... presentation, congruence_kind)
        >>> from typing import List
        >>> p = Presentation("abc")
        >>> presentation.add_rule(p, "aaaa", "a")
        >>> presentation.add_rule(p, "bbbb", "b")
        >>> presentation.add_rule(p, "cccc", "c")
        >>> presentation.add_rule(p, "abab", "aaa")
        >>> presentation.add_rule(p, "bcbc", "bbb")
        >>> kb = KnuthBendix(congruence_kind.twosided, p)
        >>> kb.number_of_classes()
        +∞
        >>> list(knuth_bendix.normal_forms(kb, Word=str).min(1).max(3))
        ['a', 'b', 'c', 'aa', 'ab', 'ac', 'ba', 'bb', 'bc', 'ca', 'cb', 'cc']
        >>> list(knuth_bendix.normal_forms(kb, Word=List[int]).min(1).max(3))
        [[97], [98], [99], [97, 97], [97, 98], [97, 99], [98, 97], [98, 98], [98, 99], [99, 97], [99, 98], [99, 99]]
    """
