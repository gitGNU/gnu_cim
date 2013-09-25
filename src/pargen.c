/* $Id: cgenpar.c,v 1.24 1994/11/03 08:41:00 cim Exp $ */

/* Copyright (C) 1994, 1998 Sverre Hvammen Johansen and Terje Mjøs,
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

#include "gen.h"
#include "extspec.h"

#define ADDNOTH 0

/******************************************************************************
                                                              GEN_CONV_AND_Q */

static void gen_conv_and_q (exp_t *rex, char procedure, char transported, char copied_all)
{
  char writetest = TRUE;
  if ((rex->rd->prefqual == rex->left->qual) ||
      (procedure && subclass (rex->left->qual, rex->rd->prefqual)))
    {
      /* AKTUELL OG FORMELL HAR SAMME KVALIFIKASJON
       * eller FOR PROSEDYRE AKTUELL HAR LIK QUAL ELLER ER EN
       * SUBKLASSE AV FORMELL QUAL.
       * FP.CONV = AP.CONV -- FP.Q = AP.Q */
      writetest = FALSE;
      goto nextcase;
    }
  else
    /* END-LIK AKTUELL OG FORMELL KVALIFIKASJON */ 
    if (subclass (rex->left->qual, rex->rd->prefqual) && !procedure)
    {
      /* AKTUELL kval. er en subklasse av FORMELL kval. 
      * FP.CONV = AP.CONV || writetest -- FP.Q = AP.Q */
    nextcase:
      if (!transported || !copied_all || writetest)
	fprintf (ccode, "((__bs%d *)__pb)->%s.conv",
			rex->rd->encl->blno, rex->rd->ident);
      if (transported)
	{
	  if (copied_all)
	    if (writetest)
	      fprintf (ccode, "|= __WRITETEST;",
			      rex->left->value.ident);
	    else;
	  else
	    {
	      gensl (rex->left, TRUE, ON);
	      if (writetest)
		fprintf (ccode, "=%s.conv | __WRITETEST;",
				rex->left->value.ident);
	      else
		fprintf (ccode, "=%s.conv;", rex->left->value.ident);
	    }
	}
      else if (writetest)
	fprintf (ccode, "=__WRITETEST;");
      else
	fprintf (ccode, "=__NOTEST;");

      fprintf (ccode, "((__bs%d *)__pb)->%s.q=", rex->rd->encl->blno,
		      rex->rd->ident);

      if (transported)
	{
	  gensl (rex->left, TRUE, ON);
	  fprintf (ccode, "%s.q", rex->left->value.ident);
	}
      else
	gen_adr_prot (ccode, rex->left->qual);
      fprintf (ccode, ";");
    }
  else
    /* END-AKTUELL KVAL. EN SUBKLASSE AV FORMELL KVAL. */ 
    if (subclass (rex->rd->prefqual, rex->left->qual))
    {
      if (transported)
	{
	  /* FORMELL kval. er en subklasse av AKTUELL kval.
	   * if(FORMELL kval. sub AP.kval)                       
	   * {                                                   
	   *    FP.CONV=readtest;FP.Q=FORMELL kval.              
	   * }else                                               
	   * if(AP.kval sub FORMELL kval.)                       
	   * {                                                   
	   *    FP.CONV=AP.CONV;FP.Q=AP.Q                        
	   *  }else                                              
	   * __rerror();                                         
           *
	   * rrin() er en runtime som utf|rer en in test         
	   * Den skal ha to prototype pekerer som parametere     
	   * i motsetning til rin() som skal ha en objektpeker   
	   * og en prototype peker                               
	   * Tester alts} om par1 in par2 */

	  fprintf (ccode, "if(__rrin(");
	  gen_adr_prot (ccode, rex->rd->prefqual);
	  fprintf (ccode, ",");

	  gensl (rex->left, TRUE, ON);
	  fprintf (ccode, "%s.q)){", rex->left->value.ident);
	}
      fprintf (ccode, "((__bs%d *)__pb)->%s.conv=__READTEST;"
	       "((__bs%d *)__pb)->%s.q= ",
	       rex->rd->encl->blno, rex->rd->ident,
	       rex->rd->encl->blno, rex->rd->ident);
      gen_adr_prot (ccode, rex->rd->prefqual);
      fprintf (ccode, ";");

      if (transported)
	{
	  fprintf (ccode, "}else if(__rrin(");
	  gensl (rex->left, TRUE, ON);
	  fprintf (ccode, "%s.q,",
			  rex->left->value.ident);
	  gen_adr_prot (ccode, rex->rd->prefqual);
	  fprintf (ccode, ")){");
	  if (!copied_all)
	    {
	      fprintf (ccode, "((__bs%d *)__pb)->%s.conv=",
			      rex->rd->encl->blno, rex->rd->ident);
	      gensl (rex->left, TRUE, ON);
	      fprintf (ccode, "%s.conv;((__bs%d *)__pb)->%s.q=", 
		       rex->left->value.ident,
		       rex->rd->encl->blno, rex->rd->ident);
	      gensl (rex->left, TRUE, ON);
	      fprintf (ccode, "%s.q;", rex->left->value.ident);
	    }
	  fprintf (ccode, "}else __rerror(__errqual);");
	}
    }
/*** END-FORMELL KVAL. ER EN SUBKLASSE AV AKTUELL KVAL.            ****/
}

/******************************************************************************
                                                               GEN_ARIT_CONV */

static void gen_arit_conv (exp_t *rex, char transported, char copied_all)
{
  if (transported != copied_all || rex->left->type != rex->rd->type)
    {
      fprintf (ccode, "((__bs%d *)__pb)->%s.conv=",
		      rex->rd->encl->blno, rex->rd->ident);

      if (transported)
	fprintf (ccode, "__ctab[");

      if (rex->left->type == TINTG && rex->rd->type == TREAL)
	fprintf (ccode, "__INTREAL");
      else if (rex->left->type == TREAL && rex->rd->type == TINTG)
	fprintf (ccode, "__REALINT");
      else
	fprintf (ccode, "__NOCONV");

      if (transported)
	{
	  fprintf (ccode, "][");
	  gensl (rex->left, TRUE, ON);
	  fprintf (ccode, "%s.conv]", rex->left->value.ident);
	}
      fprintf (ccode, ";");
    }
}

/******************************************************************************
                                                                    GEN_CONV */

