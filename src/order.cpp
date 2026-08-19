//
// libsemigroups_pybind11
// Copyright (C) 2023-2026 Murray T. Whyte
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

// C++ stl headers....
#include <string>  // for string
#include <vector>  // for vector

// libsemigroups....
#include <libsemigroups/order.hpp>  // for *_cmp, Order
#include <libsemigroups/types.hpp>  // for word_type

// pybind11....
#include <pybind11/pybind11.h>  // for arg, enum_, module, options
#include <pybind11/stl.h>       // for automatic conversions

// libsemigroups_pybind11....
#include "main.hpp"  // for init_order

namespace libsemigroups {
  namespace py = pybind11;

  namespace {
    void warn_deprecated(char const* old_name, char const* new_name) {
      std::string message = old_name;
      message += " is deprecated, and will be removed from "
                 "libsemigroups_pybind11 in v2. Instead, use ";
      message += new_name;
      message += ".";
      if (PyErr_WarnEx(PyExc_DeprecationWarning, message.c_str(), 2) < 0) {
        throw py::error_already_set();
      }
    }

    template <typename Word, typename Compare>
    void bind_deprecated_compare(py::module& m,
                                 char const* old_name,
                                 char const* new_name,
                                 Compare     compare) {
      m.def(
          old_name,
          [old_name, new_name, compare](Word const& x, Word const& y) {
            warn_deprecated(old_name, new_name);
            return compare(x, y);
          },
          py::arg("x"),
          py::arg("y"));
    }

