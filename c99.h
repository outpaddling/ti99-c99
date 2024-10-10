
#define	logtxt	"*c99 v4.0 (c) 1985, 1987 Clint Pulley"


#if defined _DOS || defined UNIX
	#define	FOPEN	tifopen
	#define	FCLOSE	tifclose
	#define	PUTCHAR	tiputchar
	#define	PUTC	tiputc
	#define	PUTS	tiputs
#else
	#define	FOPEN	fopen
	#define	FCLOSE	fclose
	#define	PUTCHAR	putchar
	#define	PUTC	putc
	#define	PUTS	puts
#endif

#ifndef NO
	#define NO	0
#endif

#ifndef YES
	#define YES	1
#endif


/* symbol table parameters
**
** This version has symtab space for
**    - 256 global symbols  (must be 2^n)
**    - 40  local symbols
*/
#define	SYMSIZ   13		/*	Symbol-table element size	*/
#define	GSYMSZ   256 * SYMSIZ	/*	Global symbol-table size	*/
#define	LSYMSZ   40 * SYMSIZ	/*	Local symbol-table size	*/
#define	GHSHMSK  255	/*	global symtab hash mask		*/

#if _DOS || UNIX
	#ifdef	C99_MAIN
	char	g_sym_tab [GSYMSZ];	/*	Global Symbol Table	*/
	char	l_sym_tab [LSYMSZ];	/*	Local Symbol Table	*/
	#else
	extern char	g_sym_tab [GSYMSZ];	/*	Global Symbol Table	*/
	extern char	l_sym_tab [LSYMSZ];	/*	Local Symbol Table	*/
	#endif
	#define	STARTGLB	&g_sym_tab [0]
	#define	ENDGLB		&g_sym_tab [GSYMSZ - 1]
	#define	STARTLOC	&l_sym_tab [0]
	#define	ENDLOC		&l_sym_tab [LSYMSZ - 1]
#else
	#define	STARTGLB	10146	/*	starts at >27A2		*/
	#define	ENDGLB		13474	/*	STARTGLB + 200 * SYMSIZ	*/
	#define	STARTLOC	13474	/*	ENDGLB	*/
	#define	ENDLOC		13994	/*	STARTLOC + 40 * SYMSIZ	*/
#endif

/* symbol table entry format */
#define	IDENT    7
#define	TYPE     8
#define	OFFSET   9
#define	ROWSIZ  11


	/* "while" statement queue */
#define	WQTABSZ		60
#define	WQSIZ		3
#define	WQMAX		57		/*	wq + WQTABSZ - WQSIZ	*/

	/* macro (define) pool */
#define	MACQSIZE	1000

#if _DOS || UNIX
	#ifdef	C99_MAIN
		char	macro_pool [MACQSIZE];
	#else
		extern char	macro_pool [MACQSIZE];
	#endif
	#define	MACQ		&macro_pool [0]	/*	macro string buffer	*/
	#define	MACMAX		&macro_pool [MACQSIZE - 1]
#else
		/* macro (define) pool */
	#define	MACQ		9146	/*	macro string buffer	*/
	#define	MACMAX		10145	/*	MACQ + MACQSIZE - 1	*/
#endif

/*	"switch" table	*/
#define	SWTABSZ		120			/*	60 entries		*/
#define	SWEND		swq + 118

/*	possible values for "class"	*/
#define	STATIC		1
#define	EXTERNAL	2

/*	possible values for "TYPE"	*/
#define	UNKNOWN		0
#define	CCHAR		1
#define	CINT		2

/*	input line	*/
#define	LINESIZE	82
#define	LINEMAX		80	/*	LINESIZE - 2	*/

/*	System wide name size (for symbols)	*/
#define	NAMESIZE	7
#define	NAMEMAX		6

/*	Input/Output definitions	*/
#ifndef _DOS
	#ifndef UNIX
		#define	NULL		0
	#endif
#endif

#ifndef BACKSPACE
	#define	BACKSPACE	8
#endif

#ifndef TAB
	#define	TAB			9
#endif

#ifndef LINEFEED
	#define	LINEFEED	10
#endif

#ifndef FORMFEED
	#define	FORMFEED	12
#endif

#ifndef RETURN
	#define	RETURN		13
#endif

#ifndef SPACE
	#define	SPACE		32
#endif

#ifndef STAR
	#define	STAR		42
#endif

#ifndef BACKSLASH
	#define	BACKSLASH	92
#endif

