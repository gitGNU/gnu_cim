#define HACTIVATE 257
#define HAFTER 258
#define HARRAY 259
#define HAT 260
#define HBEFORE 261
#define HBEGIN 262
#define HBOOLEAN 263
#define HCHARACTER 264
#define HCLASS 265
#define HCONC 266
#define HDELAY 267
#define HDO 268
#define HELSE 269
#define HEND 270
#define HEQ 271
#define HEXTERNAL 272
#define HFOR 273
#define HGE 274
#define HGO 275
#define HGOTO 276
#define HGT 277
#define HHIDDEN 278
#define HIF 279
#define HIN 280
#define HINNER 281
#define HINSPECT 282
#define HINTEGER 283
#define HIS 284
#define HLABEL 285
#define HLE 286
#define HLONG 287
#define HLT 288
#define HNAME 289
#define HNE 290
#define HNEW 291
#define HNONE 292
#define HNOTEXT 293
#define HOTHERWISE 294
#define HPRIOR 295
#define HPROCEDURE 296
#define HPROTECTED 297
#define HQUA 298
#define HREACTIVATE 299
#define HREAL 300
#define HREF 301
#define HSHORT 302
#define HSTEP 303
#define HSWITCH 304
#define HTEXT 305
#define HTHEN 306
#define HTHIS 307
#define HTO 308
#define HUNTIL 309
#define HVALUE 310
#define HVIRTUAL 311
#define HWHEN 312
#define HWHILE 313
#define HASSIGNVALUE 314
#define HASSIGNREF 315
#define HPAREXPSEPARATOR 316
#define HLABELSEPARATOR 317
#define HSTATEMENTSEPARATOR 318
#define HBEGPAR 319
#define HENDPAR 320
#define HEQR 321
#define HNER 322
#define HADD 323
#define HSUB 324
#define HMUL 325
#define HDIV 326
#define HINTDIV 327
#define HEXP 328
#define HDOTDOTDOT 329
#define HIDENTIFIER 330
#define HBOOLEANKONST 331
#define HINTEGERKONST 332
#define HCHARACTERKONST 333
#define HREALKONST 334
#define HTEXTKONST 335
#define HASSIGN 336
#define HORELSE 337
#define HANDTHEN 338
#define HEQV 339
#define HIMP 340
#define HOR 341
#define HAND 342
#define HNOT 343
#define HVALRELOPERATOR 344
#define HREFRELOPERATOR 345
#define HOBJRELOPERATOR 346
#define HTERMOPERATOR 347
#define UNEAR 348
#define HFACTOROPERATOR 349
#define HPRIMARYOPERATOR 350
#define HDOT 351
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
extern YYSTYPE yylval;
