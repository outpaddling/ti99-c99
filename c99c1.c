/*
 * >>>>>>> start of c99c1 <<<<<<<< *
 *
 * * (c) 1987 by Clint Pulley *
 *
 * * Last Edit 87/03/05  2300 * Edit # ???  : 28-Dec-99  0100  (Kristine
 * Rogers)
 */
/*
 * * Compiler begins execution here
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sysexits.h>

#define PATH_MAX    16

#ifdef __POWERC
#define _DOS
#endif

#if defined (_DOS) || defined (UNIX)
#include <stdio.h>
#include <ctype.h>
#include "c99proto.h"
#ifdef _DOS
#pragma input_line_depth (250)
#pragma input_line_recursion (on)
#endif
#define C99_MAIN
#include "c99.h"
#undef  C99_MAIN
#endif


int     main(int argc, char *argv[])

{
    /* Clear global symbols    */
    for (cptr = 0; cptr < GSYMSZ; cptr++)
    {
	g_sym_tab[cptr] = (char)0;
    }

    /* clear macros    */
    for (cptr = 0; cptr < MACQSIZE; cptr++)
    {
	macro_pool[cptr] = (char)0;
    }

    /* Clear local symbols     */
    for (cptr = 0; cptr < LSYMSZ; cptr++)
    {
	l_sym_tab[cptr] = (char)0;
    }

    locptr = SYMSIZ;    /* clear local symbols             */
    test_mode = NO;
    macptr = MACQ;      /* Clear macro pool                */
    wqindex = 0;        /* Clear "while" queue             */
    swnext = swq;       /* Clear switch table              */

    /* ...all set to zero or NO ...    */
    swactive = NO;      /* Not inside "switch"             */
    litptr = 0; /* Clear literal pool              */
    iflv = 0;   /* Clear conditional compile       */
    skiplv = 0;
    pmode = NO; /* Not in preprocessor             */
    stkp = 0;   /* Stack ptr (relative)    */
    numlin = 0; /* # source lines                  */
    numglbs = 0;        /* # global symbols used   */
    numlocs = 0;        /* # local symbols used    */
    errcnt = 0; /* # errors                                */
    errflg = 0; /* Not skipping errors             */
    eof = NO;   /* Not EOF yet                             */
    input = 0;  /* No input file                   */
    input2 = 0; /* or include file                */
    output = 0; /* No open units                   */
    ncmp = 0;   /* No open compound states */
    nxtlab = 0; /* Set label #                             */

    cmode = YES;        /* Enable preprocessing    */
    decl = -1;  /* Inhibit local declarations      */

    /*
     * *      Compiler body
     */
#if _DOS || UNIX
    /********************************/
    /* Directories are only used       */
    /* for DOS or UNIX compiles.       */
    /********************************/
    base_dir[0] = (char)0;
#endif

    printf("argc = %d\n", argc);
    
    /* get user options                */
    if (argc < 2)
    {
	ask_opts();
    }
    else
    {
	puts("Using CLI");
	char *infile = NULL, *outfile = NULL;
	ctext = atext = rtext = pushin = test_mode = NO;
	outfile = NULL;
	// Output = input - _c + _s
	for (int c = 0; c < argc; ++c)
	{
	    if (argv[c][0] == '-')
	    {
		if (strcmp(argv[c], "--c-text") == 0)
		{
		    ctext = YES;
		}
		else if (strcmp(argv[c], "--assembly-comments") == 0)
		{
		    atext = YES;
		}
		else if (strcmp(argv[c], "--r-prefix") == 0)
		{
		    rtext = YES;
		}
		else if (strcmp(argv[c], "--inline-push") == 0)
		{
		    pushin = YES;
		}
		else if (strcmp(argv[c], "--test") == 0)
		{
		    test_mode = YES;
		}
		else if ( (strcmp(argv[c], "--output") == 0) || 
			  (strcmp(argv[c], "-o") == 0 ) )
		{
		    if ( (argv[c + 1] != NULL) && (argv[c + 1][0] != '-') )
			outfile = argv[c + 1];
		    else
			usage(argv);
		}
		else
		    usage(argv);
	    }
	    else
	    {
		infile = argv[c];
		input = FOPEN(infile, "r");
	    }
	}
	if ( outfile == NULL )
	{
	    static char    default_outfile[PATH_MAX + 1], *p;
	    
	    strlcpy(default_outfile, infile, PATH_MAX + 1);
	    if ( (p = strstr(default_outfile, "_c")) != NULL )
		*p = '\0';
	    strlcat(default_outfile, "_s", PATH_MAX + 1);
	    output = FOPEN(default_outfile, "w");
	    printf("output = %s %d\n", default_outfile, output);
	}
    }

    puts("\nTI-99 filenames cannot contain a '.', so '_' is");
    puts("typically used to separate a filename extension.");
    system("ls *_c");
    if ( input == 0 )
	input = getfn("\nInput", "r");        /* Get initial input file  */
    if ( output == 0 )
	openout();  /* Get output file                 */
    header();   /* Intro (startup) code    */
    preprocess();       /* Start input                             */
    parse();    /* Process ALL input               */
    trailer();  /* Follow-up (exit) code   */
    if (ctext == YES)
    {
	sumstats();     /* Add summary and stats to output file    */
    }
    FCLOSE(output);     /* Close output file       */
    output = 0;
    summary();  /* Summarize compilation   */
    /* Then exit to system     */
}


