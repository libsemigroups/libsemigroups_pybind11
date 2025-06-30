# -*- coding: utf-8 -*-

# Copyright (c) 2022-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
The full API for :any:`Presentation` helper functions is given below.
"""

from typing import Union as _Union
from typing_extensions import Self as _Self

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module
    InversePresentationString as _InversePresentationString,
    InversePresentationWord as _InversePresentationWord,
    PresentationString as _PresentationString,
    PresentationWord as _PresentationWord,
    presentation_add_identity_rules as _add_identity_rules,
    presentation_add_inverse_rules as _add_inverse_rules,
    presentation_add_rule as _add_rule,
    presentation_add_rules as _add_rules,
    presentation_add_zero_rules as _add_zero_rules,
    presentation_are_rules_sorted as _are_rules_sorted,
    presentation_change_alphabet as _change_alphabet,
    presentation_contains_rule as _contains_rule,
    presentation_first_unused_letter as _first_unused_letter,
    presentation_greedy_reduce_length as _greedy_reduce_length,
    presentation_greedy_reduce_length_and_number_of_gens as _greedy_reduce_length_and_num_of_gens,
    presentation_is_strongly_compressible as _is_strongly_compressible,
    presentation_length as _length,
    presentation_longest_rule as _longest_rule,
    presentation_longest_rule_length as _longest_rule_length,
    presentation_longest_subword_reducing_length as _longest_subword_reducing_length,
    presentation_make_semigroup as _make_semigroup,
    presentation_normalize_alphabet as _normalize_alphabet,
    presentation_reduce_complements as _reduce_complements,
    presentation_reduce_to_2_generators as _reduce_to_2_generators,
    presentation_remove_duplicate_rules as _remove_duplicate_rules,
    presentation_remove_redundant_generators as _remove_redundant_generators,
    presentation_remove_trivial_rules as _remove_trivial_rules,
    presentation_replace_subword as _replace_subword,
    presentation_replace_word as _replace_word,
    presentation_replace_word_with_new_generator as _replace_word_with_new_generator,
    presentation_reverse as _reverse,
    presentation_shortest_rule as _shortest_rule,
    presentation_shortest_rule_length as _shortest_rule_length,
    presentation_sort_each_rule as _sort_each_rule,
    presentation_sort_rules as _sort_rules,
    presentation_strongly_compress as _strongly_compress,
    presentation_throw_if_bad_inverses as _throw_if_bad_inverses,
    presentation_to_gap_string as _to_gap_string,
)

from libsemigroups_pybind11.detail.cxx_wrapper import (
    CxxWrapper as _CxxWrapper,
    copy_cxx_mem_fns as _copy_cxx_mem_fns,
    wrap_cxx_free_fn as _wrap_cxx_free_fn,
    to_cxx as _to_cxx,
    register_cxx_wrapped_type as _register_cxx_wrapped_type,
)
from libsemigroups_pybind11.detail.decorators import copydoc as _copydoc

########################################################################
# Presentation
########################################################################


class Presentation(_CxxWrapper):  # pylint: disable=missing-class-docstring
    __doc__ = _PresentationString.__doc__

    _py_template_params_to_cxx_type = {
        (list[int],): _PresentationWord,
        (str,): _PresentationString,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    def __eq__(self: _Self, other: _Self):
        return _to_cxx(self) == _to_cxx(other)

    def __init__(self: _Self, *args, **kwargs) -> None:
        """__init__(*args, **kwargs)
        Overloaded function.

        1. __init__(self: Presentation, word: type) -> None

        Default constructor.

        This function default constructs an uninitialised :any:`Presentation`
        instance.

        :Keyword Arguments:
            * **word** (*type*) -- the type of words to use. Must be either
              ``str`` or ``list[int]``.

        2. __init__(self: Presentation, alphabet: str | list[int]) -> None

        Construct a presentation from an alphabet.

        This function constructs a :any:`Presentation` instance with the
        alphabet specified by *alphabet*.

        :param alphabet: the alphabet of the presentation.
        :type alphabet: str | list[int]

        :raises LibsemigroupsError: if there are duplicate letters in
            *alphabet*.
        """
        # super().__init__ checks if there are unexpected kwargs,
        # and sets _cxx_obj if the unique argument is a cxx_obj of type in _all_wrapped_cxx_types
        super().__init__(*args, optional_kwargs="word")
        if _to_cxx(self) is not None:
            return

        if (
            (len(args) == 0 and "word" not in kwargs)
            or (len(args) == 1 and len(kwargs) > 0)
            or len(args) > 1
        ):
            raise TypeError(
                'expected 1 positional argument or the keyword argument "word"'
                f" but found {len(args)} positional arguments, and keywords arguments "
                f"{tuple(kwargs.keys())}"
            )
        if len(args) == 0:
            self.py_template_params = (kwargs["word"],)
        if len(args) == 1:
            if not (
                isinstance(args[0], (str, list))
                or (isinstance(self, InversePresentation) and isinstance(self, Presentation))
            ):
                extra = ""
                if isinstance(self, InversePresentation):
                    extra = ", Presentation"
                raise TypeError(
                    f"expected the argument to have type one of (str, list{extra}) "
                    f"but found {type(args[0])}"
                )
            if isinstance(args[0], list) and not all(isinstance(x, int) for x in args[0]):
                raise TypeError("expected the argument to consist of int values")
            if isinstance(args[0], str):
                self.py_template_params = (str,)
            if isinstance(args[0], list):
                self.py_template_params = (list[int],)

        if len(args) == 0 or not isinstance(args[0], Presentation):
            assert self.py_template_params is not None
            # For InversePresentation __init__
            self.init_cxx_obj()
            if len(args) == 1:
                self.alphabet(args[0])

    @_copydoc(_PresentationWord.rules)
    @property
    def rules(self: _Self) -> list[_Union[list[int], str]]:
        # pylint: disable=missing-function-docstring
        return _to_cxx(self).rules

    @rules.setter
    def rules(self: _Self, val: list[_Union[list[int], str]]) -> None:
        _to_cxx(self).rules = val


_copy_cxx_mem_fns(_PresentationWord, Presentation)
_register_cxx_wrapped_type(_PresentationWord, Presentation)
_register_cxx_wrapped_type(_PresentationString, Presentation)

########################################################################
# InversePresentation
########################################################################


class InversePresentation(Presentation):
    # pylint: disable=missing-class-docstring
    __doc__ = _InversePresentationString.__doc__

    _py_template_params_to_cxx_type = {
        (list[int],): _InversePresentationWord,
        (str,): _InversePresentationString,
        (Presentation,): _Union[_InversePresentationWord, _InversePresentationString],
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    @_copydoc(Presentation.__init__)
    def __init__(self: _Self, *args, **kwargs) -> None:
        """__init__(self: InversePresentation, p: Presentation) -> None

        Construct an :any:`InversePresentation`, initially with empty inverses,
        from a :any:`Presentation`.

        :param p: the :any:`Presentation` to construct from.
        :type p: Presentation
        """
        super().__init__(*args, *kwargs)
        if _to_cxx(self) is not None:
            return
        assert isinstance(args[0], Presentation)

        self.py_template_params = args[0].py_template_params
        self.init_cxx_obj()
        self.alphabet(args[0].alphabet())
        self.rules = args[0].rules


_copy_cxx_mem_fns(_InversePresentationWord, InversePresentation)
_register_cxx_wrapped_type(_InversePresentationWord, InversePresentation)
_register_cxx_wrapped_type(_InversePresentationString, InversePresentation)

########################################################################
# Helper functions
########################################################################


add_identity_rules = _wrap_cxx_free_fn(_add_identity_rules)
add_inverse_rules = _wrap_cxx_free_fn(_add_inverse_rules)
add_rule = _wrap_cxx_free_fn(_add_rule)
add_rules = _wrap_cxx_free_fn(_add_rules)
add_zero_rules = _wrap_cxx_free_fn(_add_zero_rules)
are_rules_sorted = _wrap_cxx_free_fn(_are_rules_sorted)
change_alphabet = _wrap_cxx_free_fn(_change_alphabet)
contains_rule = _wrap_cxx_free_fn(_contains_rule)
first_unused_letter = _wrap_cxx_free_fn(_first_unused_letter)
greedy_reduce_length = _wrap_cxx_free_fn(_greedy_reduce_length)
greedy_reduce_length_and_number_of_gens = _wrap_cxx_free_fn(_greedy_reduce_length_and_num_of_gens)
is_strongly_compressible = _wrap_cxx_free_fn(_is_strongly_compressible)
length = _wrap_cxx_free_fn(_length)
longest_rule = _wrap_cxx_free_fn(_longest_rule)
longest_rule_length = _wrap_cxx_free_fn(_longest_rule_length)
longest_subword_reducing_length = _wrap_cxx_free_fn(_longest_subword_reducing_length)
make_semigroup = _wrap_cxx_free_fn(_make_semigroup)
normalize_alphabet = _wrap_cxx_free_fn(_normalize_alphabet)
reduce_complements = _wrap_cxx_free_fn(_reduce_complements)
reduce_to_2_generators = _wrap_cxx_free_fn(_reduce_to_2_generators)
remove_duplicate_rules = _wrap_cxx_free_fn(_remove_duplicate_rules)
remove_redundant_generators = _wrap_cxx_free_fn(_remove_redundant_generators)
remove_trivial_rules = _wrap_cxx_free_fn(_remove_trivial_rules)
replace_subword = _wrap_cxx_free_fn(_replace_subword)
replace_word = _wrap_cxx_free_fn(_replace_word)
replace_word_with_new_generator = _wrap_cxx_free_fn(_replace_word_with_new_generator)
reverse = _wrap_cxx_free_fn(_reverse)
shortest_rule = _wrap_cxx_free_fn(_shortest_rule)
shortest_rule_length = _wrap_cxx_free_fn(_shortest_rule_length)
sort_each_rule = _wrap_cxx_free_fn(_sort_each_rule)
sort_rules = _wrap_cxx_free_fn(_sort_rules)
strongly_compress = _wrap_cxx_free_fn(_strongly_compress)
throw_if_bad_inverses = _wrap_cxx_free_fn(_throw_if_bad_inverses)
to_gap_string = _wrap_cxx_free_fn(_to_gap_string)
