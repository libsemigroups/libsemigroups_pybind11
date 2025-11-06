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

// C std headers....
#include <cstddef>  // for size_t

// C++ stl headers....
#include <initializer_list>  // for initializer_list

// libsemigroups....
#include <libsemigroups/forest.hpp>  // for Forest
#include <libsemigroups/ranges.hpp>  // for rx::to_vector

// pybind11....
#include <pybind11/operators.h>  // for py::operator
#include <pybind11/pybind11.h>   // for class_, init, make_iterator, module
#include <pybind11/stl.h>        // for conversion of C++ to py types

// libsemigroups_pybind11....
#include "main.hpp"  // for init_forest

namespace libsemigroups {
  namespace py = pybind11;
  void init_forest(py::module& m) {
    using node_type = Forest::node_type;

    {
      py::class_<Forest> thing(m,
                               "Forest",
                               R"pbdoc(
This class represents the collection of spanning trees of the strongly
connected components of a word graph.
)pbdoc");

      thing.def("__repr__",
                [](Forest const& f) { return to_human_readable_repr(f); });
      thing.def("__copy__", [](Forest const& f) { return Forest(f); });
      thing.def(
          "copy",
          [](Forest const& f) { return Forest(f); },
          R"pbdoc(
Copy a :any:`Forest` object.

:returns: A copy.
:rtype: Forest
)pbdoc");

      thing.def(py::init<size_t>(), py::arg("n") = 0, R"pbdoc(
:sig=(self: Forest, n: int = 0) -> None:

Constructs a forest with *n* nodes.

Constructs a forest with *n* nodes, that is initialised so that the
:any:`Forest.parent` and :any:`Forest.label` of every node is :any:`UNDEFINED`.

:param n: the number of nodes, defaults to ``0``.
:type n: int
)pbdoc");
      thing.def(
          py::init([](std::vector<int_or_unsigned_constant<node_type>> const&
                          parents,
                      std::vector<int_or_unsigned_constant<node_type>> const&
                          labels) {
            using node_type = node_type;
            return make<Forest>(to_ints<node_type>(parents),
                                to_ints<node_type>(labels));
          }),
          py::arg("parents"),
          py::arg("labels"),
          R"pbdoc(
:sig=(self: Forest, parents:list[int | Undefined], labels:list[int | Undefined]) -> None:

Construct a :any:`Forest` from list of *parents* and *labels*.

:param parents: the list of parents of nodes.
:type parents: list[int | Undefined]
:param labels: the list of edge labels.
:type labels: list[int | Undefined]

:raises LibsemigroupsError:
  if  *parent* and *labels* have different sizes;
:raises LibsemigroupsError:
  *parent* and *labels* do not have the value :any:`UNDEFINED` in the same
  positions (these values indicate where the roots of the trees in the forest
  are located and so must coincide).
:raises LibsemigroupsError:
  :any:`set_parent_and_label` throws for ``parent[i]`` and ``edge_labels[i]``
  for any value of ``i``.
)pbdoc");

      thing.def("add_nodes",
                &Forest::add_nodes,
                py::arg("n"),
                R"pbdoc(
:sig=(self: Forest, n: int) -> None:

Add nodes to the :any:`Forest`.

This function adds *n* nodes to the forest, but no edges.

:param n: the number of nodes to add.
:type n: int

:complexity: At most linear in ``number_of_nodes() + n``.
)pbdoc");

      thing.def("empty",
                &Forest::empty,
                R"pbdoc(
Check if there are any nodes in the forest. This function returns
``True`` if there are ``0`` nodes in the forest, and ``False`` otherwise.

:returns:
   Whether or not the forest is empty.
:rtype:
   bool

:complexity:
   Constant.
)pbdoc");

      thing.def("init",
                &Forest::init,
                py::arg("n") = 0,
                R"pbdoc(
:sig=(self: Forest, n: int) -> Forest:

Reinitialize an existing :any:`Forest` object.

This function reinitializes an existing :any:`Forest` object so that it is in
the same state as if it had just be constructed as ``Forest(n)``.

:param n: the number of nodes, defaults to ``0``.
:type n: int

:returns: *self*.
:rtype: Forest
)pbdoc");

      thing.def(
          "label",
          [](Forest const& self,
             node_type     i) -> int_or_unsigned_constant<node_type> {
            if (self.label(i) != UNDEFINED) {
              return {self.label(i)};
            }
            return {UNDEFINED};
          },
          py::arg("i"),
          R"pbdoc(
:sig=(self: Forest, i: int) -> int | Undefined:

Returns the label of the edge from a node to its parent.

:param i:
   the node whose label is sought.
:type i:
  int

:returns:
   The label of the edge from the parent of *i* to *i*.
:rtype:
   int | Undefined

:raises LibsemigroupsError:
   if *i* exceeds ``number_of_nodes()``.

:complexity:
   Constant.
)pbdoc");

      thing.def(
          "labels",
          [](Forest const& self)
              -> std::vector<int_or_unsigned_constant<node_type>> {
            auto result
                = self.labels()
                  | rx::transform(
                      [](auto val) -> int_or_unsigned_constant<node_type> {
                        return {val};
                      })
                  | rx::to_vector();
            from_ints<node_type>(result);
            return result;
          },
          R"pbdoc(
:sig=(self: Forest) -> list[int | Undefined]:

Returns a copy of the list of edge labels in the :any:`Forest`. The value
in position *i* of this list is the label of the edge from the
parent of node *i* to *i*. If the parent equals :any:`UNDEFINED`,
then node *i* is a root node.

:returns:
   The edge labels of the forest.
:rtype:
   list[int | Undefined]

:complexity:
   Constant.
)pbdoc");

      thing.def("number_of_nodes",
                &Forest::number_of_nodes,
                R"pbdoc(
Returns the number of nodes in the forest. Returns the number of nodes
in the forest.

:returns:
   The number of nodes in the forest.
:rtype:
   int

:complexity:
   Constant.
)pbdoc");

      thing.def(py::self != py::self, py::arg("that"));
      thing.def(py::self == py::self, py::arg("that"));

      thing.def(
          "parent",
          [](Forest const& self,
             node_type     i) -> int_or_unsigned_constant<node_type> {
            return from_int(self.parent(i));
          },
          py::arg("i"),
          R"pbdoc(
:sig=(self: Forest, i: int) -> int | Undefined:

Returns the parent of a node.

:param i:
   the node whose parent is sought.
:type i:
   int

:returns:
   The parent of *i*.
:rtype:
   int | Undefined

:raises LibsemigroupsError:
   if *i* exceeds ``number_of_nodes()``.

:complexity:
   Constant
)pbdoc");

      thing.def(
          "parents",
          [](Forest const& self)
              -> std::vector<int_or_unsigned_constant<node_type>> {
            auto result
                = self.parents()
                  | rx::transform(
                      [](auto val) -> int_or_unsigned_constant<node_type> {
                        return {val};
                      })
                  | rx::to_vector();
            from_ints<node_type>(result);
            return result;
          },
          R"pbdoc(
:sig=(self: Forest) -> list[int | Undefined]:

Returns a list of parents in the :any:`Forest`. The value in position ``i`` of
this list is the parent of node ``i``. If the parent equals :any:`UNDEFINED`,
then node ``i`` is a root node.

:returns:
   The parents of the nodes in the forest.

:rtype:
   list[int | Undefined]

:complexity:
   Constant.
)pbdoc");

      thing.def(
          "path_to_root",
          [](Forest const& self, node_type i) {
            return forest::path_to_root(self, i);
          },
          py::arg("i"),
          R"pbdoc(
:sig=(self: Forest, i: int) -> list[int]:

Returns a list containing the labels of the edges on the path from the node *i* to a root node.

:param i: the node.
:type i: int

:returns: The word labelling the path from the root to *i*.
:rtype: list[int]

:raises LibsemigroupsError:
  if *i* is greater than or equal to :any:`number_of_nodes`.
)pbdoc");

      thing.def("set_parent_and_label",
                &Forest::set_parent_and_label,
                py::arg("node"),
                py::arg("parent"),
                py::arg("gen"),
                R"pbdoc(
:sig=(self: Forest, node: int, parent: int, gen: int) -> Forest:

Set the parent and edge label for a node. This function sets the parent of
*node* to be *parent*, and the associated edge-label to be *gen*.

:param node: the node whose parent and label to set.
:type node: int

:param parent: the parent node
:type parent: int

:param gen: the label of the edge from parent to node.
:type gen: int

:returns: *self*
:rtype: Forest

:raises LibsemigroupsError:
  if *node* or *parent* exceeds :any:`number_of_nodes()`.

:complexity: Constant.
)pbdoc");
    }  // class Forest

    ////////////////////////////////////////////////////////////////////////
    // Helpers
    ////////////////////////////////////////////////////////////////////////

    m.def(
        "forest_path_to_root",
        [](Forest const& f, node_type n) { return forest::path_to_root(f, n); },
        py::arg("f"),
        py::arg("n"),
        R"pbdoc(
:sig=(f: Forest, n: int) -> list[int]:

Returns a list containing the labels of the edges on the path from node *n* to
a root node.

:param f: the Forest.
:type f: Forest

:param n: the node.
:type n: int

:returns: The word labelling the path from the root to *n*.
:rtype: list[int]

:raises LibsemigroupsError:
  if *n* is greater than or equal to :any:`Forest.number_of_nodes`.
)pbdoc");

    m.def(
        "forest_depth",
        [](Forest const& f, node_type n) { return forest::depth(f, n); },
        py::arg("f"),
        py::arg("n"),
        R"pbdoc(
:sig=(f: Forest, n: int) -> int:

Returns the depth of a node in the forest, i.e. the distance, in terms of the
number of edges, from a root.

This function returns the length of the word returned by
:any:`path_to_root` and :any:`path_from_root`.

:param f: the Forest.
:type f: Forest

:param n: the node.
:type n: int

:returns: The depth of *n*.
:rtype: int

:raises LibsemigroupsError:
   if *n* is out of bounds (i.e. it is greater than or equal to
   :any:`Forest.number_of_nodes`).
)pbdoc");

    m.def(
        "forest_dot",
        [](Forest const& f) { return forest::dot(f); },
        py::arg("f"),
        R"pbdoc(
Returns a :any:`Dot` object representing a Forest.

This function returns a :any:`Dot` object representing the :any:`Forest` *f*.

:param f: the Forest.
:type f: Forest

:returns: A :any:`Dot` object.

:rtype: Dot
)pbdoc");

    m.def(
        "forest_dot",
        [](Forest const& f, std::vector<std::string> const& labels) {
          return forest::dot(f, labels);
        },
        py::arg("f"),
        py::arg("labels"),
        R"pbdoc(
:sig=(f: Forest, labels: list[str]) -> Dot:

Returns a :any:`Dot` object representing a Forest.

This function returns a :any:`Dot` object representing the :any:`Forest` *f*.
If *labels* is not empty, then each node is labelled with the path from
that node to the root of its tree with each letter replaced by the string
in the corresponding position of *labels*. If *labels* is empty, then
the nodes are not labelled by their paths.

:param f: the Forest.
:type f: Forest

:param labels: substitute for each edge label.
:type labels: list[str]

:returns: A :any:`Dot` object.
:rtype: Dot

:raises LibsemigroupsError:
    if the size of *labels* is not the same as the :any:`max_label` plus one.
)pbdoc");

    m.def("forest_is_root",
          &forest::is_root,
          py::arg("f"),
          py::arg("n"),
          R"pbdoc(
:sig=(f: Forest, n: int) -> bool:

Check if a node is the root of any tree in the :any:`Forest`.

This function returns ``True`` if the node *n* in the :any:`Forest` *f* is
a root node, and ``False`` if it is not.

:param f: the Forest.
:type f: Forest

:param n: the node.
:type n: int

:returns: Whether or not *n* is a root of *f*.
:rtype: bool

:raises LibsemigroupsError:
   if *n* is out of bounds (i.e. it is greater than or equal to
   :any:`Forest.number_of_nodes`).
)pbdoc");

    m.def(
        "forest_max_label",
        [](Forest const& f) -> int_or_unsigned_constant<Forest::label_type> {
          return from_int(forest::max_label(f));
        },
        py::arg("f"),
        R"pbdoc(
:sig=(f: Forest) -> int | Undefined:

Returns the maximum label of any edge in a :any:`Forest`.

This function returns the maximum label of any edge in the :any:`Forest` *f*
or :any:`UNDEFINED` if there are no edges.

:param f: the Forest.
:type f: Forest

:returns: The maximum label or :any:`UNDEFINED`.
:rtype: int | Undefined
)pbdoc");

    m.def(
        "forest_path_from_root",
        [](Forest const& f, Forest::node_type n) {
          return forest::path_from_root(f, n);
        },
        py::arg("f"),
        py::arg("n"),
        R"pbdoc(
:sig=(f: Forest, n: int) -> list[int]:

Returns a word containing the labels of the edges on the path from a root node
to *n*.

This function returns a word containing the labels of the edges on the path
from a root node to the node *n*.

:param f: the forest.
:type f: Forest

:param n: the node.
:type n: int

:returns: The word labelling the path from a root node to *n*.
:rtype: list[int]

.. seealso:: :any:`PathsFromRoots`
)pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // PathsFromToRootsCommon
    ////////////////////////////////////////////////////////////////////////

    {
      py::class_<forest::detail::PathsFromToRootsCommon> thing(
          m, "forest_PathsFromToRootsCommon");

      thing.def("at_end",
                &forest::detail::PathsFromToRootsCommon::at_end,
                R"pbdoc(
Check if the range is exhausted.

This function returns ``True`` if there are no more paths in the range, and
``False`` otherwise.

:returns: Whether or not the range is exhausted.
:rtype: bool
)pbdoc");

      thing.def("count",
                &forest::detail::PathsFromToRootsCommon::count,
                R"pbdoc(
Get the size of the range.

This function returns the number of paths in the range.

:returns:  the number of paths in the range.
:rtype: int
)pbdoc");

      thing.def("forest",
                &forest::detail::PathsFromToRootsCommon::forest,
                R"pbdoc(
Returns the underlying Forest object.

This function returns the :any:`Forest` object used to constructor or
initialise *self*.

:returns: A :any:`Forest` object.
:rtype: Forest
)pbdoc");

      thing.def("get",
                &forest::detail::PathsFromToRootsCommon::get,
                R"pbdoc(
Returns a reference to the current path.

This function returns a const reference to the current path from the root of
the tree containing :any:`target` to :any:`target`.

:returns: The current path.
:rtype: list[int]
)pbdoc");

      thing.def("target",
                &forest::detail::PathsFromToRootsCommon::target,
                R"pbdoc(
Get the current target of the path.

This function returns the current target of the path returned by :any:`get`.

:returns: The target of the current path.
:rtype: int
)pbdoc");
    }  // detail::PathsFromToRootsCommon

    ////////////////////////////////////////////////////////////////////////
    // PathsToRoots
    ////////////////////////////////////////////////////////////////////////

    {
      py::class_<forest::PathsToRoots, forest::detail::PathsFromToRootsCommon>
          thing(m,
                "forest_PathsToRoots",
                R"pbdoc(
Range object for iterating through paths in a :any:`Forest`.

This class represents a range object that facilitates
iterating through the paths from every node to the root of its subtree in a
:any:`Forest` object. These nodes are taken in numerical order, so the first value
returned by :any:`get` is the word to a root from node ``0``, after :any:`next`
is called, :any:`get` returns the word to a root from node ``1``, and so on.
The point of this class is to permit more efficient iteration over many paths
in a :any:`Forest` object than :any:`path_to_root` (and its
associated helper functions). :any:`path_to_root` traverses
the :any:`Forest` from the given node to the root of its tree. If the path from
nodes ``m`` and ``n`` to the root of their tree share a long common suffix, then
this suffix will be traversed twice in two calls to :any:`path_to_root`.
:any:`PathsToRoots` avoids this by finding the least common ancestor of ``m`` and
``n``, so that the suffix is not re-traversed. This works best when the nodes in
the :any:`Forest` are specified in a sensible order (such as via a depth-first or
breadth-first traversal).)pbdoc");

      thing.def("__repr__", [](forest::PathsToRoots const& thing) {
        return to_human_readable_repr(thing, ".");
      });

      thing.def("__copy__", [](forest::PathsToRoots const& pfr) {
        return forest::PathsToRoots(pfr);
      });

      thing.def(py::init<Forest const&>(),
                py::arg("f"),
                R"pbdoc(
:sig=(self: PathsToRoots, f: Forest) -> None:

Construct from a :any:`Forest`.

This function constructs a new :any:`PathsToRoots` for the :any:`Forest` *f*.

The newly constructed object does not copy *f* and is not valid if *f* is
destroyed.

:param f: the Forest.
:type f: Forest
)pbdoc");

      thing.def(
          "init",
          [](forest::PathsToRoots& self,
             Forest const&         f) -> forest::PathsToRoots& {
            return static_cast<forest::PathsToRoots&>(self.init(f));
          },
          py::arg("f"),
          R"pbdoc(
:sig=(self: PathsToRoots, f: Forest) -> PathsToRoots:

Re-initialize from a :any:`Forest`.

This function re-initializes *self* so that its underlying :any:`Forest`
object is *f*. This puts *self* back into the same state it
would have been in if it had been newly constructed from *f*.

:param f: the Forest.
:type f: Forest

:returns: ``self``
:rtype: PathsToRoots
)pbdoc");

      thing.def(
          "copy",
          [](forest::PathsToRoots const& pfr) {
            return forest::PathsToRoots(pfr);
          },
          R"pbdoc(
:sig=(self: PathsToRoots) -> PathsToRoots:

Copy a :any:`PathsToRoots` object.

:returns: A copy.
:rtype: PathsToRoots
)pbdoc");

      thing.def(
          "__iter__",
          [](forest::PathsToRoots& self) {
            return py::make_iterator(self.begin(), self.end());
          },
          // Keep the PathsToRoots object alive while the iterators
          // are alive
          py::keep_alive<0, 1>(),
          R"pbdoc(
:sig=(self: PathsToRoots) -> Iterator[list[int]]:

Returns an input iterator pointing to the first word in the range.

This function returns an input iterator pointing to the first word in a
:any:`PathsToRoots` object.

:returns: An input iterator.
:rtype: Iterator[list[int]]
)pbdoc");

      thing.def("next",
                &forest::PathsToRoots::next,
                R"pbdoc(
:sig=(self: PathsToRoots) -> None:

Advance to the next path in the range.

This function advances to the current path in the range.
If :any:`at_end` returns ``True``, then this function does nothing.

:returns: ``None``
:rtype: None
)pbdoc");

      thing.def("skip_n",
                &forest::PathsToRoots::skip_n,
                py::arg("n"),
                R"pbdoc(
:sig=(self: PathsToRoots, n: int) -> PathsToRoots:

Skip a number of paths in the range.

This function skips *n* paths in the range. If *n* is ``1``, then this is the
same as calling :any:`next`. If *n* is ``0``, then this function
does nothing.

:param n: the number of paths to skip.
:type n: int

:returns: ``self``.
:rtype: PathsToRoots
)pbdoc");
    }  // PathsToRoots

    ////////////////////////////////////////////////////////////////////////
    // PathsFromRoots
    ////////////////////////////////////////////////////////////////////////

    {
      py::class_<forest::PathsFromRoots, forest::detail::PathsFromToRootsCommon>
          thing(m,
                "forest_PathsFromRoots",
                R"pbdoc(
Range object for iterating through paths in a :any:`Forest`.

This class represents a range object that facilitates
iterating through the paths from every node to the root of its subtree in a
:any:`Forest` object. These nodes are taken in numerical order, so the first value
returned by :any:`get` is the word to a root from node ``0``, after :any:`next`
is called, :any:`get` returns the word to a root from node ``1``, and so on.
The point of this class is to permit more efficient iteration over many paths
in a :any:`Forest` object than :any:`path_from_root` (and its
associated helper functions). :any:`path_from_root` traverses
the :any:`Forest` from the given node to the root of its tree. If the path from
nodes ``m`` and ``n`` to the root of their tree share a long common suffix, then
this suffix will be traversed twice in two calls to :any:`path_from_root`.
:any:`PathsFromRoots` avoids this by finding the least common ancestor of ``m`` and
``n``, so that the suffix is not re-traversed. This works best when the nodes in
the :any:`Forest` are specified in a sensible order (such as via a depth-first or
breadth-first traversal).)pbdoc");

      thing.def("__repr__", [](forest::PathsFromRoots const& thing) {
        return to_human_readable_repr(thing, ".");
      });

      thing.def("__copy__", [](forest::PathsFromRoots const& pfr) {
        return forest::PathsFromRoots(pfr);
      });

      thing.def(py::init<Forest const&>(),
                py::arg("f"),
                R"pbdoc(
:sig=(self: PathsFromRoots, f: Forest) -> None:

Construct from a :any:`Forest`.

This function constructs a new :any:`PathsFromRoots` for the :any:`Forest` *f*.

The newly constructed object does not copy *f* and is not valid if *f* is
destroyed.

:param f: the Forest.
:type f: Forest
)pbdoc");

      thing.def(
          "init",
          [](forest::PathsFromRoots& self,
             Forest const&           f) -> forest::PathsFromRoots& {
            return static_cast<forest::PathsFromRoots&>(self.init(f));
          },
          py::arg("f"),
          R"pbdoc(
:sig=(self: PathsFromRoots, f: Forest) -> PathsFromRoots:

Re-initialize from a :any:`Forest`.

This function re-initializes *self* so that its underlying :any:`Forest`
object is *f*. This puts *self* back into the same state it
would have been in if it had been newly constructed from *f*.

:param f: the Forest.
:type f: Forest

:returns: ``self``
:rtype: PathsFromRoots
)pbdoc");

      thing.def(
          "copy",
          [](forest::PathsFromRoots const& pfr) {
            return forest::PathsFromRoots(pfr);
          },
          R"pbdoc(
:sig=(self: PathsFromRoots) -> PathsFromRoots:

Copy a :any:`PathsFromRoots` object.

:returns: A copy.
:rtype: PathsFromRoots
)pbdoc");

      thing.def(
          "__iter__",
          [](forest::PathsFromRoots& self) {
            return py::make_iterator(self.begin(), self.end());
          },
          // Keep the PathsToRoots object alive while the iterators
          // are alive
          py::keep_alive<0, 1>(),
          R"pbdoc(
:sig=(self: PathsFromRoots) -> Iterator[list[int]]:

Returns an input iterator pointing to the first word in the range.

This function returns an input iterator pointing to the first word in a
:any:`PathsFromRoots` object.

:returns: An input iterator.
:rtype: Iterator[list[int]]
)pbdoc");

      thing.def("next",
                &forest::PathsFromRoots::next,
                R"pbdoc(
:sig=(self: PathsFromRoots) -> None:

Advance to the next path in the range.

This function advances to the current path in the range.
If :any:`at_end` returns ``True``, then this function does nothing.

:returns: ``None``
:rtype: None
)pbdoc");

      thing.def("skip_n",
                &forest::PathsFromRoots::skip_n,
                py::arg("n"),
                R"pbdoc(
:sig=(self: PathsFromRoots, n: int) -> PathsFromRoots:

Skip a number of paths in the range.

This function skips *n* paths in the range. If *n* is ``1``, then this is the
same as calling :any:`next`. If *n* is ``0``, then this function
does nothing.

:param n: the number of paths to skip.
:type n: int

:returns: ``self``.
:rtype: PathsFromRoots
)pbdoc");
    }  // PathsFromRoots
  }

}  // namespace libsemigroups
