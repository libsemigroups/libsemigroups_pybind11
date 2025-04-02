#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# pylint:disable=redefined-builtin, invalid-name, too-many-arguments,
# pylint:disable=unbalanced-tuple-unpacking, unused-argument, too-many-locals
# pylint:disable=unused-import, too-many-positional-arguments
"""
This provides configuration for the generation of the docs
"""
import importlib.metadata
import sys
from pathlib import Path

# Allows sphinx to use our custom extension
sys.path.append(str(Path("_ext").resolve()))

extensions = [
    "sphinx.ext.autodoc",
    "sphinx.ext.autosummary",
    "sphinx.ext.doctest",
    "sphinx.ext.intersphinx",
    "sphinx.ext.mathjax",
    "sphinx_copybutton",
    "sphinxcontrib.bibtex",
    "libsemigroups_pybind11_extensions",
]

bibtex_bibfiles = ["libsemigroups.bib"]

autosummary_generate = True
add_module_names = True

templates_path = ["_templates"]
source_suffix = ".rst"
master_doc = "index"
project = "libsemigroups_pybind11"
copyright = (
    "2021-2025, Joseph Edwards, James Mitchell, Maria Tsalakou, Murray Whyte"
)
author = "Joseph Edwards, James Mitchell, Maria Tsalakou, Murray Whyte"
# Use the version number of the installed project
version = release = importlib.metadata.version(project)
language = "python"
exclude_patterns = ["_build", "_old"]
pygments_style = "sphinx"
highlight_language = "python"
todo_include_todos = False

html_theme = "sphinx_rtd_theme"
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
        "libsemigroups_pybind11 Documentation",
        [author],
        1,
    )
]

intersphinx_mapping = {
    "python": ("https://docs.python.org/3", None),
    "numpy": ("https://numpy.org/doc/stable/", None),
}

autodoc_default_options = {"show-inheritence": True}

autoclass_content = "both"

doctest_global_setup = """from libsemigroups_pybind11 import ReportGuard
ReportGuard(False)"""
