%{
/* $Id: cparser.y,v 1.9 1994/10/15 13:30:36 cim Exp $ */

/* Copyright (C) 1994 Sverre Hvammen Johansen,
 * Department of Informatics, University of Oslo.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

/* Beskrivelse til YACC som genererer en parser. */

/* Konstanter til bruk */
#include "const.h"
#include "mell.h"
#include "dekl.h"
#include "cimcomp.h"
#include "lex.h"
#include "navn.h"

#define EXTDECLARATION 0
#define DECLARATION 1
#define STATEMENT 2
#define EMPTYSTATEMENT 3


char *Ckind;
char *varargsid;
char *activateid;
char *simsetident,*simulationident,*fileident,*imagefileident,
     *outfileident,*infileident,*directfileident,*printfileident,
     *bytefileident,*inbytefileident,*outbytefileident,*directbytefileident;

extern char sensitive;
char ysensitive;

/* MAKROER FOR TESTUTSKRIFTER */
#ifdef DEBUG
#define mtest(x) if(option_msymbols)printf("%d  %-20s\n",x,texttoken(x));
#define mtestid(x) if(option_msymbols)printf("id:%d  %s\n",x,x);
#define mtestval(x) if(option_msymbols)printf("lval : %ld val : %d\n",x,x);
#else
#define mtest(x) 
#define mtestid(x)
#define mtestval(x)
#endif

/* Makro for det som skal ut til andre pass */
#define outmell(x) {mtest(x) (void)putc((char)x,mcode);}
#if SIZEOF_LONG == 8
#define outmellival(x) {mtestval(x) (void)putc((char)(x),mcode);(void)putc((char)((x)>>8),mcode);\
(void)putc((char)((x)>>16),mcode);(void)putc((char)((x)>>24),mcode);\
(void)putc((char)((x)>>32),mcode);(void)putc((char)((x)>>40),mcode);\
(void)putc((char)((x)>>48),mcode);(void)putc((char)((x)>>56),mcode);}
#else
#define outmellival(x) {mtestval(x) (void)putc((char)(x),mcode);(void)putc((char)((x)>>8),mcode);\
(void)putc((char)((x)>>16),mcode);(void)putc((char)((x)>>24),mcode);}
#endif
#define outmellrval(x) {outmellival(x.i1);outmellival(x.i2);}
#define outmelltval(x) outmellival((long)x)
#define outmellid(x) outmellival((long)x)
/* DEBUGING KAN KANSKJE \NSKES */
/* DA MAA YYDEBUG SETTES LIK 1 */
#ifdef DEBUB
#define YYDEBUG
#endif
#if YACC_YYOVERFLOW
#define yyoverflow
#endif

/* Mulighet for utskrift av symboler fra lex */
#define yylex ylex


/* Kontakt med lex */

/* Makroer for registrering av blokker */
#define OBSBLOCK() remblock[++ind]=TRUE
#define MBEENEWBLOCK() if(remblock[ind]==TRUE){outmell(MBLOCK);\
remblock[ind]=FALSE;newblck(KBLOKK);}
#define MBEEENDBLOCK() if(remblock[ind--]==FALSE)\
{outmell(MENDBLOCK);endblock();}
#define STOPOBSBLOCK() if(remblock[ind]==TRUE)remblock[ind]=MAYBEE
char remblock[REMBLOCKSIZE];
long ind;

/* Spesifikasjonene til YACC */
%}
%union {
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
       }

%token
        HACTIVATE HAFTER /*HAND*/ HARRAY HAT
        HBEFORE HBEGIN HBOOLEAN
        HCHARACTER HCLASS /*HCOMMENT*/ HCONC
        HDELAY HDO
        HELSE HEND HEQ /*HEQV*/ HEXTERNAL
        HFOR 
        HGE HGO HGOTO HGT
        HHIDDEN
        HIF /*HIMP*/ HIN HINNER HINSPECT HINTEGER HIS
        HLABEL HLE HLONG HLT
        HNAME HNE HNEW HNONE /*HNOT*/ HNOTEXT
        /*HOR*/ HOTHERWISE
        HPRIOR HPROCEDURE HPROTECTED
        HQUA
        HREACTIVATE HREAL HREF
        HSHORT HSTEP HSWITCH
        HTEXT HTHEN HTHIS HTO 
        HUNTIL
        HVALUE HVAR HVIRTUAL
        HWHEN HWHILE
        
        HASSIGNVALUE HASSIGNREF
        /*HDOT*/ HPAREXPSEPARATOR HLABELSEPARATOR HSTATEMENTSEPARATOR
        HBEGPAR HENDPAR
        HEQR HNER
        HADD HSUB HMUL HDIV HINTDIV HEXP
  	HDOTDOTDOT

%token <ident> HIDENTIFIER
%token <ival> HBOOLEANKONST HINTEGERKONST HCHARACTERKONST
%token <rasiival> HREALKONST
%token <tval> HTEXTKONST

%type <tval> EXT_IDENT
%type <stat_decl> DECLSTATEMENT MODULSTATEMENT MBEE_DECLSTMS MBEE_DECLSTMSU
%type <stat_decl> MODULS
%type <ident> EXPRESSION_SIMP MBEE_ARG_R_PT 

%right <token> HASSIGN
%left   HORELSE
%left   HANDTHEN
%left   HEQV 
%left   HIMP
%left   HOR
%left   HAND

