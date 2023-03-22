// libsemigroups - C++ library for semigroups and monoids
// Copyright (C) 2021 James D. Mitchell, Maria Tsalakou
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
#include <array>             // for array
#include <chrono>            // for nanoseconds
#include <functional>        // for __base, function
#include <initializer_list>  // for initializer_list
#include <iosfwd>            // for string
#include <memory>            // for shared_ptr
#include <vector>            // for vector

// libsemigroups....
#include <libsemigroups/fpsemi-intf.hpp>  // for FpSemigroupInterface, FpSemigrou...
#include <libsemigroups/knuth-bendix.hpp>  // for KnuthBendix, KnuthBendix::option...
#include <libsemigroups/runner.hpp>        // for Runner
#include <libsemigroups/types.hpp>         // for word_type, letter_type

// pybind11....
#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>  // for class_, make_iterator, enum_, init
#include <pybind11/pytypes.h>   // for py::str
#include <pybind11/stl.h>

#include <Python.h>  // for PyUnicode_DecodeLatin1

// libsemigroups_pybind11....
#include "doc-strings.hpp"  // for dead, finished, kill, report
#include "main.hpp"         // for init_knuth_bendix

namespace py = pybind11;

namespace libsemigroups {
  class FroidurePinBase;

  namespace {
    std::string to_latin1(std::string const& u) {
      static py::object bytes;
      static bool       first_call = true;
      if (first_call) {
        first_call = false;
        try {
          bytes = py::getattr(py::globals()["__builtins__"], "bytes");
        } catch (py::error_already_set& e) {
          bytes = py::globals()["__builtins__"]["bytes"];
        }
      }
      return PyBytes_AS_STRING(bytes(py::str(u), "latin1").ptr());
    }

    py::str from_latin1(std::string const& u) {
      // TODO don't pass NULL as the final param
      return py::reinterpret_steal<py::str>(
          PyUnicode_DecodeLatin1(u.data(), u.length(), NULL));
    }
  }  // namespace

