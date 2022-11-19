//
// libsemigroups_pybind11
// Copyright (C) 2021 James D. Mitchell
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

// C++ stl headers....
#include <cstdint>           // for uint16_t, uint32_t, uint8_t
#include <initializer_list>  // for initializer_list
#include <iosfwd>            // for string
#include <string>            // for basic_string
#include <vector>            // for vector

// libsemigroups....
#include <libsemigroups/string.hpp>  // for to_string, string_format
#include <libsemigroups/transf.hpp>  // for PPerm, Transf, Perm, LeastPPerm

// pybind11....
#include <pybind11/operators.h>  // for self, self_t, operator!=, operator*
#include <pybind11/pybind11.h>   // for module, class_, make_iterator
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_transf

namespace py = pybind11;

namespace libsemigroups {

  namespace {
    using detail::string_format;

    template <typename T>
    std::string transf_repr(T const& a) {
      auto out = std::string("Transf");
      out += "([";
      for (size_t i = 0; i < a.degree(); ++i) {
        out += detail::to_string(size_t(a[i]));
        if (i != a.degree() - 1) {
          out += ", ";
        }
      }
      out += "])";
      return out;
    }

    // This is the main function that installs common methods for derived
    // classes of PTransf
    template <typename T, typename S>
    void bind_ptransf(S& x) {
      using container_type = typename T::container_type const&;

      x.def(
           "__getitem__",
           [](const T& a, size_t b) { return a[b]; },
           py::is_operator())
          .def(pybind11::self != pybind11::self)
          .def(pybind11::self <= pybind11::self)
          .def(pybind11::self > pybind11::self)
          .def(pybind11::self >= pybind11::self)
          .def(pybind11::self == pybind11::self)
          .def(pybind11::self < pybind11::self)
          .def(py::self * py::self)
          .def("degree", [](T const& x) { return x.degree(); })
          .def("images",
               [](T const& x) {
                 return py::make_iterator(x.cbegin(), x.cend());
               })
          .def_static("make", &T::template make<container_type>)
          .def("identity", py::overload_cast<>(&T::identity, py::const_))
          .def_static("make_identity", py::overload_cast<size_t>(&T::identity))
          .def("rank", &T::rank)
          .def("product_inplace", &T::product_inplace);
    }

    template <typename T>
    void bind_transf(py::module& m, char const* name) {
      py::class_<T> x(m, name);
      bind_ptransf<T>(x);
      x.def("__repr__", [](T const& x) { return transf_repr(x); });
    }

    template <typename T>
    void bind_pperm(py::module& m, char const* name) {
      using value_type = typename T::value_type;
      py::class_<T> x(m, name);
      bind_ptransf<T>(x);

      x.def_static("make",
                   [](std::vector<value_type> const& dom,
                      std::vector<value_type> const& ran,
                      size_t const deg) { return T::make(dom, ran, deg); })
          .def("right_one", &T::right_one)
          .def("left_one", &T::left_one)
          .def("inverse", py::overload_cast<>(&T::inverse, py::const_))
          .def("inverse", py::overload_cast<T&>(&T::inverse, py::const_))
          .def("undef", &T::undef);
    }

    template <typename T, typename S>
    void bind_perm(py::module& m, char const* name) {
      py::class_<T, S> x(m, name);
      bind_ptransf<T>(x);
      x.def("inverse", &T::inverse);
    }
  }  // namespace

  void init_transf(py::module& m) {
    // Transformations
    bind_transf<LeastTransf<16>>(m, "Transf16");
    bind_transf<Transf<0, uint8_t>>(m, "Transf1");
    bind_transf<Transf<0, uint16_t>>(m, "Transf2");
    bind_transf<Transf<0, uint32_t>>(m, "Transf4");

    // Partial perms
    bind_pperm<LeastPPerm<16>>(m, "PPerm16");
    bind_pperm<PPerm<0, uint8_t>>(m, "PPerm1");
    bind_pperm<PPerm<0, uint16_t>>(m, "PPerm2");
    bind_pperm<PPerm<0, uint32_t>>(m, "PPerm4");

    // Perms
    bind_perm<LeastPerm<16>, LeastTransf<16>>(m, "Perm16");
    bind_perm<Perm<0, uint8_t>, Transf<0, uint8_t>>(m, "Perm1");
    bind_perm<Perm<0, uint16_t>, Transf<0, uint16_t>>(m, "Perm2");
    bind_perm<Perm<0, uint32_t>, Transf<0, uint32_t>>(m, "Perm4");
  }
}  // namespace libsemigroups
