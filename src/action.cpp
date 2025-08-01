//
// libsemigroups_pybind11
// Copyright (C) 2024 James D. Mitchell
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

// TODO RightActionPPerm1Int, not currently implemented in libsemigroups
// TODO LeftActionPPerm1Int, not currently implemented in libsemigroups
// TODO Left/RightActionPPerm*BitSet
// TODO Left/RightActionTransf*BitSet not sure this exists in libsemigroups
// TODO Left/RightActionPerm

// C++ stl headers....
#include <string_view>  // for string_view

// libsemigroups headers
#include <libsemigroups/action.hpp>
#include <libsemigroups/bmat8.hpp>
#include <libsemigroups/transf.hpp>
#include <libsemigroups/word-graph.hpp>

#include <fmt/core.h>  // for format, print

// pybind11....
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_action

namespace libsemigroups {
  namespace py = pybind11;

  namespace {
    template <typename Element,
              typename Point,
              typename Func,
              typename Traits,
              side LeftOrRight>
    void bind_action(py::module& m, std::string_view name) {
      using Action_ = Action<Element, Point, Func, Traits, LeftOrRight>;
      using const_reference_point_type =
          typename Action_::const_reference_point_type;
      using index_type = typename Action_::index_type;
      py::class_<Action_, Runner> thing(m, name.data(), R"pbdoc(
Class for determining the action of a semigroup or monoid on a set.

This page contains details of the :any:`Action` class in
``libsemigroups_pybind11`` for finding actions of semigroups, monoids, or
groups, on sets.  The notion of an "action" in the context of
``libsemigroups_pybind11`` is analogous to the notion of an orbit of a group.

You are unlikely to want to use :any:`Action` directly, but rather via the more
convenient aliases :any:`RightAction` and :any:`LeftAction`.

The function :any:`Runner.run` finds points that can be obtained by
acting on the seeds of the action by the generators of the action until no
further points can be found, or :any:`Runner.stopped` returns
``True``.  This is achieved by performing a breadth first search.

In this documentation we refer to:

* ``Element`` -- the type of the elements of the underlying semigroup
* ``Point`` -- the type of the objects on which the semigroup elements act
* ``Func`` -- the function that computes the action of ``Element`` on ``Point``
* ``Side`` -- the side of the action (if it is a left or a right action).

.. seealso::
  :any:`LeftAction`, :any:`RightAction`, and :any:`Runner`.

.. |name| replace:: :any:`Action`

.. include:: ../../_static/runner_non_inherit.rst

.. doctest::

  >>> from libsemigroups_pybind11 import RightAction, BMat8
  >>> from libsemigroups_pybind11.bmat8 import row_space_basis
  >>> o = RightAction(
  ...     seeds=[
  ...         row_space_basis(
  ...             BMat8([[1, 1, 1, 0], [1, 1, 0, 0], [0, 1, 0, 1], [0, 1, 0, 0]])
  ...         )
  ...     ],
  ...     generators=[
  ...         BMat8([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]]),
  ...         BMat8([[0, 1, 0, 0], [1, 0, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]]),
  ...         BMat8([[0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1], [1, 0, 0, 0]]),
  ...         BMat8([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [1, 0, 0, 1]]),
  ...         BMat8([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 0]]),
  ...     ],
  ... )
  >>> len(o)
  553
)pbdoc");

      thing.def("__repr__", [](Action_ const& self) {
        return to_human_readable_repr(self);
      });
      thing.def("__getitem__", &Action_::at, py::is_operator(), py::arg("pos"));

      // No doc since not used directly, but is used indirectly in the
      // Action code.
      thing.def(py::init<>());

      thing.def("__copy__", [](Action_ const& self) { return Action_(self); });
      thing.def(
          "copy",
          [](Action_ const& self) { return Action_(self); },
          R"pbdoc(
:sig=(self: Action) -> Action:

Copy an :any:`Action`.

:returns: A copy of the *self*.
:rtype: Action
)pbdoc");

