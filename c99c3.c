/*
 * >>>>>> start of c99c3 <<<<<< *
 *
 * * (c) 1987 by Clint Pulley *
 *
 * * Last Edit 87/03/05  2300 * Edit # ???  : 28-Dec-99  0100  (Kristine
 * Rogers)
 */

#include <stdio.h>
#include <sysexits.h>
#include <stdlib.h>

#ifdef __POWERC
#define _DOS
#endif

#if _DOS || UNIX
#include <stdio.h>
#include "c99proto.h"
#ifdef _DOS
#pragma input_line_depth (250)
#pragma input_line_recursion (on)
#endif
#endif

#include "c99.h"
#include "c99proto.h"

short
heir7(short lval[])

{
    short	    k;

    short	    lval2[4];

    k = heir8(lval);
    blanks();
    if ((streq(">>") == 0) && (streq("<<") == 0))
    {
	return (k);
    }
    if (k != 0)
    {
	rvalue(lval);
    }
    while (1)
    {
	if (match(">>") == YES)
	{
	    push();
	    if (heir8(lval2) != 0)
	    {
		rvalue(lval2);
	    }
	    pop();
	    dcall("ASR");	/* shift right (R8) bits */
	    continue;
	}
	if (match("<<") == YES)
	{
	    push();
	    if (heir8(lval2) != 0)
	    {
		rvalue(lval2);
	    }
	    pop();
	    dcall("ASL");	/* shift left (R8) bits */
	    continue;
	}
	return (0);
    }
}


short
heir8(short lval[])

{
    short	    k;
    short	    lval2[4];

    k = heir9(lval);
    blanks();
    if ((cur_ch() != '+') && (cur_ch() != '-'))
    {
	return (k);
    }
    if (k != 0)
    {
	rvalue(lval);
    }
    while (1)
    {
	if (match("+") == YES)
	{
	    push();
	    if (heir9(lval2) != 0)
	    {
		rvalue(lval2);
	    }
	    if (isword(lval) == YES)
	    {
		doublereg();
	    }
	    pop();
	    add();
	    continue;
	}
	if (match("-") == YES)
	{
	    push();
	    if (heir9(lval2) != 0)
	    {
		rvalue(lval2);
	    }
	    if ((isword(lval) == YES) && (isword(lval2) == NO))
	    {
		doublereg();
	    }
	    pop();
	    ot("S");
	    if (rtext == YES)
	    {
		ot("*R14+, R8");
	    }
	    else
	    {
		ot("*14+, 8");
	    }
	    newline();
	    ot("NEG");
	    if (rtext == YES)
	    {
		ot("R8");
	    }
	    else
	    {
		ot("8");
	    }
	    newline();
	    if ((isword(lval) == YES) && (isword(lval2) == YES))
	    {
		ot("SRA");
		if (rtext == YES)
		{
		    ol("R8, 1");
		}
		else
		{
		    ol("8, 1");
		}
	    }

#if _DOS || UNIX
	    needst = NO;
#else
#asm
	    CLR @ NEEDST
#endasm
#endif

		continue;
	}
	return (0);
    }
}


short
heir9(short lval[])

{
    short	    k;
    short	    lval2[4];

    k = heir10(lval);
    blanks();
    if ((cur_ch() != '*') && (cur_ch() != '/') && (cur_ch() != '%'))
    {
	return (k);
    }
    if (k != 0)
    {
	rvalue(lval);
    }
    while (1)
    {
	if (match("*") == YES)
	{
	    push();
	    if (heir9(lval2) != 0)
	    {
		rvalue(lval2);
	    }
	    pop();
	    ot("MPY");
	    if (rtext == YES)
	    {
		ol("*R14+, R8");
	    }
	    else
	    {
		ol("*14+, 8");
	    }
	    ot("MOV");
	    if (rtext == YES)
	    {
		ol("R9, R8");
	    }
	    else
	    {
		ol("9, 8");
	    }
#if _DOS || UNIX
	    needst = NO;
#else
#asm
	    CLR @ NEEDST
#endasm
#endif
		continue;
	}
	if (match("/") == YES)
	{
	    push();
	    if (heir10(lval2) != 0)
	    {
		rvalue(lval2);
	    }
	    pop();
	    dcall("DIV");
	    continue;
	}
	if (match("%") == YES)
	{
	    push();
	    if (heir10(lval2) != 0)
	    {
		rvalue(lval2);
	    }
	    pop();
	    dcall("REM");
	    continue;
	}
	return (0);
    }
}


short
heir10(short lval[])

