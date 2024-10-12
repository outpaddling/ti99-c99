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

It is my intention to add TI-99 support to
[plasm](https://github.com/outpaddling/plasm/) so that assembly to
a TI object file can be done in an automated fashion on the PC side
as well.  (This will likely take a while).

Using simple tools on both the Unix and TI-99 side for uploading
the object files to a TI-99, the coding and testing cycle can be
highly automated and greatly accelerated, following the same workflow
used for embedded microcontrollers.

The current process (rather cumbersome):

1.  Edit a C99 source file with your favorite editor on the PC side,
    and compile it with ti99-c99.  I use an IDE (integrated development
    environment) called [APE](https://github.com/outpaddling/plasm/)
    for editing and compiling.  It recognizes files ending in "_c" or "_C" as
    ti99-c99 source.  The "_c" is somewhat of a convention on the TI-99,
    as filenames cannot contain a '.'.  The '.' is used to separate the
    disk drive from the filename (e.g. DSK1.HELLO_C), much like the :
    in DOS/Windows (e.g. a:hello.c).

2.  Compile the program.  Just press F5 or Esc followed by 'r' in APE.
    This will produce a TI-99 assembly language file, e.g. HELLO_S
    from HELLO_C.

3.  Boot the TI-99 or equivalent into BASIC.

4.  On the TI-99, run the RECV_BAS program provided here.

5.  Upload the assembly language file using the send-to-ti program
    provided here.

6.  Reboot the TI-99 into the editor assembler environment.

7.  Assemble the code uploaded from the PC to create an object file
    that you can run.

The future process, when the cross-assembler is available:

1.  Edit a C99 source file as above.

2.  Compile and assemble the program using ti99-c99.

3.  Using any cartridge that supports running machine code (Editor/Assembler,
    Mini Memory, Extended BASIC), boot the TI-99 into BASIC or Extended BASIC.

4.  On the TI-99, run the RECV_BAS program provided here.

5.  Upload the object code file produced by the cross assembler.