      thing.def("init",
                &Action_::init,
                R"pbdoc(
:sig=(self: Action) -> Action:

Re-initialize an existing object. This function puts an action object back into
the same state as if it had been newly default constructed.

:returns: *self*.
:rtype: Action
)pbdoc");
      thing.def("reserve",
                &Action_::reserve,
                py::arg("val"),
                R"pbdoc(
:sig=(self: Action, val: int) -> Action:

Increase the capacity to a value that is greater or equal to *val*.

:param val: new capacity of an action instance.
:type val: int


:returns: *self*.
:rtype: Action

:raises MemoryError:  if *val* is too large.

:complexity:
  At most linear in the :any:`size()` of the :py:class:`Action`.
)pbdoc");
      thing.def("add_seed",
                &Action_::add_seed,
                py::arg("seed"),
                R"pbdoc(
:sig=(self: Action, seed: Point) -> Action:

Add a seed to the action.

A *seed* is just a starting point for the action, it will belong to the action,
as will every point that can be obtained from the seed by acting with the
generators of the action.

:param seed: the seed to add.
:type seed: Point

:returns: *self*
:rtype: Action

:complexity: Constant.
)pbdoc");
      thing.def("add_generator",
                &Action_::add_generator,
                py::arg("gen"),
                R"pbdoc(
:sig=(self: Action, gen: Element) -> Action:

Add a generator to the action.

An :py:class:`Action` instance represents the action of the semigroup
generated by the elements added via this member function.

:param gen: the generator to add.
:type gen: Element

:returns: *self*.
:rtype: Action

:complexity: Constant.
)pbdoc");
      thing.def("number_of_generators",
                &Action_::number_of_generators,
                R"pbdoc(
:sig=(self: Action) -> int:

Returns the number of generators.

:returns:
   The number of generators.

:rtype:
   int

:complexity:
   Constant.
)pbdoc");
      thing.def(
          "generators",
          [](Action_ const& self) {
            return py::make_iterator(self.generators().cbegin(),
                                     self.generators().cend());
          },
          R"pbdoc(
:sig=(self: Action) -> collections.abc.Iterator[Element]:

Returns an iterator yielding the generators.

:returns:
   An iterator yielding the generators.
:rtype:
   collections.abc.Iterator[Element]

:complexity:
   Constant.
)pbdoc");
      thing.def(
          "position",
          [](Action_& self, const_reference_point_type pt) {
            return from_int(self.position(pt));
          },
          py::arg("pt"),
          R"pbdoc(
:sig=(self: Action, pt: Point) -> int | Undefined:

Returns the position of a point in the so far discovered points.

:param pt: the point whose position is sought.
:type pt: Point

:returns: The index of *pt* in *self* or :any:`UNDEFINED`.
:rtype: int | Undefined

:complexity: Constant.
)pbdoc");
      thing.def("empty",
                &Action_::empty,
                R"pbdoc(
:sig=(self: Action) -> bool:

Checks if the :py:class:`Action` contains any points.


:returns:
   ``True`` if the action contains no points (including seeds), and
   ``False`` if not.

:rtype:
   bool

:complexity:
   Constant.
)pbdoc");