    template <typename Word>
    void bind_order_comparisons(py::module& m) {
      m.def(
          "lex_cmp",
          [](Word const& x, Word const& y) { return lex_cmp(x, y); },
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words lexicographically.

:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

.. doctest:: python

  >>> from libsemigroups_pybind11 import lex_cmp
  >>> lex_cmp("ab", "ba")
  True
  >>> lex_cmp([0, 1], [1, 0])
  True
)pbdoc");

      m.def(
          "lex_cmp",
          [](Alphabet<Word> const& alphabet, Word const& x, Word const& y) {
            return lex_cmp(alphabet, x, y);
          },
          py::arg("alphabet"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(alphabet: Alphabet, x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words lexicographically using an alphabet.

Letters are compared by their positions in *alphabet*.

:param alphabet: the alphabet that determines the ordering of letters.
:type alphabet: Alphabet
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool
:raises LibsemigroupsError: if either word contains a letter that does not
  belong to *alphabet*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, lex_cmp
  >>> alphabet = Alphabet("ba")
  >>> lex_cmp(alphabet, "b", "a")
  True
)pbdoc");

      m.def(
          "lenlex_cmp",
          [](Word const& x, Word const& y) { return lenlex_cmp(x, y); },
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using lenlex ordering.

Words are first ordered by length and then lexicographically.

:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

.. doctest:: python

  >>> from libsemigroups_pybind11 import lenlex_cmp
  >>> lenlex_cmp("bb", "aaa")
  True
  >>> lenlex_cmp([1, 1], [0, 0, 0])
  True
)pbdoc");

      m.def(
          "lenlex_cmp",
          [](Alphabet<Word> const& alphabet, Word const& x, Word const& y) {
            return lenlex_cmp(alphabet, x, y);
          },
          py::arg("alphabet"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(alphabet: Alphabet, x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using lenlex ordering and an alphabet.

Words are first ordered by length and then lexicographically, with letters
compared by their positions in *alphabet*.

:param alphabet: the alphabet that determines the ordering of letters.
:type alphabet: Alphabet
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if either word contains a letter that does not
  belong to *alphabet*.

.. doctest:: python
  >>> from libsemigroups_pybind11 import Alphabet, lenlex_cmp
  >>> alphabet = Alphabet("ba")
  >>> lenlex_cmp(alphabet, "b", "a")
  True
)pbdoc");

      m.def(
          "rev_lex_cmp",
          [](Word const& x, Word const& y) { return rev_lex_cmp(x, y); },
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using reversed lexicographic ordering.

The words are read from right to left when being compared.

:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

.. doctest:: python

  >>> from libsemigroups_pybind11 import rev_lex_cmp
  >>> rev_lex_cmp("ba", "ab")
  True
)pbdoc");

      m.def(
          "rev_lex_cmp",
          [](Alphabet<Word> const& alphabet, Word const& x, Word const& y) {
            return rev_lex_cmp(alphabet, x, y);
          },
          py::arg("alphabet"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(alphabet: Alphabet, x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using alphabet-aware reversed lexicographic ordering.

Letters are compared by their positions in *alphabet*, and the words are read
from right to left when being compared.

:param alphabet: the ordered alphabet containing the letters of both words.
:type alphabet: Alphabet
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if either word contains a letter that does not
  belong to *alphabet*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, rev_lex_cmp
  >>> rev_lex_cmp(Alphabet("ab"), "ba", "ab")
  True
)pbdoc");

      m.def(
          "rev_lenlex_cmp",
          [](Word const& x, Word const& y) { return rev_lenlex_cmp(x, y); },
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using reversed lenlex ordering.

Words are first ordered by length and then lexicographically reading
from right to left.

:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

.. doctest:: python

  >>> from libsemigroups_pybind11 import rev_lenlex_cmp
  >>> rev_lenlex_cmp([1, 0], [0, 1])
  True
)pbdoc");

      m.def(
          "rev_lenlex_cmp",
          [](Alphabet<Word> const& alphabet, Word const& x, Word const& y) {
            return rev_lenlex_cmp(alphabet, x, y);
          },
          py::arg("alphabet"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(alphabet: Alphabet, x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using alphabet-aware reversed lenlex ordering.

Words are first ordered by length and then lexicographically reading
from right to left. Letters are compared by their positions in *alphabet*.

:param alphabet: the ordered alphabet containing the letters of both words.
:type alphabet: Alphabet
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if either word contains a letter that does not
  belong to *alphabet*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, rev_lenlex_cmp
  >>> rev_lenlex_cmp(Alphabet("ab"), "ba", "ab")
  True
)pbdoc");

      m.def(
          "wt_lenlex_cmp",
          [](std::vector<size_t> const& weights, Word const& x, Word const& y) {
            return wt_lenlex_cmp(weights, x, y);
          },
          py::arg("weights"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(weights: list[int], x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using weighted lenlex ordering.

The *i*-th entry of *weights* is the weight assigned to generator *i*.
Words are first ordered by their total weight, then by length, and finally
lexicographically.

:param weights: the weight assigned to each generator.
:type weights: list[int]
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if a letter is not a valid index into *weights*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import wt_lenlex_cmp
  >>> wt_lenlex_cmp([1, 2], [1], [0, 0])
  True
)pbdoc");

      m.def(
          "wt_lenlex_cmp",
          [](Alphabet<Word> const&      alphabet,
             std::vector<size_t> const& weights,
             Word const&                x,
             Word const& y) { return wt_lenlex_cmp(alphabet, weights, x, y); },
          py::arg("alphabet"),
          py::arg("weights"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(alphabet: Alphabet, weights: list[int], x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using alphabet-aware weighted lenlex ordering.

Letters are mapped to their positions in *alphabet*, and the *i*-th entry of
*weights* is the weight assigned to the *i*-th letter of *alphabet*. Words are
first ordered by their total weight, then by length, and finally
lexicographically according to *alphabet*.

:param alphabet: the ordered alphabet containing the letters of both words.
:type alphabet: Alphabet
:param weights: the weight assigned to each letter of *alphabet*.
:type weights: list[int]
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if a letter does not belong to *alphabet*, or its
  position in *alphabet* is not a valid index into *weights*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, wt_lenlex_cmp
  >>> wt_lenlex_cmp(Alphabet("ba"), [1, 1], "b", "a")
  True
)pbdoc");

      m.def(
          "wt_lex_cmp",
          [](std::vector<size_t> const& weights, Word const& x, Word const& y) {
            return wt_lex_cmp(weights, x, y);
          },
          py::arg("weights"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(weights: list[int], x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using weighted lexicographic ordering.

The *i*-th entry of *weights* is the weight assigned to generator *i*.
Words are first ordered by their total weight and then lexicographically.

:param weights: the weight assigned to each generator.
:type weights: list[int]
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if a letter is not a valid index into *weights*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import wt_lex_cmp
  >>> wt_lex_cmp([1, 2], [1], [0, 0])
  False
)pbdoc");

      m.def(
          "wt_lex_cmp",
          [](Alphabet<Word> const&      alphabet,
             std::vector<size_t> const& weights,
             Word const&                x,
             Word const& y) { return wt_lex_cmp(alphabet, weights, x, y); },
          py::arg("alphabet"),
          py::arg("weights"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(alphabet: Alphabet, weights: list[int], x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using alphabet-aware weighted lexicographic ordering.

Letters are mapped to their positions in *alphabet*, and the *i*-th entry of
*weights* is the weight assigned to the *i*-th letter of *alphabet*. Words are
first ordered by their total weight and then lexicographically according to
*alphabet*.

:param alphabet: the ordered alphabet containing the letters of both words.
:type alphabet: Alphabet
:param weights: the weight assigned to each letter of *alphabet*.
:type weights: list[int]
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if a letter does not belong to *alphabet*, or its
  position in *alphabet* is not a valid index into *weights*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, wt_lex_cmp
  >>> wt_lex_cmp(Alphabet("ba"), [1, 1], "b", "a")
  True
)pbdoc");

      m.def(
          "len_wt_lex_cmp",
          [](std::vector<size_t> const& weights, Word const& x, Word const& y) {
            return len_wt_lex_cmp(weights, x, y);
          },
          py::arg("weights"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(weights: list[int], x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words by length, weight, and lexicographic order.

Words are first ordered by length, then by total weight, and finally
lexicographically. The *i*-th entry of *weights* is the weight assigned to
generator *i*.

:param weights: the weight assigned to each generator.
:type weights: list[int]
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if a letter is not a valid index into *weights*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import len_wt_lex_cmp
  >>> len_wt_lex_cmp([1, 1], [0, 1], [1, 0])
  True
)pbdoc");

      m.def(
          "len_wt_lex_cmp",
          [](Alphabet<Word> const&      alphabet,
             std::vector<size_t> const& weights,
             Word const&                x,
             Word const& y) { return len_wt_lex_cmp(alphabet, weights, x, y); },
          py::arg("alphabet"),
          py::arg("weights"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(alphabet: Alphabet, weights: list[int], x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words by length, weight, and alphabet-aware lexicographic order.

Letters are mapped to their positions in *alphabet*, and the *i*-th entry of
*weights* is the weight assigned to the *i*-th letter of *alphabet*. Words are
first ordered by length, then by total weight, and finally lexicographically
according to *alphabet*.

:param alphabet: the ordered alphabet containing the letters of both words.
:type alphabet: Alphabet
:param weights: the weight assigned to each letter of *alphabet*.
:type weights: list[int]
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if a letter does not belong to *alphabet*, or its
  position in *alphabet* is not a valid index into *weights*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, len_wt_lex_cmp
  >>> len_wt_lex_cmp(Alphabet("ab"), [1, 1], "ab", "ba")
  True
)pbdoc");

      m.def(
          "rev_wr_cmp",
          [](std::vector<size_t> const& levels, Word const& x, Word const& y) {
            return rev_wr_cmp(levels, x, y);
          },
          py::arg("levels"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(levels: list[int], x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using reversed wreath-product ordering.

The words are read from right to left when being compared. The *i*-th entry
of *levels* is the wreath-product level assigned to generator *i*.

:param levels: the level assigned to each generator.
:type levels: list[int]
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if a letter is not a valid index into *levels*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import rev_wr_cmp
  >>> rev_wr_cmp([0, 0], [1, 0], [0, 1])
  True
)pbdoc");

      m.def(
          "rev_wr_cmp",
          [](Alphabet<Word> const&      alphabet,
             std::vector<size_t> const& levels,
             Word const&                x,
             Word const& y) { return rev_wr_cmp(alphabet, levels, x, y); },
          py::arg("alphabet"),
          py::arg("levels"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(alphabet: Alphabet, levels: list[int], x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using alphabet-aware reversed wreath-product ordering.

Letters are mapped to their positions in *alphabet*, and the *i*-th entry of
*levels* is the wreath-product level assigned to the *i*-th letter of
*alphabet*. The words are read from right to left when being compared.

:param alphabet: the ordered alphabet containing the letters of both words.
:type alphabet: Alphabet
:param levels: the level assigned to each letter of *alphabet*.
:type levels: list[int]
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if a letter does not belong to *alphabet*, or its
  position in *alphabet* is not a valid index into *levels*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, rev_wr_cmp
  >>> rev_wr_cmp(Alphabet("ab"), [0, 0], "ba", "ab")
  True
)pbdoc");

      m.def(
          "rev_wt_lenlex_cmp",
          [](std::vector<size_t> const& weights, Word const& x, Word const& y) {
            return rev_wt_lenlex_cmp(weights, x, y);
          },
          py::arg("weights"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(weights: list[int], x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using reversed weighted lenlex ordering.

Words are first ordered by total weight, then by length, and finally
lexicographically read from right to left. The *i*-th entry of
*weights* is the weight assigned to generator *i*.

:param weights: the weight assigned to each generator.
:type weights: list[int]
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if a letter is not a valid index into *weights*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import rev_wt_lenlex_cmp
  >>> rev_wt_lenlex_cmp([1, 1], [1, 0], [0, 1])
  True
)pbdoc");

      m.def(
          "rev_wt_lenlex_cmp",
          [](Alphabet<Word> const&      alphabet,
             std::vector<size_t> const& weights,
             Word const&                x,
             Word const&                y) {
            return rev_wt_lenlex_cmp(alphabet, weights, x, y);
          },
          py::arg("alphabet"),
          py::arg("weights"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(alphabet: Alphabet, weights: list[int], x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using alphabet-aware reversed weighted lenlex ordering.

Letters are mapped to their positions in *alphabet*, and the *i*-th entry of
*weights* is the weight assigned to the *i*-th letter of *alphabet*. Words are
first ordered by total weight, then by length, and finally lexicographically
read from right to left.

:param alphabet: the ordered alphabet containing the letters of both words.
:type alphabet: Alphabet
:param weights: the weight assigned to each letter of *alphabet*.
:type weights: list[int]
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if a letter does not belong to *alphabet*, or its
  position in *alphabet* is not a valid index into *weights*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, rev_wt_lenlex_cmp
  >>> rev_wt_lenlex_cmp(Alphabet("ab"), [1, 1], "ba", "ab")
  True
)pbdoc");

      m.def(
          "rev_wt_lex_cmp",
          [](std::vector<size_t> const& weights, Word const& x, Word const& y) {
            return rev_wt_lex_cmp(weights, x, y);
          },
          py::arg("weights"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(weights: list[int], x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using reversed weighted lexicographic ordering.

Words are first ordered by total weight and then lexicographically
read from right to left. The *i*-th entry of *weights* is the weight assigned
to generator *i*.

:param weights: the weight assigned to each generator.
:type weights: list[int]
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if a letter is not a valid index into *weights*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import rev_wt_lex_cmp
  >>> rev_wt_lex_cmp([1, 1], [1, 0], [0, 1])
  True
)pbdoc");

      m.def(
          "rev_wt_lex_cmp",
          [](Alphabet<Word> const&      alphabet,
             std::vector<size_t> const& weights,
             Word const&                x,
             Word const& y) { return rev_wt_lex_cmp(alphabet, weights, x, y); },
          py::arg("alphabet"),
          py::arg("weights"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(alphabet: Alphabet, weights: list[int], x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using alphabet-aware reversed weighted lexicographic ordering.

Letters are mapped to their positions in *alphabet*, and the *i*-th entry of
*weights* is the weight assigned to the *i*-th letter of *alphabet*. Words are
first ordered by total weight and then lexicographically read from
right to left.

:param alphabet: the ordered alphabet containing the letters of both words.
:type alphabet: Alphabet
:param weights: the weight assigned to each letter of *alphabet*.
:type weights: list[int]
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if a letter does not belong to *alphabet*, or its
  position in *alphabet* is not a valid index into *weights*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, rev_wt_lex_cmp
  >>> rev_wt_lex_cmp(Alphabet("ab"), [1, 1], "ba", "ab")
  True
)pbdoc");

      m.def(
          "rev_len_wt_lex_cmp",
          [](std::vector<size_t> const& weights, Word const& x, Word const& y) {
            return rev_len_wt_lex_cmp(weights, x, y);
          },
          py::arg("weights"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(weights: list[int], x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words by length, weight, and reversed lexicographic order.

Words are first ordered by length, then by total weight, and finally
lexicographically read from right to left. The *i*-th entry of
*weights* is the weight assigned to generator *i*.

:param weights: the weight assigned to each generator.
:type weights: list[int]
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if a letter is not a valid index into *weights*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import rev_len_wt_lex_cmp
  >>> rev_len_wt_lex_cmp([1, 1], [1, 0], [0, 1])
  True
)pbdoc");

      m.def(
          "rev_len_wt_lex_cmp",
          [](Alphabet<Word> const&      alphabet,
             std::vector<size_t> const& weights,
             Word const&                x,
             Word const&                y) {
            return rev_len_wt_lex_cmp(alphabet, weights, x, y);
          },
          py::arg("alphabet"),
          py::arg("weights"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(alphabet: Alphabet, weights: list[int], x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words by length, weight, and alphabet-aware reversed lexicographic order.

Letters are mapped to their positions in *alphabet*, and the *i*-th entry of
*weights* is the weight assigned to the *i*-th letter of *alphabet*. Words are
first ordered by length, then by total weight, and finally lexicographically
read from right to left.

:param alphabet: the ordered alphabet containing the letters of both words.
:type alphabet: Alphabet
:param weights: the weight assigned to each letter of *alphabet*.
:type weights: list[int]
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if a letter does not belong to *alphabet*, or its
  position in *alphabet* is not a valid index into *weights*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, rev_len_wt_lex_cmp
  >>> rev_len_wt_lex_cmp(Alphabet("ab"), [1, 1], "ba", "ab")
  True
)pbdoc");

      m.def(
          "rpo_cmp",
          [](Word const& x, Word const& y) { return rpo_cmp(x, y); },
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using recursive-path ordering.

:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

.. warning::
   This function has significantly worse performance than :any:`lenlex_cmp`
   and :any:`lex_cmp`.

.. doctest:: python

  >>> from libsemigroups_pybind11 import rpo_cmp
  >>> rpo_cmp("a", "b")
  True
  >>> rpo_cmp([0], [1])
  True
)pbdoc");

      m.def(
          "rpo_cmp",
          [](Alphabet<Word> const& alphabet, Word const& x, Word const& y) {
            return rpo_cmp(alphabet, x, y);
          },
          py::arg("alphabet"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(alphabet: Alphabet, x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using recursive-path ordering and an alphabet.

Letters are compared by their positions in *alphabet*.

:param alphabet: the alphabet that determines the ordering of letters.
:type alphabet: Alphabet
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool
:raises LibsemigroupsError: if either word contains a letter that does not
  belong to *alphabet*.

.. warning::
   This function has significantly worse performance than :any:`lenlex_cmp`
   and :any:`lex_cmp`.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, rpo_cmp
  >>> alphabet = Alphabet("ba")
  >>> rpo_cmp(alphabet, "b", "a")
  True
)pbdoc");

      m.def(
          "rev_rpo_cmp",
          [](Word const& x, Word const& y) { return rev_rpo_cmp(x, y); },
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using reversed recursive-path ordering.

This is recursive-path ordering applied after reading both words from right to
left.

:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

.. warning::
   This function has significantly worse performance than :any:`lenlex_cmp`
   and :any:`lex_cmp`.

.. doctest:: python

  >>> from libsemigroups_pybind11 import rev_rpo_cmp
  >>> rev_rpo_cmp("a", "b")
  True
  >>> rev_rpo_cmp([0], [1])
  True
)pbdoc");

      m.def(
          "rev_rpo_cmp",
          [](Alphabet<Word> const& alphabet, Word const& x, Word const& y) {
            return rev_rpo_cmp(alphabet, x, y);
          },
          py::arg("alphabet"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(alphabet: Alphabet, x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using reversed recursive-path ordering and an alphabet.

This is recursive-path ordering applied after reading both words from right to
left, with letters compared by their positions in *alphabet*.

:param alphabet: the alphabet that determines the ordering of letters.
:type alphabet: Alphabet
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool
:raises LibsemigroupsError: if either word contains a letter that does not
  belong to *alphabet*.

.. warning::
   This function has significantly worse performance than :any:`lenlex_cmp`
   and :any:`lex_cmp`.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, rev_rpo_cmp
  >>> alphabet = Alphabet("ba")
  >>> rev_rpo_cmp(alphabet, "b", "a")
  True
)pbdoc");

      m.def(
          "wt_lenlex_cmp",
          [](std::vector<size_t> const& weights, Word const& x, Word const& y) {
            return wt_lenlex_cmp(weights, x, y);
          },
          py::arg("weights"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(weights: list[int], x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using weighted lenlex ordering.

The *i*-th entry of *weights* is the weight assigned to generator *i*.
Words are first ordered by their total weight, then by length, and finally
lexicographically.

:param weights: the weight assigned to each generator.
:type weights: list[int]
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if a letter is not a valid index into *weights*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import wt_lenlex_cmp
  >>> wt_lenlex_cmp([1, 3], [1], [0, 0])
  False
)pbdoc");

      m.def(
          "wt_lenlex_cmp",
          [](Alphabet<Word> const&      alphabet,
             std::vector<size_t> const& weights,
             Word const&                x,
             Word const& y) { return wt_lenlex_cmp(alphabet, weights, x, y); },
          py::arg("alphabet"),
          py::arg("weights"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(alphabet: Alphabet, weights: list[int], x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using alphabet-aware weighted lenlex ordering.

Letters are mapped to their positions in *alphabet*, and the *i*-th entry of
*weights* is the weight assigned to the *i*-th letter of *alphabet*. Words are
first ordered by their total weight, then by length, and finally
lexicographically according to *alphabet*.

:param alphabet: the ordered alphabet containing the letters of both words.
:type alphabet: Alphabet
:param weights: the weight assigned to each letter of *alphabet*.
:type weights: list[int]
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if a letter does not belong to *alphabet*, or its
  position in *alphabet* is not a valid index into *weights*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, wt_lenlex_cmp
  >>> wt_lenlex_cmp(Alphabet("ba"), [3, 1], "b", "a")
  False
)pbdoc");

      m.def(
          "wt_lex_cmp",
          [](std::vector<size_t> const& weights, Word const& x, Word const& y) {
            return wt_lex_cmp(weights, x, y);
          },
          py::arg("weights"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(weights: list[int], x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using weighted lexicographic ordering.

The *i*-th entry of *weights* is the weight assigned to generator *i*.
Words are first ordered by their total weight and then lexicographically.

:param weights: the weight assigned to each generator.
:type weights: list[int]
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if a letter is not a valid index into *weights*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import wt_lex_cmp
  >>> wt_lex_cmp([1, 1], [1], [0, 0])
  True
)pbdoc");

      m.def(
          "wt_lex_cmp",
          [](Alphabet<Word> const&      alphabet,
             std::vector<size_t> const& weights,
             Word const&                x,
             Word const& y) { return wt_lex_cmp(alphabet, weights, x, y); },
          py::arg("alphabet"),
          py::arg("weights"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(alphabet: Alphabet, weights: list[int], x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using alphabet-aware weighted lexicographic ordering.

Letters are mapped to their positions in *alphabet*, and the *i*-th entry of
*weights* is the weight assigned to the *i*-th letter of *alphabet*. Words are
first ordered by their total weight and then lexicographically according to
*alphabet*.

:param alphabet: the ordered alphabet containing the letters of both words.
:type alphabet: Alphabet
:param weights: the weight assigned to each letter of *alphabet*.
:type weights: list[int]
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if a letter does not belong to *alphabet*, or its
  position in *alphabet* is not a valid index into *weights*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, wt_lex_cmp
  >>> wt_lex_cmp(Alphabet("ba"), [3, 1], "a", "b")
  True
)pbdoc");

      bind_deprecated_compare<Word>(
          m,
          "lexicographical_compare",
          "lex_cmp",
          [](Word const& x, Word const& y) { return lex_cmp(x, y); });

      bind_deprecated_compare<Word>(
          m,
          "shortlex_compare",
          "lenlex_cmp",
          [](Word const& x, Word const& y) { return lenlex_cmp(x, y); });

      bind_deprecated_compare<Word>(
          m,
          "recursive_path_compare",
          "rev_rpo_cmp",
          [](Word const& x, Word const& y) { return rev_rpo_cmp(x, y); });

      m.def(
          "wr_cmp",
          [](std::vector<size_t> const& levels, Word const& x, Word const& y) {
            return wr_cmp(levels, x, y);
          },
          py::arg("levels"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(levels: list[int], x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using wreath-product ordering.

The *i*-th entry of *levels* is the level assigned to generator *i*.
Differences at higher levels dominate differences at lower levels, and
differences within one level are compared using lenlex ordering.

:param levels: the level assigned to each generator.
:type levels: list[int]
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if a letter is not a valid index into *levels*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import wr_cmp
  >>> levels = [1, 1, 0]
  >>> wr_cmp(levels, [2, 1, 2, 2], [2, 2, 1, 2])
  True
)pbdoc");

      m.def(
          "wr_cmp",
          [](Alphabet<Word> const&      alphabet,
             std::vector<size_t> const& levels,
             Word const&                x,
             Word const& y) { return wr_cmp(alphabet, levels, x, y); },
          py::arg("alphabet"),
          py::arg("levels"),
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(alphabet: Alphabet, levels: list[int], x: str | list[int], y: str | list[int]) -> bool:
:only-document-once:
Compare two words using alphabet-aware wreath-product ordering.

Letters are mapped to their positions in *alphabet*, and the *i*-th entry of
*levels* is the level assigned to the *i*-th letter of *alphabet*.
Differences at higher levels dominate differences at lower levels, and
differences within one level are compared using lenlex ordering.

:param alphabet: the ordered alphabet containing the letters of both words.
:type alphabet: Alphabet
:param levels: the level assigned to each letter of *alphabet*.
:type levels: list[int]
:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises LibsemigroupsError: if a letter does not belong to *alphabet*, or its
  position in *alphabet* is not a valid index into *levels*.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, wr_cmp
  >>> alphabet = Alphabet("bac")
  >>> levels = [1, 1, 0]
  >>> wr_cmp(alphabet, levels, "cbcc", "ccbc")
  True
)pbdoc");
    }
  }  // namespace

  void init_order(py::module& m) {
    py::options options;
    options.disable_enum_members_docstring();

    py::enum_<Order>(m, "Order", R"pbdoc(
An enum class for the possible orderings of words and strings.

The values in this enum can be used as the arguments for functions such as
:any:`ToddCoxeter.standardize` or :any:`WordRange.order` to specify which
ordering should be used.

The values :any:`Order.shortlex` and :any:`Order.recursive` are retained for
backwards compatibility; use :any:`Order.lenlex` and :any:`Order.rev_rpo`,
respectively, in new code.

.. py:attribute:: Order.none
  :value: <Order.left: 0>

   No ordering

.. py:attribute:: Order.lenlex
  :value: <Order.right: 1>

    The lenlex ordering. Words are first ordered by length, and then
    lexicographically.

.. py:attribute:: Order.shortlex
  :value: <Order.left: 2>

    The short-lex ordering. Words are first ordered by length, and then
    lexicographically.

    This is deprecated; use :any:`Order.lenlex` instead.

.. py:attribute:: Order.lex
  :value: <Order.right: 3>

    The lexicographic ordering. Note that this is not a well-order, so there
    may not be a lexicographically least word in a given congruence class of
    words.

.. py:attribute:: Order.rpo
  :value: <Order.left: 4>

    The recursive-path ordering, as described in :cite:`Jantzen2012aa`
    (Definition 1.2.14, page 24).

.. py:attribute:: Order.rev_rpo
  :value: <Order.right: 5>

    The reversed recursive-path ordering, based on the description in
    :cite:`Jantzen2012aa` (Definition 1.2.14, page 24), where words are read
    right-to-left before ordering.

.. py:attribute:: Order.recursive
  :value: <Order.right: 6>

    The recursive-path ordering, as described in :cite:`Jantzen2012aa`
    (Definition 1.2.14, page 24).

    This is deprecated; use :any:`Order.rpo` instead.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Order
  >>> Order.lenlex
  <Order.lenlex: 1>
  >>> Order.shortlex == Order.lenlex
  True
)pbdoc")
        .value("none", Order::none)
        .value("lenlex", Order::lenlex)
        .value("shortlex", Order::lenlex)
        .value("lex", Order::lex)
        .value("rpo", Order::rpo)
        .value("rev_rpo", Order::rev_rpo)
        .value("recursive", Order::rev_rpo);

    bind_order_comparisons<std::string>(m);
    bind_order_comparisons<word_type>(m);
  }
}  // namespace libsemigroups
