cppRange
========

A C++ static multi-dimentional range calculation library.

This library provides a set of operations on static multi-dimension range expressions.

A mult-dimension static range can be expressed as:

1-bit:           [3:3] or [3]
1-dimension:     [3:0]
Multi-dimension: [5:0][2:-1]

* range expression is always expressed as [higher bound : lower bound]. if the lower bound is actually larger than the higher bound, it is illegal, such as [2:3].

The supported calculations:

Belong to
    [7:0] > [3:2]                               true
    [7:0][4:0][3:-2] > [1:0][3][1:0]            true
    [5:3][2:0] > [5:1][2:0]                     false
    [5:3][2:0] > [5:1]                          false (illegal operation)
* the two range expression must have the same number of dimensions, otherwise illegal.