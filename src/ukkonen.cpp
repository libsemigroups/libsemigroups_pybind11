
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

// libsemigroups headers
#include <libsemigroups/types.hpp>  // for word_type
#include <libsemigroups/ukkonen.hpp>

// pybind11....
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_ukkonen

namespace py = pybind11;

namespace libsemigroups {

  template <typename Word>
  void bind_ukkonen_extras(py::module& m, py::class_<Ukkonen>& uk) {
    ////////////////////////////////////////////////////////////////////////
    // Ukkonen
    ////////////////////////////////////////////////////////////////////////

    uk.def(
        "index",
        [](Ukkonen const& self, Word const& w) {
          return self.index(w.begin(), w.end());
        },
        py::arg("w"),
        R"pbdoc(
:sig=(self: Ukkonen, w: str | List[int]) -> int:
:only-document-once:

Find the index of a word in the suffix tree.

If the *w* is one of the words that the suffix tree contains
(the words added to the suffix tree via :any:`ukkonen.add_word`), then this function
returns the index of that word. If the word *w* is not one of the words that the
suffix tree represents, then :any:`UNDEFINED` is returned.

:param w: the word to check.
:type w: str | List[int]

:returns: The index of *w*.
:rtype: int

:raises LibsemigroupsError:  if ``throw_if_not_unique_letters(w)`` throws.

:complexity: Linear in the length of *w*.
)pbdoc");
    uk.def(
        "throw_if_not_unique_letters",
        [](Ukkonen const& self, Word const& w) {
          return self.throw_if_not_unique_letters(w.begin(), w.end());
        },
        py::arg("w"),
        R"pbdoc(
:sig=(self: Ukkonen, w: str | List[int]) -> None:
:only-document-once:
Throw if the word *w* contains a letter equal to any of the unique letters added
to the end of words in the suffix tree.

This function throws an exception if the word *w* contains a letter equal to any
of the unique letters added to the end of words in the suffix tree.

:param w: the word to check.
:type w: str | List[int]

:raises LibsemigroupsError:  if *w* contains a letter equal to any
    of the unique letters added to the end of words in the suffix tree.

:complexity: Linear in the length of *w*.
)pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // Ukkonen helpers
    ////////////////////////////////////////////////////////////////////////
    m.def(
        "ukkonen_add_word",
        [](Ukkonen& u, Word const& w) { return ukkonen::add_word(u, w); },
        py::arg("u"),
        py::arg("w"),
        R"pbdoc(
:sig=(u: Ukkonen, w: str | List[int]) -> None:
:only-document-once:

Check and add a word to the suffix tree.

Calling this first checks that none of the letters in *w* is equal to any of 
the existing unique letters. It then invokes Ukkonen's algorithm to add
the given word to the suffix tree (if it is not already contained in
the tree). If an identical word is already in the tree, then this
function does nothing except increase the multiplicity of that word.
If *w* is empty, then this function does nothing.

:param u: the :any:`Ukkonen` object.
:type u: Ukkonen
:param w: the word to add.
:type w: str | List[int]

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws.

:complexity: Linear in the length of *w*.

.. seealso::
    
    :any:`Ukkonen.throw_if_not_unique_letters`.

)pbdoc");
    m.def(
        "add_words",
        [](Ukkonen& u, std::vector<Word> const& words) {
          ukkonen::add_words(u, words.begin(), words.end());
        },
        py::arg("u"),
        py::arg("words"),
        R"pbdoc(
:sig=(u: Ukkonen, words: List[str] | List[List[int]]) -> bool:
:only-document-once:
Add all words in a list to an :any:`Ukkonen` object.

:param u: the :any:`Ukkonen` object.
:type u: Ukkonen
:param words: the list of words to add.
:type w: List[str] | List[List[int]]

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws for any ``w`` in *words*.

.. seealso::
    
    * :any:`ukkonen.add_word`;
    * :any:`Ukkonen.throw_if_not_unique_letters`.

)pbdoc");
    m.def(
        "is_piece",
        [](Ukkonen const& u, Word const& w) { return ukkonen::is_piece(u, w); },
        py::arg("u"),
        py::arg("w"),
        R"pbdoc(
:sig=(u: Ukkonen, w: str | List[int]) -> bool:
:only-document-once:

Check if a word is a piece (occurs in two distinct places in the words of the suffix tree).

Returns ``True`` if *w* occurs in at least :math:`2` different (possibly
overlapping) places in the words contained in *u*. If no such prefix exists,
then `False` is returned.

:param u: the :any:`Ukkonen` object.
:type u: Ukkonen
:param w: the word to check.
:type w: str | List[int]

:returns: Whether *w* is a piece.
:rtype: bool

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws.

:complexity: Linear in the length of *w*.

.. seealso::

    :any:`Ukkonen.throw_if_not_unique_letters`.

)pbdoc");
    m.def(
        "is_subword",
        [](Ukkonen const& u, Word const& w) {
          return ukkonen::is_subword(u, w);
        },
        py::arg("u"),
        py::arg("w"),
        R"pbdoc(
:sig=(u: Ukkonen, w: str | List[int]) -> bool:
:only-document-once:

Check if a word is a subword of any word in a suffix tree.

Returns ``True`` if *w* is a subword of one of the words in the suffix tree
represented by the :any:`Ukkonen` instance *u*.

:param u: the :any:`Ukkonen` object.
:type u: Ukkonen
:param w: the word to check.
:type w: str | List[int]

:returns: Whether *w* is a subword of any word in *u*.
:rtype: bool

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws.

:complexity: Linear in the length of *w*.

.. seealso::

    :any:`Ukkonen.throw_if_not_unique_letters`.

)pbdoc");
    m.def(
        "is_suffix",
        [](Ukkonen const& u, Word const& w) {
          return ukkonen::is_suffix(u, w);
        },
        py::arg("u"),
        py::arg("w"),
        R"pbdoc(
:sig=(u: Ukkonen, w: str | List[int]) -> bool:
:only-document-once:

Check if a word is a suffix of any word in a suffix tree.

Returns ``True`` if *w* is a suffix of one of the words in the suffix tree
represented by the :any:`Ukkonen` instance *u*.

:param u: the :any:`Ukkonen` object.
:type u: Ukkonen
:param w: the word to check.
:type w: str | List[int]

:returns: Whether *w* is a suffix of any word in *u*.
:rtype: bool

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws.

:complexity: Linear in the length of *w*.

.. seealso::  :any:`Ukkonen.throw_if_not_unique_letters`.)pbdoc");
    m.def(
        "length_maximal_piece_prefix",
        [](Ukkonen const& u, Word const& w) {
          return ukkonen::length_maximal_piece_prefix(u, w);
        },
        py::arg("u"),
        py::arg("w"),
        R"pbdoc(
:sig=(u: Ukkonen, w: str | List[int]) -> int:
:only-document-once:

Find the length of the maximal prefix of a word occurring in two different places in a word in a suffix tree.

Returns the length of the maximal length prefix of *w* that occurs in at least
:math:`2` different (possibly overlapping) places in the words contained in *u*. 
If no such prefix exists, then ``0`` is returned.

:param u: the :any:`Ukkonen` object.
:type u: Ukkonen
:param w: the word.
:type w: str | List[int]

:returns: The length of the maximal piece prefix.
:rtype: int

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws.

:complexity: Linear in the length of *w*.

.. seealso::

    :any:`Ukkonen.throw_if_not_unique_letters`.

)pbdoc");
    m.def(
        "length_maximal_piece_suffix",
        [](Ukkonen const& u, Word const& w) {
          return ukkonen::length_maximal_piece_suffix(u, w);
        },
        py::arg("u"),
        py::arg("w"),
        R"pbdoc(
:sig=(u: Ukkonen, w: str | List[int]) -> int:
:only-document-once:

Find the length of the maximal suffix of a word occurring in two different places in a word in a suffix tree.

Returns the length of the maximal length prefix of *w* that occurs in at least
:math:`2` different (possibly overlapping) places in the words contained in *u*.
If no such prefix exists, then ``0`` is returned.

:param u: the :any:`Ukkonen` object.
:type u: Ukkonen
:param w: the word.
:type w: str | List[int]

:returns: The length of the maximal piece suffix.
:rtype: int

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws.

:complexity: Linear in the length of *w*.

.. seealso::

    :any:`Ukkonen.throw_if_not_unique_letters`.
)pbdoc");
    m.def(
        "maximal_piece_prefix",
        [](Ukkonen const& u, Word const& w) {
          return Word(w.cbegin(), ukkonen::maximal_piece_prefix(u, w));
        },
        py::arg("u"),
        py::arg("w"),
        R"pbdoc(
:sig=(u: Ukkonen, w: str | List[int]) -> str | List[int]:
:only-document-once:
Find the maximal prefix of a word occurring in two different places in a word in a suffix tree.

Returns the maximal length prefix of the word corresponding *w* that occurs in
at least :math:`2` different (possibly overlapping) places in the words
contained in *u*. If no such prefix exists, then an empty word is returned.

:param u: the :any:`Ukkonen` object.
:type u: Ukkonen
:param w: the word.
:type w: str | List[int]

:returns: The maximal piece prefix.
:rtype: str | List[int]

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws.

:complexity: Linear in the length of *w*.

.. seealso::

    :any:`Ukkonen.throw_if_not_unique_letters`.

)pbdoc");
    m.def(
        "maximal_piece_suffix",
        [](Ukkonen const& u, Word const& w) {
          return Word(ukkonen::maximal_piece_suffix(u, w), w.cend());
        },
        py::arg("u"),
        py::arg("w"),
        R"pbdoc(
:sig=(u: Ukkonen, w: str | List[int]) -> str | List[int]:
:only-document-once:
Find the maximal suffix of a word occurring in two different places in a word in a suffix tree.

Returns the maximal length suffix of the word corresponding *w* that occurs in
at least :math:`2` different (possibly overlapping) places in the words
contained in *u*. If no such suffix exists, then an empty word is returned.

:param u: the :any:`Ukkonen` object.
:type u: Ukkonen
:param w: the word.
:type w: str | List[int]

:returns: The maximal piece suffix.
:rtype: str | List[int]

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws.

:complexity: Linear in the length of *w*.

.. seealso::

    :any:`Ukkonen.throw_if_not_unique_letters`.
)pbdoc");
    m.def(
        "number_of_pieces",
        [](Ukkonen const& u, Word const& w) {
          return ukkonen::number_of_pieces(u, w);
        },
        py::arg("u"),
        py::arg("w"),
        R"pbdoc(
:sig=(u: Ukkonen, w: str | List[int]) -> int:
:only-document-once:

Find the number of pieces in a decomposition of a word (if any).

Returns the minimum number of pieces whose product equals *w* if such a product
exists, and ``0`` if no such product exists.

Recall that a *piece* is a word that occurs in two distinct positions
(possibly overlapping) of the words in the suffix tree *u*.

:param u: the :any:`Ukkonen` object.
:type u: Ukkonen
:param w: the word.
:type w: str | List[int]

:returns: The number of pieces.
:rtype: int

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws.

:complexity: Linear in the length of *w*.

.. seealso::

    :any:`Ukkonen.throw_if_not_unique_letters`.

)pbdoc");
    m.def(
        "pieces",
        [](Ukkonen const& u, Word const& w) { return ukkonen::pieces(u, w); },
        py::arg("u"),
        py::arg("w"),
        R"pbdoc(
:sig=(u: Ukkonen, w: str | List[int]) -> List[str] | List[List[int]]:
:only-document-once:
Find the pieces in a decomposition of a word (if any).

Returns a list of pieces whose product equals *w* if such a product exists, and
an empty list if no such product exists.

Recall that a *piece* is a word that occurs in two distinct positions (possibly
overlapping) of the words in the suffix tree *u*.

:param u: the :any:`Ukkonen` object.
:type u: Ukkonen
:param w: the word.
:type w: str | List[int]

:returns: The of pieces in the decomposition of *w*.
:rtype: List[str] | List[List[int]]

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws.

:complexity: Linear in the length of *w*.

.. seealso::

    :any:`Ukkonen.throw_if_not_unique_letters`.

)pbdoc");

    m.def(
        "traverse",
        [](Ukkonen const& u, Word const& w) {
          auto tmp = u.traverse(w.begin(), w.end());
          return std::pair<Ukkonen::State, Word>(tmp.first,
                                                 Word(w.begin(), tmp.second));
        },
        py::arg("u"),
        py::arg("w"),
        R"pbdoc(
:sig=(u: Ukkonen, w: str | List[int]) -> Tuple[Ukkonen.State, str | List[int]]:
:only-document-once:
Traverse the suffix tree from the root.

This function traverses the edges in the suffix tree, starting at the root node,
that are labelled by the letters in *W*. The suffix tree is traversed until the
end of *W* is reached, or a letter not corresponding to an edge is encountered.
A pair consisting of the state reached, and the portion of *w* consumed in the
traversal is returned.

:param u: the :any:`Ukkonen` object.
:type u: Ukkonen
:param w: the word.
:type w: str | List[int]

:returns: A tuple containing the :any:`State` reached, and the word consumed.
:rtype: Tuple[Ukkonen.State, str | List[int]]

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws.

:complexity: Linear in the length of *w*.

.. seealso::

    :any:`Ukkonen.throw_if_not_unique_letters`.

)pbdoc");

    m.def(
        "traverse",
        [](Ukkonen const& u, Ukkonen::State& st, Word w) {
          return Word(w.begin(), u.traverse(st, w.begin(), w.end()));
        },
        py::arg("u"),
        py::arg("st"),
        py::arg("w"),
        R"pbdoc(
:sig=(u: Ukkonen, w: str | List[int]) -> str | List[int]:
:only-document-once:
Traverse the suffix tree from the root.

This function traverses the edges in the suffix tree, starting at the
:any:`Ukkonen.State` *st*, that are labelled by the letters in *w*. The suffix
tree is traversed until the end of *w*is reached, or a letter not corresponding
to an edge is encountered. The state *st* is modified in-place to contain the
last state in the tree reached in the traversal. The returned word represents
the portion of *w* that was consumed in the traversal.

:param u: the :any:`Ukkonen` object.
:type u: Ukkonen
:param w: the word.
:type w: str | List[int]


:returns: The portion of *w* that was consumed in the traversal.
:rtype: str | List[int]

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws.

:complexity: Linear in the length of *w*.

.. seealso::

    :any:`Ukkonen.throw_if_not_unique_letters`.

)pbdoc");
  }

