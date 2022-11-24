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
        .export_values()
        .def("__add__", &fpsemigroup::operator+);

    m.def("make_presentation", &fpsemigroup::make<Presentation<word_type>>)
        .def("_symmetric_group",
             &fpsemigroup::symmetric_group,
             py::arg("n"),
             py::arg("val") = fpsemigroup::author::Carmichael)
        .def("alternating_group", &fpsemigroup::alternating_group)
        .def("full_transformation_monoid",
             &fpsemigroup::full_transformation_monoid)
        .def("partial_transformation_monoid",
             &fpsemigroup::partial_transformation_monoid)
        .def("symmetric_inverse_monoid", &fpsemigroup::symmetric_inverse_monoid)
        .def("dual_symmetric_inverse_monoid",
             &fpsemigroup::dual_symmetric_inverse_monoid)
        .def("uniform_block_bijection_monoid",
             &fpsemigroup::uniform_block_bijection_monoid)
        .def("partition_monoid", &fpsemigroup::partition_monoid)
        .def("brauer_monoid", &fpsemigroup::brauer_monoid)
        .def("rectangular_band", &fpsemigroup::rectangular_band)
        .def("stellar_monoid", &fpsemigroup::stellar_monoid)
        .def("chinese_monoid", &fpsemigroup::chinese_monoid)
        .def("monogenic_semigroup", &fpsemigroup::monogenic_semigroup)
        .def("plactic_monoid", &fpsemigroup::plactic_monoid)
        .def("stylic_monoid", &fpsemigroup::stylic_monoid)
        .def("fibonacci_semigroup", &fpsemigroup::fibonacci_semigroup)
        .def("temperley_lieb_monoid", &fpsemigroup::temperley_lieb_monoid)
        .def("singular_brauer_monoid", &fpsemigroup::singular_brauer_monoid)
        .def("orientation_preserving_monoid",
             &fpsemigroup::orientation_preserving_monoid)
        .def("orientation_reversing_monoid",
             &fpsemigroup::orientation_reversing_monoid);
  }
}  // namespace libsemigroups
