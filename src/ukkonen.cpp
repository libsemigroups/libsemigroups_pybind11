//
// libsemigroups_pybind11
// Copyright (C) 2023 James D. Mitchell
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
// C++ stl headers....

// libsemigroups....
#include <libsemigroups/types.hpp>    // for rule_type
#include <libsemigroups/ukkonen.hpp>  // for Ukkonen

// pybind11....
#include <pybind11/pybind11.h>  // for class_, init, module
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_ukkonen

namespace py = pybind11;

namespace libsemigroups {
  void init_ukkonen(py::module& m) {
    py::class_<Ukkonen>(m, "Ukkonen")
        .def(py::init<>())
        .def(py::init<Ukkonen const&>())
        .def(
            "add_word_no_checks",
            [](Ukkonen& u, word_type const& w) { u.add_word_no_checks(w); },
            R"pbdoc(
              Add a word to the suffix tree.

              Calling this function immediately invokes Ukkonen's algorithm to add
              the given word to the suffix tree (if it is not already contained in
              the tree). If an identical word is already in the tree, then this
              function does nothing except increase the multiplicity of that word.
              If ``len(w) == 0``, then this function does nothing.

              :Parameters: **w** (List[int]) -- the word to add.

              :Complexity: Linear in the length of ``w``.

              :Returns: None

              .. warning::
                This function does no checks on its arguments whatsoever. In
                particular, if the word ``w`` contains any of the unique
                letters appended to the end of any existing word in the tree,
                then bad things will happen.
            )pbdoc")
        .def(
            "add_word_no_checks",
            [](Ukkonen& u, std::string const& w) { u.add_word_no_checks(w); },
            R"pbdoc(
              Add a word to the suffix tree.

              Calling this function immediately invokes Ukkonen's algorithm to add
              the given word to the suffix tree (if it is not already contained in
              the tree). If an identical word is already in the tree, then this
              function does nothing except increase the multiplicity of that word.
              If ``len(w) == 0``, then this function does nothing.

              :Parameters: **w** (str) -- the word to add.

              :Complexity: Linear in the length of ``w``.

              :Returns: None

              .. warning::
                This function does no checks on its arguments whatsoever. In
                particular, if the word ``w`` contains any of the unique
                letters appended to the end of any existing word in the tree,
                then bad things will happen.
            )pbdoc")
        .def(
            "add_word",
            [](Ukkonen& u, word_type const& w) { u.add_word(w); },
            R"pbdoc(
              Add a word to the suffix tree.

              Calling this function immediately invokes Ukkonen's algorithm to add
              the given word to the suffix tree (if it is not already contained in
              the tree). If an identical word is already in the tree, then this
              function does nothing except increase the multiplicity of that word.
              If ``len(w) == 0``, then this function does nothing.

              :Parameters: **w** (List[int]) -- the word to add.

              :Complexity: Linear in the length of ``w``.

              :Returns: None

              :Raises:
                ``RunTimeError`` if :py:meth:`Ukkonen.validate_word` raises.
            )pbdoc")
        .def(
            "add_word",
            [](Ukkonen& u, std::string const& w) { u.add_word(w); },
            R"pbdoc(
              Add a word to the suffix tree.

              Calling this function immediately invokes Ukkonen's algorithm to add
              the given word to the suffix tree (if it is not already contained in
              the tree). If an identical word is already in the tree, then this
              function does nothing except increase the multiplicity of that word.
              If ``len(w) == 0``, then this function does nothing.

              :Parameters: **w** (str) -- the word to add.

              :Complexity: Linear in the length of ``w``.

              :Returns: None

              :Raises:
                ``RunTimeError`` if :py:meth:`Ukkonen.validate_word` raises.
            )pbdoc")
        .def("number_of_distinct_words",
             &Ukkonen::number_of_distinct_words,
             R"pbdoc(
               Returns the number of distinct non-empty words in the suffix
               tree.

               :return: A value of type ``int``.
             )pbdoc")
        .def("length_of_distinct_words",
             &Ukkonen::length_of_distinct_words,
             R"pbdoc(
               Returns the sum of the lengths of the distinct words in the
               suffix tree.

               :return: A value of type ``int``.
             )pbdoc")
        .def("length_of_words",
             &Ukkonen::length_of_words,
             R"pbdoc(
               Returns the sum of the lengths of all of the words in the suffix
               tree.

               :return: A value of type ``int``
             )pbdoc")
        .def("number_of_words",
             &Ukkonen::number_of_words,
             R"pbdoc(
               Returns the number of non-empty words in the suffix tree.

               :return: A value of type ``int``.
             )pbdoc")
        .def("max_word_length",
             &Ukkonen::max_word_length,
             R"pbdoc(
               Returns the maximum length of word in the suffix tree.

               :return: A value of type ``int``.
             )pbdoc")
        .def(
            "__iter__",
            [](Ukkonen const& x) {
              return py::make_iterator(x.cbegin(), x.cend());
            },
            R"pbdoc(
               Returns an iterator pointing to the first letter of the first
               word in the suffix tree.
            )pbdoc")
        .def(
            "validate_word",
            [](Ukkonen const& u, word_type const& w) { u.validate_word(w); },
            R"pbdoc(
              Validate a word.
            )pbdoc")
        .def("word_index",
             py::overload_cast<Ukkonen::index_type>(&Ukkonen::word_index,
                                                    py::const_),
             py::arg("i"),
             R"pbdoc(
               Returns the index of the word corresponding to a position.

               :Parameters: **i** (int) - the position.

               :Returns: A value of type word_index_type.
             )pbdoc")
        .def("unique_letter",
             &Ukkonen::unique_letter,
             py::arg("i"),
             R"pbdoc(
               Returns the unique letter added to the end of a word in the
               suffix tree.

               :param i: the index of an added word
               :type i: int

               :return: A value of type ``int``.
            )pbdoc")
        .def("multiplicity",
             &Ukkonen::multiplicity,
             py::arg("i"),
             R"pbdoc(
               Returns the multiplicity of a word by index.

               :param i: the index
               :type i: int

               :return: A value of type ``int``.
             )pbdoc")
        .def("is_unique_letter",
             &Ukkonen::is_unique_letter,
             py::arg("l"),
             R"pbdoc(
               Check if a letter is a unique letter added to the end of a word
               in the suffix tree.

               :param l: the letter to check.
               :type l: int

               :return: A value of type ``bool``.
             )pbdoc");

    m.def(
         "add_words_no_checks",
         [](Ukkonen& u, std::vector<word_type> const& words) {
           ukkonen::add_words_no_checks(u, words);
         },
         py::arg("u"),
         py::arg("words"),
         R"pbdoc(
           Add all words in a ``list`` to a Ukkonen object.

           :param u: the Ukkonen object
           :type u: Ukkonen
           :param words: the words to add
           :type words: List[List[int]]

           :returns: None

           .. warning::
             This function does no checks on its arguments whatsoever. In
             particular, if any of the words in ``words`` contains any of the
             unique letters appended to the end of any existing word in the
             tree, then bad things will happen.
         )pbdoc")
        .def(
            "add_words",
            [](Ukkonen& u, std::vector<word_type> const& words) {
              ukkonen::add_words(u, words);
            },
            py::arg("u"),
            py::arg("words"),
            R"pbdoc(
              Add all words in a ``list`` to a Ukkonen object.

              :param u: the Ukkonen object
              :type u: Ukkonen
              :param words: the words to add
              :type words: List[List[int]]

              :returns: None

              .. warning::
                This function does no checks on its arguments whatsoever. In
                particular, if any of the words in ``words`` contains any of the
                unique letters appended to the end of any existing word in the
                tree, then bad things will happen.

              :Raises:
                ``RunTimeError`` if :py:meth:`Ukkonen.validate_word` raises.
             )pbdoc")
        .def("_dot", &ukkonen::dot)
        .def("number_of_distinct_subwords",
             &ukkonen::number_of_distinct_subwords,
             py::arg("u"),
             R"pbdoc(
               Returns the number of distinct subwords of the words in a suffix tree.

               Returns the total number of distinct subwords of the words in the
               suffix tree ``u``.

               :param u:  the Ukkonen object
               :type u: Ukkonen

               :returns: A value of type ``size_t``.

               :complexity:
                 Linear in :py:meth:`Ukkonen.length_of_distinct_words`.

              :Raises:
                ``RunTimeError`` if :py:meth:`Ukkonen.validate_word` raises.
             )pbdoc")
        .def(
            "is_subword_no_checks",
            [](Ukkonen const& u, word_type const& w) {
              return ukkonen::is_subword_no_checks(u, w);
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Check if a word is a subword of any word in a suffix tree.

              Returns ``True`` if the word ``w`` is a subword of one of the words
              in the suffix tree represented by the :py:class:`Ukkonen`
              instance ``u``.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (List[int]) -- the possible subword

              :Returns: A value of type ``bool``.

              .. warning::
                This function does no checks on its arguments whatsoever. In
                particular, if the word ``w`` contains any of the unique letters
                appended to the end of any existing word in the tree, then bad
                things will happen.

              :Complexity:
                Linear in the length of ``w``.
           )pbdoc")
        .def(
            "is_subword_no_checks",
            [](Ukkonen const& u, std::string const& w) {
              return ukkonen::is_subword_no_checks(u, w);
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Check if a word is a subword of any word in a suffix tree.

              Returns ``True`` if the word ``w`` is a subword of one of the words
              in the suffix tree represented by the :py:class:`Ukkonen`
              instance ``u``.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (str) -- the possible subword

              :Returns: A value of type ``bool``.

              .. warning::
                This function does no checks on its arguments whatsoever. In
                particular, if the word ``w`` contains any of the unique letters
                appended to the end of any existing word in the tree, then bad
                things will happen.

              :Complexity:
                Linear in the length of ``w``.
             )pbdoc")
        .def(
            "is_subword",
            [](Ukkonen const& u, word_type const& w) {
              return ukkonen::is_subword(u, w);
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Check if a word is a subword of any word in a suffix tree.

              Returns ``True`` if the word ``w`` is a subword of one of the words
              in the suffix tree represented by the :py:class:`Ukkonen`
              instance ``u``.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (List[int]) -- the possible subword

              :Returns: A value of type ``bool``.

              :Complexity:
                Linear in the length of ``w``.

              :Raises:
                ``RunTimeError`` if :py:meth:`Ukkonen.validate_word` raises.
             )pbdoc")
        .def(
            "is_subword",
            [](Ukkonen const& u, std::string const& w) {
              return ukkonen::is_subword(u, w);
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Check if a word is a subword of any word in a suffix tree.

              Returns ``True`` if the word ``w`` is a subword of one of the words
              in the suffix tree represented by the :py:class:`Ukkonen`
              instance ``u``.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (str) -- the possible subword

              :Returns: A value of type ``bool``.

              :Complexity:
                Linear in the length of ``w``.

              :Raises:
                ``RunTimeError`` if :py:meth:`Ukkonen.validate_word` raises.
             )pbdoc")
        .def(
            "is_suffix_no_checks",
            [](Ukkonen const& u, word_type const& w) {
              return ukkonen::is_suffix_no_checks(u, w);
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Check if a word is a suffix of any word in a suffix tree.

              Returns ``True`` if the word ``w`` is a suffix of one of the
              words in the suffix tree represented by the Ukkonen instance
              ``u``.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (List[int]) -- the possible subword

              :Returns: A value of type ``bool``.


              .. warning::
                This function does no checks on its arguments whatsoever. In
                particular, if the word ``w`` contains any of the unique letters
                appended to the end of any existing word in the tree, then bad
                things will happen.

              :Complexity:
                Linear in the length of ``w``.
             )pbdoc")
        .def(
            "is_suffix_no_checks",
            [](Ukkonen const& u, std::string const& w) {
              return ukkonen::is_suffix_no_checks(u, w);
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Check if a word is a suffix of any word in a suffix tree.

              Returns ``True`` if the word ``w`` is a suffix of one of the
              words in the suffix tree represented by the Ukkonen instance
              ``u``.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (str) -- the possible subword

              :Returns: A value of type ``bool``.

              .. warning::
                This function does no checks on its arguments whatsoever. In
                particular, if the word ``w`` contains any of the unique letters
                appended to the end of any existing word in the tree, then bad
                things will happen.

              :Complexity:
                Linear in the length of ``w``.
             )pbdoc")
        .def(
            "is_suffix",
            [](Ukkonen const& u, word_type const& w) {
              return ukkonen::is_suffix(u, w);
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Check if a word is a suffix of any word in a suffix tree.

              Returns ``True`` if the word ``w`` is a suffix of one of the
              words in the suffix tree represented by the Ukkonen instance
              ``u``.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (List[int]) -- the possible subword

              :Returns: A value of type ``bool``.

              :Complexity:
                Linear in the length of ``w``.

              :Raises:
                ``RunTimeError`` if :py:meth:`Ukkonen.validate_word` raises.
             )pbdoc")
        .def(
            "is_suffix",
            [](Ukkonen const& u, std::string const& w) {
              return ukkonen::is_suffix(u, w);
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Check if a word is a suffix of any word in a suffix tree.

              Returns ``True`` if the word ``w`` is a suffix of one of the
              words in the suffix tree represented by the Ukkonen instance
              ``u``.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (str) -- the possible subword

              :Returns: A value of type ``bool``.

              :Complexity:
                Linear in the length of ``w``.

              :Raises:
                ``RunTimeError`` if :py:meth:`Ukkonen.validate_word` raises.
             )pbdoc")
        .def(
            "is_piece_no_checks",
            [](Ukkonen const& u, word_type const& w) {
              return ukkonen::is_piece_no_checks(u, w);
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Check if a word is a piece (occurs in two distinct places in the words
              of the suffix tree).

              Returns ``True`` if the word ``w`` that occurs in at least
              \f$2\f$ different (possibly overlapping) places in the words
              contained in ``u``. If no such prefix exists, then ``False`` is
              returned.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (List[int]) -- the possible subword
              :Returns: A value of type ``bool``.

              :Complexity:
                Linear in the length of ``w``.

              .. warning::
                This function does no checks on its arguments whatsoever. In
                particular, if the word ``w`` contains any of the unique letters
                appended to the end of any existing word in the tree, then bad
                things will happen.
             )pbdoc")
        .def(
            "is_piece_no_checks",
            [](Ukkonen const& u, std::string const& w) {
              return ukkonen::is_piece_no_checks(u, w);
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Check if a word is a piece (occurs in two distinct places in the words
              of the suffix tree).

              Returns ``True`` if the word ``w`` that occurs in at least
              \f$2\f$ different (possibly overlapping) places in the words
              contained in ``u``. If no such prefix exists, then ``False`` is
              returned.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (str) -- the possible subword
              :Returns: A value of type ``bool``.

              :Complexity:
                Linear in the length of ``w``.

              .. warning::
                This function does no checks on its arguments whatsoever. In
                particular, if the word ``w`` contains any of the unique letters
                appended to the end of any existing word in the tree, then bad
                things will happen.
             )pbdoc")
        .def(
            "is_piece",
            [](Ukkonen const& u, word_type const& w) {
              return ukkonen::is_piece(u, w);
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Check if a word is a piece (occurs in two distinct places in the words
              of the suffix tree).

              Returns ``True`` if the word ``w`` that occurs in at least
              \f$2\f$ different (possibly overlapping) places in the words
              contained in ``u``. If no such prefix exists, then ``False`` is
              returned.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (List[int]) -- the possible subword

              :Returns: A value of type ``bool``.

              :Complexity:
                Linear in the length of ``w``.

              :Raises:
                ``RunTimeError`` if :py:meth:`Ukkonen.validate_word` raises.
             )pbdoc")
        .def(
            "is_piece",
            [](Ukkonen const& u, std::string const& w) {
              return ukkonen::is_piece(u, w);
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Check if a word is a piece (occurs in two distinct places in the words
              of the suffix tree).

              Returns ``True`` if the word ``w`` that occurs in at least
              \f$2\f$ different (possibly overlapping) places in the words
              contained in ``u``. If no such prefix exists, then ``False`` is
              returned.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (str) -- the possible subword

              :Returns: A value of type ``bool``.

              :Complexity:
                Linear in the length of ``w``.

              :Raises:
                ``RunTimeError`` if :py:meth:`Ukkonen.validate_word` raises.
             )pbdoc")
        .def(
            "maximal_piece_prefix_no_checks",
            [](Ukkonen const& u, word_type const& w) {
              return word_type(w.cbegin(),
                               ukkonen::maximal_piece_prefix_no_checks(u, w));
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Find the maximal prefix of a word occurring in two different
              places in a word in a suffix tree.

              Returns the maximal length prefix of the word ``w`` that occurs
              in at least two different (possibly overlapping) places in
              the words contained in ``u``. If no such prefix exists, then
              an empty list is returned.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (List[int]) -- the possible subword

              :Returns: A value of type ``bool``.

              :Complexity:
                Linear in the length of ``w``.

              .. warning::
                This function does no checks on its arguments whatsoever. In
                particular, if the word ``w`` contains any of the unique letters
                appended to the end of any existing word in the tree, then bad
                things will happen.
             )pbdoc")
        .def(
            "maximal_piece_prefix_no_checks",
            [](Ukkonen const& u, std::string const& w) {
              return std::string(w.cbegin(),
                                 ukkonen::maximal_piece_prefix_no_checks(u, w));
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Find the maximal prefix of a word occurring in two different
              places in a word in a suffix tree.

              Returns the maximal length prefix of the word ``w`` that occurs
              in at least two different (possibly overlapping) places in
              the words contained in ``u``. If no such prefix exists, then
              an empty list is returned.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (str) -- the possible subword

              :Returns: A value of type ``bool``.

              :Complexity:
                Linear in the length of ``w``.

              .. warning::
                This function does no checks on its arguments whatsoever. In
                particular, if the word ``w`` contains any of the unique letters
                appended to the end of any existing word in the tree, then bad
                things will happen.
             )pbdoc")
        .def(
            "maximal_piece_prefix",
            [](Ukkonen const& u, word_type const& w) {
              return word_type(w.cbegin(), ukkonen::maximal_piece_prefix(u, w));
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Find the maximal prefix of a word occurring in two different
              places in a word in a suffix tree.

              Returns the maximal length prefix of the word ``w`` that occurs
              in at least two different (possibly overlapping) places in
              the words contained in ``u``. If no such prefix exists, then
              an empty list is returned.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (str) -- the possible subword

              :Returns: A value of type ``bool``.

              :Complexity:
                Linear in the length of ``w``.

              :Raises:
                ``RunTimeError`` if :py:meth:`Ukkonen.validate_word` raises.
             )pbdoc")
        .def(
            "maximal_piece_prefix",
            [](Ukkonen const& u, std::string const& w) {
              return std::string(w.cbegin(),
                                 ukkonen::maximal_piece_prefix(u, w));
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Find the maximal prefix of a word occurring in two different
              places in a word in a suffix tree.

              Returns the maximal length prefix of the word ``w`` that occurs
              in at least two different (possibly overlapping) places in
              the words contained in ``u``. If no such prefix exists, then
              an empty list is returned.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (str) -- the possible subword

              :Returns: A value of type ``bool``.

              :Complexity:
                Linear in the length of ``w``.

              :Raises:
                ``RunTimeError`` if :py:meth:`Ukkonen.validate_word` raises.
             )pbdoc")
        .def(
            "maximal_piece_suffix_no_checks",
            [](Ukkonen const& u, word_type const& w) {
              return word_type(ukkonen::maximal_piece_suffix_no_checks(u, w),
                               w.cend());
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Find the maximal suffix of a word occurring in two different
              places in a word in a suffix tree.

              Returns the maximal length suffix of the word ``w`` that occurs
              in at least two different (possibly overlapping) places in
              the words contained in ``u``. If no such suffix exists, then
              an empty list is returned.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (List[int]) -- the possible subword

              :Returns: A value of type ``bool``.

              :Complexity:
                At worst :math:`O(m ^ 2)` or :math:`O(n)` where :math:`m` is the
                length of ``w`` and :math:`n` is the return value of
                :py:meth:`Ukkonen.length_of_distinct_words`.

              .. warning::
                This function does no checks on its arguments whatsoever. In
                particular, if the word ``w`` contains any of the unique letters
                appended to the end of any existing word in the tree, then bad
                things will happen.
             )pbdoc")
        .def(
            "maximal_piece_suffix_no_checks",
            [](Ukkonen const& u, std::string const& w) {
              return std::string(ukkonen::maximal_piece_suffix_no_checks(u, w),
                                 w.cend());
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Find the maximal suffix of a word occurring in two different
              places in a word in a suffix tree.

              Returns the maximal length suffix of the word ``w`` that occurs
              in at least two different (possibly overlapping) places in
              the words contained in ``u``. If no such suffix exists, then
              an empty list is returned.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (str) -- the possible subword

              :Returns: A value of type ``bool``.

              :Complexity:
                At worst :math:`O(m ^ 2)` or :math:`O(n)` where :math:`m` is the
                length of ``w`` and :math:`n` is the return value of
                :py:meth:`Ukkonen.length_of_distinct_words`.

              .. warning::
                This function does no checks on its arguments whatsoever. In
                particular, if the word ``w`` contains any of the unique letters
                appended to the end of any existing word in the tree, then bad
                things will happen.
             )pbdoc")
        .def(
            "maximal_piece_suffix",
            [](Ukkonen const& u, word_type const& w) {
              return word_type(ukkonen::maximal_piece_suffix(u, w), w.cend());
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Find the maximal suffix of a word occurring in two different
              places in a word in a suffix tree.

              Returns the maximal length suffix of the word ``w`` that occurs
              in at least two different (possibly overlapping) places in
              the words contained in ``u``. If no such suffix exists, then
              an empty list is returned.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (List[int]) -- the possible subword

              :Returns: A value of type ``bool``.

              :Complexity:
                 At worst :math:`O(m ^ 2)` or :math:`O(n)` where :math:`m` is the
                 length of ``w`` and :math:`n` is the return value of
                 :py:meth:`Ukkonen.length_of_distinct_words`.

              :Raises:
                ``RunTimeError`` if :py:meth:`Ukkonen.validate_word` raises.
             )pbdoc")
        .def(
            "maximal_piece_suffix",
            [](Ukkonen const& u, std::string const& w) {
              return std::string(ukkonen::maximal_piece_suffix(u, w), w.cend());
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Find the maximal suffix of a word occurring in two different
              places in a word in a suffix tree.

              Returns the maximal length suffix of the word ``w`` that occurs
              in at least two different (possibly overlapping) places in
              the words contained in ``u``. If no such suffix exists, then
              an empty list is returned.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (str) -- the possible subword

              :Returns: A value of type ``bool``.

              :Complexity:
                 At worst :math:`O(m ^ 2)` or :math:`O(n)` where :math:`m` is the
                 length of ``w`` and :math:`n` is the return value of
                 :py:meth:`Ukkonen.length_of_distinct_words`.

              :Raises:
                ``RunTimeError`` if :py:meth:`Ukkonen.validate_word` raises.
             )pbdoc")
        .def(
            "number_of_pieces_no_checks",
            [](Ukkonen const& u, word_type const& w) {
              return ukkonen::number_of_pieces_no_checks(u, w);
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Find the number of pieces in a decomposition of a word (if any).

              Returns minimum number of pieces whose product equals the word
              ``w`` if such a product exists, and ``0`` if no such product
              exists. Recall that a *piece* is a word that occurs in two
              distinct positions (possibly overlapping) of the words in the
              suffix tree ``u``.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (List[int]) -- the possible subword

              :Returns: A value of type ``bool``.

              :Complexity:
                Linear in the length of ``w``.

              .. warning::
                This function does no checks on its arguments whatsoever. In
                particular, if the word ``w`` contains any of the unique letters
                appended to the end of any existing word in the tree, then bad
                things will happen.
          )pbdoc")
        .def(
            "number_of_pieces_no_checks",
            [](Ukkonen const& u, std::string const& w) {
              return ukkonen::number_of_pieces_no_checks(u, w);
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Find the number of pieces in a decomposition of a word (if any).

              Returns minimum number of pieces whose product equals the word
              ``w`` if such a product exists, and ``0`` if no such product
              exists. Recall that a *piece* is a word that occurs in two
              distinct positions (possibly overlapping) of the words in the
              suffix tree ``u``.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (str) -- the possible subword

              :Returns: A value of type ``bool``.

              :Complexity:
                Linear in the length of ``w``.

              .. warning::
                This function does no checks on its arguments whatsoever. In
                particular, if the word ``w`` contains any of the unique letters
                appended to the end of any existing word in the tree, then bad
                things will happen.
             )pbdoc")
        .def(
            "number_of_pieces",
            [](Ukkonen const& u, word_type const& w) {
              return ukkonen::number_of_pieces(u, w);
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Find the number of pieces in a decomposition of a word (if any).

              Returns minimum number of pieces whose product equals the word
              ``w`` if such a product exists, and ``0`` if no such product
              exists. Recall that a *piece* is a word that occurs in two
              distinct positions (possibly overlapping) of the words in the
              suffix tree ``u``.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (List[int]) -- the possible subword

              :Returns: A value of type ``bool``.

              :Complexity:
                Linear in the length of ``w``.

              :Raises:
                ``RunTimeError`` if :py:meth:`Ukkonen.validate_word` raises.
             )pbdoc")
        .def(
            "number_of_pieces",
            [](Ukkonen const& u, std::string const& w) {
              return ukkonen::number_of_pieces(u, w);
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Find the number of pieces in a decomposition of a word (if any).

              Returns minimum number of pieces whose product equals the word
              ``w`` if such a product exists, and ``0`` if no such product
              exists. Recall that a *piece* is a word that occurs in two
              distinct positions (possibly overlapping) of the words in the
              suffix tree ``u``.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (str) -- the possible subword

              :Returns: A value of type ``bool``.

              :Complexity:
                Linear in the length of ``w``.

              :Raises:
                ``RunTimeError`` if :py:meth:`Ukkonen.validate_word` raises.
             )pbdoc")
        .def(
            "pieces_no_checks",
            [](Ukkonen const& u, word_type const& w) {
              return ukkonen::number_of_pieces_no_checks(u, w);
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Find the pieces in a decomposition of a word (if any).

              Returns a list of the pieces whose product equals the word corresponding
              to ``first`` and \p last if such a product exists, and an empty \vector if
              no such product exists. Recall that a *piece* is a word that occurs in two
              distinct positions (possibly overlapping) of the words in the suffix tree
              ``u``.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (List[int]) -- the possible subword

              :Returns: A value of type ``List[List[int]]``.

              :Complexity:
                Linear in the length of ``w``.

              .. warning::
                This function does no checks on its arguments whatsoever. In
                particular, if the word ``w`` contains any of the unique letters
                appended to the end of any existing word in the tree, then bad
                things will happen.
             )pbdoc")
        .def(
            "pieces_no_checks",
            [](Ukkonen const& u, std::string const& w) {
              return ukkonen::pieces_no_checks(u, w);
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Find the pieces in a decomposition of a word (if any).

              Returns a list of the pieces whose product equals the word corresponding
              to ``first`` and \p last if such a product exists, and an empty \vector if
              no such product exists. Recall that a *piece* is a word that occurs in two
              distinct positions (possibly overlapping) of the words in the suffix tree
              ``u``.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (str) -- the possible subword

              :Returns: A value of type ``List[List[int]]``.

              :Complexity:
                Linear in the length of ``w``.

              .. warning::
                This function does no checks on its arguments whatsoever. In
                particular, if the word ``w`` contains any of the unique letters
                appended to the end of any existing word in the tree, then bad
                things will happen.
             )pbdoc")
        .def(
            "pieces",
            [](Ukkonen const& u, word_type const& w) {
              return ukkonen::pieces(u, w);
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Find the pieces in a decomposition of a word (if any).

              Returns a list of the pieces whose product equals the word corresponding
              to ``first`` and \p last if such a product exists, and an empty \vector if
              no such product exists. Recall that a *piece* is a word that occurs in two
              distinct positions (possibly overlapping) of the words in the suffix tree
              ``u``.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (List[int]) -- the possible subword

              :Returns: A value of type ``List[List[int]]``.

              :Complexity:
                Linear in the length of ``w``.

              :Raises:
                ``RunTimeError`` if :py:meth:`Ukkonen.validate_word` raises.
             )pbdoc")
        .def(
            "pieces",
            [](Ukkonen const& u, std::string const& w) {
              return ukkonen::pieces(u, w);
            },
            py::arg("u"),
            py::arg("w"),
            R"pbdoc(
              Find the pieces in a decomposition of a word (if any).

              Returns a list of the pieces whose product equals the word corresponding
              to ``first`` and \p last if such a product exists, and an empty \vector if
              no such product exists. Recall that a *piece* is a word that occurs in two
              distinct positions (possibly overlapping) of the words in the suffix tree
              ``u``.

              :Parameters: - **u** (:py:class:`Ukkonen`) -- the :py:class:`Ukkonen` object
                           - **w** (str) -- the possible subword

              :Returns: A value of type ``List[List[int]]``.

              :Complexity:
                Linear in the length of ``w``.

              :Raises:
                ``RunTimeError`` if :py:meth:`Ukkonen.validate_word` raises.
             )pbdoc");
  }
}  // namespace libsemigroups