  void init_ukkonen(py::module& m) {
    using index_type      = size_t;
    using node_index_type = size_t;
    using edge_index_type = size_t;

    py::class_<Ukkonen> uk(m,
                           "Ukkonen",
                           R"pbdoc(
For an implementation of Ukkonen's algorithm.

This class implements Ukkonen's algorithm for constructing a generalised suffix
tree consisting of ``List[int]`` . The implementation in this class is based on:
`https://cp-algorithms.com/string/suffix-tree-ukkonen.html <https://cp-algorithms.com/string/suffix-tree-ukkonen.html>`_

The suffix tree is updated when the member function :any:`ukkonen.add_word` is
invoked. Every non-duplicate word added to the tree has a unique letter appended
to the end. If a duplicate word is added, then the tree is not modified, but the
:any:`multiplicity` of the word is increased.
)pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // Ukkonen - inner classes
    ////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////
    // State
    ////////////////////////////////////////////////////////////////////////
    py::class_<Ukkonen::State> state(uk,
                                     "State",
                                     R"pbdoc(
The return type of :any:`ukkonen.traverse`.

This indicates the position reached by following a path in the tree.
)pbdoc");
    state.def("__repr__", [](Ukkonen::State const& self) {
      return to_human_readable_repr(self, ".");
    });
    state.def_readwrite("pos",
                        &Ukkonen::State::pos,
                        R"pbdoc(
The position in the edge leading to the node ``v`` reached.)pbdoc");
    state.def_readwrite("v",
                        &Ukkonen::State::v,
                        R"pbdoc(
The index in :any:`Ukkonen.nodes` of the node at the end of the position reached.)pbdoc");
    state.def(py::init<>(), R"pbdoc(
Default constructor.)pbdoc");
    state.def(py::init<node_index_type, edge_index_type>(),
              py::arg("vv"),
              py::arg("ppos"),
              R"pbdoc(
Construct from index and position.

:param vv: the index of the node reached. 
:type vv: int
:param ppos: the position in the edge leading to vv.
:type ppos: int
)pbdoc");
    state.def(py::init<Ukkonen::State const&>(), R"pbdoc(
Default copy constructor.)pbdoc");
    state.def("__copy__",
              [](Ukkonen::State const& that) { return Ukkonen::State(that); });
    state.def(py::self == py::self, py::arg("that"));

    ////////////////////////////////////////////////////////////////////////
    // Node
    ////////////////////////////////////////////////////////////////////////

    py::class_<Ukkonen::Node> node(uk,
                                   "Node",
                                   R"pbdoc(
The type of the nodes in the tree.
)pbdoc");
    node.def("__repr__", [](Ukkonen::Node const& self) {
      return to_human_readable_repr(self, ".");
    });
    node.def_readwrite("children",
                       &Ukkonen::Node::children,
                       R"pbdoc(
The children of the current node.
)pbdoc");
    node.def_readwrite("l",
                       &Ukkonen::Node::l,
                       R"pbdoc(
The index of the first letter in the edge leading to the node.
)pbdoc");
    node.def_readwrite("parent",
                       &Ukkonen::Node::parent,
                       R"pbdoc(
The index of the parent node.
)pbdoc");
    node.def_readwrite("r",
                       &Ukkonen::Node::r,
                       R"pbdoc(
The index of one past the last letter in the edge leading to the node.
)pbdoc");
    node.def_readwrite("link", &Ukkonen::Node::link);
    node.def_readwrite("is_real_suffix", &Ukkonen::Node::is_real_suffix);
    node.def(py::init<index_type, index_type, node_index_type>(),
             py::arg("l")      = 0,
             py::arg("r")      = 0,
             py::arg("parent") = UNDEFINED,
             R"pbdoc(
Construct a node from left most index, right most index, and parent.

:param l: the left most index and value of the data member l (defaults to ``0``). 
:type l: int

:param r: one after the right most index and value of the data member r (defaults to ``0``). 
:type r: int

:param parent: parent of the node being constructed (defaults to :any:`UNDEFINED`).
:type parent: int
)pbdoc");
    node.def("__copy__",
             [](Ukkonen::Node const& that) { return Ukkonen::Node(that); });
    node.def(py::init<Ukkonen::Node const&>(), R"pbdoc(
Default copy constructor.
)pbdoc");
    node.def(
        "child",
        [](Ukkonen::Node const& self, letter_type c) { return self.child(c); },
        py::arg("c"),
        R"pbdoc(
The index of the child node corresponding to a letter (if any).

:param c: the first letter in the edge of the node.
:type c: int

:returns: The index of the child node.
:rtype: int

:complexity: Logarithmic in the size of ``len(children)``.
)pbdoc");
    node.def("is_leaf",
             &Ukkonen::Node::is_leaf,
             R"pbdoc(
Returns ``True``` if the node is a leaf and ``False`` if not.

:returns: Whether the node is a leaf. 
:rtype: bool

:complexity: Constant.
)pbdoc");
    node.def("is_root",
             &Ukkonen::Node::is_root,
             R"pbdoc(
Returns ``True`` if the node is the root and ``False`` if not.

:returns: Whether the node is the root.
:rtype: bool

:complexity: Constant.
)pbdoc");
    node.def("length",
             &Ukkonen::Node::length,
             R"pbdoc(
The length of the edge leading into the current node.

:returns: The length of the edge.
:rtype: int

:complexity: Constant.
)pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // Ukkonen
    ////////////////////////////////////////////////////////////////////////

    uk.def("__repr__",
           py::overload_cast<Ukkonen const&>(&to_human_readable_repr));
    uk.def(py::init<>(), R"pbdoc(
Constructs an empty generalised suffix tree.
)pbdoc");
    uk.def(py::init<Ukkonen const&>(), R"pbdoc(
Default copy constructor.
)pbdoc");
    uk.def("__copy__", [](Ukkonen const& that) { return Ukkonen(that); });
    uk.def("__iter__", [](Ukkonen const& self) {
      return py::make_iterator(self.begin(), self.end());
    });
    uk.def("distance_from_root",
           &Ukkonen::distance_from_root,
           py::arg("n"),
           R"pbdoc(
Returns the distance of a node from the root.

:param n: the node.
:type n: Ukkonen.Node

:returns: The distance from the root.
:rtype: int

:complexity: At worst linear in the distance of the node *n* from the root.
)pbdoc");
    uk.def("init",
           &Ukkonen::init,
           R"pbdoc(
Initialize an existing Ukkonen object.

This function puts an :any:`Ukkonen` object back into the same state as if it
had been newly default constructed.

:returns: A reference to ``self``.
:rtype: Ukkonen

.. seealso::

  :any:`Ukkonen()`
)pbdoc");
    uk.def("is_suffix",
           &Ukkonen::is_suffix,
           py::arg("st"),
           R"pbdoc(
Check if a state corresponds to a suffix.

This function returns a an int if the state *st* corresponds to a suffix of any
word in the suffix tree. The value returned is the index of the word which the
state is a suffix of if such a word exists, and :any:`UNDEFINED` otherwise.

:param st: the state.
:type st: Ukkonen.State

:returns: The index of a word for which *st* is a suffix, or :any:`UNDEFINED`.
:rtype: int
)pbdoc");
    uk.def("length_of_distinct_words",
           &Ukkonen::length_of_distinct_words,
           R"pbdoc(
Returns the sum of the lengths of the distinct words in the suffix tree.

:returns: The length of the distinct words .
:rtype: int

:complexity: Constant.
)pbdoc");
    uk.def("length_of_words",
           &Ukkonen::length_of_words,
           R"pbdoc(
Returns the sum of the lengths of all of the words in the suffix tree.

This is the total length of all the words added to the suffix tree including duplicates, if any.

:returns: The length of the words.
:rtype: int

:complexity: :math:`O(n)` where :math:`n` is the return value of :any:`number_of_distinct_words`.
)pbdoc");
    uk.def("max_word_length",
           &Ukkonen::max_word_length,
           R"pbdoc(
Returns the maximum length of word in the suffix tree.

:returns: The maximum length of a word.
:rtype: int

:complexity: Constant.
)pbdoc");
    uk.def("multiplicity",
           &Ukkonen::multiplicity,
           py::arg("i"),
           R"pbdoc(
Returns the multiplicity of a word by index.

This function returns the number of times that the word corresponding to the index *i* was added to the suffix tree.

:param i: the node index.
:type i: int

:returns: The multiplicity.
:rtype: int

:complexity: Constant.
)pbdoc");
    uk.def("nodes",
           &Ukkonen::nodes,
           R"pbdoc(
Returns the nodes in the suffix tree.

:returns: A list of nodes.
:rtype: List[Ukkonen.Node]

:complexity: Constant.
)pbdoc");
    uk.def("number_of_distinct_words",
           &Ukkonen::number_of_distinct_words,
           R"pbdoc(
Returns the number of distinct non-empty words in the suffix tree.

This is the number of distinct non-empty words added via :any:`ukkonen.add_word`.

:returns: The number of distinct non-empty words.
:rtype: int

:complexity: Constant.
)pbdoc");
    uk.def("number_of_words",
           &Ukkonen::number_of_words,
           R"pbdoc(
Returns the number of non-empty words in the suffix tree.

This is the number of all words added via :any:`ukkonen.add_word` including
duplicates, if any.

:returns: The number of words.
:rtype: int

:complexity:  :math:`O(n)` where :math:`n` is the return value of :any:`number_of_distinct_words`.
)pbdoc");
    uk.def("unique_letter",
           &Ukkonen::unique_letter,
           py::arg("i"),
           R"pbdoc(
Returns the unique letter added to the end of the ``i-th`` distinct word in the suffix tree.

:param i: the index of an added word.
:type i: int

:returns: The unique letter.
:rtype: int

:complexity: Constant.
)pbdoc");
    uk.def(
        "word_index",
        [](Ukkonen const& self, index_type i) { return self.word_index(i); },
        py::arg("i"),
        R"pbdoc(
Returns the index of the word corresponding to a position.

This function returns the least non-negative integer ``j`` such that the *i*-th
letter in the underlying string appears in the ``j``-th word added to the suffix
tree.

:param i: the position.
:type i: int

:returns: The index of a word.
:rtype: int

:complexity: Constant.
)pbdoc");
    uk.def(
        "word_index",
        [](Ukkonen const& self, Ukkonen::Node const& n) {
          return self.word_index(n);
        },
        py::arg("n"),
        R"pbdoc(
Returns the index of the word corresponding to a node.

This function returns the least non-negative integer ``i`` such that the
node *n* corresponds to the ``i``-th word added to the suffix tree.

:param n: the node.
:type n: Ukkonen.Node

:returns: The index of a word.
:rtype: int

:complexity: Constant.
)pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // Ukkonen helpers
    ////////////////////////////////////////////////////////////////////////

    m.def("ukkonen_dot",
          &ukkonen::dot,
          py::arg("u"),
          R"pbdoc(
Returns a :any:`Dot` object representing a suffix tree.

This function returns a :any:`Dot` object representing the suffix tree
defined by *u*.

Internally, all words added to the suffix tree are stored as a single string
delimited by unique letters. The edge labels present in this :any:`Dot` object
correspond to intervals of letters in that delimited string.

:param u: the Ukkonen object.
:type u: Ukkonen

:returns: A :any:`Dot` object representing *u*.
:rtype: Dot

:raises LibsemigroupsError:  if *u* does not contain any words.
:raises LibsemigroupsError:  if the number of words in *u* is greater than 24.

)pbdoc");

    m.def("number_of_distinct_subwords",
          &ukkonen::number_of_distinct_subwords,
          py::arg("u"),
          R"pbdoc(
Returns the total number of distinct subwords of the words in the suffix tree *u*.

:param u: the Ukkonen object.
:type u: Ukkonen

:returns: The total number of distinct subwords.
:rtype: int

:complexity: Linear in ``Ukkonen.length_of_distinct_words``.
)pbdoc");

    bind_ukkonen_extras<word_type>(m, uk);
    bind_ukkonen_extras<std::string>(m, uk);
  }  // init_ukkonen
}  // namespace libsemigroups
