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

// libsemigroups headers
#include <libsemigroups/froidure-pin-base.hpp>

// pybind11....
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_froidure_pin_base

namespace libsemigroups {
  namespace py = pybind11;

  void init_froidure_pin_base(py::module& m) {
    py::class_<FroidurePinBase, Runner> thing(m,
                                              "FroidurePinBase",
                                              R"pbdoc(
Base class for :any:`FroidurePin` containing non-element specific data and
member functions. :any:`FroidurePin` is an abstract class which cannot be
instantiated directly, only via the derived class :any:`FroidurePin`.)pbdoc");

    thing.def(
        "batch_size",
        [](FroidurePinBase const& self) { return self.batch_size(); },
        R"pbdoc(
:sig=(self: FroidurePin) -> int:

Returns the current value of the batch size.

This function returns the minimum number of elements enumerated in any call to
:any:`Runner.run`.

:returns: The current value of the batch size.
:rtype: int

:complexity: Constant.
)pbdoc");
    thing.def(
        "batch_size",
        [](FroidurePinBase& self, size_t val) -> FroidurePinBase& {
          return self.batch_size(val);
        },
        py::arg("val"),
        R"pbdoc(
:sig=(self: FroidurePin, val: int) -> FroidurePin:

Set a new value for the batch size.

The *batch size* is the number of new elements to be found by any call to
:any:`Runner.run`. This is used by, for example, :any:`FroidurePin.position` so
that it is possible to find the position of an element after only partially
enumerating the semigroup.The default value of the batch size is ``8192``.

:param val: the new value for the batch size.
:type val: int

:returns: *self*.
:rtype: FroidurePin

:complexity: Constant.
)pbdoc");

    thing.def("current_left_cayley_graph",
              &FroidurePinBase::current_left_cayley_graph,
              R"pbdoc(
:sig=(self: FroidurePin) -> WordGraph:

Returns the so-far enumerated left Cayley graph.

This function return the left Cayley graph of the semigroup as it has been
enumerated so-far. No enumeration is triggered by calls to this function.

:returns:
  The (possibly partially enumerated) left Cayley graph.
:rtype:
   WordGraph

:complexity:
  Constant.
)pbdoc",
              py::return_value_policy::reference_internal);

    thing.def("current_length",
              &FroidurePinBase::current_length,
              py::arg("pos"),
              R"pbdoc(
:sig=(self: FroidurePin, pos: int) -> int:

Returns the length of the short-lex least word equal to the element with
given index.

This function returns the length of the short-lex least word (in the
generators) equal to the element with index *pos*.

:param pos: the position.
:type pos: int

:returns: The length of the word equal to the element with index *pos*.
:rtype: int

:raises LibsemigroupsError:
  if *pos* is greater than or equal to :any:`current_size`.

:complexity: Constant.

.. seealso::  :any:`length`.
)pbdoc");

    thing.def("current_max_word_length",
              &FroidurePinBase::current_max_word_length,
              R"pbdoc(
:sig=(self: FroidurePin) -> int:

Returns the maximum length of a word in the generators so far computed. Every
element of the semigroup can be expressed as the short-lex least product of the
generators that equals that element. This function returns the length of the
longest short-lex least word in the generators that has so far been enumerated.

:returns:
  The maximum length of a word so-far enumerated.
:rtype:
  int

:complexity: Constant.
)pbdoc");

    thing.def("current_number_of_rules",
              &FroidurePinBase::current_number_of_rules,
              R"pbdoc(
:sig=(self: FroidurePin) -> int:

Returns the number of relations that have been found so far. This is
only guaranteed to be the actual number of relations in a presentation
defining the semigroup if the semigroup is fully enumerated.

:returns:
  The number of rules so-far enumerated.
:rtype:
   int

:complexity:
  Constant.
)pbdoc");

