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
    using detail::string_format;

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

    // This is the main function that installs common methods for derived
    // classes of libsemigroups::PTransf
    template <typename T>
    void bind_ptransf(py::class_<T> &x,
                      char const *   type_name,
                      char const *   long_name) {
      using value_type     = typename T::value_type;
      using container_type = typename T::container_type const &;

      x.def(
          "__getitem__",
          [](const T &a, size_t b) { return a[b]; },
          py::arg("i"),
          py::is_operator(),
          R"pbdoc(
            Returns the image of ``i``.

            :param i: the value whose image is sought.
            :type i: int

            :return: An ``int``.
          )pbdoc");
      x.def(pybind11::self != pybind11::self);  // no doc
      x.def(pybind11::self <= pybind11::self);  // no doc
      x.def(pybind11::self > pybind11::self);   // no doc
      x.def(pybind11::self >= pybind11::self);  // no doc

      std::string doc = string_format(
          R"pbdoc(
            Equality comparison.

            Returns ``True`` if ``self`` equals ``that`` by comparing their
            image values.

            :param that: the %s for comparison.
            :type that: %s

            :returns: A ``bool``.
          )pbdoc",
          long_name,
          type_name);
      x.def(pybind11::self == pybind11::self, py::arg("that"), doc.c_str());

      doc = string_format(
          R"pbdoc(
            Less than comparison.

            Returns ``True`` if the list of images of ``self`` is
            lexicographically less than the list of images of ``that``.

            :param that: the %s for comparison.
            :type that: %s

            :returns: A ``bool``.
          )pbdoc",
          long_name,
          type_name);
      x.def(pybind11::self < pybind11::self, py::arg("that"), doc.c_str());

      doc = string_format(
          R"pbdoc(
            Right multiply ``self`` by ``that``.

            :param that: the %s to multiply with.
            :type that: %s

            :returns: A ``%s``.
          )pbdoc",
          long_name,
          type_name,
          type_name);
      x.def(py::self * py::self, py::arg("that"), doc.c_str());

      doc = string_format(
          R"pbdoc(
            Returns the degree.

            Returns the number of points that the %s is defined on.

            :Parameters: ``None``
            :return: An ``int``.
          )pbdoc",
          long_name);
      x.def(
          "degree", [](T const &x) { return x.degree(); }, doc.c_str());

      x.def(
          "images",
          [](T const &x) { return py::make_iterator(x.cbegin(), x.cend()); },
          R"pbdoc(
            Returns an iterator pointing at the first image value.

            :Parameters: None
            :return: An iterator.
          )pbdoc");

      doc = string_format(
          R"pbdoc(
            Construct and validate.

            Constructs a %s initialized using list ``l`` as follows: the
            image of the point ``i`` under the transformation is ``l[i]``.

            :Parameters: **l** (List[int]) the list of images.
            :Returns: A ``%s``.

            :Raises: **RuntimeError** if any value in ``l`` exceeds ``len(l)``.
          )pbdoc",
          long_name,
          type_name);

      x.def_static(
          "make", &T::template make<container_type>, py::arg("l"), doc.c_str());

      doc = string_format(
          R"pbdoc(
            Returns the identity %s on :py:meth:`degree` points.

            :Parameters: None

            :return: A ``%s``.
          )pbdoc",
          long_name,
          type_name);

      x.def("identity",
            py::overload_cast<>(&T::identity, py::const_),
            doc.c_str());

      doc = string_format(
          R"pbdoc(
            Returns the identity %s on the given number of
            points.

            :Parameters: **M** (int) - the degree.

            :Returns: A value of type ``%s``.
          )pbdoc",
          long_name,
          type_name);

      x.def_static("make_identity",
                   py::overload_cast<size_t>(&T::identity),
                   py::arg("M"),
                   doc.c_str());

      doc = string_format(
          R"pbdoc(
            Returns the number of distinct image values.

            The rank of a %s is the number of its
            distinct image values.

            :Parameters: None
            :return: An ``int``.
          )pbdoc",
          long_name);
      x.def("rank", &T::rank, doc.c_str());

      doc = string_format(
          R"pbdoc(
            Multiply two %ss and store the product in ``self``.

            :param x: a %s.
            :type x: %s
            :param y: a %s.
            :type y: %s

            :return: (None)
          )pbdoc",
          long_name,
          long_name,
          type_name,
          long_name,
          type_name);
      x.def("product_inplace",
            &T::product_inplace,
            py::arg("x"),
            py::arg("y"),
            doc.c_str());
    }

    template <typename T>
    void bind_transf(py::module &m, char const *name) {
      using value_type     = typename T::value_type;
      using container_type = typename T::container_type const &;
      py::class_<T> x(m,
                      name,
                      R"pbdoc(
                        Instances of this class implement transformations.

                        A transformation :math:`f` is just a function defined on
                        the whole of :math:`\{0,1,\ldots,n−1\}` for some positive
                        integer :math:`n` called the *degree* of :math:`f`. A
                        transformation is stored as an array of the images
                        :math:`\{(0)f,(1)f,\ldots,(n−1)f\}`.
                      )pbdoc");
      bind_ptransf(x, name, "transformation");

      // no doc
      x.def("__repr__", [name](T const &x) { return transf_repr(x, name); });
    }

    template <typename T>
    void bind_pperm(py::module &m, char const *name) {
      using value_type     = typename T::value_type;
      using container_type = typename T::container_type const &;
      py::class_<T> x(m, name, R"pbdoc(
        A *partial permutation* :math:`f` is just an injective partial
        transformation, which is stored as a vector of the images of
        :math:`\{0, 1, \ldots, n - 1\}`, i.e.  i.e. :math:`\{(0)f, (1)f,
        \ldots, (n - 1)f\}` where the value :py:obj:`UNDEFINED` is used to
        indicate that :math:`(i)f` is undefined (i.e. not among the points
        where :math:`f` is defined).
      )pbdoc");
      bind_ptransf(x, name, "partial perm");

      x.def_static(
           "make",
           [](std::vector<value_type> const &dom,
              std::vector<value_type> const &ran,
              size_t const deg) { return T::make(dom, ran, deg); },
           py::arg("dom"),
           py::arg("ran"),
           py::arg("M"),
           R"pbdoc(
            Construct from domain, range, and degree, and validate.

            :Parameters: - **dom** (List[int]) - the domain
                         - **ran** (List[int]) - the range
                         - **M** (int) - the degree
          )pbdoc")
          .def("right_one",
               &T::right_one,
               R"pbdoc(
                 Returns the right one of this.

                 :Parameters: None.
                 :return: A ``PPerm1``.
               )pbdoc")
          .def("left_one",
               &T::left_one,
               R"pbdoc(
                 Returns the left one of this.

                 :Parameters: None.
                 :return: A ``PPerm1``.
               )pbdoc")
          .def("inverse",
               py::overload_cast<>(&T::inverse, py::const_),
               R"pbdoc(
                 Returns the inverse.

                 :Parameters: None
                 :Returns: A ``PPerm1``.
               )pbdoc")
          .def("inverse",
               py::overload_cast<T &>(&T::inverse, py::const_),
               py::arg("that"),
               R"pbdoc(
                 Replace contents of a partial perm with the inverse of another.

                 :Parameters: **that** (PPerm1) - the partial perm to invert.

                 :Returns: (None)
               )pbdoc")
          .def("undef",
               &T::undef,
               R"pbdoc(
                 Returns the integer value used to represent undefined.

                 :Parameters: None.

                 :Returns: An ``int``.
               )pbdoc");
    }
  }  // namespace

  void init_transf(py::module &m) {
    // Transformations
    bind_transf<LeastTransf<16>>(m, "Transf16");
    bind_transf<Transf<0, uint8_t>>(m, "Transf1");
    bind_transf<Transf<0, uint16_t>>(m, "Transf2");
    bind_transf<Transf<0, uint32_t>>(m, "Transf4");

    // Partial perms
    bind_pperm<LeastPPerm<16>>(m, "PPerm16");
    bind_pperm<PPerm<0, uint8_t>>(m, "PPerm1");
    bind_pperm<PPerm<0, uint16_t>>(m, "PPerm2");
    bind_pperm<PPerm<0, uint32_t>>(m, "PPerm4");
  }
}  // namespace libsemigroups

