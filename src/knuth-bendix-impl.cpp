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

// TODO(1) this file is not organised like any other:
// * kb should be called thing
// * we don't chain the "def"s in the other files (easier to wrangle)

// C std headers....
#include <cstddef>  // for size_t

// libsemigroups....

#include <libsemigroups/knuth-bendix.hpp>  // for KnuthBendixImpl, KnuthBendixImpl::option...
#include <libsemigroups/obvinf.hpp>      // for is_obviously_infinite
#include <libsemigroups/runner.hpp>      // for Runner
#include <libsemigroups/types.hpp>       // for word_type, letter_type
#include <libsemigroups/word-graph.hpp>  // for WordGraph

// pybind11....
#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>  // for class_, make_iterator, enum_, init
#include <pybind11/pytypes.h>   // for py::str
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "cong-common.hpp"  // for contains etc
#include "main.hpp"         // for init_detail_knuth_bendix_impl

// TODO(2): remove unused doc from this file.

namespace libsemigroups {
  namespace py = pybind11;

  namespace detail {

    template <typename Rewriter>
    void bind_detail_knuth_bendix_impl(py::module& m, std::string const& name) {
      //////////////////////////////////////////////////////////////////////////
      // KnuthBendixImpl class definition . . .
      //////////////////////////////////////////////////////////////////////////

      py::class_<KnuthBendixImpl<Rewriter>, detail::CongruenceCommon> thing(
          m, name.c_str());

      //////////////////////////////////////////////////////////////////////////
      // KnuthBendixImpl nested classes . . .
      //////////////////////////////////////////////////////////////////////////

      py::class_<typename KnuthBendixImpl<Rewriter>::options> options(thing,
                                                                      "options",
                                                                      R"pbdoc(
This class contains various options that can be used to control the
behaviour of Knuth-Bendix.)pbdoc");

      py::options enum_options;
      enum_options.disable_enum_members_docstring();
      py::enum_<typename KnuthBendixImpl<Rewriter>::options::overlap>(options,
                                                                      "overlap",
                                                                      R"pbdoc(
:sig=(self: KnuthBendix.options.overlap, value: int) -> None:

Values for specifying how to measure the length of an overlap.

The values in this enum determine how a :any:`KnuthBendix`
instance measures the length :math:`d(AB, BC)` of the overlap of
two words :math:`AB` and :math:`BC`.

The valid values are:

.. py:attribute:: overlap.ABC
  :value: <overlap.ABC: 0>

  :math:`d(AB, BC) = |A| + |B| + |C|`

.. py:attribute:: overlap.AB_BC
  :value: <overlap.AB_BC: 1>

  :math:`d(AB, BC) = |AB| + |BC|`

.. py:attribute:: overlap.MAX_AB_BC
  :value: <overlap.MAX_AB_BC: 2>

  :math:`d(AB, BC) = max(|AB|, |BC|)`

.. seealso:: :any:`KnuthBendix.overlap_policy`
)pbdoc")
          .value("ABC", KnuthBendixImpl<Rewriter>::options::overlap::ABC)
          .value("AB_BC", KnuthBendixImpl<Rewriter>::options::overlap::AB_BC)
          .value("MAX_AB_BC",
                 KnuthBendixImpl<Rewriter>::options::overlap::MAX_AB_BC);

      thing.def("__repr__", [](KnuthBendixImpl<Rewriter>& self) {
        return to_human_readable_repr(self);
      });

      //////////////////////////////////////////////////////////////////////////
      // Things from cong-common.hpp . . .
      //////////////////////////////////////////////////////////////////////////

      def_construct_default(thing, "KnuthBendix");
      def_init_default(thing, "KnuthBendix");
      def_construct_kind_presentation(thing, "KnuthBendix");
      def_init_kind_presentation(thing, "KnuthBendix");
      def_copy(thing, "KnuthBendix");
      // The next function has a reference hence the different "KnuthBendix".
      def_number_of_classes(thing, "KnuthBendix");
      def_add_generating_pair(thing, "KnuthBendix");
      def_contains(thing, "KnuthBendix");
      def_currently_contains(thing, "KnuthBendix");
      def_reduce_no_run(thing, "KnuthBendix");
      def_reduce(thing, "KnuthBendix");

      // Not exposed in the doc
      thing.def("internal_presentation",
                &KnuthBendixImpl<Rewriter>::internal_presentation,
                py::return_value_policy::reference_internal);

      //////////////////////////////////////////////////////////////////////////
      // Setters and getters for optional parameters
      //////////////////////////////////////////////////////////////////////////

      thing.def("max_pending_rules",
                py::overload_cast<>(
                    &KnuthBendixImpl<Rewriter>::max_pending_rules, py::const_),
                R"pbdoc(
:sig=(self: KnuthBendix) -> int:

Return the number of pending rules that must accumulate before they are reduced,
processed, and added to the system.

The default value is ``128``. A value of ``1`` means :any:`Runner.run` should
attempt to add each rule as it is created without waiting for rules to
accumulate.

:return: The batch size.
:rtype: int

.. seealso:: :any:`Runner.run`.
)pbdoc");

      thing.def("max_pending_rules",
                py::overload_cast<size_t>(
                    &KnuthBendixImpl<Rewriter>::max_pending_rules),
                py::arg("val"),
                R"pbdoc(
:sig=(self: KnuthBendix, val: int) -> KnuthBendix:

Specify the number of pending rules that must accumulate before they are
reduced, processed, and added to the system.

The default value is ``128``, and should be set to ``1`` if :any:`Runner.run`
should attempt to add each rule as it is created without waiting for rules
to accumulate.

:param val:	The new value of the batch size.
:type val: int

:return: *self*.
:rtype: KnuthBendix

.. seealso:: :any:`Runner.run`.
)pbdoc");

      thing.def("check_confluence_interval",
                py::overload_cast<>(
                    &KnuthBendixImpl<Rewriter>::check_confluence_interval,
                    py::const_),
                R"pbdoc(
:sig=(self: KnuthBendix) -> int:

Return the interval at which confluence is checked.

The function :any:`Runner.run` periodically checks if the system is already
confluent. This function can be used to return how frequently this happens. It
is the number of new overlaps that should be considered before checking
confluence.

:return: The interval at which confluence is checked.
:rtype: int

.. seealso:: :any:`Runner.run`.
)pbdoc");

      thing.def(
          "check_confluence_interval",
          [](KnuthBendixImpl<Rewriter>&     self,
             int_or_constant<size_t> const& val) -> KnuthBendixImpl<Rewriter>& {
            return self.check_confluence_interval(to_int<size_t>(val));
          },
          py::return_value_policy::reference_internal,
          py::arg("val"),
          R"pbdoc(
:sig=(self: KnuthBendix, val: int | LimitMax) -> KnuthBendix:

Set the interval at which confluence is checked.

The function :py:meth:`Runner.run` periodically checks if the system is already
confluent. This function can be used to set how frequently this happens. It is
the number of new overlaps that should be considered before checking confluence.
Setting this value too low can adversely affect the performance of
:any:`Runner.run`.

The default value is ``4096``, and should be set to
:py:obj:`LIMIT_MAX` if :any:`Runner.run` should never check if the
system is already confluent.

:param val: The new value of the interval.
:type val: int | LimitMax

:return: *self*.
:rtype: KnuthBendix
)pbdoc");

      thing.def(
          "max_overlap",
          [](KnuthBendixImpl<Rewriter> const& self) {
            return from_int(self.max_overlap());
          },
          R"pbdoc(
:sig=(self: KnuthBendix) -> int | PositiveInfinity:

Return the maximum length of overlaps to be considered.

This function returns the maximum length of the overlap of two left hand sides
of rules that should be considered in :any:`Runner.run`.

:return: The maximum overlap length
:rtype: int | PositiveInfinity

.. seealso:: :any:`Runner.run`.
)pbdoc");

      thing.def(
          "max_overlap",
          [](KnuthBendixImpl<Rewriter>& self,
             int_or_constant<size_t>    val) -> KnuthBendixImpl<Rewriter>& {
            return self.max_overlap(to_int<size_t>(val));
          },
          py::return_value_policy::reference_internal,
          py::arg("val"),
          R"pbdoc(
:sig=(self: KnuthBendix, val: int | PositiveInfinity) -> KnuthBendix:

Set the maximum length of overlaps to be considered.

This function can be used to specify the maximum length of the overlap of two
left hand sides of rules that should be considered in  :any:`Runner.run`.

If this value is less than the longest left hand side of a rule, then
:any:`Runner.run` can terminate without the system being confluent.

:param val: The new value of the maximum overlap length.
:type val: int | PositiveInfinity

:return: *self*.
:rtype: KnuthBendix

.. seealso:: :any:`Runner.run`.
)pbdoc");

      thing.def(
          "max_rules",
          [](KnuthBendixImpl<Rewriter> const& self) {
            return from_int(self.max_rules());
          },
          R"pbdoc(
:sig=(self: KnuthBendix) -> int | PositiveInfinity:

Return the maximum number of rules.

This member function returns the (approximate) maximum number of rules that the
system should contain. If this is number is exceeded in calls to :any:`Runner.run`
or :any:`knuth_bendix.by_overlap_length`, then they will terminate and the
system may not be confluent.

:return: The maximum number of rules the system should contain.
:rtype: int | PositiveInfinity

..  seealso:: :any:`Runner.run`.
)pbdoc");

      thing.def(
          "max_rules",
          [](KnuthBendixImpl<Rewriter>& self,
             int_or_constant<size_t>    val) -> KnuthBendixImpl<Rewriter>& {
            return self.max_rules(to_int<size_t>(val));
          },
          py::return_value_policy::reference_internal,
          py::arg("val"),
          R"pbdoc(
:sig=(self: KnuthBendix, val: int | PositiveInfinity) -> KnuthBendix:

Set the maximum number of rules.

This member function sets the (approximate) maximum number of rules that the
system should contain. If this is number is exceeded in calls to :any:`Runner.run`
or :any:`knuth_bendix.by_overlap_length`, then they will terminate and the
system may not be confluent.

By default this value is :any:`POSITIVE_INFINITY`.

:param val: The maximum number of rules.
:type val: int | PositiveInfinity

:return: *self*.
:rtype: KnuthBendix

.. seealso:: :any:`Runner.run`.
)pbdoc");

      thing.def("overlap_policy",
                py::overload_cast<>(&KnuthBendixImpl<Rewriter>::overlap_policy,
                                    py::const_),
                R"pbdoc(
:sig=(self: KnuthBendix) -> KnuthBendix.options.overlap:

Return the overlap policy.

This function returns the way that the length of an overlap of two words in the
system is measured.

:return: The overlap policy.
:rtype: KnuthBendix.options.overlap

.. seealso:: :any:`overlap`.
)pbdoc");

      thing.def("overlap_policy",
                py::overload_cast<
                    typename KnuthBendixImpl<Rewriter>::options::overlap>(
                    &KnuthBendixImpl<Rewriter>::overlap_policy),
                py::arg("val"),
                R"pbdoc(
:sig=(self: KnuthBendix, val: KnuthBendix.options.overlap) -> KnuthBendix:

Set the overlap policy.

This function can be used to determine the way that the length of an overlap of
two words in the system is measured.

:param val: The overlap policy.
:type val: KnuthBendix.options.overlap

:return: *self*.
:rtype: KnuthBendix

.. seealso:: :any:`overlap`.
)pbdoc");

      //////////////////////////////////////////////////////////////////////////
      // Member functions for rules and rewriting
      //////////////////////////////////////////////////////////////////////////

      thing.def("number_of_active_rules",
                &KnuthBendixImpl<Rewriter>::number_of_active_rules,
                R"pbdoc(
:sig=(self: KnuthBendix) -> int:

Return the current number of active rules.

:return: The current number of active rules.
:rtype: int
)pbdoc");

      thing.def("number_of_inactive_rules",
                &KnuthBendixImpl<Rewriter>::number_of_inactive_rules,
                R"pbdoc(
:sig=(self: KnuthBendix) -> int:

Return the current number of inactive rules.

:return: The current number of inactive rules.
:rtype: int
)pbdoc");

      thing.def("total_rules",
                &KnuthBendixImpl<Rewriter>::total_rules,
                R"pbdoc(
:sig=(self: KnuthBendix) -> int:

Return the number of rules that have been created

Return the total number of Rule instances that have been created whilst whilst
the Knuth-Bendix algorithm has been running. Note that this is not the sum of
:any:`number_of_active_rules` and :any:`number_of_inactive_rules`, due
to the re-initialisation of rules where possible.

:return: The total number of rules.
:rtype: int
)pbdoc");

      //////////////////////////////////////////////////////////////////////////
      // Main member functions
      //////////////////////////////////////////////////////////////////////////

      thing.def("confluent",
                &KnuthBendixImpl<Rewriter>::confluent,
                R"pbdoc(
:sig=(self: KnuthBendix) -> bool:

Check `confluence <https://w.wiki/9DA>`_ of the current rules.

:return: ``True`` if the :py:class:`KnuthBendix`
  instance is confluent and ``False`` if it is not.
:rtype: bool
)pbdoc");

      thing.def("confluent_known",
                &KnuthBendixImpl<Rewriter>::confluent_known,
                R"pbdoc(
:sig=(self: KnuthBendix) -> bool:

Check if the current system knows the state of confluence of the current rules.

:return:
   ``True`` if the confluence of the rules in the
   :py:class:`KnuthBendix` instance is known, and
   ``False`` if it is not.
:rtype: bool
)pbdoc");

      thing.def(
          "gilman_graph",
          [](KnuthBendixImpl<Rewriter>& kb) -> WordGraph<uint32_t> const& {
            return kb.gilman_graph();
          },
          R"pbdoc(
:sig=(self: KnuthBendix) -> WordGraph:

Return the Gilman :py:class:`WordGraph`.

The Gilman :py:class:`WordGraph` is a digraph where the labels of the paths from
the initial node (corresponding to the empty word) correspond to the short-lex
normal forms of the semigroup elements.

The semigroup is finite if the graph is acyclic, and infinite otherwise.

:return: The Gilman :py:class:`WordGraph`.
:rtype: WordGraph

.. warning::
  This will terminate when the :any:`KnuthBendix` instance is
  reduced and confluent, which might be never.

.. seealso:: :any:`number_of_classes` and :any:`knuth_bendix.normal_forms`.
)pbdoc",
          py::return_value_policy::reference_internal);