%left   HNOT

%left <token> HVALRELOPERATOR HREFRELOPERATOR HOBJRELOPERATOR

%left	HCONC

%left <token> HTERMOPERATOR
%left <token> UNEAR
%left <token> HFACTOROPERATOR
%left         HPRIMARYOPERATOR 

%left   HQUA

%left   HDOT

%start  MAIN_MODULE
%%
/* GRAMATIKK FOR PROGRAM MODULES */
MAIN_MODULE     :       {	  categ=CLOCAL; outmell(MBLOCK);
                                  newblck(KBLOKK);separat_comp=FALSE;}
			MODULS	{ endblock();   outmell(MENDBLOCK);}
  		|	error HSTATEMENTSEPARATOR MBEE_DECLSTMS
		;
EXT_DECLARATION	:	HEXTERNAL
			MBEE_TYPE
			HPROCEDURE
				{ MBEENEWBLOCK();        
				  kind=KPROC;}
			EXT_LIST
		|
			HEXTERNAL
			HIDENTIFIER
			HPROCEDURE
				{ MBEENEWBLOCK();
				  type=TNOTY;
				  kind=KPROC;
				  if($2==Ckind)categ=CCPROC;else
				  yerror(1);
                                  ysensitive=sensitive;
                                  sensitive=ON;}
			HIDENTIFIER { $<ident>$=$5;
                                  sensitive=ysensitive;}
			EXTERNAL_KIND_ITEM
			        { categ=CLOCAL;}
		|	HEXTERNAL
			HCLASS
				{ MBEENEWBLOCK();        
				  kind=KCLASS;}
			EXT_LIST
			
		;
EXTERNAL_KIND_ITEM:	EXT_IDENT
			HOBJRELOPERATOR
				{ if($2!=HIS)yerror(2);}
		        MBEE_TYPE HPROCEDURE
			HIDENTIFIER
				{ kind=KPROC;
				  categ=CCPROC;
                                          newdekl($6);
                                          newblck(kind);}
                        HEADING EMPTY_BLOCK
				{ categ=CLOCAL;
				  endblock($1==NULL?$<ident>0:tag($1,(long)strlen($1)));}
/*		|
			EXT_IDENT
				{ if($1!=NULL)yerror(3);
				  newdekl($0);}
			MBEE_REST_EXT_LIST
				{ endblock();}
		;
MBEE_REST_EXT_LIST:	/* EMPTY 
		|	HPAREXPSEPARATOR EXT_KIND_LIST
		;
EXT_KIND_LIST	:	EXT_KIND_ITEM
		|	EXT_KIND_LIST HPAREXPSEPARATOR EXT_KIND_ITEM
		;
EXT_KIND_ITEM	:	HIDENTIFIER
			EXT_IDENT
				{ if($2!=NULL)yerror(3);
				  newdekl($1);}*/
		;
EMPTY_BLOCK	:	/*EMPT*/
		|	HBEGIN HEND
		;
EXT_LIST	:	EXT_ITEM
		|	EXT_LIST HPAREXPSEPARATOR EXT_ITEM
		;
EXT_ITEM	:	HIDENTIFIER
			EXT_IDENT
				{ lesinn_external_spec($1,$2);}
		;
EXT_IDENT	:	/* EMPTY */ { $$=NULL;}
		|	HVALRELOPERATOR { if($1!=HEQ)yerror(9);
                                          external=TRUE;}
			HTEXTKONST { $$=$3;external=FALSE;}
		;
/* GRAMATIKK FOR TYPER */
NO_TYPE         :       /*EMPT*/        { type=TNOTY;}
                ;
MBEE_TYPE       :       NO_TYPE
                |       TYPE
                ;
TYPE            :       HREF HBEGPAR
                        HIDENTIFIER
                                { prefquantident=$3; 
                                          type=TREF;}
                        HENDPAR 
                |       HTEXT           { type=TTEXT;}
                |       HBOOLEAN        { type=TBOOL;}
                |       HCHARACTER      { type=TCHAR;}
                |       HSHORT HINTEGER { type=TSHORT;}
                |       HINTEGER        { type=TINTG;}
                |       HREAL           { type=TREAL;}
                |       HLONG HREAL     { type=TLONG;}
                ;
                          
/* GRAMATIKK FOR DEL AV SETNINGER */
MBEE_ELSE_PART  :       /*EMPT*/
/*		|	HELSE  
			HIF
                        EXPRESSION
                        HTHEN   { 		  outmell(MELSE);
						  outmell(MIF);
                                  OBSBLOCK();}
                        BLOCK   { MBEEENDBLOCK();}
                        MBEE_ELSE_PART          { outmell(MENDIF);}*/
                |       HELSE   { OBSBLOCK();     outmell(MELSE);}
                        BLOCK   { MBEEENDBLOCK();}
                ;
FOR_LIST        :       FOR_LIST_ELEMENT        { outmell(MENDSEP);
                                                  outmell(MLISTSEP);}
                |       FOR_LIST_ELEMENT
                        HPAREXPSEPARATOR
                        FOR_LIST                { outmell(MLISTSEP);}
                ;
FOR_LIST_ELEMENT:       EXPRESSION
                        MBEE_F_L_EL_R_PT        
                ;
