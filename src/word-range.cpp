//
// libsemigroups - C++ library for semigroups and monoids
// Copyright (C) 2021-2024 James D. Mitchell
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
#include <initializer_list>  // for initializer_list
#include <iosfwd>            // for string
#include <vector>            // for vector

// libsemigroups....

#include <libsemigroups/exception.hpp>   // for LIBSEMIGROUPS_EXCEPTION
#include <libsemigroups/paths.hpp>       // for Path
#include <libsemigroups/types.hpp>       // for word_type
#include <libsemigroups/word-range.hpp>  // for number_of_words

// pybind11....
#include <pybind11/complex.h>
#include <pybind11/pybind11.h>  // for make_iterator, module
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_words

namespace libsemigroups {
  namespace py = pybind11;

  using size_type = typename std::vector<word_type>::size_type;

  void init_words(py::module& m) {
    ////////////////////////////////////////////////////////////////////////////
    // WordRange
    ////////////////////////////////////////////////////////////////////////////

    py::class_<WordRange> thing1(m,
                                 "WordRange",
                                 R"pbdoc(
Class for generating words in a given range and in a particular order.

The ``WordRange`` class is a class for generating words (i.e. lists of ``int``
values) in a given range and in a particular order.

The order and range of the words in a ``WordRange`` instance can be set using
the member functions:

* :any:`WordRange.order`
* :any:`WordRange.alphabet_size`
* :any:`WordRange.min`
* :any:`WordRange.max`
* :any:`WordRange.first`
* :any:`WordRange.last`

.. seealso::
    :any:`StringRange`

Example
-------

.. doctest:: python

  >>> from libsemigroups_pybind11 import WordRange, Order
  >>> words = WordRange();
  >>> words.order(Order.shortlex) \
  ...      .alphabet_size(2) \
  ...      .min(1) \
  ...      .max(5)
  <WordRange of length 30 with letters in [0, 2) in shortlex order>
  >>> words.order()
  <Order.shortlex: 1>

  >>> from libsemigroups_pybind11 import ToString
  >>> words.alphabet_size(1).min(0).max(10)
  <WordRange of length 10 with letters in [0, 1) in shortlex order>

  >>> strings = words | ToString("a")
  >>> list(strings)
  ['', 'a', 'aa', 'aaa', 'aaaa', 'aaaaa', 'aaaaaa', 'aaaaaaa', 'aaaaaaaa', 'aaaaaaaaa']
)pbdoc");
    thing1.def("__repr__",
               [](WordRange const& wr) { return to_human_readable_repr(wr); });
    thing1.def("__copy__",
               [](WordRange const& self) { return WordRange(self); });
    thing1.def(
        "copy",
        [](WordRange const& self) { return WordRange(self); },
        R"pbdoc(
:sig=(self: WordRange) -> WordRange:

Copy a :any:`WordRange` object.

:returns: A copy.
:rtype: WordRange
)pbdoc");
    // TODO(now) Should we hide the return type of this?
    thing1.def(
        "__or__",
        [](WordRange const& w, ToString const& to_str) {
          if (!to_str.can_convert_letter(w.alphabet_size() - 1)) {
            LIBSEMIGROUPS_EXCEPTION(
                "expected the alphabet size ({}) of the ToString object "
                "to be >= the alphabet size ({}) of the WordRange object",
                to_str.alphabet().size(),
                w.alphabet_size());
          }
          using rx::operator|;
          return w | to_str;
        },
        R"pbdoc(
For converting from ``list[int]`` to :any:`str`.

This allows the pipe operator ``|`` to be used with a :any:`ToString` object to
convert the output of a :any:`WordRange` object strings.
)pbdoc");
    thing1.def("__len__", &WordRange::count);
    thing1.def(
        "__iter__",
        [](WordRange const& w) {
          return py::make_iterator(rx::begin(w), rx::end(w));
        },
        py::keep_alive<0, 1>());
    thing1.def(py::init<>(), R"pbdoc(
Default constructor.

Constructs an empty range with:

*  :any:`WordRange.min()` equal to ``0`` ;
*  :any:`WordRange.max()` equal to ``0`` ;
*  :any:`WordRange.order()` equal to :any:`Order.shortlex` ;
*  :any:`WordRange.first()` equal to the empty word;
*  :any:`WordRange.last()` equal to the empty word;
*  :any:`WordRange.upper_bound()` equal to ``0`` ;
*  :any:`WordRange.alphabet_size()` equal to ``0``.

)pbdoc");

    thing1.def("at_end",
               &WordRange::at_end,
               R"pbdoc(
Check if the range object is exhausted.

Returns ``True`` if a :any:`WordRange` object is exhausted, and ``False`` if not.

:returns: Whether the range object is exhausted.
:rtype: bool
)pbdoc");
    thing1.def("count",
               &WordRange::count,
               R"pbdoc(
The actual size of the range.

Returns the number of words in a :any:`WordRange` object. If :any:`WordRange.order()` is
:any:`Order.shortlex` , then :any:`WordRange.size_hint()` is used. If :any:`WordRange.order()` is
not :any:`Order.shortlex`, then a copy of the range may have to be looped over
in order to find the return value of this function.

:returns: The size of the range.
:rtype: int
)pbdoc");
    thing1.def(
        "first",
        [](WordRange const& self) { return self.first(); },
        R"pbdoc(
The current first word in the range.

Returns the first word in a :any:`WordRange` object.

:returns: The first word in the range.
:rtype: list[int]

.. seealso::
    :any:`WordRange.min`
)pbdoc");
    thing1.def(
        "first",
        [](WordRange& self, word_type const& frst) -> WordRange& {
          return self.first(frst);
        },
        py::arg("frst"),
        R"pbdoc(
:sig=(self: WordRange, frst: list[int]) -> WordRange:

Set the first word in the range.

Sets the first word in a :any:`WordRange` object to be *frst*. This function
performs no checks on its arguments. If *frst* contains letters greater than
:any:`WordRange.alphabet_size()`, then the :any:`WordRange` object will be empty.
Similarly, if :any:`WordRange.first()` is greater than :any:`WordRange.last()` with respect to :any:`WordRange.order()`,
then the object will be empty.

:param frst: the first word.
:type frst: list[int]

:returns: *self*.
:rtype: WordRange

.. seealso::  :any:`WordRange.min`
)pbdoc");
    thing1.def("get",
               &WordRange::get,
               R"pbdoc(
Get the current value.

Returns the current word in a :any:`WordRange` object.

:returns: The current word.
:rtype: list[int]
)pbdoc");
    thing1.def(
        "init",
        [](WordRange& self) -> WordRange& { return self.init(); },
        R"pbdoc(
Initialize an existing :any:`WordRange` object.

This function puts a :any:`WordRange` object back into the same state as if it
had been newly default constructed, and returns that object.

:returns: *self*.
:rtype: WordRange
)pbdoc");
    thing1.def(
        "last",
        [](WordRange const& self) { return self.last(); },
        R"pbdoc(
Returns the one past the last word in a :any:`WordRange` object.

:returns: One past the last word.
:rtype: list[int]

.. seealso::
    :any:`WordRange.max`
)pbdoc");
    thing1.def(
        "last",
        [](WordRange& self, word_type const& lst) -> WordRange& {
          return self.last(lst);
        },
        py::arg("lst"),
        R"pbdoc(
:sig=(self: WordRange, lst: list[int]) -> WordRange:

Set one past the last word in the range.

Sets one past the last word in a :any:`WordRange` object to be ``lst``. This
function performs no checks on its arguments. If ``lst`` contains letters
greater than :any:`WordRange.alphabet_size()`, then the :any:`WordRange` object will be empty.

:param lst: one past the last word.
:type lst: list[int]

:returns: *self*.
:rtype: WordRange

.. seealso::
    :any:`WordRange.max`
)pbdoc");
    thing1.def(
        "max",
        [](WordRange& self, size_t val) -> WordRange& { return self.max(val); },
        py::arg("val"),
        R"pbdoc(
:sig=(self: WordRange, val: int) -> WordRange:

Set one past the last word in the range by length.

Sets one past the last word in a :any:`WordRange` object to be
``words.pow(0_w, val)`` (the word consisting of *val* letters equal to ``0`` ).

:param val: the exponent.
:type val: int

:returns: *self*.
:rtype: WordRange
)pbdoc");
    thing1.def(
        "min",
        [](WordRange& self, size_t val) -> WordRange& { return self.min(val); },
        py::arg("val"),
        R"pbdoc(
:sig=(self: WordRange, val: int) -> WordRange:

Set the first word in the range by length.

Sets the first word in a :any:`WordRange` object to be ``pow(0_w, val)``
(the word consisting of *val* letters equal to ``0`` ).

:param val: the exponent.
:type val: int

:returns: *self*.
:rtype: WordRange
)pbdoc");
    thing1.def("next",
               &WordRange::next,
               R"pbdoc(
Advance to the next value.

Advances a :any:`WordRange` object to the next value (if any).

.. seealso::  :any:`WordRange.at_end`
)pbdoc");
    thing1.def(
        "alphabet_size",
        [](WordRange const& self) { return self.alphabet_size(); },
        R"pbdoc(
The current number of letters in the alphabet.

Returns the current number of letters in a :any:`WordRange` object.

:returns: The current number of letters.
:rtype: int
)pbdoc");
    thing1.def(
        "alphabet_size",
        [](WordRange& self, size_type n) -> WordRange& {
          return self.alphabet_size(n);
        },
        py::arg("n"),
        R"pbdoc(
:sig=(self: WordRange, n: int) -> WordRange:

Set the number of letters in the alphabet.

Sets the number of letters in a :any:`WordRange` object to *n*.

:param n: the number of letters.
:type n: int

:returns: *self*.
:rtype: WordRange
)pbdoc");
    thing1.def(
        "order",
        [](WordRange const& self) { return self.order(); },
        R"pbdoc(
The current order of the words in the range.

Returns the current order of the words in a :any:`WordRange` object.

:returns: The current order.
:rtype: Order
)pbdoc");
    thing1.def(
        "order",
        [](WordRange& self, Order val) -> WordRange& {
          return self.order(val);
        },
        py::arg("val"),
        R"pbdoc(
Set the order of the words in the range.

Sets the order of the words in a :any:`WordRange` object to *val*.

:param val: the order.
:type val: Order

:returns: *self*.
:rtype: WordRange

:raises LibsemigroupsError:  if val is not :any:`Order.shortlex` or :any:`Order.lex`.
)pbdoc");
    thing1.def("size_hint",
               &WordRange::size_hint,
               R"pbdoc(
The possible size of the range.

Returns the number of words in a :any:`WordRange` object if
:any:`WordRange.order()` is :any:`Order.shortlex`. If :any:`WordRange.order()`
is not :any:`Order.shortlex`, then the return value of this function is
meaningless.

:returns: A value of type ``int``.
:rtype: int
)pbdoc");
    thing1.def(
        "upper_bound",
        [](WordRange const& self) { return self.upper_bound(); },
        R"pbdoc(
The current upper bound on the length of a word in the range.

Returns the current upper bound on the length of a word in a :any:`WordRange`
object. This setting is only used if :any:`WordRange.order()` is :any:`Order.lex`.

:returns: A value of type :any:`int`.
:rtype: int
)pbdoc");
    thing1.def(
        "upper_bound",
        [](WordRange& self, size_type n) -> WordRange& {
          return self.upper_bound(n);
        },
        py::arg("n"),
        R"pbdoc(
:sig=(self: WordRange, n: int) -> WordRange:

Set an upper bound for the length of a word in the range.

Sets an upper bound for the length of a word in a :any:`WordRange` object.
This setting is only used if :any:`WordRange.order()` is :any:`Order.lex`.

:param n: the upper bound.
:type n: int

:returns: *self*.
:rtype: WordRange
)pbdoc");
    thing1.def("valid",
               &WordRange::valid,
               R"pbdoc(
Returns whether or not the settings have been changed since the last time either next or get has been called.

Other than by calling :any:`WordRange.next()` , the value returned by :any:`WordRange.get()` may be
altered by a call to one of the following:

*  ``order(Order)``
*  ``alphabet_size(int)``
*  ``min(int)``
*  ``max(int)``
*  ``first(list[int])``
*  ``last(list[int])``
*  ``upper_bound(int)``

This function returns ``True`` if none of the above settings have been changed
since the last time :any:`WordRange.next()` or :any:`WordRange.get()` is called, and ``False`` otherwise.

:returns: Whether or not the settings have been changed.
:rtype: bool
)pbdoc");

    ////////////////////////////////////////////////////////////////////////////
    // StringRange
    ////////////////////////////////////////////////////////////////////////////
    py::class_<StringRange> thing2(m,
                                   "StringRange",
                                   R"pbdoc(
Class for generating strings in a given range and in a particular order.

The ``StringRange`` class implements a range object for strings and produces the
same output as ``WordRange() | ToString("ab")`` , but is more convenient in some
cases.

The order and range of the words in a :any:`StringRange` instance can be set
using the member functions:

* :any:`StringRange.order`
* :any:`StringRange.alphabet`
* :any:`StringRange.min`
* :any:`StringRange.max`
* :any:`StringRange.first`
* :any:`StringRange.last`

.. seealso::
    :any:`WordRange`

Example
-------

.. doctest:: python

  >>> from libsemigroups_pybind11 import StringRange, Order
  >>> strings = StringRange();
  >>> strings.order(Order.shortlex) \
  ...        .alphabet("ba") \
  ...        .min(1) \
  ...        .max(5)
  <StringRange of length 30 in shortlex order>
  >>> strings.order()
  <Order.shortlex: 1>

  >>> from libsemigroups_pybind11 import ToWord
  >>> strings.alphabet("a").min(0).max(10)
  <StringRange of length 10 in shortlex order>

  >>> wrds = strings | ToWord("a")
  >>> list(wrds)
  [[], [0], [0, 0], [0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0, 0, 0, 0]]


)pbdoc");
    thing2.def("__repr__", [](StringRange const& sr) {
      return to_human_readable_repr(sr);
    });
    thing2.def("__copy__",
               [](StringRange const& self) { return StringRange(self); });
    thing2.def(
        "copy",
        [](StringRange const& self) { return StringRange(self); },
        R"pbdoc(
:sig=(self: StringRange) -> StringRange:

Copy a :any:`StringRange` object.

:returns: A copy.
:rtype: StringRange
)pbdoc");

    thing2.def(
        "__or__",
        [](StringRange const& sr, ToWord const& to_wrd) {
          if (!std::all_of(sr.alphabet().begin(),
                           sr.alphabet().end(),
                           [&to_wrd](auto& c) {
                             return to_wrd.can_convert_letter(c);
                           })) {
            LIBSEMIGROUPS_EXCEPTION(
                "expected every letter of the alphabet (\"{}\") of "
                "the "
                "StringRange object to belong to the alphabet "
                "(\"{}\") of the "
                "ToWord object",
                sr.alphabet(),
                to_wrd.alphabet());
          }
          using rx::operator|;
          return sr | to_wrd;
        },
        R"pbdoc(
For converting from to :any:`str` to ``list[int]``.

This allows the pipe operator ``|`` to be used with a :any:`ToWord` object to
convert the output of a :any:`StringRange` object list[int].

)pbdoc");
    thing2.def("__len__", &StringRange::count);
    thing2.def(
        "__iter__",
        [](StringRange const& s) {
          return py::make_iterator(rx::begin(s), rx::end(s));
        },
        py::keep_alive<0, 1>());
    thing2.def(py::init<>(), R"pbdoc(
Default constructor.
Constructs an empty range with:

*  :any:`StringRange.min()` equal to ``0`` ;
*  :any:`StringRange.max()` equal to ``0`` ;
*  :any:`StringRange.order()` equal to :any:`Order.shortlex` ;
*  :any:`StringRange.first()` equal to the empty string;
*  :any:`StringRange.last()` equal to the empty string;
*  :any:`StringRange.upper_bound()` equal to ``0`` ;
*  :any:`StringRange.alphabet()` equal to the empty string.

)pbdoc");
    thing2.def(
        "alphabet",
        [](StringRange const& self) { return self.alphabet(); },
        R"pbdoc(
The current alphabet.

Returns the current alphabet in a :any:`StringRange` object.

:returns: The current alphabet.
:rtype: str
)pbdoc");
    thing2.def(
        "alphabet",
        [](StringRange& self, std::string const& x) -> StringRange& {
          return self.alphabet(x);
        },
        py::arg("x"),
        R"pbdoc(
Set the alphabet.

Sets the alphabet in a :any:`StringRange` object.

:param x: the alphabet.
:type x: str

:returns: *self*.
:rtype: StringRange

:raises LibsemigroupsError:  if *x* contains repeated letters.
)pbdoc");
    thing2.def("at_end",
               &StringRange::at_end,
               R"pbdoc(
Check if the range object is exhausted.

Returns ``True`` if a :any:`StringRange` object is exhausted, and ``False`` if not.

:returns: Whether the object is exhausted.
:rtype: bool
)pbdoc");
    thing2.def(
        "__iter__",
        [](StringRange const& self) {
          return py::make_iterator(self.begin(), self.end());
        },
        py::keep_alive<0, 1>(),
        R"pbdoc(
Returns an iterator over the range.

This function returns an iterator yielding the strings in a :any:`StringRange`
object.

:returns: An input iterator.

.. seealso::
    :any:`end`.

)pbdoc");
    thing2.def("count",
               &StringRange::count,
               R"pbdoc(
The actual size of the range.

Returns the number of strings in the range object. If :any:`StringRange.order()`
is :any:`Order.shortlex` , then :any:`StringRange.size_hint()` is used. If
:any:`order()` is not :any:`Order.shortlex` , then a copy of the range may have
to be looped over in order to find the return value of this function.

:returns: The size of the range.
:rtype: int
)pbdoc");
    thing2.def(
        "first",
        [](StringRange const& self) { return self.first(); },
        R"pbdoc(
The current first string in the range.

Returns the first string in a :any:`StringRange` object.

:returns: The first string.
:rtype: str

.. seealso::
    :any:`min`

)pbdoc");
    thing2.def(
        "first",
        [](StringRange& self, std::string const& frst) -> StringRange& {
          return self.first(frst);
        },
        py::arg("frst"),
        R"pbdoc(
Set the first string in the range.

Sets the first string in a :any:`StringRange` object to be *frst*.

:param frst: the first string.
:type frst: str

:returns: *self*.
:rtype: StringRange

.. seealso::  :any:`min`
)pbdoc");
    thing2.def("get",
               &StringRange::get,
               R"pbdoc(
Get the current value.

Returns the current string in a :any:`StringRange` object.

:returns: The current value.
:rtype: str
)pbdoc");
    thing2.def(
        "init",
        [](StringRange& self) -> StringRange& { return self.init(); },
        R"pbdoc(
Initialize an existing :any:`StringRange` object.

This function puts a :any:`StringRange` object back into the same state as if it
had been newly default constructed.

:returns: *self*.
:rtype: StringRange
)pbdoc");
    thing2.def(
        "last",
        [](StringRange const& self) { return self.last(); },
        R"pbdoc(
The current one past the last string in the range.

Returns one past the last string in a :any:`StringRange` object.

:returns: One past the last string.
:rtype: str

.. seealso::
    :any:`max`
)pbdoc");
    thing2.def(
        "last",
        [](StringRange& self, std::string const& lst) -> StringRange& {
          return self.last(lst);
        },
        py::arg("lst"),
        R"pbdoc(
Set one past the last string in the range.

Sets one past the last string in a :any:`StringRange` object to be *lst*.

:param lst: the first string.
:type lst: str

:returns: *self*.
:rtype: StringRange

:raises LibsemigroupsError: if *lst* contains letters not belonging to :any:`StringRange.alphabet()`.

.. seealso::
    :any:`max`
)pbdoc");
    thing2.def(
        "max",
        [](StringRange& self, size_t val) -> StringRange& {
          return self.max(val);
        },
        py::arg("val"),
        R"pbdoc(
:sig=(self: StringRange, val: int) -> StringRange:

Set one past the last string in the range by length.

Sets one past the last string in a :any:`StringRange` object to be
:math:`\alpha^n` where :math:`\alpha` is the first letter of
:any:`StringRange.alphabet` (or ``"a"`` if the alphabet is empty) and :math:`n`
corresponds to *val*.

:param val: the exponent.
:type val: int

:returns: *self*.
:rtype: StringRange
)pbdoc");
    thing2.def(
        "min",
        [](StringRange& self, size_t val) -> StringRange& {
          return self.min(val);
        },
        py::arg("val"),
        R"pbdoc(
:sig=(self: StringRange, val: int) -> StringRange:

Set the first string in the range by length.

Sets the first string in a :any:`StringRange` object to be
:math:`\alpha^n` where :math:`\alpha` is the first letter of
:any:`StringRange.alphabet` (or ``"a"`` if the alphabet is empty) and :math:`n`
corresponds to *val*.

:param val: the exponent.
:type val: int

:returns: *self*.
:rtype: StringRange
)pbdoc");
    thing2.def("next",
               &StringRange::next,
               R"pbdoc(
Advance to the next value.

Advances a :any:`StringRange` object to the next value (if any).

.. seealso::
    :any:`at_end`
)pbdoc");
    thing2.def(
        "order",
        [](StringRange const& self) { return self.order(); },
        R"pbdoc(
The current order of the strings in the range.

Returns the current order of the strings in a :any:`StringRange` object.

:returns: The current order.
:rtype: Order
)pbdoc");
    thing2.def(
        "order",
        [](StringRange& self, Order val) -> StringRange& {
          return self.order(val);
        },
        py::arg("val"),
        R"pbdoc(
Set the order of the strings in the range.

Sets the order of the strings in a :any:`StringRange` object to *val*.

:param val: the order.
:type val: Order

:returns: *self*.
:rtype: StringRange

:raises LibsemigroupsError:  if *val* is not :any:`Order.shortlex` or :any:`Order.lex`.
)pbdoc");
    thing2.def("size_hint",
               &StringRange::size_hint,
               R"pbdoc(
The possible size of the range.

Returns the number of words in a :any:`StringRange` object if
:any:`StringRange.order()` is :any:`Order.shortlex`. If :any:`order()` is not
:any:`Order.shortlex` , then the return value of this function is meaningless.

:returns: A value of type ``int``.
:rtype: int
)pbdoc");
    thing2.def(
        "upper_bound",
        [](StringRange const& self) { return self.upper_bound(); },
        R"pbdoc(
The current upper bound on the length of a string in the range.

Returns the current upper bound on the length of a string in a
:any:`StringRange` object. This setting is only used if :any:`StringRange.order()` is
:any:`Order.lex`.

:returns: A value of type :any:`int`.
:rtype: int
)pbdoc");
    thing2.def(
        "upper_bound",
        [](StringRange& self, size_type n) -> StringRange& {
          return self.upper_bound(n);
        },
        py::arg("n"),
        R"pbdoc(
:sig=(self: StringRange, n: int) -> StringRange:

Set an upper bound for the length of a string in the range.

Sets an upper bound for the length of a string in a :any:`StringRange` object.
This setting is only used if :any:`StringRange.order()` is :any:`Order.lex`.

:param n: the upper bound.
:type n: int

:returns: *self*.
:rtype: StringRange
)pbdoc");

    ////////////////////////////////////////////////////////////////////////////
    // ToWord
    ////////////////////////////////////////////////////////////////////////////

    py::class_<ToWord> thing3(m,
                              "ToWord",
                              R"pbdoc(
Class for converting strings to ``list[int]`` with specified alphabet.

An instance of this class is callable and used to convert from :any:`str` to
``list[int]``. The characters in the string are converted to integers according
to their position in the alphabet used to construct a :any:`ToWord` instance if
one is provided, or using :any:`words.human_readable_index` otherwise.

.. doctest::

  >>> from libsemigroups_pybind11 import ToWord
  >>> to_word = ToWord("bac")
  >>> to_word("bac")
  [0, 1, 2]
  >>> to_word("bababbbcbc")
  [0, 1, 0, 1, 0, 0, 0, 2, 0, 2]

  >>> to_word.init()    # clear the alphabet
  <ToWord object with alphabet "">
  >>> to_word("bac")
  [1, 0, 2]

)pbdoc");
    thing3.def("__repr__",
               [](ToWord const& twrd) { return to_human_readable_repr(twrd); });
    thing3.def(py::init<>(), R"pbdoc(
Default constructor.

Constructs an empty object with no alphabet set.
)pbdoc");
    thing3.def(py::init<std::string const&>(), py::arg("alphabet"), R"pbdoc(
Construct with given alphabet.

Construct a :any:`ToWord` object with the given alphabet.

:param alphabet: the alphabet.
:type alphabet: str

:raises LibsemigroupsError:  if there are repeated letters in *alphabet*.
)pbdoc");
    thing3.def(
        "copy",
        [](ToWord const& self) { return ToWord(self); },
        R"pbdoc(
:sig=(self: ToWord) -> ToWord:

Copy a :any:`ToWord` object.

:returns: A copy.
:rtype: ToWord
)pbdoc");
    thing3.def("__copy__", [](ToWord const& self) { return ToWord(self); });
    thing3.def("empty",
               &ToWord::empty,
               R"pbdoc(
Check if the alphabet is defined.

This function returns ``True`` if no alphabet has been defined, and ``False`` otherwise.

:returns: Whether the alphabet is empty.
:rtype: bool
)pbdoc");
    thing3.def("alphabet",
               &ToWord::alphabet,
               R"pbdoc(
Return the alphabet used for conversion.

This function returns a :any:`str` corresponding to the ordered-set
alphabet :math:`\{a_0, a_1, \dots, a_{n-1}\}` that the initialised :any:`ToWord`
object will use to convert from :any:`str` to ``list[int]``.
Specifically, :math:`a_i \mapsto i` where :math:`a_i` will correspond to a
letter in a :any:`str`, and :math:`i` is an :any:`int`.

If this function returns the empty string, then conversion will be
performed using :any:`words.human_readable_index`.

:returns: The alphabet.
:rtype: str
)pbdoc");
    thing3.def("can_convert_letter",
               &ToWord::can_convert_letter,
               py::arg("c"),
               R"pbdoc(
Check if the current ToWord instance can convert a specified letter.

This function returns ``True`` if *c* can be converted to an :any:`int` using
this :any:`ToWord` instance, and ``False`` otherwise.

:param c: the letter to check the convertibility of.
:type c: str

:returns: Whether the letter can be converted.
:rtype: bool

:raises LibsemigroupsError: if *c* is a multi-character string.
)pbdoc");
    thing3.def(
        "init",
        [](ToWord& self) -> ToWord& { return self.init(); },
        R"pbdoc(
Initialize an existing ToWord object.

This function puts a :any:`ToWord` object back into the same state as if it had
been newly default constructed.

:returns: *self*.
:rtype: ToWord

.. seealso::
    :any:`ToWord()`

)pbdoc");
    thing3.def(
        "init",
        [](ToWord& self, std::string const& alphabet) -> ToWord& {
          return self.init(alphabet);
        },
        py::arg("alphabet"),
        R"pbdoc(
Initialize an existing ToWord object.

This function puts a :any:`ToWord` object back into the same state as if it had
been newly constructed from *alphabet*.

:param alphabet: the alphabet.
:type alphabet: str

:returns: *self*.
:rtype: ToWord

:raises LibsemigroupsError:  if there are repeated letters in *alphabet*.

.. seealso::
    :any:`ToWord(str)<ToWord>`

)pbdoc");
    thing3.def(
        "__call__",
        [](ToWord const& self, std::string const& input) {
          return self.operator()(input);
        },
        py::arg("input"),
        R"pbdoc(
:sig=(self: ToWord, input: str) -> list[int]:

Convert a string to a ``list[int]``.

This function converts its argument *input* into a ``list[int]``. The characters
of *input* are converted using the alphabet used to construct the object or set
via :any:`ToWord.init()` , or with :any:`words.human_readable_index` if
:any:`ToWord.empty()` returns ``True``.

:param input: the string to convert.
:type input: str

:returns: the converted list.
:rtype: list[int]

:raises LibsemigroupsError:
   if the alphabet used to define an instance of :any:`ToWord` is not empty and
   *input* contains letters that do not correspond to letters of the alphabet.
)pbdoc");

    ////////////////////////////////////////////////////////////////////////////
    // ToString
    ////////////////////////////////////////////////////////////////////////////

    py::class_<ToString> thing4(m,
                                "ToString",
                                R"pbdoc(
Class for converting ``list[int]`` to strings with specified alphabet.

An instance of this class is callable and used to convert from ``list[int]`` to
:any:`str`. The integers in the list are converted to characters according to
their position in the alphabet used to construct a :any:`ToString` instance if
one is provided, or using :any:`words.human_readable_letter` otherwise.

.. doctest::

  >>> from libsemigroups_pybind11 import ToString
  >>> to_string = ToString("bac")
  >>> to_string([0, 1, 2])
  'bac'
  >>> to_string([0, 1, 0, 1, 0, 0, 0, 2, 0, 2])
  'bababbbcbc'

  >>> to_string.init()      # clear the alphabet
  <ToString object with alphabet "">
  >>> to_string([0, 1, 2])
  'abc'
)pbdoc");
    thing4.def("__repr__", [](ToString const& tstr) {
      return to_human_readable_repr(tstr);
    });
    thing4.def(py::init<>(), R"pbdoc(
Default constructor.

Constructs an empty object with no alphabet set.
)pbdoc");
    thing4.def(py::init<std::string const&>(), py::arg("alphabet"), R"pbdoc(
Construct with given alphabet.

Construct a :any:`ToString` object with the given alphabet.

:param alphabet: the alphabet.
:type alphabet: str

:raises LibsemigroupsError:  if there are repeated letters in *alphabet*.
)pbdoc");
    thing4.def(
        "copy",
        [](ToString const& self) { return ToString(self); },
        R"pbdoc(
:sig=(self: ToString) -> ToString:

Copy a :any:`ToString` object.

:returns: A copy.
:rtype: ToString
)pbdoc");
    thing4.def("__copy__", [](ToString const& self) { return ToString(self); });
    thing4.def("alphabet",
               &ToString::alphabet,
               R"pbdoc(
Return the alphabet used for conversion.

This function returns a :any:`str` corresponding to the ordered-set
alphabet :math:`\{a_0, a_1, \dots, a_{n-1}\}` that the initialised :any:`ToString`
object will use to convert from ``list[int]`` to :any:`str`.
Specifically, :math:`i \mapsto a_i` where :math:`i` will be an :any:`int` in a list and
:math:`a_i` will be a character in a :any:`str`.

If this function returns the empty string, then conversion will be
performed using :any:`words.human_readable_letter`.

:returns: The alphabet.
:rtype: str
)pbdoc");
    thing4.def("can_convert_letter",
               &ToString::can_convert_letter,
               py::arg("l"),
               R"pbdoc(
:sig=(self: ToString, l: int) -> bool:

Check if the current ToString instance can convert a specified letter.

This function returns ``True`` if *l* can be converted to a :any:`str` using this
ToString instance, and ``False`` otherwise.

:param l: the letter to check the convertibility of.
:type l: int

:returns: Whether the letter can be converted.
:rtype: bool
)pbdoc");
    thing4.def("empty",
               &ToString::empty,
               R"pbdoc(
Check if the alphabet is defined.

This function return ``True`` if no alphabet has been defined, and ``False`` otherwise.

:returns: A whether the alphabet is empty.
:rtype: bool
)pbdoc");
    thing4.def(
        "init",
        [](ToString& self) -> ToString& { return self.init(); },
        R"pbdoc(
Initialize an existing ToString object.

This function puts a :any:`ToString` object back into the same state as if it
had been newly default constructed.

:returns: *self*.
:rtype: ToString

.. seealso::
    :any:`ToString()`

)pbdoc");
    thing4.def(
        "init",
        [](ToString& self, std::string const& alphabet) -> ToString& {
          return self.init(alphabet);
        },
        py::arg("alphabet"),
        R"pbdoc(
Initialize an existing Tostring object.

This function puts a :any:`ToString` object back into the same state as if it
had been newly constructed from *alphabet*.

:param alphabet: the alphabet.
:type alphabet: str

:returns: *self*.
:rtype: ToString

:raises LibsemigroupsError: if there are repeated letters in *alphabet*.

.. seealso::
    :any:`ToString(str)<ToString>`

)pbdoc");
    thing4.def(
        "__call__",
        [](ToString const& self, word_type const& input) {
          return self.operator()(input);
        },
        py::arg("input"),
        R"pbdoc(
:sig=(self: ToString, input: list[int]) -> str:

Convert a ``list[int]`` to a :any:`str`.

This function converts its argument *input* into a :any:`str`. The characters of
*input* are converted using the alphabet used to construct the object or set via
:any:`init()`, or with :any:`words.human_readable_letter` if :any:`empty()` returns ``True``.

:param input: the string to convert.
:type input: list[int]

:returns: The converted string.
:rtype: str

:raises LibsemigroupsError:  if the alphabet used to define an instance of :any:`ToString` is not empty and *input* contains letters that do not correspond to letters of the alphabet.
)pbdoc");

    ////////////////////////////////////////////////////////////////////////////
    // Helper functions for words in global namespace
    ////////////////////////////////////////////////////////////////////////////

    m.def("number_of_words",
          &number_of_words,
          py::arg("n"),
          py::arg("min"),
          py::arg("max"),
          R"pbdoc(
:sig=(n: int, min: int, max: int) -> int:

Returns the number of words over a given alphabet in some range.

:param n: the number of letters.
:type n: int
:param min: the minimum length of a word.
:type min: int
:param max: one greater than the maximum length of a word.
:type max: int

:returns: The number words over an alphabet with *n* letters with length in the range ``[min, max)``.
:rtype: int

.. doctest::

  >>> from libsemigroups_pybind11 import number_of_words
  >>> number_of_words(2, 0, 10)
  1023
)pbdoc");

    m.def("random_word",
          &random_word,
          py::arg("length"),
          py::arg("nr_letters"),
          R"pbdoc(
:sig=(length: int, nr_letters: int) -> list[int]:

Returns a random word.

:param length: the length of the word.
:type length: int
:param nr_letters: the size of the alphabet.
:type nr_letters: int

:return: A random word on ``[0, ..., nr_letters - 1]`` of length *length*.
:rtype: list[int]

:raises LibsemigroupsError: if *nr_letters* is ``0``.

.. seealso::
    :py:func:`random_string`.

)pbdoc");

    m.def(
        "random_string",
        [](std::string const& alphabet, size_t length) {
          return random_string(alphabet, length);
        },
        py::arg("alphabet"),
        py::arg("length"),
        R"pbdoc(
:sig=(alphabet: str, length: int) -> str:

Returns a random string.

Returns a random string with the specified length over the specified alphabet.

:param alphabet: the alphabet over which the string is constructed.
:type alphabet: str

:param length: the length of the string.
:type length: int

:returns: A random string.
:rtype: str


.. seealso::
    :any:`random_word`
)pbdoc");

    m.def(
        "random_string",
        [](std::string const& alphabet, size_t min, size_t max) {
          return random_string(alphabet, min, max);
        },
        py::arg("alphabet"),
        py::arg("min"),
        py::arg("max"),
        R"pbdoc(
:sig=(alphabet: str, min: int, max: int) -> str:

Returns a random string.

Returns a random string with random length in the range ``[min, max)`` over
the specified alphabet.

:param alphabet: the alphabet over which the string is constructed.
:type alphabet: str

:param min: the minimum length of the returned string.
:type min: int

:param max: one above the maximum length of the returned string.
:type max: int

:returns: A random string.
:rtype: str

:raises LibsemigroupsError: if ``min > max``;
:raises LibsemigroupsError: if ``len(alphabet) == 0`` and ``min != 0``.

.. seealso::
    :any:`random_word`
)pbdoc");

    m.def(
        "random_strings",
        [](std::string const& alphabet, size_t number, size_t min, size_t max) {
          auto rnd_strs = random_strings(alphabet, number, min, max);
          return py::make_iterator(rx::begin(rnd_strs), rx::end(rnd_strs));
        },
        py::arg("alphabet"),
        py::arg("number"),
        py::arg("min"),
        py::arg("max"),
        R"pbdoc(
:sig=(alphabet: str, number: int, min: int, max:int) -> collections.abc.Iterator[str]:

Returns an iterator of random strings.

Returns an iterator of random strings, each of which with random length
in the range ``[min, max)`` over the specified alphabet.

:param alphabet: the alphabet over which the string is constructed.
:type alphabet: str

:param number: the number of random strings to construct.
:type number: int

:param min: the minimum length of the returned string.
:type min: int

:param max: one above the maximum length of the returned string.
:type max: int

:returns: An iterator of random strings.
:rtype: collections.abc.Iterator[str]

:raises LibsemigroupsError: if *min* is greater than *max*;
:raises LibsemigroupsError: if ``len(alphabet) == 0`` and ``min != 0``.

.. seealso::
    :any:`random_word`
)pbdoc");

    ////////////////////////////////////////////////////////////////////////////
    // Helper functions for words in words namespace
    ////////////////////////////////////////////////////////////////////////////

    m.def("words_parse_relations",
          py::overload_cast<char const*>(&literals::operator""_p),
          py::arg("w"),
          R"pbdoc(
:sig=(w: str) -> str:
Construct string by parsing an algebraic expression.

This function provides a means of constructing a :any:`str` from an algebraic
expression, and has the following behaviour:
* arbitrarily nested brackets;
* spaces are ignored;
* redundant matched brackets are ignored;
* only the characters in ``"()^ "`` and in ``a-zA-Z0-9`` are allowed.

:param w: the expression to parse.
:type w: str

:returns: The parsed expression.
:rtype: str

:raises LibsemigroupsError: if *w* cannot be parsed.

.. doctest::

  >>> from libsemigroups_pybind11.words import parse_relations
  >>> parse_relations("((ab)^3cc)^2")
  'abababccabababcc'
  >>> parse_relations("a^0")
  ''

)pbdoc");

    m.def("words_human_readable_index",
          &words::human_readable_index,
          py::arg("c"),
          R"pbdoc(
:sig=(c: str) -> int:
Returns the index of a character in human readable order.

This function is the inverse of :any:`words.human_readable_letter`, see the
documentation of that function for more details.

:param c: character whose index is sought.
:type c: str

:returns: The human readable index.
:rtype: int

:raises ValueError: if *c* is not exactly one character long.

.. seealso::
    :any:`words.human_readable_letter`
)pbdoc");

    m.def("words_human_readable_letter",
          &words::human_readable_letter<std::string>,
          py::arg("i"),
          R"pbdoc(
:sig=(i: int) -> str:
Returns a character by index in human readable order.

This function exists to map the numbers ``0`` to ``255`` to the possible
values of a one-byte char, in such a way that the first characters are
``a-zA-Z0-9``. The ascii ranges for these characters are: :math:`[97, 123)`,
:math:`[65, 91)`, :math:`[48, 58)` so the remaining range of chars that are
appended to the end after these chars are :math:`[0,48)`, :math:`[58, 65)`,
:math:`[91, 97)`, :math:`[123, 255]`.

This function is the inverse of :any:`words.human_readable_index`.

:param i: the index of the character.
:type i: int

:returns: The human readable character.
:rtype: str

:raises LibsemigroupsError: if *i* exceeds ``255``.
)pbdoc");

    m.def(
        "words_pow",
        [](std::string_view w, size_t n) { return words::pow(w, n); },
        py::arg("w"),
        py::arg("n"),
        R"pbdoc(
:sig=(w: list[int] | str, n: int) -> list[int] | str:
:only-document-once:
Returns the power of a word.

Returns the word *w* to the power *n*.

:param w: the word to power.
:type w: list[int] | str

:param n: the power.
:type n: int

:returns: The powered word.
:rtype: list[int] | str
)pbdoc");

    m.def(
        "words_pow",
        [](word_type const& x, size_t n) { return words::pow(x, n); },
        py::arg("w"),
        py::arg("n"),
        R"pbdoc(
:sig=(w: list[int] | str, n: int) -> list[int] | str:
:only-document-once:
)pbdoc");

    m.def(
        "words_prod",
        [](std::string const& elts, int first, int last, int step = 1) {
          return words::prod(elts, first, last, step);
        },
        py::arg("elts"),
        py::arg("first"),
        py::arg("last"),
        py::arg("step") = static_cast<int>(1),
        R"pbdoc(
:sig=(elts: list[int] | str, first: int, last: int, step: int = 1) -> list[int] | str:
:only-document-once:
Returns a product of letters.

Let *elts* correspond to the ordered set :math:`a_0, a_1, \ldots, a_{n -1}` ,
*first* to :math:`f` , *last* to :math:`l` , and *step* to :math:`s`. If
:math:`f \leq l` , let :math:`k` be the greatest positive integer such that
:math:`f + ks < l`. Then this function returns the word corresponding to
:math:`a_f a_{f + s} a_{f + 2s} \cdots a_{f + ks}`. All subscripts are taken
modulo :math:`n`.

If there is no such :math:`k` (i.e. :math:`s < 0` , or :math:`f = l` ), then the
empty word is returned.

Where :math:`f > l` , the function works analogously, where :math:`k` is the
greatest positive integer such that :math:`f + k s > l`.

:param elts: the ordered set.
:type elts: list[int] | str

:param first: the first index.
:type first: int

:param last: the last index.
:type last: int

:param step: the step.
:type step: int

:returns: The word produced.
:rtype: list[int] | str

:raises LibsemigroupsError:
        if ``step = 0``;
:raises LibsemigroupsError:
        if *elts* is empty, but the specified range is not.

.. doctest::

  >>> from libsemigroups_pybind11.words import prod
  >>> w  =  "abcdef"
  >>> prod(w,  0,  5,  2)
  'ace'
  >>> prod(w,  1,  9,  2)
  'bdfb'
  >>> prod(w,  4,  1,  -1)
  'edc'

  >>> w  =  [0, 1, 2, 3, 4, 5]
  >>> prod(w,  0,  5,  2)
  [0, 2, 4]
  >>> prod(w,  1,  9,  2)
  [1, 3, 5, 1]
  >>> prod(w,  4,  1,  -1)
  [4, 3, 2]

)pbdoc");

    m.def(
        "words_prod",
        [](word_type const& elts, int first, int last, int step = 1) {
          return words::prod(elts, first, last, step);
        },
        py::arg("elts"),
        py::arg("first"),
        py::arg("last"),
        py::arg("step") = static_cast<int>(1),
        R"pbdoc(
:sig=(elts: list[int] | str, first: int, last: int, step: int = 1) -> list[int] | str:
:only-document-once:
)pbdoc");

    m.def(
        "words_prod",
        [](std::string const& elts, size_t last) {
          return words::prod(elts, last);
        },
        py::arg("elts"),
        py::arg("last"),
        R"pbdoc(
:sig=(elts: list[int] | str, last: int) -> list[int] | str:
:only-document-once:
Returns a product of letters or words.

:param elts: the ordered set.
:type elts: list[int] | str

:param last: the last index.
:type last: int

:returns: The word produced.
:rtype: list[int] | str

:raises LibsemigroupsError:
        if *elts* is empty, but the specified range is not.

.. note::
    This returns the same as ``prod(elts, 0, last, 1)``.
)pbdoc");

    m.def(
        "words_prod",
        [](word_type elts, size_t last) { return words::prod(elts, last); },
        py::arg("elts"),
        py::arg("last"),
        R"pbdoc(
:sig=(elts: list[int] | str, last: int) -> list[int] | str:
:only-document-once:
)pbdoc");
  }  // init_words

}  // namespace libsemigroups
