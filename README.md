# bind_template package

## Overview

This repository is a practical template for building a Python package with C++ extensions powered by nanobind.

It demonstrates how to:

- organize a Python package using the src layout
- configure packaging and native builds with pyproject.toml and scikit-build-core
- manage dependencies and developer workflows with pixi
- structure tests (unit, integration, end-to-end)
- use Ruff for linting and formatting
- expose C++ code to Python with nanobind


## Goal of This Template

Use this repository as a starting point for hybrid Python/C++ packages where you need:

- clean project structure
- reproducible environments
- native extension builds through CMake
- testing and code-quality foundations

## Project Structure

The project follows the src layout, where importable Python code lives in src/bind_template.

```text
bind_template/
├── CMakeLists.txt
├── pixi.toml
├── pyproject.toml
├── src/
│   ├── bind_template/
│   │   ├── __init__.py
│   │   ├── example.py
│   │   ├── ndarray/
│   │   └── scalar/
│   └── cpp/
│       ├── bindings/
│       │   └── bind_process_scalar.cpp
│       └── lib/
│           ├── process_scalar.cpp
│           ├── process_scalar.h
│           └── process_ndarray.h
└── tests/
    ├── unit/
    ├── integration/
    └── e2e/
```

Why this layout is useful:
- it keeps Python and C++ sources clearly separated
- it keeps test and source clearly separated

## Dependency Management with Pixi

Pixi is a package manager that simplifies dependency management for different development scenarios.
pixi.toml defines dependencies and workflow commands using features and environments.

### Why use Pixi environments?

1. **Isolation**: Keep development tools (like ruff, pytest) separate from production runtime dependencies.
   This avoids bloating your installed package with unnecessary build tooling.

2. **Reproducibility**: The pixi.lock file pins all transitive dependencies to exact versions.
   Every team member, CI/CD pipeline, and deployment gets identical environments.

3. **Workflow automation**: Define named tasks in pixi.toml and run them in the correct environment context.
   No need to remember complex shell command chains.

4. **Multiple use cases**: Switch between environments for different work without reinstalling.

### Features and environments

pixi.toml organizes dependencies as features, then combines them into named environments:

Features defined in the project:
- runtime feature: python, nanobind, numpy, scipy, pip, ipython
- test feature: pytest
- lint feature: ruff


> NOTE:
>
> `pixi add cmake` can returns an error (Filename too long (os error 36)) depending on the file system.
>
> if you don't have cmake installed, try with pixi else use system installation


Tasks are defined per-feature:

```toml
[feature.test.tasks]
install = "pip install -e . "
tests_unit = "pytest -v tests/unit"
```

Environments combine features:

```toml
[environments]
default = { features = ["runtime"] }
quality = { features = ["runtime", "test", "lint"] }
```

### Usage patterns

**Pattern 1: Daily development (minimal environment)**

```bash
pixi shell
# You now have: python, nanobind, numpy, scipy, pip, ipython
# NOT pytest or ruff—keep your environment lean

pip install -e .
python -c "from bind_template.scalar.process_scalar import ProcessScalar; print(ProcessScalar().add(1, 2))"
```

**Pattern 2: Testing and code review (full development environment)**

```bash
# Run predefined tasks in the quality environment
pixi run -e quality install
pixi run -e quality tests_unit

# Or interactively work in quality environment
pixi shell -e quality
ruff check .
ruff format .
pytest -v tests/unit
```


### Benefits summary

- **For solo developers**: One command to switch tooling context (pixi shell -e quality).
- **For teams**: Onboarding is trivial—pixi shell and you're ready to code.
- **For CI/CD**: Deterministic builds thanks to pixi.lock; no "works on my machine" surprises.
- **For users**: Smaller install footprints; production never includes dev tools.

## Packaging and Build System

The file pyproject.toml defines both package metadata and the native build backend.

- build backend: scikit-build-core
- C++ binding framework: nanobind
- package metadata: project name, version, authors, Python requirement

At build time:

1. scikit-build-core drives CMake.
2. CMake compiles the nanobind extension module.
3. The compiled shared library is installed into the Python package path.

This project currently builds the process_scalar extension and installs it into bind_template/scalar.


```bash
# install the package in editable mode
pip install -e .

# uninstall the package
pip uninstall bind_template
```

**Distribution**

If the package is public, you can install with url of repository with prefix `git+`