MBEE_F_L_EL_R_PT:       /*EMPT*/
                |       HWHILE
                        EXPRESSION              { outmell(MFORWHILE);}
                |       HSTEP
                        EXPRESSION
                        HUNTIL
                        EXPRESSION              { outmell(MUNTIL);
                                                  outmell(MSTEP);}
                ;
GOTO            :       HGO
                        HTO
                |       HGOTO
                ;
CONN_STATE_R_PT :       WHEN_CLAUSE_LIST
                |       HDO   {   newblck(KCON);   outmell(MDO);
                              OBSBLOCK(); }
                        BLOCK {   endblock();  
                                  MBEEENDBLOCK();  outmell(MENDDO);}
                ;
WHEN_CLAUSE_LIST:       HWHEN
                        HIDENTIFIER
                        HDO    {   newblck(KCON);  outmell(MWHEN);
                                   OBSBLOCK();     outmellid($2);}
                        BLOCK  {   endblock();   
                              MBEEENDBLOCK(); outmell(MENDWHEN);}
                |       WHEN_CLAUSE_LIST
                        HWHEN
                        HIDENTIFIER
                        HDO    { newblck(KCON);	   outmell(MWHEN);
                                 OBSBLOCK();       outmellid($3);}
                        BLOCK  { endblock(); 
                              MBEEENDBLOCK();    outmell(MENDWHEN);}
                ;                       
MBEE_OTWI_CLAUS :       /*EMPT*/
                |       HOTHERWISE {OBSBLOCK();    outmell(MOTHERWISE);}
                                                  
                        BLOCK      {MBEEENDBLOCK();outmell(MENDOTHERWISE);}
                ;
ACTIVATOR	:	HACTIVATE		{ outmell(MBOOLEANKONST);
						  outmellival(FALSE);}
		|	HREACTIVATE		{ outmell(MBOOLEANKONST);
						  outmellival(TRUE);}
		;
SCHEDULE	:	/*EMPT*/		{ outmell(MCHARACTERKONST);
						  outmellival(DIRECT);
						  outmell(MINTEGERKONST);
						  outmellival(0);
						  outmell(MNONE);
						  outmell(MBOOLEANKONST);
						  outmellival(FALSE);}
		|	ATDELAY EXPRESSION	{ outmell(MNONE);}
			PRIOR
		|	BEFOREAFTER		{ outmell(MINTEGERKONST);
						  outmellival(0);}
			EXPRESSION		{ outmell(MBOOLEANKONST);
						  outmellival(FALSE);}
		;
ATDELAY		:	HAT			{ outmell(MCHARACTERKONST);
						  outmellival(AT);}
		|	HDELAY			{ outmell(MCHARACTERKONST);
						  outmellival(DELAYS);}
		;
BEFOREAFTER	:	HBEFORE			{ outmell(MCHARACTERKONST);
						  outmellival(BEFORE);}
		|	HAFTER			{ outmell(MCHARACTERKONST);
						  outmellival(AFTER);}
		;
PRIOR		:	/*EMPT*/		{ outmell(MBOOLEANKONST);
						  outmellival(FALSE);}
		|	HPRIOR			{ outmell(MBOOLEANKONST);
						  outmellival(TRUE);}
		;
