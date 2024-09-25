//
// libsemigroups_pybind11
// Copyright (C) 2024 James Mitchell
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
// TODO complete or delete

// libsemigroups headers
#include <libsemigroups/froidure-pin-base.hpp>

// pybind11....
// #include <pybind11/chrono.h>
// #include <pybind11/functional.h>
#include <pybind11/pybind11.h>
// #include <pybind11/stl.h>
// TODO uncomment/delete

// libsemigroups_pybind11....
#include "main.hpp"  // for init_froidure_pin_base

namespace py = pybind11;

namespace libsemigroups {

  void init_froidure_pin_base(py::module& m) {
    py::class_<FroidurePinBase> thing(m,
                                      "FroidurePinBase",
                                      R"pbdoc(
Base class for :any:`FroidurePin` containing non-element specific data and
member functions.

.. seealso::  :any:`FroidurePin`.)pbdoc");

    // TODO thing.def("__repr__", &detail::to_string<FroidurePinBase const&>);
    thing.def(
        "batch_size",
        [](FroidurePinBase const& self) { return self.batch_size(); },
        R"pbdoc(
Returns the current value of the batch size.

This function returns the minimum number of elements enumerated in any call to
:any:`run`.

:returns: The current value of the batch size.
:rtype: int

:complexity: Constant.
)pbdoc");
    /* TODO   thing.def(
    "batch_size",
    [](FroidurePinBase& self, size_t batch_size) {
    return self.batch_size(batch_size);
    },
    py::arg("batch_size"),
    R"pbdoc(
    Set a new value for the batch size.

    :param batch_size: the new value for the batch size.
    :type batch_size: int
    The *batch size* is the number of new elements to be found by any call to
    :any:`run` . This is used by, for example, :any:`FroidurePin::position` so
    that it is possible to find the position of an element after only partially
    enumerating the semigroup.The default value of the batch size is ``8192``.

    :exceptions: This function is ``noexcept`` and is guaranteed never to
    throw.

    :complexity: Constant.

    .. seealso::  :any:`batch_size`.


    :returns: A reference to ``self``.

    :rtype: FroidurePinBase
    )pbdoc");*/

    thing.def(
        "current_normal_forms",
        [](FroidurePinBase const& self) {
          return py::make_iterator(self.cbegin_current_normal_forms(),
                                   self.cend_current_normal_forms());
        },
        R"pbdoc(
Returns an iterator containing the so-far enumerated normal forms (if any).
This function returns an iterator pointing at the normal form of the first
element of the semigroup represented by a :any:`FroidurePinBase` instance (if
any). This function does not perform any enumeration of the :any:`FroidurePin`.
If you want to obtain the complete set of rules, then use :any:`normal_forms`
instead.

:returns:
  An iterator yielding a :any:`List[int]`.
:rtype:
  Iterator

:complexity:
  Constant.
)pbdoc");

    thing.def(
        "current_rules",
        [](FroidurePinBase const& self) {
          return py::make_iterator(self.cbegin_current_rules(),
                                   self.cend_current_rules());
        },
        R"pbdoc(
Returns an iterator containing the so-far enumerated rules.
Returns an iterator containing the rules in a confluent
terminating rewriting system defining a semigroup isomorphic to the one
defined by ``self``. This function does not perform any enumeration of the
:any:`FroidurePin` object. If you want to obtain the complete set of rules,
then use :any:`rules` instead.

:returns:
    An iterator yielding ``Tuple[List[int],List[int]]``.
:rtype: Iterator

:complexity: Constant

.. doctest::

    >>> S = FroidurePin(
    ... BMat8([[1,  0,  0,  0],
    ...        [1,  0,  0,  0],
    ...        [1,  0,  0,  0],
    ...        [1,  0,  0,  0]]),
    ... BMat8([[0,  1,  0,  0],
    ...        [0,  1,  0,  0],
    ...        [0,  1,  0,  0],
    ...        [0,  1,  0,  0]]),
    ... BMat8([[0,  0,  1,  0],
    ...        [0,  0,  1,  0],
    ...        [0,  0,  1,  0],
    ...        [0,  0,  1,  0]]),
    ... BMat8([[0,  0,  0,  1],
    ...        [0,  0,  0,  1],
    ...        [0,  0,  0,  1],
    ...        [0,  0,  0,  1]]))
    >>> S.size()
    4
    >>> list(S.rules())
)pbdoc");

    thing.def(
        "normal_forms",
        [](FroidurePinBase& self) {
          return py::make_iterator(self.cbegin_normal_forms(),
                                   self.cend_normal_forms());
        },
        R"pbdoc(
Returns an iterator yielding normal forms. This function returns an iterator
yielding normal forms for the elements of the semigroup represented by a
:any:`FroidurePinBase` instance. This function performs a full
enumeration of the :any:`FroidurePin`. If you want to obtain the current
normal forms without triggering an enumeration, then use
:any:`current_normal_forms` instead.

:complexity:
  Same as :any:`enumerate`.

:returns:
  An iterator of type :any:`Iterator` yielding ``List[int]``.
:rtype:
  Iterator
)pbdoc");

    thing.def(
        "rules",
        [](FroidurePinBase& self) {
          return py::make_iterator(self.cbegin_rules(), self.cend_rules());
        },
        R"pbdoc(
Returns an iterator yielding the so-far enumerated rules.

Returns an iterator yielding the rules in a confluent
terminating rewriting system defining a semigroup isomorphic to the one
defined by ``self``. This function performs a full enumeration of the
:any:`FroidurePin` object. If you want to obtain the current set of rules
without triggering any enumeration, then use :any:`current_rules` instead.

:returns:
    An iterator yielding :any:`Tuple[List[int],List[int]]` .
:rtype: Iterator

:complexity: Constant
)pbdoc");

    thing.def("current_left_cayley_graph",
              &FroidurePinBase::current_left_cayley_graph,
              R"pbdoc(
Returns the so-far enumerated left Cayley graph.

This function return the left Cayley graph of the semigroup as it has been
enumerated so-far. No enumeration is triggered by calls to this function.

:returns:
  The (possibly partially enumerated) left Cayley graph.
:rtype:
   WordGraph

:complexity:
  At worst :math:`O(|S|n)` where :math:`S` is the semigroup represented by
  ``self``, and :math:`n` is the return value of
  :any:`FroidurePin.number_of_generators`.
)pbdoc");

    thing.def("current_length",
              &FroidurePinBase::current_length,
              py::arg("pos"),
              R"pbdoc(
Returns the length of the short-lex least word equal to the element with
given index.

This function returns the length of the short-lex least word (in the
generators) equal to the element with index *pos*.

:param pos: the position.
:type pos: int

:returns: The length of the word equal to the element with index *pos*.
:rtype: int

:raises LibsemigroupsError:
  if ``pos`` is greater than or equal to :any:`current_size`.

:complexity: Constant.

.. seealso::  :any:`length`.
)pbdoc");

    thing.def("current_max_word_length",
              &FroidurePinBase::current_max_word_length,
              R"pbdoc(
Returns the maximum length of a word in the generators so far computed. Every
element of the semigroup can be expressed as the short-lex least product of the
generators that equals that element. This function returns the length of the
longest short-lex least word in the generators that has so far been enumerated.

:returns:
  A value of type ``int``.
:rtype:
  int

:complexity: Constant.
)pbdoc");

    thing.def(
        "current_minimal_factorisation",
        [](FroidurePinBase const&              self,
           FroidurePinBase::element_index_type pos) {
          return self.current_minimal_factorisation(pos);
        },
        py::arg("pos"),
        R"pbdoc(
:sig=(self: FroidurePinBase, pos: int) -> List[int]:

Returns a short-lex least word representing an element given by index.

This is the same as the two-argument member function for
:any:`current_minimal_factorisation`, but it returns a ``List[int]`` by value
instead of modifying its first argument in-place. No enumeration is triggered
by calling this function.

:param pos: the index of the element whose factorisation is sought.
:type pos: int

:returns: A minimal factorisation of the element with index *pos*.
:rtype: List[int]

:raises LibsemigroupsError: if *pos* is out of range.

:complexity:
  At worst :math:`O(mn)` where :math:`m` equals *pos* and
  :math:`n` is the return value of :any:`FroidurePin.number_of_generators`.
)pbdoc");
    thing.def("current_number_of_rules",
              &FroidurePinBase::current_number_of_rules,
              R"pbdoc(
Returns the number of relations that have been found so far. This is
only guaranteed to be the actual number of relations in a presentation
defining the semigroup if the semigroup is fully enumerated.

:returns: The number of rules so-far enumerated.
:rtype:
   int

:complexity:
  Constant.
)pbdoc");
    /*
    thing.def(
    "current_position",
    [](FroidurePinBase const& self, generator_index_type i) {
    return self.current_position(i);
    },
    py::arg("i"),
    R"pbdoc(
    Returns the position in of the generator with specified index.

    :param i: the index of the generators.
    :type i: generator_index_type
    In many cases ``current_position(i)`` will equal ``i`` , examples of when
    this will not be the case are:

    *  there are duplicate generators;
    *  :any:`FroidurePin::add_generators` was called after the semigroup was
    already partially enumerated.



    :raises LibsemigroupsError:  if ``i`` is greater than or equal to
    :any:`FroidurePin::number_of_generators`.

    :complexity: Constant.


    :returns: A value of type :any:`FroidurePinBase::element_index_type`.

    :rtype: FroidurePinBase::element_index_type
    )pbdoc");
    */
    thing.def(
        "current_position",
        [](FroidurePinBase const& self, word_type const& w) {
          return self.current_position(w);
        },
        py::arg("w"),
        R"pbdoc(
::sig=(self: FroidurePinBase, w: List[int]) -> int:

Returns the position corresponding to a word.

Returns the position in the semigroup corresponding to the element
represented by the word *w*. This function returns the position
corresponding to the element obtained by evaluating the word in the
generators *w*. No enumeration is performed, and :any:`UNDEFINED` is
returned if the position of the element corresponding to *w* cannot be
determined.

:param w: a word in the generators.
:type w: word_type

:returns: The current position of the element represented by a word.
:rtype: int

:raises LibsemigroupsError:
  if *w* contains an value exceeding :any:`FroidurePin.number_of_generators`.

:complexity:  :math:`O(n)` where :math:`n` is the length of the word *w*.
)pbdoc");
    thing.def("current_right_cayley_graph",
              &FroidurePinBase::current_right_cayley_graph,
              R"pbdoc(
::sig=(self: FroidurePinBase) -> WordGraph:

Returns a const reference to the right Cayley graph. This function
triggers a full enumeration, and then returns the right Cayley graph of
the semigroup represented by a :any:`FroidurePin` instance.

:returns:
  The full enumerated right Cayley graph.
:rtype:
  WordGraph

:complexity: Constant.
)pbdoc");

    thing.def("current_size",
              &FroidurePinBase::current_size,
              R"pbdoc(
Returns the number of elements so far enumerated. This is only the
actual size of the semigroup if the semigroup is fully enumerated.

:returns:
  The current number of elements that have been enumerated.
:rtype:
  int

:complexity:
  Constant.
)pbdoc");
    thing.def("degree",
              &FroidurePinBase::degree,
              R"pbdoc(
Returns the degree of any and all elements.

:returns:
  The degree of the elements contained in the semigroup.
:rtype:
  int

:complexity:
  Constant.
)pbdoc");
    thing.def("enumerate",
              &FroidurePinBase::enumerate,
              py::arg("limit"),
              R"pbdoc(
Enumerate until at least a specified number of elements are found.

If the semigroup is already fully enumerated, or the number of elements
previously enumerated exceeds *limit*, then calling this function does
nothing. Otherwise, :any:`run` attempts to find at least the maximum of
*limit* and :any:`batch_size` elements of the semigroup.

:param limit: the limit for current_size.
:type limit: int

:exceptions:
  This function guarantees not to throw a :any:`LibsemigroupsError`.

:complexity:
  At worst :math:`O(mn)` where :math:`m` equals *limit* and :math:`n` is the
  return value of :any:`FroidurePin.number_of_generators`.)pbdoc");

    thing.def(
        "factorisation",
        [](FroidurePinBase& self, FroidurePinBase::element_index_type pos) {
          return self.factorisation(pos);
        },
        py::arg("pos"),
        R"pbdoc(
:sig=(self: FroidurePinBase, pos: int) -> List[int]:

Returns a word representing an element given by index.

This is the same as the two-argument member function for :any:`factorisation` ,
but it returns a ``List[int]`` by value instead of modifying an argument
in-place. The key difference between this function and
:any:`minimal_factorisation` is that the resulting factorisation may not be
minimal.

:param pos: the index of the element whose factorisation is sought.
:type pos: int

:returns: A factorisation of the element with index *pos*.
:rtype: List[int]

:raises LibsemigroupsError:
  if *pos* is greater than or equal to :any:`size`.

:complexity:
  At worst :math:`O(mn)` where :math:`m` equals *pos* and
  :math:`n` is the return value of :any:`FroidurePin.number_of_generators`.
)pbdoc");

    thing.def("final_letter",
              &FroidurePinBase::final_letter,
              py::arg("pos"),
              R"pbdoc(
Returns the last letter of the element with specified index.

This function returns the final letter of the element in position ``pos``
of the semigroup, which is the index of the generator corresponding to the
final letter of the element.

:param pos: the position.
:type pos: int

:returns:
  The last letter in the minimal factorisation of the element with given
  position.
:rtype: int

:raises LibsemigroupsError:
  if ``pos`` is greater than or equal to :any:`current_size`.

:complexity: Constant.
)pbdoc");

    thing.def("first_letter",
              &FroidurePinBase::first_letter,
              py::arg("pos"),
              R"pbdoc(
Returns the first letter of the element with specified index.

This function returns the first letter of the element in position *pos*
of the semigroup, which is the index of the generator corresponding to the
first letter of the element.

:param pos: the position.
:type pos: int

:returns:
  The first letter in the minimal factorisation of the element with
  given position.
:rtype: int

:raises LibsemigroupsError:
  if *pos* is greater than or equal to :any:`current_size`.

:complexity: Constant.
)pbdoc");

    thing.def("is_monoid",
              &FroidurePinBase::is_monoid,
              R"pbdoc(
Check if the semigroup is a monoid.

:returns:
  Whether or not the one of any of the elements belongs to the semigroup.
:rtype:
  bool

:complexity:
  At worst :math:`O(|S|n)` where :math:`S` is the semigroup represented
  by ``self`` , and :math:`n` is the return value of
  :any:`FroidurePin.number_of_generators`.
)pbdoc");
    thing.def("left_cayley_graph",
              &FroidurePinBase::left_cayley_graph,
              R"pbdoc(
Returns the left Cayley graph. This function
triggers a full enumeration, and then returns the left Cayley graph of
the semigroup represented by a :any:`FroidurePin` instance.


:complexity:
  constant.

:returns:
  The fully enumerated left Cayley graph.
:rtype:
  WordGraph
)pbdoc");
    thing.def("length",
              &FroidurePinBase::length,
              py::arg("pos"),
              R"pbdoc(
Returns the length of the short-lex least word equal to the element with
given index.

:param pos: the position.
:type pos: int

:returns: The length of the element with index *pos*.
:rtype: int

:raises LibsemigroupsError:
  if *pos* is greater than or equal to :any:`size`.

:complexity: Constant.

.. seealso::  :any:`current_length`.
)pbdoc");
    thing.def(
        "minimal_factorisation",
        [](FroidurePinBase& self, FroidurePinBase::element_index_type pos) {
          return self.minimal_factorisation(pos);
        },
        py::arg("pos"),
        R"pbdoc(
:sig=(self: FroidurePinBase, pos: int) -> List[int]:

Returns a short-lex least word representing an element given by index.

:param pos: the index of the element whose factorisation is sought.
:type pos: int

:returns: A value of type ``word_type``.
:rtype: word_type

:raises LibsemigroupsError:
  if ``pos`` is greater than or equal to :any:`size()`.

:complexity:
  At worst :math:`O(mn)` where :math:`m` equals ``pos`` and :math:`n` is the
  return value of :any:`FroidurePin.number_of_generators`.
)pbdoc");

    thing.def(
        "number_of_elements_of_length",
        [](FroidurePinBase const& self, size_t len) {
          return self.number_of_elements_of_length(len);
        },
        py::arg("len"),
        R"pbdoc(
Returns the number of elements so far enumerated with given length.

This function returns the number of elements that have been enumerated so
far with length *len*. This function does not trigger any enumeration.

:param len: the length.
:type len: int

:returns: The number of elements with length *len*.
:rtype: int

:complexity: Constant.
)pbdoc");
    thing.def(
        "number_of_elements_of_length",
        [](FroidurePinBase const& self, size_t min, size_t max) {
          return self.number_of_elements_of_length(min, max);
        },
        py::arg("min"),
        py::arg("max"),
        R"pbdoc(
Returns the number of elements so far enumerated with length in a given
range.

This function returns the number of elements that have been enumerated so
far with length in the range :math:`[min, max)` . This function does not
trigger any enumeration.

:param min: the minimum length.
:type min: int

:param max: the maximum length plus one.
:type max: int

:returns: The number of elements with lengths in the specified range.
:rtype: int

:complexity: Constant.
)pbdoc");
    thing.def("number_of_rules",
              &FroidurePinBase::number_of_rules,
              R"pbdoc(
Returns the total number of relations in a presentation defining the semigroup.
This function triggers a full enumeration of the semigroup.

:returns: The number of rules so-far found.
:rtype: int

:complexity:
  At worst :math:`O(|S|n)` where :math:`S` is the semigroup represented by
  ``self`` , and :math:`n` is the return value of
  :any:`FroidurePin.number_of_generators`.
)pbdoc");
    thing.def("prefix",
              &FroidurePinBase::prefix,
              py::arg("pos"),
              R"pbdoc(
Returns the position of the longest proper prefix.

Returns the position of the prefix of the element ``x`` in position *pos*
(of the semigroup) of length one less than the length of ``x``.

:param pos: the position.
:type pos: int

:returns: The position of the prefix.
:rtype: int

:raises LibsemigroupsError:
  if *pos* is greater than or equal to :any:`current_size`.

:complexity: Constant.
)pbdoc");

    thing.def("right_cayley_graph",
              &FroidurePinBase::right_cayley_graph,
              R"pbdoc(
Returns the fully enumerated right Cayley graph.

:complexity:
  At worst :math:`O(|S|n)` where :math:`S` is the semigroup represented
  by ``self``, and :math:`n` is the return value of
  :any:`FroidurePin.number_of_generators`.

:returns:
  The fully enumerated right Cayley graph.
:rtype:
  WordGraph
)pbdoc");

    thing.def("size",
              &FroidurePinBase::size,
              R"pbdoc(
Returns the size of the semigroup represented by a FroidurePin instance.

:returns:
  The size of the semigroup.
:rtype:
  int

:complexity:
  At worst :math:`O(|S|n)` where :math:`S` is the semigroup represented
  by ``self``, and :math:`n` is the return value of
  :any:`FroidurePin.number_of_generators`.
)pbdoc");

    thing.def("suffix",
              &FroidurePinBase::suffix,
              py::arg("pos"),
              R"pbdoc(
Returns the position of the longest proper suffix.

Returns the position of the suffix of the element ``x`` in position *pos*
(of the semigroup) of length one less than the length of ``x``.

:param pos: the position.
:type pos: int

:returns:
  The position of the suffix of the element
:rtype: int

:raises LibsemigroupsError:
  if *pos* is greater than or equal to :any:`current_size`.

:complexity: Constant.
)pbdoc");
  }  // init_froidure_pin_base
}  // namespace libsemigroups
