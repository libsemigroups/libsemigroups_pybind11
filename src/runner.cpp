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
#include <libsemigroups/runner.hpp>

// pybind11....
#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_reporter, init_runner

namespace libsemigroups {
  namespace py = pybind11;

  void init_reporter(py::module& m) {
    m.def("delta",
          &delta,
          py::arg("t"),
          R"pbdoc(
:sig=(t: datetime.datetime) -> datetime.timedelta:
The time between the given point and now.

:param t:
   the time point.

:type t:
   datetime.datetime

:returns:
   The difference between the time point *t* and now.

:rtype:
   datetime.timedelta
)pbdoc");

    py::class_<Reporter> thing(m,
                               "Reporter",
                               R"pbdoc(
Collection of values related to reporting.

This class exists to collect some values related to reporting in its
derived [#sortof]_ classes. These values are:

-  :any:`report_prefix()`;
-  :any:`report_every()`;
-  :any:`last_report()`;
-  :any:`start_time()`.

This class has limited (no?) utility on its own, and you are unlikely to want to use it directly.

.. important::

   The classes that we claim derive from :any:`Reporter` in this documentation
   have all the methods of the :any:`Reporter` class but, for boring technical
   reasons, are not formally subclasses of :any:`Reporter`. If ``thing`` is an
   instance of a class derived from :any:`Reporter`, then you can use ``thing``
   as if it were an instance of :any:`Reporter` but
   ``isinstance(thing, Reporter)`` will return ``False``.

.. rubric:: Footnotes

.. [#sortof] Sort of, please see the note above.
)pbdoc");

    thing.def(py::init<>(), R"pbdoc(
Default construct a :any:`Reporter` object such that the following hold:

-  :any:`report_prefix()` is empty;
-  :any:`report_every()` is set to 1 second;
-  :any:`last_report()` is now;
-  :any:`start_time()` is now.
)pbdoc");
    thing.def(
        "copy",
        [](Reporter const& self) { return Reporter(self); },
        R"pbdoc(
:sig=(self: Reporter) -> Reporter:

Copy a :any:`Reporter` object.

:returns: A copy.
:rtype: Reporter
)pbdoc");
    thing.def("__copy__", [](Reporter const& self) { return Reporter(self); });
    thing.def(
        "init",
        [](Reporter& r) -> Reporter& { return r.init(); },
        R"pbdoc(
Initialize an existing Reporter object.

This function puts a :any:`Reporter` object back into the same state as
if it had been newly default constructed.

:returns: *self*.
:rtype: Reporter

.. seealso::  :any:`Reporter()`
)pbdoc",
        py::return_value_policy::reference_internal);
    thing.def("report",
              &Reporter::report,
              R"pbdoc(
Check if it is time to report.

This function can be used to check if enough time has passed that we should
report again. That is if the time between :any:`last_report()` and now is
greater than the value of :any:`report_every()`. If ``True`` is returned, then
:any:`last_report()` is set to now.

:returns: A value of type ``bool``.
:rtype: bool

.. seealso::  :any:`report_every`.
)pbdoc");
    thing.def(
        "report_every",
        [](Reporter& self, Reporter::nanoseconds val) {
          return self.report_every(val);
        },
        py::arg("val"),
        R"pbdoc(
Set the minimum elapsed time between reports.

This function can be used to specify at run time the minimum elapsed time
between two calls to :any:`report()` that will return ``True``. If
:any:`report()` returns ``True`` at time ``s``, then :any:`report()` will only
return ``True`` again after at least time ``s + val`` has elapsed.

:param val: the amount of time between reports.
:type val: datetime.timedelta

:returns: *self*.
:rtype: Reporter

.. seealso:: :any:`report_every`
)pbdoc");
    thing.def(
        "report_every",
        [](Reporter const& self) { return self.report_every(); },
        R"pbdoc(
Get the minimum elapsed time between reports.

:returns:
   The time delta between reports.

:rtype:
   datetime.timedelta
)pbdoc");
    thing.def("start_time",
              &Reporter::start_time,
              R"pbdoc(
Get the start time.

This is the time point at which :any:`reset_start_time()` was last called,
which is also the time of construction of a :any:`Reporter` instance if
:any:`reset_start_time()` is not explicitly called.

:returns:
   The time delta representing the start time.

:rtype:
   datetime.datetime
)pbdoc");
    thing.def("reset_start_time",
              &Reporter::reset_start_time,
              R"pbdoc(
Reset the start time (and last report) to now.

:returns: *self*.
:rtype: Reporter
)pbdoc");
    thing.def("last_report",
              &Reporter::last_report,
              R"pbdoc(
Get the time point of the last report. This function returns the time point of the
last report, as set by one of:

-  :any:`reset_start_time()`;
-  :any:`report_every()`; or
-  :any:`report()`.

:returns:
   A :any:`datetime.datetime`.

:rtype:
   datetime.datetime
)pbdoc");
    thing.def("reset_last_report",
              &Reporter::reset_last_report,
              R"pbdoc(
Set the last report time point to now.

:returns: *self*.

:rtype:
   Reporter
)pbdoc");
    thing.def(
        "report_prefix",
        [](Reporter& self, std::string const& val) -> Reporter& {
          return self.report_prefix(val);
        },
        py::arg("val"),
        R"pbdoc(
Set the prefix string for reporting.

This function sets the return value of :any:`report_prefix()` to (a copy of)
the argument *val*. Typically this prefix should be the name of the algorithm
being run at the outmost level.

:param val: the new value of the report prefix.
:type val: str

:returns: *self*.
:rtype: Reporter
)pbdoc");
    thing.def(
        "report_prefix",
        [](Reporter const& self) { return self.report_prefix(); },
        R"pbdoc(
Get the current prefix string for reporting.

This function gets the current value of the prefix string for reporting (set
via :any:`report_prefix`), which is typically the name of the algorithm being
run at the outmost level.

:returns:
   The prefix string.

:rtype:
   str
)pbdoc");
  }  // init_reporter

  void init_runner(py::module& m) {
    py::class_<Runner, Reporter> thing(m,
                                       "Runner",
                                       R"pbdoc(
Abstract class for derived [#sortof]_ classes that run an algorithm.

Many of the classes in ``libsemigroups_pybind11`` implementing the algorithms,
that are the reason for the existence of this package, are derived from
:any:`Runner`. The :any:`Runner` class exists to collect various common tasks
required by such a derived class with a possibly long running :any:`run`. These
common tasks include:

*  running for a given amount of time (:any:`run_for`)
*  running until a nullary predicate is true (:any:`run_until`)
*  checking the status of the algorithm: has it :any:`started` ?
   :any:`finished`? been killed by another thread (:any:`dead`)? has it timed
   out (:any:`timed_out`)? has it :any:`stopped` for any reason?
*  permit the function :any:`run` to be killed from another thread (:any:`kill`).

Because :any:`Runner` is an abstract class it is not possible to create instances of :any:`Runner` except via a derived class.

This class inherits from :any:`Reporter`.

.. important::

   The classes that we claim derive from :any:`Runner` in this documentation
   have all the methods of the :any:`Runner` class but, for boring technical
   reasons, are not formally subclasses of :any:`Runner`. If ``thing`` is an
   instance of a class derived from :any:`Runner`, then you can use ``thing``
   as if it were an instance of :any:`Runner` but ``isinstance(thing, Runner)``
   will return ``False``.

.. rubric:: Footnotes

.. [#sortof] Sort of, please see the note above.
)pbdoc");

    py::options options;
    options.disable_enum_members_docstring();
    py::enum_<Runner::state> state(thing, "state", R"pbdoc(
The values in this enum are used to indicate the state of a runner.

The valid values are:

.. py:attribute:: state.never_run
   :value: <state.never_run: 0>
   
   Indicates that none of :any:`Runner.run`, :any:`Runner.run_for`, or
   :any:`Runner.run_until` has been called since construction or the last call
   to :any:`Runner.init`.

.. py:attribute:: state.running_to_finish
   :value: <state.running_to_finish: 1>

   Indicates that the Runner is currently running to the finish (via
   :any:`Runner.run`).

.. py:attribute:: state.running_for
   :value: <state.running_for: 2>

   Indicates that the Runner is currently running for a specific amount of time
   (via :any:`Runner.run_for`).

.. py:attribute:: state.running_until
   :value: <state.running_until: 3>

   Indicates that the Runner is currently running until some condition is met
   (via :any:`Runner.run_until`).

.. py:attribute:: state.timed_out
   :value: <state.timed_out: 4>

   Indicates that the Runner was run via :any:`Runner.run_for` for a specific
   amount of time and that time has elapsed.

.. py:attribute:: state.stopped_by_predicate
   :value: <state.stopped_by_predicate: 6>

   Indicates that the Runner was run via :any:`Runner.run_until` until the
   condition specified by the argument to :any:`Runner.run_until` was met.

.. py:attribute:: state.not_running
   :value: <state.not_running: 7>

   Indicates that the Runner is not in any of the previous states and is not
   currently running. This can occur when, for example, :any:`Runner.run`
   throws an exception.

.. py:attribute:: state.dead
   :value: <state.dead: 8>
   
   Indicates that the Runner was killed (by another thread).
)pbdoc");
    state.value("never_run", Runner::state::never_run)
        .value("running_to_finish", Runner::state::running_to_finish)
        .value("running_for", Runner::state::running_for)
        .value("running_until", Runner::state::running_until)
        .value("timed_out", Runner::state::timed_out)
        .value("stopped_by_predicate", Runner::state::stopped_by_predicate)
        .value("not_running", Runner::state::not_running)
        .value("dead", Runner::state::dead);

    thing.def(
        "init",
        [](Runner& r) -> Runner& { return r.init(); },
        R"pbdoc(
Initialize an existing Runner object.

This function puts a :any:`Runner` object back into the same state as if it had
been newly default constructed.

:returns: *self*.
:rtype: Runner

.. seealso::  :any:`Runner()`
)pbdoc",
        py::return_value_policy::reference_internal);
    thing.def("run",
              &Runner::run,
              R"pbdoc(
Run until finished. Run the main algorithm implemented by a derived
class of :any:`Runner`.
)pbdoc");
    thing.def(
        "run_for",
        [](Runner& self, std::chrono::nanoseconds t) {
          return self.run_for(t);
        },
        py::arg("t"),
        R"pbdoc(
Run for a specified amount of time.

For this to work it is necessary to periodically check if :any:`timed_out()`
returns ``True``, and to stop if it is, in the :any:`run()` member function of
any derived class of :any:`Runner`.

:param t: the time to run for.
:type t: datetime.timedelta

.. seealso::  :any:`run_for`)pbdoc");
    thing.def("run_until",
              (void(Runner::*)(std::function<bool()>&)) & Runner::run_until,
              py::arg("func"),
              R"pbdoc(
Run until a nullary predicate returns true or finished.

:param func:
   a nullary function that will be used to determine when to stop running.

:type func:
   collections.abc.Callable[[], bool]
)pbdoc");
    thing.def("timed_out",
              &Runner::timed_out,
              R"pbdoc(
Check if the amount of time passed to run_for has elapsed.

:returns: Whether or not :any:`run_for` timed out.
:rtype: bool

.. seealso::  :any:`run_for`.
)pbdoc");
    thing.def("report_why_we_stopped",
              &Runner::report_why_we_stopped,
              R"pbdoc(
Report why run stopped. Reports whether :any:`run()` was stopped because
it is :any:`finished()`, :any:`timed_out()`, or :any:`dead()`.
)pbdoc");
    thing.def("finished",
              &Runner::finished,
              R"pbdoc(
Check if run has been run to completion or not.

This function returns ``True`` if :any:`run()` has been run to completion. For
this to work, the implementation of :any:`run()` in a derived class of
:any:`Runner` must implement a specialisation of ``finished_impl``.

:returns: A ``bool``.
:rtype: bool

.. seealso::  :any:`started()`
)pbdoc");
    thing.def("success",
              &Runner::success,
              R"pbdoc(
Check if run has been run to completion successfully.

This function returns ``True`` if :any:`run()` has been run to completion and
it was successful.

:returns: Whether or not :any:`run` was successful or not.
:rtype: bool

.. seealso::  :any:`started()`
)pbdoc");
    thing.def("started",
              &Runner::started,
              R"pbdoc(
Check if run has been called at least once before.

This function returns ``True`` if :any:`run()` has started to run (it can be
running or not).

:returns: A ``bool``.
:rtype: bool

.. seealso::  :any:`finished()`
)pbdoc");
    thing.def("running",
              &Runner::running,
              R"pbdoc(
Check if currently running.

This function returns ``True`` if :any:`run()` is in the process of running and
``False`` it is not.

:returns: Whether or not the runner is running.
:rtype: bool

.. seealso::  :any:`finished()`
)pbdoc");
    thing.def("kill",
              &Runner::kill,
              R"pbdoc(
Stop run from running (thread-safe).
This function can be used to terminate :any:`run()` from another thread. After
:any:`kill()` has been called the :any:`Runner` may no longer be in a valid
state, but will return ``True`` from :any:`dead()`.

.. seealso::  :any:`finished()`)pbdoc");
    thing.def("dead",
              &Runner::dead,
              R"pbdoc(
Check if the runner is dead.
This function can be used to check if we should terminate :any:`run()` because
it has been killed by another thread.

:returns: A ``bool``.
:rtype: bool

.. seealso::  :any:`kill()`
)pbdoc");
    thing.def("stopped",
              &Runner::stopped,
              R"pbdoc(
Check if the runner is stopped. This function can be used to check
whether or not :any:`run()` has been stopped for whatever reason. In
other words, it checks if :any:`timed_out()`, :any:`finished()`, or
:any:`dead()`.

:returns:
   A ``bool``.

:rtype:
   bool
)pbdoc");
    thing.def("stopped_by_predicate",
              &Runner::stopped_by_predicate,
              R"pbdoc(
Check if the runner was stopped, or should stop, because of the argument
last passed to run_until. If *self* is running, then the nullary
predicate is called and its return value is returned. If *self* is not
running, then ``True`` is returned if and only if the last time *self*
was running it was stopped by a call to the nullary predicate passed to
:any:`run_until()`.

:returns:
   A ``bool``.

:rtype:
   bool

:complexity:
   Constant.
)pbdoc");
    thing.def("running_for",
              &Runner::running_for,
              R"pbdoc(
Check if the runner is currently running for a particular length of
time. If the :any:`Runner` is currently running because its member
function :any:`run_for` has been invoked, then this function returns
``True``. Otherwise, ``False`` is returned.

:returns:
   A ``bool``.

:rtype:
   bool

:complexity:
   Constant.
)pbdoc");
    thing.def("running_until",
              &Runner::running_until,
              R"pbdoc(
Check if the runner is currently running until a nullary predicate
returns true. If the :any:`Runner` is currently running because its
member function :any:`run_until` has been invoked, then this function
returns ``True``. Otherwise, ``False`` is returned.

:returns:
   A ``bool``.

:rtype:
   bool

:complexity:
   Constant.
)pbdoc");
    thing.def("current_state",
              &Runner::current_state,
              R"pbdoc(
Return the current state.

This function returns the current state of the :any:`Runner`
as given by :any:`state`.

:returns:
   A value of type ``state``.
:rtype:
   Runner.state

:complexity:
   Constant.
)pbdoc");
  }  // init_runner

}  // namespace libsemigroups
