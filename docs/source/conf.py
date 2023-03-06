#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import subprocess
import sphinx_rtd_theme

extensions = [
    "sphinx.ext.autodoc",
    "sphinx.ext.intersphinx",
    "sphinx.ext.autosummary",
    "sphinx.ext.napoleon",
    "sphinx.ext.mathjax",
    "sphinx_copybutton",
    "sphinxcontrib.bibtex",
    "sphinx.ext.doctest",
]

bibtex_bibfiles = ["libsemigroups.bib"]

autosummary_generate = True
add_module_names = False

templates_path = ["_templates"]
source_suffix = ".rst"
master_doc = "index"
project = u"libsemigroups_pybind11"
copyright = u"2021-2023, J. D. Mitchell + Maria Tsalakou"
author = u"J. D. Mitchell + Maria Tsalakou"
version = u"0.9.0"
release = u"0.9.0"
language = "python"
exclude_patterns = ["_build"]
pygments_style = "sphinx"
highlight_language = "python"
todo_include_todos = False

html_theme = "sphinx_rtd_theme"
html_theme_path = [sphinx_rtd_theme.get_html_theme_path()]
htmlhelp_basename = "libsemigroups_pybind11"

html_static_path = ["_static"]
html_css_files = ["custom.css"]

# The option in the next line stops the page sections and toc (which have the
# same names) from being duplicated in the sidebar on the Konieczny and
# FroidurePin pages
html_theme_options = {"titles_only": True}

man_pages = [
    (
        master_doc,
        "libsemigroups_pybind11",
        u"libsemigroups_pybind11 Documentation",
        [author],
        1,
    )
]

intersphinx_mapping = {"https://docs.python.org/": None}

autoclass_content = "both"
