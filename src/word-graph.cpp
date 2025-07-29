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

// C++ stl headers....
#include <cstddef>  // for uint32_t
#include <vector>   // for vector

// libsemigroups....
#include <libsemigroups/config.hpp>     // for LIBSEMIGROUPS_EIGEN_ENABLED
#include <libsemigroups/constants.hpp>  // for operator!=, operator==
#include <libsemigroups/detail/int-range.hpp>  // for IntegralRange<>::value_type
#include <libsemigroups/word-graph.hpp>        // for WordGraph

// pybind11....
#include <pybind11/operators.h>  // for self, self_t, operator!=, operator*
#include <pybind11/pybind11.h>   // for class_, make_iterator, init, enum_
#include <pybind11/stl.h>        // for conversion of C++ to py types

#ifdef LIBSEMIGROUPS_EIGEN_ENABLED
#include <pybind11/eigen.h>  // for adjacency_matrix
#endif

// libsemigroups_pybind11....
#include "main.hpp"  // for init_word_graph

namespace libsemigroups {
  namespace py = pybind11;

  void init_word_graph(py::module& m) {
    using WordGraph_ = WordGraph<uint32_t>;

    using node_type  = typename WordGraph_::node_type;
    using label_type = typename WordGraph_::label_type;

    py::class_<WordGraph_> thing(m,
                                 "WordGraph",
                                 R"pbdoc(
Class for representing word graphs.

Instances of this class represent word graphs. If the word graph has ``n``
nodes, they are represented by the numbers :math:`\{0, ..., n - 1\}`, and every
node has the same number ``m`` of out-edges (edges with source that node and
target any other node or :any:`UNDEFINED`). The number ``m`` is referred to as
the *out-degree* of the word graph, or any of its nodes.)pbdoc");

    thing.def("__repr__", [](WordGraph_ const& self) {
      return to_human_readable_repr(self);
    });

    thing.def("__str__", [](WordGraph_ const& self) {
      return to_input_string(self, "WordGraph(", "[]", ")");
    });

    thing.def(py::self != py::self);
    thing.def(py::self < py::self);
    thing.def(py::self <= py::self);
    thing.def(py::self == py::self);
    thing.def(py::self > py::self);
    thing.def(py::self >= py::self);

    thing.def("__hash__", &WordGraph_::hash_value);
    thing.def("__copy__", [](WordGraph_ const& wg) { return WordGraph_(wg); });
    thing.def(
        "copy",
        [](WordGraph_ const& wg) { return WordGraph_(wg); },
        R"pbdoc(
Copy a :any:`WordGraph` object.

:returns: A copy.
:rtype: WordGraph
)pbdoc");

    thing.def(py::init<size_t, size_t>(),
              py::arg("m") = 0,
              py::arg("n") = 0,
              R"pbdoc(
:sig=(self: WordGraph, m: int = 0, n: int = 0) -> None:

Construct from number of nodes and out degree.

This function constructs a word graph with *m* nodes and where the maximum
out-degree of any node is *n*. There are no edges in the defined word graph.

:param m: the number of nodes in the word graph (default: ``0``).
:type m: int

:param n: the out-degree of every node (default: ``0``).
:type n: int

:complexity:
  :math:`O(mn)` where *m* is the number of nodes, and *n* is the
  out-degree of the word graph.)pbdoc");

    thing.def(
        py::init([](size_t                                           num_nodes,
                    std::vector<std::vector<
                        int_or_unsigned_constant<node_type>>> const& targets) {
          return make<WordGraph_>(num_nodes, to_ints<node_type>(targets));
        }),
        py::arg("num_nodes"),
        py::arg("targets"),
        R"pbdoc(
:sig=(self: WordGraph, num_nodes: int, targets: list[list[int | Undefined | PositiveInfinity | LimitMax]]) -> None:

Construct a word graph from a number of nodes and an list of targets.

This function constructs a word graph from its arguments whose
out-degree is specified by the length of the first item in *targets*.

:param num_nodes: the number of nodes in the word graph.
:type num_nodes: int

:param targets: list of the targets.
:type targets: list[list[int | Undefined | PositiveInfinity | LimitMax]]


:raises LibsemigroupsError:
    if any target is specified in *targets* is greater than or equal to
    *num_nodes*.

.. doctest::

  >>> from libsemigroups_pybind11 import WordGraph
  >>> WordGraph(5, [[0, 0], [1, 1], [2], [3, 3]])
  <WordGraph with 5 nodes, 7 edges, & out-degree 2>

)pbdoc");

    thing.def("add_nodes",
              &WordGraph_::add_nodes,
              py::arg("nr"),
              R"pbdoc(
:sig=(self: WordGraph, nr: int) -> WordGraph:

Add a number of new nodes.

This function modifies a word graph in-place so that it has *nr* new nodes
added.

:param nr: the number of nodes to add.
:type nr: int

:returns: *self*.
:rtype: WordGraph

:complexity: Linear in ``(number_of_nodes() + nr) * out_degree()``.)pbdoc");

    thing.def("add_to_out_degree",
              &WordGraph_::add_to_out_degree,
              py::arg("nr"),
              R"pbdoc(
:sig=(self: WordGraph, nr: int) -> WordGraph:

Add to the out-degree of every node.

This function modifies a word graph in-place so that the out-degree is
increased by *nr*.

:param nr: the number of new out-edges for every node.
:type nr: int

:returns: *self*.
:rtype: WordGraph

:complexity: :math:`O(mn)` where ``m`` is the number of nodes, and ``n`` is
  the new out degree of the word graph.
)pbdoc");

    thing.def(
        "nodes",
        [](WordGraph_ const& self) {
          return py::make_iterator(self.cbegin_nodes(), self.cend_nodes());
        },
        R"pbdoc(
Returns an iterator yielding the nodes of the word graph.

This function returns an iterator yielding the nodes of
the word graph.

:returns:
   An iterator yielding the nodes.

:rtype:
   collections.abc.Iterator[int]

:complexity:
   Constant.)pbdoc");

    thing.def(
        "targets",
        [](WordGraph_ const& self, node_type source) {
          auto result
              = (self.targets(source) | rx::transform([](node_type target) {
                   return from_int(target);
                 }));
          return py::make_iterator(rx::begin(result), rx::end(result));
        },
        py::arg("source"),
        R"pbdoc(
:sig=(self: WordGraph, source: int) -> collections.abc.Iterator[int | Undefined]:

Returns an iterator yielding the targets of the edges incident to a given
node.

This function returns an iterator yielding the targets of the edges incident
to the source node *source*. This target might equal :any:`UNDEFINED`.

:param source: the source node in the word graph.
:type source: int

:returns: An iterator yielding the targets.
:rtype: collections.abc.Iterator[int | Undefined]

:raises LibsemigroupsError:
  if *source* is out of range (i.e. greater than or equal to
  :any:`number_of_nodes`).

:complexity: Constant.

)pbdoc");

    thing.def("disjoint_union_inplace",
              &WordGraph_::disjoint_union_inplace,
              py::arg("that"),
              R"pbdoc(
Unites a word graph in-place.

This function changes a :any:`WordGraph` object in-place to contain the
disjoint union of itself and *that*. The node ``n`` of *that* is mapped to
``number_of_nodes() + n``.

:param that: the word graph to unite.
:type that: WordGraph

:returns: *self*.
:rtype: WordGraph

:raises LibsemigroupsError:
   if *self* and *that* do not have the same out-degree.
)pbdoc");

    thing.def(
        "induced_subgraph",
        [](WordGraph_& self, node_type first, node_type last) -> WordGraph_& {
          return self.induced_subgraph(first, last);
        },
        py::arg("first"),
        py::arg("last"),
        R"pbdoc(
:sig=(self: WordGraph, first: int, last: int) -> WordGraph:

Modify in-place to contain the subgraph induced by a range of nodes.

This function modifies a :any:`WordGraph` object in-place to contain its
subgraph induced by the range of nodes *first* to *last*.

:param first: the first node.
:type first: int

:param last: one more than the last node.
:type last: int

:returns: *self*.
:rtype: WordGraph

:raises LibsemigroupsError:  if *first* or *last* is out of range.

:raises LibsemigroupsError:
  if any edge with source in the range *first* to *last* has target outside
  the range *first* to *last*.

)pbdoc");

    thing.def(
        "init",
        [](WordGraph_& self, size_t m, size_t n) -> WordGraph_& {
          return self.init(m, n);
        },
        py::arg("m"),
        py::arg("n"),
        R"pbdoc(
:sig=(self: WordGraph, m: int, n: int) -> WordGraph:

Re-initialize the word graph to have *m* nodes and out-degree *n*.

This function puts a word graph into the state that it would have been in if it
had just been newly constructed with the same parameters *m* and *n*.

:param m: the number of nodes in the word graph.
:type m: int

:param n: the out-degree of every node.
:type n: int

:returns: *self*.
:rtype: WordGraph

:complexity:
    :math:`O(mn)` where :math:`m` is the number of nodes, and :math:`n` is the
    out-degree of the word graph.)pbdoc");

    thing.def(
        "labels_and_targets",
        [](WordGraph_ const& self, node_type source) {
          auto r = (self.labels_and_targets(source)
                    | rx::transform([](auto const& label_target) {
                        return from_ints<node_type>(label_target);
                      }));
          return py::make_iterator(rx::begin(r), rx::end(r));
        },
        py::arg("source"),
        R"pbdoc(
:sig=(self: WordGraph, source: int) -> collections.abc.Iterator[tuple[int, int | Undefined]]:

Returns an iterator yielding pairs consisting of edge labels and
target nodes.

This function returns an iterator yielding all the edge labels and
targets of edges with source *source*.

:param source: the source node.
:type source: int

:returns: An iterator.
:rtype: collections.abc.Iterator[tuple[int, int | Undefined]]

:raises LibsemigroupsError:  if *source* is out of bounds.)pbdoc");

    thing.def(
        "next_label_and_target",
        [](WordGraph_ const& self, node_type s, label_type a) {
          return from_ints<node_type>(self.next_label_and_target(s, a));
        },
        py::arg("s"),
        py::arg("a") = 0,
        R"pbdoc(
:sig=(self: WordGraph, s: int, a: int) -> tuple[int | Undefined, int | Undefined]:

Get the next target of an edge incident to a given node that doesn't equal
:any:`UNDEFINED`.

This function returns the next target of an edge with label greater than or
equal to *a* that is incident to the node *s*. If ``target(s, b)`` equals
:any:`UNDEFINED` for every value ``b`` in the range :math:`[a, n)`, where
:math:`n` is the return value of :any:`out_degree()`, then ``x.first`` and
``x.second`` equal :any:`UNDEFINED`.

:param s: the node.
:type s: int

:param a: the label (default: ``0``).
:type a: int

:returns:
  Returns a pair where the first entry is the next label after *a* and the
  second is the next target of *s* that is not :any:`UNDEFINED`.
:rtype: tuple[int | Undefined, int | Undefined]

:raises LibsemigroupsError:
  if *s* does not represent a node in *self*.

:complexity: At worst :math:`O(n)` where :math:`n` equals :any:`out_degree()`.
)pbdoc");
    // TODO(1) should we check that `a` is valid in the previous?
    thing.def(
        "number_of_edges",
        [](WordGraph_ const& self) { return self.number_of_edges(); },
        R"pbdoc(
Returns the number of edges. This function returns the total number of edges
(i.e. values ``s`` and ``a`` such that ``target(s, a)`` is not
:any:`UNDEFINED`) in the word graph.

:returns:
   The total number of edges.
:rtype:
   int

:complexity:
   :math:`O(mn)` where ``m`` is :any:`number_of_nodes()` and ``n`` is
   :any:`out_degree()`.)pbdoc");
    thing.def(
        "number_of_edges",
        [](WordGraph_ const& self, node_type s) {
          return self.number_of_edges(s);
        },
        py::arg("s"),
        R"pbdoc(
:sig=(self: WordGraph, s: int) -> int:

Returns the number of edges with given source node.

This function returns the number of edges incident to the given source node
*s*.

:param s: the node.
:type s: int

:returns: The number of edge incident to *s*.
:rtype: int

:raises LibsemigroupsError:  if *s* is not a node.

:complexity:  :math:`O(n)` where ``n`` is :any:`out_degree()`.)pbdoc");
    thing.def("number_of_nodes",
              &WordGraph_::number_of_nodes,
              R"pbdoc(
Returns the number of nodes. This function returns the number of nodes
in the word graph.

:returns:
   The number of nodes in the word graph.

:rtype:
   int

:complexity:
   Constant.)pbdoc");
    thing.def("out_degree",
              &WordGraph_::out_degree,
              R"pbdoc(
Returns the out-degree. This function returns the number of edge labels
in the word graph.

:returns:
   The number of edge labels.
:rtype:
   int

:complexity:
   Constant.)pbdoc");
    thing.def("remove_all_targets",
              &WordGraph_::remove_all_targets,
              R"pbdoc(
Remove all of the edges in the word graph. Set every target of every
source with every possible label to :any:`UNDEFINED`.

:returns: *self*.
:rtype: WordGraph

:complexity:
   :math:`O(mn)` where ``m`` is the number of nodes and ``n`` is the
   out-degree.
)pbdoc");
    thing.def("remove_label",
              &WordGraph_::remove_label,
              py::arg("a"),
              R"pbdoc(
:sig=(self: WordGraph, a: int) -> WordGraph:

Removes a given label from the word graph.

This function removes the label *a* from a :any:`WordGraph` object in-place.
This reduces the out-degree by ``1``.

:param a: the label to remove.
:type a: int

:returns: *self*.
:rtype: WordGraph

:raises LibsemigroupsError:  if *a* is out of range.
)pbdoc");
    thing.def("remove_target",
              &WordGraph_::remove_target,
              py::arg("s"),
              py::arg("a"),
              R"pbdoc(
:sig=(self: WordGraph, s: int, a: int) -> WordGraph:

Remove an edge from a node with a given label.

This function removes the edge with source node *s* labelled by *a*.

:param s: the source node.
:type s: int

:param a: the label of the edge from s.
:type a: int

:returns: *self*.
:rtype: WordGraph

:raises LibsemigroupsError:  if *s* or *a* is out of range.

:complexity: Constant.
)pbdoc");
    thing.def("reserve",
              &WordGraph_::reserve,
              py::arg("m"),
              py::arg("n"),
              R"pbdoc(
:sig=(self: WordGraph, m: int, n: int) -> WordGraph:

Ensures that the word graph has capacity for a given number of nodes, and
out-degree.

This function ensures that the word graph has capacity for *m* nodes and
*n* labels.

:param m: the number of nodes.
:type m: int

:param n: the out-degree.
:type n: int

:returns: *self*.
:rtype: WordGraph

:complexity:
  :math:`O(mn)` where *m* is the number of nodes and *n* is the
  out-degree.)pbdoc");
    thing.def("swap_targets",
              &WordGraph_::swap_targets,
              py::arg("m"),
              py::arg("n"),
              py::arg("a"),
              R"pbdoc(
:sig=(self: WordGraph, m: int, n: int, a: int) -> WordGraph:

Swaps the edge with specified label from one node with another.

This function swaps the target of the edge from the node *m* labelled *a*
with the target of the edge from the node *n* labelled *a*.

:param m: the first node.
:type m: int

:param n: the second node.
:type n: int

:param a: the label.
:type a: int

:returns: *self*.
:rtype: WordGraph

:raises LibsemigroupsError:  if any of *m*, *n*, and *a* is out of
  range.

:complexity: Constant
)pbdoc");
    thing.def(
        "target",
        [](WordGraph_& self, node_type s, label_type a, node_type t) {
          return self.target(s, a, t);
        },
        py::arg("s"),
        py::arg("a"),
        py::arg("t"),
        R"pbdoc(
:sig=(self: WordGraph, s:int, a: int, t:int) -> WordGraph:

Add an edge from one node to another with a given label.

If *s* and *t* are nodes in *self*, and *a* is in the range ``[0,
out_degree())``, then this function adds an edge from *a* to *b* labelled *a*.

:param s: the source node.
:type s: int

:param a: the label of the edge.
:type a: int

:param t: the range node.
:type t: int

:returns: *self*.
:rtype: WordGraph

:raises LibsemigroupsError:  if *s*, *a*, or *t* is not valid.

:complexity: Constant.)pbdoc");
    thing.def(
        "target",
        [](WordGraph_ const& self, node_type source, label_type a) {
          return from_int(self.target(source, a));
        },
        py::arg("source"),
        py::arg("a"),
        R"pbdoc(
:sig=(self: WordGraph, source:int, a: int) -> int:

Get the target of the edge with given source node and label.

This function returns the target of the edge with source node *source* and
label *a*.

:param source: the node.
:type source: int

:param a: the label.
:type a: int

:returns:
  Returns the node adjacent to *source* via the edge labelled *a*, or
  :any:`UNDEFINED`.
:rtype: int

:raises LibsemigroupsError: if *source* or *a* is not valid.

:complexity: Constant.)pbdoc");
    thing.def_static(
        "random",
        [](size_t number_of_nodes, size_t out_degree) {
          return WordGraph_::random(number_of_nodes, out_degree);
        },
        py::arg("number_of_nodes"),
        py::arg("out_degree"),
        R"pbdoc(
:sig=(number_of_nodes: int, out_degree: int) -> WordGraph:

Construct a random word graph from number of nodes and out-degree.

This function constructs a random word graph with *number_of_nodes* nodes and
out-degree *out_degree*.

:param number_of_nodes: the number of nodes.
:type number_of_nodes: int

:param out_degree: the out-degree of every node.
:type out_degree: int

:returns: A random word graph.
:rtype: WordGraph

:raises LibsemigroupsError: if *number_of_nodes* is less than ``2``
:raises LibsemigroupsError: if *out_degree* is less than ``2``

:complexity: :math:`O(mn)` where ``m`` is the number of nodes, and ``n`` is
  the out-degree of the word graph.)pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // Helpers
    ////////////////////////////////////////////////////////////////////////

    m.def(
        "word_graph_add_cycle",
        [](WordGraph_& wg, size_t N) { return word_graph::add_cycle(wg, N); },
        py::arg("wg"),
        py::arg("n"),
        R"pbdoc(
:sig=(wg: WordGraph, n: int) -> None:
Adds a cycle consisting of *N* new nodes.

:param wg:
   the WordGraph object to add a cycle to.

:type wg:
   WordGraph

:param n:
   the length of the cycle and number of new nodes to add.

:type n:
   int

:complexity:
   :math:`O(n)` where :math:`n` is the second parameter.
)pbdoc");

    m.def(
        "word_graph_adjacency_matrix",
        [](WordGraph_ const& wg) { return word_graph::adjacency_matrix(wg); },
        py::arg("wg"),
        R"pbdoc(
:sig=(wg: WordGraph) -> numpy.ndarray[numpy.float64] | Matrix:
Returns the adjacency matrix of a word graph.

This function returns the adjacency matrix of the word graph *wg*. The
type of the returned matrix depends on whether or not libsemigroups_ is
compiled with `eigen <http://eigen.tuxfamily.org/>`_ enabled. The returned
matrix has the number of edges with source ``s`` and target ``t`` in the
``(s, t)``-entry.

:param wg: the word graph.
:type wg: WordGraph

:returns: The adjacency matrix.
:rtype: numpy.ndarray[numpy.float64] | Matrix
)pbdoc");

    m.def(
        "word_graph_dot",
        [](WordGraph_ const& wg) { return word_graph::dot(wg); },
        py::arg("wg"),
        R"pbdoc(
:sig=(wg: WordGraph) -> Dot:
Returns a :any:`Dot` object representing a word graph.

This function returns a :any:`Dot` object representing the word graph *wg*.

:param wg: the word graph.
:type wg: WordGraph

:returns: A :any:`Dot` object.
:rtype: Dot
   )pbdoc");

    m.def(
        "word_graph_equal_to",
        [](WordGraph_ const& x,
           WordGraph_ const& y,
           node_type         first,
           node_type last) { return word_graph::equal_to(x, y, first, last); },
        py::arg("x"),
        py::arg("y"),
        py::arg("first"),
        py::arg("last"),
        R"pbdoc(
:sig=(x: WordGraph, y: WordGraph, first: int, last:int) -> bool:
Compares two word graphs on a range of nodes.

This function returns ``True`` if the word graphs *x* and *y* are equal
on the range of nodes from *first* to *last* ; and ``False`` otherwise.

The word graphs *x* and *y* are equal at a node *s* if:

*  the out-degrees of *x* and *y* coincide;
*  the edges with source ``s`` and label ``a`` have equal targets in *x*
   and *y* for every label ``a``.

:param x: the first word graph for comparison.
:type x: WordGraph

:param y: the second word graph for comparison.
:type y: WordGraph

:param first: the first node in the range.
:type first: int

:param last: the last node in the range plus 1.
:type last: int

:returns:
    Whether or not the word graphs are equal at the specified range of nodes.
:rtype: bool

:raises LibsemigroupsError:
    if *first* is not a node in *x* or not a node in *y* ; or if ``last - 1``
    is not a node in *x* or not a node in *y*.

.. note::
    It is also possible to compare two entire word graphs using ``==``.)pbdoc");

    m.def(
        "word_graph_follow_path",
        [](WordGraph_ const& wg, node_type from, word_type const& path) {
          return word_graph::follow_path(wg, from, path);
        },
        py::arg("wg"),
        py::arg("from"),
        py::arg("path"),
        R"pbdoc(
:sig=(wg: WordGraph, source: int, path: list[int]) -> int | Undefined:
Find the node that a path starting at a given node leads to (if any).

This function attempts to follow the path in the word graph *wg* starting
at the node *source* labelled by the word *path*. If this path exists,
then the last node on that path is returned. If this path does not exist,
then :any:`UNDEFINED` is returned.

:param wg: a word graph.
:type wg: WordGraph

:param source: the starting node.
:type source: int

:param path: the path to follow.
:type path: list[int]

:returns: The last node on the path or :any:`UNDEFINED`.
:rtype: int | Undefined

:raises LibsemigroupsError:
    if *source* is not a node in the word graph or *path* contains a value that
    is not an edge-label.

:complexity: Linear in the length of *path*.)pbdoc");

    m.def(
        "word_graph_is_acyclic",
        [](WordGraph_ const& wg) { return word_graph::is_acyclic(wg); },
        py::arg("wg"),
        R"pbdoc(
:sig=(wg: WordGraph) -> bool:
Check if a word graph is acyclic.

This function returns ``True`` if the word graph *wg* is acyclic and
``False`` otherwise. A word graph is acyclic if every directed cycle in the
word graph is trivial.

:param wg: the WordGraph object to check.
:type wg: WordGraph

:returns: Whether or not the word graph is acyclic.
:rtype: bool

:complexity:
    :math:`O(m + n)` where :math:`m` is the number of nodes in the
    :any:`WordGraph` *wg* and :math:`n` is the number of edges. Note that for
    :any:`WordGraph` objects the number of edges is always at most :math:`mk`
    where :math:`k` is the :any:`WordGraph.out_degree`.

.. doctest::

    >>> from libsemigroups_pybind11 import WordGraph, word_graph
    >>> wg = WordGraph()
    >>> wg.add_nodes(2)
    <WordGraph with 2 nodes, 0 edges, & out-degree 0>
    >>> wg.add_to_out_degree(1)
    <WordGraph with 2 nodes, 0 edges, & out-degree 1>
    >>> wg.target(0, 0, 1)
    <WordGraph with 2 nodes, 1 edges, & out-degree 1>
    >>> wg.target(1, 0, 0)
    <WordGraph with 2 nodes, 2 edges, & out-degree 1>
    >>> word_graph.is_acyclic(wg)
    False)pbdoc");

    m.def(
        "word_graph_is_acyclic",
        [](WordGraph_ const& wg, node_type source) {
          return word_graph::is_acyclic(wg, source);
        },
        py::arg("wg"),
        py::arg("source"),
        R"pbdoc(
:sig=(wg: WordGraph, source: int) -> bool:
Check if the word graph induced by the nodes reachable from a source node is
acyclic.

This function returns ``True`` if the word graph consisting of the nodes
reachable from *source* in the word graph *wg* is acyclic and ``False``
if not. A word graph is *acyclic* if every directed cycle in the word graph is
trivial.

:param wg: the WordGraph object to check.
:type wg: WordGraph

:param source: the source node.
:type source: int

:returns:
    Whether the induced subgraph of *wg* consisting of those nodes
    reachable from *source* is acyclic or not.
:rtype: bool

:complexity:
    :math:`O(m + n)` where :math:`m` is the number of nodes in the
    :any:`WordGraph` *wg* and :math:`n` is the number of edges. Note that for
    :any:`WordGraph` objects the number of edges is always at most :math:`mk`
    where :math:`k` is the :any:`WordGraph.out_degree`.

.. doctest::

   >>> from libsemigroups_pybind11 import WordGraph, word_graph
   >>> wg = WordGraph()
   >>> wg.add_nodes(4).add_to_out_degree(1)
   <WordGraph with 4 nodes, 0 edges, & out-degree 1>
   >>> wg.target(0, 0, 1).target(1, 0, 0).target(2, 0, 3)
   <WordGraph with 4 nodes, 3 edges, & out-degree 1>
   >>> word_graph.is_acyclic(wg)
   True
   >>> word_graph.is_acyclic(wg, 0)
   True
   >>> word_graph.is_acyclic(wg, 1)
   True
   >>> word_graph.is_acyclic(wg, 2)
   True
   >>> word_graph.is_acyclic(wg, 3)
   True)pbdoc");

    m.def(
        "word_graph_is_acyclic",
        [](WordGraph_ const& wg, node_type source, node_type target) {
          return word_graph::is_acyclic(wg, source, target);
        },
        py::arg("wg"),
        py::arg("source"),
        py::arg("target"),
        R"pbdoc(
:sig=(wg: WordGraph, source: int, target: int) -> bool:
Check if the word graph induced by the nodes reachable from a source node and
from which a target node can be reached is acyclic.

This function returns ``True`` if the word graph consisting of the nodes
reachable from *source* and from which *target* is reachable, in the word
graph *wg*, is acyclic; and ``False`` if not. A word graph is *acyclic* if
every directed cycle of the word graph is trivial.

:param wg: the WordGraph object to check.
:type wg: WordGraph

:param source: the source node.
:type source: int

:param target: the target node.
:type target: int

:returns: Whether or not the subgraph is acyclic.
:rtype: bool

:complexity:
    :math:`O(m + n)` where :math:`m` is the number of nodes in the
    :any:`WordGraph` *wg* and :math:`n` is the number of edges. Note that for
    :any:`WordGraph` objects the number of edges is always at most :math:`mk`
    where :math:`k` is the :any:`WordGraph.out_degree`.)pbdoc");

    m.def(
        "word_graph_is_compatible",
        [](WordGraph_ const& wg,
           node_type         first_node,
           node_type         last_node,
           word_type const&  lhs,
           word_type const&  rhs) {
          return word_graph::is_compatible(wg,
                                           wg.cbegin_nodes() + first_node,
                                           wg.cbegin_nodes() + last_node,
                                           lhs,
                                           rhs);
        },
        py::arg("wg"),
        py::arg("first_node"),
        py::arg("last_node"),
        py::arg("lhs"),
        py::arg("rhs"),
        R"pbdoc(
:sig=(wg: WordGraph, first_node: int, last_node: int, lhs: list[int], rhs: list[int]) -> bool:
Check if a word graph is compatible with some relations at a range of nodes.

This function returns ``True`` if the word graph *wg* is compatible with the
word *lhs* and *rhs* with source node equal to every node in the range from
*first_node* to *last_node*. This means that the paths with given sources that
are labelled by *lhs* lead to the same nodes as the paths labelled by *rhs*.

:param wg: the word graph.
:type wg: WordGraph

:param first_node: the first node.
:type first_node: int

:param last_node: one more than the last node.
:type last_node: int

:param lhs: the first rule.
:type lhs: list[int]

:param rhs: the second rule.
:type rhs: list[int]

:returns:
  Whether or not the word graph is compatible with the given rules at each one
  of the given nodes.
:rtype: bool

:raises LibsemigroupsError:
   if any of the nodes in the range between *first_node* and *last_node*
   does not belong to *wg* (i.e. is greater than or equal to
   :any:`WordGraph.number_of_nodes`).

:raises LibsemigroupsError:
   if *lhs* or *rhs* contains an invalid label (i.e. one greater than or equal
   to :any:`WordGraph.out_degree`).)pbdoc");

    m.def(
        "word_graph_is_complete",
        [](WordGraph_ const& wg) { return word_graph::is_complete(wg); },
        py::arg("wg"),
        R"pbdoc(
:sig=(wg: WordGraph) -> bool:
Check if every node has exactly :any:`WordGraph.out_degree` out-edges.

This function returns ``True`` if the word graph *wg* is complete, meaning that
every node is the source of an edge with every possible label.

:param wg: the word graph.
:type wg: WordGraph

:returns: Whether or not the word graph is complete.
:rtype: bool

:complexity:
  :math:`O(mn)` where ``m`` is :any:`WordGraph.number_of_nodes` and ``n`` is
  :any:`WordGraph.out_degree`.)pbdoc");

    m.def(
        "word_graph_is_complete",
        [](WordGraph_ const& wg, node_type first_node, node_type last_node) {
          return word_graph::is_complete(wg,
                                         wg.cbegin_nodes() + first_node,
                                         wg.cbegin_nodes() + last_node);
        },
        py::arg("wg"),
        py::arg("first_node"),
        py::arg("last_node"),
        R"pbdoc(
:sig=(wg: WordGraph, first_node: int, last_node: int) -> bool:
Check if every node in a range has exactly :any:`WordGraph.out_degree`
out-edges.

This function returns ``True`` if every node in the range defined by
*first_node* and *last_node* is complete, meaning that every such node is
the source of an edge with every possible label.

:param wg: the word graph.
:type wg: WordGraph

:param first_node: the first node.
:type first_node: int

:param last_node: one more than the last node.
:type last_node: int

:returns:
  Whether or not the word graph is complete on the given range of
  nodes.
:rtype: bool

:raises LibsemigroupsError:
  if any node in the range defined by *first_node* and *last_node* is not a
  node of *wg*.

:complexity:
  :math:`O(mn)` where ``m`` is the number of nodes in the range and ``n`` is
  :any:`WordGraph.out_degree`.
)pbdoc");

    m.def(
        "word_graph_is_connected",
        [](WordGraph_& wg) { return word_graph::is_connected(wg); },
        py::arg("wg"),
        R"pbdoc(
:sig=(wg: WordGraph) -> bool:
Check if a word graph is connected.

This function returns ``True`` if the word graph *wg* is connected and
``False`` if it is not. A word graph is *connected* if for every pair of
nodes ``s`` and ``t`` in the graph there exists a sequence :math:`u_0 = s,
\ldots, u_{n}= t` for some :math:`n\in \mathbb{N}` such that for every
:math:`i` there exists a label ``a`` such that :math:`(u_i, a, u_{i + 1})` or
:math:`(u_{i + 1}, a, u_i)` is an edge in the graph.

:param wg: the word graph.
:type wg: WordGraph

:returns: Whether or not the word graph is connected.
:rtype: bool

:raises LibsemigroupsError:
  if any target in *wg* is out of bounds, i.e. if any target ``t`` is not
  equal to :any:`UNDEFINED` and not in the nodes of *wg*.)pbdoc");

    m.def(
        "word_graph_is_reachable",
        [](WordGraph_ const& wg, node_type source, node_type target) {
          return word_graph::is_reachable(wg, source, target);
        },
        py::arg("wg"),
        py::arg("source"),
        py::arg("target"),
        R"pbdoc(
:sig=(wg: WordGraph, source: int, target: int) -> bool:
Check if there is a path from one node to another.

This function returns ``True`` if there is a path from the node *source* to
the node *target* in the word graph *wg*.

:param wg: the WordGraph object to check.
:type wg: WordGraph

:param source: the source node.
:type source: int

:param target: the source node.
:type target: int

:returns:
  Whether or not the node *target* is reachable from the node
  *source* in the word graph *wg*.
:rtype: bool

:raises LibsemigroupsError: if *source* or *target* is out of bounds.
:raises LibsemigroupsError: if any target in *wg* is out of bounds.

:complexity:
  :math:`O(m + n)` where :math:`m` is the number of nodes in the
  :any:`WordGraph` *wg* and :math:`n` is the number of edges. Note that for
  :any:`WordGraph` objects the number of edges is always at most :math:`mk`
  where :math:`k` is the :any:`WordGraph.out_degree`.)pbdoc");

    m.def(
        "word_graph_is_strictly_cyclic",
        [](WordGraph_ const& wg) { return word_graph::is_strictly_cyclic(wg); },
        py::arg("wg"),
        R"pbdoc(
:sig=(wg: WordGraph) -> bool:
Check if every node is reachable from some node.

This function returns ``True`` if there exists a node in *wg* from which
every other node is reachable; and ``False`` otherwise. A word graph is
*strictly cyclic* if there exists a node :math:`v` from which every node is
reachable (including :math:`v` ). There must be a path of length at least
:math:`1` from the original node :math:`v` to itself (i.e. :math:`v` is not
considered to be reachable from itself by default).

:param wg: the WordGraph object to check.
:type wg: WordGraph

:returns: Whether or not every node is reachable from one specific node.
:rtype: bool

:raises LibsemigroupsError:  if any target in *wg* is out of bounds.

:complexity:
    :math:`O(m + n)` where :math:`m` is the number of nodes in the
    :any:`WordGraph` *wg* and :math:`n` is the number of edges. Note that for
    :any:`WordGraph` objects the number of edges is always at most :math:`mk`
    where :math:`k` is the :any:`WordGraph.out_degree`.

.. doctest::

  >>> from libsemigroups_pybind11 import WordGraph, word_graph
  >>> wg = WordGraph(5,  [[0,  0],  [1,  1],  [2],  [3,  3]])
  >>> word_graph.is_strictly_cyclic(wg)
  False)pbdoc");

    m.def(
        "word_graph_last_node_on_path",
        [](WordGraph_ const& wg, node_type source, word_type const& w) {
          auto result = word_graph::last_node_on_path(wg, source, w);
          return std::pair(result.first,
                           std::distance(w.cbegin(), result.second));
        },
        py::arg("wg"),
        py::arg("source"),
        py::arg("w"),
        R"pbdoc(
:sig=(wg: WordGraph, source: int, w: list[int]) -> tuple[int, int]:
Returns the last node on the path labelled by a word and the index of the
position in the word reached.

:param wg: a word graph.
:type wg: WordGraph

:param source: the source node.
:type source: int

:param w: the word.
:type w: list[int]

:returns:
  A pair consisting of the last node reached and the index of
  the last letter in the word labelling an edge.
:rtype: tuple[int, int]

:complexity: At worst the length of *w*.)pbdoc");

    m.def(
        "word_graph_nodes_reachable_from",
        [](WordGraph_ const& wg, node_type source) {
          return word_graph::nodes_reachable_from(wg, source);
        },
        py::arg("wg"),
        py::arg("source"),
        R"pbdoc(
:sig=(wg: WordGraph, source: int) -> set[int]:
Returns the set of nodes reachable from a given node in a word graph.

This function returns a set consisting of all the nodes in the word graph
*wg* that are reachable from *source*.

:param wg: the word graph.
:type wg: WordGraph

:param source: the source node.
:type source: int

:returns:
  A set consisting of all the nodes in the word graph
  *wg* that are reachable from *source*.
:rtype: set[int]

:raises LibsemigroupsError:
  if *source* is out of bounds (greater than or equal to
  :any:`WordGraph.number_of_nodes`).)pbdoc");

    m.def(
        "word_graph_number_of_nodes_reachable_from",
        [](WordGraph_ const& wg, node_type source) {
          return word_graph::number_of_nodes_reachable_from(wg, source);
        },
        py::arg("wg"),
        py::arg("source"),
        R"pbdoc(
:sig=(wg: WordGraph, source: int) -> int:
Returns the number of nodes reachable from a given node in a word graph.

This function returns the number of nodes in the word graph *wg* that are
reachable from *source*.

:param wg: the word graph.
:type wg: WordGraph

:param source: the source node.
:type source: int

:returns:
  The number of nodes in the word graph *wg* that are reachable from
  *source*.
:rtype: int

:raises LibsemigroupsError:
  if *source* is out of bounds (greater than or equal to
  :any:`WordGraph.number_of_nodes`).)pbdoc");

    m.def(
        "word_graph_random_acyclic",
        [](size_t non, size_t od) {
          return word_graph::random_acyclic<node_type>(non, od);
        },
        py::arg("number_of_nodes"),
        py::arg("out_degree"),
        R"pbdoc(
:sig=(number_of_nodes: int, out_degree: int) -> WordGraph:
Construct a random acyclic word graph from number of nodes and
out-degree.

This function constructs a random acyclic word graph with *number_of_nodes*
nodes and out-degree *out_degree*. This function implements the Markov chain
algorithm given in :cite:`Carnino2011`.

:param number_of_nodes: the number of nodes.
:type number_of_nodes: int

:param out_degree: the out-degree of every node.
:type out_degree: int

:returns: A random acyclic word graph.
:rtype: WordGraph

:raises LibsemigroupsError: if  *number_of_nodes* is less than ``2``
:raises LibsemigroupsError: if  *out_degree* is less than ``2``

:complexity: At least :math:`O(mn)` where ``m`` is the number of nodes, and ``n`` is the out-degree of the word graph.)pbdoc");

    m.def(
        "word_graph_spanning_tree",
        [](WordGraph_ const& wg, node_type root) {
          return word_graph::spanning_tree(wg, root);
        },
        py::arg("wg"),
        py::arg("root"),
        R"pbdoc(
:sig=(wg: WordGraph, root: int) -> Forest:
Returns a :any:`Forest` containing a spanning tree of the nodes reachable from
a given node in a word graph.

This function returns a :any:`Forest` containing a spanning tree of the
nodes reachable from *root* in the word graph *wg*.

:param wg: the word graph.
:type wg: WordGraph

:param root: the source node.
:type root: int

:returns: A :any:`Forest` object containing a spanning tree.
:rtype: Forest

:raises LibsemigroupsError:
  if *root* is out of bounds, i.e. greater than or equal to
  :any:`WordGraph.number_of_nodes`.)pbdoc");

    m.def(
        "word_graph_spanning_tree",
        [](WordGraph_ const& wg, node_type root, Forest& f) {
          return word_graph::spanning_tree(wg, root, f);
        },
        py::arg("wg"),
        py::arg("root"),
        py::arg("f"),
        R"pbdoc(
:sig=(wg: WordGraph, root: int, f: Forest) -> None:
Replace the contents of a Forest by a spanning tree of the nodes reachable
from a given node in a word graph.

This function replaces the content of the :any:`Forest` *f* with a spanning
tree of the nodes reachable from *root* in the word graph *wg*.

:param wg: the word graph.
:type wg: WordGraph

:param root: the source node.
:type root: int

:param f: the Forest object to hold the result.
:type f: Forest

:raises LibsemigroupsError:
  if *root* is out of bounds, i.e. greater than or equal to
  :any:`WordGraph.number_of_nodes`.)pbdoc");

    m.def(
        "word_graph_standardize",
        [](WordGraph_& wg, Forest& f, Order val) {
          return word_graph::standardize(wg, f, val);
        },
        py::arg("wg"),
        py::arg("f"),
        py::arg("val"),
        R"pbdoc(
:sig=(wg: WordGraph, f: Forest, val: Order) -> bool:
Standardizes a word graph in-place.

This function standardizes the word graph *wg* according to the reduction
order specified by *val*, and replaces the contents of the :any:`Forest`
*f* with a spanning tree rooted at ``0`` for the node reachable from
``0``. The spanning tree corresponds to the order *val*.

:param wg: the word graph.
:type wg: WordGraph

:param f: the Forest object to store the spanning tree.
:type f: Forest

:param val: the order to use for standardization.
:type val: Order

:returns:
  This function returns ``True`` if the word graph *wg* is
  modified by this function (i.e. it was not standardized already), and
  ``False`` otherwise.
:rtype: bool
)pbdoc");
    m.def(
        "word_graph_standardize",
        [](WordGraph_& wg, Order val) {
          return word_graph::standardize(wg, val);
        },
        py::arg("wg"),
        py::arg("val") = Order::shortlex,
        R"pbdoc(
:sig=(wg: WordGraph, val: Order = Order.shortlex) -> tuple[bool, Forest]:
Standardizes a word graph in-place.

This function standardizes the word graph *wg* according to the reduction
order specified by *val*, and returns a :any:`Forest` object containing
a spanning tree rooted at ``0`` for the node reachable from ``0``. The
spanning tree corresponds to the order *val*.

:param wg: the word graph.
:type wg: WordGraph

:param val:
    the order to use for standardization (default: :any:`Order.shortlex`).
:type val: Order

:returns:
  A tuple whose first entry is ``True`` if the word graph *wg* is modified by
  this function (i.e. it was not standardized already), and ``False``
  otherwise. The second entry is a :any:`Forest` object containing a spanning
  tree for *wg*.
:rtype: tuple[bool, Forest]
)pbdoc");

    m.def(
        "word_graph_topological_sort",
        [](WordGraph_ const& wg) { return word_graph::topological_sort(wg); },
        py::arg("wg"),
        R"pbdoc(
:sig=(wg: WordGraph) -> list[int]:
Returns the nodes of the word graph in topological order (see below) if
possible.

If it is not empty, the returned list has the property that if an edge
from a node ``n`` points to a node ``m``, then ``m`` occurs before ``n``
in the list.

:param wg: the word graph.
:type wg: WordGraph

:returns:
  A list of the nodes of *wg* in topological order (if possible) and is
  otherwise empty.
:rtype: list[int]

:complexity:
  :math:`O(m + n)` where :math:`m` is the number of nodes in the
  :any:`WordGraph` *wg* and :math:`n` is the number of edges. Note that for
  :any:`WordGraph` objects the number of edges is always at most :math:`mk`
  where :math:`k` is the :any:`WordGraph.out_degree`.)pbdoc");

    m.def(
        "word_graph_topological_sort",
        [](WordGraph_ const& wg, node_type source) {
          return word_graph::topological_sort(wg, source);
        },
        py::arg("wg"),
        py::arg("source"),
        R"pbdoc(
:sig=(wg: WordGraph, source: int) -> list[int]:
Returns the nodes of the word graph reachable from a given node in
topological order (see below) if possible.

If it is not empty, the returned list has the property that if an edge from a
node ``n`` points to a node ``m``, then ``m`` occurs before ``n`` in the
list, and the last item in the list is *source*.

:param wg: the WordGraph object to check.
:type wg: WordGraph

:param source: the source node.
:type source: int

:returns:
  A list of the nodes reachable from *source* in *wg* in topological order (if
  possible) and is otherwise empty.
:rtype: list[int]

:complexity:
  At worst :math:`O(m + n)` where :math:`m` is the number of
  nodes in the subword graph of those nodes reachable from *source* and
  :math:`n` is the number of edges.)pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // Meeter
    ////////////////////////////////////////////////////////////////////////

    py::class_<Meeter> meeter(m,
                              "Meeter",
                              R"pbdoc(
Class for taking meets of word graphs.

This class exists for its call operators which can be used to find the meet of
two word graphs with the same :any:`WordGraph.out_degree`. This class
implements the same algorithm as that used for computing a finite state
automata recognising the intersection of the languages accepted by two given
automata. The input word graphs need not be complete, and the root nodes can
also be specified.)pbdoc");
    meeter.def("__repr__",
               [](Meeter const& x) { return to_human_readable_repr(x); });
    meeter.def("__copy__", [](Meeter const& wg) { return Meeter(wg); });
    meeter.def(
        "copy",
        [](Meeter const& wg) { return Meeter(wg); },
        R"pbdoc(
Copy a :any:`Meeter` object.

:returns: A copy.
:rtype: Meeter
)pbdoc");
    meeter.def(py::init<>(), R"pbdoc(
Default constructor.
)pbdoc");
    meeter.def(
        "is_subrelation",
        [](Meeter& self, WordGraph_ const& x, WordGraph_ const& y) {
          return self.is_subrelation(x, y);
        },
        py::arg("x"),
        py::arg("y"),
        R"pbdoc(
Check if the language accepted by one word graph is contained in that
defined by another word graph.

This function returns ``True`` if the language accepted by *x* with
initial node ``0`` and accept state every node, is a subset of the
corresponding language in *y*.

:param x: the word graph whose language we are checking might be a subset.
:type x: WordGraph

:param y: the word graph whose language we are checking might be a superset.
:type y: WordGraph

:returns: Whether or not *x* is a subrelation of *y*.
:rtype: bool

:raises LibsemigroupsError:  if *x* has no nodes;
:raises LibsemigroupsError:  if *y* has no nodes;
:raises LibsemigroupsError:  if ``x.out_degree() != y.out_degree()``.)pbdoc");
    meeter.def(
        "is_subrelation",
        [](Meeter&           self,
           WordGraph_ const& x,
           node_type         xroot,
           WordGraph_ const& y,
           node_type yroot) { return self.is_subrelation(x, xroot, y, yroot); },
        py::arg("x"),
        py::arg("xroot"),
        py::arg("y"),
        py::arg("yroot"),
        R"pbdoc(
:sig=(self: Meeter, x: WordGraph, xroot: int, y: WordGraph, yroot: int) -> bool:

Check if the language accepted by one word graph is contained in that
defined by another word graph.

This function returns ``True`` if the language accepted by *x* with
initial node *xroot* and accept state every node, is a subset of the
corresponding language in *y*.

:param x: the word graph whose language we are checking might be a subset.
:type x: WordGraph

:param xroot: the node to use as the initial state in x.
:type xroot: int

:param y: the word graph whose language we are checking might be a superset.
:type y: WordGraph

:param yroot: the node to use as an initial state in y.
:type yroot: int

:returns: Whether or not *x* is a subrelation of *y*.
:rtype: bool

:raises LibsemigroupsError: if *x* has no nodes;
:raises LibsemigroupsError: if *y* has no nodes;
:raises LibsemigroupsError: if *xroot* isn't a node in *x*;
:raises LibsemigroupsError: if *yroot* isn't a node in *y*;
:raises LibsemigroupsError: if ``x.out_degree() != y.out_degree()``.)pbdoc");

    // TODO(1): Check the return types here (and in libsemigroups too). It seems
    // as though we are returning things in a void function. JDE got quite
    // confused.
    meeter.def(
        "__call__",
        [](Meeter&           self,
           WordGraph_&       xy,
           WordGraph_ const& x,
           node_type         xroot,
           WordGraph_ const& y,
           node_type yroot) { return self.operator()(xy, x, xroot, y, yroot); },
        py::arg("xy"),
        py::arg("x"),
        py::arg("xroot"),
        py::arg("y"),
        py::arg("yroot"),
        R"pbdoc(
:sig=(self: Meeter, xy: WordGraph, x: WordGraph, xroot: int, y: WordGraph, yroot: int) -> None:

Replace the contents of a word graph with the meet of two given word
graphs with respect to given root vertices.

This function replaces the contents of the word graph *xy* with the
meet of the word graphs *x* and *y*.

:param xy: the word graph to store the result.
:type xy: WordGraph

:param x: the first word graph to meet.
:type x: WordGraph

:param xroot: the node to use as a root in x.
:type xroot: int

:param y: the second word graph to meet.
:type y: WordGraph

:param yroot: the node to use as a root in y.
:type yroot: int

:raises LibsemigroupsError: if *x* has no nodes;
:raises LibsemigroupsError: if *y* has no nodes;
:raises LibsemigroupsError: if *xroot* isn't a node in *x*;
:raises LibsemigroupsError: if *yroot* isn't a node in *y*;
:raises LibsemigroupsError: if ``x.out_degree() != y.out_degree()``.)pbdoc");

    meeter.def(
        "__call__",
        [](Meeter&           self,
           WordGraph_&       xy,
           WordGraph_ const& x,
           WordGraph_ const& y) { return self.operator()(xy, x, y); },
        py::arg("xy"),
        py::arg("x"),
        py::arg("y"),
        R"pbdoc(
Replace the contents of a word graph with the meet of two given word graphs.

This function replaces the contents of the word graph *xy* with the
meet of the word graphs *x* and *y*.

:param xy: the word graph to store the result.
:type xy: WordGraph

:param x: the first word graph to meet.
:type x: WordGraph

:param y: the second word graph to meet.
:type y: WordGraph

:raises LibsemigroupsError: if *x* has no nodes;
:raises LibsemigroupsError: if *y* has no nodes;
:raises LibsemigroupsError: if ``x.out_degree() != y.out_degree()``.)pbdoc");

    meeter.def(
        "__call__",
        [](Meeter& self, WordGraph_ const& x, WordGraph_ const& y) {
          return self.operator()(x, y);
        },
        py::arg("x"),
        py::arg("y"),
        R"pbdoc(
Returns a word graph containing the meet of two given word graphs.

This function returns a word graph containing the meet of the word graphs *x*
and *y*.

:param x: the first word graph to meet.
:type x: WordGraph

:param y: the second word graph to meet.
:type y: WordGraph

:returns: The meet of *x* and *y*
:rtype: WordGraph

:raises LibsemigroupsError: if *x* has no nodes;
:raises LibsemigroupsError: if *y* has no nodes;
:raises LibsemigroupsError: if ``x.out_degree() != y.out_degree()``.)pbdoc");

    meeter.def(
        "__call__",
        [](Meeter&           self,
           WordGraph_ const& x,
           node_type         xroot,
           WordGraph_ const& y,
           node_type yroot) { return self.operator()(x, xroot, y, yroot); },
        py::arg("x"),
        py::arg("xroot"),
        py::arg("y"),
        py::arg("yroot"),
        R"pbdoc(
:sig=(self: Meeter, x: WordGraph, xroot: int, y: WordGraph, yroot: int) -> WordGraph:

Returns a word graph containing the meet of two given word graphs with respect
to given root vertices.

This function returns a word graph containing the meet of the word graphs *x*
and *y*.

:param x: the first word graph to meet.
:type x: WordGraph

:param xroot: the node to use as a root in x.
:type xroot: int

:param y: the second word graph to meet.
:type y: WordGraph

:param yroot: the node to use as a root in y.
:type yroot: int

:returns: The meet of *x* and *y*.
:rtype: WordGraph

:raises LibsemigroupsError: if *x* has no nodes;
:raises LibsemigroupsError: if *y* has no nodes;
:raises LibsemigroupsError: if *xroot* isn't a node in *x*;
:raises LibsemigroupsError: if *yroot* isn't a node in *y*;
:raises LibsemigroupsError: if  ``x.out_degree() != y.out_degree()``.)pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // Joiner
    ////////////////////////////////////////////////////////////////////////

    py::class_<Joiner> joiner(m,
                              "Joiner",
                              R"pbdoc(
Class for taking joins of word graphs.

This class exists for its call operators which can be used to find the join
of two word graphs with the same :any:`WordGraph.out_degree`. This class
implements the Hopcroft-Karp algorithm :cite:`Hop71` for computing a finite
state automata recognising the union of the languages accepted by two given
automata. The input word graphs need not be complete, and the root nodes can
also be specified.)pbdoc");
    joiner.def("__repr__", [](Joiner const& joiner) {
      return to_human_readable_repr(joiner);
    });
    joiner.def(py::init<>(), R"pbdoc(
    Default constructor.)pbdoc");
    joiner.def("__copy__", [](Joiner const& wg) { return Joiner(wg); });
    joiner.def(
        "copy",
        [](Joiner const& wg) { return Joiner(wg); },
        R"pbdoc(
Copy a :any:`Joiner` object.

:returns: A copy.
:rtype: Joiner
)pbdoc");
    joiner.def(
        "is_subrelation",
        [](Joiner& self, WordGraph_ const& x, WordGraph_ const& y) {
          return self.is_subrelation(x, y);
        },
        py::arg("x"),
        py::arg("y"),
        R"pbdoc(
Check if the language accepted by one word graph is contained in that
defined by another word graph.

This function returns ``True`` if the language accepted by *x* with
initial node ``0`` and accept state every node, is a subset of the
corresponding language in *y*.

:param x: the word graph whose language we are checking might be a subset.
:type x: WordGraph

:param y: the word graph whose language we are checking might be a superset.
:type y: WordGraph

:returns: Whether or not *x* is a subrelation of *y*.
:rtype: bool

:raises LibsemigroupsError:  if *x* has no nodes;
:raises LibsemigroupsError:  if *y* has no nodes;
:raises LibsemigroupsError:  if ``x.out_degree() != y.out_degree()``.
)pbdoc");
    joiner.def(
        "is_subrelation",
        [](Joiner&           self,
           WordGraph_ const& x,
           node_type         xroot,
           WordGraph_ const& y,
           node_type yroot) { return self.is_subrelation(x, xroot, y, yroot); },
        py::arg("x"),
        py::arg("xroot"),
        py::arg("y"),
        py::arg("yroot"),
        R"pbdoc(
:sig=(self: Joiner, x: WordGraph, xroot: int, y: WordGraph, yroot: int) -> bool:

Check if the language accepted by one word graph is contained in that
defined by another word graph.

This function returns ``True`` if the language accepted by *x* with
initial node *xroot* and accept state every node, is a subset of the
corresponding language in *y*.

:param x: the word graph whose language we are checking might be a subset.
:type x: WordGraph

:param xroot: the node to use as the initial state in x.
:type xroot: int

:param y: the word graph whose language we are checking might be a superset.
:type y: WordGraph

:param yroot: the node to use as an initial state in y.
:type yroot: int

:returns: Whether or not *x* is a subrelation of *y*.
:rtype: bool

:raises LibsemigroupsError: if *x* has no nodes;
:raises LibsemigroupsError: if *y* has no nodes;
:raises LibsemigroupsError: if *xroot* isn't a node in *x*;
:raises LibsemigroupsError: if *yroot* isn't a node in *y*;
:raises LibsemigroupsError: if  ``x.out_degree() != y.out_degree()``.)pbdoc");

    joiner.def(
        "__call__",
        [](Joiner&           self,
           WordGraph_&       xy,
           WordGraph_ const& x,
           node_type         xroot,
           WordGraph_ const& y,
           node_type yroot) { return self.operator()(xy, x, xroot, y, yroot); },
        py::arg("xy"),
        py::arg("x"),
        py::arg("xroot"),
        py::arg("y"),
        py::arg("yroot"),
        R"pbdoc(
:sig=(self: Joiner, xy: WordGraph, x: WordGraph, xroot: int, y: WordGraph, yroot: int) -> None:

Replace the contents of a word graph with the join of two given word
graphs with respect to given root vertices.

This function replaces the contents of the word graph *xy* with the
join of the word graphs *x* and *y*.

:param xy: the word graph to store the result.
:type xy: WordGraph

:param x: the first word graph to join.
:type x: WordGraph

:param xroot: the node to use as a root in x.
:type xroot: int

:param y: the second word graph to join.
:type y: WordGraph

:param yroot: the node to use as a root in y.
:type yroot: int

:raises LibsemigroupsError: if *x* has no nodes;
:raises LibsemigroupsError: if *y* has no nodes;
:raises LibsemigroupsError: if *xroot* isn't a node in *x*;
:raises LibsemigroupsError: if *yroot* isn't a node in *y*;
:raises LibsemigroupsError: if  ``x.out_degree() != y.out_degree()``.)pbdoc");

    joiner.def(
        "__call__",
        [](Joiner&           self,
           WordGraph_&       xy,
           WordGraph_ const& x,
           WordGraph_ const& y) { return self.operator()(xy, x, y); },
        py::arg("xy"),
        py::arg("x"),
        py::arg("y"),
        R"pbdoc(
Replace the contents of a word graph with the join of two given word graphs.

This function replaces the contents of the word graph *xy* with the
join of the word graphs *x* and *y*.

:param xy: the word graph to store the result.
:type xy: WordGraph

:param x: the first word graph to join.
:type x: WordGraph

:param y: the second word graph to join.
:type y: WordGraph

:raises LibsemigroupsError: if *x* has no nodes;
:raises LibsemigroupsError: if *y* has no nodes;
:raises LibsemigroupsError: if ``x.out_degree() != y.out_degree()``.)pbdoc");

    joiner.def(
        "__call__",
        [](Joiner&           self,
           WordGraph_ const& x,
           node_type         xroot,
           WordGraph_ const& y,
           node_type yroot) { return self.operator()(x, xroot, y, yroot); },
        py::arg("x"),
        py::arg("xroot"),
        py::arg("y"),
        py::arg("yroot"),
        R"pbdoc(
:sig=(self: Joiner, x: WordGraph, xroot: int, y: WordGraph, yroot: int) -> WordGraph:

Returns a word graph containing the join of two given word graphs with respect
to given root vertices.

This function returns a word graph containing the join of the word graphs *x*
and *y*.

:param x: the first word graph to join.
:type x: WordGraph

:param xroot: the node to use as a root in x.
:type xroot: int

:param y: the second word graph to join.
:type y: WordGraph

:param yroot: the node to use as a root in y.
:type yroot: int

:returns: The join of *x* an *y*.
:rtype: WordGraph

:raises LibsemigroupsError: if *x* has no nodes;
:raises LibsemigroupsError: if *y* has no nodes;
:raises LibsemigroupsError: if *xroot* isn't a node in *x*;
:raises LibsemigroupsError: if *yroot* isn't a node in *y*;
:raises LibsemigroupsError: if  ``x.out_degree() != y.out_degree()``.)pbdoc");

    joiner.def(
        "__call__",
        [](Joiner& self, WordGraph_ const& x, WordGraph_ const& y) {
          return self.operator()(x, y);
        },
        py::arg("x"),
        py::arg("y"),
        R"pbdoc(
Returns a word graph containing the join of two given word graphs.

This function returns a word graph containing the join of the word graphs *x*
and *y*.

:param x: the first word graph to join.
:type x: WordGraph

:param y: the second word graph to join.
:type y: WordGraph

:returns: The join of *x* and *y*.
:rtype: WordGraph

:raises LibsemigroupsError: if *x* has no nodes;
:raises LibsemigroupsError: if *y* has no nodes;
:raises LibsemigroupsError: if ``x.out_degree() != y.out_degree()``.)pbdoc");
  }
}  // namespace libsemigroups
