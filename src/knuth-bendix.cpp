// libsemigroups - C++ library for semigroups and monoids
// Copyright (C) 2021-2024 James D. Mitchell, Maria Tsalakou
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

#include <libsemigroups/knuth-bendix.hpp>  // for KnuthBendix, KnuthBendix::option...
#include <libsemigroups/obvinf.hpp>        // for WordGraph
#include <libsemigroups/runner.hpp>        // for Runner
#include <libsemigroups/types.hpp>         // for word_type, letter_type
#include <libsemigroups/word-graph.hpp>  // for WordGraph

// pybind11....
#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>  // for class_, make_iterator, enum_, init
#include <pybind11/pytypes.h>   // for py::str
#include <pybind11/stl.h>

#include <Python.h>  // for PyUnicode_DecodeLatin1

// libsemigroups_pybind11....
#include "doc-strings.hpp"  // for init_knuth_bendix
#include "main.hpp"         // for init_knuth_bendix

namespace py = pybind11;

namespace libsemigroups {

  namespace {

    template <typename Rewriter>
    void bind_knuth_bendix(py::module& m, std::string const& name) {
      py::class_<KnuthBendix<Rewriter>, CongruenceInterface> kb(m,
                                                                name.c_str());
      kb.def("__repr__",
             [](KnuthBendix<Rewriter>& kb) { return knuth_bendix::repr(kb); });
      //////////////////////////////////////////////////////////////////////////
      // Initialisers
      //////////////////////////////////////////////////////////////////////////
      kb.def(py::init<KnuthBendix<Rewriter> const&>())
          .def(py::init<congruence_kind>())
          .def(py::init<congruence_kind, Presentation<std::string> const&>())
          .def(py::init<congruence_kind, Presentation<word_type> const&>());
      //////////////////////////////////////////////////////////////////////////
      // Setters and getters for optional parameters
      //////////////////////////////////////////////////////////////////////////
      kb.def(
            "batch_size",
            py::overload_cast<>(&KnuthBendix<Rewriter>::batch_size, py::const_),
            R"pbdoc(
Return the number of pending rules that must accumulate before they are reduced,
processed, and added to the system.

The default value is ``128``. A value of ``1`` means :py:meth:`run` should
attempt to add each rule as they are created without waiting for rules to
accumulate.

:Parameters: None
:return: The batch size.
:rtype: int

.. seealso:: :py:meth:`run`.
            )pbdoc")
          .def("batch_size",
               py::overload_cast<size_t>(&KnuthBendix<Rewriter>::batch_size),
               py::arg("val"),
               R"pbdoc(
Specify the number of pending rules that must accumulate before they are
reduced, processed, and added to the system.

The default value is ``128``, and should be set to ``1`` if :py:meth:`run`
should attempt to add each rule as they are created without waiting for rules
to accumulate.

:param val:	The new value of the batch size.
:type val: int
:return: A reference to ``self``.

.. seealso:: :py:meth:`run`.
               )pbdoc")
          .def("check_confluence_interval",
               py::overload_cast<>(
                   &KnuthBendix<Rewriter>::check_confluence_interval,
                   py::const_),
               R"pbdoc(
Return the interval at which confluence is checked.

The function :py:meth:`run` periodically checks if the system is already
confluent. This function can be used to return how frequently this happens. It
is the number of new overlaps that should be considered before checking
confluence.

:Parameters: None
:return: The interval at which confluence is checked.
:rtype: int

.. seealso:: :py:meth:`run`.
              )pbdoc")
          .def("check_confluence_interval",
               py::overload_cast<size_t>(&libsemigroups::KnuthBendix<
                                         Rewriter>::check_confluence_interval),
               py::arg("val"),
               R"pbdoc(
Set the interval at which confluence is checked.

The function :py:meth`run` periodically checks if the system is already
confluent. This function can be used to set how frequently this happens. It is
the number of new overlaps that should be considered before checking confluence.
Setting this value too low can adversely affect the performance of
:py:meth:`run`.

The default value is ``4096``, and should be set to
:py:obj:`LIMIT_MAX` if :py:meth:`run` should never check if the
system is already confluent.

:param val: The new value of the interval.
:type val: int
:return: A reference to ``self``.
              )pbdoc")
          .def("max_overlap",
               py::overload_cast<>(&KnuthBendix<Rewriter>::max_overlap,
                                   py::const_),
               R"pbdoc(
Return the maximum length of overlaps to be considered.

This function returns the maximum length of the overlap of two left hand sides
of rules that should be considered in :py:meth:`run`.

:Parameters: None
:return: The maximum overlap length
:rtype: int

.. seealso:: :py:meth:`run`.
               )pbdoc")
          .def("max_overlap",
               py::overload_cast<size_t>(
                   &libsemigroups::KnuthBendix<Rewriter>::max_overlap),
               py::arg("val"),
               R"pbdoc(
Set the maximum length of overlaps to be considered.

This function can be used to specify the maximum length of the overlap of two
left hand sides of rules that should be considered in  :py:meth:`run`.

If this value is less than the longest left hand side of a rule, then
:py:meth:`run` can terminate without the system being confluent.

:param val: The new value of the maximum overlap length.
:type val: int
:return: A reference to ``self``.

..seealso:: :py:meth:`run`.
              )pbdoc")
          .def("max_rules",
               py::overload_cast<>(&KnuthBendix<Rewriter>::max_rules,
                                   py::const_),
               R"pbdoc(
Return the maximum number of rules.

This member function returns the (approximate) maximum number of rules that the
system should contain. If this is number is exceeded in calls to :py:meth:`run`
or
py:meth:`knuth_bendix.by_overlap_length<libsemigroups_pybind11.knuth_bendix.by_overlap_length>`,
then they will terminate and the system may not be confluent.

:Parameters: None
:return: The maximum number of rules the system should contain.
:rtype: int

.. seealso:: :py:meth:`run`.
               )pbdoc")
          .def("max_rules",
               py::overload_cast<size_t>(
                   &libsemigroups::KnuthBendix<Rewriter>::max_rules),
               py::arg("val"),
               R"pbdoc(
Set the maximum number of rules.

This member function sets the (approximate) maximum number of rules that the
system should contain. If this is number is exceeded in calls to :py:meth:`run`
or
:py:meth:`knuth_bendix.by_overlap_length<libsemigroups_pybind11.knuth_bendix.by_overlap_length>`,
then they will terminate and the system may not be confluent.

By default this value is :py:obj:`POSITIVE_INFINITY`.

:param val: The maximum number of rules.
:type val: int
:return: A reference to ``self``.

.. seealso:: :py:meth:`run`.
              )pbdoc")
          .def("overlap_policy",
               py::overload_cast<>(&KnuthBendix<Rewriter>::overlap_policy,
                                   py::const_),
               R"pbdoc(
Return the overlap policy.

This function returns the way that the length of an overlap of two words in the
system is measured.

:Parameters: None
:return: The overlap policy.
:rtype: overlap

.. seealso:: :py:class:`overlap`.
               )pbdoc")
          .def("overlap_policy",
               py::overload_cast<
                   typename KnuthBendix<Rewriter>::options::overlap>(
                   &KnuthBendix<Rewriter>::overlap_policy),
               py::arg("val"),
               R"pbdoc(
Set the overlap policy.

This function can be used to determine the way that the length of an overlap of
two words in the system is measured.

:param val: The overlap policy.
:type val: overlap
:return: A reference to `self`.

.. seealso:: :py:class:`overlap`
               )pbdoc");
      //////////////////////////////////////////////////////////////////////////
      // Member functions for rules and rewriting
      //////////////////////////////////////////////////////////////////////////
      // TODO: Delete or include. Do CongIntf need this functionality?
      // kb.def("validate_word",
      //        &libsemigroups::KnuthBendix<Rewriter>::validate_word,
      //        py::arg("w"),
      //        R"pbdoc(
      //         Check if every letter of a word is in the presentation's
      //         alphabet.

      //         :param w: word to validate.
      //         :type w: ??
      //         :return: (None)
      //         )pbdoc")
      kb.def(
            "presentation",
            [](KnuthBendix<Rewriter>& kb) { return kb.presentation(); },
            R"pbdoc(
Return the presentation defined by the rewriting system

:Parameters: None
:return: The presentation
:rtype: Presentation
            )pbdoc")
          .def("number_of_active_rules",
               &libsemigroups::KnuthBendix<Rewriter>::number_of_active_rules,
               R"pbdoc(
Return the current number of active rules.

:Parameters: None
:return: The current number of active rules.
:rtype: int
              )pbdoc")
          .def("number_of_inactive_rules",
               &libsemigroups::KnuthBendix<Rewriter>::number_of_inactive_rules,
               R"pbdoc(
Return the current number of inactive rules.

:Parameters: None
:return: The current number of inactive rules.
:rtype: int
              )pbdoc")
          .def("total_rules",
               &libsemigroups::KnuthBendix<Rewriter>::total_rules,
               R"pbdoc(
Return the number of rules that have been created

Return the total number of Rule instances that have been created whilst whilst
the Knuth-Bendix algorithm has been running. Note that this is not the sum of
:py:meth:`number_of_active_rules` and :py:meth:`number_of_inactive_rules`, due
to the re-initialisation of rules where possible.

:Parameters: None
:return: The total number of rules.
:rtype: int
              )pbdoc")
          .def(
              "active_rules",
              [](KnuthBendix<Rewriter>& kb) {
                auto rules = kb.active_rules();
                return py::make_iterator(rx::begin(rules), rx::end(rules));
              },
              R"pbdoc(
Return a copy of the active rules.

This member function returns an iterator consisting of the pairs of strings
which represent the rewriting rules. The first entry in every such pair is
greater than the second according to the reduction ordering of the
:py:class:`KnuthBendix<KnuthBendixRewriteTrie>` instance. The rules are sorted
according to the reduction ordering used by the rewriting system, on the first
entry.

:Parameters: None
:return: A copy of the currently active rules
:rtype: collections.abc.Iterator[(str, str)]
              )pbdoc")
          .def("rewrite",
               &libsemigroups::KnuthBendix<Rewriter>::rewrite,
               py::arg("w"),
               R"pbdoc(
Rewrite a word.

Rewrites a copy of the word *w*, rewritten according to the current rules in the
KnuthBendix instance.

:param w: the word to rewrite.
:type w: str
:return: A copy of the argument *w* after it has been rewritten.
:rtype: str
               )pbdoc");
      //////////////////////////////////////////////////////////////////////////
      // Main member functions
      //////////////////////////////////////////////////////////////////////////
      kb.def("confluent",
             &libsemigroups::KnuthBendix<Rewriter>::confluent,
             R"pbdoc(
Check `confluence <https://w.wiki/9DA>`_ of the current rules.

:Parameters: None
:return: ``True`` if the :py:class:`KnuthBendix<KnuthBendixRewriteTrie>`
  instance is confluent and ``False`` if it is not.
:rtype: bool
              )pbdoc")
          .def("confluent_known",
               &libsemigroups::KnuthBendix<Rewriter>::confluent_known,
               R"pbdoc(
Check if the current system knows the state of confluence of the current rules.

:Parameters: None
:return: ``True`` if the confluence of the rules in the
  :py:class:`KnuthBendix<KnuthBendixRewriteTrie>` instance is known, and
  ``False`` if it is not.
:rtype: bool
              )pbdoc")
          .def(
              "gilman_graph",
              [](KnuthBendix<Rewriter>& kb) {
                // TODO should different node types be possible?
                WordGraph<uint32_t> g = kb.gilman_graph();
                return g;
              },
              py::return_value_policy::copy,
              // REVIEW: Should WordGraph be formatted as code, or as text?
              R"pbdoc(
Return the Gilman :py:class:`WordGraph`.

The Gilman :py:class:`WordGraph` is a digraph where the labels of the paths from
the initial node (corresponding to the empty word) correspond to the short-lex
normal forms of the semigroup elements.

The semigroup is finite if the graph is cyclic, and infinite otherwise.

:Parameters: None
:return: The Gilman :py:class:`WordGraph`.
:rtype: WordGraph

.. warning:: This will terminate when the
  :py:class:`KnuthBendix<KnuthBendixRewriteTrie>` instance is reduced and
  confluent, which might be never.

.. seealso:: :py:meth:`number_of_normal_forms` and :py:meth:`normal_forms`.
                )pbdoc")
          .def("gilman_graph_node_labels",
               &libsemigroups::KnuthBendix<Rewriter>::gilman_graph_node_labels,
               R"pbdoc(
Return the node labels of the Gilman :py:class:`WordGraph`

Return the node labels of the Gilman :py:class:`WordGraph`, corresponding to the
unique prefixes of the left-hand sides of the rules of the rewriting system.

:Parameters: None
:return: The node labels of the Gilman :py:class:`WordGraph`
:rtype: List[str]

.. seealso:: :py:meth:`gilman_graph`.
                )pbdoc");
      //////////////////////////////////////////////////////////////////////////
      // Attributes
      //////////////////////////////////////////////////////////////////////////
      kb.def("number_of_classes",
             &libsemigroups::KnuthBendix<Rewriter>::number_of_classes,
             cong_intf_doc_strings::number_of_classes)
          .def("equal_to",
               &libsemigroups::KnuthBendix<Rewriter>::equal_to,
               py::arg("u"),
               py::arg("v"),
               R"pbdoc(
Check if a pair of words are equivalent with respect to the system.

:param u: a word over the generators of the semigroup.
:type u: List[int]
:param v: a word over the generators of the semigroup.
:type v: List[int]

:return: ``True`` if the word *u* is equivalent to the word *v*, and ``False``
  otherwise.
              )pbdoc")
          .def("contains",
               py::overload_cast<word_type const&, word_type const&>(
                   &libsemigroups::KnuthBendix<Rewriter>::contains),
               py::arg("u"),
               py::arg("v"),
               cong_intf_doc_strings::contains)
          .def("normal_form",
               &libsemigroups::KnuthBendix<Rewriter>::normal_form,
               R"pbdoc()pbdoc");
      //////////////////////////////////////////////////////////////////////////
      // Inherited from CongruenceInterface
      //////////////////////////////////////////////////////////////////////////
      kb.def("kind",
             py::overload_cast<>(&libsemigroups::KnuthBendix<Rewriter>::kind,
                                 py::const_),
             R"pbdoc(
The handedness of the congruence (left, right, or 2-sided).

:return:
   A congruence_kind.
               )pbdoc")
          .def("kind",
               py::overload_cast<congruence_kind>(
                   &libsemigroups::KnuthBendix<Rewriter>::kind))
          .def("number_of_generating_pairs",
               &KnuthBendix<Rewriter>::number_of_generating_pairs)
          .def("add_pair",
               py::overload_cast<word_type const&, word_type const&>(
                   &libsemigroups::KnuthBendix<Rewriter>::add_pair))
          .def("generating_pairs", &KnuthBendix<Rewriter>::generating_pairs);
      //////////////////////////////////////////////////////////////////////////
      // Helpers
      //////////////////////////////////////////////////////////////////////////
      m.def("by_overlap_length", [](KnuthBendix<Rewriter>& kb) {
        knuth_bendix::by_overlap_length(kb);
      });
      m.def(
          "normal_forms",
          [](KnuthBendix<Rewriter>& kb) {
            return knuth_bendix::normal_forms(kb);
          },
          R"pbdoc(Test)pbdoc");
      m.def("non_trivial_classes",
            [](KnuthBendix<Rewriter>& kb1, KnuthBendix<Rewriter>& kb2) {
              return knuth_bendix::non_trivial_classes(kb1, kb2);
            });
      m.def("is_reduced", [](KnuthBendix<Rewriter>& kb1) {
        return knuth_bendix::is_reduced(kb1);
      });
      // REVIEW should the the report guard be turned off for this?
      m.def(
          "redundant_rule",
          [](Presentation<std::string> const& p, std::chrono::nanoseconds t) {
            return std::distance(p.rules.cbegin(),
                                 knuth_bendix::redundant_rule(p, t));
          },
          R"pbdoc(
Return the index of the the left hand side of a redundant rule.

Starting with the last rule in the presentation, this function attempts to
run the Knuth-Bendix algorithm on the rules of the presentation except for
the given omitted rule. For every such omitted rule, Knuth-Bendix is run for
the length of time indicated by the second parameter ``t`` and then it is
checked if the omitted rule can be shown to be redundant (rewriting both
sides of the omitted rule using the other rules using the output of the, not
necessarily finished, Knuth-Bendix algorithm).

If the omitted rule can be shown to be redundant in this way, then the index
of its left hand side is returned.

If no rule can be shown to be redundant in this way, then ``len(p.rules)``
is returned.

:warning:
  The progress of the Knuth-Bendix algorithm may differ between different
  calls to this function even if the parameters are identical. As such this
  is non-deterministic, and may produce different results with the same
  input.

:param p: the presentation.
:type p: Presentation
:param t: time to run KnuthBendix for every omitted rule
:type t: datetime.timedelta

:return: The index of a redundant rule (if any).

.. doctest::

  >>> from libsemigroups_pybind11 import knuth_bendix, presentation, Presentation
  >>> from datetime import timedelta
  >>> p = Presentation("ab")
  >>> presentation.add_rule(p, "ab", "ba")
  >>> presentation.add_rule(p, "bab", "abb")
  >>> t = timedelta(seconds = 1)
  >>> p.rules
  ['ab', 'ba', 'bab', 'abb']
  >>> knuth_bendix.redundant_rule(p, t)
  2
            )pbdoc");
      m.def("is_obviously_infinite", [](KnuthBendix<Rewriter>& kb) {
        return is_obviously_infinite(kb);
      });
    }
  }  // namespace

  void init_knuth_bendix(py::module& m) {
    bind_knuth_bendix<RewriteFromLeft>(m, "KnuthBendixRewriteFromLeft");
    bind_knuth_bendix<RewriteTrie>(m, "KnuthBendixRewriteTrie");
  }
}  // namespace libsemigroups
