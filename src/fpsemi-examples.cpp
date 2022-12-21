//
// libsemigroups_pybind11
// Copyright (C) 2022 Murray T. Whyte
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
#include <stddef.h>  // for size_t
#include <stdint.h>  // for int32_t, uint32_t

// C++ stl headers....
#include <initializer_list>  // for initializer_list
#include <vector>            // for vector

// libsemigroups....
#include <libsemigroups/fpsemi-examples.hpp>  // for monogenic_semigroup, ...
#include <libsemigroups/present.hpp>          // for Presentation
#include <libsemigroups/string.hpp>           // for to_string
#include <libsemigroups/types.hpp>            // for word_type

// pybind11....
#include <pybind11/pybind11.h>  // for class_, init, module
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_present

namespace py = pybind11;
namespace libsemigroups {
  void init_fpsemi_examples(py::module& m) {
    py::enum_<fpsemigroup::author>(m, "author")
        .value("Machine", fpsemigroup::author::Machine)
        .value("Aizenstat", fpsemigroup::author::Aizenstat)
        .value("Burnside", fpsemigroup::author::Burnside)
        .value("Carmichael", fpsemigroup::author::Carmichael)
        .value("Coxeter", fpsemigroup::author::Coxeter)
        .value("Easdown", fpsemigroup::author::Easdown)
        .value("East", fpsemigroup::author::East)
        .value("FitzGerald", fpsemigroup::author::FitzGerald)
        .value("Godelle", fpsemigroup::author::Godelle)
        .value("Guralnick", fpsemigroup::author::Guralnick)
        .value("Iwahori", fpsemigroup::author::Iwahori)
        .value("Kantor", fpsemigroup::author::Kantor)
        .value("Kassabov", fpsemigroup::author::Kassabov)
        .value("Lubotzky", fpsemigroup::author::Lubotzky)
        .value("Miller", fpsemigroup::author::Miller)
        .value("Moore", fpsemigroup::author::Moore)
        .value("Moser", fpsemigroup::author::Moser)
        .value("Sutov", fpsemigroup::author::Sutov)
        .def("__add__", &fpsemigroup::operator+);

    m.def("make_presentation", &fpsemigroup::make<Presentation<word_type>>)
        .def("symmetric_group",
             &fpsemigroup::symmetric_group,
             py::arg("n").noconvert(),
             py::arg("val") = fpsemigroup::author::Carmichael,
             py::arg("index") = 0)
        .def("alternating_group",
             &fpsemigroup::alternating_group,
             py::arg("n").noconvert(),
             py::arg("val") = fpsemigroup::author::Moore)
        .def("full_transformation_monoid",
             &fpsemigroup::full_transformation_monoid,
             py::arg("n").noconvert(),
             py::arg("val") = fpsemigroup::author::Iwahori)
        .def("partial_transformation_monoid",
             &fpsemigroup::partial_transformation_monoid,
             py::arg("n").noconvert(),
             py::arg("val") = fpsemigroup::author::Sutov)
        .def("symmetric_inverse_monoid",
             &fpsemigroup::symmetric_inverse_monoid,
             py::arg("n").noconvert(),
             py::arg("val") = fpsemigroup::author::Sutov)
        .def("dual_symmetric_inverse_monoid",
             &fpsemigroup::dual_symmetric_inverse_monoid,
             py::arg("n").noconvert(),
             py::arg("val") = fpsemigroup::author::Easdown
                              + fpsemigroup::author::East
                              + fpsemigroup::author::FitzGerald)
        .def("uniform_block_bijection_monoid",
             &fpsemigroup::uniform_block_bijection_monoid,
             py::arg("n").noconvert(),
             py::arg("val") = fpsemigroup::author::FitzGerald)
        .def("partition_monoid",
             &fpsemigroup::partition_monoid,
             py::arg("n").noconvert(),
             py::arg("val") = fpsemigroup::author::East)
        .def("brauer_monoid",
             &fpsemigroup::brauer_monoid,
             py::arg("n").noconvert())
        .def("rectangular_band",
             &fpsemigroup::rectangular_band,
             py::arg("m").noconvert(),
             py::arg("n").noconvert())
        .def("stellar_monoid",
             &fpsemigroup::stellar_monoid,
             py::arg("n").noconvert())
        .def("chinese_monoid",
             &fpsemigroup::chinese_monoid,
             py::arg("n").noconvert())
        .def("monogenic_semigroup",
             &fpsemigroup::monogenic_semigroup,
             py::arg("m").noconvert(),
             py::arg("r").noconvert())
        .def("plactic_monoid",
             &fpsemigroup::plactic_monoid,
             py::arg("n").noconvert())
        .def("stylic_monoid",
             &fpsemigroup::stylic_monoid,
             py::arg("l").noconvert())
        .def("fibonacci_semigroup",
             &fpsemigroup::fibonacci_semigroup,
             py::arg("r").noconvert(),
             py::arg("n").noconvert())
        .def("temperley_lieb_monoid",
             &fpsemigroup::temperley_lieb_monoid,
             py::arg("n").noconvert())
        .def("singular_brauer_monoid",
             &fpsemigroup::singular_brauer_monoid,
             py::arg("n").noconvert())
        .def("orientation_preserving_monoid",
             &fpsemigroup::orientation_preserving_monoid,
             py::arg("n").noconvert())
        .def("orientation_reversing_monoid",
             &fpsemigroup::orientation_reversing_monoid,
             py::arg("n").noconvert());
  }
}  // namespace libsemigroups
