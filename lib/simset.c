/*Cim_ccode*/
#include "./cim.h"
void __m_SIMSET();
__map __mapSIMSET[3]={"./simset.sim",0L,1L,
"",-123L,124L,
"",0L,2147483647L};
typedef struct /*  */
    {
        __dh h;
        __dhp c1;
        __dhp c2;
    } __bs0;
extern __bs0 __blokk0FILE;
extern __ptyp __p0FILE;
typedef struct /* FILE */
    {
        __dh h;
	__txt  filename;
	long  file;
	char  open;
	char  shared;
	char  append;
	char  create;
	char  readwrite;
	char  re_wind;
	char  purge;
    } __bs96;
extern __ptyp __p1FILE;
typedef struct /* IMAGEFILE */
    {
        __bs96 s;
	__txt  IMAGE;
    } __bs100;
extern __ptyp __p2FILE;
typedef struct /* OUTFILE */
    {
        __bs100 s;
    } __bs105;
extern __ptyp __p3FILE;
typedef struct /* INFILE */
    {
        __bs100 s;
	char  endfile;
    } __bs118;
extern __ptyp __p4FILE;
typedef struct /* DIRECTFILE */
    {
        __bs100 s;
	long  loc;
	long  maxloc;
	long  minwriteloc;
	long  imagelength;
	char  endfile;
	char  locked;
	char  lastop;
	char  writeonly;
    } __bs130;
extern __ptyp __p5FILE;
typedef struct /* PRINTFILE */
    {
        __bs105 s;
	long  line;
	long  lines_per_page;
	long  spacing;
	long  page;
    } __bs157;
extern __ptyp __p6FILE;
typedef struct /* BYTEFILE */
    {
        __bs96 s;
	char  endfile;
	char  bytesize;
    } __bs167;
extern __ptyp __p7FILE;
typedef struct /* INBYTEFILE */
    {
        __bs167 s;
    } __bs169;
extern __ptyp __p8FILE;
typedef struct /* OUTBYTEFILE */
    {
        __bs167 s;
    } __bs175;
extern __ptyp __p9FILE;
typedef struct /* DIRECTBYTEFILE */
    {
        __bs167 s;
	long  loc;
	long  maxloc;
	long  minwriteloc;
	char  locked;
	char  lastop;
	char  writeonly;
    } __bs181;
extern __ptyp __p10FILE;
typedef struct /*  */
    {
        __dh h;
    } __bs205;
__bs205 __blokk205SIMSET;
extern __ptyp __p205SIMSET;__pty   __pl205SIMSET[1]={&__p205SIMSET};
__ptyp __p205SIMSET={'B',0,1,sizeof(__bs205),0,0,0,0,0,0,0,0,__pl205SIMSET,__NULL};
extern __ptyp __p206SIMSET;
typedef struct /* SIMSET */
    {
        __dh h;
    } __bs206;
extern __ptyp __p206SIMSET;__pty   __pl206SIMSET[8]={&__p206SIMSET};
__ptyp __p206SIMSET={'C',0,2,sizeof(__bs206),2,__m_SIMSET,0,0,0,0,0,0,__pl206SIMSET,__NULL};
extern __ptyp __p207SIMSET;
typedef struct /* LINKAGE */
    {
        __dh h;
	__dhp  zzsuc;
	__dhp  zzpred;
    } __bs207;
short __rl207SIMSET[2]={(short)((char *)&((__bs207 *)0)->zzsuc-(char *)0),(short)((char *)&((__bs207 *)0)->zzpred-(char *)0),};
extern __ptyp __p207SIMSET;__pty   __pl207SIMSET[8]={&__p207SIMSET};
__ptyp __p207SIMSET={'C',0,3,sizeof(__bs207),5,__m_SIMSET,0,0,2,0,__rl207SIMSET,0,__pl207SIMSET,__NULL};
typedef struct /* SUC */
    {
        __dh h;
        __dhp er;
    } __bs208;
short __rl208SIMSET[1]={(short)((char *)&((__bs208 *)0)->er-(char *)0),};
extern __ptyp __p208SIMSET;__pty   __pl208SIMSET[8]={&__p208SIMSET};
__ptyp __p208SIMSET={'P',0,4,sizeof(__bs208),8,__m_SIMSET,0,0,1,0,__rl208SIMSET,0,__pl208SIMSET,__NULL};
typedef struct /* PRED */
    {
        __dh h;
        __dhp er;
    } __bs209;
