//
// libsemigroups - C++ library for semigroups and monoids
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

// C++ stl headers....
#include <array>             // for array
#include <chrono>            // for nanoseconds
#include <functional>        // for __base, function
#include <initializer_list>  // for initializer_list
#include <iosfwd>            // for string
#include <memory>            // for allocator, shared_ptr
#include <string>            // for operator+, char_traits, basic_st...
#include <vector>            // for vector

// libsemigroups....
#include <libsemigroups/cong-intf.hpp>  // for congruence_kind
#include <libsemigroups/constants.hpp>  // for operator==, UNDEFINED, Undefined
#include <libsemigroups/knuth-bendix.hpp>  // for KnuthBendix
#include <libsemigroups/runner.hpp>        // for Runner
#include <libsemigroups/string.hpp>        // for to_string
#include <libsemigroups/todd-coxeter.hpp>  // for ToddCoxeter, ToddCoxeter::option...
#include <libsemigroups/types.hpp>         // for word_type

// pybind11....
#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>  // for class_, enum_, init, make_iterator
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "doc-strings.hpp"  // for add_pair, class_index_to_word
#include "main.hpp"         // for init_todd_coxeter

namespace libsemigroups {
  class FroidurePinBase;
}
namespace libsemigroups {
  namespace fpsemigroup {
    class KnuthBendix;
  }
}  // namespace libsemigroups

namespace py = pybind11;

