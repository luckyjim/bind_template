# Exercises with pixi

## Add coverage feature

* Install `pytest-cov` in the `feature.test` pixi feature
* Modify the `tests_unit` task to display a coverage report at the end of the test run

### Solution

```console
bind_template$ pixi add -f test pytest-cov
```

replace 
* tests_unit = "pytest -v tests/unit"

by

* tests_unit = "pytest -v --cov=bind_template tests/unit"


```console
bind_template$ pixi run -e quality install
bind_template$ pixi run -e quality tests_unit
✨ Pixi task (tests_unit in quality): pytest -v --cov=tests/unit
==================================================== test session starts ====================================================
platform linux -- Python 3.14.6, pytest-9.1.1, pluggy-1.6.0 -- /home/jcolley/projet/reprise/binder/bind_template/.pixi/envs/quality/bin/python3.14
cachedir: .pytest_cache
rootdir: /home/jcolley/projet/reprise/binder/bind_template
configfile: pyproject.toml
plugins: cov-7.1.0
collected 3 items                                                                                                           

tests/unit/scalar/test_ProcessScalar.py::test_ProcessScalar_add PASSED                                                [ 33%]
tests/unit/scalar/test_ProcessScalar.py::test_ProcessScalar_multiply PASSED                                           [ 66%]
tests/unit/test_example.py::test_find_max_with_parabola_interp_3pt PASSED                                             [100%]

====================================================== tests coverage =======================================================
______________________________________________________ coverage: platform linux, python 3.14.6-final-0 _______________________________________________________

Name                                    Stmts   Miss  Cover
-----------------------------------------------------------
src/bind_template/__init__.py               1      0   100%
src/bind_template/example.py               24      3    88%
src/bind_template/ndarray/__init__.py       0      0   100%
src/bind_template/scalar/__init__.py        0      0   100%
-----------------------------------------------------------
TOTAL                                      25      3    88%
===================================================================== 3 passed in 0.48s ======================================================================


```
 or 

```console
$ pixi shell -e quality
(bind_template:quality) bind_template$ pixi run install
(bind_template:quality) bind_template$ pixi run test
```

## Use ruff 

[ruff](https://docs.astral.sh/ruff/) is an extremely fast Python linter and formatter written in Rust.

* Install `ruff` in the `feature.linter` pixi feature

### Add a task to format code

* Task name: `format`
* Apply it to `src/bind_template`
* Verify the result on `bind_template/example.py`

#### Solution

```console
pixi add -f test ruff
```

in pixi.toml

```console
[feature.lint.tasks]
format = "ruff format src/bind_template"
```


```console
bind_template$ pixi run -e quality format
✨ Pixi task (format in quality): ruff format src/bind_template
3 files reformatted, 1 file left unchanged

```

### Add a task to sort import

ruff can sort imports using the `I` rule set:

```console
ruff check --select I --fix <directory>
```

* Add this step to the existing `format` task
* Verify the result on `bind_template/example.py`

#### Solution

in pixi.toml

```console
[feature.lint.tasks]
format = "ruff format src/bind_template && ruff check --select I --fix src/bind_template"
```

```console
bind_template$ pixi run -e quality format
✨ Pixi task (format in quality): ruff format src/bind_template && ruff check --select I --fix src/bind_template
4 files left unchanged
Found 1 error (1 fixed, 0 remaining).
```

### Add a task to check the code

ruff is a linter. Its basic usage is:

```console
ruff check <directory>
```

* Create a `lint` task that runs ruff on the source directory

#### Solution

in pixi.toml

```console
[feature.lint.tasks]
lint = "ruff check src/bind_template"
```

```console
bind_template$ pixi run -e quality lint
✨ Pixi task (lint in quality): ruff check src/bind_template
F401 [*] `numpy` imported but unused
 --> src/bind_template/example.py:3:17
  |
1 | from logging import getLogger
2 |
3 | import numpy as np
  |                 ^^
4 | import scipy.fft as sf
5 | from scipy import interpolate
  |
help: Remove unused import: `numpy`
...
```

### Apply simple fix 

ruff can automatically fix simple issues (e.g. remove unused imports):

```console
ruff check --fix <directory>
```

* Create a `fix_code` task
* Verify the result

#### Solution

in pixi.toml

```console
[feature.lint.tasks]
fix_code = "ruff check --fix src/bind_template"
```

```console
bind_template$ pixi run -e quality fix_code
✨ Pixi task (fix_code in quality): ruff check --fix src/bind_template
Found 7 errors (7 fixed, 0 remaining).
```
Unused import of example.py have been removed.


# Exercises with nanobind

## Compilation flags for bindings

* Check which optimization flag (`-O`) is used during the binding compilation stage
* What happens when multiple `-O` flags are defined? The GCC documentation states:

> If you use multiple `-O` options, with or without level numbers, the last such option is the one that is effective.

https://gcc.gnu.org/onlinedocs/gcc-3.2.3/gcc/Optimize-Options.html

* Fix the build to enforce `-O3` using the hint provided in `CMakeLists.txt`
* Verify the result

## Create a new module `process_ndarray` with a single class

**Goal:**

Create a new extension module `process_ndarray` inside the Python package `bind_template.ndarray`, and expose a single class `ProcessNdarray` whose interface is defined in `src/cpp/lib/process_ndarray.h`.

The C++ interface to implement:

```cpp
class ProcessNdarray {
public:
    ProcessNdarray() = default;

    // Add a scalar to all elements of a C-contiguous float64 array in place.
    // Returns the number of elements modified, or a negative error code.
    int add_in_place(nb::ndarray<double, nb::c_contig>& array, double scalar);

    // Print shape, strides, device and dtype information.
    void inspect(const nb::ndarray<>& array) const;
};
```

Key points:
- `nb::ndarray<double, nb::c_contig>` constrains the input to a C-contiguous `float64` array — nanobind enforces this at the call site, so no runtime dtype cast is needed.
- `nb::ndarray<>` (untyped) is used for `inspect` to accept any array regardless of dtype or layout.
- Each extension module must have its own `NB_MODULE(name, m)` entry point in a dedicated `.cpp` file.

Tasks:
* Implement the class in `src/cpp/lib/process_ndarray.cpp`
* Create the binding entry point in `src/cpp/bindings/bind_process_ndarray.cpp`
* Update `CMakeLists.txt` to build and install the new module under `bind_template/ndarray`
* Verify the installation with `pip install -e .`
* Add a unit test

## Add a second class to the `process_scalar` module

**Goal:**

Create a new C++ class `ProcessSqrt` with a single method `sqrt_if_pos` that returns the square root of a number if it is positive, and zero otherwise.

* Implement the class in `src/cpp/lib/`
* Update `CMakeLists.txt` to include it in the `process_scalar` module
* Verify the installation with `pip install -e .`
* Add a unit test