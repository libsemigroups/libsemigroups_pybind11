//
// libsemigroups - C++ library for semigroups and monoids
// Copyright (C) 2021 James D. Mitchell
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
#include <iosfwd>            // for string
#include <vector>            // for vector

// libsemigroups....

#include <libsemigroups/paths.hpp>  // for Path
#include <libsemigroups/types.hpp>  // for word_type
#include <libsemigroups/words.hpp>  // for number_of_words

// pybind11....
#include <pybind11/pybind11.h>  // for make_iterator, module
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_words

namespace py = pybind11;

namespace libsemigroups {
  void init_words(py::module& m) {
    /*
      ////////////////////////////////////////////////////////////////////////
      // siso.hpp
      ////////////////////////////////////////////////////////////////////////

      m.def(
          "silo",
          [](std::string const& alphabet,
             size_t             upper_bound,
             std::string const& first,
             std::string const& last) {
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

          :return:
            An iterator to strings in lexicographic order over the alphabet
            ``alphabet`` of length at most ``upper_bound`` in the range
    ``[first, last)``.

          .. doctest::

             >>> from libsemigroups_pybind11 import silo
             >>> list(silo("ba", 3, "b", "aaa"))
             ['b', 'bb', 'ba', 'a', 'ab', 'aa']
          )pbdoc");

      m.def(
          "sislo",
          [](std::string const& alphabet,
             std::string const& first,
             std::string const& last) {
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

            :return:
              An iterator to strings over the alphabet ``alphabet`` in short-lex
              order and in the range ``[first, last)``.

            .. doctest::

               >>> from libsemigroups_pybind11 import sislo
               >>> list(sislo("ba", "b", "aaa"))
               ['b', 'a', 'bb', 'ba', 'ab', 'aa', 'bbb', 'bba', 'bab', 'baa',
    'abb', 'aba', 'aab'] )pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // wilo.hpp
      ////////////////////////////////////////////////////////////////////////

      m.def(
          "wilo",
          [](size_t const     n,
             size_t const     upper_bound,
             word_type const& first,
             word_type const& last) {
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

            :return:
              An iterator to words in lexicographic order over an alphabet with
              ``n`` letters of length at most ``upper_bound`` in the range
              ``[first, last)``.

            .. doctest::

               >>> from libsemigroups_pybind11 import wilo
               >>> list(wilo(2, 3, [0], [1, 1, 1]))
               [[0], [0, 0], [0, 1], [1], [1, 0], [1, 1]]
          )pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // wislo.hpp
      ////////////////////////////////////////////////////////////////////////

      m.def(
          "wislo",
          [](size_t const n, word_type const& first, word_type const& last) {
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

            :return:
              An iterator to words over an alphabet with ``n`` in short-lex
    order and in the range ``[first, last)``.

            .. doctest::

               >>> from libsemigroups_pybind11 import wislo
               >>> list(wislo(2, [0], [1, 1]))
               [[0], [1], [0, 0], [0, 1], [1, 0]]
          )pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // words.hpp
      ////////////////////////////////////////////////////////////////////////

  */
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

            :return:
              The number words over an alphabet with ``n`` with length in the
              range ``[min, max)``.

            .. doctest::

               >>> from libsemigroups_pybind11 import number_of_words
               >>> number_of_words(2, 0, 10)
               1023
          )pbdoc")
        .def("random_word",
             &random_word,
             py::arg("length"),
             py::arg("nr_letters"),
             R"pbdoc(
            Returns a random word.

            :param length: the length of the word
            :type length: int
            :param nr_letters: the size of the alphabet
            :type length: int

            :return: A random word on ``[0, ..., n - 1]`` of length ``length``
              where ``n`` is ``nr_letters``.

            :Raises: ``RunTimeError`` if ``nr_letters`` is ``0``.

            .. seealso:: :py:func:`random_string`.
          )pbdoc")
        .def("parse_relations",
             py::overload_cast<char const*>(&literals::operator""_p),
             py::arg("w"),
             R"pbdoc(
             TODO
          )pbdoc")
        .def("to_word",
             py::overload_cast<std::string_view>(&to_word),
             py::arg("sv"),
             R"pbdoc(
             TODO
          )pbdoc")
        .def(
            "to_string",
            [](std::string_view alphabet, word_type const& input) {
              return to_string(alphabet, input);
            },
            py::arg("alphabet"),
            py::arg("input"),
            R"pbdoc(
             TODO
          )pbdoc");