static void gen_conv (exp_t *rex, char procedure, char copied_all)
{
  if (rex->rd->categ == CVAR || rex->rd->categ == CNAME	)
    {
      if (rex->left->token == MIDENTIFIER &&
	  (rex->left->rd->categ == CVAR || rex->left->rd->categ == CNAME))
	{
	  if (rex->rd->type == TINTG || rex->rd->type == TREAL)
	    gen_arit_conv (rex, TRUE, copied_all);
	  else if (rex->rd->type == TREF)
	    gen_conv_and_q (rex, procedure, TRUE, copied_all);
	}
      else
	{
	  if (rex->rd->type == TINTG || rex->rd->type == TREAL)
	    gen_arit_conv (rex, FALSE, copied_all);
	  else if (rex->rd->type == TREF)
	    gen_conv_and_q (rex, procedure, FALSE, copied_all);
	}
    }
}

/******************************************************************************
                                                       SEND_TO_FORMAL_PAR    */

/* Overf|rer namekind, thunk adressen, statisk link, og evt. __conv og q
 * til den formelle nameparameter structen ved generering av thunker for
 * den aktuelle parameteren. */

static void send_to_formal_par (exp_t *rex, char addressthunk)
{
  /* Hvis hdot = FALSE er denne rutinen kalt for en label     eller array
   * name. Structen for disse har ikke et .h felt som finnes i structene for
   * enkle name-parametere. */

  if (addressthunk)
    fprintf (ccode, "((__bs%d *)__pb)->%s.namekind=__ADDRESS_THUNK;",
		    rex->rd->encl->blno, rex->rd->ident);
  else
    fprintf (ccode, "((__bs%d *)__pb)->%s.namekind=__VALUE_THUNK;",
		    rex->rd->encl->blno, rex->rd->ident);

/***** OVERF\RER THUNKENS ADRESSE OG THUNKENS STATISKE OMGIVELSE  ****/
  fprintf (ccode, "((__bs%d *)__pb)->%s.adr.ment=",
		    rex->rd->encl->blno, rex->rd->ident);
  genmodulemark(NULL);

  fprintf (ccode, ";((__bs%d *)__pb)->%s.adr.ent=%d;"
	   "((__bs%d *)__pb)->%s.sl=__lb",
	   rex->rd->encl->blno, rex->rd->ident, rex->value.thunk.label,
	   rex->rd->encl->blno, rex->rd->ident);
  if (inthunk)
    fprintf (ccode, "->sl");
  fprintf (ccode, ";");
  if (rex->rd->kind != KARRAY)
    gen_conv (rex, FALSE, FALSE);
}

/******************************************************************************
                                                    GEN_THUNK_SIMPLE_ADDRESS */

/* Genererer kode som for ADDRESS_THUNK avgj|r om thunken skal returnere
 * med en adresse eller en verdi. Dersom en verdi skal returners
 * genereres det ogs} kode som utf|rer evt. konverteringer og 
 * kvalifikasjonstester */

void gen_thunk_simple_address (exp_t *rex)
{
  switch (rex->left->token)
    {
    case MARRAYARG:
    case MARRAYADR:
      fprintf (ccode, "__er=");
      gen_ref_stack (rex->left->value.stack.ref_entry);
      fprintf (ccode, ";__ev.i=");
      gen_int_stack (rex->left->value.stack.val_entry);
      fprintf (ccode, ";");
      break;
    default:
      fprintf (ccode, "__er=");
      if (nonetest == ON)
	fprintf (ccode, "((__bp=");
      genvalue (rex->left->left);
      if (nonetest == ON)
	fprintf (ccode,
			")==__NULL?(__dhp)__rerror(__errnone):__bp)");
      fprintf 
	(ccode, ";__ev.i=((char *)&((__bs%d *)__p)->%s) - (char *)__p;",
	 rex->left->right->rd->encl->blno,
	 rex->left->right->rd->ident);
      break;
    }
  if (rex->rd->type == TBOOL || rex->rd->type == TCHAR)
    /* Leser verdien hvis det ikke er skrive-aksess. */
    fprintf (ccode, "if(!((__thunkp)__pb)->writeaccess)"
	     "__ev.c= *(char *)(((char *)__er)+__ev.i);");
  else if (rex->rd->type == TINTG || rex->rd->type == TREAL)
    {				/* Leser og konverterer verdien hvis det ikke 
				 * er skrive aksess. */
      fprintf (ccode, "if(!((__thunkp)__pb)->writeaccess)"
	       "   if(((__thunkp)__pb)->ftype==__TINTG)");
      if (rex->left->type == TINTG)
	fprintf (ccode, "__ev.i= *(long *)(((char *)__er)+__ev.i);");
      else
	fprintf (ccode, "__ev.i= *(double *)(((char *)__er)+__ev.i);");
      fprintf (ccode, "   else ");
      if (rex->left->type == TINTG)
	fprintf (ccode, "__ev.f= *(long *)(((char *)__er)+__ev.i);");
      else
	  fprintf (ccode, "__ev.f=(((__thunkp)__pb)->conv==__REALINTREAL)?"
		   "__rintrea(*(double *)(((char *)__er)+__ev.i)):"
		   "*(double *)(((char *)__er)+__ev.i);");
    }
  if (rex->rd->type == TTEXT)
    /* Leser verdien hvis det ikke er skrive-aksess. */
    fprintf (ccode, "if(!((__thunkp)__pb)->writeaccess)"
	     "__et= *(__txt *)(((char *)__er)+__ev.i);");
  else if (rex->rd->type == TREF)
    fprintf (ccode, "if(!((__thunkp)__pb)->writeaccess)"
	     "   if((((__thunkp)__pb)->conv==__READTEST ||"
	     " ((__thunkp)__pb)->conv==__READWRITETEST) &&"
	     " !__rin(*(__dhp *)(((char *)__er)+__ev.i),"
	     "((__thunkp)__pb)->q))__rerror(__errqual);"
	     "else __er= *(__dhp *)(((char *)__er)+__ev.i);");
  fprintf (ccode, "__reth();");
}

/******************************************************************************
                                                    GEN_THUNK_SIMPLE_VALUE   */

void gen_thunk_simple_value (exp_t *rex)
{
  switch (rex->left->type)
    {
    case TINTG:
      fprintf (ccode, "__ev.i=");
      break;
    case TREAL:
      fprintf (ccode, "__ev.f=");
      break;
    case TBOOL:
    case TCHAR:
      fprintf (ccode, "__ev.c=");
      break;
    case TREF:
      fprintf (ccode, "__er=");
      break;
    case TTEXT:
      fprintf (ccode, "__et= *");
      break;
    }

  genvalue (rex->left);
  fprintf (ccode, ";");

  /* KONVERTERING OG KVAL. TESTER */
  if (rex->rd->type == TINTG || rex->rd->type == TREAL)
    {				/* Leser og konverterer verdien hvis det ikke 
				 * er skrive aksess. */
      fprintf (ccode, "if(((__thunkp)__pb)->ftype==__TINTG)");
      if (rex->left->type == TINTG)
	fprintf (ccode, "/*OK*/;");
      else
	fprintf (ccode, "__ev.i=__ev.f;");
      fprintf (ccode, "else ");
      if (rex->left->type == TINTG)
	fprintf (ccode, "__ev.f=__ev.i;");
      else
	fprintf 
	  (ccode, 
	   "if(((__thunkp)__pb)->conv==__REALINTREAL)__ev.f=__rintrea(__ev.f);");
    }
  else if (rex->rd->type == TREF)
    fprintf (ccode, "if((((__thunkp)__pb)->conv==__READTEST ||"
	     " ((__thunkp)__pb)->conv==__READWRITETEST) &&"
	     " !__rin(__er,((__thunkp)__pb)->q))"
	     "__rerror(__errqual);");
  fprintf (ccode, "__reth();");
}

