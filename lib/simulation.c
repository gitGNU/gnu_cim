/*Cim_ccode*/
#include "./cim.h"
struct __tt1 {__txt tvar;__th h;char string[27];}
__tk1SIMULATION={(__textref)&__tk1SIMULATION.h.pp,26,1,1,(__pty)__TEXT,(__dhp)&__tk1SIMULATION.h.pp,__CONSTANT,26,"No\040Evtime\040for\040idle\040process"};
struct __tt2 {__txt tvar;__th h;char string[11];}
__tk2SIMULATION={(__textref)&__tk2SIMULATION.h.pp,10,1,1,(__pty)__TEXT,(__dhp)&__tk2SIMULATION.h.pp,__CONSTANT,10,"SQS:\040Empty"};
struct __tt3 {__txt tvar;__th h;char string[24];}
__tk3SIMULATION={(__textref)&__tk3SIMULATION.h.pp,23,1,1,(__pty)__TEXT,(__dhp)&__tk3SIMULATION.h.pp,__CONSTANT,23,"SQS:\040Terminated\040process"};
struct __tt4 {__txt tvar;__th h;char string[11];}
__tk4SIMULATION={(__textref)&__tk4SIMULATION.h.pp,10,1,1,(__pty)__TEXT,(__dhp)&__tk4SIMULATION.h.pp,__CONSTANT,10,"SQS:\040Empty"};
struct __tt5 {__txt tvar;__th h;char string[11];}
__tk5SIMULATION={(__textref)&__tk5SIMULATION.h.pp,10,1,1,(__pty)__TEXT,(__dhp)&__tk5SIMULATION.h.pp,__CONSTANT,10,"SQS:\040Empty"};
struct __tt6 {__txt tvar;__th h;char string[11];}
__tk6SIMULATION={(__textref)&__tk6SIMULATION.h.pp,10,1,1,(__pty)__TEXT,(__dhp)&__tk6SIMULATION.h.pp,__CONSTANT,10,"SQS:\040Empty"};
struct __tt7 {__txt tvar;__th h;char string[11];}
__tk7SIMULATION={(__textref)&__tk7SIMULATION.h.pp,10,1,1,(__pty)__TEXT,(__dhp)&__tk7SIMULATION.h.pp,__CONSTANT,10,"SQS:\040Empty"};
void __m_SIMULATION();
__map __mapSIMULATION[3]={"simulation.sim",0L,1L,
"",-233L,234L,
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
__bs205 __blokk205SIMULATION;
extern __ptyp __p205SIMULATION;__pty   __pl205SIMULATION[1]={&__p205SIMULATION};
__ptyp __p205SIMULATION={'B',0,1,sizeof(__bs205),0,0,0,0,0,0,0,0,__pl205SIMULATION,__NULL};
extern void __m_SIMSET();
typedef struct /* SIMSET */
    {
        __dh h;
    } __bs206;
extern __ptyp __p206SIMSET;
typedef struct /* LINKAGE */
    {
        __dh h;
	__dhp  zzsuc;
	__dhp  zzpred;
    } __bs207;
extern __ptyp __p207SIMSET;
typedef struct /* SUC */
    {
        __dh h;
        __dhp er;
    } __bs208;
extern __ptyp __p208SIMSET;
typedef struct /* PRED */
    {
        __dh h;
        __dhp er;
    } __bs209;
extern __ptyp __p209SIMSET;
typedef struct /* PREV */
    {
        __dh h;
        __dhp er;
    } __bs210;
extern __ptyp __p210SIMSET;
typedef struct /* LINK */
    {
        __bs207 s;
    } __bs211;
extern __ptyp __p211SIMSET;
typedef struct /* OUT */
    {
        __dh h;
    } __bs212;
extern __ptyp __p212SIMSET;
typedef struct /* FOLLOW */
    {
        __dh h;
	__dhp  PTR;
    } __bs213;
extern __ptyp __p213SIMSET;
typedef struct /* PRECEDE */
    {
        __dh h;
	__dhp  PTR;
    } __bs214;
extern __ptyp __p214SIMSET;
typedef struct /* INTO */
    {
        __dh h;
	__dhp  S;
    } __bs215;
extern __ptyp __p215SIMSET;
typedef struct /* HEAD */
    {
        __bs207 s;
    } __bs216;
extern __ptyp __p216SIMSET;
typedef struct /* FIRST */
    {
        __dh h;
        __dhp er;
    } __bs217;
extern __ptyp __p217SIMSET;
typedef struct /* LAST */
    {
        __dh h;
        __dhp er;
    } __bs218;
extern __ptyp __p218SIMSET;
typedef struct /* EMPTY */
    {
        __dh h;
        char ec;
    } __bs219;
extern __ptyp __p219SIMSET;
typedef struct /* CARDINAL */
    {
        __dh h;
        long ev;
    } __bs220;
extern __ptyp __p220SIMSET;
typedef struct /* CLEAR */
    {
        __dh h;
    } __bs221;
extern __ptyp __p221SIMSET;
extern __ptyp __p222SIMULATION;
typedef struct /* SIMULATION */
    {
        __bs206 s;
	__dhp  zzsqs;
	__dhp  MAIN;
        __dhp __r1;
    } __bs222;
short __rl222SIMULATION[3]={(short)((char *)&((__bs222 *)0)->__r1-(char *)0),(short)((char *)&((__bs222 *)0)->zzsqs-(char *)0),(short)((char *)&((__bs222 *)0)->MAIN-(char *)0),};
extern __ptyp __p222SIMULATION;__pty   __pl222SIMULATION[8]={&__p206SIMSET,&__p222SIMULATION};
__ptyp __p222SIMULATION={'C',1,2,sizeof(__bs222),2,__m_SIMULATION,0,0,3,0,__rl222SIMULATION,0,__pl222SIMULATION,__NULL};
typedef struct /* CURRENT */
    {
        __dh h;
        __dhp er;
    } __bs223;
short __rl223SIMULATION[1]={(short)((char *)&((__bs223 *)0)->er-(char *)0),};
extern __ptyp __p223SIMULATION;__pty   __pl223SIMULATION[8]={&__p223SIMULATION};
__ptyp __p223SIMULATION={'P',0,3,sizeof(__bs223),5,__m_SIMULATION,0,0,1,0,__rl223SIMULATION,0,__pl223SIMULATION,__NULL};
typedef struct /* TIME */
    {
        __dh h;
        double ef;
    } __bs224;
extern __ptyp __p224SIMULATION;__pty   __pl224SIMULATION[1]={&__p224SIMULATION};
__ptyp __p224SIMULATION={'P',0,3,sizeof(__bs224),6,__m_SIMULATION,0,0,0,0,0,0,__pl224SIMULATION,__NULL};
extern __ptyp __p225SIMULATION;
typedef struct /* PROCESS */
    {
        __bs211 s;
	__dhp  zzsqssuc;
	__dhp  zzsqspred;
	double  zzevtime;
	char  zzterminated_process;
    } __bs225;
short __rl225SIMULATION[2]={(short)((char *)&((__bs225 *)0)->zzsqssuc-(char *)0),(short)((char *)&((__bs225 *)0)->zzsqspred-(char *)0),};
extern __ptyp __p225SIMULATION;__pty   __pl225SIMULATION[8]={&__p207SIMSET,&__p211SIMSET,&__p225SIMULATION};
__ptyp __p225SIMULATION={'C',2,3,sizeof(__bs225),7,__m_SIMULATION,0,0,2,0,__rl225SIMULATION,0,__pl225SIMULATION,__NULL};
typedef struct /* IDLE */
    {
        __dh h;
        char ec;
    } __bs226;
extern __ptyp __p226SIMULATION;__pty   __pl226SIMULATION[1]={&__p226SIMULATION};
__ptyp __p226SIMULATION={'P',0,4,sizeof(__bs226),10,__m_SIMULATION,0,0,0,0,0,0,__pl226SIMULATION,__NULL};
typedef struct /* TERMINATED */
    {
        __dh h;
        char ec;
    } __bs227;
extern __ptyp __p227SIMULATION;__pty   __pl227SIMULATION[1]={&__p227SIMULATION};
__ptyp __p227SIMULATION={'P',0,4,sizeof(__bs227),11,__m_SIMULATION,0,0,0,0,0,0,__pl227SIMULATION,__NULL};
typedef struct /* EVTIME */
    {
        __dh h;
        double ef;
    } __bs228;
extern __ptyp __p228SIMULATION;__pty   __pl228SIMULATION[1]={&__p228SIMULATION};
__ptyp __p228SIMULATION={'P',0,4,sizeof(__bs228),12,__m_SIMULATION,0,0,0,0,0,0,__pl228SIMULATION,__NULL};
typedef struct /* NEXTEV */
    {
        __dh h;
        __dhp er;
    } __bs229;
short __rl229SIMULATION[1]={(short)((char *)&((__bs229 *)0)->er-(char *)0),};
extern __ptyp __p229SIMULATION;__pty   __pl229SIMULATION[8]={&__p229SIMULATION};
__ptyp __p229SIMULATION={'P',0,4,sizeof(__bs229),13,__m_SIMULATION,0,0,1,0,__rl229SIMULATION,0,__pl229SIMULATION,__NULL};
typedef struct /* activat */
    {
        __dh h;
	char  REAC;
	__dhp  X;
	char  CODE;
	double  T;
	__dhp  Y;
	char  PRIO;
	__dhp  b;
	__dhp  cur;
	double  tm;
    } __bs230;
short __rl230SIMULATION[4]={(short)((char *)&((__bs230 *)0)->X-(char *)0),(short)((char *)&((__bs230 *)0)->Y-(char *)0),(short)((char *)&((__bs230 *)0)->b-(char *)0),(short)((char *)&((__bs230 *)0)->cur-(char *)0),};
extern __ptyp __p230SIMULATION;__pty   __pl230SIMULATION[1]={&__p230SIMULATION};
__ptyp __p230SIMULATION={'P',0,3,sizeof(__bs230),14,__m_SIMULATION,0,0,4,0,__rl230SIMULATION,0,__pl230SIMULATION,__NULL};
typedef struct /* HOLD */
    {
        __dh h;
	double  t;
	__dhp  p;
	__dhp  q;
    } __bs231;
short __rl231SIMULATION[2]={(short)((char *)&((__bs231 *)0)->p-(char *)0),(short)((char *)&((__bs231 *)0)->q-(char *)0),};
extern __ptyp __p231SIMULATION;__pty   __pl231SIMULATION[1]={&__p231SIMULATION};
__ptyp __p231SIMULATION={'P',0,3,sizeof(__bs231),15,__m_SIMULATION,0,0,2,0,__rl231SIMULATION,0,__pl231SIMULATION,__NULL};
typedef struct /* PASSIVATE */
    {
        __dh h;
	__dhp  p;
    } __bs232;
short __rl232SIMULATION[1]={(short)((char *)&((__bs232 *)0)->p-(char *)0),};
extern __ptyp __p232SIMULATION;__pty   __pl232SIMULATION[1]={&__p232SIMULATION};
__ptyp __p232SIMULATION={'P',0,3,sizeof(__bs232),16,__m_SIMULATION,0,0,1,0,__rl232SIMULATION,0,__pl232SIMULATION,__NULL};
typedef struct /* WAIT */
    {
        __dh h;
	__dhp  S;
	__dhp  p;
    } __bs233;
short __rl233SIMULATION[2]={(short)((char *)&((__bs233 *)0)->S-(char *)0),(short)((char *)&((__bs233 *)0)->p-(char *)0),};
extern __ptyp __p233SIMULATION;__pty   __pl233SIMULATION[1]={&__p233SIMULATION};
__ptyp __p233SIMULATION={'P',0,3,sizeof(__bs233),17,__m_SIMULATION,0,0,2,0,__rl233SIMULATION,0,__pl233SIMULATION,__NULL};
typedef struct /* CANCEL */
    {
        __dh h;
	__dhp  x;
	__dhp  cur;
    } __bs234;
short __rl234SIMULATION[2]={(short)((char *)&((__bs234 *)0)->x-(char *)0),(short)((char *)&((__bs234 *)0)->cur-(char *)0),};
extern __ptyp __p234SIMULATION;__pty   __pl234SIMULATION[1]={&__p234SIMULATION};
__ptyp __p234SIMULATION={'P',0,3,sizeof(__bs234),18,__m_SIMULATION,0,0,2,0,__rl234SIMULATION,0,__pl234SIMULATION,__NULL};
extern __ptyp __p235SIMULATION;
typedef struct /* zzmain_program */
    {
        __bs225 s;
    } __bs235;
extern __ptyp __p235SIMULATION;__pty   __pl235SIMULATION[8]={&__p207SIMSET,&__p211SIMSET,&__p225SIMULATION,&__p235SIMULATION};
__ptyp __p235SIMULATION={'C',3,3,sizeof(__bs235),19,__m_SIMULATION,0,0,0,0,0,0,__pl235SIMULATION,__NULL};
typedef struct /* ACCUM */
    {
        __dh h;
	__aritnamepar  A;
	__aritnamepar  B;
	__aritnamepar  C;
	double  D;
        __dhp __r1;
        __valuetype __v1;
        __valuetype __v2;
        __valuetype __v3;
        __valuetype __v4;
        __valuetype __v5;
    } __bs236;
short __rl236SIMULATION[7]={(short)((char *)&((__bs236 *)0)->__r1-(char *)0),(short)((char *)&((__bs236 *)0)->A.bp-(char *)0),(short)((char *)&((__bs236 *)0)->A.sl-(char *)0),(short)((char *)&((__bs236 *)0)->B.bp-(char *)0),(short)((char *)&((__bs236 *)0)->B.sl-(char *)0),(short)((char *)&((__bs236 *)0)->C.bp-(char *)0),(short)((char *)&((__bs236 *)0)->C.sl-(char *)0),};
extern __ptyp __p236SIMULATION;__pty   __pl236SIMULATION[1]={&__p236SIMULATION};
__ptyp __p236SIMULATION={'P',0,3,sizeof(__bs236),22,__m_SIMULATION,0,0,7,0,__rl236SIMULATION,0,__pl236SIMULATION,__NULL};
void __m_SIMULATION(void){goto __s;
# 27 "simulation.sim"
__sto= (__dhp)&__blokk205SIMULATION;__rb(&__p205SIMULATION);goto __ll0;/* START CLASS SIMULATION *//* START PROCEDURE CURRENT */__l5:
# 33 "simulation.sim"
;((__bs223 *)__lb)->er=((__bs225 *)((__bp=((__bs222 *)__lb->sl)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc;__er=((__bs223 *)__lb)->er;__rep();goto __sw;/* SLUTT PROCEDURE CURRENT *//* START PROCEDURE TIME */__l6:
# 35 "simulation.sim"
;(((__bs224 *)__lb)->ef=(((__bs225 *)((__bp=((__bs225 *)((__bp=((__bs222 *)__lb->sl)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzevtime));__ev.f=((__bs224 *)__lb)->ef;__rep();goto __sw;/* SLUTT PROCEDURE TIME *//* START CLASS PROCESS *//* START PROCEDURE IDLE */__l10:
# 46 "simulation.sim"
;(((__bs226 *)__lb)->ec=((((__bs225 *)__lb->sl)->zzsqssuc==__NULL)));__ev.c=((__bs226 *)__lb)->ec;__rep();goto __sw;/* SLUTT PROCEDURE IDLE *//* START PROCEDURE TERMINATED */__l11:
# 48 "simulation.sim"
;(((__bs227 *)__lb)->ec=(((__bs225 *)__lb->sl)->zzterminated_process));__ev.c=((__bs227 *)__lb)->ec;__rep();goto __sw;/* SLUTT PROCEDURE TERMINATED *//* START PROCEDURE EVTIME */__l12:
# 50 "simulation.sim"

# 51 "simulation.sim"
;if(!((((__bs225 *)__lb->sl)->zzsqssuc==__NULL)))goto __ll2;
# 52 "simulation.sim"
;__rterror((__txtvp)&__tk1SIMULATION);goto __ll1;__ll2:;(((__bs228 *)__lb)->ef=(((__bs225 *)__lb->sl)->zzevtime));__ll1:__ev.f=((__bs228 *)__lb)->ef;__rep();goto __sw;/* SLUTT PROCEDURE EVTIME *//* START PROCEDURE NEXTEV */__l13:
# 54 "simulation.sim"

# 56 "simulation.sim"
;((__bs229 *)__lb)->er=(((__bp=(((((__bs225 *)__lb->sl)->zzsqssuc==__NULL)||(((__bs225 *)__lb->sl)->zzsqssuc==((__bs222 *)__lb->sl->sl)->zzsqs))?__NULL:((__bs225 *)__lb->sl)->zzsqssuc))!=__NULL && (__bp->pp->pref[2]!= &__p225SIMULATION))?(__dhp)__rerror(__errqual):__bp);__er=((__bs229 *)__lb)->er;__rep();goto __sw;/* SLUTT PROCEDURE NEXTEV */__l7:
# 37 "simulation.sim"
__renddecl(2);goto __sw;__l8:
# 58 "simulation.sim"
;((__bs225 *)__lb)->zzsqssuc=((__bs225 *)__lb)->zzsqspred=__NULL;
# 60 "simulation.sim"
;__rdetach(__lb,23,__m_SIMULATION);goto __sw;__l23:;
# 61 "simulation.sim"
__rinner(2);goto __sw;__l9:
# 62 "simulation.sim"
;(((__bs225 *)__lb)->zzterminated_process=(1));
# 65 "simulation.sim"
;((__bs225 *)((__bp=((__bs225 *)__lb)->zzsqssuc)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred=((__bs225 *)__lb)->zzsqspred;
# 66 "simulation.sim"
;((__bs225 *)((__bp=((__bs225 *)__lb)->zzsqspred)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc=((__bs225 *)__lb)->zzsqssuc;
# 67 "simulation.sim"
;((__bs225 *)__lb)->zzsqspred=((__bs225 *)__lb)->zzsqssuc=__NULL;
# 69 "simulation.sim"
;if(!((((__bs225 *)((__bp=((__bs222 *)__lb->sl)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc==((__bs222 *)__lb->sl)->zzsqs)))goto __ll4;
# 70 "simulation.sim"
;__rterror((__txtvp)&__tk2SIMULATION);goto __ll3;__ll4:;__rresume(((__bs225 *)((__bp=((__bs222 *)__lb->sl)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc,24,__m_SIMULATION);goto __sw;__l24:;__ll3:
# 72 "simulation.sim"
;__rterror((__txtvp)&__tk3SIMULATION);
# 73 "simulation.sim"
__rendclass(2);goto __sw;/* SLUTT CLASS PROCESS *//* START PROCEDURE activat */__l14:
# 75 "simulation.sim"
(((__bp=((__bs230 *)__lb)->X)!=__NULL && (__bp->pp->pref[2]!= &__p225SIMULATION))?(__dhp)__rerror(__errqual):__bp);(((__bp=((__bs230 *)__lb)->Y)!=__NULL && (__bp->pp->pref[2]!= &__p225SIMULATION))?(__dhp)__rerror(__errqual):__bp);
# 85 "simulation.sim"
;if(!(((((__bs230 *)__lb)->X!=__NULL)&&((!((__bs225 *)((__bp=((__bs230 *)__lb)->X)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzterminated_process)&&(((__bs230 *)__lb)->REAC||(((__bs225 *)((__bp=((__bs230 *)__lb)->X)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc==__NULL))))))goto __ll5;
# 87 "simulation.sim"
;((__bs230 *)__lb)->cur=((__bs225 *)((__bp=((__bs222 *)__lb->sl)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc;;(((__bs230 *)__lb)->tm=(((__bs225 *)((__bp=((__bs230 *)__lb)->cur)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzevtime));
# 89 "simulation.sim"
;if(!(((unsigned char)((__bs230 *)__lb)->CODE==(unsigned char)1)))goto __ll7;
# 91 "simulation.sim"
;if(!((((__bs230 *)__lb)->X==((__bs230 *)__lb)->cur)))goto __ll8;;goto __l25;__ll8:
# 92 "simulation.sim"
;(((__bs230 *)__lb)->T=(((__bs230 *)__lb)->tm));;((__bs230 *)__lb)->b=((__bs222 *)__lb->sl)->zzsqs;
# 93 "simulation.sim"
goto __ll6;__ll7:
# 94 "simulation.sim"
;if(!(((unsigned char)((__bs230 *)__lb)->CODE==(unsigned char)2)))goto __ll10;
# 96 "simulation.sim"
;if(!((((__bs230 *)__lb)->T<=((__bs230 *)__lb)->tm)))goto __ll11;
# 97 "simulation.sim"
;if(!((((__bs230 *)__lb)->PRIO&&(((__bs230 *)__lb)->X==((__bs230 *)__lb)->cur))))goto __ll13;;goto __l25;__ll13:;(((__bs230 *)__lb)->T=(((__bs230 *)__lb)->tm));__ll12:__ll11:
# 98 "simulation.sim"
goto __ll9;__ll10:
# 99 "simulation.sim"
;if(!(((unsigned char)((__bs230 *)__lb)->CODE==(unsigned char)3)))goto __ll15;
# 101 "simulation.sim"
;(((__bs230 *)__lb)->T=((((__bs230 *)__lb)->T+((__bs230 *)__lb)->tm)));
# 102 "simulation.sim"
;if(!((((__bs230 *)__lb)->T<=((__bs230 *)__lb)->tm)))goto __ll16;
# 103 "simulation.sim"
;if(!((((__bs230 *)__lb)->PRIO&&(((__bs230 *)__lb)->X==((__bs230 *)__lb)->cur))))goto __ll18;;goto __l25;__ll18:;(((__bs230 *)__lb)->T=(((__bs230 *)__lb)->tm));__ll17:__ll16:
# 104 "simulation.sim"
goto __ll14;__ll15:
# 107 "simulation.sim"
;if(!(((((__bs230 *)__lb)->Y==__NULL)||(((__bs225 *)((__bp=((__bs230 *)__lb)->Y)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc==__NULL))))goto __ll19;
# 109 "simulation.sim"
;if(!((((__bs225 *)((__bp=((__bs230 *)__lb)->X)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc!=__NULL)))goto __ll20;
# 111 "simulation.sim"
;((__bs225 *)((__bp=((__bs225 *)((__bp=((__bs230 *)__lb)->X)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred=((__bs225 *)((__bp=((__bs230 *)__lb)->X)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred;
# 112 "simulation.sim"
;((__bs225 *)((__bp=((__bs225 *)((__bp=((__bs230 *)__lb)->X)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc=((__bs225 *)((__bp=((__bs230 *)__lb)->X)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc;
# 113 "simulation.sim"
;((__bs225 *)((__bp=((__bs230 *)__lb)->X)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred=__NULL;;((__bs225 *)((__bp=((__bs230 *)__lb)->X)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc=__NULL;
# 114 "simulation.sim"
__ll20:
# 116 "simulation.sim"
;if(!((((__bs225 *)((__bp=((__bs222 *)__lb->sl)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc==((__bs222 *)__lb->sl)->zzsqs)))goto __ll21;;__rterror((__txtvp)&__tk4SIMULATION);__ll21:
# 117 "simulation.sim"
;goto __l25;__ll19:
# 120 "simulation.sim"
;if(!((((__bs230 *)__lb)->X==((__bs230 *)__lb)->Y)))goto __ll22;;goto __l25;__ll22:
# 122 "simulation.sim"
;(((__bs230 *)__lb)->T=(((__bs225 *)((__bp=((__bs230 *)__lb)->Y)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzevtime));
# 124 "simulation.sim"
;if(!(((unsigned char)((__bs230 *)__lb)->CODE==(unsigned char)4)))goto __ll24;;((__bs230 *)__lb)->b=((__bs225 *)((__bp=((__bs230 *)__lb)->Y)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred;goto __ll23;__ll24:;((__bs230 *)__lb)->b=((__bs230 *)__lb)->Y;__ll23:
# 125 "simulation.sim"
__ll14:__ll9:__ll6:
# 127 "simulation.sim"
;if(!((((__bs225 *)((__bp=((__bs230 *)__lb)->X)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc!=__NULL)))goto __ll25;
# 129 "simulation.sim"
;((__bs225 *)((__bp=((__bs225 *)((__bp=((__bs230 *)__lb)->X)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred=((__bs225 *)((__bp=((__bs230 *)__lb)->X)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred;
# 130 "simulation.sim"
;((__bs225 *)((__bp=((__bs225 *)((__bp=((__bs230 *)__lb)->X)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc=((__bs225 *)((__bp=((__bs230 *)__lb)->X)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc;
# 131 "simulation.sim"
__ll25:
# 133 "simulation.sim"
;if(!((((__bs230 *)__lb)->b==__NULL)))goto __ll26;
# 135 "simulation.sim"
;((__bs230 *)__lb)->b=((__bs225 *)((__bp=((__bs222 *)__lb->sl)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred;
# 136 "simulation.sim"
__ll27:;if(!((((__bs225 *)((__bp=((__bs230 *)__lb)->b)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzevtime>((__bs230 *)__lb)->T)))goto __ll28;;((__bs230 *)__lb)->b=((__bs225 *)((__bp=((__bs230 *)__lb)->b)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred;goto __ll27;__ll28:
# 137 "simulation.sim"
;if(!(((__bs230 *)__lb)->PRIO))goto __ll29;
# 138 "simulation.sim"
__ll30:;if(!((((__bs225 *)((__bp=((__bs230 *)__lb)->b)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzevtime==((__bs230 *)__lb)->T)))goto __ll31;;((__bs230 *)__lb)->b=((__bs225 *)((__bp=((__bs230 *)__lb)->b)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred;goto __ll30;__ll31:__ll29:
# 139 "simulation.sim"
__ll26:
# 141 "simulation.sim"
;(((__bs225 *)((__bp=((__bs230 *)__lb)->X)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzevtime=(((__bs230 *)__lb)->T));
# 142 "simulation.sim"
;((__bs225 *)((__bp=((__bs230 *)__lb)->X)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred=((__bs230 *)__lb)->b;;((__bs225 *)((__bp=((__bs230 *)__lb)->X)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc=((__bs225 *)((__bp=((__bs230 *)__lb)->b)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc;
# 143 "simulation.sim"
;((__bs225 *)((__bp=((__bs230 *)__lb)->b)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc=((__bs230 *)__lb)->X;;((__bs225 *)((__bp=((__bs225 *)((__bp=((__bs230 *)__lb)->X)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred=((__bs230 *)__lb)->X;
# 145 "simulation.sim"
;if(!((((__bs225 *)((__bp=((__bs222 *)__lb->sl)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc!=((__bs230 *)__lb)->cur)))goto __ll32;;__rresume(((__bs225 *)((__bp=((__bs222 *)__lb->sl)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc,26,__m_SIMULATION);goto __sw;__l26:;__ll32:
# 146 "simulation.sim"
__ll5:
# 147 "simulation.sim"
/*exit_230*/__l25:
# 148 "simulation.sim"
__repp();goto __sw;/* SLUTT PROCEDURE activat *//* START PROCEDURE HOLD */__l15:
# 150 "simulation.sim"

# 153 "simulation.sim"
;((__bs231 *)__lb)->p=((__bs225 *)((__bp=((__bs222 *)__lb->sl)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc;
# 154 "simulation.sim"
;if(!((((__bs231 *)__lb)->t> 0.0000000000000000e+00)))goto __ll33;;(((__bs225 *)((__bp=((__bs231 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzevtime=((((__bs225 *)((__bp=((__bs231 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzevtime+((__bs231 *)__lb)->t)));__ll33:
# 155 "simulation.sim"
;(((__bs231 *)__lb)->t=(((__bs225 *)((__bp=((__bs231 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzevtime));
# 156 "simulation.sim"
;if(!(((((__bs225 *)((__bp=((__bs231 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc!=((__bs222 *)__lb->sl)->zzsqs)&&(((__bs225 *)((__bp=((__bs225 *)((__bp=((__bs231 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzevtime<=((__bs231 *)__lb)->t))))goto __ll34;
# 158 "simulation.sim"
;((__bs225 *)((__bp=((__bs225 *)((__bp=((__bs231 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred=((__bs225 *)((__bp=((__bs231 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred;
# 159 "simulation.sim"
;((__bs225 *)((__bp=((__bs225 *)((__bp=((__bs231 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc=((__bs225 *)((__bp=((__bs231 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc;
# 161 "simulation.sim"
;((__bs231 *)__lb)->q=((__bs225 *)((__bp=((__bs222 *)__lb->sl)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred;
# 162 "simulation.sim"
__ll35:;if(!((((__bs225 *)((__bp=((__bs231 *)__lb)->q)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzevtime>((__bs231 *)__lb)->t)))goto __ll36;;((__bs231 *)__lb)->q=((__bs225 *)((__bp=((__bs231 *)__lb)->q)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred;goto __ll35;__ll36:
# 164 "simulation.sim"
;((__bs225 *)((__bp=((__bs231 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred=((__bs231 *)__lb)->q;;((__bs225 *)((__bp=((__bs231 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc=((__bs225 *)((__bp=((__bs231 *)__lb)->q)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc;
# 165 "simulation.sim"
;((__bs225 *)((__bp=((__bs231 *)__lb)->q)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc=((__bs231 *)__lb)->p;;((__bs225 *)((__bp=((__bs225 *)((__bp=((__bs231 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred=((__bs231 *)__lb)->p;
# 167 "simulation.sim"
;__rresume(((__bs225 *)((__bp=((__bs222 *)__lb->sl)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc,27,__m_SIMULATION);goto __sw;__l27:;
# 168 "simulation.sim"
__ll34:
# 169 "simulation.sim"
__repp();goto __sw;/* SLUTT PROCEDURE HOLD *//* START PROCEDURE PASSIVATE */__l16:
# 171 "simulation.sim"

# 173 "simulation.sim"
;((__bs232 *)__lb)->p=((__bs225 *)((__bp=((__bs222 *)__lb->sl)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc;
# 174 "simulation.sim"
;((__bs225 *)((__bp=((__bs225 *)((__bp=((__bs232 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred=((__bs225 *)((__bp=((__bs232 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred;
# 175 "simulation.sim"
;((__bs225 *)((__bp=((__bs225 *)((__bp=((__bs232 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc=((__bs225 *)((__bp=((__bs232 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc;
# 176 "simulation.sim"
;((__bs225 *)((__bp=((__bs232 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred=__NULL;;((__bs225 *)((__bp=((__bs232 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc=__NULL;
# 178 "simulation.sim"
;if(!((((__bs225 *)((__bp=((__bs222 *)__lb->sl)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc==((__bs222 *)__lb->sl)->zzsqs)))goto __ll38;
# 179 "simulation.sim"
;__rterror((__txtvp)&__tk5SIMULATION);goto __ll37;__ll38:;__rresume(((__bs225 *)((__bp=((__bs222 *)__lb->sl)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc,28,__m_SIMULATION);goto __sw;__l28:;__ll37:
# 180 "simulation.sim"
__repp();goto __sw;/* SLUTT PROCEDURE PASSIVATE *//* START PROCEDURE WAIT */__l17:
# 182 "simulation.sim"
(((__bp=((__bs233 *)__lb)->S)!=__NULL && (__bp->pp->pref[1]!= &__p216SIMSET))?(__dhp)__rerror(__errqual):__bp);
# 184 "simulation.sim"
;((__bs233 *)__lb)->p=((__bs225 *)((__bp=((__bs222 *)__lb->sl)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc;
# 185 "simulation.sim"
__sl=((__bp=((__bs233 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp);__rcpp(&__p215SIMSET);((__bs215 *)__pb)->S=((__bs233 *)__lb)->S;__rcpb(29,__m_SIMULATION);return;__l29:;;
# 187 "simulation.sim"
;((__bs225 *)((__bp=((__bs225 *)((__bp=((__bs233 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred=((__bs225 *)((__bp=((__bs233 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred;
# 188 "simulation.sim"
;((__bs225 *)((__bp=((__bs225 *)((__bp=((__bs233 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc=((__bs225 *)((__bp=((__bs233 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc;
# 189 "simulation.sim"
;((__bs225 *)((__bp=((__bs233 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred=__NULL;;((__bs225 *)((__bp=((__bs233 *)__lb)->p)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc=__NULL;
# 191 "simulation.sim"
;if(!((((__bs225 *)((__bp=((__bs222 *)__lb->sl)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc==((__bs222 *)__lb->sl)->zzsqs)))goto __ll40;
# 192 "simulation.sim"
;__rterror((__txtvp)&__tk6SIMULATION);goto __ll39;__ll40:;__rresume(((__bs225 *)((__bp=((__bs222 *)__lb->sl)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc,30,__m_SIMULATION);goto __sw;__l30:;__ll39:
# 193 "simulation.sim"
__repp();goto __sw;/* SLUTT PROCEDURE WAIT *//* START PROCEDURE CANCEL */__l18:
# 195 "simulation.sim"
(((__bp=((__bs234 *)__lb)->x)!=__NULL && (__bp->pp->pref[2]!= &__p225SIMULATION))?(__dhp)__rerror(__errqual):__bp);
# 198 "simulation.sim"
;if(!(((((__bs234 *)__lb)->x!=__NULL)&&(((__bs225 *)((__bp=((__bs234 *)__lb)->x)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc!=__NULL))))goto __ll41;
# 200 "simulation.sim"
;((__bs234 *)__lb)->cur=((__bs225 *)((__bp=((__bs222 *)__lb->sl)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc;
# 201 "simulation.sim"
;((__bs225 *)((__bp=((__bs225 *)((__bp=((__bs234 *)__lb)->x)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred=((__bs225 *)((__bp=((__bs234 *)__lb)->x)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred;
# 202 "simulation.sim"
;((__bs225 *)((__bp=((__bs225 *)((__bp=((__bs234 *)__lb)->x)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc=((__bs225 *)((__bp=((__bs234 *)__lb)->x)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc;
# 203 "simulation.sim"
;((__bs225 *)((__bp=((__bs234 *)__lb)->x)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred=__NULL;;((__bs225 *)((__bp=((__bs234 *)__lb)->x)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc=__NULL;
# 205 "simulation.sim"
;if(!((((__bs234 *)__lb)->x==((__bs234 *)__lb)->cur)))goto __ll42;
# 207 "simulation.sim"
;if(!((((__bs225 *)((__bp=((__bs222 *)__lb->sl)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc==((__bs222 *)__lb->sl)->zzsqs)))goto __ll44;
# 208 "simulation.sim"
;__rterror((__txtvp)&__tk7SIMULATION);goto __ll43;__ll44:;__rresume(((__bs225 *)((__bp=((__bs222 *)__lb->sl)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc,31,__m_SIMULATION);goto __sw;__l31:;__ll43:
# 209 "simulation.sim"
__ll42:
# 210 "simulation.sim"
__ll41:
# 211 "simulation.sim"
__repp();goto __sw;/* SLUTT PROCEDURE CANCEL *//* START CLASS zzmain_program */__l19:
# 213 "simulation.sim"
__renddecl(3);goto __sw;__l20:
# 215 "simulation.sim"
__ll45:;if(!(1))goto __ll46;
# 216 "simulation.sim"
;__rdetach(__lb,32,__m_SIMULATION);goto __sw;__l32:;goto __ll45;__ll46:
# 213 "simulation.sim"
__rinner(3);goto __sw;__l21:
# 216 "simulation.sim"
__rendclass(3);goto __sw;/* SLUTT CLASS zzmain_program *//* START PROCEDURE ACCUM */__l22:
# 218 "simulation.sim"

# 221 "simulation.sim"
if(__rgetsa(&((__bs236 *)__lb)->A,0L,33,__m_SIMULATION))goto __sw;__l33:;((__bs236 *)__lb)->__r1= __er;((__bs236 *)__lb)->__v1.i= __ev.i;if(__rgetav(__TREAL,&((__bs236 *)__lb)->A,0L,34,__m_SIMULATION))goto __sw;__l34:;((__bs236 *)__lb)->__v2.f= __ev.f;if(__rgetav(__TREAL,&((__bs236 *)__lb)->C,0L,35,__m_SIMULATION))goto __sw;__l35:;((__bs236 *)__lb)->__v3.f= __ev.f;__sl=__lb->sl;__rcp(&__p224SIMULATION,0L);__rcpb(36,__m_SIMULATION);goto __sw;__l36:;((__bs236 *)__lb)->__v4.f= __ev.f;if(__rgetav(__TREAL,&((__bs236 *)__lb)->B,0L,37,__m_SIMULATION))goto __sw;__l37:;((__bs236 *)__lb)->__v5.f= __ev.f;(__ev.f=((((__bs236 *)__lb)->__v2.f+(((__bs236 *)__lb)->__v3.f*(((__bs236 *)__lb)->__v4.f-((__bs236 *)__lb)->__v5.f)))));if((__nvp= &((__bs236 *)__lb)->A)->conv==__NOCONV) *(double *)(((char *)((__bs236 *)__lb)->__r1)+((__bs236 *)__lb)->__v1.i)=__ev.f;else if(__nvp->conv==__INTREAL) *(long *)(((char *)((__bs236 *)__lb)->__r1)+((__bs236 *)__lb)->__v1.i)=__ev.f;else  *(double *)(((char *)((__bs236 *)__lb)->__r1)+((__bs236 *)__lb)->__v1.i)=__rintrea(__ev.f);if(__rgetsa(&((__bs236 *)__lb)->B,0L,38,__m_SIMULATION))goto __sw;__l38:;((__bs236 *)__lb)->__r1= __er;((__bs236 *)__lb)->__v1.i= __ev.i;__sl=__lb->sl;__rcp(&__p224SIMULATION,0L);__rcpb(39,__m_SIMULATION);goto __sw;__l39:;((__bs236 *)__lb)->__v2.f= __ev.f;(__ev.f=(((__bs236 *)__lb)->__v2.f));if((__nvp= &((__bs236 *)__lb)->B)->conv==__NOCONV) *(double *)(((char *)((__bs236 *)__lb)->__r1)+((__bs236 *)__lb)->__v1.i)=__ev.f;else if(__nvp->conv==__INTREAL) *(long *)(((char *)((__bs236 *)__lb)->__r1)+((__bs236 *)__lb)->__v1.i)=__ev.f;else  *(double *)(((char *)((__bs236 *)__lb)->__r1)+((__bs236 *)__lb)->__v1.i)=__rintrea(__ev.f);if(__rgetsa(&((__bs236 *)__lb)->C,0L,40,__m_SIMULATION))goto __sw;__l40:;((__bs236 *)__lb)->__r1= __er;((__bs236 *)__lb)->__v1.i= __ev.i;if(__rgetav(__TREAL,&((__bs236 *)__lb)->C,0L,41,__m_SIMULATION))goto __sw;__l41:;((__bs236 *)__lb)->__v2.f= __ev.f;(__ev.f=((((__bs236 *)__lb)->__v2.f+((__bs236 *)__lb)->D)));if((__nvp= &((__bs236 *)__lb)->C)->conv==__NOCONV) *(double *)(((char *)((__bs236 *)__lb)->__r1)+((__bs236 *)__lb)->__v1.i)=__ev.f;else if(__nvp->conv==__INTREAL) *(long *)(((char *)((__bs236 *)__lb)->__r1)+((__bs236 *)__lb)->__v1.i)=__ev.f;else  *(double *)(((char *)((__bs236 *)__lb)->__r1)+((__bs236 *)__lb)->__v1.i)=__rintrea(__ev.f);
# 222 "simulation.sim"
__repp();goto __sw;/* SLUTT PROCEDURE ACCUM */__l2:
# 27 "simulation.sim"
__renddecl(1);goto __sw;__l3:
# 224 "simulation.sim"
__sl=__lb;__rcp(&__p225SIMULATION,0L);__rccb(42,__m_SIMULATION);goto __sw;__l42:;((__bs222 *)__lb)->__r1= __er;((__bs222 *)__lb)->zzsqs=((__bs222 *)__lb)->__r1;;(((__bs225 *)((__bp=((__bs222 *)__lb)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzevtime=( -1.0000000000000000e+00));
# 226 "simulation.sim"
__sl=__lb;__rcp(&__p235SIMULATION,0L);__rccb(43,__m_SIMULATION);goto __sw;__l43:;((__bs222 *)__lb)->__r1= __er;((__bs222 *)__lb)->MAIN=((__bs222 *)__lb)->__r1;
# 227 "simulation.sim"
;((__bs225 *)((__bp=((__bs222 *)__lb)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc=((__bs222 *)__lb)->MAIN;;((__bs225 *)((__bp=((__bs222 *)__lb)->zzsqs)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred=((__bs222 *)__lb)->MAIN;
# 228 "simulation.sim"
;((__bs225 *)((__bp=((__bs222 *)__lb)->MAIN)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqssuc=((__bs222 *)__lb)->zzsqs;;((__bs225 *)((__bp=((__bs222 *)__lb)->MAIN)==__NULL?(__dhp)__rerror(__errnone):__bp))->zzsqspred=((__bs222 *)__lb)->zzsqs;
# 27 "simulation.sim"
__rinner(1);goto __sw;__l4:
# 230 "simulation.sim"
__rendclass(1);goto __sw;/* SLUTT CLASS SIMULATION */__ll0:__rbe();__sw:if(__goto.ment!=(void (*)())__m_SIMULATION)return;__s:switch(__goto.ent){case 2: goto __l2;
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
case 29: goto __l29;
case 30: goto __l30;
case 31: goto __l31;
case 32: goto __l32;
case 33: goto __l33;
case 34: goto __l34;
case 35: goto __l35;
case 36: goto __l36;
case 37: goto __l37;
case 38: goto __l38;
case 39: goto __l39;
case 40: goto __l40;
case 41: goto __l41;
case 42: goto __l42;
case 43: goto __l43;
}}
