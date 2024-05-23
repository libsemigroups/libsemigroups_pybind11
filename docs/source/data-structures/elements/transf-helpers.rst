.. Copyright (c) 2024, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Transf, PPerm, and Perm helpers
===============================

This page contains the documentation for various helper functions for
manipulating transformations, partial permutations, and permutations. 

In the following documentation we write ``Transf`` to mean any of:

* ``StaticTransf16``
* ``Transf1``
* ``Transf2``
* ``Transf4``

and similarly for ``PPerm`` and ``Perm``.

Full API
--------

.. py:function:: one(f: Transf | PPerm | Perm) -> Transf | PPerm | Perm

   Returns the identity on the same number of points as the degree of *f*.
   This function returns a newly constructed object of the same type as *f*
   that fixes every value from ``0`` to ``f.degree()``.
   
   :param f: the transformation.
   :type f: Transf | PPerm | Perm
   
   :complexity: Linear in the degree of *f*.
   
   :returns: The identity.
   :rtype: Transf | PPerm | Perm 

.. py:function:: inverse(f: PPerm | Perm) -> PPerm | Perm
  
   Returns the inverse of a partial perm or permutation.
  
   This function returns a newly constructed inverse of *f*. The *inverse* of
   a partial permutation ``f`` is the partial perm ``g`` such that ``fgf =
   f`` and ``gfg = g``.
  
   :param f: the partial perm.
   :type f: PPerm | Perm
   
   :complexity: Linear in the degree of *f*.
   
   :returns: The inverse of *f*.
   :rtype: PPerm | Perm 

.. py:function:: domain(f: Transf | PPerm | Perm) -> List[int]
  
   Returns the set of points where a partial transformation is defined.
   
   Returns a ``List[int]`` containing those values ``i`` such that:

   * :math:`i\in \{0, \ldots, n - 1\}` where `n` is the degree of *f*; and
   * ``f[i] != UNDEFINED``.
   
   :param f: the transformation.
   :type f: Transf | PPerm | Perm
   
   :complexity: Linear in the degree of *f*.
   
   :returns: The sorted list of points in the domain.
   :rtype: List[int] 
   
.. py:function:: image(f: Transf | PPerm | Perm) -> List[int]

   Returns the set of points in the image of a partial transformation.
   
   Returns a ``List[int]`` containing those values ``f[i]`` such that:

   * :math:`i\in \{0, \ldots, n - 1\}` where `n` is the degree of **f**; and
   * ``f[i] != UNDEFINED``.
   
   :param f: the transformation.
   :type f: Transf | PPerm | Perm
   
   :complexity: Linear in the degree of **f**.
   
   :returns: The sorted list of points in the image.
   :rtype: List[int] 

.. py:function:: right_one(f: PPerm) -> PPerm
  
   Returns the right one of a partial perm.
  
   This function returns a newly constructed partial perm with degree equal
   to that of **f** that fixes every value in the image of **f**, and is 
   :any:`UNDEFINED` on any other values.
   
   :param f: the partial perm.
   :type f: PPerm

   :returns: The right one of ``f``.
   :rtype: PPerm

   :complexity: Linear in the degree of **f**.

.. py:function:: left_one(f: PPerm) -> PPerm

   Returns the left one of a partial perm.
  
   This function returns a newly constructed partial perm with degree equal
   to that of **f** that fixes every value in the domain of **f**, and is 
   :any:`UNDEFINED` on any other values.
   
   :param f: the partial perm.
   :type f: PPerm

   :returns: The left one of ``f``.
   :rtype: PPerm

   :complexity: Linear in the degree of **f**.