/******************************************************************************
                                                       GENSIMPLEPAR          */

static void gensimplepar (exp_t *rex)
{
  int i;
/***** ENKEL INTEGER, REAL, CHAR, REF,TEXT ELLER BOOL  PARAMETER     ****/
  exp_t *re;
  char index_is_const = TRUE;

  if (rex->rd->categ == CVALUE && rex->rd->type == TTEXT)
    {
      /* T E X T  V A L U E  P A R A M E T E R */
      fprintf (ccode, "((__bs%d *)__pb)->%s= *__rcopy(%ldL,",
	       rex->rd->encl->blno, rex->rd->ident,
	       ant_stack (rex, rex->left, 1, 1, 1)); /* TBD Skal ikke dette gjøres anderledes, hvordan skal det i tilfelle gjøres */
      genvalue (rex->left);
      fprintf (ccode, ");");
    }
  else if (rex->rd->categ == CDEFLT)
    {
      /* S T A N D A R D   O V E R F \ R I N G */

      if (rex->rd->type == TTEXT)
	{
	  fprintf 
	    (ccode, "((__bs%d *)__pb)->%s= *", rex->rd->encl->blno,
	     rex->rd->ident);
	  genvalue (rex->left);
	  fprintf (ccode, ";");
	}
      else
	{
	  fprintf (ccode, "((__bs%d *)__pb)->%s=", rex->rd->encl->blno,
			  rex->rd->ident);
	  genvalue (rex->left);
	  fprintf (ccode, ";");
	}
    }
  else if (rex->rd->categ == CVAR)
    {
      /* V A R  P A R A M E T E R */

      if (rex->left->rd->categ == CVAR)
	{
	  /* AKTUELL PARAMETER ER EN FORMELL VAR  PARAMETER I EN YTRE
	   * PROSEDYRE. VIDEREF\RING AV EN ENKEL VAR PARAMETER. */

	  /* Tilordner bp */
	  fprintf (ccode, "((__bs%d *)__pb)->%s=",
			  rex->rd->encl->blno, rex->rd->ident);
	  gensl (rex->left, TRUE, ON);
	  fprintf (ccode, "%s;", rex->left->value.ident);
	  gen_conv (rex, FALSE, TRUE);
	} /* END VIDEREFRING AV ENKEL VAR-PARAMETER */ 
      else if (rex->left->rd->categ == CNAME)
	{
	  /* Aktuell parameter er en formell NAME-par i 
	   * en ytre prosedyre. Kallet p} transcall som
	   * legger ut kode for kall p} __rgetsa. Den
	   * returnerer adressen til variabelen i er og 
	   * ev. */
	  fprintf (ccode, "((__bs%d *)__pb)->%s.bp=__er;"
		   "((__bs%d *)__pb)->%s.ofs=__ev.i;", 
		   rex->rd->encl->blno, rex->rd->ident,
		   rex->rd->encl->blno, rex->rd->ident);
	  gen_conv (rex, FALSE, FALSE);
	}
      else
	{
	  /* ENKEL VAR PARAMETER, IKKE VIDEREF\RING  Tilordner bp */
	  fprintf (ccode, "((__bs%d *)__pb)->%s.bp=", 
		   rex->rd->encl->blno, rex->rd->ident);

	  switch (rex->left->token)
	    {
	    case MDOT:
	      if (nonetest == ON)
		fprintf (ccode, "((__bp=");
	      genvalue (rex->left->left);
	      if (nonetest == ON)
		fprintf (ccode,
			      ")==__NULL?(__dhp)__rerror(__errnone):__bp)");
	      break;
	    case MARRAYADR:
	      /* Peker til array ligger p} stakken */
	      gen_ref_stack (rex->left->value.stack.ref_entry);
	      break;
	    case MIDENTIFIER:
	      gensl (rex->left, FALSE, ON);
	      break;
	    }
	  fprintf (ccode, ";((__bs%d *)__pb)->%s.ofs=", 
		   rex->rd->encl->blno,rex->rd->ident);

	  if (rex->left->token == MARRAYADR)
	    {
	      gen_int_stack (rex->left->value.stack.val_entry);
	      fprintf (ccode, ";");
	    }
	  else
	    fprintf (ccode, "((char *)&((__bs%d *)__p)->%s)"
		     "-(char *)__p;",
		     rex->left->rd->encl->blno, rex->left->rd->ident,
		     rex->rd->encl->blno, rex->rd->ident);
	  gen_conv (rex, FALSE, FALSE);
	}			/* END IKKE VIDEREF\RING AV ENKEL
				 * VAR-PARAMETER */
    }
  else
    /* END-if(rex->rd->categ == CVAR) */ if (rex->rd->categ == CNAME)
    {
      /* N A M E   P A R A M E T E R */
      switch (rex->left->token)
	{
	case MTEXTKONST:
	  /* VALUE NOTHUNK  Overf|rer peker til textvariabelen for konstanten 
	   */
	  fprintf (ccode, "((__bs%d *)__pb)->%s.tp=",
		   rex->rd->encl->blno, rex->rd->ident);
	  genvalue (rex->left);
	  /* namekind = VALUE_NOTHUNK */
	  fprintf (ccode, ";((__bs%d *)__pb)->%s.namekind=__VALUE_NOTHUNK;",
		   rex->rd->encl->blno, rex->rd->ident);
	  break;
	case MINTEGERKONST:
	case MREALKONST:
	case MCHARACTERKONST:
	case MBOOLEANKONST:
	case MNONE:
	  /* VALUE NOTHUNK Overf|rer verdien. */
	  fprintf (ccode, "((__bs%d *)__pb)->%s.v.",
		   rex->rd->encl->blno, rex->rd->ident);
	  switch (rex->rd->type)
	    {
	    case TINTG:
	      fprintf (ccode, "i=");
	      break;
	    case TREAL:
	      fprintf (ccode, "f=");
	      break;
	    case TBOOL:
	    case TCHAR:
	      fprintf (ccode, "c=");
	      break;
	    case TREF:
	      fprintf (ccode, "r=");
	      break;
	    default:;
	    }
	  genvalue (rex->left);
	  fprintf (ccode, ";");

	  /* namekind = VALUE_NOTHUNK */

	  fprintf (ccode, "((__bs%d *)__pb)->%s.namekind=__VALUE_NOTHUNK;",
		   rex->rd->encl->blno, rex->rd->ident);

	  if (rex->rd->type == TREAL || rex->rd->type == TINTG)
	    /* __conv = NOCONV */
	    fprintf (ccode, "((__bs%d *)__pb)->%s.conv=__NOCONV;",
		     rex->rd->encl->blno, rex->rd->ident);
	  else if (rex->rd->type == TREF)
	    /* __conv = notest */
	    fprintf (ccode, "((__bs%d *)__pb)->%s.conv=__NOTEST;",
		     rex->rd->encl->blno, rex->rd->ident);
	  break;
	case MIDENTIFIER:
	  if (rex->left->rd->categ == CNAME)
	    {
	      /* AKTUELL PARAMETER ER EN FORMELL NAME-PARAMETER I EN YTRE
	       * PROSEDYRE. VIDERF\RING  Setter bp, en hjelpevariabel, til }
	       * peker p} den aktuelle parameterens blokk. Dermed blir
	       * aksessveien kortere under kopieringen. */

	      fprintf (ccode, "((__bs%d *)__pb)->%s=",
		       rex->rd->encl->blno, rex->rd->ident);
	      gensl (rex->left, TRUE, ON);
	      fprintf (ccode, "%s;", rex->left->value.ident);
	      gen_conv (rex, FALSE, TRUE);
	    }
	    /* END-VIDEREF\RING AV FORMELL NAME-PARAMETER I EN YTRE
	     * PROSEDYRE. */ 
	  else if (rex->left->rd->categ == CVAR)
	    {
	      /* AKTUELL PARAMETER ER EN FORMELL VAR-PARAMETER I EN YTRE
	       * PROSEDYRE. Setter bp, en hjelpevariabel, til } peker p} den
	       * aktuelle parameterens blokk. Dermed blir aksessveien kortere 
	       * under kopieringen. */

	      fprintf (ccode, "__bp=");
	      gensl (rex->left, FALSE, ON);

	      /* Tilordner den formelle name-parameterens bp og ofs */
	      fprintf (ccode, ";((__bs%d *)__pb)->%s.bp="
		"((__bs%d *)__bp)->%s.bp;((__bs%d *)__pb)->%s.v.ofs="
		"((__bs%d *)__bp)->%s.ofs;", 
		       rex->rd->encl->blno, rex->rd->ident,
		       rex->left->rd->encl->blno,
		       rex->left->value.ident,
		       rex->rd->encl->blno, rex->rd->ident,
		       rex->left->rd->encl->blno,
		       rex->left->value.ident);
#if ADDNOTH
	      fprintf (ccode, "((__bs%d *)__pb)->%s.namekind"
		       "=__ADDRESS_NOTHUNK;",
		       rex->rd->encl->blno, rex->rd->ident);
#endif
	      gen_conv (rex, FALSE, FALSE);
	    }
	    /* END-AKTUELL PAR ER EN FORMELL VAR-PAR. */
	  else
	    {
	      /* ADDRESS NOTHUNK Tilordner den formelle name-parameterens bp
	       * og ofs */
	      fprintf (ccode, "((__bs%d *)__pb)->%s.bp=",
			      rex->rd->encl->blno, rex->rd->ident);
	      gensl (rex->left, FALSE, OFF);
	      fprintf (ccode, ";((__bs%d *)__pb)->%s.v.ofs="
		       "((char *)&((__bs%d *)__p)->%s)-(char *)__p;",
		       rex->rd->encl->blno, rex->rd->ident,
		       rex->left->rd->encl->blno,
		       rex->left->rd->ident, rex->left->rd->encl->blno);
#if ADDNOTH
	      fprintf (ccode, "((__bs%d *)__pb)->%s.namekind"
		       "=__ADDRESS_NOTHUNK;",
		       rex->rd->encl->blno, rex->rd->ident);
#endif
	      gen_conv (rex, FALSE, FALSE);
	    }
	  break;
	case MARRAYADR:
	  /* ARRAY HVOR ALLE INDEKSENE BEST]R AV KONSTANTER  ADDRESS
	   * NOTHUNK  Tilordner den formelle name-parameterens bp og ofs */

	  fprintf (ccode, "((__bs%d *)__pb)->%s.bp=",
		   rex->rd->encl->blno, rex->rd->ident);
	  gen_ref_stack (rex->left->value.stack.ref_entry);
	  fprintf (ccode, ";((__bs%d *)__pb)->%s.v.ofs=",
		   rex->rd->encl->blno, rex->rd->ident);
	  gen_ref_stack (rex->left->value.stack.val_entry);
	  fprintf (ccode, ";");
#if ADDNOTH
	  fprintf (ccode, "((__bs%d *)__pb)->%s.namekind"
		   "=__ADDRESS_NOTHUNK;",
		   rex->rd->encl->blno, rex->rd->ident);
#endif
	  gen_conv (rex, FALSE, FALSE);
	  break;
	}			/* END SWITCH */
    }				/* END-if(rex->rd->categ == CNAME) */
  else				/* FEIL */
    ;
}				/* END GENSIMPLEPAR */

