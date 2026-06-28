#pragma once


class ProcessNdarray {
public:
    ProcessNdarray() = default;

    // add scalar for all elements of array of double, return size of array
    int add_in_place(array<double>, double scalar);

    // print shape and dtype
    int inspect(?? array) const;
};