/*	possible values for "IDENT"	*/
#define	LABEL		0
#define	VARIABLE	1
#define	ARRAY		2
#define	FUNCTION	3
#define	POINTER		4	/*	note that POINTER and PTRARR1/2		*/
#define	PTRAR1		5	/*	must be the highest valued items	*/
#define	PTRAR2		6	/*	in this list						*/

/*	statement types (tokens)	*/
#define	STIF		1
#define	STWHILE		2
#define	STRETURN	3
#define	STBREAK		4
#define	STCONT		5
#define	STASM		6
#define	STEXP		7
#define	STDO		8
#define	STFOR		9
#define	STSWITCH	10
#define	STCASE		11
#define	STDEF		12
#define	STGOTO		13
#define	STLABEL		14

/*	entry offsets in while queue	*/
#define	WQSP		0
#define	WQLOOP		1
#define	WQLAB		2

/*	literal pool	*/
#define  LITABSZ  4096
#define  LITMAX   4094	/*	LITABSZ-2	*/

#if _DOS || UNIX
	#ifdef	C99_MAIN
	short		lit_pool [LITABSZ];
	#else
	extern short		lit_pool [LITABSZ];
	#endif
#endif


/*	Definitions for heirarchical parsing
**
** lval [0]-symbol table address, 0 for constant
** lval [1]-type of indirect object to fetch, 0 for static
** lval [2]-type of pointer or array, 0 otherwise
** lval [3]-Table Number:
**		0 = Unused.
**		1 = Global Symbol-Table.
**		2 = Local Symbol-Table.
**		3 = Macro Symbol-Table.
**		4 = Literal-Pool Symbol-Table.
*/

#define TABLE_UNKNOWN	0
#define TABLE_GLOBAL	1
#define TABLE_LOCAL		2
#define TABLE_MACRO		3
#define TABLE_LIT		4



#ifdef	C99_MAIN

short	 litptr;	/*	ptr to lit pool entry		*/

short	 cptr;		/*	work ptr to any char buffer		*/
short	 cptr2;		/*	work ptr to any char buffer		*/
short	 locptr;	/*	ptr to symtab entries			*/
char	*macptr;	/*	macro buffer index				*/
char	*lptrc;
char	*mptrc;			/*	ptrs to line buffers		*/
char	 msname [NAMESIZE];	/*	macro name				*/

short		 swdefault;		/*	default label #				*/

short		 wq [WQTABSZ];	/*	while queue					*/
short		 stkp;			/*	compiler relative stk ptr	*/
short		 wqindex;		/*	ptr to next entry			*/
short		*swnext;		/*	ptr to next entry			*/
short		 swq [SWTABSZ];	/*	switch table				*/
short		 swactive;		/*	true inside switch			*/
short		 iflv;			/*	#if... nesting level		*/
short		 skiplv;		/*	#if... level for skipping	*/
short		 pmode;			/*	true in #if... processing	*/
short		 test_mode;		/*	Non-zero = Diagnostic/debug mode.	*/
short		 numlin;		/*	# source lines processed	*/
short		 numglbs;		/*	# global symbols used		*/
short		 numlocs;		/*	max local symbols used		*/
short		 errcnt;		/*	# errors in compilation		*/
short		 errflg;		/*	true if error in statement	*/
short		 eof;			/*	set non-zero on final input eof */
short		 input;			/*	iob # for input file			*/
short		 input2;		/*	iob # for "include" file		*/
short		 output;		/*	iob # for output file (if any)	*/
short		 nxtlab;		/*	next avail label #				*/
short		 ncmp;			/*	# open compound statements		*/
short		 cmode;			/*	non-zero while parsing c-code	*/
						/*	zero when passing assembly code	*/
short		 decl;			/*	# local bytes declared			*/
short		 ctext;			/*	non-zero to intermix c-source	*/
short		 atext;			/*	non-zero to add assembly-source comments	*/
short		 rtext;			/*	non-zero to add "r" before register numbers	*/
short		 pushin;		/*	non-zero for input_line push code	*/
short		 lptr;			/*	line buffer indices				*/
short		 noloc;			/*	disables block locals			*/
short		 lastst;		/*	last executed statement type	*/
short		 litlab;		/*	label # assigned to literal pool	*/
short		 numltf;		/*	# local symbols in this fcn			*/
short		 nogo;			/*	disables goto						*/
short		 argtop;		/*	top of arg sp						*/
short		 argstk;		/*	function arg sp						*/
short		 mptr;			/*	line buffer indices					*/
short		 needst = NO;	/*	set if ABS needed for conditionals	*/
short		*iptr;			/*	work ptr to any int buffer			*/
short		 outcol = 1;	/*	column counter for tab function		*/


