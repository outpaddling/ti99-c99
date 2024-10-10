
/*      Routines defined in c99c1.c             */
int     main (void);
void    parse (void);
void    dumpglbsyms (void);
void    dumplocsyms (void);
void    dumpsyms (char *StartAddr, char *EndAddr);
void    dumplits (short typ);
void    sumstats (void);
void    summary (void);
void    ask_opts (void);
void    openout (void);
void    doinclude (void);
short   dodeclare (short class);
void    declglb (short typ, short class);   /* typ is CCHAR or CINT */
void    declloc (short typ);            /* typ is CCHAR or CINT */
short   needsub (void);
void    doentry (void);
short   cdim2 (void);
void    newfunc (void);
void    getarg (short t);    /* t = CCHAR or CINT */
short   statement (void);
void    doif (void);
void    dowhile (void);
void    doif (void);
void    dowhile (void);
void    dodo (void);
void    dofor (void);
void    doswitch (void);
void    docase (void);
void    ns (void);
void    doexpr (void);
void    compound (void);
void    dodefault (void);
void    dogoto (void);
short   dolabel (void);
void    doreturn (void);
void    dobreak (void);
void    docont (void);
void    doasm (void);

/*      Routines defined in c99c2.c             */
void     callfunction (char *ptr);              /* symbol table entry (or 0) */
short    initials (short typ,short id,short dim);
short    init (short typ,short id,short *dim);
void     junk (void);
short    endst (void);
void     illname (void);
void     multidef (void);
void     needbrack (char *str);
void     needlval (void);
void     cnserr (void);
void     ntcerr (void);
void     oflerr (void);
void     oocerr (void);
void     ndcerr (void);
short    findglb (char *sname); /*      Search for symbol in gloabl symbol-table.       */
short    findloc (char *sname); /*      Search for symbol in local symbol-table.        */
short    addglb (char *sname, short id, short typ, short value, short r);
short    addloc (char *sname, short id, short typ, short value, short r);
void     entsym (char *sname, char *Table, short id, short typ, short value, short rsiz);
short    addlbl (void);
short    symname (char *sname);
short    getlabel (void);
void     printlabel (short label);
void     prlbll (short label);
void     pl (char *str);
void     addwhile (short ptr []);
void     delwhile (void);
short    readwhile (short  ptr);
short    cur_ch (void);         /*      "peek-ahead" to current character from input file.      */
short    next_ch (void);        /*      "peek-ahead" to next character from input file.         */
short    get_ch (void);         /*      Get a character from the input file.                            */
short    isy (void);
void     kill  (void);
short    inbyte (void);
short    inchar (void);
void     input_line (void);
void     ifline (void);
void     noiferr (void);
short    keepch (short c);
void     preprocess (void);
void     addmac (void);
short    putmac (short c);
char    *findmac (char *sname);
short    outbyte (short c);
void     outstr (char *ptr);
void     outlbl (char *ptr);
void     otlbll (char ptr []);
void     outcom (char ptr[]);
void     newline (void);
void     tab (void);
void     errrpt (char *txt);
void     outl (char ptr[]);
void     ol (char ptr[]);
void     ot (char ptr[]);
short    match (char *lit);
short    amatch (char *lit, short len);
void     blanks (void);
short    PDBody (short item);
short    PHBody (short item);
short    outhex (short item);
short    outdec (short item);
void     outdcl (short item);
short    isptr (char *ptr);
short    ischar (char *ptr);
short    isword (short lval []);
short    ptrchk (short lval [],short lval2 []);
void     expression (void);
short    heir1 (short lval []);
short    heir1a (short lval []); 
short    heir1b (short lval []);
short    heir2 (short lval []);
short    heir3 (short lval []);
short    heir4 (short lval []);
short    heir5 (short lval []);
short    heir6 (short lval []);


/*      Routines defined in c99c3.c     */
short   heir7 (short lval []);
short   heir8 (short lval []);
short   heir9 (short lval []);
short   heir10 (short lval []);
short   heir11 (short lval []);
short   primary (short *lval);
short   setl2 (short lval [], char ptr []);
void    step1 (short lval [], short up);
void    step2 (short lval [], short up);
void    updn (short lval [], short up);
short   dolog (char *symb, char *opcode, char *dropop, char *endop, short (*heir)(), short lval []);
void    droptst (short k, char *opcode, short label, short lval []);
void    condexp (void);
void    address (char *ptr);
void    store (short lval []);
void    rvalue (short lval []);
void    test (short label, short paren);
void    briff (short label);
short   constant (short *val);
short   constx (short *val);
void    outval (short *lval);
short   hexnum (short *val);
short   number (short *val);
short   pstr (short *val);
short   qstr (short *val);
void    stolit (short val, short siz);
short   litchr (void);
void    comment (void);
void    header (void);
void    trailer (void);
void    ddata (void);
void    getmem (char *sym);
void    getloc (char *sym);
void    putmem (char *sym);
void    putstk (short typeobj);
void    indirect (short typeobj);
void    immed (void);
void    push (void);
void    pop (void);
void    ret ();
void    dcall (char *sname);
void    jump (short label);
short   modstk (short newsp);
void    doublereg (void);
void    add (void);
void    clrp (void);


/*      Routines defined in c99csb.c    */
short            getfn (char *text, char *m);

#if _DOS || UNIX
short    getlit (short litptr, short type);
void     putlit (short val, short litptr, short size);
short    tiputc (short c, short unit);
short    tfclose (short unit);
short    tfopen (char *name, char *mode);
short    getlit (short litptr, short type);
void     putlit (short val, short litptr, short size);
short    hash (char *sname);
short    alpha (short c);
short    alnum (short c);
short    numeric (short c);
short    getint (char *addr);
void     putint (short i, char *addr);
short    streq (char *str);
short    astreq (char str1 [], char str2 [], short len);
short    cfread (char *str1, short unit);
short    tiputchar (short c);
short    tiputs (char *str);
short    tigets (char *str, short max);
short    tifopen (char *name, char *mode);
short    tifclose (short unit);
#endif