short __rl209SIMSET[1]={(short)((char *)&((__bs209 *)0)->er-(char *)0),};
extern __ptyp __p209SIMSET;__pty   __pl209SIMSET[8]={&__p209SIMSET};
__ptyp __p209SIMSET={'P',0,4,sizeof(__bs209),9,__m_SIMSET,0,0,1,0,__rl209SIMSET,0,__pl209SIMSET,__NULL};
typedef struct /* PREV */
    {
        __dh h;
        __dhp er;
    } __bs210;
short __rl210SIMSET[1]={(short)((char *)&((__bs210 *)0)->er-(char *)0),};
extern __ptyp __p210SIMSET;__pty   __pl210SIMSET[8]={&__p210SIMSET};
__ptyp __p210SIMSET={'P',0,4,sizeof(__bs210),10,__m_SIMSET,0,0,1,0,__rl210SIMSET,0,__pl210SIMSET,__NULL};
extern __ptyp __p211SIMSET;
typedef struct /* LINK */
    {
        __bs207 s;
    } __bs211;
extern __ptyp __p211SIMSET;__pty   __pl211SIMSET[8]={&__p207SIMSET,&__p211SIMSET};
__ptyp __p211SIMSET={'C',1,3,sizeof(__bs211),11,__m_SIMSET,0,0,0,0,0,0,__pl211SIMSET,__NULL};
typedef struct /* OUT */
    {
        __dh h;
    } __bs212;
extern __ptyp __p212SIMSET;__pty   __pl212SIMSET[1]={&__p212SIMSET};
__ptyp __p212SIMSET={'P',0,4,sizeof(__bs212),14,__m_SIMSET,0,0,0,0,0,0,__pl212SIMSET,__NULL};
typedef struct /* FOLLOW */
    {
        __dh h;
	__dhp  PTR;
    } __bs213;
short __rl213SIMSET[1]={(short)((char *)&((__bs213 *)0)->PTR-(char *)0),};
extern __ptyp __p213SIMSET;__pty   __pl213SIMSET[1]={&__p213SIMSET};
__ptyp __p213SIMSET={'P',0,4,sizeof(__bs213),15,__m_SIMSET,0,0,1,0,__rl213SIMSET,0,__pl213SIMSET,__NULL};
typedef struct /* PRECEDE */
    {
        __dh h;
	__dhp  PTR;
    } __bs214;
short __rl214SIMSET[1]={(short)((char *)&((__bs214 *)0)->PTR-(char *)0),};
extern __ptyp __p214SIMSET;__pty   __pl214SIMSET[1]={&__p214SIMSET};
__ptyp __p214SIMSET={'P',0,4,sizeof(__bs214),16,__m_SIMSET,0,0,1,0,__rl214SIMSET,0,__pl214SIMSET,__NULL};
typedef struct /* INTO */
    {
        __dh h;
	__dhp  S;
    } __bs215;
short __rl215SIMSET[1]={(short)((char *)&((__bs215 *)0)->S-(char *)0),};
extern __ptyp __p215SIMSET;__pty   __pl215SIMSET[1]={&__p215SIMSET};
__ptyp __p215SIMSET={'P',0,4,sizeof(__bs215),17,__m_SIMSET,0,0,1,0,__rl215SIMSET,0,__pl215SIMSET,__NULL};
extern __ptyp __p216SIMSET;
typedef struct /* HEAD */
    {
        __bs207 s;
    } __bs216;
extern __ptyp __p216SIMSET;__pty   __pl216SIMSET[8]={&__p207SIMSET,&__p216SIMSET};
__ptyp __p216SIMSET={'C',1,3,sizeof(__bs216),18,__m_SIMSET,0,0,0,0,0,0,__pl216SIMSET,__NULL};
typedef struct /* FIRST */
    {
        __dh h;
        __dhp er;
    } __bs217;
short __rl217SIMSET[1]={(short)((char *)&((__bs217 *)0)->er-(char *)0),};
extern __ptyp __p217SIMSET;__pty   __pl217SIMSET[8]={&__p217SIMSET};
__ptyp __p217SIMSET={'P',0,4,sizeof(__bs217),21,__m_SIMSET,0,0,1,0,__rl217SIMSET,0,__pl217SIMSET,__NULL};
typedef struct /* LAST */
    {
        __dh h;
        __dhp er;
    } __bs218;
