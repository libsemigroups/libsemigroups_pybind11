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

namespace py = pybind11;

namespace libsemigroups {

  namespace {
    template <typename Element, typename Point>
    void bind_imagerightaction(py::module& m, std::string const& name) {
      using ImageRightAction_ = ImageRightAction<Element, Point>;

      py::class_<ImageRightAction_>(m,
                                    name.c_str(),
                                    R"pbdoc(
Adapter for computing the image of a point under a right action.

:Keyword Arguments:
  * *Element* -- the type of the elements in the action
  * *Point* -- the type of the points acted on

This class provides call operators of the following signatures:

1. ``(res: Point, pt: Point, x: Element)``
2. ``(pt: Point, x: Element)``

In form (1): the call operator changes *res* in-place to contain the
image of the point *pt* under the right action of the element *x*. The
purpose of the 1st parameter is to avoid repeated allocations of memory to hold
temporary points that are discarded soon after they are created.

In form (2): the call operator returns the image of the point *pt* under the
right action of the element *x*.

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
          .def(py::init<>())
          // The following doesn't yet work because mostly it's not possible to
          // change <res> in place.
          // .def("__call__",
          //      [](ImageRightAction_ const& self,
          //         Point&                   res,
          //         Point const&             pt,
          //         Element const&           x) -> void { self(res, pt, x); })
          .def("__call__",
               [](ImageRightAction_ const& self,
                  Point const&             pt,
                  Element const&           x) -> Point {
                 // Copy pt, to ensure that pt and res have the same degree
                 Point res = pt;
                 self(res, pt, x);
                 return res;
               });
    }  // bind_imagerightaction

    template <typename Element, typename Point>
    void bind_imageleftaction(py::module& m, std::string const& name) {
      using ImageLeftAction_ = ImageLeftAction<Element, Point>;

      py::class_<ImageLeftAction_>(m,
                                   name.c_str(),
                                   R"pbdoc(
Adapter for computing the image of a point under a left action.

:Keyword Arguments:
  * *Element* -- the type of the elements in the action
  * *Point* -- the type of the points acted on

This class provides call operators of the following signatures:

1. ``(res: Point, pt: Point, x: Element)``
2. ``(pt: Point, x: Element)``

In form (1): the call operator changes *res* in-place to contain the
image of the point *pt* under the left action of the element *x*. The
purpose of the 1st parameter is to avoid repeated allocations of memory to hold
temporary points that are discarded soon after they are created.

In form (2): the call operator returns the image of the point *pt* under the
left action of the element *x*.

.. doctest::

    >>> from libsemigroups_pybind11 import PPerm, ImageLeftAction
    >>> x = PPerm([0, 1, 2, 3, 5, 6, 9], [9, 7, 3, 5, 4, 2, 1], 10)
    >>> pt = PPerm([0, 1, 2, 3, 5, 6], [0, 1, 2, 3, 5, 6], 10)
    >>> func = ImageLeftAction(point=pt, element=x)
    >>> func(pt, x)
    PPerm([2, 3, 6, 9], [2, 3, 6, 9], 10)
)pbdoc")
          .def(py::init<>())
          // The following doesn't yet work because mostly it's not possible to
          // change <res> in place.
          //          .def("__call__",
          //               [](ImageLeftAction_ const& self,
          //                  Point&                  res,
          //                  Point const&            pt,
          //                  Element const&          x) { self(res, pt, x); })
          .def("__call__",
               [](ImageLeftAction_ const& self,
                  Point const&            pt,
                  Element const&          x) {
                 // Copy pt, to ensure that pt and res have the same degree
                 Point res = pt;
                 self(res, pt, x);
                 return res;
               });
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
