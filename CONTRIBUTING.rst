Information for developers
==========================

How to compile
--------------

To build ``libsemigroups_pybind11``, it is first required to have a system
install of ``libsemigroups``. This is explained in full detail in the
``libsemigroups``
`documentation <https://libsemigroups.github.io/libsemigroups/md_install.html>`_.

If you only intend to contribute to ``libsemigroups_pybind11``, and not 
``libsemigroups``, it is sufficient to have the conda version of
``libsemigroups``.

If you intend to develop both ``libsemigroups`` and ``libsemigroups_pybind11``,
it is recommended to build ``libsemigroups`` from the sources. Furthermore, it is
recommended to install ``libsemigroups`` without ``hpcombi``. Then, with
``libsemigroups`` installed, the Python bindings can be ``pip`` installed. This
may require the environment variable ``$PKG_CONFIG_PATH`` to be edited.

To create an environment with, ``pip``, correct environment variables, and some
other helpful tools:

.. code-block:: console

    source etc/make-dev-environment.sh [package_manager]

where [package_manager] is your favourite conda-like package manager, such as
conda or mamba. The default value is mamba. Note that this DOES NOT *yet* work
with micromamba.

To build libsemigroups (with the above environment active):

First, clone and prepare the libsemigroups repository:

.. code-block:: console

    git clone https://github.com/libsemigroups/libsemigroups
    cd libsemigroups
    ./autogen.sh

Then choose one of the following installation methods:

**Option 1: System-wide installation**

For a system-wide installation (requires sudo):

.. code-block:: console

    ./configure --disable-hpcombi
    make -j8
    sudo make install

**Option 2: Install into conda/mamba environment**

.. code-block:: console

    ./configure --prefix=$CONDA_PREFIX --disable-hpcombi
    make -j8
    make install

In both cases, ``-j8`` instructs the compiler to use 8 threads (adjust based on your
system).

To build the Python bindings (with CCache) inside the ``libsemigroups_pybind11``
directory:

.. code-block:: console

    CC="ccache gcc" CXX="ccache g++"  pip install .

Building the skeleton of a class
--------------------------------

If you are adding the bindings for a ``libsemigroups`` class that does not yet
exist in ``libsemigroups_pybind11``, please consider running the script
``generate_pybind11.py`` found in the ``etc/`` directory of
``libsemigroups``.

For example:

.. code-block:: console

    etc/generate_pybind11.py libsemigroups::KnuthBendix

to create a skeleton of the code required to bind the ``KnuthBendix`` class.

This script will not generate everything for you; you will be required to edit
the output. If you followed the guidance output by the script, the class will
now be accessible in ``_libsemigroups_pybind11``. You must now check the
contents of that file adheres to the styles set out in this guide.

The bindings
------------

The purpose of this section is to aid in the writing of the bindings, with
a focus on the required style. This is not a detailed guide on how to use
``pybind11``. For that, please consult the
`pybind11 documentation <https://pybind11.readthedocs.io/en/stable>`__.

An example of binding a function
________________________________

A basic guide on how to create bindings for a simple function can be found
`here <https://pybind11.readthedocs.io/en/stable/basics.html#creating-bindings-for-a-simple-function>`__.

In the ``libsemigroups`` context, to bind the function ``bar`` from the class
``foo`` to the module ``m``:

.. code-block:: cpp

    // This is the left-hand side of the file
        m.def("bar",
            &libsemigroups::foo::bar,
            py::arg("a"),
            py::arg("b"),
            R"pbdoc(
    :sig=(a: str, b: list[int]) -> int:
    A brief description of bar that is one line long

    A more detailed description of bar, that may explain how each of the
    parameters *a* and *b* should be used. This can be more than one line long,
    and can contain cross-references to other python objects such as
    :py:meth:`baz`, :py:class:`int` or :py:obj:`Konieczny`. It can also contain
    literals like ``True`` or ``False``.

    :param a: an explanation of what *a* is.
    :type a: str
    :param b: an explanation of what *b* is.
    :type b: list[int]

    :return: The value that should be returned
    :rtype: int

    :raises LibsemigroupsError: Why this raises the error

    .. seealso:: Something that might be interesting.
    )pbdoc");

Notice that there should be **NO BLOCK INDENTATION** in the docstring. This is
so that ``sphinx`` builds the docs correctly.


Some more on docstings
______________________

Please adhere to the
`Sphinx docstring format <https://sphinx-rtd-tutorial.readthedocs.io/en/latest/docstrings.html>`__
when writing your documentation. A summary of some useful conventions are shown
below.

