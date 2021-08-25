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

#include "doc-strings.hpp"
#include "main.hpp"

namespace libsemigroups {
  namespace {

    template <typename T>
    py::list convert(detail::DynamicArray2<T> const &da) {
      py::list result;
      for (size_t i = 0; i < da.number_of_rows(); ++i) {
        py::list row;
        for (size_t j = 0; j < da.number_of_cols(); ++j) {
          row.append(da.get(i, j));
        }
        result.append(row);
      }
      return result;
    }

    template <typename T, typename S = FroidurePinTraits<T>>
    void bind_froidure_pin(py::module &m, std::string typestr) {
      using Class              = FroidurePin<T, S>;
      using const_element_type = typename FroidurePin<T, S>::const_element_type;
      using element_type       = typename FroidurePin<T, S>::element_type;
      using const_reference    = typename FroidurePin<T, S>::const_reference;
      std::string pyclass_name = std::string("FroidurePin") + typestr;
      py::class_<Class, std::shared_ptr<Class>>(m,
                                                pyclass_name.c_str(),
                                                py::buffer_protocol(),
                                                py::dynamic_attr(),
                                                R"pbdoc(
                          This class implements the :cite:`Froidure1997aa`
                          algorithm for the element type :py:class:`Transf1`.
                        )pbdoc")
          .def(py::init<>(),
               R"pbdoc(
                 Default constructor.

                 Constructs a :py:class:`FroidurePinTransf1` instance with no
                 generators.

                 :Parameters: None

                 .. seealso:: :py:meth:`add_generator` and :py:meth:`add_generators`.
               )pbdoc")
          .def(py::init<Class const &>(),
               R"pbdoc(
                 Copy constructor.

                 Constructs a new :py:class:`FroidurePinTransf1` which is an
                 exact copy of ``that``. No enumeration is triggered for either
                 ``that`` or of the newly constructed
                 :py:class:`FroidurePinTransf1`.

                 :Parameters:
                   **that** (FroidurePinTransf1) the ``FroidurePin`` to copy.
               )pbdoc")
          .def("size",
               &Class::size,
               R"pbdoc(
                 Returns the size.

                 :Parameters: None
                 :return: An ``int``.
               )pbdoc")
          .def("add_generator",
               &Class::add_generator,
               py::arg("x"),
               R"pbdoc(
                 Add a copy of an element to the generators.

                 :param x: the generator to add.
                 :type x: Transf1

                 :return: None
               )pbdoc")
          .def("number_of_generators",
               &Class::number_of_generators,
               R"pbdoc(
              Returns the number of generators.

              :Parameters: None
              :return: An ``int``.
            )pbdoc")
          .def("batch_size",
               py::overload_cast<size_t>(&Class::batch_size),
               py::arg("batch_size"),
               R"pbdoc(
                 Set a new value for the batch size.

                 :Parameters: **batch_size** (int) - the new value for the batch size.

                 :Returns: ``self``.
               )pbdoc")
          .def("batch_size",
               py::overload_cast<>(&Class::batch_size, py::const_),
               R"pbdoc(
                 Returns the current value of the batch size.

                 :Parameters: None
                 :Returns: A ``int``.
               )pbdoc")
          .def("max_threads",
               py::overload_cast<size_t>(&Class::max_threads),
               py::arg("number_of_threads"),
               R"pbdoc(
                 Set the maximum number of threads.

                 :Parameters: **number_of_threads** (int) - the maximum number of
                              threads to use.

                 :Returns: ``self``.
               )pbdoc")
          .def("max_threads",
               py::overload_cast<>(&Class::max_threads, py::const_),
               R"pbdoc(
                 Returns the current value of the maximum number of threads.

                 :Parameters: None
                 :Returns: A ``int``.
               )pbdoc")
          .def("concurrency_threshold",
               py::overload_cast<size_t>(&Class::concurrency_threshold),
               py::arg("thrshld"),
               R"pbdoc(
                 Set the threshold for concurrency to be used by member functions.

                 :Parameters: **thrshld** (int) - the new threshold.

                 :Returns: ``self``.
               )pbdoc")
          .def("concurrency_threshold",
               py::overload_cast<>(&Class::concurrency_threshold, py::const_),
               R"pbdoc(
                 Returns the current value of the concurrency threshold.

                 :Parameters: None
                 :Returns: A ``int``.
               )pbdoc")
          .def("reserve",
               &Class::reserve,
               R"pbdoc(
                 Requests the given capacity for elements.

                 :param val: the number of elements to reserve space for.
                 :type val: int

                 :return: None
               )pbdoc")
          .def("immutable",
               py::overload_cast<bool>(&Class::immutable),
               py::arg("val"),
               R"pbdoc(
                 Set immutability.

                 :Parameters: **val** (bool) - the new value.

                 :Returns: ``self``.
               )pbdoc")
          .def("immutable",
               py::overload_cast<>(&Class::immutable, py::const_),
               R"pbdoc(
                 Returns the current value of immutability.

                 :Parameters: None
                 :Returns: A ``bool``.
               )pbdoc")
          .def("is_monoid",
               &Class::is_monoid,
               R"pbdoc(
                 Check if the semigroup is a monoid.

                 :Parameters: None
                 :return:
                   ``True`` if the semigroup contains the identity of its element
                   type, and ``False`` if not.
               )pbdoc")
          .def(
              "current_size",
              [](Class const &x) { return x.current_size(); },
              R"pbdoc(
               Returns the number of elements so far enumerated.

                 :Parameters: None
                 :return: An ``int``.
               )pbdoc")
          .def(
              "current_number_of_rules",
              [](Class const &x) { return x.current_number_of_rules(); },
              R"pbdoc(
                 Returns the number of relations that have been found so far.

                 :Parameters: None
                 :return: An ``int``.
               )pbdoc")
          .def("enumerate",
               &FroidurePinBase::enumerate,
               py::arg("limit"),
               R"pbdoc(
                 Enumerate until at least a specified number of elements are
                 found.

                 :param limit: the limit for :py:meth:`current_size`
                 :type limit: int

                 :return: (None)
               )pbdoc")
          .def(
              "right_cayley_graph",
              [](Class &x) { return convert(x.right_cayley_graph()); },
              R"pbdoc(
                 Returns the right Cayley graph.

                 :Parameters: None

                 :return: A ``List[List[int]]``.
               )pbdoc")
          .def(
              "left_cayley_graph",
              [](Class &x) { return convert(x.left_cayley_graph()); },
              R"pbdoc(
                 Returns the left Cayley graph.

                 :Parameters: None
                 :return: A ``List[List[int]]``.
               )pbdoc")
          .def(
              "current_max_word_length",
              [](Class const &x) { return x.current_max_word_length(); },
              R"pbdoc(
                 Returns the maximum length of a word in the generators so far
                 computed.

                 :Parameters: None
                 :return: An ``int``.
               )pbdoc")
          .def("current_position",
               py::overload_cast<word_type const &>(&Class::current_position,
                                                    py::const_),
               py::arg("w"),
               R"pbdoc(
                 Returns the position corresponding to a word.

                 :Parameters: **w** (List[int]) - a word in the generators
                 :Returns: An ``int`` or :py:obj:`UNDEFINED`.
               )pbdoc")
          .def("current_position",
               py::overload_cast<letter_type>(&Class::current_position,
                                              py::const_),
               py::arg("i"),
               R"pbdoc(
                 Returns the position in of the generator with specified index.

                 :Parameters: **i** (int) - the index of the generator
                 :Returns: An ``int``.
               )pbdoc")
          .def("current_position",  // FIXME This doesn't work for some reason
               py::overload_cast<const_reference>(&Class::current_position,
                                                  py::const_),
               py::arg("x"),
               R"pbdoc(
                 Find the position of an element with no enumeration.

                 :Parameters: **x** (Transf1) - a possible element.
                 :Returns: An ``int``.
               )pbdoc")
          .def("minimal_factorisation",
               py::overload_cast<element_index_type>(
                   &Class::minimal_factorisation),
               py::arg("pos"),
               R"pbdoc(
                 Returns a short-lex least word representing an element given by
                 index.

                 :Parameters: **pos** (int) - the index of the element whose
                              factorisation is sought

                 :Returns: A ``List[int]``.
               )pbdoc")
          .def("factorisation",
               py::overload_cast<element_index_type>(&Class::factorisation),
               py::arg("pos"),
               R"pbdoc(
                 Returns a word representing an element given by index.

                 :Parameters: **pos** (int) - the index of the element whose
                              factorisation is sought

                 :Returns: A ``List[int]``.
               )pbdoc")
          .def("factorisation",
               py::overload_cast<const_reference>(&Class::factorisation),
               py::arg("x"),
               R"pbdoc(
                 Factorise an element as a word in the generators.

                 :Parameters: **x** (Transf1) - the possible element to
                              factorise.

                 :Returns: A ``List[int]``.
               )pbdoc")
          .def("number_of_rules",
               &Class::number_of_rules,
               R"pbdoc(
                 Returns the total number of relations in the presentation.

                 :Parameters: None
                 :return: An ``int``.
               )pbdoc")
          .def(
              "rules",
              [](Class const &x) {
                return py::make_iterator(x.cbegin_rules(), x.cend_rules());
              },
              R"pbdoc(
               Returns an iterator pointing to the first rule (if any).

                 :Parameters: None
                 :return: An iterator.
               )pbdoc")
          .def("current_length",
               &libsemigroups::FroidurePinBase::current_length,
               py::arg("pos"),
               R"pbdoc(
                 Returns the length of the short-lex least word.

                 :param pos: the position
                 :type pos: int

                 :return: An ``int``.
               )pbdoc")
          .def("length",
               &libsemigroups::FroidurePinBase::length,
               py::arg("pos"),
               R"pbdoc(
                 Returns the length of the short-lex least word.

                 :param pos: the position
                 :type pos: int

                 :return: An ``int``.
               )pbdoc")
          .def("product_by_reduction",
               &libsemigroups::FroidurePinBase::product_by_reduction,
               py::arg("i"),
               py::arg("j"),
               R"pbdoc(
                 Compute a product using the Cayley graph.

                 :param i: the first index of an element
                 :type i: int
                 :param j: the second index of an element
                 :type j: int

                 :return: An ``int``.
               )pbdoc")
          .def("prefix",
               &libsemigroups::FroidurePinBase::prefix,
               py::arg("pos"),
               R"pbdoc(
                 Returns the position of the longest proper prefix.

                 :param pos: the position
                 :type pos: int

                 :return: An ``int``.
               )pbdoc")
          .def("suffix",
               &libsemigroups::FroidurePinBase::suffix,
               py::arg("pos"),
               R"pbdoc(
                 Returns the position of the longest proper suffix.

                 :param pos: the position
                 :type pos: int

                 :return: An ``int``.
               )pbdoc")
          .def("first_letter",
               &libsemigroups::FroidurePinBase::first_letter,
               py::arg("pos"),
               R"pbdoc(
                 Returns the first letter of the element with specified index.

                 :param pos: the position
                 :type pos: int

                 :return: An ``int``.
                 :return: A value of type letter_type.
               )pbdoc")
          .def("final_letter",
               &libsemigroups::FroidurePinBase::final_letter,
               py::arg("pos"),
               R"pbdoc(
                 Returns the last letter of the element with specified index.

                 :param pos: the position
                 :type pos: int

                 :return: An ``int``.
               )pbdoc")
          .def(
              "degree",
              [](Class const &x) { return x.degree(); },
              R"pbdoc(
                 Returns the degree of any and all elements.

                 :Parameters: None
                 :Returns: An ``int``.
               )pbdoc")
          .def("run", &Class::run, runner_doc_strings::run)
          .def("run_for",
               (void(Class::  // NOLINT(whitespace/parens)
                         *)(std::chrono::nanoseconds))
                   & Runner::run_for,
               py::arg("t"),
               runner_doc_strings::run_for)
          .def("run_until",
               (void(Class::  // NOLINT(whitespace/parens)
                         *)(std::function<bool()> &))
                   & Runner::run_until,
               py::arg("func"),
               runner_doc_strings::run_until)
          .def("kill", &Class::kill, runner_doc_strings::kill)
          .def("dead", &Class::dead, runner_doc_strings::dead)
          .def("finished", &Class::finished, runner_doc_strings::finished)
          .def("started", &Class::started, runner_doc_strings::started)
          .def("report", &Class::report, runner_doc_strings::report)
          .def("report_every",
               (void(Class::  // NOLINT(whitespace/parens)
                         *)(std::chrono::nanoseconds))
                   & Runner::report_every,
               py::arg("t"),
               runner_doc_strings::report_every)
          .def("report_why_we_stopped",
               &Class::report_why_we_stopped,
               runner_doc_strings::report_why_we_stopped)
          .def(
              "running",
              [](Class const &x) { return x.running(); },
              runner_doc_strings::running)
          .def("timed_out", &Class::timed_out, runner_doc_strings::timed_out)
          .def("stopped", &Class::stopped, runner_doc_strings::stopped)
          .def("stopped_by_predicate",
               &Class::stopped_by_predicate,
               runner_doc_strings::stopped_by_predicate)
          .def(
              "add_generators",
              [](Class &x, std::vector<element_type> const &y) {
                x.add_generators(y);
              },
              py::arg("coll"),
              R"pbdoc(
                 Add collection of generators.

                 :Parameters: **coll** (List[Transf1]) - the collection of
                              generators to add.

                 :Returns: (None)
               )pbdoc")
          .def(
              "closure",
              [](Class &x, std::vector<element_type> const &y) {
                x.closure(y);
              },
              py::arg("coll"),
              R"pbdoc(
                Add non-redundant generators in collection.

                :Parameters: **coll** (List[Transf1]) - the collection of
                              generator to add.

                :Returns: (None)
              )pbdoc")
          .def(
              "copy_add_generators",
              [](Class &x, std::vector<element_type> const &y) {
                return x.copy_add_generators(y);
              },
              py::arg("coll"),
              R"pbdoc(
                Copy and add a collection of generators.

                :Parameters: **coll** (List[Transf1]) - the collection of
                             generators to add.

                :Returns:
                   A new ``FroidurePin`` instance generated by the
                   generators of ``self`` and ``coll``.
              )pbdoc")
          .def(
              "copy_closure",
              [](Class &x, std::vector<element_type> const &y) {
                return x.copy_closure(y);
              },
              py::arg("coll"),
              R"pbdoc(
                Copy and add non-redundant generators.

                :Parameters: **coll** (List[Transf1]) - the collection of
                             generators to add.

                :Returns:
                  A new ``FroidurePin`` instance generated by the
                  generators of ``self`` and the non-redundant generators in
                  ``coll``.
              )pbdoc")
          .def("word_to_element",
               &Class::word_to_element,
               py::arg("w"),
               R"pbdoc(
                 Convert a word in the generators to an element.

                 :param w: the word in the generators to evaluate.
                 :type w: List[int]

                 :return: A copy of the element represented by the word ``w``.
               )pbdoc")
          .def("generator",
               &Class::generator,
               py::arg("i"),
               R"pbdoc(
                 Returns the generator with specified index.

                 :param i: the index of a generator.
                 :type i: int

                 :return: An element.
               )pbdoc")
          .def("contains",
               &Class::contains,
               py::arg("x"),
               R"pbdoc(
                 Test membership of an element.

                 :param x: a possible element.
                 :type x: Transf1

                 :return: A ``bool``.
               )pbdoc")
          .def("sorted_position",
               &Class::sorted_position,
               py::arg("x"),
               R"pbdoc(
                 Returns the sorted index of an element.

                 :param x:  a possible element.
                 :type x: Transf1

                 :return: An ``int``.
               )pbdoc")
          .def("position",
               &Class::position,
               py::arg("x"),
               R"pbdoc(
                 Find the position of an element with enumeration if necessary.

                 :param x: a possible element.
                 :type x: Transf1

                 :return: An ``int``.
               )pbdoc")
          .def("sorted_at",
               &Class::sorted_at,
               py::arg("i"),
               R"pbdoc(
                 Access element specified by sorted index with bound checks.

                 :param i: the sorted index of the element to access.
                 :type i: int

                 :return: The element with index ``i`` (if any).
               )pbdoc")
          .def("at",
               &Class::at,
               py::arg("i"),
               R"pbdoc(
                 Access element specified by index with bound checks.

                 :param i: the index of the element to access.
                 :type i: int

                 :return: The element with index ``i`` (if any).
               )pbdoc")
          .def("__iter__",
               [](Class const &x) {
                 return py::make_iterator(x.cbegin(), x.cend());
               })
          .def(
              "sorted",
              [](Class &x) {
                return py::make_iterator(x.cbegin_sorted(), x.cend_sorted());
              },
              R"pbdoc(
                Returns an iterator pointing to the first element (sorted).
                :Parameters: None
                :return: An iterator..
              )pbdoc")
          .def(
              "idempotents",
              [](Class &x) {
                return py::make_iterator(x.cbegin_idempotents(),
                                         x.cend_idempotents());
              },
              R"pbdoc(
                Returns an iterator pointing at the first idempotent.

                :return: An iterator.
              )pbdoc")
          .def("number_of_idempotents",
               &Class::number_of_idempotents,
               R"pbdoc(
                 Returns the number of idempotents.

                 :Parameters: None
                 :return: An ``int``.
               )pbdoc")
          .def("is_idempotent",
               &Class::is_idempotent,
               py::arg("i"),
               R"pbdoc(
                 Check if an element is an idempotent via its index.

                 :param i: the index of the element
                 :type i: int

                 :return: A ``bool``.
               )pbdoc")
          .def("position_to_sorted_position",
               &Class::position_to_sorted_position,
               py::arg("i"),
               R"pbdoc(
                 Returns the sorted index of an element via its index.

                 :param i: the index of the element
                 :type i: int

                 :return: A value of type element_index_type.
               )pbdoc")
          .def("is_finite",
               &Class::is_finite,
               R"pbdoc(
                 Check finiteness.

                 :return: A value of type :py:obj:``tril``.
               )pbdoc")
          .def("equal_to",
               &Class::equal_to,
               py::arg("x"),
               py::arg("y"),
               R"pbdoc(
                 Check equality of words in the generators.

                 :param x: the first word for comparison
                 :type x: List[int]
                 :param y: the second word for comparison
                 :type y: List[int]

                 :return: A ``bool``.
               )pbdoc")
          .def("fast_product",
               &Class::fast_product,
               py::arg("i"),
               py::arg("j"),
               R"pbdoc(
                 Multiply elements via their indices.

                 :param i: the index of the first element to multiply
                 :type i: int
                 :param j: the index of the second element to multiply
                 :type j: int

                 :return: An ``int``.
               )pbdoc");
    }
  }  // namespace

  void init_froidure_pin(py::module &m) {
    py::class_<FroidurePinBase, std::shared_ptr<FroidurePinBase>>(
        m, "FroidurePinBase");

    bind_froidure_pin<LeastTransf<16>>(m, "Transf16");
    bind_froidure_pin<Transf<0, uint8_t>>(m, "Transf1");
    bind_froidure_pin<Transf<0, uint16_t>>(m, "Transf2");
    bind_froidure_pin<Transf<0, uint32_t>>(m, "Transf4");
    bind_froidure_pin<detail::KBE,
                      FroidurePinTraits<detail::KBE, fpsemigroup::KnuthBendix>>(
        m, "KBE");
    bind_froidure_pin<detail::TCE,
                      FroidurePinTraits<detail::TCE, detail::TCE::Table>>(
        m, "TCE");
  }
}  // namespace libsemigroups