      thing.def("size",
                &Action_::size,
                R"pbdoc(
:sig=(self: Action) -> int:

Returns the size of the fully enumerated action.

:returns:
   The size of the action, a value of type ``int``.

:rtype:
   int

:complexity:
   The time complexity is :math:`O(mn)` where :math:`m` is the total
   number of points in the orbit and :math:`n` is the number of
   generators.
)pbdoc");
      thing.def("current_size",
                &Action_::current_size,
                R"pbdoc(
:sig=(self: Action) -> int:

Returns the number of points found so far.

:returns:
   The current size.
:rtype:
   int

:complexity:
   Constant.
)pbdoc");
      thing.def(
          "cache_scc_multipliers",
          [](Action_ const& self) { return self.cache_scc_multipliers(); },
          R"pbdoc(
:sig=(self: Action) -> bool:

Returns whether or not we are caching scc multipliers. If the returned
value of this function is ``True``, then the values returned by
:any:`multiplier_from_scc_root()` and :any:`multiplier_to_scc_root()`
are cached, and not recomputed every time one of these functions is
called.

:returns:
   ``True`` if caching is enabled, and ``False`` if not.

:rtype:
   bool
)pbdoc");
      thing.def(
          "cache_scc_multipliers",
          [](Action_& self, bool val) -> Action_& {
            return self.cache_scc_multipliers(val);
          },
          py::arg("val"),
          R"pbdoc(
:sig=(self: Action, val: bool) -> Action:

Set whether or not to cache scc multipliers.

If the parameter *val* is ``True``, then the values returned by
:any:`multiplier_from_scc_root()` and :any:`multiplier_to_scc_root()` are
cached, and not recomputed every time one of these functions is called.

:param val: the value.
:type val: bool

:returns: *self*.
:rtype: Action

:complexity: Constant.
)pbdoc");
      thing.def("multiplier_from_scc_root",
                &Action_::multiplier_from_scc_root,
                py::arg("pos"),
                R"pbdoc(
:sig=(self: Action, pos: int) -> Element:

Returns a multiplier from a scc root to a given index.

Returns an element ``x`` of the semigroup generated by the generators in the
action such that if ``r`` is the root of the strongly connected component
containing ``self[pos]``, then after calling ``Func(res, r, x)`` the point
``res`` equals ``self[pos]``.

:param pos: a position in the action.
:type pos: int

:returns: The multiplier.
:rtype: Element

:raises LibsemigroupsError:
  if there are no generators yet added or the index *pos* is out of range.

:complexity:
  At most :math:`O(mn)` where :math:`m` is the complexity of multiplying
  elements of type ``Element`` and :math:`n` is the size of the fully enumerated
  orbit.
)pbdoc");
      thing.def("multiplier_to_scc_root",
                &Action_::multiplier_to_scc_root,
                py::arg("pos"),
                R"pbdoc(
:sig=(self: Action, pos: int) -> Element:

Returns a multiplier from a given index to a scc root.

Returns an element ``x`` of the semigroup generated by the generators in the
action such that after ``Func(res, at(pos), x)`` the point ``res`` is the root
of the strongly connected component containing ``self[pos]``.

:param pos: a position in the action.
:type pos: int

:returns: The multiplier.
:rtype: Element

:raises LibsemigroupsError:
  if there are no generators yet added or the index *pos* is out of range.

:complexity:
  At most :math:`O(mn)` where :math:`m` is the complexity of multiplying
  elements of type ``Element`` and :math:`n` is the size of the fully enumerated
  orbit.
)pbdoc");
      thing.def(
          "root_of_scc",
          [](Action_& self, const_reference_point_type x) {
            return self.root_of_scc(x);
          },
          py::arg("x"),
          R"pbdoc(
:sig=(self: Action, x: Point) -> Point:

Returns the root point of a strongly connected component containing a
``Point``.

:param x: the point whose root we want to find.
:type x: Point

:returns: The root point.
:rtype: Point

:raises LibsemigroupsError:  if the point *x* does not belong to the action.

:complexity:
  At most :math:`O(mn)` where :math:`m` is the complexity of multiplying
  elements of type ``Element`` and :math:`n` is the size of the fully enumerated
  orbit.
)pbdoc");
      thing.def(
          "root_of_scc",
          [](Action_& self, index_type pos) { return self.root_of_scc(pos); },
          py::arg("pos"),
          R"pbdoc(
:sig=(self: Action, pos: int) -> Point:

Returns the root point of a strongly connected component.

:param pos: the index of the point in the action whose root we want to find.
:type pos: int

:returns: The root point.
:rtype: Point

:raises LibsemigroupsError:  if the index *pos* is out of range.

:complexity: At most :math:`O(mn)` where :math:`m` is the complexity of
    multiplying elements of type ``Element`` and :math:`n` is the size of the
    fully enumerated orbit.
)pbdoc");
      thing.def("word_graph",
                &Action_::word_graph,
                R"pbdoc(
:sig=(self: Action) -> WordGraph:

Returns the word graph of the completely enumerated action.

:returns:
   The word graph of the action.
:rtype:
   WordGraph

:complexity:
   At most :math:`O(mn)` where :math:`m` is the complexity of
   multiplying elements of type ``Element`` and :math:`n` is the
   size of the fully enumerated orbit.
)pbdoc");
      thing.def("scc",
                &Action_::scc,
                R"pbdoc(
:sig=(self: Action) -> Gabow:

Returns a :any:`Gabow` object for strongly connected components.

:returns:
   A :py:class:`Gabow` object.
:rtype:
   Gabow

:complexity:
   At most :math:`O(mn)` where :math:`m` is the complexity of
   multiplying elements of type ``Element`` and :math:`n` is the
   size of the fully enumerated orbit.
)pbdoc");