/*
 * * Process all input text
 */
/*
 * At this level, only static declarations, * defines, includes, and function *
 * definitions are legal...
 */

void
parse(void)

{
    while (eof == NO)   /* Do until no more input  */
    {
	if (amatch("extern", 6) == YES)
	{       /* External declaration    */
	    dodeclare(EXTERNAL);
	    blanks();   /* Force EOF if pending    */
	    continue;
	}
	if (dodeclare(STATIC) == YES)
	{       /* Static declaration      */
	    blanks();   /* Force EOF if pending    */
	    continue;
	}
	if (amatch("entry", 5) == YES)
	{
	    doentry();
	    ns();
	    blanks();   /* Force EOF if pending    */
	    continue;
	}
	if (match("#asm") == YES)
	{       /* assembly-code, just             */
	    doasm();    /* insert into code.               */
	    blanks();   /* Force EOF if pending    */
	    continue;
	}
	if (match("#includ") == YES)
	{       /* Include-file    */
	    doinclude();
	    blanks();   /* Force EOF if pending    */
	    continue;
	}
	if (match("#define") == YES)
	{
	    addmac();
	    blanks();   /* Force EOF if pending    */
	    continue;
	}
	newfunc();
	blanks();       /* Force EOF if pending    */
    }
}


/*
 * * Dump the symbol-table
 */

void
dumpglbsyms(void)

{

    if (ctext == YES)
    {
	if (output != 0)
	{
	    /* Output file is open; add header.        */
	    outcom(" ");
	    outcom(" Global Symbols");
	    outcom(" ");
	}
	else
	{
	    /* Output file is not open; print header.  */
	    ol("\n Global Symbols\n");
	}
	dumpsyms(STARTGLB, ENDGLB);
    }

    return;
}


/*
 * * Dump the local symbol-table
 */

void
dumplocsyms(void)

{

    if (ctext == YES)
    {
	if (output != 0)
	{
	    outcom(" ");
	    outcom(" Local Symbols");
	    outcom(" ");
	}
	else
	{
	    ol("\n Local Symbols\n");
	}
	dumpsyms(STARTLOC, ENDLOC);
    }

    return;
}



/*********
Dump a symbol-table
**********/

void
dumpsyms(char *StartAddr, char *EndAddr)

{

    short           count;
    short           id;
    short           type;
    short           offset;
    short           size;
    short           symbol;
    short           symcount;
    short           pointer;

    /* Print header for Symbol-table   */
    symbol = 0;
    symcount = 0;
    while (&StartAddr[symbol] < EndAddr)
    {   /* For each valid table-entry...   */
	if (alnum(StartAddr[symbol]) != NO)
	{
	    symcount++;
	}
	symbol += SYMSIZ;
    }
    if (symcount == 0)
    {
	/************************************/
	/* No symbols used; advise user.   */
	/************************************/
	if (output != 0)
	{
	    /************************************/
	    /* Send to results(listing)-file.  */
	    /************************************/
	    outcom("    None Used.");
	    outcom(" ");
	}
	else
	{
	    /****************************/
	    /* Send to standard-out.   */
	    /****************************/
	    ol("    None Used.\n");
	}
	return;
    }
    if (output != 0)
    {
	/************************************/
	/* Send to results(listing)-file.  */
	/************************************/
	outcom("    Symbol   RefersTo   Type  Offset   Size ");
	outcom(" ");
    }
    else
    {
	/****************************/
	/* Send to standard-out.   */
	/****************************/
	ol("   Symbol   RefersTo   Type  Offset   Size \n");
    }
    symbol = 0;
    while (&StartAddr[symbol] < EndAddr)
    {   /* For each valid table-entry...   */
	if (alnum(StartAddr[symbol]) != 0)
	{
	    count = 0;
	    pointer = symbol;
	    if (output != 0)
	    {
		/************************************/
		/* If sending to results(listing)- */
		/* file, insert a comment-symbol.  */
		/************************************/
		comment();
	    }

	    /****************************************/
	    /* Tab-out to column 4 if stdout or        */
	    /* Column 6 if results(listing) file.      */
	    /****************************************/
	    outstr("    ");

	    /****************************/
	    /* Print symbol-address.   */
	    /****************************/
	    while (StartAddr[pointer] != (char)0)
	    {
		count++;
		outbyte(StartAddr[pointer]);
		pointer++;
	    }

	    while (count < 9)
	    {
		/****************************************/
		/* Tab-out to column 13(stdout) or 15.     */
		/****************************************/
		count++;
		outbyte(SPACE);
	    }

	    /************************/
	    /* Print Symbol-type.      */
	    /************************/
	    id = (short)StartAddr[symbol + IDENT];
	    id &= 0xff;
	    outstr(SymTypes[id]);

	    /****************************************************/
	    /* Print other Symbol-info ONLY if not a function. */
	    /****************************************************/
	    if (id != FUNCTION)
	    {
		type = (short)StartAddr[symbol + TYPE];
		type &= 0xff;

		/****************************************/
		/* Tab-out to column 24(stdout) or 26.     */
		/****************************************/
		outstr("   ");

		/****************************/
		/* Print Variable-type.    */
		/****************************/
		outstr(VarTypes[type]);
		offset = (unsigned short)getint(&StartAddr[symbol + OFFSET]);
		offset &= 0xffff;

		/****************************************/
		/* Tab-out to column 24(stdout) or 26.     */
		/****************************************/
		outstr("   ");

		/****************************/
		/* Print Variable-offset.  */
		/****************************/
		count = outdec(offset);

		/****************************************/
		/* Tab-out to column 31(stdout) or 33.     */
		/****************************************/
		while (count < 6)
		{
		    count++;
		    outbyte(SPACE);
		}
		size = getint(&StartAddr[symbol + ROWSIZ]);
		size &= 0xffff;

		/****************************************/
		/* Tab-out to column 39(stdout) or 41.     */
		/****************************************/
		outstr("  ");

		/****************************/
		/* Print Variable's size.  */
		/****************************/
		outhex(size);
	    }
	    outbyte(LINEFEED);
	}
	symbol += SYMSIZ;
    }
}


