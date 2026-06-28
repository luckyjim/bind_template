#include <nanobind/nanobind.h>

namespace nb = nanobind;

void bind_process_scalar(nb::module_&);

NB_MODULE(process_scalar, m) {
    bind_process_scalar(m);
}