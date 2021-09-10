.. Copyright (c) 2021, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

FroidurePin
===========

The class :py:class:`FroidurePin` implements the Froidure-Pin algorithm as
described in the article `"Algorithms for computing finite semigroups"
<https://www.irif.fr/~jep/PDF/Rio.pdf>`_
by Veronique Froidure and Jean-Eric Pin.


A :py:class:`FroidurePin` instance is defined by a generating set, and the main
method is :py:meth:`run`, which implements the Froidure-Pin Algorithm.  If
:py:meth:`run` is invoked and :py:meth:`finished` returns ``True``, then the
size, the left and right Cayley graphs are determined, and a confluent
terminating presentation for the semigroup is known.

The methods of the various types of ``FroidurePin``  are identical, and
so we only include details for the :py:class:`FroidurePinTransf1` class. 

.. contents::

Types of FroidurePin available
------------------------------

For reference the complete list of ``FroidurePin`` types in
``libsemigroups_pybind11`` is:

* :py:class:`FroidurePinTransf16`
* :py:class:`FroidurePinTransf1`
* :py:class:`FroidurePinTransf2`
* :py:class:`FroidurePinTransf4`
* :py:class:`FroidurePinPPerm16`
* :py:class:`FroidurePinPPerm1`
* :py:class:`FroidurePinPPerm2`
* :py:class:`FroidurePinPPerm4`
* :py:class:`FroidurePinPerm`
* :py:class:`FroidurePinPerm1`
* :py:class:`FroidurePinPerm2`
* :py:class:`FroidurePinPerm4`
* :py:class:`FroidurePinBipartition`
* :py:class:`FroidurePinPBR`

Constructors
------------

.. autosummary:: 
   ~FroidurePinTransf1

Generators
----------

.. autosummary::
   FroidurePinTransf1.add_generator
   FroidurePinTransf1.add_generators
   FroidurePinTransf1.closure
   FroidurePinTransf1.copy_closure
   FroidurePinTransf1.copy_add_generators
   FroidurePinTransf1.number_of_generators
   FroidurePinTransf1.generator

Settings
--------

.. autosummary::
   FroidurePinTransf1.batch_size
   FroidurePinTransf1.concurrency_threshold
   FroidurePinTransf1.immutable
   FroidurePinTransf1.max_threads
   FroidurePinTransf1.reserve

Membership + comparison
-----------------------

.. autosummary::
   FroidurePinTransf1.contains
   FroidurePinTransf1.current_position
   FroidurePinTransf1.position_to_sorted_position
   FroidurePinTransf1.position
   FroidurePinTransf1.sorted_position
   FroidurePinTransf1.equal_to

Accessors
---------

.. autosummary::
   FroidurePinTransf1.__iter__
   FroidurePinTransf1.at
   FroidurePinTransf1.current_size
   FroidurePinTransf1.size
   FroidurePinTransf1.sorted_at
   FroidurePinTransf1.sorted

Attributes
----------

.. autosummary::
   FroidurePinTransf1.is_monoid
   FroidurePinTransf1.is_finite
   FroidurePinTransf1.degree

Idempotents
-----------

.. autosummary::
   FroidurePinTransf1.is_idempotent
   FroidurePinTransf1.idempotents
   FroidurePinTransf1.number_of_idempotents

Cayley graphs
-------------

.. autosummary::
   FroidurePinTransf1.left_cayley_graph
   FroidurePinTransf1.right_cayley_graph

Factorisation, products, and relations
--------------------------------------

.. autosummary::
   FroidurePinTransf1.current_length
   FroidurePinTransf1.current_max_word_length
   FroidurePinTransf1.current_number_of_rules
   FroidurePinTransf1.factorisation
   FroidurePinTransf1.length
   FroidurePinTransf1.minimal_factorisation
   FroidurePinTransf1.number_of_rules
   FroidurePinTransf1.product_by_reduction
   FroidurePinTransf1.fast_product
   FroidurePinTransf1.rules
   FroidurePinTransf1.word_to_element

Prefixes and suffixes
---------------------
   
.. autosummary::
   FroidurePinTransf1.final_letter
   FroidurePinTransf1.first_letter
   FroidurePinTransf1.prefix
   FroidurePinTransf1.suffix

Running and reporting
---------------------

.. autosummary::
   FroidurePinTransf1.enumerate
   FroidurePinTransf1.run
   FroidurePinTransf1.run_for
   FroidurePinTransf1.run_until
   FroidurePinTransf1.kill
   FroidurePinTransf1.dead
   FroidurePinTransf1.finished
   FroidurePinTransf1.started
   FroidurePinTransf1.report
   FroidurePinTransf1.report_every
   FroidurePinTransf1.report_why_we_stopped
   FroidurePinTransf1.running
   FroidurePinTransf1.timed_out
   FroidurePinTransf1.stopped
   FroidurePinTransf1.stopped_by_predicate

Full documentation
------------------

.. autoclass:: FroidurePinTransf1
   :members:
   :special-members: __iter__