/*
 * * Dump the macro-table
 */

void
dumpmacs()

{

    short           count;

    //short mac_count;
    short           index;

    /*
     * short    type; short    offset; short    size; short    pointer;
     */
    if (ctext == NO)
    {
	return;
    }
    if (output != 0)
    {
	/************************************/
	/* Send to results(listing)-file.  */
	/************************************/
	outcom(" ");
	outcom(" Macros");
	outcom(" ");
	outcom("    Name     Definition ");
	outcom(" ");
    }
    else
    {
	/****************************/
	/* Send to standard-out.   */
	/****************************/
	ol("\n Macros\n");
	ol("   Name     Definition \n");
    }
    //mac_count = 0;
    index = 0;
    while (index < MACQSIZE)
    {
	count = 0;
	if (output != 0)
	{
	    comment();
	}
	outstr("    ");
	while (macro_pool[index] != (char)0)
	{
	    count++;
	    outbyte(macro_pool[index]);
	    index++;
	}
	index++;
	while (count < 9)
	{
	    count++;
	    outbyte(SPACE);
	}
	while (macro_pool[index] != (char)0)
	{
	    outbyte(macro_pool[index]);
	    index++;
	}
	index++;
	outbyte(LINEFEED);
	if (macro_pool[index] == (char)0)
	{
	    break;
	}
    }
}


/*
 * * Dump the literal pool
 */

void
dumplits(short typ)

{
    short           j;
    short           index;

    index = 0;  /* init an index... */
    while (index < litptr)      /* to loop with */
    {
	if (typ == CCHAR)
	{
	    ot("BYTE");
	    tab();
	    j = 12;
	}
	else
	{
	    ddata();
	    j = 8;
	}
	while (j--)
	{
	    outdec(getlit(index, typ));
	    index = index + typ;
	    if ((j == 0) || (index >= litptr))
	    {
		newline();      /* need <cr> */
		break;
	    }
	    outbyte(',');       /* item separator */
	}
    }
}


/*
 * * Report at end of file
 */

void
sumstats()

{
    /* see if anything left hanging... */
    if (ncmp != 0)
    {   /* Delete this {   */
	errrpt("MISSING }");
    }
    if (output == 0)
    {
	return;
    }
    outcom(" ");
    outstr("*   ");
    outdec(numlin);
    ol("source lines processed");
    outcom(" ");
    outstr("*   ");
    outdec(numglbs);
    ol("global symbols used");
    outstr("*   ");
    outdec(numlocs);
    ol("(max) local symbols used");
    outcom(" ");
    switch (errcnt)
    {
    case 0:
	outcom("   No errors found");
	break;

    case 1:
	outcom("   One error found");
	break;

    default:
	outstr("*   ");
	outdec(errcnt); /* total # errors */
	ol("errors found");
	break;
    }
    outcom(" ");
#if _DOS || UNIX
    dumpglbsyms();
    dumpmacs();
#endif
}


/*
 * * Report for user
 */

void
summary()

{
    /* see if anything left hanging... */
    if (ncmp != 0)
    {   /* Delete this {   */
	errrpt("MISSING }");
    }
    pl("\n\nc99 Compilation Complete\n");
    outdec(numlin);
    ol("source lines processed\n");
    outdec(numglbs);
    ol("global symbols used");
    outdec(numlocs);
    ol("(max) local symbols used\n");
    if (errcnt != 0)
    {
	if (errcnt == 1)
	{
	    outdec(errcnt);     /* total # errors */
	    ol(" One error found\n");
	}
	outdec(errcnt); /* total # errors */
	ol("errors found\n");
    }
    else
    {
	outl("No errors found\n");
    }
}


