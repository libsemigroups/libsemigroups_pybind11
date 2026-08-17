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
#include <string>       // for string
#include <type_traits>  // for is_same_v
#include <vector>       // for vector

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

.. seealso:: :any:`LenLexCmp` for a reusable len-lex comparison object.

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

.. seealso:: :any:`RPOCmp` for a reusable recursive-path comparison object.

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

.. seealso::
   :any:`RevRPOCmp` for a reusable reversed recursive-path comparison object.

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

.. seealso::
   :any:`RevRPOCmp` for a reusable reversed recursive-path comparison object.

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

    ////////////////////////////////////////////////////////////////////////
    // Functions for binding the various Cmp structs such as WtLenLexCmp,
    // RPOCmp, and so on
    ////////////////////////////////////////////////////////////////////////

    // The remaining documentation is included in the corresponding
    // alphabet-aware binding function below.
    template <typename Cmp>
    void bind_cmp_default(py::module& m, std::string name) {
      std::string arg_type(name);
      name += "CmpDefault";
      arg_type += "Cmp";
      py::class_<Cmp> thing(m, name.c_str());

      thing.def(py::init<>(),
                fmt::format(R"pbdoc(
:sig=(self: {0}) -> None:
Construct a comparison object.

Constructs an object whose call operator compares either ``str`` or
``list[int]`` words using the natural order of their letters.

.. doctest:: python

  >>> from libsemigroups_pybind11 import {0}
  >>> {0}()("a", "b")
  True
)pbdoc",
                            arg_type)
                    .c_str());

      thing.def("__repr__",
                [](Cmp const& self) { return to_human_readable_repr(self); });

      thing.def("__copy__", [](Cmp const& self) { return Cmp(self); });

      thing.def("copy", [](Cmp const& self) { return Cmp(self); });

      thing.def(
          "__call__",
          [](Cmp const& self, std::string const& x, std::string const& y) {
            return self(x, y);
          },
          py::arg("x"),
          py::arg("y"));

      thing.def(
          "__call__",
          [](Cmp const& self, word_type const& x, word_type const& y) {
            return self(x, y);
          },
          py::arg("x"),
          py::arg("y"));
    }

    template <typename Cmp>
    void bind_configured_cmp_default(
        py::module&        m,
        std::string const& name,
        std::string const& configuration,
        std::vector<size_t> const& (Cmp::*get_configuration)() const) {
      std::string const binding_name = name + "Default";
      py::class_<Cmp>   thing(m, binding_name.c_str());

      thing.def(py::init<>(),
                fmt::format(R"pbdoc(
:sig=(self: {0}) -> None:
Construct a comparison object with an empty {1} vector.

.. doctest:: python

  >>> from libsemigroups_pybind11 import {0}
  >>> {0}().{1}()
  []
)pbdoc",
                            name,
                            configuration)
                    .c_str());
      thing.def(py::init<std::vector<size_t> const&>(),
                py::arg(configuration.c_str()),
                fmt::format(R"pbdoc(
:sig=(self: {0}, {1}: list[int]) -> None:
Construct a comparison object for index words.

:param {1}: the {1} of the generators.
:type {1}: list[int]

.. doctest:: python

  >>> from libsemigroups_pybind11 import {0}
  >>> {0}([1, 2]).{1}()
  [1, 2]
)pbdoc",
                            name,
                            configuration)
                    .c_str());
      thing.def("__repr__",
                [](Cmp const& self) { return to_human_readable_repr(self); });
      thing.def("__copy__", [](Cmp const& self) { return Cmp(self); });
      thing.def("copy", [](Cmp const& self) { return Cmp(self); });
      thing.def("init", [](Cmp& self) -> Cmp& { return self.init(); });
      thing.def(
          "init",
          [](Cmp& self, std::vector<size_t> const& values) -> Cmp& {
            return self.init(values);
          },
          py::arg(configuration.c_str()));
      thing.def(configuration.c_str(), [get_configuration](Cmp const& self) {
        return (self.*get_configuration)();
      });
      thing.def(
          "__call__",
          [](Cmp const& self, word_type const& x, word_type const& y) {
            return self(x, y);
          },
          py::arg("x"),
          py::arg("y"));
    }

    template <typename Cmp, typename Word>
    void bind_configured_cmp_with_alphabet(
        py::module&        m,
        char const*        binding_name,
        std::string const& name,
        std::string const& configuration,
        std::string const& ordering,
        std::vector<size_t> const& (Cmp::*get_configuration)() const) {
      py::class_<Cmp> thing(m,
                            binding_name,
                            fmt::format(R"pbdoc(
Compare words using {2}.

Use ``{0}({1})`` to compare ``list[int]`` index words, where the entries of
*{1}* correspond to the indices. Use ``{0}(alphabet, {1})`` to compare words
whose letters belong to *alphabet*. The latter form copies both arguments and
only accepts words with the same type as *alphabet*.

.. note::
  The constructor fixes whether this object is alphabet-aware and fixes the
  word type of an alphabet-aware object. Reinitialization cannot change either.

.. seealso::

  :any:`Alphabet`

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, {0}
  >>> {0}([1, 2])([0], [1])
  True
  >>> {0}(Alphabet("ab"), [1, 2])("a", "b")
  True
  >>> {0}(Alphabet([0, 1]), [1, 2])([0], [1])
  True
)pbdoc",
                                        name,
                                        configuration,
                                        ordering)
                                .c_str());

      thing.def(py::init<Alphabet<Word> const&, std::vector<size_t> const&>(),
                py::arg("alphabet"),
                py::arg(configuration.c_str()),
                fmt::format(R"pbdoc(
:sig=(self: {0}, alphabet: Alphabet, {1}: list[int]) -> None:
Construct a comparison object from an alphabet and {1}.

The two arguments must have the same size.

:param alphabet: the alphabet defining the letters and their order.
:type alphabet: Alphabet
:param {1}: the {1} of the letters in *alphabet*.
:type {1}: list[int]

:raises LibsemigroupsError:
  if *alphabet* and *{1}* have different sizes.
:raises TypeError:
  if the arguments do not have the required types.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, {0}
  >>> {0}(Alphabet("ab"), [1, 2])("a", "b")
  True
)pbdoc",
                            name,
                            configuration)
                    .c_str());
      thing.def("__repr__",
                [](Cmp const& self) { return to_human_readable_repr(self); });
      thing.def("__copy__", [](Cmp const& self) { return Cmp(self); });
      thing.def(
          "copy",
          [](Cmp const& self) { return Cmp(self); },
          fmt::format(R"pbdoc(
:sig=(self: {0}) -> {0}:
Copy a comparison object.

:returns: An independent copy of *self*.
:rtype: {0}

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, {0}
  >>> {0}(Alphabet("ab"), [1, 2]).copy()("a", "b")
  True
)pbdoc",
                      name)
              .c_str());
      thing.def(
          "init",
          [](Cmp&                       self,
             Alphabet<Word> const&      alphabet,
             std::vector<size_t> const& values) -> Cmp& {
            return self.init(alphabet, values);
          },
          py::arg("alphabet"),
          py::arg(configuration.c_str()),
          fmt::format(R"pbdoc(
:sig=(self: {0}, alphabet: Alphabet, {1}: list[int]) -> {0}:
Reinitialize the comparison object.

For an alphabet-aware object, pass an alphabet of the original word type and a
same-sized {1} vector. For an index-word object, call ``init({1})`` or
``init()``; the latter clears the vector.

:returns: The first argument *self*.
:rtype: {0}

:raises AttributeError:
  if the arguments do not match the construction mode of *self*.
:raises LibsemigroupsError:
  if *alphabet* and *{1}* have different sizes.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, {0}
  >>> compare = {0}(Alphabet("ab"), [1, 2])
  >>> compare.init(Alphabet("ba"), [1, 2]) is compare
  True
)pbdoc",
                      name,
                      configuration)
              .c_str());
      thing.def(
          "__call__",
          [](Cmp const& self, Word const& x, Word const& y) {
            return self(x, y);
          },
          py::arg("x"),
          py::arg("y"),
          fmt::format(R"pbdoc(
:sig=(self: {0}, x: str | list[int], y: str | list[int]) -> bool:
Compare two words using {2}.

Index-word objects accept ``list[int]``. Alphabet-aware objects accept words
of the same type as their alphabet.

:returns: Whether *x* is less than *y*.
:rtype: bool

:raises TypeError:
  if the word types do not match the construction mode.
:raises LibsemigroupsError:
  if a word contains a letter not represented by the stored configuration.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, {0}
  >>> {0}([1, 2])([0], [1])
  True
  >>> {0}(Alphabet("ab"), [1, 2])("a", "b")
  True
)pbdoc",
                      name,
                      configuration,
                      ordering)
              .c_str());
      thing.def("alphabet",
                &Cmp::alphabet,
                fmt::format(R"pbdoc(
:sig=(self: {0}) -> Alphabet:
Return the stored alphabet.

:raises AttributeError:
  if *self* was constructed without an alphabet.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, {0}
  >>> alphabet = Alphabet("ab")
  >>> {0}(alphabet, [1, 2]).alphabet() == alphabet
  True
)pbdoc",
                            name)
                    .c_str(),
                py::return_value_policy::reference_internal);
      thing.def(
          configuration.c_str(),
          [get_configuration](Cmp const& self) {
            return (self.*get_configuration)();
          },
          fmt::format(R"pbdoc(
:sig=(self: {0}) -> list[int]:
Return the stored {1}.

.. doctest:: python

  >>> from libsemigroups_pybind11 import {0}
  >>> {0}([1, 2]).{1}()
  [1, 2]
)pbdoc",
                      name,
                      configuration)
              .c_str());
    }

    template <typename Word>
    void bind_lex_cmp_with_alphabet(py::module& m, char const* name) {
      using LexCmp_ = LexCmp<Word>;

      // The Python wrapper copies all documentation from this
      // specialization, except those given above.
      py::class_<LexCmp_> thing(m, name, R"pbdoc(
Compare words lexicographically.

Use ``LexCmp()`` to compare either ``str`` or ``list[int]`` words using the
natural order of their letters. Use ``LexCmp(alphabet)`` to compare words by
the positions of their letters in *alphabet*. The latter form copies
*alphabet* and only accepts words with the same type as *alphabet*.

.. note::
  The constructor fixes whether this object is alphabet-aware (i.e. constructed
  from an :any:`Alphabet` object). It also fixes the word type of an
  alphabet-aware object. In particular, ``LexCmp()`` only accepts ``init()``
  (which does nothing); it cannot be changed into an alphabet-aware object by
  calling ``init(alphabet)``. Similarly,
  ``LexCmp(alphabet).init(new_alphabet)`` requires *new_alphabet* to have the
  same word type as *alphabet*.

.. seealso::

  :any:`Alphabet`

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, LexCmp
  >>> LexCmp()("a", "b")
  True
  >>> LexCmp()([0], [1])
  True
  >>> LexCmp(Alphabet("ba"))("b", "a")
  True
)pbdoc");

      thing.def(py::init<Alphabet<Word> const&>(),
                py::arg("alphabet"),
                R"pbdoc(
:sig=(self: LexCmp, alphabet: Alphabet) -> None:
Construct a lexicographic comparison object from an alphabet.

Constructs an object whose call operator compares words by their positions in
*alphabet*. The type of letters in *alphabet* also fixes the accepted word type
for the call operator.

:param alphabet: the optional alphabet defining the order of letters.
:type alphabet: Alphabet

:raises TypeError:
  if more than one argument is given or the argument is not an :any:`Alphabet`.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, LexCmp
  >>> LexCmp(Alphabet("ba"))("b", "a")
  True
)pbdoc");

      thing.def("__repr__", [](LexCmp_ const& self) {
        return to_human_readable_repr(self);
      });

      thing.def("__copy__", [](LexCmp_ const& self) { return LexCmp_(self); });

      thing.def(
          "copy",
          [](LexCmp_ const& self) { return LexCmp_(self); },
          R"pbdoc(
:sig=(self: LexCmp) -> LexCmp:
Copy a comparison object.

The copy has the same mode and word type as *self*. For an alphabet-aware
object, the stored alphabet is also copied, so subsequently reinitializing one
comparison object does not affect the other.

:returns: A copy of *self*.
:rtype: LexCmp

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, LexCmp
  >>> LexCmp(Alphabet("ba")).copy()("b", "a")
  True
)pbdoc");

      thing.def(
          "init",
          [](LexCmp_& self, Alphabet<Word> const& alphabet) -> LexCmp_& {
            return self.init(alphabet);
          },
          py::arg("alphabet"),
          R"pbdoc(
:sig=(self: LexCmp, alphabet: Alphabet) -> LexCmp:
Reinitialize the comparison object.

If *self* was constructed using an :any:`Alphabet`, then ``init(new_alphabet)``
puts *self* back into the same state it would have been had it been newly
constructed from *alphabet*.

:param alphabet: the replacement alphabet.
:type alphabet: Alphabet

:returns: The first argument *self*.
:rtype: LexCmp

:raises TypeError:
  if the type of the words in the new alphabet is not the same as the existing alphabet.
:raises AttributeError:
  if *self* was constructed without an alphabet.

.. warning::
  This method only works if *self* was constructed as
  ``LexCmp(alphabet)``. An object constructed as ``LexCmp()`` does not have a
  stored alphabet.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, LexCmp
  >>> compare = LexCmp(Alphabet("ab"))
  >>> compare.init(Alphabet("ba")) is compare
  True
  >>> compare("b", "a")
  True
)pbdoc");

      thing.def(
          "__call__",
          [](LexCmp_ const& self, Word const& x, Word const& y) {
            return self(x, y);
          },
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(self: LexCmp, x: str | list[int], y: str | list[int]) -> bool:
Compare two words lexicographically.

If *self* was constructed as ``LexCmp()``, then *x* and *y* must either both
be strings or both be lists of integers, and letters are compared using their
natural order. If *self* was constructed using an :any:`Alphabet`, then *x* and
*y* must have the same type of words as *alphabet*, and letters are compared by
their positions in the alphabet.

:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises TypeError:
  if *x* and *y* are not both strings or both lists of integers, or if their
  type does not match the alphabet used to construct *self*.
:raises LibsemigroupsError:
  if *self* is alphabet-aware and either word contains a letter that does not
  belong to its alphabet.

.. doctest:: python

  >>> from libsemigroups_pybind11 import LexCmp
  >>> LexCmp()("ab", "ba")
  True
  >>> LexCmp()([0, 1], [1, 0])
  True
)pbdoc");

      thing.def("alphabet",
                &LexCmp_::alphabet,
                R"pbdoc(
:sig=(self: LexCmp) -> Alphabet:
Return the alphabet used to compare letters.

:returns: The stored alphabet.
:rtype: Alphabet

:raises AttributeError:
  if *self* was constructed without an alphabet.

.. warning::
  This method only works if *self* was constructed as
  ``LexCmp(alphabet)``. An object constructed as ``LexCmp()`` does not have a
  stored alphabet.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, LexCmp
  >>> alphabet = Alphabet("ba")
  >>> LexCmp(alphabet).alphabet() == alphabet
  True
)pbdoc",
                py::return_value_policy::reference_internal);
    }  // bind_lex_cmp_with_alphabet

    template <typename Word>
    void bind_lenlex_cmp_with_alphabet(py::module& m, char const* name) {
      using LenLexCmp_ = LenLexCmp<Word>;

      // The Python wrapper copies all documentation from this
      // specialization, except those given above.
      py::class_<LenLexCmp_> thing(m, name, R"pbdoc(
Compare words using len-lex ordering.

Words are first compared by length and then lexicographically. Use
``LenLexCmp()`` to compare either ``str`` or ``list[int]`` words using the
natural order of their letters. Use ``LenLexCmp(alphabet)`` to compare words
of equal length by the positions of their letters in *alphabet*. The latter
form copies *alphabet* and only accepts words with the same type as *alphabet*.

.. note::
  The constructor fixes whether this object is alphabet-aware (i.e. constructed
  from an :any:`Alphabet` object). It also fixes the word type of an
  alphabet-aware object. In particular, ``LenLexCmp()`` only accepts
  ``init()`` (which does nothing); it cannot be changed into an alphabet-aware
  object by calling ``init(alphabet)``. Similarly,
  ``LenLexCmp(alphabet).init(new_alphabet)`` requires *new_alphabet* to have
  the same word type as *alphabet*.

.. seealso::

  :any:`Alphabet`

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, LenLexCmp
  >>> LenLexCmp()("a", "b")
  True
  >>> LenLexCmp()([0], [1])
  True
  >>> LenLexCmp(Alphabet("ba"))("b", "a")
  True
)pbdoc");

      thing.def(py::init<Alphabet<Word> const&>(),
                py::arg("alphabet"),
                R"pbdoc(
:sig=(self: LenLexCmp, alphabet: Alphabet) -> None:
Construct a len-lex comparison object from an alphabet.

Constructs an object whose call operator first compares words by length and
then compares words of equal length by the positions of their letters in
*alphabet*. The type of letters in *alphabet* also fixes the accepted word
type for the call operator.

:param alphabet: the optional alphabet defining the order of letters.
:type alphabet: Alphabet

:raises TypeError:
  if more than one argument is given or the argument is not an :any:`Alphabet`.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, LenLexCmp
  >>> LenLexCmp(Alphabet("ba"))("b", "a")
  True
)pbdoc");

      thing.def("__repr__", [](LenLexCmp_ const& self) {
        return to_human_readable_repr(self);
      });

      thing.def("__copy__",
                [](LenLexCmp_ const& self) { return LenLexCmp_(self); });

      thing.def(
          "copy",
          [](LenLexCmp_ const& self) { return LenLexCmp_(self); },
          R"pbdoc(
:sig=(self: LenLexCmp) -> LenLexCmp:
Copy a comparison object.

The copy has the same mode and word type as *self*. For an alphabet-aware
object, the stored alphabet is also copied, so subsequently reinitializing one
comparison object does not affect the other.

:returns: A copy of *self*.
:rtype: LenLexCmp

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, LenLexCmp
  >>> LenLexCmp(Alphabet("ba")).copy()("b", "a")
  True
)pbdoc");

      thing.def(
          "init",
          [](LenLexCmp_& self, Alphabet<Word> const& alphabet) -> LenLexCmp_& {
            return self.init(alphabet);
          },
          py::arg("alphabet"),
          R"pbdoc(
:sig=(self: LenLexCmp, alphabet: Alphabet) -> LenLexCmp:
Reinitialize the comparison object.

If *self* was constructed using an :any:`Alphabet`, then
``init(new_alphabet)`` puts *self* back into the same state it would have been
had it been newly constructed from *alphabet*.

:param alphabet: the replacement alphabet.
:type alphabet: Alphabet

:returns: The first argument *self*.
:rtype: LenLexCmp

:raises TypeError:
  if the type of the words in the new alphabet is not the same as the existing
  alphabet.
:raises AttributeError:
  if *self* was constructed without an alphabet.

.. warning::
  This method only works if *self* was constructed as
  ``LenLexCmp(alphabet)``. An object constructed as ``LenLexCmp()`` does not have a
  stored alphabet.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, LenLexCmp
  >>> compare = LenLexCmp(Alphabet("ab"))
  >>> compare.init(Alphabet("ba")) is compare
  True
  >>> compare("b", "a")
  True
)pbdoc");

      thing.def(
          "__call__",
          [](LenLexCmp_ const& self, Word const& x, Word const& y) {
            return self(x, y);
          },
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(self: LenLexCmp, x: str | list[int], y: str | list[int]) -> bool:
Compare two words using len-lex ordering.

Words are first compared by length and then lexicographically. If *self* was
constructed as ``LenLexCmp()``, then *x* and *y* must either both be strings or
both be lists of integers, and letters are compared using their natural order.
If *self* was constructed using an :any:`Alphabet`, then *x* and *y* must have
the same type of words as *alphabet*, and letters are compared by their
positions in the alphabet.

:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises TypeError:
  if *x* and *y* are not both strings or both lists of integers, or if their
  type does not match the alphabet used to construct *self*.
:raises LibsemigroupsError:
  if *self* is alphabet-aware and either word contains a letter that does not
  belong to its alphabet.

.. doctest:: python

  >>> from libsemigroups_pybind11 import LenLexCmp
  >>> LenLexCmp()("z", "aa")
  True
  >>> LenLexCmp()([9], [0, 0])
  True
)pbdoc");

      thing.def("alphabet",
                &LenLexCmp_::alphabet,
                R"pbdoc(
:sig=(self: LenLexCmp) -> Alphabet:
Return the alphabet used to compare letters.

:returns: The stored alphabet.
:rtype: Alphabet

:raises AttributeError:
  if *self* was constructed without an alphabet.

.. warning::
  This method only works if *self* was constructed as
  ``LenLexCmp(alphabet)``. An object constructed as ``LenLexCmp()`` does not
  have a stored alphabet.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, LenLexCmp
  >>> alphabet = Alphabet("ba")
  >>> LenLexCmp(alphabet).alphabet() == alphabet
  True
)pbdoc",
                py::return_value_policy::reference_internal);
    }  // bind_lenlex_cmp_with_alphabet

    template <typename Word>
    void bind_rpo_cmp_with_alphabet(py::module& m, char const* name) {
      using RPOCmp_ = RPOCmp<Word>;

      // The Python wrapper copies all documentation from this specialization,
      // except those given above.
      py::class_<RPOCmp_> thing(m, name, R"pbdoc(
Compare words using recursive-path ordering.

Use ``RPOCmp()`` to compare either ``str`` or ``list[int]`` words using the
natural order of their letters. Use ``RPOCmp(alphabet)`` to compare words by
the positions of their letters in *alphabet*. The latter form copies
*alphabet* and only accepts words with the same type as *alphabet*.

.. note::
  The constructor fixes whether this object is alphabet-aware (i.e. constructed
  from an :any:`Alphabet` object). It also fixes the word type of an
  alphabet-aware object. In particular, ``RPOCmp()`` only accepts ``init()``
  (which does nothing); it cannot be changed into an alphabet-aware object by
  calling ``init(alphabet)``. Similarly,
  ``RPOCmp(alphabet).init(new_alphabet)`` requires *new_alphabet* to have the
  same word type as *alphabet*.

.. warning::
  This comparison has significantly worse performance than :any:`LenLexCmp`
  and :any:`LexCmp`.

.. seealso::

  :any:`Alphabet`

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, RPOCmp
  >>> RPOCmp()("a", "b")
  True
  >>> RPOCmp()([0], [1])
  True
  >>> RPOCmp(Alphabet("ba"))("b", "a")
  True
)pbdoc");

      thing.def(py::init<Alphabet<Word> const&>(),
                py::arg("alphabet"),
                R"pbdoc(
:sig=(self: RPOCmp, alphabet: Alphabet) -> None:
Construct a recursive-path comparison object from an alphabet.

Constructs an object whose call operator compares words using recursive-path
ordering and orders letters by their positions in *alphabet*. The type of
letters in *alphabet* also fixes the accepted word type for the call operator.

:param alphabet: the optional alphabet defining the order of letters.
:type alphabet: Alphabet

:raises TypeError:
  if more than one argument is given or the argument is not an :any:`Alphabet`.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, RPOCmp
  >>> RPOCmp(Alphabet("ba"))("b", "a")
  True
)pbdoc");

      thing.def("__repr__", [](RPOCmp_ const& self) {
        return to_human_readable_repr(self);
      });

      thing.def("__copy__", [](RPOCmp_ const& self) { return RPOCmp_(self); });

      thing.def(
          "copy",
          [](RPOCmp_ const& self) { return RPOCmp_(self); },
          R"pbdoc(
:sig=(self: RPOCmp) -> RPOCmp:
Copy a comparison object.

The copy has the same mode and word type as *self*. For an alphabet-aware
object, the stored alphabet is also copied, so subsequently reinitializing one
comparison object does not affect the other.

:returns: A copy of *self*.
:rtype: RPOCmp

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, RPOCmp
  >>> RPOCmp(Alphabet("ba")).copy()("b", "a")
  True
)pbdoc");

      thing.def(
          "init",
          [](RPOCmp_& self, Alphabet<Word> const& alphabet) -> RPOCmp_& {
            return self.init(alphabet);
          },
          py::arg("alphabet"),
          R"pbdoc(
:sig=(self: RPOCmp, alphabet: Alphabet) -> RPOCmp:
Reinitialize the comparison object.

If *self* was constructed using an :any:`Alphabet`, then
``init(new_alphabet)`` puts *self* back into the same state it would have been
had it been newly constructed from *alphabet*.

:param alphabet: the replacement alphabet.
:type alphabet: Alphabet

:returns: The first argument *self*.
:rtype: RPOCmp

:raises TypeError:
  if the type of the words in the new alphabet is not the same as the existing
  alphabet.
:raises AttributeError:
  if *self* was constructed without an alphabet.

.. warning::
  This method only works if *self* was constructed as
  ``RPOCmp(alphabet)``. An object constructed as ``RPOCmp()`` does not have a
  stored alphabet.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, RPOCmp
  >>> compare = RPOCmp(Alphabet("ab"))
  >>> compare.init(Alphabet("ba")) is compare
  True
  >>> compare("b", "a")
  True
)pbdoc");

      thing.def(
          "__call__",
          [](RPOCmp_ const& self, Word const& x, Word const& y) {
            return self(x, y);
          },
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(self: RPOCmp, x: str | list[int], y: str | list[int]) -> bool:
Compare two words using recursive-path ordering.

If *self* was constructed as ``RPOCmp()``, then *x* and *y* must either both
be strings or both be lists of integers, and letters are compared using their
natural order. If *self* was constructed using an :any:`Alphabet`, then *x* and
*y* must have the same type of words as *alphabet*, and letters are compared by
their positions in the alphabet.

:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises TypeError:
  if *x* and *y* are not both strings or both lists of integers, or if their
  type does not match the alphabet used to construct *self*.
:raises LibsemigroupsError:
  if *self* is alphabet-aware and either word contains a letter that does not
  belong to its alphabet.

.. warning::
  This comparison has significantly worse performance than :any:`LenLexCmp`
  and :any:`LexCmp`.

.. doctest:: python

  >>> from libsemigroups_pybind11 import RPOCmp
  >>> RPOCmp()("a", "b")
  True
  >>> RPOCmp()([0], [1])
  True
)pbdoc");

      thing.def("alphabet",
                &RPOCmp_::alphabet,
                R"pbdoc(
:sig=(self: RPOCmp) -> Alphabet:
Return the alphabet used to compare letters.

:returns: The stored alphabet.
:rtype: Alphabet

:raises AttributeError:
  if *self* was constructed without an alphabet.

.. warning::
  This method only works if *self* was constructed as
  ``RPOCmp(alphabet)``. An object constructed as ``RPOCmp()`` does not have a
  stored alphabet.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, RPOCmp
  >>> alphabet = Alphabet("ba")
  >>> RPOCmp(alphabet).alphabet() == alphabet
  True
)pbdoc",
                py::return_value_policy::reference_internal);
    }  // bind_rpo_cmp_with_alphabet

    template <typename Word>
    void bind_rev_rpo_cmp_with_alphabet(py::module& m, char const* name) {
      using RevRPOCmp_ = RevRPOCmp<Word>;

      // The Python wrapper copies all documentation from this specialization,
      // except those given above.
      py::class_<RevRPOCmp_> thing(m, name, R"pbdoc(
Compare words using reversed recursive-path ordering.

This is recursive-path ordering applied after reading both words from right to
left. Use ``RevRPOCmp()`` to compare either ``str`` or ``list[int]`` words
using the natural order of their letters. Use ``RevRPOCmp(alphabet)`` to
compare words by the positions of their letters in *alphabet*. The latter form
copies *alphabet* and only accepts words with the same type as *alphabet*.

.. note::
  The constructor fixes whether this object is alphabet-aware (i.e. constructed
  from an :any:`Alphabet` object). It also fixes the word type of an
  alphabet-aware object. In particular, ``RevRPOCmp()`` only accepts
  ``init()`` (which does nothing); it cannot be changed into an alphabet-aware
  object by calling ``init(alphabet)``. Similarly,
  ``RevRPOCmp(alphabet).init(new_alphabet)`` requires *new_alphabet* to have
  the same word type as *alphabet*.

.. warning::
  This comparison has significantly worse performance than :any:`LenLexCmp`
  and :any:`LexCmp`.

.. seealso::

  :any:`Alphabet`

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, RevRPOCmp
  >>> RevRPOCmp()("ab", "ba")
  True
  >>> RevRPOCmp()([0, 1], [1, 0])
  True
  >>> RevRPOCmp(Alphabet("ba"))("b", "a")
  True
)pbdoc");

      thing.def(py::init<Alphabet<Word> const&>(),
                py::arg("alphabet"),
                R"pbdoc(
:sig=(self: RevRPOCmp, alphabet: Alphabet) -> None:
Construct a reversed recursive-path comparison object from an alphabet.

Constructs an object whose call operator compares words using reversed
recursive-path ordering and orders letters by their positions in *alphabet*.
The type of letters in *alphabet* also fixes the accepted word type for the
call operator.

:param alphabet: the optional alphabet defining the order of letters.
:type alphabet: Alphabet

:raises TypeError:
  if more than one argument is given or the argument is not an :any:`Alphabet`.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, RevRPOCmp
  >>> RevRPOCmp(Alphabet("ba"))("b", "a")
  True
)pbdoc");

      thing.def("__repr__", [](RevRPOCmp_ const& self) {
        return to_human_readable_repr(self);
      });

      thing.def("__copy__",
                [](RevRPOCmp_ const& self) { return RevRPOCmp_(self); });

      thing.def(
          "copy",
          [](RevRPOCmp_ const& self) { return RevRPOCmp_(self); },
          R"pbdoc(
:sig=(self: RevRPOCmp) -> RevRPOCmp:
Copy a comparison object.

The copy has the same mode and word type as *self*. For an alphabet-aware
object, the stored alphabet is also copied, so subsequently reinitializing one
comparison object does not affect the other.

:returns: A copy of *self*.
:rtype: RevRPOCmp

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, RevRPOCmp
  >>> RevRPOCmp(Alphabet("ba")).copy()("b", "a")
  True
)pbdoc");

      thing.def(
          "init",
          [](RevRPOCmp_& self, Alphabet<Word> const& alphabet) -> RevRPOCmp_& {
            return self.init(alphabet);
          },
          py::arg("alphabet"),
          R"pbdoc(
:sig=(self: RevRPOCmp, alphabet: Alphabet) -> RevRPOCmp:
Reinitialize the comparison object.

If *self* was constructed using an :any:`Alphabet`, then
``init(new_alphabet)`` puts *self* back into the same state it would have been
had it been newly constructed from *alphabet*.

:param alphabet: the replacement alphabet.
:type alphabet: Alphabet

:returns: The first argument *self*.
:rtype: RevRPOCmp

:raises TypeError:
  if the type of the words in the new alphabet is not the same as the existing
  alphabet.
:raises AttributeError:
  if *self* was constructed without an alphabet.

.. warning::
  This method only works if *self* was constructed as
  ``RevRPOCmp(alphabet)``. An object constructed as ``RevRPOCmp()`` does not
  have a stored alphabet.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, RevRPOCmp
  >>> compare = RevRPOCmp(Alphabet("ab"))
  >>> compare.init(Alphabet("ba")) is compare
  True
  >>> compare("b", "a")
  True
)pbdoc");

      thing.def(
          "__call__",
          [](RevRPOCmp_ const& self, Word const& x, Word const& y) {
            return self(x, y);
          },
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(self: RevRPOCmp, x: str | list[int], y: str | list[int]) -> bool:
Compare two words using reversed recursive-path ordering.

This is recursive-path ordering applied after reading both words from right to
left. If *self* was constructed as ``RevRPOCmp()``, then *x* and *y* must
either both be strings or both be lists of integers, and letters are compared
using their natural order. If *self* was constructed using an :any:`Alphabet`,
then *x* and *y* must have the same type of words as *alphabet*, and letters are
compared by their positions in the alphabet.

:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises TypeError:
  if *x* and *y* are not both strings or both lists of integers, or if their
  type does not match the alphabet used to construct *self*.
:raises LibsemigroupsError:
  if *self* is alphabet-aware and either word contains a letter that does not
  belong to its alphabet.

.. warning::
  This comparison has significantly worse performance than :any:`LenLexCmp`
  and :any:`LexCmp`.

.. doctest:: python

  >>> from libsemigroups_pybind11 import RevRPOCmp
  >>> RevRPOCmp()("ab", "ba")
  True
  >>> RevRPOCmp()([0, 1], [1, 0])
  True
)pbdoc");

      thing.def("alphabet",
                &RevRPOCmp_::alphabet,
                R"pbdoc(
:sig=(self: RevRPOCmp) -> Alphabet:
Return the alphabet used to compare letters.

:returns: The stored alphabet.
:rtype: Alphabet

:raises AttributeError:
  if *self* was constructed without an alphabet.

.. warning::
  This method only works if *self* was constructed as
  ``RevRPOCmp(alphabet)``. An object constructed as ``RevRPOCmp()`` does not
  have a stored alphabet.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, RevRPOCmp
  >>> alphabet = Alphabet("ba")
  >>> RevRPOCmp(alphabet).alphabet() == alphabet
  True
)pbdoc",
                py::return_value_policy::reference_internal);
    }  // bind_rev_rpo_cmp_with_alphabet

    template <typename Word>
    void bind_rev_lex_cmp_with_alphabet(py::module& m, char const* name) {
      using RevLexCmp_ = RevLexCmp<Word>;

      // The Python wrapper copies all documentation from this specialization,
      // except those given above.
      py::class_<RevLexCmp_> thing(m, name, R"pbdoc(
Compare words using reversed lexicographic ordering.

This is lexicographic ordering applied after reading both words from right to
left.

Use ``RevLexCmp()`` to compare either ``str`` or ``list[int]`` words using the
natural order of their letters. Use ``RevLexCmp(alphabet)`` to compare words by
the positions of their letters in *alphabet*. The latter form copies
*alphabet* and only accepts words with the same type as *alphabet*.

.. note::
  The constructor fixes whether this object is alphabet-aware (i.e. constructed
  from an :any:`Alphabet` object). It also fixes the word type of an
  alphabet-aware object. In particular, ``RevLexCmp()`` only accepts ``init()``
  (which does nothing); it cannot be changed into an alphabet-aware object by
  calling ``init(alphabet)``. Similarly,
  ``RevLexCmp(alphabet).init(new_alphabet)`` requires *new_alphabet* to have the
  same word type as *alphabet*.

.. seealso::

  :any:`Alphabet`

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, RevLexCmp
  >>> RevLexCmp()("a", "b")
  True
  >>> RevLexCmp()([0], [1])
  True
  >>> RevLexCmp(Alphabet("ba"))("b", "a")
  True
)pbdoc");

      thing.def(py::init<Alphabet<Word> const&>(),
                py::arg("alphabet"),
                R"pbdoc(
:sig=(self: RevLexCmp, alphabet: Alphabet) -> None:
Construct a reversed lexicographic comparison object from an alphabet.

Constructs an object whose call operator compares words by their positions in
*alphabet*. The type of letters in *alphabet* also fixes the accepted word type
for the call operator.

:param alphabet: the optional alphabet defining the order of letters.
:type alphabet: Alphabet

:raises TypeError:
  if more than one argument is given or the argument is not an :any:`Alphabet`.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, RevLexCmp
  >>> RevLexCmp(Alphabet("ba"))("b", "a")
  True
)pbdoc");

      thing.def("__repr__", [](RevLexCmp_ const& self) {
        return to_human_readable_repr(self);
      });

      thing.def("__copy__",
                [](RevLexCmp_ const& self) { return RevLexCmp_(self); });

      thing.def(
          "copy",
          [](RevLexCmp_ const& self) { return RevLexCmp_(self); },
          R"pbdoc(
:sig=(self: RevLexCmp) -> RevLexCmp:
Copy a comparison object.

The copy has the same mode and word type as *self*. For an alphabet-aware
object, the stored alphabet is also copied, so subsequently reinitializing one
comparison object does not affect the other.

:returns: A copy of *self*.
:rtype: RevLexCmp

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, RevLexCmp
  >>> RevLexCmp(Alphabet("ba")).copy()("b", "a")
  True
)pbdoc");

      thing.def(
          "init",
          [](RevLexCmp_& self, Alphabet<Word> const& alphabet) -> RevLexCmp_& {
            return self.init(alphabet);
          },
          py::arg("alphabet"),
          R"pbdoc(
:sig=(self: RevLexCmp, alphabet: Alphabet) -> RevLexCmp:
Reinitialize the comparison object.

If *self* was constructed using an :any:`Alphabet`, then ``init(new_alphabet)``
puts *self* back into the same state it would have been had it been newly
constructed from *alphabet*.

:param alphabet: the replacement alphabet.
:type alphabet: Alphabet

:returns: The first argument *self*.
:rtype: RevLexCmp

:raises TypeError:
  if the type of the words in the new alphabet is not the same as the existing alphabet.
:raises AttributeError:
  if *self* was constructed without an alphabet.

.. warning::
  This method only works if *self* was constructed as
  ``RevLexCmp(alphabet)``. An object constructed as ``RevLexCmp()`` does not have a
  stored alphabet.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, RevLexCmp
  >>> compare = RevLexCmp(Alphabet("ab"))
  >>> compare.init(Alphabet("ba")) is compare
  True
  >>> compare("b", "a")
  True
)pbdoc");

      thing.def(
          "__call__",
          [](RevLexCmp_ const& self, Word const& x, Word const& y) {
            return self(x, y);
          },
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(self: RevLexCmp, x: str | list[int], y: str | list[int]) -> bool:
Compare two words using reversed lexicographic ordering.

This is lexicographic ordering applied after reading both words from right to
left.

If *self* was constructed as ``RevLexCmp()``, then *x* and *y* must either both
be strings or both be lists of integers, and letters are compared using their
natural order. If *self* was constructed using an :any:`Alphabet`, then *x* and
*y* must have the same type of words as *alphabet*, and letters are compared by
their positions in the alphabet.

:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises TypeError:
  if *x* and *y* are not both strings or both lists of integers, or if their
  type does not match the alphabet used to construct *self*.
:raises LibsemigroupsError:
  if *self* is alphabet-aware and either word contains a letter that does not
  belong to its alphabet.

.. doctest:: python

  >>> from libsemigroups_pybind11 import RevLexCmp
  >>> RevLexCmp()("ba", "ab")
  True
  >>> RevLexCmp()([1, 0], [0, 1])
  True
)pbdoc");

      thing.def("alphabet",
                &RevLexCmp_::alphabet,
                R"pbdoc(
:sig=(self: RevLexCmp) -> Alphabet:
Return the alphabet used to compare letters.

:returns: The stored alphabet.
:rtype: Alphabet

:raises AttributeError:
  if *self* was constructed without an alphabet.

.. warning::
  This method only works if *self* was constructed as
  ``RevLexCmp(alphabet)``. An object constructed as ``RevLexCmp()`` does not have a
  stored alphabet.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, RevLexCmp
  >>> alphabet = Alphabet("ba")
  >>> RevLexCmp(alphabet).alphabet() == alphabet
  True
)pbdoc",
                py::return_value_policy::reference_internal);
    }  // bind_rev_lex_cmp_with_alphabet

    template <typename Word>
    void bind_rev_lenlex_cmp_with_alphabet(py::module& m, char const* name) {
      using RevLenLexCmp_ = RevLenLexCmp<Word>;

      // The Python wrapper copies all documentation from this specialization,
      // except those given above.
      py::class_<RevLenLexCmp_> thing(m, name, R"pbdoc(
Compare words using reversed len-lex ordering.

Words are first compared by length and then lexicographically after being read
from right to left. Use ``RevLenLexCmp()`` to compare either ``str`` or
``list[int]`` words using the natural order of their letters. Use
``RevLenLexCmp(alphabet)`` to compare words of equal length by the positions of
their letters in *alphabet*. The latter form copies *alphabet* and only accepts
words with the same type as *alphabet*.

.. note::
  The constructor fixes whether this object is alphabet-aware (i.e. constructed
  from an :any:`Alphabet` object). It also fixes the word type of an
  alphabet-aware object. In particular, ``RevLenLexCmp()`` only accepts
  ``init()`` (which does nothing); it cannot be changed into an alphabet-aware
  object by calling ``init(alphabet)``. Similarly,
  ``RevLenLexCmp(alphabet).init(new_alphabet)`` requires *new_alphabet* to have
  the same word type as *alphabet*.

.. seealso::

  :any:`Alphabet`

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, RevLenLexCmp
  >>> RevLenLexCmp()("a", "b")
  True
  >>> RevLenLexCmp()([0], [1])
  True
  >>> RevLenLexCmp(Alphabet("ba"))("b", "a")
  True
)pbdoc");

      thing.def(py::init<Alphabet<Word> const&>(),
                py::arg("alphabet"),
                R"pbdoc(
:sig=(self: RevLenLexCmp, alphabet: Alphabet) -> None:
Construct a reversed len-lex comparison object from an alphabet.

Constructs an object whose call operator first compares words by length and
then compares words of equal length from right to left by the positions of
their letters in *alphabet*. The type of letters in *alphabet* also fixes the
accepted word type for the call operator.

:param alphabet: the optional alphabet defining the order of letters.
:type alphabet: Alphabet

:raises TypeError:
  if more than one argument is given or the argument is not an :any:`Alphabet`.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, RevLenLexCmp
  >>> RevLenLexCmp(Alphabet("ba"))("b", "a")
  True
)pbdoc");

      thing.def("__repr__", [](RevLenLexCmp_ const& self) {
        return to_human_readable_repr(self);
      });

      thing.def("__copy__",
                [](RevLenLexCmp_ const& self) { return RevLenLexCmp_(self); });

      thing.def(
          "copy",
          [](RevLenLexCmp_ const& self) { return RevLenLexCmp_(self); },
          R"pbdoc(
:sig=(self: RevLenLexCmp) -> RevLenLexCmp:
Copy a comparison object.

The copy has the same mode and word type as *self*. For an alphabet-aware
object, the stored alphabet is also copied, so subsequently reinitializing one
comparison object does not affect the other.

:returns: A copy of *self*.
:rtype: RevLenLexCmp

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, RevLenLexCmp
  >>> RevLenLexCmp(Alphabet("ba")).copy()("b", "a")
  True
)pbdoc");

      thing.def(
          "init",
          [](RevLenLexCmp_& self, Alphabet<Word> const& alphabet)
              -> RevLenLexCmp_& { return self.init(alphabet); },
          py::arg("alphabet"),
          R"pbdoc(
:sig=(self: RevLenLexCmp, alphabet: Alphabet) -> RevLenLexCmp:
Reinitialize the comparison object.

If *self* was constructed using an :any:`Alphabet`, then
``init(new_alphabet)`` puts *self* back into the same state it would have been
had it been newly constructed from *alphabet*.

:param alphabet: the replacement alphabet.
:type alphabet: Alphabet

:returns: The first argument *self*.
:rtype: RevLenLexCmp

:raises TypeError:
  if the type of the words in the new alphabet is not the same as the existing
  alphabet.
:raises AttributeError:
  if *self* was constructed without an alphabet.

.. warning::
  This method only works if *self* was constructed as
  ``RevLenLexCmp(alphabet)``. An object constructed as ``RevLenLexCmp()`` does not have a
  stored alphabet.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, RevLenLexCmp
  >>> compare = RevLenLexCmp(Alphabet("ab"))
  >>> compare.init(Alphabet("ba")) is compare
  True
  >>> compare("b", "a")
  True
)pbdoc");

      thing.def(
          "__call__",
          [](RevLenLexCmp_ const& self, Word const& x, Word const& y) {
            return self(x, y);
          },
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(
:sig=(self: RevLenLexCmp, x: str | list[int], y: str | list[int]) -> bool:
Compare two words using reversed len-lex ordering.

Words are first compared by length and then lexicographically after being read
from right to left. If *self* was constructed as ``RevLenLexCmp()``, then *x*
and *y* must either both be strings or both be lists of integers, and letters
are compared using their natural order. If *self* was constructed using an
:any:`Alphabet`, then *x* and *y* must have the same type of words as
*alphabet*, and letters are compared by their positions in the alphabet.

:param x: the first word.
:type x: str | list[int]
:param y: the second word.
:type y: str | list[int]
:returns: Whether *x* is less than *y*.
:rtype: bool

:raises TypeError:
  if *x* and *y* are not both strings or both lists of integers, or if their
  type does not match the alphabet used to construct *self*.
:raises LibsemigroupsError:
  if *self* is alphabet-aware and either word contains a letter that does not
  belong to its alphabet.

.. doctest:: python

  >>> from libsemigroups_pybind11 import RevLenLexCmp
  >>> RevLenLexCmp()("ba", "ab")
  True
  >>> RevLenLexCmp()([1, 0], [0, 1])
  True
)pbdoc");

      thing.def("alphabet",
                &RevLenLexCmp_::alphabet,
                R"pbdoc(
:sig=(self: RevLenLexCmp) -> Alphabet:
Return the alphabet used to compare letters.

:returns: The stored alphabet.
:rtype: Alphabet

:raises AttributeError:
  if *self* was constructed without an alphabet.

.. warning::
  This method only works if *self* was constructed as
  ``RevLenLexCmp(alphabet)``. An object constructed as ``RevLenLexCmp()`` does not
  have a stored alphabet.

.. doctest:: python

  >>> from libsemigroups_pybind11 import Alphabet, RevLenLexCmp
  >>> alphabet = Alphabet("ba")
  >>> RevLenLexCmp(alphabet).alphabet() == alphabet
  True
)pbdoc",
                py::return_value_policy::reference_internal);
    }  // bind_rev_lenlex_cmp_with_alphabet
  }    // namespace

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

    bind_cmp_default<LexCmp<>>(m, "Lex");
    bind_cmp_default<RevLexCmp<>>(m, "RevLex");
    bind_cmp_default<LenLexCmp<>>(m, "LenLex");
    bind_cmp_default<RevLenLexCmp<>>(m, "RevLenLex");
    bind_cmp_default<RPOCmp<>>(m, "RPO");
    bind_cmp_default<RevRPOCmp<>>(m, "RevRPO");

    bind_lex_cmp_with_alphabet<std::string>(m, "LexCmpString");
    bind_lex_cmp_with_alphabet<word_type>(m, "LexCmpWord");

    bind_rev_lex_cmp_with_alphabet<std::string>(m, "RevLexCmpString");
    bind_rev_lex_cmp_with_alphabet<word_type>(m, "RevLexCmpWord");

    bind_lenlex_cmp_with_alphabet<std::string>(m, "LenLexCmpString");
    bind_lenlex_cmp_with_alphabet<word_type>(m, "LenLexCmpWord");

    bind_rev_lenlex_cmp_with_alphabet<std::string>(m, "RevLenLexCmpString");
    bind_rev_lenlex_cmp_with_alphabet<word_type>(m, "RevLenLexCmpWord");

    bind_rpo_cmp_with_alphabet<std::string>(m, "RPOCmpString");
    bind_rpo_cmp_with_alphabet<word_type>(m, "RPOCmpWord");

    bind_rev_rpo_cmp_with_alphabet<std::string>(m, "RevRPOCmpString");
    bind_rev_rpo_cmp_with_alphabet<word_type>(m, "RevRPOCmpWord");

    bind_configured_cmp_default<WrCmp<>>(
        m, "WrCmp", "levels", &WrCmp<>::levels);
    bind_configured_cmp_with_alphabet<WrCmp<std::string>, std::string>(
        m,
        "WrCmpString",
        "WrCmp",
        "levels",
        "wreath-product ordering",
        &WrCmp<std::string>::levels);
    bind_configured_cmp_with_alphabet<WrCmp<word_type>, word_type>(
        m,
        "WrCmpWord",
        "WrCmp",
        "levels",
        "wreath-product ordering",
        &WrCmp<word_type>::levels);

    bind_configured_cmp_default<RevWrCmp<>>(
        m, "RevWrCmp", "levels", &RevWrCmp<>::levels);
    bind_configured_cmp_with_alphabet<RevWrCmp<std::string>, std::string>(
        m,
        "RevWrCmpString",
        "RevWrCmp",
        "levels",
        "reversed wreath-product ordering",
        &RevWrCmp<std::string>::levels);
    bind_configured_cmp_with_alphabet<RevWrCmp<word_type>, word_type>(
        m,
        "RevWrCmpWord",
        "RevWrCmp",
        "levels",
        "reversed wreath-product ordering",
        &RevWrCmp<word_type>::levels);

    bind_configured_cmp_default<WtLenLexCmp<>>(
        m, "WtLenLexCmp", "weights", &WtLenLexCmp<>::weights);
    bind_configured_cmp_with_alphabet<WtLenLexCmp<std::string>, std::string>(
        m,
        "WtLenLexCmpString",
        "WtLenLexCmp",
        "weights",
        "weighted len-lex ordering",
        &WtLenLexCmp<std::string>::weights);
    bind_configured_cmp_with_alphabet<WtLenLexCmp<word_type>, word_type>(
        m,
        "WtLenLexCmpWord",
        "WtLenLexCmp",
        "weights",
        "weighted len-lex ordering",
        &WtLenLexCmp<word_type>::weights);

    bind_configured_cmp_default<RevWtLenLexCmp<>>(
        m, "RevWtLenLexCmp", "weights", &RevWtLenLexCmp<>::weights);
    bind_configured_cmp_with_alphabet<RevWtLenLexCmp<std::string>, std::string>(
        m,
        "RevWtLenLexCmpString",
        "RevWtLenLexCmp",
        "weights",
        "reversed weighted len-lex ordering",
        &RevWtLenLexCmp<std::string>::weights);
    bind_configured_cmp_with_alphabet<RevWtLenLexCmp<word_type>, word_type>(
        m,
        "RevWtLenLexCmpWord",
        "RevWtLenLexCmp",
        "weights",
        "reversed weighted len-lex ordering",
        &RevWtLenLexCmp<word_type>::weights);

    bind_configured_cmp_default<WtLexCmp<>>(
        m, "WtLexCmp", "weights", &WtLexCmp<>::weights);
    bind_configured_cmp_with_alphabet<WtLexCmp<std::string>, std::string>(
        m,
        "WtLexCmpString",
        "WtLexCmp",
        "weights",
        "weighted lexicographic ordering",
        &WtLexCmp<std::string>::weights);
    bind_configured_cmp_with_alphabet<WtLexCmp<word_type>, word_type>(
        m,
        "WtLexCmpWord",
        "WtLexCmp",
        "weights",
        "weighted lexicographic ordering",
        &WtLexCmp<word_type>::weights);

    bind_configured_cmp_default<RevWtLexCmp<>>(
        m, "RevWtLexCmp", "weights", &RevWtLexCmp<>::weights);
    bind_configured_cmp_with_alphabet<RevWtLexCmp<std::string>, std::string>(
        m,
        "RevWtLexCmpString",
        "RevWtLexCmp",
        "weights",
        "reversed weighted lexicographic ordering",
        &RevWtLexCmp<std::string>::weights);
    bind_configured_cmp_with_alphabet<RevWtLexCmp<word_type>, word_type>(
        m,
        "RevWtLexCmpWord",
        "RevWtLexCmp",
        "weights",
        "reversed weighted lexicographic ordering",
        &RevWtLexCmp<word_type>::weights);

    bind_configured_cmp_default<LenWtLexCmp<>>(
        m, "LenWtLexCmp", "weights", &LenWtLexCmp<>::weights);
    bind_configured_cmp_with_alphabet<LenWtLexCmp<std::string>, std::string>(
        m,
        "LenWtLexCmpString",
        "LenWtLexCmp",
        "weights",
        "length then weighted lexicographic ordering",
        &LenWtLexCmp<std::string>::weights);
    bind_configured_cmp_with_alphabet<LenWtLexCmp<word_type>, word_type>(
        m,
        "LenWtLexCmpWord",
        "LenWtLexCmp",
        "weights",
        "length then weighted lexicographic ordering",
        &LenWtLexCmp<word_type>::weights);

    bind_configured_cmp_default<RevLenWtLexCmp<>>(
        m, "RevLenWtLexCmp", "weights", &RevLenWtLexCmp<>::weights);
    bind_configured_cmp_with_alphabet<RevLenWtLexCmp<std::string>, std::string>(
        m,
        "RevLenWtLexCmpString",
        "RevLenWtLexCmp",
        "weights",
        "length then reversed weighted lexicographic ordering",
        &RevLenWtLexCmp<std::string>::weights);
    bind_configured_cmp_with_alphabet<RevLenWtLexCmp<word_type>, word_type>(
        m,
        "RevLenWtLexCmpWord",
        "RevLenWtLexCmp",
        "weights",
        "length then reversed weighted lexicographic ordering",
        &RevLenWtLexCmp<word_type>::weights);
  }
}  // namespace libsemigroups
