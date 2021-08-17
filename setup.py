import json
import os

print(json.dumps(dict(os.environ), sort_keys=True, indent=4))

from packaging import version
import pkgconfig
import re
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import sys
import setuptools

__version__ = "0.0.0"


def minimum_libsemigroups_version():
    return "1.3.3"


def compare_version_numbers(supplied, required):
    "Returns True if supplied >= required"

    if isinstance(supplied, str) and isinstance(required, str):
        return version.parse(supplied) >= version.parse(required)
    else:
        raise TypeError("expected a string, got a " + vers.__name__)


def libsemigroups_version():
    # the try-except is require pkgconfig v1.5.0 which is very recent, and
    # hence not on conda at time of writing.
    try:
        v = pkgconfig.modversion("libsemigroups")
    except AttributeError:
        # this is just the guts of the modversion method in pkgconfig v1.5.1
        v = pkgconfig.pkgconfig._query("libsemigroups", "--modversion")
    if re.search("\d+\.\d+\.\d+-\d+-\w{7}", v):
        # i.e. supplied is of the form: 1.1.0-6-g8b04c08
        v = re.search("\d+\.\d\.+\d+-\d+", v).group(0)
    return v


if "PKG_CONFIG_PATH" not in os.environ:
    os.environ["PKG_CONFIG_PATH"] = ""

pkg_config_path = os.environ["PKG_CONFIG_PATH"].split(":")

if "CONDA_PREFIX" in os.environ:
    conda_env_pkg_config = os.path.join(
        os.environ["CONDA_PREFIX"], "lib", "pkgconfig"
    )
    if (
        os.path.exists(conda_env_pkg_config)
        and not conda_env_pkg_config in pkg_config_path
    ):
        os.environ["PKG_CONFIG_PATH"] += ":" + conda_env_pkg_config

if "CONDA_DEFAULT_ENV" in os.environ and "CONDA_ENVS_PATH" in os.environ:
    conda_env_pkg_config = os.path.join(
        os.environ["CONDA_ENVS_PATH"],
        os.environ["CONDA_DEFAULT_ENV"],
        "lib",
        "pkgconfig",
    )
    if (
        os.path.exists(conda_env_pkg_config)
        and not conda_env_pkg_config in pkg_config_path
    ):
        os.environ["PKG_CONFIG_PATH"] += ":" + conda_env_pkg_config

if "/usr/local/lib/pkgconfig" not in pkg_config_path:
    if (
        "PKG_CONFIG_PATH" in os.environ
        and not len(os.environ["PKG_CONFIG_PATH"]) == 0
    ):
        os.environ["PKG_CONFIG_PATH"] += ":/usr/local/lib/pkgconfig"
    else:
        os.environ["PKG_CONFIG_PATH"] = "/usr/local/lib/pkgconfig"

if not pkgconfig.exists("libsemigroups"):
    print(json.dumps(dict(os.environ), sort_keys=True, indent=4))
    raise ImportError("cannot locate libsemigroups library")
elif not compare_version_numbers(
    libsemigroups_version(), minimum_libsemigroups_version()
):
    raise ImportError(
        "libsemigroups version at least {0} is required, found {1}".format(
            minimum_libsemigroups_version(), libsemigroups_version()
        )
    )


class get_pybind_include(object):
    """Helper class to determine the pybind11 include path

    The purpose of this class is to postpone importing pybind11
    until it is actually installed, so that the ``get_include()``
    method can be invoked."""

    def __init__(self, user=False):
        self.user = user

    def __str__(self):
        import pybind11

        return pybind11.get_include(self.user)


library_path = pkgconfig.pkgconfig._query("libsemigroups", "--libs-only-L")
assert (
    library_path[:2] == "-L"
), "The first two characters of the library path to the libsemigroups.so etc should be '-L'"

# Try to use pkg-config to add the path to libsemigroups.so etc to
# LD_LIBRARY_PATH so that cppyy can find it. We only try to do this, and ignore
# it if it fails, because it's sometime possible to cppyy.load_library even
# though the path to libsemigroups.so etc is not in LD_LIBRARY_PATH. This is
# the case, for example, on JDM's computer.

