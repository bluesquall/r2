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


Style
-----
Although I've written `r2` in C, I'm designing most of it in an object-oriented way. To do this, I'm following many [style points from nanomsg], but I haven't yet gone back through to follow all those points strictly. (Pull requests revising things to follow Sustrik's nanomsg style more closely are appreciated, and will probably be merged quickly.)

The most important points to follow (for now) are:

* Classes are defined as raw structs.

* Names of all the functions of the class are prefixed by the class name.

* First argument in the class function should be pointer to the class. The argument should be called 'self'. 

* Function, struct and variable names are in lower case. Constants and macros are in upper case.

* If identifier consists of multiple words, the words are separated by underscores.



[style points from nanomsg]: http://nanomsg.org/development-style.html