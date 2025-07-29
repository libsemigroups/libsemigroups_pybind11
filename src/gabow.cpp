//
// libsemigroups_pybind11
// Copyright (C) 2021-2024 James D. Mitchell
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

// return_value_policy::reference_internal

// C++ stl headers....
#include <array>             // for array
#include <cstddef>           // for uint32_t
#include <cstdint>           // for uint64_t
#include <initializer_list>  // for initializer_list
#include <string>            // for to_string, basic_string
#include <vector>            // for vector

// libsemigroups....
#include <libsemigroups/constants.hpp>  // for operator!=, operator==

#include <libsemigroups/detail/int-range.hpp>  // for IntegralRange<>::value_type
#include <libsemigroups/forest.hpp>            // for Forest
#include <libsemigroups/gabow.hpp>             // for Gabow
#include <libsemigroups/word-graph.hpp>        // for WordGraph, word_graph

// pybind11....
#include <pybind11/operators.h>  // for self, self_t, operator!=, operator*
#include <pybind11/pybind11.h>   // for class_, make_iterator, init, enum_
#include <pybind11/stl.h>        // for conversion of C++ to py types

// libsemigroups_pybind11....
#include "main.hpp"  // for init_word_graph

namespace libsemigroups {
  namespace py    = pybind11;
  using node_type = uint32_t;
  void init_gabow(py::module& m) {
    ////////////////////////////////////////////////////////////////////////
    // Gabow
    ////////////////////////////////////////////////////////////////////////

    using Gabow_ = Gabow<node_type>;

    py::class_<Gabow_> thing(m,
                             "Gabow",
                             R"pbdoc(
Class implementing Gabow's :cite:`Gab00` algorithm for computing strongly
connected components of a :any:`WordGraph`.

Instances of this class can be used to compute, and provide information about,
the strongly connected components of the :any:`WordGraph` used to construct the
instance. The strongly connected components are lazily evaluated when triggered
by a relevant member function. The complexity of Gabow's algorithm is
at most :math:`O(mn)` where ``m`` is :any:`WordGraph.number_of_nodes()` and
``n`` is :any:`WordGraph.out_degree()`.
)pbdoc");

    thing.def("__repr__",
              [](Gabow_ const& g) { return to_human_readable_repr(g); });

    thing.def("__copy__", [](Gabow_ const& g) { return Gabow_(g); });

    thing.def(
        "copy",
        [](Gabow_ const& self) { return Gabow_(self); },
        R"pbdoc(
:sig=(self: Gabow) -> Gabow:

Copy a :any:`Gabow` object.

:returns: A copy.
:rtype: Gabow
)pbdoc");

    thing.def(py::init<WordGraph<node_type> const&>(),
              py::arg("wg"),
              R"pbdoc(
Construct a :any:`Gabow` from a :any:`WordGraph`.

This function constructs a :any:`Gabow` object from a :any:`WordGraph` *wg*.

:param wg: the :any:`WordGraph` to construct from.
:type wg: WordGraph
)pbdoc");

    thing.def("component",
              &Gabow_::component,
              py::arg("i"),
              R"pbdoc(
:sig=(self: Gabow, i: int) -> list[int]:

Returns a list containing the strongly connected component with given index.

This function returns a list containing the strongly connected components with
index *i* of the :any:`WordGraph` (returned by :any:`Gabow.word_graph`) used
to construct the :any:`Gabow` instance.

:param i: the index of a strongly connected component.
:type i: int

:returns: The component with index *i*.
:rtype: list[int]

:raises LibsemigroupsError:
  if there is *i* is greater than or equal to
  :any:`number_of_components`.

.. note::
   This function triggers the computation of the strongly connected
   components (if they are not already known).

.. seealso::  :any:`component_of` to obtain the :any:`component` of a node.
)pbdoc");

    thing.def("component_of",
              &Gabow_::component_of,
              py::arg("n"),
              R"pbdoc(
:sig=(self: Gabow, n: int) -> list[int]:

Returns a list containing the strongly connected component of a given node.

This function returns a list containing the strongly connected components of
the node *n* of the :any:`WordGraph` (returned by :any:`Gabow.word_graph` ) used to
construct the :any:`Gabow` instance.

:param n: the node.
:type n: int

:returns: The component of the node *n*.
:rtype: list[int]

:raises LibsemigroupsError:
  if *n* is greater than or equal to ``word_graph().number_of_nodes()``.

.. note::
   This function triggers the computation of the strongly connected
   components (if they are not already known).
)pbdoc");

    thing.def("components",
              &Gabow_::components,
              R"pbdoc(
This function returns a list of lists containing all of the strongly connected
components of the :any:`WordGraph` (returned by :any:`Gabow.word_graph`) used to
construct the :any:`Gabow` instance.

:returns:
   The strongly connected components of :any:`Gabow.word_graph`.
:rtype:
   list[list[int]]

.. note::
   This function triggers the computation of the strongly connected
   components (if they are not already known).
)pbdoc");

    thing.def("has_components",
              &Gabow_::has_components,
              R"pbdoc(
Check whether the strongly connected components have been found. This
function returns ``True`` if the strongly connected components of a
:any:`Gabow` object have already been computed and ``False`` if not.

:returns:
   Whether or not the strongly connected components have been found already.
:rtype:
   bool
)pbdoc");

    thing.def("id",
              &Gabow_::id,
              py::arg("n"),
              R"pbdoc(
:sig=(self: Gabow, n: int) -> int:

Returns the id-number of the strongly connected component of a node. This
function can be used to determine the id-number of the node *n* in the
underlying graph of a :any:`Gabow` instance.

:param n: the node.
:type n: int

:returns: The id-number of the strongly connected component of *n*.
:rtype: int

:raises LibsemigroupsError:
  if *n* is greater than or equal to ``word_graph().number_of_nodes()``.

.. note::
   This function triggers the computation of the strongly connected
   components (if they are not already known).
)pbdoc");

    thing.def("init",
              &Gabow_::init,
              py::arg("wg"),
              R"pbdoc(
This function re-initializes a :any:`Gabow` object so that it is in the same
state as if it had just been constructed from *wg*.

:param wg: The word graph.
:type wg: WordGraph

:returns: *self*.
:rtype: Gabow
)pbdoc");

    thing.def("number_of_components",
              &Gabow_::number_of_components,
              R"pbdoc(
This function returns the number of strongly connected components of the
underlying :any:`WordGraph` (returned by :any:`Gabow.word_graph`).

:returns:
   The number of strongly connected components.
:rtype:
   int

.. note::
   This function triggers the computation of the strongly connected
   components (if they are not already known).
)pbdoc");

    thing.def("reverse_spanning_forest",
              &Gabow_::reverse_spanning_forest,
              py::return_value_policy::reference_internal,
              R"pbdoc(
This function returns a :any:`Forest` comprised of spanning trees for
each strongly connected component of a :any:`Gabow` object, rooted on
the minimum node of that component, with edges oriented towards the
root.

:returns:
   A reverse spanning forest for the underlying word graph.
:rtype:
   Forest

.. note::
   This function triggers the computation of the strongly connected
   components (if they are not already known).
)pbdoc");

    thing.def("root_of",
              &Gabow_::root_of,
              py::arg("n"),
              R"pbdoc(
:sig=(self: Gabow, n: int) -> int:

Returns the root of the strongly connected component containing a given node.

This function returns the root of the strongly connected component containing
the node *n* of the underlying :any:`WordGraph`. Two nodes ``a`` and ``b``
belong to the same strongly connected component if and only if ``root_of(a) ==
root_of(b)``.

:param n: the node.
:type n: int

:returns:
  The root of the strongly connected component containing the node *n*.
:rtype:
  int

:raises LibsemigroupsError:
  if *n* is greater than or equal to :any:`WordGraph.number_of_nodes` of the
  underlying word graph.

.. note::
   This function triggers the computation of the strongly connected
   components (if they are not already known).
)pbdoc");

    thing.def(
        "roots",
        [](Gabow_& self) {
          auto r = self.roots();
          return py::make_iterator(rx::begin(r), rx::end(r));
        },
        R"pbdoc(
:sig=(self: Gabow) -> collections.abc.Iterator[int]:

Returns an iterator yielding the roots of the strongly connected
components of the underlying word graph.

:returns:
   An iterator to the roots.
:rtype:
   collections.abc.Iterator[int]

.. note::
   This function triggers the computation of the strongly connected
   components (if they are not already known).
)pbdoc");

    thing.def("spanning_forest",
              &Gabow_::spanning_forest,
              py::return_value_policy::reference_internal,
              R"pbdoc(
Returns a spanning forest of the strongly connected components. This
function returns a :any:`Forest` comprised of spanning trees for each
strongly connected component of a :any:`Gabow` object, rooted on the
minimum node of that component, with edges oriented away from the root.

:returns:
   A spanning forest for the underlying word graph.
:rtype:
   Forest

.. note::
   This function triggers the computation of the strongly connected
   components (if they are not already known).
)pbdoc");

    thing.def("word_graph",
              &Gabow_::word_graph,
              py::return_value_policy::reference_internal,
              R"pbdoc(
This function returns the underlying word graph.

:returns:
   The underlying word graph.
:rtype:
   WordGraph
)pbdoc");
  }
}  // namespace libsemigroups
