#include "process_ndarray.h"

#include <cstdint>
#include <cstdio>
#include <limits>

int ProcessNdarray::add_in_place(nb::ndarray<double, nb::c_contig>& array, double scalar) {
    if (!array.is_valid()) {
        return -1;
    }

    const size_t n = array.size();
    if (n > static_cast<size_t>(std::numeric_limits<int>::max())) {
        return -2;
    }

    auto *data = array.data();
    for (size_t i = 0; i < n; ++i) {
        data[i] += scalar;
    }

    return static_cast<int>(n);
}

void ProcessNdarray::inspect(const nb::ndarray<>& array) const {
    std::printf("Array data pointer : %p\n", array.data());
    std::printf("Array dimension : %zu\n", array.ndim());
    for (size_t i = 0; i < array.ndim(); ++i) {
        std::printf("Array dimension [%zu] : %zu\n", i, array.shape(i));
        std::printf("Array stride    [%zu] : %zd\n", i, array.stride(i));
    }
    std::printf("Device ID = %u (cpu=%i, cuda=%i)\n",
                static_cast<unsigned>(array.device_id()),
                int(array.device_type() == nb::device::cpu::value),
                int(array.device_type() == nb::device::cuda::value));
    std::printf("Array dtype: int16=%i, uint32=%i, float32=%i, float64=%i\n",
                array.dtype() == nb::dtype<int16_t>(),
                array.dtype() == nb::dtype<uint32_t>(),
                array.dtype() == nb::dtype<float>(),
                array.dtype() == nb::dtype<double>());
}
