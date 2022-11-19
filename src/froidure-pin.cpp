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
#include <algorithm>         // for fill, copy
#include <array>             // for array
#include <chrono>            // for nanoseconds
#include <cstdint>           // for uint16_t, uint32_t, uint8_t
#include <functional>        // for __base, function
#include <initializer_list>  // for initializer_list
#include <memory>            // for allocator, shared_ptr
#include <ostream>           // for operator<<, string, ostringstream
#include <string>            // for char_traits, operator+, basic_st...
#include <unordered_map>     // for operator==, operator!=
#include <vector>            // for vector

// libsemigroups....
#include <libsemigroups/bipart.hpp>             // for Bipartition
#include <libsemigroups/bmat8.hpp>              // for BMat8
#include <libsemigroups/constants.hpp>          // for operator==
#include <libsemigroups/containers.hpp>         // for DynamicArray2
#include <libsemigroups/froidure-pin-base.hpp>  // for FroidurePinBase
#include <libsemigroups/froidure-pin.hpp>  // for FroidurePin<>::element_index_type
#include <libsemigroups/kbe.hpp>     // for KBE, FroidurePin<>::factorisation
#include <libsemigroups/matrix.hpp>  // for MaxPlusTruncMat, MinPlusTruncMat
#include <libsemigroups/pbr.hpp>     // for PBR
#include <libsemigroups/runner.hpp>  // for Runner
#include <libsemigroups/tce.hpp>     // for TCE, TCE::Table
#include <libsemigroups/transf.hpp>  // for PPerm, Transf, Perm, LeastPPerm
#include <libsemigroups/types.hpp>   // for letter_type, word_type

// pybind11....
#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>  // for module, make_iterator, class_, init
#include <pybind11/stl.h>       // for operator<<

// libsemigroups_pybind11....
#include "doc-strings.hpp"  // for dead, finished, kill, report
#include "main.hpp"         // for init_froidure_pin

namespace libsemigroups {
  namespace fpsemigroup {
    class KnuthBendix;
  }
}  // namespace libsemigroups

namespace libsemigroups {
  namespace {

    template <typename T>
    py::list convert(detail::DynamicArray2<T> const& da) {
      py::list result;
      for (size_t i = 0; i < da.number_of_rows(); ++i) {
        py::list row;
        for (size_t j = 0; j < da.number_of_cols(); ++j) {
          row.append(da.get(i, j));
        }
        result.append(row);
      }
      return result;
    }

    template <typename T>
    std::string froidure_pin_repr(T& fp) {
      std::ostringstream out;
      out << "FroidurePin([";
      const auto  separator = ", ";
      const auto* sep       = "";

      for (size_t i = 0; i < fp.number_of_generators(); ++i) {
        auto obj = py::cast(fp.generator(i));
        out << sep << obj.attr("__repr__")();
        sep = separator;
      }
      out << "])";
      return out.str();
    }

