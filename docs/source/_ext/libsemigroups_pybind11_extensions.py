# pylint:disable=too-many-arguments, unused-argument, too-many-locals
# pylint:disable=too-many-positional-arguments
"""Custom functions and class for building the docs of libsemigroups_pybind11"""

import re

from inspect import getdoc

from sphinx.addnodes import desc_content, desc, index
from sphinx.ext.autodoc.directive import AutodocDirective
from sphinx.util import logging
from sphinx.util.display import progress_message

BOLD_TEXT = "\033[1m"
YELLOW = "\033[93m"
END_COLOUR = "\033[0m"

################################################################################
# Custom directive
################################################################################

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
            logger.warning(f"The docstring for {self.arguments[0]} cannot be found.")
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


################################################################################
# String replacement
################################################################################

# This set is used to check which of the strings defined below actually get
# replaced when the doc is built. It should be left empty.
strings_replaced = set()

# This dictionary should be of the form "bad type" -> "good type", and
# replacements will be performed globally. Hyperlinks will be added in the
# signature if "good type" is a valid (potentially user defined) python type
type_replacements = {
    r"_?libsemigroups_pybind11\.": "",
}

# This dictionary should be of the form class_name -> (pattern, repl), where
# "pattern" should be replaced by "repl" in the signature of all functions in
# "class_name"
class_specific_replacements = {
    "Sims1": [("SimsSettingsSims1", "Sims1")],
    "Sims2": [("SimsSettingsSims2", "Sims2")],
    "MinimalRepOrc": [
        ("SimsSettingsMinimalRepOrc", "MinimalRepOrc"),
    ],
    "RepOrc": [
        ("SimsSettingsRepOrc", "RepOrc"),
    ],
}

# This dictionary should be of the form bad_string -> good_string. These
# replacements will be made in each docstring, and will be useful for removing
# things like the signatures that sphinx inserts into every docstring
docstring_replacements = {}


# This is what sphinx considers to be a signature
custom_signature_re = re.compile(
    r""":sig=([\w.]+::)?            # explicit module name
          ([\w.]+\.)?               # module and/or class name(s)
          (?:(\w+)  \s*)?           # thing name
          (?: \[\s*(.*)\s*\])?      # type parameters list
          (?: \((.*)\))?            # arguments
          (?:\s* -> \s* (.*))?:""",  # return annotation
    re.VERBOSE,
)

inserted_signature_re = re.compile(
    r"""^([\w.]+::)?               # explicit module name
          ([\w.]+\.)?              # module and/or class name(s)
          (?:(\w+)  \s*)?            # thing name
          (?: \[\s*(.*)\s*\])?       # type parameters list
          (?: \((.*)\))?             # arguments
          (?:\s* -> \s* (.*))?$""",  # return annotation
    re.VERBOSE | re.MULTILINE,
)


def sub_if_not_none(pattern, repl, *strings):
    """Make regex replacement on inputs that are not None"""
    out = []
    for string in strings:
        if string is None:
            out.append(string)
        else:
            new_string, n = re.subn(pattern, repl, string)
            out.append(new_string)
            if n > 0:
                strings_replaced.add(pattern)
    if len(out) == 1:
        return out[0]
    return out


def sig_alternative(doc, signature, return_annotation):
    """Find an alternative signature defined in the docstring

    If there is no signature specified using :sig=...:, then no changes occur.
    If multiple different signatures are specified using :sig=...:, then the
    signature is set to (*args, **kwargs). Otherwise, the signature is set to
    the unique signature specified using :sig=...:.
    """
    if not doc:
        return signature, return_annotation

    m = set(re.findall(custom_signature_re, doc))
    if len(m) == 0:
        return signature, return_annotation
    if len(m) > 1:
        return "(*args, **kwargs)", ""

    _, _, _, _, args, return_annotation = m.pop()
    new_sig = f"({args})"
    return new_sig, return_annotation


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
    if what == "class":
        return None, None

    signature, return_annotation = sig_alternative(getdoc(obj), signature, return_annotation)
    for class_name, repl_pairs in class_specific_replacements.items():
        if class_name in name:
            for find, repl in repl_pairs:
                signature, return_annotation = sub_if_not_none(
                    find, repl, signature, return_annotation
                )

    for typename, repl in type_replacements.items():
        signature, return_annotation = sub_if_not_none(typename, repl, signature, return_annotation)
    return signature, return_annotation