.. list-table::
    :header-rows: 1
    :widths: 1 3 2

    * - Element
      - Markup
      - See also
    * - Parameter
      - .. raw:: html

          <code class="code docutils literal notranslate">*args*</code>

      - `reStructuredText markup (Python Developer's Guide) <https://devguide.python.org/documentation/markup/>`__
    * - Literals
      - .. raw:: html

          <code class="code docutils literal notranslate">``True``</code>,&nbsp;
          <code class="code docutils literal notranslate">``len(s) - 1``</code>

      - 
    * - Cross-references
      - .. raw:: html

          <code class="code docutils literal notranslate">:role:`target`</code>
 
      - `Cross referencing (Sphinx) <https://www.sphinx-doc.org/en/master/usage/referencing.html>`__
    * - Python cross-references
      - .. raw:: html

          <code class="code docutils literal notranslate">:py:class:`int`</code><br/>
          <code class="code docutils literal notranslate">:py:obj:`collections.abc.Iterator[(str, str)]`</code><br/>
          <code class="code docutils literal notranslate">:py:meth:`knuth_bendix.by_overlap_length &lt;libsemigroups_pybind11.knuth_bendix.by_overlap_length&gt;`</code>

      - `Cross-referencing Python Objects (Sphinx) <https://www.sphinx-doc.org/en/master/usage/domains/python.html#the-python-domain>`__
    * - Clever cross-references
      - .. raw:: html

          <code class="code docutils literal notranslate">:any:`int`</code>
 
      - `Cross-referencing anything (Sphinx) <https://www.sphinx-doc.org/en/master/usage/referencing.html#cross-referencing-anything>`__
    * - Maths
      - .. raw:: html

          <code class="code docutils literal notranslate">:math:`O(mn)`</code>

      - `Interpreted text roles (Docutils) <https://docutils.sourceforge.io/docs/ref/rst/roles.html#math>`__
    * - Code (with doctest)
      - .. code-block:: rst

            .. doctest::

              >>> 2+2
              4


      - `Doctest blocks (Sphinx) <https://www.sphinx-doc.org/en/master/usage/restructuredtext/basics.html#doctest-blocks>`__

At compile-time, Pybind11 attempts to figure out what the type hints of all the
functions should be. Usually, these are quite good, but sometimes it might be
necessary to adjust these manually. This can be done by adding a special
signature line to the start of your docsting:

.. code-block:: cpp
  
  R"pbdoc(
  :sig=(arg0: Type0, arg1: Type1) -> ReturnType:

  ... 
  )pbdoc"


Inheritance
___________

If the class you are binding inherits from another class, this should also be
reflected in Python. This is done when creating the ``pybind11::class_`` object
by passing a template parameter for the class that is being inherited from. As
an example, since the ``Runner`` class inherits from the ``Reporter`` class,
the code for the bindings of the ``Runner`` class will start with:

.. code-block:: cpp

  pybind11::class_<Runner, Reporter> runner(m, "Runner");
                           ^^^^^^^^

Making your functions available in ``libsemigroups_pybind11``
-------------------------------------------------------------

If you followed the instructions in the ``generate_pybind11.py`` script from the
``libsemigroups`` project, the class you have added bindings for should now be
available in ``_libsemigroups_pybind11`` (note the leading underscore). How to
make this available in ``libsemigroups_pybind11`` depends on several factors.

A class with no helpers or templates
____________________________________

If the class you are binding has no templates or helper functions, then you
need to add it to the list imports in 
`<src/libsemigroups_pybind11/__init__.py>`__.

A class with helpers
____________________

If a class has a helper namespace, this should be respected in Python by
creating a module with the same name in the ``src/libsemigroups_pybind11``
directory. In that module, all of the relevant helper functions should be
imported from ``_libsemigroups_pybind11``.

A class with templates
______________________

If a class has templates parameters then, in ``_libsemigroups_pybind11``, there
will be one class for each combination of templates. Instead of calling these
directly, a Python class should be constructed that acts as a way of dispatching
the correct functionality. This can class should inherit from 
``libsemigroups.detail.CxxWrapper``. An example of such a class can be found in
`<src/libsemigroups_pybind11/sims.py>`__.

The documentation
-----------------

Each class should have a directory in either ``docs/source/data-structures`` or
``docs/source/main-algorithms``. Usually, this directory will contain an
``index.rst``, a file that documents the class, and a file that documents any
helper functions. If the thing you are documenting is quite complex, it is fine
to split these files further.

An example of an ``.rst`` file that documents a class look like this:

.. code-block:: rst

    .. Copyright (c) 20XX, Name

        Distributed under the terms of the GPL license version 3.

        The full license is in the file LICENSE, distributed with this software.

    .. currentmodule:: libsemigroups_pybind11

    Class-Name
    ==========

    .. autoclass:: YourClass
        :doc-only:


    Contents
    --------
    .. autosummary::
        :signatures: short

        YourClass.foo
        YourClass.bar
        YourClass.baz
        YourClass.qux
        YourClass.quux
        YourClass.corge


    Full API
    --------
    .. autoclass:: YourClass
        :class-doc-from: init
        :members:

For an example, see
`docs/source/main-algorithms/knuth-bendix/knuth-bendix.rst <docs/source/main-algorithms/knuth-bendix/knuth-bendix.rst?plain=1>`__

An ``.rst`` file that documents some helper functions may look like this:

.. code-block:: rst

  .. Copyright (c) 20XX, YOUR NAME

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

  .. currentmodule:: libsemigroups_pybind11

  Class-name helpers
  ====================

  This page contains the documentation for various helper functions for
  manipulating ``class`` objects. All such functions are contained in the
  submodule ``libsemigroups_pybind11.class``.

  Contents
  --------
  .. autosummary::
    :signatures: short

    foo
    bar
    baz

  Full API
  --------
  .. automodule:: libsemigroups_pybind11.class
    :members:
    :imported-members:

A sample ``index.rst`` file may look like this:

.. code-block:: rst

  .. Copyright (c) 20XX, YOUR NAME

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

  Class
  =====

    This page describes the functionality for the class in
    ``libsemigroups_pybind11``.


  .. toctree::
    :maxdepth: 1

    class
    class-helpers

Post-processing
_________________

When ``make doc`` is run, the content of these ``.rst`` files is converted to
html. Before this is done, some processing can be done on the docs. In
`<docs/source/_ext/libsemigroups_pybind11_extensions.py>`__, there are three
dictionaries that can be used to make replacements.

The first dictionary is called ``type_replacements`` that serves as a map from
bad type names -> good type names that should be replaced in the signature
of every function. This can be used to translate from confusing C++ type names
to nice Python type names.

The second dictionary is called ``class_specific_replacements`` that serves as a
map from "class name" -> ("good type", "bad type"). This will be used to
replace bad type names with good type names in all signatures of a particular
class.

The third dictionary is called ``docstring_replacements`` and serves as a map
from "bad" strings to "good" strings in the doc-strings of each function. This
will be used to change things in the doc-string that can't easily be done by
editing the source code of that doc-string. One example of this is when pybind11
includes the signature of a function in its docstring so that some Sphinx parser
knows how to correctly render that function. If you want to import that function
with a different name (e.g. ``... import pbr_one as one``), the
pybind11-inserted signature is wrong, and should be removed. You can use this
dictionary to do that.

After the doc has been converted to html, it may still be desirable to make
text replacements. This can be done by adding to the ``replacements`` dictionary
in `<etc/replace-strings-in-doc.py>`__.

Including your files in the doc
_______________________________

Inside `<docs/source/index.rst>`__, you will find the table of contents tree
(toctree) for this project. Within that, you will find the names of files
(without the ``.rst`` extension) of different classifications of structures that
``libsemigroups_pybind11`` implements, such as congruences, digraphs, semigroups
and words. Within each of these files, there is another toctree containing
the paths to the docs of various classes.

To the relevant toctree, add the path to either the ``index.rst`` file for the
class.

Checking your contributions
---------------------------

When you think you have finished writing the bindings, please add a test file to
the ``tests/`` directory that tests each of the functions that have just had
bindings added, including inherited functions.

Then run

.. code-block:: bash

  make check

and ensure everything passes.

File overview
-------------

As a quick reference, the files that you may need to create, edit or refer to
whilst contributing are::

  libsemigroups_pybind11/
  ├── docs/
  │   └── source/
  |       ├── _ext/
  |       |   └── libsemigroups_pybind11_extensions.py
  |       ├── data-structures/
  |       ├── main-algorithms/
  │       |   └── class-name/
  │       |       ├── index.rst
  │       |       ├── class.rst
  │       |       └── helpers.rst
  │       ├── conf.py
  │       └── index.rst
  ├── etc/
  │   └── replace-string-in-doc.py
  ├── src/
  |   ├──libsemigroups_pybind11/
  │   |   ├── __init__.py
  │   |   └── class_name.py
  │   └── class-name.cpp
  ├── tests/
  │   └── test_class_name.py
  └── CONTRIBUTING.rst (this file!)