/*
 * * Get options from user
 */

void
ask_opts(void)

{
    kill();     /* clear input line */
    pl(logo);   /* print banner */
    newline();
    /* Set defaults to NO      */
    atext = NO;
    rtext = NO;
    ctext = NO;
    pushin = NO;

    /* Does user want C source code as comments?       */
    pl("Include c-text? [n] ");

#if _DOS || UNIX
    tigets(line, LINEMAX);      /* Get answer      */
#else
    gets(line); /* Get answer      */
#endif

    if (isy() == YES)
    {
	ctext = YES;    /* User said YES   */
    }

    /* Does user want assembly-language comments?      */
    PUTS("Add assembly-language comments to outfile? [n] ");
#if _DOS || UNIX
    tigets(line, LINEMAX);      /* Get answer      */
#else
    gets(line); /* Get answer      */
#endif
    if (isy() == YES)
    {
	atext = YES;    /* User said YES   */
    }

    /* Does user want assembly-language comments?      */
    PUTS("Use \'r\' before register-numbers in outfile? [n] ");
#if _DOS || UNIX
    tigets(line, LINEMAX);      /* Get answer      */
#else
    gets(line); /* Get answer      */
#endif
    if (isy() == YES)
    {
	rtext = YES;    /* User said YES   */
    }


    PUTS("Inline push code? [n] ");
#if _DOS || UNIX
    tigets(line, LINEMAX);      /* Get answer      */
#else
    gets(line); /* Get answer      */
#endif

    if (isy() == YES)
    {
	pushin = YES;   /* User said YES   */
    }

    PUTS("Test-mode? [n] ");

#if _DOS || UNIX
    tigets(line, LINEMAX);      /* Get answer      */
#else
    gets(line); /* Get answer      */
#endif

    if (isy() == YES)
    {
	test_mode = YES;        /* User said YES   */
    }
    kill();     /* Erase line      */
}


/*
 * * Get output filename
 */

void
openout(void)

{
    puts("\nCommon practice is to replace '_c' in the source filename");
    puts("with '_s' for the assembly language output.\n");
    output = getfn("Output", "w");
}


/*
 * * Open an include file
 */

void
doinclude(void)

{
    char           *cp;

    blanks();   /* skip over to name */

    cp = &line[lptr];
    if (*cp == '"')
    {
	++lptr;
	while (*++cp != '"')
	{
	    ;
	}
	*cp = (char)0;
    }
    input2 = FOPEN(line + lptr, "r");
    if (input2 == 0)
    {
	errrpt("BAD FILENAME");
    }
    kill();     /* clear rest of line */
    /* so next read will come from new file (if open */
}


/*
 * test for global declarations
 */

short
dodeclare(short class)

{
    if (amatch("char", 4) == YES)
    {
	declglb(CCHAR, class);
	ns();
	return (YES);
    }
    else
    {
	if ((amatch("int", 3) == YES) || (class == EXTERNAL))
	{
	    declglb(CINT, class);
	    ns();
	    return (YES);
	}
    }
    return (NO);
}


/*
 * * Declare a static variable *   (i.e. define for use)
 */
/*
 * makes an entry in the symbol table so subsequent * references can call
 * symbol by name
 */

void
declglb(short type, short class)        /* typ is CCHAR or CINT */

{

    short           symtyp;
    short           varsize;
    short           r;

    do
    {
	r = 0;
	if (endst() == YES)
	{
	    break;      /* do line */
	}
	if ((match("*") == YES) || (match("(*") == YES))        /* pointer ? */
	{
	    symtyp = POINTER;   /* yes     */
	    varsize = 0;
	}
	else
	{
	    symtyp = VARIABLE;  /* no      */
	    varsize = 1;
	}
	if (symname(ssname) == NO)      /* name ok? */
	{
	    illname();  /* no... */
	}
	if (findglb(ssname) != 0)       /* already there? */
	{
	    multidef();
	}
	match(")");
	if (match("()") == YES)
	{
	    symtyp = FUNCTION;
	    varsize = FUNCTION;
	}
	else
	{
	    if (match("[") == YES)      /* array? */
	    {
		r = cdim2();
		varsize = r * needsub();        /* !0 = array      */
		if (symtyp == POINTER)
		{
		    symtyp = PTRAR1;
		}
		else
		{
		    symtyp = ARRAY;
		}
	    }
	}
	if (class == EXTERNAL)
	{
	    ot("REF");
	    tab();
	    otlbll(ssname);
	}
	else
	{
	    if (symtyp != FUNCTION)
	    {
		symtyp = initials(type, symtyp, varsize);
		if (type == CCHAR)
		{
		    ol("EVEN");
		}
	    }
	}
	addglb(ssname, symtyp, type, varsize, r);       /* add symbol */
    } while (match(",") == YES);
}


/*
 * * Declare local variables * (i.e. define for use)
 */
/*
 * * adds symbol table entry with appropriate * stack offset to find it again
 */

