R2
==

A collection of C utilities.

Epoch
-----
Provides functions to get epoch in microseconds, milliseconds, or seconds.

Buffer
------
A (very) simple buffer with methods to fill from a file descriptor and to get
a line with various terminators.

Serial
------

Provides a struct and utility functions for serial input and output, 
using the buffer above.

Serial-LCM interface
--------------------

A small utility library to bridge between serial devices and [LCM].
(Requires [LCM] and some [LCM] types.)

Analog-Digital Converter
------------------------
Basically reads integer numbers from a file descriptor, and optionally applies
scale and offset to produce a floating point output.

ADC-LCM interface
-----------------
A small utility library to bridge between ADC devices and [LCM].
(Requires [LCM] and some [LCM] types.)

