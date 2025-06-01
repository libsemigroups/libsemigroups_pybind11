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
The time between the given point and now.

:param t:
   the time point

:type t:
   datetime.timedelta

:returns:
   The difference between the time point ``t`` and now.

:rtype:
   datetime.timedelta
)pbdoc");

    py::class_<Reporter> thing(m,
                               "Reporter",
                               R"pbdoc(
Collection of values related to reporting.

This class exists to collect some values related to reporting in its derived
classes. These values are:

-  :any:`report_prefix()`;
-  :any:`report_every()`;
-  :any:`last_report()`;
-  :any:`start_time()`.
)pbdoc");

    thing.def(py::init<>(), R"pbdoc(
Default construct a :any:`Reporter` object such that the following hold:

-  :any:`report_prefix()` is empty;
-  :any:`report_every()` is set to 1 second;
-  :any:`last_report()` is now;
-  :any:`start_time()` is now.
)pbdoc");
    thing.def(py::init<Reporter const&>(), R"pbdoc(
Default copy constructor.
)pbdoc");
    thing.def(
        "init",
        [](Reporter& r) -> Reporter& { return r.init(); },
        R"pbdoc(
Initialize an existing Reporter object.

This function puts a :any:`Reporter` object back into the same state as
if it had been newly default constructed.

:returns: ``self``.
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

This function can be used to specify at run time the minimum elapsed time between two calls to :any:`report()` that will return ``True``. If :any:`report()` returns ``True`` at time ``s`` , then :any:`report()` will only return ``True`` again after at least time ``s + t`` has elapsed.

:param val: the amount of time between reports.
:type val: datetime.timedelta

:returns: ``self``.
:rtype: Reporter

.. seealso::  :any:`report_every`
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

This is the time point at which
:any:`reset_start_time()` was last called, which is also the time of
construction of a :any:`Reporter` instance if :any:`reset_start_time()`
is not explicitly called.

:returns:
   The time delta representing the start time.

:rtype:
   datetime.timedelta
)pbdoc");
    thing.def("reset_start_time",
              &Reporter::reset_start_time,
              R"pbdoc(
Reset the start time (and last report) to now.

:returns: ``self``.
:rtype: Reporter
)pbdoc");
    thing.def("last_report",
              &Reporter::last_report,
              R"pbdoc(
Get the time point of the last report. Returns the time point of the
last report, as set by one of:

-  :any:`reset_start_time()`;
-  :any:`report_every()`; or
-  :any:`report()`.

:returns:
   A :any:`datetime.timedelta`.

:rtype:
   datetime.timedelta
)pbdoc");
    thing.def("reset_last_report",
              &Reporter::reset_last_report,
              R"pbdoc(
Set the last report time point to now.

:returns: ``self``.

:rtype:
   Reporter
)pbdoc");
    thing.def(
        "report_prefix",
        [](Reporter& self, std::string const& val) {
          return self.report_prefix(val);
        },
        py::arg("val"),
        R"pbdoc(
Set the prefix string for reporting.

This function sets the return value of :any:`report_prefix()` to (a copy of) the argument ``val``. Typically this prefix should be the name of the algorithm being run at the outmost level.

:param val: the new value of the report prefix.
:type val: str

:returns: ``self``.
:rtype: Reporter
)pbdoc");
    thing.def(
        "report_prefix",
        [](Reporter const& self) { return self.report_prefix(); },
        R"pbdoc(
Get the current prefix string for reporting. This function gets the
current value of the prefix string for reporting (set via
:any:`report_prefix`), which is typically the name of the
algorithm being run at the outmost level.

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
Abstract class for derived classes that run an algorithm.

Many of the classes in ``libsemigroups`` implementing the algorithms, that
are the reason for the existence of this library, are derived from
:any:`Runner`. The :any:`Runner` class exists to collect various common
tasks required by such a derived class with a possibly long running
:any:`run`. These common tasks include:

*  running for a given amount of time (:any:`run_for`)
*  running until a nullary predicate is true (:any:`run_until`)
*  checking the status of the algorithm: has it :any:`started` ? :any:`finished`? been killed by another thread (:any:`dead`)? has it timed out (:any:`timed_out`)? has it :any:`stopped` for any reason?
*  permit the function :any:`run` to be killed from another thread (:any:`kill`).
)pbdoc");

    py::enum_<Runner::state> state(m, "Runner.state", R"pbdoc(
Indicates that none of :any:`run` , :any:`run_for` , or :any:`run_until`
has been called since construction or the last call to :any:`init`.
)pbdoc");
    state
        .value("never_run", Runner::state::never_run, R"pbdoc(
Indicates that none of :any:`run` , :any:`run_for` , or :any:`run_until`
has been called since construction or the last call to :any:`init`.
)pbdoc")
        .value("running_to_finish", Runner::state::running_to_finish, R"pbdoc(
Indicates that none of :any:`run` , :any:`run_for` , or :any:`run_until`
has been called since construction or the last call to :any:`init`.
)pbdoc")
        .value("running_for", Runner::state::running_for, R"pbdoc(
Indicates that none of :any:`run` , :any:`run_for` , or :any:`run_until`
has been called since construction or the last call to :any:`init`.
)pbdoc")
        .value("running_until", Runner::state::running_until, R"pbdoc(
Indicates that none of :any:`run` , :any:`run_for` , or :any:`run_until`
has been called since construction or the last call to :any:`init`.
)pbdoc")
        .value("timed_out", Runner::state::timed_out, R"pbdoc(
Indicates that none of :any:`run` , :any:`run_for` , or :any:`run_until`
has been called since construction or the last call to :any:`init`.
)pbdoc")
        .value("stopped_by_predicate",
               Runner::state::stopped_by_predicate,
               R"pbdoc(
Indicates that none of :any:`run` , :any:`run_for` , or :any:`run_until`
has been called since construction or the last call to :any:`init`.
)pbdoc")
        .value("not_running", Runner::state::not_running, R"pbdoc(
Indicates that none of :any:`run` , :any:`run_for` , or :any:`run_until`
has been called since construction or the last call to :any:`init`.
)pbdoc")
        .value("dead", Runner::state::dead, R"pbdoc(
Indicates that none of :any:`run` , :any:`run_for` , or :any:`run_until`
has been called since construction or the last call to :any:`init`.
)pbdoc");
    thing.attr("state") = state;
    thing.def(
        "init",
        [](Runner& r) -> Runner& { return r.init(); },
        R"pbdoc(
Initialize an existing Runner object.
This function puts a :any:`Runner` object back into the same state as if it had been newly default constructed.

:returns: ``self``.
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

For this to work it is necessary to periodically check if :any:`timed_out()` returns ``True``, and to stop if it is, in the :any:`run()` member function of any derived class of :any:`Runner`.

:param t: the time to run for.
:type t: datetime.timedelta

.. seealso::  :any:`run_for`)pbdoc");
    thing.def("run_until",
              (void (Runner::*)(std::function<bool()>&)) &Runner::run_until,
              py::arg("func"),
              R"pbdoc(
Run until a nullary predicate returns true or finished.

:param func:
   a function pointer.

:type func:
   bool(*)()
)pbdoc");
    thing.def("timed_out",
              &Runner::timed_out,
              R"pbdoc(
Check if the amount of time passed to run_for has elapsed.

:returns: A ``bool``
:rtype: bool

.. seealso::  :any:`run_for`.
)pbdoc");
    thing.def("report_why_we_stopped",
              &Runner::report_why_we_stopped,
              R"pbdoc(
Report why run stopped. Reports whether :any:`run()` was stopped because
it is :any:`finished()` , :any:`timed_out()` , or :any:`dead()`.
)pbdoc");
    thing.def("finished",
              &Runner::finished,
              R"pbdoc(
Check if run has been run to completion or not.

Returns ``True`` if :any:`run()` has been run to completion. For this to work, the implementation of :any:`run()` in a derived class of :any:`Runner` must implement a specialisation of ``finished_impl``.

:returns: A ``bool``.
:rtype: bool

.. seealso::  :any:`started()`
)pbdoc");
    thing.def("success",
              &Runner::success,
              R"pbdoc(
Check if run has been run to completion successfully.

Returns ``True`` if :any:`run()` has been run to completion and it was
successful.

:returns: A ``bool``.
:rtype: bool

.. seealso::  :any:`started()`
)pbdoc");
    thing.def("started",
              &Runner::started,
              R"pbdoc(
Check if run has been called at least once before.
Returns ``True`` if :any:`run()` has started to run (it can be running or not).

:returns: A ``bool``.
:rtype: bool

.. seealso::  :any:`finished()`
)pbdoc");
    thing.def("running",
              &Runner::running,
              R"pbdoc(
Check if currently running.
Returns ``True`` if :any:`run()` is in the process of running and ``False`` it is not.

:returns: A ``bool``.
:rtype: bool

.. seealso::  :any:`finished()`
)pbdoc");
    thing.def("kill",
              &Runner::kill,
              R"pbdoc(
Stop run from running (thread-safe).
This function can be used to terminate :any:`run()` from another thread. After :any:`kill()` has been called the :any:`Runner` may no longer be in a valid state, but will return ``True`` from :any:`dead()`.

.. seealso::  :any:`finished()`)pbdoc");
    thing.def("dead",
              &Runner::dead,
              R"pbdoc(
Check if the runner is dead.
This function can be used to check if we should terminate :any:`run()` because it has been killed by another thread.

:returns: A ``bool``.
:rtype: bool

.. seealso::  :any:`kill()`
)pbdoc");
    thing.def("stopped",
              &Runner::stopped,
              R"pbdoc(
Check if the runner is stopped. This function can be used to check
whether or not :any:`run()` has been stopped for whatever reason. In
other words, it checks if :any:`timed_out()` , :any:`finished()` , or
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
last passed to run_until. If ``self`` is running, then the nullary
predicate is called and its return value is returned. If ``self`` is not
running, then ``True`` is returned if and only if the last time ``self``
was running it was stopped by a call to the nullary predicate passed to
:any:`run_until()`.

:complexity:
   Constant.

:returns:
   A ``bool``.

:rtype:
   bool
)pbdoc");
    thing.def("running_for",
              &Runner::running_for,
              R"pbdoc(
Check if the runner is currently running for a particular length of
time. If the :any:`Runner` is currently running because its member
function :any:`run_for` has been invoked, then this function returns
``True``. Otherwise, ``False`` is returned.

:complexity:
   Constant.

:returns:
   A ``bool``.

:rtype:
   bool
)pbdoc");
    thing.def("running_until",
              &Runner::running_until,
              R"pbdoc(
Check if the runner is currently running until a nullary predicate
returns true. If the :any:`Runner` is currently running because its
member function :any:`run_until` has been invoked, then this function
returns ``True``. Otherwise, ``False`` is returned.

:complexity:
   Constant.

:returns:
   A ``bool``.

:rtype:
   bool
)pbdoc");
    thing.def("current_state",
              &Runner::current_state,
              R"pbdoc(
Return the current state. Returns the current state of the :any:`Runner`
as given by :any:`state`.

:complexity:
   Constant.

:returns:
   A value of type ``state``.

:rtype:
   state
)pbdoc");
  }  // init_runner

}  // namespace libsemigroups
