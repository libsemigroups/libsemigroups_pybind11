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
#include <libsemigroups/exception.hpp>
#include <libsemigroups/pbr.hpp>

// pybind11....
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_pbr

namespace libsemigroups {

  namespace py = pybind11;
  void init_pbr(py::module& m) {
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
    thing.def(py::init<size_t>(),
              py::arg("n"),
              R"pbdoc(
:sig=(self: PBR, n: int) -> None:

Construct empty PBR of given degree.

:param n: the degree
:type n: int
)pbdoc");
    thing.def(py::init([](PBR::vector_type<int32_t> left,
                          PBR::vector_type<int32_t> right) {
                return make<PBR>(left, right);
              }),
              py::arg("left"),
              py::arg("right"),
              R"pbdoc(
:sig=(self: PBR, left: list[list[int]], right: list[list[int]]) -> None:       

Construct from adjacencies ``1`` to ``n`` and ``-1`` to ``-n``.

Construct from adjacencies ``1`` to ``n`` and ``-1`` to ``-n``. The parameters
*left* and *right* should be containers of ``n`` lists of integer values, so
that the list in position ``i`` of *left* is the list of points adjacent to
``i`` in the :any:`PBR`, and the list in position ``i`` of *right* is the
list of points adjacent to ``n + i`` in the :any:`PBR`. A negative value ``i``
corresponds to ``n - i``.

:param left: container of adjacencies of ``1`` to ``n``
:type left: list[list[int]]

:param right: container of adjacencies of ``n + 1`` to ``2n``.
:type right: list[list[int]]

:raises LibsemigroupsError: if the resultant PBR:

    *  would not describe a binary relation on an even number of points; or
    *  would have a point related to a point that is greater than :any:`PBR.degree`;
)pbdoc");
    thing.def(
        py::init([](PBR::vector_type<uint32_t> x) { return make<PBR>(x); }),
        py::arg("x"),
        R"pbdoc(
:sig=(self: PBR, x: list[list[int]]) -> None:

Construct from adjacencies ``0`` to ``2n - 1``.

Construct from adjacencies ``0`` to ``2n - 1``. The parameter *x* must be a
container of lists of ``int`` with size ``2n`` for some integer ``n``,
and the list in position ``i`` is the list of points adjacent to ``i``
in the :any:`PBR` constructed.

:param x: the container of lists of adjacencies.
:type x: list[list[int]]

:raises LibsemigroupsError: if the resultant PBR:

    *  would not describe a binary relation on an even number of points; or
    *  would have a point related to a point that is greater than :any:`PBR.degree`;
    *  *x* contains a list of points related to a point that is not sorted.

)pbdoc");
    thing.def(py::self < py::self,
              py::arg("that"),
              R"pbdoc(
Compare for less.

:param that: a PBR to compare with.
:type that: PBR

:returns:  ``True`` if *self* is less than *that*, and ``False`` otherwise.
:rtype: bool

:complexity: At worst linear in :any:`PBR.degree`.
    )pbdoc");
    thing.def(py::self == py::self,
              py::arg("that"),
              R"pbdoc(
Compare two PBRs for equality.

:param that: a PBR to compare with.
:type that: PBR

:returns:  ``True`` if *self* equals *that*, and ``False`` otherwise.
:rtype: bool

:complexity: At worst linear in :any:`PBR.degree`.
)pbdoc");
    thing.def(
        "__mul__",
        [](PBR const& a, PBR const& b) {
          if (a.degree() != b.degree()) {
            LIBSEMIGROUPS_EXCEPTION("the degree of the first argument "
                                    "({}) is not equal to the degree "
                                    "of the second argument ({})",
                                    a.degree(),
                                    b.degree());
          }
          return a * b;
        },
        py::arg("that"),
        R"pbdoc(
Multiply two PBRs.

Returns a newly constructed PBR equal to the product of *self* and
*that*.

:param that: a PBR.
type that: PBR

:returns: *self* * *that*
:rtype: PBR

:complexity:
Cubic in :any:`PBR.degree`.
)pbdoc");
    thing.def("__ge__", [](PBR const& a, PBR const& b) { return a >= b; });
    thing.def("__gt__", [](PBR const& a, PBR const& b) { return a > b; });
    thing.def("__le__", [](PBR const& a, PBR const& b) { return a <= b; });
    thing.def("__ne__", [](PBR const& a, PBR const& b) { return a != b; });
    thing.def("__copy__", [](PBR const& self) { return PBR(self); });
    thing.def("__hash__", &PBR::hash_value, py::is_operator());
    thing.def(
        "__getitem__",
        [](const PBR& a, size_t b) { return a.at(b); },
        py::arg("i"),
        py::is_operator(),
        R"pbdoc(
Returns the nodes adjacent to the given node.

:param i: an integer
:type i: int

:returns: The nodes adjacent to *i*.
:rtype: int

:raises LibsemigroupsError: if *i* >= :any:`number_of_nodes`.

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
    thing.def("number_of_points",
              &PBR::number_of_points,
              R"pbdoc(
Returns the number of points of a :any:`PBR`.

:returns: A value of type ``int``.
:rtype: int

:complexity: Constant.
)pbdoc");
    thing.def("product_inplace",
              &PBR::product_inplace,
              py::arg("x"),
              py::arg("y"),
              py::arg("thread_id") = 0,
              R"pbdoc(
:sig=(self: PBR, x: PBR, y: PBR, thread_id: int) -> None:
Multiply two PBR objects and store the product in *self*.

The parameter *thread_id* is required since some temporary storage is
required to find the product of *x* and *y*.  Note that if different
threads call this member function with the same value of *thread_id*
then bad things will happen.

:param x: the left multiplicand.
:type x: PBR
:param y: the right multiplicand.
:type y: PBR
:param thread_id: the index of the calling thread (defaults to ``0``).
:type thread_id: int

:raises LibsemigroupsError: if:
  * the :any:`PBR.degree` of *x* is not the same as the :any:`PBR.degree` of *y*;
  * the :any:`PBR.degree` of *self* is not the same as the :any:`PBR.degree` of *x*; or
  * either *x* or *y* is the same object as `self`.
)pbdoc");
    thing.def(
        "copy",
        [](PBR const& self) { return PBR(self); },
        R"pbdoc(
Copy a :any:`PBR` object.

:returns: A copy.
:rtype: PBR
)pbdoc");

    // Helper namespace

    m.def(
        "pbr_one",
        [](PBR const& x) { return pbr::one(x); },
        py::arg("x"),
        R"pbdoc(
:sig=(x: PBR) -> PBR:

Returns the identity PBR with degree ``x.degree()``.

This member function returns a new :any:`PBR` with degree equal to the :any:`PBR.degree` of
*x*, where every value is adjacent to its negative. Equivalently, ``i`` is
adjacent ``i + n`` and vice versa for every ``i`` less than the degree
``n``.

:param x: A PBR.
:type x: PBR

:returns: The identity.
:rtype: PBR
)pbdoc");
    m.def(
        "pbr_one",
        [](size_t n) { return pbr::one(n); },
        py::arg("n"),
        R"pbdoc(
:sig=(n: int) -> PBR:

Returns the identity PBR with specified degree.

This function returns a new :any:`PBR` with degree equal to *n* where every
value is adjacent to its negative. Equivalently, ``i`` is adjacent
``i + n`` and vice versa for every ``i`` less than the degree ``n``.

:param n: the degree.
:type n: int

:returns: The identity.
:rtype: PBR
)pbdoc");
  }  // init_pbr

}  // namespace libsemigroups
