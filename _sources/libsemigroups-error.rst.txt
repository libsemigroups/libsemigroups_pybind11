..
    Copyright (c) 2024 Joseph Edwards

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Exceptions
==========

This page describes the custom error type used in ``libsemigroups_pybind11``,
namely :any:`LibsemigroupsError`, and related functions. Other built-in
exceptions, such as :any:`ValueError` and :any:`TypeError`, may also be raised
in this project. See the
`Python documentation <https://docs.python.org/3/library/exceptions.html>`_
for further details.

The Python module ``libsemigroups_pybind11`` was designed to mirror the C++
library libsemigroups_ as closely as possible, whilst navigating the
fundamental difference between Python and C++. This is done with the help of 
pybind11_.

For various implementational reasons (mostly related to Python's lack of an
analogue for C++'s templating system), the Python code exposed by ``pybind11``
is less streamlined and concise than the C++ library. To try and address this,
the authors of ``libsemigroups_pybind11`` have further wrapped the Python code
produced by ``pybind11`` to make the Python and C++ user experience as similar
as possible.

The Python bindings of the libsemigroups_ code produced by ``pybind11``
reside in an intermediate module called ``_libsemigroups_pybind11`` (note the
leading underscore), and the public-facing nicely wrapped code resides in this
module — ``libsemigroups_pybind11``.

Should this impact the way you, the user, use ``libsemigroups_pybind11``? For
the most part, no. It should be possible to use ``libsemigroups_pybind11`` in
the ways documented on this site without the knowledge that
``_libsemigroups_pybind11`` even exists. The notable exceptions to this relate
to type names and error messages. A lot of the code in
``libsemigroups_pybind11`` has been imported from ``_libsemigroups_pybind11``,
and this is visible in qualified type names. For example:

.. doctest:: python

    >>> from libsemigroups_pybind11 import WordGraph
    >>> WordGraph
    <class '_libsemigroups_pybind11.WordGraph'>

Additionally, some functions or classes in the ``_libsemigroups_pybind11``
module have additional prefixes and suffixes relative to their
``libsemigroups_pybind11`` counterpart. These usually relate to the module that
the function or class is contained within or a type the function or class is
defined upon. These may appear in error messages. For example:

.. doctest:: python

    >>> from libsemigroups_pybind11 import aho_corasick, AhoCorasick
    >>> ac = AhoCorasick()
    >>> aho_corasick.add_word(ac, False)
    Traceback (most recent call last):
        ...
    TypeError: aho_corasick_add_word(): incompatible function arguments. The following argument types are supported:
        1. (ac: _libsemigroups_pybind11.AhoCorasick, w: list[int]) -> int
        2. (ac: _libsemigroups_pybind11.AhoCorasick, w: str) -> int
    <BLANKLINE>
    Invoked with: <AhoCorasick with 1 node>, False

The authors of ``libsemigroups_pybind11`` have gone to a lot of effort to try
and make error messages clear, specific and intelligible; however, if you
encounter any errors with unclear messages, please let us know on the 
`issue tracker`_.

Full API
--------

.. autoexception:: LibsemigroupsError
    :show-inheritance:

    This is the main type of exception raised by the custom data-structures and
    algorithms of ``libsemigroups_pybind11``. It is raised when the underlying
    C++ code from libsemigroups_ raises a ``LibsemigroupsException``.

    .. doctest:: python

        >>> from libsemigroups_pybind11 import FroidurePin, Perm
        >>> gens = [Perm([3, 0, 1, 2]), Perm([1, 2, 0, 3]), Perm([2, 1, 0, 3])]
        >>> S = FroidurePin(gens)
        >>> S
        <partially enumerated FroidurePin with 3 generators, 3 elements, Cayley graph ⌀ 1, & 0 rules>

        >>> S.generator(3) # Bad: there are only three generators
        Traceback (most recent call last):
            ...
        LibsemigroupsError: generator index out of bounds, expected value in [0, 3), got 3

    .. note::

        If you believe your code is incorrectly throwing a
        :any:`LibsemigroupsError`, or isn't throwing a :any:`LibsemigroupsError`
        but you think it should, please let us known by opening an issue on the
        `issue tracker`_.

.. autofunction:: error_message_with_prefix
