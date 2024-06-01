.. Copyright (c) 2024, James D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

Actions
=======

This page contains details of the ``Action`` class in
``libsemigroups_pybind11`` for finding actions of semigroups, monoids, or
groups, on sets.  The notion of an "action" in the context of
``libsemigroups_pybind11`` is analogous to the notion of an orbit of a group.

The function :any:`run <Runner.run>` finds points that can be obtained by
acting on the seeds of the action by the generators of the action until no further
points can be found, or :any:`stopped <Runner.stopped>` returns ``True``.  This
is achieved by performing a breadth first search.

In this documentation we refer to:

* ``Element`` -- the type of the elements of the underlying semigroup
* ``Point`` -- the type of the objects on which the semigroup elements act
* ``Func`` -- the function that computes the action of ``Element`` on ``Point``
* ``Side`` -- the side of the action (if it is a left or a right action).

The following helper functions are also available:

* :any:`LeftAction`
* :any:`RightAction`

.. doctest::

  >>> from libsemigroups_pybind11 import RightAction, BMat8
  >>> from libsemigroups_pybind11.bmat8 import row_space_basis
  >>> o = RightAction(Element=BMat8, Point=BMat8)
  >>> o.add_seed(row_space_basis(
  ...     BMat8(
  ...         [[1, 1, 1, 0],
  ...          [1, 1, 0, 0],
  ...          [0, 1, 0, 1],
  ...          [0, 1, 0, 0]]))
  ... ).add_generator(
  ...     BMat8([[1, 0, 0, 0],
  ...            [0, 1, 0, 0],
  ...            [0, 0, 1, 0],
  ...            [0, 0, 0, 1]])
  ... ).add_generator(
  ...     BMat8([[0, 1, 0, 0],
  ...            [1, 0, 0, 0],
  ...            [0, 0, 1, 0],
  ...            [0, 0, 0, 1]])
  ... ).add_generator(
  ...     BMat8([[0, 1, 0, 0],
  ...            [0, 0, 1, 0],
  ...            [0, 0, 0, 1],
  ...            [1, 0, 0, 0]])
  ... ).add_generator(
  ...     BMat8([[1, 0, 0, 0],
  ...            [0, 1, 0, 0],
  ...            [0, 0, 1, 0],
  ...            [1, 0, 0, 1]])
  ... ).add_generator(
  ...     BMat8([[1, 0, 0, 0],
  ...            [0, 1, 0, 0],
  ...            [0, 0, 1, 0],
  ...            [0, 0, 0, 0]])
  ... )
  <incomplete action with 5 generators, 1 points>
  >>> len(o)
  553

Contents
--------

.. autosummary::
    :nosignatures:
    
    ~RightActionPPerm16List
    RightActionPPerm16List.add_generator
    RightActionPPerm16List.add_seed
    RightActionPPerm16List.cache_scc_multipliers
    RightActionPPerm16List.current_size
    RightActionPPerm16List.empty
    RightActionPPerm16List.init
    RightActionPPerm16List.iterator
    RightActionPPerm16List.multiplier_from_scc_root
    RightActionPPerm16List.multiplier_to_scc_root
    RightActionPPerm16List.position
    RightActionPPerm16List.reserve
    RightActionPPerm16List.root_of_scc
    RightActionPPerm16List.scc
    RightActionPPerm16List.size
    RightActionPPerm16List.word_graph

Full API
--------

.. currentmodule:: libsemigroups_pybind11

.. autofunction:: RightAction

.. autofunction:: LeftAction

.. currentmodule:: _libsemigroups_pybind11

.. autoclass:: RightActionPPerm16List
    :members:
    :show-inheritance:
    :class-doc-from: class
