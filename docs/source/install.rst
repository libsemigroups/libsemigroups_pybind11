.. Copyright (c) 2021, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. |libsemigroups-pybind11-version| replace:: 0.1.1

Installation
============

It is currently only possible to install ``libsemigroups_pybind11`` from its
sources. We plan to add the option of installing with ``conda`` and possibly
``pip`` in the future. 

From the sources
----------------

Before installing ``libsemigroups_pybind11`` from its sources you should first
perform a system install of the C++ library ``libsemigroups``. For information
about how to install ``libsemigroups`` see `the installation guide <https://libsemigroups.readthedocs.io/en/latest/install.html>`_.

Assuming that you have ``libsemigroups`` installed you can install
``libsemigroups_pybind11`` as follows:

::

   git clone https://github.com/libsemigroups/libsemigroups_pybind11
   cd libsemigroups_pybind11
   pip install .

From a release archive
~~~~~~~~~~~~~~~~~~~~~~

To build ``libsemigroups_pybind11`` from a release archive:

.. parsed-literal::

   curl -L -O https://github.com/libsemigroups/libsemigroups_pybind11/releases/latest/download/libsemigroups_pybind11-|libsemigroups-pybind11-version|.tar.gz
   tar -xf libsemigroups_pybind11-|libsemigroups-pybind11-version|.tar.gz 
   rm -f libsemigroups_pybind11-|libsemigroups-pybind11-version|.tar.gz
   cd libsemigroups_pybind11-|libsemigroups-pybind11-version|
   pip install .

Building the documentation 
--------------------------

The following are required to be able to build the documentation:

1. ``python3``
2. the python packages: ``jinja2 sphinx sphinx_rtd_theme sphinxcontrib-bibtex
   sphinx_copybutton``

Assuming you already have ``python3`` install, on Mac OSX you can install all of
the above by doing:

::

    python3 -m pip3 install -r docs/requirements 

Then it ought to be possible to just run ``make doc`` in the ``libsemigroups``
directory. 

Issues
------

If you find any problems with ``libsemigroups_pybind11``, or have any
suggestions for features that you’d like to see, please use the `issue tracker
<https://github.com/libsemigroups/libsemigroups_pybind11/issues>`__.