{
    short	    my_ptr;
    short	    k;

    char	   *table_base;

    if (match("++") == YES)
    {
	if (heir10(lval) == 0)
	{
	    needlval();
	    return (0);
	}
	step1(lval, 1);
	return (0);
    }
    if (match("--") == YES)
    {
	if (heir10(lval) == 0)
	{
	    needlval();
	    return (0);
	}
	step1(lval, 0);
	return (0);
    }
    if (match("-") == YES)
    {
	if (heir10(lval) != 0)
	{
	    rvalue(lval);
	}
	ot("NEG");
	if (rtext == YES)
	{
	    ol("R8");
	}
	else
	{
	    ol("8");
	}
#if _DOS || UNIX
	needst = NO;
#else
#asm
	CLR @ NEEDST
#endasm
#endif
	    return (0);
    }
    if (match("~") == YES)
    {
	if (heir10(lval) != 0)
	{
	    rvalue(lval);
	}
	ot("INV");
	ol("8");
#if _DOS || UNIX
	needst = NO;
#else
#asm
	CLR @ NEEDST
#endasm
#endif
	    return (0);
    }
    if (match("!") == YES)
    {
	if (heir10(lval) != 0)
	{
	    rvalue(lval);
	}
	dcall("LNEG");
	return (0);
    }
    if (match("*") == YES)
    {
	if (heir10(lval) != 0)
	{
	    rvalue(lval);
	}
	lval[1] = CINT;
	lval[2] = 0;
	my_ptr = *lval;
	switch (lval[3])
	{
	case TABLE_GLOBAL:
	    table_base = g_sym_tab;
	    break;

	case TABLE_LOCAL:
	    table_base = l_sym_tab;
	    break;

	case TABLE_MACRO:
	    table_base = macro_pool;
	    break;

	default:
	    break;
	}
	if (my_ptr != 0)
	{
	    lval[1] = table_base[my_ptr + TYPE];
	}
#if _DOS || UNIX
	needst = NO;
#else
#asm
	CLR @ NEEDST
#endasm
#endif
	    return (1);
    }
    if (match("&") == YES)
    {
	if (heir10(lval) == 0)
	{
	    errrpt("BAD ADDRESS");
	    return (0);
	}
	my_ptr = *lval;
	switch (lval[3])
	{
	case TABLE_GLOBAL:
	    table_base = g_sym_tab;
	    break;

	case TABLE_LOCAL:
	    table_base = l_sym_tab;
	    break;

	case TABLE_MACRO:
	    table_base = macro_pool;
	    break;

	default:
	    fprintf(stderr, "%s(): Invalid lval[3].\n", __FUNCTION__);
	    exit(EX_SOFTWARE);
	    break;
	}
	setl2(lval, &table_base[my_ptr]);
	if (lval[1] != 0)
	{
	    return (0);
	}
	address(&table_base[my_ptr]);
	lval[1] = table_base[my_ptr + TYPE];
	return (0);
    }
    k = heir11(lval);
    if (match("++") == YES)
    {
	if (k == 0)
	{
	    needlval();
	    return (0);
	}
	step2(lval, 1);
	return (0);
    }
    if (match("--") == YES)
    {
	if (k == 0)
	{
	    needlval();
	    return (0);
	}
	step2(lval, 0);
	return (0);
    }
    return (k);
}



short
heir11(short lval[])

