typedef union {
	long token;
	long ival;
        long arrdim;
	double rval;
	char *ident;
	char *tval;
	char stat_decl;
  	char kind;
       } YYSTYPE;
#define	HACTIVATE	258
#define	HAFTER	259
#define	HARRAY	260
#define	HAT	261
#define	HBEFORE	262
#define	HBEGIN	263
#define	HBOOLEAN	264
#define	HCHARACTER	265
#define	HCLASS	266
#define	HCONC	267
#define	HDELAY	268
#define	HDO	269
#define	HELSE	270
#define	HEND	271
#define	HEQ	272
#define	HEXTERNAL	273
#define	HFOR	274
#define	HGE	275
#define	HGO	276
#define	HGOTO	277
#define	HGT	278
#define	HHIDDEN	279
#define	HIF	280
#define	HIN	281
#define	HINNER	282
#define	HINSPECT	283
#define	HINTEGER	284
#define	HIS	285
#define	HLABEL	286
#define	HLE	287
#define	HLONG	288
#define	HLT	289
#define	HNAME	290
#define	HNE	291
#define	HNEW	292
#define	HNONE	293
#define	HNOTEXT	294
#define	HOTHERWISE	295
#define	HPRIOR	296
#define	HPROCEDURE	297
#define	HPROTECTED	298
#define	HQUA	299
#define	HREACTIVATE	300
#define	HREAL	301
#define	HREF	302
#define	HSHORT	303
#define	HSTEP	304
#define	HSWITCH	305
#define	HTEXT	306
#define	HTHEN	307
#define	HTHIS	308
#define	HTO	309
#define	HUNTIL	310
#define	HVALUE	311
#define	HVAR	312
#define	HVIRTUAL	313
#define	HWHEN	314
#define	HWHILE	315
#define	HASSIGNVALUE	316
#define	HASSIGNREF	317
#define	HPAREXPSEPARATOR	318
#define	HLABELSEPARATOR	319
#define	HSTATEMENTSEPARATOR	320
#define	HBEGPAR	321
#define	HENDPAR	322
#define	HEQR	323
#define	HNER	324
#define	HADD	325
#define	HSUB	326
#define	HMUL	327
#define	HDIV	328
#define	HINTDIV	329
#define	HEXP	330
#define	HDOTDOTDOT	331
#define	HIDENTIFIER	332
#define	HBOOLEANKONST	333
#define	HINTEGERKONST	334
#define	HCHARACTERKONST	335
#define	HREALKONST	336
#define	HTEXTKONST	337
#define	HASSIGN	338
#define	HORELSE	339
#define	HANDTHEN	340
#define	HEQV	341
#define	HIMP	342
#define	HOR	343
#define	HAND	344
#define	HNOT	345
#define	HVALRELOPERATOR	346
#define	HREFRELOPERATOR	347
#define	HOBJRELOPERATOR	348
#define	HTERMOPERATOR	349
#define	UNEAR	350
#define	HFACTOROPERATOR	351
#define	HPRIMARYOPERATOR	352
#define	HDOT	353


extern YYSTYPE yylval;