/* GRAMATIKK FOR SETNINGER OG DEKLARASJONER */
MODULSTATEMENT  :       HWHILE
                        EXPRESSION
                        HDO     { STOPOBSBLOCK(); outmell(MWHILE);
                                  OBSBLOCK();}
                        BLOCK   { MBEEENDBLOCK(); outmell(MENDWHILE);
					  	          $$=STATEMENT;}
	    	|       HIF
                        EXPRESSION
                        HTHEN   { STOPOBSBLOCK(); outmell(MIF);
                                  OBSBLOCK();}
                        BLOCK   { MBEEENDBLOCK();}
                        MBEE_ELSE_PART          { outmell(MENDIF);
							  $$=STATEMENT;}
		|       HFOR    
                        HIDENTIFIER
                        HASSIGN { STOPOBSBLOCK(); outmell(MIDENTIFIER);
                                                  outmellid($2);}
                        FOR_LIST
                        HDO     { newblck(KFOR);
                        if($3==HASSIGNVALUE)      outmell(MFOR)
                                        else      outmell(MFORR);
                                  OBSBLOCK();     outmell(MFORDO);}
                        BLOCK   { MBEEENDBLOCK();
                                  endblock(); outmell(MENDFOR);
							  $$=STATEMENT;}
		|       GOTO
                        EXPRESSION              { outmell(MGOTO);
                                  STOPOBSBLOCK();	  $$=STATEMENT;}
		|       HINSPECT
                        EXPRESSION              { outmell(MINSPECT);
                                  STOPOBSBLOCK();
                                  newblck(KINSP);}
                        CONN_STATE_R_PT
                                { endblock();}
                        MBEE_OTWI_CLAUS         { outmell(MENDINSPECT);
							  $$=STATEMENT;}
		|       HINNER  { STOPOBSBLOCK(); outmell(MINNER);
                                  reginner();		  $$=STATEMENT;}
                |       HIDENTIFIER
                        HLABELSEPARATOR 
                                { STOPOBSBLOCK();
                                  type=TLABEL;
                                  kind=KSIMPLE;
                                  newdekl($1);    outmell(MLABEL);
                                                  outmellid($1);
                                                  outmell(MENDLABEL);}
                        DECLSTATEMENT   { if($4<=DECLARATION)
                                            { yerror(27);
                                              $$=DECLARATION;}
                                          else $$=$4;}
                |       EXPRESSION_SIMP
                        HBEGIN
                                { $<ident>$=$1; }
			IMPORT_SPEC_MODULE
				{ outmell(MPRBLOCK);
				  prefquantident=$1;
                                  newblck(KPRBLK);}
                        MBEE_DECLSTMS
                        HEND    { endblock(); outmell(MENDPRBLOCK);
                                                          $$=STATEMENT;}
		|	EXPRESSION_SIMP HBEGIN error HSTATEMENTSEPARATOR
                        MBEE_DECLSTMS HEND              { $$=STATEMENT;
			         endblock(); outmell(MENDPRBLOCK);}
		|	EXPRESSION_SIMP HBEGIN error HEND
						        { $$=STATEMENT;
			         endblock(); outmell(MENDPRBLOCK);}

                |       EXPRESSION_SIMP
                                { STOPOBSBLOCK();         $$=STATEMENT;
                                                  outmell(MENDASSIGN);}
		|	ACTIVATOR EXPRESSION SCHEDULE
						{      	  $$=STATEMENT;
						  outmell(MENDSEP);
						  outmell(MARGUMENTSEP);
						  outmell(MARGUMENTSEP);
						  outmell(MARGUMENTSEP);
						  outmell(MARGUMENTSEP);
						  outmell(MARGUMENTSEP);
						  outmell(MARGUMENTSEP);
						  outmell(MARGUMENT);
						  outmellid(activateid);
						  outmell(MENDASSIGN);}
                |       HBEGIN
                                { STOPOBSBLOCK();
                                  OBSBLOCK();}
                        MBEE_DECLSTMS
                        HEND    { MBEEENDBLOCK();         $$=STATEMENT;}
		|       MBEE_TYPE HPROCEDURE
                        HIDENTIFIER
                                { MBEENEWBLOCK(); outmell(MPROCEDURE);
                                          kind=KPROC;
                                          newdekl($3);
                                          newblck(kind);}
                        HEADING BLOCK   { endblock(); $$=DECLARATION;
                                                  outmell(MENDPROCEDURE);}
		|       HIDENTIFIER
			HCLASS
                        NO_TYPE
				{ $<ident>$=$1; }
			IMPORT_SPEC_MODULE
                        HIDENTIFIER
				{ prefquantident=$1;
				  outmell(MCLASS);
                                          kind=KCLASS;
                                          newdekl($6);
                                          newblck(kind);}
                        HEADING
                        BLOCK           { endblock(); $$=DECLARATION;
                                                  outmell(MENDCLASS);}
                |       HCLASS
                        NO_TYPE 
                        HIDENTIFIER
                                { prefquantident=0;
                                  MBEENEWBLOCK(); outmell(MCLASS);
                                          kind=KCLASS;
                                          newdekl($3);
                                          newblck(kind);}
                        HEADING
                        BLOCK           { endblock(); $$=DECLARATION;
                                                  outmell(MENDCLASS);}
                |       EXT_DECLARATION			{ $$=EXTDECLARATION;}
		|       /*EMPT*/{ STOPOBSBLOCK();	  $$=EMPTYSTATEMENT;}
                ;
IMPORT_SPEC_MODULE:		{ MBEENEWBLOCK(); 
                                  kind=KCLASS;
				  if($<ident>0==simsetident && 
				     finddecl(simsetident,cblock,FALSE)==NULL)
				    lesinn_external_spec(simsetident,
					SIMSETATRFILE);
				  if($<ident>0==simulationident && finddecl(
				    simulationident,cblock,FALSE)==NULL)
				    lesinn_external_spec(simulationident,
					SIMULATIONATRFILE);
				  if(($<ident>0==fileident && finddecl(
				      fileident,cblock,FALSE)==NULL) ||
				     ($<ident>0==outfileident && finddecl(
				      outfileident,cblock,FALSE)==NULL) ||
				     ($<ident>0==infileident && finddecl(
				      infileident,cblock,FALSE)==NULL) ||
				     ($<ident>0==directfileident && finddecl(
				      directfileident,cblock,FALSE)==NULL) ||
				     ($<ident>0==printfileident && finddecl(
				      printfileident,cblock,FALSE)==NULL) ||
				     ($<ident>0==bytefileident && finddecl(
				      bytefileident,cblock,FALSE)==NULL) ||
				     ($<ident>0==inbytefileident && finddecl(
				      inbytefileident,cblock,FALSE)==NULL) ||
				     ($<ident>0==outbytefileident && finddecl(
				      outbytefileident,cblock,FALSE)==NULL) ||
				     ($<ident>0==directbytefileident && finddecl(
				      directbytefileident,cblock,FALSE)==NULL))
				    lesinn_external_spec(fileident,
					FILEATRFILE);}
		;
