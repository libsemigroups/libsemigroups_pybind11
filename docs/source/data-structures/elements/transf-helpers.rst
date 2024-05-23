.. Copyright (c) 2024, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

StaticTransf16, StaticPPerm16, and StaticPerm16 helpers
=======================================================

This page contains the documentation for various helper functions for
manipulating transformations, partial permutations, and permutations. 

Full API
--------

.. py:function:: one(f: StaticTransf16 | StaticPPerm16 | StaticPerm16) -> StaticTransf16 | StaticPPerm16 | StaticPerm16

   Returns the identity on the same number of points as the degree of *f*.
   This function returns a newly constructed object of the same type as *f*
   that fixes every value from ``0`` to ``f.degree()``.
   
   :param f: the transformation.
   :type f: StaticTransf16 | StaticPPerm16 | StaticPerm16
   
   :complexity: Linear in the degree of *f*.
   
   :returns: The identity.
   :rtype: StaticTransf16 | StaticPPerm16 | StaticPerm16 

.. py:function:: inverse(f: StaticPPerm16 | StaticPerm16) -> StaticPPerm16 | StaticPerm16
  
   Returns the inverse of a partial perm or permutation.
  
   This function returns a newly constructed inverse of *f*. The *inverse* of
   a partial permutation ``f`` is the partial perm ``g`` such that ``fgf =
   f`` and ``gfg = g``.
  
   :param f: the partial perm.
   :type f: StaticPPerm16 | StaticPerm16
   
   :complexity: Linear in the degree of *f*.
   
   :returns: The inverse of *f*.
   :rtype: StaticPPerm16 | StaticPerm16 

.. py:function:: domain(f: StaticTransf16 | StaticPPerm16 | StaticPerm16) -> List[int]
  
   Returns the set of points where a partial transformation is defined.
   
   Returns a ``List[int]`` containing those values ``i`` such that:

   * :math:`i\in \{0, \ldots, n - 1\}` where `n` is the degree of *f*; and
   * ``f[i] != UNDEFINED``.
   
   :param f: the transformation.
   :type f: StaticTransf16 | StaticPPerm16 | StaticPerm16
   
   :complexity: Linear in the degree of *f*.
   
   :returns: The sorted list of points in the domain.
   :rtype: List[int] 
   
.. py:function:: image(f: StaticTransf16 | StaticPPerm16 | StaticPerm16) -> List[int]

   Returns the set of points in the image of a partial transformation.
   
   Returns a ``List[int]`` containing those values ``f[i]`` such that:

   * :math:`i\in \{0, \ldots, n - 1\}` where `n` is the degree of **f**; and
   * ``f[i] != UNDEFINED``.
   
   :param f: the transformation.
   :type f: StaticTransf16 | StaticPPerm16 | StaticPerm16
   
   :complexity: Linear in the degree of **f**.
   
   :returns: The sorted list of points in the image.
   :rtype: List[int] 

.. py:function:: right_one(f: StaticPPerm16) -> StaticPPerm16
  
   Returns the right one of a partial perm.
  
   This function returns a newly constructed partial perm with degree equal
   to that of **f** that fixes every value in the image of **f**, and is 
   :any:`UNDEFINED` on any other values.
   
   :param f: the partial perm.
   :type f: StaticPPerm16

   :returns: The right one of ``f``.
   :rtype: StaticPPerm16

   :complexity: Linear in the degree of **f**.

.. py:function:: left_one(f: StaticPPerm16) -> StaticPPerm16

   Returns the left one of a partial perm.
  
   This function returns a newly constructed partial perm with degree equal
   to that of **f** that fixes every value in the domain of **f**, and is 
   :any:`UNDEFINED` on any other values.
   
   :param f: the partial perm.
   :type f: StaticPPerm16

   :returns: The left one of ``f``.
   :rtype: StaticPPerm16

   :complexity: Linear in the degree of **f**.
