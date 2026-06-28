#include <nanobind/nanobind.h>

#include "process_scalar.h"

namespace nb = nanobind;

NB_MODULE(process_scalar, m)
{
    m.doc() = "A C++ class binding by nanobind";
    nb::class_<ProcessScalar>(m, "ProcessScalar")
        .def(nb::init<>())
        .def("add", &ProcessScalar::add)
        .def("multiply", &ProcessScalar::multiply);
}