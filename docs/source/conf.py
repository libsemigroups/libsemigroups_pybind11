#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import subprocess
import sphinx_rtd_theme
import re
from docutils import nodes
from docutils.statemachine import StringList
from sphinx.util.docutils import SphinxDirective
from sphinx.ext.autodoc import ClassDocumenter, Options
from sphinx.ext.autodoc.directive import DocumenterBridge

# Custom Directive


class ClassDescription(SphinxDirective):
    required_arguments = 1

    def run(self):
        reporter = self.state.document.reporter

        # generate the output
        params = DocumenterBridge(
            self.env,
            reporter,
            Options(),
            self.lineno,
            self.state,
        )
        documenter = ClassDocumenter(params, self.arguments[0])
        documenter.generate()

        # record all filenames as dependencies -- this will at least
        # partially make automatic invalidation possible
        for fn in params.record_dependencies:
            self.state.document.settings.record_dependencies.add(fn)

        node = nodes.paragraph()
        node.document = self.state.document
        params.result = StringList(documenter.get_doc()[0])
        self.state.nested_parse(params.result, 0, node)
        return node.children


extensions = [
    "sphinx.ext.autodoc",
    "sphinx.ext.autosummary",
    "sphinx.ext.doctest",
    "sphinx.ext.intersphinx",
    "sphinx.ext.mathjax",
    "sphinx.ext.napoleon",
    "sphinx_copybutton",
    "sphinxcontrib.bibtex",
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

autodoc_default_options = {"show-inheritence": True}

autoclass_content = "both"

# This dictionary should be of the form "bad type" -> "good type", and
# replacements will be performed globally. Hyperlinks will be added in the
# signature if "good type" is a valid (potentially user defined) python type
type_replacements = {
    r"libsemigroups::Presentation<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >": r"Presentation",
    r"libsemigroups::BMat8": r"BMat8",
    r"libsemigroups::WordGraph<unsigned int>": r"WordGraph",
    r"libsemigroups::Gabow<unsigned int>": r"Gabow",
}

# This dictionary should be of the form class_name -> (pattern, repl), where
# "pattern" should be replaced by "repl" in the signature of all functions in
# "class_name"
class_specific_replacements = {"RowActionBMat8": (r"\bBMat8\b", "Element")}


def sub_if_not_none(pattern, repl, *strings):
    out = []
    for string in strings:
        if string is None:
            out.append(string)
        else:
            out.append(re.sub(pattern, repl, string))
    return out


def change_sig(app, what, name, obj, options, signature, return_annotation):
    # if what in to_replace:
    for typename, repl in type_replacements.items():
        signature, return_annotation = sub_if_not_none(
            typename, repl, signature, return_annotation
        )

    for class_name, repl_pair in class_specific_replacements.items():
        if class_name in name:
            find, repl = repl_pair
            signature, return_annotation = sub_if_not_none(
                find, repl, signature, return_annotation
            )
    return signature, return_annotation


def setup(app):
    app.connect("autodoc-process-signature", change_sig)
    app.add_directive("classdoc", ClassDescription)
