# Exercises with pixi

## Add coverage feature

* Install `pytest-cov` in the `feature.test` pixi feature
* Modify the `tests_unit` task to display a coverage report at the end of the test run

## Use ruff

[ruff](https://docs.astral.sh/ruff/) is an extremely fast Python linter and formatter written in Rust.

* Install `ruff` in the `feature.linter` pixi feature

### Add a task to format code

* Task name: `format`
* Apply it to `src/bind_template`
* Verify the result on `bind_template/example.py`

### Add a task to sort imports

ruff can sort imports using the `I` rule set:

```console
ruff check --select I --fix <directory>
```

* Add this step to the existing `format` task
* Verify the result on `bind_template/example.py`

### Add a task to check the code

ruff is a linter. Its basic usage is:

```console
ruff check <directory>
```

* Create a `lint` task that runs ruff on the source directory

### Apply simple fixes

ruff can automatically fix simple issues (e.g. remove unused imports):

```console
ruff check --fix <directory>
```

* Create a `fix_code` task
* Verify the result


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