DECLSTATEMENT	:	MODULSTATEMENT
		|       TYPE
                        HIDENTIFIER
			MBEE_CONSTANT
                        HPAREXPSEPARATOR 
                                { MBEENEWBLOCK();
                                          kind=KSIMPLE;
                                          newdekl($2);
					  categ=CLOCAL;}
                        IDENTIFIER_LISTC                { $$=DECLARATION;}
                |       TYPE
                        HIDENTIFIER
			MBEE_CONSTANT
                                { MBEENEWBLOCK();
                                          kind=KSIMPLE;
                                          newdekl($2);
					  categ=CLOCAL;	  $$=DECLARATION;}
                |       MBEE_TYPE 
                        HARRAY  { MBEENEWBLOCK();
                                          kind=KARRAY;}
                        ARR_SEGMENT_LIST                { $$=DECLARATION;}
                |       HSWITCH
                        HIDENTIFIER
                        HASSIGN { MBEENEWBLOCK(); outmell(MIDENTIFIER);
                                                  outmellid($2);
                                          type=TLABEL;
                                          kind=KARRAY;
                                          newdekl($2);}
                        SWITCH_LIST                     { $$=DECLARATION;
                                                   outmell(MSWITCH);
                                                   outmell(MENDSWITCH);}
                ;
BLOCK           :       DECLSTATEMENT   { if($1<=DECLARATION)yerror(29);}
                |       HBEGIN MBEE_DECLSTMS HEND
		|	HBEGIN error HSTATEMENTSEPARATOR MBEE_DECLSTMS HEND
		|	HBEGIN error HEND
		;
MBEE_DECLSTMS   :       MBEE_DECLSTMSU  { if($1<=DECLARATION)yerror(28);
					  $$=$1;}
                ;
MBEE_DECLSTMSU  :       DECLSTATEMENT   { $$=$1;}
                |       MBEE_DECLSTMSU
                        HSTATEMENTSEPARATOR
                        DECLSTATEMENT   { if($1>=STATEMENT && $3<=DECLARATION)
                                            yerror(26);
                                          $$=$3;}
                ;
MODULS		:	MODULSTATEMENT	{ if($1==DECLARATION)
					  {separat_comp=TRUE;gettimestamp();}
                                          $$=$1;}
  		|	MODULS HSTATEMENTSEPARATOR MODULSTATEMENT
					{ if($1>=STATEMENT && $3<=DECLARATION)
                                            yerror(26);else
       					  if($1>=STATEMENT 
					     && $3!=EMPTYSTATEMENT)yerror(25);
                                          if(separat_comp && $3==STATEMENT)
                                             yerror(25);
                                          if($3==DECLARATION && !separat_comp)
					  {separat_comp=TRUE;gettimestamp();}
					  $$=$3;}
		;
/* GRAMATIKK FOR DEL AV DEKLARASJONER */
ARR_SEGMENT_LIST:       ARRAY_SEGMENT
                        HBEGPAR
                        BAUND_PAIR_LIST HENDPAR { outmell(MARRAY);
                                                  outmell(MENDARRAY);
                                                  settarraydim();}
                |       ARR_SEGMENT_LIST
                        HPAREXPSEPARATOR
                        ARRAY_SEGMENT
                        HBEGPAR
                        BAUND_PAIR_LIST HENDPAR { outmell(MARRAY);
                                                  outmell(MENDARRAY);
                                                  settarraydim();}
                ;
ARRAY_SEGMENT   :       HIDENTIFIER             { outmell(MIDENTIFIER);
                                                  outmellid($1);
                                                  outmell(MENDSEP);
                                                  outmell(MARRAYSEP);
                                                  newdekl($1);
                                   if(sistearray==NULL)sistearray=clastdecl;}
                |       HIDENTIFIER
                        HPAREXPSEPARATOR        { outmell(MIDENTIFIER);
                                                  outmellid($1);
                                                  newdekl($1);
                                   if(sistearray==NULL)sistearray=clastdecl;}
                        ARRAY_SEGMENT           { outmell(MARRAYSEP);}
                ;
BAUND_PAIR_LIST :       EXPRESSION
                        HLABELSEPARATOR
                        EXPRESSION              { ++arrdim;
                                                  outmell(MBOUNDPARSEP);
                                                  outmell(MENDSEP);
                                                  outmell(MBOUNDSEP);}
                |       EXPRESSION
                        HLABELSEPARATOR
                        EXPRESSION
                        HPAREXPSEPARATOR        { outmell(MBOUNDPARSEP);}
                        BAUND_PAIR_LIST         { ++arrdim;
                                                  outmell(MBOUNDSEP);}
                ;
SWITCH_LIST     :       EXPRESSION              { outmell(MENDSEP);
                                                  outmell(MSWITCHSEP);}
                |       EXPRESSION
                        HPAREXPSEPARATOR
                        SWITCH_LIST             { outmell(MSWITCHSEP);}
                ;
HEADING         :       MBEE_FMAL_PAR_P HSTATEMENTSEPARATOR
                        MBEE_MODE_PART  { categ=CSPEC;}
                        MBEE_SPEC_PART  { kind=KNOKD; /*SJEKK AT ALT ER DEF*/}
                        MBEE_PROT_PART  { categ=CVIRT;}
                        MBEE_VIRT_PART
			/*                        ERROR_PART*/
                                        { categ=CLOCAL;}
                ;
MBEE_FMAL_PAR_P :       /*EMPT*/
                |       FMAL_PAR_PART
                ;
FMAL_PAR_PART   :       HBEGPAR NO_TYPE
                        MBEE_LISTV HENDPAR
                ;