    thing.def("current_right_cayley_graph",
              &FroidurePinBase::current_right_cayley_graph,
              R"pbdoc(
:sig=(self: FroidurePin) -> WordGraph:

Returns the so-far enumerated right Cayley graph.

This function return the right Cayley graph of the semigroup as it has been
enumerated so-far. No enumeration is triggered by calls to this function.

:returns:
  The (possibly partially enumerated) left Cayley graph.
:rtype:
  WordGraph

:complexity: Constant.
)pbdoc",
              py::return_value_policy::reference_internal);

    thing.def("current_size",
              &FroidurePinBase::current_size,
              R"pbdoc(
:sig=(self: FroidurePin) -> int:

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
:sig=(self: FroidurePin) -> int:

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
:sig=(self: FroidurePin, limit: int) -> None:

Enumerate until at least a specified number of elements are found.

If the semigroup is already fully enumerated, or the number of elements
previously enumerated exceeds *limit*, then calling this function does
nothing. Otherwise, :any:`Runner.run` attempts to find at least the maximum of
*limit* and :any:`batch_size` elements of the semigroup.

:param limit: the limit.
:type limit: int

:complexity:
  At worst :math:`O(mn)` where :math:`m` equals *limit* and :math:`n` is the
  return value of :any:`FroidurePin.number_of_generators`.)pbdoc");

    thing.def("final_letter",
              &FroidurePinBase::final_letter,
              py::arg("pos"),
              R"pbdoc(
:sig=(self: FroidurePin, pos: int) -> int:

Returns the last letter of the element with specified index.

This function returns the final letter of the element in position *pos*
of the semigroup, which is the index of the generator corresponding to the
final letter of the element.

:param pos: the position.
:type pos: int

:returns:
  The last letter in the minimal factorisation of the element with given
  position.
:rtype: int

:raises LibsemigroupsError:
  if *pos* is greater than or equal to :any:`current_size`.

:complexity: Constant.
)pbdoc");

    thing.def("first_letter",
              &FroidurePinBase::first_letter,
              py::arg("pos"),
              R"pbdoc(
:sig=(self: FroidurePin, pos: int) -> int:

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

    thing.def("contains_one",
              &FroidurePinBase::contains_one,
              R"pbdoc(
:sig=(self: FroidurePin) -> bool:

Check if the categorical multiplicative identity is an element.

:returns:
  Whether or not the one of any of the elements belongs to the semigroup.
:rtype:
  bool

:complexity:
  At worst :math:`O(|S|n)` where :math:`S` is the semigroup represented
  by *self*, and :math:`n` is the return value of
  :any:`FroidurePin.number_of_generators`.
)pbdoc");

    thing.def("currently_contains_one",
              &FroidurePinBase::currently_contains_one,
              R"pbdoc(
:sig=(self: FroidurePin) -> bool:

Check if the categorical multiplicative identity is an element.

:returns:
  Whether or not the one of any of the elements belongs to the semigroup.
:rtype:
  bool

:complexity:
  At worst :math:`O(|S|n)` where :math:`S` is the semigroup represented
  by *self*, and :math:`n` is the return value of
  :any:`FroidurePin.number_of_generators`.
)pbdoc");

    thing.def("left_cayley_graph",
              &FroidurePinBase::left_cayley_graph,
              R"pbdoc(
:sig=(self: FroidurePin) -> WordGraph:

Returns the left Cayley graph.

This function triggers a full enumeration, and then returns the left Cayley
graph of the semigroup represented by a :any:`FroidurePin` instance.

:returns:
  The fully enumerated left Cayley graph.
:rtype:
  WordGraph

:complexity:
  At worst :math:`O(|S|n)` where :math:`S` is the semigroup represented
  by *self*, and :math:`n` is the return value of
  :any:`FroidurePin.number_of_generators`.
)pbdoc",
              py::return_value_policy::reference_internal);
    thing.def("length",
              &FroidurePinBase::length,
              py::arg("pos"),
              R"pbdoc(
:sig=(self: FroidurePin, pos: int) -> int:

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
        "number_of_elements_of_length",
        [](FroidurePinBase const& self, size_t len) {
          return self.number_of_elements_of_length(len);
        },
        py::arg("len"),
        R"pbdoc(
:sig=(self: FroidurePin, len: int) -> int:

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
:sig=(self: FroidurePin, min: int, max: int) -> int:

Returns the number of elements so far enumerated with length in a given
range.

This function returns the number of elements that have been enumerated so
far with length in the range :math:`[min, max)`. This function does not
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
:sig=(self: FroidurePin) -> int:

Returns the total number of relations in a presentation defining the semigroup.
This function triggers a full enumeration of the semigroup.

:returns: The number of rules so-far found.
:rtype: int

:complexity:
  At worst :math:`O(|S|n)` where :math:`S` is the semigroup represented by
  *self*, and :math:`n` is the return value of
  :any:`FroidurePin.number_of_generators`.
)pbdoc");

    thing.def("prefix",
              &FroidurePinBase::prefix,
              py::arg("pos"),
              R"pbdoc(
:sig=(self: FroidurePin, pos: int) -> int:

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
:sig=(self: FroidurePin) -> WordGraph:

Returns the fully enumerated right Cayley graph.

:returns:
  The fully enumerated right Cayley graph.
:rtype:
  WordGraph

:complexity:
  At worst :math:`O(|S|n)` where :math:`S` is the semigroup represented
  by *self*, and :math:`n` is the return value of
  :any:`FroidurePin.number_of_generators`.

)pbdoc",
              py::return_value_policy::reference_internal);

    thing.def("size",
              &FroidurePinBase::size,
              R"pbdoc(
:sig=(self: FroidurePin) -> int:

Returns the size of the semigroup represented by a :any:`FroidurePin` instance.

:returns:
  The size of the semigroup.
:rtype:
  int

:complexity:
  At worst :math:`O(|S|n)` where :math:`S` is the semigroup represented
  by *self*, and :math:`n` is the return value of
  :any:`FroidurePin.number_of_generators`.
)pbdoc");

    thing.def("suffix",
              &FroidurePinBase::suffix,
              py::arg("pos"),
              R"pbdoc(
:sig=(self: FroidurePin, pos: int) -> int:

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

    ////////////////////////////////////////////////////////////////////////
    // Helper functions
    ////////////////////////////////////////////////////////////////////////

    {
      py::options options;
      options.disable_function_signatures();

      m.def("froidure_pin_product_by_reduction",
            &froidure_pin::product_by_reduction,
            py::arg("fp"),
            py::arg("i"),
            py::arg("j"),
            R"pbdoc(
:sig=(fp: FroidurePin, i: int, j: int) -> int:

Compute a product using the Cayley graph.

This function finds the product of ``fp[i]`` and ``fp[j]`` by
following the path in the right Cayley graph from *i* labelled by
the word ``froidure_pin.minimal_factorisation(fp, j)`` or, if
``froidure_pin.minimal_factorisation(fp, i)`` is shorter, by following the
path in the left Cayley graph from *j* labelled by
``froidure_pin.minimal_factorisation(fp, i)``.

:param fp:
  the :any:`FroidurePin` object.
:type fp:
  FroidurePin

:param i:
  the index of an element.
:type i:
  int

:param j:
  another index of an element.
:type j:
  int

:returns:
   The index of the product.
:rtype:
   int

:raises LibsemigroupsError:
   if *i* or *j* is greater than or equal to
   :any:`FroidurePin.current_size`.

:complexity:
   :math:`O(n)` where :math:`n` is the minimum of the lengths of
   ``minimal_factorisation(fp, i)`` and ``minimal_factorisation(fp, j)``.
)pbdoc");

      m.def(
          "froidure_pin_current_minimal_factorisation",
          [](FroidurePinBase const&              fp,
             FroidurePinBase::element_index_type pos) {
            return froidure_pin::current_minimal_factorisation(fp, pos);
          },
          py::arg("fp"),
          py::arg("pos"),
          R"pbdoc(
:sig=(fp: FroidurePin, pos: int) -> list[int]:

Returns the short-lex least word representing an element given by index.

This function returns the short-lex least word (in the generators) representing the element in *fp* with index *pos*.

:param fp:
   the :any:`FroidurePin` object.
:type fp:
   FroidurePin

:param pos: the index of the element whose factorisation is sought.
:type pos: int

:returns: A minimal factorisation of the element with index *pos*.
:rtype: list[int]

:raises LibsemigroupsError:
    if *pos* is not strictly less than :any:`FroidurePin.current_size`.

:complexity:
  At worst :math:`O(mn)` where :math:`m` equals *pos* and
  :math:`n` is the return value of :any:`FroidurePin.number_of_generators`.

.. note::
    No enumeration is triggered by calling this function.
)pbdoc");

      // The following function is documented in src/froidure-pin.cpp
      m.def(
          "froidure_pin_minimal_factorisation",
          [](FroidurePinBase& fp, FroidurePinBase::element_index_type pos) {
            return froidure_pin::minimal_factorisation(fp, pos);
          },
          py::arg("fp"),
          py::arg("pos"));

      m.def(
          "froidure_pin_current_normal_forms",
          [](FroidurePinBase const& fp) {
            return py::make_iterator(fp.cbegin_current_normal_forms(),
                                     fp.cend_current_normal_forms());
          },
          R"pbdoc(
:sig=(fp: FroidurePin) -> collections.abc.Iterator[list[int]]:

Returns an iterator yielding the so-far enumerated normal forms (if any).

This function returns an iterator yielding the normal forms of the semigroup
represented by *fp* instance (if any). This function does not perform any
enumeration of *fp*. If you want to obtain the
complete set of rules, then use :any:`normal_forms` instead.

:param fp:
   the :any:`FroidurePin` object.
:type fp:
   FroidurePin

:returns:
  An iterator yielding a ``list[int]``.
:rtype:
  collections.abc.Iterator[list[int]]

:complexity:
  Constant.
)pbdoc");

      m.def(
          "froidure_pin_current_rules",
          [](FroidurePinBase const& fp) {
            return py::make_iterator(fp.cbegin_current_rules(),
                                     fp.cend_current_rules());
          },
          R"pbdoc(
:sig=(fp: FroidurePin) -> collections.abc.Iterator[tuple[list[int], list[int]]]:

Returns an iterator yielding the so-far enumerated rules.

This function returns an iterator yielding the rules in a confluent terminating
rewriting system defining a semigroup isomorphic to the one defined by
*fp*. This function does not perform any enumeration of *fp*. If you want
to obtain the complete set of rules, then use :any:`rules` instead.

:param fp:
   the :any:`FroidurePin` object.
:type fp:
   FroidurePin

:returns:
    An iterator.
:rtype: collections.abc.Iterator[tuple[list[int], list[int]]]

:complexity: Constant

.. code-block:: python

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
    [([0, 0], [0]),
     ([0, 1], [1]),
     ([0, 2], [2]),
     ([0, 3], [3]),
     ([1, 0], [0]),
     ([1, 1], [1]),
     ([1, 2], [2]),
     ([1, 3], [3]),
     ([2, 0], [0]),
     ([2, 1], [1]),
     ([2, 2], [2]),
     ([2, 3], [3]),
     ([3, 0], [0]),
     ([3, 1], [1]),
     ([3, 2], [2]),
     ([3, 3], [3])]
)pbdoc");

      m.def(
          "froidure_pin_normal_forms",
          [](FroidurePinBase& fp) {
            return py::make_iterator(fp.cbegin_normal_forms(),
                                     fp.cend_normal_forms());
          },
          R"pbdoc(
:sig=(fp: FroidurePin) -> collections.abc.Iterator[list[int]]:

Returns an iterator yielding normal forms. This function returns an iterator
yielding normal forms for the elements of the semigroup represented by
*fp* instance. This function performs a full enumeration of *fp*. If you
want to obtain the current normal forms without triggering an enumeration,
then use :any:`current_normal_forms` instead.

:param fp:
   the :any:`FroidurePin` object.
:type fp:
   FroidurePin

:returns:
  An iterator of normal forms.
:rtype:
  collections.abc.Iterator[list[int]]
)pbdoc");

      m.def(
          "froidure_pin_rules",
          [](FroidurePinBase& fp) {
            return py::make_iterator(fp.cbegin_rules(), fp.cend_rules());
          },
          R"pbdoc(
:sig=(fp: FroidurePin) -> collections.abc.Iterator[tuple[list[int], list[int]]]:

Returns an iterator yielding the rules.

This function returns an iterator yielding the rules in a confluent terminating
rewriting system defining a semigroup isomorphic to the one defined by
*fp*. This function performs a full enumeration of *fp* If you want to
obtain the current set of rules without triggering any enumeration, then
use :any:`current_rules` instead.

:param fp:
   the :any:`FroidurePin` object.
:type fp:
   FroidurePin

:returns:
    An iterator yielding rules.
:rtype:
    collections.abc.Iterator[tuple[list[int], list[int]]]
)pbdoc");
    }
  }  // init_froidure_pin_base
}  // namespace libsemigroups