{
    short	    k;
    short	    my_ptr;

    char	   *table_base;

    k = primary(lval);
    switch (lval[3])
    {
    case TABLE_GLOBAL:
	table_base = g_sym_tab;
	break;

    case TABLE_LOCAL:
	table_base = l_sym_tab;
	break;

    case TABLE_MACRO:
	table_base = macro_pool;
	break;

    default:
	break;
    }
    my_ptr = *lval;
    blanks();
    if ((cur_ch() == '[') || (cur_ch() == '('))
    {
	while (1)
	{
	    if (match("[") == YES)
	    {
		if (my_ptr == 0)
		{
		    cnserr();
		    junk();
		    needbrack("]");
		    return (0);
		}
		else
		{
		    switch (lval[3])
		    {
		    case TABLE_GLOBAL:
			table_base = g_sym_tab;
			break;

		    case TABLE_LOCAL:
			table_base = l_sym_tab;
			break;

		    case TABLE_MACRO:
			table_base = macro_pool;
			break;

		    default:
			break;
		    }
		    if ((table_base[my_ptr + IDENT] == POINTER) ||
			(table_base[my_ptr + IDENT] == PTRAR2))
		    {
			rvalue(lval);
		    }
		    else
		    {
			if ((table_base[my_ptr + IDENT] != ARRAY) &&
			    (isptr(&table_base[my_ptr]) == NO))
			{
			    cnserr();
			    k = 0;
			}
		    }
		}
		push();
		expression();
		needbrack("]");
		switch (lval[3])
		{
		case TABLE_GLOBAL:
		    table_base = g_sym_tab;
		    break;

		case TABLE_LOCAL:
		    table_base = l_sym_tab;
		    break;

		case TABLE_MACRO:
		    table_base = macro_pool;
		    break;

		default:
		    break;
		}
		if (match("[") == YES)	/* check for 2nd subscript */
		{
		    if (rtext == YES)
		    {
			ot("LI R7,");
		    }
		    else
		    {
			ot("LI 7,");
		    }
		    outdcl(getint(&table_base[my_ptr + ROWSIZ]));
		    ot("MPY");	/* Row * Rowsize   */
		    if (rtext == YES)
		    {
			ol("R8, R7");
		    }
		    else
		    {
			ol("8, 7");
		    }
		    push();
		    expression();	/* eval 2nd subscript */
		    needbrack("]");
		    pop();
		    add();	/* column */
		}
		switch (lval[3])
		{
		case TABLE_GLOBAL:
		    table_base = g_sym_tab;
		    break;

		case TABLE_LOCAL:
		    table_base = l_sym_tab;
		    break;

		case TABLE_MACRO:
		    table_base = macro_pool;
		    break;

		default:
		    break;
		}
		if ((table_base[my_ptr + TYPE] == CINT) ||
		    (table_base[my_ptr + IDENT] >= PTRAR1))
		{
		    doublereg();
		}
		pop();
		add();
		if (table_base[my_ptr + IDENT] >= PTRAR1)
		{
		    lval[1] = CINT;
		    setl2(lval, &table_base[my_ptr]);
		}
		else
		{
		    lval[1] = table_base[my_ptr + TYPE];
		    lval[2] = 0;
		}
		k = 1;
		continue;
	    }
	    switch (lval[3])
	    {
	    case TABLE_GLOBAL:
		table_base = g_sym_tab;
		break;

	    case TABLE_LOCAL:
		table_base = l_sym_tab;
		break;

	    case TABLE_MACRO:
		table_base = macro_pool;
		break;

	    default:
		break;
	    }
	    if (match("(") == YES)
	    {
		if (my_ptr == 0)
		{
		    callfunction(NULL);
		}
		else
		{
		    if (table_base[my_ptr + IDENT] != FUNCTION)
		    {
			rvalue(lval);
			callfunction(NULL);
		    }
		    else
		    {
			callfunction(&table_base[my_ptr]);
		    }
		}
		k = 0;
		*lval = 0;
		continue;
	    }
	    return (k);
	}
    }
    if (my_ptr == 0)
    {
	return (k);
    }
    if (table_base[my_ptr + IDENT] == FUNCTION)
    {
	address(&table_base[my_ptr]);
	return (0);
    }
    return (k);
}



short
primary(short *lval)

{
    short	    num;
    short	    k;
    short	    my_lptr;
    short	    save_lptr;
    short	    ptr;
    char	    sname[NAMESIZE];

    if (match("(") == YES)
    {
	do
	{
	    k = heir1(lval);
	}
	while (match(",") == YES);
	needbrack(")");
	return (k);
    }
    lval[2] = 0;

#if _DOS || UNIX
    needst = NO;
#else
#asm
    CLR @ NEEDST
#endasm
#endif

	my_lptr = lptr;
    if (symname(sname) == YES)
    {
	ptr = findloc(sname);
	if (ptr != 0)
	{
	    lval[3] = TABLE_LOCAL;
	    if (l_sym_tab[ptr + IDENT] == LABEL)
	    {
		oocerr();
		return (0);
	    }
	    getloc(&l_sym_tab[ptr]);
	    *lval = ptr;
	    lval[1] = l_sym_tab[ptr + TYPE];
	    if (isptr(&l_sym_tab[ptr]) == YES)
	    {
		lval[1] = CINT;
		setl2(lval, &l_sym_tab[ptr]);
	    }
	    if (l_sym_tab[ptr + IDENT] == ARRAY)
	    {
		setl2(lval, &l_sym_tab[ptr]);
		return (0);
	    }
	    else
	    {
		return (1);
	    }
	}
	ptr = findglb(sname);
	if (ptr != 0)
	{
	    lval[3] = TABLE_GLOBAL;
	    if (g_sym_tab[ptr + IDENT] != FUNCTION)
	    {
		*lval = ptr;
		lval[1] = 0;
		if ((g_sym_tab[ptr + IDENT] != ARRAY) && (g_sym_tab[ptr + IDENT] != PTRAR1))
		{
		    if (isptr(&g_sym_tab[ptr]) == YES)
		    {
			setl2(lval, &g_sym_tab[ptr]);
		    }
		    return (1);
		}
		address(&g_sym_tab[ptr]);
		lval[1] = setl2(lval, &g_sym_tab[ptr]);
		return (0);
	    }
	}
	save_lptr = lptr;
	lptr = my_lptr;
	if (constant(&num) == YES)
	{
	    *lval = 0;
	    lval[1] = 0;
	    return (0);
	}
	lptr = save_lptr;
	ptr = addglb(sname, FUNCTION, CINT, 0, 1);
	lval[3] = TABLE_GLOBAL;
	*lval = (short)ptr;
	lval[1] = 0;
	return (0);
    }
    if (constant(&num) == YES)
    {
	*lval = 0;
	lval[1] = 0;
	return (0);
    }
    else
    {
	errrpt("BAD EXPRESSION");
	clrp();
	junk();
	return (0);
    }
}


