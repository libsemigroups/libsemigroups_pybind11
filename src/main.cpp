#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <libsemigroups/libsemigroups.hpp>

#define RUN_UNTIL(func)                                               \
  .def("run_until",                                                   \
       (void (func::*)(std::function<bool()> &)) & Runner::run_until, \
       py::arg("func"),                                               \
       R"pbdoc(
           Run until a nullary predicate returns true or the algorithm
           finishes.

           :param func: a function.
           :type func: func

           :return: (None)
           )pbdoc")

namespace py = pybind11;

namespace libsemigroups {

  using Transf256            = Transformation<uint8_t>;
  using FroidurePinTransf256 = FroidurePin<Transf256>;
  using rule_type            = FpSemigroupInterface::rule_type;

  namespace detail {
    std::string transf_repr(Transformation<uint8_t> const &a) {
      auto out = std::string("Transf256([");
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
    void bind_froidure_pin(py::module &m, std::string typestr) {
      using Class              = FroidurePin<T>;
      std::string pyclass_name = std::string("FroidurePin") + typestr;
      py::class_<Class>(
          m, pyclass_name.c_str(), py::buffer_protocol(), py::dynamic_attr())
          .def(py::init<std::vector<T> const &>())
          .def(py::init<>())
          .def("size", &Class::size)
          .def("add_generator", &Class::add_generator)
          .def("number_of_generators", &Class::nr_generators)
          .def("batch_size", py::overload_cast<size_t>(&Class::batch_size))
          .def("batch_size",
               py::overload_cast<>(&Class::batch_size, py::const_))
          .def("max_threads", py::overload_cast<size_t>(&Class::max_threads))
          .def("max_threads",
               py::overload_cast<>(&Class::max_threads, py::const_))
          .def("concurrency_threshold",
               py::overload_cast<size_t>(&Class::concurrency_threshold))
          .def("concurrency_threshold",
               py::overload_cast<>(&Class::concurrency_threshold, py::const_))
          .def("reserve", &Class::reserve);
    }
  }  // namespace detail

  PYBIND11_MODULE(libsemigroups_pybind11, m) {
    // py::options options;
    // options.disable_function_signatures();
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: libsemigroups_pybind11

        .. autosummary::
           :toctree: _generate

        .. autofunction:: silo
    )pbdoc";

    ////////////////////////////////////////////////////////////////////////
    // element.hpp
    ////////////////////////////////////////////////////////////////////////

