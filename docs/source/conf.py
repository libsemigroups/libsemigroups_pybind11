#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# pylint:disable=redefined-builtin, invalid-name, too-many-arguments
# pylint:disable=unbalanced-tuple-unpacking, unused-argument, too-many-locals
"""
This provides configuration for the generation of the docs
"""

import re
from collections import defaultdict
import sphinx_rtd_theme
from sphinx.addnodes import desc_content, desc, index
from sphinx.ext.autodoc.directive import AutodocDirective
from sphinx.util import logging

logger = logging.getLogger(__name__)


class ExtendedAutodocDirective(AutodocDirective):
    """An extended directive class for all autodoc directives.

    It performs the same as AutodocDirective, with the additional ability to
    display only the docstring, everything but the docstring
    """

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

        return super().run()

    def doc_only_run(self):
        """Format parsed text to contain only docstring only"""
        content = super().run()

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
        content = super().run()

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
add_module_names = True

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
    (
        r"libsemigroups::Presentation<std::__cxx11::basic_string<char, "
        r"std::char_traits<char>, std::allocator<char> > >"
    ): r"Presentation",
    r"libsemigroups::BMat8": r"BMat8",
    r"libsemigroups::WordGraph<unsigned int>": r"WordGraph",
    r"libsemigroups::Gabow<unsigned int>": r"Gabow",
}

# This dictionary should be of the form class_name -> (pattern, repl), where
# "pattern" should be replaced by "repl" in the signature of all functions in
# "class_name"
class_specific_replacements = {
    "RightActionPPerm16List": [
        ("libsemigroups::PPerm<16ul, unsigned char>", "Element"),
        ("libsemigroups::Element", "Element"),
    ],
    "StaticTransf16": [
        ("PTransfBase16", "StaticTransf16"),
        (
            "class_<libsemigroups::Transf<16ul, unsigned char>, "
            "libsemigroups::PTransfBase<unsigned char, "
            "std::__1::array<unsigned char, 16ul>>>",
            "StaticTransf16",
        ),
        (
            "class_<libsemigroups::Transf<16ul, unsigned char>, "
            "libsemigroups::PTransfBase<unsigned char, "
            "std::array<unsigned char, 16ul> > >",
            "StaticTransf16",
        ),
    ],
    "StaticPPerm16": [
        ("PTransfBase16", "StaticPPerm16"),
        (
            "class_<libsemigroups::PPerm<16ul, unsigned char>, "
            "libsemigroups::PTransfBase<unsigned char, "
            "std::__1::array<unsigned char, 16ul>>>",
            "StaticPPerm16",
        ),
        (
            "class_<libsemigroups::PPerm<16ul, unsigned char>, "
            "libsemigroups::PTransfBase<unsigned char, "
            "std::array<unsigned char, 16ul> > >",
            "StaticPPerm16",
        ),
    ],
    "StaticPerm16": [
        (
            "class_<libsemigroups::Perm<16ul, unsigned char>, "
            "libsemigroups::Transf<16ul, unsigned char>>",
            "StaticPerm16",
        ),
        (
            "class_<libsemigroups::Perm<16ul, unsigned char>, "
            "libsemigroups::Transf<16ul, unsigned char> >",
            "StaticPerm16",
        ),
        ("PTransfBase16", "StaticPerm16"),
        ("Transf", "StaticPerm16"),
    ],
}


def sub_if_not_none(pattern, repl, *strings):
    """Make regex replacement on inputs that are not None"""
    out = []
    for string in strings:
        if string is None:
            out.append(string)
        else:
            out.append(re.sub(pattern, repl, string))
    if len(out) == 1:
        return out[0]
    return out


def sig_alternative(doc, signature):
    """Find an alternative signature defined in the docstring

    Note that this is only intended to be used when :only-document-once: is set.
    If there is not exactly one signature set using :sig=...:, then no changes
    occur.
    """
    if not doc:
        return signature

    m = set(re.findall(r":sig=(.*):\n", doc))
    if len(m) != 1:
        return signature

    new_sig = m.pop()
    new_sig = re.sub(r"\\:", ":", new_sig)
    return new_sig