```console
$ pip install git+https://github.com/luckyjim/bind_template.git

Collecting git+https://github.com/luckyjim/bind_template.git
  Cloning https://github.com/luckyjim/bind_template.git 
  ...
Successfully built bind-template
Installing collected packages: bind-template
Successfully installed bind-template-0.0.1
```

## Testing Strategy

The repository is organized around three test levels:

- unit tests: validate isolated functions/classes (fast feedback)
- integration tests: validate interactions between components
- end-to-end tests: validate complete user-facing workflows

If you are interested in software testing, you can consult the [International Software Testing Qualifications Board (ISTQB)](https://en.wikipedia.org/wiki/International_Software_Testing_Qualifications_Board) courses, like [Certified Tester
Foundation Level](https://istqb.org/wp-content/uploads/2024/11/ISTQB_CTFL_Syllabus_v4.0.1.pdf) as well as [the definitions of terms related to software testing](https://glossary.istqb.org/en_US/search?term=&exact_matches_first=true).

Current automated examples include unit tests for the C++-backed ProcessScalar class.

Pytest basics used in this project:

- test module names should use the test_ prefix (for example: test_ProcessScalar.py)
- test function names should use the test_ prefix
- use assert for scalar values and simple conditions
- use numpy.allclose() when comparing two floating-point arrays

Example:

```python
import numpy as np

def test_addition():
   assert 1 + 2 == 3

def test_array_values():
   expected = np.array([0.1, 0.2, 0.3])
   actual = np.array([0.10000001, 0.19999999, 0.30000002])
   assert np.allclose(actual, expected)
```

Consult the [pytest doc](https://docs.pytest.org/en/stable/index.html) to discover all amazing possibility of this library.

## Code Quality

Ruff is the primary code-quality tool in this project.

- linter: detects style and correctness issues
- formatter: enforces a consistent code style
- auto-fix: can automatically resolve a subset of issues

Typical commands:

```bash
ruff check .
ruff format .
```

## nanobind Integration

nanobind allows calling C++ code from Python.

### Source Organization

- C++ implementation files are in src/cpp/lib
- nanobind binding code is in src/cpp/bindings

### CMake Essentials

Three key points in CMakeLists.txt:

1. Find nanobind:

```cmake
find_package(nanobind CONFIG REQUIRED)
```

2. Build the extension module:

```cmake
nanobind_add_module(process_scalar
    STABLE_ABI
    src/cpp/bindings/bind_process_scalar.cpp
    src/cpp/lib/process_scalar.cpp
)
```

3. Install the shared library into the Python package:

```cmake
install(TARGETS process_scalar LIBRARY DESTINATION bind_template/scalar)
```

The generated shared object (.so on Linux) is then importable from Python as a regular module.

### Binding Code Walkthrough


This code defines a process_scalar module exposing one class, ProcessScalar.

```c++
#include <nanobind/nanobind.h>
#include "process_scalar.h"

namespace nb = nanobind;

NB_MODULE(process_scalar, m)
{
    m.doc() = "A C++ class binding by nanobind";

    nb::class_<ProcessScalar>(m, "ProcessScalar","A simple class from C++")
        .def(nb::init<>())
        .def("add", &ProcessScalar::add)
        .def("multiply", &ProcessScalar::multiply);
}
```

- NB_MODULE(process_scalar, m)
   Defines the Python extension module.
- m.doc() = "A C++ class binding by nanobind";
   Sets the module-level docstring.
- nb::class_<ProcessScalar>(m, ...)
   Declares a Python-visible class backed by the C++ ProcessScalar type.
- .def("add", &ProcessScalar::add)
   Binds a Python method name to a C++ member function.

The complete nanobind documentation is available [here](https://nanobind.readthedocs.io/en/latest/).

### Compilation and installation

**Development/debug mode**

In the default pixi environment, you can run CMake directly:

```console
$ cmake -S . -B build -Dnanobind_DIR=$(python -m nanobind --cmake_dir)
$ cmake --build build
$ cmake --build build --target clean
```

**Installation**

Installation is performed with `pip install .` thanks to the build system configured in pyproject.toml:

```console
[build-system]
requires = ["scikit-build-core >=0.4.3", "nanobind >=1.3.2"]
build-backend = "scikit_build_core.build"
```

### Minimal Usage Example

```python
from bind_template.scalar.process_scalar import ProcessScalar

obj = ProcessScalar()
print(obj.add(1, 2))
print(obj.multiply(3, 4))
```

Expected output:

```text
3
12
```

## Exercices

File exo.md contents two kinds of exercice :
* pixi for quality tool
* use of nanobind

Correction in branches
* pixi_solution
* ndarray_solution