  void init_knuth_bendix(py::module& m) {
    using rule_type = FpSemigroupInterface::rule_type;

    m.def("redundant_rule_strings",
          [](Presentation<std::string>& p, std::chrono::nanoseconds t) {
            return std::distance(p.rules.cbegin(),
                                 presentation::redundant_rule(p, t));
          });
    m.def("redundant_rule_words",
          [](Presentation<word_type>& p, std::chrono::nanoseconds t) {
            return std::distance(p.rules.cbegin(),
                                 presentation::redundant_rule(p, t));
          });

    py::class_<fpsemigroup::KnuthBendix,
               std::shared_ptr<fpsemigroup::KnuthBendix>>
        kb(m, "KnuthBendix");

    py::enum_<fpsemigroup::KnuthBendix::options::overlap>(kb,
                                                          "overlap",
                                                          R"pbdoc(
             Values for specifying how to measure the length of an overlap.

             The values in this enum determine how a :py:class:`KnuthBendix`
             instance measures the length :math:`d(AB, BC)` of the overlap of
             two words :math:`AB` and :math:`BC`.

             .. seealso:: :py:meth:`overlap_policy`
           )pbdoc")
        .value("ABC",
               fpsemigroup::KnuthBendix::options::overlap::ABC,
               R"pbdoc(
                 :math:`d(AB, BC) = |A| + |B| + |C|`
               )pbdoc")
        .value("AB_BC",
               fpsemigroup::KnuthBendix::options::overlap::AB_BC,
               R"pbdoc(
                 :math:`d(AB, BC) = |AB| + |BC|`
               )pbdoc")
        .value("MAX_AB_BC",
               fpsemigroup::KnuthBendix::options::overlap::MAX_AB_BC,
               R"pbdoc(
                 :math:`d(AB, BC) = max(|AB|, |BC|)`
               )pbdoc");

    kb.def(py::init<>(),
           R"pbdoc(
             Default constructor.
           )pbdoc")
        .def("__repr__",
             [](fpsemigroup::KnuthBendix const& kb) {
               auto n    = (kb.alphabet().empty()
                                ? "-"
                                : detail::to_string(kb.alphabet().size()));
               auto conf = (kb.confluent() ? "" : "non-");
               return std::string("<") + conf + "confluent KnuthBendix with "
                      + n + " letters + "
                      + detail::to_string(kb.number_of_active_rules())
                      + " active rules>";
             })
        .def(
            "set_alphabet",
            [](fpsemigroup::KnuthBendix& kb, std::string const& a) {
              kb.set_alphabet(to_latin1(a));
            },
            py::arg("a"),
            R"pbdoc(
               Set the alphabet of the finitely presented semigroup.

               :Parameters: **a** (str) - the alphabet.

               :Returns: None
             )pbdoc")
        .def("set_alphabet",
             py::overload_cast<size_t>(&fpsemigroup::KnuthBendix::set_alphabet),
             py::arg("n"),
             R"pbdoc(
               Set the size of the alphabet.

               :Parameters: **n** (int) - the number of letters.
               :Returns: None
             )pbdoc")
        .def(
            "alphabet",
            [](fpsemigroup::KnuthBendix const& kb) -> py::str {
              return from_latin1(kb.alphabet());
            },
            R"pbdoc(
              Returns the alphabet.

              :Parameters: None
              :Returns: A string.
            )pbdoc")
        .def(
            "alphabet",
            [](fpsemigroup::KnuthBendix const& kb, size_t i) -> py::str {
              return from_latin1(kb.alphabet(i));
            },
            py::arg("i"),
            R"pbdoc(
              Returns the i-th letter of the alphabet of the finitely presented
              semigroup represented by this.

              :Parameters: **i** (int) - the index of the letter.

              :Returns: A string.
            )pbdoc")
        .def(
            "add_rule",
            [](fpsemigroup::KnuthBendix& kb,
               std::string const&        u,
               std::string const&        v) -> void {
              kb.add_rule(to_latin1(u), to_latin1(v));
            },
            py::arg("u"),
            py::arg("v"),
            R"pbdoc(
               Add a rule.

               :Parameters: - **u** (str) - the left-hand side of the rule
                              being added.
                            - **v** (str) - the right-hand side of the rule
                              being added.

               :Returns: None
             )pbdoc")
        .def("size",
             &fpsemigroup::KnuthBendix::size,
             R"pbdoc(
               Returns the size of the finitely presented semigroup.

               :Parameters: None

               :return: A ``int`` or :py:obj:`POSITIVE_INFINITY`.
             )pbdoc")
        .def("number_of_active_rules",
             &fpsemigroup::KnuthBendix::number_of_active_rules,
             R"pbdoc(
               Returns the current number of active rules in the KnuthBendix
               instance.

               :Parameters: None
               :return: An ``int``.
           )pbdoc")
        .def("confluent",
             &fpsemigroup::KnuthBendix::confluent,
             R"pbdoc(
               Check if the KnuthBendix instance is confluent.

               :Parameters: None

               :return:
                 ``True`` if the KnuthBendix instance is confluent and
                 ``False`` if it is not.
           )pbdoc")
        .def(
            "active_rules",
            [](fpsemigroup::KnuthBendix const& kb) {
              auto result = kb.active_rules();
              std::for_each(result.begin(), result.end(), [](rule_type& rule) {
                rule.first  = from_latin1(rule.first);
                rule.second = from_latin1(rule.second);
              });
              return result;
            },
            R"pbdoc(
               Returns a copy of the active rules of the KnuthBendix instance.

               :Parameters: None

               :return: A copy of the currently active rules.
             )pbdoc")
        .def(
            "number_of_rules",
            [](fpsemigroup::KnuthBendix const& kb) {
              return kb.number_of_rules();
            },
            R"pbdoc(
               Returns the number of rules of the finitely presented semigroup.

               :Parameters: None
               :return: A ``int``.
             )pbdoc")
        .def("set_identity",
             py::overload_cast<letter_type>(
                 &fpsemigroup::KnuthBendix::set_identity),
             py::arg("id"),
             R"pbdoc(
               Set a character in alphabet() to be the identity using its index.

               :Parameters: **id** (int) - the index of the character to be the
                                           identity.

               :Returns: None
             )pbdoc")
        .def(
            "set_identity",
            [](fpsemigroup::KnuthBendix& kb, std::string const& id) {
              kb.set_identity(to_latin1(id));
            },
            py::arg("id"),
            R"pbdoc(
               Set a character in alphabet() to be the identity.

               :Parameters: **id** (str) - a string containing the character to
                                           be the identity.

               :Returns: None
            )pbdoc")
        .def(
            "identity",
            [](fpsemigroup::KnuthBendix const& kb) {
              return from_latin1(kb.identity());
            },
            R"pbdoc(
               Returns the identity of this, or raises an exception if there
               isn't one.

               :Parameters: None
               :return: A string of length 1.
            )pbdoc")
        .def(
            "set_inverses",
            [](fpsemigroup::KnuthBendix& kb, std::string const& a) {
              return kb.set_inverses(to_latin1(a));
            },
            py::arg("a"),
            R"pbdoc(
               Set the inverses of letters in alphabet().

               :param a: a string containing the inverses of the generators.
               :type a: str

               :return: None
             )pbdoc")
        .def(
            "inverses",
            [](fpsemigroup::KnuthBendix const& kb) {
              return from_latin1(kb.inverses());
            },
            R"pbdoc(
               Returns the inverses of this, or raises an exception if there aren't
               any.

               :Parameters: None
               :return: A ``str``.
             )pbdoc")
        .def("is_obviously_finite",
             &fpsemigroup::KnuthBendix::is_obviously_finite,
             R"pbdoc(
               Return ``True`` if the finitely presented semigroup represented by
               this is obviously finite, and ``False`` if it is not obviously
               finite.

               :return: A ``bool``.
             )pbdoc")
        .def("is_obviously_infinite",
             &fpsemigroup::KnuthBendix::is_obviously_infinite,
             R"pbdoc(
               Returns ``True`` if the finitely presented semigroup represented by
               this is obviously infinite, and ``False`` if it is not obviously
               infinite.

               :return: A bool.
             )pbdoc")
        .def(
            "equal_to",
            [](fpsemigroup::KnuthBendix& kb,
               std::string const&        u,
               std::string const&        v) {
              return kb.equal_to(to_latin1(u), to_latin1(v));
            },
            py::arg("u"),
            py::arg("v"),
            R"pbdoc(
               Check if two words represent the same element.

               :Parameters: - **u** (str) - first word for comparison.
                            - **v** (str) - second word for comparison.

               :Returns:
                 ``True`` if the strings ``u`` and ``v`` represent the same
                 element of the finitely presented semigroup, and ``False``
                 otherwise.
              )pbdoc")
        .def("equal_to",
             py::overload_cast<word_type const&, word_type const&>(
                 &fpsemigroup::KnuthBendix::equal_to),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               Check if two words represent the same element.

               :Parameters: - **u** (List[int]) - first word for comparison.
                            - **v** (List[int]) - second word for comparison.

               :Returns:
                 ``True`` if the words ``u`` and ``v`` represent the same
                 element of the finitely presented semigroup, and ``False``
                 otherwise.
             )pbdoc")
        .def(
            "normal_form",
            [](fpsemigroup::KnuthBendix& kb, std::string const& w) {
              return from_latin1(kb.normal_form(to_latin1(w)));
            },
            py::arg("w"),
            R"pbdoc(
               Returns a normal form for a string.

               :Parameters: **w** (str) - the word whose normal form we want to find.

               :Returns: A ``str``.
             )pbdoc")
        .def("normal_form",
             py::overload_cast<word_type const&>(
                 &fpsemigroup::KnuthBendix::normal_form),
             py::arg("w"),
             R"pbdoc(
               Returns a normal form for a word.

               :Parameters: **w** (List[int]) - the word whose normal form we
                            want to find.

               :Returns:
                 The normal form of the parameter ``w``.
             )pbdoc")
        .def("add_rule",
             py::overload_cast<word_type const&, word_type const&>(
                 &fpsemigroup::KnuthBendix::add_rule),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               Add a rule.

               :Parameters: - **u** (List[int]) - the left-hand side of the rule.
                            - **v** (List[int]) - the right-hand side of the rule.

               :Returns: None
             )pbdoc")
        .def("contains_empty_string",
             &fpsemigroup::KnuthBendix::contains_empty_string,
             R"pbdoc(
               Returns whether or not the empty string belongs to the finitely
               presented semigroup represented by this.

               :return: A ``bool``.
             )pbdoc")
        .def("number_of_normal_forms",
             &fpsemigroup::KnuthBendix::number_of_normal_forms,
             py::arg("min"),
             py::arg("max"),
             R"pbdoc(
               Returns the number of normal forms with length in a given range.

               :param min: the minimum length of a normal form to count
               :type min: int
               :param max:
                 one larger than the maximum length of a normal form to count.
               :type max: int

               :return: An ``int``.
             )pbdoc")
        .def("validate_letter",
             py::overload_cast<char>(&fpsemigroup::KnuthBendix::validate_letter,
                                     py::const_),
             py::arg("c"),
             R"pbdoc(
               Validates a letter.

               :Parameters: **c** (str) - the letter to validate.

               :Returns: None
             )pbdoc")
        .def("validate_letter",
             py::overload_cast<letter_type>(
                 &fpsemigroup::KnuthBendix::validate_letter, py::const_),
             py::arg("c"),
             R"pbdoc(
               Validates a letter.

               :Parameters: **c** (int) - the letter to validate.

               :Returns: None
             )pbdoc")
        .def(
            "validate_word",
            [](fpsemigroup::KnuthBendix const& kb, std::string const& w) {
              kb.validate_word(to_latin1(w));
            },
            py::arg("w"),
            R"pbdoc(
               Validates a word.

               :Parameters: **w** (str) - the word to validate.

               :Returns: None
             )pbdoc")
        .def("validate_word",
             py::overload_cast<word_type const&>(
                 &fpsemigroup::KnuthBendix::validate_word, py::const_),
             py::arg("w"),
             R"pbdoc(
               Validates a word.

               This function checks that the word ``w`` is defined over the
               same alphabet as an instance of :py:class:`KnuthBendix`.

               :Parameters: **w** (List[int]) - the word to validate.

               :Returns: None
           )pbdoc")
        .def(
            "froidure_pin",
            [](fpsemigroup::KnuthBendix& x) { return x.froidure_pin(); },
            R"pbdoc(
               Returns a :py:class:`FroidurePin` instance isomorphic to the
               finitely presented semigroup defined by this.

               :Parameters: None

               :return: A :py:class:`FroidurePin`.
            )pbdoc")
        .def(
            "has_froidure_pin",
            [](fpsemigroup::KnuthBendix const& x) {
              return x.has_froidure_pin();
            },
            R"pbdoc(
               Returns ``True`` if a :py:class:`FroidurePin` instance isomorphic
               to the finitely presented semigroup defined by this has already
               been computed, and ``False`` if not.

               :Parameters: None

               :return: A ``bool``.
             )pbdoc")
        .def("run_for",
             (void(fpsemigroup::KnuthBendix::  // NOLINT(whitespace/parens)
                       *)(std::chrono::nanoseconds))
                 & Runner::run_for,
             py::arg("t"),
             runner_doc_strings::run_for)
        .def("run_until",
             (void(fpsemigroup::KnuthBendix::  // NOLINT(whitespace/parens)
                       *)(std::function<bool()>&))
                 & Runner::run_until,
             py::arg("func"),
             runner_doc_strings::run_until)
        .def("run", &fpsemigroup::KnuthBendix::run, runner_doc_strings::run)
        .def("kill", &fpsemigroup::KnuthBendix::kill, runner_doc_strings::kill)
        .def("dead", &fpsemigroup::KnuthBendix::dead, runner_doc_strings::dead)
        .def("finished",
             &fpsemigroup::KnuthBendix::finished,
             runner_doc_strings::finished)
        .def("started",
             &fpsemigroup::KnuthBendix::started,
             runner_doc_strings::started)
        .def(
            "running",
            [](fpsemigroup::KnuthBendix const& kb) { return kb.running(); },
            runner_doc_strings::running)
        .def("timed_out",
             &fpsemigroup::KnuthBendix::timed_out,
             runner_doc_strings::timed_out)
        .def("stopped",
             &fpsemigroup::KnuthBendix::stopped,
             runner_doc_strings::stopped)
        .def("stopped_by_predicate",
             &fpsemigroup::KnuthBendix::stopped_by_predicate,
             runner_doc_strings::stopped_by_predicate)
        .def("report",
             &fpsemigroup::KnuthBendix::report,
             runner_doc_strings::report)
        .def("report_every",
             (void(fpsemigroup::KnuthBendix::  // NOLINT(whitespace/parens)
                       *)(std::chrono::nanoseconds))
                 & Runner::report_every,
             py::arg("t"),
             runner_doc_strings::report_every)
        .def("report_why_we_stopped",
             &fpsemigroup::KnuthBendix::report_why_we_stopped,
             runner_doc_strings::report_why_we_stopped)
        .def("char_to_uint",
             &fpsemigroup::KnuthBendix::char_to_uint,
             py::arg("a"),
             R"pbdoc(
               Convert a single letter ``string`` to a ``int`` representing the
               same generator.

               :param a: the string to convert.
               :type a: str

               :return: an ``int``.
             )pbdoc")
        .def("uint_to_char",
             &fpsemigroup::KnuthBendix::uint_to_char,
             py::arg("a"),
             R"pbdoc(
               Convert an ``int`` to a ``char`` representing the
               same generator of the finitely presented semigroup represented
               by this.

               :param a: the letter to convert.
               :type a: int

               :return: A ``str``.
             )pbdoc")
        .def(
            "string_to_word",
            [](fpsemigroup::KnuthBendix const& kb, std::string const& w) {
              return kb.string_to_word(to_latin1(w));
            },
            py::arg("w"),
            R"pbdoc(
               Convert a string to a list of ``int`` representing the same
               element of the finitely presented semigroup represented by this.

               :param w: the string to convert.
               :type w: str

               :return: a ``List[int]``.
           )pbdoc")
        .def(
            "word_to_string",
            [](fpsemigroup::KnuthBendix const& kb, word_type const& w) {
              return from_latin1(kb.word_to_string(w));
            },
            py::arg("w"),
            R"pbdoc(
               Convert a list of ``int`` to a string representing the same
               element of the finitely presented semigroup represented by this.

               :param w: the list to convert.
               :type w: List[int]

               :return: A string.
             )pbdoc")
        .def("to_gap_string",
             &fpsemigroup::KnuthBendix::to_gap_string,
             R"pbdoc(
               Returns a string containing GAP commands for defining a finitely
               presented semigroup equal to that represented by this.

               :Parameters: None

               :return: A string.
             )pbdoc")
        .def("check_confluence_interval",
             &fpsemigroup::KnuthBendix::check_confluence_interval,
             py::arg("val"),
             R"pbdoc(
              Set the interval at which confluence is checked.

              :param val: the new value of the interval.
              :type val: int

              :return: ``self``.
            )pbdoc")
        .def("overlap_policy",
             &fpsemigroup::KnuthBendix::overlap_policy,
             py::arg("val"),
             R"pbdoc(
               Set the overlap policy.

               :param val: the maximum number of rules.
               :type val: int

               :return: ``self``.
            )pbdoc")
        .def("max_overlap",
             &fpsemigroup::KnuthBendix::max_overlap,
             py::arg("val"),
             R"pbdoc(
               Set the maximum length of overlaps to be considered.

               :param val: the new value of the maximum overlap length.
               :type val: int

               :return: ``self``.
             )pbdoc")
        .def("max_rules",
             &fpsemigroup::KnuthBendix::max_rules,
             py::arg("val"),
             R"pbdoc(
               Set the maximum number of rules.

               :param val: the maximum number of rules.
               :type val: int

               :return: ``self``.
             )pbdoc")
        .def("knuth_bendix_by_overlap_length",
             &fpsemigroup::KnuthBendix::knuth_bendix_by_overlap_length,
             R"pbdoc(
               Run the Knuth-Bendix algorithm by overlap length.

               :Parameters: None

               :return: None
             )pbdoc")
        .def(
            "rules",
            [](fpsemigroup::KnuthBendix const& kb) {
              return py::make_iterator(kb.cbegin_rules(), kb.cend_rules());
            },
            R"pbdoc(
              Returns an iterator to the generating pairs of the congruence.
            )pbdoc")
        .def(
            "normal_forms",
            [](fpsemigroup::KnuthBendix& kb, size_t const mn, size_t const mx) {
              return py::make_iterator(kb.cbegin_normal_forms(mn, mx),
                                       kb.cend_normal_forms());
            },
            py::arg("mn"),
            py::arg("mx"),
            R"pbdoc(
              Returns an iterator to the normal forms with length in the given
              range.

              :param mn: the minimum length.
              :type mn: int
              :param mx: the maximum length.
              :type mx: int

              :return: An iterator.
            )pbdoc")
        .def(
            "normal_forms_alphabet",
            [](fpsemigroup::KnuthBendix& kb,
               std::string const&        lphbt,
               size_t const              mn,
               size_t const              mx) {
              return py::make_iterator(kb.cbegin_normal_forms(lphbt, mn, mx),
                                       kb.cend_normal_forms());
            },
            py::arg("lphbt"),
            py::arg("mn"),
            py::arg("mx"),
            R"pbdoc(
              Returns an iterator to the normal forms of the congruence using
              the specified alphabet, and with length in the given range.

              :param lphbt: the alphabet.
              :type lphbt: str
              :param mn: the minimum length.
              :type mn: int
              :param mx: the maximum length.
              :type mx: int

              :return: An iterator.
            )pbdoc")
        .def("add_rule",
             py::overload_cast<rule_type>(&fpsemigroup::KnuthBendix::add_rule),
             py::arg("rel"),
             R"pbdoc(
               Add a rule.

               :Parameters: **rel** (Tuple[str, str]) - the rule being added.

               :Returns: None
            )pbdoc")
        .def("add_rules",
             py::overload_cast<std::vector<rule_type> const&>(
                 &fpsemigroup::KnuthBendix::add_rules),
             py::arg("rels"),
             R"pbdoc(
               Add the rules in a list.

               :Parameters: **rels** (List[Tuple[str, str]]) - list of rules to
                            add.

               :Returns: None
             )pbdoc")
        .def("add_rules",
             py::overload_cast<FroidurePinBase&>(
                 &fpsemigroup::KnuthBendix::add_rules),
             py::arg("rels"),
             R"pbdoc(
               Add the rules in a ``FroidurePin`` instance.

               :Parameters: **rels** (FroidurePin) - the instance.

               :Returns: None
             )pbdoc")
        .def("gilman_digraph",
             &fpsemigroup::KnuthBendix::gilman_digraph,
             py::return_value_policy::copy,
             R"pbdoc(
               Returns the associated Gilman digraph (or automata).

               :Parameters: None

               :return: A copy of an :py:class:`ActionDigraph`.
             )pbdoc")
        .def("rewrite",
             py::overload_cast<std::string>(&fpsemigroup::KnuthBendix::rewrite,
                                            py::const_),
             R"pbdoc(
               Rewrite a word.

               Rewrites a copy of the string  ``w`` rewritten according to the current
               rules in the ``KnuthBendix`` instance.

               :param w: the word to rewrite.
               :type w: str

               :returns: A copy of the argument ``w`` after it has been rewritten.
             )pbdoc");
  }  // namespace
}  // namespace libsemigroups
