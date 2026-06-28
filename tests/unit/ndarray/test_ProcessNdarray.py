from bind_template.ndarray.process_ndarray import ProcessNdarray
import numpy as np


def test_ProcessNdarray_add_in_place():
    arr = np.arange(10, dtype=np.float64)
    obj = ProcessNdarray()
    obj.add_in_place(arr, 5)
    assert np.all(arr == np.arange(5, 15, dtype=np.float64))
    