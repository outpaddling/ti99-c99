/*
 * C99CSB - c99 compiler subroutines *
 *
 * * (c) 1987 by Clint Pulley *
 *
 * * Last Edit 87/03/05  2300 * Edit # ???  : 28-Dec-99  0100  (Kristine
 * Rogers)
 */
#ifdef __POWERC
#define	_DOS
#endif

#if _DOS || UNIX
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#ifdef _DOS
#include <process.h>
#pragma input_line_depth (250)
#include "myfuncs.h"
#endif
#ifdef UNIX
#include <unistd.h>
#endif
#include <stdlib.h>
#include "c99proto.h"
#else
#asm
IDT 'c99csb'
*
DEF GETFN, GETLIT, PUTLIT, HASH, SLAST
DEF ALPHA, NUMERI, AN, ASTREQ, STREQ, GETINT, PUTINT
REF VMBW, VMBR, FOPEN, LINE, LPTR
#endasm
#endif

#include "c99.h"

/*
 * * get filename and open file
 */
short
getfn(char *text, char *m)

{
    short	    unit;

    char	    fn[LINESIZE];


    unit = 0;
    while (1)
    {
	PUTS(text);
	PUTS(" filename? ");

#if _DOS || UNIX
	tigets(fn, sizeof(fn));
#ifdef _DOS
	unix2dos_path(fn);
#endif
	/*
	 * if (base_dir [0] == (char) 0) { strcpy (base_dir, dirname (fn)); }
	 */
#else
	gets(fn);
#endif

	if (fn[0] == (char)0)
	{
	    break;	/* return 0 if null name */
	}
	unit = FOPEN(fn, m);
	if (unit != 0)
	{
	    break;
	}
	PUTS("bad filename-try again\n");
    }
    return (unit);
}

#if _DOS || UNIX

short
getlit(short litptr, short type)
{
    short	    val;
    char	    out_string[50];

    if ((type == CCHAR) || (type == CINT))
    {
	val = lit_pool[litptr];
	return (val);
    }

    sprintf(out_string, "getlit: Unknown type \"%d\"!", type);
    errrpt(out_string);
    exit(1);
}

void
putlit(short val, short litptr, short size)

{
    char	    out_string[50];

    if ((size == CCHAR) || (size == CINT))
    {
	lit_pool[litptr] = val;
	return;
    }
    sprintf(out_string, "putlit: Unknown size \"%d\"!", size);
    errrpt(out_string);
    exit(1);
}

short
hash(char *sname)
{

    short	    index;
    short	    hash_value;
    short	    temp;

    hash_value = 0;
    index = 0;
    temp = 0;

    while (sname[index] != (char)0)
    {
	temp = (short)sname[index];
	temp &= 0xff;
	hash_value += temp;
	index++;
    }
    hash_value &= 0xff;
    if (hash_value == 0)
    {
	hash_value++;
    }
    return (hash_value);
}

short
alpha(short c)
{
    if (isalpha(c) == 0)
    {
	if (c != '_')
	{
	    return (0);
	}
    }
    return (c);
}
short

alnum(short c)
{
    if (isalnum(c) == 0)
    {
	if (c != '_')
	{
	    return (0);
	}
    }
    return (c);
}

short
numeric(short c)
{
    if (isdigit(c) == 0)
    {
	return (0);
    }
    return (c);
}

short
getint(char *addr)

{

    short	    data;
    short	    result;

    result = (short)*addr++;
    result &= 0xff;
    data = (short)*addr;
    data <<= 8;
    data &= 0xff00;
    result |= data;
    return (result);
}

void
putint(short i, char *addr)
{

    short	    data;

    data = i;
    data &= 0xff;
    *addr++ = (char)data;
    data = i;
    data >>= 8;
    data &= 0xff;
    *addr = (char)data;
}

short
streq(char *str)
{

    short	    line_index;
    short	    str_index;

    line_index = lptr;
    str_index = 0;
    while (str[str_index] == line[line_index])
    {
	if (str[str_index] == (char)0)
	{
	    break;
	}
	line_index++;
	str_index++;
    }
    if (str[str_index] == (char)0)
    {
	return (str_index);
    }
    return (0);
}

short
astreq(char str1[], char str2[], short len)
{

    short	    count;

    for (count = 0; count < len; count++)
    {
	if (str1[count] != str2[count])
	{
	    break;
	}
	if (str1[count] == (char)0)
	{
	    break;
	}
	if (str2[count] == (char)0)
	{
	    break;
	}
    }
    if ((isalnum(str1[count]) != 0) || (str1[count] == '_'))
    {
	return (0);
    }
    if ((isalnum(str2[count]) != 0) || (str2[count] == '_'))
    {
	return (0);
    }
    return (count);
}