short __rl218SIMSET[1]={(short)((char *)&((__bs218 *)0)->er-(char *)0),};
extern __ptyp __p218SIMSET;__pty   __pl218SIMSET[8]={&__p218SIMSET};
__ptyp __p218SIMSET={'P',0,4,sizeof(__bs218),22,__m_SIMSET,0,0,1,0,__rl218SIMSET,0,__pl218SIMSET,__NULL};
typedef struct /* EMPTY */
    {
        __dh h;
        char ec;
    } __bs219;
extern __ptyp __p219SIMSET;__pty   __pl219SIMSET[1]={&__p219SIMSET};
__ptyp __p219SIMSET={'P',0,4,sizeof(__bs219),23,__m_SIMSET,0,0,0,0,0,0,__pl219SIMSET,__NULL};
typedef struct /* CARDINAL */
    {
        __dh h;
	long  I;
	__dhp  PTR;
        long ev;
    } __bs220;
short __rl220SIMSET[1]={(short)((char *)&((__bs220 *)0)->PTR-(char *)0),};
extern __ptyp __p220SIMSET;__pty   __pl220SIMSET[1]={&__p220SIMSET};
__ptyp __p220SIMSET={'P',0,4,sizeof(__bs220),24,__m_SIMSET,0,0,1,0,__rl220SIMSET,0,__pl220SIMSET,__NULL};
typedef struct /* CLEAR */
    {
        __dh h;
	__dhp  PTR;
	__dhp  PTRSUC;
    } __bs221;
