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

    // second class of module 
    // nb::class_<OtherClassOfModule>(m, "OtherClassOfModule")
    // ...
}