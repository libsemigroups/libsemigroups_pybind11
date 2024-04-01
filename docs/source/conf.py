#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import subprocess
import sphinx_rtd_theme
import re


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
project = "libsemigroups_pybind11"
copyright = "2021-2024, J. D. Mitchell + Maria Tsalakou"
author = "J. D. Mitchell + Maria Tsalakou"
version = "0.10.1"
release = "0.10.1"
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
        "libsemigroups_pybind11 Documentation",
        [author],
        1,
    )
]

intersphinx_mapping = {"python": ("https://docs.python.org/", None)}

autoclass_content = "both"

type_replacements = {
    r"_libsemigroups_pybind11.KnuthBendixRewriteTrie": r"KnuthBendix",
    r"libsemigroups::Presentation<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >": r"Presentation",
}
# to_replace = {"method", "function", "class"}


def change_doc(app, what, name, obj, options, lines):
    # if what in to_replace:
    for i, line in enumerate(lines):
        changes = 0
        for typename, repl in type_replacements.items():
            line, n = re.subn(typename, repl, line)
            changes += n
        if changes > 0:
            lines[i] = line


def change_sig(app, what, name, obj, options, signature, return_annotation):
    # if what in to_replace:
    for typename, repl in type_replacements.items():
        if signature is not None:
            signature = re.sub(typename, repl, signature)
        if return_annotation is not None:
            return_annotation = re.sub(typename, repl, return_annotation)
    return signature, return_annotation


def setup(app):
    app.connect("autodoc-process-docstring", change_doc)
    app.connect("autodoc-process-signature", change_sig)
