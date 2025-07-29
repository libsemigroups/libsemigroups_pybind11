//
// libsemigroups_pybind11
// Copyright (C) 2024 James Mitchell
//
// This program is free software:you can redistribute it and/or modify
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

// TODO(0.5): remove the doc that isn't actually used

// libsemigroups headers
#include <chrono>

#include <libsemigroups/todd-coxeter.hpp>

#include <libsemigroups/detail/cong-common-class.hpp>

// pybind11....
#include <pybind11/chrono.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "cong-common.hpp"  // for contains
#include "main.hpp"         // for init_detail_todd_coxeter_impl

namespace libsemigroups {
  namespace py = pybind11;
  using std::literals::operator""sv;

  void init_detail_todd_coxeter_impl(py::module& m) {
    using ToddCoxeterImpl_ = detail::ToddCoxeterImpl;
    py::class_<ToddCoxeterImpl_, detail::CongruenceCommon> thing(
        m, "ToddCoxeterImpl");

    py::class_<ToddCoxeterImpl_::options> options(thing,
                                                  "options",
                                                  R"pbdoc(
This class containing various options that can be used to control the
behaviour of Todd-Coxeter.)pbdoc");

    py::options enum_settings;
    enum_settings.disable_enum_members_docstring();

    py::enum_<ToddCoxeterImpl_::options::strategy> strategy(options,
                                                            "strategy",
                                                            R"pbdoc(
:sig=(self: ToddCoxeter.options.strategy, value: int):

Values for defining the strategy.

The values in this enum can be used as the argument for the method
:py:meth:`ToddCoxeter.strategy` to specify which strategy should be
used when performing a coset enumeration.

The valid values are :

.. py:attribute:: strategy.hlt
  :value: <strategy.hlt: 0>
  
  This value indicates that the HLT (Hazelgrove-Leech-Trotter) strategy should be used. This is analogous to ACE's R-style.

.. py:attribute:: strategy.felsch
  :value: <strategy.felsch: 1>
  
  This value indicates that the Felsch strategy should be used. This is analogous to ACE's C-style.

.. py:attribute:: strategy.CR
  :value: <strategy.CR: 2>
  
  This strategy is meant to mimic the ACE strategy of the same name. The Felsch is run until at least :any:`f_defs` nodes are defined, then the HLT strategy is run until at least :any:`hlt_defs` divided by :math:`N` nodes have been defined, where :math:`N` is the sum of the lengths of the words in the presentation and generating pairs. These steps are repeated until the enumeration terminates.

.. py:attribute:: strategy.R_over_C
  :value: <strategy.R_over_C: 3>
  
  This strategy is meant to mimic the ACE strategy R/C. The HLT strategy is run until the first lookahead is triggered (when :the number of nodes active is at least :any:`lookahead_next`). A full lookahead is then performed, and then the CR strategy is used.

.. py:attribute:: strategy.Cr
  :value: <strategy.Cr: 4>
  
  This strategy is meant to mimic the ACE strategy Cr. The Felsch strategy is run until at least :any:`f_defs` new nodes have been defined, then the HLT strategy is run until at least :any:`hlt_defs` divided by :math:`N` nodes have been defined, where :math:`N` is the sum of the lengths of the words in the presentation and generating pairs. Then the Felsch strategy is run.

.. py:attribute:: strategy.Rc
  :value: <strategy.Rc: 5>
  
  This strategy is meant to mimic the ACE strategy Rc. The HLT strategy is run until at least :any:`hlt_defs` divided by :math:`N` new nodes have been defined (where :math:`N` is the sum of the lengths of the words in the presentation and generating pairs) the Felsch strategy is then run until at least :any:`f_defs` new nodes are defined, and then the HLT strategy is run.



        )pbdoc");
    strategy.value("hlt", ToddCoxeterImpl_::options::strategy::hlt)
        .value("felsch", ToddCoxeterImpl_::options::strategy::felsch)
        .value("CR", ToddCoxeterImpl_::options::strategy::CR)
        .value("R_over_C", ToddCoxeterImpl_::options::strategy::R_over_C)
        .value("Cr", ToddCoxeterImpl_::options::strategy::Cr)
        .value("Rc", ToddCoxeterImpl_::options::strategy::Rc)
        .export_values();

    py::enum_<ToddCoxeterImpl_::options::lookahead_extent>(options,
                                                           "lookahead_extent",
                                                           R"pbdoc(
:sig=(self: ToddCoxeter.options.lookahead_extent, value: int):

Enum for specifying the extent of any lookahead performed.

The values in this enum can be used as the argument for
:any:`ToddCoxeter.lookahead_extent` to specify the extent of any lookahead that
should be performed.

The valid values are :

.. py:attribute:: lookahead_extent.full
  :value: <lookahead_extent.full: 0>

  Perform a full lookahead from every node in the word graph. Full lookaheads are therefore sometimes slower but may detect more coincidences than a partial lookahead.

.. py:attribute:: lookahead_extent.partial
  :value: <lookahead_extent.partial: 1>
  
  Perform a partial lookahead starting from the current node in the word graph. Partial lookaheads are sometimes faster but may not detect as many coincidences as a full lookahead.
)pbdoc")
        .value("full", ToddCoxeterImpl_::options::lookahead_extent::full)
        .value("partial", ToddCoxeterImpl_::options::lookahead_extent::partial)
        .export_values();

    py::enum_<ToddCoxeterImpl_::options::lookahead_style>(options,
                                                          "lookahead_style",
                                                          R"pbdoc(
:sig=(self: ToddCoxeter.options.lookahead_style, value: int):

Enum class for specifying the style of any lookahead performed.

The values in this enum can be used as the argument for
:any:`ToddCoxeter.lookahead_style` to specify the style of any lookahead that
should be performed.

The valid values are :

.. py:attribute:: lookahead_style.hlt
  :value: <lookahead_style.hlt: 0>

  The lookahead will be done in HLT style by following the paths labelled by every relation from every node in the range specified by :any:`lookahead_extent.full` or :any:`lookahead_extent.partial`.

.. py:attribute:: lookahead_style.felsch
  :value: <lookahead_style.hlt: 0>

  The lookahead will be done in Felsch style where every edge is considered in every path labelled by a relation in which it occurs.

)pbdoc")
        .value("hlt", ToddCoxeterImpl_::options::lookahead_style::hlt)
        .value("felsch", ToddCoxeterImpl_::options::lookahead_style::felsch)
        .export_values();

    py::enum_<ToddCoxeterImpl_::options::def_policy>(options,
                                                     "def_policy",
                                                     R"pbdoc(
:sig=(self: ToddCoxeter.options.def_policy, value: int):

Enum class containing values for specifying how to handle edge
definitions.

The values in this enum can be used as the argument for
:any:`ToddCoxeter.def_policy`.

For our purposes, a *definition* is a recently defined edge in the
word graph that we are attempting to construct in an instance of
:any:`ToddCoxeter`. The values in this enum influence how these
definitions are stored and processed.

For every definition held in the definition stack, a depth first
search through the Felsch tree of the generating pairs is
performed. The aim is to only follow paths from nodes in the word
graph labelled by generating pairs that actually pass through the
edge described by a definition.

The values in this enum represent what to do if the number of
definitions in the stack exceeds the value :any:`ToddCoxeter.def_max`.

The valid values are:

.. py:attribute:: def_policy.no_stack_if_no_space
  :value: <def_policy.no_stack_if_no_space: 0>

  Do not put newly generated definitions in the stack if the stack already has size :any:`def_max`.

.. py:attribute:: def_policy.purge_from_top
  :value: <def_policy.purge_from_top: 1>

  If the definition stack has size :any:`def_max` and a new definition is generated, then definitions with dead source node are are popped from the top of the stack (if any).

.. py:attribute:: def_policy.purge_all
  :value: <def_policy.purge_all: 2>

  If the definition stack has size :any:`def_max` and a new definition is generated, then definitions with dead source node are are popped from the entire of the stack (if any).

.. py:attribute:: def_policy.discard_all_if_no_space
  :value: <def_policy.discard_all_if_no_space: 3>

  If the definition stack has size :any:`def_max` and a new definition is generated, then all definitions in the stack are discarded.

.. py:attribute:: def_policy.unlimited
  :value: <def_policy.unlimited: 4>

  There is no limit to the number of definitions that can be put in the stack.

)pbdoc")
        .value("no_stack_if_no_space",
               ToddCoxeterImpl_::options::def_policy::no_stack_if_no_space)
        .value("purge_from_top",
               ToddCoxeterImpl_::options::def_policy::purge_from_top)
        .value("purge_all", ToddCoxeterImpl_::options::def_policy::purge_all)
        .value("discard_all_if_no_space",
               ToddCoxeterImpl_::options::def_policy::discard_all_if_no_space)
        .value("unlimited", ToddCoxeterImpl_::options::def_policy::unlimited)
        .export_values();

    py::enum_<ToddCoxeterImpl_::word_graph_type::options::def_version>(
        options,
        "def_version",
        R"pbdoc(
:sig=(self: ToddCoxeter.options.def_version, value: int):
Values for specifying how to handle definitions.

The valid values are:

.. py:attribute:: def_version.one
  :value: <def_version.one: 0>
  
  Version 1 definition processing.
  
.. py:attribute:: def_version.two
  :value: <def_version.two: 1>
  
  Version 2 definition processing.
)pbdoc")
        .value("one",
               ToddCoxeterImpl_::word_graph_type::options::def_version::one)
        .value("two",
               ToddCoxeterImpl_::word_graph_type::options::def_version::two)
        .export_values();

    //////////////////////////////////////////////////////////////////////////
    // Things from cong-common.hpp . . .
    //////////////////////////////////////////////////////////////////////////

    def_construct_default(thing, "ToddCoxeterImpl_");
    def_init_default(thing, "ToddCoxeterImpl_");
    def_construct_kind_presentation(thing, "ToddCoxeterImpl_");
    def_init_kind_presentation(thing, "ToddCoxeterImpl_");
    def_number_of_classes(thing, "ToddCoxeter");
    def_copy(thing, "ToddCoxeterImpl_");
    def_add_generating_pair(thing, "ToddCoxeterImpl_");
    def_currently_contains(thing, "ToddCoxeterImpl_");
    def_contains(thing, "ToddCoxeterImpl_");
    def_reduce_no_run(thing, "ToddCoxeterImpl_", doc{.detail = R"pbdoc(
If the  :any:`ToddCoxeter` instance is not :any:`Runner.finished`,
then it might be that equivalent input words produce different output
words. This function triggers no congruence enumeration.)pbdoc"sv});
    def_reduce(thing, "ToddCoxeterImpl_");
    // There's no generating_pairs for ToddCoxeterImpl_ only
    // internal_generating_pairs

    ////////////////////////////////////////////////////////////////////////
    // Constructors + Initializers
    ////////////////////////////////////////////////////////////////////////

    thing.def(py::init<congruence_kind, ToddCoxeterImpl_&>(),
              py::arg("knd"),
              py::arg("tc"),
              R"pbdoc(
:sig=(self: ToddCoxeter, knd: congruence_kind, tc: ToddCoxeter) -> None:
:only-document-once:

Construct from :any:`congruence_kind` and :any:`ToddCoxeter`.

This function constructs a :any:`ToddCoxeter` instance representing a
congruence of kind *knd* over the :any:`ToddCoxeter` instance *tc*. The
:any:`ToddCoxeter` instance constructed in this way represents a quotient of
the word graph represented by *tc*.

:param knd: the kind (onesided, or twosided) of the congruence.
:type knd: congruence_kind

:param tc: the :any:`ToddCoxeter` instance.
:type tc: :any:`ToddCoxeter`

:raises LibsemigroupsError:
  if the arguments *knd* and *tc* are not compatible. If the first item is
  ``tc.kind()`` and the second is the parameter *knd*, then compatible
  arguments are (one-sided, one-sided), (two-sided, one-sided), and (two-sided,
  two-sided).)pbdoc");

    thing.def(py::init<congruence_kind, WordGraph<uint32_t> const&>(),
              py::arg("knd"),
              py::arg("wg"),
              R"pbdoc(
:sig=(self: ToddCoxeter, knd: congruence_kind, wg: WordGraph) -> None:
:only-document-once:

Construct from :any:`congruence_kind` and :any:`WordGraph`.

This function constructs a :any:`ToddCoxeter` instance representing a
congruence of kind *knd* over the :any:`WordGraph` *wg*. The
:any:`ToddCoxeter` instance constructed in this way represents a
quotient of the word graph *wg*. If *wg* happens to be the left
or right Cayley graph of a semigroup or monoid, then the
:any:`ToddCoxeter` instance will represent a quotient of that
semigroup.

:param knd: the kind (onesided or twosided) of the congruence.
:type knd: congruence_kind

:param wg: the word graph.
:type wg: WordGraph
)pbdoc");

    thing.def(
        "init",
        [](ToddCoxeterImpl_&       self,
           congruence_kind         knd,
           ToddCoxeterImpl_ const& tc) -> ToddCoxeterImpl_& {
          return self.init(knd, tc);
        },
        py::arg("knd"),
        py::arg("tc"),
        R"pbdoc(
:sig=(self: ToddCoxeter, knd: congruence_kind, tc: ToddCoxeter) -> ToddCoxeter:
:only-document-once:

Re-initialize a :any:`ToddCoxeter` instance.

This function puts a :any:`ToddCoxeter` instance back into the state
that it would have been in if it had just been newly constructed from
*knd* and *tc*.

:param knd: the kind (onesided, or twosided) of the congruence.
:type knd: congruence_kind

:param tc: the :any:`ToddCoxeter` instance.
:type tc: ToddCoxeter

:returns: *self*.
:rtype: ToddCoxeter

:raises LibsemigroupsError:
  if the arguments *knd* and *tc* are not compatible. If the first item is
  ``tc.kind()`` and the second is the parameter *knd*, then compatible
  arguments are (one-sided, one-sided), (two-sided, one-sided), and (two-sided,
  two-sided).
)pbdoc");

    thing.def(
        "init",
        [](ToddCoxeterImpl_&          self,
           congruence_kind            knd,
           WordGraph<uint32_t> const& wg) -> ToddCoxeterImpl_& {
          return self.init(knd, wg);
        },
        py::arg("knd"),
        py::arg("wg"),
        R"pbdoc(
:sig=(self: ToddCoxeter, knd: congruence_kind, wg: WordGraph) -> ToddCoxeter:
:only-document-once:

Re-initialize a :any:`ToddCoxeter` instance.

This function puts a :any:`ToddCoxeter` instance back into the state
that it would have been in if it had just been newly constructed from
*knd* and *wg*.

:param knd: the kind (onesided or twosided) of the congruence.
:type knd: congruence_kind

:param wg: the word graph.
:type wg: WordGraph

:returns: *self*.
:rtype: ToddCoxeter
    )pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // Settings
    ////////////////////////////////////////////////////////////////////////

    thing.def(
        "def_max",
        [](ToddCoxeterImpl_ const& self) { return self.def_max(); },
        R"pbdoc(
:sig=(self: ToddCoxeter) -> int:

Get the current value of the setting for the maximum number of
definitions.

:returns:
   The current value of the setting.
:rtype:
   int
)pbdoc");

    thing.def(
        "def_max",
        [](ToddCoxeterImpl_& self, size_t val) -> ToddCoxeterImpl_& {
          return self.def_max(val);
        },
        py::arg("val"),
        R"pbdoc(
:sig=(self: ToddCoxeter, val: int) -> ToddCoxeter:
Set the maximum number of definitions in the stack.

This setting specifies the maximum number of definitions that can be in the
stack at any given time. What happens if there are the maximum number of
definitions in the stack and a new definition is generated is governed by
:any:`ToddCoxeter.def_policy`.

The default value of this setting is ``2000``.

:param val: the maximum size of the definition stack.
:type val: int

:returns: *self*.
:rtype: ToddCoxeter
)pbdoc");
    thing.def(
        "def_policy",
        [](ToddCoxeterImpl_ const& self) { return self.def_policy(); },
        R"pbdoc(
:sig=(self: ToddCoxeter) -> ToddCoxeter.options.def_policy:

Get the current value of the definition policy. This function returns
the current value of the definition policy which specifies how to handle
definitions. For details see :any:`options.def_policy`.

:returns:
   The current value of the setting, a value of type
   :any:`options.def_policy`.

:rtype:
   ToddCoxeter.options.def_policy
)pbdoc");

    thing.def(
        "def_policy",
        [](ToddCoxeterImpl_& self, ToddCoxeterImpl_::options::def_policy val)
            -> ToddCoxeterImpl_& { return self.def_policy(val); },
        py::arg("val"),
        R"pbdoc(
:sig=(self: ToddCoxeter, val: ToddCoxeter.options.def_policy) -> ToddCoxeter:

Set the definition policy.

This function can be used to specify how to handle definitions. For details see
:any:`options.def_policy`. The default value of this setting is
``options.def_policy.no_stack_if_no_space``.

:param val: the policy to use.
:type val: ToddCoxeter.options.def_policy

:returns: *self*.
:rtype: ToddCoxeter
)pbdoc");

    thing.def(
        "def_version",
        [](ToddCoxeterImpl_& self) { return self.def_version(); },
        R"pbdoc(
:sig=(self: ToddCoxeter) -> ToddCoxeter.options.def_version:

The current value of the definition policy setting.

:returns:
   The current value of the setting.
:rtype:
   ToddCoxeter.options.def_version
)pbdoc");

    thing.def(
        "def_version",
        [](ToddCoxeterImpl_& self, ToddCoxeterImpl_::options::def_version val)
            -> ToddCoxeterImpl_& { return self.def_version(val); },
        py::arg("val"),
        R"pbdoc(
:sig=(self: ToddCoxeter, val: ToddCoxeter.options.def_version) -> ToddCoxeter:

This function can be used to specify how which version of definition handling
to use. For details see :any:`options.def_version`.

The default value of this setting is ``options.def_version.two``.

:param val: the version to use.
:type val: ToddCoxeter.options.def_version

:returns: *self*.
:rtype: ToddCoxeter
)pbdoc");

    thing.def(
        "f_defs",
        [](ToddCoxeterImpl_ const& self) { return self.f_defs(); },
        R"pbdoc(
:sig=(self: ToddCoxeter) -> int:

Get the number of Felsch style definitions in ACE strategies. This
function returns the approx number of Felsch style definitions in each
phase of the `ACE <https://staff.itee.uq.edu.au/havas/>`_ style
strategies:

- ``options.strategy.CR``;
- ``options.strategy.R_over_C``;
- ``options.strategy.R_over_C``;
- ``options.strategy.Cr``; and
- ``options.strategy.Rc``.

If the strategy is not one of those listed above, then this setting is
ignored.

The default value of this setting is ``10 ** 5``.

:returns:
   The current value of the setting.
:rtype:
   int
)pbdoc");

    thing.def(
        "f_defs",
        [](ToddCoxeterImpl_& self, size_t val) -> ToddCoxeterImpl_& {
          return self.f_defs(val);
        },
        py::arg("val"),
        R"pbdoc(
:sig=(self: ToddCoxeter, val: int) -> ToddCoxeter:

Set the number of Felsch style definitions in ACE strategies.

This function can be used to set the approx number of Felsch style definitions
in each phase of the `ACE <https://staff.itee.uq.edu.au/havas/>`_
style strategies:

* ``options.strategy.CR``;
* ``options.strategy.R_over_C``;
* ``options.strategy.R_over_C``;
* ``options.strategy.Cr``; and
* ``options.strategy.Rc``.

If the strategy is not one of those listed above, then this setting is
ignored.

The default value of this setting is ``10 ** 5``.

:param val: the value to use.
:type val: int

:returns: *self*.
:rtype: ToddCoxeter

:raises LibsemigroupsError: if *val* is ``0``.
)pbdoc");

    thing.def(
        "hlt_defs",
        [](ToddCoxeterImpl_ const& self) { return self.hlt_defs(); },
        R"pbdoc(
:sig=(self: ToddCoxeter) -> int:

Get the number of HLT style definitions in ACE strategies. This function
returns the approx number of HLT style definitions in each phase of
the `ACE <https://staff.itee.uq.edu.au/havas/>`_ style strategies:

-  ``options.strategy.CR``;
-  ``options.strategy.R_over_C``;
-  ``options.strategy.R_over_C``;
-  ``options.strategy.Cr``; and
-  ``options.strategy.Rc``.

If the strategy is not one of those listed above, then this setting is
ignored.

The default value of this setting is ``10 ** 5``.

:returns:
   The current value of the setting.
:rtype:
   int
)pbdoc");

    thing.def(
        "hlt_defs",
        [](ToddCoxeterImpl_& self, size_t val) -> ToddCoxeterImpl_& {
          return self.hlt_defs(val);
        },
        py::arg("val"),
        R"pbdoc(
:sig=(self: ToddCoxeter, val: int) -> ToddCoxeter:

Set the number of HLT style definitions in ACE strategies.

This function can be used to set the approx number of HLT style definitions in
each phase of the `ACE <https://staff.itee.uq.edu.au/havas/>`_
style strategies:

*  ``options.strategy.CR``;
*  ``options.strategy.R_over_C``;
*  ``options.strategy.R_over_C``;
*  ``options.strategy.Cr``; and
*  ``options.strategy.Rc``.

If the strategy is not one of those listed above, then this setting is ignored.

The default value of this setting is ``2 * 10 ** 5``.

:param val: the value to use.
:type val: int

:returns: *self*.
:rtype: ToddCoxeter

:raises LibsemigroupsError:  if *val* is ``0``.

)pbdoc");

    thing.def(
        "large_collapse",
        [](ToddCoxeterImpl_ const& self) { return self.large_collapse(); },
        R"pbdoc(
:sig=(self: ToddCoxeter) -> int:

Get the current size of a large collapse. This function can be used to
get what is currently considered a "large" collapse. See
:any:`large_collapse` for the meaning of this setting.

The default value of this setting is ``10 ** 5``.

:returns:
   The current value of the setting.
:rtype:
   int
)pbdoc");
    thing.def(
        "large_collapse",
        [](ToddCoxeterImpl_& self, size_t val) -> ToddCoxeterImpl_& {
          return self.large_collapse(val);
        },
        py::arg("val"),
        R"pbdoc(
:sig=(self: ToddCoxeter, val: int) -> ToddCoxeter:

Set the size of a large collapse.

This function can be used to set what should be considered a "large"
collapse.By default when processing coincidences nodes are merged in the word
graph one pair at a time, and the in-neighbours of the surviving node are
updated at the same time. If the number of coincidences is large, then it might
be that a pair of nodes are merged at one step, then the surviving node is
merged with another node at a future step, and this may happen many many times.
This results in the in-neighbours of the surviving nodes being repeatedly
traversed, which can result in a significant performance penalty. It can be
beneficial to stop updating the in-neighbours as nodes are merged, and to just
rebuild the entire in-neighbours data structure by traversing the entire word
graph after all coincidences have been processed. This is beneficial if the
number of surviving nodes is relatively small in comparison to the number of
nodes merged. The purpose of this setting is to specify what should be
considered a "large" collapse, or more precisely, what number of coincidences
in the stack will trigger a change from updating the in-neighbours one-by-one
to traversing the entire graph once after all coincidences have been
processed.

The default value of this setting is ``100000``.

:param val: the value to use.
:type val: int

:returns: *self*.
:rtype: ToddCoxeter
)pbdoc");
    thing.def(
        "lookahead_extent",
        [](ToddCoxeterImpl_ const& self) { return self.lookahead_extent(); },
        R"pbdoc(
:sig=(self: ToddCoxeter) -> ToddCoxeter.options.lookahead_extent:

Get the current value of the lookahead extent. This function returns the
current value of the lookahead extent setting. The default value of this
setting is ``options.lookahead_extent.partial``.

:returns:
   The current lookahead extent.
:rtype:
   ToddCoxeter.options.lookahead_extent
)pbdoc");
    thing.def(
        "lookahead_extent",
        [](ToddCoxeterImpl_&                           self,
           ToddCoxeterImpl_::options::lookahead_extent val)
            -> ToddCoxeterImpl_& { return self.lookahead_extent(val); },
        py::arg("val"),
        R"pbdoc(
:sig=(self: ToddCoxeter, val: ToddCoxeter.options.lookahead_extent) -> ToddCoxeter:

Set the lookahead extent.

This function can be used to specify the extent of any lookaheads that might
take place in a congruence enumeration. The possible values are
``options.lookahead_extent.partial`` or ``options.lookahead_extent.full``. The
default value of this setting is ``options.lookahead_extent.partial``.

:param val: the extent.
:type val: ToddCoxeter.options.lookahead_extent

:returns: *self*.
:rtype: ToddCoxeter
)pbdoc");
    thing.def(
        "lookahead_growth_factor",
        [](ToddCoxeterImpl_ const& self) {
          return self.lookahead_growth_factor();
        },
        R"pbdoc(
:sig=(self: ToddCoxeter) -> float:

Get the current value of the lookahead growth factor. This function
returns the current value of the lookahead growth factor. See
:any:`lookahead_growth_factor` for a full explanation of this
setting.

:returns:
   The lookahead growth factor.
:rtype:
   float
)pbdoc");
    thing.def(
        "lookahead_growth_factor",
        [](ToddCoxeterImpl_& self, float val) -> ToddCoxeterImpl_& {
          return self.lookahead_growth_factor(val);
        },
        py::arg("val"),
        R"pbdoc(
:sig=(self: ToddCoxeter, val: float) -> ToddCoxeter:

Set the lookahead growth factor.

This setting determines by what factor the number of nodes required to trigger
a lookahead grows. More specifically, at the end of any lookahead if the number
of active nodes already exceeds the value of :any:`lookahead_next` or the
number of nodes killed during the lookahead is less than the number of active
nodes divided by :any:`lookahead_growth_threshold`, then the value of
:any:`lookahead_next` is increased by a multiple of *val*. The default value
is of this setting is ``2.0``.

:param val: the value indicating the lookahead growth factor.
:type val: float

:returns: *self*.
:rtype: ToddCoxeter

:raises LibsemigroupsError:  if *val* is less than ``1.0``.
)pbdoc");
    thing.def(
        "lookahead_growth_threshold",
        [](ToddCoxeterImpl_ const& self) {
          return self.lookahead_growth_threshold();
        },
        R"pbdoc(
:sig=(self: ToddCoxeter) -> int:

Get the current value of the lookahead growth threshold. This function returns
the current value of the lookahead growth threshold. See
:any:`lookahead_growth_threshold` for a full description of this setting.

:returns:
   The current value of the lookahead growth threshold.
:rtype:
   int
)pbdoc");
    thing.def(
        "lookahead_growth_threshold",
        [](ToddCoxeterImpl_& self, size_t val) -> ToddCoxeterImpl_& {
          return self.lookahead_growth_threshold(val);
        },
        py::arg("val"),
        R"pbdoc(
:sig=(self: ToddCoxeter, val: int) -> ToddCoxeter:

Set the lookahead growth threshold.

This setting determines the threshold for the number of nodes required to
trigger a lookahead. More specifically, at the end of any lookahead if the
number of active nodes already exceeds the value of :any:`lookahead_next` or
the number of nodes killed during the lookahead is less than the number of
active nodes divided by :any:`lookahead_growth_threshold`, then the value of
:any:`lookahead_next` is increased. The default value is ``4``.

:param val: the value indicating the lookahead growth threshold.
:type val: int

:returns: *self*.
:rtype: ToddCoxeter
)pbdoc");
    thing.def(
        "lookahead_min",
        [](ToddCoxeterImpl_ const& self) { return self.lookahead_min(); },
        R"pbdoc(
:sig=(self: ToddCoxeter) -> int:

Get the current value of the minimum lookahead setting. This function
returns the current value of the minimum lookahead. See
:any:`lookahead_min` for a full description of this setting.  The
default value is ``10 ** 4``.

:returns:
   The current value of the minimum lookahead.
:rtype:
   int
)pbdoc");
    thing.def(
        "lookahead_min",
        [](ToddCoxeterImpl_& self, size_t val) -> ToddCoxeterImpl_& {
          return self.lookahead_min(val);
        },
        py::arg("val"),
        R"pbdoc(
:sig=(self: ToddCoxeter, val: int) -> ToddCoxeter:

Set the minimum value of lookahead_next.

After a lookahead is performed the value of :any:`lookahead_next` is modified
depending on the outcome of the current lookahead. If the return value of
:any:`lookahead_next` is too small or too large, then the value is adjusted
according to :any:`lookahead_growth_factor` and
:any:`lookahead_growth_threshold`. This setting specified the minimum possible
value for :any:`lookahead_next()`. The default value is ``10 ** 4``.

:param val: value indicating the minimum value of lookahead_next.
:type val: int

:returns: *self*.
:rtype: ToddCoxeter
)pbdoc");
    thing.def(
        "lookahead_next",
        [](ToddCoxeterImpl_ const& self) { return self.lookahead_next(); },
        R"pbdoc(
:sig=(self: ToddCoxeter) -> int:

Get the current value of the lookahead next setting. This function returns the
current value of the lookahead next setting. See the other overload of this
function for a full description of this setting.

:returns:
   The number of active nodes that will trigger the next lookahead.

:rtype:
   int
)pbdoc");
    thing.def(
        "lookahead_next",
        [](ToddCoxeterImpl_& self, size_t val) -> ToddCoxeterImpl_& {
          return self.lookahead_next(val);
        },
        py::arg("val"),
        R"pbdoc(
:sig=(self: ToddCoxeter, val: int) -> ToddCoxeter:

Set the threshold that will trigger a lookahead.

If the number of active nodes exceeds the value set by this function, then a
lookahead of style :any:`ToddCoxeter.lookahead_style` and extent
:any:`ToddCoxeter.lookahead_extent` will be triggered. The default value is
``5 * 10 ** 6``.

:param val: value indicating the initial threshold.
:type val: int

:returns: *self*.
:rtype: ToddCoxeter
)pbdoc");
    thing.def(
        "lookahead_stop_early_interval",
        [](ToddCoxeterImpl_ const& self) {
          return self.lookahead_stop_early_interval();
        },
        R"pbdoc(
:sig=(self: ToddCoxeter) -> datetime.timedelta:

Get the current value of the lookahead stop early interval. This
function returns the current value of the lookahead stop early interval.
See :any:`lookahead_stop_early_interval` for a
full description of this setting.

:returns:
   The length of the interval.
:rtype:
   datetime.timedelta
)pbdoc");
    thing.def(
        "lookahead_stop_early_interval",
        [](ToddCoxeterImpl_&        self,
           std::chrono::nanoseconds val) -> ToddCoxeterImpl_& {
          return self.lookahead_stop_early_interval(val);
        },
        py::arg("val"),
        R"pbdoc(
:sig=(self: ToddCoxeter, val: datetime.timedelta) -> ToddCoxeter:

Set the lookahead stop early interval.

During any lookaheads that are performed, it is periodically checked what
proportion of the active nodes have been killed since the previous such check.
This function can be used to set the interval between these checks. The purpose
of this setting is to allow lookaheads to be stopped early if the number of
nodes being killed is too small (for example, if :math:`<1%` of nodes were
killed in the previous second, then we might want to stop the lookahead early,
since lookaheads take some time but may not result in many nodes being
killed).

The default value is 1 second.

:param val: the new value for the interval.
:type val: datetime.timedelta

:returns: *self*.
:rtype: ToddCoxeter
)pbdoc");
    thing.def(
        "lookahead_stop_early_ratio",
        [](ToddCoxeterImpl_ const& self) {
          return self.lookahead_stop_early_ratio();
        },
        R"pbdoc(
:sig=(self: ToddCoxeter) -> float:

Get the current value of the lookahead stop early ratio. This function
returns the current value of the lookahead stop early ratio. See
:any:`lookahead_stop_early_ratio` for a full description of this
setting.

:returns:
   The ratio.
:rtype:
   float
)pbdoc");
    thing.def(
        "lookahead_stop_early_ratio",
        [](ToddCoxeterImpl_& self, float val) -> ToddCoxeterImpl_& {
          return self.lookahead_stop_early_ratio(val);
        },
        py::arg("val"),
        R"pbdoc(
:sig=(self: ToddCoxeter, val: float) -> ToddCoxeter:

Set the lookahead stop early ratio.

During any lookaheads that are performed, it is periodically checked what
proportion of the active nodes have been killed since the previous such check.
This function can be used to set the minimum proportion of the active nodes
that must be killed every :any:`lookahead_stop_early_interval` to avoid the
lookahead being stopped early. The purpose of this setting is to allow
lookaheads to be stopped early if the number of nodes being killed is too small
(for example, if no nodes were killed in the previous second, then we might
want to stop the lookahead early, since lookaheads take some time but may not
result in many nodes being killed).

:param val: the proportion of active nodes.
:type val: float

:returns: *self*.
:rtype: ToddCoxeter

:raises LibsemigroupsError:
  if *val* is not in the interval :math:`[0, 1)`.
)pbdoc");
    thing.def(
        "lookahead_style",
        [](ToddCoxeterImpl_ const& self) { return self.lookahead_style(); },
        R"pbdoc(
:sig=(self: ToddCoxeter) -> ToddCoxeter.options.lookahead_style:

Get the current value of the lookahead style. This function returns the current
value of the lookahead style. See :any:`ToddCoxeter.lookahead_style`
for a full description of this setting.

:returns:
   The current lookahead style.
:rtype:
   ToddCoxeter.options.lookahead_style
)pbdoc");
    thing.def(
        "lookahead_style",
        [](ToddCoxeterImpl_&                          self,
           ToddCoxeterImpl_::options::lookahead_style val)
            -> ToddCoxeterImpl_& { return self.lookahead_style(val); },
        py::arg("val"),
        R"pbdoc(
:sig=(self: ToddCoxeter, val: ToddCoxeter.options.lookahead_style) -> ToddCoxeter:

Set the style of lookahead.

This function can be used to set the style of any lookaheads that are performed
during the congruence enumeration. The possible values are
``options.lookahead_style.hlt`` and ``options.lookahead_style.felsch``.

The default value of this setting is ``options.lookahead_style.hlt``.

:param val: the style of lookahead to use.
:type val: ToddCoxeter.options.lookahead_style

:returns: *self*.
:rtype: ToddCoxeter
)pbdoc");
    thing.def(
        "lower_bound",
        [](ToddCoxeterImpl_ const& self) { return self.lower_bound(); },
        R"pbdoc(
:sig=(self: ToddCoxeter) -> int:

Get the current value of the lower bound. This function returns the
current value of the lower bound. See :any:`lower_bound` for a
full description of this setting.

:returns:
   The current lower bound.
:rtype:
   int
)pbdoc");
    thing.def(
        "lower_bound",
        [](ToddCoxeterImpl_& self, size_t val) -> ToddCoxeterImpl_& {
          return self.lower_bound(val);
        },
        py::arg("val"),
        R"pbdoc(
:sig=(self: ToddCoxeter, val: int) -> ToddCoxeter:

Specify the minimum number of classes that may permit any enumeration early
stop.

This function can be used to set a lower bound for the number of classes of the
congruence represented by a :any:`ToddCoxeter` instance. If the number of
active nodes becomes at least the value of the argument, and the word graph is
complete (:any:`word_graph.is_complete` returns ``True``), then the
enumeration is terminated. When the given bound is equal to the number of
classes, this may prevent following the paths labelled by relations at many
nodes when there is no possibility of finding coincidences.The default value is
:any:`UNDEFINED`.

:param val: value indicating the lower bound.
:type val: int

:returns: *self*.
:rtype: ToddCoxeter
)pbdoc");
    thing.def(
        "save",
        [](ToddCoxeterImpl_ const& self) { return self.save(); },
        R"pbdoc(
:sig=(self: ToddCoxeter) -> bool:

Get the current value of the save setting. This function returns the
current value of the save setting. See :any:`save` for a full
description of this setting.

:returns:
   The current value.
:rtype:
   bool
)pbdoc");
    thing.def(
        "save",
        [](ToddCoxeterImpl_& self, bool val) -> ToddCoxeterImpl_& {
          return self.save(val);
        },
        py::arg("val"),
        R"pbdoc(
:sig=(self: ToddCoxeter, val: bool) -> ToddCoxeter:

Set whether or not to process definitions during HLT.

If the argument of this function is ``True`` and the HLT strategy is being
used, then definitions are processed during any enumeration. The default value
is ``False``.

:param val: value indicating whether or not to process deductions.
:type val: bool

:returns: *self*.
:rtype: ToddCoxeter
)pbdoc");
    thing.def(
        "strategy",
        [](ToddCoxeterImpl_ const& self) { return self.strategy(); },
        R"pbdoc(
:sig=(self: ToddCoxeter) -> ToddCoxeter.options.strategy:

Get the current value of the strategy setting. This function returns the
current value of the strategy setting. See :any:`options.strategy` for a full
description of this setting.

:returns:
   The current value.
:rtype:
   ToddCoxeter.options.strategy
)pbdoc");
    thing.def(
        "strategy",
        [](ToddCoxeterImpl_& self, ToddCoxeterImpl_::options::strategy val)
            -> ToddCoxeterImpl_& { return self.strategy(val); },
        py::arg("val"),
        R"pbdoc(
:sig=(self: ToddCoxeter, val: ToddCoxeter.options.strategy) -> ToddCoxeter:

Specify the congruence enumeration strategy.

The strategy used during the enumeration can be specified using this function.
The default value is :any:`options.strategy`.

:param val: value indicating which strategy to use.
:type val: ToddCoxeter.options.strategy

:returns: *self*.
:rtype: ToddCoxeter
)pbdoc");
    thing.def(
        "use_relations_in_extra",
        [](ToddCoxeterImpl_ const& self) {
          return self.use_relations_in_extra();
        },
        R"pbdoc(
:sig=(self: ToddCoxeter) -> bool:

Get the current value of the "use relations in extra" setting. This function
returns the current value of the "use relations in extra" setting. See the
other overload of :any:`use_relations_in_extra` for a fuller description of
this setting.

:returns:
   The current value.
:rtype:
   bool
)pbdoc");
    thing.def(
        "use_relations_in_extra",
        [](ToddCoxeterImpl_& self, bool val) -> ToddCoxeterImpl_& {
          return self.use_relations_in_extra(val);
        },
        py::arg("val"),
        R"pbdoc(
:sig=(self: ToddCoxeter, val: bool) -> ToddCoxeter:

Set whether or not to perform an HLT-style push of the defining relations at the identity.

If a :any:`ToddCoxeter` instance is defined over a finitely presented semigroup
or monoid and the Felsch strategy is being used, it can be useful to follow all
the paths from the identity labelled by the underlying relations. This setting
specifies whether or not to do this.The default value of this setting is
``False``.

:param val: the boolean value.
:type val: bool

:returns: *self*.
:rtype: ToddCoxeter
)pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // Accessors
    ////////////////////////////////////////////////////////////////////////

    thing.def("current_spanning_tree",
              &ToddCoxeterImpl_::current_spanning_tree,
              R"pbdoc(
:sig=(self: ToddCoxeter) -> Forest:

Get the current possible spanning tree of the underlying word graph.

This function returns the current value of a possible
spanning tree (a :any:`Forest`) for the underlying :any:`WordGraph` (returned
by :any:`current_word_graph`). This spanning tree is only populated during
calls to :any:`standardize` and as such might contain nothing, or a spanning
tree of a previous value of :any:`current_word_graph`. Some care should be
used with the return value of this function, and it might be better to use the
function :any:`spanning_tree`, which has none of these limitation. If
:any:`Runner.finished` returns ``True``, and :any:`standardize` has been called prior
to a call to this function, then the returned :any:`Forest` will represent a
valid spanning tree for the :any:`WordGraph` returned by
:any:`current_word_graph` or :any:`word_graph`.

:returns:
   A possible spanning tree of the underlying :any:`WordGraph`.
:rtype:
   Forest
)pbdoc",
              // The next line seems to be essential, o/w the returned object is
              // copied. Weirdly doesn't need to be added to the "spanning_tree"
              // function...
              py::return_value_policy::reference_internal);

    thing.def(
        "current_word_graph",
        [](ToddCoxeterImpl_ const& self) -> WordGraph<uint32_t> const& {
          return self.current_word_graph();
        },
        R"pbdoc(
:sig=(self: ToddCoxeter) -> WordGraph:

Get the current word graph.

In some sense, the purpose of the Todd-Coxeter algorithm is to produce a
:any:`WordGraph` of the action of a set of generators on the classes of a
congruence. This function can be used to obtain that
:any:`WordGraph` as it currently exists within a :any:`ToddCoxeter`
instance. This function does not trigger any enumeration.The :any:`WordGraph`
returned by this function may be in a rather complicated state. No guarantees
are given: about the values of the active nodes (i.e. they may be any
non-negative integers in any order); that the number of nodes (including those
that are inactive) should coincide with the number of active nodes; that the
graph is complete; or that the graph is compatible with the relations of the
:any:`presentation` or with the generating_pairs. The functions
:any:`standardize` and :any:`shrink_to_fit` can be used to modify the returned
word graph in-place to have (possibly) more reasonable characteristics.

:returns:
  The underlying :any:`WordGraph`.
:rtype:
  WordGraph
)pbdoc",
        // The next line seems to be essential, o/w the returned object is
        // copied. Weirdly doesn't need to be added to the "word_graph"
        // function...
        py::return_value_policy::reference_internal);

