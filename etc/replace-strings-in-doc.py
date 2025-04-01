import argparse
import re
from glob import glob

BOLD_TEXT = "\033[1m"
YELLOW = "\033[93m"
END_COLOUR = "\033[0m"

parser = argparse.ArgumentParser(
    prog="replace-strings-in-doc.py", description="Replaces strings in html"
)
parser.add_argument(
    "-c",
    "--check-unused",
    help="check whether all of the specified replacements are actually used",
    action="store_true",
)
args = parser.parse_args()

replacements = {
    "FroidurePinPBR": "FroidurePin",
    "ImageLeftActionPPerm1PPerm1": "ImageLeftAction",
    "ImageRightActionPPerm1PPerm1": "ImageRightAction",
    "KambitesMultiStringView": "Kambites",
    "KnuthBendixImplRewriteTrie": "KnuthBendix",
    "KnuthBendixStringRewriteTrie": "KnuthBendix",
    "PPerm1": "PPerm",
    "Perm1": "Perm",
    "PresentationStrings": "Presentation",
    "PresentationWords": "Presentation",
    "RightActionPPerm1List": "Action",
    "SchreierSimsPerm1": "SchreierSims",
    "ToddCoxeterImpl": "ToddCoxeter",
    "ToddCoxeterWord": "ToddCoxeter",
    "Transf1": "Transf",
    "_libsemigroups_pybind11.FroidurePinBase": "FroidurePinBase",
    "libsemigroups::BMat8": "BMat8",
    "libsemigroups::Presentation<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >": "Presentation",
    "libsemigroups_pybind11.bmat8": "bmat8",
    "libsemigroups_pybind11.knuth_bendix": "knuth_bendix",
    r"CongruenceWord": "Congruence",
    r"\bKoniecznyBMat8\b": "Konieczny",
    r"KoniecznyBMat8DClass": "Konieczny.DClass",
    r"SimsSettingsSims1": "SimsSettings",
    r"_libsemigroups_pybind11.": "",
    r"libsemigroups_pybind11\.": "",
    r"StephenPresentationWords": "Stephen",
}

html_glob = "docs/_build/html/**/*.html"
files = glob(html_glob, recursive=True)

actually_replaced = set()

print(BOLD_TEXT + "Making post-build string replacements..." + END_COLOUR)
for file in sorted(files):
    with open(file, "r") as f:
        content = f.read()
    num_matches = 0
    for expr, replacement in replacements.items():
        content, n = re.subn(expr, replacement, content)
        if n > 0:
            num_matches += n
            actually_replaced.add(expr)

    if num_matches > 0:
        print(f"String replacements made in {file}")
        with open(file, "w") as f:
            f.write(content)

if args.check_unused:
    number_unused = 0
    print(
        BOLD_TEXT + "Checking for unused string replacements..." + END_COLOUR,
        end=" ",
    )
    for bad_string, good_string in replacements.items():
        if bad_string not in actually_replaced:
            number_unused += 1
            print(
                YELLOW
                + f'WARNING: "{bad_string}" -> "{good_string}" [unused-replacement]'
                + END_COLOUR
            )
    if number_unused > 0:
        print(f"Please correct this in {__file__}")
    print("done")
