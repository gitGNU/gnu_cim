/*Cim_ccode*/
#include "./cim.h"
#include "simulation.h"
void __m_SIMULATION(){goto __s;
# 27 "simulation.sim"
__sto= (__dhp)&__blokk205SIMULATION;__rb(&__p205SIMULATION);/* START CLASS SIMULATION */goto __ll0;__l2:/* START PROCEDURE CURRENT */goto __ll1;__l5:
# 33 "simulation.sim"
((__bs223 *)__lb)->er=((__bs225 *)((__bs222 *)__lb->sl)->zzsqs)->zzsqssuc;__er=((__bs223 *)__lb)->er;__rep();goto __sw;__ll1:/* SLUTT PROCEDURE CURRENT *//* START PROCEDURE TIME */goto __ll2;__l6:
# 35 "simulation.sim"
(((__bs224 *)__lb)->ef=(((__bs225 *)((__bs225 *)((__bs222 *)__lb->sl)->zzsqs)->zzsqssuc)->zzevtime));__ev.f=((__bs224 *)__lb)->ef;__rep();goto __sw;__ll2:/* SLUTT PROCEDURE TIME *//* START CLASS PROCESS */goto __ll3;__l7:
# 37 "simulation.sim"
/* START PROCEDURE IDLE */goto __ll4;__l10:
# 46 "simulation.sim"
(((__bs226 *)__lb)->ec=((((__bs225 *)__lb->sl)->zzsqssuc==__NULL)));__ev.c=((__bs226 *)__lb)->ec;__rep();goto __sw;__ll4:/* SLUTT PROCEDURE IDLE *//* START PROCEDURE TERMINATED */goto __ll5;__l11:
# 48 "simulation.sim"
(((__bs227 *)__lb)->ec=(((__bs225 *)__lb->sl)->zzterminated_process));__ev.c=((__bs227 *)__lb)->ec;__rep();goto __sw;__ll5:/* SLUTT PROCEDURE TERMINATED *//* START PROCEDURE EVTIME */goto __ll6;__l12:
# 50 "simulation.sim"

# 51 "simulation.sim"
if(!((((__bs225 *)__lb->sl)->zzsqssuc==__NULL)))goto __ll7;
# 52 "simulation.sim"
__rterror((__txtvp)&__tk1SIMULATION);goto __ll8;__ll7:(((__bs228 *)__lb)->ef=(((__bs225 *)__lb->sl)->zzevtime));__ll8:__ev.f=((__bs228 *)__lb)->ef;__rep();goto __sw;__ll6:/* SLUTT PROCEDURE EVTIME *//* START PROCEDURE NEXTEV */goto __ll9;__l13:
# 54 "simulation.sim"

# 56 "simulation.sim"
((__bs229 *)__lb)->er=(((__bp=(((((__bs225 *)__lb->sl)->zzsqssuc==__NULL)||(((__bs225 *)__lb->sl)->zzsqssuc==((__bs222 *)__lb->sl->sl)->zzsqs))?__NULL:((__bs225 *)__lb->sl)->zzsqssuc))!=__NULL && (__bp->pp->pref[2]!= &__p225SIMULATION))?(__dhp)__rerror(__errqual):__bp);__er=((__bs229 *)__lb)->er;__rep();goto __sw;__ll9:/* SLUTT PROCEDURE NEXTEV */
# 58 "simulation.sim"
__renddecl(2);goto __sw;__l8:((__bs225 *)__lb)->zzsqssuc=((__bs225 *)__lb)->zzsqspred=__NULL;
# 60 "simulation.sim"
__rdetach(__lb,14,__m_SIMULATION);goto __sw;__l14:;
# 61 "simulation.sim"
__rinner(2);goto __sw;__l9:
# 62 "simulation.sim"
(((__bs225 *)__lb)->zzterminated_process=(1));
# 65 "simulation.sim"
((__bs225 *)((__bs225 *)__lb)->zzsqssuc)->zzsqspred=((__bs225 *)__lb)->zzsqspred;
# 66 "simulation.sim"
((__bs225 *)((__bs225 *)__lb)->zzsqspred)->zzsqssuc=((__bs225 *)__lb)->zzsqssuc;
# 67 "simulation.sim"
((__bs225 *)__lb)->zzsqspred=((__bs225 *)__lb)->zzsqssuc=__NULL;
# 69 "simulation.sim"
if(!((((__bs225 *)((__bs222 *)__lb->sl)->zzsqs)->zzsqssuc==((__bs222 *)__lb->sl)->zzsqs)))goto __ll10;
# 70 "simulation.sim"
__rterror((__txtvp)&__tk2SIMULATION);goto __ll11;__ll10:__rresume(((__bs225 *)((__bs222 *)__lb->sl)->zzsqs)->zzsqssuc,15,__m_SIMULATION);goto __sw;__l15:;__ll11:
# 72 "simulation.sim"
__rterror((__txtvp)&__tk3SIMULATION);
# 73 "simulation.sim"
__rendclass(2);goto __sw;__ll3:/* SLUTT CLASS PROCESS *//* START PROCEDURE activat */goto __ll12;__l16:
# 75 "simulation.sim"
(((__bp=((__bs230 *)__lb)->X)!=__NULL && (__bp->pp->pref[2]!= &__p225SIMULATION))?(__dhp)__rerror(__errqual):__bp);(((__bp=((__bs230 *)__lb)->Y)!=__NULL && (__bp->pp->pref[2]!= &__p225SIMULATION))?(__dhp)__rerror(__errqual):__bp);
# 85 "simulation.sim"
if(!(((((__bs230 *)__lb)->X!=__NULL)&&((!((__bs225 *)((__bs230 *)__lb)->X)->zzterminated_process)&&(((__bs230 *)__lb)->REAC||(((__bs225 *)((__bs230 *)__lb)->X)->zzsqssuc==__NULL))))))goto __ll13;
# 87 "simulation.sim"
((__bs230 *)__lb)->cur=((__bs225 *)((__bs222 *)__lb->sl)->zzsqs)->zzsqssuc;(((__bs230 *)__lb)->tm=(((__bs225 *)((__bs230 *)__lb)->cur)->zzevtime));
# 89 "simulation.sim"
if(!(((unsigned char)((__bs230 *)__lb)->CODE==(unsigned char)1)))goto __ll14;
# 91 "simulation.sim"
if(!((((__bs230 *)__lb)->X==((__bs230 *)__lb)->cur)))goto __ll15;goto __l17;__ll15:
# 92 "simulation.sim"
(((__bs230 *)__lb)->T=(((__bs230 *)__lb)->tm));((__bs230 *)__lb)->b=((__bs222 *)__lb->sl)->zzsqs;
# 93 "simulation.sim"
goto __ll16;__ll14:
# 94 "simulation.sim"
if(!(((unsigned char)((__bs230 *)__lb)->CODE==(unsigned char)2)))goto __ll17;
# 96 "simulation.sim"
if(!((((__bs230 *)__lb)->T<=((__bs230 *)__lb)->tm)))goto __ll18;
# 97 "simulation.sim"
if(!((((__bs230 *)__lb)->PRIO&&(((__bs230 *)__lb)->X==((__bs230 *)__lb)->cur))))goto __ll19;goto __l17;__ll19:(((__bs230 *)__lb)->T=(((__bs230 *)__lb)->tm));__ll20:__ll18:
# 98 "simulation.sim"
goto __ll21;__ll17:
# 99 "simulation.sim"
if(!(((unsigned char)((__bs230 *)__lb)->CODE==(unsigned char)3)))goto __ll22;
# 101 "simulation.sim"
(((__bs230 *)__lb)->T=((((__bs230 *)__lb)->T+((__bs230 *)__lb)->tm)));
# 102 "simulation.sim"
if(!((((__bs230 *)__lb)->T<=((__bs230 *)__lb)->tm)))goto __ll23;
# 103 "simulation.sim"
if(!((((__bs230 *)__lb)->PRIO&&(((__bs230 *)__lb)->X==((__bs230 *)__lb)->cur))))goto __ll24;goto __l17;__ll24:(((__bs230 *)__lb)->T=(((__bs230 *)__lb)->tm));__ll25:__ll23:
# 104 "simulation.sim"
goto __ll26;__ll22:
# 107 "simulation.sim"
if(!(((((__bs230 *)__lb)->Y==__NULL)||(((__bs225 *)((__bs230 *)__lb)->Y)->zzsqssuc==__NULL))))goto __ll27;
# 109 "simulation.sim"
if(!((((__bs225 *)((__bs230 *)__lb)->X)->zzsqssuc!=__NULL)))goto __ll28;
# 111 "simulation.sim"
((__bs225 *)((__bs225 *)((__bs230 *)__lb)->X)->zzsqssuc)->zzsqspred=((__bs225 *)((__bs230 *)__lb)->X)->zzsqspred;
# 112 "simulation.sim"
((__bs225 *)((__bs225 *)((__bs230 *)__lb)->X)->zzsqspred)->zzsqssuc=((__bs225 *)((__bs230 *)__lb)->X)->zzsqssuc;
# 113 "simulation.sim"
((__bs225 *)((__bs230 *)__lb)->X)->zzsqspred=__NULL;((__bs225 *)((__bs230 *)__lb)->X)->zzsqssuc=__NULL;
# 114 "simulation.sim"
__ll28:
# 116 "simulation.sim"
if(!((((__bs225 *)((__bs222 *)__lb->sl)->zzsqs)->zzsqssuc==((__bs222 *)__lb->sl)->zzsqs)))goto __ll29;__rterror((__txtvp)&__tk4SIMULATION);__ll29:
# 117 "simulation.sim"
goto __l17;__ll27:
# 120 "simulation.sim"
if(!((((__bs230 *)__lb)->X==((__bs230 *)__lb)->Y)))goto __ll30;goto __l17;__ll30:
# 122 "simulation.sim"
(((__bs230 *)__lb)->T=(((__bs225 *)((__bs230 *)__lb)->Y)->zzevtime));
# 124 "simulation.sim"
if(!(((unsigned char)((__bs230 *)__lb)->CODE==(unsigned char)4)))goto __ll31;((__bs230 *)__lb)->b=((__bs225 *)((__bs230 *)__lb)->Y)->zzsqspred;goto __ll32;__ll31:((__bs230 *)__lb)->b=((__bs230 *)__lb)->Y;__ll32:
# 125 "simulation.sim"
__ll26:__ll21:__ll16:
# 127 "simulation.sim"
if(!((((__bs225 *)((__bs230 *)__lb)->X)->zzsqssuc!=__NULL)))goto __ll33;
# 129 "simulation.sim"
((__bs225 *)((__bs225 *)((__bs230 *)__lb)->X)->zzsqssuc)->zzsqspred=((__bs225 *)((__bs230 *)__lb)->X)->zzsqspred;
# 130 "simulation.sim"
((__bs225 *)((__bs225 *)((__bs230 *)__lb)->X)->zzsqspred)->zzsqssuc=((__bs225 *)((__bs230 *)__lb)->X)->zzsqssuc;
# 131 "simulation.sim"
__ll33:
# 133 "simulation.sim"
if(!((((__bs230 *)__lb)->b==__NULL)))goto __ll34;
# 135 "simulation.sim"
((__bs230 *)__lb)->b=((__bs225 *)((__bs222 *)__lb->sl)->zzsqs)->zzsqspred;
# 136 "simulation.sim"
__ll35:if(!((((__bs225 *)((__bs230 *)__lb)->b)->zzevtime>((__bs230 *)__lb)->T)))goto __ll36;((__bs230 *)__lb)->b=((__bs225 *)((__bs230 *)__lb)->b)->zzsqspred;goto __ll35;__ll36:
# 137 "simulation.sim"
if(!(((__bs230 *)__lb)->PRIO))goto __ll37;
# 138 "simulation.sim"
__ll38:if(!((((__bs225 *)((__bs230 *)__lb)->b)->zzevtime==((__bs230 *)__lb)->T)))goto __ll39;((__bs230 *)__lb)->b=((__bs225 *)((__bs230 *)__lb)->b)->zzsqspred;goto __ll38;__ll39:__ll37:
# 139 "simulation.sim"
__ll34:
# 141 "simulation.sim"
(((__bs225 *)((__bs230 *)__lb)->X)->zzevtime=(((__bs230 *)__lb)->T));
# 142 "simulation.sim"
((__bs225 *)((__bs230 *)__lb)->X)->zzsqspred=((__bs230 *)__lb)->b;((__bs225 *)((__bs230 *)__lb)->X)->zzsqssuc=((__bs225 *)((__bs230 *)__lb)->b)->zzsqssuc;
# 143 "simulation.sim"
((__bs225 *)((__bs230 *)__lb)->b)->zzsqssuc=((__bs230 *)__lb)->X;((__bs225 *)((__bs225 *)((__bs230 *)__lb)->X)->zzsqssuc)->zzsqspred=((__bs230 *)__lb)->X;
# 145 "simulation.sim"
if(!((((__bs225 *)((__bs222 *)__lb->sl)->zzsqs)->zzsqssuc!=((__bs230 *)__lb)->cur)))goto __ll40;__rresume(((__bs225 *)((__bs222 *)__lb->sl)->zzsqs)->zzsqssuc,18,__m_SIMULATION);goto __sw;__l18:;__ll40:
# 146 "simulation.sim"
__ll13:
# 147 "simulation.sim"
/*exit_230*/__l17:
# 148 "simulation.sim"
__repp();goto __sw;__ll12:/* SLUTT PROCEDURE activat *//* START PROCEDURE HOLD */goto __ll41;__l19:
# 150 "simulation.sim"

# 153 "simulation.sim"
((__bs231 *)__lb)->p=((__bs225 *)((__bs222 *)__lb->sl)->zzsqs)->zzsqssuc;
# 154 "simulation.sim"
if(!((((__bs231 *)__lb)->t> 0.0000000000000000e+00)))goto __ll42;(((__bs225 *)((__bs231 *)__lb)->p)->zzevtime=((((__bs225 *)((__bs231 *)__lb)->p)->zzevtime+((__bs231 *)__lb)->t)));__ll42:
# 155 "simulation.sim"
(((__bs231 *)__lb)->t=(((__bs225 *)((__bs231 *)__lb)->p)->zzevtime));
# 156 "simulation.sim"
if(!(((((__bs225 *)((__bs231 *)__lb)->p)->zzsqssuc!=((__bs222 *)__lb->sl)->zzsqs)&&(((__bs225 *)((__bs225 *)((__bs231 *)__lb)->p)->zzsqssuc)->zzevtime<=((__bs231 *)__lb)->t))))goto __ll43;
# 158 "simulation.sim"
((__bs225 *)((__bs225 *)((__bs231 *)__lb)->p)->zzsqssuc)->zzsqspred=((__bs225 *)((__bs231 *)__lb)->p)->zzsqspred;
# 159 "simulation.sim"
((__bs225 *)((__bs225 *)((__bs231 *)__lb)->p)->zzsqspred)->zzsqssuc=((__bs225 *)((__bs231 *)__lb)->p)->zzsqssuc;
# 161 "simulation.sim"
((__bs231 *)__lb)->q=((__bs225 *)((__bs222 *)__lb->sl)->zzsqs)->zzsqspred;
# 162 "simulation.sim"
__ll44:if(!((((__bs225 *)((__bs231 *)__lb)->q)->zzevtime>((__bs231 *)__lb)->t)))goto __ll45;((__bs231 *)__lb)->q=((__bs225 *)((__bs231 *)__lb)->q)->zzsqspred;goto __ll44;__ll45:
# 164 "simulation.sim"
((__bs225 *)((__bs231 *)__lb)->p)->zzsqspred=((__bs231 *)__lb)->q;((__bs225 *)((__bs231 *)__lb)->p)->zzsqssuc=((__bs225 *)((__bs231 *)__lb)->q)->zzsqssuc;
# 165 "simulation.sim"
((__bs225 *)((__bs231 *)__lb)->q)->zzsqssuc=((__bs231 *)__lb)->p;((__bs225 *)((__bs225 *)((__bs231 *)__lb)->p)->zzsqssuc)->zzsqspred=((__bs231 *)__lb)->p;
# 167 "simulation.sim"
__rresume(((__bs225 *)((__bs222 *)__lb->sl)->zzsqs)->zzsqssuc,20,__m_SIMULATION);goto __sw;__l20:;
# 168 "simulation.sim"
__ll43:
# 169 "simulation.sim"
__repp();goto __sw;__ll41:/* SLUTT PROCEDURE HOLD *//* START PROCEDURE PASSIVATE */goto __ll46;__l21:
# 171 "simulation.sim"

# 173 "simulation.sim"
((__bs232 *)__lb)->p=((__bs225 *)((__bs222 *)__lb->sl)->zzsqs)->zzsqssuc;
# 174 "simulation.sim"
((__bs225 *)((__bs225 *)((__bs232 *)__lb)->p)->zzsqssuc)->zzsqspred=((__bs225 *)((__bs232 *)__lb)->p)->zzsqspred;
# 175 "simulation.sim"
((__bs225 *)((__bs225 *)((__bs232 *)__lb)->p)->zzsqspred)->zzsqssuc=((__bs225 *)((__bs232 *)__lb)->p)->zzsqssuc;
# 176 "simulation.sim"
((__bs225 *)((__bs232 *)__lb)->p)->zzsqspred=__NULL;((__bs225 *)((__bs232 *)__lb)->p)->zzsqssuc=__NULL;
# 178 "simulation.sim"
if(!((((__bs225 *)((__bs222 *)__lb->sl)->zzsqs)->zzsqssuc==((__bs222 *)__lb->sl)->zzsqs)))goto __ll47;
# 179 "simulation.sim"
__rterror((__txtvp)&__tk5SIMULATION);goto __ll48;__ll47:__rresume(((__bs225 *)((__bs222 *)__lb->sl)->zzsqs)->zzsqssuc,22,__m_SIMULATION);goto __sw;__l22:;__ll48:
# 180 "simulation.sim"
__repp();goto __sw;__ll46:/* SLUTT PROCEDURE PASSIVATE *//* START PROCEDURE WAIT */goto __ll49;__l23:
# 182 "simulation.sim"
(((__bp=((__bs233 *)__lb)->S)!=__NULL && (__bp->pp->pref[1]!= &__p216SIMSET))?(__dhp)__rerror(__errqual):__bp);
# 184 "simulation.sim"
((__bs233 *)__lb)->p=((__bs225 *)((__bs222 *)__lb->sl)->zzsqs)->zzsqssuc;
# 185 "simulation.sim"
__sl=((__bs233 *)__lb)->p;__rcpp(&__p215SIMSET);((__bs215 *)__pb)->S=((__bs233 *)__lb)->S;__rcpb(24,__m_SIMULATION);return;__l24:;
# 187 "simulation.sim"
((__bs225 *)((__bs225 *)((__bs233 *)__lb)->p)->zzsqssuc)->zzsqspred=((__bs225 *)((__bs233 *)__lb)->p)->zzsqspred;
# 188 "simulation.sim"
((__bs225 *)((__bs225 *)((__bs233 *)__lb)->p)->zzsqspred)->zzsqssuc=((__bs225 *)((__bs233 *)__lb)->p)->zzsqssuc;
# 189 "simulation.sim"
((__bs225 *)((__bs233 *)__lb)->p)->zzsqspred=__NULL;((__bs225 *)((__bs233 *)__lb)->p)->zzsqssuc=__NULL;
# 191 "simulation.sim"
if(!((((__bs225 *)((__bs222 *)__lb->sl)->zzsqs)->zzsqssuc==((__bs222 *)__lb->sl)->zzsqs)))goto __ll50;
# 192 "simulation.sim"
__rterror((__txtvp)&__tk6SIMULATION);goto __ll51;__ll50:__rresume(((__bs225 *)((__bs222 *)__lb->sl)->zzsqs)->zzsqssuc,25,__m_SIMULATION);goto __sw;__l25:;__ll51:
# 193 "simulation.sim"
__repp();goto __sw;__ll49:/* SLUTT PROCEDURE WAIT *//* START PROCEDURE CANCEL */goto __ll52;__l26:
# 195 "simulation.sim"
(((__bp=((__bs234 *)__lb)->x)!=__NULL && (__bp->pp->pref[2]!= &__p225SIMULATION))?(__dhp)__rerror(__errqual):__bp);
# 198 "simulation.sim"
if(!(((((__bs234 *)__lb)->x!=__NULL)&&(((__bs225 *)((__bs234 *)__lb)->x)->zzsqssuc!=__NULL))))goto __ll53;
# 200 "simulation.sim"
((__bs234 *)__lb)->cur=((__bs225 *)((__bs222 *)__lb->sl)->zzsqs)->zzsqssuc;
# 201 "simulation.sim"
((__bs225 *)((__bs225 *)((__bs234 *)__lb)->x)->zzsqssuc)->zzsqspred=((__bs225 *)((__bs234 *)__lb)->x)->zzsqspred;
# 202 "simulation.sim"
((__bs225 *)((__bs225 *)((__bs234 *)__lb)->x)->zzsqspred)->zzsqssuc=((__bs225 *)((__bs234 *)__lb)->x)->zzsqssuc;
# 203 "simulation.sim"
((__bs225 *)((__bs234 *)__lb)->x)->zzsqspred=__NULL;((__bs225 *)((__bs234 *)__lb)->x)->zzsqssuc=__NULL;
# 205 "simulation.sim"
if(!((((__bs234 *)__lb)->x==((__bs234 *)__lb)->cur)))goto __ll54;
# 207 "simulation.sim"
if(!((((__bs225 *)((__bs222 *)__lb->sl)->zzsqs)->zzsqssuc==((__bs222 *)__lb->sl)->zzsqs)))goto __ll55;
# 208 "simulation.sim"
__rterror((__txtvp)&__tk7SIMULATION);goto __ll56;__ll55:__rresume(((__bs225 *)((__bs222 *)__lb->sl)->zzsqs)->zzsqssuc,27,__m_SIMULATION);goto __sw;__l27:;__ll56:
# 209 "simulation.sim"
__ll54:
# 210 "simulation.sim"
__ll53:
# 211 "simulation.sim"
__repp();goto __sw;__ll52:/* SLUTT PROCEDURE CANCEL *//* START CLASS zzmain_program */goto __ll57;__l28:
# 213 "simulation.sim"

# 215 "simulation.sim"
__renddecl(3);goto __sw;__l29:__ll58:if(!(1))goto __ll59;
# 216 "simulation.sim"
__rdetach(__lb,31,__m_SIMULATION);goto __sw;__l31:;goto __ll58;__ll59:__rinner(3);goto __sw;__l30:__rendclass(3);goto __sw;__ll57:/* SLUTT CLASS zzmain_program *//* START PROCEDURE ACCUM */goto __ll60;__l32:
# 218 "simulation.sim"

# 221 "simulation.sim"
if(__rgetsa(&((__bs236 *)__lb)->A,0L,33,__m_SIMULATION))goto __sw;__l33:__r[1]=__er;__v[1].i=__ev.i;if(__rgetav(__TREAL,&((__bs236 *)__lb)->A,65792L,34,__m_SIMULATION))goto __sw;__l34:__v[2].f=__ev.f;if(__rgetav(__TREAL,&((__bs236 *)__lb)->C,131328L,35,__m_SIMULATION))goto __sw;__l35:__v[3].f=__ev.f;__sl=__lb->sl;__rcp(&__p224SIMULATION,196864L);__rcpb(36,__m_SIMULATION);goto __sw;__l36:__v[4].f=__ev.f;if(__rgetav(__TREAL,&((__bs236 *)__lb)->B,262400L,37,__m_SIMULATION))goto __sw;__l37:__v[5].f=__ev.f;(__ev.f=((__v[2].f+(__v[3].f*(__v[4].f-__v[5].f)))));if((__nvp= &((__bs236 *)__lb)->A)->conv==__NOCONV) *(double *)(((char *)__r[1])+__v[1].i)=__ev.f;else if(__nvp->conv==__INTREAL) *(long *)(((char *)__r[1])+__v[1].i)=__ev.f;else  *(double *)(((char *)__r[1])+__v[1].i)=__rintrea(__ev.f);if(__rgetsa(&((__bs236 *)__lb)->B,0L,38,__m_SIMULATION))goto __sw;__l38:__r[1]=__er;__v[1].i=__ev.i;__sl=__lb->sl;__rcp(&__p224SIMULATION,65792L);__rcpb(39,__m_SIMULATION);goto __sw;__l39:__v[2].f=__ev.f;(__ev.f=(__v[2].f));if((__nvp= &((__bs236 *)__lb)->B)->conv==__NOCONV) *(double *)(((char *)__r[1])+__v[1].i)=__ev.f;else if(__nvp->conv==__INTREAL) *(long *)(((char *)__r[1])+__v[1].i)=__ev.f;else  *(double *)(((char *)__r[1])+__v[1].i)=__rintrea(__ev.f);if(__rgetsa(&((__bs236 *)__lb)->C,0L,40,__m_SIMULATION))goto __sw;__l40:__r[1]=__er;__v[1].i=__ev.i;if(__rgetav(__TREAL,&((__bs236 *)__lb)->C,65792L,41,__m_SIMULATION))goto __sw;__l41:__v[2].f=__ev.f;(__ev.f=((__v[2].f+((__bs236 *)__lb)->D)));if((__nvp= &((__bs236 *)__lb)->C)->conv==__NOCONV) *(double *)(((char *)__r[1])+__v[1].i)=__ev.f;else if(__nvp->conv==__INTREAL) *(long *)(((char *)__r[1])+__v[1].i)=__ev.f;else  *(double *)(((char *)__r[1])+__v[1].i)=__rintrea(__ev.f);
# 222 "simulation.sim"
__repp();goto __sw;__ll60:/* SLUTT PROCEDURE ACCUM */
# 224 "simulation.sim"
__renddecl(1);goto __sw;__l3:__sl=__lb;__rcp(&__p225SIMULATION,0L);__rccb(42,__m_SIMULATION);goto __sw;__l42:__r[1]=__er;((__bs222 *)__lb)->zzsqs=__r[1];(((__bs225 *)((__bs222 *)__lb)->zzsqs)->zzevtime=( -1.0000000000000000e+00));
# 226 "simulation.sim"
__sl=__lb;__rcp(&__p235SIMULATION,0L);__rccb(43,__m_SIMULATION);goto __sw;__l43:__r[1]=__er;((__bs222 *)__lb)->MAIN=__r[1];
# 227 "simulation.sim"
((__bs225 *)((__bs222 *)__lb)->zzsqs)->zzsqssuc=((__bs222 *)__lb)->MAIN;((__bs225 *)((__bs222 *)__lb)->zzsqs)->zzsqspred=((__bs222 *)__lb)->MAIN;
# 228 "simulation.sim"
((__bs225 *)((__bs222 *)__lb)->MAIN)->zzsqssuc=((__bs222 *)__lb)->zzsqs;((__bs225 *)((__bs222 *)__lb)->MAIN)->zzsqspred=((__bs222 *)__lb)->zzsqs;
# 230 "simulation.sim"
__rinner(1);goto __sw;__l4:__rendclass(1);goto __sw;__ll0:/* SLUTT CLASS SIMULATION */__sw:if(__goto.ment!=(void (*)())__m_SIMULATION)return;__s:switch(__goto.ent){case 2: goto __l2;case 3: goto __l3;case 4: goto __l4;case 5: goto __l5;case 6: goto __l6;case 7: goto __l7;case 8: goto __l8;case 9: goto __l9;case 10: goto __l10;case 11: goto __l11;case 12: goto __l12;case 13: goto __l13;case 14: goto __l14;case 15: goto __l15;case 16: goto __l16;case 17: goto __l17;case 18: goto __l18;case 19: goto __l19;case 20: goto __l20;case 21: goto __l21;case 22: goto __l22;case 23: goto __l23;case 24: goto __l24;case 25: goto __l25;case 26: goto __l26;case 27: goto __l27;case 28: goto __l28;case 29: goto __l29;case 30: goto __l30;case 31: goto __l31;case 32: goto __l32;case 33: goto __l33;case 34: goto __l34;case 35: goto __l35;case 36: goto __l36;case 37: goto __l37;case 38: goto __l38;case 39: goto __l39;case 40: goto __l40;case 41: goto __l41;case 42: goto __l42;case 43: goto __l43;}}void __init_SIMULATION(){__pl205SIMULATION[0] = &__p205SIMULATION;__rl222SIMULATION[0]=(short)((char *)&((__bs222 *)__p)->zzsqs-(char *)__p);__rl222SIMULATION[1]=(short)((char *)&((__bs222 *)__p)->MAIN-(char *)__p);__pl222SIMULATION[1] = &__p222SIMULATION;__p222SIMULATION.adr.ment=__m_SIMULATION;__rl223SIMULATION[0]=(short)((char *)&((__bs223 *)__p)->er-(char *)__p);__pl223SIMULATION[0] = &__p223SIMULATION;__p223SIMULATION.adr.ment=__m_SIMULATION;__pl224SIMULATION[0] = &__p224SIMULATION;__p224SIMULATION.adr.ment=__m_SIMULATION;__rl225SIMULATION[0]=(short)((char *)&((__bs225 *)__p)->zzsqssuc-(char *)__p);__rl225SIMULATION[1]=(short)((char *)&((__bs225 *)__p)->zzsqspred-(char *)__p);__pl225SIMULATION[2] = &__p225SIMULATION;__p225SIMULATION.adr.ment=__m_SIMULATION;__pl226SIMULATION[0] = &__p226SIMULATION;__p226SIMULATION.adr.ment=__m_SIMULATION;__pl227SIMULATION[0] = &__p227SIMULATION;__p227SIMULATION.adr.ment=__m_SIMULATION;__pl228SIMULATION[0] = &__p228SIMULATION;__p228SIMULATION.adr.ment=__m_SIMULATION;__rl229SIMULATION[0]=(short)((char *)&((__bs229 *)__p)->er-(char *)__p);__pl229SIMULATION[0] = &__p229SIMULATION;__p229SIMULATION.adr.ment=__m_SIMULATION;__rl230SIMULATION[0]=(short)((char *)&((__bs230 *)__p)->X-(char *)__p);__rl230SIMULATION[1]=(short)((char *)&((__bs230 *)__p)->Y-(char *)__p);__rl230SIMULATION[2]=(short)((char *)&((__bs230 *)__p)->b-(char *)__p);__rl230SIMULATION[3]=(short)((char *)&((__bs230 *)__p)->cur-(char *)__p);__pl230SIMULATION[0] = &__p230SIMULATION;__p230SIMULATION.adr.ment=__m_SIMULATION;__rl231SIMULATION[0]=(short)((char *)&((__bs231 *)__p)->p-(char *)__p);__rl231SIMULATION[1]=(short)((char *)&((__bs231 *)__p)->q-(char *)__p);__pl231SIMULATION[0] = &__p231SIMULATION;__p231SIMULATION.adr.ment=__m_SIMULATION;__rl232SIMULATION[0]=(short)((char *)&((__bs232 *)__p)->p-(char *)__p);__pl232SIMULATION[0] = &__p232SIMULATION;__p232SIMULATION.adr.ment=__m_SIMULATION;__rl233SIMULATION[0]=(short)((char *)&((__bs233 *)__p)->S-(char *)__p);__rl233SIMULATION[1]=(short)((char *)&((__bs233 *)__p)->p-(char *)__p);__pl233SIMULATION[0] = &__p233SIMULATION;__p233SIMULATION.adr.ment=__m_SIMULATION;__rl234SIMULATION[0]=(short)((char *)&((__bs234 *)__p)->x-(char *)__p);__rl234SIMULATION[1]=(short)((char *)&((__bs234 *)__p)->cur-(char *)__p);__pl234SIMULATION[0] = &__p234SIMULATION;__p234SIMULATION.adr.ment=__m_SIMULATION;__pl235SIMULATION[3] = &__p235SIMULATION;__p235SIMULATION.adr.ment=__m_SIMULATION;__rl236SIMULATION[0]=(short)((char *)&((__bs236 *)__p)->A.h.bp-(char *)__p);__rl236SIMULATION[1]=(short)((char *)&((__bs236 *)__p)->A.h.sl-(char *)__p);__rl236SIMULATION[2]=(short)((char *)&((__bs236 *)__p)->B.h.bp-(char *)__p);__rl236SIMULATION[3]=(short)((char *)&((__bs236 *)__p)->B.h.sl-(char *)__p);__rl236SIMULATION[4]=(short)((char *)&((__bs236 *)__p)->C.h.bp-(char *)__p);__rl236SIMULATION[5]=(short)((char *)&((__bs236 *)__p)->C.h.sl-(char *)__p);__pl236SIMULATION[0] = &__p236SIMULATION;__p236SIMULATION.adr.ment=__m_SIMULATION;}