    py::class_<libsemigroups::Words>(m, "Words")
        .def(py::init<>())
        .def("init",
             &libsemigroups::Words::init,
             R"pbdoc(
               Initialize an existing Words object.

               :return: A reference to this.
               )pbdoc")
        .def(py::init<Words const&>())
        .def(
            "__iter__",
            [](Words const& w) {
              return py::make_iterator(w.begin(), w.end());
            },
            py::keep_alive<0, 1>())
        .def("get",
             &libsemigroups::Words::get,
             R"pbdoc(
               Get the current value.

               :return: A value of type output_type.
               )pbdoc")
        .def("next",
             &libsemigroups::Words::next,
             R"pbdoc(
               Advance to the next value.

               )pbdoc")
        .def("at_end",
             &libsemigroups::Words::at_end,
             R"pbdoc(
               Check if the range object is exhausted.

               :return: A value of type bool.
               )pbdoc")
        .def("size_hint",
             &libsemigroups::Words::size_hint,
             R"pbdoc(
               The possible size of the range.

               :return: A value of type size_t.
               )pbdoc")
        .def("count",
             &libsemigroups::Words::count,
             R"pbdoc(
               The actual size of the range.

               :return: A value of type size_t.
               )pbdoc")
        .def(
            "number_of_letters",
            py::overload_cast<size_t>(&libsemigroups::Words::number_of_letters),
            py::arg("n"),
            R"pbdoc(
               Set the number of letters in the alphabet.

               :Parameters: **n** (??) - the number of letters.

               :Returns: A reference to this.
               )pbdoc")
        .def("number_of_letters",
             py::overload_cast<>(&libsemigroups::Words::number_of_letters,
                                 py::const_),
             R"pbdoc(
               The current number of letters in the alphabet.


               :Returns: A value of type size_t.
               )pbdoc")
        .def("first",
             py::overload_cast<word_type const&>(&libsemigroups::Words::first),
             py::arg("frst"),
             R"pbdoc(
               Set the first word in the range.

               :Parameters: **frst** (??) - the first word.

               :Returns: A reference to this.
               )pbdoc")
        .def("first",
             py::overload_cast<>(&libsemigroups::Words::first, py::const_),
             R"pbdoc(
               The current first word in the range.


               :Returns: A const reference to a word_type.
               )pbdoc")
        .def("last",
             py::overload_cast<word_type const&>(&libsemigroups::Words::last),
             py::arg("lst"),
             R"pbdoc(
               Set one past the last word in the range.

               :Parameters: **lst** (??) - the first word.

               :Returns: A reference to this.
               )pbdoc")
        .def("last",
             py::overload_cast<>(&libsemigroups::Words::last, py::const_),
             R"pbdoc(
               The current one past the last word in the range.


               :Returns: A const reference to a word_type.
               )pbdoc")
        .def("order",
             py::overload_cast<Order>(&libsemigroups::Words::order),
             py::arg("val"),
             R"pbdoc(
               Set the order of the words in the range.

               :Parameters: **val** (??) - the order.

               :Returns: A reference to this.
               )pbdoc")
        .def("order",
             py::overload_cast<>(&libsemigroups::Words::order, py::const_),
             R"pbdoc(
               The current order of the words in the range.


               :Returns: A value of type Order.
               )pbdoc")
        .def("upper_bound",
             py::overload_cast<size_t>(&libsemigroups::Words::upper_bound),
             py::arg("n"),
             R"pbdoc(
               Set an upper bound for the length of a word in the range.

               :Parameters: **n** (??) - the upper bound.

               :Returns: A reference to this.
               )pbdoc")
        .def(
            "upper_bound",
            py::overload_cast<>(&libsemigroups::Words::upper_bound, py::const_),
            R"pbdoc(
               The current upper bound on the length of a word in the range.


               :Returns: A value of type size_t.
               )pbdoc")
        .def("min",
             &libsemigroups::Words::min,
             py::arg("val"),
             R"pbdoc(
               Set the first word in the range by length.

               :param val: the exponent.
               :type val: ??

               :return: A reference to this.
               )pbdoc")
        .def("max",
             &libsemigroups::Words::max,
             py::arg("val"),
             R"pbdoc(
               Set one past the last word in the range by length.

               :param val: the exponent.
               :type val: ??

               :return: A reference to this.
               )pbdoc");

