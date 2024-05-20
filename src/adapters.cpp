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

// libsemigroups_pybind11....
#include "main.hpp"  // for init_TODO

namespace py = pybind11;

namespace libsemigroups {

  namespace {
    template <typename Element, typename Point>
    void bind_imagerightaction(py::module& m, std::string const& name) {
      using ImageRightAction_ = ImageRightAction<Element, Point>;

      py::class_<ImageRightAction_>(m, name.c_str())
          .def(py::init<>())
          .def("__call__",
               [](ImageRightAction_ const& self,
                  Point&                   res,
                  Point const&             pt,
                  Element const&           x) { self(res, pt, x); });
    }  // bind_imagerightaction

    template <typename Element, typename Point>
    void bind_imageleftaction(py::module& m, std::string const& name) {
      using ImageLeftAction_ = ImageLeftAction<Element, Point>;

      py::class_<ImageLeftAction_>(m, name.c_str())
          .def(py::init<>())
          .def("__call__",
               [](ImageLeftAction_ const& self,
                  Point&                  res,
                  Point const&            pt,
                  Element const&          x) { self(res, pt, x); });
    }  // bind_imageleftaction

  }  // namespace

  void init_imagerightaction(py::module& m) {
    // One call to bind is required per list of types
    bind_imagerightaction<BMat8, BMat8>(m, "ImageRightActionBMat8BMat8");
    bind_imageleftaction<BMat8, BMat8>(m, "ImageLeftActionBMat8BMat8");
    bind_imagerightaction<PPerm<16>, PPerm<16>>(
        m, "ImageRightActionPPerm16PPerm16");
    bind_imageleftaction<PPerm<16>, PPerm<16>>(m,
                                               "ImageLeftActionPPerm16PPerm16");
  }

}  // namespace libsemigroups