/******************************************************************************
                                                              GENLABELPAREXP */

static void genlabelparexp (exp_t *rex, exp_t *formellpar, char thunk)
{
  /* Denne rutinen kalles i forbindelse med } generere kode for  label
   * parameteroverf|ring hvor den aktuelle parameteren er et uttrykk (eks.
   * p(if a then l1 else l2)).Rutinen kalles istedenfor genvalue, og
   * genererer kode for et uttrykk av "if-i-uttrykk"-setninger som skal
   * gi labelens adresse og objekt-peker. Genvalue ville lagd kode
   * for } hoppe til labelen.
   * Parameteren rex peker til en node i uttrykks-treet (enten    
   * MIFE, MELSEE eller MIDENTIFIER) mens formellpar  peker p} noden for
   * den formelle parameteren. Hvis den formelle parameteren har
   * categ==CNAME, skal det genereres en thunk. Parameteren exit er
   * labelen etter hele uttrykket. Hvis det skal genereres en thunk (dvs,
   * formellpar->rd->categ==CNAME) legges det ikke ned hopp til denne
   * labelen siden kall p} RT-rutinen reth() avslutter hver gren. */

  if (rex->token == MIFE)
    {
      fprintf (ccode, "if(");
      genvalue (rex->left);
      fprintf (ccode, "){");
      genlabelparexp (rex->right->left, formellpar, thunk);
      fprintf (ccode, "}else{");
      genlabelparexp (rex->right->right, formellpar, thunk);
      fprintf (ccode, "}");
    }
  else
    {				/* rex->token==MIDENTIFIER Hvis det ikke er
				 * tatt av en label i systemet, s} gj|res det 
				 * her, og den legges i plev attributtet */

      if (rex->token == MARRAYARG)
	{
	  fprintf (ccode, "__swv=");
	  genvalue (rex->right->left);
	  fprintf (ccode, ";");
	}

      if (rex->rd->plev == 0)
	rex->rd->plev = newlabel ();

      if (thunk)
	fprintf (ccode, "__ev.adr.ent=");
      else
	fprintf (ccode, "((__bs%d *)__pb)->%s.adr.ent=",
			formellpar->rd->encl->blno, formellpar->rd->ident);
      fprintf (ccode, "%d;", rex->rd->plev);

      if (thunk)
	fprintf (ccode, "__ev.adr.ment=");
      else
	fprintf (ccode, "((__bs%d *)__pb)->%s.adr.ment=",
			formellpar->rd->encl->blno, formellpar->rd->ident);
      genmodulemark(NULL);

      if (thunk)
	fprintf (ccode, ";__er=");
      else
	fprintf (ccode, ";((__bs%d *)__pb)->%s.ob=", 
		 formellpar->rd->encl->blno, formellpar->rd->ident);

      gensl (rex, FALSE, ON);
      fprintf (ccode, ";");
    }
}

