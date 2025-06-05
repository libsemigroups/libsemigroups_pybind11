//
// libsemigroups_pybind11
// Copyright (C) 2024 James D. Mitchell
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
#include <libsemigroups/adapters.hpp>
#include <libsemigroups/bmat8.hpp>
#include <libsemigroups/transf.hpp>

// pybind11....
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_imagerightaction

namespace libsemigroups {
  namespace py = pybind11;

  namespace {
    template <typename Element, typename Point>
    void bind_imagerightaction(py::module& m, std::string const& name) {
      using ImageRightAction_ = ImageRightAction<Element, Point>;

      py::class_<ImageRightAction_>(m,
                                    name.c_str(),
                                    R"pbdoc(
Adapter for computing the image of a point under a right action.

This class provides a call operator with signature ``(pt: Point, x: Element) ->
Point``, returning the image of the point *pt* under the right action of the
element *x*.

.. doctest::

    >>> from libsemigroups_pybind11 import PPerm, ImageRightAction
    >>> x = PPerm([0, 1, 2, 3, 5, 6, 9], [9, 7, 3, 5, 4, 2, 1], 10)
    >>> pt = PPerm([0, 1, 2, 3, 5, 6], [0, 1, 2, 3, 5, 6], 10)
    >>> func = ImageRightAction(point=pt, element=x)
    >>> func(pt, x)
    PPerm([2, 3, 4, 5, 7, 9], [2, 3, 4, 5, 7, 9], 10)
    >>> func(_, x)
    PPerm([1, 3, 4, 5], [1, 3, 4, 5], 10)
    >>> func(_, x)
    PPerm([4, 5, 7], [4, 5, 7], 10)
    >>> func(_, x)
    PPerm([4], [4], 10)
    >>> func(_, x)
    PPerm([], [], 10)
    >>> func(_, x)
    PPerm([], [], 10)
)pbdoc")
          // The next constructor isn't available in python so no doc.
          .def(py::init<>())
          // The following doesn't yet work because mostly it's not possible to
          // change <res> in place.
          // .def("__call__",
          //      [](ImageRightAction_ const& self,
          //         Point&                   res,
          //         Point const&             pt,
          //         Element const&           x) -> void { self(res, pt, x); })
          .def(
              "__call__",
              [](ImageRightAction_ const& self,
                 Point const&             pt,
                 Element const&           x) -> Point {
                // Copy pt, to ensure that pt and res have the same degree
                Point res = pt;
                self(res, pt, x);
                return res;
              },
              py::arg("pt"),
              py::arg("x"),
              R"pbdoc(
:sig=(self: ImageRightAction, pt: Point, x: Element) -> Point:

Return the image of a point acted on by an element.

This call operator returns the image of *pt* acted on by *x*.

:param pt: the point on which to act.
:type pt: Point

:param x: the element doing the acting.
:type x: Element

:returns: The image of *pt* acted on by *x*.
:rtype: Point

:raises TypeError:
    If the wrapped C++ type of the sample objects passed via *x* and
    *point* are not the same as the wrapped types of the arguments in any
    invocation of the call operator. For example, if *point* is ``PPerm([], [],
    256)``, then the underlying C++ type uses 8-bit integers to store image
    values. So, any partial permutation passed as the 1st argument to the
    call operator must be of degree at most ``256``.
)pbdoc");
    }  // bind_imagerightaction

    template <typename Element, typename Point>
    void bind_imageleftaction(py::module& m, std::string const& name) {
      using ImageLeftAction_ = ImageLeftAction<Element, Point>;

      py::class_<ImageLeftAction_>(m,
                                   name.c_str(),
                                   R"pbdoc(
Adapter for computing the image of a point under a left action.

This class provides a call operator with signature ``(pt: Point, x: Element) ->
Point``, returning the image of the point *pt* under the left action of the
element *x*.

.. doctest::

    >>> from libsemigroups_pybind11 import PPerm, ImageLeftAction
    >>> x = PPerm([0, 1, 2, 3, 5, 6, 9], [9, 7, 3, 5, 4, 2, 1], 10)
    >>> pt = PPerm([0, 1, 2, 3, 5, 6], [0, 1, 2, 3, 5, 6], 10)
    >>> func = ImageLeftAction(point=pt, element=x)
    >>> func(pt, x)
    PPerm([2, 3, 6, 9], [2, 3, 6, 9], 10)
)pbdoc")
          // The next constructor isn't available in python so no doc.
          .def(py::init<>())
          // The following doesn't yet work because mostly it's not possible to
          // change <res> in place.
          //          .def("__call__",
          //               [](ImageLeftAction_ const& self,
          //                  Point&                  res,
          //                  Point const&            pt,
          //                  Element const&          x) { self(res, pt, x); })
          .def(
              "__call__",
              [](ImageLeftAction_ const& self,
                 Point const&            pt,
                 Element const&          x) {
                // Copy pt, to ensure that pt and res have the same degree
                Point res = pt;
                self(res, pt, x);
                return res;
              },
              py::arg("pt"),
              py::arg("x"),
              R"pbdoc(
:sig=(self: ImageLeftAction, pt: Point, x: Element) -> Point:

Return the image of a point acted on by an element.

This call operator returns the image of *pt* acted on by *x*.

:param pt: the point on which to act.
:type pt: Point

:param x: the element doing the acting.
:type x: Element

:returns: The image of *pt* acted on by *x*.
:rtype: Point

:raises TypeError:
    If the wrapped C++ type of the sample objects passed via *x* and
    *point* are not the same as the wrapped types of the arguments in any
    invocation of the call operator. For example, if *point* is ``PPerm([], [],
    256)``, then the underlying C++ type uses 8-bit integers to store image
    values. So, any partial permutation passed as the 1st argument to the
    call operator must be of degree at most ``256``.
)pbdoc");
    }  // bind_imageleftaction

  }  // namespace

  void init_imagerightaction(py::module& m) {
    // One call to bind is required per list of types
    bind_imagerightaction<BMat8, BMat8>(m, "ImageRightActionBMat8BMat8");
    bind_imageleftaction<BMat8, BMat8>(m, "ImageLeftActionBMat8BMat8");

    bind_imagerightaction<PPerm<0, uint8_t>, PPerm<0, uint8_t>>(
        m, "ImageRightActionPPerm1PPerm1");
    bind_imageleftaction<PPerm<0, uint8_t>, PPerm<0, uint8_t>>(
        m, "ImageLeftActionPPerm1PPerm1");

    using point_type = typename PPerm<0, uint8_t>::point_type;
    // FIXME this doesn't work, since the reference argument doesn't get passed
    bind_imagerightaction<PPerm<0, uint8_t>, std::vector<point_type>>(
        m, "ImageRightActionPPerm1List");
  }

}  // namespace libsemigroups
