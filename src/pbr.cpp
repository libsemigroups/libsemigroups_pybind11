//
// libsemigroups_pybind
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

// libsemigroups headers
#include <libsemigroups/pbr.hpp>

// pybind11....
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_PBR

namespace py = pybind11;

namespace libsemigroups {

  void init_PBR(py::module& m) {
    py::class_<PBR> thing(m,
                          "PBR",
                          R"pbdoc(
Class for representing PBRs.

*Partitioned binary relations* (PBRs) are a
generalisation of a bipartitions, and were introduced by Martin and Mazorchuk in
:cite:`Martin2011aa`.
)pbdoc");
    thing.def("__repr__",
              py::overload_cast<PBR const&>(&to_human_readable_repr));
    thing.def(py::self < py::self,
              py::arg("that"),
              R"pbdoc(
Compare for less.

:param that: a PBR to compare with.
:type that: PBR

:returns:  ``True`` if ``self`` is less than ``that``, and ``False`` otherwise.
:rtype: bool

:complexity: At worst linear in :any:`degree`.
    )pbdoc");
    thing.def(py::self == py::self,
              py::arg("that"),
              R"pbdoc(
Compare two PBRs for equality.

:param that: a PBR to compare with.
:type that: PBR

:returns:  ``True`` if ``self`` equals ``that``, and ``False`` otherwise.
:rtype: bool

:complexity: At worst linear in :any:`degree`.
)pbdoc");
    // TODO(later) uncomment when there is safe multiplication
    //     thing.def(py::self * py::self,
    //               py::arg("that"),
    //               R"pbdoc(
    // Multiply two PBRs.

    // Returns a newly constructed PBR equal to the product of ``self`` and
    // *that*.

    // :param that: a PBR.
    // type that: PBR

    // :returns: ``sel`` * *that*
    // :rtype: PBR

    // :complexity:
    // Cubic in degree().
    // )pbdoc");
    thing.def("__copy__", [](PBR const& self) { return PBR(self); });
    thing.def(
        "copy",
        [](PBR const& self) { return PBR(self); },
        R"pbdoc(
Copy a PBR object.

:returns: A copy.
:rtype: PBR
)pbdoc");
    thing.def(py::init<size_t>(),
              py::arg("n"),
              R"pbdoc(
Construct empty PBR of given degree.

:param n: the degree
:type n: int
)pbdoc");
    thing.def(py::init([](PBR::vector_type<int32_t> left,
                          PBR::vector_type<int32_t> right) {
                return to_pbr(left, right);
              }),
              py::arg("left"),
              py::arg("right"),
              R"pbdoc(
Construct from adjacencies ``1`` to ``n`` and ``-1`` to ``-n``.

Construct from adjacencies ``1`` to ``n`` and ``-1`` to ``-n``. The parameters
*left* and *right* should be containers of ``n`` vectors of integer values,
so that the vector in position ``i`` of *left* is the list of points
adjacent to ``i`` in the :any:`PBR`, and the vector in position ``i`` of
*right* is the list of points adjacent to ``n + i`` in the :any:`PBR` .
A negative value ``i`` corresponds to ``n - i``.

:param left: container of adjacencies of ``1`` to ``n`` 
:type left: List[List[int]]

:param right: container of adjacencies of ``n + 1`` to ``2n``.
:type right: List[List[int]]

:raises LibsemigroupsError: if the resultant PBR:

    *  would not describe a binary relation on an even number of points; or
    *  would have a point related to a point that is greater than :any:`degree`;
)pbdoc");
    thing.def(py::init([](PBR::vector_type<uint32_t> x) { return to_pbr(x); }),
              py::arg("x"),
              R"pbdoc(
Construct from adjacencies ``0`` to ``2n - 1``.

Construct from adjacencies ``0`` to ``2n - 1``. The parameter *x* must be a
container of vectors of ``int`` with size ``2n`` for some integer ``n``,
and the vector in position ``i`` is the list of points adjacent to ``i``
in the :any:`PBR` constructed.

:param x: the container of vectors of adjacencies.
:type x: List[List[int]]

:raises LibsemigroupsError: if the resultant PBR:

    *  would not describe a binary relation on an even number of points; or
    *  would have a point related to a point that is greater than :any:`degree`;
    *  *x* contains a list of points related to a point that is not sorted.

)pbdoc");
    thing.def("degree",
              &PBR::degree,
              R"pbdoc(
Returns the degree of a :any:`PBR`.

Returns the degree of a :any:`PBR`, where the *degree* of a :any:`PBR` is half
the number of points in the :any:`PBR`.

:returns: The degree of the PBR.
:rtype: int

:complexity: Constant.
)pbdoc");
    thing.def("__hash__", &PBR::hash_value, py::is_operator());
    thing.def("number_of_points",
              &PBR::number_of_points,
              R"pbdoc(
Returns the number of points of a :any:`PBR`.

:returns: A value of type ``int``.
:rtype: int

:complexity: Constant.
)pbdoc");
    thing.def(
        "__getitem__",
        [](const PBR& a, size_t b) { return a[b]; },
        py::arg("i"),
        py::is_operator(),
        R"pbdoc(
Returns the nodes adjacent to the given node.

:param i: an integer
:type i: int

:returns: The nodes adjavent to *i*.
:rtype: int
)pbdoc");

    // Helper namespace

    m.def(
        "pbr_one",
        [](PBR const& x) { return pbr::one(x); },
        py::arg("x"),
        R"pbdoc(
Returns the identity PBR with degree ``x.degree()``.

This member function returns a new :any:`PBR` with degree equal to the :any:`PBR.degree` of
*x*, where every value is adjacent to its negative. Equivalently, ``i`` is
adjacent ``i + n`` and vice versa for every ``i`` less than the degree
``n``.

:param x: The identity.
:type x: PBR

:returns: The identity.
:rtype: PBR
)pbdoc");
    m.def(
        "pbr_one",
        [](size_t n) { return pbr::one(n); },
        py::arg("n"),
        R"pbdoc(
Returns the identity PBR with specified degree.

This function returns a new :any:`PBR` with degree equal to *n* where every
value is adjacent to its negative. Equivalently, ``i`` is adjacent
``i + n`` and vice versa for every ``i`` less than the degree ``n``.

:param n: the degree.
:type n: int

:returns: The identity.
:rtype: PBR
)pbdoc");
  }  // init_PBR

}  // namespace libsemigroups
