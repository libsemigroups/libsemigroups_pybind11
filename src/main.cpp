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

// libsemigroups....
#include <libsemigroups/config.hpp>         // for LIBSEMIGROUPS_EIGEN_ENABLED
#include <libsemigroups/detail/report.hpp>  // for ReportGuard

// pybind11....
#include <pybind11/pybind11.h>  // for module_, class_, enum_, init

namespace libsemigroups {
  namespace py = pybind11;

  PYBIND11_MODULE(_libsemigroups_pybind11, m) {
    ////////////////////////////////////////////////////////////////////////
    // Module attributes
    ////////////////////////////////////////////////////////////////////////

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__")                   = "dev";
#endif
#ifdef LIBSEMIGROUPS_EIGEN_ENABLED
    m.attr("LIBSEMIGROUPS_EIGEN_ENABLED")
        = static_cast<bool>(LIBSEMIGROUPS_EIGEN_ENABLED);
#else
    m.attr("LIBSEMIGROUPS_EIGEN_ENABLED")   = false;
#endif

#ifdef LIBSEMIGROUPS_HPCOMBI_ENABLED
    m.attr("LIBSEMIGROUPS_HPCOMBI_ENABLED")
        = static_cast<bool>(LIBSEMIGROUPS_HPCOMBI_ENABLED);
#else
    m.attr("LIBSEMIGROUPS_HPCOMBI_ENABLED") = false;
#endif

    // At compile time, pybind11 tries to determine what the python type of a
    // C++ object will be. This is used when determining what typehints should
    // go in the doc, the contents of pybind11 error messages, and may be used
    // in function dispatch related ways (though JDE's not sure about that).
    // Therefore, it is important that we try to init our classes in the order
    // in which they are used.

    // It seems as though the code still compiles if we don't manage to get the
    // order perfect (but not always).

    ////////////////////////////////////////////////////////////////////////
    // Classes that need to be initialised early
    ////////////////////////////////////////////////////////////////////////

    // Must be before most things
    init_constants(m);
    init_order(m);
    init_types(m);

    // Must be before runners
    init_reporter(m);
    init_runner(m);

    // Must be before cong classes
    init_present(m);
    init_inverse_present(m);
    // Must be before anything with visualisation
    init_dot(m);

    // Must be before WordGraph
    init_forest(m);

    // Must be before anything that returns or requires a WordGraph
    init_word_graph(m);

    // Must come before anything that uses elements
    init_bmat8(m);
    init_matrix(m);
    init_pbr(m);
    init_transf(m);

    // Must come before paths
    init_words(m);

    ////////////////////////////////////////////////////////////////////////
    // Classes in (almost) alphabetical order
    ////////////////////////////////////////////////////////////////////////

    init_detail_cong_common(m);
    init_detail_todd_coxeter_impl(m);
    init_detail_knuth_bendix_impl(m);
    init_detail_report(m);

    init_action(m);
    init_aho_corasick(m);
    init_bipart(m);
    init_blocks(m);
    init_cong(m);
    init_error(m);
    init_freeband(m);
    init_froidure_pin_base(m);  // Must be before init_froidure_pin
    init_froidure_pin(m);
    init_gabow(m);
    init_imagerightaction(m);
    init_kambites(m);
    init_kbe(m);
    init_knuth_bendix(m);
    init_konieczny(m);
    init_obvinf(m);
    init_paths(m);
    init_presentation_examples(m);
    init_ranges(m);
    init_schreier_sims(m);
    init_sims(m);
    init_stephen(m);
    init_todd_coxeter(m);
    init_ukkonen(m);

    ////////////////////////////////////////////////////////////////////////
    // Classes that need to be initialised late
    ////////////////////////////////////////////////////////////////////////

    init_to_congruence(m);
    init_to_froidure_pin(m);
    init_to_knuth_bendix(m);
    init_to_present(m);
    init_to_present(m);
    init_to_todd_coxeter(m);
  }
}  // namespace libsemigroups