MBEE_LISTV      :       /*EMPT*/
                |       LISTV
                ;
LISTV           :       HIDENTIFIER     { kind=KNOKD;   
                                          categ=CDEFLT;
                                          newdekl($1);}
                |	HDOTDOTDOT      { kind=KNOKD;   
                                          categ=CDEFLT;
                                          type=TVARARGS;
					  newdekl(varargsid);
					  type=TNOTY;}
                |       HIDENTIFIER     { kind=KNOKD;   
                                          categ=CDEFLT;
                                          newdekl($1);}
                        HPAREXPSEPARATOR LISTV
                |       FPP_SPEC
                |       FPP_SPEC
                        HPAREXPSEPARATOR LISTV
                ;
FPP_HEADING     :       HBEGPAR NO_TYPE
                        FPP_MBEE_LISTV HENDPAR
                ;
FPP_MBEE_LISTV  :       /*EMPT*/
                |       FPP_LISTV
                ;
FPP_LISTV       :	HDOTDOTDOT      { kind=KNOKD;   
                                          categ=CDEFLT;
                                          type=TVARARGS;
					  newdekl(varargsid);
					  type=TNOTY;}
                |       FPP_SPEC
                |       FPP_SPEC
                        HPAREXPSEPARATOR LISTV
                ;
FPP_SPEC        :       FPP_CATEG FPP_SPECIFIER HIDENTIFIER     
                                       { newdekl($3);}
		|	FPP_CATEG FPP_PROC_DECL_IN_SPEC
		;
FPP_SPECIFIER   :       TYPE            { kind=KSIMPLE;}
                |       MBEE_TYPE
                        HARRAY          { kind=KARRAY;}
                |       HLABEL          { type=TLABEL;
                                          kind=KSIMPLE;}
                |       HSWITCH         { type=TLABEL;
                                          kind=KARRAY;}
                ;
FPP_CATEG       :       HNAME HLABELSEPARATOR           
                                       { categ=CNAME;}
                |       HVALUE HLABELSEPARATOR        
                                       { categ=CVALUE;}
                |       HVAR HLABELSEPARATOR        
                                       { categ=CVAR;}
                |       /*EMPT*/       { categ=CDEFLT;}
                ;
FPP_PROC_DECL_IN_SPEC:	MBEE_TYPE HPROCEDURE
                        HIDENTIFIER
                                	{ kind=KPROC;
					  $<ival>$=categ;
                                          newdekl($3);
                                          newblck(kind);}
                        FPP_HEADING
					{ categ=$<ival>4; /* M} settes tilbake*/}
				{ endblock();}
		;
IDENTIFIER_LISTV:       HIDENTIFIER     { newdekl($1);}
                |	HDOTDOTDOT {	  type=TVARARGS;
					  newdekl(varargsid);
					  type=TNOTY;}
                |       HIDENTIFIER     { newdekl($1);}
                        HPAREXPSEPARATOR IDENTIFIER_LISTV
                ;
MBEE_MODE_PART  :       /*EMPT*/
                |       MODE_PART
                ;
MODE_PART       :       NAME_PART
                |       VALUE_PART
                |       VAR_PART
                |       NAME_PART VALUE_PART
                |       VALUE_PART NAME_PART
                |       NAME_PART VAR_PART
                |       VAR_PART NAME_PART
                |       VALUE_PART VAR_PART
                |       VAR_PART VALUE_PART
                |       VAR_PART NAME_PART VALUE_PART
                |       NAME_PART VAR_PART VALUE_PART
                |       NAME_PART VALUE_PART VAR_PART
                |       VAR_PART VALUE_PART NAME_PART
                |       VALUE_PART VAR_PART NAME_PART
                |       VALUE_PART NAME_PART VAR_PART
                ;
NAME_PART       :       HNAME           { categ=CNAME;}
                        IDENTIFIER_LISTV
                        HSTATEMENTSEPARATOR
                ;
VAR_PART        :       HVAR            { categ=CVAR;}
                        IDENTIFIER_LISTV
                        HSTATEMENTSEPARATOR
                ;
VALUE_PART      :       HVALUE          { categ=CVALUE;}
                        IDENTIFIER_LISTV HSTATEMENTSEPARATOR
                ;
MBEE_SPEC_PART  :       /*EMPT*/
                |       SPEC_PART
                ;
SPEC_PART       :       ONE_SPEC
                |       SPEC_PART ONE_SPEC
                ;
ONE_SPEC	:	SPECIFIER IDENTIFIER_LIST HSTATEMENTSEPARATOR
		|	NO_TYPE HPROCEDURE HIDENTIFIER HOBJRELOPERATOR
			  { if($4!=HIS) yerror(8);} 
			PROC_DECL_IN_SPEC HSTATEMENTSEPARATOR
                |       FPP_PROC_DECL_IN_SPEC HSTATEMENTSEPARATOR
                |       MBEE_TYPE HPROCEDURE HIDENTIFIER HSTATEMENTSEPARATOR
					{ yerror(45);}
                |       MBEE_TYPE HPROCEDURE HIDENTIFIER HPAREXPSEPARATOR
                        IDENTIFIER_LIST HSTATEMENTSEPARATOR
					{ yerror(45);}
		;
