.. Copyright (c) 2024, James D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

Actions
=======

This page contains details of the ``RowActionBMat8`` class in
``libsemigroups_pybind11`` for finding actions of semigroups, or groups, on sets.  The
notion of an "action" in the context of ``libsemigroups_pybind11`` is analogous to the
notion of an orbit of a group.

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
  >>> o = RightAction(Element=BMat8, Point=BMat8)
  >>> o.add_seed(
  ...     BMat8(
  ...         [[1, 1, 1, 0],
  ...          [1, 1, 0, 0],
  ...          [0, 1, 0, 1],
  ...          [0, 1, 0, 0]]).row_space_basis()
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
  <incomplete right action with 5 generators, 1 points>
  >>> o.size()
  553

Contents
--------

.. autosummary::
    :nosignatures:
    
    ~RowActionBMat8
    RowActionBMat8.__getitem__
    RowActionBMat8.add_generator
    RowActionBMat8.add_seed
    RowActionBMat8.cache_scc_multipliers
    RowActionBMat8.current_size
    RowActionBMat8.empty
    RowActionBMat8.init
    RowActionBMat8.iterator
    RowActionBMat8.multiplier_from_scc_root
    RowActionBMat8.multiplier_to_scc_root
    RowActionBMat8.position
    RowActionBMat8.reserve
    RowActionBMat8.root_of_scc
    RowActionBMat8.scc
    RowActionBMat8.size
    RowActionBMat8.word_graph

Full API
--------

.. currentmodule:: libsemigroups_pybind11

.. autofunction:: RightAction

.. autofunction:: LeftAction

.. currentmodule:: _libsemigroups_pybind11

.. autoclass:: RowActionBMat8
    :members:
    :show-inheritance:
    :class-doc-from: class

    .. autofunction:: libsemigroups_pybind11.action.Action

