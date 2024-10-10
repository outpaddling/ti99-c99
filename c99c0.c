
/*
** c99 compiler for the TI99/4A
**
** (c) 1985, 1987 by Clint Pulley
**
** Edit # 167  : 25-Oct-87  1110
** Edit # ???  : 28-Dec-99  0100  (Kristine Rogers)
**
** Based on Jeff Lomicka's version (82/01/25)
** of the small-c compiler by Ron Cain
**
** Extensions based on the work of Jim Hendrix
*/
/*	c99 Update History
Version  Date    Changes and new features:
  1.3  85/09/09  -Original release version.
  1.31 85/11/24  -Improves code for ++ and -- operators.
  1.32 85/12/01  -Implements \ completely.
                 -Allows quoted filenames in #include.
  1.4  85/12/11  -Improves generated code by eliminating pop from R9 whenever
                  possible (revised CSUP).
                 -Generates DEF MAIN automatically.
                 -Displays errors on screen and reports only the first error
                  a statement.
                 -Has default screen colors of white/blue.
  1.41 85/12/22  -Corrects problems with pointer arithmetic & logical operators.
                 -Changes compiler screen display.
                 -Outputs literals after each function.
                 -Partially implements , operator.
  1.5  86/01/05  -Allows local declarations only at start of a block.
                 -Parses argument declarations of the form (*func)().
                 -Implements do/while.
  1.51 86/01/15  -Converts character type test and string compare functions to
                  assembly code for memory reduction and faster compiles.
                 -Allows _ in names.
  1.6  86/01/19  -Implements switch/case (revised CSUP).
                 -Fully implements , operator.
  1.7  86/01/26  -Implements for.
                 -Removes #outseg.
  1.8  86/02/01  -Implements initializers on global declarations.
  1.9  86/02/01  -Provides input_line push code option.
  2.0  86/02/02  -Provides descriptive error messages on screen.
  2.1x 86/08/08  -Implements 2 dimensional arrays.
                 -Implements extern and entry.
  2.1  86/10/14  -Corrects problem with generated labels for initializers when
                  c-text included.
  2.2  87/01/18  -Implements conditional compile directives.
                 -Expands static symbol table to 256 entries.
  2.3  87/03/05  -Implements &&, || with early dropout.
                 -Eliminates use 0f 99/4A Pad addresses in generated code.
                 -Uses hash coding for global symbol table access.
  3.0  87/04/18  -Eliminates generation of ABS instruction before conditionals.
                 -Corrects problem with Pad address elimination.
                 -Treats while(1) as a special case.
                 -Revises indirect call to not use R10.
                 -Changes code for byte (char) load/store using R10 as pointer
                  to ls byte of R8.
                 -Requires revised CSUP.
  3.1  87/10/10  -Implements pointer arrays.
                 -Implements conditional expression operator.
  3.2  87/10/10  -Implements <gasp!> goto.
                 -Reports type of missing token.
  3.3  87/10/13  -Corrects pointer array implementation.
  4.0  87/10/24  -Final 99/4A version.
                 -Reports lines processed, global and local symbol counts.
                 -Implements hex constants.
                 -Issues "call files(3)" during initialization.
                 -Corrects problem with decimal input.
*/
/* external linkage */
#ifdef __POWERC
	#define	_DOS
#endif

#if defined (_DOS) || defined (UNIX)
	#include <stdio.h>
#else
	entry line,lptr;  /* for C99CSB */
	extern fopen(),fclose(),cfread(),putc();  /* in CCFIO */
	extern getlit(),putlit(),getfn(),alpha(); /* in C99CSB */
	extern numeric(),an(),astreq(),streq(),putint(),getint(),hash();
#endif
/*
** global storage
*/
#if !defined (_DOS) && !defined (UNIX)
	#asm
	AORG >8330
	#endasm
#endif

#include "c99.h"


#if !defined (_DOS) && !defined (UNIX)
	#asm
	RORG
	#endasm
	#include "c99c1.c"
	#include "c99c2.c"
	#include "c99c3.c"
/*	#include DSK1.C99C1
	#include DSK1.C99C2
	#include DSK1.C99C3	*/
#endif
