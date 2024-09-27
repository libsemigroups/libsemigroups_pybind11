
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

// C std headers....
// TODO complete or delete

// C++ stl headers....
// TODO complete or delete

// libsemigroups headers
#include <libsemigroups/types.hpp>  // for word_type
#include <libsemigroups/ukkonen.hpp>

// pybind11....
// #include <pybind11/chrono.h>
// #include <pybind11/functional.h>
// #include <pybind11/pybind11.h>
// #include <pybind11/stl.h>
// TODO uncomment/delete

// libsemigroups_pybind11....
#include "main.hpp"  // for init_ukkonen

namespace py = pybind11;

namespace libsemigroups {

  void init_ukkonen(py::module& m) {
    using const_iterator = typename word_type::const_iterator;
    using index_type     = size_t;

    py::class_<Ukkonen> thing(m,
                              "Ukkonen",
                              R"pbdoc(
For an implementation of Ukkonen's algorithm.

This class implements Ukkonen's algorithm for constructing a generalised suffix
tree consisting of ``List[int]`` . The implementation in this class is based on:
`https://cp-algorithms.com/string/suffix-tree-ukkonen.html <https://cp-algorithms.com/string/suffix-tree-ukkonen.html>`_

The suffix tree is updated when the member function :any:`Ukonen.add_word` is
invoked. Every non-duplicate word added to the tree has a unique letter appended
to the end. If a duplicate word is added, then the tree is not modified, but the
:any:`multiplicity` of the word is increased.
)pbdoc");
    thing.def("__repr__",
              py::overload_cast<Ukkonen const&>(&to_human_readable_repr));
    thing.def(py::init<>(), R"pbdoc(
Default constructor.
Constructs an empty generalised suffix tree.

)pbdoc");
    thing.def(py::init<Ukkonen const&>(), R"pbdoc(
Default copy constructor.
)pbdoc");
    thing.def("__copy__", [](Ukkonen const& that) { return Ukkonen(that); });
    thing.def(
        "add_word",
        [](Ukkonen& self, word_type const& w) { return self.add_word(w); },
        py::arg("w"),
        R"pbdoc(
Check and add a word to the suffix tree.

Calling this first checking that none of the letters in *w* is equal to any of 
the existing unique letters. It then invokes Ukkonen's algorithm to add
the given word to the suffix tree (if it is not already contained in
the tree). If an identical word is already in the tree, then this
function does nothing except increase the multiplicity of that word.
If *w* is empty, then this function does nothing.

:param w: the word to add.
:type w: List[int]

:raises LibsemigroupsError:  if ``throw_if_not_unique_letters(w)`` throws.

:complexity:
Linear in the distance between `first` and `last`.
)pbdoc");
    thing.def("__iter__", [](Ukkonen const& self) {
      return py::make_iterator(self.begin(), self.end());
    });
    thing.def("distance_from_root",
              &Ukkonen::distance_from_root,
              py::arg("n"),
              R"pbdoc(
Returns the distance of a node from the root.

:param n: the node.
:type n: Node

:returns: The distance from the root.
:rtype: int

:complexity: At worst the distance of the node ``n`` from the root.
)pbdoc");
    thing.def(
        "index",
        [](Ukkonen const& self, word_type const& w) {
          self.index(w.begin(), w.end());
        },
        py::arg("w"),
        R"pbdoc(
Find the index of a word in the suffix tree.

If the word corresponding *w* is one of the words that the suffix tree contains
(the words added to the suffix tree via :any:`add_word`, then this function
returns the index of that word. If the word *w* is not one of the words that the
suffix tree represents, then :any:`UNDEFINED` is returned.

:param w: the word to check.
:type w: List[int]

:returns: The index of *w*.
:rtype: int

:raises LibsemigroupsError:  if ``throw_if_not_unique_letters(w)``

:complexity: Linear in the length of *w*.
)pbdoc");
    thing.def("init",
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
    thing.def("is_suffix",
              &Ukkonen::is_suffix,
              py::arg("st"),
              R"pbdoc(
Check if a state corresponds to a suffix.

This function returns a ``word_index_type`` if the state ``st`` corresponds to a suffix of any word in the suffix tree. The value returned is the index of the word which the state is a suffix of.

:param st: the state.
:type st: State



:complexity: At worst the distance of the node ``n`` from the root.


:returns: A value of type ``word_index_type``.

:rtype: word_index_type
)pbdoc");
    thing.def("is_unique_letter",
              &Ukkonen::is_unique_letter,
              py::arg("l"),
              R"pbdoc(
Check if a letter is a unique letter added to the end of a word in the suffix tree.

:param l: the letter_type to check.
:type l: letter_type
Returns ``True`` if ``l`` is one of the unique letters added to the end of a word in the suffix tree.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity: Constant.


:returns: A value of type ``bool``.

:rtype: bool
)pbdoc");
    thing.def("length_of_distinct_words",
              &Ukkonen::length_of_distinct_words,
              R"pbdoc(
Returns the sum of the lengths of the distinct words in the suffix tree.
Returns the sum of the lengths of the distinct words in the suffix tree.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity: Constant.

:returns: A value of type ``int``.

:rtype: int
)pbdoc");
    thing.def("length_of_words",
              &Ukkonen::length_of_words,
              R"pbdoc(
Returns the sum of the lengths of all of the words in the suffix tree.
Returns the sum of the lengths of all of the words in the suffix tree. This is the total length of all the words added to the suffix tree including duplicates, if any.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity:  :math:`O(n)` where :math:`n` is the return value of :any:`number_of_distinct_words`.

:returns: A value of type ``int``.

:rtype: int
)pbdoc");
    thing.def("max_word_length",
              &Ukkonen::max_word_length,
              R"pbdoc(
Returns the maximum length of word in the suffix tree.
Returns the maximum length of word in the suffix tree.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity: Constant.

:returns: A value of type ``int``.

:rtype: int
)pbdoc");
    thing.def("multiplicity",
              &Ukkonen::multiplicity,
              py::arg("i"),
              R"pbdoc(
Returns the multiplicity of a word by index.

:param i: the node.
:type i: word_index_type
This function returns the number of times that the word corresponding to the index ``i`` was added to the suffix tree.



:complexity: Constant.


:returns: A value of type ``int``.

:rtype: int
)pbdoc");
    thing.def("nodes",
              &Ukkonen::nodes,
              R"pbdoc(
Returns the nodes in the suffix tree.
Returns the nodes in the suffix tree.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity: Constant.

:returns: A const reference to a std::vector<Node> of ``Ukkonen::Node`` objects.

:rtype: list
)pbdoc");
    thing.def("number_of_distinct_words",
              &Ukkonen::number_of_distinct_words,
              R"pbdoc(
Returns the number of distinct non-empty words in the suffix tree.
Returns the number of distinct non-empty words in the suffix tree. This is the number of distinct non-empty words added via :any:`Ukkonen::add_word` or :any:`Ukkonen::add_word_no_checks`.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity: Constant.

:returns: A value of type ``int``.

:rtype: int
)pbdoc");
    thing.def("number_of_words",
              &Ukkonen::number_of_words,
              R"pbdoc(
Returns the number of non-empty words in the suffix tree.
Returns the number of non-empty words in the suffix tree. This is the number of all words added via :any:`Ukkonen::add_word` or :any:`Ukkonen::add_word_no_checks` including duplicates, if any.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity:  :math:`O(n)` where :math:`n` is the return value of :any:`number_of_distinct_words`.

:returns: A value of type ``int``.

:rtype: int
)pbdoc");
    thing.def(
        "throw_if_not_unique_letters",
        [](Ukkonen const& self, word_type const& w) {
          return self.throw_if_not_unique_letters(w);
        },
        py::arg("w"),
        R"pbdoc(
Throw if the word *w* contains a letter equal to any of the unique letters added to the end of words in the suffix tree.

This function throws an exception if the word *w* contains a letter equal to any
of the unique letters added to the end of words in the suffix tree.

:param w: the word to check.
:type w: List[int]

:raises LibsemigroupsError:  if ``is_unique_letter(l)`` returns ``True`` for any ``l`` in *w*.

:complexity: Linear in the length of *w*.
)pbdoc");
    thing.def(
        "traverse",
        [](Ukkonen const& self, word_type const& w) {
          return self.traverse(w.begin(), w.end());
        },
        py::arg("w"),
        R"pbdoc(
Traverse the suffix tree from the root.
See :any:`traverse_no_checks(Iterator, Iterator)`.

:raises LibsemigroupsError:  if ``throw_if_not_unique_letters(first,
last)`` throws.)pbdoc");
    //     thing.def(
    //         "traverse",
    //         [](Ukkonen const& self, State& st, Iterator first, Iterator last)
    //         {
    //           return self.traverse(st, first, last);
    //         },
    //         py::arg("st"),
    //         py::arg("first"),
    //         py::arg("last"),
    //         R"pbdoc(
    // Traverse the suffix tree from the root.
    // See :any:`traverse_no_checks(State, Iterator, Iterator)`.

    // :raises LibsemigroupsError:  if ``throw_if_not_unique_letters(first,
    // last)`` throws.)pbdoc");
    thing.def("unique_letter",
              &Ukkonen::unique_letter,
              py::arg("i"),
              R"pbdoc(
Returns the unique letter added to the end of a word in the suffix tree.

:param i: the index of an added word.
:type i: word_index_type
Returns the unique letter added to the end of the ``i-th`` distinct word added to the suffix tree.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity: Constant.


:returns: A value of type ``unique_letter_type``.

:rtype: unique_letter_type
)pbdoc");
    thing.def(
        "word_index",
        [](Ukkonen const& self, index_type i) { return self.word_index(i); },
        py::arg("i"),
        R"pbdoc(
Returns the index of the word corresponding to a position.

:param i: the position.
:type i: index_type
This function returns the least non-negative integer ``j`` such that the ``Ukkonen::begin() + i`` points to a character in the ``j`` -th word added to the suffix tree.



:complexity: Constant.


:returns: A value of type ``word_index_type``.

:rtype: word_index_type
)pbdoc");
    //     thing.def(
    //         "word_index",
    //         [](Ukkonen const& self, Node const& n) { return
    //         self.word_index(n); }, py::arg("n"), R"pbdoc(
    // Returns the index of the word corresponding to a node.

    // :param n: the node.
    // :type n: Node
    // This function returns the least non-negative integer ``i`` such that the
    // node ``n`` corresponds to the ``i`` -th word added to the suffix tree.

    // :complexity: Constant.

    // :returns: A value of type ``word_index_type``.

    // :rtype: word_index_type
    // )pbdoc");

    m.def(
        "add_words",
        [](Ukkonen& u, std::vector<word_type> const& words) {
          return ukkonen::add_words(u, words);
        },
        py::arg("u"),
        py::arg("words"),
        R"pbdoc(
Add all words in a range to a Ukkonen object.
See :any:`add_words_no_checks(Ukkonen, std::vector )`.

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws for any ``w`` in ``words``.

.. seealso::  :any:`Ukkonen::throw_if_not_unique_letters`.)pbdoc");
    //     m.def("dfs",
    //           &ukkonen::dfs,
    //           py::arg("u"),
    //           py::arg("helper"),
    //           R"pbdoc(
    // Perform a depth first search in a suffix tree.

    // :param u: the Ukkonen object.
    // :type u: Ukkonen

    // :param helper: the helper object.
    // :type helper: T
    // This function can be used to perform a depth first search in the suffix
    // tree ``u`` . The 2nd parameter is a helper object that must implement:

    // * A function ``void pre_order(Ukkonen  u, size_t i)`` ;
    // * A function ``void post_order(Ukkonen  u, size_t i)`` ; and
    // * A function ``auto yield(Ukkonen  u)``.

    // The function ``T::pre_order`` is called when the node ``n`` with index
    // ``i`` is first encountered in the depth-first search, and the function
    // ``T::post_order`` is called when the subtree rooted at ``n`` has been
    // completely explored.The function ``yield`` is called at the end of the
    // depth-first search and its return value is returned by this function.

    // :returns: A value whose type is the same as the return type of
    // ``T::yield``.

    // :rtype: typename T
    // )pbdoc");
    m.def("dot",
          &ukkonen::dot,
          py::arg("u"),
          R"pbdoc(
Returns a string containing a GraphViz representation of a suffix tree.

:param u: the Ukkonen object.
:type u: Ukkonen
Returns a string containing a`GraphViz <https://graphviz.org>`_ representation of a suffix tree.

:raises LibsemigroupsError:  if ``u`` does not contain any words.

:raises LibsemigroupsError:  if the number of words in ``u`` is greater than 24.


:returns: A value of type ``str``.

:rtype: str
)pbdoc");
    m.def(
        "is_piece",
        [](Ukkonen const& u, word_type const& w) {
          return ukkonen::is_piece(u, w);
        },
        py::arg("u"),
        py::arg("w"),
        R"pbdoc(
Check if a word is a piece (occurs in two distinct places in the words of the suffix tree).
See :any:`is_piece_no_checks(Ukkonen , Iterator, Iterator)`.

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws.

.. seealso::  :any:`Ukkonen::throw_if_not_unique_letters`.)pbdoc");
    m.def(
        "is_subword",
        [](Ukkonen const& u, word_type const& w) {
          return ukkonen::is_subword(u, w);
        },
        py::arg("u"),
        py::arg("w"),
        R"pbdoc(
Check if a word is a subword of any word in a suffix tree.
See :any:`is_subword_no_checks(Ukkonen , Iterator, Iterator)`.

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws.

.. seealso::  :any:`Ukkonen::throw_if_not_unique_letters`.)pbdoc");

    m.def(
        "is_suffix",
        [](Ukkonen const& u, word_type const& w) {
          return ukkonen::is_suffix(u, w);
        },
        py::arg("u"),
        py::arg("w"),
        R"pbdoc(
Check if a word is a suffix of any word in a suffix tree.
See :any:`is_suffix_no_checks(Ukkonen , Iterator, Iterator)`.

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws.

.. seealso::  :any:`Ukkonen::throw_if_not_unique_letters`.)pbdoc");

    m.def(
        "length_maximal_piece_prefix",
        [](Ukkonen const& u, word_type const& w) {
          return ukkonen::length_maximal_piece_prefix(u, w);
        },
        py::arg("u"),
        py::arg("w"),
        R"pbdoc(
Find the length of the maximal prefix of a word occurring in two different places in a word in a suffix tree.
See :any:`length_maximal_piece_prefix_no_checks(Ukkonen , Iterator,
 Iterator)`.

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws.

.. seealso::  :any:`Ukkonen::throw_if_not_unique_letters`.)pbdoc");
    m.def(
        "length_maximal_piece_suffix",
        [](Ukkonen const& u, word_type const& w) {
          return ukkonen::length_maximal_piece_suffix(u, w);
        },
        py::arg("u"),
        py::arg("w"),
        R"pbdoc(
Find the length of the maximal suffix of a word occurring in two different places in a word in a suffix tree.
See :any:`length_maximal_piece_suffix_no_checks(Ukkonen , Iterator,
 Iterator)`.

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws.

.. seealso::  :any:`Ukkonen::throw_if_not_unique_letters`.)pbdoc");
    m.def(
        "maximal_piece_prefix",
        [](Ukkonen const& u, word_type const& w) {
          return ukkonen::maximal_piece_prefix(u, w);
        },
        py::arg("u"),
        py::arg("w"),
        R"pbdoc(
Find the maximal prefix of a word occurring in two different places in a word in a suffix tree.
See :any:`maximal_piece_prefix_no_checks(Ukkonen , Iterator,
 Iterator)`.

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws.

.. seealso::  :any:`Ukkonen::throw_if_not_unique_letters`.)pbdoc");
    m.def(
        "maximal_piece_suffix",
        [](Ukkonen const& u, word_type const& w) {
          return ukkonen::maximal_piece_suffix(u, w);
        },
        py::arg("u"),
        py::arg("w"),
        R"pbdoc(
Find the maximal suffix of a word occurring in two different places in a word in a suffix tree.
See :any:`maximal_piece_suffix_no_checks(Ukkonen , Iterator,
 Iterator)`.

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws.

.. seealso::  :any:`Ukkonen::throw_if_not_unique_letters`.)pbdoc");
    m.def("number_of_distinct_subwords",
          &ukkonen::number_of_distinct_subwords,
          py::arg("u"),
          R"pbdoc(
Returns the number of distinct subwords of the words in a suffix tree.

:param u: the Ukkonen object.
:type u: Ukkonen
Returns the total number of distinct subwords of the words in the suffix tree ``u``.



:complexity: Linear in ``Ukkonen::length_of_distinct_words``.


:returns: A value of type ``int``.

:rtype: int
)pbdoc");
    m.def(
        "number_of_pieces",
        [](Ukkonen const& u, word_type const& w) {
          return ukkonen::number_of_pieces(u, w);
        },
        py::arg("u"),
        py::arg("w"),
        R"pbdoc(
Find the number of pieces in a decomposition of a word (if any).
See :any:`number_of_pieces_no_checks(Ukkonen , Iterator,
 Iterator)`.

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws.

.. seealso::  :any:`Ukkonen::throw_if_not_unique_letters`.)pbdoc");
    m.def(
        "pieces",
        [](Ukkonen const& u, word_type const& w) {
          return ukkonen::pieces(u, w);
        },
        py::arg("u"),
        py::arg("w"),
        R"pbdoc(
Find the pieces in a decomposition of a word (if any).
See :any:`pieces_no_checks(Ukkonen , Iterator, Iterator)`.

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws.

.. seealso::  :any:`Ukkonen::throw_if_not_unique_letters`.)pbdoc");
    m.def("traverse",
          &ukkonen::traverse,
          py::arg("u"),
          py::arg("w"),
          R"pbdoc(
Traverse the suffix tree from the root.
See :any:`Ukkonen::traverse_no_checks`.

:raises LibsemigroupsError:  if ``u.throw_if_not_unique_letters(w)`` throws.

.. seealso::  :any:`Ukkonen::throw_if_not_unique_letters` . // :any:`TODO(later)` :any:`Add` :any:`other` :any:`overloads`.)pbdoc");

  }  // init_ukkonen

}  // namespace libsemigroups
