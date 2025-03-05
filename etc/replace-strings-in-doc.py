import re
import os
from os import listdir
from os.path import isfile, splitext


def dict_sub(replacements, string):
    """replacements has the form {"regex1": "replacement", "regex2": "replacement2", ...}"""
    global_expression = re.compile(
        "|".join("(" + x + ")" for x in replacements)
    )
    replacements_by_group = {}
    group = 1
    for expr, replacement in replacements.items():
        replacements_by_group[group] = replacement
        group += re.compile(expr).groups + 1

    def choose(match):
        return replacements_by_group[match.lastindex]

    return re.subn(global_expression, choose, string)


def all_html_files(start):
    files = set()

    def dive(path):
        for entry in listdir(path):
            candidate = os.path.join(path, entry)
            if isfile(candidate) and splitext(entry)[1] == ".html":
                files.add(candidate)
            elif not isfile(candidate):
                dive(candidate)

    dive(start)
    return files


html_path = "docs/_build/html"
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
    r"_libsemigroups_pybind11.": "",
    r"libsemigroups_pybind11\.": "",
    r"SimsSettingsSims1": "SimsSettings",
}
files = all_html_files(html_path)

print("Making post-build string replacements . . .")
for file in files:
    with open(file, "r") as f:
        content = f.read()

    output, num_matches = dict_sub(replacements, content)
    if num_matches > 0:
        print(f"String replacements made in {file}")
        with open(file, "w") as f:
            f.write(output)
