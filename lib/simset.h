/*Cim_hcode*/
__map __mapSIMSET[2]={"simset.sim",0L,1L,
"",0L,2147483647L};
void __m_SIMSET();
typedef struct /* B */
    {
        __dh h;
    } __bs205;
__bs205 __blokk205SIMSET;
__pty   __pl205SIMSET[1]={0};
__ptyp __p205SIMSET={'B',0,1,sizeof(__bs205),0,0,0,0,0,0,0,0,__pl205SIMSET,__NULL};
typedef struct /* SIMSET */
    {
        __dh h;
    } __bs206;
__pty   __pl206SIMSET[8]={0};
__ptyp __p206SIMSET={'C',0,2,sizeof(__bs206),2,0,0,0,0,0,0,0,__pl206SIMSET,__NULL};
typedef struct /* LINKAGE */
    {
        __dh h;
        __dhp zzsuc;
        __dhp zzpred;
    } __bs207;
short __rl207SIMSET[2];
__pty   __pl207SIMSET[8]={0};
__ptyp __p207SIMSET={'C',0,3,sizeof(__bs207),5,0,0,0,2,0,__rl207SIMSET,0,__pl207SIMSET,__NULL};
typedef struct /* SUC */
    {
        __dh h;
        __dhp er;
    } __bs208;
short __rl208SIMSET[1];
__pty   __pl208SIMSET[8]={0};
__ptyp __p208SIMSET={'P',0,4,sizeof(__bs208),8,0,0,0,1,0,__rl208SIMSET,0,__pl208SIMSET,__NULL};
typedef struct /* PRED */
    {
        __dh h;
        __dhp er;
    } __bs209;
short __rl209SIMSET[1];
__pty   __pl209SIMSET[8]={0};
__ptyp __p209SIMSET={'P',0,4,sizeof(__bs209),9,0,0,0,1,0,__rl209SIMSET,0,__pl209SIMSET,__NULL};
typedef struct /* PREV */
    {
        __dh h;
        __dhp er;
    } __bs210;
short __rl210SIMSET[1];
__pty   __pl210SIMSET[8]={0};
__ptyp __p210SIMSET={'P',0,4,sizeof(__bs210),10,0,0,0,1,0,__rl210SIMSET,0,__pl210SIMSET,__NULL};
typedef struct /* LINK */
    {
        __bs207 s;
    } __bs211;
__pty   __pl211SIMSET[8]={&__p207SIMSET,0};
__ptyp __p211SIMSET={'C',1,3,sizeof(__bs211),11,0,0,0,0,0,0,0,__pl211SIMSET,__NULL};
typedef struct /* OUT */
    {
        __dh h;
    } __bs212;
__pty   __pl212SIMSET[1]={0};
__ptyp __p212SIMSET={'P',0,4,sizeof(__bs212),14,0,0,0,0,0,0,0,__pl212SIMSET,__NULL};
typedef struct /* FOLLOW */
    {
        __dh h;
        __dhp PTR;
    } __bs213;
short __rl213SIMSET[1];
__pty   __pl213SIMSET[1]={0};
__ptyp __p213SIMSET={'P',0,4,sizeof(__bs213),15,0,0,0,1,0,__rl213SIMSET,0,__pl213SIMSET,__NULL};
typedef struct /* PRECEDE */
    {
        __dh h;
        __dhp PTR;
    } __bs214;
short __rl214SIMSET[1];
__pty   __pl214SIMSET[1]={0};
__ptyp __p214SIMSET={'P',0,4,sizeof(__bs214),17,0,0,0,1,0,__rl214SIMSET,0,__pl214SIMSET,__NULL};
typedef struct /* INTO */
    {
        __dh h;
        __dhp S;
    } __bs215;
short __rl215SIMSET[1];
__pty   __pl215SIMSET[1]={0};
__ptyp __p215SIMSET={'P',0,4,sizeof(__bs215),19,0,0,0,1,0,__rl215SIMSET,0,__pl215SIMSET,__NULL};
typedef struct /* HEAD */
    {
        __bs207 s;
    } __bs216;
__pty   __pl216SIMSET[8]={&__p207SIMSET,0};
__ptyp __p216SIMSET={'C',1,3,sizeof(__bs216),21,0,0,0,0,0,0,0,__pl216SIMSET,__NULL};
typedef struct /* FIRST */
    {
        __dh h;
        __dhp er;
    } __bs217;
short __rl217SIMSET[1];
__pty   __pl217SIMSET[8]={0};
__ptyp __p217SIMSET={'P',0,4,sizeof(__bs217),24,0,0,0,1,0,__rl217SIMSET,0,__pl217SIMSET,__NULL};
typedef struct /* LAST */
    {
        __dh h;
        __dhp er;
    } __bs218;
short __rl218SIMSET[1];
__pty   __pl218SIMSET[8]={0};
__ptyp __p218SIMSET={'P',0,4,sizeof(__bs218),25,0,0,0,1,0,__rl218SIMSET,0,__pl218SIMSET,__NULL};
typedef struct /* EMPTY */
    {
        __dh h;
        char ec;
    } __bs219;
__pty   __pl219SIMSET[1]={0};
__ptyp __p219SIMSET={'P',0,4,sizeof(__bs219),26,0,0,0,0,0,0,0,__pl219SIMSET,__NULL};
typedef struct /* CARDINAL */
    {
        __dh h;
        long I;
        __dhp PTR;
        long ev;
    } __bs220;
short __rl220SIMSET[1];
__pty   __pl220SIMSET[1]={0};
__ptyp __p220SIMSET={'P',0,4,sizeof(__bs220),27,0,0,0,1,0,__rl220SIMSET,0,__pl220SIMSET,__NULL};
typedef struct /* CLEAR */
    {
        __dh h;
        __dhp PTR;
        __dhp PTRSUC;
    } __bs221;
short __rl221SIMSET[2];
__pty   __pl221SIMSET[1]={0};
__ptyp __p221SIMSET={'P',0,4,sizeof(__bs221),28,0,0,0,2,0,__rl221SIMSET,0,__pl221SIMSET,__NULL};
