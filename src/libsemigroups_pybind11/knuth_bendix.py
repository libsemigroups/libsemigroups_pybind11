# Copyright (c) 2024, J. D. Mitchell, Joseph Edwards
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""This page contains the documentation for various helper functions for
manipulating :any:`KnuthBendix` objects. All such functions
are contained in the submodule ``knuth_bendix``.
"""

from warnings import warn

from _libsemigroups_pybind11 import (
    KnuthBendixStringLenLexSet as _KnuthBendixStringLenLexSet,
    KnuthBendixStringLenLexTrie as _KnuthBendixStringLenLexTrie,
    KnuthBendixStringRevRPOSet as _KnuthBendixStringRevRPOSet,
    KnuthBendixStringRevRPOTrie as _KnuthBendixStringRevRPOTrie,
    KnuthBendixWordLenLexSet as _KnuthBendixWordLenLexSet,
    KnuthBendixWordLenLexTrie as _KnuthBendixWordLenLexTrie,
    KnuthBendixWordRevRPOSet as _KnuthBendixWordRevRPOSet,
    KnuthBendixWordRevRPOTrie as _KnuthBendixWordRevRPOTrie,
    Order as _Order,
    knuth_bendix_by_overlap_length as _knuth_bendix_by_overlap_length,
    knuth_bendix_is_reduced as _knuth_bendix_is_reduced,
    knuth_bendix_non_trivial_classes as _knuth_bendix_non_trivial_classes,
    knuth_bendix_normal_forms as _knuth_bendix_normal_forms,
    knuth_bendix_partition as _knuth_bendix_partition,
    knuth_bendix_redundant_rule as _knuth_bendix_redundant_rule,
)

from .detail.congruence_common import CongruenceCommon as _CongruenceCommon
from .detail.cxx_wrapper import (
    copy_cxx_mem_fns as _copy_cxx_mem_fns,
    register_cxx_wrapped_type as _register_cxx_wrapped_type,
    to_cxx as _to_cxx,
    wrap_cxx_free_fn as _wrap_cxx_free_fn,
)
from .detail.decorators import copydoc as _copydoc
from .presentation import Presentation as _Presentation

########################################################################
# The KnuthBendix class
########################################################################


class KnuthBendix(_CongruenceCommon):
    __doc__ = _KnuthBendixStringLenLexTrie.__doc__

    _py_template_params_to_cxx_type = {
        (list[int],): _KnuthBendixWordLenLexTrie,
        (str,): _KnuthBendixStringLenLexTrie,
        (list[int], "Trie", _Order.lenlex): _KnuthBendixWordLenLexTrie,
        (str, "Trie", _Order.lenlex): _KnuthBendixStringLenLexTrie,
        (list[int], "Set", _Order.lenlex): _KnuthBendixWordLenLexSet,
        (str, "Set", _Order.lenlex): _KnuthBendixStringLenLexSet,
        (list[int], "Trie", _Order.rpo): _KnuthBendixWordRevRPOTrie,
        (str, "Trie", _Order.rpo): _KnuthBendixStringRevRPOTrie,
        (list[int], "Set", _Order.rpo): _KnuthBendixWordRevRPOSet,
        (str, "Set", _Order.rpo): _KnuthBendixStringRevRPOSet,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
            strict=True,
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    options = _KnuthBendixStringLenLexTrie.options

    @_copydoc(_KnuthBendixStringLenLexTrie.__init__)
    def __init__(self, *args, rewriting_system="Trie", order=_Order.lenlex, **kwargs) -> None:
        if "rewriter" in kwargs:
            # TODO(v2) remove this entire if-statement
            warn(
                'The keyword argument "rewriter" is deprecated, please use a combination '
                'of "rewriting_system" and "order" instead. If you also specified the '
                'keyword argument "rewriting_system" or "order", then the value(s) you '
                "specified will be ignored!",
                DeprecationWarning,
                2,
            )
            if kwargs["rewriter"] not in ("RewriteFromLeft", "RewriteTrie"):
                rewriter = kwargs["rewriter"]
                raise TypeError(
                    'expected the keyword argument "rewriter" to be '
                    f'"RewriteFromLeft" or "RewriteTrie", but found "{rewriter}"'
                )
            if kwargs["rewriter"] == "RewriteFromLeft":
                rewriting_system = "Set"
                order = _Order.lenlex
            else:
                rewriting_system = "Trie"
                order = _Order.lenlex

        msg = f"""expected either:
1) 2 positional arguments of types congruence_kind and Presentation; or
2) 0 positional arguments and the keyword argument "word"
   (and possibly the keyword arguments "rewriting_system" and "order").
Found {len(args)} positional arguments and keyword arguments
{list(kwargs.keys())}!"""

        super().__init__(*args, wrong_num_args_msg=msg, **kwargs)
        if _to_cxx(self) is not None:
            return
        # args + kwargs are o/w checked by super().__init__ above
        if len(args) == 2:
            if isinstance(args[1], _Presentation):
                self.py_template_params = args[1].py_template_params + (rewriting_system, order)
            else:
                raise TypeError(
                    f"expected the 2nd argument to be a Presentation, but found {type(args[1])}"
                )
        self.init_cxx_obj(*args)


########################################################################
# Copy mem fns from sample C++ type and register types
########################################################################

_copy_cxx_mem_fns(_KnuthBendixStringLenLexTrie, KnuthBendix)
_register_cxx_wrapped_type(_KnuthBendixStringLenLexTrie, KnuthBendix)
_register_cxx_wrapped_type(_KnuthBendixWordLenLexTrie, KnuthBendix)
_register_cxx_wrapped_type(_KnuthBendixStringLenLexSet, KnuthBendix)
_register_cxx_wrapped_type(_KnuthBendixWordLenLexSet, KnuthBendix)

########################################################################
# Helpers
########################################################################

by_overlap_length = _wrap_cxx_free_fn(_knuth_bendix_by_overlap_length)
is_reduced = _wrap_cxx_free_fn(_knuth_bendix_is_reduced)
non_trivial_classes = _wrap_cxx_free_fn(_knuth_bendix_non_trivial_classes)
normal_forms = _wrap_cxx_free_fn(_knuth_bendix_normal_forms)
partition = _wrap_cxx_free_fn(_knuth_bendix_partition)
redundant_rule = _wrap_cxx_free_fn(_knuth_bendix_redundant_rule)

__all__ = [
    "KnuthBendix",
    "by_overlap_length",
    "is_reduced",
    "non_trivial_classes",
    "normal_forms",
    "partition",
    "redundant_rule",
]
