#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# pylint:disable=redefined-builtin, invalid-name, too-many-arguments, unbalanced-tuple-unpacking, unused-argument
"""
This provides configuration for the generation of the docs
"""

import re
import sphinx_rtd_theme
from sphinx.addnodes import desc_content, desc, index
from sphinx.ext.autodoc.directive import (
    AutodocDirective,
    DocumenterBridge,
    process_documenter_options,
    parse_generated_content,
)
from sphinx.util import logging
from sphinx.util.docutils import StringList

logger = logging.getLogger(__name__)


class ExtendedAutodocDirective(AutodocDirective):
    """A an extended directive class for all autodoc directives.

    It performs the same as AutodocDirective, with the additional ability to
    display only the docstring, everything but the docstring, and also formats
    overloaded functions nicely.
    """

    def fix_overloads(self, content):
        """Indent overloaded function documentation and format signatures"""
        overloading = False
        output = StringList(content)
        offset = 0  # How many additional lines we have added to output
        indent = "   "  # How much to indent overloaded functions by
        for i, line in enumerate(content):

            # Stop overloading when we see a new method
            if ".. py:method::" in line:
                overloading = False

            # Start overloading and capture the name of the overloaded function
            if "Overloaded function." in line:
                overloading = True
                m = re.match(r"\s+?\d. (.*?)\(", content[i + 2])
                overloaded_function = m.group(1)
                overload_counter = 1
                continue

            if overloading and line != "":
                if f"{overload_counter}. {overloaded_function}" in line:
                    # Capture the initial indent and the function signature
                    m = re.match(r"(\s+?)\d. (.*)", line)
                    parent_indent = m.group(1)

                    # Add adjusted content to the output
                    new_line = f"{parent_indent[:-3]}{indent}.. py:method:: {m.group(2)}"
                    output.data[i + offset] = new_line
                    output.insert(
                        i + offset + 1,
                        StringList([f"{parent_indent}{indent}:no-index:"]),
                    )
                    overload_counter += 1
                    offset += 1
                else:
                    output.data[i + offset] = indent + line
        return output

    def basic_run(self):
        """Generate and parse the docstring"""
        reporter = self.state.document.reporter

        try:
            source, lineno = reporter.get_source_and_line(self.lineno)
        except AttributeError:
            source, lineno = (None, None)
        logger.debug(
            "[autodoc] %s:%s: input:\n%s", source, lineno, self.block_text
        )

        # look up target Documenter
        objtype = self.name[4:]  # strip prefix (auto-).
        doccls = self.env.app.registry.documenters[objtype]

        # process the options with the selected documenter's option_spec
        try:
            documenter_options = process_documenter_options(
                doccls, self.config, self.options
            )
        except (KeyError, ValueError, TypeError) as exc:
            # an option is either unknown or has a wrong type
            logger.error(
                "An option to %s is either unknown or has an invalid value: %s",
                self.name,
                exc,
                location=(self.env.docname, lineno),
            )
            return []

        # generate the output
        params = DocumenterBridge(
            self.env, reporter, documenter_options, lineno, self.state
        )
        documenter = doccls(params, self.arguments[0])
        documenter.generate(more_content=self.content)
        if not params.result:
            return []

        logger.debug("[autodoc] output:\n%s", "\n".join(params.result))

        # record all filenames as dependencies -- this will at least
        # partially make automatic invalidation possible
        for fn in params.record_dependencies:
            self.state.document.settings.record_dependencies.add(fn)

        params.result = self.fix_overloads(params.result)

        result = parse_generated_content(self.state, params.result, documenter)
        return result

    def run(self):
        """Handle custom options and then generate parsed output"""

        if "doc-only" in self.options and "no-doc" in self.options:
            logger.warning("Cannot set both 'doc-only' and 'no-doc' options.")
            return []

        if "doc-only" in self.options:
            # delete option so Autodoc Directive doesn't complain
            del self.options["doc-only"]
            self.options["noindex"] = True
            return self.doc_only_run()

        if "no-doc" in self.options:
            # delete option so Autodoc Directive doesn't complain
            del self.options["no-doc"]
            return self.no_doc_run()

        return self.basic_run()

    def doc_only_run(self):
        """Format parsed text to contain only docstring only"""
        content = self.basic_run()

        if not content:
            return []

        node = content[0].parent

        # Find the docstring portion of the output
        docstring = list(node.findall(condition=desc_content))

        if not docstring:
            logger.warning(
                f"The docstring for {self.arguments[0]} cannot be found."
            )
            return []

        return docstring

    def no_doc_run(self):
        """Format parsed text to contain everything but docstring"""
        content = self.basic_run()

        if not content:
            return []

        node = content[0].parent

        description = node.next_node(condition=desc_content)

        # Remove any nodes before index or description, as this is where the
        # docstring is stored
        description_children = description.children.copy()
        for child in description_children:
            if isinstance(child, (index, desc)):
                break
            description.remove(child)
        return content


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
copyright = (
    "2021-2024, Joseph Edwards, James Mitchell, Maria Tsalakou, Murray Whyte"
)
author = "Joseph Edwards, James Mitchell, Maria Tsalakou, Murray Whyte"
version = "1.0.0"
release = "1.0.0"
language = "python"
exclude_patterns = ["_build", "_old"]
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
    """Add custom behaviour"""
    app.connect("autodoc-process-signature", change_sig)
    app.add_directive("autoclass", ExtendedAutodocDirective)
    app.add_directive("autofunction", ExtendedAutodocDirective)
