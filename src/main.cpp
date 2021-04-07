//
// libsemigroups - C++ library for semigroups and monoids
// Copyright (C) 2020 James D. Mitchell
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

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <libsemigroups/libsemigroups.hpp>

#include "main.hpp"

namespace py = pybind11;

namespace libsemigroups {

  using Transf256            = Transformation<uint8_t>;
  using FroidurePinTransf256 = FroidurePin<Transf256>;

  namespace detail {
    std::string transf_repr(Transformation<uint8_t> const &a) {
      auto out = std::string("Transf256([");
      for (size_t i = 0; i < a.degree(); ++i) {
        out += detail::to_string(size_t(a[i]));
        if (i != a.degree() - 1) {
          out += ", ";
        }
      }
      out += "])";
      return out;
    }

    template <typename T>
    void bind_froidure_pin(py::module &m, std::string typestr) {
      using Class              = FroidurePin<T>;
      std::string pyclass_name = std::string("FroidurePin") + typestr;
      py::class_<Class>(
          m, pyclass_name.c_str(), py::buffer_protocol(), py::dynamic_attr())
          .def(py::init<std::vector<T> const &>())
          .def(py::init<>())
          .def("size", &Class::size)
          .def("add_generator", &Class::add_generator)
          .def("number_of_generators", &Class::nr_generators)
          .def("batch_size", py::overload_cast<size_t>(&Class::batch_size))
          .def("batch_size",
               py::overload_cast<>(&Class::batch_size, py::const_))
          .def("max_threads", py::overload_cast<size_t>(&Class::max_threads))
          .def("max_threads",
               py::overload_cast<>(&Class::max_threads, py::const_))
          .def("concurrency_threshold",
               py::overload_cast<size_t>(&Class::concurrency_threshold))
          .def("concurrency_threshold",
               py::overload_cast<>(&Class::concurrency_threshold, py::const_))
          .def("reserve", &Class::reserve);
    }
  }  // namespace detail

  PYBIND11_MODULE(libsemigroups_pybind11, m) {
    // py::options options;
    // options.disable_function_signatures();

    ////////////////////////////////////////////////////////////////////////
    // Enums
    ////////////////////////////////////////////////////////////////////////

    py::enum_<congruence_type>(m, "congruence_type")
        .value("left", congruence_type::left)
        .value("right", congruence_type::right)
        .value("twosided", congruence_type::twosided)
        .export_values();

    py::enum_<tril>(m, "tril")
        .value("true", tril::TRUE)
        .value("false", tril::FALSE)
        .value("unknown", tril::unknown)
        .export_values();

    py::class_<ReportGuard>(m, "ReportGuard").def(py::init<bool>());

    init_action_digraph(m);
    init_bmat8(m);
    init_cong(m);
    init_fpsemi(m);
    init_knuth_bendix(m);
    init_todd_coxeter(m);
    init_words(m);

    ////////////////////////////////////////////////////////////////////////
    // element.hpp
    ////////////////////////////////////////////////////////////////////////

    py::class_<Transformation<uint8_t>>(m, "Transf256")
        .def(py::init<std::vector<uint8_t> const &>())
        .def("rank",
             &Transformation<uint8_t>::crank,
             R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc")
        .def("degree", &Transformation<uint8_t>::degree)
        .def(py::self * py::self)
        .def(
            "__getitem__",
            [](const Transformation<uint8_t> &a, size_t b) { return a[b]; },
            py::is_operator())
        .def("__repr__", &detail::transf_repr);

    detail::bind_froidure_pin<Transf256>(m, "Transf256");
    detail::bind_froidure_pin<BMat8>(m, "BMat8");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
  }
}  // namespace libsemigroups
