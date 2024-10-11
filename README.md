# ti99-c99

This is a port of Clint Pulley's C99 compiler for the Texas Instruments
TI-99 and related home computers, adapted from Ron Cain's Small-C.
It implements a subset of C from the 1980s + TI-99 extensions such
as embedded TMS9900 assembly language, and should not be confused with
the C99 ISO standard.

The C99 compiler was designed to run on the TI-99, alongside the
Editor/Assembler system.  This is an adaptation of the code by Kristine
Rogers for cross-compiling on Unix or DOS systems.  See readme.txt for
notes on the porting effort.

It is my intention to add TI-99 support to
[plasm](https://github.com/outpaddling/plasm/) so that assembly to
a TI object file can be done in an automated fashion on the PC side
as well.  (This will likely take a while).

Using simple tools on both the Unix and TI-99 side for uploading
the object files to a TI-99, the coding and testing cycle can be
highly automated and greatly accelerated, following the same workflow
used for embedded microcontrollers.