short __rl221SIMSET[2]={(short)((char *)&((__bs221 *)0)->PTR-(char *)0),(short)((char *)&((__bs221 *)0)->PTRSUC-(char *)0),};
extern __ptyp __p221SIMSET;__pty   __pl221SIMSET[1]={&__p221SIMSET};
__ptyp __p221SIMSET={'P',0,4,sizeof(__bs221),25,__m_SIMSET,0,0,2,0,__rl221SIMSET,0,__pl221SIMSET,__NULL};
void __m_SIMSET(){goto __s;goto __ll0;/* START CLASS SIMSET *//* START CLASS LINKAGE *//* START PROCEDURE SUC */__l8:
# 31 "./simset.sim"

# 32 "./simset.sim"
;((__bs208 *)__lb)->er=(((__bp=(__rin(((__bs207 *)__lb->sl)->zzsuc,&__p211SIMSET)?((__bs207 *)__lb->sl)->zzsuc:__NULL))!=__NULL && (__bp->pp->pref[1]!= &__p211SIMSET))?(__dhp)__rerror(__errqual):__bp);__er=((__bs208 *)__lb)->er;__rep();goto __sw;/* SLUTT PROCEDURE SUC *//* START PROCEDURE PRED */__l9:
# 34 "./simset.sim"

# 35 "./simset.sim"
;((__bs209 *)__lb)->er=(((__bp=(__rin(((__bs207 *)__lb->sl)->zzpred,&__p211SIMSET)?((__bs207 *)__lb->sl)->zzpred:__NULL))!=__NULL && (__bp->pp->pref[1]!= &__p211SIMSET))?(__dhp)__rerror(__errqual):__bp);__er=((__bs209 *)__lb)->er;__rep();goto __sw;/* SLUTT PROCEDURE PRED *//* START PROCEDURE PREV */__l10:
# 37 "./simset.sim"
;((__bs210 *)__lb)->er=((__bs207 *)__lb->sl)->zzpred;__er=((__bs210 *)__lb)->er;__rep();goto __sw;/* SLUTT PROCEDURE PREV */__l5:
# 28 "./simset.sim"
__renddecl(0);goto __sw;__l6:__rinner(0);goto __sw;__l7:
# 39 "./simset.sim"
__rendclass(0);goto __sw;/* SLUTT CLASS LINKAGE *//* START CLASS LINK *//* START PROCEDURE OUT */__l14:
# 45 "./simset.sim"

# 46 "./simset.sim"
;if(!((((__bs207 *)__lb->sl)->zzsuc!=__NULL)))goto __ll1;
# 47 "./simset.sim"
;((__bs207 *)((__bp=((__bs207 *)__lb->sl)->zzsuc)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzpred=((__bs207 *)__lb->sl)->zzpred;
# 48 "./simset.sim"
;((__bs207 *)((__bp=((__bs207 *)__lb->sl)->zzpred)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsuc=((__bs207 *)__lb->sl)->zzsuc;
# 49 "./simset.sim"
;((__bs207 *)__lb->sl)->zzsuc=((__bs207 *)__lb->sl)->zzpred=__NULL;
# 50 "./simset.sim"
__ll1:__repp();goto __sw;/* SLUTT PROCEDURE OUT *//* START PROCEDURE FOLLOW */__l15:
# 52 "./simset.sim"
(((__bp=((__bs213 *)__lb)->PTR)!=__NULL && (__bp->pp->pref[0]!= &__p207SIMSET))?(__dhp)__rerror(__errqual):__bp);
# 53 "./simset.sim"
__sl=__lb->sl;__rcpp(&__p212SIMSET);__rcpb(26,__m_SIMSET);goto __sw;__l26:;;
# 54 "./simset.sim"
;if(!(((((__bs213 *)__lb)->PTR!=__NULL)&&(((__bs207 *)((__bp=((__bs213 *)__lb)->PTR)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsuc!=__NULL))))goto __ll2;
# 55 "./simset.sim"
;((__bs207 *)__lb->sl)->zzpred=((__bs213 *)__lb)->PTR;
# 56 "./simset.sim"
;((__bs207 *)__lb->sl)->zzsuc=((__bs207 *)((__bp=((__bs213 *)__lb)->PTR)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsuc;
# 57 "./simset.sim"
(__r[1]=(((__bs207 *)__lb->sl)->zzsuc));((__bs207 *)((__bp=__r[1])==__NULL?(__dhp)__rerror(__errnone):__bp))->zzpred=((__bs207 *)((__bp=((__bs213 *)__lb)->PTR)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsuc=__lb->sl;
# 58 "./simset.sim"
__ll2:__repp();goto __sw;/* SLUTT PROCEDURE FOLLOW *//* START PROCEDURE PRECEDE */__l16:
# 60 "./simset.sim"
(((__bp=((__bs214 *)__lb)->PTR)!=__NULL && (__bp->pp->pref[0]!= &__p207SIMSET))?(__dhp)__rerror(__errqual):__bp);
# 61 "./simset.sim"
__sl=__lb->sl;__rcpp(&__p212SIMSET);__rcpb(27,__m_SIMSET);goto __sw;__l27:;;
# 62 "./simset.sim"
;if(!(((((__bs214 *)__lb)->PTR!=__NULL)&&(((__bs207 *)((__bp=((__bs214 *)__lb)->PTR)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzpred!=__NULL))))goto __ll3;
# 63 "./simset.sim"
;((__bs207 *)__lb->sl)->zzsuc=((__bs214 *)__lb)->PTR;
# 64 "./simset.sim"
;((__bs207 *)__lb->sl)->zzpred=((__bs207 *)((__bp=((__bs214 *)__lb)->PTR)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzpred;
# 65 "./simset.sim"
(__r[1]=(((__bs207 *)__lb->sl)->zzpred));((__bs207 *)((__bp=__r[1])==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsuc=((__bs207 *)((__bp=((__bs214 *)__lb)->PTR)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzpred=__lb->sl;
# 66 "./simset.sim"
__ll3:__repp();goto __sw;/* SLUTT PROCEDURE PRECEDE *//* START PROCEDURE INTO */__l17:
# 68 "./simset.sim"
(((__bp=((__bs215 *)__lb)->S)!=__NULL && (__bp->pp->pref[1]!= &__p216SIMSET))?(__dhp)__rerror(__errqual):__bp);__sl=__lb->sl;__rcpp(&__p214SIMSET);((__bs214 *)__pb)->PTR=((__bs215 *)__lb)->S;__rcpb(28,__m_SIMSET);goto __sw;__l28:;;__repp();goto __sw;/* SLUTT PROCEDURE INTO */__l11:
# 42 "./simset.sim"
__renddecl(1);goto __sw;__l12:__rinner(1);goto __sw;__l13:
# 70 "./simset.sim"
__rendclass(1);goto __sw;/* SLUTT CLASS LINK *//* START CLASS HEAD *//* START PROCEDURE FIRST */__l21:
# 76 "./simset.sim"

# 78 "./simset.sim"
;((__bs217 *)__lb)->er=(((__bp=(__rin(((__bs207 *)__lb->sl)->zzsuc,&__p211SIMSET)?((__bs207 *)__lb->sl)->zzsuc:__NULL))!=__NULL && (__bp->pp->pref[1]!= &__p211SIMSET))?(__dhp)__rerror(__errqual):__bp);__er=((__bs217 *)__lb)->er;__rep();goto __sw;/* SLUTT PROCEDURE FIRST *//* START PROCEDURE LAST */__l22:
# 80 "./simset.sim"

# 82 "./simset.sim"
;((__bs218 *)__lb)->er=(((__bp=(__rin(((__bs207 *)__lb->sl)->zzpred,&__p211SIMSET)?((__bs207 *)__lb->sl)->zzpred:__NULL))!=__NULL && (__bp->pp->pref[1]!= &__p211SIMSET))?(__dhp)__rerror(__errqual):__bp);__er=((__bs218 *)__lb)->er;__rep();goto __sw;/* SLUTT PROCEDURE LAST *//* START PROCEDURE EMPTY */__l23:
# 84 "./simset.sim"
;(((__bs219 *)__lb)->ec=((((__bs207 *)__lb->sl)->zzsuc==__lb->sl)));__ev.c=((__bs219 *)__lb)->ec;__rep();goto __sw;/* SLUTT PROCEDURE EMPTY *//* START PROCEDURE CARDINAL */__l24:
# 86 "./simset.sim"

# 94 "./simset.sim"
;((__bs220 *)__lb)->PTR=((__bs207 *)__lb->sl)->zzsuc;
# 95 "./simset.sim"
__ll4:;if(!((((__bs220 *)__lb)->PTR!=__lb->sl)))goto __ll5;
# 96 "./simset.sim"
;(((__bs220 *)__lb)->I=((((__bs220 *)__lb)->I+1L)));
# 97 "./simset.sim"
;((__bs220 *)__lb)->PTR=((__bs207 *)((__bp=((__bs220 *)__lb)->PTR)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsuc;
# 98 "./simset.sim"
goto __ll4;__ll5:
# 101 "./simset.sim"
;(((__bs220 *)__lb)->ev=(((__bs220 *)__lb)->I));__ev.i=((__bs220 *)__lb)->ev;__rep();goto __sw;/* SLUTT PROCEDURE CARDINAL *//* START PROCEDURE CLEAR */__l25:
# 104 "./simset.sim"

# 107 "./simset.sim"
;((__bs221 *)__lb)->PTR=((__bs207 *)__lb->sl)->zzsuc;
# 108 "./simset.sim"
__ll6:;if(!((((__bs221 *)__lb)->PTR!=__lb->sl)))goto __ll7;
# 110 "./simset.sim"
;((__bs221 *)__lb)->PTRSUC=((__bs207 *)((__bp=((__bs221 *)__lb)->PTR)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsuc;
# 111 "./simset.sim"
(__r[1]=(((__bs221 *)__lb)->PTR));((__bs207 *)((__bp=__r[1])==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsuc=((__bs207 *)((__bp=((__bs221 *)__lb)->PTR)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzpred=__NULL;
# 112 "./simset.sim"
;((__bs221 *)__lb)->PTR=((__bs221 *)__lb)->PTRSUC;
# 113 "./simset.sim"
goto __ll6;__ll7:
# 115 "./simset.sim"
;((__bs207 *)__lb->sl)->zzsuc=((__bs207 *)__lb->sl)->zzpred=__lb->sl;__repp();goto __sw;/* SLUTT PROCEDURE CLEAR */__l18:
# 73 "./simset.sim"
__renddecl(1);goto __sw;__l19:
# 118 "./simset.sim"
;((__bs207 *)__lb)->zzsuc=((__bs207 *)__lb)->zzpred=__lb;
# 73 "./simset.sim"
__rinner(1);goto __sw;__l20:
# 118 "./simset.sim"
__rendclass(1);goto __sw;/* SLUTT CLASS HEAD */__l2:
# 25 "./simset.sim"
__renddecl(0);goto __sw;__l3:__rinner(0);goto __sw;__l4:
# 120 "./simset.sim"
__rendclass(0);goto __sw;/* SLUTT CLASS SIMSET */__ll0:__sw:if(__goto.ment!=(void (*)())__m_SIMSET)return;__s:switch(__goto.ent){case 2: goto __l2;
case 3: goto __l3;
case 4: goto __l4;
case 5: goto __l5;
case 6: goto __l6;
case 7: goto __l7;
case 8: goto __l8;
case 9: goto __l9;
case 10: goto __l10;
case 11: goto __l11;
case 12: goto __l12;
case 13: goto __l13;
case 14: goto __l14;
case 15: goto __l15;
case 16: goto __l16;
case 17: goto __l17;
case 18: goto __l18;
case 19: goto __l19;
case 20: goto __l20;
case 21: goto __l21;
case 22: goto __l22;
case 23: goto __l23;
case 24: goto __l24;
case 25: goto __l25;
case 26: goto __l26;
case 27: goto __l27;
case 28: goto __l28;
}}