void
declloc(short type)     /* typ is CCHAR or CINT */

{
    short           varsize;
    short           symtyp;
    short           t;
    short           r;

    if ((decl < 0) || (swactive == YES) || (noloc != 0))
    {
	oocerr();       /* illegal in switch or with goto */
    }
    do
    {
	r = 0;
	if (endst() == YES)
	{
	    return;
	}
	varsize = 2;
	if (match("*") == YES)
	{
	    symtyp = POINTER;
	}
	else
	{
	    symtyp = VARIABLE;
	}
	if (symname(ssname) == NO)
	{
	    illname();
	}
	if (findloc(ssname) != 0)
	{
	    multidef();
	}
	if (match("[") == YES)
	{
	    t = needsub();
	    if (t != 0)
	    {   /* Dimension was specified; use it.        */
		if (symtyp == POINTER)
		{
		    symtyp = PTRAR1;
		}
		else
		{
		    symtyp = ARRAY;
		}
		r = cdim2();    /* check for second dimension.     */
		t = t * r;
		if ((type == CINT) || (symtyp == PTRAR1))
		{
		    varsize = t + t;
		}
		else
		{
		    varsize = t + (short)(t & 1);
		}
	    }
	    else
	    {   /* Dimension was NOT specified.    */
		if (symtyp == POINTER)
		{
		    symtyp = PTRAR2;
		}
		else
		{
		    symtyp = POINTER;
		}
	    }
	}
	decl = decl + varsize;
	addloc(ssname, symtyp, type, stkp - decl, r);
    } while (match(",") == YES);
}

/*
 * * Get required array size
 */
/*
 * invoked when declared variable is followed by "[" * this routine makes
 * subscript the absolute * size of the array.
 */

short           nsval;

/**/

short
needsub(void)

{
    if (match("]") == YES)
    {
	return (0);     /* NULL size */
    }
    if (number(&nsval) == NO)   /* go after a number */
    {
	ntcerr();       /* it isn't                */
	nsval = 1;      /* so force one    */
    }
    if (nsval < 0)
    {
	errrpt("ILLEGAL SIZE");
	nsval = (-nsval);
    }
    needbrack("]");     /* force single dimension */
    return (nsval);     /* and return size */
}


/* Generate DEF directives for entry points        */

void
doentry(void)

{
    do
    {
	if (endst() == YES)
	{
	    return;
	}
	if (symname(ssname) == NO)
	{
	    illname();
	}
	ot("DEF");
	tab();
	otlbll(ssname);
    } while (match(",") == YES);
}


/*
 * * Check for second dimension
 */
/*
 * return subscript value if found * return 1 if not found or error
 */

short
cdim2(void)

{
    short           r;

    if (match("[") == YES)
    {
	r = needsub();
	if (r != 0)
	{
	    return (r);
	}
	else
	{
	    errrpt("DIMENSION NEEDED");
	}
    }
    return (1);
}


/*
 * * Begin a function
 */
/*
 * Called from "parse" this routine tries to make a function * out of what
 * follows.
 */

void
newfunc(void)

{
    short           my_ptr;

    lastst = 0;
    litptr = 0;
    nogo = 0;
    noloc = 0;
    numltf = 0;
    /* Ignore return-type; assume it ALWAYS short      */
    if (match("int") == YES)
    {
	match("*");
    }
    if (match("void") == YES)
    {
	match("*");
    }
    if (match("char") == YES)
    {
	match("*");
    }
    /* Get next slot in label table.   */
    litlab = getlabel();
    if (symname(ssname) == NO)
    {
	illname();
	kill(); /* invalidate line */
	return;
    }
    my_ptr = findglb(ssname);
    if (my_ptr != 0)    /* already in symbol table?        */
    {
	if (g_sym_tab[my_ptr + IDENT] != FUNCTION)
	{
	    multidef();
	    /* already variable by that name */
	}
	else
	{
	    if (g_sym_tab[my_ptr + OFFSET] == FUNCTION)
	    {
		multidef();
		/* already function by that name */
	    }
	    else
	    {
		g_sym_tab[my_ptr + OFFSET] = FUNCTION;
		/* otherwise we have what was earlier */
		/* assumed to be a function */
	    }
	}
    }
    /* if not in table, define as a function now       */
    else
    {
	addglb(ssname, FUNCTION, CINT, FUNCTION, 1);
    }
    /* we had better see open paren for args...        */
    if (match("(") == NO)
    {
	errrpt("MISSING (");
    }
    if (astreq(ssname, "main", 4) != 0)
    {
	ol("DEF MAIN");
    }
    otlbll(ssname);
    pl(ssname); /* print function name */
    /* ** Clear local stack pointer */
    locptr = SYMSIZ;
    argstk = 0; /* init arg count */
    while (match(")") == NO)    /* then count args */
	/* any legal name bumps arg count */
    {
	if (match("void") == YES)
	{
	    match("*");
	    if (alnum(cur_ch()) == 0)
	    {
		continue;
	    }
	}
	if (symname(ssname) == YES)
	{
	    if (findloc(ssname) != 0)
	    {
		multidef();
	    }
	    else
	    {
		addloc(ssname, LABEL, LABEL, argstk, 1);
		argstk = (short)(argstk + 2);
	    }
	}
	else
	{
	    illname();
	    junk();
	}
	blanks();
	/* if not closing paren, should be comma */
	if (streq(")") == 0)
	{
	    if (match(",") == NO)
	    {
		ndcerr();
	    }
	}
	if (endst() == YES)
	{
	    break;
	}
    }
    stkp = 0;   /* preset stack ptr */
    argtop = argstk;
    while (argstk != 0)
	/* now let user declare what types of things */
	/* those arguments were */
    {
	if (amatch("char", 4) == YES)
	{
	    getarg(CCHAR);
	    ns();
	}
	else
	{
	    if (amatch("int", 3) == YES)
	    {
		getarg(CINT);
		ns();
	    }
	    else
	    {
		errrpt("MISSING ARGS");
		break;
	    }
	}
    }
    statement();
    if ((lastst != STRETURN) && (lastst != STGOTO))
    {
	ret();
    }
    if (litptr != 0)
    {
	printlabel(litlab);
	dumplits(CCHAR);
	ol("EVEN");
    }
    if (numltf > numlocs)
    {
	numlocs = numltf;
    }
}


