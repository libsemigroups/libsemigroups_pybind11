# -*- coding: utf-8 -*-

# Copyright (c) 2025 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# The module doc string is what appears at the top of the helper function
# doc page, and so is omitted.

# pylint: disable=no-name-in-module, missing-module-docstring
# pylint: disable=missing-function-docstring


from _libsemigroups_pybind11 import (
    is_obviously_infinite as _is_obviously_infinite,
    tril as _tril,
)

from .detail.decorators import copydoc as _copydoc
from .detail.cxx_wrapper import CxxWrapper as _CxxWrapper, to_cxx as _to_cxx


# TODO use _wrap_cxx_free_fn
@_copydoc(_is_obviously_infinite)
def is_obviously_infinite(thing: _CxxWrapper) -> _tril:
    return _is_obviously_infinite(_to_cxx(thing))
