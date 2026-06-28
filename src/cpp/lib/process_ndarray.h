#pragma once

#include <nanobind/ndarray.h>

namespace nb = nanobind;

class ProcessNdarray {
public:
    ProcessNdarray() = default;

    // add scalar for all elements of array , return size of array
    int add_in_place(nb::ndarray<double, nb::c_contig>& array, double scalar);

    // print shape and dtype
    void inspect(const nb::ndarray<>& array) const;
};