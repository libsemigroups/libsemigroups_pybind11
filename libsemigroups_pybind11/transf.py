# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name
# pylint: disable=bad-option-value, consider-using-f-string, duplicate-code

"""
This package provides a the user-facing python part of libsemigroups_pybind11
relating to transformations.
"""
from typing import List, Union
from _libsemigroups_pybind11 import (
    StaticTransf16,
    Transf1,
    Transf2,
    Transf4,
    StaticPPerm16,
    PPerm1,
    PPerm2,
    PPerm4,
    StaticPerm16,
    Perm1,
    Perm2,
    Perm4,
)


def Transf(
    images: List[int],
) -> Union[StaticTransf16, Transf1, Transf2, Transf4]:
    """
    Constructs a transformation from a list of images, as follows: the
    image of the point ``i`` under the transformation is ``imgs[i]``.

    The return type is:

    * ``StaticTransf16`` if **imgs** has length in the range ``[0, 16)``;
    * ``Transf1`` if **imgs** has length in the range ``[16, 256)``
    * ``Transf2`` if **imgs** has length in the range ``[256, 65536)``
    * ``Transf4`` if **imgs** has length in the range ``[65536, 4294967296)``

    :param imgs: the list of images
    :type imgs: List[int]

    :raises LibsemigroupsError:
      if any of the size of **imgs** is not compatible with the type.
    :raises LibsemigroupsError: if any value in **imgs** exceeds ``len(imgs)``.

    :returns: A transformation.
    :rtype:
      :py:class:`StaticTransf16`, :py:class:`Transf1`, :py:class:`Transf2`,
      or :py:class:`Transf4`

    :complexity: Linear in :py:meth:`degree`.
    """
    if len(images) <= 16:
        images += range(len(images), 16)
        return StaticTransf16(images)
    if len(images) <= 2**8:
        return Transf1(images)
    if len(images) <= 2**16:
        return Transf2(images)
    if len(images) <= 2**32:
        return Transf4(images)
    raise ValueError(
        f"the argument (a list) is must have length at most {2**32}, found {len(images)}"
    )


def PPerm(
    dom: List[int], img: List[int], deg: int
) -> Union[StaticPPerm16, PPerm1, PPerm2, PPerm4]:
    """
    Constructs a partial perm ``f`` from domain, image, and degree: so that
    ``f[dom[i]] == ran[i]`` for every ``i``; and ``f`` is :any:`UNDEFINED`
    for every value not in **dom**.

    The return type is:

    * ``StaticPPerm16`` if **deg** is in the range ``[0, 16)``;
    * ``PPerm1`` if **deg** is in the range ``[16, 256)``
    * ``PPerm2`` if **deg** is in the range ``[256, 65536)``
    * ``PPerm4`` if **deg** is in the range ``[65536, 4294967296)``

    :param dom: the domain
    :type dom: List[int]
    :param ran: the image
    :type ran: List[int]
    :param M: the degree
    :type M: int

    :raises LibsemigroupsError:
      the value **M** is not compatible with the type.
    :raises LibsemigroupsError:
      **dom** and **ran** do not have the same size.
    :raises LibsemigroupsError:
      any value in **dom** or **ran** is greater than **M**.
    :raises LibsemigroupsError:
      there are repeated entries in **dom** or **ran**.

    :returns: A partial perm.
    :rtype:
      :py:class:`StaticPPerm16`, :py:class:`PPerm1`, :py:class:`PPerm2`,
      or :py:class:`PPerm4`
    """
    if deg <= 16:
        return StaticPPerm16(dom, img, 16)
    if deg <= 2**8:
        return PPerm1(dom, img, deg)
    if deg <= 2**16:
        return PPerm2(dom, img, deg)
    if deg <= 2**32:
        return PPerm4(dom, img, deg)
    raise ValueError(
        "the argument (a list) is must have length at most {2**32}, found {deg}"
    )


def Perm(images: List[int]) -> Union[StaticPerm16, Perm1, Perm2, Perm4]:
    """
    Constructs a permutation from a list of images, as follows: the
    image of the point ``i`` under the permutation is ``imgs[i]``.

    The return type is:

    * ``StaticPerm16`` if **imgs** has length in the range ``[0, 16)``;
    * ``Perm1`` if **imgs** has length in the range ``[16, 256)``
    * ``Perm2`` if **imgs** has length in the range ``[256, 65536)``
    * ``Perm4`` if **imgs** has length in the range ``[65536, 4294967296)``

    :param imgs: the list of images
    :type imgs: List[int]

    :raises LibsemigroupsError:
      if any of the size of **imgs** is not compatible with the type.
    :raises LibsemigroupsError: if any value in **imgs** exceeds ``len(imgs)``.
    :raises LibsemigroupsError: if there are repeated values in **imgs**.

    :returns: A permutation.
    :rtype:
      :py:class:`StaticPerm16`, :py:class:`Perm1`, :py:class:`Perm2`,
      or :py:class:`Perm4`

    :complexity: Linear in :py:meth:`degree`.
    """
    if len(images) <= 16:
        images += range(len(images), 16)
        return StaticPerm16(images)
    if len(images) <= 2**8:
        return Perm1(images)
    if len(images) <= 2**16:
        return Perm2(images)
    if len(images) <= 2**32:
        return Perm4(images)
    raise ValueError(
        "the argument (a list) is must have length at most {2**32}, found {deg}"
    )
