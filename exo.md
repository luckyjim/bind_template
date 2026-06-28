# Exercices for pixi

## Add coverage feature

* install in "feature.test" the pytest-cov library
* modify task tests_unit to obtain the coverage at the end of test

## Use ruff 

[ruff](https://docs.astral.sh/ruff/) is a linter and formater very fast.

* install ruff in  "feature.linter"

### Add task format code

* name of task : `format`
* use it on src/bind_template 
* check the result with bind_template/example.py

### Add task to sort import

ruff can also sort import with the syntaxe

```console
ruff check --select I --fix <directory>
 ```

* add this feature in task `format`
* check the result with bind_template/example.py

### Add task to check the code

ruff is a linter with syntaxe

```console
ruff check <directory>
```

* create task `lint` and use ruff

### Apply simple fix 

ruff can also apply simple fix to the code, like remove unused import, with syntax

```console
ruff check --fix <directory>
```

* create a task `fix_code`
* check the result


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
