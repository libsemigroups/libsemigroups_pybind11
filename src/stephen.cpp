//
// libsemigroups_pybind11
// Copyright (C) 2025 Reinis Cirpons
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
// TODO complete or delete

// C++ stl headers....
// #include <pybind11/detail/common.h>
// TODO complete or delete

// libsemigroups headers
#include <libsemigroups/constants.hpp>  // for POSITIVE_INFINITY
#include <libsemigroups/stephen.hpp>    // for Stephen
#include <libsemigroups/types.hpp>      // for word_type
// TODO uncomment/delete

// pybind11....
// #include <pybind11/chrono.h>
// #include <pybind11/functional.h>
// #include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
//  TODO uncomment/delete

// libsemigroups_pybind11....
#include "main.hpp"  // for init_stephen

namespace py = pybind11;

namespace libsemigroups {

  namespace {
    template <typename PresentationType>
    void bind_stephen(py::module& m, std::string const& name) {
      using Stephen_ = Stephen<PresentationType>;

      py::class_<Stephen_, Runner> stephen(m,
                                           name.c_str(),
                                           R"pbdoc(
For constructing the word graph of left factors of a word in an f.p. semigroup.

Defined in ``sims.hpp``.This class implements :any:`Stephen` 's procedure for constructing the :any:`WordGraph` corresponding to the left factors of a word in a finitely presented semigroup or a finitely presented inverse semigroup. The algorithm implemented in this class is closely related to the Todd-Coxeter algorithm (as implemented in ToddCoxeter) and originates in :any:`[49]`.)pbdoc");
      stephen.def("__repr__", [](Stephen<PresentationType> const& stephen) {
        return to_human_readable_repr(stephen);
      });
      stephen.def(py::init<>(), R"pbdoc(
Default constructor.
Default constructs an empty instance, use :any:`init` and :any:`set_word` to specify the presentation and the word, respectively.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.)pbdoc");
      stephen.def(py::init<PresentationType const&>(), R"pbdoc(
Construct from a presentation (copy).

:param p: the presentation. 
:type p: PresentationType
Construct an instance from the presentation ``p`` . This constructor copies ``p``.)pbdoc");
      stephen.def(py::init<Stephen_ const&>(), R"pbdoc(
Default copy constructor.
)pbdoc");
      stephen.def("accept_state",
                  &Stephen_::accept_state,
                  R"pbdoc(
Get the accept state of the word graph.
This function triggers the algorithm implemented in this class (if it hasn't been triggered already), and then returns the accept state of the produced word graph.

:raises LibsemigroupsError:  if no presentation was set at the construction or with :any:`Stephen::init` or if no word was set with :any:`Stephen::set_word`. \cong_intf_warn_undecidable{ :any:`Stephen` }.

:returns: A :any:`node_type`.

:rtype: node_type
)pbdoc");
      stephen.def(
          "init",
          [](Stephen_& self) -> Stephen_& { return self.init(); },
          R"pbdoc(
Reinitialize an existing Stephen object.
This function puts a :any:`Stephen` object back into the same state as if it had been newly default constructed.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.

:returns: A reference to ``self``.

:rtype: Stephen
)pbdoc");
      stephen.def(
          "init",
          [](Stephen_& self, PresentationType const& p) -> Stephen_& {
            return self.init(p);
          },
          py::arg("p"),
          R"pbdoc(
Initialize from a presentation (copy).

:param p: the presentation.
:type p: PresentationType
This function puts a :any:`Stephen` object back into the same state as if it had been newly constructed from the presentation ``p`` . This initializer copies ``p``.


:returns: A reference to ``self``.

:rtype: Stephen
)pbdoc");
      stephen.def("is_word_set",
                  &Stephen_::is_word_set,
                  R"pbdoc(
Check if the initial word is set.
Returns true if a word has been set with :any:`set_word` since the last presentation change and false otherwise.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:returns: A bool.

:rtype: bool
)pbdoc");
      stephen.def(
          "__imul__",
          [](Stephen_& self, Stephen_& other) -> Stephen_& {
            self *= other;
            return self;
          },
          py::arg("that"),
          R"pbdoc(
Append a Stephen object.

:param that: the Stephen instance to append.
:type that: Stephen
This function appends the :any:`Stephen` object ``that`` to ``self`` . This modifies the current :any:`Stephen` instance in-place. The result is a :any:`Stephen` instance with underlying word equal to the concatenation of ``this.word()`` and ``that.word()``.The advantage of this is that if either ``self`` or ``that`` have already been (partially) run, then we can reuse the underlying word graphs instead of having to recompute them completely from scratch.

:raises LibsemigroupsError:  if no presentation was set at the construction of ``self`` or ``that`` or with :any:`Stephen::init` or if no word was set with :any:`Stephen::set_word`.

:raises LibsemigroupsError:  if the presentations for ``self`` and ``that`` differ.)pbdoc");
      stephen.def(
          "__eq__",
          [](Stephen_& self, Stephen_& other) { return self == other; },
          py::arg("that"),
          // TODO(0): change this to doc of operator==
          R"pbdoc(
Append a Stephen object.

:param that: the Stephen instance to append.
:type that: Stephen
This function appends the :any:`Stephen` object ``that`` to ``self`` . This modifies the current :any:`Stephen` instance in-place. The result is a :any:`Stephen` instance with underlying word equal to the concatenation of ``this.word()`` and ``that.word()``.The advantage of this is that if either ``self`` or ``that`` have already been (partially) run, then we can reuse the underlying word graphs instead of having to recompute them completely from scratch.

:raises LibsemigroupsError:  if no presentation was set at the construction of ``self`` or ``that`` or with :any:`Stephen::init` or if no word was set with :any:`Stephen::set_word`.

:raises LibsemigroupsError:  if the presentations for ``self`` and ``that`` differ.)pbdoc");
      stephen.def("presentation",
                  &Stephen_::presentation,
                  R"pbdoc(
Get the input presentation.
This function returns a const reference to the input presentation.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:returns: A const reference to a :any:`presentation_type`.

:rtype: presentation_type
)pbdoc");
      // TODO(2): Change to support std::string once we have that implemented in
      // libsemigroups itself
      stephen.def(
          "set_word",
          [](Stephen_& self, word_type const& word) -> Stephen_& {
            return stephen::set_word(self, word);
          },
          py::arg("word"),
          R"pbdoc(
Set the initial word.

:param word: the word to be set.
:type word: word_type

This function sets the word whose left factors, or equivalent words, are sought.

:raises LibsemigroupsError:  if any of the values pointed at by the iterators is out of range, i.e. they do not belong to ``presentation().alphabet()`` and ``Presentation::validate_word`` throws.

.. seealso::  :any:`stephen::set_word`


:returns: A reference to ``self``.

:rtype: typename Iterator1
)pbdoc");
      stephen.def("word",
                  &Stephen_::word,
                  R"pbdoc(
Get the initial word.
Returns a const reference to the word set by :any:`set_word`.

:raises LibsemigroupsError:  if no presentation was set at the construction or with :any:`Stephen::init` or if no word was set with :any:`Stephen::set_word`.

:returns: A const reference to a :any:`word_type`.

:rtype: word_type
)pbdoc");
      stephen.def("word_graph",
                  &Stephen_::word_graph,
                  R"pbdoc(
Get the word graph.
Returns a const reference to the word graph in its present state. The algorithm implemented in this class is not triggered by calls to this function.

:raises LibsemigroupsError:  if no presentation was set at the construction or with :any:`Stephen::init` or if no word was set with :any:`Stephen::set_word`.

:returns: A const reference to a :any:`word_graph_type`.

:rtype: word_graph_type
)pbdoc");
      stephen.def_static("initial_state",
                         &Stephen_::initial_state,
                         R"pbdoc(
Get the initial state of the word graph.
This function return the initial state of :any:`word_graph`.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:returns: A :any:`node_type`.

:rtype: node_type
)pbdoc");

      // Helpers
      m.def("accepts",
            &stephen::accepts<PresentationType>,
            py::arg("s"),
            py::arg("w"),
            R"pbdoc(
Check if a word is equivalent to Stephen::word.

:param s: the Stephen instance 
:type s: Stephen

:param w: a const reference to the input word.
:type w: word_type
This function triggers the algorithm implemented in this class (if it hasn't been triggered already), and then returns ``True`` if the input word ``w`` is equivalent to :any:`Stephen::word` in the semigroup defined by :any:`Stephen::presentation` . A word is equivalent to :any:`Stephen::word` if it labels a path in :any:`Stephen::word_graph` with source ``0`` and target :any:`Stephen::accept_state`.

:raises LibsemigroupsError:  if no presentation was set at the construction of ``s`` or with :any:`Stephen::init` or if no word was set with :any:`Stephen::set_word`. \cong_intf_warn_undecidable{ :any:`Stephen` }.


:returns: A ``bool``.

:rtype: typename PresentationType
)pbdoc");
      m.def("dot",
            &stephen::dot<PresentationType>,
            py::arg("s"),
            R"pbdoc(
Returns a Dot object representing the Stephen word graph.

:param s: the Stephen object.
:type s: Stephen
This function returns a :any:`Dot` object representing the underlying word graph of the :any:`Stephen` object ``s``.


:returns: A :any:`Dot` object.

:rtype: typename PresentationType
)pbdoc");
      m.def("is_left_factor",
            &stephen::is_left_factor<PresentationType>,
            py::arg("s"),
            py::arg("w"),
            R"pbdoc(
Check if a word is a left factor of Stephen::word.

:param s: the Stephen instance 
:type s: Stephen

:param w: a const reference to the input word.
:type w: word_type
This function triggers the algorithm implemented in this class (if it hasn't been triggered already), and then returns ``True`` if the input word ``w`` is a left factor of :any:`Stephen::word` in the semigroup defined by :any:`Stephen::presentation` . A word is a left factor of :any:`Stephen::word` if it labels a path in :any:`Stephen::word_graph` with source ``0``.

:raises LibsemigroupsError:  if no presentation was set at the construction of ``s`` or with :any:`Stephen::init` or if no word was set with :any:`Stephen::set_word`. \cong_intf_warn_undecidable{ :any:`Stephen` }.


:returns: A ``bool``.

:rtype: typename PresentationType
)pbdoc");
      m.def("left_factors",
            &stephen::left_factors<PresentationType>,
            py::arg("s"),
            R"pbdoc(
Returns a range object containing all the words (in short-lex order) that are left factors of Stephen::word.

:param s: the Stephen instance
:type s: Stephen
This function triggers the algorithm implemented in this class (if it hasn't been triggered already).

:raises LibsemigroupsError:  if no presentation was set at the construction of ``s`` or with :any:`Stephen::init` or if no word was set with :any:`Stephen::set_word`. \cong_intf_warn_undecidable{ :any:`Stephen` }.


:returns: A range object containing all the words (in short-lex order) that are left factors of :any:`Stephen::word`.

:rtype: typename PresentationType
)pbdoc");
      m.def("number_of_left_factors",
            &stephen::number_of_left_factors<PresentationType>,
            py::arg("s"),
            py::arg("min") = 0,
            py::arg("max") = static_cast<size_t>(POSITIVE_INFINITY),
            R"pbdoc(
Returns the number of left factors with length in a given range.

:param s: the Stephen instance. 
:type s: Stephen

:param min: the minimum length of a word (default: 0). 
:type min: int

:param max: one more than the maximum length of a word (default: POSITIVE_INFINITY).
:type max: int
This function returns the number of left factors of the :any:`Stephen::word` in the instance ``s`` with length between ``min`` and ``max`` . This is the same as the number of paths in :any:`Stephen::word_graph` (if :any:`Stephen::run` has been called) with source ``0`` and length in the range ``min`` to ``max``.

:raises LibsemigroupsError:  if no presentation was set at the construction of ``s`` or with :any:`Stephen::init` or if no word was set with :any:`Stephen::set_word`. \cong_intf_warn_undecidable{ :any:`Stephen` }.

.. seealso:: WordGraph::number_of_paths.


:returns: A ``int``.

:rtype: typename PresentationType
)pbdoc");
      m.def("number_of_words_accepted",
            &stephen::number_of_words_accepted<PresentationType>,
            py::arg("s"),
            py::arg("min") = 0,
            py::arg("max") = static_cast<size_t>(POSITIVE_INFINITY),
            R"pbdoc(
Returns the number of words accepted with length in a given range.

:param s: the Stephen instance. 
:type s: Stephen

:param min: the minimum length of a word (default: 0). 
:type min: int

:param max: one more than the maximum length of a word (default: POSITIVE_INFINITY).
:type max: int
This function returns the number of words that are equivalent to :any:`Stephen::word` in the instance ``s`` with length between ``min`` and ``max`` . This is the same as the number of paths in :any:`Stephen::word_graph` (if :any:`Stephen::run` has been called) with source ``0`` , target :any:`Stephen::accept_state` , and length in the range ``min`` to ``max``.

:raises LibsemigroupsError:  if no presentation was set at the construction of ``s`` or with :any:`Stephen::init` or if no word was set with :any:`Stephen::set_word`. \cong_intf_warn_undecidable{ :any:`Stephen` }.

.. seealso:: WordGraph::number_of_paths.


:returns: A ``int``.

:rtype: typename PresentationType
)pbdoc");
      m.def("words_accepted",
            &stephen::words_accepted<PresentationType>,
            py::arg("s"),
            R"pbdoc(
Returns a range object containing all words equivalent to Stephen::word in short-lex order.

:param s: the Stephen instance
:type s: Stephen
This function triggers the algorithm implemented in this class (if it hasn't been triggered already).

:raises LibsemigroupsError:  if no presentation was set at the construction of ``s`` or with :any:`Stephen::init` or if no word was set with :any:`Stephen::set_word`. \cong_intf_warn_undecidable{ :any:`Stephen` }.


:returns: A range object containing all words equivlanet to :any:`Stephen::word` in short-lex order.

:rtype: typename PresentationType
)pbdoc");
    }

  }  // namespace
  void init_stephen(py::module& m) {
    // TODO(2): figure out how to handle std::string Stephens once that's
    // supported
    bind_stephen<Presentation<word_type>>(m, "StephenPresentationWords");
    bind_stephen<InversePresentation<word_type>>(
        m, "StephenInversePresentationWords");
  }

}  // namespace libsemigroups
