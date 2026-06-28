import numpy as np


from bind_template.example import find_max_with_parabola_interp_3pt

def test_find_max_with_parabola_interp_3pt():
    def parabole(v_x):
        return -10 * v_x * v_x + 20 * v_x + 30

    n_sig = 29
    v_x = np.linspace(0, 3, n_sig)
    epsilon = 0.001
    v_y = parabole(v_x) + np.random.normal(0, epsilon, n_sig)
    idx = np.argmax(v_y)
    x_max, y_max = find_max_with_parabola_interp_3pt(v_x, v_y, idx)
    print(x_max, y_max)
    assert np.isclose(x_max, 1.0, atol=epsilon * 10)
    assert np.isclose(y_max, 40.0, atol=epsilon * 10)