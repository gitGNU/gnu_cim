/*Cim_hcode*/
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
__map __mapSIMULATION[2]={"simulation.sim",0L,1L,
"",0L,2147483647L};
void __m_SIMULATION();
typedef struct /* B */
    {
        __dh h;
    } __bs205;
__bs205 __blokk205SIMULATION;
__pty   __pl205SIMULATION[1]={0};
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
        __dhp zzsuc;
        __dhp zzpred;
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
        __dhp PTR;
    } __bs213;
extern __ptyp __p213SIMSET;
typedef struct /* PRECEDE */
    {
        __dh h;
        __dhp PTR;
    } __bs214;
extern __ptyp __p214SIMSET;
typedef struct /* INTO */
    {
        __dh h;
        __dhp S;
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
typedef struct /* SIMULATION */
    {
        __bs206 s;
        __dhp zzsqs;
        __dhp MAIN;
    } __bs222;
short __rl222SIMULATION[2];
__pty   __pl222SIMULATION[8]={&__p206SIMSET,0};
__ptyp __p222SIMULATION={'C',1,2,sizeof(__bs222),2,0,0,0,2,0,__rl222SIMULATION,0,__pl222SIMULATION,__NULL};
typedef struct /* CURRENT */
    {
        __dh h;
        __dhp er;
    } __bs223;
short __rl223SIMULATION[1];
__pty   __pl223SIMULATION[8]={0};
__ptyp __p223SIMULATION={'P',0,3,sizeof(__bs223),5,0,0,0,1,0,__rl223SIMULATION,0,__pl223SIMULATION,__NULL};
typedef struct /* TIME */
    {
        __dh h;
        double ef;
    } __bs224;
__pty   __pl224SIMULATION[1]={0};
__ptyp __p224SIMULATION={'P',0,3,sizeof(__bs224),6,0,0,0,0,0,0,0,__pl224SIMULATION,__NULL};
typedef struct /* PROCESS */
    {
        __bs211 s;
        __dhp zzsqssuc;
        __dhp zzsqspred;
        double zzevtime;
        char zzterminated_process;
    } __bs225;
short __rl225SIMULATION[2];
__pty   __pl225SIMULATION[8]={&__p207SIMSET,&__p211SIMSET,0};
__ptyp __p225SIMULATION={'C',2,3,sizeof(__bs225),7,0,0,0,2,0,__rl225SIMULATION,0,__pl225SIMULATION,__NULL};
typedef struct /* IDLE */
    {
        __dh h;
        char ec;
    } __bs226;
__pty   __pl226SIMULATION[1]={0};
__ptyp __p226SIMULATION={'P',0,4,sizeof(__bs226),10,0,0,0,0,0,0,0,__pl226SIMULATION,__NULL};
typedef struct /* TERMINATED */
    {
        __dh h;
        char ec;
    } __bs227;
__pty   __pl227SIMULATION[1]={0};
__ptyp __p227SIMULATION={'P',0,4,sizeof(__bs227),11,0,0,0,0,0,0,0,__pl227SIMULATION,__NULL};
typedef struct /* EVTIME */
    {
        __dh h;
        double ef;
    } __bs228;
__pty   __pl228SIMULATION[1]={0};
__ptyp __p228SIMULATION={'P',0,4,sizeof(__bs228),12,0,0,0,0,0,0,0,__pl228SIMULATION,__NULL};
typedef struct /* NEXTEV */
    {
        __dh h;
        __dhp er;
    } __bs229;
short __rl229SIMULATION[1];
__pty   __pl229SIMULATION[8]={0};
__ptyp __p229SIMULATION={'P',0,4,sizeof(__bs229),13,0,0,0,1,0,__rl229SIMULATION,0,__pl229SIMULATION,__NULL};
typedef struct /* activat */
    {
        __dh h;
        char REAC;
        __dhp X;
        char CODE;
        double T;
        __dhp Y;
        char PRIO;
        __dhp b;
        __dhp cur;
        double tm;
    } __bs230;
short __rl230SIMULATION[4];
__pty   __pl230SIMULATION[1]={0};
__ptyp __p230SIMULATION={'P',0,3,sizeof(__bs230),16,0,0,0,4,0,__rl230SIMULATION,0,__pl230SIMULATION,__NULL};
typedef struct /* HOLD */
    {
        __dh h;
        double t;
        __dhp p;
        __dhp q;
    } __bs231;
short __rl231SIMULATION[2];
__pty   __pl231SIMULATION[1]={0};
__ptyp __p231SIMULATION={'P',0,3,sizeof(__bs231),19,0,0,0,2,0,__rl231SIMULATION,0,__pl231SIMULATION,__NULL};
typedef struct /* PASSIVATE */
    {
        __dh h;
        __dhp p;
    } __bs232;
short __rl232SIMULATION[1];
__pty   __pl232SIMULATION[1]={0};
__ptyp __p232SIMULATION={'P',0,3,sizeof(__bs232),21,0,0,0,1,0,__rl232SIMULATION,0,__pl232SIMULATION,__NULL};
typedef struct /* WAIT */
    {
        __dh h;
        __dhp S;
        __dhp p;
    } __bs233;
short __rl233SIMULATION[2];
__pty   __pl233SIMULATION[1]={0};
__ptyp __p233SIMULATION={'P',0,3,sizeof(__bs233),23,0,0,0,2,0,__rl233SIMULATION,0,__pl233SIMULATION,__NULL};
typedef struct /* CANCEL */
    {
        __dh h;
        __dhp x;
        __dhp cur;
    } __bs234;
short __rl234SIMULATION[2];
__pty   __pl234SIMULATION[1]={0};
__ptyp __p234SIMULATION={'P',0,3,sizeof(__bs234),26,0,0,0,2,0,__rl234SIMULATION,0,__pl234SIMULATION,__NULL};
typedef struct /* zzmain_program */
    {
        __bs225 s;
    } __bs235;
__pty   __pl235SIMULATION[8]={&__p207SIMSET,&__p211SIMSET,&__p225SIMULATION,0};
__ptyp __p235SIMULATION={'C',3,3,sizeof(__bs235),28,0,0,0,0,0,0,0,__pl235SIMULATION,__NULL};
typedef struct /* ACCUM */
    {
        __dh h;
        __aritnamepar A;
        __aritnamepar B;
        __aritnamepar C;
        double D;
    } __bs236;
short __rl236SIMULATION[6];
__pty   __pl236SIMULATION[1]={0};
__ptyp __p236SIMULATION={'P',0,3,sizeof(__bs236),32,0,0,0,6,0,__rl236SIMULATION,0,__pl236SIMULATION,__NULL};
