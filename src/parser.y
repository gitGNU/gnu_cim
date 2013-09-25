%{
/* $Id: cparser.y,v 1.9 1994/10/15 13:30:36 cim Exp $ */

/* Copyright (C) 1994, 1998 Sverre Hvammen Johansen,
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
#include "dekl.h"
#include "cimcomp.h"
#include "lex.h"
#include "name.h"
#include "mellbuilder.h"
#include <stdio.h>
#include <obstack.h>
char *xmalloc();
void free();
void yyerror (char s[]);
#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

static struct obstack os_block;
static char *first_object_allocated_ptr_block;

#define EXTDECLARATION 0
#define DECLARATION 1
#define STATEMENT 2
#define EMPTYSTATEMENT 3

static char type, kind, categ;

char *Ckind;
char *varargsid;
char *activateid;
char *simsetident,*simulationident,*fileident,*imagefileident,
     *outfileident,*infileident,*directfileident,*printfileident,
     *bytefileident,*inbytefileident,*outbytefileident,*directbytefileident;

extern char sensitive;
char ysensitive;

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
typedef struct _blockstack blockstack_t;
struct _blockstack
{
  char rem;
  blockstack_t *prev;
}  *blockp;

#define OBSBLOCK() { blockstack_t *prev= blockp;\
     blockp= (blockstack_t *) \
       obstack_alloc (&os_block,sizeof (blockstack_t));\
     blockp->prev= prev;\
     blockp->rem=TRUE;}

#define MBEENEWBLOCK() if (blockp->rem == TRUE) {\
       mout(MBLOCK);\
       blockp->rem= FALSE;begin_block(KBLOKK);}

#define MBEEENDBLOCK() { blockstack_t *prev= blockp->prev;\
     if(blockp->rem==FALSE)\
       {mout(MENDBLOCK);end_block(NULL,CCNO);}\
     obstack_free (&os_block, blockp);\
     blockp= prev;}

#define STOPOBSBLOCK() if(blockp->rem==TRUE)blockp->rem=MAYBEE

/* Spesifikasjonene til YACC */
%}
%union {
	long token;
	long ival;
        long arrdim;
	double rval;
	char *ident;
	char *tval;
	char stat_decl;
  	char kind;
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
%token <rval> HREALKONST
%token <tval> HTEXTKONST

%type <tval> EXT_IDENT
%type <stat_decl> DECLSTATEMENT MODULSTATEMENT MBEE_DECLSTMS MBEE_DECLSTMSU
%type <stat_decl> MODULS
%type <ident> EXPRESSION_SIMP MBEE_ARG_R_PT 
%type <arrdim> BAUND_PAIR_LIST
%type <kind> FOR_LIST

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
MAIN_MODULE     :       {	  categ=CLOCAL; mout(MBLOCK);
                                  begin_block(KBLOKK);separat_comp=FALSE;}
			MODULS	{ end_block(NULL,CCNO);   mout(MENDBLOCK);}
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
				  yerror (1);
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
				{ if($2!=HIS)yerror (2);}
		        MBEE_TYPE HPROCEDURE
			HIDENTIFIER
				{         reg_decl($6, type, KPROC, CCPROC);
                                          begin_block(kind);}
                        HEADING EMPTY_BLOCK
				{ categ=CLOCAL;
				  end_block($1==NULL?$<ident>0:tag($1),CCCPROC);}
/*		|
			EXT_IDENT
				{ if($1!=NULL)yerror (3);
				  reg_decl($0, type, kind, categ);}
			MBEE_REST_EXT_LIST
				{ end_block(NULL,CCNO);}
		;
MBEE_REST_EXT_LIST:	/* EMPTY 
		|	HPAREXPSEPARATOR EXT_KIND_LIST
		;
EXT_KIND_LIST	:	EXT_KIND_ITEM
		|	EXT_KIND_LIST HPAREXPSEPARATOR EXT_KIND_ITEM
		;
EXT_KIND_ITEM	:	HIDENTIFIER
			EXT_IDENT
				{ if($2!=NULL)yerror (3);
				  reg_decl($1, type, kind, categ);}*/
		;
EMPTY_BLOCK	:	/*EMPT*/
		|	HBEGIN HEND
		;
EXT_LIST	:	EXT_ITEM
		|	EXT_LIST HPAREXPSEPARATOR EXT_ITEM
		;
EXT_ITEM	:	HIDENTIFIER
			EXT_IDENT
				{ lesinn_external_spec($1,$2, kind);}
		;
EXT_IDENT	:	/* EMPTY */ { $$=NULL;}
		|	HVALRELOPERATOR { if($1!=HEQ)yerror (9);
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
                        HTHEN   { 		  mout(MELSE);
						  mout(MIF);
                                  OBSBLOCK();}
                        BLOCK   { MBEEENDBLOCK();}
                        MBEE_ELSE_PART          { mout(MENDIF);}*/
                |       HELSE   { OBSBLOCK();     mout(MELSE);}
                        BLOCK   { MBEEENDBLOCK();}
                ;
FOR_LIST        :       FOR_LIST_ELEMENT        { mout(MENDSEP);
                                                  mout(MLISTSEP);
						  $$=KFOR;}
                |       FOR_LIST_ELEMENT
                        HPAREXPSEPARATOR
                        FOR_LIST                { mout(MLISTSEP);
						  $$=KFORLIST;}
                ;
FOR_LIST_ELEMENT:       EXPRESSION
                        MBEE_F_L_EL_R_PT        
                ;
MBEE_F_L_EL_R_PT:       /*EMPT*/
                |       HWHILE
                        EXPRESSION              { mout(MFORWHILE);}
                |       HSTEP
                        EXPRESSION
                        HUNTIL
                        EXPRESSION              { mout(MUNTIL);
                                                  mout(MSTEP);}
                ;
GOTO            :       HGO
                        HTO
                |       HGOTO
                ;
CONN_STATE_R_PT :       WHEN_CLAUSE_LIST
                |       HDO   {   begin_block(KCON);   mout(MDO);
                              OBSBLOCK(); }
                        BLOCK {   end_block(NULL,CCNO);  
                                  MBEEENDBLOCK();  mout(MENDDO);}
                ;
WHEN_CLAUSE_LIST:       HWHEN
                        HIDENTIFIER
                        HDO    {   begin_block(KCON);  mout(MIDENTIFIER);
                                   OBSBLOCK();     mout_id($2);
				   		   mout(MWHEN);}
                        BLOCK  {   end_block(NULL,CCNO);   
                              MBEEENDBLOCK(); mout(MENDWHEN);}
                |       WHEN_CLAUSE_LIST
                        HWHEN
                        HIDENTIFIER
                        HDO    { begin_block(KCON);	   mout(MIDENTIFIER);
                                 OBSBLOCK();       mout_id($3);
				 		   mout(MWHEN);}
                        BLOCK  { end_block(NULL,CCNO); 
                              MBEEENDBLOCK();    mout(MENDWHEN);}
                ;                       
MBEE_OTWI_CLAUS :       /*EMPT*/
                |       HOTHERWISE {OBSBLOCK();    mout(MOTHERWISE);}
                                                  
                        BLOCK      {MBEEENDBLOCK();mout(MENDOTHERWISE);}
                ;
ACTIVATOR	:	HACTIVATE		{ mout(MBOOLEANKONST);
						  mout_ival(FALSE);}
		|	HREACTIVATE		{ mout(MBOOLEANKONST);
						  mout_ival(TRUE);}
		;
SCHEDULE	:	/*EMPT*/		{ mout(MCHARACTERKONST);
						  mout_ival(DIRECT);
						  mout(MINTEGERKONST);
						  mout_ival(0);
						  mout(MNONE);
						  mout(MBOOLEANKONST);
						  mout_ival(FALSE);}
		|	ATDELAY EXPRESSION	{ mout(MNONE);}
			PRIOR
		|	BEFOREAFTER		{ mout(MINTEGERKONST);
						  mout_ival(0);}
			EXPRESSION		{ mout(MBOOLEANKONST);
						  mout_ival(FALSE);}
		;
ATDELAY		:	HAT			{ mout(MCHARACTERKONST);
						  mout_ival(AT);}
		|	HDELAY			{ mout(MCHARACTERKONST);
						  mout_ival(DELAYS);}
		;
BEFOREAFTER	:	HBEFORE			{ mout(MCHARACTERKONST);
						  mout_ival(BEFORE);}
		|	HAFTER			{ mout(MCHARACTERKONST);
						  mout_ival(AFTER);}
		;
PRIOR		:	/*EMPT*/		{ mout(MBOOLEANKONST);
						  mout_ival(FALSE);}
		|	HPRIOR			{ mout(MBOOLEANKONST);
						  mout_ival(TRUE);}
		;
/* GRAMATIKK FOR SETNINGER OG DEKLARASJONER */
MODULSTATEMENT  :       HWHILE
                        EXPRESSION
                        HDO     { STOPOBSBLOCK(); mout(MWHILE);
                                  OBSBLOCK();}
                        BLOCK   { MBEEENDBLOCK(); mout(MENDWHILE);
					  	          $$=STATEMENT;}
	    	|       HIF
                        EXPRESSION
                        HTHEN   { STOPOBSBLOCK(); mout(MIF);
                                  OBSBLOCK();}
                        BLOCK   { MBEEENDBLOCK();}
                        MBEE_ELSE_PART          { mout(MENDIF);
							  $$=STATEMENT;}
		|       HFOR    
                        HIDENTIFIER
                        HASSIGN { STOPOBSBLOCK(); mout(MIDENTIFIER);
                                                  mout_id($2);}
                        FOR_LIST
                        HDO     { begin_block($5);
                        if($3==HASSIGNVALUE)      mout(MFOR);
                                        else      mout(MFORR);
                                  OBSBLOCK();     mout(MFORDO);}
                        BLOCK   { MBEEENDBLOCK();
                                  end_block(NULL,CCNO); mout(MENDFOR);
							  $$=STATEMENT;}
		|       GOTO
                        EXPRESSION              { mout(MGOTO);
                                  STOPOBSBLOCK();	  $$=STATEMENT;}
		|       HINSPECT
                        EXPRESSION              { mout(MINSPECT);
                                  STOPOBSBLOCK();
                                  begin_block(KINSP);}
                        CONN_STATE_R_PT
                                { end_block(NULL,CCNO);}
                        MBEE_OTWI_CLAUS         { mout(MENDINSPECT);
							  $$=STATEMENT;}
		|       HINNER  { STOPOBSBLOCK(); mout(MINNER);
                                  reg_inner();		  $$=STATEMENT;}
                |       HIDENTIFIER
                        HLABELSEPARATOR 
                                { STOPOBSBLOCK();
                                  reg_decl($1, TLABEL, KSIMPLE, categ);    mout(MLABEL);
                                                  mout_id($1);
                                                  mout(MENDLABEL);}
                        DECLSTATEMENT   { if($4<=DECLARATION)
                                            { yerror (27);
                                              $$=DECLARATION;}
                                          else $$=$4;}
                |       EXPRESSION_SIMP
                        HBEGIN
                                { $<ident>$=$1; }
			IMPORT_SPEC_MODULE
				{ mout(MPRBLOCK);
				  prefquantident=$1;
                                  begin_block(KPRBLK);}
                        MBEE_DECLSTMS
                        HEND    { end_block(NULL,CCNO); mout(MENDPRBLOCK);
                                                          $$=STATEMENT;}
		|	EXPRESSION_SIMP HBEGIN error HSTATEMENTSEPARATOR
                        MBEE_DECLSTMS HEND              { $$=STATEMENT;
			         end_block(NULL,CCNO); mout(MENDPRBLOCK);}
		|	EXPRESSION_SIMP HBEGIN error HEND
						        { $$=STATEMENT;
			         end_block(NULL,CCNO); mout(MENDPRBLOCK);}

                |       EXPRESSION_SIMP
                                { STOPOBSBLOCK();         $$=STATEMENT;
                                                  mout(MENDASSIGN);}
		|	ACTIVATOR EXPRESSION SCHEDULE
						{      	  $$=STATEMENT;
						  mout(MENDSEP);
						  mout(MARGUMENTSEP);
						  mout(MARGUMENTSEP);
						  mout(MARGUMENTSEP);
						  mout(MARGUMENTSEP);
						  mout(MARGUMENTSEP);
						  mout(MARGUMENTSEP);
						  mout(MARGUMENT);
						  mout_id(activateid);
						  mout(MENDASSIGN);}
                |       HBEGIN
                                { STOPOBSBLOCK();
                                  OBSBLOCK();}
                        MBEE_DECLSTMS
                        HEND    { MBEEENDBLOCK();         $$=STATEMENT;}
		|       MBEE_TYPE HPROCEDURE
                        HIDENTIFIER
                                { MBEENEWBLOCK(); mout(MPROCEDURE);
                                          reg_decl($3, type, KPROC, categ);
                                          begin_block(KPROC);}
                        HEADING BLOCK   { end_block(NULL,CCNO); $$=DECLARATION;
                                                  mout(MENDPROCEDURE);}
		|       HIDENTIFIER
			HCLASS
                        NO_TYPE
				{ $<ident>$=$1; }
			IMPORT_SPEC_MODULE
                        HIDENTIFIER
				{ prefquantident=$1;
				  mout(MCLASS);
                                          reg_decl($6, TNOTY, KCLASS, categ);
                                          begin_block(KCLASS);}
                        HEADING
                        BLOCK           { end_block(NULL,CCNO); $$=DECLARATION;
                                                  mout(MENDCLASS);}
                |       HCLASS
                        NO_TYPE 
                        HIDENTIFIER
                                { prefquantident=0;
                                  MBEENEWBLOCK(); mout(MCLASS);
                                          reg_decl($3, TNOTY, KCLASS, categ);
                                          begin_block(KCLASS);}
                        HEADING
                        BLOCK           { end_block(NULL,CCNO); $$=DECLARATION;
                                                  mout(MENDCLASS);}
                |       EXT_DECLARATION			{ $$=EXTDECLARATION;}
		|       /*EMPT*/{ STOPOBSBLOCK();	  $$=EMPTYSTATEMENT;}
                ;
IMPORT_SPEC_MODULE:		{ MBEENEWBLOCK(); 
                                  kind=KCLASS;
				  if($<ident>0==simsetident && 
				     find_decl(simsetident,cblock,FALSE)==NULL)
				    lesinn_external_spec(simsetident,
					SIMSETATRFILE, kind);
				  if($<ident>0==simulationident && find_decl(
				    simulationident,cblock,FALSE)==NULL)
				    lesinn_external_spec(simulationident,
					SIMULATIONATRFILE, kind);
				  if(($<ident>0==fileident && find_decl(
				      fileident,cblock,FALSE)==NULL) ||
				     ($<ident>0==outfileident && find_decl(
				      outfileident,cblock,FALSE)==NULL) ||
				     ($<ident>0==infileident && find_decl(
				      infileident,cblock,FALSE)==NULL) ||
				     ($<ident>0==directfileident && find_decl(
				      directfileident,cblock,FALSE)==NULL) ||
				     ($<ident>0==printfileident && find_decl(
				      printfileident,cblock,FALSE)==NULL) ||
				     ($<ident>0==bytefileident && find_decl(
				      bytefileident,cblock,FALSE)==NULL) ||
				     ($<ident>0==inbytefileident && find_decl(
				      inbytefileident,cblock,FALSE)==NULL) ||
				     ($<ident>0==outbytefileident && find_decl(
				      outbytefileident,cblock,FALSE)==NULL) ||
				     ($<ident>0==directbytefileident && find_decl(
				      directbytefileident,cblock,FALSE)==NULL))
				    lesinn_external_spec(fileident,
					FILEATRFILE, kind);}
		;
DECLSTATEMENT	:	MODULSTATEMENT
		|       TYPE
                        HIDENTIFIER
			MBEE_CONSTANT
                        HPAREXPSEPARATOR 
                                { MBEENEWBLOCK();
                                          kind=KSIMPLE;
                                          reg_decl($2, type, KSIMPLE, categ);
					  categ=CLOCAL;}
                        IDENTIFIER_LISTC                { $$=DECLARATION;}
                |       TYPE
                        HIDENTIFIER
			MBEE_CONSTANT
                                { MBEENEWBLOCK();
                                          reg_decl($2, type, KSIMPLE, categ);
					  categ=CLOCAL;	  $$=DECLARATION;}
                |       MBEE_TYPE 
                        HARRAY  { MBEENEWBLOCK();
                                          kind=KARRAY;}
                        ARR_SEGMENT_LIST                { $$=DECLARATION;}
                |       HSWITCH
                        HIDENTIFIER
                        HASSIGN { MBEENEWBLOCK(); mout(MIDENTIFIER);
                                                  mout_id($2);
                                          reg_decl($2, TLABEL, KARRAY, categ);}
                        SWITCH_LIST                     { $$=DECLARATION;
                                                   mout(MSWITCH);
                                                   mout(MENDSWITCH);}
                ;
BLOCK           :       DECLSTATEMENT   { if($1<=DECLARATION)yerror (29);}
                |       HBEGIN MBEE_DECLSTMS HEND
		|	HBEGIN error HSTATEMENTSEPARATOR MBEE_DECLSTMS HEND
		|	HBEGIN error HEND
		;
MBEE_DECLSTMS   :       MBEE_DECLSTMSU  { if($1<=DECLARATION)yerror (28);
					  $$=$1;}
                ;
MBEE_DECLSTMSU  :       DECLSTATEMENT   { $$=$1;}
                |       MBEE_DECLSTMSU
                        HSTATEMENTSEPARATOR
                        DECLSTATEMENT   { if($1>=STATEMENT && $3<=DECLARATION)
                                            yerror (26);
                                          $$=$3;}
                ;
MODULS		:	MODULSTATEMENT	{ if($1==DECLARATION)
					  {separat_comp=TRUE;gettimestamp();}
                                          $$=$1;}
  		|	MODULS HSTATEMENTSEPARATOR MODULSTATEMENT
					{ if($1>=STATEMENT && $3<=DECLARATION)
                                            yerror (26);else
       					  if($1>=STATEMENT 
					     && $3!=EMPTYSTATEMENT)yerror (25);
                                          if(separat_comp && $3==STATEMENT)
                                             yerror (25);
                                          if($3==DECLARATION && !separat_comp)
					  {separat_comp=TRUE;gettimestamp();}
					  $$=$3;}
		;
/* GRAMATIKK FOR DEL AV DEKLARASJONER */
ARR_SEGMENT_LIST:       ARR_SEGMENT
                |       ARR_SEGMENT_LIST
                        HPAREXPSEPARATOR
                        ARR_SEGMENT
                ;
ARR_SEGMENT	:       ARRAY_SEGMENT
                        HBEGPAR
                        BAUND_PAIR_LIST HENDPAR { mout(MARRAY);
                                                  mout(MENDARRAY);
                                                  set_array_dim($3);}
                ;
ARRAY_SEGMENT   :       ARRAY_SEGMENT_EL        { mout(MENDSEP);
                                                  mout(MARRAYSEP);}

                |       ARRAY_SEGMENT_EL
                        HPAREXPSEPARATOR 
                        ARRAY_SEGMENT           { mout(MARRAYSEP);}
                ;
ARRAY_SEGMENT_EL:       HIDENTIFIER             { mout(MIDENTIFIER);
                                                  mout_id($1);
                                                  reg_decl($1, type, kind, categ);
				   if(last_array==NULL)
                                     last_array=cblock->lastparloc;}
                ;
BAUND_PAIR_LIST :       BAUND_PAIR              { mout(MENDSEP);
                                                  mout(MBOUNDSEP);
                                                  $$=1;}
                |       BAUND_PAIR
                        HPAREXPSEPARATOR
                        BAUND_PAIR_LIST         { mout(MBOUNDSEP);
                                                  $$=$3+1;}
                ;
BAUND_PAIR      :       EXPRESSION
                        HLABELSEPARATOR
                        EXPRESSION              { mout(MBOUNDPARSEP);}
                ;
SWITCH_LIST     :       EXPRESSION              { mout(MENDSEP);
                                                  mout(MSWITCHSEP);}
                |       EXPRESSION
                        HPAREXPSEPARATOR
                        SWITCH_LIST             { mout(MSWITCHSEP);}
                ;
HEADING         :       MBEE_FMAL_PAR_P HSTATEMENTSEPARATOR { kind=KNOKD;}
                        MBEE_MODE_PART  { categ=CSPEC;}
                        MBEE_SPEC_PART  { kind=KNOKD;}
                        MBEE_PROT_PART  { categ=CVIRT;}
                        MBEE_VIRT_PART
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
LISTV           :       HIDENTIFIER     { reg_decl($1, type, KNOKD, CDEFLT);}
                |	FPP_CATEG HDOTDOTDOT      { reg_decl(varargsid, TVARARGS, KNOKD, categ);}
                |       HIDENTIFIER     { reg_decl($1, type, KNOKD, CDEFLT);}
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
FPP_LISTV       :	FPP_CATEG HDOTDOTDOT      { reg_decl(varargsid, TVARARGS, KNOKD, categ);}
                |       FPP_SPEC
                |       FPP_SPEC
                        HPAREXPSEPARATOR LISTV
                ;
FPP_SPEC        :       FPP_CATEG SPECIFIER HIDENTIFIER     
                                       { reg_decl($3, type, kind, categ);}
		|	FPP_CATEG FPP_PROC_DECL_IN_SPEC
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
                                	{ $<ival>$=categ;
                                          reg_decl($3, type, KPROC, categ);
                                          begin_block(KPROC);}
                        FPP_HEADING
					{ categ=$<ival>4; /* M} settes tilbake*/}
				{ end_block(NULL,CCNO);}
		;
IDENTIFIER_LISTV:       HIDENTIFIER     { reg_decl($1, type, kind, categ);}
                |	HDOTDOTDOT {	  reg_decl(varargsid, TVARARGS, kind, categ);}
                |       HIDENTIFIER     { reg_decl($1, type, kind, categ);}
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
			  { if($4!=HIS) yerror (8);} 
			PROC_DECL_IN_SPEC HSTATEMENTSEPARATOR
                |       FPP_PROC_DECL_IN_SPEC HSTATEMENTSEPARATOR
                |       MBEE_TYPE HPROCEDURE HIDENTIFIER HSTATEMENTSEPARATOR
					{ yerror (45);}
                |       MBEE_TYPE HPROCEDURE HIDENTIFIER HPAREXPSEPARATOR
                        IDENTIFIER_LIST HSTATEMENTSEPARATOR
					{ yerror (45);}
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
                                	{ $<ival>$=categ;
                                          reg_decl($3, type, KPROC, categ);
                                          begin_block(KPROC);}
                        HEADING
					{ categ=$<ival>4; /* M} settes tilbake*/}
			MBEE_BEGIN_END 
				{ end_block(NULL,CCNO);}
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
IDENTIFIER_LIST :       HIDENTIFIER     { reg_decl($1, type, kind, categ);}
                |       IDENTIFIER_LIST HPAREXPSEPARATOR
                        HIDENTIFIER     { reg_decl($3, type, kind, categ);}
                ;
IDENTIFIER_LISTC:       HIDENTIFIER 
			MBEE_CONSTANT   { reg_decl($1, type, kind, categ);
					  categ=CLOCAL;}
                |       IDENTIFIER_LISTC HPAREXPSEPARATOR
                        HIDENTIFIER 
			MBEE_CONSTANT   { reg_decl($3, type, kind, categ);
					  categ=CLOCAL;}
                ;
MBEE_CONSTANT	:	/* EMPTY */
		|	HVALRELOPERATOR
				{ MBEENEWBLOCK();
			    if($1!=HEQ) yerror (8);
					  if(type==TREF)yerror (7);
					  categ=CCONSTU;
						  mout(MIDENTIFIER);
						  mout_id($<token>0);} 
			EXPRESSION		{ mout(MASSIGN);
						  mout(MCONST);}
		;

/* GRAMATIKK FOR UTTRYKK */
EXPRESSION      :       EXPRESSION_SIMP {}
                |       HIF
                        EXPRESSION
                        HTHEN
                        EXPRESSION
                        HELSE
                        EXPRESSION              { mout(MELSEE);
                                                  mout(MIFE);}
                ;
EXPRESSION_SIMP :	EXPRESSION_SIMP
			HASSIGN
			EXPRESSION { if($2==HASSIGNREF)mout(MASSIGNR);
                                          else    mout(MASSIGN);$$=NULL;}
		|

			EXPRESSION_SIMP
                        HCONC
                        EXPRESSION_SIMP         { mout(MCONC);$$=NULL;}
                |       EXPRESSION_SIMP HOR
                        HELSE
                        EXPRESSION_SIMP
                        %prec HORELSE           { mout(MORELSEE);$$=NULL;}
                |       EXPRESSION_SIMP HAND
                        HTHEN
                        EXPRESSION_SIMP
                        %prec HANDTHEN          { mout(MANDTHENE);$$=NULL;}
                |       EXPRESSION_SIMP
                        HEQV EXPRESSION_SIMP    { mout(MEQV);$$=NULL;}
                |       EXPRESSION_SIMP
                        HIMP EXPRESSION_SIMP    { mout(MIMP);$$=NULL;}
                |       EXPRESSION_SIMP
                        HOR EXPRESSION_SIMP     { mout(MOR);$$=NULL;}
                |       EXPRESSION_SIMP
                        HAND EXPRESSION_SIMP    { mout(MAND);$$=NULL;}
                |       HNOT EXPRESSION_SIMP    { mout(MNOT);$$=NULL;}
                |       EXPRESSION_SIMP
                        HVALRELOPERATOR
                        EXPRESSION_SIMP
                                { switch($2)
                                    {   case HEQ: mout(MEQ);break;
                                        case HNE: mout(MNE);break;
                                        case HLT: mout(MLT);break;
                                        case HLE: mout(MLE);break;
                                        case HGT: mout(MGT);break;
                                        case HGE: mout(MGE);break;
                                    }$$=NULL;}
                |       EXPRESSION_SIMP
                        HREFRELOPERATOR
                        EXPRESSION_SIMP
                                { if($2==HNER)    mout(MNER);
                                        else      mout(MEQR);$$=NULL;}
                |       EXPRESSION_SIMP
                        HOBJRELOPERATOR
                        EXPRESSION_SIMP
                                { if($2==HIS)     mout(MIS);
                                        else      mout(MINS);$$=NULL;}
                |       HTERMOPERATOR
                        EXPRESSION_SIMP %prec UNEAR
                                { if($1==HADD)    mout(MUADD);
                                        else      mout(MUSUB);$$=NULL;}
                |       EXPRESSION_SIMP
                        HTERMOPERATOR
                        EXPRESSION_SIMP
                                { if($2==HADD)    mout(MADD);
                                        else      mout(MSUB);$$=NULL;}
                |       EXPRESSION_SIMP
                        HFACTOROPERATOR
                        EXPRESSION_SIMP         
                                { if($2==HMUL)    mout(MMUL); else
                                  if($2==HDIV)    mout(MDIV);
                                        else      mout(MINTDIV);$$=NULL;}
                |       EXPRESSION_SIMP
                        HPRIMARYOPERATOR
                        EXPRESSION_SIMP         { mout(MPRIMARY);$$=NULL;}
                |       HBEGPAR
                        EXPRESSION HENDPAR      { mout(MNOOP);$$=NULL;}
                |       HTEXTKONST              { mout(MTEXTKONST);
                                                  mout_tval($1);$$=NULL;}
                |       HCHARACTERKONST         { mout(MCHARACTERKONST);
                                                  mout_ival($1);$$=NULL;}
                |       HREALKONST              { mout(MREALKONST);
                                                  mout_rval($1);$$=NULL;}
                |       HINTEGERKONST           { mout(MINTEGERKONST);
                                                  mout_ival($1);$$=NULL;}
                |       HBOOLEANKONST           { mout(MBOOLEANKONST);
                                                  mout_ival($1);$$=NULL;}
                |       HNONE                   { mout(MNONE);$$=NULL;}
                |       HIDENTIFIER
                                { $<ident>$=$1;}
                        MBEE_ARG_R_PT
                |       HTHIS HIDENTIFIER       { mout(MTHIS);
                                                  mout_id($2);$$=NULL;}
                |       HNEW
                        HIDENTIFIER
                        ARG_R_PT                { mout(MNEWARG);
                                                  mout_id($2);$$=NULL;}
                |       EXPRESSION_SIMP
                        HDOT
                        EXPRESSION_SIMP         { mout(MDOT);$$=NULL;}
                |       EXPRESSION_SIMP
                        HQUA HIDENTIFIER        { mout(MQUA);
                                                  mout_id($3);$$=NULL;}
                ;
ARG_R_PT        :       /*EMPTY*/               { mout(MENDSEP);}
                |       HBEGPAR
                        ARGUMENT_LIST HENDPAR
                ;
MBEE_ARG_R_PT   :       /*EMPTY*/               { mout(MIDENTIFIER);
                                                  mout_id($<ident>0);
						  $$=$<ident>0;}
                |       HBEGPAR
                        ARGUMENT_LIST HENDPAR   { mout(MARGUMENT);
                                                  mout_id($<ident>0);}
                ;
ARGUMENT_LIST   :       EXPRESSION              { mout(MENDSEP);
                                                  mout(MARGUMENTSEP);}
                |       EXPRESSION
                        HPAREXPSEPARATOR
                        ARGUMENT_LIST           { mout(MARGUMENTSEP);}
                ;
%%
/******************************************************************************
                                                                YYERROR      */

/* Oppdages feil så blir denne procedyren kalt */

void yyerror (char s[])
  {
     yaccerror=TRUE;
#if 0
     if(s[0]=='s')yerror (13);else
     if(s[0]=='y')yerror (14);else
       yerror (16);
#else
     yerror (21,s);
#endif
     mout(MERROR);
  }

/******************************************************************************
                                                                YLEX         */
                                                                             
#ifdef yylex
#undef yylex
int ylex(void)
{
  long i;
  i=yylex();
#ifdef DEBUG
  if(option_lex)
    {
      printf("line: %ld yylex:",lineno);
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

void parser_init (void)
{
  obstack_init (&os_block);
  first_object_allocated_ptr_block= obstack_alloc (&os_block, 0);
}

void parser_init_pass1 (void)
{
  activateid=tag("activat");
  varargsid=tag("...");
  Ckind=tag("C");
  simsetident=tag("SIMSET");
  simulationident=tag("SIMULATION");
  fileident=tag("FILE");
  imagefileident=tag("IMAGEFILE");
  outfileident=tag("OUTFILE");
  infileident=tag("INFILE");
  directfileident=tag("DIRECTFILE");
  printfileident=tag("PRINTFILE");
  bytefileident=tag("BYTEFILE");
  inbytefileident=tag("INBYTEFILE");
  outbytefileident=tag("OUTBYTEFILE");
  directbytefileident=tag("DIRECTBYTEFILE");
  blockp= (blockstack_t *)
    obstack_alloc (&os_block,sizeof (blockstack_t));
  blockp->prev= NULL;
  blockp->rem=FALSE;
}

void parser_reinit (void)
{
  obstack_free (&os_block, first_object_allocated_ptr_block);
}