SPECIFIER       :       TYPE            { kind=KSIMPLE;}
                |       MBEE_TYPE
                        HARRAY          { kind=KARRAY;}
                |       HLABEL          { type=TLABEL;
                                          kind=KSIMPLE;}
                |       HSWITCH         { type=TLABEL;
                                          kind=KARRAY;}
                ;
PROC_DECL_IN_SPEC:	MBEE_TYPE HPROCEDURE
                        HIDENTIFIER
                                	{ kind=KPROC;
					  $<ival>$=categ;
                                          newdekl($3);
                                          newblck(kind);}
                        HEADING
					{ categ=$<ival>4; /* M} settes tilbake*/}
			MBEE_BEGIN_END 
				{ endblock();}
		;
MBEE_BEGIN_END	:	/* EMPTY */
		|	HBEGIN HEND
		;
MBEE_PROT_PART  :       /*EMPT*/
                |       PROTECTION_PART
                ;
PROTECTION_PART :       PROT_SPECIFIER IDENTIFIER_LIST
                        HSTATEMENTSEPARATOR
                |       PROTECTION_PART  PROT_SPECIFIER
                        IDENTIFIER_LIST HSTATEMENTSEPARATOR
                ;
PROT_SPECIFIER  :       HHIDDEN         { categ=CHIDEN;}
                |       HPROTECTED      { categ=CPROT;}
                |       HHIDDEN
                        HPROTECTED      { categ=CHIPRO;}
                |       HPROTECTED
                        HHIDDEN         { categ=CHIPRO;}
                ;
MBEE_VIRT_PART  :       /*EMPT*/
                |       VIRTUAL_PART
                ;
VIRTUAL_PART    :       HVIRTUAL
                        HLABELSEPARATOR
                        MBEE_SPEC_PART
                ;
ERROR_PART      :       /*EMPT*/
                |       ERROR_PART MODE_PART            { yerror(17);}
                |       ERROR_PART SPEC_PART            { yerror(18);}
                |       ERROR_PART PROTECTION_PART      { yerror(19);}
                |       ERROR_PART VIRTUAL_PART         { yerror(20);}
/*                |       error
/*                                ^
                                  I
                                  --- CATEG OG KIND */
                ;
IDENTIFIER_LIST :       HIDENTIFIER     { newdekl($1);}
                |       IDENTIFIER_LIST HPAREXPSEPARATOR
                        HIDENTIFIER     { newdekl($3);}
                ;
IDENTIFIER_LISTC:       HIDENTIFIER 
			MBEE_CONSTANT   { newdekl($1);
					  categ=CLOCAL;}
                |       IDENTIFIER_LISTC HPAREXPSEPARATOR
                        HIDENTIFIER 
			MBEE_CONSTANT   { newdekl($3);
					  categ=CLOCAL;}
                ;
MBEE_CONSTANT	:	/* EMPTY */
		|	HVALRELOPERATOR
				{ MBEENEWBLOCK();
			    if($1!=HEQ) yerror(8);
					  if(type==TREF)yerror(7);
					  categ=CCONSTU;
						  outmell(MIDENTIFIER);
						  outmellid($<token>0);} 
			EXPRESSION		{ outmell(MASSIGN);
						  outmell(MCONST);}
		;

/* GRAMATIKK FOR UTTRYKK */
EXPRESSION      :       EXPRESSION_SIMP {}
                |       HIF
                        EXPRESSION
                        HTHEN
                        EXPRESSION
                        HELSE
                        EXPRESSION              { outmell(MELSEE);
                                                  outmell(MIFE);}
                ;