short
cfread(char *str1, short unit)
{

    short	    index;

    if (unit <= 0)
    {

	tigets(str1, LINEMAX);
	return (1);
    }

    if (file_table[unit] == NULL)
    {
	return (0);
    }
    if (fgets(str1, LINEMAX, file_table[unit]) == NULL)
    {
	return (-1);
    }
    index = (short)strlen(str1);
    while (index > 0)
    {
	if (iscntrl(str1[index - 1]) == NO)
	{
	    break;
	}
	str1[index - 1] = (char)0;
	index--;
    }
    return (1);
}

short
tiputchar(short c)
{
    short	    status;

    status = (short)putc(c, stderr);
    return (status);
}

short
tiputc(short c, short unit)
{
    short	    status;

    if (unit == 0)
    {
	status = (short)putc(c, stdout);
	return (status);
    }

    if (file_table[unit] == NULL)
    {
	return (0);
    }
    status = (short)fputc(c, file_table[unit]);
    if (c == '\n')
    {
	fflush(file_table[unit]);
    }
    return (status);
}

short
tiputs(char *str)
{
    short	    status;

    status = (short)fputs(str, stderr);
    if (status == EOF)
    {
	return (0);
    }
    return (status);
}

short
tigets(char *str, short max)
{
    short	    status;

    if (fgets(str, max, stdin) == NULL)
    {
	return (0);
    }
    status = (short)strlen(str);
    while (status > 0)
    {
	if (iscntrl(str[status - 1]) == NO)
	{
	    break;
	}
	str[status - 1] = (char)0;
	status--;
    }
    return (status);
}

short
tifopen(char *name, char *mode)
{

    short	    index;

    char	    fullname[LINESIZE];


    /* Find an unused entry.	*/
    index = 1;
    while (file_table[index] != NULL)
    {
	index++;
	if (index > 4)
	{
	    return (0);
	}
    }
#ifdef _DOS
    str_tolower(name);
    unix2dos_path(name);
#endif
    file_table[index] = fopen(name, mode);
    if ((file_table[index] == NULL)
	&& (base_dir[0] != (char)0)
	&& (strncmp(name, base_dir, strlen(base_dir)) != 0))
    {
	sprintf(fullname, "%s/%s", base_dir, name);
#ifdef _DOS
	str_tolower(fullname);
	unix2dos_path(fullname);
#endif
	file_table[index] = fopen(fullname, mode);
    }

    if (file_table[index] != NULL)
    {
	return (index);
    }

    return (0);
}

short
tifclose(short unit)
{

    if (file_table[unit] == NULL)
    {
	return (0);
    }
    fflush(file_table[unit]);
    fclose(file_table[unit]);
    file_table[unit] = NULL;
    return (0);
}

#else

#asm
**VRAM LITERAL POOL ACCESS
*
LITBAS EQU > 2000 START OF VRAM SPACE
LITBUF BSS 2
*
*c = getlit(litptr, type);
GET		LITERAL CHAR OR WORD
*
		GETLIT MOV @ 4(14), 0 POINTER
		AI 0, LITBAS VRAM ADDRESS
		LI 1, LITBUF
		MOV @ 2(14), 2
#BYTES
		BLWP @ VMBR GET VALUE
		MOV @ LITBUF, 8
		CI 2, 2
		JEQ GETL1 IF WORD
		SRL 8, 8
		GETL1 B * 13
*
*		putlit(c, litptr, type);
STORE		LITERAL CHAR OR WORD
*
		PUTLIT MOV @ 2(14), 2
#BYTES
		MOV @ 4(14), 0
		AI 0, LITBAS
		MOV @ 6(14), 3 VALUE
		LI 1, LITBUF
		CI 2, 2
		JEQ PUTL1 IF WORD
		SWPB 3
		PUTL1 MOV 3, *1
		BLWP @ VMBW
		B * 13
*
*		hash(sname);
SYMBOL		NAME HASHING
*
	       *A 1, 8 ACCUMULATE
		SLA 8, 1 USE SHIFT TO HASH
		JMP HASH1
		HASHX B * 13
*
	       **isspace(c);
TRUE		IF c IS ALPHABETIC OR _
*
		ISSPAC MOV @ 2(14), 0
		CLR R8 CLEAR AN FLAG
		CI 0, 32
		JE ISSPA1 IF == ' '
		CI 0, 9
		JL ISSPA0 IF < 9
		CI 0, 13
		JH ISSPA0 IF > 13
