//
// libsemigroups_pybind11
// Copyright (C) 2023-2026 Murray T. Whyte
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
#include <string>  // for string

// libsemigroups....
#include <libsemigroups/order.hpp>  // for *_cmp, Order
#include <libsemigroups/types.hpp>  // for word_type

// pybind11....
#include <pybind11/pybind11.h>  // for arg, enum_, module, options
#include <pybind11/stl.h>       // for automatic conversions

// libsemigroups_pybind11....
#include "main.hpp"  // for init_order

namespace libsemigroups {
  namespace py = pybind11;

  namespace {
    void warn_deprecated(char const* old_name, char const* new_name) {
      std::string message = old_name;
      message += " is deprecated, and will be removed from "
                 "libsemigroups_pybind11 in v2. Instead, use ";
      message += new_name;
      message += ".";
      if (PyErr_WarnEx(PyExc_DeprecationWarning, message.c_str(), 2) < 0) {
        throw py::error_already_set();
      }
    }

    template <typename Word, typename Compare, typename AlphabetCompare>
    void bind_compare(py::module&     m,
                      char const*     name,
                      Compare         compare,
                      AlphabetCompare alphabet_compare,
                      char const*     doc) {
      m.def(
          name,
          [compare](Word const& x, Word const& y) { return compare(x, y); },
          py::arg("x"),
          py::arg("y"),
          doc);
      m.def(
          name,
          [alphabet_compare](
              Alphabet<Word> const& alphabet, Word const& x, Word const& y) {
            return alphabet_compare(alphabet, x, y);
          },
          py::arg("alphabet"),
          py::arg("x"),
          py::arg("y"),
          doc);
    }

    template <typename Word, typename Compare>
    void bind_deprecated_compare(py::module& m,
                                 char const* old_name,
                                 char const* new_name,
                                 Compare     compare,
                                 char const* doc) {
      m.def(
          old_name,
          [old_name, new_name, compare](Word const& x, Word const& y) {
            warn_deprecated(old_name, new_name);
            return compare(x, y);
          },
          py::arg("x"),
          py::arg("y"),
          doc);
    }