/*
 * * Declare argument types
 */
/*
 * called from "newfunc" this routine adds an entry in the * local symbol
 * table for each named argument * Function completely rewritten  rev. 1P
 */

void
getarg(short t) /* t = CCHAR or CINT */

{
    short           argptr;
    short           legalname;
    short           j;
    short           r;
    short           data;

    while (1)
    {
	if (argstk == 0)
	{
	    return;     /* no more args */
	}
	if ((match("*") == YES) || (match("(*") == YES))
	{
	    j = POINTER;
	}
	else
	{
	    j = VARIABLE;
	}
	legalname = symname(ssname);
	if (legalname == NO)
	{
	    illname();
	}
	match(")");     /* parse (*fcn) () */
	match("()");
	if (match("[") == YES)  /* pointer? */
	    /* it is a pointer, so skip all between "[]"       */
	{
	    while (inbyte() != ']')
	    {
		if (endst() == YES)
		{
		    break;
		}
	    }
	    if (j == POINTER)
	    {
		j = PTRAR2;
	    }
	    else
	    {
		j = POINTER;
	    }
	    r = cdim2();
	}
	if (j == VARIABLE)
	{
	    t = CINT;
	}
	if (legalname != NO)
	{
	    argptr = findloc(ssname);
	    if (argptr != 0)
	    {
		/* Add in details of the type and address of */
		/* the name  */
		l_sym_tab[argptr + IDENT] = (char)j;
		l_sym_tab[argptr + TYPE] = (char)t;
		data = (short)getint(&l_sym_tab[argptr + OFFSET]);
		data = argtop - data;
		putint(data, &l_sym_tab[argptr + OFFSET]);
		/*
		 * putint (argtop - getint (argptr + OFFSET), argptr +
		 * OFFSET);
		 */
		putint(r, &l_sym_tab[argptr + ROWSIZ]);
	    }
	    else
	    {
		errrpt("NOT AN ARGUMENT");
	    }
	}
	argstk = (short)(argstk - 2);   /* cnt down */
	if (endst() == YES)
	{
	    return;
	}
	if (match(",") == NO)
	{
	    ndcerr();
	}
    }
}


/*
 * * Statement parser
 */
/*
 * called whenever syntax requires * a statement. * this routine performs
 * that statement * and returns a number telling which one
 */

short
statement(void)

{
    if ((cur_ch() == 0) && (eof == YES))
    {
	return (0);
    }
    if (amatch("char", 4) == YES)
    {
	declloc(CCHAR);
	ns();
	return (lastst);
    }
    if (amatch("int", 3) == YES)
    {
	declloc(CINT);
	ns();
	return (lastst);
    }
    if (decl >= 0)
    {
	stkp = modstk(stkp - decl);
	if (ncmp > 1)
	{
	    nogo = decl;        /* disable goto in inner blocks */
	}
	decl = -1;
    }
    if (match("{") == YES)      /* delete this (}) */
    {
	compound();
	return (lastst);
    }
    if (amatch("if", 2) == YES)
    {
	doif();
	lastst = STIF;
	return (lastst);
    }
    if (amatch("while", 5) == YES)
    {
	dowhile();
	lastst = STWHILE;
	return (lastst);
    }
    if (amatch("do", 2) == YES)
    {
	dodo();
	lastst = STDO;
	return (lastst);
    }
    if (amatch("for", 3) == YES)
    {
	dofor();
	lastst = STFOR;
	return (lastst);
    }
    if (amatch("switch", 6) == YES)
    {
	doswitch();
	lastst = STSWITCH;
	return (lastst);
    }
    if (amatch("case", 4) == YES)
    {
	docase();
	lastst = STCASE;
	return (lastst);
    }
    if (amatch("defaul", 6) == YES)
    {
	dodefault();
	lastst = STDEF;
	return (lastst);
    }
    if (amatch("goto", 4) == YES)
    {
	dogoto();
	lastst = STGOTO;
	return (lastst);
    }
    if (dolabel() == YES)
    {
	lastst = STLABEL;
	return (lastst);
    }
    if (amatch("return", 6) == YES)
    {
	doreturn();
	ns();
	lastst = STRETURN;
	return (lastst);
    }

    if (amatch("break", 5) == YES)
    {
	dobreak();
	ns();
	lastst = STBREAK;
	return (lastst);
    }
    if (amatch("contin", 6) == YES)
    {
	docont();
	ns();
	lastst = STCONT;
	return (lastst);
    }
    if (match(";") == YES)
    {
	errflg = 0;
	return (lastst);
    }
    if (match("#asm") == YES)
    {
	doasm();
	lastst = STASM;
	return (lastst);
    }
    /* if nothing else, assume it's an expression */
    doexpr();
    ns();
    lastst = STEXP;
    return (lastst);
}