EXPRESSION_SIMP :	EXPRESSION_SIMP
			HASSIGN
			EXPRESSION { if($2==HASSIGNREF)outmell(MASSIGNR)
                                          else    outmell(MASSIGN);$$=NULL;}
		|

			EXPRESSION_SIMP
                        HCONC
                        EXPRESSION_SIMP         { outmell(MCONC);$$=NULL;}
                |       EXPRESSION_SIMP HOR
                        HELSE
                        EXPRESSION_SIMP
                        %prec HORELSE           { outmell(MORELSE);$$=NULL;}
                |       EXPRESSION_SIMP HAND
                        HTHEN
                        EXPRESSION_SIMP
                        %prec HANDTHEN          { outmell(MANDTHEN);$$=NULL;}
                |       EXPRESSION_SIMP
                        HEQV EXPRESSION_SIMP    { outmell(MEQV);$$=NULL;}
                |       EXPRESSION_SIMP
                        HIMP EXPRESSION_SIMP    { outmell(MIMP);$$=NULL;}
                |       EXPRESSION_SIMP
                        HOR EXPRESSION_SIMP     { outmell(MOR);$$=NULL;}
                |       EXPRESSION_SIMP
                        HAND EXPRESSION_SIMP    { outmell(MAND);$$=NULL;}
                |       HNOT EXPRESSION_SIMP    { outmell(MNOT);$$=NULL;}
                |       EXPRESSION_SIMP
                        HVALRELOPERATOR
                        EXPRESSION_SIMP
                                { switch($2)
                                    {   case HEQ: outmell(MEQ);break;
                                        case HNE: outmell(MNE);break;
                                        case HLT: outmell(MLT);break;
                                        case HLE: outmell(MLE);break;
                                        case HGT: outmell(MGT);break;
                                        case HGE: outmell(MGE);break;
                                    }$$=NULL;}
                |       EXPRESSION_SIMP
                        HREFRELOPERATOR
                        EXPRESSION_SIMP
                                { if($2==HNER)    outmell(MNER)
                                        else      outmell(MEQR);$$=NULL;}
                |       EXPRESSION_SIMP
                        HOBJRELOPERATOR
                        EXPRESSION_SIMP
                                { if($2==HIS)     outmell(MIS)
                                        else      outmell(MINS);$$=NULL;}
                |       HTERMOPERATOR
                        EXPRESSION_SIMP %prec UNEAR
                                { if($1==HADD)    outmell(MUADD)
                                        else      outmell(MUSUB);$$=NULL;}
                |       EXPRESSION_SIMP
                        HTERMOPERATOR
                        EXPRESSION_SIMP
                                { if($2==HADD)    outmell(MADD)
                                        else      outmell(MSUB);$$=NULL;}
                |       EXPRESSION_SIMP
                        HFACTOROPERATOR
                        EXPRESSION_SIMP         
                                { if($2==HMUL)    outmell(MMUL) else
                                  if($2==HDIV)    outmell(MDIV)
                                        else      outmell(MINTDIV);$$=NULL;}
                |       EXPRESSION_SIMP
                        HPRIMARYOPERATOR
                        EXPRESSION_SIMP         { outmell(MPRIMARY);$$=NULL;}
                |       HBEGPAR
                        EXPRESSION HENDPAR      { outmell(MNOOP);$$=NULL;}
                |       HTEXTKONST              { outmell(MTEXTKONST);
                                                  outmelltval($1);$$=NULL;}
                |       HCHARACTERKONST         { outmell(MCHARACTERKONST);
                                                  outmellival($1);$$=NULL;}
                |       HREALKONST              { outmell(MREALKONST);
                                                  outmellrval($1);$$=NULL;}
                |       HINTEGERKONST           { outmell(MINTEGERKONST);
                                                  outmellival($1);$$=NULL;}
                |       HBOOLEANKONST           { outmell(MBOOLEANKONST);
                                                  outmellival($1);$$=NULL;}
                |       HNONE                   { outmell(MNONE);$$=NULL;}
                |       HIDENTIFIER
                                { $<ident>$=$1;}
                        MBEE_ARG_R_PT
                |       HTHIS HIDENTIFIER       { outmell(MTHIS);
                                                  outmellid($2);$$=NULL;}
                |       HNEW
                        HIDENTIFIER
                        ARG_R_PT                { outmell(MNEWARG);
                                                  outmellid($2);$$=NULL;}
                |       EXPRESSION_SIMP
                        HDOT
                        EXPRESSION_SIMP         { outmell(MDOT);$$=NULL;}
                |       EXPRESSION_SIMP
                        HQUA HIDENTIFIER        { outmell(MQUA);
                                                  outmellid($3);$$=NULL;}
                ;
ARG_R_PT        :       /*EMPTY*/               { outmell(MENDSEP);}
                |       HBEGPAR
                        ARGUMENT_LIST HENDPAR
                ;
MBEE_ARG_R_PT   :       /*EMPTY*/               { outmell(MIDENTIFIER);
                                                  outmellid($<ident>0);
						  $$=$<ident>0;}
                |       HBEGPAR
                        ARGUMENT_LIST HENDPAR   { outmell(MARGUMENT);
                                                  outmellid($<ident>0);}
                ;
ARGUMENT_LIST   :       EXPRESSION              { outmell(MENDSEP);
                                                  outmell(MARGUMENTSEP);}
                |       EXPRESSION
                        HPAREXPSEPARATOR
                        ARGUMENT_LIST           { outmell(MARGUMENTSEP);}
                ;
%%
/******************************************************************************
                                                                YYERROR      */

/* Oppdages feil så blir denne procedyren kalt */

yyerror(s)char s[];
  {
     yaccerror=TRUE;
#if 0
     if(s[0]=='s')yerror(13);else
     if(s[0]=='y')yerror(14);else
       yerror(16);
#else
     yerror(21,s);
#endif
     outmell(MERROR);
  }

/******************************************************************************
                                                                YLEX         */
                                                                             
#ifdef yylex
#undef yylex
ylex()
{
  long i;
  i=yylex();
#ifdef DEBUG
  if(option_lex)
    {
      printf("line: %ld yylex:",yylineno);
      print_lexsymbol(i,&yylval);
      printf("\n");
    }
#endif
  return(i);
}
#define yylex ylex
#endif

/******************************************************************************
                                                                 INIT_PARSER */

init_parser()
{
  activateid=tag("activat",7);
  varargsid=tag("...",3);
  Ckind=tag("C",1);
  simsetident=tag("SIMSET",6);
  simulationident=tag("SIMULATION",10);
  fileident=tag("FILE",4);
  imagefileident=tag("IMAGEFILE",9);
  outfileident=tag("OUTFILE",7);
  infileident=tag("INFILE",6);
  directfileident=tag("DIRECTFILE",10);
  printfileident=tag("PRINTFILE",9);
  bytefileident=tag("BYTEFILE",8);
  inbytefileident=tag("INBYTEFILE",10);
  outbytefileident=tag("OUTBYTEFILE",11);
  directbytefileident=tag("DIRECTBYTEFILE",14);
}

