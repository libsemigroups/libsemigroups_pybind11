//
// libsemigroups_pybind11
// Copyright (C) 2024 James D. Mitchell
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
#include <libsemigroups/bipart.hpp>

// pybind11....
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_blocks, init_bipart

namespace libsemigroups {
  namespace py = pybind11;

  void init_blocks(py::module& m) {
    py::class_<Blocks> thing(m,
                             "Blocks",
                             R"pbdoc(
A :any:`Blocks` object represents a signed partition of the set :math:`\{0,
\ldots, n - 1\}`.

It is possible to associate to every :any:`Bipartition` a pair of blocks,
:any:`Bipartition.left_blocks()` and :any:`Bipartition.right_blocks()`,
which determine the Green's :math:`\mathscr{L}`- and
:math:`\mathscr{R}`-classes of the :any:`Bipartition` in the monoid of all
bipartitions. This is the purpose of this class.

See also :any:`Bipartition` for more details and context.
)pbdoc");
    thing.def("__repr__", [](Blocks const& self) {
      return to_human_readable_repr(self, "[]");
    });
    thing.def(py::self != py::self);
    thing.def(py::self < py::self);
    thing.def(py::self == py::self);

    thing.def("__copy__", [](Blocks const& self) { return Blocks(self); });
    thing.def("__hash__", &Blocks::hash_value, py::is_operator());

    thing.def(
        "copy",
        [](Blocks const& self) { return Blocks(self); },
        R"pbdoc(
Copy a Blocks object.

:returns: A copy.
:rtype: Blocks
)pbdoc");

    // We omit the other constructors because in python we can't do anything
    // with them.

    thing.def(py::init([](std::vector<std::vector<int32_t>> const& blocks) {
                return make<Blocks>(blocks);
              }),
              py::arg("blocks"),
              R"pbdoc(
:sig=(self: Blocks, blocks: list[list[int]]) -> None:

Constructs a Blocks object from a list of lists of integers.

This function constructs a :any:`Blocks` object from a list of lists of
integers, so that the blocks consisting of negative values are
transverse and those consisting of positive values are not.

:param blocks: the blocks
:type blocks: list[list[int]]

:raises LibsemigroupsError:
  if the set consisting of the absolute values of the entries in *blocks* is
  not :math:`\{1, \ldots, n\}` where :math:`n` is the maximum such value.

:raises LibsemigroupsError:  if ``0`` is an item in any block.

:raises LibsemigroupsError:  if any block is empty.

:raises LibsemigroupsError:  if any block contains both negative and positive values.

:raises LibsemigroupsError:  if the constructed :any:`Blocks` object is not valid.

:complexity:
  linear in the sum of the sizes of the lists in blocks.
)pbdoc");

    thing.def(
        "iterator",
        [](Blocks const& self) {
          return py::make_iterator(self.cbegin(), self.cend());
        },
        R"pbdoc(
Return a const iterator yielding the indices of the blocks.

:returns:
   An iterator yielding the indices of blocks.
:rtype:
   collections.abc.Iterator[int]

:complexity:
   Constant.
)pbdoc");

    thing.def("degree",
              &Blocks::degree,
              R"pbdoc(
Return the degree of a blocks object. The *degree* of a :any:`Blocks`
object is the size of the set of which it is a partition, or the size of
the ``blocks`` used to construct *self*.

:returns:
   The degree of a :any:`Blocks` object.
:rtype:
   int
)pbdoc");

    thing.def(
        "is_transverse_block",
        [](Blocks const& self, size_t index) {
          return self.is_transverse_block(index);
        },
        py::arg("index"),
        R"pbdoc(
:sig=(self: Blocks, index: int) -> bool:

Check if a block is a transverse block.

This function returns ``True`` if the block with index *index* is a transverse
(or signed) block and it returns ``False`` if it is not transverse (or
unsigned).

:param index: the index of a block
:type index: int

:returns: Whether or not the given block is transverse.
:rtype: bool

:raises LibsemigroupsError:
    if *index* is not in the range :math:`[0, n)` where
    :math:`n` is the return value of :any:`number_of_blocks`.

:complexity: Constant.

)pbdoc");

    thing.def("lookup",
              &Blocks::lookup,
              R"pbdoc(
Returns the transverse blocks lookup.

This function returns the transverse blocks lookup. The value in position ``i``
of the returned list is ``True`` if the block with index ``i`` is transverse
and ``False`` if it is not.

:returns:
   The transverse block lookup list.
:rtype:
   list[bool]

:complexity:
   Constant.
)pbdoc");

    thing.def("number_of_blocks",
              &Blocks::number_of_blocks,
              R"pbdoc(
Return the number of blocks in a Blocks object. This function returns the
number of parts in the partition that instances of this class represent.

:returns:
   The number of blocks.
:rtype:
   int

:complexity:
   At worst :math:`O(n)` where :math:`n` is :any:`degree()`.
)pbdoc");

    thing.def("rank",
              &Blocks::rank,
              R"pbdoc(
Return the number of transverse blocks. This function returns the number
of ``True`` values in :any:`lookup()`.

:returns:
   The number of signed (transverse) blocks in *self*.
:rtype:
   int

:complexity:
   At most linear in the number of blocks.
)pbdoc");

    thing.def(
        "__getitem__",
        [](Blocks const& a, size_t b) { return a.at(b); },
        py::is_operator());

    ////////////////////////////////////////////////////////////////////////
    // Helpers
    ////////////////////////////////////////////////////////////////////////

    m.def("blocks_underlying_partition",
          &blocks::underlying_partition,
          py::arg("x"),
          R"pbdoc(
:sig=(x: Blocks) -> list[list[int]]:
Return the underlying partition of a :any:`Blocks` object. The *underlying
partition* of a :any:`Blocks` object *x* is the partition of a subset
:math:`P` of :math:`\{-n, \ldots, -1\}\cup \{1, \ldots, n\}` such that:

- :math:`\{|y|\mid y\in P\} = \{1, \ldots, n\}`;
- a block of the partition consists of negative numbers if and only if
  the corresponding block of *x* is a transverse block.

:param x: the Blocks object.
:type x: Blocks

:returns:
   The underlying partition of the parameter *x*.
:rtype:
   list[list[int]]

:complexity:
   :math:`O(n)` where :math:`n` is the ``x.degree()``.
)pbdoc");
  }  // init_blocks

