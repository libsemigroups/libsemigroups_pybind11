# Copyright (c) 2025 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""This page contains the documentation for the HPCombi functionality exposed
in ``libsemigroups_pybind11``.
"""

from _libsemigroups_pybind11 import LIBSEMIGROUPS_HPCOMBI_ENABLED

if LIBSEMIGROUPS_HPCOMBI_ENABLED:
    from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module
        hpcombi_Perm16 as Perm16,
        hpcombi_PPerm16 as PPerm16,
        hpcombi_PTransf16 as PTransf16,
        hpcombi_Transf16 as Transf16,
        hpcombi_Vect16 as Vect16,
    )

    # The following fools sphinx into thinking that Vect16 etc are not
    # aliases.

    Perm16.__module__ = __name__
    Perm16.__name__ = "Perm16"
    PPerm16.__module__ = __name__
    PPerm16.__name__ = "PPerm16"
    PTransf16.__module__ = __name__
    PTransf16.__name__ = "PTransf16"
    Transf16.__module__ = __name__
    Transf16.__name__ = "Transf16"
    Vect16.__module__ = __name__
    Vect16.__name__ = "Vect16"
