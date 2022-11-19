.. Copyright (c) 2022, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Konieczny
=========

.. py:class:: Konieczny

   This page contains the documentation for functionality in
   ``libsemigroups_pybind11`` for Konieczny's algorithm described in the
   article `Green's equivalences in finite semigroups of binary relations`_ by
   `Janusz Konieczny`_. This algorithm is similar to that of Lallement and
   McFadden in `this`_ paper. It differs in being applicable to subsemigroups
   of a non-regular semigroup, though is essentially the same algorithm for
   elements which happen to be regular.

   A :py:class:`Konieczny` instance is defined by a generating set, and the
   main function is :py:meth:`Konieczny.run`, which implements Konieczny's
   Algorithm. If :py:meth:`Konieczny.run` is invoked and
   :py:meth:`Konieczny.finished` returns ``True``, then the size, partial
   order of :math:`\mathscr{D}`-classes, and frames for each
   :math:`\mathscr{D}`-class are known.

   While :py:class:`Konieczny` is not a class the objects returned by
   :py:class:`Konieczny` have identical methods, and so we document
   :py:class:`Konieczny` as if it was a class.

.. _Green's equivalences in finite semigroups of binary relations:
     https://link.springer.com/article/10.1007/BF02573672
.. _this: https://www.sciencedirect.com/science/article/pii/S0747717108800570
.. _Janusz Konieczny: https://www.umw.edu/directory/employee/janusz-konieczny/

.. toctree::
   :hidden:

   init
   generators
   membership
   attributes
   idempotents
   greens
   runner

Constructors
------------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:meth:`Konieczny.__init__`
     - Construct from list of generators.

Generators
----------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:meth:`Konieczny.add_generator`
     - Add a copy of an element to the generators.
   * - :py:meth:`Konieczny.number_of_generators`
     - Returns the number of generators.
   * - :py:meth:`Konieczny.generator`
     - Returns the generator with specified index.
   * - :py:meth:`Konieczny.generators`
     - Returns an iterator pointing to the first generator.

Membership
----------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:meth:`Konieczny.contains`
     - Test membership of an element.
   * - :py:meth:`Konieczny.__contains__`
     - Test membership of an element.

Attributes
----------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:meth:`Konieczny.degree`
     - Returns the degree of any and all elements.
   * - :py:meth:`Konieczny.size`
     - Returns the size.
   * - :py:meth:`Konieczny.current_size`
     - Returns the current size.

Idempotents
-----------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:meth:`Konieczny.number_of_idempotents`
     - Returns the number of idempotents.
   * - :py:meth:`Konieczny.current_number_of_idempotents`
     - Returns the current number of idempotents.

Green's relations
-----------------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:meth:`Konieczny.is_regular_element`
     - Test regularity of an element.
   * - :py:meth:`Konieczny.D_class_of_element`
     - Returns the :math:`\mathscr{D}`-class containing an element.
   * - :py:meth:`Konieczny.D_classes`
     - Returns an iterator pointing to the first :math:`\mathscr{D}`-class.
   * - :py:meth:`Konieczny.regular_D_classes`
     - Returns an iterator pointing to the first regular :math:`\mathscr{D}`-class.
   * - :py:meth:`Konieczny.number_of_D_classes`
     - Returns the number of :math:`\mathscr{D}`-classes.
   * - :py:meth:`Konieczny.number_of_L_classes`
     - Returns the number of :math:`\mathscr{L}`-classes.
   * - :py:meth:`Konieczny.number_of_R_classes`
     - Returns the number of :math:`\mathscr{R}`-classes.
   * - :py:meth:`Konieczny.number_of_H_classes`
     - Returns the number of :math:`\mathscr{H}`-classes.
   * - :py:meth:`Konieczny.current_number_of_D_classes`
     - Returns the current number of :math:`\mathscr{D}`-classes.
   * - :py:meth:`Konieczny.current_number_of_L_classes`
     - Returns the current number of :math:`\mathscr{L}`-classes.
   * - :py:meth:`Konieczny.current_number_of_R_classes`
     - Returns the current number of :math:`\mathscr{R}`-classes.
   * - :py:meth:`Konieczny.current_number_of_H_classes`
     - Returns the current number of :math:`\mathscr{H}`-classes.
   * - :py:meth:`Konieczny.number_of_regular_D_classes`
     - Returns the number of regular :math:`\mathscr{D}`-classes.
   * - :py:meth:`Konieczny.number_of_regular_L_classes`
     - Returns the number of regular :math:`\mathscr{L}`-classes.
   * - :py:meth:`Konieczny.number_of_regular_R_classes`
     - Returns the number of regular :math:`\mathscr{R}`-classes.
   * - :py:meth:`Konieczny.current_number_of_regular_D_classes`
     - Returns the number of regular :math:`\mathscr{D}`-classes.
   * - :py:meth:`Konieczny.current_number_of_regular_L_classes`
     - Returns the number of regular :math:`\mathscr{L}`-classes.
   * - :py:meth:`Konieczny.current_number_of_regular_R_classes`
     - Returns the number of regular :math:`\mathscr{R}`-classes.

Running and reporting
---------------------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:meth:`Konieczny.enumerate`
     - Enumerate until at least a specified number of elements are found.
   * - :py:meth:`Konieczny.run`
     - Run the algorithm until it finishes.
   * - :py:meth:`Konieczny.run_for`
     - Run for a specified amount of time.
   * - :py:meth:`Konieczny.run_until`
     - Run until a nullary predicate returns ``True`` or :py:meth:`finished`.
   * - :py:meth:`Konieczny.kill`
     - Stop running the algorithm (thread-safe).
   * - :py:meth:`Konieczny.dead`
     - Check if the runner is dead.
   * - :py:meth:`Konieczny.finished`
     - Check if the main algorithm has been run to completion or not.
   * - :py:meth:`Konieczny.started`
     - Check if :py:meth:`run` has been called at least once before.
   * - :py:meth:`Konieczny.report`
     - Check if it is time to report.
   * - :py:meth:`Konieczny.report_every`
     - Set the minimum elapsed time between reports.
   * - :py:meth:`Konieczny.report_why_we_stopped`
     - Report why we stopped.
   * - :py:meth:`Konieczny.running`
     - Check if currently running.
   * - :py:meth:`Konieczny.timed_out`
     - Check if the main algorithm has or should timed out.
   * - :py:meth:`Konieczny.stopped`
     - Check if the main algorithm has or should stop.
   * - :py:meth:`Konieczny.stopped_by_predicate`
     - Check if the main algorithm was, or should be, stopped by the nullary
       predicate passed as first argument to :py:meth:`run_until`.
