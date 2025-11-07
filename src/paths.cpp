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

    py::options options;
    options.disable_enum_members_docstring();

    py::enum_<paths::algorithm>(m, "paths_algorithm", R"pbdoc(
    An enum for specifying the algorithm to the functions :any:`number_of_paths`.

The valid values are:

.. py:attribute:: algorithm.dfs
  :value: <algorithm.dfs: 0>

  Use a depth-first-search.

.. py:attribute:: algorithm.matrix
  :value: <algorithm.matrix: 1>

  Use the adjacency matrix and matrix multiplication.

.. py:attribute:: algorithm.acyclic
  :value: <algorithm.acyclic: 2>

  Use a dynamic programming approach for acyclic word graphs.

.. py:attribute:: algorithm.trivial
  :value: <algorithm.trivial: 3>

  Try to utilise some corner cases.

.. py:attribute:: algorithm.automatic
  :value: <algorithm.automatic: 4>

  The function :any:`number_of_paths` tries to decide which algorithm is best.
)pbdoc")
        .value("dfs", paths::algorithm::dfs)
        .value("matrix", paths::algorithm::matrix)
        .value("acyclic", paths::algorithm::acyclic)
        .value("trivial", paths::algorithm::trivial)
        .value("automatic", paths::algorithm::automatic);

    ////////////////////////////////////////////////////////////////////////
    // Helper functions
    ////////////////////////////////////////////////////////////////////////

    m.def(
        "paths_number_of_paths_algorithm",
        [](WordGraph<node_type> const& wg, node_type source) {
          return number_of_paths_algorithm(wg, source);
        },
        py::arg("wg"),
        py::arg("source"),
        R"pbdoc(
:sig=(wg: WordGraph, source: int) -> paths.algorithm:

Returns the :any:`paths.algorithm` used by :any:`number_of_paths`.

This function returns the algorithm that would be used by a call to :any:`number_of_paths`
with the same arguments.

:param wg: the word graph.
:type wg: WordGraph

:param source: the source node.
:type source: int

:returns: The algorithm.
:rtype: paths.algorithm

:complexity: Constant.
)pbdoc");

    m.def(
        "paths_number_of_paths_algorithm",
        [](WordGraph<node_type> const& wg,
           node_type                   source,
           size_t                      min,
           int_or_constant<size_t>     max) {
          return number_of_paths_algorithm(
              wg, source, min, to_int<size_t>(max));
        },
        py::arg("wg"),
        py::arg("source"),
        py::arg("min"),
        py::arg("max"),
        R"pbdoc(
:sig=(wg: WordGraph, source: int, min: int, max: int | PositiveInfinity) -> paths.algorithm:

Returns the :any:`paths.algorithm` used by :any:`number_of_paths`.

This function returns the algorithm used by :any:`number_of_paths` to compute
the number of paths originating at the given source node with length in
the range ``[min, max)``.

:param wg: the word graph.
:type wg: WordGraph

:param source: the source node.
:type source: int

:param min: the minimum length of paths to count.
:type min: int

:param max: the maximum length of paths to count.
:type max: int | PositiveInfinity

:returns: The algorithm.
:rtype: paths.algorithm

:complexity:
  At worst :math:`O(nm)` where :math:`n` is the number of nodes and :math:`m`
  is the out-degree of the word graph.
)pbdoc");

    m.def(
        "paths_number_of_paths_algorithm",
        [](WordGraph<node_type> const& wg,
           node_type                   source,
           node_type                   target,
           size_t                      min,
           int_or_constant<size_t>     max) {
          return number_of_paths_algorithm(
              wg, source, target, min, to_int<size_t>(max));
        },
        py::arg("wg"),
        py::arg("source"),
        py::arg("target"),
        py::arg("min"),
        py::arg("max"),
        R"pbdoc(
:sig=(wg: WordGraph, source: int, target: int, min: int, max: int | PositiveInfinity) -> paths.algorithm:

Returns the :any:`paths.algorithm` used by :any:`number_of_paths`.

This function returns the algorithm used by :any:`number_of_paths` to
compute the number of paths originating at the given source node and
ending at the given target node with length in the range ``[min, max)``.

:param wg: the word graph.
:type wg: WordGraph

:param source: the source node.
:type source: int

:param target: the target node.
:type target: int

:param min: the minimum length of paths to count.
:type min: int

:param max: the maximum length of paths to count.
:type max: int | PositiveInfinity

:returns: The algorithm.
:rtype: paths.algorithm

:complexity:
  At worst :math:`O(nm)` where :math:`n` is the number of nodes and :math:`m`
  is the out-degree of the word graph.
)pbdoc");

    m.def(
        "paths_number_of_paths",
        [](WordGraph<node_type> const& wg, node_type source) {
          return from_int(number_of_paths(wg, source));
        },
        py::arg("wg"),
        py::arg("source"),
        R"pbdoc(
:sig=(wg: WordGraph, source: int) -> int | PositiveInfinity:

Returns the number of paths from a source node.

This function returns the number of paths in the word graph *wg* starting at
*source*.

:param wg: the word graph.
:type wg: WordGraph

:param source: the source node.
:type source: int

:returns: The number of paths.
:rtype: int | PositiveInfinity

:raises LibsemigroupsError: if *source* is not a node of the word graph *wg*.

:complexity:
  At worst :math:`O(nm)` where :math:`n` is the number of nodes and :math:`m`
  is the out-degree of the word graph.

.. note::
    If ``libsemigroups`` is compiled with the flag ``--enable-eigen``, then
    this function makes use of the `Eigen` library for linear algebra (see
    :cite:`Guennebaud2010aa`).

.. warning::
    If the number of paths exceeds ``2 ** 64``, then return value of
    this function will not be correct.
)pbdoc");

    m.def(
        "paths_number_of_paths",
        [](WordGraph<node_type> const& wg,
           node_type                   source,
           size_t                      min,
           int_or_constant<size_t>     max,
           paths::algorithm            lgrthm) {
          return from_int(
              number_of_paths(wg, source, min, to_int<size_t>(max), lgrthm));
        },
        py::arg("wg"),
        py::arg("source"),
        py::arg("min"),
        py::arg("max"),
        py::arg("lgrthm") = paths::algorithm::automatic,
        R"pbdoc(
:sig=(wg: WordGraph, source: int, min: int, max: int | PositiveInfinity, lgrthm: paths.algorithm = paths.algorithm.automatic) -> int | PositiveInfinity:

Returns the number of paths starting at a given node with length in a
given range.

This function returns the number of paths starting at a given node with
length in a given range.

:param wg: the word graph.
:type wg: WordGraph

:param source: the source node.
:type source: int

:param min: the minimum length of paths to count.
:type min: int

:param max: the maximum length of paths to count.
:type max: int | PositiveInfinity

:param lgrthm: the algorithm to use (defaults to :any:`paths.algorithm.automatic`).
:type lgrthm: paths.algorithm

:returns: The number of paths.
:rtype: int | PositiveInfinity

:complexity:
  The complexity depends on the value of *lgrthm* as follows:

  * :any:`paths.algorithm.dfs`: :math:`O(r)` where :math:`r` is the number of paths
    in the word graph starting at *source*
  * :any:`paths.algorithm.matrix`: at worst :math:`O(n ^ 3 k)` where :math:`n` is the
    number of nodes and :math:`k` equals *max*.
  * :any:`paths.algorithm.acyclic`: at worst :math:`O(nm)` where :math:`n` is the number
    of nodes and :math:`m` is the out-degree of the word graph (only valid if
    the subgraph induced by the nodes reachable from *source* is acyclic)
  * :any:`paths.algorithm.trivial`: at worst :math:`O(nm)` where :math:`n` is the number
    of nodes and :math:`m` is the out-degree of the word graph (only valid in
    some circumstances)
  * :any:`paths.algorithm.automatic`: attempts to select the fastest algorithm of the
    preceding algorithms and then applies that.

.. note::
    If ``libsemigroups`` is compiled with the flag ``--enable-eigen``, then
    this function makes use of the `Eigen` library for linear algebra (see
    :cite:`Guennebaud2010aa`).

.. warning::
    If the number of paths exceeds ``2 ** 64``, then return value of
    this function will not be correct.

.. warning::
    If *lgrthm* is :any:`paths.algorithm.automatic`, then it is not
    always the case that the fastest algorithm is used.
)pbdoc");

    m.def(
        "paths_number_of_paths",
        [](WordGraph<node_type> const& wg,
           node_type                   source,
           node_type                   target,
           size_t                      min,
           int_or_constant<size_t>     max,
           paths::algorithm            algrthm) {
          return number_of_paths(wg, source, target, min, to_int<size_t>(max));
        },
        py::arg("wg"),
        py::arg("source"),
        py::arg("target"),
        py::arg("min"),
        py::arg("max"),
        py::arg("lgrthm") = paths::algorithm::automatic,
        R"pbdoc(
:sig=(wg: WordGraph, source: int, target: int, min: int, max: int | PositiveInfinity, lgrthm: paths.algorithm = paths.algorithm.automatic) -> int | PositiveInfinity:

Returns the number of paths starting at the node *source* and ending at
node *target* with length in a given range.

This function returns the number of paths starting at the node *source*
and ending at node *target* with length in a given range.

:param wg: the word graph.
:type wg: WordGraph

:param source: the source node.
:type source: int

:param target: the target node.
:type target: int

:param min: the minimum length of paths to count.
:type min: int

:param max: the maximum length of paths to count.
:type max: int | PositiveInfinity

:param lgrthm: the algorithm to use (defaults to :any:`paths.algorithm.automatic`).
:type lgrthm: paths.algorithm

:returns: The number of paths.
:rtype: int | PositiveInfinity

:complexity:
  The complexity depends on the value of *lgrthm* as follows:

  * :any:`paths.algorithm.dfs`: :math:`O(r)` where :math:`r` is the number of paths
    in the word graph starting at *source*
  * :any:`paths.algorithm.matrix`: at worst :math:`O(n ^ 3 k)` where :math:`n` is the
    number of nodes and :math:`k` equals *max*.
  * :any:`paths.algorithm.acyclic`: at worst :math:`O(nm)` where :math:`n` is the number
    of nodes and :math:`m` is the out-degree of the word graph (only valid if
    the subgraph induced by the nodes reachable from *source* is acyclic)
  * :any:`paths.algorithm.trivial`: at worst :math:`O(nm)` where :math:`n` is the number
    of nodes and :math:`m` is the out-degree of the word graph (only valid in
    some circumstances)
  * :any:`paths.algorithm.automatic`: attempts to select the fastest algorithm of the
    preceding algorithms and then applies that.

.. note::
    If ``libsemigroups`` is compiled with the flag ``--enable-eigen``, then
    this function makes use of the `Eigen` library for linear algebra (see
    :cite:`Guennebaud2010aa`).

.. warning::
    If the number of paths exceeds ``2 ** 64``, then return value of
    this function will not be correct.

.. warning::
    If *lgrthm* is :any:`paths.algorithm.automatic`, then it is not
    always the case that the fastest algorithm is used.
)pbdoc");
  }
}  // namespace libsemigroups