/******************************************************************************
                                                             GEN_THUNK_LABLE */

void gen_thunk_lable (exp_t *rex)
{
  /* genlabelparexp skriver ut uttrykket, og tilordner ment, ent og ob for
   * hver gren i uttrykket. (if-i-uttrykk) Den skriver ogs} ut kallet for
   * reth() til slutt */
  genlabelparexp (rex->left, rex, TRUE);
  fprintf (ccode, "__reth();");
}

/******************************************************************************
                                                           GENLABELSWITCHPAR */

static void genlabelswitchpar (exp_t *rex)
{
  int i;

  if (rex->left->token == MIDENTIFIER)
    {
      switch (rex->left->rd->categ)
	{
	case CNAME:
	  if (rex->rd->kind != KARRAY && rex->rd->categ != CNAME)
	    {
	      /* Label par og ikke switch par. Aktuell parameter er en name 
	       * parameter i en ytre prosedyre.       M} kalle p} transcall
	       * som genererer kode for kall p} __rgetlab() . og som
	       * returnerer med adressen i modul og ev, og objekt peker i
	       * er. */
	      fprintf (ccode, "((__bs%d *)__pb)->%s.adr=__ev.adr;"
		       "((__bs%d *)__pb)->%s.ob=__er;",
		       rex->rd->encl->blno, rex->rd->ident,
		       rex->rd->encl->blno, rex->rd->ident);
	      break;
	    }
	  goto other;
	case CDEFLT:
	case CVAR:
	  if (rex->rd->kind != KARRAY && rex->rd->categ == CNAME)
	    {
	      /* Viderf|ring av en label parameter  Kopierer aktuell
	       * parameter spesifikasjon som er en formell parameter
	       * spesifikasjon i ytre en prosedyre. (ment, ent ,ob,( sl og
	       * namekind i tillegg for NAME) Setter bp, en hjelpevariabel,
	       * til } peker p} den   aktuelle parameterens blokk. Dermed
	       * blir aksessveien kortere under kopieringen. */
	      fprintf (ccode, "__bp=");
	      gensl (rex->left, FALSE, ON);
	      fprintf (ccode, ";((__bs%d *)__pb)->%s.adr="
		       "((__bs%d *)__bp)->%s.adr;"
		       "((__bs%d *)__pb)->%s.ob=((__bs%d *)__bp)->%s.ob;",
		       rex->rd->encl->blno, rex->rd->ident,
		       rex->left->rd->encl->blno, rex->left->value.ident,
		       rex->rd->encl->blno, rex->rd->ident,
		       rex->left->rd->encl->blno, rex->left->value.ident);
#if ADDNOTH
	      fprintf (ccode, "((__bs%d *)__pb)->%s.namekind"
		       "=__ADDRESS_NOTHUNK;",
		       rex->rd->encl->blno, rex->rd->ident);
#endif
	      break;
	    }
	other:
	  /* VIDEREF\RING AV FORMELL CDEFLT ELLER CVAR (eller NAME for
	   * switch) I EN YTRE PROSEDYRE  KOPIERER ment, ent og ob. Setter
	   * bp, en hjelpevariabel, til } peker p} den  aktuelle
	   * parameterens blokk. Dermed blir aksessveien under kopieringen. 
	   */
	  
	  fprintf (ccode, "((__bs%d *)__pb)->%s=",
		   rex->rd->encl->blno, rex->rd->ident);
	  gensl (rex->left, TRUE, ON);
	  fprintf (ccode, "%s;", rex->left->value.ident);
	  break;
	case CVIRT:
	  fprintf (ccode, "((__bs%d *)__pb)->%s.ob=",
		   rex->rd->encl->blno, rex->rd->ident);
	  gensl (rex->left, FALSE, ON);
	  /* ment og ent er gitt av virt tabellen */
	  fprintf (ccode, ";((__bs%d *)__pb)->%s.adr=" 
		   "((__bs%d *)__pb)->%s.ob->pp->virtlab[%d];",
		   rex->rd->encl->blno, rex->rd->ident,
		   rex->rd->encl->blno, rex->rd->ident,
		   rex->left->rd->virtno - 1);
#if ADDNOTH
	  if (rex->rd->kind != KARRAY && rex->rd->categ == CNAME)
	    fprintf (ccode, "((__bs%d *)__pb)->%s.namekind=__ADDRESS_NOTHUNK;",
		     rex->rd->encl->blno, rex->rd->ident);
#endif
	  break;
	case CLOCAL:
	  fprintf (ccode, "((__bs%d *)__pb)->%s.adr.ment=",
		   rex->rd->encl->blno, rex->rd->ident);
	  
	  /* Bestemmer modulnavnet */
	  
	  genmodulemark(rex->left->rd->encl->timestamp);
	  fprintf (ccode, ";");
	  
	  /* Hvis det ikke er tatt av en label i systemet, s} gj|res det
	   * her, og den legges i plev attributtet */
	  
	  if (rex->left->rd->plev == 0)
	    rex->left->rd->plev = newlabel ();
	  
	  fprintf (ccode, "((__bs%d *)__pb)->%s.adr.ent=%d;"
		   "((__bs%d *)__pb)->%s.ob=",
		   rex->rd->encl->blno, rex->rd->ident,
		   rex->left->rd->plev,
		   rex->rd->encl->blno, rex->rd->ident);
	  gensl (rex->left, FALSE, ON);
	  fprintf (ccode, ";");
#if ADDNOTH
	  if (rex->rd->kind != KARRAY && rex->rd->categ == CNAME)
	    fprintf (ccode, "((__bs%d *)__pb)->%s.namekind=__ADDRESS_NOTHUNK;",
		     rex->rd->encl->blno, rex->rd->ident);
#endif
	  break;
	}
    }
  else
    /* FORMELL CATEG LIK CDEFLT eller CVAR for label eller       CATEG LIK
     * CDEFLT, CVAR eller CNAME for switch. Alle disse tilfellene skal
     * behandles likt. Aktuell token kan enten   v{re MIDENTIFIER eller
     * MIFE. Hvis det er MIFE, kalles     genlabelparexp som legger ut kode 
     * slik at overf|ringen    gj|res i hver gren. */
    genlabelparexp (rex->left, rex, FALSE);
}

