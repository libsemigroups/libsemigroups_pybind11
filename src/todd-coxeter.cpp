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

// libsemigroups headers
#include <chrono>
#include <libsemigroups/cong-intf.hpp>
#include <libsemigroups/todd-coxeter.hpp>

// pybind11....
#include <pybind11/chrono.h>
// #include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_todd_coxeter

namespace py = pybind11;

namespace libsemigroups {

  void init_todd_coxeter(py::module& m) {
    py::class_<ToddCoxeter, CongruenceInterface> thing(
        m, "ToddCoxeter", R"pbdoc(
Class containing an implementation of the Todd-Coxeter Algorithm.

This class contains an implementation of the Todd-Coxeter
algorithm for computing 1-sided (right), and 2-sided congruences on
a semigroup or monoid.

In this documentation we use the term "congruence enumeration" to mean
the execution of (any version of) the Todd-Coxeter algorithm.

.. seealso :any:`congruence_kind` and :any:`tril`.

.. doctest::

   >>> from libsemigroups_pybind11 import presentation, Presentation, ToddCoxeter
   >>> from libsemigroups_pybind11 import congruence_kind, word_graph, Order
   >>> from libsemigroups_pybind11 import todd_coxeter
   >>> p = Presentation("ab")
   >>> p.contains_empty_word(True)
   <monoid presentation with 2 letters, 0 rules, and length 0>
   >>> presentation.add_rule(p, "aa", "")
   >>> presentation.add_rule(p, "a", "b")
   >>> tc = ToddCoxeter(congruence_kind.onesided, p)
   >>> tc.strategy(ToddCoxeter.options.strategy.felsch)
   <ToddCoxeter over <monoid presentation with 2 letters, 2 rules, and length 4> with 1/1 active/nodes>
   >>> tc.number_of_classes()
   2
   >>> tc.contains("aaaa", "aa")
   True
   >>> tc.index_of("aaaa")
   0
   >>> options = ToddCoxeter.options
   >>> p = Presentation("abcd")
   >>> presentation.add_rule(p, "aa", "a");
   >>> presentation.add_rule(p, "ba", "b");
   >>> presentation.add_rule(p, "ab", "b");
   >>> presentation.add_rule(p, "ca", "c");
   >>> presentation.add_rule(p, "ac", "c");
   >>> presentation.add_rule(p, "da", "d");
   >>> presentation.add_rule(p, "ad", "d");
   >>> presentation.add_rule(p, "bb", "a");
   >>> presentation.add_rule(p, "cd", "a");
   >>> presentation.add_rule(p, "ccc", "a");
   >>> presentation.add_rule(p, "bcbcbcbcbcbcbc", "a");
   >>> presentation.add_rule(p, "bcbdbcbdbcbdbcbdbcbdbcbdbcbdbcbd", "a");
   >>> tc = ToddCoxeter(congruence_kind.twosided, p)
   >>> tc.strategy(options.strategy.hlt).lookahead_extent(options.lookahead_extent.partial).save(False)
   <ToddCoxeter over <semigroup presentation with 4 letters, 12 rules, and length 79> with 1/2 active/nodes>
   >>> tc.number_of_classes()
   10752
   >>> tc
   <ToddCoxeter over <semigroup presentation with 4 letters, 12 rules, and length 79> with 10753/2097153 active/nodes>
   >>> tc.word_graph()
   <WordGraph with 10753 nodes, 43012 edges, & out-degree 4>
   >>> it = todd_coxeter.normal_forms(tc, Word=str)
   >>> [next(it) for _ in range(10)]
   ['a', 'b', 'c', 'd', 'bc', 'bd', 'cb', 'db', 'bcb', 'bdb']
   >>> tc.standardize(Order.lex)
   True
   >>> it = todd_coxeter.normal_forms(tc, Word=str)
   >>> [next(it) for _ in range(10)]
   ['a', 'ab', 'abc', 'abcb', 'abcbc', 'abcbcb', 'abcbcbc', 'abcbcbcb', 'abcbcbcbc', 'abcbcbcbcb']
   )pbdoc");

    py::class_<ToddCoxeter::options> options(thing,
                                             "options",
                                             R"pbdoc(
This class containing various options that can be used to control the
behaviour of Todd-Coxeter.)pbdoc");

    py::enum_<ToddCoxeter::options::strategy> strategy(options,
                                                       "strategy",
                                                       R"pbdoc(
          Values for defining the strategy.

          The values in this enum can be used as the argument for the method
          :py:meth:`ToddCoxeter.strategy` to specify which strategy should be
          used when performing a coset enumeration.
        )pbdoc");
    strategy
        .value(
            "hlt",
            ToddCoxeter::options::strategy::hlt,
            R"pbdoc( This value indicates that the HLT (Hazelgrove-Leech-Trotter) strategy should be used. This is analogous to ACE's R-style.)pbdoc")
        .value(
            "felsch",
            ToddCoxeter::options::strategy::felsch,
            R"pbdoc( This value indicates that the Felsch strategy should be used. This is analogous to ACE's C-style.)pbdoc")
        .value(
            "CR",
            ToddCoxeter::options::strategy::CR,
            R"pbdoc(This strategy is meant to mimic the ACE strategy of the same name. The Felsch is run until at least :any:`f_defs` nodes are defined, then the HLT strategy is run until at least :any:`hlt_defs` divided by :math:`N` nodes have been defined, where :math:`N` is the sum of the lengths of the words in the presentation and generating pairs. These steps are repeated until the enumeration terminates.)pbdoc")
        .value(
            "R_over_C",
            ToddCoxeter::options::strategy::R_over_C,
            R"pbdoc(This strategy is meant to mimic the ACE strategy R/C. The HLT strategy is run until the first lookahead is triggered (when :the number of nodes active is at least :any:`lookahead_next`). A full lookahead is then performed, and then the CR strategy is used.)pbdoc")
        .value(
            "Cr",
            ToddCoxeter::options::strategy::Cr,
            R"pbdoc(This strategy is meant to mimic the ACE strategy Cr. The Felsch strategy is run until at least :any:`f_defs` new nodes have been defined, then the HLT strategy is run until at least :any:`hlt_defs` divided by :math:`N` nodes have been defined, where :math:`N` is the sum of the lengths of the words in the presentation and generating pairs. Then the Felsch strategy is run.)pbdoc")
        .value(
            "Rc",
            ToddCoxeter::options::strategy::Rc,
            R"pbdoc(This strategy is meant to mimic the ACE strategy Rc. The HLT strategy is run until at least :any:`hlt_defs` divided by :math:`N` new nodes have been defined (where :math:`N` is the sum of the lengths of the words in the presentation and generating pairs) the Felsch strategy is then run until at least :any:`f_defs` new nodes are defined, and then the HLT strategy is run.)pbdoc")
        .export_values();

    py::enum_<ToddCoxeter::options::lookahead_extent>(options,
                                                      "lookahead_extent",
                                                      R"pbdoc(
Enum for specifying the extent of any lookahead performed.

The values in this enum can be used as the argument for
:any:`ToddCoxeter.lookahead_extent` to specify the extent of any lookahead that
should be performed.)pbdoc")
        .value(
            "full",
            ToddCoxeter::options::lookahead_extent::full,
            R"pbdoc(Perform a full lookahead from every node in the word graph. Full lookaheads are therefore sometimes slower but may detect more coincidences than a partial lookahead.)pbdoc")
        .value(
            "partial",
            ToddCoxeter::options::lookahead_extent::partial,
            R"pbdoc(Perform a partial lookahead starting from the current node in the word graph. Partial lookaheads are sometimes faster but may not detect as many coincidences as a full lookahead.)pbdoc")
        .export_values();

    py::enum_<ToddCoxeter::options::lookahead_style>(
        options,
        "lookahead_style",
        R"pbdoc(Enum class for specifying the style of any lookahead
performed.

The values in this enum can be used as the argument for
:any:`ToddCoxeter.lookahead_style` to specify the style of any lookahead that
should be performed.)pbdoc")
        .value(
            "hlt",
            ToddCoxeter::options::lookahead_style::hlt,
            R"pbdoc( The lookahead will be done in HLT style by following the paths labelled by every relation from every node in the range specified by lookahead_extent::full or lookahead_extent::partial.)pbdoc")
        .value(
            "felsch",
            ToddCoxeter::options::lookahead_style::felsch,
            R"pbdoc(The lookahead will be done in Felsch style where every edge is considered in every path labelled by a relation in which it occurs.)pbdoc")
        .export_values();

    py::enum_<ToddCoxeter::options::def_policy>(options,
                                                "def_policy",
                                                R"pbdoc(
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
)pbdoc")
        .value(
            "no_stack_if_no_space",
            ToddCoxeter::options::def_policy::no_stack_if_no_space,
            R"pbdoc(Do not put newly generated definitions in the stack if the stack already has size :any:`def_max`.)pbdoc")
        .value(
            "purge_from_top",
            ToddCoxeter::options::def_policy::purge_from_top,
            R"pbdoc(If the definition stack has size :any:`def_max` and a new definition is generated, then definitions with dead source node are are popped from the top of the stack (if any).)pbdoc")
        .value(
            "purge_all",
            ToddCoxeter::options::def_policy::purge_all,
            R"pbdoc(If the definition stack has size :any:`def_max` and a new definition is generated, then definitions with dead source node are are popped from the entire of the stack (if any).)pbdoc")
        .value(
            "discard_all_if_no_space",
            ToddCoxeter::options::def_policy::discard_all_if_no_space,
            R"pbdoc(If the definition stack has size :any:`def_max` and a new definition is generated, then all definitions in the stack are discarded.)pbdoc")
        .value(
            "unlimited",
            ToddCoxeter::options::def_policy::unlimited,
            R"pbdoc(There is no limit to the number of definitions that can be put in the stack.)pbdoc")
        .export_values();

    py::enum_<ToddCoxeter::word_graph_type::options::def_version>(
        options,
        "def_version",
        R"pbdoc(Values for specifying how to handle definitions.)pbdoc")
        .value("one",
               ToddCoxeter::word_graph_type::options::def_version::one,
               R"pbdoc(Version 1 definition processing.)pbdoc")
        .value("two",
               ToddCoxeter::word_graph_type::options::def_version::two,
               R"pbdoc(Version 2 definition processing.)pbdoc")
        .export_values();

    ////////////////////////////////////////////////////////////////////////
    // Constructors + Initializers
    ////////////////////////////////////////////////////////////////////////

    thing.def("__repr__", [](ToddCoxeter const& self) {
      return to_human_readable_repr(self);
    });

    thing.def(py::init<>(),
              R"pbdoc(
Default constructor. This function default constructs an uninitialised
:any:`ToddCoxeter` instance.
)pbdoc");
    thing.def(
        "init",
        [](ToddCoxeter& self) { return self.init(); },
        R"pbdoc(
Re-initialize a ToddCoxeter instance. This function puts a
:any:`ToddCoxeter` instance back into the state that it would have been
in if it had just been newly default constructed.

:returns:
    ``self``.
:rtype:
    ToddCoxeter
    )pbdoc");
    thing.def(
        "copy",
        [](ToddCoxeter const& self) { return ToddCoxeter(self); },
        R"pbdoc(
Copy a :any:`ToddCoxeter` object.

:returns: A copy.
:rtype: ToddCoxeter
    )pbdoc");
    thing.def(
        "__copy__",
        [](ToddCoxeter const& self) { return ToddCoxeter(self); },
        R"pbdoc(
Copy a :any:`ToddCoxeter` object.

:returns: A copy.
:rtype: ToddCoxeter
    )pbdoc");

    thing.def(py::init<congruence_kind, Presentation<std::string> const&>(),
              py::arg("knd"),
              py::arg("p"),
              R"pbdoc(
:sig=(self: ToddCoxeter, knd: congruence_kind, p: Presentation) -> None:

Construct from :any:`congruence_kind` and :any:`PresentationStrings`.

This function constructs a :any:`ToddCoxeter` instance representing a
congruence of kind *knd* over the semigroup or monoid defined by the
presentation *p*. The type of the words in *p* can be anything, but will be
converted into ``List[int]``. This means that if the input presentation
uses :any:`str`, for example, as the word type, then this presentation is
converted into a :any:`PresentationStrings` using ``List[int]`` instead. This
converted presentation can be recovered using :any:`ToddCoxeter.presentation`.

:param knd: the kind (onesided or twosided) of the congruence.
:type knd: congruence_kind

:param p: the presentation.
:type p: PresentationStrings

:raises LibsemigroupsError: if *p* is not valid.)pbdoc");

    thing.def(py::init<congruence_kind, Presentation<word_type> const&>(),
              py::arg("knd"),
              py::arg("p"),
              R"pbdoc(
:sig=(self: ToddCoxeter, knd: congruence_kind, p: Presentation) -> None:
TODO(0) make this disappear
)pbdoc");

    thing.def(py::init<congruence_kind, ToddCoxeter&>(),
              py::arg("knd"),
              py::arg("tc"),
              R"pbdoc(
:sig=(self: ToddCoxeter, knd: congruence_kind, tc: ToddCoxeter) -> None:

Construct from :any:`congruence_kind` and :any:`ToddCoxeter`.

This function constructs a :any:`ToddCoxeter` instance representing a
congruence of kind *knd* over the :any:`ToddCoxeter` instance *tc*. The
:any:`ToddCoxeter` instance constructed in this way represents a quotient of
the word graph represented by *tc*.

:param knd: the kind (onesided, or twosided) of the congruence.
:type knd: congruence_kind

:param tc: the ToddCoxeter instance.
:type tc: ToddCoxeter

:raises LibsemigroupsError:
  if the arguments *knd* and *tc* are not compatible. If the first item is
  ``tc.kind()`` and the second is the parameter *knd* , then compatible
  arguments are (one-sided, one-sided), (two-sided, one-sided), and (two-sided,
  two-sided).)pbdoc");

    thing.def(py::init<congruence_kind, WordGraph<uint32_t> const&>(),
              py::arg("knd"),
              py::arg("wg"),
              R"pbdoc(
:sig=(self: ToddCoxeter, knd: congruence_kind, wg: WordGraph) -> None:
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
        [](ToddCoxeter&                   self,
           congruence_kind                knd,
           Presentation<word_type> const& p) { return self.init(knd, p); },
        py::arg("knd"),
        py::arg("p"),
        R"pbdoc(
:sig=(self: ToddCoxeter, knd: congruence_kind, p: Presentation) -> ToddCoxeter:

Re-initialize a ToddCoxeter instance.

This function re-initializes a :any:`ToddCoxeter` instance as if it
had been newly constructed from *knd* and *p*.

:param knd: the kind (onesided or twosided) of the congruence.
:type knd: :any:`congruence_kind`

:param p: the presentation.
:type p: PresentationStrings

:returns:  ``self``.
:rtype: ToddCoxeter

:raises LibsemigroupsError: if *p* is not valid.
    )pbdoc");
    thing.def(
        "init",
        [](ToddCoxeter&                     self,
           congruence_kind                  knd,
           Presentation<std::string> const& p) { return self.init(knd, p); },
        py::arg("knd"),
        py::arg("p"),
        R"pbdoc(
:sig=(self: ToddCoxeter, knd: congruence_kind, p: Presentation) -> ToddCoxeter:
TODO(0) make this disappear
)pbdoc");
    thing.def(
        "init",
        [](ToddCoxeter& self, congruence_kind knd, ToddCoxeter const& tc) {
          return self.init(knd, tc);
        },
        py::arg("knd"),
        py::arg("tc"),
        R"pbdoc(
:sig=(self: ToddCoxeter, knd: congruence_kind, tc: ToddCoxeter) -> ToddCoxeter:

Re-initialize a ToddCoxeter instance.

This function puts a :any:`ToddCoxeter` instance back into the state
that it would have been in if it had just been newly constructed from
``knd`` and ``tc``.

:param knd: the kind (onesided, or twosided) of the congruence.
:type knd: congruence_kind

:param tc: the ToddCoxeter instance.
:type tc: ToddCoxeter

:returns: ``self``.
:rtype: ToddCoxeter

:raises LibsemigroupsError:
  if the arguments ``knd`` and ``tc`` are not compatible. If the first item is
  ``tc.kind()`` and the second is the parameter ``knd`` , then compatible
  arguments are (one-sided, one-sided), (two-sided, one-sided), and (two-sided,
  two-sided).
)pbdoc");

    thing.def(
        "init",
        [](ToddCoxeter&               self,
           congruence_kind            knd,
           WordGraph<uint32_t> const& wg) { return self.init(knd, wg); },
        py::arg("knd"),
        py::arg("wg"),
        R"pbdoc(
:sig=(self: ToddCoxeter, knd: congruence_kind, wg: WordGraph) -> ToddCoxeter:

Re-initialize a ToddCoxeter instance.

This function puts a :any:`ToddCoxeter` instance back into the state
that it would have been in if it had just been newly constructed from
*knd* and *wg*.

:param knd: the kind (onesided or twosided) of the congruence.
:type knd: congruence_kind

:param wg: the word graph.
:type wg: WordGraph

:returns: ``self``.
:rtype: ToddCoxeter
    )pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // Settings
    ////////////////////////////////////////////////////////////////////////

    thing.def(
        "def_max",
        [](ToddCoxeter const& self) { return self.def_max(); },
        R"pbdoc(
Get the current value of the setting for the maximum number of
definitions.

:returns:
   The current value of the setting.
:rtype:
   int
)pbdoc");

    thing.def(
        "def_max",
        [](ToddCoxeter& self, size_t val) -> ToddCoxeter& {
          return self.def_max(val);
        },
        py::arg("val"),
        R"pbdoc(
Set the maximum number of definitions in the stack.

This setting specifies the maximum number of definitions that can be in the
stack at any given time. What happens if there are the maximum number of
definitions in the stack and a new definition is generated is governed by
:any:`ToddCoxeter.def_policy`.

The default value of this setting is ``2000``.

:param val: the maximum size of the definition stack.
:type val: int

:returns: ``self``.
:rtype: ToddCoxeter
)pbdoc");
    thing.def(
        "def_policy",
        [](ToddCoxeter const& self) { return self.def_policy(); },
        R"pbdoc(
Get the current value of the definition policy. This function returns
the current value of the definition policy which specifies how to handle
definitions. For details see :any:`options.def_policy`.

:returns:
   The current value of the setting, a value of type
   :any:`options.def_policy`.

:rtype:
   options.def_policy
)pbdoc");

    thing.def(
        "def_policy",
        [](ToddCoxeter& self, ToddCoxeter::options::def_policy val)
            -> ToddCoxeter& { return self.def_policy(val); },
        py::arg("val"),
        R"pbdoc(
Set the definition policy.

This function can be used to specify how to handle definitions. For details see
:any:`options.def_policy`. The default value of this setting is
``options.def_policy.no_stack_if_no_space``.

:param val: the policy to use.
:type val: options.def_policy

:returns: ``self``.
:rtype: ToddCoxeter
)pbdoc");

    thing.def(
        "def_version",
        [](ToddCoxeter& self) { return self.def_version(); },
        R"pbdoc(
The current value of the definition policy setting.

:returns:
   The current value of the setting.
:rtype:
   options.def_version
)pbdoc");

    thing.def(
        "def_version",
        [](ToddCoxeter& self, ToddCoxeter::options::def_version val)
            -> ToddCoxeter& { return self.def_version(val); },
        py::arg("val"),
        R"pbdoc(
This function can be used to specify how which version of definition handling
to use. For details see :any:`options.def_version`.

The default value of this setting is ``options.def_version.two``.
)pbdoc");

    thing.def(
        "f_defs",
        [](ToddCoxeter const& self) { return self.f_defs(); },
        R"pbdoc(
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

The default value of this setting is ``100000``.

:returns:
   The current value of the setting.
:rtype:
   int
)pbdoc");

    thing.def(
        "f_defs",
        [](ToddCoxeter& self, size_t val) -> ToddCoxeter& {
          return self.f_defs(val);
        },
        py::arg("val"),
        R"pbdoc(
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

The default value of this setting is ``100'000``.

:param val: the value to use.
:type val: int

:returns: ``self``.
:rtype: ToddCoxeter

:raises LibsemigroupsError: if ``val`` is ``0``.
)pbdoc");

    thing.def(
        "hlt_defs",
        [](ToddCoxeter const& self) { return self.hlt_defs(); },
        R"pbdoc(
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

The default value of this setting is ``100000``.

:returns:
   The current value of the setting.
:rtype:
   int
)pbdoc");

    thing.def(
        "hlt_defs",
        [](ToddCoxeter& self, size_t val) -> ToddCoxeter& {
          return self.hlt_defs(val);
        },
        py::arg("val"),
        R"pbdoc(
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

The default value of this setting is ``200000``.

:param val: the value to use.
:type val: int

:returns: ``self``.
:rtype: ToddCoxeter

:raises LibsemigroupsError:  if ``val`` is ``0``.

)pbdoc");

    thing.def(
        "large_collapse",
        [](ToddCoxeter const& self) { return self.large_collapse(); },
        R"pbdoc(
Get the current size of a large collapse. This function can be used to
get what is currently considered a "large" collapse. See
:any:`large_collapse` for the meaning of this setting.

The default value of this setting is ``100'000``.

:returns:
   The current value of the setting.
:rtype:
   int
)pbdoc");
    thing.def(
        "large_collapse",
        [](ToddCoxeter& self, size_t val) -> ToddCoxeter& {
          return self.large_collapse(val);
        },
        py::arg("val"),
        R"pbdoc(
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

:returns: ``self``.
:rtype: ToddCoxeter
)pbdoc");
    thing.def(
        "lookahead_extent",
        [](ToddCoxeter const& self) { return self.lookahead_extent(); },
        R"pbdoc(
Get the current value of the lookahead extent. This function returns the
current value of the lookahead extent setting. The default value of this
setting is ``options.lookahead_extent.partial``.

:returns:
   The current lookahead extent.
:rtype:
   options.lookahead_extent
)pbdoc");
    thing.def(
        "lookahead_extent",
        [](ToddCoxeter& self, ToddCoxeter::options::lookahead_extent val)
            -> ToddCoxeter& { return self.lookahead_extent(val); },
        py::arg("val"),
        R"pbdoc(
Set the lookahead extent.

This function can be used to specify the extent of any lookaheads that might
take place in a congruence enumeration. The possible values are
``options.lookahead_extent.partial`` or ``options.lookahead_extent.full``. The
default value of this setting is ``options.lookahead_extent.partial``.

:param val: the extent.
:type val: options.lookahead_extent

:returns: ``self``.
:rtype: ToddCoxeter
)pbdoc");
    thing.def(
        "lookahead_growth_factor",
        [](ToddCoxeter const& self) { return self.lookahead_growth_factor(); },
        R"pbdoc(
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
        [](ToddCoxeter& self, float val) -> ToddCoxeter& {
          return self.lookahead_growth_factor(val);
        },
        py::arg("val"),
        R"pbdoc(
Set the lookahead growth factor.

This setting determines by what factor the number of nodes required to trigger
a lookahead grows. More specifically, at the end of any lookahead if the number
of active nodes already exceeds the value of :any:`lookahead_next` or the
number of nodes killed during the lookahead is less than the number of active
nodes divided by :any:`lookahead_growth_threshold` , then the value of
:any:`lookahead_next` is increased by a multiple of ``val``. The default value
is of this setting is ``2.0``.

:param val: the value indicating the lookahead growth factor.
:type val: float

:returns: ``self``.
:rtype: ToddCoxeter

:raises LibsemigroupsError:  if ``val`` is less than ``1.0``.
)pbdoc");
    thing.def(
        "lookahead_growth_threshold",
        [](ToddCoxeter const& self) {
          return self.lookahead_growth_threshold();
        },
        R"pbdoc(
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
        [](ToddCoxeter& self, size_t val) -> ToddCoxeter& {
          return self.lookahead_growth_threshold(val);
        },
        py::arg("val"),
        R"pbdoc(
Set the lookahead growth threshold.

This setting determines the threshold for the number of nodes required to
trigger a lookahead. More specifically, at the end of any lookahead if the
number of active nodes already exceeds the value of :any:`lookahead_next` or
the number of nodes killed during the lookahead is less than the number of
active nodes divided by :any:`lookahead_growth_threshold` , then the value of
:any:`lookahead_next` is increased. The default value is ``4``.

:param val: the value indicating the lookahead growth threshold.
:type val: int

:returns: ``self``.
:rtype: ToddCoxeter
)pbdoc");
    thing.def(
        "lookahead_min",
        [](ToddCoxeter const& self) { return self.lookahead_min(); },
        R"pbdoc(
Get the current value of the minimum lookahead setting. This function
returns the current value of the minimum lookahead. See
:any:`lookahead_min` for a full description of this setting.  The
default value is ``10000``.

:returns:
   The current value of the minimum lookahead.
:rtype:
   int
)pbdoc");
    thing.def(
        "lookahead_min",
        [](ToddCoxeter& self, size_t val) -> ToddCoxeter& {
          return self.lookahead_min(val);
        },
        py::arg("val"),
        R"pbdoc(
Set the minimum value of lookahead_next.

After a lookahead is performed the value of :any:`lookahead_next` is modified
depending on the outcome of the current lookahead. If the return value of
:any:`lookahead_next` is too small or too large, then the value is adjusted
according to :any:`lookahead_growth_factor` and
:any:`lookahead_growth_threshold`. This setting specified the minimum possible
value for :any:`lookahead_next()`. The default value is ``10000``.

:param val: value indicating the minimum value of lookahead_next.
:type val: int

:returns: ``self``.
:rtype: ToddCoxeter
)pbdoc");
    thing.def(
        "lookahead_next",
        [](ToddCoxeter const& self) { return self.lookahead_next(); },
        R"pbdoc(
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
        [](ToddCoxeter& self, size_t val) -> ToddCoxeter& {
          return self.lookahead_next(val);
        },
        py::arg("val"),
        R"pbdoc(
Set the threshold that will trigger a lookahead.

If the number of active nodes exceeds the value set by this function, then a
lookahead of style :any:`ToddCoxeter.lookahead_style` and extent
:any:`ToddCoxeter.lookahead_extent` will be triggered. The default value is 5
million.

:param val: value indicating the initial threshold.
:type val: int

:returns: ``self``.
:rtype: ToddCoxeter
)pbdoc");
    thing.def(
        "lookahead_stop_early_interval",
        [](ToddCoxeter const& self) {
          return self.lookahead_stop_early_interval();
        },
        R"pbdoc(
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
        [](ToddCoxeter& self, std::chrono::nanoseconds val) -> ToddCoxeter& {
          return self.lookahead_stop_early_interval(val);
        },
        py::arg("val"),
        R"pbdoc(
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

:returns: ``self``.
:rtype: ToddCoxeter
)pbdoc");
    thing.def(
        "lookahead_stop_early_ratio",
        [](ToddCoxeter const& self) {
          return self.lookahead_stop_early_ratio();
        },
        R"pbdoc(
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
        [](ToddCoxeter& self, float val) -> ToddCoxeter& {
          return self.lookahead_stop_early_ratio(val);
        },
        py::arg("val"),
        R"pbdoc(
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

:returns: ``self``.
:rtype: ToddCoxeter

:raises LibsemigroupsError:
  if ``val`` is not in the interval :math:`[0, 1)`.
)pbdoc");
    thing.def(
        "lookahead_style",
        [](ToddCoxeter const& self) { return self.lookahead_style(); },
        R"pbdoc(
Get the current value of the lookahead style. This function returns the current
value of the lookahead style. See :any:`ToddCoxeter.lookahead_style` for a full description
of this setting.

:returns:
   The current lookahead style.
:rtype:
   options.lookahead_style
)pbdoc");
    thing.def(
        "lookahead_style",
        [](ToddCoxeter& self, ToddCoxeter::options::lookahead_style val)
            -> ToddCoxeter& { return self.lookahead_style(val); },
        py::arg("val"),
        R"pbdoc(
Set the style of lookahead.

This function can be used to set the style of any lookaheads that are performed
during the congruence enumeration. The possible values are
``options.lookahead_style.hlt`` and ``options.lookahead_style.felsch``.

The default value of this setting is ``options.lookahead_style.hlt``.

:param val: the style of lookahead to use.
:type val: options.lookahead_style

:returns: ``self``.
:rtype: ToddCoxeter
)pbdoc");
    thing.def(
        "lower_bound",
        [](ToddCoxeter const& self) { return self.lower_bound(); },
        R"pbdoc(
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
        [](ToddCoxeter& self, size_t val) -> ToddCoxeter& {
          return self.lower_bound(val);
        },
        py::arg("val"),
        R"pbdoc(
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

:returns: ``self``.
:rtype: ToddCoxeter
)pbdoc");
    thing.def(
        "save",
        [](ToddCoxeter const& self) { return self.save(); },
        R"pbdoc(
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
        [](ToddCoxeter& self, bool val) -> ToddCoxeter& {
          return self.save(val);
        },
        py::arg("val"),
        R"pbdoc(
Set whether or not to process definitions during HLT.

If the argument of this function is ``True`` and the HLT strategy is being
used, then definitions are processed during any enumeration. The default value
is ``False``.

:param val: value indicating whether or not to process deductions.
:type val: bool

:returns: ``self``.
:rtype: ToddCoxeter
)pbdoc");
    thing.def(
        "strategy",
        [](ToddCoxeter const& self) { return self.strategy(); },
        R"pbdoc(
Get the current value of the strategy setting. This function returns the
current value of the strategy setting. See :any:`options.strategy` for a full
description of this setting.

:returns:
   The current value.
:rtype:
   options.strategy
)pbdoc");
    thing.def(
        "strategy",
        [](ToddCoxeter& self, ToddCoxeter::options::strategy val)
            -> ToddCoxeter& { return self.strategy(val); },
        py::arg("val"),
        R"pbdoc(
Specify the congruence enumeration strategy.

The strategy used during the enumeration can be specified using this function.
The default value is :any:`options.strategy`.

:param val: value indicating which strategy to use.
:type val: options.strategy

:returns: ``self``.
:rtype: ToddCoxeter
)pbdoc");
    thing.def(
        "use_relations_in_extra",
        [](ToddCoxeter const& self) { return self.use_relations_in_extra(); },
        R"pbdoc(
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
        [](ToddCoxeter& self, bool val) -> ToddCoxeter& {
          return self.use_relations_in_extra(val);
        },
        py::arg("val"),
        R"pbdoc(
Set whether or not to perform an HLT-style push of the defining relations at the identity.

If a :any:`ToddCoxeter` instance is defined over a finitely presented semigroup
or monoid and the Felsch strategy is being used, it can be useful to follow all
the paths from the identity labelled by the underlying relations. This setting
specifies whether or not to do this.The default value of this setting is
``False``.

:param val: the boolean value.
:type val: bool

:returns: ``self``.
:rtype: ToddCoxeter
)pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // Interface common functions
    ////////////////////////////////////////////////////////////////////////

    thing.def(
        "add_generating_pair",
        [](ToddCoxeter&     self,
           word_type const& u,
           word_type const& v) -> ToddCoxeter& {
          return todd_coxeter::add_generating_pair(self, u, v);
        },
        py::arg("u"),
        py::arg("v"),
        R"pbdoc(
:sig=(self: ToddCoxeter, u: List[int] | str, v: List[int] | str) -> ToddCoxeter:
:only-document-once:

Add a generating pair.

This function adds a generating pair to the congruence represented by a :any:`ToddCoxeter` instance.

:param u: the first item in the pair.
:type u: List[int] | str

:param v: the second item in the pair.
:type v: List[int] | str

:raises LibsemigroupsError:
  if any of the values in *u* or *v* is out of range, i.e. they do not belong
  to ``presentation().alphabet()`` and :any:`PresentationStrings.validate_word`
  raises.

:raises LibsemigroupsError:  if :any:`Runner.started` returns ``True``.

:returns: ``self``.
:rtype: ToddCoxeter
)pbdoc");

    thing.def(
        "add_generating_pair",
        [](ToddCoxeter&       self,
           std::string const& u,
           std::string const& v) -> ToddCoxeter& {
          return todd_coxeter::add_generating_pair(self, u, v);
        },
        py::arg("u"),
        py::arg("v"),
        R"pbdoc(
:sig=(self: ToddCoxeter, u: List[int] | str, v: List[int] | str) -> ToddCoxeter:)pbdoc");

    thing.def(
        "contains",
        [](ToddCoxeter& self, word_type const& u, word_type const& v) {
          return todd_coxeter::contains(self, u, v);
        },
        py::arg("u"),
        py::arg("v"),
        R"pbdoc(
:sig=(self: ToddCoxeter, u: List[int] | str, v: List[int] | str) -> bool:
:only-document-once:

Check containment of a pair of words.

This function checks whether or not the words *u* and *v* are contained in the
congruence represented by a :any:`ToddCoxeter` instance. This function triggers
a full enumeration, which may never terminate.

:param u: the first item in the pair.
:type u: List[int] | str

:param v: the second item in the pair.
:type v: List[int] | str

:returns: Whether or not the pair belongs to the congruence.
:rtype: bool

:raises LibsemigroupsError:
  if any of the values in *u* or *v* is out of range, i.e. they do not belong
  to ``presentation().alphabet()`` and :any:`PresentationStrings.validate_word`
  raises.
)pbdoc");

    thing.def(
        "contains",
        [](ToddCoxeter& self, std::string const& u, std::string const& v) {
          return todd_coxeter::contains(self, u, v);
        },
        py::arg("u"),
        py::arg("v"),
        R"pbdoc(:sig=(self: ToddCoxeter, u: List[int] | str, v: List[int] | str) -> bool:)pbdoc");

    thing.def(
        "currently_contains",
        [](ToddCoxeter& self, word_type const& u, word_type const& v) {
          return todd_coxeter::currently_contains(self, u, v);
        },
        py::arg("u"),
        py::arg("v"),
        R"pbdoc(
:sig=(self: ToddCoxeter, u: List[int] | str, v: List[int] | str) -> bool:
:only-document-once:

Check whether a pair of words is already known to belong to the congruence.

This function checks whether or not the words *u* and *v* are already known to
be contained in the congruence represented by a :any:`ToddCoxeter` instance.
This function performs no enumeration, so it is possible for the words to be
contained in the congruence, but that this is not currently known.

:param u: the first item in the pair.
:type u: List[int] | str

:param v: the second item in the pair.
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
)pbdoc");

    thing.def(
        "currently_contains",
        [](ToddCoxeter& self, std::string const& u, std::string const& v) {
          return todd_coxeter::currently_contains(self, u, v);
        },
        py::arg("u"),
        py::arg("v"),
        R"pbdoc(
:sig=(self: ToddCoxeter, u: List[int] | str, v: List[int] | str) -> bool:
)pbdoc");

    thing.def("_number_of_classes",
              &ToddCoxeter::number_of_classes,
              R"pbdoc(
:sig=(self: ToddCoxeter) -> int | PositiveInfinity:

Compute the number of classes in the congruence. This function computes
the number of classes in the congruence represented by a
:any:`ToddCoxeter` instance by running the congruence enumeration until
it terminates.

:returns:
   The number of congruences classes of a :any:`ToddCoxeter` instance if
   this number is finite, or :any:`POSITIVE_INFINITY` in some cases if
   this number is not finite.

:rtype:
   int
)pbdoc");

    thing.def(
        "reduce",
        [](ToddCoxeter& self, word_type const& w) {
          return todd_coxeter::reduce(self, w);
        },
        py::arg("w"),
        R"pbdoc(
:sig=(self: ToddCoxeter, w: List[int] | str) -> List[int] | str:
:only-document-once:

Reduce a word.

This function triggers a full enumeration and then returns a reduced word
equivalent to the input word *w*. The word output by this function is
equivalent to the input word in the congruence defined by a :any:`ToddCoxeter`
instance. In other words, the output word is a normal form for the input word
or equivalently a canconical representative of its congruence class.

:param w: the input word.
:type w: List[int] | str

:raises LibsemigroupsError:
  if any of the values in *w* is out of range, i.e. they do not belong to
  ``presentation().alphabet()`` and :any:`PresentationStrings.validate_word` raises.
  )pbdoc");

    thing.def(
        "reduce",
        [](ToddCoxeter& self, std::string const& w) {
          return todd_coxeter::reduce(self, w);
        },
        py::arg("w"),
        R"pbdoc(
:sig=(self: ToddCoxeter, w: List[int] | str) -> List[int] | str:
  )pbdoc");

    thing.def(
        "reduce_no_run",
        [](ToddCoxeter& self, word_type const& w) {
          return todd_coxeter::reduce_no_run(self, w);
        },
        py::arg("w"),
        R"pbdoc(
:sig=(self: ToddCoxeter, w: List[int] | str) -> List[int] | str:
:only-document-once:

Reduce a word.

This function writes a reduced word equivalent to the input word *w* This
function triggers no congruence enumeration. The word output by this function
is equivalent to the input word in the congruence. If is :any:`Runner.finished`, then
the output word is a normal form for the input word. If the  :any:`ToddCoxeter`
instance is not :any:`Runner.finished`, then it might be that equivalent input words
produce different output words.

:param w: the input word.
:type w: List[int] | str

:raises LibsemigroupsError:
  if any of the values in *w* is out of range, i.e. they do not belong to
  ``presentation().alphabet()`` and :any:`PresentationStrings.validate_word` raises.)pbdoc");

    thing.def(
        "reduce_no_run",
        [](ToddCoxeter& self, std::string const& w) {
          return todd_coxeter::reduce_no_run(self, w);
        },
        py::arg("w"),
        R"pbdoc(
:sig=(self: ToddCoxeter, w: List[int] | str) -> List[int] | str:)pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // Accessors
    ////////////////////////////////////////////////////////////////////////

    thing.def("current_spanning_tree",
              &ToddCoxeter::current_spanning_tree,
              R"pbdoc(
Get the current possible spanning tree of the underlying word graph.

This function returns a const reference to the current value of a possible
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
)pbdoc");
    thing.def(
        "current_word_graph",
        [](ToddCoxeter const& self) -> WordGraph<uint32_t> const& {
          return self.current_word_graph();
        },
        R"pbdoc(
Get the current word graph.

In some sense, the purpose of the
Todd-Coxeter algorithm is to produce a :any:`WordGraph` of the action of
a set of generators on the classes of a congruence. This function can be
used to obtain a reference to that :any:`WordGraph` as it currently
exists within a :any:`ToddCoxeter` instance. This function does not
trigger any enumeration.The :any:`WordGraph` returned by this function
may be in a rather complicated state. No guarantees are given: about the
values of the active nodes (i.e. they may be any non-negative integers
in any order); that the number of nodes (including those that are
inactive) should coincide with the number of active nodes; that the
graph is complete; or that the graph is compatible with the relations of
the :any:`presentation` or with the generating_pairs. The functions
:any:`standardize` and :any:`shrink_to_fit` can be used to modify
the returned word graph in-place to have (possibly) more reasonable
characteristics.

:returns:
  The underlying :any:`WordGraph`.
:rtype:
  WordGraph
)pbdoc");

    thing.def(
        "is_standardized",
        [](ToddCoxeter const& self) { return self.is_standardized(); },
        R"pbdoc(
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
        [](ToddCoxeter& self, Order val) { return self.is_standardized(val); },
        py::arg("val"),
        R"pbdoc(
Check if the word graph is currently standardized with respect to a given order.

This function returns ``True`` if the :any:`current_word_graph` has been
standardized with respect to the order ``val`` ; and ``False`` if not.

:param val: the Order to check for.
:type val: Order

:returns: Whether or not the current word graph is standardized with respect to a given order.
:rtype: bool
)pbdoc");
    thing.def("presentation",
              &ToddCoxeter::presentation,
              R"pbdoc(
Get the presentation used to define a ToddCoxeter instance (if any). If
a :any:`ToddCoxeter` instance is constructed or initialised using a
presentation, then this presentation is returned by this function.
If the :any:`ToddCoxeter` instance was constructed or
initialised from a :any:`WordGraph`, then this presentation will be
empty.

:returns:
  The defining presentation.
:rtype:
   Presentation
)pbdoc");
    thing.def("spanning_tree",
              &ToddCoxeter::spanning_tree,
              R"pbdoc(
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
              &ToddCoxeter::standardization_order,
              R"pbdoc(
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
        [](ToddCoxeter& self) -> WordGraph<uint32_t> const& {
          return self.word_graph();
        },
        R"pbdoc(
Get the word graph after performing a full congruence enumeration.

In some sense, the purpose of the Todd-Coxeter algorithm is to produce a
:any:`WordGraph` of the action of a set of generators on the classes of a
congruence. This function can be used to obtain a reference to that
:any:`WordGraph` instance. This function triggers a full enumeration.The
:any:`WordGraph` returned by this function may be in a rather complicated
state. The active nodes (and nodes) will be :math:`\{0, \ldots, n - 1\}` where
:math:`n` is the number of classes in the congruence if :any:`presentation`
contains the empty word; or the number of classes plus one if
:any:`presentation` does not contain the empty word. The returned
:any:`WordGraph` is also short-lex standardized. The returned :any:`WordGraph`
will usually be complete and compatible with the relations of the
:any:`presentation` and with the generating_pairs. The :any:`WordGraph` may not
be complete or compatible for some values of the settings. For example, if the
setting :any:`lower_bound` is used but is not the same as the number of classes
in the congruence, then the :any:`WordGraph` returned by this function may not
be compatible with the relations of :any:`presentation` or :any:`generating_pairs`.

:returns:
   The underlying :any:`WordGraph`.
:rtype:
  WordGraph
)pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // Modifiers
    ////////////////////////////////////////////////////////////////////////

    thing.def("perform_lookahead",
              &ToddCoxeter::perform_lookahead,
              py::arg("stop_early"),
              R"pbdoc(
Perform a lookahead.

This function can be used to explicitly perform a lookahead. The style and
extent of this lookahead are controlled by the settings :any:`ToddCoxeter.lookahead_style`
and :any:`ToddCoxeter.lookahead_extent`. If the argument *stop_early* is ``True`` , then
the settings :any:`lookahead_stop_early_interval` and
:any:`lookahead_stop_early_ratio` are used to determine whether or not the
lookahead should be aborted early. If *stop_early* is ``False`` , then these
settings are ignored.

:param stop_early:
    whether or not to consider stopping the lookahead early if
    too few nodes are killed.
:type stop_early: bool
)pbdoc");
    thing.def("shrink_to_fit",
              &ToddCoxeter::shrink_to_fit,
              R"pbdoc(
Shrink the underlying word graph to remove all dead nodes. This function
triggers a full enumeration, and standardization, and removes from
:any:`word_graph` any dead nodes. If :any:`Runner.finished` returns ``False`` ,
then this function does nothing.
)pbdoc");
    thing.def("standardize",
              &ToddCoxeter::standardize,
              py::arg("val"),
              R"pbdoc(
Standardize the current_word_graph.

This function standardizes the return value of :any:`current_word_graph` , and
does not trigger any enumeration. See :any:`standardization_order` for a full
description. The return value of this function indicates whether or not the
:any:`current_word_graph` was modified. In other words, if this function
returns ``True``, then the word graph was not previously standardized with
respect to *val* , and was modified by calling this function if ``False`` is
returned, then the word graph was previously standardized with respect to
*val* (although this might not have been known), and was not modified by
calling this function.

:param val: the order of the standardization.
:type val: Order

:returns: Whether or not the word graph was modified by the standardization.
:rtype: bool

.. seealso::  :any:`word_graph.standardize` and :any:`current_spanning_tree`.
)pbdoc");

    thing.def(
        "current_index_of",
        [](ToddCoxeter const& self, word_type const& w) {
          return todd_coxeter::current_index_of(self, w);
        },
        py::arg("w"),
        R"pbdoc(
:sig=(self: ToddCoxeter, w: List[int] | str) -> int:
)pbdoc");

    thing.def(
        "_current_index_of",
        [](ToddCoxeter const& self, std::string const& w) {
          return todd_coxeter::current_index_of(self, w);
        },
        py::arg("w"),
        R"pbdoc(
:sig=(self: ToddCoxeter, w: List[int] | str) -> int:

Returns the current index of the class containing a word.

This function returns the current index of the class containing the word *w* No
enumeration is triggered by calls to this function. Unless :any:`Runner.finished`
returns ``True``, the index returned by this function is essentially arbitrary,
and can only really be used to check whether or not two words are currently
known to belong to the congruence. The returned index is obtained by following
the path in :any:`current_word_graph` from node ``0`` labelled by the word *w*.
If there is no such path, then :any:`UNDEFINED` is returned.

:param w: the word.
:type w: List[int] | str

:returns: The current index of the class containing the word.
:rtype: int | Undefined

:raises LibsemigroupsError:
  if any of the values in *w* is out of range, i.e. they do not belong to
  ``presentation().alphabet()`` and ``PresentationStrings.validate_word``
  raises.
)pbdoc");

    thing.def(
        "index_of",
        [](ToddCoxeter& self, word_type const& w) {
          return todd_coxeter::index_of(self, w);
        },
        py::arg("w"),
        R"pbdoc(
:sig=(self: ToddCoxeter, w: List[int] | str) -> int:
:only-document-once:

Returns the index of the class containing a word.

This function returns the index of the class containing the word *w* A full
enumeration is triggered by calls to this function. If the
:any:`current_word_graph` has not already been standardized, then this function
first standardizes it with respect to :any:`Order.shortlex`; otherwise the
existing standardization order is used. The returned index is obtained by
following the path in :any:`current_word_graph` from node ``0`` labelled by the
word *w* Since a full enumeration is triggered by calls to this function, the
word graph is complete, and so the return value is never :any:`UNDEFINED`.

:param w: the word.
:type w: List[int] | str

:returns: The index of the class containing the word.
:rtype: int

:raises LibsemigroupsError:
    if any of the values in *w*, i.e. they do not belong to
    ``presentation().alphabet()`` and ``PresentationStrings.validate_word`` raises.
)pbdoc");
    thing.def(
        "index_of",
        [](ToddCoxeter& self, std::string const& w) {
          return todd_coxeter::index_of(self, w);
        },
        py::arg("w"),
        R"pbdoc(
:sig=(self: ToddCoxeter, w: List[int] | str) -> int:
)pbdoc");

    // FIXME(0) this causes a full enumeration
    thing.def(
        "current_word_of",
        [](ToddCoxeter& self, size_t i) {
          return todd_coxeter::current_word_of<word_type>(self, i);
        },
        py::arg("i"),
        R"pbdoc(
Returns a current word representing a class with given index.

This function returns the current word representing the class with index *i*.
No enumeration is triggered by calls to this function, but
:any:`current_word_graph` is standardized (using :any:`Order.shortlex`) if it
is not already standardized. The output word is obtained by following a path in
:any:`current_spanning_tree` from the node corresponding to index *i* back to
the root of that tree.

:returns: The word representing the *i*-th class.
:rtype: List[int]

:param i: the index of the class.
:type i: int

:raises LibsemigroupsError:  if *i* is out of bounds.
)pbdoc");

    thing.def(
        "current_str_of",
        [](ToddCoxeter& self, size_t i) {
          return todd_coxeter::current_word_of<std::string>(self, i);
        },
        py::arg("i"),
        R"pbdoc(
Returns a current word representing a class with given index.

This function returns the current word representing the class with index *i*.
No enumeration is triggered by calls to this function, but
:any:`current_word_graph` is standardized (using :any:`Order.shortlex`) if it
is not already standardized. The output word is obtained by following a path in
:any:`current_spanning_tree` from the node corresponding to index *i* back to
the root of that tree.

:returns: The word representing the *i*-th class.
:rtype: str

:param i: the index of the class.
:type i: int

:raises LibsemigroupsError: if *i* is out of bounds.
)pbdoc");

    thing.def(
        "word_of",
        [](ToddCoxeter& self, size_t i) {
          return todd_coxeter::word_of(self, i);
        },
        py::arg("i"),
        R"pbdoc(
Returns a word representing a class with given index.

This function returns the word representing the class with index *i*. A full
enumeration is triggered by calls to this function. The output word is obtained
by following a path in :any:`current_spanning_tree` from the node corresponding
to index *i* back to the root of that tree.

:returns: The word representing the *i*-th class.
:rtype: List[int]

:param i: the index of the class.
:type i: int

:raises LibsemigroupsError:  if *i* is out of bounds.
)pbdoc");
    thing.def(
        "str_of",
        [](ToddCoxeter& self, size_t i) {
          return todd_coxeter::word_of<std::string>(self, i);
        },
        py::arg("i"),
        R"pbdoc(
Returns a word representing a class with given index.

This function returns the word representing the class with index *i*. A full
enumeration is triggered by calls to this function. The output word is obtained
by following a path in :any:`current_spanning_tree` from the node corresponding
to index *i* back to the root of that tree.

:returns: The word representing the *i*-th class.
:rtype: str

:param i: the index of the class.
:type i: int

:raises LibsemigroupsError: if *i* is out of bounds.
)pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // Helpers
    ////////////////////////////////////////////////////////////////////////

    m.def(
        "str_class_by_index",
        [](ToddCoxeter& tc, size_t n) {
          auto c = todd_coxeter::class_by_index<std::string>(tc, n);
          // is this ok, does c somehow get copied into the iterator?
          return py::make_iterator(rx::begin(c), rx::end(c));
        },
        py::arg("tc"),
        py::arg("n"),
        R"pbdoc(
Returns an iterator yielding every string in the congruence class with given
index.

This function returns an iterator yielding every string belonging to the
class with index *n* in the congruence represented by the :any:`ToddCoxeter`
instance *tc*. Calls to this function trigger a full enumeration of *tc*.

:param tc: the ToddCoxeter instance.
:type tc: ToddCoxeter

:param n: the index of the class.
:type n: int

:returns: A iterator yielding the class with index *n*.
:rtype: Iterator[str]

:raises LibsemigroupsError:
    if *n* is greater than or equal to ``tc.number_of_classes()``.
)pbdoc");

    m.def(
        "word_class_by_index",
        [](ToddCoxeter& tc, size_t n) {
          auto c = todd_coxeter::class_by_index<word_type>(tc, n);
          // is this ok, does c somehow get copied into the iterator?
          return py::make_iterator(rx::begin(c), rx::end(c));
        },
        py::arg("tc"),
        py::arg("n"),
        R"pbdoc(
Returns an iterator yielding every word ``List[int]`` in the congruence class
with given index.

This function returns an iterator yielding every word belonging to the
class with index *n* in the congruence represented by the :any:`ToddCoxeter`
instance *tc*. Calls to this function trigger a full enumeration of *tc*.

:param tc: the ToddCoxeter instance.
:type tc: ToddCoxeter

:param n: the index of the class.
:type n: int

:returns: A iterator yielding the class with index *n*.
:rtype: Iterator[List[int]]

:raises LibsemigroupsError:
    if *n* is greater than or equal to ``tc.number_of_classes()``.
  )pbdoc");

    m.def(
        "class_of",
        [](ToddCoxeter& tc, word_type const& w) {
          auto c = todd_coxeter::class_of(tc, w);
          // is this ok, does c somehow get copied into the iterator?
          return py::make_iterator(rx::begin(c), rx::end(c));
        },
        py::arg("tc"),
        py::arg("w"),
        R"pbdoc(
:sig=(tc: ToddCoxeter, w: List[int] | str) -> Iterator[List[int] | str]:
:only-document-once:

Returns an iterator yielding every word (of the same type as *w*) in
the congruence class of the given word *w*.

This function returns an iterator yielding every word in belonging to the same
class as the input word *w* in the congruence represented by the
:any:`ToddCoxeter` instance *tc*. Calls to this function trigger a full
enumeration of *tc*.

:param tc: the ToddCoxeter instance.
:type tc: ToddCoxeter

:param w: the input word.
:type w: List[int] | str

:returns: An iterator yielding words in the class of the input word.
:rtype: Iterator[List[int] | str]

:raises LibsemigroupsError:
  if any of the values in *w* is out of range, i.e. they do not belong to
  ``presentation().alphabet()`` and ``PresentationStrings.validate_word``
  raises.)pbdoc");

    m.def(
        "class_of",
        [](ToddCoxeter& tc, std::string const& w) {
          auto c = todd_coxeter::class_of(tc, w);
          // is this ok, does c somehow get copied into the iterator?
          return py::make_iterator(rx::begin(c), rx::end(c));
        },
        py::arg("tc"),
        py::arg("w"),
        R"pbdoc(
:sig=(tc: ToddCoxeter, w: List[int] | str) -> Iterator[List[int] | str]:)pbdoc");

    // The next 2 functions are documented in the wrapper in
    // libsemigroups_pybind11/todd_coxeter.py, because they have the
    // additional kwarg Word to specify the output type.
    m.def("todd_coxeter_word_normal_forms", [](ToddCoxeter& tc) {
      auto nf = todd_coxeter::normal_forms<word_type>(tc);
      return py::make_iterator(rx::begin(nf), rx::end(nf));
    });

    m.def("todd_coxeter_str_normal_forms", [](ToddCoxeter& tc) {
      auto nf = todd_coxeter::normal_forms<std::string>(tc);
      return py::make_iterator(rx::begin(nf), rx::end(nf));
    });

    m.def("todd_coxeter_is_non_trivial",
          &todd_coxeter::is_non_trivial,
          py::arg("tc"),
          py::arg("tries")     = 10,
          py::arg("try_for")   = std::chrono::milliseconds(100),
          py::arg("threshold") = 0.99,
          R"pbdoc(
:sig=(tc: ToddCoxeter, tries: int, try_for: timedelta, threshold: float) -> tril:

Check if the congruence has more than one class.

Returns :any:`tril.true` if it is possible to show that the congruence is
non-trivial; :any:`tril.false` if the congruence is already known to be
trivial; and :any:`tril.unknown` if it is not possible to show that the
congruence is non-trivial. This function attempts to find a non-trivial
congruence containing the congruence represented by a :any:`ToddCoxeter`
instance.

:param tc: the ToddCoxeter instance.
:type tc: ToddCoxeter

:param tries:
  the number of attempts to find a non-trivial super-congruence
  (default: ``10``).
:type tries: int

:param try_for:
  the amount of time to enumerate the congruence after choosing a random pair
  of representatives and identifying them (default: 100 milliseconds).
:type try_for: datetime.timedelta

:param threshold: the threshold (default: ``0.99``).
:type threshold: float

:returns: Whether or not a non-trivial quotient was found.
:rtype: tril
)pbdoc");

    m.def(
        "todd_coxeter_non_trivial_classes",
        [](ToddCoxeter& tc, std::vector<word_type> const& words) {
          return todd_coxeter::non_trivial_classes(
              tc, rx::iterator_range(words.begin(), words.end()));
        },
        py::arg("tc"),
        py::arg("words"),
        R"pbdoc(
:sig=(tc: ToddCoxeter, words: List[List[int] | str]) -> List[List[List[int]] | List[str]]:
:only-document-once:

Find the non-trivial classes in the partition of a list of words.

This function returns the classes with size at least :math:`2` in the partition
of the words in the list *words* induced by the :any:`ToddCoxeter` instance
*tc*. This function triggers a full enumeration of *tc*.

:param tc: the ToddCoxeter instance.
:type tc: ToddCoxeter

:param words: the input list of words.
:type r: List[List[int] | str]

:returns: The partition of the input list.
:rtype: List[List[List[int]] | List[str]]

:raises LibsemigroupsError:
  if the number of classes in *tc* is infinite. In this case, the enumeration
  of *tc* will not terminate successfully.
)pbdoc");

    m.def(
        "todd_coxeter_non_trivial_classes",
        [](ToddCoxeter& tc, std::vector<std::string> const& words) {
          return todd_coxeter::non_trivial_classes(
              tc, rx::iterator_range(words.begin(), words.end()));
        },
        py::arg("tc"),
        py::arg("words"),
        R"pbdoc(
:sig=(tc: ToddCoxeter, words: List[List[int] | str]) -> List[List[List[int]] | List[str]]:
)pbdoc");

    m.def(
        "todd_coxeter_partition",
        [](ToddCoxeter& tc, std::vector<word_type> const& words) {
          return todd_coxeter::partition(
              tc, rx::iterator_range(words.begin(), words.end()));
        },
        py::arg("tc"),
        py::arg("words"),
        R"pbdoc(
:sig=(tc: ToddCoxeter, words: List[List[int] | str]) -> List[List[List[int]] | List[str]]:
:only-document-once:

Partition a list of words.

This function returns the classes in the partition of the words in the input
list *words* induced by the :any:`ToddCoxeter` instance *tc*. This function
triggers a full enumeration of *tc*.

:param tc: the ToddCoxeter instance.
:type tc: ToddCoxeter

:param words: the input list of words.
:type words: List[List[int] | str]

:returns: The partitioned list of words.
:rtype: List[List[List[int]] | List[str]]

:raises LibsemigroupsError:
  if the number of classes in *tc* is infinite. In this case, the enumeration
  of *tc* will not terminate successfully.
)pbdoc");

    m.def(
        "todd_coxeter_partition",
        [](ToddCoxeter& tc, std::vector<std::string> const& words) {
          return todd_coxeter::partition(
              tc, rx::iterator_range(words.begin(), words.end()));
        },
        py::arg("tc"),
        py::arg("words"),
        R"pbdoc(
:sig=(tc: ToddCoxeter, words: List[List[int] | str]) -> List[List[List[int]] | List[str]]:
)pbdoc");

    m.def(
        "todd_coxeter_redundant_rule",
        [](Presentation<word_type> const& p, std::chrono::milliseconds t)
            -> std::optional<std::pair<word_type, word_type>> {
          auto it = todd_coxeter::redundant_rule(p, t);
          if (it != p.rules.cend()) {
            return std::make_pair(*it, *(it + 1));
          }
          return {};
        },
        py::arg("p"),
        py::arg("t"),
        R"pbdoc(
:sig=(p: PresentationStrings, t: timedelta) -> Tuple[List[int], List[int]] | Tuple[str, str] | None:
:only-document-once:

Return a redundant rule or ``None``.

Starting with the last rule in the presentation, this function attempts to run
the Todd-Coxeter algorithm on the rules of the presentation except for a given
omitted rule. For every such omitted rule, Todd-Coxeter is run for the length
of time indicated by the second parameter *t*, and then it is checked if the
omitted rule can be shown to be redundant. If the omitted rule can be shown to
be redundant in this way, then this rule is returned If no rule can be shown to
be redundant in this way, then ``None`` is returned.

:param p: the presentation.
:type p: PresentationStrings

:param t: time to run Todd-Coxeter for every omitted rule.
:type t: timedelta

:returns: A redundant rule or ``None``.
:rtype: Tuple[List[int], List[int]] | Tuple[str, str] | None
)pbdoc");

    m.def(
        "todd_coxeter_redundant_rule",
        [](Presentation<std::string> const& p, std::chrono::milliseconds t)
            -> std::optional<std::pair<std::string, std::string>> {
          auto it = todd_coxeter::redundant_rule(p, t);
          if (it != p.rules.cend()) {
            return std::make_pair(*it, *(it + 1));
          }
          return {};
        },
        py::arg("p"),
        py::arg("t"),
        R"pbdoc(
:sig=(p: PresentationStrings, t: timedelta) -> Tuple[List[int], List[int]] | Tuple[str, str] | None:
)pbdoc");
  }  // init_todd_coxeter
}  // namespace libsemigroups
