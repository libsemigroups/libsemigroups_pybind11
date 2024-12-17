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
#include <cstddef>  // for size_t

// C++ stl headers....
#include <chrono>  // for nanoseconds

// libsemigroups....

#include <libsemigroups/knuth-bendix.hpp>  // for KnuthBendix, KnuthBendix::option...
#include <libsemigroups/obvinf.hpp>        // for is_obviously_infinite
#include <libsemigroups/runner.hpp>        // for Runner
#include <libsemigroups/types.hpp>         // for word_type, letter_type
#include <libsemigroups/word-graph.hpp>  // for WordGraph

// pybind11....
#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>  // for class_, make_iterator, enum_, init
#include <pybind11/pytypes.h>   // for py::str
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_knuth_bendix

namespace py = pybind11;

namespace libsemigroups {

  namespace {
    template <typename Word, typename Rewriter>
    void reduce_no_run(
        py::class_<KnuthBendix<Rewriter>, CongruenceInterface>& thing) {
      thing.def(
          "reduce_no_run",
          [](KnuthBendix<Rewriter>& self, Word const& w) {
            return knuth_bendix::reduce_no_run(self, w);
          },
          py::arg("w"),
          R"pbdoc(
:sig=(self: KnuthBendix, w: List[int] | str) -> List[int] | str:
:only-document-once:

Reduce a word.

Rewrites the word *w* according to the current rules in the :py:class:`KnuthBendixRewriteTrie`
instance.

:param w: the input word.
:type w: List[int] | str

:raises LibsemigroupsError:
  if any of the values in *w* is out of range, i.e. they do not belong to
  ``presentation().alphabet()`` and :any:`PresentationStrings.validate_word`
  raises.)pbdoc");
    }

    template <typename Word, typename Rewriter>
    void reduce(py::class_<KnuthBendix<Rewriter>, CongruenceInterface>& thing) {
      thing.def(
          "reduce",
          [](KnuthBendix<Rewriter>& self, Word const& w) {
            return knuth_bendix::reduce_no_run(self, w);
          },
          py::arg("w"),
          R"pbdoc(
:sig=(self: KnuthBendix, w: List[int] | str) -> List[int] | str:
:only-document-once:

Reduce a word.

This function triggers a full enumeration and then reduces the word *w*
according to the rules in the :py:class:`KnuthBendixRewriteTrie`
instance.

:param w: the input word.
:type w: List[int] | str

:raises LibsemigroupsError:
  if any of the values in *w* is out of range, i.e. they do not belong to
  ``presentation().alphabet()`` and :any:`PresentationStrings.validate_word`
  raises.
)pbdoc");
    }

    template <typename Word, typename Rewriter>
    void
    contains(py::class_<KnuthBendix<Rewriter>, CongruenceInterface>& thing) {
      thing.def(
          "contains",
          [](KnuthBendix<Rewriter>& self, Word const& u, Word const& v) {
            return knuth_bendix::contains(self, u, v);
          },
          py::arg("u"),
          py::arg("v"),
          R"pbdoc(
:sig=(self: KnuthBendix, u: List[int] | str, v: List[int] | str) -> bool:
:only-document-once:

Check containment of a pair of words.

This function checks whether or not the words *u* and *v* are contained in the
congruence represented by a :py:class:`KnuthBendixRewriteTrie` instance.

:param u: the first word.
:type u: List[int] | str

:param v: the second word.
:type v: List[int] | str

:returns: Whether or not the pair belongs to the congruence.
:rtype: bool

:raises LibsemigroupsError:
  if any of the values in *u* or *v* is out of range, i.e. they do not belong
  to ``presentation().alphabet()`` and :any:`PresentationStrings.validate_word`
  raises.
)pbdoc");
    }