      ////////////////////////////////////////////////////////////////////////
      // Helpers
      ////////////////////////////////////////////////////////////////////////

      // Almost all helpers are only implemented for KnuthBendix

      // is_obviously_infinite might as well be implemented here, since none
      // of the input or output depends on the type of words in the object.
      // Also no prefix because not in a subpackage.
      m.def(
          "is_obviously_infinite",
          [](KnuthBendixImpl<Rewriter>& kb) {
            return is_obviously_infinite(kb);
          },
          R"pbdoc(
:sig=(kb: KnuthBendix) -> bool:
:only-document-once:

Function for checking if the quotient of a finitely presented semigroup
or monoid defined by a :py:class:`KnuthBendix` object is
obviously infinite or not.

This function returns ``True`` if the quotient of the finitely
presented semigroup or monoid defined by the
:py:class:`KnuthBendix` object *kb* is obviously
infinite; ``False`` is returned if it is not.

:param kb: the :py:class:`KnuthBendix` instance.
:type kb: KnuthBendix

:returns:
   Whether or not the congruence defined by a
   :py:class:`KnuthBendix` instance obviously has infinitely
   many classes.
:rtype:
   bool

.. note::
    If this function returns ``False``, it is still possible that the
    quotient defined by the :py:class:`KnuthBendix`
    object *kb* is infinite.
)pbdoc");
    }
  }  // namespace detail

  void init_detail_knuth_bendix_impl(py::module& m) {
    detail::bind_detail_knuth_bendix_impl<detail::RewriteFromLeft>(
        m, "KnuthBendixImplRewriteFromLeft");
    detail::bind_detail_knuth_bendix_impl<detail::RewriteTrie>(
        m, "KnuthBendixImplRewriteTrie");
  }
}  // namespace libsemigroups