    py::class_<Transformation<uint8_t>>(m, "Transf256")
        .def(py::init<std::vector<uint8_t> const &>())
        .def("rank",
             &Transformation<uint8_t>::crank,
             R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc")
        .def("degree", &Transformation<uint8_t>::degree)
        .def(py::self * py::self)
        .def(
            "__getitem__",
            [](const Transformation<uint8_t> &a, size_t b) { return a[b]; },
            py::is_operator())
        .def("__repr__", &detail::transf_repr);

    ////////////////////////////////////////////////////////////////////////
    // bmat8.hpp
    ////////////////////////////////////////////////////////////////////////

    py::class_<BMat8>(m, "BMat8")
        .def(py::init<>(), R"pbdoc(
      Returns an uninitalised BMat8.
      )pbdoc")
        .def(py::init<uint64_t>())
        .def(py::init<BMat8 const &>())
        .def(py::init<std::vector<std::vector<bool>> const &>())
        .def("__eq__", &BMat8::operator==)
        .def("__lt__", &BMat8::operator<)
        .def("get",
             &BMat8::get,
             py::arg("i"),
             py::arg("j"),
             R"pbdoc(
           Returns the entry in the (i, j)th position.
           )pbdoc")
        .def("set",
             &BMat8::set,
             py::arg("i"),
             py::arg("j"),
             py::arg("val"),
             R"pbdoc(
           Sets the (i, j)th entry to ``val``.
           )pbdoc")
        .def("to_int",
             &BMat8::to_int,
             R"pbdoc(
          Returns the integer representation of the ``BMat8``, that is
          an integer obtained by interpreting an 8 x 8 ``BMat8`` as a
          sequence of 64 bits (reading rows left to right, from top to bottom)
          and then realising this sequence as an integer.
           )pbdoc")
        .def("transpose",
             &BMat8::transpose,
             R"pbdoc(
           Returns the transpose of ``self``.
           )pbdoc")
        .def(py::self * py::self)
        .def_static("random", py::overload_cast<>(&BMat8::random))
        .def_static("random", py::overload_cast<size_t>(&BMat8::random))
        .def("swap",
             &BMat8::swap,
             py::arg("other"),
             R"pbdoc(
            Swaps the contents of ``self`` and ``other``.
           )pbdoc")
        .def("row_space_basis",
             &BMat8::row_space_basis,
             R"pbdoc(
           This method returns a ``BMat8`` whose non-zero rows form a basis for
           the row space of ``self``.
           )pbdoc")
        .def("col_space_basis",
             &BMat8::col_space_basis,
             R"pbdoc(
           This method returns a ``BMat8`` whose non-zero columns form a basis for
           the column space of ``self``.
           )pbdoc")
        .def("rows",
             &BMat8::rows,
             R"pbdoc(
           This method returns a list of integers representing the rows of
           ``self``. The list will always be of length 8, even if ``self``
           was constructed with fewer rows.
           )pbdoc")
        .def("row_space_size",
             &BMat8::row_space_size,
             R"pbdoc(
           Returns the size of the row space of ``self``.
           )pbdoc")
        .def("nr_rows",
             &BMat8::nr_rows,
             R"pbdoc(
           Returns the number of non-zero rows in ``self``.
           )pbdoc")
        .def("is_regular_element",
             &BMat8::is_regular_element,
             R"pbdoc(
           Check whether ``self`` is a regular element of the full boolean
           matrix monoid of appropriate dimension.
           )pbdoc")
        .def("one",
             &BMat8::one,  // py::arg("dim") = 8,
             R"pbdoc(
           This method returns the ``BMat8`` with the first ``dim`` entries in
           the main diagonal equal to ``1`` and every other value equal to ``0``.
           )pbdoc")
        .def("__repr__", &detail::to_string<BMat8>)
        .def("nr_cols",
             &bmat8_helpers::nr_cols,
             R"pbdoc(
           Returns the number of non-zero columns in ``self``.
           )pbdoc")
        .def("column_space_size",
             &bmat8_helpers::col_space_size,
             R"pbdoc(
           Returns the size of the row space of ``self``.
           )pbdoc")
        .def("minimum_dim",
             &bmat8_helpers::minimum_dim,
             R"pbdoc(
            This member function returns the maximal ``i`` such that row ``i``
            or column ``i`` contains a ``1``.
            )pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // cong.hpp
    ////////////////////////////////////////////////////////////////////////

    py::enum_<congruence_type>(m, "congruence_type")
        .value("left", congruence_type::left)
        .value("right", congruence_type::right)
        .value("twosided", congruence_type::twosided)
        .export_values();

    py::enum_<tril>(m, "tril")
        .value("True", tril::TRUE)
        .value("False", tril::FALSE)
        .value("unknown", tril::unknown)
        .export_values();

    py::class_<Congruence>(m, "Congruence")
        .def(py::init<congruence_type>())
        .def(py::init<congruence_type, FpSemigroup &>())
        .def("set_nr_generators",
             &Congruence::set_nr_generators,
             py::arg("n"),
             R"pbdoc(
           Set the number of generators of the congruence.

           :param n: the number of generators.
           :type n: int

           :return: (None)
           )pbdoc")
        .def("nr_generators",
             &Congruence::nr_generators,
             R"pbdoc(
           Returns the number of generators.

           :return: The number of generators of the semigroup of the congruence that an object of this type represents, or UNDEFINED.
           )pbdoc")
        .def("add_pair",
             py::overload_cast<word_type const &, word_type const &>(
                 &Congruence::add_pair),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
           Add a generating pair to the congruence.

           :param u: a word (list of integers) over the generators of the semigroup.
           :type u: list
           :param v: a word (list of integers) over the generators of the semigroup.
           :type v: list

           :return: (None)
           )pbdoc")
        // overload cast didn't work for report_every
        .def("report_every",
             (void (Congruence::*)(std::chrono::nanoseconds))
                 & Runner::report_every,
             py::arg("t"),
             R"pbdoc(
           Set the minimum elapsed time between reports.

           :param t: the amount of time (in nanoseconds) between reports.
           :type t: int

           :return: (None)
           )pbdoc")
        .def("report",
             &Congruence::report,
             R"pbdoc(
           Check if it is time to report.

           :return: A bool.
           )pbdoc")
        .def("report_why_we_stopped",
             &Congruence::report_why_we_stopped,
             R"pbdoc(
           Report why Runner::run stopped.

           :return: (None)
           )pbdoc")
        .def("kill",
             &Congruence::kill,
             R"pbdoc(
           Stop Runner::run from running (thread-safe).

           :return: (None).
           )pbdoc")
        .def("run",
             &Congruence::run,
             R"pbdoc(
           Attempt to enumerate the congruence.

           :return: (None)
           )pbdoc")
        .def("run_for",
             (void (Congruence::*)(std::chrono::nanoseconds)) & Runner::run_for,
             py::arg("t"),
             R"pbdoc(
           Run for a specified amount of time.

           :param t: the time in nanoseconds to run for.
           :type t: int

           :return: (None)

           Example
           -------

           .. code-block:: python

              from datetime import timedelta
              from libsemigroups_pybind11 import Congruence
              from libsemigroups_pybind11 import congruence_type

              C = Congruence(congruence_type.twosided)
              C.set_nr_generators(1)
              C.add_pair([0] * 1000, [0] * 999)
              C.run_for(timedelta(microseconds=10))
           )pbdoc")
        .def("run_until",
             (void (Congruence::*)(std::function<bool()> &))
                 & Runner::run_until,
             py::arg("func"),
             R"pbdoc(
           Run until a nullary predicate returns true or Runner::finished.

           :param func: a function.
           :type func: func

           :return: (None)
           )pbdoc")
        .def("less",
             &Congruence::less,
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
           This function returns ``True`` if the congruence class of ``u`` is
           less than the class of ``v`` in a total ordering of congruence
           classes.

           :param u: a word (list of integers) over the generators of the semigroup.
           :type u: list
           :param v: a word (list of integers) over the generators of the semigroup.
           :type v: list

           :return: ``True`` if the class of ``u`` is less than that of ``v``.
           )pbdoc")
        .def("const_contains",
             &Congruence::const_contains,
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
           Check if a pair of words is known to belong to the congruence.

           :param u: a word (list of integers) over the generators of the semigroup
           :type u: list
           :param v: a word (list of integers) over the generators of the semigroup
           :type v: list

           :return: tril::TRUE if the words u and v are known to belong to the same congruence class tril::FALSE if the words are known to not belong to the same congruence class tril::unknown otherwise.
           )pbdoc")
        .def("nr_classes",
             &Congruence::nr_classes,
             R"pbdoc(
           Computes the total number of classes in the congruence represented
           by an instance of this type.

           :return: The number of congruences classes of this if this number is finite, or POSITIVE_INFINITY in some cases if this number is not finite.
           )pbdoc")
        .def("contains",
             &Congruence::contains,
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
           Check if a pair of words belongs to the congruence.

           :param u: a word (list of integers) over the generators of the
           semigroup.
           :type u: list
           :param v: a word (list of integers) over the generators of the
           semigroup.
           :type v: list

           :return: true if the words u and v belong to the same congruence class, and false otherwise.
           )pbdoc")
        .def("nr_classes",
             &Congruence::nr_classes,
             R"pbdoc(
           Computes the total number of classes in the congruence represented by an
           instance of this type.

           :return: The number of congruences classes of this if this number is finite, or POSITIVE_INFINITY in some cases if this number is not finite.
           )pbdoc")
        .def("nr_non_trivial_classes",
             &Congruence::nr_non_trivial_classes,
             R"pbdoc(
           Returns the number of non-trivial classes (size > 1) of the
           congruence.

           :return: The number of non-trivial classes of the congruence.
           )pbdoc")
        .def("quotient_froidure_pin",
             &Congruence::quotient_froidure_pin,
             R"pbdoc(
           Returns a semigroup represented as an instance of a derived class of
           FroidurePinBase that is isomorphic to the quotient of the parent
           semigroup of this by the 2-sided congruence that this represents.

           :return: A std::shared_ptr to FroidurePinBase.
           )pbdoc")
        .def("has_quotient_froidure_pin",
             &Congruence::has_quotient_froidure_pin,
             R"pbdoc(
           Returns true if the congruence represented by this object knows an
           isomorphic quotient semigroup represented by an instance of
           FroidurePin.

           :return: A bool.
           )pbdoc")
        .def("parent_froidure_pin",
             &Congruence::parent_froidure_pin,
             R"pbdoc(
           Returns a shared_ptr to the parent FroidurePin over which the
           congruence represented by this object was defined, if it exists.

           :return: A std::shared_ptr to FroidurePinBase.
           )pbdoc")
        .def("has_parent_froidure_pin",
             &Congruence::has_parent_froidure_pin,
             R"pbdoc(
           Returns true if the congruence represented by this was created from
           a FroidurePin instance.

           :return: A bool.
           )pbdoc")
        .def("is_quotient_obviously_finite",
             &Congruence::is_quotient_obviously_finite,
             R"pbdoc(
           Return true if the number of classes in the congruence represented
           by this is obviously finite, and false if it is not obviously
           finite.

           :return: A bool.
           )pbdoc")
        .def("is_quotient_obviously_infinite",
             &Congruence::is_quotient_obviously_infinite,
             R"pbdoc(
           Return true if the number of classes in the congruence represented
           by this is obviously infinite, and false if it is not obviously
           infinite.

           :return: A bool.
           )pbdoc")
        .def("has_todd_coxeter",
             &Congruence::has_todd_coxeter,
             R"pbdoc(
           Checks if a congruence::ToddCoxeter instance is being used to
           compute the congruence.

           :return: A bool.
           )pbdoc")
        .def("todd_coxeter",
             &Congruence::todd_coxeter,
             R"pbdoc(
           Returns the congruence::ToddCoxeter instance used to compute the
           congruence (if any).

           :return: A shared_ptr to a congruence::ToddCoxeter or nullptr.
           )pbdoc")
        .def("has_knuth_bendix",
             &Congruence::has_knuth_bendix,
             R"pbdoc(
           Checks if a congruence::KnuthBendix instance is being used to
           compute the congruence.

           :return: A bool.
           )pbdoc")
        .def("knuth_bendix",
             &Congruence::knuth_bendix,
             R"pbdoc(
           Returns the congruence::KnuthBendix instance used to compute the
           congruence (if any).

           :return: A shared_ptr to a congruence::KnuthBendix or nullptr.
           )pbdoc")
        .def("word_to_class_index",
             &Congruence::word_to_class_index,
             py::arg("w"),
             R"pbdoc(
           If the congruence, that an object of this type represents, is
           defined over a semigroup with generators $A$, then this function
           defines a surjective function from the set of all words over $A$ to
           either $\{0, 1, \ldots, n - 1\}$, where $n$ is the number of
           classes, or to the non-negative integers $\{0, 1, \ldots\}$ if this
           has infinitely many classes.

           :param w: the word whose class index we want to find. The parameter w must be a list of integers consisting of indices of the generators of the semigroup over which this is defined.
           :type w: list

           :return: The index of the congruence class corresponding to word.
           )pbdoc")
        .def("class_index_to_word",
             &Congruence::class_index_to_word,
             py::arg("i"),
             R"pbdoc(
           If the congruence, that an object of this type represents, is
           defined over a semigroup with generators $A$, then this function
           defines a injective function from $\{0, 1, \ldots, n - 1\}$, where
           $n$ is the number of classes, or to the non-negative integers $\{0,
           1, \ldots\}$ if this has infinitely many classes, to a fixed set of
           words over $A$ representing distinct congruences classes.

           :param i: the index of the class whose representative we want to
           find.
           :type i: int

           :return: The word representing the i-th class of the congruence
           )pbdoc")
        .def("kind",
             &Congruence::kind,
             R"pbdoc(
           Return if the congruence represented by this object was created as a
           left, right, or two-sided congruence.

           :return: A congruence_type.
           )pbdoc")
        .def("dead",
             &Congruence::dead,
             R"pbdoc(
           Check if the runner is dead.

           :return: A bool.
           )pbdoc")
        .def("finished",
             &Congruence::finished,
             R"pbdoc(
           Check if Runner::run has been run to completion or not.

           :return: A bool.
           )pbdoc")
        .def("started",
             &Congruence::started,
             R"pbdoc(
           Check if Runner::run has already been called.

           :return: A bool.
           )pbdoc")
        .def("stopped",
             &Congruence::stopped,
             R"pbdoc(
           Check if the runner is stopped.

           :return: A bool.
           )pbdoc")
        .def("timed_out",
             &Congruence::timed_out,
             R"pbdoc(
           Check if the amount of time specified to Runner::run_for has elapsed.

           :return: A bool
           )pbdoc")
        .def("running",
             &Congruence::running,
             R"pbdoc(
           Check if a Runner instance is currently running.

           :return: true if Runner::run is in the process to run and false it is not.
           )pbdoc")
        .def("stopped_by_predicate",
             &Congruence::stopped_by_predicate,
             R"pbdoc(
           Check if the runner was, or should, stop because the nullary
           predicate passed as first argument to Runner::run_until.

           :return: A bool.
           )pbdoc")
        .def(
            "generating_pairs",
            [](Congruence const &c) {
              return py::make_iterator(c.cbegin_generating_pairs(),
                                       c.cend_generating_pairs());
            },
            R"pbdoc(
           Returns an iterator pointing to the first generating pair of
           the congruence.
           )pbdoc")
        .def(
            "non_trivial_classes",
            [](Congruence &c) {
              return py::make_iterator(c.cbegin_ntc(), c.cend_ntc());
            },
            R"pbdoc(
          Returns an iterator pointing to the first non-trivial (size > 1)
          class of the congruence.
          )pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // fpsemi.hpp
    ////////////////////////////////////////////////////////////////////////

    py::class_<libsemigroups::FpSemigroup>(m, "FpSemigroup")
        .def(py::init<>())
        .def(py::init<std::shared_ptr<FroidurePinBase>>())
        .def(py::init<FpSemigroup const &>())
        .def("validate_letter",
             py::overload_cast<char>(
                 &libsemigroups::FpSemigroup::validate_letter, py::const_),
             py::arg("c"),
             R"pbdoc(
               Validates a letter.

               :Parameters: **c** (??) - the letter to validate.

               :Returns: (None)
               )pbdoc")
        .def("validate_letter",
             py::overload_cast<letter_type>(
                 &libsemigroups::FpSemigroup::validate_letter, py::const_),
             py::arg("c"),
             R"pbdoc(
               Validates a letter.

               :Parameters: **c** (??) - the letter to validate.

               :Returns: (None)
               )pbdoc")
        .def("validate_word",
             py::overload_cast<word_type const &>(
                 &libsemigroups::FpSemigroup::validate_word, py::const_),
             py::arg("w"),
             R"pbdoc(
               Validates a word.

               :Parameters: **w** (??) - the word to validate.

               :Returns: (None)
               )pbdoc")
        .def("validate_word",
             py::overload_cast<std::string const &>(
                 &libsemigroups::FpSemigroup::validate_word, py::const_),
             py::arg("w"),
             R"pbdoc(
               Validates a word.

               :Parameters: **w** (??) - the word to validate.

               :Returns: (None)
               )pbdoc")
        .def("set_alphabet",
             py::overload_cast<size_t>(
                 &libsemigroups::FpSemigroup::set_alphabet),
             py::arg("n"),
             R"pbdoc(
               Set the size of the alphabet.

               :Parameters: **n** (??) - the number of letters.

               :Returns: (None)
               )pbdoc")
        .def("set_alphabet",
             py::overload_cast<std::string const &>(
                 &libsemigroups::FpSemigroup::set_alphabet),
             py::arg("a"),
             R"pbdoc(
               Set the alphabet of the finitely presented semigroup.

               :Parameters: **a** (??) - the alphabet.

               :Returns: (None)
               )pbdoc")
        .def("alphabet",
             py::overload_cast<>(&libsemigroups::FpSemigroup::alphabet,
                                 py::const_),
             R"pbdoc(
               Returns the alphabet of the finitely presented semigroup represented by this.


               :Returns: A const reference to the alphabet, a value of type std::string.
               )pbdoc")
        .def("alphabet",
             py::overload_cast<size_t>(&libsemigroups::FpSemigroup::alphabet,
                                       py::const_),
             py::arg("i"),
             R"pbdoc(
               Returns the i-th letter of the alphabet of the finitely presented semigroup represented by this.

               :Parameters: **i** (??) - the index of the letter.

               :Returns: A std::string by value.
               )pbdoc")
        .def("set_identity",
             py::overload_cast<letter_type>(
                 &libsemigroups::FpSemigroup::set_identity),
             py::arg("id"),
             R"pbdoc(
               Set a character in alphabet() to be the identity using its index.

               :Parameters: **id** (??) - the index of the character to be the identity.

               :Returns: (None)
               )pbdoc")
        .def("set_identity",
             py::overload_cast<std::string const &>(
                 &libsemigroups::FpSemigroup::set_identity),
             py::arg("id"),
             R"pbdoc(
               Set a character in alphabet() to be the identity.

               :Parameters: **id** (??) - a string containing the character to be the identity.

               :Returns: (None)
               )pbdoc")
        .def("identity",
             &libsemigroups::FpSemigroup::identity,
             R"pbdoc(
               Returns the identity of this, or throws an exception if there isn't one.

               :return: A const reference to the identity, a value of type std::string.
               )pbdoc")
        .def("set_inverses",
             &libsemigroups::FpSemigroup::set_inverses,
             py::arg("a"),
             R"pbdoc(
               Set the inverses of letters in alphabet().

               :param a: a string of length alphabet().size().
               :type a: ??

               :return: (None)
               )pbdoc")
        .def("inverses",
             &libsemigroups::FpSemigroup::inverses,
             R"pbdoc(
               Returns the inverses of this, or throws an exception if there aren't any.

               :return: A const reference to the inverses, a value of type std::string.
               )pbdoc")
        .def("add_rule",
             py::overload_cast<relation_type>(
                 &libsemigroups::FpSemigroup::add_rule),
             py::arg("rel"),
             R"pbdoc(
               Add a rule.

               :Parameters: **rel** (??) - the rule being added.

               :Returns: (None)
               )pbdoc")
        .def(
            "add_rule",
            py::overload_cast<rule_type>(&libsemigroups::FpSemigroup::add_rule),
            py::arg("rel"),
            R"pbdoc(
               Add a rule.

               :Parameters: **rel** (??) - the rule being added.

               :Returns: (None)
               )pbdoc")
        .def("add_rule",
             py::overload_cast<std::initializer_list<size_t>,
                               std::initializer_list<size_t>>(
                 &libsemigroups::FpSemigroup::add_rule),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               Add a rule.

               :Parameters: - **u** (??) - the left-hand side of the rule being added.
                            - **v** (??) - the right-hand side of the rule being added.

               :Returns: (None)
               )pbdoc")
        .def("add_rule",
             py::overload_cast<std::string const &, std::string const &>(
                 &libsemigroups::FpSemigroup::add_rule),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               Add a rule.

               :Parameters: - **u** (??) - the left-hand side of the rule being added.
                            - **v** (??) - the right-hand side of the rule being added.

               :Returns: (None)
               )pbdoc")
        .def("add_rule",
             py::overload_cast<word_type const &, word_type const &>(
                 &libsemigroups::FpSemigroup::add_rule),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               Add a rule.

               :Parameters: - **u** (??) - the left-hand side of the rule being added.
                            - **v** (??) - the right-hand side of the rule being added.

               :Returns: (None)
               )pbdoc")
        .def("add_rules",
             py::overload_cast<FroidurePinBase &>(
                 &libsemigroups::FpSemigroup::add_rules),
             py::arg("S"),
             R"pbdoc(
               Add the rules of a finite presentation for S to this.

               :Parameters: **S** (??) - a FroidurePin object representing a semigroup.

               :Returns: (None)
               )pbdoc")
        .def("add_rules",
             py::overload_cast<std::vector<rule_type> const &>(
                 &libsemigroups::FpSemigroup::add_rules),
             py::arg("rels"),
             R"pbdoc(
               Add the rules in a vector to this.

               :Parameters: **rels** (??) - a vector of FpSemigroupInterface::rule_type.

               :Returns: (None)
               )pbdoc")
        .def("nr_rules",
             &libsemigroups::FpSemigroup::nr_rules,
             R"pbdoc(
               Returns the number of rules of the finitely presented semigroup represented by this.

               :return: A value of type size_t.
               )pbdoc")
        .def("report_every",
             (void (libsemigroups::FpSemigroup::*)(std::chrono::nanoseconds))
                 & Runner::report_every,
             py::arg("t"),
             R"pbdoc(
               Set the minimum elapsed time between reports.

               :Parameters: **t** (??) - the amount of time (in nanoseconds) between reports.

               :Returns: (None)
               )pbdoc")
        .def("report",
             &libsemigroups::FpSemigroup::report,
             R"pbdoc(
               Check if it is time to report.

               :return: A bool.
               )pbdoc")
        .def("report_why_we_stopped",
             &libsemigroups::FpSemigroup::report_why_we_stopped,
             R"pbdoc(
               Report why Runner::run stopped.

               :return: (None)
               )pbdoc")
        .def("kill",
             &libsemigroups::FpSemigroup::kill,
             R"pbdoc(
               Stop Runner::run from running (thread-safe).

               :return: (None).
               )pbdoc")
        .def("run",
             &libsemigroups::FpSemigroup::run,
             R"pbdoc(
               Member function that wraps run_impl which is implemented in a derived class.

               :return: (None)
               )pbdoc")
        .def("run_for",
             (void (libsemigroups::FpSemigroup::*)(std::chrono::nanoseconds))
                 & Runner::run_for,
             py::arg("t"),
             R"pbdoc(
               Run for a specified amount of time.

               :Parameters: **t** (??) - the time in nanoseconds to run for.

               :Returns: (None)
               )pbdoc")
        .def("run_until",
             (void (libsemigroups::FpSemigroup::*)(std::function<bool()> &))
                 & Runner::run_until,
             py::arg("func"),
             R"pbdoc(
               Run until a nullary predicate returns true or Runner::finished.

               :Parameters: **func** (??) - a function pointer.

               :Returns: (None)
               )pbdoc")
        .def("dead",
             &libsemigroups::FpSemigroup::dead,
             R"pbdoc(
               Check if the runner is dead.

               :return: A bool.
               )pbdoc")
        .def("finished",
             &libsemigroups::FpSemigroup::finished,
             R"pbdoc(
               Check if Runner::run has been run to completion or not.

               :return: A bool.
               )pbdoc")
        .def("started",
             &libsemigroups::FpSemigroup::started,
             R"pbdoc(
               Check if Runner::run has already been called.

               :return: A bool.
               )pbdoc")
        .def("stopped",
             &libsemigroups::FpSemigroup::stopped,
             R"pbdoc(
               Check if the runner is stopped.

               :return: A bool.
               )pbdoc")
        .def("timed_out",
             &libsemigroups::FpSemigroup::timed_out,
             R"pbdoc(
               Check if the amount of time specified to Runner::run_for has elapsed.

               :return: A bool
               )pbdoc")
        .def("running",
             &libsemigroups::FpSemigroup::running,
             R"pbdoc(
               Check if a Runner instance is currently running.

               :return: true if Runner::run is in the process to run and false it is not.
               )pbdoc")
        .def("stopped_by_predicate",
             &libsemigroups::FpSemigroup::stopped_by_predicate,
             R"pbdoc(
               Check if the runner was, or should, stop because the nullary predicate passed as first argument to Runner::run_until.

               :return: A bool.
               )pbdoc")
        .def("normal_form",
             py::overload_cast<std::string const &>(
                 &libsemigroups::FpSemigroup::normal_form),
             py::arg("w"),
             R"pbdoc(
               Returns a normal form for a string.

               :Parameters: **w** (??) - the word whose normal form we want to find. The parameter w must be a std::string consisting of letters in alphabet().

               :Returns: the normal form of the parameter w, a value of type std::string.
               )pbdoc")
        .def("normal_form",
             py::overload_cast<word_type const &>(
                 &libsemigroups::FpSemigroup::normal_form),
             py::arg("w"),
             R"pbdoc(
               Returns a normal form for a libsemigroups::word_type.

               :Parameters: **w** (??) - the word whose normal form we want to find. The parameter w must be a libsemigroups::word_type consisting of indices of the generators of the finitely presented semigroup that this represents.

               :Returns: the normal form of the parameter w, a value of type libsemigroups::word_type.
               )pbdoc")
        .def("normal_form",
             py::overload_cast<std::initializer_list<letter_type>>(
                 &libsemigroups::FpSemigroup::normal_form),
             py::arg("w"),
             R"pbdoc(
               Returns a normal form for a libsemigroups::word_type.

               :Parameters: **w** (??) - the word whose normal form we want to find. The parameter w must be a libsemigroups::word_type consisting of indices of the generators of the finitely presented semigroup that this represents.

               :Returns: the normal form of the parameter w, a value of type libsemigroups::word_type.
               )pbdoc")
        .def("equal_to",
             py::overload_cast<std::initializer_list<letter_type>,
                               std::initializer_list<letter_type>>(
                 &libsemigroups::FpSemigroup::equal_to),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               Check if two words represent the same element.

               :Parameters: - **u** (??) - a libsemigroups::word_type consisting of indices of the generators of the finitely presented semigroup.
                            - **v** (??) - a libsemigroups::word_type consisting of indices of the generators of the finitely presented semigroup.

               :Returns: true if the words u and v represent the same element of the finitely presented semigroup, and false otherwise.
               )pbdoc")
        .def("equal_to",
             py::overload_cast<std::string const &, std::string const &>(
                 &libsemigroups::FpSemigroup::equal_to),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               Check if two words represent the same element.

               :Parameters: - **u** (??) - a string over the alphabet of the finitely presented semigroup.
                            - **v** (??) - a string over the alphabet of the finitely presented semigroup.

               :Returns: true if the strings u and v represent the same element of the finitely presented semigroup, and false otherwise.
               )pbdoc")
        .def("equal_to",
             py::overload_cast<word_type const &, word_type const &>(
                 &libsemigroups::FpSemigroup::equal_to),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               Check if two words represent the same element.

               :Parameters: - **u** (??) - a libsemigroups::word_type consisting of indices of the generators of the finitely presented semigroup.
                            - **v** (??) - a libsemigroups::word_type consisting of indices of the generators of the finitely presented semigroup.

               :Returns: true if the words u and v represent the same element of the finitely presented semigroup, and false otherwise.
               )pbdoc")
        .def("word_to_string",
             &libsemigroups::FpSemigroup::word_to_string,
             py::arg("w"),
             R"pbdoc(
               Convert a libsemigroups::word_type to a std::string representing the same element of the finitely presented semigroup represented by this.

               :param w: the libsemigroups::word_type to convert.
               :type w: ??

               :return: a std::string.
               )pbdoc")
        .def("string_to_word",
             &libsemigroups::FpSemigroup::string_to_word,
             py::arg("w"),
             R"pbdoc(
               Convert a string to a libsemigroups::word_type representing the same element of the finitely presented semigroup represented by this.

               :param w: the string to convert.
               :type w: ??

               :return: a libsemigroups::word_type.
               )pbdoc")
        .def("uint_to_char",
             &libsemigroups::FpSemigroup::uint_to_char,
             py::arg("a"),
             R"pbdoc(
               Convert a libsemigroups::letter_type to a char representing the same generator of the finitely presented semigroup represented by this.

               :param a: the libsemigroups::letter_type to convert.
               :type a: ??

               :return: A char.
               )pbdoc")
        .def("char_to_uint",
             &libsemigroups::FpSemigroup::char_to_uint,
             py::arg("a"),
             R"pbdoc(
               Convert a char to a libsemigroups::letter_type representing the same generator of the finitely presented semigroup represented by this.

               :param a: the string to convert.
               :type a: ??

               :return: a libsemigroups::letter_type.
               )pbdoc")
        .def("has_froidure_pin",
             &libsemigroups::FpSemigroup::has_froidure_pin,
             R"pbdoc(
               Returns true if a FroidurePin instance isomorphic to the finitely presented semigroup defined by this has already been computed, and false if not.

               :return: A bool.
               )pbdoc")
        .def("froidure_pin",
             &libsemigroups::FpSemigroup::froidure_pin,
             R"pbdoc(
               Returns a FroidurePin instance isomorphic to the finitely presented semigroup defined by this.

               :return: A shared pointer to a FroidurePinBase.
               )pbdoc")
        .def("has_knuth_bendix",
             &libsemigroups::FpSemigroup::has_knuth_bendix,
             R"pbdoc(
               Checks if a fpsemigroup::KnuthBendix instance is being used to compute the finitely presented semigroup represented by this.

               :return: A bool.
               )pbdoc")
        .def("knuth_bendix",
             &libsemigroups::FpSemigroup::knuth_bendix,
             R"pbdoc(
               Returns the fpsemigroup::KnuthBendix instance used to compute the finitely presented semigroup (if any).

               :return: A shared_ptr to a congruence::KnuthBendix or nullptr.
               )pbdoc")
        .def("has_todd_coxeter",
             &libsemigroups::FpSemigroup::has_todd_coxeter,
             R"pbdoc(
               Checks if a fpsemigroup::ToddCoxeter instance is being used to compute the finitely presented semigroup represented by this.

               :return: A bool.
               )pbdoc")
        .def("todd_coxeter",
             &libsemigroups::FpSemigroup::todd_coxeter,
             R"pbdoc(
               Returns the libsemigroups::fpsemigroup::ToddCoxeter instance used to compute the finitely presented semigroup (if any).

               :return: A shared_ptr to a congruence::KnuthBendix or nullptr.
               )pbdoc")
        .def("is_obviously_finite",
             &libsemigroups::FpSemigroup::is_obviously_finite,
             R"pbdoc(
               Return true if the finitely presented semigroup represented by this is obviously finite, and false if it is not obviously finite.

               :return: A bool.
               )pbdoc")
        .def("is_obviously_infinite",
             &libsemigroups::FpSemigroup::is_obviously_infinite,
             R"pbdoc(
               Return true if the finitely presented semigroup represented by this is obviously infinite, and false if it is not obviously infinite.

               :return: A bool.
               )pbdoc")
        .def("size",
             &libsemigroups::FpSemigroup::size,
             R"pbdoc(
               Returns the size of the finitely presented semigroup.

               :return: A size_t, the value of which equals the size of this if this number is finite, or libsemigroups::POSITIVE_INFINITY in some cases if this number is not finite.
               )pbdoc")
        .def("cbegin_rules",
             &libsemigroups::FpSemigroup::cbegin_rules,
             R"pbdoc(
               Returns an iterator pointing to the first rule of this.

               :return: A FpSemigroupInterface::const_iterator.
               )pbdoc")
        .def("cend_rules",
             &libsemigroups::FpSemigroup::cend_rules,
             R"pbdoc(
               Returns an iterator pointing one past the last rule of this.

               :return: A FpSemigroupInterface::const_iterator.
               )pbdoc")
        .def("to_gap_string",
             &libsemigroups::FpSemigroup::to_gap_string,
             R"pbdoc(
               Returns a string containing GAP commands for defining a finitely presented semigroup equal to that represented by this.

               :return: A std::string.
               )pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // siso.hpp
    ////////////////////////////////////////////////////////////////////////

    m.def(
        "silo",
        [](std::string const &alphabet,
           size_t             upper_bound,
           std::string const &first,
           std::string const &last) {
          return py::make_iterator(
              cbegin_silo(alphabet, upper_bound, first, last),
              cend_silo(alphabet, upper_bound, first, last));
        },
        py::arg("alphabet"),
        py::arg("upper_bound"),
        py::arg("first"),
        py::arg("last"),
        R"pbdoc(
      Returns an iterator to strings in lexicographic order (silo).

      :param alphabet: the alphabet
      :type alphabet: str
      :param upper_bound: the maximum length of string to return
      :type upper_bound: int
      :param first: the first string
      :type first: str
      :param last: one past the last string
      :type last: str

      :return: An iterator to strings in lexicographic order over the alphabet ``alphabet`` of length at most ``upper_bound`` in the range ``[first, last)``.

      Example
      -------
      .. code-block:: python

         [x for x in silo("ba", 3, "b", "aaa")]
         # ["b", "bb", "ba", "a", "ab", "aa"]

      )pbdoc");

    m.def(
        "sislo",
        [](std::string const &alphabet,
           std::string const &first,
           std::string const &last) {
          return py::make_iterator(cbegin_sislo(alphabet, first, last),
                                   cend_sislo(alphabet, first, last));
        },
        py::arg("alphabet"),
        py::arg("first"),
        py::arg("last"),
        R"pbdoc(
      Returns an iterator to strings in short-lex order (sislo).

      :param alphabet: the alphabet
      :type alphabet: str
      :param first: the first string
      :type first: str
      :param last: one past the last string
      :type last: str

      :return: An iterator to strings over the alphabet ``alphabet`` in short-lex order and in the range ``[first, last)``.

      Example
      -------
      .. code-block:: python

         [x for x in sislo("ba", "b", "aaa")]
         # ['b', 'a', 'bb', 'ba', 'ab', 'aa', 'bbb', 'bba', 'bab', 'baa', 'abb', 'aba', 'aab']
      )pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // todd-coxeter.hpp
    ////////////////////////////////////////////////////////////////////////

    using sort_function_type
        = std::function<bool(word_type const &, word_type const &)>;

    py::class_<congruence::ToddCoxeter> tc(m, "ToddCoxeter");

    tc.def(py::init<congruence_type>())
        .def(py::init<congruence_type, congruence::ToddCoxeter &>())
        .def(py::init<congruence_type, fpsemigroup::KnuthBendix &>())
        .def(py::init<congruence::ToddCoxeter const &>())
        .def("set_nr_generators",
             &congruence::ToddCoxeter::set_nr_generators,
             py::arg("n"),
             R"pbdoc(
           Set the number of generators of the congruence.

           :param n: the number of generators.
           :type n: int

           :return: (None)
           )pbdoc")
        .def("nr_generators",
             &congruence::ToddCoxeter::nr_generators,
             R"pbdoc(
           Returns the number of generators.

           :return: The number of generators of the semigroup of the congruence that an object of this type represents, or UNDEFINED.
           )pbdoc")
        .def("add_pair",
             py::overload_cast<word_type const &, word_type const &>(
                 &congruence::ToddCoxeter::add_pair),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
           Add a generating pair to the congruence.

           :param u: a word (list of integers) over the generators of the semigroup.
           :type u: list
           :param v: a word (list of integers) over the generators of the semigroup.
           :type v: list

           :return: (None)
           )pbdoc")
        .def("nr_generating_pairs",
             &congruence::ToddCoxeter::nr_generating_pairs,
             R"pbdoc(
           Returns the number of generating pairs added by
           CongruenceInterface::add_pair.

           :return: The number of generating pairs of the congruence that an object of this type represents.
           )pbdoc")
        .def("froidure_pin_policy",
             py::overload_cast<congruence::ToddCoxeter::policy::froidure_pin>(
                 &congruence::ToddCoxeter::froidure_pin_policy),
             R"pbdoc(
           Sets the value of the Froidure-Pin policy specified by the argument
           ToddCoxeter.policy.froidure_pin.
           )pbdoc")
        .def("lookahead",
             &congruence::ToddCoxeter::lookahead,
             R"pbdoc(
           Sets the type of lookahead to be used when using the HLT strategy.
           )pbdoc")
        .def("lower_bound",
             &congruence::ToddCoxeter::lower_bound,
             R"pbdoc(
           Sets a lower bound for the number of classes of the congruence
           represented by a ToddCoxeter instance.
           )pbdoc")
        .def("next_lookahead",
             &congruence::ToddCoxeter::next_lookahead,
             R"pbdoc(
           If the number of cosets active exceeds the value set by this function,
           then a lookahead, of the type set by lookahead, is triggered.
           )pbdoc")
        .def("save",
             &congruence::ToddCoxeter::save,
             R"pbdoc(
           If the argument of this function is true and the HLT strategy is being
           used, then deductions are processed during the enumeration.
           )pbdoc")
        .def("standardize",
             py::overload_cast<bool>(&congruence::ToddCoxeter::standardize),
             R"pbdoc(
           If the argument of this function is true, then the coset table is
           standardized (according to the short-lex order) during the coset
           enumeration.
           )pbdoc")
        .def("strategy",
             (congruence::ToddCoxeter::policy::strategy(
                 congruence::ToddCoxeter::*)() const)
                 & congruence::ToddCoxeter::strategy,
             R"pbdoc(
           Returns the value of the strategy used during the coset enumeration.
           )pbdoc")
        .def("strategy",
             py::overload_cast<congruence::ToddCoxeter::policy::strategy>(
                 &congruence::ToddCoxeter::strategy),
             R"pbdoc(
           Set the strategy used during the coset enumeration can be specified
           using this function.
           )pbdoc")
        .def("random_interval",
             (congruence::ToddCoxeter
              & (congruence::ToddCoxeter::*) (std::chrono::nanoseconds))
                 & congruence::ToddCoxeter::random_interval,
             R"pbdoc(
           Sets the duration in nanoseconds that a given randomly selected
           strategy will run for, when using the random strategy
           (policy::strategy::random).
           )pbdoc")
        .def("sort_generating_pairs",
             py::overload_cast<sort_function_type>(
                 &congruence::ToddCoxeter::sort_generating_pairs),
             py::arg("func"),
             R"pbdoc(
           Sorts all existing generating pairs according to the binary function
           func.

           :param func: a value of type binary predicate that defines a linear order on the relations in a ToddCoxeter instance.
           :type func: a function.

           :return: ``self``.
           )pbdoc")
        .def("random_shuffle_generating_pairs",
             &congruence::ToddCoxeter::random_shuffle_generating_pairs,
             R"pbdoc(
           Randomly shuffle all existing generating pairs.

           :return: ``self``.
           )pbdoc")
        .def("report_every",
             (void (congruence::ToddCoxeter::*)(std::chrono::nanoseconds))
                 & Runner::report_every,
             py::arg("t"),
             R"pbdoc(
           Set the minimum elapsed time between reports.

           :param t: the amount of time (in nanoseconds) between reports.
           :type t: int

           :return: (None)
           )pbdoc")
        .def("report",
             &congruence::ToddCoxeter::report,
             R"pbdoc(
           Check if it is time to report.

           :return: A bool.
           )pbdoc")
        .def("report_why_we_stopped",
             &congruence::ToddCoxeter::report_why_we_stopped,
             R"pbdoc(
           Report why Runner::run stopped.

           :return: (None)
           )pbdoc")
        .def("kill",
             &congruence::ToddCoxeter::kill,
             R"pbdoc(
           Stop Runner::run from running (thread-safe).

           :return: (None).
           )pbdoc")
        .def("run",
             &congruence::ToddCoxeter::run,
             R"pbdoc(
           Run the algorithm until it finishes.

           :return: (None)
           )pbdoc")
        .def("run_for",
             (void (congruence::ToddCoxeter::*)(std::chrono::nanoseconds))
                 & Runner::run_for,
             py::arg("t"),
             R"pbdoc(
           Run for a specified amount of time.

           :param t: the time to run for.
           :type t: datetime.timedelta

           :return: (None)

           Example
           -------

           .. code-block:: python

              from datetime import timedelta
              from libsemigroups_pybind11 import ToddCoxeter, congruence_type

              tc = ToddCoxeter(congruence_type.twosided)
              tc.set_nr_generators(1)
              tc.add_pair([0] * 1000, [0] * 999)
              tc.run_for(timedelta(microseconds=10))
           )pbdoc")
        .def("run_until",
             (void (congruence::ToddCoxeter::*)(std::function<bool()> &))
                 & Runner::run_until,
             py::arg("func"),
             R"pbdoc(
           Run until a nullary predicate returns true or the algorithm
           finishes.

           :param func: a function.
           :type func: func

           :return: (None)
           )pbdoc")
        .def("less",
             &congruence::ToddCoxeter::less,
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
           This function returns true if the congruence class of v is less than
           the class of v in a total ordering of congruence classes.

           :param u: a word (list of integers) over the generators of the semigroup.
           :type u: list
           :param v: a word (list of integers) over the generators of the semigroup.
           :type v: list

           :return: true if the class of u is less than that of .
           )pbdoc")
        .def("const_contains",
             &congruence::ToddCoxeter::const_contains,
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
           Check if a pair of words is known to belong to the congruence.

           :param u: a word (list of integers) over the generators of the semigroup.
           :type u: list
           :param v: a word (list of integers) over the generators of the semigroup.
           :type v: list

           :return: ``tril.True`` if the words ``u`` and ``v`` are known to belong to the same congruence class, ``tril.False`` if the words are known to not belong to the same congruence class, ``tril::unknown`` otherwise.
           )pbdoc")
        .def("contains",
             &congruence::ToddCoxeter::contains,
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
           Check if a pair of words belongs to the congruence.

           :param u: a word (list of integers) over the generators of the semigroup.
           :type u: list
           :param v: a word (list of integers) over the generators of the semigroup.
           :type v: list

           :return: true if the words u and v belong to the same congruence class, and false otherwise.
           )pbdoc")
        .def("empty",
             &congruence::ToddCoxeter::empty,
             R"pbdoc(
           Returns true if there are no relations or generating pairs in the
           ToddCoxeter instance, and the number of active cosets is 1 (the
           minimum possible).

     )pbdoc")
        .def("nr_classes",
             &congruence::ToddCoxeter::nr_classes,
             R"pbdoc(
           Computes the total number of classes in the congruence represented
           by an instance of this type.

           :return: The number of congruences classes of this if this number is finite, or POSITIVE_INFINITY in some cases if this number is not finite.
           )pbdoc")
        .def("nr_non_trivial_classes",
             &congruence::ToddCoxeter::nr_non_trivial_classes,
             R"pbdoc(
           Returns the number of non-trivial classes (size > 1) of the congruence.

           :return: The number of non-trivial classes of the congruence.
           )pbdoc")
        .def("reserve",
             &congruence::ToddCoxeter::reserve,
             R"pbdoc(
           Reserves the capacity specified by the argument in the data
           structures for cosets used in a ToddCoxeter instance.
           )pbdoc")
        .def("shrink_to_fit",
             &congruence::ToddCoxeter::shrink_to_fit,
             R"pbdoc(
           Release all memory used to store free cosets, and any other unnecessary
           data if the enumeration is finished.
           )pbdoc")
        .def("quotient_froidure_pin",
             &congruence::ToddCoxeter::quotient_froidure_pin,
             R"pbdoc(
           Returns a semigroup represented as an instance of a derived class of
           FroidurePinBase that is isomorphic to the quotient of the parent
           semigroup of this by the 2-sided congruence that this represents.

           :return: A std::shared_ptr to FroidurePinBase.
           )pbdoc")
        .def("has_quotient_froidure_pin",
             &congruence::ToddCoxeter::has_quotient_froidure_pin,
             R"pbdoc(
           Returns true if the congruence represented by this object knows an
           isomorphic quotient semigroup represented by an instance of
           FroidurePin.

           :return: A bool.
           )pbdoc")
        .def("parent_froidure_pin",
             &congruence::ToddCoxeter::parent_froidure_pin,
             R"pbdoc(
           Returns a shared_ptr to the parent FroidurePin over which the
           congruence represented by this object was defined, if it exists.

           :return: A std::shared_ptr to FroidurePinBase.
           )pbdoc")
        .def("has_parent_froidure_pin",
             &congruence::ToddCoxeter::has_parent_froidure_pin,
             R"pbdoc(
           Returns true if the congruence represented by this was created from
           a FroidurePin instance.

           :return: A bool.
           )pbdoc")
        .def("is_quotient_obviously_finite",
             &congruence::ToddCoxeter::is_quotient_obviously_finite,
             R"pbdoc(
          Return true if the number of classes in the congruence represented by
          this is obviously finite, and false if it is not obviously finite.

          :return: A bool.
          )pbdoc")
        .def("is_quotient_obviously_infinite",
             &congruence::ToddCoxeter::is_quotient_obviously_infinite,
             R"pbdoc(
           Return ``True`` if the number of classes in the congruence
           represented by this is obviously infinite, and ``False`` if it is
           not obviously infinite.

           :return: A bool.
           )pbdoc")
        .def("word_to_class_index",
             &congruence::ToddCoxeter::word_to_class_index,
             py::arg("w"),
             R"pbdoc(
           If the congruence, that an object of this type represents, is
           defined over a semigroup with generators $A$, then this function
           defines a surjective function from the set of all words over $A$ to
           either $\{0, 1, \ldots, n - 1\}$, where $n$ is the number of
           classes, or to the non-negative integers $\{0, 1, \ldots\}$ if this
           has infinitely many classes.

           :param w: the word (list of integers) whose class index we want to find.
           :type w: list

           :return: The index of the congruence class corresponding to word.
           )pbdoc")
        .def("class_index_to_word",
             &congruence::ToddCoxeter::class_index_to_word,
             py::arg("i"),
             R"pbdoc(
           If the congruence, that an object of this type represents, is
           defined over a semigroup with generators :math:`A`, then this
           function defines a injective function from :math:`\{0, 1, \ldots, n
           - 1\}`, where $n$ is the number of classes, or to the non-negative
           integers :math:`\{0, 1, \ldots\}` if this has infinitely many
           classes, to a fixed set of words over :math:`A` representing
           distinct congruences classes.

           :param i: the index of the class whose representative we want to find.
           :type i: int

           :return: A word representing the i-th class of the congruence.
           )pbdoc")
        .def("kind",
             &congruence::ToddCoxeter::kind,
             R"pbdoc(
           Return if the congruence represented by this object was created as a
           left, right, or two-sided congruence.

           :return: A ``congruence_type``.
           )pbdoc")
        .def("complete",
             &congruence::ToddCoxeter::complete,
             R"pbdoc(
           Returns ``True`` if the coset table is complete, and ``False`` if it
           is not.
           )pbdoc")
        .def("compatible",
             &congruence::ToddCoxeter::compatible,
             R"pbdoc(
           Returns true if the coset table is compatible with the relations and
           generating pairs used to create this, and false if it is not.

           )pbdoc")
        .def("dead",
             &congruence::ToddCoxeter::dead,
             R"pbdoc(
           Check if the runner is dead.

           :return: A bool.
           )pbdoc")
        .def("finished",
             &congruence::ToddCoxeter::finished,
             R"pbdoc(
           Check if Runner::run has been run to completion or not.

           :return: A bool.
           )pbdoc")
        .def("started",
             &congruence::ToddCoxeter::started,
             R"pbdoc(
           Check if Runner::run has already been called.

           :return: A bool.
           )pbdoc")
        .def("stopped",
             &congruence::ToddCoxeter::stopped,
             R"pbdoc(
           Check if the runner is stopped.

           :return: A bool.
           )pbdoc")
        .def("timed_out",
             &congruence::ToddCoxeter::timed_out,
             R"pbdoc(
           Check if the amount of time specified to Runner::run_for has elapsed.

           :return: A bool
           )pbdoc")
        .def("stopped_by_predicate",
             &congruence::ToddCoxeter::stopped_by_predicate,
             R"pbdoc(
           Check if the runner was, or should, stop because the nullary
           predicate passed as first argument to Runner::run_until.

           :return: A bool.
           )pbdoc")
        .def("running",
             &congruence::ToddCoxeter::running,
             R"pbdoc(
           Check if a Runner instance is currently running.

           :return: true if Runner::run is in the process to run and false it is not.
           )pbdoc")
        .def("is_standardized",
             &congruence::ToddCoxeter::is_standardized,
             R"pbdoc(
           Returns true if the ToddCoxeter instance is standardized.
           )pbdoc")
        .def("standardize",
             py::overload_cast<congruence::ToddCoxeter::order>(
                 &congruence::ToddCoxeter::standardize),
             R"pbdoc(
           If the argument of this function is true, then the coset table is
           standardized (according to the short-lex order) during the coset
           enumeration.
           )pbdoc")
        .def(
            "generating_pairs",
            [](congruence::ToddCoxeter const &tc) {
              return py::make_iterator(tc.cbegin_generating_pairs(),
                                       tc.cend_generating_pairs());
            },
            R"pbdoc(
          Returns an iterator to the generating pairs of the congruence.
          )pbdoc")
        .def(
            "non_trivial_classes",
            [](congruence::ToddCoxeter &tc) {
              return py::make_iterator(tc.cbegin_ntc(), tc.cend_ntc());
            },
            R"pbdoc(
          Returns an iterator pointing to the first non-trivial (size > 1)
          class of the congruence.
          )pbdoc")
        .def(
            "normal_forms",
            [](congruence::ToddCoxeter &tc) {
              return py::make_iterator(tc.cbegin_normal_forms(),
                                       tc.cend_normal_forms());
            },
            R"pbdoc(
          Returns an iterator to the normal forms of the congruence represented
          by an instance of ToddCoxeter.
          )pbdoc");

    py::enum_<congruence::ToddCoxeter::order>(tc, "order")
        .value("none", congruence::ToddCoxeter::order::none)
        .value("shortlex", congruence::ToddCoxeter::order::shortlex)
        .value("lex", congruence::ToddCoxeter::order::lex)
        .value("recursive", congruence::ToddCoxeter::order::recursive);

    py::enum_<congruence::ToddCoxeter::policy::strategy>(tc, "strategy_options")
        .value("hlt", congruence::ToddCoxeter::policy::strategy::hlt)
        .value("felsch", congruence::ToddCoxeter::policy::strategy::felsch)
        .value("random", congruence::ToddCoxeter::policy::strategy::random);

    py::enum_<congruence::ToddCoxeter::policy::lookahead>(tc,
                                                          "lookahead_options")
        .value("full", congruence::ToddCoxeter::policy::lookahead::full)
        .value("partial", congruence::ToddCoxeter::policy::lookahead::partial);

    ////////////////////////////////////////////////////////////////////////
    // wilo.hpp
    ////////////////////////////////////////////////////////////////////////

    m.def(
        "wilo",
        [](size_t const     n,
           size_t const     upper_bound,
           word_type const &first,
           word_type const &last) {
          return py::make_iterator(cbegin_wilo(n, upper_bound, first, last),
                                   cend_wilo(n, upper_bound, first, last));
        },
        py::arg("n"),
        py::arg("upper_bound"),
        py::arg("first"),
        py::arg("last"),
        R"pbdoc(
      Returns an iterator to words in lexicographic order (wilo).

      :param n: the number of letters
      :type n: int
      :param upper_bound: the maximum length of string to return
      :type upper_bound: int
      :param first: the first word
      :type first: list
      :param last: one past the last word
      :type last: list

      :return: An iterator to words in lexicographic order over an alphabet with  ``n`` letters of length at most ``upper_bound`` in the range ``[first, last)``.

      Example
      -------
      .. code-block:: python

         [x for x in wilo(2, 3, [0], [1, 1, 1])]
         # [[0], [0, 0], [0, 1], [1], [1, 0], [1, 1]]
      )pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // wislo.hpp
    ////////////////////////////////////////////////////////////////////////

    m.def(
        "wislo",
        [](size_t const n, word_type const &first, word_type const &last) {
          return py::make_iterator(cbegin_wislo(n, first, last),
                                   cend_wislo(n, first, last));
        },
        py::arg("n"),
        py::arg("first"),
        py::arg("last"),
        R"pbdoc(
      Returns an iterator to strings in short-lex order (sislo).

      :param n: the number of letter
      :type n: int
      :param first: the first word
      :type first: list
      :param last: one past the last word
      :type last: list

      :return: An iterator to words over an alphabet with ``n`` in short-lex order and in the range ``[first, last)``.

      Example
      -------
      .. code-block:: python

         [x for x in wislo(2, [0], [1, 1])]
         # [[0], [1], [0, 0], [0, 1], [1, 0]]
      )pbdoc");

    m.def("number_of_words",
          &number_of_words,
          py::arg("n"),
          py::arg("min"),
          py::arg("max"),
          R"pbdoc(
      Returns the number of words over a given alphabet in some range.

      :param n: the number of letter
      :type n: int
      :param min: the minimum length of a word
      :type min: int
      :param max: one greater than the maximum length of a word
      :type max: int

      :return: The number words over an alphabet with ``n`` with length in the range ``[min, max)``.

      Example
      -------
      .. code-block:: python

         number_of_words(2, 0, 10)
         # 1023
      )pbdoc");

    detail::bind_froidure_pin<Transf256>(m, "Transf256");
    detail::bind_froidure_pin<BMat8>(m, "BMat8");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
  }
}  // namespace libsemigroups