    thing.def(
        "is_standardized",
        [](ToddCoxeterImpl_ const& self) { return self.is_standardized(); },
        R"pbdoc(
:sig=(self: ToddCoxeter) -> bool:

Check if the word graph is currently standardized with respect to any
order. This function returns ``True`` if the :any:`current_word_graph`
has been standardized with respect to the any :any:`Order` other than
:any:`Order.none`.

:returns:
   Whether or not the current word graph is standardized with respect to
   any order.
:rtype:
   bool
)pbdoc");
    thing.def(
        "is_standardized",
        [](ToddCoxeterImpl_& self, Order val) {
          return self.is_standardized(val);
        },
        py::arg("val"),
        R"pbdoc(
:sig=(self: ToddCoxeter, val: Order) -> bool:

Check if the word graph is currently standardized with respect to a given order.

This function returns ``True`` if the :any:`current_word_graph` has been
standardized with respect to the order *val* ; and ``False`` if not.

:param val: the Order to check for.
:type val: Order

:returns:
  Whether or not the current word graph is standardized with respect to
  a given order.
:rtype: bool
)pbdoc");

    // internal_generating_pairs are in detail::CongruenceCommon

    // Not exposed in the doc
    thing.def("internal_presentation",
              &ToddCoxeterImpl_::internal_presentation,
              py::return_value_policy::reference_internal);