short
setl2(short lval[], char pointer[])

{
    lval[2] = pointer[TYPE];
    return (lval[2]);
}


void
step1(short lval[], short up)

{
    updn(lval, up);
    rvalue(lval);

#if _DOS || UNIX
    needst = NO;
#else
#asm
    CLR @ NEEDST
#endasm
#endif

}


void
step2(short lval[], short up)

{

    short	    my_ptr;

    char	   *table_base;

    if (lval[1] != 0)
    {
	switch (lval[3])
	{
	case TABLE_GLOBAL:
	    table_base = g_sym_tab;
	    break;

	case TABLE_LOCAL:
	    table_base = l_sym_tab;
	    break;

	case TABLE_MACRO:
	    table_base = macro_pool;
	    break;

	default:
	    fprintf(stderr, "%s(): Invalid lval[3].\n", __FUNCTION__);
	    exit(EX_SOFTWARE);
	    break;
	}
	my_ptr = *lval;
	if (ischar(&table_base[my_ptr]) == YES)
	{
	    ot("MOVB");
	    if (rtext == YES)
	    {
		ol("*R8, R9");
	    }
	    else
	    {
		ol("*8, 9");
	    }
	    updn(lval, up);
	    ot("MOV");
	    if (rtext == YES)
	    {
		ol("R9, R8");
	    }
	    else
	    {
		ol("9, 8");
	    }
	    ot("SRL");
	    if (rtext == YES)
	    {
		ol("R8, R8");
	    }
	    else
	    {
		ol("8, 8");
	    }
	}
	else
	{
	    ot("MOV");
	    if (rtext == YES)
	    {
		ol("*R8, R9");
	    }
	    else
	    {
		ol("*8, 9");
	    }
	    updn(lval, up);
	    ot("MOV");
	    if (rtext == YES)
	    {
		ol("R9, R8");
	    }
	    else
	    {
		ol("9, 8");
	    }
	}

#if _DOS || UNIX
	needst = NO;
#else
#asm
	CLR @ NEEDST
#endasm
#endif

    }
    else
    {
	rvalue(lval);
	updn(lval, up);

#if _DOS || UNIX
	needst = YES;
#else
#asm
	SETO @ NEEDST
#endasm
#endif

    }
}


void
updn(short lval[], short up)

{
    short	    my_ptr;

    char	   *table_base;

    my_ptr = *lval;
    switch (lval[3])
    {
    case TABLE_GLOBAL:
	table_base = g_sym_tab;
	break;

    case TABLE_LOCAL:
	table_base = l_sym_tab;
	break;

    case TABLE_MACRO:
	table_base = macro_pool;
	break;

    default:
	fprintf(stderr, "%s(): Invalid lval[3].\n", __FUNCTION__);
	exit(EX_SOFTWARE);
	break;
    }
    if (ischar(&table_base[my_ptr]) == YES)
    {
	ot("LI");
	if (rtext == YES)
	{
	    ot("R7, >");
	}
	else
	{
	    ot("7, >");
	}
	if (up != 0)
	{
	    outl("0100");
	}
	else
	{
	    outl("FF00");
	}
	ot("AB");
	if (rtext == YES)
	{
	    ot("R7, ");
	}
	else
	{
	    ot("7, ");
	}
	if (lval[1] != 0)
	{
	    if (rtext == YES)
	    {
		outl("*R8");
	    }
	    else
	    {
		outl("*8");
	    }
	}
	else
	{
	    outbyte('@');
	    otlbll(&table_base[my_ptr]);
	}
    }
    else
    {
	if (up != 0)
	{
	    ot("INC");
	}
	else
	{
	    ot("DEC");
	}
	if (isword(lval) == YES)
	{
	    outbyte('T');
	}
	if (lval[1] != 0)
	{
	    if (rtext == YES)
	    {
		ol("*R8");
	    }
	    else
	    {
		ol("*8");
	    }
	}
	else
	{
	    ot("@");
	    otlbll(&table_base[my_ptr]);
	}
    }
}


/*
 * compile &&, || with early dropout
 */

short
		dolog(char *symb, char *opcode, char *dropop, char *endop, short (*heir) (), short lval[])

{
    short	    k;
    short	    hit;
    short	    droplab;

    hit = 0;
    while (1)
    {
	k = heir(lval);
	if (match(symb) == YES)
	{
	    if (hit == 0)
	    {
		++hit;
		droplab = getlabel();
	    }
	    droptst(k, opcode, droplab, lval);
	    continue;
	}
	if (hit != 0)
	{
	    droptst(k, opcode, droplab, lval);
	    ol(endop);
	    ot("JMP");
	    ol("$ + 4");
	    prlbll(droplab);
	    ol(dropop);

#if _DOS || UNIX
	    needst = NO;
#else
#asm
	    CLR @ NEEDST
#endasm
#endif

		lval[0] = 0;
	    lval[1] = 0;
	    lval[2] = 0;
	    return (0);
	}
	return (k);
    }
}


/*
 * test for early dropout from &&, ||
 */

void
droptst(short k, char *opcode, short label, short lval[])

{
    if (k != 0)
    {
	rvalue(lval);
    }
    if (needst)
    {
	ot("ABS");
	if (rtext == YES)
	{
	    ol("R8");
	}
	else
	{
	    ol("8");
	}
    }
    ot(opcode);
    ol("$ + 6");
    jump(label);
}


/*
 * compile conditional expression ( ? a : b )
 */

void
condexp(void)

{
    short	    flab1;
    short	    flab2;

    flab1 = getlabel();	/* false */
    flab2 = getlabel();	/* true  */
    briff(flab1);
    doexpr();
    jump(flab2);
    prlbll(flab1);
    needbrack(":");
    doexpr();
    prlbll(flab2);
}


void
address(char *pointer)

{
    immed();
    outlbl(pointer);
    if (atext == YES)
    {
	while (outcol < 32)
	{
	    tab();
	}
	ot("*   Immediate Value");
    }
    newline();
}


void
store(short lval[])

{
    short	    my_ptr;

    char	   *table_base;

    if (lval[1] == 0)
    {
	my_ptr = *lval;
	switch (lval[3])
	{
	case TABLE_GLOBAL:
	    table_base = g_sym_tab;
	    break;

	case TABLE_LOCAL:
	    table_base = l_sym_tab;
	    break;

	case TABLE_MACRO:
	    table_base = macro_pool;
	    break;

	default:
	    fprintf(stderr, "%s(): Invalid lval[3].\n", __FUNCTION__);
	    exit(EX_SOFTWARE);
	    break;
	}
	putmem(&table_base[my_ptr]);
    }
    else
    {
	putstk(lval[1]);
    }
#if _DOS || UNIX
    needst = NO;
#else
#asm
    CLR @ NEEDST
#endasm
#endif

}


void
rvalue(short lval[])

{

    short	    my_ptr;

    char	   *table_base;

    if (*lval == 0)
    {
	return;
    }
    if (lval[1] == 0)
    {
	my_ptr = *lval;
	switch (lval[3])
	{
	case TABLE_GLOBAL:
	    table_base = g_sym_tab;
	    break;

	case TABLE_LOCAL:
	    table_base = l_sym_tab;
	    break;

	case TABLE_MACRO:
	    table_base = macro_pool;
	    break;

	default:
	    fprintf(stderr, "%s(): Invalid lval[3].\n", __FUNCTION__);
	    exit(EX_SOFTWARE);
	    break;
	}
	getmem(&table_base[my_ptr]);
    }
    else
    {
	indirect(lval[1]);
    }
}


void
test(short label, short paren)

{
    if (paren)
    {
	needbrack("(");
    }
    doexpr();
    if (paren)
    {
	needbrack(")");
    }
    briff(label);
}


/*
 * branch to label if primary register is false
 */

void
briff(short label)

{
    if (needst)
    {
	ot("ABS");	/* Generate status */
	if (rtext == YES)
	{
	    ol("R8");
	}
	else
	{
	    ol("8");
	}
    }
    ot("JNE");	/* Skip if r8 true */
    ol("$ + 6");	/* Skip if r8 true */
    jump(label);
}


short
constant(short *val)

{
    if (hexnum(val) == YES)
    {
	outval(val);
    }
    else
    {
	if (number(val) == YES)
	{
	    outval(val);
	}
	else
	{
	    if (pstr(val) == YES)
	    {
		outval(val);
	    }
	    else
	    {
		if (qstr(val) == YES)
		{
		    immed();
		    printlabel(litlab);
		    if (*val != 0)
		    {
			outbyte('+');
			outdec(*val);
		    }
		    if (atext == YES)
		    {
			while (outcol < 32)
			{
			    tab();
			}
			ot("*   Immediate Value");
		    }
		    newline();
		}
		else
		{
		    return (NO);
		}
	    }
	}
    }
    return (YES);
}


short
constx(short *val)

{
    if (hexnum(val) == YES)
    {
	;
    }
    else
    {
	if (number(val) == YES)
	{
	    ;
	}
	else
	{
	    if (pstr(val) == YES)
	    {
		;
	    }
	    else
	    {
		ntcerr();
		*val = 0;
		return (NO);
	    }
	}
    }
    return (YES);
}


void
outval(short *lval)

{
    if (*lval != 0)
    {
	immed();
	outdec(*lval);
	if (atext == YES)
	{
	    while (outcol < 32)
	    {
		tab();
	    }
	    ot("*   Immediate Value");
	}
	newline();
    }
    else
    {
	clrp();
    }
}


short
hexnum(short *val)

{
    short	    c;
    short	    k;
    short	    t;

    if ((match("0X") == YES) || (match("0x") == YES))
    {
	k = 0;
	while (1)
	{
	    c = cur_ch();
	    if (numeric(c) != 0)
	    {
		t = 48;
	    }
	    else
	    {
		if ((c >= 'A') & (c <= 'F'))
		{
		    t = 55;
		}
		else
		{
		    if ((c >= 'a') & (c <= 'f'))
		    {
			t = 87;
		    }
		    else
		    {
			break;
		    }
		}
	    }
	    k = (k << 4) + (inbyte() - t);
	}
	*val = k;
	return (YES);
    }
    return (NO);
}


short
number(short *val)

{
    short	    k;
    short	    minus;
    short	    my_char;
    char	    mac_name[LINESIZE];

    char	   *macro;

    k = 0;
    minus = 0;
    if (match("-") == YES)
    {
	minus = 1;
    }
    if (numeric(cur_ch()) == 0)
    {
	if (alpha(cur_ch()) == 0)
	{
	    return (NO);
	}
	if (symname(mac_name) == YES)
	{
	    macro = findmac(mac_name);
	    if ((macro == NULL) || (*macro == (char)0))
	    {
		return (NO);
	    }
	    while (*macro != (char)0)
	    {
		my_char = *macro++;
		my_char &= 0xff;
		if (numeric(my_char) == 0)
		{
		    break;
		}
		my_char &= 0xf;
		k *= 10;
		k += my_char;
	    }
	}
    }
    else
    {
	while (numeric(cur_ch()) != 0)
	{
	    k *= 10;
	    k += inbyte();
	    k -= '0';
	}
    }
    if (minus != 0)
    {
	k = (-k);
    }
    *val = k;
    return (YES);
}


short
pstr(short *val)

{
    short	    c;
    short	    k;

    k = 0;
    if (match("'") == NO)
    {
	return (NO);
    }
    c = litchr();
    while (c != 39)
    {
	if (cur_ch() == 0)
	{
	    break;
	}
	k = k << 8 | c;
	c = litchr();
    }
    *val = k;
    return (YES);
}


short
qstr(short *val)

{
    if (match(quote) == NO)
    {
	return (NO);
    }
    *val = litptr;
    while (cur_ch() != '"')
    {
	if (cur_ch() == 0)
	{
	    break;
	}
	stolit(litchr(), CCHAR);
    }
    get_ch();
    stolit(0, CCHAR);
    return (YES);
}


void
stolit(short val, short siz)

{
    if (litptr >= LITMAX)
    {
	oflerr();
	return;
    }
    putlit(val, litptr, siz);
    litptr = litptr + siz;
}


/*
 * * return current literal char & bump lptr
 */

short
litchr(void)

{
    short	    i;
    short	    c;

    if (cur_ch() != BACKSLASH)
    {
	return (get_ch());
    }
    get_ch();
    switch (cur_ch())
    {
    case 'n':
	get_ch();
	return (LINEFEED);
	break;

    case 't':
	get_ch();
	return (TAB);	/* HT */
	break;

    case 'b':
	get_ch();
	return (BACKSPACE);	/* BS */
	break;

    case 'f':
	get_ch();
	return (FORMFEED);	/* FF */
	break;

    case 'r':
	get_ch();
	return (RETURN);	/* CR */
	break;

    }
    i = 4;
    c = 0;
    while ((--i) & (cur_ch() >= '0') & (cur_ch() < '8'))
    {
	c <<= 3;
	c += get_ch();
	c -= '0';
    }
    if (i == 3)
    {
	return (get_ch());
    }
    else
    {
	return (c);
    }
}


/* */
/* Begin a comment line for the assembler */

void
comment(void)

{
    outbyte(STAR);
}


/*
 * Output assembler info before any code is generated
 */

void
header(void)

{
    outl(logo);
    ot("REF");
    ol("C$CIND, C$DIV, C$REM, C$ASR, C$ASL, C$EQ, C$NE, C$LT, C$LE");
    ot("REF");
    ol("C$GT, C$GE, C$ULT, C$ULE, C$UGT, C$UGE, C$LNEG, C$SWCH");
    ot("REF");
    ol("GETCHA, GETS, PUTCHA, PUTS, LOCATE, POLL, TSCRN, EXIT, C$CALL");
}


/*
 * Output END directive after all code
 */

void
trailer(void)

{
    ol("END");
}


/*
 * output DATA directive
 */

void
ddata(void)

{
    ot("DATA");
    tab();
}


/*
 * fetch a static memory cell into the primary register
 */

void
getmem(char *sym)

{
    if (ischar(sym) == YES)
    {
	ot("CLR");
	if (rtext == YES)
	{
	    ol("R8");
	}
	else
	{
	    ol("8");
	}
	ot("MOVB");
	ot("@");
	outlbl(sym);
	if (rtext == YES)
	{
	    outl(", *R10");
	}
	else
	{
	    outl(", *10");
	}
    }
    else
    {
	ot("MOV");
	ot("@");
	outlbl(sym);
	if (rtext == YES)
	{
	    outl(", R8");
	}
	else
	{
	    outl(", 8");
	}
    }
}


/*
 * Fetch the address of the specified symbol * into the primary register
 */

void
getloc(char *sym)

{
    short	    k;

    k = getint(sym + OFFSET) - stkp;
    ot("MOV");
    if (rtext == YES)
    {
	ol("R14, R8");
    }
    else
    {
	ol("14, 8");
    }
    if (atext == YES)
    {
	while (outcol < 32)
	{
	    tab();
	}
	ot("*   Get Address of Symbol to (Step 1)");
    }
    newline();
    if (k != 0)
    {
	if (k == 2)
	{
	    ot("INCT");
	    if (rtext == YES)
	    {
		ot("R8");
	    }
	    else
	    {
		ot("8");
	    }
	    if (atext == YES)
	    {
		while (outcol < 32)
		{
		    tab();
		}
		ot("*   Get Address of Symbol (Step 2)");
	    }
	}
	else
	{
	    ot("AI");
	    if (rtext == YES)
	    {
		ot("R8, ");
	    }
	    else
	    {
		ot("8, ");
	    }
	    outdec(k);
	    if (atext == YES)
	    {
		while (outcol < 32)
		{
		    tab();
		}
		ot("*   Get Address of Symbol (Step 2)");
	    }
	}
    }
    newline();
}


/*
 * putmem and putstk must not alter the primary register
 */
/*
 * Store the primary register into the specified * static memory cell
 */

void
putmem(char *sym)

{
    if (ischar(sym) == YES)
    {
	ot("MOVB");
	if (rtext == YES)
	{
	    ot("*R10, @");
	}
	else
	{
	    ot("*10, @");
	}
    }
    else
    {
	ot("MOV");
	if (rtext == YES)
	{
	    ot("R8, @");
	}
	else
	{
	    ot("8, @");
	}
    }
    otlbll(sym);
}


/*
 * Store the specified object type in the primary register * at the address
 * on the top of the stack
 */

void
putstk(short typeobj)

{
    pop();
    ot("MOV");
    if (rtext == YES)
    {
	ol("*R14+, R9");
    }
    else
    {
	ol("*14+, 9");
    }
    if (typeobj == CCHAR)
    {
	ot("MOVB");
	if (rtext == YES)
	{
	    ol("*R10, *R9");
	}
	else
	{
	    ol("*10, *9");
	}
    }
    else
    {
	ot("MOV");
	if (rtext == YES)
	{
	    ol("R8, *R9");
	}
	else
	{
	    ol("8, *9");
	}
    }
}


/* Fetch the specified object type indirect through the */
/* primary register into the primary register */

void
indirect(short typeobj)

{
    if (typeobj == CCHAR)
    {
	ot("MOVB");
	if (rtext == YES)
	{
	    ol("*R8, R8");
	}
	else
	{
	    ol("*8, 8");
	}
	ot("SRL");
	if (rtext == YES)
	{
	    ol("R8, R8");
	}
	else
	{
	    ol("8, 8");
	}
    }
    else
    {
	ot("MOV");
	if (rtext == YES)
	{
	    ol("*R8, R8");
	}
	else
	{
	    ol("*8, 8");
	}
    }
}


/* Print partial instruction to load immediate */
/* into the primary register */

void
immed(void)

{
    ot("LI");
    if (rtext == YES)
    {
	ot("R8, ");
    }
    else
    {
	ot("8, ");
    }
}


/* Push the primary register onto the stack */

void
push(void)

{
    if (pushin == YES)
    {
	ot("DECT");
	if (rtext == YES)
	{
	    ot("R14");
	}
	else
	{
	    ot("14");
	}
	if (atext == YES)
	{
	    while (outcol < 32)
	    {
		tab();
	    }
	    ot("*   Stack Push (Part 1)");
	}
	newline();
	ot("MOV");
	if (rtext == YES)
	{
	    ot("R8, *R14");
	}
	else
	{
	    ot("8, *14");
	}
	if (atext == YES)
	{
	    while (outcol < 32)
	    {
		tab();
	    }
	    ot("*   Stack Push (Part 2)");
	}
    }
    else
    {
	ot("BL");
	if (rtext == YES)
	{
	    ot("R15");
	}
	else
	{
	    ot("15");
	}
	if (atext == YES)
	{
	    while (outcol < 32)
	    {
		tab();
	    }
	    ot("*   Stack Push");
	}
    }
    newline();

#if _DOS || UNIX
    stkp -= 2;
#else
#asm
    DECT @ STKP
#endasm
#endif

}


/* Alter stack pointer for pop operation */

void
pop(void)

{

#if _DOS || UNIX
    stkp += 2;
#else
#asm
    INCT @ STKP
#endasm
#endif

}


/* Return from subroutine */

void
ret(void)

{
    ot("B");
    if (rtext == YES)
    {
	ot("*R13");
    }
    else
    {
	ot("*13");
    }
    if (atext == YES)
    {
	while (outcol < 32)
	{
	    tab();
	}
	ot("*   Return From Subroutine");
    }
    newline();
}


/* Direct call to compiler subroutine */

void
dcall(char *sname)

{
    ot("BL");
    ot("@C$");
    outstr(sname);
    if (atext == YES)
    {
	while (outcol < 32)
	{
	    tab();
	}
	ot("*   Direct Call To Subroutine");
    }
    newline();

#if _DOS || UNIX
    needst = NO;
#else
#asm
    CLR @ NEEDST
#endasm
#endif

}


/* Jump to specified internal label number */

void
jump(short label)

{
    ot("B");
    ot("@");
    printlabel(label);
    if (atext == YES)
    {
	while (outcol < 32)
	{
	    tab();
	}
	ot("*   Jump To ");
	printlabel(label);
    }
    newline();
}


/* Modify the stack pointer to the new value indicated */

short
modstk(short newsp)

{
    short	    k;

    if ((newsp & 1) != 0)	/* ensure word boundary */
    {
	if (newsp > 0)
	{
	    newsp++;
	}
	else
	{
	    newsp--;
	}
    }
    k = newsp - stkp;
    if (k == 0)
    {
	return (newsp);
    }
    if (k == 2)
    {
	ot("INCT");
	if (rtext == YES)
	{
	    ot("R14, ");
	}
	else
	{
	    ot("14, ");
	}
	newline();
	return (newsp);
    }
    if (k == -2)
    {
	ot("DECT");
	if (rtext == YES)
	{
	    ot("R14, ");
	}
	else
	{
	    ot("14, ");
	}
	newline();
	return (newsp);
    }
    ot("AI");
    if (rtext == YES)
    {
	ot("R14, ");
    }
    else
    {
	ot("14, ");
    }
    outdcl(k);
    return (newsp);
}


/* Double the primary register */

void
doublereg(void)

{
    ot("A");
    if (rtext == YES)
    {
	ot("R8, R8");
    }
    else
    {
	ot("8, 8");
    }
    if (atext == YES)
    {
	while (outcol < 32)
	{
	    tab();
	}
	ot("*   Left-Shift (Double) Primary Register");
    }
    newline();
}


/* Add stack top to primary register */
void
add(void)

{
    ot("A");
    if (rtext == YES)
    {
	ot("*R14+, R8");
    }
    else
    {
	ot("*14+, 8");
    }
    if (atext == YES)
    {
	while (outcol < 32)
	{
	    tab();
	}
	ot("*   Add Top-of-Stack to Primary Register");
    }
    newline();

#if _DOS || UNIX
    needst = NO;
#else
#asm
    CLR @ NEEDST
#endasm
#endif

}


/* Clear primary reg */
void
clrp(void)

{
    ot("S");
    ot("8, 8");
    if (atext == YES)
    {
	while (outcol < 32)
	{
	    tab();
	}
	ot("*   Clear Primary Register");
    }
    newline();
}

/* <<<<<  End of compiler  >>>>> */