*
		ISSPA1 SETO 8 RETURN TRUE
		ISSPA0 B * 13
*
**		alpha(c);
TRUE		IF c IS ALPHABETIC OR _
*
		ALPHA MOV @ 2(14), 0
		CLR 7 CLEAR AN FLAG
		BL @ TST
		B * 13
*
*		an(c);
TRUE		IF c IS ALPHANUMERIC
*
		ALNUM MOV @ 2(14), 0
		SETO 7 SET AN FLAG
		BL @ TST
		B * 13
*
*		numeric(c);
TRUE		IF c IS NUMERIC
*
		NUMERI MOV @ 2(14), 0
		BL @ TSTN
		B * 13
*
	       *JL TST0 IF < 'A'
		CI 0, 'Z'
		JLE TSTT IF IN(A - Z)
CI 0, '_'
JEQ TSTT IF '_'
CI 0, 'a'
JL TSTF IF < 'a'
CI 0, 'z'
JLE TSTT IF IN(a - z)
TSTF CLR 8
RT RETURN FALSE
TST0 ABS 7 TEST AN FLAG
JEQ TSTF IF ALPHA ONLY
TSTN CI 0, '0'
JL TSTF IF < '0'
CI 0, '9'
JH TSTF IF NOT IN(0 - 9)
TSTT SETO 8 RETURN TRUE
RT
*
*STRING COMPARE FUNCTIONS
*
*astreq(str1, str2, len);
COMPARE		str1 AND str2 FOR UP TO
*		len CHARACTERS.IF A MATCH, BOTH MUST TERMINATE WITH
*		NON - ALPHA CHARACTERS.RETURNS 0 OR
#CHARS MATCHED.
*
		ASTREQ MOV @ 6(14), 1 ADDR str1
		MOV @ 4(14), 2 ADDR str2
		MOV @ 2(14), 3 len
		CLR 4 COUNTER
		CLR 5 REFERENCE
		ASTR1 C 4, 3
		JEQ ASTR2 IF len CHARS COMPARED
		CB * 1, *2 COMPARE CHARS
		JNE ASTR2 IF NO MATCH
		CB * 1, 5 TEST FOR 0 BYTE
		JEQ ASTR2 IF END OF str1
		CB * 2, 5
		JEQ ASTR2 IF END OF str2
		INC 1
		INC 2
		INC 4
		JMP ASTR1
		ASTR2 SETO 7 SET AN FLAG
		MOVB * 1, 0 TEST str1 CHAR
		SRL 0, 8
		BL @ TST
		ABS 8
		JNE ASTR3 IF AN
		MOVB * 2, 0
		SRL 0, 8
		BL @ TST TEST str2 CHAR
		ABS 8
		JNE ASTR3
		MOV 4, 8 RETURN COUNT
		B * 13
		ASTR3 CLR 8 RETURN FALSE
		B * 13
*
*		streq(str);
COMPARE		str WITH THE INPUT LINE, STARTING
*		AT LPTR.IF A MATCH WHEN str 's 0-BYTE IS ENCOUNTERED,
*		RETURN
#CHARS MATCHED. IF NO MATCH, RETURN 0.
*
		STREQ MOV @ 2(14), 1 ADDR str
		LI 2, LINE LINE BUFFER ADDR
		A @ LPTR, 2 CURRENT POSITION
		CLR 3 REFERENCE
		CLR 8 CHAR COUNTER
		STREQ1 CB * 1, 3 TEST FOR 0 - BYTE
		JEQ STREQ3 IF END OF STR(MATCH)
CB * 1 +, *2 +
JNE STREQ2 IF NO MATCH
INC 8
JMP STREQ1
STREQ2 CLR 8 NO MATCH - RETURN 0
STREQ3 B * 13
*
*SYMBOL TABLE OPERATIONS
*
*putint(i, addr);
STORE		i AT addr, LO BYTE FIRST
*
		PUTINT MOV @ 4(14), 1 VALUE
		MOV @ 2(14), 2 SYMTAB ADDR
		SWPB 1
		MOVB 1, *2 + LO BYTE
		SWPB 1
		MOVB 1, *2 HI BYTE
		B * 13
*
	       *i = getint(addr);
GET		i FROM addr, LO BYTE FIRST
*
		GETINT MOV @ 2(14), 1 SYMTAB ADDR
		MOVB * 1 +, 8 LO BYTE
		SWPB 8
		MOVB * 1, 8 HI BYTE
		B * 13
*
		SLAST
#endasm
#endif
