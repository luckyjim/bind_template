#include <nanobind/nanobind.h>

#include "process_ndarray.h"

namespace nb = nanobind;

NB_MODULE(process_ndarray, m) {
    m.doc() = "A C++ class binding by nanobind with Ndarray type";
    nb::class_<ProcessNdarray>(m, "ProcessNdarray", "This class used a Ndarray array from python")
        .def(nb::init<>())
        .def("add_in_place", &ProcessNdarray::add_in_place)
        .def("inspect", &ProcessNdarray::inspect);
}