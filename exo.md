# Exercices for pixi

## Add coverage feature

* install in "feature.test" the pytest-cov library
* modify task tests_unit to obtain the coverage at the end of test

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

[ruff](https://docs.astral.sh/ruff/) is a linter and formater very fast.

* install ruff in  "feature.linter"

### Add task format code

* name of task : `format`
* use it on src/bind_template 
* check the result with bind_template/example.py

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

### Add task to sort import

ruff can also sort import with the syntaxe

```console
ruff check --select I --fix <directory>
 ```

* add this feature in task `format`
* check the result with bind_template/example.py

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

### Add task to check the code

ruff is a linter with syntaxe

```console
ruff check <directory>
```

* create task `lint` and use ruff

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

ruff can also apply simple fix to the code, like remove unused import, with syntax

```console
ruff check --fix <directory>
```

* create a task `fix_code`
* check the result

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


# Exercices for nanobind

## Add a second class to module process_scalar

Create a new C++ class ProcessDouble with unique method sqrt_if_pos which computes square root if number is positive else return zero.

* create the new class
* modify the cMakelist.txt to bind it in python
* check the installation in package with pip install
* add a test

## Create a new module process_ndarray with unique class

create a new module process_ndarray in package `run_template.ndarray` and add unique class `ProcessNdarray` with the interface defined in `src/cpp/lib/process_ndarray.h`


```c++
class ProcessNdarray {
public:
    ProcessNdarray() = default;

    // add scalar for all elements of array of double, return size of array
    int add_in_place(array<double>, double scalar);

    // print shape and dtype
    int inspect(?? array) const;
};
```


* create the new class
* modify the cMakelist.txt to bind it in python
* check the installation in package with pip install
* add a test
