//
// libsemigroups_pybind11
// Copyright (C) 2024 Joseph Edwards
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
#include <vector>  // for vector

// libsemigroups....
#include <libsemigroups/aho-corasick.hpp>  // for AhoCorasick, AhoCorasick::...

// pybind11....
#include <pybind11/pybind11.h>  // for class_, init, module

// libsemigroups_pybind11....
#include "main.hpp"  // for init_aho_corasick

namespace py = pybind11;

namespace libsemigroups {

  void init_aho_corasick(py::module& m) {
    py::class_<AhoCorasick> thing(m,
                                  "AhoCorasick",
                                  R"pbdoc(
For an implementation of the Aho-Corasick algorithm.

This class implements a trie based data structure with suffix links to be used
with the Aho-Corasick dictionary searching algorithm. An introduction to this
algorithm can be found `here <https://en.wikipedia.org/wiki/Aho%E2%80%93Corasick_algorithm>`_.

Several helper functions are provided in the ``aho_corasick``
namespace.)pbdoc");
    thing.def("__repr__", &aho_corasick::repr);
    // thing.def_static("root",
    //                  &AhoCorasick::root,
    //                  R"pbdoc(
    // Constant for the root of the trie.
    // )pbdoc");
    thing.def(py::init<>(), R"pbdoc(
Construct an empty AhoCorasick.

Construct an :any:`AhoCorasick` containing only the root that corresponds to the
empty word :math:`\varepsilon`.
)pbdoc");
    thing.def(py::init<AhoCorasick const&>(), R"pbdoc(
Default copy constructor.
)pbdoc");
    thing.def("init",
              &AhoCorasick::init,
              R"pbdoc(
Reinitialise an existing AhoCorasick object.
This function puts an :any:`AhoCorasick` object back into the same state as if it had been newly default constructed.

:returns: A reference to ``self``.

:rtype: AhoCorasick
)pbdoc");
    thing.def("number_of_nodes",
              &AhoCorasick::number_of_nodes,
              R"pbdoc(
Returns the number of nodes in the trie.
This function Returns the number of nodes in the trie.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity: Constant

:returns: A ``int``.

:rtype: int
)pbdoc");
    thing.def("add_word",
              &AhoCorasick::add_word<AhoCorasick::const_iterator>,
              py::arg("first"),
              py::arg("last"),
              R"pbdoc(
Check and add a word to the trie.
This function does the same as :any:`add_word_no_checks(Iterator, Iterator)` after first checking that the word corresponding to ``first`` and ``last`` does not correspond to an existing terminal node in the trie.

:raises LibsemigroupsError:  if the word corresponding to ``first`` and ``last`` corresponds to an existing terminal node in the trie.

.. seealso::  :any:`add_word_no_checks`)pbdoc");
    thing.def("rm_word",
              &AhoCorasick::rm_word<AhoCorasick::const_iterator>,
              py::arg("first"),
              py::arg("last"),
              R"pbdoc(
Check and add a word to the trie.
This function does the same as :any:`rm_word_no_checks(Iterator, Iterator)` after first checking that the word corresponding to ``first`` and ``last`` is terminal node in the trie.

:raises LibsemigroupsError:  if the word corresponding to ``first`` and ``last`` does not correspond to an existing terminal node in the trie.

.. seealso::  :any:`rm_word_no_checks`)pbdoc");
    thing.def("traverse",
              &AhoCorasick::traverse,
              py::arg("current"),
              py::arg("a"),
              R"pbdoc(
After checking, traverse the trie using suffix links where necessary.
See :any:`traverse_no_checks`

:raises LibsemigroupsError:  if ``validate_active_node_index(current)`` throws.)pbdoc");
    thing.def("signature",
              &AhoCorasick::signature,
              py::arg("w"),
              py::arg("i"),
              R"pbdoc(
After checking, find the signature of a node.
See :any:`signature_no_checks`

:raises LibsemigroupsError:  if ``validate_active_node_index(i)`` throws.)pbdoc");
    thing.def("height",
              &AhoCorasick::height,
              py::arg("i"),
              R"pbdoc(
After checking, calculate the height of a node.
See :any:`height_no_checks`

:raises LibsemigroupsError:  if ``validate_active_node_index(i)`` throws.)pbdoc");
    thing.def("suffix_link",
              &AhoCorasick::suffix_link,
              py::arg("current"),
              R"pbdoc(
After checking, calculate the index of the suffix link of a node.
See :any:`suffix_link_no_checks`

:raises LibsemigroupsError:  if ``validate_active_node_index(current)`` throws.)pbdoc");
    thing.def("node",
              &AhoCorasick::node,
              py::arg("i"),
              R"pbdoc(
After checking, return the node given an index.
See :any:`node_no_checks`

:raises LibsemigroupsError:  if ``validate_node_index(i)`` throws.)pbdoc");
    thing.def("child",
              &AhoCorasick::child,
              py::arg("parent"),
              py::arg("letter"),
              R"pbdoc(
After checking, return the child of parent with edge-label letter.
See :any:`child_no_checks`

:raises LibsemigroupsError:  if ``validate_active_node_index(parent)`` throws.)pbdoc");
    thing.def("validate_node_index",
              &AhoCorasick::validate_node_index,
              py::arg("i"),
              R"pbdoc(
Check if an index corresponds to a node.

:param i: the index to validate
:type i: index_type

This function checks if the given index ``i`` corresponds to the index of a node.

:complexity: Constant

:raises LibsemigroupsError:  if ``i`` does not correspond to the index of a node; that is, if ``i`` is larger than the size of the container storing the indices of nodes.)pbdoc");
    thing.def("validate_active_node_index",
              &AhoCorasick::validate_active_node_index,
              py::arg("i"),
              R"pbdoc(
Check if an index corresponds to a node currently in the trie.

:param i: the index to validate
:type i: index_type

The implementation of :any:`AhoCorasick` uses two different types of node; *active* and *inactive* . An active node is a node that is currently a node in the trie. An inactive node is a node that used to be part of the trie, but has since been removed. It may later become active again after being reinitialised (see :any:`init` ), and exists as a way of minimising how frequently memory needs to be allocated and deallocated for nodes.This function validates whether the given index ``i`` corresponds to an active node.

:complexity: Constant

:raises LibsemigroupsError:  if ``validate_node_index(i)`` throws, or if ``i`` is not an active node.

.. seealso::  :any:`validate_node_index` , :any:`init`.)pbdoc");

    // Helpers
    using index_type = AhoCorasick::index_type;

    m.def("add_word",
          &aho_corasick::add_word<std::string>,
          py::arg("ac"),
          py::arg("w"),
          R"pbdoc(
TODO doc.
)pbdoc");
    m.def("rm_word",
          &aho_corasick::rm_word<std::string>,
          py::arg("ac"),
          py::arg("w"),
          R"pbdoc(
TODO doc.
)pbdoc");
    m.def(
        "traverse_from",
        [](AhoCorasick const&          ac,
           index_type                  start,
           AhoCorasick::const_iterator first,
           AhoCorasick::const_iterator last) {
          return aho_corasick::traverse_from(ac, start, first, last);
        },
        py::arg("ac"),
        py::arg("start"),
        py::arg("first"),
        py::arg("last"),
        R"pbdoc(
TODO doc.
)pbdoc");
    m.def(
        "traverse_from",
        [](AhoCorasick const& ac, index_type start, char const& w) {
          return aho_corasick::traverse_from(ac, start, w);
        },
        py::arg("ac"),
        py::arg("start"),
        py::arg("w"),
        R"pbdoc(
TODO doc.
)pbdoc");
    m.def(
        "traverse_from",
        [](AhoCorasick const& ac, index_type start, word_type const& w) {
          return aho_corasick::traverse_from(ac, start, w);
        },
        py::arg("ac"),
        py::arg("start"),
        py::arg("w"),
        R"pbdoc(
TODO doc.
)pbdoc");
    m.def(
        "traverse",
        [](AhoCorasick const&          ac,
           AhoCorasick::const_iterator first,
           AhoCorasick::const_iterator last) {
          return aho_corasick::traverse(ac, first, last);
        },
        py::arg("ac"),
        py::arg("first"),
        py::arg("last"),
        R"pbdoc(
TODO doc.
)pbdoc");
    m.def(
        "traverse",
        [](AhoCorasick const& ac, std::string const& w) {
          return aho_corasick::traverse(ac, w);
        },
        py::arg("ac"),
        py::arg("w"),
        R"pbdoc(
TODO doc.
)pbdoc");

  }  // init_aho_corasick

}  // namespace libsemigroups
