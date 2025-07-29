from bs4 import BeautifulSoup
from glob import iglob
import re
import sys

BOLD_TEXT = "\033[1m"
YELLOW = "\033[93m"
END_COLOUR = "\033[0m"

files = iglob("docs/_build/**/*.html", recursive=True)
param_with_type = re.compile(r"(\w+)(?: \((.+?)\))?")
warning_issued = False


def warn(message):
    """Print a yellow warning message, and record the fact a warning has been issued"""
    global warning_issued
    warning_issued = True
    print(YELLOW + f"WARNING: {message}" + END_COLOUR)


def extract_signature(func, func_name) -> tuple[dict[str, str], str]:
    """Extract the parameters and typehints from the signature of a function

    This function interrogates the signature of a function and returns:
        * a dictionary that maps parameters to their typehints
        * the return typehint
    """
    param_to_typehint = {}
    return_typehint = ""
    sig = func.find("dt", class_="sig sig-object py")
    for param in sig.find_all("em", class_="sig-param"):
        param_component = param.find_all("span", class_="n")
        if len(param_component) == 0 or len(param_component) > 2:
            warn(f"unexpected element in doc of {func_name}. Skipping . . .")
        elif len(param_component) == 1:
            param_to_typehint[param_component[0].get_text()] = ""
        else:
            param_to_typehint[param_component[0].get_text()] = param_component[1].get_text()
    return_block = sig.find("span", class_="sig-return-typehint")
    if return_block:
        return_typehint = return_block.get_text()
    return param_to_typehint, return_typehint


def extract_documented_signature(func, name) -> tuple[dict[str, str], str]:
    """Extract the parameters and typehints from the docstring of a function

    This function interrogates the docstring of a function and returns:
        * a dictionary that maps parameters to their typehints
        * the return typehint

    It also issues a warning if the field-list of function information (like parameters, return
    value, complexity, exceptions) is broken into multiple lists. This can happen if, for example,
    the description of a function appears between the documentation of some parameters, like:

    R"pbdoc(
    A short title

    :param p: a parameter that is being documented above the function description.

    A detailed description of the function.

    :param q: a parameter that is being documented below the function description.
    )
    """
    param_to_typehint = {}
    return_typehint = ""
    field_list = func.find("dl", class_="field-list simple")
    if not field_list:
        return param_to_typehint, return_typehint

    if field_list.find_next_sibling("dl", class_="field-list simple") is not None:
        warn(
            f"The function {name} has multiple field lists, but should have exactly one. "
            "Does the function description come after the documentation of a parameter? "
            "[multiple-field-lists]"
        )

    for heading in field_list.find_all("dt"):
        heading_text = heading.get_text()
        if "Parameters" in heading_text or "Keyword Arguments" in heading_text:
            param_lines = heading.find_next_sibling("dd")
            for param_line in param_lines.find_all("p"):
                m = re.search(param_with_type, param_line.get_text())
                if not m:
                    continue
                if len(m.groups()) > 2:
                    warn("unexpected element in doc. Skipping . . .")
                elif m[2] is None:
                    param_to_typehint[m[1]] = ""
                else:
                    param_to_typehint[m[1]] = m[2]
        elif "Return type" in heading_text:
            return_typehint = heading.find_next_sibling("dd").get_text().strip()

    return param_to_typehint, return_typehint


def compare_parameters(sig_params, doc_params, name):
    """Compares two dictionaries of parameters

    This function compares a dictionary of parameters from the signature with a dictionary of
    parameters from the docstring. Specifically, it checks that:
        * all of the parameters that appear in the signature (apart from "args", "kwargs" or "self)
          also appear in the docstring;
        * all of the parameters that appear in the docstring also appear in the signature; and
        * all of the parameters in the docstring have the same typehint as their counterpart in the
          signature.

    An issue is warned if any of the above fail.
    """
    for sig_param in sig_params:
        if sig_param in ["args", "kwargs", "self"]:
            continue
        if sig_param not in doc_params:
            warn(
                f"The parameter '{sig_param}' of {name} appears in the signature, but does not "
                "appear in the docstring [undocumented-parameter]"
            )
        elif sig_params[sig_param] != doc_params[sig_param]:
            warn(
                f"The parameter '{sig_param}' of {name} has the typehint '{sig_params[sig_param]}' "
                f"in the signature, but the typehint '{doc_params[sig_param]}' in the docstring "
                "[mismatched-typehints]"
            )
    for doc_param in doc_params:
        if doc_param not in sig_params:
            warn(
                f"The parameter '{doc_param}' of {name} appears in the docstring, but does not "
                "appear in the signature [missing-parameter]"
            )


def compare_return_types(sig_return_type, doc_return_type, name):
    """Compares two return typehints

    This function compares the return typehint from the signature with the return typehint from the
    docstring. The type "None" and the absence of a type are treated as the same.

    An issue is warned if typehints are not the same.
    """
    none_types = ["None", ""]
    if sig_return_type in none_types and doc_return_type in none_types:
        return
    if sig_return_type != doc_return_type:
        warn(
            f"The return type of {name} has the typehint '{sig_return_type}' in the signature, but "
            f"the typehint '{doc_return_type}' in the docstring [mismatched-return-types]"
        )


def process_file(filename):
    """Check all the functions in a file have consistent signatures and docstrings"""
    with open(filename) as f:
        content = f.read()

    soup = BeautifulSoup(content, "html.parser")
    for func in soup.find_all("dl", class_=lambda x: x in {"py function", "py method"}):
        first_segment = func.find("p")
        if first_segment and "Overloaded function" in first_segment.get_text():
            continue

        sig_block = func.find("dt")
        while "id" not in sig_block.attrs:
            sig_block = sig_block.parent.find_parent("dl").find("dt")
        func_name = sig_block["id"]

        sig_params, sig_return_typehint = extract_signature(func, func_name)
        doc_params, doc_return_typehint = extract_documented_signature(func, func_name)

        compare_parameters(sig_params, doc_params, func_name)
        compare_return_types(sig_return_typehint, doc_return_typehint, func_name)


print(BOLD_TEXT + "Checking parameter consistency..." + END_COLOUR)
for filename in files:
    process_file(filename)
print(BOLD_TEXT + "Done" + END_COLOUR)

if warning_issued and any("-W" in arg for arg in sys.argv):
    sys.exit(1)