      thing.def(
          "apply",
          [](Action_ const&             self,
             const_reference_point_type pt,
             Element const&             x) { return self.apply(pt, x); },
          py::arg("pt"),
          py::arg("x"),
          R"pbdoc(
:sig=(self: Action, pt: Point, x: Element) -> Point:

This function returns the point obtained by applying the action defined by
*self* to the point *pt* and the element *x*.

:param pt: the point.
:type pt: Point

:param x: the element.
:type x: Element

:returns:
  The image of *pt* under the action of *x*.
:rtype: Point
)pbdoc");
    }  // bind_action

    template <typename Element, typename Point>
    void bind_right_action(py::module& m, std::string_view name) {
      bind_action<Element,
                  Point,
                  ImageRightAction<Element, Point>,
                  ActionTraits<Element, Point>,
                  side::right>(m, name);
    }

    template <typename Element, typename Point>
    void bind_left_action(py::module& m, std::string_view name) {
      bind_action<Element,
                  Point,
                  ImageLeftAction<Element, Point>,
                  ActionTraits<Element, Point>,
                  side::left>(m, name);
    }
  }  // namespace

  void init_action(py::module& m) {
    py::options options;
    options.disable_enum_members_docstring();
    py::enum_<side>(m,
                    "side",
                    R"pbdoc(
The values in this enum can be used to indicate that the action in an
:any:`Action` instance should be a left action.

The valid values are:

.. py:attribute:: side.left
  :value: <side.left: 0>

   Value indicating that the action in an :any:`Action` instance should be a
   left action.

.. py:attribute:: side.right
  :value: <side.left: 1>

   Value indicating that the action in an :any:`Action` instance should be a
   right action.
)pbdoc")
        .value("left", side::left)
        .value("right", side::right);

    // One call to bind is required per list of types

    bind_right_action<BMat8, BMat8>(m, "RightActionBMat8BMat8");
    bind_left_action<BMat8, BMat8>(m, "LeftActionBMat8BMat8");

    bind_right_action<PPerm<0, uint8_t>, PPerm<0, uint8_t>>(
        m, "RightActionPPerm1PPerm1");
    // TODO add analogues of the next lines for HPCombi
    // bind_right_action<PPerm<16>, std::vector<uint8_t>>(
    //     m, "RightActionPPerm1List");
    bind_right_action<PPerm<0, uint8_t>, std::vector<uint8_t>>(
        m, "RightActionPPerm1List");
    bind_right_action<PPerm<0, uint16_t>, std::vector<uint16_t>>(
        m, "RightActionPPerm2List");
    bind_right_action<PPerm<0, uint32_t>, std::vector<uint32_t>>(
        m, "RightActionPPerm4List");

    bind_left_action<PPerm<0, uint8_t>, PPerm<0, uint8_t>>(
        m, "LeftActionPPerm1PPerm1");
    // TODO add analogues of the next lines for HPCombi
    // bind_left_action<PPerm<16>, std::vector<uint8_t>>(m,
    //                                                   "LeftActionPPerm1List");
    bind_left_action<PPerm<0, uint8_t>, std::vector<uint8_t>>(
        m, "LeftActionPPerm1List");
    bind_left_action<PPerm<0, uint16_t>, std::vector<uint16_t>>(
        m, "LeftActionPPerm2List");
    bind_left_action<PPerm<0, uint32_t>, std::vector<uint32_t>>(
        m, "LeftActionPPerm4List");

    // TODO add analogues of the next lines for HPCombi
    // bind_right_action<Transf<16>, std::vector<uint8_t>>(
    //     m, "RightActionTransf16List");
    bind_right_action<Transf<0, uint8_t>, std::vector<uint8_t>>(
        m, "RightActionTransf1List");
    bind_right_action<Transf<0, uint16_t>, std::vector<uint16_t>>(
        m, "RightActionTransf2List");
    bind_right_action<Transf<0, uint32_t>, std::vector<uint32_t>>(
        m, "RightActionTransf4List");

    // TODO add analogues of the next lines for HPCombi
    // bind_left_action<Transf<16>, std::vector<uint8_t>>(
    //     m, "LeftActionTransf16List");
    bind_left_action<Transf<0, uint8_t>, std::vector<uint8_t>>(
        m, "LeftActionTransf1List");
    bind_left_action<Transf<0, uint16_t>, std::vector<uint16_t>>(
        m, "LeftActionTransf2List");
    bind_left_action<Transf<0, uint32_t>, std::vector<uint32_t>>(
        m, "LeftActionTransf4List");
  }

}  // namespace libsemigroups
