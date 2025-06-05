//
// libsemigroups - C++ library for semigroups and monoids
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

#include "main.hpp"

// C std headers....
#include <cstddef>  // for size_t
#include <stdexcept>

// libsemigroups....
#include <libsemigroups/config.hpp>     // for LIBSEMIGROUPS_EIGEN_ENABLED
#include <libsemigroups/constants.hpp>  // for PositiveInfinity, Undefined, POSI...
#include <libsemigroups/detail/kbe.hpp>     // for KBE, operator<<
#include <libsemigroups/detail/report.hpp>  // for ReportGuard
#include <libsemigroups/detail/string.hpp>  // for to_string
#include <libsemigroups/detail/tce.hpp>     // for TCE
#include <libsemigroups/exception.hpp>      // for LibsemigroupsException
#include <libsemigroups/types.hpp>          // for tril, tril::FALSE, tril::TRUE

#include <libsemigroups/detail/cong-common-class.hpp>  // for congruence_kind, congruence_kind:...

// pybind11....
#include <pybind11/operators.h>  // for self, operator<, operator==, self_t
#include <pybind11/pybind11.h>   // for module_, class_, enum_, init

namespace libsemigroups {
  namespace py = pybind11;

  // TODO move to separate fil
  bool ERROR_MESSAGE_WITH_PREFIX = false;

  void error_message_with_prefix(bool value) {
    ERROR_MESSAGE_WITH_PREFIX = value;
  }

  bool error_message_with_prefix() {
    return ERROR_MESSAGE_WITH_PREFIX;
  }

  std::string formatted_error_message(std::runtime_error const& e) {
    if (error_message_with_prefix()) {
      return std::string(e.what());
    } else {
      // TODO this doesn't work well if backward is enabled.
      std::string out(e.what());
      size_t      pos = out.find(": ");
      if (pos != std::string::npos) {
        out.erase(0, pos + 2);
      }
      return out;
    }
  }

  // TODO move to types.cpp
  // This has its own function so the py::options can be set for just this enum
  void init_tril(py::module& m) {
    py::options options;
    options.disable_enum_members_docstring();
    py::enum_<tril>(m, "tril", R"pbdoc(
The values in this enum can be used to indicate a result is true, false, or not
currently known.

The valid values are:

.. py:attribute:: tril.false
  :value: <tril.false: 0>

  Value representing false.

.. py:attribute:: tril.true
  :value: <tril.true: 1>

  Value representing true.

.. py:attribute:: tril.unknown
  :value: <tril.unknown: 2>

  Value representing unknown (either true or false).
)pbdoc")
        .value("true", tril::TRUE)
        .value("false", tril::FALSE)
        .value("unknown", tril::unknown);
  }

  PYBIND11_MODULE(_libsemigroups_pybind11, m) {
#ifdef LIBSEMIGROUPS_EIGEN_ENABLED
    m.attr("LIBSEMIGROUPS_EIGEN_ENABLED")
        = static_cast<bool>(LIBSEMIGROUPS_EIGEN_ENABLED);
#else
    m.attr("LIBSEMIGROUPS_EIGEN_ENABLED") = false;
#endif

#ifdef LIBSEMIGROUPS_HPCOMBI_ENABLED
    m.attr("LIBSEMIGROUPS_HPCOMBI_ENABLED")
        = static_cast<bool>(LIBSEMIGROUPS_HPCOMBI_ENABLED);
#else
    m.attr("LIBSEMIGROUPS_HPCOMBI_ENABLED") = false;
#endif

    ////////////////////////////////////////////////////////////////////////
    // Exceptions
    ////////////////////////////////////////////////////////////////////////

    // TODO this doesn't seem to properly catch all LibsemigroupsExceptions,
    // particularly on macOS. This may have been resolved in pybind11 2.12.0
    static py::exception<LibsemigroupsException> exc(
        m, "LibsemigroupsError", PyExc_RuntimeError);
    py::register_exception_translator([](std::exception_ptr p) {
      try {
        if (p) {
          std::rethrow_exception(p);
        }
      } catch (LibsemigroupsException const& e) {
        exc(formatted_error_message(e).c_str());
      } catch (py::stop_iteration const& e) {
        throw e;
      } catch (std::runtime_error const& e) {
        exc(formatted_error_message(e).c_str());
      }
    });

    ////////////////////////////////////////////////////////////////////////
    // Classes that need to be initialised early
    ////////////////////////////////////////////////////////////////////////

    init_constants(m);
    init_reporter(m);
    init_runner(m);
    init_tril(m);

    ////////////////////////////////////////////////////////////////////////
    // Classes in (almost) alphabetical order
    ////////////////////////////////////////////////////////////////////////

    init_detail_cong_common(m);
    init_detail_todd_coxeter_impl(m);
    init_detail_knuth_bendix_impl(m);

    init_action(m);
    init_aho_corasick(m);
    init_bipart(m);
    init_blocks(m);
    init_bmat8(m);
    init_cong(m);
    init_dot(m);
    init_forest(m);
    init_freeband(m);
    init_froidure_pin_base(m);  // Must be before init_froidure_pin
    init_froidure_pin(m);
    init_gabow(m);
    init_imagerightaction(m);
    init_kambites(m);
    init_knuth_bendix(m);
    init_konieczny(m);
    init_matrix(m);
    init_obvinf(m);
    init_order(m);
    init_paths(m);
    init_pbr(m);
    init_present(m);
    init_inverse_present(m);  // Must be after init_present
    init_presentation_examples(m);
    init_ranges(m);
    init_schreier_sims(m);
    init_sims(m);
    init_stephen(m);
    init_to_congruence(m);
    init_to_froidure_pin(m);
    init_to_knuth_bendix(m);
    init_to_present(m);
    init_to_present(m);
    init_to_todd_coxeter(m);
    init_todd_coxeter(m);
    init_transf(m);
    init_ukkonen(m);
    init_word_graph(m);
    init_words(m);

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif

    ////////////////////////////////////////////////////////////////////////
    // Enums
    ////////////////////////////////////////////////////////////////////////

    // TODO to types.cpp
    py::enum_<congruence_kind>(m, "congruence_kind", R"pbdoc(
    The values in this class can be used to indicate that a congruence should
    be 1-sided or 2-sided.
  )pbdoc")
        .value("onesided",
               congruence_kind::onesided,
               R"pbdoc(Type for 1-sided congruences (right).)pbdoc")
        .value("twosided",
               congruence_kind::twosided,
               R"pbdoc(Type for 2-sided congruences.)pbdoc");

    py::class_<ReportGuard>(m,
                            "ReportGuard",
                            R"pbdoc(
      Objects of this type can be used to enable printing of some information
      during various of the computation in ``libsemigroups_pybind11``. Reporting
      is enable (or not) at construction time, and disable when the
      :py:class:`ReportGuard` goes out of scope.
      )pbdoc")
        .def(py::init<bool>(),
             py::arg("val") = true,
             R"pbdoc(
Constructs a :py:class:`ReportGuard` with reporting enabled by
default.

:param val:
   whether to report or not (default: ``True``).

:type val:
   bool
  )pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // Global variables
    ////////////////////////////////////////////////////////////////////////

    // TODO: Doc
    m.def("error_message_with_prefix",
          py::overload_cast<>(&error_message_with_prefix));
    m.def("error_message_with_prefix",
          py::overload_cast<bool>(&error_message_with_prefix));
  }

}  // namespace libsemigroups