    py::class_<libsemigroups::ToWord>(m, "ToWord")
        .def(py::init<>())
        .def("init",
             py::overload_cast<>(&libsemigroups::ToWord::init),
             R"pbdoc(
               Initialize an existing ToWord object.

               :Returns: A reference to this.
               )pbdoc")
        .def(py::init<std::string const&>())
        .def(
            "init",
            py::overload_cast<std::string const&>(&libsemigroups::ToWord::init),
            py::arg("alphabet"),
            R"pbdoc(
               Initialize an existing ToWord object.

               :param alphabet: the alphabet.

               :Returns: A reference to ``self``.
               )pbdoc")
        .def(py::init<ToWord const&>())
        .def("empty",
             &libsemigroups::ToWord::empty,
             R"pbdoc(
               Check if the alphabet is defined.

               :return: A value of type bool.
               )pbdoc")
        .def("__call__",
             py::overload_cast<std::string const&>(
                 &libsemigroups::ToWord::operator(), py::const_),
             R"pbdoc(
TODO
               )pbdoc");

    py::class_<libsemigroups::Strings>(m, "Strings")
        .def(py::init<>())
        .def("init",
             &libsemigroups::Strings::init,
             R"pbdoc(
               Initialize an existing Strings object.

               :return: A reference to this.
               )pbdoc")
        .def(py::init<Strings const&>())
        .def("get",
             &libsemigroups::Strings::get,
             R"pbdoc(
               Get the current value.

               :return: A value of type output_type.
               )pbdoc")
        .def(
            "__iter__",
            [](Strings const& w) {
              return py::make_iterator(w.begin(), w.end());
            },
            py::keep_alive<0, 1>())
        .def("next",
             &libsemigroups::Strings::next,
             R"pbdoc(
               Advance to the next value.

               )pbdoc")
        .def("at_end",
             &libsemigroups::Strings::at_end,
             R"pbdoc(
               Check if the range object is exhausted.

               :return: A value of type bool.
               )pbdoc")
        .def("size_hint",
             &libsemigroups::Strings::size_hint,
             R"pbdoc(
               The possible size of the range.

               :return: A value of type size_t.
               )pbdoc")
        .def("count",
             &libsemigroups::Strings::count,
             R"pbdoc(
               The actual size of the range.

               :return: A value of type size_t.
               )pbdoc")
        .def("alphabet",
             py::overload_cast<std::string const&>(
                 &libsemigroups::Strings::alphabet),
             py::arg("x"),
             R"pbdoc(
               Set the alphabet.

               :Parameters: **x** (??) - the alphabet.

               :Returns: A reference to this.
               )pbdoc")
        .def("alphabet",
             py::overload_cast<>(&libsemigroups::Strings::alphabet, py::const_),
             R"pbdoc(
               The current alphabet.

               :Returns: A value of type std::string.
               )pbdoc")
        .def("first",
             py::overload_cast<std::string const&>(
                 &libsemigroups::Strings::first),
             py::arg("frst"),
             R"pbdoc(
               Set the first string in the range.

               :Parameters: **frst** (??) - the first string.

               :Returns: A reference to this.
               )pbdoc")
        .def("first",
             py::overload_cast<>(&libsemigroups::Strings::first, py::const_),
             R"pbdoc(
               The current first string in the range.


               :Returns: A std::string by value.
               )pbdoc")
        .def("last",
             py::overload_cast<std::string const&>(
                 &libsemigroups::Strings::last),
             py::arg("lst"),
             R"pbdoc(
               Set one past the last string in the range.

               :Parameters: **lst** (??) - the first string.

               :Returns: A reference to this.
               )pbdoc")
        .def("last",
             py::overload_cast<>(&libsemigroups::Strings::last, py::const_),
             R"pbdoc(
               The current one past the last string in the range.


               :Returns: A std::string by value.
               )pbdoc")
        .def("order",
             py::overload_cast<Order>(&libsemigroups::Strings::order),
             py::arg("val"),
             R"pbdoc(
               Set the order of the strings in the range.

               :Parameters: **val** (??) - the order.

               :Returns: A reference to this.
               )pbdoc")
        .def("order",
             py::overload_cast<>(&libsemigroups::Strings::order, py::const_),
             R"pbdoc(
               The current order of the strings in the range.


               :Returns: A value of type Order.
               )pbdoc")
        .def("upper_bound",
             py::overload_cast<size_t>(&libsemigroups::Strings::upper_bound),
             py::arg("n"),
             R"pbdoc(
               Set an upper bound for the length of a string in the range.

               :Parameters: **n** (??) - the upper bound.

               :Returns: A reference to this.
               )pbdoc")
        .def("upper_bound",
             py::overload_cast<>(&libsemigroups::Strings::upper_bound,
                                 py::const_),
             R"pbdoc(
               The current upper bound on the length of a string in the range.

               :Returns: A value of type ``int``.
               )pbdoc")
        .def("min",
             &libsemigroups::Strings::min,
             py::arg("val"),
             R"pbdoc(
               Set the first string in the range by length.

               :param val: the exponent.
               :type val: ??

               :return: A reference to this.
               )pbdoc")
        .def("max",
             &libsemigroups::Strings::max,
             py::arg("val"),
             R"pbdoc(
               Set one past the last string in the range by length.

               :param val: the exponent.
               :type val: ??

               :return: A reference to this.
               )pbdoc");