/******************************************************************************
                                                             GEN_THUNK_ARRAY */

void gen_thunk_array (exp_t *rex)
{
  fprintf (ccode, "__er=(__dhp)");
  genvalue (rex->left);
  fprintf (ccode, ";__reth();");
}

/******************************************************************************
                                                                 GENARRAYPAR */

static void genarraypar (exp_t *rex)
{
  int i;
  switch (rex->rd->categ)
    {
    case CVALUE:
      /* V A L U E   P A R A M E T E R */

      fprintf (ccode, "__ap=(__arrp)__rca(");
      if (rex->left->token == MIDENTIFIER)
      {
        if (rex->left->rd->categ == CNAME)
          fprintf (ccode, "__er");
        else
          {
            gensl (rex->left, TRUE, OFF);
            fprintf (ccode, "%s", rex->left->rd->ident);
          }
      }
      else
	genvalue (rex->left);
      fprintf (ccode, ");");
      fprintf (ccode, "((__bs%d *)__pb)->%s=__ap;"
	            ,rex->rd->encl->blno, rex->rd->ident);
      break;
    case CDEFLT:
    case CVAR:
      /* STANDARD ELLER VAR OVERF\RING */

      fprintf (ccode, "((__bs%d *)__pb)->%s=", rex->rd->encl->blno,
		      rex->rd->ident);
      if (rex->left->token == MIDENTIFIER)
	{
	  if (rex->left->rd->categ == CNAME)
	    fprintf (ccode, "(__arrp)__er");
	  else
	    {
	      gensl (rex->left, TRUE, OFF);
	      fprintf (ccode, "%s", rex->left->rd->ident);
	    }
	}
      else
	genvalue (rex->left);
      fprintf (ccode, ";");
      break;
    case CNAME:
      if (rex->left->token == MIDENTIFIER)
	{
	  if (rex->left->rd->categ == CNAME)
	    {
	      /* Viderf|ring av en array parameter Kopierer aktuell parameter 
	       * spesifikasjon som er en  formell parameter spesifikasjon i
	       * ytre en prosedyre. (ment, ent ,sl, ap og namekind) Setter
	       * bp, en hjelpevariabel, til } peker p} den  aktuelle
	       * parameterens blokk. Dermed blir aksessveien kortere under
	       * kopieringen. */

	      fprintf (ccode, "((__bs%d *)__pb)->%s=",
			      rex->rd->encl->blno, rex->rd->ident);
	      gensl (rex->left, TRUE, ON);
	      fprintf (ccode, "%s;", rex->left->value.ident);
	    }
	  else
	    {
	      /* ADDRESS_NOTHUNK */
	      fprintf (ccode, "((__bs%d *)__pb)->%s.ap=",
			      rex->rd->encl->blno, rex->rd->ident
		);
	      gensl (rex->left, TRUE, ON);
	      fprintf (ccode, "%s;", rex->left->rd->ident);
#if ADDNOTH
	      fprintf (ccode, "((__bs%d *)__pb)->%s.namekind"
		       "=__ADDRESS_NOTHUNK;",
		       rex->rd->encl->blno, rex->rd->ident);
#endif
	    }
	}
      break;
    }				/* END SWITCH */
}				/* END-GENARRAYPAR */

/******************************************************************************
                                                         GEN_THUNK_PROCEDURE */

void gen_thunk_procedure (exp_t *rex)
{
  fprintf (ccode, "__sl=");
  if (nonetest == ON)
    fprintf (ccode, "((__bp=");
  genvalue (rex->left->left);
  if (nonetest == ON)
    fprintf (ccode, ")==__NULL?(__dhp)__rerror(__errnone):__bp)");
  fprintf (ccode, ";");

  fprintf (ccode, "__pp= ");
  if (rex->left->rd->categ == CVIRT)
    fprintf (ccode, "__sl->pp->virt[%d]", rex->left->right->rd->virtno - 1);
  else
    gen_adr_prot (ccode, rex->left->right->rd);
  fprintf (ccode, ";__reth();");
}

/******************************************************************************
                                                             GENPROCEDUREPAR */