    template <typename T, typename S = FroidurePinTraits<T>>
    void bind_froidure_pin(py::module& m, std::string typestr) {
      using Class              = FroidurePin<T, S>;
      using element_type       = typename FroidurePin<T, S>::element_type;
      using const_reference    = typename FroidurePin<T, S>::const_reference;
      std::string pyclass_name = std::string("FroidurePin") + typestr;
      py::class_<Class, std::shared_ptr<Class>, FroidurePinBase> x(
          m, pyclass_name.c_str(), py::buffer_protocol(), py::dynamic_attr());

      x.attr("element_type") = m.attr(typestr.c_str());

      x.def(py::init<std::vector<element_type> const&>(), py::arg("coll"))
          .def(py::init<Class const&>(), py::arg("that"))
          .def("size", &Class::size)
          .def("add_generator", &Class::add_generator, py::arg("x"))
          .def("number_of_generators", &Class::number_of_generators)
          .def("batch_size",
               py::overload_cast<size_t>(&Class::batch_size),
               py::arg("val"))
          .def("batch_size",
               py::overload_cast<>(&Class::batch_size, py::const_))
          .def("max_threads",
               py::overload_cast<size_t>(&Class::max_threads),
               py::arg("val"))
          .def("max_threads",
               py::overload_cast<>(&Class::max_threads, py::const_))
          .def("concurrency_threshold",
               py::overload_cast<size_t>(&Class::concurrency_threshold),
               py::arg("thrshld"))
          .def("concurrency_threshold",
               py::overload_cast<>(&Class::concurrency_threshold, py::const_))
          .def("reserve", &Class::reserve)
          .def("immutable",
               py::overload_cast<bool>(&Class::immutable),
               py::arg("val"))
          .def("immutable", py::overload_cast<>(&Class::immutable, py::const_))
          .def("is_monoid", &Class::is_monoid)
          .def("current_size", [](Class const& x) { return x.current_size(); })
          .def("current_number_of_rules",
               [](Class const& x) { return x.current_number_of_rules(); })
          .def("enumerate", &FroidurePinBase::enumerate, py::arg("limit"))
          .def("right_cayley_graph",
               [](Class& x) { return convert(x.right_cayley_graph()); })
          .def("left_cayley_graph",
               [](Class& x) { return convert(x.left_cayley_graph()); })
          .def("current_max_word_length",
               [](Class const& x) { return x.current_max_word_length(); })
          .def("current_position",
               py::overload_cast<const_reference>(&Class::current_position,
                                                  py::const_),
               py::arg("x"))
          .def("current_position",
               py::overload_cast<word_type const&>(&Class::current_position,
                                                   py::const_),
               py::arg("w"))
          .def("current_position",
               py::overload_cast<letter_type>(&Class::current_position,
                                              py::const_),
               py::arg("i"))
          .def("minimal_factorisation",
               py::overload_cast<element_index_type>(
                   &Class::minimal_factorisation),
               py::arg("pos"))
          .def("factorisation",
               py::overload_cast<element_index_type>(&Class::factorisation),
               py::arg("pos"))
          .def("factorisation",
               py::overload_cast<const_reference>(&Class::factorisation),
               py::arg("x"))
          .def("number_of_rules", &Class::number_of_rules)
          .def("rules",
               [](Class const& x) {
                 return py::make_iterator(x.cbegin_rules(), x.cend_rules());
               })
          .def("current_length",
               &FroidurePinBase::current_length,
               py::arg("pos"))
          .def("length", &FroidurePinBase::length, py::arg("pos"))
          .def("product_by_reduction",
               &FroidurePinBase::product_by_reduction,
               py::arg("i"),
               py::arg("j"))
          .def("prefix", &FroidurePinBase::prefix, py::arg("pos"))
          .def("suffix", &FroidurePinBase::suffix, py::arg("pos"))
          .def("first_letter", &FroidurePinBase::first_letter, py::arg("pos"))
          .def("final_letter", &FroidurePinBase::final_letter, py::arg("pos"))
          .def("degree", [](Class const& x) { return x.degree(); })
          .def("run", &Class::run, runner_doc_strings::run)
          .def("run_for",
               (void(Class::  // NOLINT(whitespace/parens)
                         *)(std::chrono::nanoseconds))
                   & Runner::run_for,
               py::arg("t"),
               runner_doc_strings::run_for)
          .def("run_until",
               (void(Class::  // NOLINT(whitespace/parens)
                         *)(std::function<bool()>&))
                   & Runner::run_until,
               py::arg("func"),
               runner_doc_strings::run_until)
          .def("kill", &Class::kill, runner_doc_strings::kill)
          .def("dead", &Class::dead, runner_doc_strings::dead)
          .def("finished", &Class::finished, runner_doc_strings::finished)
          .def("started", &Class::started, runner_doc_strings::started)
          .def("report", &Class::report, runner_doc_strings::report)
          .def("report_every",
               (void(Class::  // NOLINT(whitespace/parens)
                         *)(std::chrono::nanoseconds))
                   & Runner::report_every,
               py::arg("t"),
               runner_doc_strings::report_every)
          .def("report_why_we_stopped",
               &Class::report_why_we_stopped,
               runner_doc_strings::report_why_we_stopped)
          .def(
              "running",
              [](Class const& x) { return x.running(); },
              runner_doc_strings::running)
          .def("timed_out", &Class::timed_out, runner_doc_strings::timed_out)
          .def("stopped", &Class::stopped, runner_doc_strings::stopped)
          .def("stopped_by_predicate",
               &Class::stopped_by_predicate,
               runner_doc_strings::stopped_by_predicate)
          .def(
              "add_generators",
              [](Class& x, std::vector<element_type> const& y) {
                x.add_generators(y);
              },
              py::arg("coll"))
          .def(
              "closure",
              [](Class& x, std::vector<element_type> const& y) {
                x.closure(y);
              },
              py::arg("coll"))
          .def(
              "copy_add_generators",
              [](Class& x, std::vector<element_type> const& y) {
                return x.copy_add_generators(y);
              },
              py::arg("coll"))
          .def(
              "copy_closure",
              [](Class& x, std::vector<element_type> const& y) {
                return x.copy_closure(y);
              },
              py::arg("coll"))
          .def("word_to_element", &Class::word_to_element, py::arg("w"))
          .def("generator", &Class::generator, py::arg("i"))
          .def("contains", &Class::contains, py::arg("x"))
          .def("sorted_position", &Class::sorted_position, py::arg("x"))
          .def("position", &Class::position, py::arg("x"))
          .def("sorted_at", &Class::sorted_at, py::arg("i"))
          .def("at", &Class::at, py::arg("i"))
          .def("__iter__",
               [](Class const& x) {
                 return py::make_iterator(x.cbegin(), x.cend());
               })
          .def("sorted",
               [](Class& x) {
                 return py::make_iterator(x.cbegin_sorted(), x.cend_sorted());
               })
          .def("idempotents",
               [](Class& x) {
                 return py::make_iterator(x.cbegin_idempotents(),
                                          x.cend_idempotents());
               })
          .def("number_of_idempotents", &Class::number_of_idempotents)
          .def("is_idempotent", &Class::is_idempotent, py::arg("i"))
          .def("position_to_sorted_position",
               &Class::position_to_sorted_position,
               py::arg("i"))
          .def("is_finite", &Class::is_finite)
          .def("equal_to", &Class::equal_to, py::arg("x"), py::arg("y"))
          .def("fast_product", &Class::fast_product, py::arg("i"), py::arg("j"))
          .def("__repr__", &froidure_pin_repr<Class>);
    }
  }  // namespace