    thing.def("spanning_tree",
              &ToddCoxeterImpl_::spanning_tree,
              py::return_value_policy::reference_internal,
              R"pbdoc(
:sig=(self: ToddCoxeter) -> Forest:

Get the spanning tree of the underlying word graph. This function
returns a spanning tree (a :any:`Forest`) for the
underlying :any:`WordGraph` (returned by :any:`word_graph`) with the
nodes appearing in short-lex order. This function triggers a full
congruence enumeration.

:returns:
   A spanning tree of the underlying :any:`WordGraph`.
:rtype:
   Forest
)pbdoc");
    thing.def("standardization_order",
              &ToddCoxeterImpl_::standardization_order,
              R"pbdoc(
:sig=(self: ToddCoxeter) -> Order:

Get the current standardization order of the underlying word graph.

This function returns the standardization order currently used in the
underlying word graph. The return value of this function will be the argument
of the most recent call to :any:`standardize`; or
:any:`Order.none`.

The return value of this function indicates the following:

- :any:`Order.none` implies that no standardization has been performed
  and:

   - the return value of :any:`reduce` will essentially arbitrary;
   - the return values of :any:`todd_coxeter.normal_forms` or
     :any:`todd_coxeter.normal_forms` will be essentially arbitrary;
   - the classes of the congruence will be indexed in an arbitrary order;

- :any:`Order.shortlex` implies that:

   - the return value of :any:`reduce` will be the short-lex least word
     belonging to a given congruence class;
   - the return values of :any:`todd_coxeter.normal_forms` and
     :any:`todd_coxeter.normal_forms` will be in
     short-lex order;
   - the classes of the congruence will be indexed in short-lex order on
     the short-lex least word;

- :any:`Order.lex` implies that:

   - the return values of :any:`todd_coxeter.normal_forms` and
     :any:`todd_coxeter.normal_forms` will be ordered lexicographically.
   - the return values of :any:`reduce` and the indexes of class are
     essentially arbitrary because there is not necessarily a
     lexicographically least word in every class;

- :any:`Order.recursive` implies that:

   - the return value of :any:`reduce` will be the recursive path least
     word belonging to a given congruence class;
   - the return values of :any:`todd_coxeter.normal_forms` and
     :any:`todd_coxeter.normal_forms` will be ordered by the
     recursive path order;
   - the classes of the congruence will be indexed in recursive path order
     on the recursive path least word.

:returns:
   The current standardization order.
:rtype:
   Order
)pbdoc");
    thing.def(
        "word_graph",
        [](ToddCoxeterImpl_& self) -> WordGraph<uint32_t> const& {
          return self.word_graph();
        },
        py::return_value_policy::reference_internal,
        R"pbdoc(
:sig=(self: ToddCoxeter) -> WordGraph:

Get the word graph after performing a full congruence enumeration.

In some sense, the purpose of the Todd-Coxeter algorithm is to produce a
:any:`WordGraph` of the action of a set of generators on the classes of a
congruence. This function can be used to obtain that
:any:`WordGraph` instance. This function triggers a full enumeration.The
:any:`WordGraph` returned by this function may be in a rather complicated
state. The active nodes (and nodes) will be :math:`\{0, \ldots, n - 1\}` where
:math:`n` is the number of classes in the congruence if :any:`presentation`
contains the empty word; or the number of classes plus one if
:any:`presentation` does not contain the empty word. The returned
:any:`WordGraph` is also short-lex standardized. The returned :any:`WordGraph`
will usually be complete and compatible with the relations of the
:any:`presentation` and with the :any:`ToddCoxeter.generating_pairs`. The
:any:`WordGraph` may not be complete or compatible for some values of the
settings. For example, if the setting :any:`lower_bound` is used but is not the
same as the number of classes in the congruence, then the :any:`WordGraph`
returned by this function may not be compatible with the relations of
:any:`presentation` or :any:`ToddCoxeter.generating_pairs`.

:returns:
   The underlying :any:`WordGraph`.
:rtype:
  WordGraph
)pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // Modifiers
    ////////////////////////////////////////////////////////////////////////

