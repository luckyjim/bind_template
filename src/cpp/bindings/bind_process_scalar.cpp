#include <nanobind/nanobind.h>

#include "process_scalar.h"

namespace nb = nanobind;

void bind_process_scalar(nb::module_& m)
{
    m.doc() = "A C++ class binding by nanobind";
    nb::class_<ProcessScalar>(m, "ProcessScalar")
        .def(nb::init<>())
        .def("add", &ProcessScalar::add)
        .def("multiply", &ProcessScalar::multiply);
}