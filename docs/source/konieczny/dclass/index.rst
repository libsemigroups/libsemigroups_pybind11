.. Copyright (c) 2022, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.


Konieczny.DClass
=================

.. py:class:: Konieczny.DClass


   The nested abstract class :py:obj:`Konieczny.DClass` represents a
   :math:`\mathscr{D}`-class of a finite semigroup via a frame as computed in
   Konieczny’s algorithm. See :py:obj:`Konieczny` for more details.

   :py:obj:`Konieczny.DClass` instances cannot be directly constructed; instead
   you should obtain a :math:`\mathscr{D}`-class by calling
   :py:meth:`Konieczny.D_class_of_element`.

.. toctree::
   :hidden:

   methods.rst

Methods
-------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:meth:`Konieczny.DClass.contains`
     - Test membership of an element.

   * - :py:meth:`Konieczny.DClass.is_regular_D_class`
     - Test regularity of a :math:`\mathscr{D}`-class.

   * - :py:meth:`Konieczny.DClass.number_of_L_classes`
     - Returns the number of :math:`\mathscr{L}`-classes.

   * - :py:meth:`Konieczny.DClass.number_of_R_classes`
     - Returns the number of :math:`\mathscr{R}`-classes.

   * - :py:meth:`Konieczny.DClass.number_of_idempotents`
     - Returns the number of idempotents.

   * - :py:meth:`Konieczny.DClass.rep`
     - Returns a representative of the :math:`\mathscr{D}`-class.

   * - :py:meth:`Konieczny.DClass.size`
     - Returns the size of a :math:`\mathscr{D}`-class.

   * - :py:meth:`Konieczny.DClass.size_H_class`
     - Returns the size of the :math:`\mathscr{H}`-classes.
