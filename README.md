# bind_template

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

pixi.toml defines dependencies and workflow commands using features and environments.

- runtime feature: python, nanobind, numpy, scipy, pip, ipython
- test feature: pytest
- lint feature: ruff

```bash
# install package in feature lint 
pixi add -f lint ruff
```

For each feature, we can define a task in pixi.toml, like:

```bash
[feature.test.tasks]
install = "pip install -e . "
```

Environment is a set of features:
- default environment: runtime
- quality environment: runtime + test + lint

```console
[environments]
quality = { features = ["runtime","test","lint"] }
```
run task in env, with pixi run

```bash
pixi run -e quality install
```

active shell with pixi shell
```bash
pixi shell -e quality 
```

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



## Testing Strategy

The repository is organized around three test levels:

- unit tests: validate isolated functions/classes (fast feedback)
- integration tests: validate interactions between components
- end-to-end tests: validate complete user-facing workflows

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

Development/debug mode

In the default pixi environment, you can run CMake directly:

```console
$ cmake -S . -B build -Dnanobind_DIR=$(python -m nanobind --cmake_dir)
$ cmake --build build
$ cmake --build build --target clean
```

Installation

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

find in exo.md file two kinds of exercices:
* pixi for quality tool
* use of nanobind