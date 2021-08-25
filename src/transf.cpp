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

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <libsemigroups/libsemigroups.hpp>

#include "main.hpp"

namespace py = pybind11;

namespace libsemigroups {
  namespace {
    template <typename T>
    std::string transf_repr(T const &a, char const *name) {
      auto out = std::string(name);
      out += "([";
      for (size_t i = 0; i < a.degree(); ++i) {
        out += detail::to_string(size_t(a[i]));
        if (i != a.degree() - 1) {
          out += ", ";
        }
      }
      out += "])";
      return out;
    }

    template <typename T>
    void bind_transf(py::module &m, char const *name) {
      using value_type     = typename T::value_type;
      using container_type = typename T::container_type const &;
      py::class_<T>(m,
                    name,
                    R"pbdoc(
                      Instances of this class implement transformations.

                      A transformation :math:`f` is just a function defined on
                      the whole of :math:`\{0,1,\ldots,n−1\}` for some positive
                      integer :math:`n` called the *degree* of :math:`f`. A
                      transformation is stored as an array of the images
                      :math:`\{(0)f,(1)f,\ldots,(n−1)f\}`.
                    )pbdoc")
          .def_static("make",
                      &T::template make<container_type>,
                      py::arg("l"),
                      R"pbdoc(
                        Construct and validate.

                        Constructs a transformation initialized using list ``l``
                        as follows: the image of the point ``i`` under the
                        transformation is ``l[i]``.

                        :param l: the list of images.
                        :type l: List[int]
                        :returns: A :py:class:`Transf`.
                        :raises RuntimeError:
                          if any value in ``l`` exceeds ``len(l)`` or is equal to
                          :py:obj:`UNDEFINED`.
                      )pbdoc")
          // no doc
          .def("__repr__", [name](T const &x) { return transf_repr(x, name); })
          .def(
              "__getitem__",
              [](const T &a, size_t b) { return a[b]; },
              py::arg("i"),
              py::is_operator(),
              R"pbdoc(
                Returns the image of ``i``.

                :param i: the value whose image is sought.
                :type i: int

                :return: An ``int``.
              )pbdoc")
          .def(pybind11::self == pybind11::self,
               py::arg("that"),
               R"pbdoc(
                 Equality comparison.

                 Returns ``True`` if ``self`` equals ``that`` by comparing their
                 image values.

                 :param that: the transformation for comparison.
                 :type that: Transf

                 :returns: A ``bool``.
               )pbdoc")
          .def(pybind11::self != pybind11::self)
          .def(pybind11::self < pybind11::self,
               py::arg("that"),
               R"pbdoc(
                 Less than comparison.

                 Returns ``True`` if the list of images of ``self`` is
                 lexicographically less than the list of images of ``that``.

                 :param that: the transformation for comparison.
                 :type that: Transf

                 :returns: A ``bool``.
               )pbdoc")
          .def(pybind11::self <= pybind11::self)  // no doc
          .def(pybind11::self > pybind11::self)   // no doc
          .def(pybind11::self >= pybind11::self)  // no doc
          .def(py::self * py::self,
               py::arg("that"),
               R"pbdoc(
                 Right multiply ``self`` by ``that``.

                 :param that: the transformation to multiply with
                 :type that: Transf

                 :returns: A ``Transf``.
               )pbdoc")
          .def("identity",
               py::overload_cast<>(&T::identity, py::const_),
               R"pbdoc(
                 Returns the identity transformation on :py:meth:`degree`
                 points.

                 :Parameters: None

                 :return: A ``Transf``.
               )pbdoc")
          .def_static("make_identity",
                      py::overload_cast<size_t>(&T::identity),
                      py::arg("M"),
                      R"pbdoc(
               Returns the identity transformation on the given number of
               points.

               :Parameters: **M** (int) - the degree.

               :Returns: A value of type ``Transf``.
               )pbdoc")
          .def("product_inplace",
               &T::product_inplace,
               py::arg("x"),
               py::arg("y"),
               R"pbdoc(
               Multiply two transformations and store the product in ``self``.

               :param x: a transformation.
               :type x: Transf
               :param y: a transformation.
               :type y: Transf

               :return: (None)
               )pbdoc")
          .def(
              "degree",
              [](T const &x) { return x.degree(); },
              R"pbdoc(
                 Returns the degree.

                 Returns the number of points that the transformation is
                 defined on.

                 :Parameters: None
                 :return: An ``int``.
               )pbdoc")
          .def("rank",
               &T::rank,
               R"pbdoc(
                 Returns the number of distinct image values.

                 The rank of a transformation is the number of its
                 distinct image values.

                 :Parameters: None
                 :return: An ``int``.
               )pbdoc")
          .def(
              "images",
              [](T const &x) {
                return py::make_iterator(x.cbegin(), x.cend());
              },
              R"pbdoc(
                Returns an iterator  pointing at the
                first image value.

                :Parameters: None
                :return: An iterator.
              )pbdoc");
    }
  }  // namespace

  void init_transf(py::module &m) {
    bind_transf<LeastTransf<16>>(m, "Transf16");
    bind_transf<Transf<0, uint8_t>>(m, "Transf1");
    bind_transf<Transf<0, uint16_t>>(m, "Transf2");
    bind_transf<Transf<0, uint32_t>>(m, "Transf4");
  }
}  // namespace libsemigroups