    template <typename Word, typename Rewriter>
    void currently_contains(
        py::class_<KnuthBendix<Rewriter>, CongruenceInterface>& thing) {
      thing.def(
          "currently_contains",
          [](KnuthBendix<Rewriter> const& self, Word const& u, Word const& v) {
            return knuth_bendix::currently_contains(self, u, v);
          },
          py::arg("u"),
          py::arg("v"),
          R"pbdoc(
:sig=(self: KnuthBendix, u: List[int] | str, v: List[int] | str) -> bool:
:only-document-once:

Check whether a pair of words is already known to belong to the congruence.

This function checks whether or not the words *u* and *v* are already known to
be contained in the congruence represented by a :py:class:`KnuthBendixRewriteTrie` instance.
This function performs no enumeration, so it is possible for the words to be
contained in the congruence, but that this is not currently known.

:param u: the first word.
:type u: List[int] | str

:param v: the second word.
:type v: List[int] | str

:returns:
    *  :any:`tril.true` if the words are known to belong to the congruence;
    *  :any:`tril.false` if the words are known to not belong to the congruence;
    *  :any:`tril.unknown` otherwise.
:rtype: tril

:raises LibsemigroupsError:
  if any of the values in *u* or *v* is out of range, i.e. they do not belong
  to ``presentation().alphabet()`` and :any:`PresentationStrings.validate_word`
  raises.

:raises LibsemigroupsError:
    if :any:`small_overlap_class` is not at least :math:`4`.
)pbdoc");
    }

#define CONCAT(first, second) first##second
#define CONCAT3(first, second, third) first##second##third

#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

    template <typename Word, typename Rewriter>
    void bind_normal_form_range(py::module& m, char const* name) {
      using NormalFormRange
          = detail::KnuthBendixNormalFormRange<Word, Rewriter, ShortLexCompare>;
      py::class_<NormalFormRange> thing1(m, name);

      thing1.def("__repr__", [](NormalFormRange const& nfr) {
        return to_human_readable_repr(nfr);
      });

      thing1.def("__copy__", [](NormalFormRange const& nfr) {
        return NormalFormRange(nfr);
      });
      // __len__ is not allowed to return anything other than an int, hence
      // __len__ and count don't have the same behaviour.
      thing1.def("__len__", [](NormalFormRange const& nfr) {
        auto result = nfr.count();
        if (result == POSITIVE_INFINITY) {
          return py::module_::import("sys").attr("maxsize").cast<uint64_t>();
        }
        return result;
      });

      thing1.def("__iter__", [](NormalFormRange const& nfr) {
        return py::make_iterator(rx::begin(nfr), rx::end(nfr));
      });

      thing1.def("at_end", [](NormalFormRange& nfr) { return nfr.at_end(); });
      thing1.def("count", [](NormalFormRange& nfr) { return nfr.count(); });

      thing1.def("get", [](NormalFormRange& nfr) { return nfr.get(); });

      thing1.def("max", [](NormalFormRange const& self) { return self.max(); });

      thing1.def("max",
                 [](NormalFormRange& self, PositiveInfinity const& val)
                     -> NormalFormRange& { return self.max(val); });
      thing1.def("max",
                 [](NormalFormRange& self, size_t val) -> NormalFormRange& {
                   return self.max(val);
                 });
      thing1.def("min", [](NormalFormRange const& self) { return self.min(); });
      thing1.def("min",
                 [](NormalFormRange& self, size_t val) -> NormalFormRange& {
                   return self.min(val);
                 });
      thing1.def("next", [](NormalFormRange& nfr) { nfr.next(); });
    }

