.. include:: <isoamsa.txt>

Information for developers
==========================
How to compile
--------------
To build ``libsemigroups_pybind11``, it is first required to have a system
install of ``libsemigroups``. This is explained in full detail in the
``libsemigroups``
`documentation <https://libsemigroups.readthedocs.io/en/latest/install.html>`_.

It is recommended to install libsemigroups without ``hpcombi`` and with an 
external version of ``fmt`` that can be found using the environment variable
``$LD_LIBRARY_PATH``. Then, with ``libsemigroups`` installed, the python
bindings can be ``pip`` installed. This may require the environment variable
``$PKG_CONFIG_PATH`` to be edited.

To create a conda environment with ``fmt``, ``pip``, and correct environment
variables :

.. code-block:: bash
    
    conda create -n libsemigroups python pip fmt
    conda activate libsemigroups
    conda env config vars set LD_LIBRARY_PATH="$CONDA_PREFIX/lib"
    conda env config vars set PKG_CONFIG_PATH="$CONDA_PREFIX/lib/pkgconfig:$CONDA_PREFIX/share/pkgconfig:/usr/local/lib/pkgconfig"

To build libsemigroups (with the above environment active):

.. code-block:: bash

    git clone https://github.com/libsemigroups/libsemigroups
    cd libsemigroups
    ./autogen.sh && ./configure --disable-hpcombi --with-external-fmt && sudo make install -j8

where ``-j8`` instructs the compiler to use 8 threads.

To build the python bindings (with CCache):

.. code-block:: bash

    CC="ccache gcc" CXX="ccache g++"  pip install .

Building the skeleton of a class
--------------------------------

If you are adding the bindings for a ``libsemigroups`` class that does not yet
exist in ``libsemigroups_pybind11``, please consider running the script
``generate_pybind11.py`` found in the ``etc/`` directory of
``libsemigroups``.

For example:

.. code-block:: bash

    etc/generate_pybind11.py libsemigroups::KnuthBendix

to create a skeleton of the code required to bind the ``KnuthBendix`` class.

This script will not generate everything for you; you will be required to edit
the output. If you followed the guidance output by the script, the class will
now be accessible in ``_libsemigroups_pybind11``. You must now check the
contents of that file adheres to the styles set out in this guide. Furthermore,
must follow the steps described in 

The bindings
------------

The purpose of this section is to aid in the writing of the bindings, with
a focus on the required style. This is not a detailed guide on how to use
``pybind11``. For that, please consult the
`pybind11 documentation <https://pybind11.readthedocs.io/en/stable>`__.

An example of binding a function
--------------------------------

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
    A brief description of bar that is one line long

    A more detailed description of bar, that may explain how each of the
    parameters *a* and *b* should be used. This can be more than one line long,
    and can contain cross-references to other python objects such as
    :py:meth:`baz`, :py:class:`int` or :py:obj:`Konieczny`. It can also contain
    literals like ``True`` or ``False``.

    :param a: an explanation of what *a* is.
    :type a: str
    :param b: an explanation of what *b* is.
    :type b: int

    :raises LibsemigroupsError: Why this raises the error 

    :return: The value that should be returned
    :rtype: int

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



Inheritance
___________
If the class you are binding inherits from another class, this should also be
reflected in python. This is done when creating the ``pybind11::class`` object
by passing a template parameter for the class that is being inherited from. As
an example, since the ``KnuthBendix`` class inherits from the
``CongruenceInterface`` class, the code for the bindings of the ``KnuthBendix``
class will start with:

.. code-block:: cpp

  pybind11::class_<KnuthBendix<Rewriter>, CongruenceInterface> kb(m, name.c_str());
                                          ^^^^^^^^^^^^^^^^^^^

Class templates
_______________

TODO: Something about having init and bind functions.

The documentation
-----------------
Each class should have a ``.rst`` file in ``docs/source`` that looks like this:

.. code-block:: rst

    .. Copyright (c) 20XX, Name

        Distributed under the terms of the GPL license version 3.

        The full license is in the file LICENSE, distributed with this software.

    .. currentmodule:: _libsemigroups_pybind11

    Class-Name
    ==========

    A description of what the methods in this class do.

    .. doctest::
        
        >>> # This should be a quick example of how to create an instance of
        >>> # YourClass, and run a few functions.
        >>> from libsemigroups_pybind11 import YourClass
        >>> y = YourClass()
        >>> y.run()
        True
        >>> y.count()
        42
    
    Contents
    --------
    .. autosummary::
        :nosignatures:

        YourClass.foo
        YourClass.bar
        YourClass.baz
        YourClass.qux
        YourClass.quux
        YourClass.corge


    Full API
    --------
    .. autoclass:: YourClass
        :members:

For an example, see
`docs/source/knuth-bendix/knuth-bendix.rst <docs/source/knuth-bendix/knuth-bendix.rst?plain=1>`__

When ``make doc`` is run, the content of this ``.rst`` files is converted to
html. Before this is done, some processing can be done on the docs. In
`<docs/source/conf.py>`__, there is a dictionary called ``type_replacements``
that serves as a map from bad type names |map| good type names. This can be used
to translate from confusing c++ type names to nice python type names. It should
also be used to 


.. TODO
.. Say where the doc should be added
.. Anything in a namespace needs to be imported into a py file
.. Anything templated needs a py file to make a pseudo-constructor function
.. Post-processing
.. Check that all functions (including those inherit run as intended