  void init_bipart(py::module& m) {
    py::class_<Bipartition> thing(m,
                                  "Bipartition",
                                  R"pbdoc(
Class for representing bipartitions.

A *bipartition* is a partition of the set :math:`\{0, ..., 2n - 1\}`
for some non-negative integer :math:`n`; see the
`Semigroups package for GAP documentation
<https://semigroups.github.io/Semigroups/doc/chap3_mj.html>`_
for more details.
)pbdoc");

    thing.def("__repr__", [](Bipartition const& self) {
      return to_human_readable_repr(self, "[]");
    });
    thing.def("__copy__",
              [](Bipartition const& self) { return Bipartition(self); });
    thing.def("__hash__", &Bipartition::hash_value, py::is_operator());
    thing.def(
        "__getitem__",
        [](Bipartition const& a, size_t b) { return a.at(b); },
        py::is_operator());
    thing.def(py::self < py::self);
    thing.def(py::self == py::self);
    thing.def(py::self * py::self);

    thing.def(
        "copy",
        [](Bipartition const& self) { return Bipartition(self); },
        R"pbdoc(
Copy a Bipartition object.

:returns: A copy.
:rtype: Bipartition
)pbdoc");

    // We omit the other constructors because in python we can't do anything
    // with them.

    thing.def(py::init([](std::vector<std::vector<int32_t>> const& blocks) {
                return make<Bipartition>(blocks);
              }),
              py::arg("blocks"),
              R"pbdoc(
:sig=(self: Bipartition, blocks: list[list[int]]) -> None:

Construct a bipartition from a partition.

The items in *blocks* should be:

*  duplicate-free;
*  pairwise disjoint; and
*  partition the set :math:`\{-n, \ldots,  -1, 1, \ldots, n\}` for some
   positive integer :math:`n`.

:param blocks: the underlying partition.
:type blocks: list[list[int]]

:raises LibsemigroupsError: if any of the conditions above is not met.
)pbdoc");

    thing.def(py::init([](std::vector<uint32_t> const& lookup) {
                return make<Bipartition>(lookup);
              }),
              py::arg("lookup"),
              R"pbdoc(
:sig=(self: Bipartition, lookup: list[int]) -> None:

Construct a bipartition from a blocks lookup.

The parameter *lookup*:

*  is copied;
*  must have length :math:`2n` for some positive integer :math:`n`;
*  consist of non-negative integers; and
*  have the property that if :math:`i`, :math:`i > 0` occurs in *lookup*,
   then :math:`i - 1` occurs earlier in *lookup*. The value of ``blocks[i]``
   should represent the index of the block containing ``i``.

For example, if *lookup* is ``[0, 1, 1, 2, 1, 1, 3, 1, 1, 4, 5, 6]``, then
the above conditions are satisfied, but if *lookup* is ``[1, 0, 1, 10]`` then
they are not.

:param lookup: a lookup for the blocks of the bipartition being constructed.
:type lookup: list[int]

:raises LibsemigroupsError: if any of the conditions above is not met.
)pbdoc");

    thing.def(
        "iterator",
        [](Bipartition const& self) {
          return py::make_iterator(self.cbegin(), self.cend());
        },
        R"pbdoc(
Return an iterator yielding the indices of the blocks.

:returns:
   An iterator yielding the indices of the blocks.
:rtype:
   collections.abc.Iterator[int]

:complexity:
   Constant.
)pbdoc");

    thing.def("degree",
              &Bipartition::degree,
              R"pbdoc(
Return the degree of the bipartition.

A bipartition is of degree :math:`n` if it is a partition of
:math:`\{0, \ldots, 2n - 1\}`.

:returns:
   The degree.
:rtype:
   int
)pbdoc");

    thing.def("is_transverse_block",
              &Bipartition::is_transverse_block,
              py::arg("index"),
              R"pbdoc(
:sig=(self: Bipartition, index: int) -> bool:
Check if a block is a transverse block.

A block of a biparition is *transverse* if it contains integers less than and
greater than :math:`n`, which is the degree of the bipartition.

:param index: the index of a block
:type index: int

:returns: Whether or not the given block is transverse.
:rtype: bool

:raises LibsemigroupsError:
  if *index* is not in the range from ``0`` to
  :any:`number_of_left_blocks`.

:complexity:
  At worst :math:`O(n)` where :math:`n` is the
  :any:`degree()`.
)pbdoc");

    thing.def("number_of_blocks",
              &Bipartition::number_of_blocks,
              R"pbdoc(
Return the number of blocks in a Bipartition. This function returns the
number of parts in the partition that instances of this class represent.

:returns:
   The number of blocks.

:rtype:
   int

:complexity:
   At worst :math:`O(n)` where :math:`n` is the :any:`degree()`.
)pbdoc");

    thing.def("number_of_left_blocks",
              &Bipartition::number_of_left_blocks,
              R"pbdoc(
Return the number of blocks containing a positive integer. The *left
blocks* of a bipartition is the partition of :math:`\{0, \ldots, n -
1\}` induced by the bipartition. This member function returns the number
of blocks in this partition.

:returns:
   The number of left blocks.

:rtype:
   int

:complexity:
   At worst :math:`O(n)` where :math:`n` is the :any:`degree()`.
)pbdoc");

    // We call the no_checks variant here because x cannot (or at least should
    // not) be invalid
    thing.def(
        "left_blocks",
        [](Bipartition const& x) {
          return Blocks(*(x.left_blocks_no_checks()));
        },
        R"pbdoc(
:sig=(self: Bipartition) -> Blocks:

Returns a copy of the left blocks of a bipartition.

The *left blocks* of a bipartition is the partition of
:math:`\{0, \ldots, n - 1\}` induced by the bipartition. This
function returns a :any:`Blocks` object representing this partition.

:returns: A copy of the left blocks.
:rtype: Blocks

:complexity: :math:`O(n)` where :math:`n` is :any:`degree()`.
)pbdoc");

    thing.def(
        "right_blocks",
        [](Bipartition const& x) {
          return Blocks(*(x.right_blocks_no_checks()));
        },
        R"pbdoc(
:sig=(self: Bipartition) -> Blocks:

Returns a copy of the right blocks of a bipartition.

The *right blocks* of a bipartition is the partition of
:math:`\{0, \ldots, n - 1\}` induced by the bipartition. This
function returns a :any:`Blocks` object representing this partition.

:returns: A copy of the right blocks.
:rtype: Blocks

:complexity: :math:`O(n)` where :math:`n` is :any:`degree()`.
)pbdoc");

    thing.def("number_of_right_blocks",
              &Bipartition::number_of_right_blocks,
              R"pbdoc(
Return the number of blocks containing a negative integer. The *right
blocks* of a bipartition is the partition of :math:`\{n, \ldots, 2n -
1\}` induced by the bipartition. This member function returns the number
of blocks in this partition.

:returns:
   The number of right blocks.

:rtype:
   int

:complexity:
   At worst :math:`O(n)` where :math:`n` is the :any:`degree()`.
)pbdoc");

    thing.def("rank",
              &Bipartition::rank,
              R"pbdoc(
Return the number of transverse blocks. The *rank* of a bipartition is
the number of blocks containing both positive and negative values, which
are referred to as the *transverse* blocks.

:returns:
   The number of transverse blocks.

:rtype:
   int

:complexity:
   :math:`O(n)` where :math:`n` is :any:`degree()`.
)pbdoc");

    thing.def_static("one",
                     &Bipartition::one,
                     py::arg("n"),
                     R"pbdoc(
:sig=(self: Bipartition, n: int) -> Bipartition:

Return the identity bipartition of given degree.

Returns the identity bipartition of degree ``n``. The *identity bipartition* of
degree :math:`n` has blocks :math:`\{i, -i\}` for all :math:`i\in \{0, \ldots,
n - 1\}`. This member function returns a new identity bipartition of degree
equal to ``n``.

:param n: the degree of the identity to be returned.
:type n: int

:returns: A newly constructed :any:`Bipartition`.
:rtype: Bipartition
)pbdoc");

    thing.def("lookup",
              &Bipartition::lookup,
              R"pbdoc(
Returns the transverse blocks lookup. The value in position ``i`` of the
returned list is ``True`` if the block with index ``i`` is transverse and
``False`` if it is not transverse.

:returns:
   The lookup for transvere blocks.
:rtype:
   list[bool]

:complexity:
   Constant.
)pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // Helpers
    ////////////////////////////////////////////////////////////////////////

    m.def("bipartition_one",
          &bipartition::one,
          py::arg("f"),
          R"pbdoc(
:sig=(f: Bipartition) -> Bipartition:
Return the identity bipartition with the same degree as the given bipartition.

The *identity bipartition* of degree :math:`n` has blocks :math:`\{i, -i\}` for
all :math:`i\in \{0, \ldots, n - 1\}`. This function returns a new identity
bipartition of degree equal to the degree of *self*.

:param f: a bipartition
:type f: Bipartition

:returns: A newly constructed :any:`Bipartition`.
:rtype: Bipartition
)pbdoc");

    m.def("bipartition_underlying_partition",
          &bipartition::underlying_partition,
          py::arg("f"),
          R"pbdoc(
:sig=(f: Bipartition) -> list[list[int]]:

Return the underlying partition of a :any:`Bipartition` object. The *underlying
partition* of a bipartition *f* is the partition of a subset :math:`P` of
:math:`\{-n, \ldots, -1\}\cup \{1, \ldots, n\}` such that:

-  :math:`\{|y|\mid y\in P\} = \{1, \ldots, n\}` ;
-  a block of the partition consists of negative numbers if and only if
   the corresponding block of *y* is a transverse block.

:param f: the bipartition
:type f: Bipartition

:returns:
   The underlying partition of the :any:`Bipartition`.
:rtype:
   list[list[int]]

:complexity:
   :math:`O(n)` where :math:`n` is the degree().
)pbdoc");
  }  // init_bipart

}  // namespace libsemigroups
