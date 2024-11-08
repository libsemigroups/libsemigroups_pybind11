.. Copyright (c) 2024, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Transf1, PPerm1, and Perm1 helpers
=======================================================

This page contains the documentation for various helper functions for
manipulating transformations, partial permutations, and permutations. 

Full API
--------

.. py:function:: one(f: Transf1 | PPerm1 | Perm1) -> Transf1 | PPerm1 | Perm1

   Returns the identity on the same number of points as the degree of *f*.
   This function returns a newly constructed object of the same type as *f*
   that fixes every value from ``0`` to ``f.degree()``.
   
   :param f: the transformation.
   :type f: Transf1 | PPerm1 | Perm1
   
   :complexity: Linear in the degree of *f*.
   
   :returns: The identity.
   :rtype: Transf1 | PPerm1 | Perm1 

.. py:function:: inverse(f: PPerm1 | Perm1) -> PPerm1 | Perm1
  
   Returns the inverse of a partial perm or permutation.
  
   This function returns a newly constructed inverse of *f*. The *inverse* of
   a partial permutation ``f`` is the partial perm ``g`` such that ``fgf =
   f`` and ``gfg = g``.
  
   :param f: the partial perm.
   :type f: PPerm1 | Perm1
   
   :complexity: Linear in the degree of *f*.
   
   :returns: The inverse of *f*.
   :rtype: PPerm1 | Perm1 

.. py:function:: domain(f: Transf1 | PPerm1 | Perm1) -> List[int]
  
   Returns the set of points where a partial transformation is defined.
   
   Returns a ``List[int]`` containing those values ``i`` such that:

   * :math:`i\in \{0, \ldots, n - 1\}` where `n` is the degree of *f*; and
   * ``f[i] != UNDEFINED``.
   
   :param f: the transformation.
   :type f: Transf1 | PPerm1 | Perm1
   
   :complexity: Linear in the degree of *f*.
   
   :returns: The sorted list of points in the domain.
   :rtype: List[int] 
   
.. py:function:: image(f: Transf1 | PPerm1 | Perm1) -> List[int]

   Returns the set of points in the image of a partial transformation.
   
   Returns a ``List[int]`` containing those values ``f[i]`` such that:

   * :math:`i\in \{0, \ldots, n - 1\}` where `n` is the degree of **f**; and
   * ``f[i] != UNDEFINED``.
   
   :param f: the transformation.
   :type f: Transf1 | PPerm1 | Perm1
   
   :complexity: Linear in the degree of **f**.
   
   :returns: The sorted list of points in the image.
   :rtype: List[int] 

.. py:function:: right_one(f: PPerm1) -> PPerm1
  
   Returns the right one of a partial perm.
  
   This function returns a newly constructed partial perm with degree equal
   to that of **f** that fixes every value in the image of **f**, and is 
   :any:`UNDEFINED` on any other values.
   
   :param f: the partial perm.
   :type f: PPerm1

   :returns: The right one of ``f``.
   :rtype: PPerm1

   :complexity: Linear in the degree of **f**.

.. py:function:: left_one(f: PPerm1) -> PPerm1

   Returns the left one of a partial perm.
  
   This function returns a newly constructed partial perm with degree equal
   to that of **f** that fixes every value in the domain of **f**, and is 
   :any:`UNDEFINED` on any other values.
   
   :param f: the partial perm.
   :type f: PPerm1

   :returns: The left one of ``f``.
   :rtype: PPerm1

   :complexity: Linear in the degree of **f**.
