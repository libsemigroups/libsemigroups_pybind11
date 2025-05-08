# -*- coding: utf-8 -*-

# Copyright (c) 2022-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.


"""
This package provides the function _view for the Dot class, which uses the
graphviz package.
"""

from graphviz import Source as _Source

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module
    Dot as _Dot,
)


def _view(  # pylint: disable=too-many-arguments, too-many-positional-arguments
    self,
    filename=None,
    directory=None,
    fmt=None,
    engine=None,
    encoding="utf-8",
    renderer=None,
    formatter=None,
    loaded_from_path=None,
):
    s = _Source(
        str(self),
        filename=filename,
        directory=directory,
        format=fmt,
        engine=engine,
        encoding=encoding,
        renderer=renderer,
        formatter=formatter,
        loaded_from_path=loaded_from_path,
    )
    s.view()


_Dot.view = _view
