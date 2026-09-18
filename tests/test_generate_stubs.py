# Distributed under the terms of the GPL license version 3.
# The full license is in the file LICENSE, distributed with this software.
# AI assistance: OpenAI Codex helped write these generator checks.

"""Check stub generation, including signatures attached to wrappers at runtime."""

# pylint: disable=missing-function-docstring

import ast
import importlib.util
import subprocess
import sys
import types
from pathlib import Path

import pytest


@pytest.fixture(name="stub_generator", scope="module")
def fixture_stub_generator():
    path = Path(__file__).resolve().parents[1] / "etc" / "generate-stubs.py"
    spec = importlib.util.spec_from_file_location("generate_stubs", path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


@pytest.fixture(name="generated_action", scope="module")
def fixture_generated_action(stub_generator, tmp_path_factory):
    pytest.importorskip("pybind11_stubgen")
    output = tmp_path_factory.mktemp("action_stubs")
    original = Path(__file__).resolve().parents[1] / "src/libsemigroups_pybind11/action.pyi"
    before = original.read_bytes() if original.exists() else None
    paths = stub_generator.generate("libsemigroups_pybind11.action", output, strict=True)
    assert paths == [output / "libsemigroups_pybind11/action.pyi"]
    assert (original.read_bytes() if original.exists() else None) == before
    return paths[0]


def test_documented_signature_precedence(stub_generator):
    doc = """size(self: SomeCppType, unused: int) -> str
    :sig=(self: Action) -> int:
    :sig=(self: Action) -> int:
    """
    signatures = stub_generator.documented_signatures("size", doc)
    assert len(signatures) == 1
    assert [arg.arg for arg in signatures[0].args.args] == ["self"]
    assert ast.unparse(signatures[0].returns) == "int"


def test_documented_overloads(stub_generator):
    signatures = stub_generator.documented_signatures(
        "setting",
        "setting(*args, **kwargs)\nOverloaded function.\n\n"
        "1. setting(self: Cpp) -> bool\n"
        "2. setting(self: Cpp, value: bool) -> Cpp\n",
    )
    assert len(signatures) == 2
    assert [len(sig.args.args) for sig in signatures] == [1, 2]


def test_signature_defaults_are_not_executed(stub_generator):
    signatures = stub_generator.documented_signatures(
        "f", ":sig=(x: str = this_function_does_not_exist()) -> None:"
    )
    assert len(signatures) == 1
    assert isinstance(signatures[0].args.defaults[0], ast.Call)


def test_invalid_native_base(stub_generator, capsys):
    text = "class BoundClass(detail::Base):\n    def size(self) -> int: ...\n"
    tree = stub_generator.parse_stub(text, "example", strict=False)
    cls = next(node for node in tree.body if isinstance(node, ast.ClassDef))
    assert ast.unparse(cls.bases[0]) == "typing.Any"
    assert cls.body[0].name == "size"
    assert "detail::Base" in capsys.readouterr().err
    with pytest.raises(SyntaxError):
        stub_generator.parse_stub(text, "example", strict=True)


def test_unrelated_syntax_errors_are_not_suppressed(stub_generator):
    with pytest.raises(SyntaxError):
        stub_generator.parse_stub("def broken(: ...", "example", strict=False)


def test_native_keyword_parameter(stub_generator):
    tree = stub_generator.parse_stub(
        "def f(graph: object, from: int, path: list[int]) -> int: ...", "example", strict=False
    )
    args = tree.body[0].args
    assert [arg.arg for arg in args.posonlyargs] == ["graph", "from_"]
    assert [arg.arg for arg in args.args] == ["path"]
    ast.parse(ast.unparse(tree))
    tree = stub_generator.parse_stub("def f(from: int, /): ...", "example", strict=False)
    assert tree.body[0].args.posonlyargs[0].arg == "from_"
    with pytest.raises(SyntaxError):
        stub_generator.parse_stub("def f(*, from: int): ...", "example", strict=False)


def test_native_bmat8(stub_generator, tmp_path):
    pytest.importorskip("pybind11_stubgen")
    paths = stub_generator.generate("_libsemigroups_pybind11", tmp_path)
    path = tmp_path / "_libsemigroups_pybind11.pyi"
    assert path in paths
    tree = ast.parse(path.read_text(encoding="utf-8"), feature_version=(3, 10))
    cls = next(
        node for node in tree.body if isinstance(node, ast.ClassDef) and node.name == "BMat8"
    )
    methods = [node for node in cls.body if isinstance(node, ast.FunctionDef)]
    assert {node.name for node in methods} >= {"__init__", "__getitem__", "__mul__", "to_int"}
    assert ast.unparse(next(node.returns for node in methods if node.name == "to_int")) == "int"
    assert sum(node.name == "__init__" for node in methods) > 1


def test_public_class_selection_uses_exports_and_identity(stub_generator):
    native = types.ModuleType("native")
    public = types.ModuleType("public")
    helper = types.ModuleType("public.helper")
    native.RealName = type("RenamedForDocs", (), {})
    native.Hidden = type("Hidden", (), {})
    public.Alias = native.RealName
    public.AccidentallyImported = native.Hidden
    public.Wrapper = type("RealName", (), {})
    public.helper = helper
    public.__all__ = ["Alias", "Wrapper", "helper"]
    helper.__all__ = ["Root"]
    helper.Root = public  # A cycle in exported modules must terminate.
    assert stub_generator.public_native_classes(public, native) == {"RealName"}
    helper.AlsoPublic = native.Hidden
    helper.__all__.append("AlsoPublic")
    assert stub_generator.public_native_classes(public, native) == {"RealName", "Hidden"}


def test_native_typing_dependencies(stub_generator):
    native = types.ModuleType("native")
    for name in ("Base", "Result", "Nested", "Unused", "Hidden"):
        setattr(native, name, type(name, (), {}))
    native.DEFAULT = 0
    tree = ast.parse(
        "class Public(Base):\n"
        '    """Hidden is mentioned only in documentation."""\n'
        '    def f(self, x: list["Result"] = DEFAULT) -> native.Nested.Inner: ...\n'
    )
    assert stub_generator.native_dependencies(tree, native) == {
        "Base",
        "Result",
        "Nested",
        "DEFAULT",
    }


def test_public_only_rejects_wrapper_target(stub_generator, tmp_path):
    with pytest.raises(ValueError, match="--public-only requires"):
        stub_generator.generate("libsemigroups_pybind11.action", tmp_path, public_only=True)
    assert not any(tmp_path.iterdir())


def test_public_native_stub(stub_generator, tmp_path, capfd):
    pytest.importorskip("pybind11_stubgen")
    native = importlib.import_module("_libsemigroups_pybind11")
    public = importlib.import_module("libsemigroups_pybind11")

    before = {name: value for name, value in vars(native).items() if isinstance(value, type)}
    paths = stub_generator.generate("_libsemigroups_pybind11", tmp_path, public_only=True)
    assert paths == [tmp_path / "_libsemigroups_pybind11.pyi"]
    text = paths[0].read_text(encoding="utf-8")
    tree = ast.parse(text, feature_version=(3, 10))
    classes = {node.name: node for node in tree.body if isinstance(node, ast.ClassDef)}
    assert stub_generator.public_native_classes(public, native) <= classes.keys()
    assert {"BMat8", "Runner", "forest_PathsFromRoots"} <= classes.keys()
    assert {
        "AlphabetWord",
        "AlphabetString",
        "RightActionPPerm1List",
        "FroidurePinBMat8",
    }.isdisjoint(classes)
    # An unexported base is kept because public PathsFromRoots inherits from it.
    assert "forest_PathsFromToRootsCommon" in classes
    assert stub_generator.native_dependencies(tree, native) <= stub_generator.bindings(tree.body)
    assert "__all__" not in stub_generator.bindings(tree.body)
    assert "def __getattr__(name: str) -> _typeshed.Incomplete:" in text
    iterator = next(
        node
        for node in classes["BMat8"].body
        if isinstance(node, ast.FunctionDef) and node.name == "__iter__"
    )
    assert ast.unparse(iterator.returns) == "collections.abc.Iterator[list[bool]]"
    assert all(getattr(native, name) is value for name, value in before.items())
    assert "AlphabetWord" not in capfd.readouterr().err
    stub_generator.generate("_libsemigroups_pybind11", tmp_path, public_only=True, force=True)
    assert paths[0].read_text(encoding="utf-8") == text


def test_public_bmat8_with_mypy(stub_generator, tmp_path):
    pytest.importorskip("pybind11_stubgen")
    pytest.importorskip("mypy")
    stub_generator.generate("_libsemigroups_pybind11", tmp_path, public_only=True)
    # Isolate the selected BMat8 section: other public classes still have malformed
    # native signatures. The selection/dependency test checks the complete output.
    stub = tmp_path / "_libsemigroups_pybind11.pyi"
    tree = ast.parse(stub.read_text(encoding="utf-8"))
    tree.body = [
        node
        for node in tree.body
        if isinstance(node, (ast.Import, ast.ImportFrom))
        or isinstance(node, ast.ClassDef)
        and node.name == "BMat8"
    ]
    stub.write_text(ast.unparse(tree), encoding="utf-8")
    package = tmp_path / "libsemigroups_pybind11"
    package.mkdir()
    (package / "__init__.pyi").write_text(
        "from _libsemigroups_pybind11 import BMat8 as BMat8\n", encoding="utf-8"
    )
    client = tmp_path / "client.py"
    client.write_text(
        "from typing_extensions import assert_type\n"
        "from libsemigroups_pybind11 import BMat8\n"
        "matrix = BMat8([[True, False], [False, True]])\n"
        "assert_type(matrix.to_int(), int)\n"
        "assert_type(matrix[0, 1], bool)\n"
        "assert_type(matrix[0], list[bool])\n"
        "for row in matrix:\n"
        "    assert_type(row, list[bool])\n"
        "matrix.nonexistent()  # type: ignore[attr-defined]\n"
        "BMat8('bad')  # type: ignore[arg-type]\n",
        encoding="utf-8",
    )
    # Check client inference while unrelated draft-native annotations remain incomplete.
    config = tmp_path / "mypy.ini"
    config.write_text(
        "[mypy]\nfollow_imports = silent\nfollow_imports_for_stubs = True\n", encoding="utf-8"
    )
    result = subprocess.run(
        [
            sys.executable,
            "-m",
            "mypy",
            "--python-version=3.10",
            "--ignore-missing-imports",
            "--config-file",
            str(config),
            "--warn-unused-ignores",
            "--cache-dir",
            str(tmp_path / "cache"),
            str(client),
        ],
        cwd=tmp_path,
        capture_output=True,
        text=True,
        check=False,
    )
    assert result.returncode == 0, result.stdout + result.stderr


def test_action_generic_signatures(generated_action):
    text = generated_action.read_text(encoding="utf-8")
    tree = ast.parse(text, feature_version=(3, 10))
    classes = {node.name: node for node in tree.body if isinstance(node, ast.ClassDef)}
    action = classes["Action"]
    methods = [node for node in action.body if isinstance(node, ast.FunctionDef)]
    by_name = {node.name: node for node in methods}
    assert ast.unparse(action.bases[-1]) == "_Generic[_Element, _Point]"
    assert ast.unparse(by_name["__getitem__"].returns) == "_Point"
    assert ast.unparse(by_name["generators"].returns) == "Iterator[_Element]"
    assert ast.unparse(by_name["add_seed"].returns) == "typing_extensions.Self"
    assert ast.unparse(by_name["__iter__"].returns) == "collections.abc.Iterator[_Point]"
    assert len(by_name["add_seed"].args.posonlyargs) == 2
    assert not by_name["add_seed"].args.args
    assert ast.unparse(classes["RightAction"].bases[0]) == "Action[_Element, _Point]"
    assert ast.unparse(classes["LeftAction"].bases[0]) == "Action[_Element, _Point]"
    constructor = {arg.arg: arg for arg in by_name["__init__"].args.kwonlyargs}
    assert ast.unparse(constructor["seeds"].annotation) == "list[_Point] | None"
    assert "type[libsemigroups_pybind11.adapters.ImageRightAction]" in ast.unparse(
        constructor["func"].annotation
    )
    overloads = [node for node in methods if node.name == "cache_scc_multipliers"]
    assert [ast.unparse(node.returns) for node in overloads] == ["bool", "typing_extensions.Self"]
    assert all(ast.unparse(node.decorator_list[0]) == "typing.overload" for node in overloads)
    assert "_all_wrapped_cxx_types" not in text
    assert ":sig=" not in text


def test_regeneration_is_deterministic(stub_generator, generated_action):
    before = generated_action.read_bytes()
    stub_generator.generate(
        "libsemigroups_pybind11.action", generated_action.parents[1], force=True, strict=True
    )
    assert generated_action.read_bytes() == before


def test_overwrite_requires_force(stub_generator, generated_action, tmp_path):
    destination = tmp_path / "libsemigroups_pybind11/action.pyi"
    destination.parent.mkdir()
    destination.write_text("# hand edited stub\n", encoding="utf-8")
    with pytest.raises(FileExistsError, match="--force"):
        stub_generator.generate("libsemigroups_pybind11.action", tmp_path)
    assert destination.read_text(encoding="utf-8") == "# hand edited stub\n"
    assert generated_action.exists()


def test_another_wrapper_module(stub_generator, tmp_path):
    pytest.importorskip("pybind11_stubgen")
    paths = stub_generator.generate("libsemigroups_pybind11.sims", tmp_path, strict=True)
    tree = ast.parse(paths[0].read_text(encoding="utf-8"), feature_version=(3, 10))
    classes = {node.name: node for node in tree.body if isinstance(node, ast.ClassDef)}
    assert {"Sims1", "Sims2", "RepOrc"} <= classes.keys()
    assert any(
        isinstance(node, ast.FunctionDef) and node.name == "find_if"
        for node in classes["Sims1"].body
    )


@pytest.mark.parametrize("name", ["presentation", "matrix", "forest", "detail.dot"])
def test_properties_enums_and_reexports(stub_generator, tmp_path, name):
    pytest.importorskip("pybind11_stubgen")
    paths = stub_generator.generate(f"libsemigroups_pybind11.{name}", tmp_path, strict=True)
    path = next(path for path in paths if path.name != "examples.pyi")
    tree = ast.parse(path.read_text(encoding="utf-8"))
    classes = {node.name: node for node in tree.body if isinstance(node, ast.ClassDef)}
    if name == "presentation":
        accessors = [
            node
            for node in classes["Presentation"].body
            if isinstance(node, ast.FunctionDef) and node.name == "rules"
        ]
        assert [ast.unparse(node.decorator_list[0]) for node in accessors] == [
            "property",
            "rules.setter",
        ]
        assert [len(node.args.args) for node in accessors] == [1, 2]
    elif name == "matrix":
        members = [node for node in classes["MatrixKind"].body if isinstance(node, ast.Assign)]
        assert {node.targets[0].id for node in members} >= {"Boolean", "Integer", "MaxPlus"}
    else:
        assert not classes
        imports = [node for node in tree.body if isinstance(node, ast.ImportFrom)]
        assert any(node.module == "_libsemigroups_pybind11" for node in imports)
        if name == "detail.dot":
            assert any(alias.name == "Dot" for node in imports for alias in node.names)


def test_generated_stub_with_mypy(generated_action, tmp_path):
    pytest.importorskip("mypy")
    # Isolate this check from the hand-written Action stub in the source tree.
    package = tmp_path / "libsemigroups_pybind11"
    package.mkdir()
    (package / "__init__.pyi").write_text("", encoding="utf-8")
    stub = package / "action.pyi"
    stub.write_bytes(generated_action.read_bytes())
    client = tmp_path / "client.py"
    client.write_text(
        "from typing_extensions import assert_type\n"
        "from libsemigroups_pybind11.action import RightAction\n"
        "def check(action: RightAction[object, list[int]]) -> None:\n"
        "    assert_type(action[0], list[int])\n"
        "    assert_type(action.cache_scc_multipliers(), bool)\n"
        "    assert_type(action.cache_scc_multipliers(True), RightAction[object, list[int]])\n"
        "    for point in action:\n"
        "        assert_type(point, list[int])\n"
        "    action.add_seed('bad')  # type: ignore[arg-type]\n"
        "    action.reserve(val=10)  # type: ignore[call-arg]\n",
        encoding="utf-8",
    )
    result = subprocess.run(
        [
            sys.executable,
            "-m",
            "mypy",
            "--python-version",
            "3.10",
            "--ignore-missing-imports",
            "--follow-imports=silent",
            "--warn-unused-ignores",
            "--cache-dir",
            str(tmp_path / "cache"),
            str(stub),
            str(client),
        ],
        cwd=tmp_path,
        capture_output=True,
        text=True,
        check=False,
    )
    assert result.returncode == 0, result.stdout + result.stderr


def test_missing_generator_dependency(stub_generator, monkeypatch, tmp_path):
    monkeypatch.setattr(stub_generator.importlib.util, "find_spec", lambda _: None)
    with pytest.raises(RuntimeError, match="--with pybind11-stubgen"):
        stub_generator.generate("libsemigroups_pybind11.action", tmp_path)
    assert not any(tmp_path.iterdir())


@pytest.mark.parametrize(
    "options, class_name",
    [
        (["libsemigroups_pybind11.action"], "Action"),
        (["_libsemigroups_pybind11"], "BMat8"),
        (["_libsemigroups_pybind11", "--public-only"], "BMat8"),
    ],
)
@pytest.mark.parametrize("stdout_options", [[], ["--stdout"]])
def test_stdout_cli(stub_generator, tmp_path, options, class_name, stdout_options):
    pytest.importorskip("pybind11_stubgen")
    sentinel = tmp_path / "keep.pyi"
    sentinel.write_text("# existing stub\n", encoding="utf-8")
    result = subprocess.run(
        [sys.executable, stub_generator.__file__, *options, *stdout_options],
        cwd=tmp_path,
        capture_output=True,
        text=True,
        check=False,
    )
    assert result.returncode == 0, result.stderr
    assert result.stdout.startswith(stub_generator.HEADER)
    tree = ast.parse(result.stdout, feature_version=(3, 10))
    assert any(isinstance(node, ast.ClassDef) and node.name == class_name for node in tree.body)
    assert "Selected " not in result.stdout
    if options[0] == "_libsemigroups_pybind11":
        assert "Selected " in result.stderr
        assert not any(
            isinstance(node, ast.ClassDef) and node.name == "AlphabetWord" for node in tree.body
        )
    assert list(tmp_path.iterdir()) == [sentinel]
    assert sentinel.read_text(encoding="utf-8") == "# existing stub\n"


def test_explicit_output_directory_cli(stub_generator, generated_action, tmp_path):
    output = tmp_path / "stubs"
    result = subprocess.run(
        [
            sys.executable,
            stub_generator.__file__,
            "libsemigroups_pybind11.action",
            "--output-dir",
            str(output),
        ],
        cwd=tmp_path,
        capture_output=True,
        text=True,
        check=False,
    )
    assert result.returncode == 0, result.stderr
    stub = output / "libsemigroups_pybind11/action.pyi"
    assert stub.read_bytes() == generated_action.read_bytes()
    assert result.stdout == f"{stub}\n"


def test_stdout_rejects_multiple_stubs(stub_generator, monkeypatch, capsys, tmp_path):
    monkeypatch.chdir(tmp_path)
    monkeypatch.setattr(sys, "argv", ["generate-stubs.py", "package"])
    monkeypatch.setattr(
        stub_generator,
        "render_stubs",
        lambda *_args, **_kwargs: {
            Path("a.pyi"): "class A: ...\n",
            Path("b.pyi"): "class B: ...\n",
        },
    )
    assert stub_generator.main() == 1
    result = capsys.readouterr()
    assert result.out == ""
    assert "Standard output requires exactly one stub" in result.err
    assert not any(tmp_path.iterdir())


def test_stdout_failure_keeps_output_empty(stub_generator, monkeypatch, capsys):
    monkeypatch.setattr(sys, "argv", ["generate-stubs.py", "example", "--strict"])

    def fail(*_args, **kwargs):
        assert kwargs["strict"]
        print("Import diagnostic")
        raise RuntimeError("generation failed")

    monkeypatch.setattr(stub_generator, "render_stubs", fail)
    assert stub_generator.main() == 1
    result = capsys.readouterr()
    assert result.out == ""
    assert "Import diagnostic" in result.err
    assert "generation failed" in result.err


@pytest.mark.parametrize(
    "options", [["--stdout", "--output-dir", "stubs"], ["--stdout", "--force"], ["--force"]]
)
def test_stdout_rejects_file_options(stub_generator, monkeypatch, capsys, options):
    monkeypatch.setattr(sys, "argv", ["generate-stubs.py", "example", *options])
    with pytest.raises(SystemExit) as error:
        stub_generator.main()
    assert error.value.code == 2
    assert capsys.readouterr().out == ""


@pytest.mark.parametrize("options", [[], ["--public-only"]])
def test_module_argument_is_required(stub_generator, monkeypatch, capsys, options):
    monkeypatch.setattr(sys, "argv", ["generate-stubs.py", *options])
    with pytest.raises(SystemExit) as error:
        stub_generator.main()
    assert error.value.code == 2
    result = capsys.readouterr()
    assert result.out == ""
    assert "the following arguments are required: module" in result.err


@pytest.mark.parametrize(
    "options, public_only",
    [
        (["_libsemigroups_pybind11"], True),
        (["_libsemigroups_pybind11", "--public-only"], True),
        (["_libsemigroups_pybind11", "--no-public-only"], False),
        (["libsemigroups_pybind11.action"], False),
    ],
)
def test_cli_public_only_selection(stub_generator, monkeypatch, capsys, options, public_only):
    monkeypatch.setattr(sys, "argv", ["generate-stubs.py", *options])

    def render(module_name, **kwargs):
        assert module_name == options[0]
        assert kwargs["public_only"] is public_only
        return {Path("example.pyi"): "class Example: ...\n"}

    monkeypatch.setattr(stub_generator, "render_stubs", render)
    assert stub_generator.main() == 0
    assert capsys.readouterr().out == "class Example: ...\n"


@pytest.mark.parametrize("public_only", [False, True])
def test_failed_generation_preserves_destination(
    stub_generator, monkeypatch, tmp_path, public_only
):
    monkeypatch.setattr(stub_generator.importlib.util, "find_spec", lambda _: object())
    destination = tmp_path / "action.pyi"
    destination.write_text("# keep this stub\n", encoding="utf-8")

    def fail(command, **_kwargs):
        if public_only:
            assert command[-1] == "1"  # Strict mode is passed to the isolated backend.
        else:
            assert "--exit-code" in command
        raise subprocess.CalledProcessError(1, command)

    monkeypatch.setattr(stub_generator.subprocess, "run", fail)
    with pytest.raises(subprocess.CalledProcessError):
        stub_generator.generate(
            "_libsemigroups_pybind11" if public_only else "libsemigroups_pybind11.action",
            tmp_path,
            strict=True,
            public_only=public_only,
        )
    assert list(tmp_path.iterdir()) == [destination]
    assert destination.read_text(encoding="utf-8") == "# keep this stub\n"