    template <typename Rewriter>
    void bind_knuth_bendix(py::module& m, std::string const& name) {
      py::class_<KnuthBendix<Rewriter>, CongruenceInterface> kb(m,
                                                                name.c_str(),
                                                                R"pbdoc(
Class containing an implementation of the Knuth-Bendix Algorithm.

On this page we describe the functionality relating to the Knuth-Bendix
algorithm for semigroups and monoids in ``libsemigroups``. This page contains
details of the member functions of the class :any:`KnuthBendixRewriteTrie`.
This class is used to represent a `string rewriting system
<https://w.wiki/9Re>`_ defining a 1- or 2-sided congruence on a finitely
presented monoid or semigroup.

:any:`KnuthBendixRewriteTrie` inherits from :any:`Runner` and
:any:`CongruenceInterface`; and has the nested class
:any:`KnuthBendixRewriteTrie.options`.

 .. doctest::

    >>> from libsemigroups_pybind11 import (KnuthBendix, Presentation,
    ... presentation, congruence_kind)
    >>> p = Presentation("abc")
    >>> presentation.add_rule(p, "aaaa", "a")
    >>> presentation.add_rule(p, "bbbb", "b")
    >>> presentation.add_rule(p, "cccc", "c")
    >>> presentation.add_rule(p, "abab", "aaa")
    >>> presentation.add_rule(p, "bcbc", "bbb")
    >>> kb = KnuthBendix(congruence_kind.twosided, p)
    >>> not kb.confluent()
    True
    >>> kb.run()
    >>> kb.number_of_active_rules()
    31
    >>> kb.confluent()
    True
)pbdoc");

      py::class_<typename KnuthBendix<Rewriter>::options> options(kb,
                                                                  "options",
                                                                  R"pbdoc(
This class containing various options that can be used to control the
behaviour of Knuth-Bendix.)pbdoc");

      py::enum_<typename KnuthBendix<Rewriter>::options::overlap>(options,
                                                                  "overlap",
                                                                  R"pbdoc(
Values for specifying how to measure the length of an overlap.

The values in this enum determine how a :any:`KnuthBendixRewriteTrie`
instance measures the length :math:`d(AB, BC)` of the overlap of
two words :math:`AB` and :math:`BC`.

.. seealso:: :any:`KnuthBendixRewriteTrie.overlap_policy`
)pbdoc")
          .value("ABC",
                 KnuthBendix<Rewriter>::options::overlap::ABC,
                 R"pbdoc(:math:`d(AB, BC) = |A| + |B| + |C|`)pbdoc")
          .value("AB_BC",
                 KnuthBendix<Rewriter>::options::overlap::AB_BC,
                 R"pbdoc(:math:`d(AB, BC) = |AB| + |BC|`)pbdoc")
          .value("MAX_AB_BC",
                 KnuthBendix<Rewriter>::options::overlap::MAX_AB_BC,
                 R"pbdoc(:math:`d(AB, BC) = max(|AB|, |BC|)`)pbdoc");

      kb.def("__repr__", [](KnuthBendix<Rewriter>& kb) {
        return to_human_readable_repr(kb);
      });

      //////////////////////////////////////////////////////////////////////////
      // Initialisers
      //////////////////////////////////////////////////////////////////////////

      kb.def(py::init<>(),
             R"pbdoc(
:sig=() -> None:
:only-document-once:
Constructs a :any:`KnuthBendixRewriteTrie` instance with no
rules, and the short-lex reduction ordering.

This function default constructs an uninitialised :any:`KnuthBendixRewriteTrie`
instance.
)pbdoc")
          .def(py::init<congruence_kind, Presentation<std::string> const&>(),
               py::arg("knd"),
               py::arg("p"),
               R"pbdoc(
:sig=(knd: congruence_kind, p: PresentationStrings) -> None:
:only-document-once:

Construct from :any:`congruence_kind` and :any:`PresentationStrings`.

This function constructs a :any:`KnuthBendixRewriteTrie` instance representing
a congruence of kind *knd* over the semigroup or monoid defined by the
presentation *p*.

:param knd: the kind (onesided or twosided) of the congruence.
:type knd: congruence_kind

:param p: the presentation.
:type p: PresentationStrings

:raises LibsemigroupsError:  if *p* is not valid.)pbdoc")
          .def(py::init<congruence_kind, Presentation<word_type> const&>(),
               R"pbdoc(
:sig=(knd: congruence_kind, p: PresentationStrings) -> None:
:only-document-once:
)pbdoc")
          .def(
              "init",
              [](KnuthBendix<Rewriter>& self) { return self.init(); },
              R"pbdoc(
Remove the presentation and rewriter data. This function clears the rewriter,
presentation, settings and stats from the :any:`KnuthBendixRewriteTrie` object,
putting it back into the state it would be in if it was newly default
constructed.

:returns: ``self``.

:rtype:
   KnuthBendix
)pbdoc")
          .def(
              "init",
              [](KnuthBendix<Rewriter>&           self,
                 congruence_kind                  knd,
                 Presentation<std::string> const& p) {
                return self.init(knd, p);
              },
              py::arg("knd"),
              py::arg("p"),
              R"pbdoc(
:sig=(self: KnuthBendixRewriteTrie, knd: congruence_kind, p: PresentationStrings) -> KnuthBendixRewriteTrie:
:only-document-once:

Re-initialize a KnuthBendix instance.

This function puts a :any:`KnuthBendixRewriteTrie` instance back into the state
that it would have been in if it had just been newly constructed from *knd* and
*p*.

:param knd: the kind (onesided or twosided) of the congruence.
:type knd: congruence_kind

:param p: the presentation.
:type p: PresentationStrings

:returns: ``self``.
:rtype: KnuthBendix

:raises LibsemigroupsError:  if *p* is not valid.
)pbdoc")
          .def(
              "init",
              [](KnuthBendix<Rewriter>&         self,
                 congruence_kind                knd,
                 Presentation<word_type> const& p) {
                return self.init(knd, p);
              },
              py::arg("knd"),
              py::arg("p"),
              R"pbdoc(
:sig=(self: KnuthBendixRewriteTrie, knd: congruence_kind, p: PresentationStrings) -> KnuthBendixRewriteTrie:
:only-document-once:
)pbdoc")
          .def(
              "copy",
              [](KnuthBendix<Rewriter> const& self) {
                return KnuthBendix<Rewriter>(self);
              },
              R"pbdoc(
Copy a :py:class:`KnuthBendixRewriteTrie` object.

:returns: A copy.
:rtype: KnuthBendixRewriteTrie
)pbdoc")
          .def("__copy__", [](KnuthBendix<Rewriter> const& self) {
            return KnuthBendix<Rewriter>(self);
          });

      //////////////////////////////////////////////////////////////////////////
      // Setters and getters for optional parameters
      //////////////////////////////////////////////////////////////////////////

      kb.def("max_pending_rules",
             py::overload_cast<>(&KnuthBendix<Rewriter>::max_pending_rules,
                                 py::const_),
             R"pbdoc(
Return the number of pending rules that must accumulate before they are reduced,
processed, and added to the system.

The default value is ``128``. A value of ``1`` means :any:`Runner.run` should
attempt to add each rule as they are created without waiting for rules to
accumulate.

:return: The batch size.
:rtype: int

.. seealso:: :any:`Runner.run`.
)pbdoc")
          .def("max_pending_rules",
               py::overload_cast<size_t>(
                   &KnuthBendix<Rewriter>::max_pending_rules),
               py::arg("val"),
               R"pbdoc(
Specify the number of pending rules that must accumulate before they are
reduced, processed, and added to the system.

The default value is ``128``, and should be set to ``1`` if :any:`Runner.run`
should attempt to add each rule as they are created without waiting for rules
to accumulate.

:param val:	The new value of the batch size.
:type val: int

:return: ``self``.
:rtype: KnuthBendixRewriteTrie

.. seealso:: :any:`Runner.run`.
)pbdoc")
          .def("check_confluence_interval",
               py::overload_cast<>(
                   &KnuthBendix<Rewriter>::check_confluence_interval,
                   py::const_),
               R"pbdoc(
Return the interval at which confluence is checked.

The function :any:`Runner.run` periodically checks if the system is already
confluent. This function can be used to return how frequently this happens. It
is the number of new overlaps that should be considered before checking
confluence.

:return: The interval at which confluence is checked.
:rtype: int

.. seealso:: :any:`Runner.run`.
)pbdoc")
          .def("check_confluence_interval",
               py::overload_cast<size_t>(&libsemigroups::KnuthBendix<
                                         Rewriter>::check_confluence_interval),
               py::arg("val"),
               R"pbdoc(
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
:type val: int

:return: ``self``.
:rtype: KnuthBendixRewriteTrie
)pbdoc")
          .def("max_overlap",
               py::overload_cast<>(&KnuthBendix<Rewriter>::max_overlap,
                                   py::const_),
               R"pbdoc(
Return the maximum length of overlaps to be considered.

This function returns the maximum length of the overlap of two left hand sides
of rules that should be considered in :any:`Runner.run`.

:return: The maximum overlap length
:rtype: int

.. seealso:: :any:`Runner.run`.
)pbdoc")
          .def("max_overlap",
               py::overload_cast<size_t>(
                   &libsemigroups::KnuthBendix<Rewriter>::max_overlap),
               py::arg("val"),
               R"pbdoc(
Set the maximum length of overlaps to be considered.

This function can be used to specify the maximum length of the overlap of two
left hand sides of rules that should be considered in  :any:`Runner.run`.

If this value is less than the longest left hand side of a rule, then
:any:`Runner.run` can terminate without the system being confluent.

:param val: The new value of the maximum overlap length.
:type val: int

:return: ``self``.
:rtype: KnuthBendixRewriteTrie

.. seealso:: :any:`Runner.run`.
)pbdoc")
          .def("max_rules",
               py::overload_cast<>(&KnuthBendix<Rewriter>::max_rules,
                                   py::const_),
               R"pbdoc(
Return the maximum number of rules.

This member function returns the (approximate) maximum number of rules that the
system should contain. If this is number is exceeded in calls to :any:`Runner.run`
or :any:`knuth_bendix.by_overlap_length`, then they will terminate and the
system may not be confluent.

:return: The maximum number of rules the system should contain.
:rtype: int

..  seealso:: :any:`Runner.run`.
)pbdoc")
          .def("max_rules",
               py::overload_cast<size_t>(
                   &libsemigroups::KnuthBendix<Rewriter>::max_rules),
               py::arg("val"),
               R"pbdoc(
Set the maximum number of rules.

This member function sets the (approximate) maximum number of rules that the
system should contain. If this is number is exceeded in calls to :any:`Runner.run`
or :any:`knuth_bendix.by_overlap_length`, then they will terminate and the
system may not be confluent.

By default this value is :any:`POSITIVE_INFINITY`.

:param val: The maximum number of rules.
:type val: int

:return: ``self``.
:rtype: KnuthBendixRewriteTrie

.. seealso:: :any:`Runner.run`.
)pbdoc")
          .def("overlap_policy",
               py::overload_cast<>(&KnuthBendix<Rewriter>::overlap_policy,
                                   py::const_),
               R"pbdoc(
:sig=(self: KnuthBendixRewriteTrie) -> KnuthBendixRewriteTrie.options.overlap:

Return the overlap policy.

This function returns the way that the length of an overlap of two words in the
system is measured.

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
:sig=(self: KnuthBendixRewriteTrie, val: KnuthBendixRewriteTrie.options.overlap) -> KnuthBendixRewriteTrie:

Set the overlap policy.

This function can be used to determine the way that the length of an overlap of
two words in the system is measured.

:param val: The overlap policy.
:type val: overlap

:return: ``self``.
:rtype: KnuthBendixRewriteTrie

.. seealso:: :py:class:`overlap`
)pbdoc");
      //////////////////////////////////////////////////////////////////////////
      // Member functions for rules and rewriting
      //////////////////////////////////////////////////////////////////////////

      kb.def(
            "presentation",
            [](KnuthBendix<Rewriter>& kb) { return kb.presentation(); },
            R"pbdoc(
Return the presentation defined by the rewriting system

:return: The presentation
:rtype: PresentationStrings
)pbdoc")
          .def("number_of_active_rules",
               &libsemigroups::KnuthBendix<Rewriter>::number_of_active_rules,
               R"pbdoc(
Return the current number of active rules.

:return: The current number of active rules.
:rtype: int
)pbdoc")
          .def("number_of_inactive_rules",
               &libsemigroups::KnuthBendix<Rewriter>::number_of_inactive_rules,
               R"pbdoc(
Return the current number of inactive rules.

:return: The current number of inactive rules.
:rtype: int
)pbdoc")
          .def("total_rules",
               &libsemigroups::KnuthBendix<Rewriter>::total_rules,
               R"pbdoc(
Return the number of rules that have been created

Return the total number of Rule instances that have been created whilst whilst
the Knuth-Bendix algorithm has been running. Note that this is not the sum of
:any:`number_of_active_rules` and :any:`number_of_inactive_rules`, due
to the re-initialisation of rules where possible.

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

This member function returns an iterator yielding of the pairs of strings
which represent the rewriting rules. The first entry in every such pair is
greater than the second according to the reduction ordering of the
:py:class:`KnuthBendixRewriteTrie` instance. The rules are sorted
according to the reduction ordering used by the rewriting system, on the first
entry.

:return: An iterator yielding the currently active rules.
:rtype: Iterator[(str, str)]
)pbdoc");

      reduce_no_run<std::string>(kb);
      reduce_no_run<word_type>(kb);

      reduce<std::string>(kb);
      reduce<word_type>(kb);

      contains<std::string>(kb);
      contains<word_type>(kb);

      currently_contains<std::string>(kb);
      currently_contains<word_type>(kb);

      //////////////////////////////////////////////////////////////////////////
      // Main member
      // functions
      //////////////////////////////////////////////////////////////////////////

      kb.def("confluent",
             &libsemigroups::KnuthBendix<Rewriter>::confluent,
             R"pbdoc(
Check `confluence <https://w.wiki/9DA>`_ of the current rules.

:return: ``True`` if the :py:class:`KnuthBendixRewriteTrie`
  instance is confluent and ``False`` if it is not.
:rtype: bool
)pbdoc")
          .def("confluent_known",
               &libsemigroups::KnuthBendix<Rewriter>::confluent_known,
               R"pbdoc(
Check if the current system knows the state of confluence of the current rules.

:return:
   ``True`` if the confluence of the rules in the
   :py:class:`KnuthBendixRewriteTrie` instance is known, and
   ``False`` if it is not.
:rtype: bool
)pbdoc")
          .def(
              "gilman_graph",
              [](KnuthBendix<Rewriter>& kb) -> WordGraph<uint32_t> const& {
                return kb.gilman_graph();
              },
              py::return_value_policy::copy,
              // REVIEW: Should WordGraph be formatted as code, or as text?
              R"pbdoc(
Return the Gilman :py:class:`WordGraph`.

The Gilman :py:class:`WordGraph` is a digraph where the labels of the paths from
the initial node (corresponding to the empty word) correspond to the short-lex
normal forms of the semigroup elements.

The semigroup is finite if the graph is cyclic, and infinite otherwise.

:return: The Gilman :py:class:`WordGraph`.
:rtype: WordGraph

.. warning:: This will terminate when the
  :any:`KnuthBendixRewriteTrie` instance is reduced and
  confluent, which might be never.

.. seealso:: :any:`number_of_classes` and :any:`knuth_bendix.normal_forms`.
)pbdoc")
          .def("gilman_graph_node_labels",
               &libsemigroups::KnuthBendix<Rewriter>::gilman_graph_node_labels,
               R"pbdoc(
Return the node labels of the Gilman :py:class:`WordGraph`

Return the node labels of the Gilman :py:class:`WordGraph`, corresponding to the
unique prefixes of the left-hand sides of the rules of the rewriting system.

:return: The node labels of the Gilman :py:class:`WordGraph`
:rtype: List[str]

.. seealso:: :any:`gilman_graph`.
)pbdoc");

      //////////////////////////////////////////////////////////////////////////
      // Attributes
      //////////////////////////////////////////////////////////////////////////

      kb.def("_number_of_classes",
             &libsemigroups::KnuthBendix<Rewriter>::number_of_classes,
             R"pbdoc(
:sig=(kb: KnuthBendixRewriteTrie) -> int | PositiveInfinity:

Compute the number of classes in the congruence.

This function computes the number of classes in the congruence represented by a
:py:class:`KnuthBendixRewriteTrie` instance.

:returns:
    The number of congruences classes of a :py:class:`KnuthBendixRewriteTrie` object.
:rtype:
    int | PositiveInfinity
)pbdoc");

      //////////////////////////////////////////////////////////////////////////
      // Helpers
      //////////////////////////////////////////////////////////////////////////

      m.def(
          "knuth_bendix_by_overlap_length",
          [](KnuthBendix<Rewriter>& kb) {
            knuth_bendix::by_overlap_length(kb);
          },
          py::arg("kb"),
          R"pbdoc(
:sig=(kb: KnuthBendixRewriteTrie) -> None:
:only-document-once:

Run the Knuth-Bendix algorithm by considering all overlaps of a given length.

This function runs the Knuth-Bendix algorithm on the rewriting system
represented by a :any:`KnuthBendixRewriteTrie` instance by considering all overlaps of a
given length :math:`n` (according to the :any:`libsemigroups_pybind11.overlap`) before those overlaps
of length :math:`n + 1`.

:param kb: the :any:`KnuthBendixRewriteTrie` instance.
:type kb: KnuthBendixRewriteTrie
)pbdoc");
      m.def(
          "knuth_bendix_is_reduced",
          [](KnuthBendix<Rewriter>& kb) {
            return knuth_bendix::is_reduced(kb);
          },
          py::arg("kb"),
          R"pbdoc(
:sig=(kb: KnuthBendixRewriteTrie) -> bool:
:only-document-once:

Check if the all rules are reduced with respect to each other.

:param kb: the KnuthBendix instance defining the rules that are to be checked for being reduced.
:type kb: KnuthBendixRewriteTrie

:returns:
  ``True`` if for each pair :math:`(A, B)` and :math:`(C, D)` of rules stored
  within the :any:`KnuthBendixRewriteTrie` instance, :math:`C` is neither a subword of
  :math:`A` nor :math:`B`. Returns ``False`` otherwise.
:rtype: bool
)pbdoc");

      // The next 2 functions are documented in the wrapper in
      // libsemigroups_pybind11/knuth_bendix.py, because they have the
      // additional kwarg Word to specify the output type.
      m.def("knuth_bendix_str_non_trivial_classes",
            [](KnuthBendix<Rewriter>& kb1, KnuthBendix<Rewriter>& kb2) {
              return knuth_bendix::non_trivial_classes<std::string>(kb1, kb2);
            });
      m.def("knuth_bendix_word_non_trivial_classes",
            [](KnuthBendix<Rewriter>& kb1, KnuthBendix<Rewriter>& kb2) {
              return knuth_bendix::non_trivial_classes<word_type>(kb1, kb2);
            });

      bind_normal_form_range<std::string, Rewriter>(
          m, (name + "_strings").c_str());
      bind_normal_form_range<word_type, Rewriter>(m, (name + "_words").c_str());

      // The next 2 functions are documented in the wrapper in
      // libsemigroups_pybind11/knuth_bendix.py, because they have the
      // additional kwarg Word to specify the output type.
      m.def("knuth_bendix_str_normal_forms", [](KnuthBendix<Rewriter>& kb) {
        return knuth_bendix::normal_forms<std::string>(kb);
      });
      m.def("knuth_bendix_word_normal_forms", [](KnuthBendix<Rewriter>& kb) {
        return knuth_bendix::normal_forms<word_type>(kb);
      });

      m.def(
          "knuth_bendix_redundant_rule",
          [](Presentation<word_type> const& p, std::chrono::milliseconds t)
              -> std::optional<std::pair<word_type, word_type>> {
            auto it = knuth_bendix::redundant_rule(p, t);
            if (it != p.rules.cend()) {
              return std::make_pair(*it, *(it + 1));
            }
            return {};
          },
          R"pbdoc(
:sig=(p: PresentationStrings, t: datetime.timedelta) -> Tuple[List[int], List[int]] | Tuple[str, str] | None:
:only-document-once:

Return a redundant rule or ``None``.

Starting with the last rule in the presentation, this function attempts to run
the Knuth-Bendix algorithm on the rules of the presentation except for a given
omitted rule. For every such omitted rule, Knuth-Bendix is run for the length
of time indicated by the second parameter *t*, and then it is checked if the
omitted rule can be shown to be redundant. If the omitted rule can be shown to
be redundant in this way, then this rule is returned If no rule can be shown to
be redundant in this way, then ``None`` is returned.

:param p: the presentation.
:type p: PresentationStrings

:param t: time to run Knuth-Bendix for every omitted rule.
:type t: datetime.timedelta

:returns: A redundant rule or ``None``.
:rtype: Tuple[List[int], List[int]] | Tuple[str, str] | None

:warning:
  The progress of the Knuth-Bendix algorithm may differ between different
  calls to this function even if the parameters are identical. As such this
  is non-deterministic, and may produce different results with the same
  input.

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
  ('bab', 'abb')
)pbdoc");
      m.def(
          "knuth_bendix_redundant_rule",
          [](Presentation<std::string> const& p, std::chrono::milliseconds t)
              -> std::optional<std::pair<std::string, std::string>> {
            auto it = knuth_bendix::redundant_rule(p, t);
            if (it != p.rules.cend()) {
              return std::make_pair(*it, *(it + 1));
            }
            return {};
          },
          R"pbdoc(
:sig=(p: PresentationStrings, t: datetime.timedelta) -> Tuple[List[int], List[int]] | Tuple[str, str] | None:
:only-document-once:
)pbdoc");
      m.def(
          "is_obviously_infinite",
          [](KnuthBendix<Rewriter>& kb) { return is_obviously_infinite(kb); },
          R"pbdoc(
:sig=(kb: KnuthBendixRewriteTrie) -> bool:
:only-document-once:

Function for checking if the quotient of a finitely presented semigroup or
monoid defined by a :py:class:`KnuthBendixRewriteTrie` object is obviously
infinite or not.

This function returns ``True`` if the quotient of the finitely presented
semigroup or monoid defined by the :py:class:`KnuthBendixRewriteTrie` object
*kb* is obviously infinite; ``False`` is returned if it is not.

:param kb: the :py:class:`KnuthBendixRewriteTrie` instance.
:type kb: KnuthBendix

:returns:
    Whether or not the congruence defined by a
    :py:class:`KnuthBendixRewriteTrie` instance obviously has infinitely many
    classes.
:rtype:
    bool

.. note::
    If this function returns ``False``, it is still possible that the quotient
    defined by the :py:class:`KnuthBendixRewriteTrie` object *kb* is infinite.
)pbdoc");
    }
  }  // namespace

  void init_knuth_bendix(py::module& m) {
    bind_knuth_bendix<detail::RewriteFromLeft>(m, "KnuthBendixRewriteFromLeft");
    bind_knuth_bendix<detail::RewriteTrie>(m, "KnuthBendixRewriteTrie");
  }
}  // namespace libsemigroups