    thing.def("perform_lookahead",
              &ToddCoxeterImpl_::perform_lookahead,
              py::arg("stop_early"),
              R"pbdoc(
:sig=(self: ToddCoxeter, stop_early: bool) -> None:

Perform a lookahead.

This function can be used to explicitly perform a lookahead. The style and
extent of this lookahead are controlled by the settings
:any:`ToddCoxeter.lookahead_style` and
:any:`ToddCoxeter.lookahead_extent`. If the argument *stop_early* is
``True``, then the settings :any:`lookahead_stop_early_interval` and
:any:`lookahead_stop_early_ratio` are used to determine whether or not the
lookahead should be aborted early. If *stop_early* is ``False``, then these
settings are ignored.

:param stop_early:
    whether or not to consider stopping the lookahead early if
    too few nodes are killed.
:type stop_early: bool
)pbdoc");
    thing.def("shrink_to_fit",
              &ToddCoxeterImpl_::shrink_to_fit,
              R"pbdoc(
:sig=(self: ToddCoxeter) -> None:

Shrink the underlying word graph to remove all dead nodes. This function
triggers a full enumeration, and standardization, and removes from
:any:`word_graph` any dead nodes. If :any:`Runner.finished` returns ``False``,
then this function does nothing.
)pbdoc");
    thing.def("standardize",
              &ToddCoxeterImpl_::standardize,
              py::arg("val"),
              R"pbdoc(
:sig=(self: ToddCoxeter, val: Order) -> bool:

Standardize :any:`ToddCoxeter.current_word_graph`.

This function standardizes the return value of :any:`current_word_graph`, and
does not trigger any enumeration. See :any:`standardization_order` for a full
description. The return value of this function indicates whether or not the
:any:`current_word_graph` was modified. In other words, if this function
returns ``True``, then the word graph was not previously standardized with
respect to *val*, and was modified by calling this function if ``False`` is
returned, then the word graph was previously standardized with respect to
*val* (although this might not have been known), and was not modified by
calling this function.

:param val: the order of the standardization.
:type val: Order

:returns: Whether or not the word graph was modified by the standardization.
:rtype: bool

.. seealso::  :any:`word_graph.standardize` and :any:`current_spanning_tree`.
)pbdoc");
  }  // init_todd_coxeter

}  // namespace libsemigroups
