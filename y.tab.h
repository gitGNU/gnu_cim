typedef union {
	long token;
	long ival;
	double rval;
	struct
	  {
	    long i1;
	    long i2;
	  } rasiival;
	char *ident;
	char *tval;
	char stat_decl;
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
#define	HVIRTUAL	312
#define	HWHEN	313
#define	HWHILE	314
#define	HASSIGNVALUE	315
#define	HASSIGNREF	316
#define	HPAREXPSEPARATOR	317
#define	HLABELSEPARATOR	318
#define	HSTATEMENTSEPARATOR	319
#define	HBEGPAR	320
#define	HENDPAR	321
#define	HEQR	322
#define	HNER	323
#define	HADD	324
#define	HSUB	325
#define	HMUL	326
#define	HDIV	327
#define	HINTDIV	328
#define	HEXP	329
#define	HDOTDOTDOT	330
#define	HIDENTIFIER	331
#define	HBOOLEANKONST	332
#define	HINTEGERKONST	333
#define	HCHARACTERKONST	334
#define	HREALKONST	335
#define	HTEXTKONST	336
#define	HASSIGN	337
#define	HORELSE	338
#define	HANDTHEN	339
#define	HEQV	340
#define	HIMP	341
#define	HOR	342
#define	HAND	343
#define	HNOT	344
#define	HVALRELOPERATOR	345
#define	HREFRELOPERATOR	346
#define	HOBJRELOPERATOR	347
#define	HTERMOPERATOR	348
#define	UNEAR	349
#define	HFACTOROPERATOR	350
#define	HPRIMARYOPERATOR	351
#define	HDOT	352


extern YYSTYPE yylval;
