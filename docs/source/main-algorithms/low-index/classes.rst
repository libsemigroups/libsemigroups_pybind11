.. Copyright (c) 2024 R. Cirpons

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

Classes for low-index congruences
=================================

On this page we describe the functionality relating to the Sims's
low-index congruence algorithm for semigroups and monoids that is available in
``libsemigroups_pybind11``. 

.. doctest::

   >>> from libsemigroups_pybind11 import Sims1, Presentation, presentation
   >>> p = Presentation([0, 1])
   >>> presentation.add_rule(p, [0, 1], [1, 0])
   >>> sims = Sims1(p)
   >>> sims.number_of_congruences(4)
   117

Contents
--------

.. autosummary::
   :nosignatures:

   ~MinimalRepOrc
   ~RepOrc
   ~Sims1
   ~Sims2
   ~SimsRefinerFaithful
   ~SimsRefinerIdeals
   ~SimsSettingsSims1
   ~SimsStats

Full API
--------

.. autoclass:: MinimalRepOrc
   :members:
   :inherited-members:

.. autoclass:: RepOrc
   :members:
   :inherited-members:

.. autoclass:: Sims1
   :members:
   :inherited-members:

.. autoclass:: Sims2
   :members:
   :inherited-members:

.. autoclass:: SimsRefinerFaithful
   :members:
   :inherited-members:
   :special-members: __call__

.. autoclass:: SimsRefinerIdeals
   :members:
   :inherited-members:
   :special-members: __call__

.. autoclass:: SimsSettingsSims1
   :members:
   :inherited-members:

.. autoclass:: SimsStats
   :members:
   :inherited-members:
