# -*- coding: utf-8 -*-

# Copyright (c) 2025 Joseph Edwards
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, missing-module-docstring

from _libsemigroups_pybind11 import to_froidure_pin, to_todd_coxeter
from .froidure_pin import FroidurePin
from .todd_coxeter import ToddCoxeter


def to(*args, Return):
    """Convert from one type of `libsemigroups` object to another

    This function converts the the arguments specified in *args* to object of
    type *Return*.

    :param Return: the type of object to convert to.

    :returns: an object of type *Return*.

    .. doctest::

        >>> from libsemigroups_pybind11 import (
        ...     congruence_kind,
        ...     FroidurePin,
        ...     KnuthBendix,
        ...     Presentation,
        ...     presentation,
        ...     to,
        ... )

        >>> p = Presentation([0, 1])
        >>> presentation.add_rule(p, [0, 1], [1, 0])
        >>> presentation.add_rule(p, [0, 0], [0])
        >>> presentation.add_rule(p, [1, 1], [1])
        >>> kb = KnuthBendix(congruence_kind.twosided, p)

        >>> fp = to(kb, Return=FroidurePin)
        >>> fp
        <partially enumerated FroidurePin with 2 generators, 2 elements, Cayley graph ⌀ 1, & 0 rules>

        >>> fp.run()
        >>> fp
        <fully enumerated FroidurePin with 2 generators, 3 elements, Cayley graph ⌀ 2, & 3 rules>

    """
    if return_type is FroidurePin:
        return to_froidure_pin(*args)
    elif return_type is ToddCoxeter:
        return to_todd_coxeter(*args)

    raise TypeError(
        """expected the 2nd positional argument to be one of:
            * FroidurePin
            * ToddCoxeter
        """
        f"but found {Return}"
    )