char	mline [LINESIZE];	/*	temp macro buffer			*/
char	ssname [NAMESIZE];	/*	symbol name					*/
char	logo [40] = logtxt;	/*	banner text					*/
char	quote [2] = {'"', 0};	/*	literal string for "	*/
char	line [LINESIZE];	/*	parsing buffer				*/

#if _DOS || UNIX
FILE	*file_table [5] = { NULL, NULL, NULL, NULL, NULL } ;
char	base_dir [LINESIZE];	/*	Base-directory for sources	*/
char	*VarTypes [] =
{
	"Unk ",
	"Char",
	"Int ",
	""
};
char	*SymTypes [] =
{
	"Label   ",
	"Variable",
	"Array   ",
	"Function",
	"Pointer ",
	"Ptrar1  ",
	"Ptrar2  ",
	""
};
#endif	/*	#if _DOS || UNIX	*/


#else	/*	#ifdef C99_MAIN	*/


extern short cptr;			/*	work ptr to any char buffer		*/
extern short cptr2;			/*	work ptr to any char buffer		*/
extern short locptr;		/*	ptr to symtab entries			*/
extern char	*macptr;		/*	macro buffer index				*/
extern char	*lptrc;
extern char	*mptrc;			/*	ptrs to line buffers			*/
extern char	 msname [NAMESIZE];		/*	macro name						*/


extern short	 swdefault;		/*	default label #				*/
extern short	 wq [WQTABSZ];			/*	while queue					*/
extern short	 stkp;			/*	compiler relative stk ptr	*/
extern short	 wqindex;		/*	ptr to next entry			*/
extern short	*swnext;		/*	ptr to next entry			*/
extern short	 swq [SWTABSZ];	/*	switch table				*/
extern short	 swactive;		/*	true inside switch			*/
extern short	 iflv;			/*	#if... nesting level		*/
extern short	 skiplv;		/*	#if... level for skipping	*/
extern short	 pmode;			/*	true in #if... processing	*/
extern short	 test_mode;		/*	Non-zero = Diagnostic/debug mode.	*/
extern short	 numlin;		/*	# source lines processed	*/
extern short	 numglbs;		/*	# global symbols used		*/
extern short	 numlocs;		/*	max local symbols used		*/
extern short	 errcnt;		/*	# errors in compilation		*/
extern short	 errflg;		/*	true if error in statement	*/
extern short	 eof;			/*	set non-zero on final input eof */
extern short	 input;			/*	iob # for input file			*/
extern short	 output;		/*	iob # for output file (if any)	*/
extern short	 input2;		/*	iob # for "include" file		*/
extern short	 nxtlab;		/*	next avail label #				*/
extern short	 ncmp;			/*	# open compound statements		*/
extern short	 cmode;			/*	non-zero while parsing c-code	*/
							/*	zero when passing assembly code	*/
extern short	 decl;			/*	# local bytes declared			*/
extern short	 ctext;			/*	non-zero to intermix c-source	*/
extern short	 atext;			/*	non-zero to add assembly-source comments	*/
extern short	 rtext;			/*	non-zero to add "r" before register numbers	*/
extern short	 pushin;		/*	non-zero for input_line push code	*/
extern short	 lptr;			/*	line buffer indices				*/
extern short	 noloc;			/*	disables block locals			*/
extern short	 lastst;		/*	last executed statement type	*/
extern short	 litlab;		/*	label # assigned to literal pool	*/
extern short	 numltf;		/*	# local symbols in this fcn			*/
extern short	 nogo;			/*	disables goto						*/
extern short	 argtop;		/*	top of arg sp						*/
extern short	 argstk;		/*	function arg sp						*/
extern short	 mptr;			/*	line buffer indices					*/
extern short	 needst;		/*	set if ABS needed for conditionals	*/
extern short	*iptr;			/*	work ptr to any int buffer			*/
extern short	 litptr;		/*	ptr to lit pool entry				*/
extern short	 outcol;		/*	column counter for tab function		*/


extern char	mline [LINESIZE];		/*	temp macro buffer			*/
extern char	ssname [NAMESIZE];		/*	symbol name					*/
extern char	logo [40];				/*	banner text					*/
extern char	quote [2];				/*	literal string for "		*/
extern char	line [LINESIZE];		/*	parsing buffer				*/

#if _DOS || UNIX
extern FILE	*file_table [5];
extern char	base_dir [LINESIZE];	/*	Base-directory for sources	*/
extern char	*VarTypes [];
extern char	*SymTypes [];
#endif	/*	#if _DOS || UNIX	*/

#endif	/*	#ifdef C99_MAIN	*/