  void init_froidure_pin(py::module& m) {
    py::class_<FroidurePinBase, std::shared_ptr<FroidurePinBase>>(
        m, "FroidurePinBase");

    bind_froidure_pin<LeastTransf<16>>(m, "Transf16");
    bind_froidure_pin<Transf<0, uint8_t>>(m, "Transf1");
    bind_froidure_pin<Transf<0, uint16_t>>(m, "Transf2");
    bind_froidure_pin<Transf<0, uint32_t>>(m, "Transf4");
    bind_froidure_pin<LeastPPerm<16>>(m, "PPerm16");
    bind_froidure_pin<PPerm<0, uint8_t>>(m, "PPerm1");
    bind_froidure_pin<PPerm<0, uint16_t>>(m, "PPerm2");
    bind_froidure_pin<PPerm<0, uint32_t>>(m, "PPerm4");
    bind_froidure_pin<LeastPerm<16>>(m, "Perm16");
    bind_froidure_pin<Perm<0, uint8_t>>(m, "Perm1");
    bind_froidure_pin<Perm<0, uint16_t>>(m, "Perm2");
    bind_froidure_pin<Perm<0, uint32_t>>(m, "Perm4");
    bind_froidure_pin<detail::KBE,
                      FroidurePinTraits<detail::KBE, fpsemigroup::KnuthBendix>>(
        m, "KBE");
    bind_froidure_pin<detail::TCE,
                      FroidurePinTraits<detail::TCE, detail::TCE::Table>>(
        m, "TCE");
    bind_froidure_pin<Bipartition>(m, "Bipartition");
    bind_froidure_pin<PBR>(m, "PBR");

    bind_froidure_pin<BMat8>(m, "BMat8");
    bind_froidure_pin<BMat<>>(m, "BMat");
    bind_froidure_pin<IntMat<>>(m, "IntMat");
    bind_froidure_pin<MaxPlusMat<>>(m, "MaxPlusMat");
    bind_froidure_pin<MinPlusMat<>>(m, "MinPlusMat");
    bind_froidure_pin<ProjMaxPlusMat<>>(m, "ProjMaxPlusMat");
    bind_froidure_pin<MaxPlusTruncMat<>>(m, "MaxPlusTruncMat");
    bind_froidure_pin<MinPlusTruncMat<>>(m, "MinPlusTruncMat");
    bind_froidure_pin<NTPMat<>>(m, "NTPMat");
  }
}  // namespace libsemigroups
