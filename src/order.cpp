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

// C std headers....

// C++ stl headers....

// libsemigroups....
#include <libsemigroups/order.hpp>  // for order
#include <libsemigroups/types.hpp>  // for order

// pybind11....
#include <pybind11/pybind11.h>  // for class_, make_iterator, init, enum_
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_paths

namespace libsemigroups {
  namespace py = pybind11;

  void init_order(py::module& m) {
    py::options options;
    options.disable_enum_members_docstring();

    py::enum_<Order>(m, "Order", R"pbdoc(
An enum class for the possible orderings of words and strings.

The values in this enum can be used as the arguments for functions such as
:any:`ToddCoxeter.standardize` or :any:`WordRange.order` to specify which
ordering should be used. The normal forms for congruence classes are given with
respect to one of the orders specified by the values in this enum.

The valid values are:

.. py:attribute:: Order.none
  :value: <Order.none: 0>

  No ordering

.. py:attribute:: Order.shortlex
  :value: <Order.shortlex: 1>

  The short-lex ordering. Word are first ordered by length, and then
  lexicographically.

.. py:attribute:: Order.lex
  :value: <Order.lex: 2>

  The lexicographic ordering. Note that this is not a well-order, so there may
  not be a lexicographically least word in a given congruence class of words.

.. py:attribute:: Order.recursive
  :value: <Order.recursive: 3>

  The recursive-path ordering, as described in :cite:`Jantzen2012aa` (Definition
  1.2.14, page 24).
)pbdoc")
        .value("none", Order::none)
        .value("shortlex", Order::shortlex)
        .value("lex", Order::lex)
        .value("recursive", Order::recursive);

    // No prefix because not in subpackage
    m.def(
        "lexicographical_compare",
        [](std::string const& x, std::string const& y) {
          return lexicographical_compare(x, y);
        },
        py::arg("x"),
        py::arg("y"),
        R"pbdoc(
:sig=(x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two values of type :any:`str` or ``list[int]`` using using lexicographical ordering.

:param x: the first object for comparison.
:type x: str | list[int]

:param y: the second object for comparison.
:type y: str | list[int]

:returns: The boolean value ``True`` if *x* is lexicographically less than *y*, and ``False`` otherwise.
:rtype: bool

:complexity: At most :math:`O(n)` where :math:`n` is the minimum of the length of *x* and the length of *y*.
)pbdoc");

    // No prefix because not in subpackage
    m.def(
        "lexicographical_compare",
        [](word_type const& x, word_type const& y) {
          return lexicographical_compare(x, y);
        },
        py::arg("x"),
        py::arg("y"),
        R"pbdoc(
:sig=(x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
)pbdoc");

    // No prefix because not in subpackage
    m.def(
        "shortlex_compare",
        [](std::string const& x, std::string const& y) {
          return shortlex_compare(x, y);
        },
        py::arg("x"),
        py::arg("y"),
        R"pbdoc(
:sig=(x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two values of type :any:`str` or ``list[int]`` using shortlex ordering.

:param x: the first object for comparison.
:type x: str | list[int]

:param y: the second object for comparison.
:type y: str | list[int]

:returns: The boolean value ``True`` if *x`* is short-lex less than *y*, and ``False`` otherwise.
:rtype: bool

:complexity: At most :math:`O(n)` where :math:`n` is the minimum of the length of *x* and the length of *y*.
)pbdoc");

    // No prefix because not in subpackage
    m.def(
        "shortlex_compare",
        [](word_type const& x, word_type const& y) {
          return shortlex_compare(x, y);
        },
        py::arg("x"),
        py::arg("y"),
        R"pbdoc(
:sig=(x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
)pbdoc");

    // No prefix because not in subpackage
    m.def(
        "recursive_path_compare",
        [](std::string const& x, std::string const& y) {
          return recursive_path_compare(x, y);
        },
        py::arg("x"),
        py::arg("y"),
        R"pbdoc(
:sig=(x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two values of type :any:`str` or ``list[int]`` using recursive-path ordering.

Compare two values of type :any:`str` or ``list[int]`` using the recursive path comparison described in :cite:`Jantzen2012aa` (Definition 1.2.14, page 24).

If :math:`u, v\in X ^ {*}`, :math:`u \neq v`, and :math:`u = a'u`,
:math:`v = bv'` for some :math:`a,b \in X`, :math:`u',v'\in X ^ {*}`, then
:math:`u > v` if one of the following conditions holds:

#. :math:`a = b` and :math:`u' \geq v'`;
#. :math:`a > b` and :math:`u  > v'`;
#. :math:`b > a` and :math:`u' > v`.

This documentation and the implementation of :any:`recursive_path_compare`
is based on the source code of :cite:`Holt2018aa`.

:param x: the first object for comparison.
:type x: str | list[int]

:param y: the second object for comparison.
:type y: str | list[int]

:returns: The boolean value ``True`` if *x* is less than *y* with respect to the recursive path ordering, and ``False`` otherwise.
:rtype: bool

:warning: This function has significantly worse performance than :any:`shortlex_compare` and :any:`lexicographical_compare`.
)pbdoc");

    // No prefix because not in subpackage
    m.def(
        "recursive_path_compare",
        [](word_type const& x, word_type const& y) {
          return recursive_path_compare(x, y);
        },
        py::arg("x"),
        py::arg("y"),
        R"pbdoc(
:sig=(x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
)pbdoc");
  }
}  // namespace libsemigroups