def make_only_doc(lines):
    """
    Extract the unique docstrings from a sequence of overloaded functions.
    """

    # To keep track of unique strings
    sigs = set()

    # To track how many overloaded functions there should be
    overload_counter = 1

    # To track whether or not to delete overloads
    deleting = False

    # To track if we actually have any repeated documentation
    called_correctly = False
    i = 0
    while i < len(lines):
        line = lines[i]
        m = re.search(r":sig=(.*):$", line)
        if m is not None:
            sig = m.group(1)
            if sig in sigs:
                deleting = True
                called_correctly = True
            else:
                deleting = False
                sigs.add(sig)
                lines[i - 3] = re.sub(
                    r"\d+(\. .*)\(.*$",
                    str(overload_counter) + r"\1" + sig,
                    lines[i - 3],
                )
                overload_counter += 1

        if deleting:
            # We do i - 3 because the :sig=...: appears three lines later than
            # the actual signatures
            del lines[i - 3]
        else:
            i += 1

    # If we were deleting when we got to the end of lines, we should delete the
    # last 3 lines that got missed.
    if deleting:
        del lines[-3:]

    if not called_correctly:
        print(
            "\033[93m:only-document-once: has been invoked in a function where "
            "documentation has not been repeated. Invoked in:\n" + "\n\033[0m".join(lines)
        )
        return

    # If the new doc shouldn't be overloaded, remove the "Overloaded
    # function" part
    if len(sigs) == 1:
        while ":sig=" not in lines[0]:
            del lines[0]


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
    overloaded_function = ""
    new_name = ""
    input_text = list(lines)
    offset = 0  # How many additional lines we have added to output
    indent = "   "  # How much to indent overloaded functions by
    directive = f".. py:{what}::"

    for i, line in enumerate(input_text):
        if line == "":
            continue

        # Start overloading and capture the name of the overloaded function
        if "Overloaded function." in line:
            overloading = True
            try:
                m = re.search(r"\s*?\d+\. (.*?)\(", input_text[i + 2])
                if not m:
                    return
            except IndexError:
                return
            overloaded_function = m.group(1)
            new_name = re.sub(r"^.*\.", "", name)
            overload_counter = 1
            continue

        if overloading:
            if f"{overload_counter}. {overloaded_function}" in line:
                # Capture the initial indent and the function signature
                new_sig = False
                if i + 3 < len(input_text):
                    m = re.match(custom_signature_re, input_text[i + 3])
                    if m is not None:
                        new_sig = True
                        _, _, _, _, args, return_annotation = m.groups()
                        signature = f"({args})"
                m = re.match(r"(\s*?)\d+\. .*(\(.*\))(?:\s*->\s*(.*))?", line)
                parent_indent = m.group(1)
                if not new_sig:
                    signature = m.group(2)
                    return_annotation = m.group(3)
                # Make replacements in signature
                signature, return_annotation = change_sig(
                    name=name,
                    signature=signature,
                    return_annotation=return_annotation,
                )

                # Add adjusted content to the output
                new_line = (
                    f"{parent_indent}{indent[:-3]}{directive} "
                    f"{new_name}{signature} -> {return_annotation}"
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


def remove_doc_annotations(app, what, name, obj, options, lines):
    """Remove any special decorations from the documentation"""
    if len(lines) == 0:
        return

    # Remove inserted signatures if they have the wrong name
    m = re.match(inserted_signature_re, lines[0])
    if m:
        specified_name = m[3]
        short_name = name.split(".")[-1]
        if short_name != specified_name:
            del lines[0]

    for i in range(len(lines) - 1, -1, -1):
        for bad, good in docstring_replacements.items():
            lines[i], n = re.subn(bad, good, lines[i])
            if n > 0:
                strings_replaced.add(bad)
        if ":only-document-once:" in lines[i] or ":sig=" in lines[i] or ":ret=" in lines[i]:
            del lines[i]


@progress_message("Checking for unused string replacements")
def check_string_replacements(app, env):
    """Check which string replacements actually get used.

    The checks are only performed when a fresh environment is used to build the
    documentation, to avoid false negatives.
    """
    if not app.fresh_env_used:
        return

    # Figure out how many replacements should have been made, and only complain
    # if the actual number is different
    maximum_number_of_replacements = (
        len(type_replacements)
        + len(
            [pattern for patterns in class_specific_replacements.values() for pattern in patterns]
        )
        + len(docstring_replacements)
    )
    if len(strings_replaced) == maximum_number_of_replacements:
        return

    # Check which replacements were not used
    any_warnings = False
    for bad_type, good_type in type_replacements.items():
        if bad_type not in strings_replaced:
            any_warnings = True
            logger.warning(
                f'"{bad_type}" -> "{good_type}"',
                type="unused-replacement",
            )

    for class_name, repls in class_specific_replacements.items():
        for pattern, repl in repls:
            if pattern not in strings_replaced:
                any_warnings = True
                logger.warning(
                    f'"{pattern}" -> "{repl}" in {class_name}',
                    type="unused-replacement",
                )
    for bad_string, good_string in docstring_replacements.items():
        if bad_string not in strings_replaced:
            any_warnings = True
            logger.warning(
                f'"{bad_string}" -> "{good_string}"',
                type="unused-replacement",
            )
    if any_warnings:
        logger.info(f"Please correct this in {__file__}")


def document_class(app, what, name, obj, options, lines):
    """Document a class using its __init__ function

    This function sets the docstring of a class to be the __init__ function
    (including the signature), rather than just the docstring of the __init__
    function.
    """
    if what != "class":
        return
    try:
        if options["class-doc-from"] != "init":
            return

        init_doc = getdoc(obj.__init__)

        if (
            not init_doc
            or "Initialize self.  See help(type(self)) for accurate signature." in init_doc
        ):
            lines[:] = []
        else:
            lines[:] = [f".. autofunction:: {name}.__init__\n"]
    except KeyError:
        return


commands = {
    r":param \w+:": 1,
    r":type \w+:": 1,
    r":returns:": 2,
    r":rtype:": 3,
    r":raises \w+:": 4,
    r":complexity:": 5,
    r"\.\. note": 6,
    r"\.\. warning": 7,
    r"\.\. seealso": 8,
    r"\.\. doctest": 9,
}
COMMANDS_WRONG_ORDER = False
CORRECT_ORDER = """1. :param: or :type:
2. :returns:
3. :rtype:
4. :raises:
5. :complexity: 
6. .. note:: 
7. .. warning::
8. .. seealso::
9. .. doctest::
"""


def check_order(app, what, name, obj, options, lines):
    """Check that the sections of the doc are in the correct order."""
    # pylint: disable=global-statement
    global COMMANDS_WRONG_ORDER
    highest_level = 0
    for i, line in enumerate(lines):
        if re.search(r"^\s*\d+\. ", line):
            highest_level = 0
            continue

        for command, level in commands.items():
            baddness = re.search(command, line)
            if not baddness:
                continue
            if level >= highest_level:
                highest_level = level
                break
            logger.warning(
                f"{baddness[0]} is not in the correct place in docstring line {i + 1} of {name}."
            )
            COMMANDS_WRONG_ORDER = True


def print_command_order_info(app, env):
    """If the sections of the doc are in the wrong order, print the correct
    order
    """
    if COMMANDS_WRONG_ORDER:
        logger.warning(f"The correct ordering is:\n{CORRECT_ORDER}")


def setup(app):
    """Add custom behaviour to the build process"""
    app.add_directive("autoclass", ExtendedAutodocDirective, override=True)
    app.add_directive("autofunction", ExtendedAutodocDirective, override=True)
    app.add_directive("automodule", ExtendedAutodocDirective, override=True)
    app.connect("autodoc-process-docstring", check_order)
    app.connect("autodoc-process-docstring", document_class)
    app.connect("autodoc-process-docstring", only_doc_once)
    app.connect("autodoc-process-docstring", fix_overloads)
    app.connect("autodoc-process-signature", change_sig)
    app.connect("autodoc-process-docstring", remove_doc_annotations)
    app.connect("env-updated", print_command_order_info)
    app.connect("env-check-consistency", check_string_replacements)
