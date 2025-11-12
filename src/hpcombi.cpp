//
// libsemigroups_pybind11
// Copyright (C) 2025 James D. Mitchell
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
#include <libsemigroups/hpcombi.hpp>

#ifdef LIBSEMIGROUPS_HPCOMBI_ENABLED

// pybind11....
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_hpcombi

namespace py = pybind11;

namespace HPCombi {
  namespace {
    using LibsemigroupsException = ::libsemigroups::LibsemigroupsException;

    std::string repr(Vect16 const& self, std::string_view prefix) {
      auto result   = std::to_string(self);
      result[0]     = '[';
      result.back() = ']';
      return fmt::format("{}({})", prefix, result);
    }

    ////////////////////////////////////////////////////////////////////////
    // Vect16
    ////////////////////////////////////////////////////////////////////////

    void init_hpcombi_vect16(py::module& m) {
      py::class_<Vect16> thing(m,
                               "hpcombi_Vect16",
                               R"pbdoc(
Vector of ``16`` bytes, with some SIMD optimized methods, superclass of
:any:`hpcombi.Transf16`. Entries in :any:`Vect16` must be integers in the range :math:`[0, 256)`.

This class belongs to the ``hpcombi`` subpackage of ``libsemigroups_pybind11``.

The functionality described on this page is only available if
:any:`LIBSEMIGROUPS_HPCOMBI_ENABLED` is ``True``.
)pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // Special methods
      ////////////////////////////////////////////////////////////////////////

      thing.def("__repr__",
                [](Vect16 const& thing) { return repr(thing, "Vect16"); });

      thing.def(
          "__getitem__",
          [](Vect16& self, size_t i) {
            if (i > 15) {
              throw std::out_of_range(fmt::format(
                  "index out of range, expected value in [0, 16), found {}",
                  i));
            }
            return self[i];
          },
          py::arg("i"),
          py::is_operator());

      thing.def(
          "__setitem__",
          [](Vect16& x, size_t i, size_t val) {
            if (i > 15) {
              throw std::out_of_range(fmt::format(
                  "index out of range, expected value in [0, 16), found {}",
                  i));
            }
            x[i] = val;
          },
          py::is_operator());

      thing.def("__len__", [](Vect16 const&) { return Vect16::size(); });

      thing.def("__copy__", [](Vect16 const& v) { return Vect16(v); });

      thing.def("__hash__",
                [](Vect16 const& v) { return std::hash<Vect16>{}(v); });

      ////////////////////////////////////////////////////////////////////////
      // Operators
      ////////////////////////////////////////////////////////////////////////

      thing.def(py::self == py::self);
      thing.def(py::self != py::self);
      thing.def(py::self < py::self);

      // NOTE: size isn't bound, use "len" instead

      ////////////////////////////////////////////////////////////////////////
      // Constructors
      ////////////////////////////////////////////////////////////////////////

      thing.def(py::init<>(), R"pbdoc(
Default constructor.

Constructs a :any:`Vect16` object with its entries uninitialized. This means there
is no guarantee about the values in the constructed object.
)pbdoc");

      thing.def(py::init([](std::vector<uint8_t> img) {
                  if (img.size() < 16) {
                    img.resize(16, 0);
                  } else if (img.size() > 16) {
                    LIBSEMIGROUPS_EXCEPTION(
                        "expected a list of at most 16 values, found {}",
                        img.size());
                  }
                  return Vect16({img[0],
                                 img[1],
                                 img[2],
                                 img[3],
                                 img[4],
                                 img[5],
                                 img[6],
                                 img[7],
                                 img[8],
                                 img[9],
                                 img[10],
                                 img[11],
                                 img[12],
                                 img[13],
                                 img[14],
                                 img[15]});
                }),
                R"pbdoc(
:sig=(self: Vect16, img: list[int]) -> None:

Construct a :any:`Vect16` from a list of its entries.

This function constructs a :any:`Vect16` from the list *img* of its entries.
If the length of *img* is less than ``16``, then the constructed :any:`Vect16`
is padded with ``0`` values at the end.

:param img: The list of images.
:type img: list[int]

:raises LibsemigroupsError: if any value in *img* exceeds ``255``.
:raises LibsemigroupsError: if the length of *img* exceeds ``16``.
:raises TypeError: if any value in *img* is larger than ``255``.

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import Vect16
    >>> Vect16([1, 2, 3, 4, 255])
    Vect16([ 1, 2, 3, 4,255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
)pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // Other methods
      ////////////////////////////////////////////////////////////////////////

      // NOTE: the following doesn't compile, so skipping

      // thing.def("as_array",
      //           &Vect16::as_array,
      // R"pbdoc()pbdoc");

      thing.def(
          "copy",
          [](Vect16 const& v) { return Vect16(v); },
          R"pbdoc(
:sig=(self: Vect16) -> Vect16:

Copy a :any:`Vect16`.

:returns: A copy of the argument.
:rtype: Vect16

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import Vect16
    >>> x = Vect16([1, 2, 3, 4, 255])
    >>> x.copy() is not x
    True
    >>> x.copy() == x
    True
)pbdoc");

      thing.def("first_diff",
                &Vect16::first_diff,
                py::arg("other"),
                py::arg("bound") = 16,
                R"pbdoc(
:sig=(self: Vect16, other: Vect16, bound: int = 16) -> int:

Returns the position of the first diff.

This function returns the first diff in *self* and *other* among the first
*bound* entries. That is, the minimum ``i`` such that ``self[i] != other[i]``
where ``i`` is in the range from ``0`` to ``bound - 1``. If *self* and *other*
agree up to position ``bound - 1``, then ``16`` is returned.

:param other: The vector for comparison.
:type other: Vect16

:param bound: The bound (defaults to ``16``).
:type bound: int

:returns: The position of the first difference or ``16``.
:rtype: int

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import Vect16
    >>> Vect16([1, 2, 3, 4, 255]).first_diff(Vect16([1, 2, 3, 4, 245]))
    4
)pbdoc");

      thing.def("last_diff",
                &Vect16::last_diff,
                py::arg("other"),
                py::arg("bound") = 16,
                R"pbdoc(
:sig=(self: Vect16, other: Vect16, bound: int = 16) -> int:

Returns the position of the last diff.

This function returns the last diff in *self* and *other* among the first
*bound* entries. That is, the maximum ``i`` such that ``self[i] != other[i]``
where ``i`` is in the range from ``0`` to ``bound - 1``. If *self* and *other*
agree up to position ``bound - 1``, then ``16`` is returned.

:param other: The vector for comparison.
:type other: Vect16

:param bound: The bound (defaults to ``16``).
:type bound: int

:returns: The position of the last difference or ``16``.
:rtype: int

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import Vect16
    >>> Vect16([1, 2, 3, 4, 255]).last_diff(Vect16([1, 2, 3, 4, 245]))
    4
)pbdoc");

      thing.def("first_zero",
                &Vect16::first_zero,
                py::arg("bound") = 16,
                R"pbdoc(
:sig=(self: Vect16, bound: int = 16) -> int:

Returns the position of the first zero.

This function returns the first zero in *self* among the first *bound* entries.
That is, the minimum ``i`` such that ``self[i] == 0`` where ``i`` is in the
range from ``0`` to ``bound - 1``. If *self* contains no zeros, then ``16`` is
returned.

:param bound: The bound (defaults to ``16``).
:type bound: int

:returns: The position of the first zero or ``16``.
:rtype: int

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import Vect16
    >>> Vect16([1, 2, 3, 4, 255]).first_zero()
    5
    >>> Vect16().first_zero()
    0
    >>> Vect16([1, 2, 3, 4, 255]).first_zero(3)
    16
)pbdoc");

      thing.def("last_zero",
                &Vect16::last_zero,
                py::arg("bound") = 16,
                R"pbdoc(
:sig=(self: Vect16, bound: int = 16) -> int:

Returns the position of the last zero.

This function returns the last zero in *self* among the last *bound* entries.
That is, the maximum ``i`` such that ``self[i] == 0`` where ``i`` is in the
range from ``0`` to ``bound - 1``. If *self* contains no zeros, then ``16`` is
returned.

:param bound: The bound (defaults to ``16``).
:type bound: int

:returns: The position of the last zero or ``16``.
:rtype: int

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import Vect16
    >>> Vect16([1, 2, 3, 4, 255]).last_zero()
    15
    >>> Vect16([1, 2, 3, 4, 255]).first_zero(3)
    16
    >>> Vect16().last_zero()
    15
)pbdoc");

      thing.def("first_non_zero",
                &Vect16::first_non_zero,
                py::arg("bound") = 16,
                R"pbdoc(
:sig=(self: Vect16, bound: int = 16) -> int:

Returns the position of the first non-zero item.

This function returns the first non-zero item in *self* among the first *bound* entries.
That is, the minimum ``i`` such that ``self[i] != 0`` where ``i`` is in the
range from ``0`` to ``bound - 1``. If *self* contains no non-zero items, then ``16`` is
returned.

:param bound: The bound (defaults to ``16``).
:type bound: int

:returns: The position of the first zero or ``16``.
:rtype: int

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import Vect16
    >>> Vect16([1, 2, 3, 4, 255]).first_non_zero()
    0
    >>> Vect16().first_non_zero()
    16
)pbdoc");

      thing.def("last_non_zero",
                &Vect16::last_non_zero,
                py::arg("bound") = 16,
                R"pbdoc(
:sig=(self: Vect16, bound: int = 16) -> int:

Returns the position of the first non-zero item.

This function returns the first non-zero item in *self* among the first *bound* entries.
That is, the minimum ``i`` such that ``self[i] != 0`` where ``i`` is in the
range from ``0`` to ``bound - 1``. If *self* contains no non-zero items, then ``16`` is
returned.

:param bound: The bound (defaults to ``16``).
:type bound: int

:returns: The position of the first zero or ``16``.
:rtype: int

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import Vect16
    >>> Vect16([1, 2, 3, 4, 255]).last_non_zero()
    4
    >>> Vect16([1, 2, 3, 4, 255]).last_non_zero(3)
    2
    >>> Vect16().last_non_zero()
    16
)pbdoc");

      // NOTE: This seems unnecessary, see the test file, we can
      // already do list(x) and [x for x in v] etc, so no need for
      // this.

      //       thing.def(
      //           "iterator",
      //           [](Vect16 const& self) {
      //             return py::make_iterator(self.cbegin(),
      //             self.cend());
      //           },
      //           R"pbdoc(
      // )pbdoc");

      thing.def("less_partial",
                &Vect16::less_partial,
                py::arg("other"),
                py::arg("bound") = 16,
                R"pbdoc(
:sig=(self: Vect16, other: Vect16, bound: int = 16) -> int:

Returns the difference of the first diff.

This function returns the first non-zero difference (if any) between in
``self[i]`` and ``other[i]`` among the first *bound* entries or ``0``.

:param other: The vector for comparison.
:type other: Vect16

:param bound: The bound (defaults to ``16``).
:type bound: int

:returns: The difference or ``0``.
:rtype: int

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import Vect16
    >>> v = Vect16([0, 1, 2, 3])
    >>> u = Vect16([0, 1, 2, 10])
    >>> v.less_partial(u, 3)
    0
    >>> v.less_partial(u, 4)
    -7
    >>> u.less_partial(v, 4)
    7
    >>> u.less_partial(v, 16)
    7
    >>> v.less_partial(u, 16)
    -7
)pbdoc");

      // NOTE: the following function is named badly, and I'm not
      // completely sure what it does when the values in "other" are >
      // 15, so omitting for now.

      // thing.def("permuted",
      //           &Vect16::permuted,
      //           py::arg("other"),
      //           R"pbdoc(
      // )pbdoc");

      thing.def("sum",
                &Vect16::sum,
                R"pbdoc(
:sig=(self: Vect16) -> int:

Returns the sum of the entries mod ``256``.

This function returns the sum of the items in *self* mod ``256``.

:returns: The sum of the items in *self* mod ``256``.
:rtype: int

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import Vect16
    >>> Vect16([0, 1, 2, 3]).sum()
    6
    >>> Vect16([0, 1, 2, 10]).sum()
    13
    >>> Vect16([1, 2, 3, 255]).sum()
    5
)pbdoc");

      thing.def("partial_sums",
                &Vect16::partial_sums,
                R"pbdoc(
:sig=(self: Vect16) -> Vect16:

Returns the :any:`Vect16` of partial sums of the entries mod ``256``.

This function returns the :any:`Vect16` of partial sums of the items in *self*
mod ``256``.

:returns: The partial sums of the items in *self* mod ``256``.
:rtype: Vect16

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import Vect16
    >>> Vect16([0, 1, 2, 3]).partial_sums()
    Vect16([ 0, 1, 3, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6])
    >>> Vect16([1, 2, 3, 255]).partial_sums()
    Vect16([ 1, 3, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5])
)pbdoc");

      thing.def("eval16",
                &Vect16::eval16,
                R"pbdoc(
:sig=(self: Vect16) -> Vect16:

Counts how many times each value in :math:`[0, 16)` appears in *self*.

This function counts how many times each value in :math:`[0, 16)` appears in
*self*. In other words, this function returns a :any:`Vect16` such that the
item in position ``i`` is the number of occurrences of ``i`` in *self*.

:returns: The counts.
:rtype: Vect16

.. warning: Values in *self* larger than ``15`` are ignored.

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import Vect16
   >>> Vect16([5, 5, 2, 5, 1, 6,12, 4, 0, 3, 2,11,12,13,14,15]).eval16()
   Vect16([ 1, 1, 2, 1, 1, 3, 1, 0, 0, 0, 0, 1, 2, 1, 1, 1])

)pbdoc");

      thing.def(
          "is_permutation",
          [](Vect16 const& self, size_t bound) {
            return self.is_permutation(bound);
          },
          py::arg("bound") = 16,
          R"pbdoc(
:sig=(self: Vect16, bound: int = 16) -> bool:

Returns whether or not the vector defines a permutation.

This function returns ``True`` if the first *bound* entries of *self*
define a permutation; and ``False`` otherwise.

:param bound: The number of entries to check (defaults to ``16``).
:type bound: int

:returns: Whether or not *self* is a permutation of its first *bound* entries.
:rtype: bool

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import Vect16
   >>> Vect16([5, 5, 2, 5, 1, 6,12, 4, 0, 3, 2,11,12,13,14,15]).is_permutation()
   False
   >>> Vect16([1, 0, 2, 3, 4, 4]).is_permutation()
   False
   >>> Vect16([1, 0, 2, 3, 4] + list(range(5, 16))).is_permutation()
   True
)pbdoc");
    }  // init_hpcombi_vect16

    ////////////////////////////////////////////////////////////////////////
    // PTransf16
    ////////////////////////////////////////////////////////////////////////

    void init_hpcombi_ptransf16(py::module& m) {
      py::class_<PTransf16, Vect16> thing(m,
                                          "hpcombi_PTransf16",
                                          R"pbdoc(
Class representing partial transformations.

SIMD accelerated class :any:`PTransf16` representing partial transformations on
up to ``16`` points. Partial means it might not be defined everywhere.
Undefined images are encoded as ``255``.

This class belongs to the ``hpcombi`` subpackage of ``libsemigroups_pybind11``.

The functionality described on this page is only available if
:any:`LIBSEMIGROUPS_HPCOMBI_ENABLED` is ``True``.

:any:`PTransf16` inherits from :any:`Vect16`.
)pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // Special methods
      ////////////////////////////////////////////////////////////////////////

      thing.def("__repr__",
                [](PTransf16 const& self) { return repr(self, "PTransf16"); });

      thing.def(
          "__mul__",
          // The next line is not a type, but is consistent with the
          // other transformations in libsemigroups_pybind11, since
          // function composition in HPCombi is backwards.
          [](PTransf16 const& x, PTransf16 const& y) { return y * x; },
          py::is_operator());

      thing.def("__copy__", [](PTransf16 const& v) { return PTransf16(v); });

      ////////////////////////////////////////////////////////////////////////
      // Constructors
      ////////////////////////////////////////////////////////////////////////

      thing.def(py::init<>(), R"pbdoc(
:sig=(self: PTransf16) -> None:

Default constructor.

Constructs a :any:`PTransf16` object with its entries uninitialized. This means
there is no guarantee about the values in the constructed object.
)pbdoc");

      thing.def(py::init([](std::vector<uint8_t> img) {
                  return ::libsemigroups::make<PTransf16>(std::move(img));
                }),
                R"pbdoc(
:sig=(self: PTransf16, img: list[int]) -> None:

Construct a :any:`PTransf16` from a list of images.

This function constructs a :any:`PTransf16` from the list *img* of its entries.
If the length of *img* is less than ``16``, then the constructed :any:`PTransf16`
is fixed points at the end.

:param img: The list of images.
:type img: list[int]

:raises LibsemigroupsError: if any value in *img* exceeds ``16`` and is not equal to ``255``.
:raises LibsemigroupsError: if the length of *img* exceeds ``16``.
:raises TypeError: if any value in *img* is larger than ``255``.

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import PTransf16
   >>> PTransf16([1, 255, 1, 10])
   PTransf16([ 1,255, 1,10, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15])
)pbdoc");

      thing.def(  // Seems like the last arg does nothing
          py::init([](std::vector<uint8_t> dom, std::vector<uint8_t> ran) {
            return ::libsemigroups::make<PTransf16>(dom, ran, 16);
          }),
          R"pbdoc(
:sig=(self: PTransf16, dom: list[int], im: list[int]) -> None:

Construct from domain and image.

Constructs a partial transformation of degree *n* such that ``(dom[i])f =
im[i]`` for all ``i`` and which is undefined (``255`` represents undefined in
this context) on every other value in the range :math:`[0, n)`.

:param dom: the domain.
:type dom: list[int]
:param im: the image.
:type im: list[int]

:raises LibsemigroupsError: *dom* and *im* do not have the same size.
:raises LibsemigroupsError: any value in *dom* or *im* is greater than *15*.
:raises LibsemigroupsError: there are repeated entries in *dom*.

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import PTransf16
    >>> PTransf16([1, 2], [3, 4])
    PTransf16([255, 3, 4,255,255,255,255,255,255,255,255,255,255,255,255,255])
)pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // Static methods
      ////////////////////////////////////////////////////////////////////////

      thing.def_static("one",
                       &PTransf16::one,
                       R"pbdoc(
:sig=one() -> PTransf16:

Returns the identity partial transformation.

This function returns the identity :any:`PTransf16` which fixes every value in
:math:`[0, 16)`.

:returns: The identity transformation.
:rtype: PTransf16

.. doctest::

        >>> from libsemigroups_pybind11.hpcombi import PTransf16
        >>> x = PTransf16([1, 0, 1])
        >>> x * x.one() == PTransf16.one() * x == x
        True
)pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // Other methods
      ////////////////////////////////////////////////////////////////////////

      thing.def(
          "copy",
          [](PTransf16 const& v) { return PTransf16(v); },
          R"pbdoc(
:sig=(self: PTransf16) -> PTransf16:

Copy a :any:`PTransf16`.

:returns: A copy of the argument.
:rtype: PTransf16

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import PTransf16
    >>> x = PTransf16([1, 2, 3, 4, 255])
    >>> x.copy() is not x
    True
    >>> x.copy() == x
    True
)pbdoc");

      thing.def("validate",
                &PTransf16::validate,
                py::arg("bound") = 16,
                R"pbdoc(
:sig=(self: PTransf16, bound: int = 16) -> bool:

Check whether or not a :any:`PTransf16` is well-defined.

This function returns ``True`` if *self* is a well-defined partial
transformation (i.e. no image value is larger than ``15``) on the values
``0`` up to *bound*.

:param bound: the bound (defaults to ``16``).
:type bound: int

:returns: Whether or not *self* is valid.
:rtype: bool

.. note::
  It should not be possible to create an invalid :any:`PTransf16` in
  ``libsemigroups_pybind11``, and this function is only included for
  completeness.

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import PTransf16
    >>> PTransf16([1, 0, 1]).validate()
    True
)pbdoc");

      // NOTE: the following doesn't compile and so is omitted.

      //       thing.def("image_mask_cmpestrm",
      //                 &PTransf16::image_mask_cmpestrm,
      //                 py::arg("complement"),
      //                 R"pbdoc(
      // Returns a mask for the image of *this.
      // )pbdoc");

      thing.def(
          "image_mask_ref",
          [](PTransf16 const& self, bool complement) {
            return Vect16(self.image_mask_ref(complement));
          },
          py::arg("complement") = false,
          R"pbdoc(
:sig=(self: PTransf16, complement: bool = False) -> Vect16:

Returns a mask for the image.

This function returns a mask for the image of *self* or its complement
depending on the value of *complement*. If *complement* is ``True``, then the
returned mask has ``0`` in position ``i`` for every ``i`` in the image of
*self* and ``255`` (undefined) otherwise. If *complement* is ``False``, then
the returned mask has ``0`` in position ``i`` for every ``i`` not in the image
of *self* and ``255`` otherwise.

This is the reference implementation, use :any:`image_mask` for better performance.

:param complement: whether or not the complement is sought (defaults to ``False``).
:type complement: bool

:returns: The image mask or its complement.
:rtype: Vect16

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import PTransf16
    >>> PTransf16([1, 0, 1]).image_mask_ref(True)
    Vect16([ 0, 0,255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
    >>> PTransf16([1, 0, 1]).image_mask_ref(False)
    Vect16([255,255, 0,255,255,255,255,255,255,255,255,255,255,255,255,255])
)pbdoc");

      thing.def(
          "image_mask",
          [](PTransf16 const& self, bool complement) {
            return Vect16(self.image_mask(complement));
          },
          py::arg("complement") = false,
          R"pbdoc(
:sig=(self: PTransf16, complement: bool = False) -> Vect16:

Returns a mask for the image.

This function returns a mask for the image of *self* or its complement
depending on the value of *complement*. If *complement* is ``True``, then the
returned mask has ``0`` in position ``i`` for every ``i`` in the image of
*self* and ``255`` (undefined) otherwise. If *complement* is ``False``, then
the returned mask has ``0`` in position ``i`` for every ``i`` not in the image
of *self* and ``255`` otherwise.

:param complement: whether or not the complement is sought (defaults to ``False``).
:type complement: bool

:returns: The image mask or its complement.
:rtype: Vect16

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import PTransf16
    >>> PTransf16([1, 0, 1]).image_mask(True)
    Vect16([ 0, 0,255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
    >>> PTransf16([1, 0, 1]).image_mask(False)
    Vect16([255,255, 0,255,255,255,255,255,255,255,255,255,255,255,255,255])
)pbdoc");

      thing.def("image_bitset",
                &PTransf16::image_bitset,
                py::arg("complement") = false,
                R"pbdoc(
:sig=(self: PTransf16, complement: bool = False) -> int:

Returns a bit mask (as an ``int``) for the image of *self* (or its complement).

This function returns a bitset mask for the image of *self* or its complement
depending on the value of *complement*. If *complement* is ``True``, then the
returned mask has ``1`` in bit ``i`` if and only if ``i`` is in the image of
*self*.  If *complement* is ``False``, then the returned mask has ``0`` in bit
``i`` if and only if ``i`` is in the image of *self*.

:param complement: whether or not the complement is sought (defaults to ``False``).
:type complement: bool

:returns: The image bitset or its complement.
:rtype: int

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import PTransf16
    >>> x = PTransf16([1, 3, 1, 255, 10])
    >>> x.image_bitset()
    65514
    >>> bin(x.image_bitset())
    '0b1111111111101010'
    >>> bin(x.image_bitset(True))
    '0b10101'
    >>> bool(x.image_bitset() & 1 << 1)
    True
    >>> bool(x.image_bitset() & 1 << 2)
    False
)pbdoc");

      thing.def(
          "domain_mask",
          [](PTransf16 const& self, bool complement) {
            return Vect16(self.domain_mask(complement));
          },
          py::arg("complement") = false,
          R"pbdoc(
:sig=(self: PTransf16, complement: bool = False) -> Vect16:

Returns a mask for the domain.

This function returns a mask for the domain of *self* or its complement
depending on the value of *complement*. If *complement* is ``True``, then the
returned mask has ``0`` in position ``i`` for every ``i`` in the domain of
*self* and ``255`` (undefined) otherwise. If *complement* is ``False``, then
the returned mask has ``0`` in position ``i`` for every ``i`` not in the domain
of *self* and ``255`` otherwise.

:param complement: whether or not the complement is sought (defaults to ``False``).
:type complement: bool

:returns: The domain mask or its complement.
:rtype: Vect16

.. doctest::

        >>> from libsemigroups_pybind11.hpcombi import PTransf16
        >>> PTransf16([1, 0, 1]).domain_mask(True)
        Vect16([ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
        >>> PTransf16([1, 0, 1]).domain_mask(False)
        Vect16([255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255])
)pbdoc");

      thing.def("domain_bitset",
                &PTransf16::domain_bitset,
                py::arg("complement") = false,
                R"pbdoc(
:sig=(self: PTransf16, complement: bool = False) -> int:

Returns a bit mask (as an ``int``) for the domain of *self* (or its complement).

This function returns a bitset mask for the domain of *self* or its complement
depending on the value of *complement*. If *complement* is ``True``, then the
returned mask has ``1`` in bit ``i`` if and only if ``i`` is in the domain of
*self*.  If *complement* is ``False``, then the returned mask has ``0`` in bit
``i`` if and only if ``i`` is in the domain of *self*.

:param complement: whether or not the complement is sought (defaults to ``False``).
:type complement: bool

:returns: The domain bitset or its complement.
:rtype: int

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import PTransf16
    >>> x = PTransf16([1, 3, 1, 255, 10])
    >>> x.domain_bitset()
    65527
    >>> bin(x.domain_bitset())
    '0b1111111111110111'
    >>> bin(x.domain_bitset(True))
    '0b1000'
    >>> bool(x.domain_bitset() & 1 << 1)
    True
    >>> bool(x.domain_bitset() & 1 << 3)
    False
)pbdoc");

      thing.def("right_one",
                // This is not a typo, everything is reversed in HPCombi
                &PTransf16::left_one,
                R"pbdoc(
:sig=(self: PTransf16) -> PTransf16:
Returns the right one of a partial transformation.

This function returns a newly constructed :any:`PTransf16` with the same image as
*self* and that acts as the identity on *self* by right multiplication.

:returns: A right one of *self*.
:rtype: PTransf16

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import PTransf16
    >>> x = PTransf16([1, 3, 1, 255, 10])
    >>> x.image_bitset() == x.right_one().image_bitset()
    True
    >>> x * x.right_one() == x
    True
)pbdoc");

      thing.def("left_one",
                // This is not a typo, everything is reversed in HPCombi
                &PTransf16::right_one,
                R"pbdoc(
:sig=(self: PTransf16) -> PTransf16:

Returns the left one of a partial transformation.

This function returns a newly constructed :any:`PTransf16` with the same image as
*self* and that acts as the identity on *self* by left multiplication.

:returns: A left one of *self*.
:rtype: PTransf16

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import PTransf16
   >>> x = PTransf16([1, 3, 1, 255, 10])
   >>> x.domain_bitset() == x.left_one().domain_bitset()
   True
   >>> x.left_one() * x == x
   True
)pbdoc");

      thing.def("rank_ref",
                &PTransf16::rank_ref,
                R"pbdoc(
:sig=(self: PTransf16) -> int:
Returns the size of the image set of a partial transformation.

This function returns the size of the image set of *self*.

:returns: The size of the image set.
:rtype: int

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import PTransf16
   >>> x = PTransf16([1, 3, 1, 255, 10])
   >>> x.rank_ref()
   13
)pbdoc");

      thing.def("rank",
                &PTransf16::rank,
                R"pbdoc(
:sig=(self: PTransf16) -> int:
Returns the size of the image set of a partial transformation.

This function returns the size of the image set of *self*.

:returns: The size of the image set.
:rtype: int

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import Vect16
   >>> x = PTransf16([1, 3, 1, 255, 10])
   >>> x.rank()
   13
)pbdoc");

      thing.def("rank_cmpestrm",
                &PTransf16::rank_cmpestrm,
                R"pbdoc(
:sig=(self: PTransf16) -> int:

Returns the size of the image set of a partial transformation.

This function returns the size of the image set of *self*.

:returns: The size of the image set.
:rtype: int

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import Vect16
   >>> x = PTransf16([1, 3, 1, 255, 10])
   >>> x.rank_cmpestrm()
   13
)pbdoc");

      thing.def(
          "fix_points_mask",
          [](PTransf16 const& self, bool complement) {
            return Vect16(self.fix_points_mask(complement));
          },
          py::arg("complement") = false,
          R"pbdoc(
:sig=(self: PTransf16, complement: bool = False) -> Vect16:

Returns a mask for the fixed points of a partial transformation.

This function returns a mask for the fixed points of *self* or its complement
depending on the value of *complement*. If *complement* is ``True``, then the
returned mask has ``255`` in position ``i`` for every fixed point ``i`` of
*self* and ``0`` (undefined) otherwise. If *complement* is ``False``, then
``0`` and ``255`` are switched in the output.

:param complement: whether or not the complement is sought (defaults to ``False``).
:type complement: bool

:returns: The fixed points mask or its complement.
:rtype: Vect16

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import Vect16
   >>> x = PTransf16([1, 3, 1, 255, 10])
   >>> x.fix_points_mask()
   Vect16([ 0, 0, 0, 0, 0,255,255,255,255,255,255,255,255,255,255,255])
   >>> x.fix_points_mask(True)
   Vect16([255,255,255,255,255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
   >>> x.one().fix_points_mask()
   Vect16([255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255])
   >>> x.one().fix_points_mask(True)
   Vect16([ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
)pbdoc");

      thing.def("fix_points_bitset",
                &PTransf16::fix_points_bitset,
                py::arg("complement") = false,
                R"pbdoc(
:sig=(self: PTransf16, complement: bool = False) -> int:

Returns a bit mask (as an ``int``) for the fixed, or non-fixed, points of *self*.

This function returns a bitset mask for the fixed points of *self* or the
non-fixed points of *self* depending on the value of *complement*. If
*complement* is ``True``, then the returned mask has ``1`` in bit ``i`` if and
only if ``i`` is fixed by *self*.  If *complement* is ``False``, then the
returned mask has ``0`` in bit ``i`` if and only if ``i`` is fixed by *self*.

:param complement: whether or not the complement is sought (defaults to ``False``).
:type complement: bool

:returns: The fixed points bitset or its complement.
:rtype: int

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import PTransf16
   >>> x = PTransf16([1, 3, 2, 255, 10])
   >>> x.fix_points_bitset()
   65508
   >>> x.fix_points_bitset(False)
   65508
   >>> x.fix_points_bitset(True)
   27
   >>> bin(x.fix_points_bitset())
   '0b1111111111100100'
   >>> bin(x.fix_points_bitset(True))
   '0b11011'
)pbdoc");

      thing.def("smallest_fix_point",
                &PTransf16::smallest_fix_point,
                R"pbdoc(
:sig=(self: PTransf16) -> int:

Returns the smallest fix point.

This function returns the smallest integer ``i`` such that
``self[i] == i`` or ``255`` if ``self[i] != i`` for all ``i < 16``.

:returns: The smallest fixed point.
:rtype: int

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import PTransf16
   >>> PTransf16([1, 3, 2, 255, 10]).smallest_fix_point()
   2
)pbdoc");

      thing.def("smallest_moved_point",
                &PTransf16::smallest_moved_point,
                R"pbdoc(
:sig=(self: PTransf16) -> int:

Returns the smallest moved point.

This function returns the smallest integer ``i`` such that
``self[i] != i`` or ``255`` if ``self[i] == i`` for all ``i < 16``.

:returns: The smallest moved point.
:rtype: int

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import PTransf16
   >>> PTransf16([1, 3, 2, 255, 10]).smallest_moved_point()
   0
   >>> PTransf16.one().smallest_moved_point()
   255
)pbdoc");

      thing.def("largest_fix_point",
                &PTransf16::largest_fix_point,
                R"pbdoc(
:sig=(self: PTransf16) -> int:

Returns the largest fix point.

This function returns the largest integer ``i`` such that
``self[i] == i`` or ``255`` if ``self[i] != i`` for all ``i < 16``.

:returns: The largest fixed point.
:rtype: int

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import PTransf16
   >>> PTransf16([1, 3, 2, 255, 10]).largest_fix_point()
   15
   >>> PTransf16.one().largest_fix_point()
   15
)pbdoc");

      thing.def("largest_moved_point",
                &PTransf16::largest_moved_point,
                R"pbdoc(
:sig=(self: PTransf16) -> int:

Returns the largest moved point.

This function returns the largest integer ``i`` such that
``self[i] != i`` or ``255`` if ``self[i] == i`` for all ``i < 16``.

:returns: The largest moved point.
:rtype: int

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import PTransf16
   >>> PTransf16([1, 3, 2, 255, 10]).largest_moved_point()
   4
   >>> PTransf16.one().largest_moved_point()
   255
)pbdoc");

      thing.def("nb_fix_points",
                &PTransf16::nb_fix_points,
                R"pbdoc(
:sig=(self: PTransf16) -> int:

Returns the number of fixed points.

This function returns the number of integers ``i`` such that
``self[i] != i`` and ``i < 16``.

:returns: The number of fixed points.
:rtype: int

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import PTransf16
   >>> PTransf16([1, 3, 2, 255, 10]).nb_fix_points()
   12
   >>> PTransf16.one().nb_fix_points()
   16
)pbdoc");
    }  // init_hpcombi_ptransf16

    ////////////////////////////////////////////////////////////////////////
    // Transf16
    ////////////////////////////////////////////////////////////////////////

    void init_hpcombi_transf16(py::module& m) {
      py::class_<Transf16, PTransf16> thing(m,
                                            "hpcombi_Transf16",
                                            R"pbdoc(
Class representing transformations.

SIMD accelerated class :any:`Transf16` representing transformations on
up to ``16`` points.

This class belongs to the ``hpcombi`` subpackage of ``libsemigroups_pybind11``.

The functionality described on this page is only available if
:any:`LIBSEMIGROUPS_HPCOMBI_ENABLED` is ``True``.

:any:`Transf16` inherits from :any:`PTransf16`.
)pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // Special methods
      ////////////////////////////////////////////////////////////////////////

      thing.def("__repr__",
                [](Transf16 const& self) { return repr(self, "Transf16"); });

      thing.def("__int__",
                [](Transf16 const& x) { return static_cast<uint64_t>(x); });

      thing.def("__copy__", [](Transf16 const& v) { return Transf16(v); });

      ////////////////////////////////////////////////////////////////////////
      // Constructors
      ////////////////////////////////////////////////////////////////////////

      thing.def(py::init<>(), R"pbdoc(
:sig=(self: Transf16) -> None:

Default constructor.

Constructs a :any:`Transf16` object with its entries uninitialized. This means
there is no guarantee about the values in the constructed object.

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import Transf16
   >>> x = Transf16()
)pbdoc");

      thing.def(py::init([](std::vector<uint8_t> img) {
                  return ::libsemigroups::make<Transf16>(std::move(img));
                }),
                R"pbdoc(
:sig=(self: Transf16, img: list[int]) -> None:

Construct a :any:`Transf16` from a list of images.

This function constructs a :any:`Transf16` from the list *img* of its entries.
If the length of *img* is less than ``16``, then the constructed :any:`Transf16`
is padded with fixed points at the end.

:param img: The list of images.
:type img: list[int]

:raises LibsemigroupsError: if any value in *img* is in the range :math:`[16, 256)`.
:raises LibsemigroupsError: if the length of *img* exceeds ``16``.
:raises TypeError: if any value in *img* is larger than ``255``.

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import Transf16
   >>> Transf16([1, 3, 1, 10])
   Transf16([ 1, 3, 1,10, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15])
)pbdoc");

      thing.def(py::init<uint64_t>(), R"pbdoc(
:sig=(self: Transf16, n: int) -> None:

Construct a transformation from its 64 bits compressed.

This function constructs a :any:`Transf16` from its integer representation *n*.

:param n: the integer representation.
:type n: int

:raises TypeError: if *n* is not in the range :math:`[0, 2 ^{64})`.

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import Transf16
   >>> Transf16(2 ** 64 - 1)
   Transf16([15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15])
   >>> int(Transf16([15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15]))
   18446744073709551615
)pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // Other methods
      ////////////////////////////////////////////////////////////////////////

      thing.def(
          "copy",
          [](Transf16 const& v) { return Transf16(v); },
          R"pbdoc(
:sig=(self: Transf16) -> Transf16:

Copy a :any:`Transf16`.

:returns: A copy of the argument.
:rtype: Transf16

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import Transf16
    >>> x = Transf16([1, 2, 3, 4, 4])
    >>> x.copy() is not x
    True
    >>> x.copy() == x
    True
)pbdoc");

      thing.def("validate",
                &Transf16::validate,
                py::arg("bound") = 16,
                R"pbdoc(
:sig=(self: Transf16, bound: int = 16) -> bool:

Check whether or not a :any:`Transf16` is well-defined.

This function returns ``True`` if *self* is a well-defined partial
transformation (i.e. no image value is larger than ``15``) on the values
``0`` up to *bound*.

:param bound: the bound (defaults to ``16``).
:type bound: int

:returns: Whether or not *self* is valid.
:rtype: bool

.. note::
  It should not be possible to create an invalid :any:`Transf16` in
  ``libsemigroups_pybind11``, and this function is only included for
  completeness.

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import Transf16
    >>> Transf16([1, 0, 1]).validate()
    True
)pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // Static methods
      ////////////////////////////////////////////////////////////////////////

      thing.def_static("one",
                       &Transf16::one,
                       R"pbdoc(
:sig=one() -> Transf16:

Returns the identity partial transformation.

This function returns the identity :any:`Transf16` which fixes every value in
:math:`[0, 16)`.

:returns: The identity transformation.
:rtype: Transf16

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import Transf16
   >>> x = Transf16([1, 0, 1])
   >>> x * x.one() == Transf16.one() * x == x
   True
)pbdoc");
    }  // init_hpcombi_transf16

    ////////////////////////////////////////////////////////////////////////
    // Perm16
    ////////////////////////////////////////////////////////////////////////

    void init_hpcombi_perm16(py::module& m) {
      py::class_<Perm16, Transf16> thing(m,
                                         "hpcombi_Perm16",
                                         R"pbdoc(
Class representing permutations.

SIMD accelerated class :any:`Perm16` representing permutations on
up to ``16`` points.

This class belongs to the ``hpcombi`` subpackage of ``libsemigroups_pybind11``.

The functionality described on this page is only available if
:any:`LIBSEMIGROUPS_HPCOMBI_ENABLED` is ``True``.

:any:`Perm16` inherits from :any:`Transf16`.
)pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // Special methods
      ////////////////////////////////////////////////////////////////////////

      thing.def("__copy__", [](Perm16 const& self) { return Perm16(self); });

      thing.def("__int__",
                [](Perm16 const& x) { return static_cast<uint64_t>(x); });

      thing.def("__pow__", [](Perm16 const& self, int n) {
        if (n == -1) {
          return self.inverse();
        }
        // TODO implement
        throw pybind11::type_error("unsupported operand type(s) ** or pow(): "
                                   "'Perm16' and 'int");
      });

      thing.def("__repr__",
                [](Perm16 const& self) { return repr(self, "Perm16"); });

      ////////////////////////////////////////////////////////////////////////
      // Static methods
      ////////////////////////////////////////////////////////////////////////

      thing.def_static("one",
                       &Perm16::one,
                       R"pbdoc(
:sig=one() -> Perm16:

Returns the identity permutation.

This function returns the identity :any:`Perm16` which fixes every value in
:math:`[0, 16)`.

:returns: The identity permutation.
:rtype: Perm16

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import Perm16
    >>> x = Perm16([1, 2, 0])
    >>> x * x.one() == Perm16.one() * x == x
    True
)pbdoc");

      thing.def_static(
          "elementary_transposition",
          [](size_t i) {
            if (i >= 15) {
              LIBSEMIGROUPS_EXCEPTION("argument out of range, expected a value "
                                      "in [0, 16) but found {}",
                                      i);
            }
            return Perm16::elementary_transposition(i);
          },
          py::arg("i"),
          R"pbdoc(
:sig=elementary_transposition(i: int) -> Perm16:

Returns the elementary transposition swapping *i* and *i* + 1.

This function returns the elementary transposition swapping *i* and *i* + 1.
for every value *i* in :math:`[0, 15)`.

:param i: the minimum value to be transposed.
:type i: int

:returns: The transposition :math:`(i\ i + 1)`.
:rtype: Perm16

:raises LibsemigroupsError:
  if any value in *i* is greater than or equal to ``15``.

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import Perm16
    >>> Perm16.elementary_transposition(2)
    Perm16([ 0, 1, 3, 2, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15])
)pbdoc");

      thing.def_static(
          "unrankSJT",
          // If anything other than 16 is passed as argument to unrankSJT, then
          // the returned "permutation" is padding with trailing 0s and so not
          // a permutation. Hence skipping this arg.
          [](uint64_t r) { return Perm16::unrankSJT(16, r); },
          py::arg("r"),
          R"pbdoc(
:sig=unrankSJT(r: int) -> Perm16:

Returns the *r*-th permutation of size ``16`` in the Steinhaus–Johnson–Trotter
order.

This function returns the *r*-th permutation of size ``16`` in the
Steinhaus–Johnson–Trotter order.

:param r: The index.
:type r: int

:returns: The *r*-th permutation.
:rtype: Perm16

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import Perm16
    >>> Perm16.unrankSJT(2)
    Perm16([ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,15,13,14])
)pbdoc");

      // TODO implement when
      // https://github.com/libsemigroups/HPCombi/issues/60
      // is resolved.
      //      thing.def_static("random",
      //                       &Perm16::random,
      //                       py::arg("bound") = 16,
      //                       R"pbdoc(
      // A random permutation on ``0`` up to .
      // )pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // Constructors
      ////////////////////////////////////////////////////////////////////////

      thing.def(py::init<>(), R"pbdoc(
:sig=(self: Perm16) -> None:

Default constructor.

Constructs a :any:`Perm16` object with its entries uninitialized. This means
there is no guarantee about the values in the constructed object.

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import Perm16
   >>> x = Perm16()
)pbdoc");

      thing.def(py::init([](std::vector<uint8_t> img) {
                  return ::libsemigroups::make<Perm16>(std::move(img));
                }),
                R"pbdoc(
:sig=(self: Perm16, img: list[int]) -> None:

Construct a :any:`Perm16` from a list of images.

This function constructs a :any:`Perm16` from the list *img* of its entries.
If the length of *img* is less than ``16``, then the constructed :any:`Perm16`
is padded with fixed points at the end.

:param img: The list of images.
:type img: list[int]

:raises LibsemigroupsError: if any value in *img* is in the range :math:`[16, 256)`.
:raises LibsemigroupsError: if the length of *img* exceeds ``16``.
:raises LibsemigroupsError: if there are repeated values in *img*.
:raises TypeError: if any value in *img* is larger than ``255``.

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import Perm16
   >>> Perm16([1, 3, 2, 0])
   Perm16([ 1, 3, 2, 0, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15])
)pbdoc");

      thing.def(py::init<uint64_t>(), R"pbdoc(
:sig=(self: Perm16, n: int) -> None:

Construct a permutation from its 64 bits compressed.

This function constructs a :any:`Perm16` from its integer representation *n*.

:param n: the integer representation.
:type n: int

:raises TypeError: if *n* is not in the range :math:`[0, 2 ^{64})`.

.. warning::
  For most values of *n* the returned object is not a valid permutation.

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import Perm16
   >>> Perm16(17863200008537477760)
   Perm16([ 0, 2, 1, 4, 3, 5, 6, 7, 8, 9,10,11,12,13,14,15])
   >>> int(Perm16([0, 2, 1, 4, 3]))
   17863200008537477760
)pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // Other methods
      ////////////////////////////////////////////////////////////////////////

      thing.def(
          "copy",
          [](Perm16 const& v) { return Perm16(v); },
          R"pbdoc(
:sig=(self: Perm16) -> Perm16:

Copy a :any:`Perm16`.

:returns: A copy of the argument.
:rtype: Perm16

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import Perm16
    >>> x = Perm16([1, 2, 3, 4, 0])
    >>> x.copy() is not x
    True
    >>> x.copy() == x
    True
)pbdoc");

      thing.def("validate",
                &Perm16::validate,
                py::arg("bound") = 16,
                R"pbdoc(
:sig=(self: Perm16, bound: int = 16) -> bool:

Check whether or not a :any:`Perm16` is well-defined.

This function returns ``True`` if *self* is a well-defined permutation
(i.e. no image value is larger than ``15`` and
no repeated image value) on the values ``0`` up to *bound*.

:param bound: the bound (defaults to ``16``).
:type bound: int

:returns: Whether or not *self* is valid.
:rtype: bool

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import Perm16
    >>> Perm16([1, 0, 2]).validate()
    True
    >>> x = Perm16([1, 0, 2])
    >>> x[0] = 0
    >>> x.validate()
    False
)pbdoc");

      thing.def("inverse",
                &Perm16::inverse,
                R"pbdoc(
:sig=(self: Perm16) -> Perm16:

Returns the inverse permutation.

This function returns the inverse of *self*.

:returns: The inverse of *self*.
:rtype: Perm16

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import Perm16
  >>> x = Perm16([0,3,2,4,1])
  >>> x.inverse()
  Perm16([ 0, 4, 2, 1, 3, 5, 6, 7, 8, 9,10,11,12,13,14,15])
  >>> x ** -1
  Perm16([ 0, 4, 2, 1, 3, 5, 6, 7, 8, 9,10,11,12,13,14,15])
)pbdoc");

      thing.def("inverse_ref",
                &Perm16::inverse_ref,
                R"pbdoc(
:sig=(self: Perm16) -> Perm16:

Returns the inverse permutation.

This function returns the inverse of *self*. This function is the same as
:any:`inverse` but with a different algorithm (using a loop and indexed
access).

:returns: The inverse of *self*.
:rtype: Perm16

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import Perm16
  >>> x = Perm16([0,3,2,4,1])
  >>> x.inverse_ref()
  Perm16([ 0, 4, 2, 1, 3, 5, 6, 7, 8, 9,10,11,12,13,14,15])
)pbdoc");

      thing.def("inverse_arr",
                &Perm16::inverse_arr,
                R"pbdoc(
:sig=(self: Perm16) -> Perm16:

Returns the inverse permutation.

This function returns the inverse of *self*. This function is the same as
:any:`inverse` but with a different algorithm (using reference cast to arrays).

:returns: The inverse of *self*.
:rtype: Perm16

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import Perm16
  >>> x = Perm16([0,3,2,4,1])
  >>> x.inverse_arr()
  Perm16([ 0, 4, 2, 1, 3, 5, 6, 7, 8, 9,10,11,12,13,14,15])
)pbdoc");

      thing.def("inverse_sort",
                &Perm16::inverse_sort,
                R"pbdoc(
:sig=(self: Perm16) -> Perm16:

Returns the inverse permutation.

This function returns the inverse of *self*. This function is the same as
:any:`inverse` but with a different algorithm: insert the identity in the least
significant bits and sort using a sorting network.

:returns: The inverse of *self*.
:rtype: Perm16

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import Perm16
  >>> x = Perm16([0,3,2,4,1])
  >>> x.inverse_sort()
  Perm16([ 0, 4, 2, 1, 3, 5, 6, 7, 8, 9,10,11,12,13,14,15])
)pbdoc");

      thing.def("inverse_find",
                &Perm16::inverse_find,
                R"pbdoc(
:sig=(self: Perm16) -> Perm16:

Returns the inverse permutation.

This function returns the inverse of *self*. This function is the same as
:any:`inverse` but with a different algorithm (some kind of vectorized
dichotomic search).

:returns: The inverse of *self*.
:rtype: Perm16

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import Perm16
  >>> x = Perm16([0,3,2,4,1])
  >>> x.inverse_find()
  Perm16([ 0, 4, 2, 1, 3, 5, 6, 7, 8, 9,10,11,12,13,14,15])
)pbdoc");

      thing.def("inverse_pow",
                &Perm16::inverse_pow,
                R"pbdoc(
:sig=(self: Perm16) -> Perm16:

Returns the inverse permutation.

This function returns the inverse of *self*. This function is the same as
:any:`inverse` but with a different algorithm (raising to the power
:math:`\text{lcm}(1, 2, ..., n) - 1`).

:returns: The inverse of *self*.
:rtype: Perm16

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import Perm16
  >>> x = Perm16([0,3,2,4,1])
  >>> x.inverse_pow()
  Perm16([ 0, 4, 2, 1, 3, 5, 6, 7, 8, 9,10,11,12,13,14,15])
)pbdoc");

      thing.def("inverse_cycl",
                &Perm16::inverse_cycl,
                R"pbdoc(
:sig=(self: Perm16) -> Perm16:

Returns the inverse permutation.

This function returns the inverse of *self*. This function is the same as
:any:`inverse` but with a different algorithm (compute power from :math:`n/2`
to :math:`n`, when :math:`\sigma^k(i)=i` then
:math:`\sigma^{-1}(i)=\sigma^{k-1}(i)`).

:returns: The inverse of *self*.
:rtype: Perm16

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import Perm16
  >>> x = Perm16([0,3,2,4,1])
  >>> x.inverse_pow()
  Perm16([ 0, 4, 2, 1, 3, 5, 6, 7, 8, 9,10,11,12,13,14,15])
)pbdoc");

      thing.def(
          "lehmer",
          [](Perm16 const& self) { return Vect16(self.lehmer()); },
          R"pbdoc(
:sig=(self: Perm16) -> Vect16:

Returns the Lehmer code of a permutation.

This function returns the Lehmer code of a permutation computed using fast
vector comparison.

:returns: the Lehmer code.
:rtype: Vect16

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import Perm16
  >>> Perm16([0,3,2,4,1]).lehmer()
  Vect16([ 0, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
)pbdoc");

      thing.def(
          "lehmer_ref",
          [](Perm16 const& self) { return Vect16(self.lehmer_ref()); },
          R"pbdoc(
:sig=(self: Perm16) -> Vect16:

Returns the Lehmer code of a permutation.

This function returns the Lehmer code of a permutation computed
using loop and indexed access.

:returns: the Lehmer code.
:rtype: Vect16

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import Perm16
  >>> Perm16([0,3,2,4,1]).lehmer_ref()
  Vect16([ 0, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
)pbdoc");

      thing.def(
          "lehmer_arr",
          [](Perm16 const& self) { return Vect16(self.lehmer_arr()); },
          R"pbdoc(
:sig=(self: Perm16) -> Vect16:

Returns the Lehmer code of a permutation.

This function returns the Lehmer code of a permutation computed
using array, loop, and indexed access.

:returns: the Lehmer code.
:rtype: Vect16

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import Perm16
  >>> Perm16([0,3,2,4,1]).lehmer_arr()
  Vect16([ 0, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
)pbdoc");

      thing.def("length",
                &Perm16::length,
                R"pbdoc(
:sig=(self: Perm16) -> int:

Returns the Coxeter length of a permutation.

This function returns the Coxeter length (i.e. number of inversions) of the
permutation *self* (using vector Lehmer and fast horizontal sum).

:returns: The Coxeter length.
:rtype: int

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import Perm16
  >>> Perm16([0,3,2,4,1]).length()
  4
)pbdoc");

      thing.def("length_ref",
                &Perm16::length_ref,
                R"pbdoc(
:sig=(self: Perm16) -> int:

Returns the Coxeter length of a permutation.

This function returns the Coxeter length (i.e. number of inversions) of the
permutation *self* (using loop and indexed access).

:returns: The Coxeter length.
:rtype: int

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import Perm16
  >>> Perm16([0,3,2,4,1]).length_ref()
  4
)pbdoc");

      thing.def("length_arr",
                &Perm16::length_arr,
                R"pbdoc(
:sig=(self: Perm16) -> int:

Returns the Coxeter length of a permutation.

This function returns the Coxeter length (i.e. number of inversions) of the
permutation *self* (using loop and indexed access after a cast to
``std::array``).

:returns: The Coxeter length.
:rtype: int

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import Perm16
  >>> Perm16([0,3,2,4,1]).length_arr()
  4
)pbdoc");

      thing.def("nb_descents",
                &Perm16::nb_descents,
                R"pbdoc(
:sig=(self: Perm16) -> int:

Returns the number of descent of a permutation.

This function returns the the number of descent of a permutation.
(found using vector shift and comparison).

:returns: The number of descents in *self*.
:rtype: int

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import Perm16
  >>> Perm16([0,3,2,4,1]).nb_descents()
  2
)pbdoc");

      thing.def("nb_descents_ref",
                &Perm16::nb_descents_ref,
                R"pbdoc(
:sig=(self: Perm16) -> int:

Returns the number of descent of a permutation.

This function returns the the number of descent of a permutation.
(found using a loop).

:returns: The number of descents in *self*.
:rtype: int

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import Perm16
  >>> Perm16([0,3,2,4,1]).nb_descents_ref()
  2
)pbdoc");

      thing.def(
          "cycles_partition",
          [](Perm16 const& self) { return Vect16(self.cycles_partition()); },
          R"pbdoc(
:sig=(self: Perm16) -> Vect16:

Returns the set partition of the cycles of a permutation.

This function returns the set partition of the cycles of a permutation,
represented the vector :math:`v` where :math:`v[i]` contains the smallest
element in the cycle of :math:`v` in *self*.

:returns: The set partition of the cycles of the permutation.
:rtype: Vect16

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import Perm16
  >>> x = Perm16([1,2,3,6,0,5,4,7,8,9,10,11,12,15,14,13])
  >>> x.cycles_partition()
  Vect16([ 0, 0, 0, 0, 0, 5, 0, 7, 8, 9,10,11,12,13,14,13])
)pbdoc");

      thing.def("nb_cycles",
                &Perm16::nb_cycles,
                R"pbdoc(
:sig=(self: Perm16) -> int:

Returns the number of cycles of a permutation.

This function returns the number of cycles of a permutation.

:returns: The number of cycles.
:rtype: int

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import Perm16
  >>> x = Perm16([1,2,3,6,0,5,4])
  >>> x.nb_cycles()
  11
)pbdoc");

      thing.def("nb_cycles_ref",
                &Perm16::nb_cycles_ref,
                R"pbdoc(
:sig=(self: Perm16) -> int:

Returns the number of cycles of a permutation.

This function returns the number of cycles of a permutation (different
algorithm using a boolean vector).

:returns: The number of cycles.
:rtype: int

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import Perm16
  >>> x = Perm16([1,2,3,6,0,5,4])
  >>> x.nb_cycles_ref()
  11
)pbdoc");

      thing.def("nb_cycles_unroll",
                &Perm16::nb_cycles_unroll,
                R"pbdoc(
:sig=(self: Perm16) -> int:

Returns the number of cycles of a permutation.

This function returns the number of cycles of a permutation (different
algorithm using :any:`cycles_partition`).

:returns: The number of cycles.
:rtype: int

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import Perm16
  >>> x = Perm16([1,2,3,6,0,5,4])
  >>> x.nb_cycles_unroll()
  11
)pbdoc");

      thing.def("left_weak_leq",
                &Perm16::left_weak_leq,
                py::arg("other"),
                R"pbdoc(
:sig=(self: Perm16, other: Perm16) -> bool:

Compare two permutations using the left weak order.

This function returns ``True`` if *self* is less than *other* in
the left weak order. This function uses :any:`length`.

:param other: the permutation for comparison.
:type other: Perm16

:returns: Whether *self* is less than or equal to *other*.
:rtype: bool

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import Perm16
  >>> x, y = Perm16([2, 0, 3, 1]), Perm16([3, 0, 2, 1])
  >>> x.left_weak_leq(y)
  True
)pbdoc");

      thing.def("left_weak_leq_ref",
                &Perm16::left_weak_leq_ref,
                py::arg("other"),
                R"pbdoc(
:sig=(self: Perm16, other: Perm16) -> bool:

Compare two permutations using the left weak order.

This function returns ``True`` if *self* is less than *other* in the left weak
order. This function implements an algorithm testing inclusion of inversions
one by one.

:param other: the permutation for comparison.
:type other: Perm16

:returns: Whether *self* is less than or equal to *other*.
:rtype: bool

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import Perm16
  >>> x, y = Perm16([2, 0, 3, 1]), Perm16([3, 0, 2, 1])
  >>> x.left_weak_leq_ref(y)
  True
)pbdoc");

      thing.def("left_weak_leq_length",
                &Perm16::left_weak_leq_length,
                py::arg("other"),
                R"pbdoc(
:sig=(self: Perm16, other: Perm16) -> bool:

Compare two permutations using the left weak order.

This function returns ``True`` if *self* is less than *other* in the left weak
order. This function implements an algorithm with vectorized test of inclusion.

:param other: the permutation for comparison.
:type other: Perm16

:returns: Whether *self* is less than or equal to *other*.
:rtype: bool

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import Perm16
  >>> x, y = Perm16([2, 0, 3, 1]), Perm16([3, 0, 2, 1])
  >>> x.left_weak_leq_length(y)
  True
)pbdoc");
    }  // init_hpcombi_perm16

    ////////////////////////////////////////////////////////////////////////
    // PPerm16
    ////////////////////////////////////////////////////////////////////////

    void init_hpcombi_pperm16(py::module& m) {
      py::class_<PPerm16, PTransf16> thing(m,
                                           "hpcombi_PPerm16",
                                           R"pbdoc(
Class representing partial permutations.

SIMD accelerated class :any:`PPerm16` representing partial permutations on
up to ``16`` points (i.e. bijections between subsets of :math:`\{0\dots 15\}`).
Undefined images are encoded as ``255``.

This class belongs to the ``hpcombi`` subpackage of ``libsemigroups_pybind11``.

The functionality described on this page is only available if
:any:`LIBSEMIGROUPS_HPCOMBI_ENABLED` is ``True``.

:any:`Perm16` inherits from :any:`PTransf16`.
)pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // Special methods
      ////////////////////////////////////////////////////////////////////////

      thing.def("__copy__", [](PPerm16 const& self) { return PPerm16(self); });

      thing.def("__repr__",
                [](PPerm16 const& self) { return repr(self, "PPerm16"); });

      thing.def("__pow__", [](PPerm16 const& self, int n) {
        if (n == -1) {
          return self.inverse_ref();
        }
        // TODO implement other powers
        throw pybind11::type_error("unsupported operand type(s) ** or pow(): "
                                   "'PPerm16' and 'int");
      });

      ////////////////////////////////////////////////////////////////////////
      // Static methods
      ////////////////////////////////////////////////////////////////////////

      thing.def_static("one",
                       &PPerm16::one,
                       R"pbdoc(
:sig=one() -> PPerm16:

Returns the identity permutation.

This function returns the identity :any:`PPerm16` which fixes every value in
:math:`[0, 16)`.

:returns: The identity permutation.
:rtype: PPerm16

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import PPerm16
    >>> x = PPerm16([1, 2, 0])
    >>> x * x.one() == PPerm16.one() * x == x
    True
)pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // Constructors
      ////////////////////////////////////////////////////////////////////////

      thing.def(py::init<>(), R"pbdoc(
:sig=(self: PPerm16) -> None:

Default constructor.

Constructs a :any:`PPerm16` object with its entries uninitialized. This means
there is no guarantee about the values in the constructed object.

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import PPerm16
   >>> x = PPerm16()
)pbdoc");

      thing.def(  // Seems like the last arg does nothing
          py::init([](std::vector<uint8_t> dom, std::vector<uint8_t> ran) {
            return ::libsemigroups::make<PPerm16>(dom, ran, 16);
          }),
          R"pbdoc(
:sig=(self: PPerm16, dom: list[int], im: list[int]) -> None:

Construct from domain and image.

Constructs a partial permutation of degree *n* such that ``(dom[i])f =
im[i]`` for all ``i`` and which is undefined (``255`` represents undefined in
this context) on every other value in the range :math:`[0, n)`.

:param dom: the domain.
:type dom: list[int]
:param im: the image.
:type im: list[int]

:raises LibsemigroupsError: *dom* and *im* do not have the same size.
:raises LibsemigroupsError: any value in *dom* or *im* is greater than *15*.
:raises LibsemigroupsError: there are repeated entries in *dom* or *im*.
:raises TypeError: any value in *dom* or *im* is greater than *15*.

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import PPerm16
    >>> PPerm16([1, 2], [3, 4])
    PPerm16([255, 3, 4,255,255,255,255,255,255,255,255,255,255,255,255,255])
)pbdoc");

      thing.def(py::init([](std::vector<uint8_t> img) {
                  return ::libsemigroups::make<PPerm16>(std::move(img));
                }),
                R"pbdoc(
:sig=(self: PPerm16, img: list[int]) -> None:

Construct a :any:`PPerm16` from a list of images.

This function constructs a :any:`PPerm16` from the list *img* of its entries.
If the length of *img* is less than ``16``, then the constructed :any:`PPerm16`
is padded with fixed points at the end.

:param img: The list of images.
:type img: list[int]

:raises LibsemigroupsError:
  if any value in *img* exceeds ``16`` and is not equal to ``255``.
:raises LibsemigroupsError: if the length of *img* exceeds ``16``.
:raises TypeError: if any value in *img* is larger than ``255``.

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import PPerm16
   >>> PPerm16([1, 255, 4, 10])
   PPerm16([ 1,255, 4,10, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15])
)pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // Other methods
      ////////////////////////////////////////////////////////////////////////

      thing.def(
          "copy",
          [](PPerm16 const& v) { return PPerm16(v); },
          R"pbdoc(
:sig=(self: PPerm16) -> PPerm16:

Copy a :any:`PPerm16`.

:returns: A copy of the argument.
:rtype: PPerm16

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import PPerm16
    >>> x = PPerm16([1, 2, 3, 4, 0])
    >>> x.copy() is not x
    True
    >>> x.copy() == x
    True
)pbdoc");

      thing.def("validate",
                &PPerm16::validate,
                py::arg("bound") = 16,
                R"pbdoc(
:sig=(self: PPerm16, bound: int = 16) -> bool:

Check whether or not a :any:`PPerm16` is well-defined.

This function returns ``True`` if *self* is a well-defined partial permutation
(i.e. no defined image value is larger than ``15`` and no repeated image value)
on the values ``0`` up to *bound*.

:param bound: the bound (defaults to ``16``).
:type bound: int

:returns: Whether or not *self* is valid.
:rtype: bool

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import PPerm16
    >>> PPerm16([1, 0, 2]).validate()
    True
    >>> x = PPerm16([1, 0, 2])
    >>> x[0] = 0
    >>> x.validate()
    False
)pbdoc");

      thing.def("inverse_ref",
                &PPerm16::inverse_ref,
                R"pbdoc(
:sig=(self: PPerm16) -> PPerm16:

Returns the inverse permutation.

This function returns the inverse of *self*. The inverse of a partial perm
:math:`x` is the unique partial permutation :math:`y` such that :math:`xyx = x`
and :math:`xyx = x`.

:returns: The inverse of *self*.
:rtype: PPerm16

.. doctest::

  >>> from libsemigroups_pybind11.hpcombi import PPerm16
  >>> x = PPerm16([0,3,2,4,1])
  >>> x.inverse_ref()
  PPerm16([ 0, 4, 2, 1, 3, 5, 6, 7, 8, 9,10,11,12,13,14,15])
)pbdoc");

      thing.def("right_one",
                // This is not a typo, everything is reversed in HPCombi
                &PPerm16::left_one,
                R"pbdoc(
:sig=(self: PPerm16) -> PPerm16:
Returns the right one of a partial transformation.

This function returns a newly constructed :any:`PPerm16` with the same image as
*self* and that acts as the identity on *self* by right multiplication.

:returns: The right one of *self*.
:rtype: PPerm16

.. doctest::

    >>> from libsemigroups_pybind11.hpcombi import PPerm16
    >>> x = PPerm16([1, 3, 4, 255, 10])
    >>> x * x.right_one() == x
    True
)pbdoc");

      thing.def("left_one",
                // This is not a typo, everything is reversed in HPCombi
                &PPerm16::right_one,
                R"pbdoc(
:sig=(self: PPerm16) -> PPerm16:

Returns the left one of a partial transformation.

This function returns a newly constructed :any:`PPerm16` with the same image as
*self* and that acts as the identity on *self* by left multiplication.

:returns: The left one of *self*.
:rtype: PPerm16

.. doctest::

   >>> from libsemigroups_pybind11.hpcombi import PPerm16
   >>> x = PPerm16([1, 3, 4, 255, 10])
   >>> x.left_one() * x == x
   True
)pbdoc");
    }  // init_hpcombi_pperm16
  }  // namespace
}  // namespace HPCombi

namespace libsemigroups {
  void init_hpcombi(py::module& m) {
    HPCombi::init_hpcombi_vect16(m);
    HPCombi::init_hpcombi_ptransf16(m);
    HPCombi::init_hpcombi_transf16(m);
    HPCombi::init_hpcombi_perm16(m);
    HPCombi::init_hpcombi_pperm16(m);
  }
}  // namespace libsemigroups
#endif
