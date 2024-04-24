.. Copyright (c) 2021, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

FroidurePin
===========

.. py:class:: FroidurePin

   This page contains the documentation for functionality in
   ``libsemigroups_pybind11`` for the Froidure-Pin algorithm.
   The class :py:class:`FroidurePin` implements the Froidure-Pin algorithm as
   described in the article `"Algorithms for computing finite semigroups"
   <https://www.irif.fr/~jep/PDF/Rio.pdf>`_
   by Veronique Froidure and Jean-Eric Pin.


   A :py:class:`FroidurePin` instance is defined by a generating set, and the main
   method is :py:meth:`run`, which implements the Froidure-Pin Algorithm.  If
   :py:meth:`run` is invoked and :py:meth:`finished` returns ``True``, then the
   size, the left and right Cayley graphs are determined, and a confluent
   terminating presentation for the semigroup is known.

   While :py:class:`FroidurePin` is not a class the objects returned by
   :py:class:`FroidurePin` have identical methods, and so we document
   :py:class:`FroidurePin` as if it was a class.

.. toctree::
   :hidden:

   init
   generators
   settings
   membership
   accessors
   attributes
   idempotents
   cayley-graphs
   factorisation
   prefix
   runner

Constructors
------------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:meth:`FroidurePin.__init__`
     - Overloaded function.

Generators
----------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:meth:`FroidurePin.add_generator`
     - Add a copy of an element to the generators.
   * - :py:meth:`FroidurePin.add_generators`
     - Add collection of generators.
   * - :py:meth:`FroidurePin.closure`
     - Add non-redundant generators in collection.
   * - :py:meth:`FroidurePin.copy_closure`
     - Copy and add non-redundant generators.
   * - :py:meth:`FroidurePin.copy_add_generators`
     - Copy and add a collection of generators.
   * - :py:meth:`FroidurePin.number_of_generators`
     - Returns the number of generators.
   * - :py:meth:`FroidurePin.generator`
     - Returns the generator with specified index.

Settings
--------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:meth:`FroidurePin.batch_size`
     - Overloaded function.
   * - :py:meth:`FroidurePin.concurrency_threshold`
     - Overloaded function.
   * - :py:meth:`FroidurePin.immutable`
     - Overloaded function.
   * - :py:meth:`FroidurePin.max_threads`
     - Overloaded function.
   * - :py:meth:`FroidurePin.reserve`
     - Requests the given capacity for elements.

Membership + comparison
-----------------------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:meth:`FroidurePin.contains`
     - Test membership of an element.
   * - :py:meth:`FroidurePin.current_position`
     - Overloaded function.
   * - :py:meth:`FroidurePin.position_to_sorted_position`
     - Returns the sorted index of an element via its index.
   * - :py:meth:`FroidurePin.position`
     - Find the position of an element with enumeration if necessary.
   * - :py:meth:`FroidurePin.sorted_position`
     - Returns the sorted index of an element.
   * - :py:meth:`FroidurePin.equal_to`
     - Check equality of words in the generators.

Accessors
---------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:meth:`FroidurePin.__iter__`
     - Returns an iterator pointing to the first element.
   * - :py:meth:`FroidurePin.at`
     - Access element specified by index with bound checks.
   * - :py:meth:`FroidurePin.current_size`
     - Returns the number of elements so far enumerated.
   * - :py:meth:`FroidurePin.size`
     - Returns the size.
   * - :py:meth:`FroidurePin.sorted_at`
     - Access element specified by sorted index with bound checks.
   * - :py:meth:`FroidurePin.sorted`
     - Returns an iterator pointing to the first element (sorted).

Attributes
----------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:meth:`FroidurePin.is_monoid`
     - Check if the semigroup is a monoid.
   * - :py:meth:`FroidurePin.is_finite`
     - Check finiteness.
   * - :py:meth:`FroidurePin.degree`
     - Returns the degree of any and all elements.

Idempotents
-----------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:meth:`FroidurePin.is_idempotent`
     - Check if an element is an idempotent via its index.
   * - :py:meth:`FroidurePin.idempotents`
     - Returns an iterator pointing at the first idempotent.
   * - :py:meth:`FroidurePin.number_of_idempotents`
     - Returns the number of idempotents.

Cayley graphs
-------------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:meth:`FroidurePin.left_cayley_graph`
     - Returns the left Cayley graph
   * - :py:meth:`FroidurePin.right_cayley_graph`
     - Returns the right Cayley graph

Factorisation, products, and relations
--------------------------------------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:meth:`FroidurePin.current_length`
     - Returns the length of the short-lex least word.
   * - :py:meth:`FroidurePin.current_max_word_length`
     - Returns the maximum length of a word in the generators so far computed.
   * - :py:meth:`FroidurePin.current_number_of_rules`
     - Returns the number of relations that have been found so far.
   * - :py:meth:`FroidurePin.factorisation`
     - Overloaded function.
   * - :py:meth:`FroidurePin.length`
     - Returns the length of the short-lex least word.
   * - :py:meth:`FroidurePin.minimal_factorisation`
     - Returns a short-lex least word representing an element given by index.
   * - :py:meth:`FroidurePin.number_of_rules`
     - Returns the total number of relations in the presentation.
   * - :py:meth:`FroidurePin.product_by_reduction`
     - Compute a product using the Cayley graph.
   * - :py:meth:`FroidurePin.fast_product`
     - Multiply elements via their indices.
   * - :py:meth:`FroidurePin.rules`
     - Returns an iterator pointing to the first rule (if any).
   * - :py:meth:`FroidurePin.word_to_element`
     - Convert a word in the generators to an element.

Prefixes and suffixes
---------------------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:meth:`FroidurePin.final_letter`
     - Returns the last letter of the element with specified index.
   * - :py:meth:`FroidurePin.first_letter`
     - Returns the first letter of the element with specified index.
   * - :py:meth:`FroidurePin.prefix`
     - Returns the index of the longest proper prefix.
   * - :py:meth:`FroidurePin.suffix`
     - Returns the position of the longest proper suffix.

Running and reporting
---------------------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:meth:`FroidurePin.enumerate`
     - Enumerate until at least a specified number of elements are found.
   * - :py:meth:`FroidurePin.run`
     - Run the algorithm until it finishes.
   * - :py:meth:`FroidurePin.run_for`
     - Run for a specified amount of time.
   * - :py:meth:`FroidurePin.run_until`
     - Run until a nullary predicate returns ``True`` or :py:meth:`finished`.
   * - :py:meth:`FroidurePin.kill`
     - Stop running the algorithm (thread-safe).
   * - :py:meth:`FroidurePin.dead`
     - Check if the runner is dead.
   * - :py:meth:`FroidurePin.finished`
     - Check if the main algorithm has been run to completion or not.
   * - :py:meth:`FroidurePin.started`
     - Check if :py:meth:`run` has been called at least once before.
   * - :py:meth:`FroidurePin.report`
     - Check if it is time to report.
   * - :py:meth:`FroidurePin.report_every`
     - Set the minimum elapsed time between reports.
   * - :py:meth:`FroidurePin.report_why_we_stopped`
     - Report why we stopped.
   * - :py:meth:`FroidurePin.running`
     - Check if currently running.
   * - :py:meth:`FroidurePin.timed_out`
     - Check if the main algorithm has or should timed out.
   * - :py:meth:`FroidurePin.stopped`
     - Check if the main algorithm has or should stop.
   * - :py:meth:`FroidurePin.stopped_by_predicate`
     - Check if the main algorithm was, or should be, stopped by the nullary
       predicate passed as first argument to :py:meth:`run_until`.
