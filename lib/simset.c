/*Cim_ccode*/
#include "./cim.h"
#include "simset.h"
void __m_SIMSET(){goto __s;
# 25 "simset.sim"
__sto= (__dhp)&__blokk205SIMSET;__rb(&__p205SIMSET);/* START CLASS SIMSET */goto __ll0;__l2:/* START CLASS LINKAGE */goto __ll1;__l5:
# 28 "simset.sim"
/* START PROCEDURE SUC */goto __ll2;__l8:
# 31 "simset.sim"

# 32 "simset.sim"
((__bs208 *)__lb)->er=(((__bp=(__rin(((__bs207 *)__lb->sl)->zzsuc,&__p211SIMSET)?((__bs207 *)__lb->sl)->zzsuc:__NULL))!=__NULL && (__bp->pp->pref[1]!= &__p211SIMSET))?(__dhp)__rerror(__errqual):__bp);__er=((__bs208 *)__lb)->er;__rep();goto __sw;__ll2:/* SLUTT PROCEDURE SUC *//* START PROCEDURE PRED */goto __ll3;__l9:
# 34 "simset.sim"

# 35 "simset.sim"
((__bs209 *)__lb)->er=(((__bp=(__rin(((__bs207 *)__lb->sl)->zzpred,&__p211SIMSET)?((__bs207 *)__lb->sl)->zzpred:__NULL))!=__NULL && (__bp->pp->pref[1]!= &__p211SIMSET))?(__dhp)__rerror(__errqual):__bp);__er=((__bs209 *)__lb)->er;__rep();goto __sw;__ll3:/* SLUTT PROCEDURE PRED *//* START PROCEDURE PREV */goto __ll4;__l10:
# 37 "simset.sim"
((__bs210 *)__lb)->er=((__bs207 *)__lb->sl)->zzpred;__er=((__bs210 *)__lb)->er;__rep();goto __sw;__ll4:/* SLUTT PROCEDURE PREV */__renddecl(0);goto __sw;__l6:
# 39 "simset.sim"
__rinner(0);goto __sw;__l7:__rendclass(0);goto __sw;__ll1:/* SLUTT CLASS LINKAGE *//* START CLASS LINK */goto __ll5;__l11:
# 42 "simset.sim"
/* START PROCEDURE OUT */goto __ll6;__l14:
# 45 "simset.sim"

# 46 "simset.sim"
if(!((((__bs207 *)__lb->sl)->zzsuc!=__NULL)))goto __ll7;
# 47 "simset.sim"
((__bs207 *)((__bs207 *)__lb->sl)->zzsuc)->zzpred=((__bs207 *)__lb->sl)->zzpred;
# 48 "simset.sim"
((__bs207 *)((__bs207 *)__lb->sl)->zzpred)->zzsuc=((__bs207 *)__lb->sl)->zzsuc;
# 49 "simset.sim"
((__bs207 *)__lb->sl)->zzsuc=((__bs207 *)__lb->sl)->zzpred=__NULL;
# 50 "simset.sim"
__ll7:__repp();goto __sw;__ll6:/* SLUTT PROCEDURE OUT *//* START PROCEDURE FOLLOW */goto __ll8;__l15:
# 52 "simset.sim"
(((__bp=((__bs213 *)__lb)->PTR)!=__NULL && (__bp->pp->pref[0]!= &__p207SIMSET))?(__dhp)__rerror(__errqual):__bp);
# 53 "simset.sim"
__sl=__lb->sl;__rcpp(&__p212SIMSET);__rcpb(16,__m_SIMSET);goto __sw;__l16:;
# 54 "simset.sim"
if(!(((((__bs213 *)__lb)->PTR!=__NULL)&&(((__bs207 *)((__bs213 *)__lb)->PTR)->zzsuc!=__NULL))))goto __ll9;
# 55 "simset.sim"
((__bs207 *)__lb->sl)->zzpred=((__bs213 *)__lb)->PTR;
# 56 "simset.sim"
((__bs207 *)__lb->sl)->zzsuc=((__bs207 *)((__bs213 *)__lb)->PTR)->zzsuc;
# 57 "simset.sim"
__r[1]=(__dhp)((__bs207 *)__lb->sl)->zzsuc;((__bs207 *)__r[1])->zzpred=((__bs207 *)((__bs213 *)__lb)->PTR)->zzsuc=__lb->sl;
# 58 "simset.sim"
__ll9:__repp();goto __sw;__ll8:/* SLUTT PROCEDURE FOLLOW *//* START PROCEDURE PRECEDE */goto __ll10;__l17:
# 60 "simset.sim"
(((__bp=((__bs214 *)__lb)->PTR)!=__NULL && (__bp->pp->pref[0]!= &__p207SIMSET))?(__dhp)__rerror(__errqual):__bp);
# 61 "simset.sim"
__sl=__lb->sl;__rcpp(&__p212SIMSET);__rcpb(18,__m_SIMSET);goto __sw;__l18:;
# 62 "simset.sim"
if(!(((((__bs214 *)__lb)->PTR!=__NULL)&&(((__bs207 *)((__bs214 *)__lb)->PTR)->zzpred!=__NULL))))goto __ll11;
# 63 "simset.sim"
((__bs207 *)__lb->sl)->zzsuc=((__bs214 *)__lb)->PTR;
# 64 "simset.sim"
((__bs207 *)__lb->sl)->zzpred=((__bs207 *)((__bs214 *)__lb)->PTR)->zzpred;
# 65 "simset.sim"
__r[1]=(__dhp)((__bs207 *)__lb->sl)->zzpred;((__bs207 *)__r[1])->zzsuc=((__bs207 *)((__bs214 *)__lb)->PTR)->zzpred=__lb->sl;
# 66 "simset.sim"
__ll11:__repp();goto __sw;__ll10:/* SLUTT PROCEDURE PRECEDE *//* START PROCEDURE INTO */goto __ll12;__l19:
# 68 "simset.sim"
(((__bp=((__bs215 *)__lb)->S)!=__NULL && (__bp->pp->pref[1]!= &__p216SIMSET))?(__dhp)__rerror(__errqual):__bp);__sl=__lb->sl;__rcpp(&__p214SIMSET);((__bs214 *)__pb)->PTR=((__bs215 *)__lb)->S;__rcpb(20,__m_SIMSET);goto __sw;__l20:;__repp();goto __sw;__ll12:/* SLUTT PROCEDURE INTO */__renddecl(1);goto __sw;__l12:
# 70 "simset.sim"
__rinner(1);goto __sw;__l13:__rendclass(1);goto __sw;__ll5:/* SLUTT CLASS LINK *//* START CLASS HEAD */goto __ll13;__l21:
# 73 "simset.sim"
/* START PROCEDURE FIRST */goto __ll14;__l24:
# 76 "simset.sim"

# 78 "simset.sim"
((__bs217 *)__lb)->er=(((__bp=(__rin(((__bs207 *)__lb->sl)->zzsuc,&__p211SIMSET)?((__bs207 *)__lb->sl)->zzsuc:__NULL))!=__NULL && (__bp->pp->pref[1]!= &__p211SIMSET))?(__dhp)__rerror(__errqual):__bp);__er=((__bs217 *)__lb)->er;__rep();goto __sw;__ll14:/* SLUTT PROCEDURE FIRST *//* START PROCEDURE LAST */goto __ll15;__l25:
# 80 "simset.sim"

# 82 "simset.sim"
((__bs218 *)__lb)->er=(((__bp=(__rin(((__bs207 *)__lb->sl)->zzpred,&__p211SIMSET)?((__bs207 *)__lb->sl)->zzpred:__NULL))!=__NULL && (__bp->pp->pref[1]!= &__p211SIMSET))?(__dhp)__rerror(__errqual):__bp);__er=((__bs218 *)__lb)->er;__rep();goto __sw;__ll15:/* SLUTT PROCEDURE LAST *//* START PROCEDURE EMPTY */goto __ll16;__l26:
# 84 "simset.sim"
(((__bs219 *)__lb)->ec=((((__bs207 *)__lb->sl)->zzsuc==__lb->sl)));__ev.c=((__bs219 *)__lb)->ec;__rep();goto __sw;__ll16:/* SLUTT PROCEDURE EMPTY *//* START PROCEDURE CARDINAL */goto __ll17;__l27:
# 86 "simset.sim"

# 94 "simset.sim"
((__bs220 *)__lb)->PTR=((__bs207 *)__lb->sl)->zzsuc;
# 95 "simset.sim"
__ll18:if(!((((__bs220 *)__lb)->PTR!=__lb->sl)))goto __ll19;
# 96 "simset.sim"
(((__bs220 *)__lb)->I=((((__bs220 *)__lb)->I+1L)));
# 97 "simset.sim"
((__bs220 *)__lb)->PTR=((__bs207 *)((__bs220 *)__lb)->PTR)->zzsuc;
# 98 "simset.sim"
goto __ll18;__ll19:
# 101 "simset.sim"
(((__bs220 *)__lb)->ev=(((__bs220 *)__lb)->I));__ev.i=((__bs220 *)__lb)->ev;__rep();goto __sw;__ll17:/* SLUTT PROCEDURE CARDINAL *//* START PROCEDURE CLEAR */goto __ll20;__l28:
# 104 "simset.sim"

# 107 "simset.sim"
((__bs221 *)__lb)->PTR=((__bs207 *)__lb->sl)->zzsuc;
# 108 "simset.sim"
__ll21:if(!((((__bs221 *)__lb)->PTR!=__lb->sl)))goto __ll22;
# 110 "simset.sim"
((__bs221 *)__lb)->PTRSUC=((__bs207 *)((__bs221 *)__lb)->PTR)->zzsuc;
# 111 "simset.sim"
__r[1]=(__dhp)((__bs221 *)__lb)->PTR;((__bs207 *)__r[1])->zzsuc=((__bs207 *)((__bs221 *)__lb)->PTR)->zzpred=__NULL;
# 112 "simset.sim"
((__bs221 *)__lb)->PTR=((__bs221 *)__lb)->PTRSUC;
# 113 "simset.sim"
goto __ll21;__ll22:
# 115 "simset.sim"
((__bs207 *)__lb->sl)->zzsuc=((__bs207 *)__lb->sl)->zzpred=__lb->sl;__repp();goto __sw;__ll20:/* SLUTT PROCEDURE CLEAR */
# 118 "simset.sim"
__renddecl(1);goto __sw;__l22:((__bs207 *)__lb)->zzsuc=((__bs207 *)__lb)->zzpred=__lb;__rinner(1);goto __sw;__l23:__rendclass(1);goto __sw;__ll13:/* SLUTT CLASS HEAD */__renddecl(0);goto __sw;__l3:
# 120 "simset.sim"
__rinner(0);goto __sw;__l4:__rendclass(0);goto __sw;__ll0:/* SLUTT CLASS SIMSET */__sw:if(__goto.ment!=(void (*)())__m_SIMSET)return;__s:switch(__goto.ent){case 2: goto __l2;case 3: goto __l3;case 4: goto __l4;case 5: goto __l5;case 6: goto __l6;case 7: goto __l7;case 8: goto __l8;case 9: goto __l9;case 10: goto __l10;case 11: goto __l11;case 12: goto __l12;case 13: goto __l13;case 14: goto __l14;case 15: goto __l15;case 16: goto __l16;case 17: goto __l17;case 18: goto __l18;case 19: goto __l19;case 20: goto __l20;case 21: goto __l21;case 22: goto __l22;case 23: goto __l23;case 24: goto __l24;case 25: goto __l25;case 26: goto __l26;case 27: goto __l27;case 28: goto __l28;}}void __init_SIMSET(){__pl205SIMSET[0] = &__p205SIMSET;__pl206SIMSET[0] = &__p206SIMSET;__p206SIMSET.adr.ment=__m_SIMSET;__rl207SIMSET[0]=(short)((char *)&((__bs207 *)__p)->zzsuc-(char *)__p);__rl207SIMSET[1]=(short)((char *)&((__bs207 *)__p)->zzpred-(char *)__p);__pl207SIMSET[0] = &__p207SIMSET;__p207SIMSET.adr.ment=__m_SIMSET;__rl208SIMSET[0]=(short)((char *)&((__bs208 *)__p)->er-(char *)__p);__pl208SIMSET[0] = &__p208SIMSET;__p208SIMSET.adr.ment=__m_SIMSET;__rl209SIMSET[0]=(short)((char *)&((__bs209 *)__p)->er-(char *)__p);__pl209SIMSET[0] = &__p209SIMSET;__p209SIMSET.adr.ment=__m_SIMSET;__rl210SIMSET[0]=(short)((char *)&((__bs210 *)__p)->er-(char *)__p);__pl210SIMSET[0] = &__p210SIMSET;__p210SIMSET.adr.ment=__m_SIMSET;__pl211SIMSET[1] = &__p211SIMSET;__p211SIMSET.adr.ment=__m_SIMSET;__pl212SIMSET[0] = &__p212SIMSET;__p212SIMSET.adr.ment=__m_SIMSET;__rl213SIMSET[0]=(short)((char *)&((__bs213 *)__p)->PTR-(char *)__p);__pl213SIMSET[0] = &__p213SIMSET;__p213SIMSET.adr.ment=__m_SIMSET;__rl214SIMSET[0]=(short)((char *)&((__bs214 *)__p)->PTR-(char *)__p);__pl214SIMSET[0] = &__p214SIMSET;__p214SIMSET.adr.ment=__m_SIMSET;__rl215SIMSET[0]=(short)((char *)&((__bs215 *)__p)->S-(char *)__p);__pl215SIMSET[0] = &__p215SIMSET;__p215SIMSET.adr.ment=__m_SIMSET;__pl216SIMSET[1] = &__p216SIMSET;__p216SIMSET.adr.ment=__m_SIMSET;__rl217SIMSET[0]=(short)((char *)&((__bs217 *)__p)->er-(char *)__p);__pl217SIMSET[0] = &__p217SIMSET;__p217SIMSET.adr.ment=__m_SIMSET;__rl218SIMSET[0]=(short)((char *)&((__bs218 *)__p)->er-(char *)__p);__pl218SIMSET[0] = &__p218SIMSET;__p218SIMSET.adr.ment=__m_SIMSET;__pl219SIMSET[0] = &__p219SIMSET;__p219SIMSET.adr.ment=__m_SIMSET;__rl220SIMSET[0]=(short)((char *)&((__bs220 *)__p)->PTR-(char *)__p);__pl220SIMSET[0] = &__p220SIMSET;__p220SIMSET.adr.ment=__m_SIMSET;__rl221SIMSET[0]=(short)((char *)&((__bs221 *)__p)->PTR-(char *)__p);__rl221SIMSET[1]=(short)((char *)&((__bs221 *)__p)->PTRSUC-(char *)__p);__pl221SIMSET[0] = &__p221SIMSET;__p221SIMSET.adr.ment=__m_SIMSET;}