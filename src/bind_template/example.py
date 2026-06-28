



import matplotlib.pyplot as plt

import scipy.fft as sf


from scipy.signal import butter, filtfilt, freqz, hilbert, lfilter
import numpy as np


from logging import getLogger
from scipy import interpolate

logger = getLogger(__name__)


def interpol_at_new_x(a_x, a_y, new_x, kind="linear"):
    """
    Interpolation of discreet function F defined by set of point F(a_x)=a_y for new_x value
    and set to zero outside interval definition a_x

    :param a_x (float, (N)): F(a_x) = a_y, N size of a_x
    :param a_y (float, (N)): F(a_x) = a_y
    :param new_x (float, (M)): new value of x

    :return: F(new_x) (float, (M)): interpolation of F at new_x
    """
    func_interpol = interpolate.interp1d(a_x, a_y, kind, bounds_error=False, fill_value=(0.0, 0.0))
    return func_interpol(new_x)



def find_max_with_parabola_interp_3pt(x_trace, y_trace, idx_max):
    """Parabolic interpolation of the maximum with 3 points

    trace : all values >= 0

    :param x_trace:
    :param y_trace:
    algo Mode pic, input 3 values and the middle one is max:
        parabola : ax^2 + bx + c
        offset of (x0, y0)
        solve coef a, b , interpolation of the maximum is
          x_m = x0 - b/2a
          y_m = y0 - b^2/4a
    :param idx_max: index of sample max, idx_max < nb_sample
    :type idx_max: int
    :return: x_max, y_max
    """
    if (idx_max >= len(x_trace) - 1) or idx_max == 0:
        return x_trace[idx_max], y_trace[idx_max]
    logger.debug(f"Parabola interp: mode pic {idx_max} {len(x_trace)}")
    # remove offset (x0, v0)
    y_pic = y_trace[idx_max : idx_max + 2] - y_trace[idx_max - 1]
    x_pic = x_trace[idx_max : idx_max + 2] - x_trace[idx_max - 1]
    logger.debug(x_trace[idx_max : idx_max + 2])
    logger.debug(y_trace[idx_max : idx_max + 2])
    # solve coef a, b
    r_pic = y_pic / x_pic
    c_a = (r_pic[1] - r_pic[0]) / (x_pic[1] - x_pic[0])
    c_b = r_pic[0] - c_a * x_pic[0]
    # interpolation of the maximum is
    x_m = -c_b / (2 * c_a)
    x_max = x_trace[idx_max - 1] + x_m
    y_max = y_trace[idx_max - 1] + x_m * c_b / 2
    return x_max, y_max