library_path_no_L = library_path[:2]
if os.path.exists(library_path_no_L):
    if (
        "LD_LIBRARY_PATH" in os.environ
        and len(os.environ["LD_LIBRARY_PATH"]) != 0
    ):
        LD_LIBRARY_PATH = os.environ["LD_LIBRARY_PATH"]
        if LD_LIBRARY_PATH.find(library_path_no_L) == -1:
            prefix = "" if LD_LIBRARY_PATH[-1] == ":" else ":"
            os.environ["LD_LIBRARY_PATH"] += prefix + library_path_no_L
    else:
        os.environ["LD_LIBRARY_PATH"] = library_path_no_L

include_path = [
    get_pybind_include(),
    get_pybind_include(user=True),
    "/usr/local/include",
    "/usr/local/include/libsemigroups",
]

if "CONDA_PREFIX" in os.environ:
    include_path.append(
        os.path.join(os.environ["CONDA_PREFIX"], "include", "eigen3")
    )

if "CONDA_DEFAULT_ENV" in os.environ and "CONDA_ENVS_PATH" in os.environ:
    include_path.append(
        os.path.join(
            os.environ["CONDA_ENVS_PATH"],
            os.environ["CONDA_DEFAULT_ENV"],
            "include",
            "eigen3",
        )
    )

ext_modules = [
    Extension(
        "libsemigroups_pybind11",
        [
            "src/action-digraph.cpp",
            "src/bmat8.cpp",
            "src/cong.cpp",
            "src/fpsemi.cpp",
            "src/knuth-bendix.cpp",
            "src/main.cpp",
            "src/todd-coxeter.cpp",
            "src/words.cpp",
        ],
        include_dirs=include_path,
        language="c++",
        libraries=["semigroups"],
        extra_link_args=[library_path, "-L/usr/local/lib"],
    ),
]


# As of Python 3.6, CCompiler has a `has_flag` method.
# cf http://bugs.python.org/issue26689
def has_flag(compiler, flagname):
    """Return a boolean indicating whether a flag name is supported on
    the specified compiler.
    """
    import tempfile

    with tempfile.NamedTemporaryFile("w", suffix=".cpp") as f:
        f.write("int main (int argc, char **argv) { return 0; }")
        try:
            compiler.compile([f.name], extra_postargs=[flagname])
        except setuptools.distutils.errors.CompileError:
            return False
    return True


def cpp_flag(compiler):
    """Return the -std=c++[11/14] compiler flag.

    The c++14 is prefered over c++11 (when it is available).
    """
    if has_flag(compiler, "-std=c++14"):
        return "-std=c++14"
    elif has_flag(compiler, "-std=c++11"):
        return "-std=c++11"
    else:
        raise RuntimeError(
            "Unsupported compiler -- at least C++11 support " "is needed!"
        )


class BuildExt(build_ext):
    """A custom build extension for adding compiler-specific options."""

    c_opts = {
        "msvc": ["/EHsc"],
        "unix": [],
    }

    if sys.platform == "darwin":
        c_opts["unix"] += ["-stdlib=libc++", "-mmacosx-version-min=10.7"]

    def build_extensions(self):
        ct = self.compiler.compiler_type
        opts = self.c_opts.get(ct, [])
        if ct == "unix":
            opts.append('-DVERSION_INFO="%s"' % self.distribution.get_version())
            opts.append(cpp_flag(self.compiler))
            if has_flag(self.compiler, "-fvisibility=hidden"):
                opts.append("-fvisibility=hidden")
        elif ct == "msvc":
            opts.append(
                '/DVERSION_INFO=\\"%s\\"' % self.distribution.get_version()
            )
        for ext in self.extensions:
            ext.extra_compile_args = opts
        build_ext.build_extensions(self)


setup(
    name="libsemigroups_pybind11",
    version=__version__,
    author="James D. Mitchell",
    author_email="jdm3@st-andrews.ac.uk",
    url="https://github.com/libsemigroups/libsemigroups_pybind11",
    description="TODO",
    long_description="",
    ext_modules=ext_modules,
    setup_requires=["pkgconfig>=0.29.2"],
    install_requires=["pybind11>=2.5", "packaging>=20.4"],
    tests_require=["nose", "tox"],
    cmdclass={"build_ext": BuildExt},
    zip_safe=False,
)
