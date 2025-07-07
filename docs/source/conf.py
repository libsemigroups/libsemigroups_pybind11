#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# pylint: disable=invalid-name, line-too-long
"""
This provides configuration for the generation of the docs

More detail of the available configuration options can be found in the
following places:

    * sphinx:           https://www.sphinx-doc.org/en/master/usage/configuration.html#module-conf
    * autodoc:          https://www.sphinx-doc.org/en/master/usage/extensions/autodoc.html#configuration
    * autosummary:      https://www.sphinx-doc.org/en/master/usage/extensions/autosummary.html#generating-stub-pages-automatically
    * bibtex:           https://sphinxcontrib-bibtex.readthedocs.io/en/latest/usage.html#configuration
    * coppybutton:      https://sphinx-copybutton.readthedocs.io/en/latest/use.html for full info
    * doctest:          https://www.sphinx-doc.org/en/master/usage/extensions/doctest.html#configuration
    * intersphinx:      https://www.sphinx-doc.org/en/master/usage/extensions/intersphinx.html#configuration
    * mathjax:          https://www.sphinx-doc.org/en/master/usage/extensions/math.html#module-sphinx.ext.mathjax
    * sphinx_rtd_theme: https://sphinx-rtd-theme.readthedocs.io/en/stable/configuring.html
"""

import importlib.metadata
import sys
from pathlib import Path

########################################################################
# sphinx options
########################################################################

############ Project information ############

# The documented project’s name
project = "libsemigroups_pybind11"

# The project’s author(s)
author = "Reinis Cirpons, Joseph Edwards, James Mitchell, Maria Tsalakou, Murray Whyte"

# A copyright statement. The string '%Y' will be replaced with the current
# four-digit year.
project_copyright = "2021-%Y, " + author

# The major project version, used as the replacement for the |version| default
# substitution.
# TODO(0): We should probably change this so that the we only have the major
# version, rather than all of the git-related gunk
version = importlib.metadata.version(project)

# The full project version, used as the replacement for the |release| default
# substitution.
release = version

############ Options for highlighting ############

# The default language to highlight source code in.
highlight_language = "python"

# The style name to use for Pygments highlighting of source code.
pygments_style = "sphinx"

############ Options for markup ############

# A string of reStructuredText that will be included at the end of every source
# file that is read.
rst_epilog = ""
# Read link all targets from file
with open("_static/links.rst", encoding="utf-8") as f:
    rst_epilog += f.read()

############ Options for nitpicky mode ############

# In nitpicky mode, Sphinx will warn about all references where the target
# cannot be found.
nitpicky = True

# A set or list of (warning_type, target) tuples that should be ignored when
# generating warnings in "nitpicky mode".
# See https://github.com/sphinx-doc/sphinx/issues/10785 for why the numpy ones
# don't work.
nitpick_ignore = {
    ("py:class", "Element"),
    ("py:class", "Letter"),
    ("py:class", "Point"),
    ("py:class", "Range"),
    ("py:class", "Word"),
    ("py:class", "adapters.Point"),
    ("py:class", "adapters.Element"),
    ("py:class", "numpy.float64"),
}

############ Options for source files ############

# A list of glob-style patterns that should be excluded when looking for source
# files.
exclude_patterns = ["_build", "_old", "_static"]

# The name of the document containing the master toctree directive, and hence
# the root of the entire tree.
master_doc = "index"

# A dictionary mapping the file extensions (suffixes) of source files to
# their file types.
source_suffix = {".rst": "restructuredtext"}

############ Options for  HTML output ############

# The theme for HTML output.
html_theme = "sphinx_rtd_theme"

# A dictionary of options that influence the look and feel of the selected
# theme.
# The titles_only option stops the page sections and toc (which have the
# same names) from being duplicated in the sidebar on the Konieczny and
# FroidurePin pages.
html_theme_options = {
    "titles_only": True,
    "style_nav_header_background": "#2980B9",
}

# The project logo
html_logo = "../pictures/libsemigroups_pybind11_logo.svg"

# The project favicon
html_favicon = html_logo

# A list of CSS files.
html_css_files = ["custom.css"]

# A list of paths that contain custom static files (such as style sheets or
# script files).
html_static_path = ["_static"]

############ Options for HTML help output ############

# Output file base name for HTML help builder.
htmlhelp_basename = "libsemigroups_pybind11"

############ Options for manual page output ############

# How to group the document tree into manual pages.
man_pages = [
    (
        master_doc,
        "libsemigroups_pybind11",
        "libsemigroups_pybind11 Documentation",
        author,
        1,
    )
]

############ Options for the Python domain ############


# A boolean that decides whether module names are prepended to all object names.
# We set this to True here, but remove "libsemigroups_pybind11\..*" from the doc
# everywhere. This is done so we still get the submodule names, but not the
# global module name. A nicer, but more involved solution, could use some Sphinx
# magic as done in https://stackoverflow.com/a/72658470/15278419.
add_module_names = True

########################################################################
# Extension options
########################################################################

# Allows sphinx to use our custom extension
sys.path.append(str(Path("_ext").resolve()))

extensions = [
    "sphinx.ext.autodoc",
    "sphinx.ext.autosummary",
    "sphinxcontrib.bibtex",
    "sphinx_copybutton",
    "sphinx.ext.doctest",
    "sphinx.ext.intersphinx",
    "sphinx.ext.mathjax",
    "libsemigroups_pybind11_extensions",
]

############ autodoc ############

# What content will be inserted into the main body of an autoclass directive.
autoclass_content = "class"

############ autosummary ############

# Boolean indicating whether to scan all found documents for autosummary
# directives, and to generate stub pages for each.
autosummary_generate = False

############ bibtex ############

# List of bib files
bibtex_bibfiles = ["libsemigroups.bib"]

############ copybutton ############

# Setting that means we don't copy the prompt characters like >>> or $
copybutton_exclude = ".linenos, .gp"

############ doctest ############

# Python code that is treated like it were put in a testsetup directive for
# every file that is tested, and for every group.
doctest_global_setup = """from libsemigroups_pybind11 import ReportGuard
ReportGuard(False)"""

############ intersphinx ############

# Thhe locations and names of other projects that should be linked to in this
# documentation.
intersphinx_mapping = {
    "python": ("https://docs.python.org/3", None),
    "numpy": ("https://numpy.org/doc/stable/", None),
}