/*
 * * Semicolon enforcer
 */
/* called whenever syntax requires a semicolon */

void
ns(void)

{
    if (match(";") == NO)
    {
	errrpt("MISSING ;");
    }
    else
    {
	errflg = 0;
    }
}


/*
 * * evaluate an expression (allow , )
 */

void
doexpr(void)

{
    do
    {
	expression();
    }
    while (match(",") == YES);
}


/*
 * * Compound statement
 */
/* allow any number of statements to fall between "{}" */

void
compound(void)

{
    short           savsp;
    short           savloc;

    savsp = stkp;
    savloc = locptr;
    decl = 0;
    ++ncmp;     /* new level open */
    /* delete this {   */
    while (match("}") == NO)
    {
	if (eof == YES)
	{
	    /* delete this {   */
	    errrpt("MISSING }");
	    break;
	}
	else
	{
	    statement();        /* do one */
	}
    }
    --ncmp;     /* close current level */
    if ((lastst != STRETURN) && (lastst != STGOTO))
    {
	modstk(savsp);
    }
    stkp = savsp;
    cptr = savloc;      /* release symtab space, retaining labels */
    while (cptr < locptr)
    {
	cptr2 = (short)(cptr + SYMSIZ);
	if (l_sym_tab[cptr + IDENT] == LABEL)
	{
	    while (cptr < cptr2)
	    {
		l_sym_tab[savloc++] = l_sym_tab[cptr++];
	    }
	}
	else
	{
	    cptr = cptr2;
	}
    }
    locptr = savloc;
    decl = -1;
}


/*
 * *   "if" statement
 */

void
doif(void)

{
    short           flab1;
    short           flab2;

    flab1 = getlabel(); /* get label for false branch      */
    test(flab1, 1);     /* get expression, and branch false        */
    statement();        /* if true, do a statement         */
    if (amatch("else", 4) == NO)        /* if...else?                                      */
    {
	/* simple "if"...print false label */
	prlbll(flab1);
	return; /* and exit        */
    }
    /* an "if...else" statement.       */
    flab2 = getlabel();
    if ((lastst != STRETURN) && (lastst != STGOTO))
    {
	jump(flab2);    /* bypass false code */
    }
    prlbll(flab1);      /* print false label */
    statement();        /* and do "else" clause */
    prlbll(flab2);      /* print true label */
}


/*
 * * "while" statement
 */

void
dowhile(void)

{
    short           lwq[WQSIZ]; /* allocate local queue */

    addwhile(lwq);      /* add entry to queue      */
    /* (for "break" statement) */
    prlbll(lwq[WQLOOP]);        /* loop label      */
    if (match("(1)") == NO)
    {
	test(lwq[WQLAB], 1);    /* if not while(1), see if true    */
    }
    statement();        /* if so, do a statement   */
    jump(lwq[WQLOOP]);  /* loop to label                   */
    prlbll(lwq[WQLAB]); /* exit label                              */
    delwhile(); /* delete queue entry              */
}


/*
 * * "do" statement
 */

void
dodo(void)

{
    short           lwq[WQSIZ];
    short           top;

    addwhile(lwq);
    top = getlabel();
    prlbll(top);
    statement();
    needbrack("while");
    prlbll(lwq[WQLOOP]);
    test(lwq[WQLAB], 1);
    jump(top);
    prlbll(lwq[WQLAB]);
    delwhile();
    ns();
}


/*
 * * "for" statement
 */

void
dofor(void)

{
    short           lwq[WQSIZ];
    short           lab1;
    short           lab2;

    addwhile(lwq);
    lab1 = getlabel();
    lab2 = getlabel();
    needbrack("(");
    if (match(";") == NO)
    {
	doexpr();       /* expr 1 */
	ns();
    }
    prlbll(lab1);
    if (match(";") == NO)
    {
	test(lwq[WQLAB], 0);    /* expr 2 */
	ns();
    }
    jump(lab2);
    prlbll(lwq[WQLOOP]);
    if (match(")") == NO)
    {
	doexpr();       /* expr 3 */
	needbrack(")");
    }
    jump(lab1);
    prlbll(lab2);
    statement();
    jump(lwq[WQLOOP]);
    prlbll(lwq[WQLAB]);
    delwhile();
}


