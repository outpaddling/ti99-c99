# ti99-c99

## Background

This is a port of Clint Pulley's C99 compiler for the Texas Instruments
TI-99 and related home computers, adapted from Ron Cain's Small-C.
It implements a subset of C from the 1980s + TI-99 extensions such
as embedded TMS9900 assembly language, and should not be confused with
the C99 ISO standard.

The C99 compiler was designed to run on the TI-99, alongside the
Editor/Assembler system.  This is an adaptation of the code by Kristine
Rogers for cross-compiling on Unix or DOS systems.  See readme.txt for
notes on the porting effort.

## Usage

The script ti99cc integrates c99, which outputs assembly language,
with xas99.py, a cross-assembler for the TI that outputs a TI-99
object file ready to load and run on the TI-99/4A.  I.e., ti99cc
behaves more or less like the standard Unix cc command.

Using simple tools on both the Unix and TI-99 side for uploading
the object files to a TI-99, the coding and testing cycle can be
greatly accelerated, following the same workflow commonly
used for embedded microcontrollers:

1.  Edit a C99 source file as above.

2.  Compile and assemble the program using ti99-c99.

3.  Using any cartridge that supports running machine code (Editor/Assembler,
    Mini Memory, Extended BASIC), boot the TI-99 into BASIC or Extended BASIC.

4.  On the TI-99, run the RECV_BAS program provided here.

5.  Upload the object code file produced by the cross assembler.
