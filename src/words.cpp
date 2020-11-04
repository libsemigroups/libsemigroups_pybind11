//
// libsemigroups - C++ library for semigroups and monoids
// Copyright (C) 2020 James D. Mitchell
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

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <libsemigroups/libsemigroups.hpp>

#include "main.hpp"

namespace py = pybind11;

namespace libsemigroups {
  void init_words(py::module &m) {
    ////////////////////////////////////////////////////////////////////////
    // siso.hpp
    ////////////////////////////////////////////////////////////////////////

    m.def(
        "silo",
        [](std::string const &alphabet,
           size_t             upper_bound,
           std::string const &first,
           std::string const &last) {
          return py::make_iterator(
              cbegin_silo(alphabet, upper_bound, first, last),
              cend_silo(alphabet, upper_bound, first, last));
        },
        py::arg("alphabet"),
        py::arg("upper_bound"),
        py::arg("first"),
        py::arg("last"),
        R"pbdoc(
        Returns an iterator to strings in lexicographic order (silo).

        :param alphabet: the alphabet
        :type alphabet: str
        :param upper_bound: the maximum length of string to return
        :type upper_bound: int
        :param first: the first string
        :type first: str
        :param last: one past the last string
        :type last: str

        :return: An iterator to strings in lexicographic order over the alphabet ``alphabet`` of length at most ``upper_bound`` in the range ``[first, last)``.

        Example
        -------
        .. code-block:: python

           [x for x in silo("ba", 3, "b", "aaa")]
           # ["b", "bb", "ba", "a", "ab", "aa"]

      )pbdoc");

    m.def(
        "sislo",
        [](std::string const &alphabet,
           std::string const &first,
           std::string const &last) {
          return py::make_iterator(cbegin_sislo(alphabet, first, last),
                                   cend_sislo(alphabet, first, last));
        },
        py::arg("alphabet"),
        py::arg("first"),
        py::arg("last"),
        R"pbdoc(
        Returns an iterator to strings in short-lex order (sislo).

        :param alphabet: the alphabet
        :type alphabet: str
        :param first: the first string
        :type first: str
        :param last: one past the last string
        :type last: str

        :return: An iterator to strings over the alphabet ``alphabet`` in short-lex order and in the range ``[first, last)``.

        Example
        -------
        .. code-block:: python

           [x for x in sislo("ba", "b", "aaa")]
           # ['b', 'a', 'bb', 'ba', 'ab', 'aa', 'bbb', 'bba', 'bab', 'baa', 'abb', 'aba', 'aab']
        )pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // wilo.hpp
    ////////////////////////////////////////////////////////////////////////

    m.def(
        "wilo",
        [](size_t const     n,
           size_t const     upper_bound,
           word_type const &first,
           word_type const &last) {
          return py::make_iterator(cbegin_wilo(n, upper_bound, first, last),
                                   cend_wilo(n, upper_bound, first, last));
        },
        py::arg("n"),
        py::arg("upper_bound"),
        py::arg("first"),
        py::arg("last"),
        R"pbdoc(
      Returns an iterator to words in lexicographic order (wilo).

      :param n: the number of letters
      :type n: int
      :param upper_bound: the maximum length of string to return
      :type upper_bound: int
      :param first: the first word
      :type first: list
      :param last: one past the last word
      :type last: list

      :return: An iterator to words in lexicographic order over an alphabet with  ``n`` letters of length at most ``upper_bound`` in the range ``[first, last)``.

      Example
      -------
      .. code-block:: python

         [x for x in wilo(2, 3, [0], [1, 1, 1])]
         # [[0], [0, 0], [0, 1], [1], [1, 0], [1, 1]]
      )pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // wislo.hpp
    ////////////////////////////////////////////////////////////////////////

    m.def(
        "wislo",
        [](size_t const n, word_type const &first, word_type const &last) {
          return py::make_iterator(cbegin_wislo(n, first, last),
                                   cend_wislo(n, first, last));
        },
        py::arg("n"),
        py::arg("first"),
        py::arg("last"),
        R"pbdoc(
        Returns an iterator to strings in short-lex order (sislo).

        :param n: the number of letter
        :type n: int
        :param first: the first word
        :type first: list
        :param last: one past the last word
        :type last: list

        :return: An iterator to words over an alphabet with ``n`` in short-lex order and in the range ``[first, last)``.

        Example
        -------
        .. code-block:: python

           [x for x in wislo(2, [0], [1, 1])]
           # [[0], [1], [0, 0], [0, 1], [1, 0]]
        )pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // words.hpp
    ////////////////////////////////////////////////////////////////////////

    m.def("number_of_words",
          &number_of_words,
          py::arg("n"),
          py::arg("min"),
          py::arg("max"),
          R"pbdoc(
          Returns the number of words over a given alphabet in some range.

          :param n: the number of letter
          :type n: int
          :param min: the minimum length of a word
          :type min: int
          :param max: one greater than the maximum length of a word
          :type max: int

          :return: The number words over an alphabet with ``n`` with length in the range ``[min, max)``.

          Example
          -------
          .. code-block:: python

             number_of_words(2, 0, 10)
             # 1023
          )pbdoc");
  }
}  // namespace libsemigroups