/*
 * * "switch" statement
 */

void
doswitch(void)

{
    short           lwq[WQSIZ];
    short           endlab;
    short           swact;
    short           swdef;
    short          *swnex;
    short          *swptr;

    swact = swactive;
    swdef = swdefault;
    swnex = swnext;
    swptr = swnext;
    addwhile(lwq);
    wq[wqindex - 2] = 0;        /* clear loop label        */
    needbrack("(");
    doexpr();   /* eval switch expression  */
    needbrack(")");
    swdefault = 0;
    swactive = YES;
    endlab = getlabel();
    jump(endlab);
    statement();        /* cases etc. */
    jump(lwq[WQLAB]);
    prlbll(endlab);
    dcall("SWCH");
    while (swptr < swnext)      /* build dispatch table */
    {
	ddata();
	printlabel(*swptr++);   /* case label */
	outbyte(',');
	outdcl(*swptr++);       /* case value */
    }
    ddata();
    outdcl(0);
    if (swdefault != NO)
    {
	jump(swdefault);
    }
    prlbll(lwq[WQLAB]);
    delwhile();
    swnext = swnex;
    swdefault = swdef;
    swactive = swact;
}


/*
 * * "case" statement
 */

void
docase(void)

{
    short           val;

    if (swactive == NO)
    {
	oocerr();
    }
    if (swnext > SWEND)
    {
	oflerr();
	return;
    }
    *swnext = getlabel();
    prlbll(*swnext++);
    constx(&val);
    *swnext++ = val;
    needbrack(":");
}


/*
 * * "default" statement
 */

void
dodefault(void)

{
    if (swactive == YES)
    {
	if (swdefault != 0)
	{
	    multidef();
	}
    }
    else
    {
	oocerr();
    }
    needbrack(":");
    swdefault = getlabel();
    prlbll(swdefault);
}


/*
 * * "goto" statement
 */

void
dogoto(void)

{
    if (nogo > 0)
    {
	oocerr();       /* illegal with block locals */
    }
    else
    {
	noloc = 1;
    }
    if (symname(ssname) == YES)
    {
	jump(addlbl());
    }
    else
    {
	illname();
    }
    ns();
}


/*
 * * check for "label";
 */

short
dolabel(void)

{
    short           savlpt;

    blanks();
    savlpt = lptr;
    if (symname(ssname) == YES)
    {
	if (get_ch() == ':')
	{
	    prlbll(addlbl());
	    return (YES);
	}
	else
	{
	    lptr = savlpt;
	}
    }
    return (NO);
}


/*
 * * "return" statement
 */

void
doreturn(void)

{
    /* if not end of statement, get an expression */
    if (endst() == NO)
    {
	doexpr();
    }
#if _DOS || UNIX
    dumplocsyms();
#endif
    modstk(0);  /* clean up stk */
    ret();      /* and exit function */
}


/*
 * * "break" statement
 */

void
dobreak(void)

{
    short           my_ptr;

    /* see if any "whiles" are open */
    my_ptr = readwhile(wqindex);
    if (my_ptr == 0)
    {
	return; /* no */
    }
    modstk((wq[my_ptr + WQSP]));        /* else clean up stk ptr */
    jump(wq[my_ptr + WQLAB]);   /* jump to exit label */
}


/*
 * * "continue" statement
 */

void
docont(void)

{
    short           my_ptr;

    my_ptr = wqindex;
    /* see if any "whiles" are open */
    while (1)
    {
	my_ptr = readwhile(my_ptr);
	if (my_ptr == 0)
	{
	    return;     /* no */
	}
	if (wq[my_ptr + WQLOOP])
	{
	    break;
	}
    }
    modstk((wq[my_ptr + WQSP]));        /* else clean up stk ptr */
    jump(wq[my_ptr + WQLOOP]);  /* jump to loop label */
}


/*
 * * "asm" pseudo-statement
 */
/*
 * enters mode where assembly language statement are * passed intact through
 * parser
 */

void
doasm(void)

{
    short           ecks;

    cmode = NO; /* mark mode as "asm" */
    while (1)
    {
	input_line();   /* get and print lines */
	if (match("#endasm") == YES)
	{
	    outcom(line);
	    break;      /* until... */
	}
	if (eof == YES)
	{
	    break;
	}
	ecks = 0;
	while (isspace(line[ecks]) != 0)
	{
	    ecks++;
	}
	ol(&line[ecks]);
    }
    kill();     /* invalidate line */
    cmode = YES;        /* then back to parse level */
}


void    usage(char *argv[])

{
    fprintf(stderr, "Usage: %s\n"
		    "       [--c-text] [--assembly-comments] [--r-prefix]\n"
		    "       [--inline-push] [--test]\n"
		    "       source_c -o source_s\n", argv[0]);
    exit(EX_USAGE);
}
