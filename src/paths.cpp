//
// libsemigroups_pybind11
// Copyright (C) 2023-2024 Murray T. Whyte
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

// C++ stl headers....
#include <cstddef>  // for uint32_t
#include <cstdint>  // for uint64_t

// libsemigroups....
#include <libsemigroups/constants.hpp>   // for operator!=, operator==
#include <libsemigroups/order.hpp>       // for order
#include <libsemigroups/paths.hpp>       // for Paths
#include <libsemigroups/word-graph.hpp>  // for WordGraph, word_graph

// pybind11....
#include <pybind11/operators.h>  // for self, self_t, operator!=, operator*
#include <pybind11/pybind11.h>   // for class_, make_iterator, init, enum_
#include <pybind11/stl.h>        // for conversion of C++ to py types

// libsemigroups_pybind11....
#include "main.hpp"  // for init_paths

namespace libsemigroups {
  namespace py    = pybind11;
  using node_type = uint32_t;
  using size_type = typename WordGraph<node_type>::size_type;

  void init_paths(py::module& m) {
    ////////////////////////////////////////////////////////////////////////
    // Paths
    ////////////////////////////////////////////////////////////////////////

    using Paths_ = Paths<node_type>;

    py::class_<Paths_> thing1(m,
                              "Paths",
                              R"pbdoc(
Class for iterating through paths in a :any:`WordGraph`.

This class represents a range object that facilitates iterating through the
paths in a :any:`WordGraph` from a given :any:`source` (to a possible
:any:`target` node) in a particular :any:`order`.

.. note::
  Changing the value of :any:`source`, :any:`target`, :any:`min`, :any:`max`,
  or :any:`order` resets the :any:`Paths` object to point at the first word in
  the specified range.
)pbdoc");
    thing1.def("__repr__",
               [](Paths_ const& p) { return to_human_readable_repr(p); });
    thing1.def("__copy__", [](Paths_ const& p) { return Paths_(p); });
    thing1.def(
        "copy",
        [](Paths_ const& self) { return Paths_(self); },
        R"pbdoc(
:sig=(self: Paths) -> Paths:

Copy a :any:`Paths` object.

:returns: A copy.
:rtype: Paths
)pbdoc");
    thing1.def("__or__", [](Paths_ const& p, ToString const& to_str) {
      using rx::operator|;
      return p | to_str;
    });

    // __len__ is not allowed to return anything other than an int, hence
    // __len__ and count don't have the same behaviour.
    thing1.def("__len__", [](Paths_ const& p) {
      p.throw_if_source_undefined();
      auto result = p.count();
      if (result == POSITIVE_INFINITY) {
        return py::module_::import("sys").attr("maxsize").cast<uint64_t>();
      }
      return result;
    });

    thing1.def("__iter__", [](Paths_ const& p) {
      return py::make_iterator(rx::begin(p), rx::end(p));
    });

    thing1.def(
        "init",
        [](Paths_& self, WordGraph<node_type> const& wg) -> Paths_& {
          return self.init(wg);
        },
        py::arg("wg"),
        R"pbdoc(
Reinitialize a :any:`Paths` object.

This function puts a :any:`Paths` object back into the same state as if it had
been newly constructs from the :any:`WordGraph` *wg*.

:param wg: the word graph.
:type wg: WordGraph

:returns: *self*
:rtype: Paths
)pbdoc");

    thing1.def(py::init<WordGraph<node_type> const&>(),
               py::arg("wg"),
               R"pbdoc(
Construct from a :any:`WordGraph`.

This function constructs a :any:`Paths` object from the :any:`WordGraph`
*wg*.

:param wg: the word graph.
:type wg: WordGraph
)pbdoc");

    thing1.def(
        "at_end",
        [](Paths_& p) {
          p.throw_if_source_undefined();
          return p.at_end();
        },
        R"pbdoc(
Check if the range is exhausted. This function returns ``True`` if there
are no more paths in the range, and ``False`` otherwise.

:returns:
   Whether or not the range is exhausted.
:rtype:
   bool

:raises LibsemigroupsError: if ``source() == UNDEFINED``.
)pbdoc");

    thing1.def(
        "count",
        [](Paths_& p) {
          p.throw_if_source_undefined();
          return from_int(p.count());
        },
        R"pbdoc(
:sig=(self: Paths) -> int | PositiveInfinity:

Get the size of the range. This function returns the number of paths
remaining in the range (in particular, if :any:`next` is called then the
return value of :any:`count` decreases by ``1``).

:returns:
   The number of paths remaining in the range.
:rtype:
   int | PositiveInfinity

:raises LibsemigroupsError: if ``source() == UNDEFINED``.
    )pbdoc");

    thing1.def("current_target",
               &Paths_::current_target,
               R"pbdoc(
Get the current target node of the path labelled by :any:`Paths.get`. This
function returns the current target node of the path labelled by
:any:`Paths.get`. If there is no such path (because, for example, the source
node hasn't been defined, then :any:`UNDEFINED` is returned).

:returns:
   The current target node of the path labelled by :any:`Paths.get` or
   :any:`UNDEFINED`.
:rtype:
   int
)pbdoc");

    thing1.def(
        "get",
        [](Paths_& p) {
          p.throw_if_source_undefined();
          return p.get();
        },
        R"pbdoc(
Get the current path in the range.

:returns:
   The current path.
:rtype:
   list[int]

:raises LibsemigroupsError: if ``source() == UNDEFINED``.
)pbdoc");
    thing1.def(
        "max",
        [](Paths_ const& self) { return from_int(self.max()); },
        R"pbdoc(
:sig=(self: Paths) -> int | PositiveInfinity:
:only-document-once:

Get the maximum length of path in the range.

This function returns the current maximum length of paths in the range.
The initial value is :any:`POSITIVE_INFINITY`.

:returns:
   The maximum length of paths in the range.

:rtype:
   int | PositiveInfinity
)pbdoc");
    thing1.def(
        "max",
        [](Paths_& self, PositiveInfinity const& val) -> Paths_& {
          return self.max(val);
        },
        R"pbdoc(
:sig=(self: Paths, val: int | PositiveInfinity) -> Paths:
:only-document-once:
Set the maximum length of path in the range.

This function can be used to set the maximum length of path that will be
contained in the range. If this function is not called, then the range will
contain paths of unbounded length (possibly infinitely many).

:param val: the maximum path length.
:type val: int | PositiveInfinity

:returns: *self*.
:rtype: Paths
)pbdoc");
    thing1.def(
        "max",
        [](Paths_& self, size_type val) -> Paths_& { return self.max(val); },
        py::arg("val"),
        R"pbdoc(
:sig=(self: Paths, val: int | PositiveInfinity) -> Paths:
:only-document-once:
)pbdoc");
    thing1.def(
        "min",
        [](Paths_ const& self) { return self.min(); },
        R"pbdoc(
Get the minimum length of path in the range. This function returns the
current minimum length of paths in the range. The initial value is
``0``.

:returns:
   The minimum length of paths in the range.
:rtype:
   int
)pbdoc");
    thing1.def(
        "min",
        [](Paths_& self, size_type val) -> Paths_& { return self.min(val); },
        py::arg("val"),
        R"pbdoc(
:sig=(self: Paths, val: int) -> Paths:

Set the minimum length of path in the range.

This function can be used to set the minimum length of paths that will be
contained in the range. If this function is not called, then the range will
contain paths starting with length ``0``.

:param val: the minimum path length.
:type val: int

:returns: *self*.
:rtype: Paths
)pbdoc");
    thing1.def(
        "next",
        [](Paths_& p) {
          p.throw_if_source_undefined();
          p.next();
        },
        R"pbdoc(
Advance to the next path in the range. Advance to the current path in
the range. If :any:`at_end` returns ``True``, then this function does
nothing.

:raises LibsemigroupsError: if ``source() == UNDEFINED``.
)pbdoc");
    thing1.def(
        "order",
        [](Paths_ const& self) { return self.order(); },
        R"pbdoc(
Get the order of the paths in the range. This function returns the
current order of the paths in the range defined by a :any:`Paths`
object. The initial value is :any:`Order.shortlex`.

:returns:
   The order of the paths in the range.

:rtype:
   Order
)pbdoc");
    thing1.def(
        "order",
        [](Paths_& self, Order val) -> Paths_& { return self.order(val); },
        py::arg("val"),
        R"pbdoc(
Set the order of the paths in the range.

This function can be used to set the order of the paths in the range defined by
a :any:`Paths` object. The initial value is :any:`Order.shortlex`.

:param val: the order of the paths in the range.
:type val: Order

:returns: *self*.
:rtype: Paths

:raises LibsemigroupsError:
  if *val* is not :any:`Order.shortlex` or :any:`Order.lex`.
)pbdoc");
    thing1.def(
        "source",
        [](Paths_ const& self) { return self.source(); },
        R"pbdoc(
Get the current source node of every path in the range. This function
returns the current source node of the every path in the range defined
by a :any:`Paths` object. This initial value is :any:`UNDEFINED`.

:returns:
   The current source node.

:rtype:
   int
)pbdoc");
    thing1.def(
        "source",
        [](Paths_& self, node_type n) -> Paths_& { return self.source(n); },
        py::arg("n"),
        R"pbdoc(
:sig=(self: Paths, n: int) -> Paths:

Set the source node of every path in the range.

This function can be used to set the source node of all of the paths in the range.

:param n: the source node.
:type n: int

:returns: *self*.
:rtype: Paths

:raises LibsemigroupsError:
  if *n* is not a node in the underlying :any:`WordGraph` (:any:`word_graph`).
)pbdoc");
    thing1.def(
        "target",
        [](Paths_ const& self) { return self.target(); },
        R"pbdoc(
Get the current target node of every path in the range. This function
returns the target node of the every path in the range defined by a
:any:`Paths` object. This initial value is :any:`UNDEFINED`.

:returns:
   The target node.

:rtype:
   int
)pbdoc");
    thing1.def(
        "target",
        [](Paths_& self, node_type n) -> Paths_& { return self.target(n); },
        py::arg("n"),
        R"pbdoc(
:sig=(self: Paths, n: int) -> Paths:

Set the target node of every path in the range.

This function can be used to set the target node (or the "to" node) of all of
the paths in the range. It is not necessary to set this value. If the target
node is set to :any:`UNDEFINED`, then the range will contain every path from
:any:`source` to every possible target in the underlying :any:`WordGraph`
(:any:`word_graph`).

:param n: the target node.
:type n: int

:returns: *self*.
:rtype: Paths

:raises LibsemigroupsError:
  if *n* is not a node in the underlying :any:`WordGraph` (:any:`word_graph`)
  and *n* is not :any:`UNDEFINED`.
)pbdoc");
    thing1.def("word_graph",
               &Paths_::word_graph,
               R"pbdoc(
The underlying word graph. This function returns underlying
:any:`WordGraph` of the :any:`Paths` object. This is the
:any:`WordGraph` defining the paths in the range.

:returns:
   The underlying word graph.
:rtype:
   WordGraph
)pbdoc");
  }
}  // namespace libsemigroups