static void genprocedurepar (exp_t *rex)
{
  int i;
  /* OVERF\RING AV PROSEDYRER SOM PARAMETERE */

  if (rex->left->token == MIDENTIFIER)
    {
      switch (rex->left->rd->categ)
	{
	case CDEFLT:
	case CVAR:
	  /* Kopiere psl (prosedyrens statiske omgivelse) og  pp
	   * (prosedyrens prototype) og overf|rer evt. __conv og q */
	  fprintf (ccode, "__bp=");
	  gensl (rex->left, FALSE, ON);
	  fprintf (ccode, ";((__bs%d *)__pb)->%s.psl=((__bs%d *)__bp)->%s.psl;"
		   "((__bs%d *)__pb)->%s.pp=((__bs%d *)__bp)->%s.pp;",
		   rex->rd->encl->blno, rex->rd->ident,
		   rex->left->rd->encl->blno, rex->left->rd->ident,
		   rex->rd->encl->blno, rex->rd->ident,
		   rex->left->rd->encl->blno, rex->left->rd->ident);
	  gen_conv (rex, TRUE, FALSE);
	  break;
	case CNAME:
	  if (rex->rd->categ == CNAME)
	    {
	      /* Videresending av NAME-par. Kopierer hele den aktuelle
	       * beskrivelsen, dvs. sl,pp,psl,adr.men, adr.ent og namekind
	       * pluss evt. __conv og q. */

	      fprintf (ccode, "((__bs%d *)__pb)->%s=",
		       rex->rd->encl->blno, rex->rd->ident);
	      gensl (rex->left, TRUE, ON);
	      fprintf (ccode, "%s;", rex->left->value.ident);
	      gen_conv (rex, TRUE, TRUE);
	    }
	  else
	    {
	      /* AKTUELL PARAMETER ER EN NAME-PAR I EN YTRE PROSEDYRE
	       * Kallerp} transcall som skriver ut koden for kallet
	       * __rgetproc. Den rutinen returnerer med statisk 
	       * omgivelse i sl og prototypen i pp.
	       * Disse overf|res til den formelle parameteren */

	      fprintf (ccode, "((__bs%d *)__pb)->%s.psl=__sl;"
		       "((__bs%d *)__pb)->%s.pp=__pp;", 
		       rex->rd->encl->blno, rex->rd->ident,
		       rex->rd->encl->blno, rex->rd->ident);
	      gen_conv (rex, TRUE, FALSE);
	    }
	  break;
	case CVIRT:
	case CLOCAL:
	  fprintf (ccode, "((__bs%d *)__pb)->%s.psl=",
		   rex->rd->encl->blno, rex->rd->ident);
	  gensl (rex->left, FALSE, OFF);
	  fprintf (ccode, ";");
	  if (rex->left->rd->categ == CVIRT)
	    /* M} teste under RUN-TIME om
	     * virtuell-tabbelen er null */
	    fprintf (ccode, "if((__pp=((__bs%d *)__pb)->%s.psl->"
		     "pp->virt[%d])==__NULL)__rerror(__errvirt);",
		     rex->rd->encl->blno, rex->rd->ident,
		     rex->left->rd->virtno - 1);
	  fprintf (ccode, "((__bs%d *)__pb)->%s.pp= ",
		   rex->rd->encl->blno, rex->rd->ident);
	  if (rex->left->rd->categ == CVIRT)
	    fprintf (ccode, "__pp;");
	  else
	    {
	      gen_adr_prot (ccode, rex->left->rd);
	      fprintf (ccode, ";");
	    }
	  gen_conv (rex, TRUE, FALSE);
	  break;
	}			/* END-SWITCH */
#if ADDNOTH
      if (rex->rd->categ == CNAME && rex->left->rd->categ != CNAME)
	fprintf (ccode, "((__bs%d *)__pb)->%s.namekind=__ADDRESS_NOTHUNK;",
		 rex->rd->encl->blno, rex->rd->ident);
#endif
    }			/* END aktuell par.token = MIDENTIFIER */
  else
    {
      /* Aktuell par.token = MDOT */

      fprintf (ccode, "((__bs%d *)__pb)->%s.psl=", 
	       rex->rd->encl->blno, rex->rd->ident);
      if (nonetest == ON)
	fprintf (ccode, "((__bp=");
      genvalue (rex->left->left);
      if (nonetest == ON)
	fprintf (ccode, ")==__NULL?(__dhp)__rerror(__errnone):__bp)");
      fprintf (ccode, ";");

      if (rex->left->rd->categ == CVIRT)
	/* M} teste under RUN-TIME om
	 * virtuell-tabbelen er null */
	fprintf (ccode, "if((__pp=((__bs%d *)__pb)->%s.psl->"
		 "pp->virt[%d])==__NULL)__rerror(__errvirt);",
		 rex->rd->encl->blno, rex->rd->ident,
		 rex->left->right->rd->virtno - 1);
      fprintf (ccode, "((__bs%d *)__pb)->%s.pp= ",
	       rex->rd->encl->blno, rex->rd->ident);
      if (rex->left->right->rd->categ == CVIRT)
	fprintf (ccode, "__pp;");
      else
	{
	  gen_adr_prot (ccode, rex->left->right->rd);
	  fprintf (ccode, ";");
	}
      gen_conv (rex, TRUE, FALSE);
    }
}

/******************************************************************************
                                                                GENPROCPARAM */

void genprocparam (exp_t *re)
{
  exp_t *rex;

  for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
    {
      if(rex->token == MSENTCONC)  
	{
	  genvalue (rex->left); fprintf (ccode, ";");
	} 
      else if (rex->token == MSENDADDRESSTHUNKTOFORMALPAR)
	{
	  send_to_formal_par (rex, TRUE);
	} 
      else if (rex->token == MSENDVALUETHUNKTOFORMALPAR)
	{
	  send_to_formal_par (rex, FALSE);
	} 
      else if (rex->rd->kind == KSIMPLE)
	{
	  /* ENKEL PARAMETER */

	  if (rex->rd->type == TLABEL)	/* LABEL PARAMETER */
	    genlabelswitchpar (rex);
	  else
	    gensimplepar (rex);	/* INTEGER, REAL, CHARACTER,REF */
	  /* TEXT ELLER BOOLEAN PARAMETER */
	}
      else
	/* END-ENKEL PARAMETER */ 
	if (rex->rd->kind == KARRAY)
	  {
	    if (rex->rd->type != TLABEL)	/* ARRAY  PARAMETER */
	      genarraypar (rex);
	    else
	      genlabelswitchpar (rex);
	  }
	else if (rex->rd->kind == KPROC)
	  genprocedurepar (rex);
	else			/* FEIL */; 
    }/* END FOR L\KKE */
}			/* END GENPROCPARAM */

/******************************************************************************
                                                      GENPREDEFPROCCALL      */

