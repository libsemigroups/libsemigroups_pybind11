.. Copyright (c) 2020, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

FpSemigroup
===========

This is a class for representing finitely presented semigroups and
monoids.

On this page we describe the functionality relating to the :class:`FpSemigroup`
class. This class can be used for computing a finitely presented semigroup or
monoid by running every applicable algorithm from ``libsemigroups`` (and
possibly some variants of the same algorithm) in parallel. This class is
provided for convenience, at present it is not very customisable, and lacks
some of the fine grained control offered by the classes implementing individual
algorithms, such as :class:`.ToddCoxeter` and :class:`.KnuthBendix`.

.. code-block:: python

   S = FpSemigroup()
   S.set_alphabet(3)
   S.set_identity(0)
   S.add_rule([1, 2], [0])
   S.is_obviously_infinite() # True


.. autoclass:: FpSemigroup
   :members:
