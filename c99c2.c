/* >>>>>>> start of c99c2 <<<<<<<<< 
**
** (c) 1987 by Clint Pulley
**
** Last Edit 87/03/05  2300
** Edit # ???  : 28-Dec-99  0100  (Kristine Rogers)
*/
/*
** Perform a function call
**
** called from heir11, this routine will either call
** the named function, or if the supplied ptr is
** zero, will call the address in R8
*/

#ifdef __POWERC
	#define _DOS
#endif

#if _DOS || UNIX
	#include <stdio.h>
	#include <ctype.h>
	#ifdef UNIX
		#include <unistd.h>
	#endif
	#ifdef _DOS
		#pragma input_line_depth (250)
		#pragma input_line_recursion (on)
		#include <process.h>
	#endif
	#include <stdlib.h>
	#include "c99proto.h"
#endif

#include "c99.h"



void
callfunction (char *pointer)            /* symbol table entry (or NULL) */

{
	short           nargs;

	nargs = 0;
	blanks ();  /* already saw open paren */
	if (pointer == NULL)
	{
		ot ("MOV"); /* save call addr */
		if (rtext == YES)
		{
			ol ("R8, @C$CALL"); /* save call addr */
		}
		else
		{
			ol ("8, @C$CALL"); /* save call addr */
		}
	}
	while (streq (")") == 0)
	{
		if (endst () == YES)
		{
			break;
		}
		expression ();  /* get an argument */
		push ();
		nargs = (short) (nargs + 2);  /* count args*2 */
		if (match (",") == NO)
		{
			break;
		}
	}
	needbrack (")");
	if (pointer != NULL)
	{
		ot ("BL");
		if (rtext == YES)
		{
			ot ("*R12");
		}
		else
		{
			ot ("*12");
		}
		if (atext == YES)
		{
			while (outcol < 32)
			{
				tab ();
			}
			ot ("*   Call A Function");
		}
		newline ();
		ddata (); 
		otlbll (pointer);
	}
	else
	{
		dcall ("CIND");
	}
	stkp = modstk (stkp + nargs);  /* clean up arguments */

#if _DOS || UNIX
	needst = YES;
#else
	#asm
	SETO @NEEDST  /* set status needed flag */
	#endasm
#endif

}


/* initialize global objects
*/

short
initials (short typ, short id, short dim)

{
	short           saved;
	short           needll;

	litptr = 0;
	needll = 0;
	if (dim == 0)
	{
		dim = -1;
	}
	saved = dim;
	outlbl (ssname);
	if (match ("=") == YES)
	{ 
		newline ();
		litlab = getlabel ();
		if (match ("{") == YES)
		{ 
			while (dim != 0)
			{ 
				needll = init (typ, id, &dim);
				if (match (",") == NO)
				{
					break;
				}
			}
			needbrack ("}");
		}
		else
		{
			needll = init (typ, id, &dim);
		}
	}
	if ((dim == -1) && (dim == saved))
	{ 
		typ = CINT;
		stolit (0, typ);
		if (id == PTRAR1)
		{
			id = PTRAR2;
		}
		else
		{
			id = POINTER;
		}
	}
	if ((id >= POINTER) & (dim > 0))
	{ 
		ot ("BSS");
		tab ();
		outdcl (dim << 1);
	}
	if (needll != 0)
	{
		prlbll (litlab);
	}
	dumplits (typ);
	if ((id < POINTER) & (dim > 0))
	{ 
		ot ("BSS");
		tab ();
		outdcl (dim * typ);
	}
	return (id);
}


/* evaluate one initializer
*/

short
init (short typ, short id,short *dim)

{
	short           val;
	short           nllflg;

	nllflg = 0;
	if (qstr (&val) == YES)
	{ 
		if ((id == VARIABLE) || (typ != CCHAR))
		{
			errrpt ("MUST BE PTR OR ARRAY");
		}
		if (id >= POINTER)
		{
			--*dim;
		}
		else
		{
			*dim = *dim - (litptr - val);
		}
		if (id >= POINTER)
		{ 
			ot ("DATA");
			tab ();
			printlabel (litlab);
			if (val != 0)
			{ 
				outbyte ('+');
				outdec (val);
			}
			newline ();
			++nllflg;
		}
	}
	else
	{
		if (constx (&val) == YES)
		{ 
			if (id >= POINTER)
			{
				errrpt ("CAN'T BE PTR");
			}
			stolit (val, typ);
			--*dim;
		}
	}
	return (nllflg);
}


void
junk (void)

{
	if (alnum (inbyte ()) != 0)
	{
		while (alnum (cur_ch ()) != 0)
		{
			get_ch ();
		}
	}
	else
	{
		while (alnum (cur_ch ()) == 0)
		{
			if (cur_ch () == 0)
			{
				break;
			}
			get_ch ();
		}
	}
	blanks ();
}


short
endst (void)

{
	blanks ();
	if ((streq (";") != 0) || (cur_ch () == 0))
	{
		return (YES);
	}
	return (NO);
}


void
illname (void)

{
	errrpt ("ILLEGAL NAME");
	junk ();
}


void
multidef (void)

{
	errrpt ("ALREADY IN USE");
}



char tknmsg[]="MISSING       ";

void
needbrack (char *str)

{
	char    *tptr;
	if (match (str) == YES)
	{
		return;
	}
	tptr = tknmsg + 8;
	while (*str != (char) 0)
	{
		*tptr++ = *str++;
		*tptr = (char) 0;
	}
	errrpt (tknmsg);
}


void
needlval (void)

{
	errrpt ("NOT AN LVAL");
}


void
cnserr (void)

{
	errrpt ("CAN'T SUBSCRIPT");
}


void
ntcerr (void)

{
	errrpt ("NOT A CONSTANT");
}


void
oflerr (void)

{
	errrpt ("TABLE OVERFLOW");
}


void
oocerr (void)

{
	errrpt ("OUT OF CONTEXT");
}


void
ndcerr (void)

{
	errrpt ("MISSING ,");
}


/* search global symbol table
** return cptr if name found
** return 0 if not found (cptr->new entry)
** set cptr2=0 if table full
*/

short
findglb (char *sname)   /*      Search for symbol in gloabl symbol-table.       */

{
	short           index;

	index = hash (sname);
	index &= GHSHMSK;
	index *= SYMSIZ;
	cptr2 = index;

	cptr = cptr2;
	while (g_sym_tab [cptr] != (char) 0)
	{
		if (astreq (sname, &g_sym_tab [cptr], NAMEMAX) != 0)
		{
			return (cptr);
		}
		cptr = (short) (cptr + SYMSIZ);
		if (cptr >= GSYMSZ)
		{
			cptr = 0;
		}
		if (cptr == cptr2)
		{
			cptr2 = 0;
			return (0);
		}
	}
	return (0);
}

short
findloc (char *sname)   /*      Search for symbol in local symbol-table.        */

{
	short   my_ptr;

	my_ptr = SYMSIZ;
	while (my_ptr < locptr)
	{
		if (astreq (sname, &l_sym_tab [my_ptr], NAMEMAX) != 0)
		{
			return (my_ptr);
		}
		my_ptr = (short) (my_ptr + SYMSIZ);
	}
	return (0);
}


short
addglb (char *sname, short id, short typ, short value, short r)

{
	if (findglb (sname) != 0)
	{
		return (cptr);
	}
	if (cptr2 == 0)
	{
		oflerr ();
		return (0);
	}
	entsym (sname, g_sym_tab, id, typ, value, r);
	++numglbs;
	return (cptr);
}


short
addloc (char *sname, short ident, short vartyp, short value, short r)

{
	cptr = findloc (sname);
	if (cptr != 0)
	{
		return (cptr);
	}
	if (locptr >= LSYMSZ)
	{
		oflerr ();
		return (0);
	}
	cptr = locptr;
	entsym (sname, l_sym_tab, ident, vartyp, value, r);
	locptr = (short) (locptr + SYMSIZ);
	++numltf;
	return (cptr);
}


void
entsym (char *sname, char *Table, short id, short typ, short value, short rsiz)

{
	short   my_ptr;

	my_ptr = cptr;
	while (alnum (*sname) != 0)  /* copy name */
	{
		Table [my_ptr++] = *sname++;
	}
	Table [my_ptr] = (char) 0;
	Table [cptr + IDENT] = (char) id;
	Table [cptr + TYPE] = (char) typ;
	putint (value, &Table [cptr + OFFSET]);
	putint (rsiz, &Table [cptr + ROWSIZ]);
}


short
addlbl (void)

{
	short           result;

	addloc (ssname, LABEL, LABEL, getlabel (), 0);
	if (l_sym_tab [cptr + IDENT] != LABEL)
	{
		errrpt ("NOT A LABEL");
	}
	result = getint (&l_sym_tab [cptr + OFFSET]);
	return (result);
}


/* Test if next input string is legal symbol name */

short
symname (char *sname)

{
	short           k;

	blanks ();
	if (alpha (cur_ch ()) == 0)
	{
		return (NO);
	}
	k = 0;
	while (alnum (cur_ch ()) != 0)
	{
		sname [k++] = (char) get_ch ();
	}
	sname [k] = (char) 0;
	return (YES);
}


/* Return next avail internal label number */

short
getlabel (void)

{
	return (++nxtlab);
}


/* Print specified number as label */

void
printlabel (short label)

{
	outstr ("C$");
	outdec (label);
}


void
prlbll (short label)

{
	printlabel (label);
	newline ();
}


/* Print a carriage return and a string only to console */

void
pl (char *str)

{
	PUTCHAR (LINEFEED);
	PUTS (str);
}


void
addwhile (short pointer [])

{
	short k;

	pointer [WQSP] = stkp;                  /*      save stack ptr          */
	pointer [WQLOOP] = getlabel (); /*      and looping label       */
	pointer [WQLAB] = getlabel ();  /*      and exit label          */
	if (wqindex >= WQMAX)
	{ 
		oflerr (); 
		return;
	}
	k = 0;
	while (k < WQSIZ)
	{
		wq [wqindex++] = pointer [k++];
	}
}


void
delwhile (void)

{
	if (wqindex > 0)
	{
		wqindex = (short) (wqindex - WQSIZ);
	}
}


short
readwhile (short index)

{
	short           result;

	if (index <= 0)
	{
		oocerr ();
		return (0);
	}
	else
	{
		result = (short) (index - WQSIZ);
		return (result);
	}
}


short
cur_ch (void)           /*      "peek-ahead" to current character from input file.      */

{
	short           result;

	result = line [lptr];
	result &= 0xff;
	return (result);
}


short
next_ch (void)          /*      "peek-ahead" to next character from input file. */

{
	short           result;

	if (cur_ch () == 0)
	{
		return (0);
	}
	else
	{
		result = line [lptr + 1];
		result &= 0xff;
		return (result);
	}
}


short
get_ch (void)                   /*      Get a character from the input file.    */

{
	short           result;

	if (cur_ch () == 0)
	{
		return (0);
	}
	else
	{
		result = line [lptr++];
		result &= 0xff;
		return (result);
	}
}


short
isy (void)

{

	if ((line [0] == 'y') || (line [0] == 'Y'))
	{
		return (YES);
	}
	return (NO);
}


void
kill  (void)

{
	lptr = 0;
	line [0] = (char) 0;
}


short
inbyte (void)

{
	short           result;

	while (cur_ch () == 0)
	{ 
		if (eof == YES)
		{
			return (0);
		}
		preprocess ();
	}
	result = get_ch ();
	return (result);
}


short
inchar (void)

{
	short           result;

	if (cur_ch () == 0)
	{
		input_line ();
	}
	if (eof == YES)
	{
		return (0);
	}
	result = get_ch ();
	return (result);
}


void
input_line (void)               /*      Read a line from input file.    */

{
	short           unit;

	errflg = 0;
	while (1)
	{ 
		if (eof == YES)
		{
			return;
		}
		unit = input2;
		if (unit == 0)
		{
			unit = input;
		}
		kill ();
		lptr = cfread (line, unit);
		++numlin;
#ifndef _DOS
#ifndef UNIX
		poll (1);
#endif
#endif
		if (lptr <= 0)
		{ 
			FCLOSE (unit);
			unit = 0;
			kill ();
			if (input2 != 0)
			{
				input2 = 0;
			}
			else
			{
				eof = YES;
			}
		}
		if (lptr != 0)
		{ 
			if ((ctext == YES) && (cmode == YES))
			{
				outcom (line);
			}
			lptr = 0;
			if (test_mode == YES)
			{
				PUTS (line);
				PUTS ("\n");
			}
			return;
		}
	}
}


/* process conditional compile directives */

void
ifline (void)   /*      Check #ifdef-type lines */

{
	pmode = YES;
	while (1)
	{ 
		input_line ();
		if (eof == YES)
		{
			break;
		}
		blanks ();
		if (cur_ch () == '#')
		{ 
			if (match ("#ifdef") == YES)
			{ 
				++iflv;
				if (skiplv)
				{
					continue;
				}
				if (symname (msname) == YES)
				{
					if (findmac (msname) == NULL)
					{
						skiplv = iflv;
					}
				}
				continue;
			}
			if (match ("#ifndef") == YES)
			{
				++iflv;
				if (skiplv != 0)
				{
					continue;
				}
				if (symname (msname) == YES)
				{
					if (findmac (msname) != NULL)
					{
						skiplv = iflv;
					}
				}
				continue;
			}
			if (match ("#else") == YES)
			{ 
				if (iflv != 0)
				{ 
					if (skiplv == iflv)
					{
						skiplv = 0;
					}
					else
					{
						if (skiplv == 0)
						{
							skiplv = iflv;
						}
					}
				}
				else
				{
					noiferr ();
				}
				continue;
			}
			if (match ("#endif") == YES)
			{ 
				if (iflv != 0)
				{ 
					if (skiplv == iflv)
					{
						skiplv = 0;
					}
					--iflv;
				}
				else
				{
					noiferr ();
				}
				continue;
			}
		}
		if (skiplv != 0)
		{
			continue;
		}
		break;
	}
	pmode = NO;
}


void
noiferr (void)

{
	errrpt ("NO MATCHING #if...");
}


short
keepch (short c)

{
	mline [mptr] = (char) c;
	if (mptr < LINEMAX)
	{
		mptr++;
	}
	return (c);
}


void
preprocess (void)

{
	char    *cp; 
	short    k;
	short    c;

	if (cmode == YES)
	{ 
		ifline ();
		if (eof == YES)
		{
			return;
		}
	}
	else
	{ 
		input_line ();
		return;
	}
	mptr = 0;
	lptr = 0;
	while (cur_ch () != 0)
	{
		if (isspace (cur_ch ()) != NO)
		{
			keepch (SPACE);
			while (isspace (cur_ch ()) != NO)
			{
				get_ch ();
			}
		}
		else
		{
			if (cur_ch () == '"')
			{
				keepch (cur_ch ());
				get_ch ();
				while (cur_ch () != '"')
				{
					if (cur_ch () == 0)
					{
						errrpt ("MISSING \42");
						break;
					}
					keepch (get_ch ());
				}
				get_ch ();
				keepch ('"');
			}
			else
			{
				if (cur_ch () == 39)
				{
					keepch (39);
					get_ch ();
					while (cur_ch () != 39)
					{
						if (cur_ch () == 0)
						{
							errrpt ("MISSING '");
							break;
						}
						keepch (get_ch ());
					}
					get_ch ();
					keepch (39);
				}
				else
				{
					if ((cur_ch () == '/') && (next_ch () == '*'))
					{                       /*      Filter-out comments.    */
						inchar ();
						inchar ();
						while (1) {
							if (inchar () == '*')
							{
								if (cur_ch () == '/')
								{               /*      End of comment. */
									break;
								}
							}
							if (eof == YES)
							{
								break;
							}
						}
						inchar ();
					}
					else
					{
						if (alnum (cur_ch ()) != NO)
						{
							k = 0;
							while (alnum (cur_ch ()) != NO)
							{
								if (k < NAMEMAX)
								{
									msname [k++] = (char) cur_ch ();
								}
								get_ch ();
							}
							msname [k] = (char) 0;
							cp = NULL;
							if (alpha (cur_ch ()) != NO)
							{
								cp = findmac (msname);
							}
							if (cp != NULL)
							{
								while (*cp != (char) 0)
								{
									c = *cp++;
									keepch (c);
								}
							}
							else
							{
								cp = msname;
								while (*cp != (char) 0)
								{
									c = *cp++;
									keepch (c);
								}
							}
						}
						else
						{
							keepch (get_ch ());
						}
					}
				}
			}
		}
	}
	keepch (0);
	if (mptr >= LINEMAX)
	{
		errrpt ("LINE TOO LONG");
	}
	lptrc = line; 
	mptrc = mline;
/*      while (*lptrc++ = *mptrc++); */
	*lptrc = *mptrc;
	while (*mptrc != (char) 0)
	{
		*lptrc++ = *mptrc++;
		*lptrc = (char) 0;
	}
	lptr = 0;
}


void
addmac (void)

{
	char    *k;

	if (symname (msname) == NO)
	{ 
		illname ();
		kill ();
		return;
	}
	k = msname;
	while (putmac (*k) != 0)
	{
		k++;
	}
	while (cur_ch () != 0)
	{ 
		if (isspace (cur_ch ()) != NO)
		{
			get_ch ();
		}
		else
		{
			break;
		}
	}
	while (putmac (get_ch ()) != 0)
	{
		;
	}
	if (macptr >= MACMAX)
	{
		oflerr ();
	}
}


short
putmac (short c)

{
	*macptr = (char) c;
	if (macptr < MACMAX)
	{
		macptr++;
	}
	return (c);
}


char *
findmac (char *sname)

{
	char    *k;

	k = MACQ;


	while (k < macptr)
	{
		if (astreq (sname, k, NAMEMAX) != 0)
		{
			while (*k != (char) 0)
			{
				k++;
			}
			k++;            /*      Skip-over intervening zero-byte.        */
			return (k);     /*      Return pointer to definition.           */
		}
		while (*k != (char) 0)
		{
			k++;            /*      Skip-over any remaining portion of name.        */
		}
		k++;            /*      Skip-over intervening zero-byte.        */
		while ((*k != (char) 0) && (k < macptr))
		{
			k++;            /*      Skip-over definition.   */
		}
		k++;            /*      Skip-over intervening zero-byte.        */
	}
	return (NULL);
}


short
outbyte (short c)

{
	if (c == 0)
	{
		return (0);
	}
	outcol++;
	if (c == '\n')
	{
		outcol = 1;
	}
	if ((PUTC (c, output)) <= 0)
	{ 
		errrpt ("OUTPUT FILE ERROR");
		exit (1);
	}
	return (c);
}

void
outstr (char *pointer)

{
	while (*pointer != (char) 0)
	{
		outbyte (*pointer);
		pointer++;
	}
}


void
outlbl (char *pointer)

{
	short           c;

	c = *pointer;
	while (*pointer != (char) 0)
	{ 
		c = *pointer++;
		if ((c > 96) && (c < 123))
		{
			c = (short) (c - 32);
		}
		if (c == '_')
		{
			c = '#';
		}
		outbyte (c);
	}
}


void
otlbll (char pointer [])

{
	outlbl (pointer); 
	newline ();
}


void
outcom (char pointer [])        /*      Print the referenced string as a comment.       */

{
	comment ();
	outl (pointer);
}


void
newline (void)  /*      Go to the next line.    */

{
	outbyte (LINEFEED);
}


void
tab (void)              /*      Move to the next eightth-column.        */

{
	
	outbyte (SPACE);        /*      Send at least one space.        */
	while ((outcol & 7) != 0)
	{
		outbyte (SPACE);
	}
}


void
errrpt (char *txt)

{
	char    dummy [LINESIZE];
	short   k;

	if (errflg != 0)
	{
		return;
	}
	else
	{
		errflg = 1;
	}
	pl ("!! ERROR : "); 
	outl (" ************************************ ");
	outstr ("!! ERROR : "); 
	PUTS (txt);
	outl (txt);
	pl (line); 
	outl (line); 
	PUTCHAR (LINEFEED);
	k = 0;
	while (++k < lptr)
	{
		PUTCHAR (SPACE);
		outbyte (SPACE);
	}
	PUTCHAR ('^');
	outbyte ('^');
	outbyte (LINEFEED);
	outl (" ************************************ ");
	if (++errcnt > 40)
	{
		pl (">40 errors\n");
		exit (1);
	}
#if _DOS || UNIX
	tigets (dummy, LINEMAX);
#else
	gets (dummy);
#endif
	if ((dummy [0] == 'q') || (dummy [0] == 'Q'))
	{
		PUTS ("Really quit? [n] ");

#if _DOS || UNIX
		tigets (dummy, LINEMAX);    /* get answer */
#else
		gets (dummy);
#endif

		if ((dummy [0] == 'y') || (dummy [0] == 'Y'))
		{
			ol (" Compilation aborted by user. ");
			FCLOSE (input);
			input = 0;
			FCLOSE (output);
			output = 0;
			exit (1);
		}
	}

}


void
outl (char pointer [])

{
	outstr (pointer);
	newline ();
}


void
ol (char pointer [])

{
	ot (pointer);
	newline ();
}


void
ot (char pointer [])

{
	tab ();
	outstr (pointer);
}


short
match (char *lit)

{
	short           k;

	blanks ();
	k = streq (lit);
	if (k != 0)
	{
		lptr = lptr + k;
		return (YES);
	}
	return (NO);
}


short
amatch (char *lit, short len)

{
	short           k;

	blanks ();
	k = astreq (line + lptr, lit, len);
	if (k != 0)
	{
		lptr = lptr + k;
		while (alnum (cur_ch ()) != 0)
		{
			inbyte ();
		}
		return (YES);
	}
	return (NO);
}


void
blanks (void)

{
	while (1)
	{ 
		while (cur_ch () != 0)
		{ 
			if (isspace (cur_ch ()) != NO)
			{
				get_ch ();
			}
			else
			{
				return;
			}
		}
		if (pmode == YES)
		{
			return;
		}
		preprocess ();
		if (eof == YES)
		{
			break;
		}
	}
}


short
PDBody (short item)

{
	short           chtr;
	short           size;

	size = 0;

	if (item != 0)
	{
		chtr = item;            /*      Calculate rightmost character   */
		chtr %= 10;                     /*      Calculate rightmost character   */
		chtr += '0';            /*      Calculate rightmost character   */
		item /= 10;
		size = PDBody (item);
		outbyte (chtr);
		size++;
	}
	return (size);
}


short
PHBody (short item)

{
	short           chtr;
	short           size;

	size = 0;

	if (item != 0)
	{
		chtr = item;            /*      Calculate rightmost character   */
		chtr &= 0xf;
		chtr += '0';
		if (chtr > '9')
		{
			chtr += 7;
		}
		item >>= 4;
		item &= 0xfff;
		size = PHBody (item);
		outbyte (chtr);
		size++;
	}
	return (size);
}


short
outdec (short item)

{

	short   size;

	if (item == 0)
	{
		outbyte ('0');
		return (1);
	}
	else
	{
		if (item == 16384 << 1)
		{ 
			outstr ("32768");
			return (5);
		} 
		else
		{ 
			if (item < 0)
			{ 
				outbyte ('-');
				size = PDBody (-item);
			}
			else
			{
				size = PDBody (item);
			}
		}
	}
	return (size);
}


short
outhex (short item)

{
	short           size;

	size = 0;

	if (item == 0)
	{
		outbyte ('0');
		return (1);
	}
	size = PHBody (item);
	return (size);
}


void
outdcl (short item)

{
	outdec (item); 
	newline ();
}


short
isptr (char *pointer)

{
	if (pointer [IDENT] >= POINTER)
	{
		return (YES);
	}
	return (NO);
}


short
ischar (char *pointer)

{
	if ((isptr (pointer) == NO) && (pointer [TYPE] == CCHAR))
	{
		return (YES);
	}
	return (NO);
}


short
isword (short lval [])

{
	if (lval [2] == CINT)
	{
		return (YES);
	}
	return (NO);
}


short
ptrchk (short lval [], short lval2 [])

{
	short           result;

	result = lval [2] | lval2 [2];
	return (result);
}


/* heirarchical parsing
**
** lval [0]-symbol table address, 0 for constant
** lval [1]-type of indirect object to fetch, 0 for static
** lval [2]-type of pointer or array, 0 otherwise
** lval [3]-Table Number:
**      0 = Unused.
**      1 = Global Symbol-Table.
**      2 = Local Symbol-Table.
**      3 = Macro Symbol-Table.
*/

void
expression (void)

{
	short   lval [4];

	if (heir1 (lval) != 0)
	{
		rvalue (lval);
	}
}


short
heir1 (short lval [])

{
	short    cx1;
	short   *cx2;
	short    k;
	short    lval2 [4];

	k = heir1a (lval);
	if (match ("?") == YES)
	{ 
		if (k != 0)
		{
			rvalue (lval);
		}
		condexp ();
		k = 0;
	}
	if (match ("=") == YES)
	{
		if (k == 0)
		{
			needlval ();
			return (0);
		}
		if (lval [1] != 0)
		{
			push ();
		}
		if (heir1 (lval2) != 0)
		{
			rvalue (lval2);
		}
		if (match ("?") == YES)
		{ 
			cx1 = lval [1];
			cx2 = lval;
			condexp ();
			lval [1] = cx1;
			lval = cx2;
		}
		store (lval);
		return (0);
	}
	else
	{
		return (k);
	}
}


short
heir1a (short lval [])

{
	short           result;

	result = dolog ("||", "JEQ", "MOV 15,8", "S 8,8", heir1b, lval);
	return (result);
}


short
heir1b (short lval [])

{
	short           result;

	result = dolog ("&&", "JNE", "S 8, 8", "MOV 15,8", heir2, lval);
	return (result);
}


short
heir2 (short lval [])

{
	short           k;
	short           lval2 [4];

	k = heir3 (lval);
	blanks ();
	if (cur_ch () != '|')
	{
		return (k);
	}
	if (streq ("||") != 0)
	{
		return (k);
	}
	if (k != 0)
	{
		rvalue (lval);
	}
	while (1)
	{
		if (match ("|") == YES)
		{
			push ();
			if (heir3 (lval2) != 0)
			{
				rvalue (lval2);
			}
			pop ();
			ot ("SOC");
			if (rtext == YES)
			{
				ol ("*R14+, R8");
			}
			else
			{
				ol ("*14+, 8");
			}
#if _DOS || UNIX
			needst = NO;
#else
	#asm
		CLR @NEEDST
	#endasm
#endif
		}
		else
		{
			return (0);
		}
	}
}


short
heir3 (short lval [])

{
	short           k;
	short           lval2 [4];

	k = heir4 (lval);
	blanks ();
	if (cur_ch () != '^')
	{
		return (k);
	}
	if (k)
	{
		rvalue (lval);
	}
	while (1)
	{
		if (match ("^") == YES)
		{
			push ();
			if (heir4 (lval2) != 0)
			{
				rvalue (lval2);
			}
			pop ();
			ot ("XOR");
			if (rtext == YES)
			{
				ol ("*R14+, R8");
			}
			else
			{
				ol ("*14+, 8");
			}
#if _DOS || UNIX
			needst = NO;
#else
	#asm
			CLR @NEEDST
	#endasm
#endif
		}
		else
		{
			return (0);
		}
	}
}


short
heir4 (short lval [])

{
	short           k;
	short           lval2 [4];

	k = heir5 (lval);
	blanks ();
	if (cur_ch () != '&')
	{
		return (k);
	}
	if (streq ("&&") != 0)
	{
		return (k);
	}
	if (k != 0)
	{
		rvalue (lval);
	}
	while (1)
	{
		if (match ("&") == YES)
		{
			push ();
			if (heir5 (lval2) != 0)
			{
				rvalue (lval2);
			}
			pop ();
			ot ("INV");
			if (rtext == YES)
			{
				ol ("*R14");
			}
			else
			{
				ol ("*14");
			}
			ot ("SZC");
			if (rtext == YES)
			{
				ol ("*R14+, R8");
			}
			else
			{
				ol ("*14+, 8");
			}
#if _DOS || UNIX
			needst = NO;
#else
	#asm
			CLR @NEEDST
	#endasm
#endif
		}
		else
		{
			return (0);
		}
	}
}


short
heir5 (short lval [])

{
	short           k;
	short           lval2 [4];

	k = heir6 (lval);
	blanks ();
	if ((streq ("==") == 0) && (streq ("!=") == 0))
	{
		return (k);
	}
	if (k)
	{
		rvalue (lval);
	}
	while (1)
	{
		if (match ("==") == YES)
		{
			push ();
			if (heir6 (lval2) != 0)
			{
				rvalue (lval2);
			}
			pop ();
			dcall ("EQ");
		}
		else
		{
			if (match ("!=") == YES)
			{
				push ();
				if (heir6 (lval2) != 0)
				{
					rvalue (lval2);
				}
				pop ();
				dcall ("NE");
			}
			else
			{
				return (0);
			}
		}
	}
}


short
heir6 (short lval [])

{
	short           k;
	short           lval2 [4];

	k = heir7 (lval);
	blanks ();
	if ((streq ("<") == 0) && (streq (">") == 0) &&
	  (streq ("<=") == 0) && (streq (">=") == 0))
	{
		return (k);
	}
	if (streq (">>") != 0)
	{
		return (k);
	}
	if (streq ("<<") != 0)
	{
		return (k);
	}
	if (k)
	{
		rvalue (lval);
	}
	while (1)
	{
		if (match ("<=") == YES)
		{
			push ();
			if (heir7 (lval2) != 0)
			{
				rvalue (lval2);
			}
			pop ();
			if (ptrchk (lval, lval2) != 0)
			{
				dcall ("ULE");
			}
			else
			{
				dcall ("LE");
			}
		}
		else
		{
			if (match (">=") == YES)
			{
				push ();
				if (heir7 (lval2) != 0)
				{
					rvalue (lval2);
				}
				pop ();
				if (ptrchk (lval, lval2) != 0)
				{
					dcall ("UGE");
				}
				else
				{
					dcall ("GE");
				}
			}
			else
			{
				if ((streq ("<") != 0) && (streq ("<<")) == 0)
				{
					inbyte ();
					push ();
					if (heir7 (lval2) != 0)
					{
						rvalue (lval2);
					}
					pop ();
					if (ptrchk (lval, lval2) != 0)
					{
						dcall ("ULT");
					}
					else
					{
						dcall ("LT");
					}
				}
				else
				{
					if ((streq (">") != 0) && (streq (">>") == 0))
					{
						inbyte ();
						push ();
						if (heir7 (lval2) != 0)
						{
							rvalue (lval2);
						}
						pop ();
						if (ptrchk (lval, lval2) != 0)
						{
							dcall ("UGT");
						}
						else
						{
							dcall ("GT");
						}
					}
					else
					{
						return (0);
					}
				}
			}
		}
	}
}