    using node_type = uint32_t;
    py::class_<ToStrings>(m, "ToStrings").def(py::init<std::string const&>());

    py::class_<ToStrings::Range<Paths<node_type>>>(m, "ToStringsRange1")
        .def(py::init<Paths<node_type> const&, ToStrings const&>())
        .def("get", &ToStrings::Range<Paths<node_type>>::get)
        .def("__getitem__", &ToStrings::Range<Paths<node_type>>::get)
        .def("next", &ToStrings::Range<Paths<node_type>>::next)
        .def("at_end", &ToStrings::Range<Paths<node_type>>::at_end)
        .def("__len__", &ToStrings::Range<Paths<node_type>>::size_hint);

    py::class_<ToStrings::Range<ReversiblePaths<node_type>>>(m,
                                                             "ToStringsRange2")
        .def(py::init<ReversiblePaths<node_type> const&, ToStrings const&>())
        .def("get", &ToStrings::Range<ReversiblePaths<node_type>>::get)
        .def("__getitem__", &ToStrings::Range<ReversiblePaths<node_type>>::get)
        .def("next", &ToStrings::Range<ReversiblePaths<node_type>>::next)
        .def("at_end", &ToStrings::Range<ReversiblePaths<node_type>>::at_end)
        .def("__len__",
             &ToStrings::Range<ReversiblePaths<node_type>>::size_hint)
        .def("__iter__",
             [](ToStrings::Range<ReversiblePaths<node_type>> const& ts) {
               return py::make_iterator(rx::begin(ts), rx::end(ts));
             });
  }
}  // namespace libsemigroups
