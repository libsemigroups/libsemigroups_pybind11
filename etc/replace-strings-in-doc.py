import re
from glob import glob

BOLD_TEXT = "\033[1m"
YELLOW = "\033[93m"
END_COLOUR = "\033[0m"

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
}

html_glob = "docs/_build/html/**/*.html"
files = glob(html_glob, recursive=True)


print(BOLD_TEXT + "Making post-build string replacements..." + END_COLOUR)
for file in sorted(files):
    with open(file, "r") as f:
        content = f.read()
    num_matches = 0
    for expr, replacement in replacements.items():
        content, n = re.subn(expr, replacement, content)
        if n > 0:
            num_matches += n

    if num_matches > 0:
        print(f"String replacements made in {file}")
        with open(file, "w") as f:
            f.write(content)
