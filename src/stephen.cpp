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
Class template for constructing a word graph of left factors a word in a f.p. semigroup.

This page describes the class :any:`StephenPresentationWords` which implements Stephen's
procedure for constructing the :any:`WordGraph` corresponding to the left
factors of a word in a finitely presented semigroup or a finitely presented
inverse semigroup. The algorithm implemented in this class is closely related
to the Todd-Coxeter algorithm (as implemented in :any:`ToddCoxeterWord`) and
originates in :cite:`Stephen1987aa`.

:any:`StephenPresentationWords` inherits from :any:`Runner`.


 .. doctest::

    >>> from libsemigroups_pybind11 import (Stephen, stephen, Presentation,
    ... presentation)
    >>> p = Presentation([0, 1])
    >>> presentation.add_rule(p, [0, 0, 0], [0])
    >>> presentation.add_rule(p, [1, 1, 1], [1])
    >>> presentation.add_rule(p, [0, 1, 0, 1], [0, 0])
    >>> s = Stephen(p)
    >>> s.set_word([1, 1, 0, 1])
    >>> s.run()
    >>> stephen.accepts(s, [1, 1, 0, 0, 1, 0])
    True
    >>> stephen.accepts(s, [])
    False
)pbdoc");
      stephen.def("__repr__", [](Stephen<PresentationType> const& stephen) {
        return to_human_readable_repr(stephen);
      });
      stephen.def(py::init<>(), R"pbdoc(
This function default constructs an empty instance of :any:`StephenPresentationWords`. Use
:py:meth:`~StephenPresentationWords.init` and :py:meth:`~StephenPresentationWords.set_word` to specify the presentation and the word,
respectively.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.
)pbdoc");
      stephen.def(py::init<PresentationType const&>(),
                  py::arg("p"),
                  R"pbdoc(
:sig=(self: StephenPresentationWords, p: PresentationStrings) -> None:
This function constructs :any:`StephenPresentationWords` from a presentation.

:param p: the presentation. 
:type p: PresentationStrings
)pbdoc");
      stephen.def(py::init<Stephen_ const&>(),
                  py::arg("s"),
                  R"pbdoc(
This function costructs a :any:`StephenPresentationWords` object by copying another :any:`StephenPresentationWords` object.

:param s: the :any:`StephenPresentationWords` object to copy.
:type s: StephenPresentationWords
)pbdoc");
      stephen.def("accept_state",
                  &Stephen_::accept_state,
                  R"pbdoc(
This function gets the accept state of the word graph. Running this function
triggers the algorithm implemented in this class (if it hasn't been triggered
already), and then returns the accept state of the produced word graph.

:returns: the node.
:rtype: node_type

:raises LibsemigroupsError:
  if no presentation was set at the construction or
  with :py:meth:`~StephenPresentationWords.init` or if no word was set with :py:meth:`~StephenPresentationWords.set_word`.

.. warning::
    Termination of the Stephen algorithm is undecidable in general, and
    this function may never terminate.
)pbdoc");
      stephen.def(
          "init",
          [](Stephen_& self) -> Stephen_& { return self.init(); },
          R"pbdoc(
Reinitialize an existing Stephen object.

This function puts a :any:`StephenPresentationWords` object back into the same state as if it
had been newly default constructed.

:returns: A reference to ``self``.
:rtype: StephenPresentationWords

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.
)pbdoc");
      stephen.def(
          "init",
          [](Stephen_& self, PresentationType const& p) -> Stephen_& {
            return self.init(p);
          },
          py::arg("p"),
          R"pbdoc(
:sig=(self: StephenPresentationWords, p: PresentationStrings) -> StephenPresentationWords:
Initialize from a presentation.

This function puts a :any:`StephenPresentationWords` object back into the same state as if it
had been newly constructed from the presentation ``p``.

:param p: the presentation.
:type p: PresentationStrings

:returns: A reference to ``self``.
:rtype: StephenPresentationWords
)pbdoc");
      stephen.def("is_word_set",
                  &Stephen_::is_word_set,
                  R"pbdoc(
Check if the initial word is set.

Returns true if a word has been set with :py:meth:`~StephenPresentationWords.set_word` since the last
presentation change and false otherwise.

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

This function appends the :any:`StephenPresentationWords` object ``that`` to ``self`` . This
modifies the current :any:`StephenPresentationWords` instance in-place. The result is a
:any:`StephenPresentationWords` instance with underlying word equal to the concatenation of
``this.word()`` and ``that.word()``.The advantage of this is that if either
``self`` or ``that`` have already been (partially) run, then we can reuse the
underlying word graphs instead of having to recompute them completely from
scratch.

:param that: the Stephen instance to append.
:type that: StephenPresentationWords

:returns: A reference to ``self``.
:rtype: StephenPresentationWords

:raises LibsemigroupsError:
  if no presentation was set at the construction of
  ``self`` or ``that`` or with :py:meth:`~StephenPresentationWords.init` or if no word was set with
  :py:meth:`~StephenPresentationWords.set_word`.

:raises LibsemigroupsError:
  if the presentations for ``self`` and ``that`` differ.
)pbdoc");
      stephen.def(
          "__eq__",
          [](Stephen_& self, Stephen_& other) { return self == other; },
          py::arg("that"),
          R"pbdoc(
Check equality of two Stephen instances.

This function triggers a run of the Stephen algorithm of ``self`` and ``other``,
if it hasn't been run already, and then checks that ``this.word()`` equals
``that.word()`` in the underlying semigroup.

:param that: the Stephen instance to compare.
:type that: StephenPresentationWords

:returns: A bool.
:rtype: bool

:raises LibsemigroupsError: 
  if no presentation was set at the construction of
  ``self`` or ``that`` or with :py:meth:`~StephenPresentationWords.init` or if no word was set
  with :py:meth:`~StephenPresentationWords.set_word`.

:raises LibsemigroupsError:
  if the presentations for ``self`` and ``that`` differ.

.. warning::
  Termination of the Stephen algorithm is undecidable in general, and
  this function may never terminate.
)pbdoc");
      stephen.def("presentation",
                  &Stephen_::presentation,
                  R"pbdoc(
:sig=(self: StephenPresentationWords) -> PresentationStrings:
Get the input presentation.

:returns: A presentation.
:rtype: PresentationStrings
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

This function sets the word whose left factors, or equivalent words, are sought.

:param word: the word to be set.
:type word: List[int]

:returns: ``self``.
:rtype: StephenPresentationWords

:raises LibsemigroupsError:
  if any of the values pointed at by the iterators
  is out of range, i.e. they do not belong to ``presentation().alphabet()``.
)pbdoc");
      stephen.def("word",
                  &Stephen_::word,
                  R"pbdoc(
Get the initial word.

Returns the word set by :py:meth:`~StephenPresentationWords.set_word`.

:returns: A  word.
:rtype: List[int]

:raises LibsemigroupsError:
  if no presentation was set at the construction or
  with :py:meth:`~StephenPresentationWords.init` or if no word was set with
  :py:meth:`~StephenPresentationWords.set_word`.
)pbdoc");
      stephen.def("word_graph",
                  &Stephen_::word_graph,
                  R"pbdoc(
Get the word graph.
Returns the word graph in its present state. The algorithm implemented in this
class is not triggered by calls to this function.

:returns: A :any:`WordGraph`.
:rtype: WordGraph

:raises LibsemigroupsError:
  if no presentation was set at the construction or
  with :py:meth:`~StephenPresentationWords.init` or if no word was set with
  :py:meth:`~StephenPresentationWords.set_word`.
)pbdoc");
      stephen.def_static("initial_state",
                         &Stephen_::initial_state,
                         R"pbdoc(
Get the initial state of the word graph.

:returns: the node.
:rtype: node_type
)pbdoc");

      // Helpers
      m.def("accepts",
            &stephen::accepts<PresentationType>,
            py::arg("s"),
            py::arg("w"),
            R"pbdoc(
:sig=(s: StephenPresentationWords, w: List[int]) -> bool:
:only-document-once:
Check if a word is accepted by a :any:`StephenPresentationWords` instance.

This function triggers the algorithm implemented in this class (if it hasn't
been triggered already), and then returns ``True`` if the input
word ``w`` labels a path in
:any:`StephenPresentationWords.word_graph` with source ``0`` and target
:any:`StephenPresentationWords.accept_state`.

For a :any:`StephenPresentationWords` instance constructed from a :any:`PresentationStrings`,
a word ``w`` is accepted if and
only if ``w`` is equivalent to :any:`StephenPresentationWords.word` in the semigroup defined by
:any:`StephenPresentationWords.presentation`.

For a :any:`StephenPresentationWords` instance constructed from a :any:`InversePresentationStrings`,
a word ``w`` is accepted
if and only if :math:`uu^{-1}w` is equivalent to :math:`u` in the semigroup
defined by :any:`StephenPresentationWords.presentation`, where :math:`u` is the value of
:any:`StephenPresentationWords.word`.

:param s: the Stephen instance 
:type s: StephenPresentationWords

:param w: the input word.
:type w: List[int]

:returns: A ``bool``.
:rtype: bool

:raises LibsemigroupsError:
  if no presentation was set at the construction of
  ``s`` or with :any:`StephenPresentationWords.init` or if no word was set with
  :any:`StephenPresentationWords.set_word`.

.. warning::
    Termination of the Stephen algorithm is undecidable in general, and
    this function may never terminate.
)pbdoc");
      m.def("dot",
            &stephen::dot<PresentationType>,
            py::arg("s"),
            R"pbdoc(
:sig=(s: StephenPresentationWords) -> Dot:
:only-document-once:
Return a :any:`Dot` object representing the underlying word graph of the :any:`StephenPresentationWords` object ``s``.

:param s: the Stephen object.
:type s: Stephen

:returns: A :any:`Dot` object.
:rtype: Dot
)pbdoc");
      m.def("is_left_factor",
            &stephen::is_left_factor<PresentationType>,
            py::arg("s"),
            py::arg("w"),
            R"pbdoc(
:sig=(s: StephenPresentationWords, w: List[int]) -> bool:
:only-document-once:
Check if a word is a left factor of :any:`StephenPresentationWords.word`.

This function triggers the algorithm implemented in this class (if it hasn't
been triggered already), and then returns ``True`` if the input
word ``w`` is a left factor of :any:`StephenPresentationWords.word`
in the semigroup defined by
:any:`StephenPresentationWords.presentation` . A word is a left
factor of :any:`StephenPresentationWords.word` if it labels a path
in :any:`StephenPresentationWords.word_graph` with source ``0``.

:param s: the Stephen instance.
:type s: StephenPresentationWords

:param w: a const reference to the input word.
:type w: List[int]

:returns: A ``bool``.
:rtype: bool

:raises LibsemigroupsError:
  if no presentation was set at the construction of ``s`` or with
  :any:`StephenPresentationWords.init` or if no word was set with
  :any:`StephenPresentationWords.set_word`.

.. warning::
    Termination of the Stephen algorithm is undecidable in general, and
    this function may never terminate.
)pbdoc");
      m.def("left_factors",
            &stephen::left_factors<PresentationType>,
            py::arg("s"),
            R"pbdoc(
:sig=(s: StephenPresentationWords) -> Paths:
:only-document-once:
Returns a :any:`Paths` object containing all the words (in short-lex order) that are left factors of :any:`StephenPresentationWords.word`.

This function triggers the algorithm implemented in this class (if it hasn't been triggered already).

:param s: the Stephen instance
:type s: StephenPresentationWords

:returns: A :any:`Paths` object containing all the words (in short-lex order) that are left factors of :any:`StephenPresentationWords.word`.
:rtype: Paths

:raises LibsemigroupsError:  
  if no presentation was set at the construction of
  ``s`` or with :any:`StephenPresentationWords.init` or if no word
  was set with :any:`StephenPresentationWords.set_word`.

.. warning::
    Termination of the Stephen algorithm is undecidable in general, and
    this function may never terminate.
)pbdoc");
      m.def("number_of_left_factors",
            &stephen::number_of_left_factors<PresentationType>,
            py::arg("s"),
            py::arg("min") = 0,
            py::arg("max") = static_cast<size_t>(POSITIVE_INFINITY),
            R"pbdoc(
:sig=(s: StephenPresentationWords, min: int = 0, max: int | PositiveInfinity = POSITIVE_INFINITY) -> int | PositiveInfinity:
:only-document-once:
Returns the number of left factors with length in a given range.

This function returns the number of left factors of the
:any:`StephenPresentationWords.word` in the instance ``s`` with
length between ``min`` and ``max`` . This is the same as the number
of paths in :any:`StephenPresentationWords.word_graph` (if the inherited
:any:`Runner.run` method of ``s`` has been called) with source
``0`` and length in the range ``min`` to ``max``.

:param s: the Stephen instance. 
:type s: StephenPresentationWords

:param min: the minimum length of a word (default: 0). 
:type min: int

:param max: one more than the maximum length of a word (default: :any:`POSITIVE_INFINITY`).
:type max: int | PositiveInfinity

:returns: An ``int`` or :any:`POSITIVE_INFINITY`.
:rtype: int | PositiveInfinity

:raises LibsemigroupsError:
  if no presentation was set at the construction of
  ``s`` or with :any:`StephenPresentationWords.init` or if no word
  was set with :any:`StephenPresentationWords.set_word`.

.. warning::
  Termination of the Stephen algorithm is undecidable in general, and
  this function may never terminate.
)pbdoc");
      m.def("number_of_words_accepted",
            &stephen::number_of_words_accepted<PresentationType>,
            py::arg("s"),
            py::arg("min") = 0,
            py::arg("max") = static_cast<size_t>(POSITIVE_INFINITY),
            R"pbdoc(
:sig=(s: StephenPresentationWords, min: int = 0, max: int | PositiveInfinity = POSITIVE_INFINITY) -> int | PositiveInfinity:
:only-document-once:
Returns the number of words accepted with length in a given range.

This function triggers the algorithm implemented in this class (if it
hasn't been triggered already) and then returns the the number of paths
in :any:`StephenPresentationWords.word_graph` with source ``0``, target
:any:`StephenPresentationWords.accept_state`, and length in the
range ``min`` to ``max``.
        
For a :any:`StephenPresentationWords` instance constructed from a :any:`PresentationStrings`
this is the same as the number of words that are equivalent to
:any:`StephenPresentationWords.word` with length between ``min`` and ``max``.

For a :any:`StephenPresentationWords` instance constructed from a :any:`InversePresentationStrings`,
this is the same as the number of words ``w`` such that
:math:`uu^{-1}w` is equivalent to :math:`u` in the semigroup
defined by :any:`StephenPresentationWords.presentation`, where :math:`u` is the value of
:any:`StephenPresentationWords.word`.

:param s: the Stephen instance. 
:type s: StephenPresentationWords

:param min: the minimum length of a word (default: 0). 
:type min: int

:param max: one more than the maximum length of a word (default: :any:`POSITIVE_INFINITY`).
:type max: int | PositiveInfinity

:returns: An ``int`` or :any:`POSITIVE_INFINITY`.
:rtype: int | PositiveInfinity

:raises LibsemigroupsError:
  if no presentation was set at the construction of ``s`` or with
  :any:`StephenPresentationWords.init` or if no word was set with
  :any:`StephenPresentationWords.set_word`.

.. warning::
  Termination of the Stephen algorithm is undecidable in general, and
  this function may never terminate.
)pbdoc");
      m.def("words_accepted",
            &stephen::words_accepted<PresentationType>,
            py::arg("s"),
            R"pbdoc(
:sig=(s: StephenPresentationWords) -> Paths:
:only-document-once:
Returns a :any:`Paths` object containing all words accepted by a Stephen instance in short-lex order.

This function triggers the algorithm implemented in this class (if it hasn't been triggered already).

:param s: the Stephen instance
:type s: StephenPresentationWords

:returns: A :any:`Paths` object containing all words equivalent to :any:`StephenPresentationWords.word` in short-lex order.
:rtype: Paths

:raises LibsemigroupsError:
  if no presentation was set at the construction of
  ``s`` or with :any:`StephenPresentationWords.init` or if no word
  was set with :any:`StephenPresentationWords.set_word`.

.. warning::
  Termination of the Stephen algorithm is undecidable in general, and
  this function may never terminate.
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