namespace libsemigroups {
  void init_todd_coxeter(py::module& m) {
    using sort_function_type
        = std::function<bool(word_type const&, word_type const&)>;

    py::class_<congruence::ToddCoxeter> tc(m, "ToddCoxeter");

    py::enum_<congruence::ToddCoxeter::order>(tc,
                                              "order",
                                              R"pbdoc(
            The possible arguments for :py:meth:`standardize`.

            The values in this enum can be used as the argument for
            :py:meth:`standardize` to specify which ordering should be used.  The
            normal forms for congruence classes are given with respect to one of
            the orders specified by the values in this enum.
          )pbdoc")
        .value("none",
               congruence::ToddCoxeter::order::none,
               R"pbdoc(
            No standardization has been done.
          )pbdoc")
        .value("shortlex",
               congruence::ToddCoxeter::order::shortlex,
               R"pbdoc(
          Normal forms are the short-lex least word belonging to a given
          congruence class.
        )pbdoc")
        .value("lex",
               congruence::ToddCoxeter::order::lex,
               R"pbdoc(
          Normal forms are the lexicographical least word belonging to a given
          congruence class.
        )pbdoc")
        .value("recursive",
               congruence::ToddCoxeter::order::recursive,
               R"pbdoc(
          Normal forms are the recursive-path least word belonging to a given
          congruence class.
        )pbdoc");

    py::enum_<congruence::ToddCoxeter::options::strategy>(tc,
                                                          "strategy_options",
                                                          R"pbdoc(
          Values for defining the strategy.

          The values in this enum can be used as the argument for the method
          :py:meth:`strategy` to specify which strategy should be used when
          performing a coset enumeration.
        )pbdoc")
        .value("hlt",
               congruence::ToddCoxeter::options::strategy::hlt,
               R"pbdoc(
                 This value indicates that the HLT (Hazelgrove-Leech-Trotter)
                 strategy should be used. This is analogous to ACE's R-style.
               )pbdoc")
        .value("felsch",
               congruence::ToddCoxeter::options::strategy::felsch,
               R"pbdoc(
                 This value indicates that the Felsch strategy should be used.
                 This is analogous to ACE's C-style.
               )pbdoc")
        .value("random",
               congruence::ToddCoxeter::options::strategy::random,
               R"pbdoc(
                 This value indicates that a random combination of the HLT and
                 Felsch strategies should be used. A random strategy (and
                 associated options) are selected from one of the 10 options:

                 1. HLT + full lookahead + no deduction processing + standardization
                 2. HLT + full lookahead + deduction processing + standardization
                 3. HLT + full lookahead + no deduction processing + no standardization
                 4. HLT + full lookahead + deduction processing + no standardization
                 5. HLT + partial lookahead + no deduction processing + standardization
                 6. HLT + partial lookahead + deduction processing + standardization
                 7. HLT + partial lookahead + no deduction processing + no standardization
                 8. HLT + partial lookahead + deduction processing + no standardization
                 9. Felsch + standardization
                 10. Felsch + no standardization

                 and this strategy is then run for approximately the amount
                 of time specified by the setting :py:meth:`random_interval`.
               )pbdoc");

    py::enum_<congruence::ToddCoxeter::options::lookahead>(tc,
                                                           "lookahead_options",
                                                           R"pbdoc(
          Values for specifying the type of lookahead to perform.

          The values in this enum can be used as the argument for
          :py:meth:`lookahead` to specify the type of lookahead that should be
          performed when using the HLT strategy.
        )pbdoc")
        .value("full",
               congruence::ToddCoxeter::options::lookahead::full,
               R"pbdoc(
                 A *full* lookahead is one starting from the initial coset.
                 Full lookaheads are therefore sometimes slower but may
                 detect more coincidences than a partial lookahead.
               )pbdoc")
        .value("partial",
               congruence::ToddCoxeter::options::lookahead::partial,
               R"pbdoc(
                 A *partial* lookahead is one starting from the current coset.
                 Partial lookaheads are therefore sometimes faster but may not
                 detect as many coincidences as a full lookahead.
               )pbdoc");

    py::enum_<congruence::ToddCoxeter::options::froidure_pin>(
        tc,
        "froidure_pin_options",
        R"pbdoc(
          Values for specifying whether to use relations or Cayley graph.

          The values in this enum can be used as the argument for
          :py:meth:`froidure_pin_policy` to specify whether the
          defining relations, or the left/right Cayley graph, of a
          :py:class:`FroidurePin` instance, should be used in the coset
          enumeration.

          If the number of classes in the congruence represented by a
          :py:class:`ToddCoxeter` instance is relatively small, by some
          definition, compared to the size of the semigroup represented by the
          :py:class:`FroidurePin` instance, then the ``use_relations`` option
          is often faster. If the number of classes is relatively large, then
          ``use_cayley_graph`` is often faster.

          It is guaranteed that run will terminate in an amount of time
          proportionate to the size of the input if the policy
          ``use_cayley_graph`` is used, whereas the run time when using the
          policy ``use_relations`` can be arbitrarily high regardless of the
          size of the input.
        )pbdoc")
        .value("none",
               congruence::ToddCoxeter::options::froidure_pin::none,
               R"pbdoc(
                 No policy has been specified.
               )pbdoc")
        .value("use_relations",
               congruence::ToddCoxeter::options::froidure_pin::use_relations,
               R"pbdoc(
                 Use the relations of a :py:class:`FroidurePin` instance.
               )pbdoc")
        .value("use_cayley_graph",
               congruence::ToddCoxeter::options::froidure_pin::use_cayley_graph,
               R"pbdoc(
                 Use the left or right Cayley graph of a :py:class:`FroidurePin`
                 instance.
               )pbdoc");

    tc.def(py::init<congruence_kind>(),
           py::arg("kind"),
           R"pbdoc(
             Construct from kind (left/right/2-sided) and options.

             Constructs an empty instance of an interface to a congruence of
             type specified by the argument.

             :Parameters: - **kind** (congruence_kind) the handedness of the congruence.

             :Complexity: Constant.

             .. seealso:: :py:meth:`set_number_of_generators` and
                          :py:meth:`add_pair`.
         )pbdoc")
        .def(py::init<congruence_kind, congruence::ToddCoxeter&>(),
             py::arg("knd"),
             py::arg("tc"),
             R"pbdoc(
               Construct from kind (left/right/2-sided) and
               :py:class:`ToddCoxeter`.

               This constructor creates a new :py:class:`ToddCoxeter` instance
               representing a left, right, or two-sided congruence over the
               quotient semigroup represented by a :py:class:`ToddCoxeter`
               instance.

               :Parameters: - **knd** (congruence_kind) the handedness of the
                              congruence.
                            - **tc** (ToddCoxeter) the :py:class:`ToddCoxeter`
                              representing the underlying semigroup

               :Raises:
                 `RuntimeError <https://docs.python.org/3/library/exceptions.html#RuntimeError>`_ - if ``tc`` is a left, or right, congruence, and
                 ``knd`` is not left, or not right, respectively.
             )pbdoc")
        .def(py::init<congruence_kind, fpsemigroup::KnuthBendix&>(),
             py::arg("knd"),
             py::arg("kb"),
             R"pbdoc(
               Construct from kind (left/right/2-sided) and
               :py:class:`KnuthBendix`.

               A constructor that creates a new :py:class:`ToddCoxeter`
               instance representing a left, right, or two-sided congruence
               over the semigroup represented by a :py:class:`KnuthBendix`
               instance.

               :Parameters: - **knd** (congruence_kind) the handedness of the
                              congruence.
                            - **kb** (KnuthBendix) the :py:class:`KnuthBendix`
                              representing the underlying semigroup.
             )pbdoc")
        .def(py::init<congruence::ToddCoxeter const&>(),
             R"pbdoc(
               Copy constructor.

               Constructs a complete copy of ``that``, including all of the settings,
               table, defining relations, and generating pairs.

               :Parameters: - **that** (ToddCoxeter) the ToddCoxeter instance to copy.
             )pbdoc")
        .def(py::init<congruence_kind, std::shared_ptr<FroidurePinBase>>(),
             R"pbdoc(
               Construct from kind (left/right/2-sided) and FroidurePin.

               This constructor creates a :py:class:`ToddCoxeter` instance
               representing a left, right, or two-sided congruence over the
               semigroup represented by a :py:class:`FroidurePin`
               object.

               :Parameters: - **knd** (congruence_kind) the kind of the congruence being constructed
                            - **fp** (FroidurePin) the semigroup over which the congruence is to be defined.
             )pbdoc")
        // TODO(later) more of constructors?
        .def("__repr__",
             [](congruence::ToddCoxeter const& tc) {
               auto n = (tc.number_of_generators() == UNDEFINED
                             ? "-"
                             : detail::to_string(tc.number_of_generators()));

               return std::string("<ToddCoxeter object with ") + n
                      + " generators and "
                      + detail::to_string(tc.number_of_generating_pairs())
                      + " pairs>";
             })
        .def("set_number_of_generators",
             &congruence::ToddCoxeter::set_number_of_generators,
             py::arg("n"),
             cong_intf_doc_strings::set_number_of_generators)
        .def(
            "number_of_generators",
            [](congruence::ToddCoxeter const& tc) {
              return tc.number_of_generators();
            },
            cong_intf_doc_strings::number_of_generators)
        .def("add_pair",
             py::overload_cast<word_type const&, word_type const&>(
                 &congruence::ToddCoxeter::add_pair),
             py::arg("u"),
             py::arg("v"),
             cong_intf_doc_strings::add_pair)
        .def(
            "number_of_generating_pairs",
            [](congruence::ToddCoxeter const& tc) {
              return tc.number_of_generating_pairs();
            },
            cong_intf_doc_strings::number_of_generating_pairs)
        .def("froidure_pin_policy",
             py::overload_cast<congruence::ToddCoxeter::options::froidure_pin>(
                 &congruence::ToddCoxeter::froidure_pin_policy),
             R"pbdoc(
               Sets the value of the Froidure-Pin policy specified by the
               argument :py:obj:`ToddCoxeter.froidure_pin_options`.
             )pbdoc")
        .def(
            "froidure_pin_policy",
            [](congruence::ToddCoxeter const& x) {
              return x.froidure_pin_policy();
            },
            R"pbdoc(
               Gets the value of the Froidure-Pin policy.
             )pbdoc")
        .def("lookahead",
             py::overload_cast<congruence::ToddCoxeter::options::lookahead>(
                 &congruence::ToddCoxeter::lookahead),
             R"pbdoc(
               Sets the type of lookahead to be used when using the HLT strategy.
             )pbdoc")
        .def("lower_bound",
             py::overload_cast<size_t>(&congruence::ToddCoxeter::lower_bound),
             R"pbdoc(
               Sets a lower bound for the number of classes of the congruence
               represented by a ToddCoxeter instance.
             )pbdoc")
        .def(
            "next_lookahead",
            py::overload_cast<size_t>(&congruence::ToddCoxeter::next_lookahead),
            R"pbdoc(
               If the number of cosets active exceeds the value set by this function,
               then a lookahead, of the type set by lookahead, is triggered.
             )pbdoc")
        .def("save",
             py::overload_cast<bool>(&congruence::ToddCoxeter::save),
             R"pbdoc(
               If the argument of this function is ``True`` and the HLT
               strategy is being used, then deductions are processed during the
               enumeration.
             )pbdoc")
        .def("standardize",
             py::overload_cast<bool>(&congruence::ToddCoxeter::standardize),
             R"pbdoc(
               If the argument of this function is ``True``, then the coset table is
               standardized (according to the short-lex order) during the coset
               enumeration.
             )pbdoc")
        .def("strategy",
             (congruence::ToddCoxeter::options::strategy(
                 congruence::ToddCoxeter::*)() const)
                 & congruence::ToddCoxeter::strategy,
             R"pbdoc(
               Returns the value of the strategy used during the coset
               enumeration.
             )pbdoc")
        .def("strategy",
             py::overload_cast<congruence::ToddCoxeter::options::strategy>(
                 &congruence::ToddCoxeter::strategy),
             R"pbdoc(
               Set the strategy used during the coset enumeration can be
               specified using this function.
             )pbdoc")
        .def("random_interval",
             (congruence::ToddCoxeter
              & (congruence::ToddCoxeter::*) (std::chrono::nanoseconds))
                 & congruence::ToddCoxeter::random_interval,
             R"pbdoc(
               Sets the duration in nanoseconds that a given randomly selected
               strategy will run for, when using the random strategy
               (:py:obj:`ToddCoxeter.strategy_options.random`).
             )pbdoc")
        .def("sort_generating_pairs",
             py::overload_cast<sort_function_type>(
                 &congruence::ToddCoxeter::sort_generating_pairs),
             py::arg("func"),
             R"pbdoc(
               Sorts all existing generating pairs according to the binary function
               func.

               :param func:
                 a binary predicate that defines a linear order on the relations in
                 a :py:class:`ToddCoxeter` instance.
               :type func: Callable[], bool
             )pbdoc")
        .def("random_shuffle_generating_pairs",
             &congruence::ToddCoxeter::random_shuffle_generating_pairs,
             R"pbdoc(
               Randomly shuffle all existing generating pairs.
             )pbdoc")
        .def("report_every",
             (void(congruence::ToddCoxeter::*)(std::chrono::nanoseconds))
                 & Runner::report_every,
             py::arg("t"),
             runner_doc_strings::report_every)
        .def("report",
             &congruence::ToddCoxeter::report,
             runner_doc_strings::report)
        .def("report_why_we_stopped",
             &congruence::ToddCoxeter::report_why_we_stopped,
             runner_doc_strings::report_why_we_stopped)
        .def("kill", &congruence::ToddCoxeter::kill, runner_doc_strings::kill)
        .def("run", &congruence::ToddCoxeter::run, runner_doc_strings::run)
        .def("run_for",
             (void(congruence::ToddCoxeter::*)(std::chrono::nanoseconds))
                 & Runner::run_for,
             py::arg("t"),
             runner_doc_strings::run_for)
        .def("run_until",
             (void(congruence::ToddCoxeter::*)(std::function<bool()>&))
                 & Runner::run_until,
             py::arg("func"),
             runner_doc_strings::run_until)
        .def("less",
             &congruence::ToddCoxeter::less,
             py::arg("u"),
             py::arg("v"),
             cong_intf_doc_strings::less)
        .def("const_contains",
             &congruence::ToddCoxeter::const_contains,
             py::arg("u"),
             py::arg("v"),
             cong_intf_doc_strings::const_contains)
        .def("contains",
             &congruence::ToddCoxeter::contains,
             py::arg("u"),
             py::arg("v"),
             cong_intf_doc_strings::contains)
        .def("empty",
             &congruence::ToddCoxeter::empty,
             R"pbdoc(
               Returns ``True`` if there are no relations or generating pairs
               in the ToddCoxeter instance, and the number of active cosets is
               1 (the minimum possible).
             )pbdoc")
        .def("number_of_classes",
             &congruence::ToddCoxeter::number_of_classes,
             cong_intf_doc_strings::number_of_classes)
        .def("number_of_non_trivial_classes",
             &congruence::ToddCoxeter::number_of_non_trivial_classes,
             cong_intf_doc_strings::number_of_non_trivial_classes)
        .def("reserve",
             &congruence::ToddCoxeter::reserve,
             R"pbdoc(
               Reserves the capacity specified by the argument in the data
               structures for cosets used in a ToddCoxeter instance.
             )pbdoc")
        .def("shrink_to_fit",
             &congruence::ToddCoxeter::shrink_to_fit,
             R"pbdoc(
               Release all memory used to store free cosets, and any other unnecessary
               data if the enumeration is finished.
             )pbdoc")
        .def("quotient_froidure_pin",
             &congruence::ToddCoxeter::quotient_froidure_pin,
             cong_intf_doc_strings::quotient_froidure_pin)
        .def("has_quotient_froidure_pin",
             &congruence::ToddCoxeter::has_quotient_froidure_pin,
             cong_intf_doc_strings::has_quotient_froidure_pin)
        .def("parent_froidure_pin",
             &congruence::ToddCoxeter::parent_froidure_pin,
             cong_intf_doc_strings::parent_froidure_pin)
        .def(
            "has_parent_froidure_pin",
            [](congruence::ToddCoxeter const& tc) {
              return tc.has_parent_froidure_pin();
            },
            cong_intf_doc_strings::has_parent_froidure_pin)
        .def("is_quotient_obviously_finite",
             &congruence::ToddCoxeter::is_quotient_obviously_finite,
             cong_intf_doc_strings::is_quotient_obviously_finite)
        .def("is_quotient_obviously_infinite",
             &congruence::ToddCoxeter::is_quotient_obviously_infinite,
             cong_intf_doc_strings::is_quotient_obviously_infinite)
        .def("word_to_class_index",
             &congruence::ToddCoxeter::word_to_class_index,
             py::arg("w"),
             cong_intf_doc_strings::word_to_class_index)
        .def("class_index_to_word",
             &congruence::ToddCoxeter::class_index_to_word,
             py::arg("i"),
             cong_intf_doc_strings::class_index_to_word)
        .def(
            "kind",
            [](congruence::ToddCoxeter const& tc) { return tc.kind(); },
            cong_intf_doc_strings::kind)
        .def("complete",
             &congruence::ToddCoxeter::complete,
             R"pbdoc(
               Returns ``True`` if the coset table is complete, and ``False`` if it
               is not.
             )pbdoc")
        .def(
            "compatible",
            [](congruence::ToddCoxeter const& tc) { return tc.compatible(); },
            R"pbdoc(
               Returns ``True`` if the coset table is compatible with the
               relations and generating pairs used to create this, and
               ``False`` if it is not.
             )pbdoc")
        .def("dead", &congruence::ToddCoxeter::dead, runner_doc_strings::dead)
        .def("finished",
             &congruence::ToddCoxeter::finished,
             runner_doc_strings::finished)
        .def("timed_out",
             &congruence::ToddCoxeter::timed_out,
             runner_doc_strings::timed_out)
        .def("stopped_by_predicate",
             &congruence::ToddCoxeter::stopped_by_predicate,
             runner_doc_strings::stopped_by_predicate)
        .def("is_standardized",
             &congruence::ToddCoxeter::is_standardized,
             R"pbdoc(
               Returns ``True`` if the :py:class:`ToddCoxeter` instance is
               standardized.
           )pbdoc")
        .def("standardize",
             py::overload_cast<congruence::ToddCoxeter::order>(
                 &congruence::ToddCoxeter::standardize),
             R"pbdoc(
               If the argument of this function is ``True``, then the coset
               table is standardized (according to the short-lex order) during
               the coset enumeration.
             )pbdoc")
        .def(
            "generating_pairs",
            [](congruence::ToddCoxeter const& tc) {
              return py::make_iterator(tc.cbegin_generating_pairs(),
                                       tc.cend_generating_pairs());
            },
            cong_intf_doc_strings::generating_pairs)
        .def(
            "non_trivial_classes",
            [](congruence::ToddCoxeter& tc) {
              return py::make_iterator(tc.cbegin_ntc(), tc.cend_ntc());
            },
            cong_intf_doc_strings::non_trivial_classes)
        .def(
            "normal_forms",
            [](congruence::ToddCoxeter& tc) {
              return py::make_iterator(tc.cbegin_normal_forms(),
                                       tc.cend_normal_forms());
            },
            R"pbdoc(
              Returns an iterator to the normal forms of the congruence
              represented by an instance of :py:class:`ToddCoxeter`.
            )pbdoc")
        .def("to_gap_string",
             &congruence::ToddCoxeter::to_gap_string,
             R"pbdoc(
              Returns a string containing a GAP definition of the finitely presented semigroup represented by a ``ToddCoxeter`` instance.

              :parameters: None

              :returns: A string
             )pbdoc");
  }
}  // namespace libsemigroups
