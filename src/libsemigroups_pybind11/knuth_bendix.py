# -*- coding: utf-8

# Copyright (c) 2024, J. D. Mitchell, Joseph Edwards
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This page contains the documentation for various helper functions for
manipulating :any:`KnuthBendix` objects. All such functions
are contained in the submodule ``knuth_bendix``.
"""

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module
    KnuthBendixStringRewriteFromLeft as _KnuthBendixStringRewriteFromLeft,
    KnuthBendixStringRewriteTrie as _KnuthBendixStringRewriteTrie,
    KnuthBendixWordRewriteFromLeft as _KnuthBendixWordRewriteFromLeft,
    KnuthBendixWordRewriteTrie as _KnuthBendixWordRewriteTrie,
    congruence_kind as _congruence_kind,
    knuth_bendix_by_overlap_length as _knuth_bendix_by_overlap_length,
    knuth_bendix_is_reduced as _knuth_bendix_is_reduced,
    knuth_bendix_non_trivial_classes as _knuth_bendix_non_trivial_classes,
    knuth_bendix_normal_forms as _knuth_bendix_normal_forms,
    knuth_bendix_partition as _knuth_bendix_partition,
    knuth_bendix_redundant_rule as _knuth_bendix_redundant_rule,
)

from .detail.decorators import (
    copydoc as _copydoc,
)

from .detail.cxx_wrapper import (
    copy_cxx_mem_fns as _copy_cxx_mem_fns,
    to_cxx as _to_cxx,
    wrap_cxx_free_fn as _wrap_cxx_free_fn,
    register_cxx_wrapped_type as _register_cxx_wrapped_type,
)

from .detail.congruence_common import CongruenceCommon as _CongruenceCommon

from .presentation import (
    Presentation as _Presentation,
)

########################################################################
# The KnuthBendix class
########################################################################


class KnuthBendix(_CongruenceCommon):  # pylint: disable=missing-class-docstring
    __doc__ = _KnuthBendixStringRewriteTrie.__doc__

    _py_template_params_to_cxx_type = {
        (list[int],): _KnuthBendixWordRewriteTrie,
        (str,): _KnuthBendixStringRewriteTrie,
        (list[int], "RewriteTrie"): _KnuthBendixWordRewriteTrie,
        (str, "RewriteTrie"): _KnuthBendixStringRewriteTrie,
        (list[int], "RewriteFromLeft"): _KnuthBendixWordRewriteFromLeft,
        (str, "RewriteFromLeft"): _KnuthBendixStringRewriteFromLeft,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    options = _KnuthBendixStringRewriteTrie.options

    @_copydoc(_KnuthBendixStringRewriteTrie.__init__)
    def __init__(self, *args, rewriter="RewriteTrie", **kwargs) -> None:
        if rewriter not in ("RewriteFromLeft", "RewriteTrie"):
            raise TypeError(
                f'expected the keyword argument "rewriter" to be '
                f'"RewriteFromLeft" or "RewriteTrie", but found "{rewriter}"'
            )

        msg = f"""expected either:
1) 2 positional arguments of types congruence_kind and Presentation; or
2) 0 positional arguments and the keyword argument "word"
   (and possibly the keyword argument "rewriter").
Found {len(args)} positional arguments and keyword arguments
{list(kwargs.keys())}!"""

        super().__init__(*args, wrong_num_args_msg=msg, **kwargs)
        if _to_cxx(self) is not None:
            return
        if len(args) == 2:
            if isinstance(args[1], _Presentation):
                self.py_template_params = args[1].py_template_params + (
                    rewriter,
                )
            else:
                raise TypeError(
                    f"expected the 2nd argument to be a Presentation, but found {type(args[1])}"
                )
        self.init_cxx_obj(*args)


########################################################################
# Copy mem fns from sample C++ type and register types
########################################################################

_copy_cxx_mem_fns(_KnuthBendixStringRewriteTrie, KnuthBendix)
_register_cxx_wrapped_type(_KnuthBendixStringRewriteTrie, KnuthBendix)
_register_cxx_wrapped_type(_KnuthBendixWordRewriteTrie, KnuthBendix)
_register_cxx_wrapped_type(_KnuthBendixStringRewriteFromLeft, KnuthBendix)
_register_cxx_wrapped_type(_KnuthBendixWordRewriteFromLeft, KnuthBendix)

########################################################################
# Helpers
########################################################################

by_overlap_length = _wrap_cxx_free_fn(_knuth_bendix_by_overlap_length)
is_reduced = _wrap_cxx_free_fn(_knuth_bendix_is_reduced)
non_trivial_classes = _wrap_cxx_free_fn(_knuth_bendix_non_trivial_classes)
normal_forms = _wrap_cxx_free_fn(_knuth_bendix_normal_forms)
partition = _wrap_cxx_free_fn(_knuth_bendix_partition)
redundant_rule = _wrap_cxx_free_fn(_knuth_bendix_redundant_rule)
