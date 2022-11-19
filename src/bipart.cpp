//
// libsemigroups_pybind11
// Copyright (C) 2021 James D. Mitchell
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
#include <stddef.h>  // for size_t
#include <stdint.h>  // for uint32_t

// C++ stl headers....
#include <initializer_list>  // for initializer_list
#include <vector>            // for vector

// libsemigroups....
#include <libsemigroups/bipart.hpp>  // for Bipartition, operator!=, operator*

// pybind11....
#include <pybind11/operators.h>  // for self, self_t, operator!=, operator*
#include <pybind11/pybind11.h>   // for class_, make_iterator, init, module
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_bipart

namespace py = pybind11;

namespace libsemigroups {
  // TODO(later) implement Blocks and uncomment the def's below.

  void init_bipart(py::module& m) {
    py::class_<Bipartition>(m,
                            "Bipartition",
                            R"pbdoc(
   A *bipartition* is a partition of the set :math:`\{0, ..., 2n - 1\}` for
   some non-negative integer :math:`n` see the `Semigroups package for GAP
   documentation <https://semigroups.github.io/Semigroups/doc/chap3_mj.html>`_
   for more details.
                            )pbdoc")
        .def(py::init<Bipartition const&>())
        .def_static("make_identity",
                    py::overload_cast<size_t>(&Bipartition::identity),
                    py::arg("n"),
                    R"pbdoc(
                      Returns an identity bipartition.

                      :Parameters: **n** (int) - the degree of the identity to be
                                               returned.

                      :Returns: A newly constructed ``Bipartition``.
                   )pbdoc")
        .def("identity",
             py::overload_cast<>(&Bipartition::identity, py::const_),
             R"pbdoc(
               Returns an identity bipartition.


               :Returns: A newly constructed ``Bipartition``.
             )pbdoc")
        .def_static("make",
                    &Bipartition::make<std::vector<uint32_t> const&>,
                    R"pbdoc(
                      Validates the arguments, constructs a bipartition and
                      validates it.
                    )pbdoc")
        .def("product_inplace",
             &Bipartition::product_inplace,
             py::arg("x"),
             py::arg("y"),
             py::arg("thread_id") = 0,
             R"pbdoc(
               Modify the current bipartition in-place to contain the product
               of two bipartitions.

               :param x: the first bipartition to multiply
               :type x: Bipartition
               :param y: the second bipartition to multiply
               :type y: Bipartition
               :param thread_id: the index of the calling thread (defaults to 0)
               :type thread_id: int

               :return: (None)
             )pbdoc")
        .def(
            "__getitem__",
            [](const Bipartition& a, size_t b) -> uint32_t { return a.at(b); },
            py::arg("i"),
            py::is_operator(),
            R"pbdoc(
              Returns the index of the block containing a value.

              :param i: an integer
              :type i: int

              :return: A ``int``.
            )pbdoc")
        // TODO(later) not implemented in libsemigroups
        // .def(py::self <= py::self)  // no doc
        // .def(py::self > py::self)   // no doc
        // .def(py::self >= py::self)  // no doc
        .def(py::self != py::self)  // no doc
        .def(pybind11::self == pybind11::self,
             py::arg("that"),
             R"pbdoc(
               Equality comparison.

               Returns ``True`` if ``self`` equals ``that`` by comparing their
               image values.

               :param that: the ``Bipartition`` for comparison.
               :type that: Bipartition

               :returns: A ``bool``.
             )pbdoc")
        .def(pybind11::self < pybind11::self,
             py::arg("that"),
             R"pbdoc(
               Less than comparison.

               Returns ``True`` if ``self`` is less than ``that``.

               :param that: the ``Bipartition`` for comparison.
               :type that: Bipartition

               :returns: A ``bool``.
            )pbdoc")
        .def(py::self * py::self,
             py::arg("that"),
             R"pbdoc(
               Right multiply ``self`` by ``that``.

               :param that: the ``Bipartition`` to multiply with.
               :type that: Bipartition

               :returns: A ``Bipartition``.
             )pbdoc")
        .def("degree",
             &Bipartition::degree,
             R"pbdoc(
               Returns the degree of the ``Bipartition``.

               :Parameters: None.
               :return: An ``int``.
             )pbdoc")
        .def("is_transverse_block",
             &Bipartition::is_transverse_block,
             py::arg("index"),
             R"pbdoc(
               Check if a block is a transverse block.

               :param index: the index of a block
               :type index: int

               :return: A ``bool``.
             )pbdoc")
        .def("number_of_blocks",
             &Bipartition::number_of_blocks,
             R"pbdoc(
               Returns the number of blocks in a ``Bipartition``.

               :Parameters: None.
               :return: An ``int``.
             )pbdoc")
        .def("rank",
             &Bipartition::rank,
             R"pbdoc(
               Returns the number of transverse blocks.

               :Parameters: None.
               :return: An ``int``
             )pbdoc")
        .def("__hash__",
             &Bipartition::hash_value,
             R"pbdoc(
               Returns a hash value.

               :Parameters: None.
               :return: An ``int``
             )pbdoc")
        .def("number_of_right_blocks",
             &Bipartition::number_of_right_blocks,
             R"pbdoc(
               Returns the number of blocks containing a negative integer.

               :Parameters: None.
               :return: An ``int``.
             )pbdoc")
        .def("number_of_left_blocks",
             &Bipartition::number_of_left_blocks,
             R"pbdoc(
               Returns the number of blocks containing a positive integer.

               :Parameters: None.
               :return: An ``int``.
             )pbdoc")
        .def(
            "lookup",
            [](Bipartition& x) {
              // TODO(later) because cbegin_lookup points to a std::vector<bool>
              // I couldn't figure out how to use an make_iterator here
              return std::vector<bool>(x.cbegin_lookup(), x.cend_lookup());
            },
            R"pbdoc(
              Returns a list whose ``i``-th entry indicates whether or not the
              block with index ``i`` is transverse or not.

              :Parameters: None.
              :return: A ``list``.
            )pbdoc")
        .def(
            "left_blocks",
            [](Bipartition const& x) {
              return py::make_iterator(x.cbegin_left_blocks(),
                                       x.cend_left_blocks());
            },
            R"pbdoc(
               Returns an iterator pointing to the index of the first left
               block.

              :Parameters: None.
              :return: An iterator.
             )pbdoc")
        .def(
            "right_blocks",
            [](Bipartition const& x) {
              return py::make_iterator(x.cbegin_right_blocks(),
                                       x.cend_right_blocks());
            },
            R"pbdoc(
              Returns an iterator pointing to the index of the first right
              block.

              :Parameters: None.
              :return: An iterator.
            )pbdoc");
  }
}  // namespace libsemigroups