void genpredefproccall (exp_t *re)
{
  int i;
  /* Hvis danger = TRUE s} skal returverdien legges p} stakken */
  
  exp_t *rex;

  if (re->rd->descr->codeclass != CCEXIT)
    fprintf (ccode, "%s(", re->rd->descr->rtname);
  
  switch (re->rd->descr->codeclass)
    {
    case CCRANDOMRUTDANGER:
    case CCSIMPLEDANGER:
    case CCSIMPLE:
      break;
    case CCDETACH:	/* Detach *//* gensl */

      if (is_after_dot (re) || seen_th_insp (re))
	gensl (re, FALSE, nonetest);
      else
	genchain (firstclass (), FALSE);

      fprintf (ccode, ",");

      /* No break at end of this case */
    case CCCALLRESUME:	/* CALL  og RESUME  */
      for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
	{
	  genvalue (rex->left);
	  fprintf (ccode, ",");
	}
      fprintf (ccode, "%d,", i = newlabel ());
      genmodulemark(NULL);
      fprintf (ccode, ");");
      exitlabel (i);	/* Reentrings punkt */
      return;
      break;
    case CCEXIT:		/* TERMINATE_PROGRAM */
      if (separat_comp)
	fprintf 
	  (ccode, "__goto.ent=%d,__goto.ment=__NULL;return;", 
	   STOPLABEL);
      else
	gotolabel (STOPLABEL);
      not_reached = TRUE;
      return;
      break;
    case CCTEXTDANGER:
    case CCTEXT:
      /* TEXT-attributt prosedyre. F|rste parameter skal v{re
       * en peker til tekstvariabelen */
	genvalue (re->up->left);
      if (re->right->token != MENDSEP)
	fprintf (ccode, ",");
      break;
    case CCBLANKSCOPY:
    case CCFILEBLANKSCOPY:
      fprintf (ccode, "%ldL", re->value.n_of_stack_elements);
      if (re->right->token != MENDSEP 
	  || re->rd->descr->codeclass == CCFILEBLANKSCOPY)
	fprintf (ccode, ",");
      if (re->rd->descr->codeclass == CCBLANKSCOPY)
	break;
    case CCFILEDANGER:
    case CCFILE:
      /* En av fil-prosedyrene. F|rste parameter er peker til fil
       * klasse objektet */
      gensl (re, FALSE, nonetest);
      
      if (re->right->token != MENDSEP)
	fprintf (ccode, ",");
      break;
    }			/* END-SWITCH */
  
  /* Overf|rer bruker parameterene */
  
  for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
    {
      if (rex->rd->categ == CVAR)
	{
	  /* Siste parameter til random drawing */
	  /* Som er en NAME (VAR) parameter */
	  fprintf (ccode, "&");
	}
      genvalue (rex->left);
      
      if (rex->right->token != MENDSEP)
	fprintf (ccode, ",");
    }
  fprintf (ccode, ")");
  
}				/* END-Genpredefproccall */


/******************************************************************************
  GETFIRSTCLASSATTRIBUT */

/* Hjelperutine som retunerer med en peker til DECL-objektet til f|rste
 * attributtet i klassen som parameteren peker p}. Den leter f|rst rekursivt
 * i prefiks klassene. Rutinen brukes under overf|ring av referanse
 * parametere til eksterne C-prosedyrer. */


static decl_t *getfirstclassattribut (decl_t *rd)
{
  decl_t *rdd;
  
  if (rd->plev != 0 
      && (rdd = getfirstclassattribut (rd->prefqual)) != NULL)
    return (rdd);
  
  for (rdd = rd->descr->parloc; rdd != NULL &&
       !(rdd->categ == CLOCAL && (rdd->kind == KSIMPLE 
				  || rdd->kind == KARRAY));
       rdd = rdd->next);
  return (rdd);
}

/******************************************************************************
  PAR_TO_CPROC */

/* Overf|rer parameter til en ekstern C-prosedyre.
 * rex->left->rd angir den aktuelle parameteren, mens rex->rd angir den
 * formelle. */

static void par_to_cproc (exp_t *rex)
{
  decl_t *rd;
  switch (rex->rd->kind)
    {
    case KSIMPLE:
      if (rex->rd->type == TTEXT)
	{
	  if (rex->rd->categ == CVALUE)
	    {
	      /* By value, Kopierer teksten over i C-space */
	      fprintf (ccode, "__rcopytexttoc(");
	      genvalue (rex->left);
	      fprintf (ccode, ")");
	    }
	  else if (rex->rd->categ == CDEFLT)
	    {
	      /* By referanse, Overf|rer en peker til f|rste character. 
	       * (dette gj|res av rt-rutienen raddroffirstchar */
	      fprintf (ccode, "__raddroffirstchar(");
	      genvalue (rex->left);
	      fprintf (ccode, ")");
	    }
	}
      else if (rex->rd->categ == CVAR || rex->rd->categ == CNAME)
	{			/* Enkel parameter (ikke TEXT) by name */
	  fprintf (ccode, "&");
	  genvalue (rex->left);
	}
      else
	{
	  /* Overf|rt p} standard m}te. (IKKE TEXT */
	  if (rex->rd->type == TREF)
	    {
	      /* Skal overf|re adressen til f|rste attributt Setter rd
	       * til } peke p} f|rste attributt i klassen
	       * rex->left->qual eller i en av dens prefiks-klasser.
	       * Hvis klassen ikke har noen attributter overf|res NULL */
	      
	      rd = getfirstclassattribut (rex->left->qual);
	      if (rd == NULL)
		fprintf (ccode, "__NULL");
	      else
		{
		  fprintf (ccode, "&((__bs%d *)", rd->encl->blno);
		  genvalue (rex->left);
		  fprintf (ccode, ")->%s", rd->ident);
		}
	    }
	  else
	    genvalue (rex->left);
	}
      break;
    case KARRAY:
      /* Overef|ring av array som parameter. Lovlige
       * overf|ringsmodus er
       * For TEXT: by value = Lager et array av (char
       * i C-space, kopierer alle
       * tekster til C-space
       * by referense (categ = CDEFLT
       * Lager et (char *) array i
       * C-space, og setter disse til
       * peke p} tekstene i SIMULA
       * space. (f}rste tegn i teksten
       * by name -> ulovlig
       * For REF:  by referanse (categ = CDEFLT
       * Overf|rer peker til f|rste
       * element i arrayet
       * by name -> ulovlig
       * For andre by value   Kopierer arrayet til C-space
       * by referanse (categ = CDEFLT
       * by name (og var) : Peker til f|rste
       * element */
      if (rex->rd->type == TTEXT)
	{
	  fprintf (ccode, "__rcopytextarrtoc(");
	  genvalue (rex->left);
	  if (rex->rd->categ == CVALUE)
	    fprintf (ccode, ",__TRUE)");
	  else
	    fprintf (ccode, ",__FALSE)");
	}
      else
	{
	  if (rex->rd->categ == CVALUE)
	    fprintf (ccode, "__rcopyarrtoc(");
	  else
	    fprintf (ccode, "__raddroffirstelem(");
	  genvalue (rex->left);
	  fprintf (ccode, ")");
	}
      break;
    case KPROC:
      /* Bare lovlig } overf|re C-prosedyrer */
      fprintf (ccode, "(&%s)()",
		      (rex->left->token == MDOT ?
		       rex->left->right->rd->descr->rtname :
		       rex->left->rd->descr->rtname));
      break;
      
    }
}



/******************************************************************************
  GENCPROCCALL      */
void gencproccall (exp_t *re)
{
  exp_t *rex;
  
  fprintf (ccode, "%s(", re->rd->descr->rtname);
  
  /* Overf|rer parameterene */
  
  for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
    {
      par_to_cproc (rex);
      if (rex->right->token != MENDSEP)
	fprintf (ccode, ",");
      
    }			/* END-OVERF\RING AV PARAMETERE */
  
  fprintf (ccode, ")");
  
}