    template <typename Word>
    void bind_order_comparisons(py::module& m) {
      bind_compare<Word>(
          m,
          "lex_cmp",
          [](Word const& x, Word const& y) { return lex_cmp(x, y); },
          [](Alphabet<Word> const& alphabet, Word const& x, Word const& y) {
            return lex_cmp(alphabet, x, y);
          },
          R"pbdoc(
:sig=(x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words lexicographically.

The three-argument overload ``lex_cmp(alphabet, x, y)`` compares letters by
their positions in *alphabet*. It raises :any:`LibsemigroupsError` if either
word contains a letter that does not belong to *alphabet*.

:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool
)pbdoc");

      bind_compare<Word>(
          m,
          "lenlex_cmp",
          [](Word const& x, Word const& y) { return lenlex_cmp(x, y); },
          [](Alphabet<Word> const& alphabet, Word const& x, Word const& y) {
            return lenlex_cmp(alphabet, x, y);
          },
          R"pbdoc(
:sig=(x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using len-lex ordering.

Words are first ordered by length and then lexicographically. The
three-argument overload ``lenlex_cmp(alphabet, x, y)`` compares letters by
their positions in *alphabet*. It raises :any:`LibsemigroupsError` if either
word contains a letter that does not belong to *alphabet*.

:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool
)pbdoc");

      bind_compare<Word>(
          m,
          "rpo_cmp",
          [](Word const& x, Word const& y) { return rpo_cmp(x, y); },
          [](Alphabet<Word> const& alphabet, Word const& x, Word const& y) {
            return rpo_cmp(alphabet, x, y);
          },
          R"pbdoc(
:sig=(x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using recursive-path ordering.

The three-argument overload ``rpo_cmp(alphabet, x, y)`` compares letters by
their positions in *alphabet*. It raises :any:`LibsemigroupsError` if either
word contains a letter that does not belong to *alphabet*.

:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

.. warning::
   This function has significantly worse performance than :any:`lenlex_cmp`
   and :any:`lex_cmp`.
)pbdoc");

      bind_compare<Word>(
          m,
          "rev_rpo_cmp",
          [](Word const& x, Word const& y) { return rev_rpo_cmp(x, y); },
          [](Alphabet<Word> const& alphabet, Word const& x, Word const& y) {
            return rev_rpo_cmp(alphabet, x, y);
          },
          R"pbdoc(
:sig=(x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using reversed recursive-path ordering.

This is recursive-path ordering applied after reading both words from right to
left. The three-argument overload ``rev_rpo_cmp(alphabet, x, y)`` compares
letters by their positions in *alphabet*. It raises
:any:`LibsemigroupsError` if either word contains a letter that does not belong
to *alphabet*.

:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

.. warning::
   This function has significantly worse performance than :any:`lenlex_cmp`
   and :any:`lex_cmp`.
)pbdoc");

      bind_deprecated_compare<Word>(
          m,
          "lexicographical_compare",
          "lex_cmp",
          [](Word const& x, Word const& y) { return lex_cmp(x, y); },
          R"pbdoc(
:sig=(x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words lexicographically.

:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

.. deprecated:: 1.5
   This will be removed from ``libsemigroups_pybind11`` in v2. Instead, use
   :any:`lex_cmp`.
)pbdoc");

      bind_deprecated_compare<Word>(
          m,
          "shortlex_compare",
          "lenlex_cmp",
          [](Word const& x, Word const& y) { return lenlex_cmp(x, y); },
          R"pbdoc(
:sig=(x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using len-lex ordering.

:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

.. deprecated:: 1.5
   This will be removed from ``libsemigroups_pybind11`` in v2. Instead, use
   :any:`lenlex_cmp`.
)pbdoc");

      bind_deprecated_compare<Word>(
          m,
          "recursive_path_compare",
          "rev_rpo_cmp",
          [](Word const& x, Word const& y) { return rev_rpo_cmp(x, y); },
          R"pbdoc(
:sig=(x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using reversed recursive-path ordering.

:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

.. deprecated:: 1.5
   This will be removed from ``libsemigroups_pybind11`` in v2. Instead, use
   :any:`rev_rpo_cmp`.
)pbdoc");
    }
  }  // namespace

  void init_order(py::module& m) {
    py::options options;
    options.disable_enum_members_docstring();

    py::enum_<Order>(m, "Order", R"pbdoc(
An enum class for the possible orderings of words and strings.

The values in this enum can be used as the arguments for functions such as
:any:`ToddCoxeter.standardize` or :any:`WordRange.order` to specify which
ordering should be used.

The values :any:`Order.shortlex` and :any:`Order.recursive` are retained for
backwards compatibility; use :any:`Order.lenlex` and :any:`Order.rev_rpo`,
respectively, in new code.

.. py:attribute:: Order.none
  :value: <Order.left: 0>

   No ordering

.. py:attribute:: Order.lenlex
  :value: <Order.right: 1>

    The len-lex ordering. Words are first ordered by length, and then
    lexicographically.

.. py:attribute:: Order.shortlex
  :value: <Order.left: 2>

    The short-lex ordering. Words are first ordered by length, and then
    lexicographically.

    This is deprecated; use :any:`Order.lenlex` instead.

.. py:attribute:: Order.lex
  :value: <Order.right: 3>

    The lexicographic ordering. Note that this is not a well-order, so there
    may not be a lexicographically least word in a given congruence class of
    words.

.. py:attribute:: Order.rpo
  :value: <Order.left: 4>

    The recursive-path ordering, as described in :cite:`Jantzen2012aa`
    (Definition 1.2.14, page 24).

.. py:attribute:: Order.rev_rpo
  :value: <Order.right: 5>

    The reversed recursive-path ordering, based on the description in
    :cite:`Jantzen2012aa` (Definition 1.2.14, page 24), where words are read
    right-to-left before ordering.

.. py:attribute:: Order.recursive
  :value: <Order.right: 6>

    The recursive-path ordering, as described in :cite:`Jantzen2012aa`
    (Definition 1.2.14, page 24).
  
    This is deprecated; use :any:`Order.rpo` instead.
)pbdoc")
        .value("none", Order::none)
        .value("lenlex", Order::lenlex)
        .value("shortlex", Order::lenlex)
        .value("lex", Order::lex)
        .value("rpo", Order::rpo)
        .value("rev_rpo", Order::rev_rpo)
        .value("recursive", Order::rev_rpo);

    bind_order_comparisons<std::string>(m);
    bind_order_comparisons<word_type>(m);
  }
}  // namespace libsemigroups