def change_sig(
    app=None,
    what=None,
    name=None,
    obj=None,
    options=None,
    signature=None,
    return_annotation=None,
):
    """Make type replacement in function signatures"""
    signature = sig_alternative(obj.__doc__, signature)
    for class_name, repl_pairs in class_specific_replacements.items():
        if class_name in name:
            for find, repl in repl_pairs:
                signature, return_annotation = sub_if_not_none(
                    find, repl, signature, return_annotation
                )

    for typename, repl in type_replacements.items():
        signature, return_annotation = sub_if_not_none(
            typename, repl, signature, return_annotation
        )
    return signature, return_annotation


def make_only_doc(lines):
    """
    Extract the unique docstrings from a sequence of overloaded functions.

    This function assumes that the the first n overloaded functions are unique,
    and that they then repeat periodically.
    """

    # Find the lines the signatures occur in
    sigs = defaultdict(list)
    lines_to_sig = {}
    for i, line in enumerate(lines):
        m = re.search(r":sig=(.*):$", line)
        if m is not None:
            sig = m.group(1)
            sigs[sig] += [i]
            lines_to_sig[i] = sig
            current = sigs[sig]
            if len(current) > 1:
                break

    locations = list(sigs.values())
    locations.sort()

    if len(locations) == 0 or len(locations[0]) <= 1:
        raise RuntimeError(
            ":only-document-once: has been invoked in a function where "
            "documentation has not been repeated. Invoked in:\n"
            + "\n".join(lines)
        )

    # Find the period of repetition, and remove all lines after the end of the
    # first period
    start = locations[0][0]
    end = locations[0][1]
    del lines[end - 3 :]

    # If the new doc shouldn't be overloaded, remove the "Overloaded
    # function" part
    if len(sigs) == 1:
        del lines[: start + 2]
        return

    # Otherwise, replace the signature in the correct place
    for line_sequence in locations[::-1]:
        first = line_sequence[0]
        decl_line = first - 3
        lines[decl_line] = re.sub(
            r"(\s*?\d+\. .*)\(.*$",
            r"\1" + lines_to_sig[first],
            lines[decl_line],
        )
        del lines[first : first + 2]


def only_doc_once(app, what, name, obj, options, lines):
    """
    Edit docstring to only include one version of the doc for an overloaded
    function if necessary
    """

    if any(":only-document-once:" in line for line in lines):
        make_only_doc(lines)


def fix_overloads(app, what, name, obj, options, lines):
    """Indent overloaded function documentation and format signatures"""
    overloading = False
    input = list(lines)
    offset = 0  # How many additional lines we have added to output
    indent = "   "  # How much to indent overloaded functions by
    directive = f".. py:{what}::"

    for i, line in enumerate(input):
        if line == "":
            continue

        # Start overloading and capture the name of the overloaded function
        if "Overloaded function." in line:
            overloading = True
            m = re.search(r"\s*?\d+\. (.*?)\(", input[i + 2])
            overloaded_function = m.group(1)
            overload_counter = 1
            continue

        if overloading:
            if f"{overload_counter}. {overloaded_function}" in line:
                # Capture the initial indent and the function signature
                m = re.match(r"(\s*?)\d+\. (.*)", line)
                parent_indent = m.group(1)
                # Make replacements in signature
                signature = change_sig(name=name, signature=m.group(2))[0]

                # Add adjusted content to the output
                new_line = (
                    f"{parent_indent}{indent[:-3]}{directive} {signature}"
                )
                lines[i + offset] = new_line
                lines.insert(
                    i + offset + 1,
                    f"{parent_indent}{indent}:noindex:",
                )
                overload_counter += 1
                offset += 1
            else:
                lines[i + offset] = indent + line


def setup(app):
    """Add custom behaviour"""
    app.add_directive("autoclass", ExtendedAutodocDirective, override=True)
    app.add_directive("autofunction", ExtendedAutodocDirective, override=True)
    app.add_directive("automodule", ExtendedAutodocDirective, override=True)
    app.connect("autodoc-process-docstring", only_doc_once)
    app.connect("autodoc-process-docstring", fix_overloads)
    app.connect("autodoc-process-signature", change_sig)
