/* $Id: cgenpar.c,v 1.24 1994/11/03 08:41:00 cim Exp $ */

/* Copyright (C) 1994 Sverre Hvammen Johansen and Terje Mjøs,
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
#include "lex.h"
#include "extspec.h"

#define ADDNOTH 0

/******************************************************************************
                                                              GEN_CONV_AND_Q */

static gen_conv_and_q (rex, procedure, transported, copied_all)
     struct EXP *rex;
     char procedure,
       transported,
       copied_all;
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
	(void) fprintf (ccode, "((__bs%d *)__pb)->%s.conv",
			rex->rd->encl->blno, rex->rd->ident);
      if (transported)
	{
	  if (copied_all)
	    if (writetest)
	      (void) fprintf (ccode, "|= __WRITETEST;",
			      rex->left->value.ident);
	    else;
	  else
	    {
	      genchainrdencl (rex->left, TRUE);
	      if (writetest)
		(void) fprintf (ccode, "=%s.conv | __WRITETEST;",
				rex->left->value.ident);
	      else
		(void) fprintf (ccode, "=%s.conv;", rex->left->value.ident);
	    }
	}
      else if (writetest)
	(void) fprintf (ccode, "=__WRITETEST;");
      else
	(void) fprintf (ccode, "=__NOTEST;");

      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.q=", rex->rd->encl->blno,
		      rex->rd->ident);

      if (transported)
	{
	  genchainrdencl (rex->left, TRUE);
	  (void) fprintf (ccode, "%s.q", rex->left->value.ident);
	}
      else
	gen_adr_prot (rex->left->qual);
      (void) fprintf (ccode, ";");
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

	  (void) fprintf (ccode, "if(__rrin(");
	  gen_adr_prot (rex->rd->prefqual);
	  (void) fprintf (ccode, ",");

	  genchainrdencl (rex->left, TRUE);
	  (void) fprintf (ccode, "%s.q)){", rex->left->value.ident);
	}
      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.conv=__READTEST;",
		      rex->rd->encl->blno, rex->rd->ident);

      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.q= ",
		      rex->rd->encl->blno, rex->rd->ident);
      gen_adr_prot (rex->rd->prefqual);
      (void) fprintf (ccode, ";");

      if (transported)
	{
	  (void) fprintf (ccode, "}elseif(__rrin(");
	  genchainrdencl (rex->left, TRUE);
	  (void) fprintf (ccode, "%s.q,",
			  rex->left->value.ident);
	  gen_adr_prot (rex->rd->prefqual);
	  (void) fprintf (ccode, ")){");
	  if (!copied_all)
	    {
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.conv=",
			      rex->rd->encl->blno, rex->rd->ident);
	      genchainrdencl (rex->left, TRUE);
	      (void) fprintf (ccode, "%s.conv;", rex->left->value.ident);

	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.q=", 
			      rex->rd->encl->blno,
			      rex->rd->ident);
	      genchainrdencl (rex->left, TRUE);
	      (void) fprintf (ccode, "%s.q;", rex->left->value.ident);
	    }
	  (void) fprintf (ccode, "}else __rerror(__errqual);");
	}
    }
/*** END-FORMELL KVAL. ER EN SUBKLASSE AV AKTUELL KVAL.            ****/
}

/******************************************************************************
                                                               GEN_ARIT_CONV */

static gen_arit_conv (rex, transported, copied_all)
     struct EXP *rex;
     char transported,
       copied_all;
{
  if (transported != copied_all || rex->left->type != rex->rd->type)
    {
      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.conv=",
		      rex->rd->encl->blno, rex->rd->ident);

      if (transported)
	(void) fprintf (ccode, "__ctab[");

      if (rex->left->type == TINTG && rex->rd->type == TREAL)
	(void) fprintf (ccode, "__INTREAL");
      else if (rex->left->type == TREAL && rex->rd->type == TINTG)
	(void) fprintf (ccode, "__REALINT");
      else
	(void) fprintf (ccode, "__NOCONV");

      if (transported)
	{
	  (void) fprintf (ccode, "][");
	  genchainrdencl (rex->left, TRUE);
	  (void) fprintf (ccode, "%s.conv]", rex->left->value.ident);
	}
      (void) fprintf (ccode, ";");
    }
}

/******************************************************************************
                                                                    GEN_CONV */

static gen_conv (rex, procedure, copied_all)
     struct EXP *rex;
     char procedure,
       copied_all;
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

static send_to_formal_par (rex, addressthunk, ent, hdot)
     struct EXP *rex;
     char addressthunk;
     int ent;
     char hdot;
{
  /* Hvis hdot = FALSE er denne rutinen kalt for en label     eller array
   * name. Structen for disse har ikke et .h felt som finnes i structene for
   * enkle name-parametere. */

  char t[3];

  if (hdot)
    (void) strcpy (t, "h.");
  else
    (void) strcpy (t, "");

  if (addressthunk)
    (void) fprintf (ccode, "((__bs%d *)__pb)->%s.%snamekind=__ADDRESS_THUNK;",
		    rex->rd->encl->blno, rex->rd->ident, t);
  else
    (void) fprintf (ccode, "((__bs%d *)__pb)->%s.%snamekind=__VALUE_THUNK;",
		    rex->rd->encl->blno, rex->rd->ident, t);

/***** OVERF\RER THUNKENS ADRESSE                                   ****/
  if (separat_comp)
    (void) fprintf (ccode, "((__bs%d *)__pb)->%s.%sadr.ment=__m_%s;",
		    rex->rd->encl->blno, rex->rd->ident, t, timestamp);
  else
    (void) fprintf (ccode, "((__bs%d *)__pb)->%s.%sadr.ment=__NULL;",
		    rex->rd->encl->blno, rex->rd->ident, t);

  (void) fprintf (ccode, "((__bs%d *)__pb)->%s.%sadr.ent=%d;",
		  rex->rd->encl->blno, rex->rd->ident, t, ent);

  /* OG THUNKENS STATISKE OMGIVELSE */
  (void) fprintf (ccode, "((__bs%d *)__pb)->%s.%ssl=__lb",
		  rex->rd->encl->blno, rex->rd->ident, t);
  if (inthunk)
    (void) fprintf (ccode, "->sl");
  (void) fprintf (ccode, ";");
  if (rex->rd->kind != KARRAY)
    gen_conv (rex, FALSE, FALSE);
}

/******************************************************************************
                                                                   GEN_THUNK */

static gen_thunk (rex, inner_thunk_part, label)
     struct EXP *rex;
     int (*inner_thunk_part) ();
     int label;
{
/***** STARTER GENERERINGEN AV THUNKEN                   ****/
  gotollabel (push = newllabel ());
  (void) fprintf (ccode, "/* ADDRESS_THUNK */");
  typelabel (label);

/***** Markerer at sl m} f|lges en gang ekstra inne i fra thunken***/
  inthunk++;

  gencall (rex, rex->left);

  inner_thunk_part (rex);
  (void) fprintf (ccode, "__reth();");
  gotoswitch ();
  typellabel (pop);
  inthunk--;
}

/******************************************************************************
                                                    GEN_THUNK_SIMPLE_ADDRESS */

/* Genererer kode som for ADDRESS_THUNK avgj|r om thunken skal returnere
 * med en adresse eller en verdi. Dersom en verdi skal returners
 * genereres det ogs} kode som utf|rer evt. konverteringer og 
 * kvalifikasjonstester */

static gen_thunk_simple_address (rex)
     struct EXP *rex;
{
  switch (rex->left->token)
    {
    case MARRAYARG:
    case MARRAYADR:
      (void) fprintf (ccode, "__er=__r[%d];", (int) rex->left->value.ival);
      (void) fprintf (ccode, "__ev.i=__v[%d].i;", (int) rex->left->left);
      break;
    default:
      (void) fprintf (ccode, "__er=");
      if (nonetest == ON)
	(void) fprintf (ccode, "((__bp=");
      genvalue (rex->left->left);
      if (nonetest == ON)
	(void) fprintf (ccode,
			")==__NULL?(__dhp)__rerror(__errnone):__bp)");
      (void) fprintf (ccode, ";");
      (void) fprintf 
	(ccode, "__ev.i=((char *)&((__bs%d *)__p)->%s) - (char *)__p;",
	 rex->left->right->rd->encl->blno,
	 rex->left->right->rd->ident);
      break;
    }
  if (rex->rd->type == TBOOL || rex->rd->type == TCHAR)
    {				/* Leser verdien hvis det ikke er skrive
				 * aksess. */
      (void) fprintf (ccode, "if(!((__thunkp)__pb)->writeaccess)");
      (void) fprintf (ccode, "__ev.c= *(char *)(((char *)__er)+__ev.i);");
    }
  else if (rex->rd->type == TINTG || rex->rd->type == TREAL)
    {				/* Leser og konverterer verdien hvis det ikke 
				 * er skrive aksess. */
      (void) fprintf (ccode, "if(!((__thunkp)__pb)->writeaccess)");
      (void) fprintf (ccode, "   if(((__thunkp)__pb)->ftype==__TINTG)");
      if (rex->left->type == TINTG)
	(void) fprintf (ccode, "__ev.i= *(long *)(((char *)__er)+__ev.i);");
      else
	(void) fprintf (ccode, "__ev.i= *(double *)(((char *)__er)+__ev.i);");
      (void) fprintf (ccode, "   else ");
      if (rex->left->type == TINTG)
	(void) fprintf (ccode, "__ev.f= *(long *)(((char *)__er)+__ev.i);");
      else
	{
	  (void) fprintf 
	    (ccode, "__ev.f=(((__thunkp)__pb)->conv==__REALINTREAL)?");
	  (void) fprintf (ccode, "__rintrea(*(double *)(((char *)__er)+__ev.i)):");
	  (void) fprintf (ccode, "*(double *)(((char *)__er)+__ev.i);");
	}
    }
  if (rex->rd->type == TTEXT)
    {				/* Leser verdien hvis det ikke er skrive
				 * aksess. */
      (void) fprintf (ccode, "if(!((__thunkp)__pb)->writeaccess)");
      (void) fprintf (ccode, "__et= *(__txt *)(((char *)__er)+__ev.i);");
    }
  else if (rex->rd->type == TREF)
    {
      (void) fprintf (ccode, "if(!((__thunkp)__pb)->writeaccess)");
      (void) fprintf (ccode, "   if((((__thunkp)__pb)->conv==__READTEST ||");
      (void) fprintf (ccode, " ((__thunkp)__pb)->conv==__READWRITETEST) &&");
      (void) fprintf (ccode, " !__rin(*(__dhp *)(((char *)__er)+__ev.i),((__thunkp)__pb)->q))");
      (void) fprintf (ccode, "__rerror(__errqual);");
      (void) fprintf 
	(ccode, "else __er= *(__dhp *)(((char *)__er)+__ev.i);");
    }
}

/******************************************************************************
                                                    GEN_THUNK_SIMPLE_VALUE   */

static gen_thunk_simple_value (rex)
     struct EXP *rex;
{
  switch (rex->left->type)
    {
    case TINTG:
      (void) fprintf (ccode, "__ev.i=");
      break;
    case TREAL:
      (void) fprintf (ccode, "__ev.f=");
      break;
    case TBOOL:
    case TCHAR:
      (void) fprintf (ccode, "__ev.c=");
      break;
    case TREF:
      (void) fprintf (ccode, "__er=");
      break;
    case TTEXT:
      (void) fprintf (ccode, "__et= *");
      break;
    }

  genvalue (rex->left);
  (void) fprintf (ccode, ";");

  /* KONVERTERING OG KVAL. TESTER */
  if (rex->rd->type == TINTG || rex->rd->type == TREAL)
    {				/* Leser og konverterer verdien hvis det ikke 
				 * er skrive aksess. */
      (void) fprintf (ccode, "if(((__thunkp)__pb)->ftype==__TINTG)");
      if (rex->left->type == TINTG)
	(void) fprintf (ccode, "/*OK*/;");
      else
	(void) fprintf (ccode, "__ev.i=__ev.f;");
      (void) fprintf (ccode, "else ");
      if (rex->left->type == TINTG)
	(void) fprintf (ccode, "__ev.f=__ev.i;");
      else
	(void) fprintf 
	  (ccode, 
	   "if(((__thunkp)__pb)->conv==__REALINTREAL)__ev.f=__rintrea(__ev.f);");
    }
  else if (rex->rd->type == TREF)
    {
      (void) fprintf (ccode, "if((((__thunkp)__pb)->conv==__READTEST ||");
      (void) fprintf (ccode, " ((__thunkp)__pb)->conv==__READWRITETEST) &&");
      (void) fprintf (ccode, " !__rin(__er,((__thunkp)__pb)->q))");
      (void) fprintf (ccode, "__rerror(__errqual);");
    }
}

/******************************************************************************
                                                       GENSIMPLEPAR          */

static gensimplepar (rex)
     struct EXP *rex;
{
  int i;
/***** ENKEL INTEGER, REAL, CHAR, REF,TEXT ELLER BOOL  PARAMETER     ****/
  struct EXP *re;
  char index_is_const = TRUE;

  if (rex->rd->categ == CVALUE && rex->rd->type == TTEXT)
    {
      /* T E X T  V A L U E  P A R A M E T E R */
      gencall (rex, rex->left);
      (void) fprintf (ccode, "((__bs%d *)__pb)->%s= *__rcopy(",
		      rex->rd->encl->blno, rex->rd->ident);
      gen_ant_stack (rex, rex->left);
      (void) fprintf (ccode, ",");
      genvalue (rex->left);
      (void) fprintf (ccode, ");");
    }
  else if (rex->rd->categ == CDEFLT)
    {
      /* S T A N D A R D   O V E R F \ R I N G */
      gencall (rex, rex->left);

      if (rex->rd->type == TTEXT)
	{
	  (void) fprintf 
	    (ccode, "((__bs%d *)__pb)->%s= *", rex->rd->encl->blno,
	     rex->rd->ident);
	  genvalue (rex->left);
	  (void) fprintf (ccode, ";");
	}
      else
	{
	  (void) fprintf (ccode, "((__bs%d *)__pb)->%s=", rex->rd->encl->blno,
			  rex->rd->ident);
	  genvalue (rex->left);
	  (void) fprintf (ccode, ";");
	}
    }
  else if (rex->rd->categ == CVAR)
    {
      /* V A R  P A R A M E T E R */

      gencall (rex, rex->left);

      if (rex->left->rd->categ == CVAR)
	{
	  /* AKTUELL PARAMETER ER EN FORMELL VAR  PARAMETER I EN YTRE
	   * PROSEDYRE. VIDEREF\RING AV EN ENKEL VAR PARAMETER. */

	  /* Tilordner bp */
	  (void) fprintf (ccode, "((__bs%d *)__pb)->%s=",
			  rex->rd->encl->blno, rex->rd->ident);
	  genchainrdencl (rex->left, TRUE);
	  (void) fprintf (ccode, "%s;", rex->left->value.ident);
	  gen_conv (rex, FALSE, TRUE);
	} /* END VIDEREFRING AV ENKEL VAR-PARAMETER */ 
      else if (rex->left->rd->categ == CNAME)
	{			/* Aktuell parameter er en formell NAME-par i 
				 * en ytre prosedyre. Kallet p} gencall som
				 * legger ut kode for kall p} __rgetsa. Den
				 * returnerer adressen til variabelen i er og 
				 * ev. */

	  (void) fprintf 
	    (ccode, "((__bs%d *)__pb)->%s.bp=__er;", rex->rd->encl->blno,
	     rex->rd->ident);
	  (void) fprintf 
	    (ccode, "((__bs%d *)__pb)->%s.ofs=__ev.i;", rex->rd->encl->blno,
	     rex->rd->ident);
	  gen_conv (rex, FALSE, FALSE);
	}
      else
	{
	  /* ENKEL VAR PARAMETER, IKKE VIDEREF\RING  Tilordner bp */
	  (void) fprintf 
	    (ccode, "((__bs%d *)__pb)->%s.bp=", rex->rd->encl->blno,
	     rex->rd->ident);

	  switch (rex->left->token)
	    {
	    case MDOT:
	      if (nonetest == ON)
		(void) fprintf (ccode, "((__bp=");
	      genvalue (rex->left->left);
	      if (nonetest == ON)
		(void) fprintf (ccode,
			      ")==__NULL?(__dhp)__rerror(__errnone):__bp)");
	      break;
	    case MARRAYADR:
	      /* Peker til array ligger p} stakken */
	      (void) fprintf (ccode, "__r[%d]", (int) rex->left->value.ival);
	      break;
	    case MIDENTIFIER:
	      genchainrdencl (rex->left, FALSE);
	      break;
	    }
	  (void) fprintf (ccode, ";");

	  (void) fprintf 
	    (ccode, "((__bs%d *)__pb)->%s.ofs=", rex->rd->encl->blno,
	     rex->rd->ident);

	  if (rex->left->token == MARRAYADR)
	    (void) fprintf (ccode, "__v[%d].i;", (int) rex->left->left);
	  else
	    {
/***** Da m} det enten v{re MDOT eller MIDENTIFIER.         ****/
#if 0
	      (void) fprintf 
		(ccode, "((char *)&((__bs%d *)((__bs%d *)__pb)->%s.bp)->%s)",
		 rex->left->rd->encl->blno, rex->rd->encl->blno,
		 rex->rd->ident, rex->left->rd->ident);
	      (void) fprintf 
		(ccode, "-((char *)&((__bs%d *)__pb)->%s.bp->pp);",
		 rex->rd->encl->blno, rex->rd->ident);
#else
	      (void) fprintf 
		(ccode, "((char *)&((__bs%d *)__p)->%s)",
		 rex->left->rd->encl->blno, rex->left->rd->ident);
	      (void) fprintf 
		(ccode, "-(char *)__p;",
		 rex->rd->encl->blno, rex->rd->ident);
#endif
	    }
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
	  (void) fprintf (ccode, "((__bs%d *)__pb)->%s.tp="
			  ,rex->rd->encl->blno, rex->rd->ident);
	  genvalue (rex->left);
	  (void) fprintf (ccode, ";");
	  /* namekind = VALUE_NOTHUNK */
	  (void) fprintf 
	    (ccode, "((__bs%d *)__pb)->%s.h.namekind=__VALUE_NOTHUNK;",
	     rex->rd->encl->blno, rex->rd->ident);
	  break;
	case MINTEGERKONST:
	case MREALKONST:
	case MCHARACTERKONST:
	case MBOOLEANKONST:
	case MNONE:
	  /* VALUE NOTHUNK Overf|rer verdien. */
	  (void) fprintf (ccode, "((__bs%d *)__pb)->%s.h.v.",
			  rex->rd->encl->blno, rex->rd->ident);
	  switch (rex->rd->type)
	    {
	    case TINTG:
	      (void) fprintf (ccode, "i=");
	      break;
	    case TREAL:
	      (void) fprintf (ccode, "f=");
	      break;
	    case TBOOL:
	    case TCHAR:
	      (void) fprintf (ccode, "c=");
	      break;
	    case TREF:
	      (void) fprintf (ccode, "r=");
	      break;
	    default:;
	    }
	  genvalue (rex->left);
	  (void) fprintf (ccode, ";");

	  /* namekind = VALUE_NOTHUNK */

	  (void) fprintf 
	    (ccode, "((__bs%d *)__pb)->%s.h.namekind=__VALUE_NOTHUNK;",
	     rex->rd->encl->blno, rex->rd->ident);

	  if (rex->rd->type == TREAL || rex->rd->type == TINTG)
	    {
	      /* __conv = NOCONV */
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.conv=__NOCONV;",
			      rex->rd->encl->blno, rex->rd->ident);
	    }
	  else if (rex->rd->type == TREF)
	    {
	      /* __conv = notest */
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.conv=__NOTEST;",
			      rex->rd->encl->blno, rex->rd->ident);
	    }
	  break;
	case MIDENTIFIER:
	  if (rex->left->rd->categ == CNAME)
	    {
	      /* AKTUELL PARAMETER ER EN FORMELL NAME-PARAMETER I EN YTRE
	       * PROSEDYRE. VIDERF\RING  Setter bp, en hjelpevariabel, til }
	       * peker p} den aktuelle parameterens blokk. Dermed blir
	       * aksessveien kortere under kopieringen. */

	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s=",
			      rex->rd->encl->blno, rex->rd->ident);
	      genchainrdencl (rex->left, TRUE);
	      (void) fprintf (ccode, "%s;", rex->left->value.ident);
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

	      (void) fprintf (ccode, "__bp=");
	      genchainrdencl (rex->left, FALSE);
	      (void) fprintf (ccode, ";");

	      /* Tilordner den formelle name-parameterens bp og ofs */
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.h.bp=",
			      rex->rd->encl->blno, rex->rd->ident);
	      (void) fprintf 
		(ccode, "((__bs%d *)__bp)->%s.bp;", rex->left->rd->encl->blno,
			      rex->left->value.ident);

	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.h.v.ofs=",
			      rex->rd->encl->blno, rex->rd->ident);
	      (void) fprintf 
		(ccode, 
		 "((__bs%d *)__bp)->%s.ofs;", rex->left->rd->encl->blno,
		 rex->left->value.ident);
#if ADDNOTH
	      (void) fprintf 
		(ccode, "((__bs%d *)__pb)->%s.h.namekind=__ADDRESS_NOTHUNK;",
		 rex->rd->encl->blno, rex->rd->ident);
#endif
	      gen_conv (rex, FALSE, FALSE);
	    }
	    /* END-AKTUELL PAR ER EN FORMELL VAR-PAR. */
	  else
	    {
	      /* ADDRESS NOTHUNK Tilordner den formelle name-parameterens bp
	       * og ofs */
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.h.bp=",
			      rex->rd->encl->blno, rex->rd->ident);
	      gensl (rex->left, FALSE, OFF);
	      (void) fprintf (ccode, ";");
#if 0
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.h.v.ofs=",
			      rex->rd->encl->blno, rex->rd->ident);
	      (void) fprintf (ccode,
		     "((char *)&((__bs%d *)((__bs%d *)__pb)->%s.h.bp)->%s)",
			      rex->left->rd->encl->blno, rex->rd->encl->blno,
			      rex->rd->ident, rex->left->rd->ident);
	      (void) fprintf 
		(ccode, "-((char *)&((__bs%d *)__pb)->%s.h.bp->pp);",
		 rex->rd->encl->blno, rex->rd->ident);
#else
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.h.v.ofs=",
			      rex->rd->encl->blno, rex->rd->ident);
	      (void) fprintf (ccode,
		     "((char *)&((__bs%d *)__p)->%s)",
			      rex->left->rd->encl->blno,
			      rex->left->rd->ident);
	      (void) fprintf 
		(ccode, "-(char *)__p;",
		 rex->left->rd->encl->blno);
#endif

	      /* Tilordner namekind */
#if ADDNOTH
	      (void) fprintf 
		(ccode, "((__bs%d *)__pb)->%s.h.namekind=__ADDRESS_NOTHUNK;",
		 rex->rd->encl->blno, rex->rd->ident);
#endif
	      gen_conv (rex, FALSE, FALSE);
	    }
	  break;
	case MARRAYARG:
	  /* SJEKKER OM ALLE INDEKSENE BEST]R AV KONSTANTER TRENGER I S] FALL 
	   * IKKE GENERERE EN THUNK. */

	  for (re = rex->left->right; index_is_const && re->token != MENDSEP;
	       re = re->right)
	    if (re->left->token != MINTEGERKONST)
	      index_is_const = FALSE;

	  if (index_is_const)
	    {
	      /* ARRAY HVOR ALLE INDEKSENE BEST]R AV KONSTANTER  ADDRESS
	       * NOTHUNK  Tilordner den formelle name-parameterens bp og ofs */

	      gencall (rex, rex->left);

	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.h.bp=__r[%d];",
			      rex->rd->encl->blno, rex->rd->ident,
			      (int) rex->left->value.ival);

	      (void) fprintf 
		(ccode, "((__bs%d *)__pb)->%s.h.v.ofs=__v[%d].i;",
		 rex->rd->encl->blno, rex->rd->ident,
		 (int) rex->left->left);

	      /* Tilordner namekind */
#if ADDNOTH
	      (void) fprintf 
		(ccode, "((__bs%d *)__pb)->%s.h.namekind=__ADDRESS_NOTHUNK;",
		 rex->rd->encl->blno, rex->rd->ident);
#endif
	      gen_conv (rex, FALSE, FALSE);
	    }
	  else
	    /* END-ARRAY MED KONSTANTE INDEKSER */
	    {
	      /* ARRAY HVOR IKKE ALLE INDEKSENE BEST]R AV KONSTANTER ELLER ET 
	       * DOT'et ARRAY. ADDRESS THUNK */

	      send_to_formal_par (rex, TRUE, i = newlabel (), TRUE);

	      gen_thunk (rex, gen_thunk_simple_address, i);
	    }
	  break;
	case MDOT:
	  /* Dersom det er et dot'et prosedyre-kall, s} skal  det genereres
	   * VALUE_THUNK og ikke ADDRESS_THUNK. */
	  if (rex->left->right->rd->kind != KPROC)
	    {
	      send_to_formal_par (rex, TRUE, i = newlabel (), TRUE);

	      gen_thunk (rex, gen_thunk_simple_address, i);
	      break;
	    }
	  else;			/* Denne grenen skal IKKE ha break, Skal gli
				 * rett over i neste case. */
	default:

	  send_to_formal_par (rex, FALSE, i = newlabel (), TRUE);

	  gen_thunk (rex, gen_thunk_simple_value, i);

	}			/* END SWITCH */
    }				/* END-if(rex->rd->categ == CNAME) */
  else				/* FEIL */
    ;
}				/* END GENSIMPLEPAR */

/******************************************************************************
                                                              GENLABELPAREXP */

static genlabelparexp (rex, formellpar, thunk)
     struct EXP *rex,
      *formellpar;
     char thunk;
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
      (void) fprintf (ccode, "if(");
      genvalue (rex->left);
      (void) fprintf (ccode, "){");
      genlabelparexp (rex->right->left, formellpar, thunk);
      (void) fprintf (ccode, "}else{");
      genlabelparexp (rex->right->right, formellpar, thunk);
      (void) fprintf (ccode, "}");
    }
  else
    {				/* rex->token==MIDENTIFIER Hvis det ikke er
				 * tatt av en label i systemet, s} gj|res det 
				 * her, og den legges i plev attributtet */

      if (rex->token == MARRAYARG)
	{
	  (void) fprintf (ccode, "__swv=");
	  genvalue (rex->right->left);
	  (void) fprintf (ccode, ";");
	}

      if (rex->rd->idplev.plev == 0)
	rex->rd->idplev.plev = newlabel ();

      if (thunk)
	(void) fprintf (ccode, "__ev.adr.ent=");
      else
	(void) fprintf (ccode, "((__bs%d *)__pb)->%s.adr.ent=",
			formellpar->rd->encl->blno, formellpar->rd->ident);
      (void) fprintf (ccode, "%d;", rex->rd->idplev.plev);

      if (thunk)
	(void) fprintf (ccode, "__ev.adr.ment=");
      else
	(void) fprintf (ccode, "((__bs%d *)__pb)->%s.adr.ment=",
			formellpar->rd->encl->blno, formellpar->rd->ident);
      if (separat_comp)
	(void) fprintf (ccode, "__m_%s;", timestamp);
      else
	(void) fprintf (ccode, "__NULL;");

      if (thunk)
	(void) fprintf (ccode, "__er=");
      else
	(void) fprintf 
	  (ccode, "((__bs%d *)__pb)->%s.ob=", formellpar->rd->encl->blno,
	   formellpar->rd->ident);

      genchainrdencl (rex, FALSE);
      (void) fprintf (ccode, ";");
    }
}

/******************************************************************************
                                                             GEN_THUNK_LABLE */

static gen_thunk_lable (rex)
     struct EXP *rex;
{
  /* genlabelparexp skriver ut uttrykket, og tilordner ment, ent og ob for
   * hver gren i uttrykket. (if-i-uttrykk) Den skriver ogs} ut kallet for
   * reth() til slutt */
  genlabelparexp (rex->left, rex, TRUE);
}

/******************************************************************************
                                                           GENLABELSWITCHPAR */

static genlabelswitchpar (rex)
     struct EXP *rex;
{
  int i;
  if (rex->rd->categ == CNAME && rex->rd->kind != KARRAY)
    {				/* Label overf|rt by name */
      if (rex->left->token == MIDENTIFIER)
	{
	  switch (rex->left->rd->categ)
	    {
	      /* AKTUELL PARAMETER ER: */
	    case CNAME:
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s=",
			      rex->rd->encl->blno, rex->rd->ident);
	      genchainrdencl (rex->left, TRUE);
	      (void) fprintf (ccode, "%s;", rex->left->value.ident);
	      break;
	    case CVAR:
	    case CDEFLT:
	      /* Viderf|ring av en label parameter  Kopierer aktuell
	       * parameter spesifikasjon som er en formell parameter
	       * spesifikasjon i ytre en prosedyre. (ment, ent ,ob,( sl og
	       * namekind i tillegg for NAME) Setter bp, en hjelpevariabel,
	       * til } peker p} den   aktuelle parameterens blokk. Dermed
	       * blir aksessveien kortere under kopieringen. */
	      (void) fprintf (ccode, "__bp=");
	      genchainrdencl (rex->left, FALSE);
	      (void) fprintf (ccode, ";");
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.adr=",
			      rex->rd->encl->blno, rex->rd->ident);
	      (void) fprintf (ccode, "((__bs%d *)__bp)->%s.adr;",
			 rex->left->rd->encl->blno, rex->left->value.ident);
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.ob=",
			      rex->rd->encl->blno, rex->rd->ident);
	      (void) fprintf (ccode, "((__bs%d *)__bp)->%s.ob;",
			 rex->left->rd->encl->blno, rex->left->value.ident);
#if ADDNOTH
	      (void) fprintf 
		(ccode, "((__bs%d *)__pb)->%s.namekind=__ADDRESS_NOTHUNK;",
		 rex->rd->encl->blno, rex->rd->ident);
#endif
	      break;
	    case CVIRT:
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.ob=",
			      rex->rd->encl->blno, rex->rd->ident);
	      genchainrdencl (rex->left, FALSE);
	      (void) fprintf (ccode, ";");
	      /* ment og ent er gitt av virt tabellen */
	      (void) fprintf 
		(ccode, "((__bs%d *)__pb)->%s.adr=", rex->rd->encl->blno,
		 rex->rd->ident);
	      (void) fprintf 
		(ccode, "((__bs%d *)__pb)->%s.ob->pp->virtlab[%d];",
		 rex->rd->encl->blno, rex->rd->ident,
		 rex->left->rd->virtno - 1);
#if ADDNOTH
	      (void) fprintf 
		(ccode, "((__bs%d *)__pb)->%s.namekind=__ADDRESS_NOTHUNK;",
		 rex->rd->encl->blno, rex->rd->ident);
#endif
	      break;
	    case CLOCAL:
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.adr.ment=",
			      rex->rd->encl->blno, rex->rd->ident);

	      /* Bestemmer modulnavnet */

	      if (rex->left->rd->encl->externid != 0)
		(void) fprintf (ccode, "__m_%s;",
			 extract_timestamp (rex->left->rd->encl->externid));
	      else if (separat_comp)
		(void) fprintf (ccode, "__m_%s;", timestamp);
	      else
		(void) fprintf (ccode, "__NULL;");

/***** Hvis det ikke er tatt av en label i systemet, s}  ****/
/***** gj|res det her, og den legges i plev attributtet  ****/

	      if (rex->left->rd->idplev.plev == 0)
		rex->left->rd->idplev.plev = newlabel ();

	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.adr.ent=%d;",
			      rex->rd->encl->blno, rex->rd->ident,
			      rex->left->rd->idplev.plev);

	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.ob=",
			      rex->rd->encl->blno, rex->rd->ident);
	      genchainrdencl (rex->left, FALSE);
	      (void) fprintf (ccode, ";");
#if ADDNOTH
	      (void) fprintf 
		(ccode, "((__bs%d *)__pb)->%s.namekind=__ADDRESS_NOTHUNK;",
		 rex->rd->encl->blno, rex->rd->ident);
#endif
	      break;
	    }			/* END SWITCH */
	}
      else
	/* END AKTUELL ER EN IDENTIFIER */
	{
	  /* ADDRESS_THUNK */
	  send_to_formal_par (rex, TRUE, i = newlabel (), FALSE);

	  gen_thunk (rex, gen_thunk_lable, i);

	}			/* END-ADDRESS_THUNK  */
    }
  else
    /* END CNAME og LABEL */
    {
      /* FORMELL CATEG LIK CDEFLT eller CVAR for label eller       CATEG LIK
       * CDEFLT, CVAR eller CNAME for switch. Alle disse tilfellene skal
       * behandles likt. Aktuell token kan enten   v{re MIDENTIFIER eller
       * MIFE. Hvis det er MIFE, kalles     genlabelparexp som legger ut kode 
       * slik at overf|ringen    gj|res i hver gren. */

      if (rex->left->token != MIDENTIFIER)
	genlabelparexp (rex->left, rex, FALSE);
      else
	/* AKTUELL TOKEN M] DA VAERE MIDENTIFIER med categ LIK CNAME,
	 * CVAR, CDEFLT eller CNAME. */
	  switch (rex->left->rd->categ)
	  {
	  case CNAME:
	    if (rex->rd->kind != KARRAY)
	      {
		/* Label par og ikke switch par. Aktuell parameter er en name 
		 * parameter i en ytre prosedyre.       M} kalle p} gencall
		 * som genererer kode for kall p} __rgetlab() . og som
		 * returnerer med adressen i modul og ev, og objekt peker i
		 * er. */
		gencall (rex, rex->left);

		(void) fprintf (ccode, "((__bs%d *)__pb)->%s.adr=__ev.adr;",
				rex->rd->encl->blno, rex->rd->ident);
		(void) fprintf (ccode, "((__bs%d *)__pb)->%s.ob=__er;",
				rex->rd->encl->blno, rex->rd->ident);
		break;
	      }
	    else;
	    /* Denne grenen skal IKKE ha break, Skal gli rett 
	     * over i neste case. */
	  case CDEFLT:
	  case CVAR:
	    /* VIDEREF\RING AV FORMELL CDEFLT ELLER CVAR (eller NAME for
	     * switch) I EN YTRE PROSEDYRE  KOPIERER ment, ent og ob. Setter
	     * bp, en hjelpevariabel, til } peker p} den  aktuelle
	     * parameterens blokk. Dermed blir aksessveien under kopieringen. 
	     */

	    (void) fprintf (ccode, "((__bs%d *)__pb)->%s=",
			    rex->rd->encl->blno, rex->rd->ident);
	    genchainrdencl (rex->left, TRUE);
	    (void) fprintf (ccode, "%s;", rex->left->value.ident);
	    break;
	  case CVIRT:
	    (void) fprintf (ccode, "((__bs%d *)__pb)->%s.ob=",
			    rex->rd->encl->blno, rex->rd->ident);
	    genchainrdencl (rex->left, FALSE);
	    (void) fprintf (ccode, ";");
	    /* ment og ent er gitt av virt tabellen */
	    (void) fprintf 
	      (ccode, "((__bs%d *)__pb)->%s.adr=", rex->rd->encl->blno,
	       rex->rd->ident);
	    (void) fprintf 
	      (ccode, "((__bs%d *)__pb)->%s.ob->pp->virtlab[%d];",
	       rex->rd->encl->blno, rex->rd->ident,
	       rex->left->rd->virtno - 1);
	    break;
	  case CLOCAL:
	    (void) fprintf (ccode, "((__bs%d *)__pb)->%s.adr.ment=",
			    rex->rd->encl->blno, rex->rd->ident);

	    /* Bestemmer modulnavnet */

	    if (rex->left->rd->encl->externid != 0)
	      (void) fprintf (ccode, "__m_%s;",
			 extract_timestamp (rex->left->rd->encl->externid));
	    else if (separat_comp)
	      (void) fprintf (ccode, "__m_%s;", timestamp);
	    else
	      (void) fprintf (ccode, "__NULL;");

	    /* Hvis det ikke er tatt av en label i systemet, s} gj|res det
	     * her, og den legges i plev attributtet */

	    if (rex->left->rd->idplev.plev == 0)
	      rex->left->rd->idplev.plev = newlabel ();

	    (void) fprintf (ccode, "((__bs%d *)__pb)->%s.adr.ent=%d;",
			    rex->rd->encl->blno, rex->rd->ident,
			    rex->left->rd->idplev.plev);

	    (void) fprintf (ccode, "((__bs%d *)__pb)->%s.ob=",
			    rex->rd->encl->blno, rex->rd->ident);
	    genchainrdencl (rex->left, FALSE);
	    (void) fprintf (ccode, ";");
	    break;
	  }
    }				/* END IKKE LABEL NAME PARAMETER */

}

/******************************************************************************
                                                             GEN_THUNK_ARRAY */

static gen_thunk_array (rex)
     struct EXP *rex;
{
  (void) fprintf (ccode, "__er=(__dhp)");
  genvalue (rex->left);
  (void) fprintf (ccode, ";");
}

/******************************************************************************
                                                                 GENARRAYPAR */

static genarraypar (rex)
     struct EXP *rex;
{
  int i;
  switch (rex->rd->categ)
    {
    case CVALUE:
      /* V A L U E   P A R A M E T E R */
      gencall (rex, rex->left);

      (void) fprintf (ccode, "__ap=(__arrp)__rca(");
      if (rex->left->rd->categ == CNAME)
	(void) fprintf (ccode, "__er);");
      else
	{
	  gensl (rex->left, TRUE, OFF);
	  (void) fprintf (ccode, "%s);", rex->left->rd->ident);
	}
      (void) fprintf (ccode, "((__bs%d *)__pb)->%s=__ap;"
		      ,rex->rd->encl->blno, rex->rd->ident);
      break;
    case CDEFLT:
    case CVAR:
      /* STANDARD ELLER VAR OVERF\RING */
      gencall (rex, rex->left);
      (void) fprintf (ccode, "((__bs%d *)__pb)->%s=", rex->rd->encl->blno,
		      rex->rd->ident);
      if (rex->left->token == MIDENTIFIER)
	{
	  if (rex->left->rd->categ == CNAME)
	    (void) fprintf (ccode, "(__arrp)__er");
	  else
	    {
	      gensl (rex->left, TRUE, OFF);
	      (void) fprintf (ccode, "%s", rex->left->rd->ident);
	    }
	}
      else
	genvalue (rex->left);
      (void) fprintf (ccode, ";");
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

	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s=",
			      rex->rd->encl->blno, rex->rd->ident);
	      genchainrdencl (rex->left, TRUE);
	      (void) fprintf (ccode, "%s;", rex->left->value.ident);
	    }
	  else
	    {
	      /* ADDRESS_NOTHUNK */
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.ap=",
			      rex->rd->encl->blno, rex->rd->ident
		);
	      genchainrdencl (rex->left, TRUE);
	      (void) fprintf (ccode, "%s;", rex->left->rd->ident);
#if ADDNOTH
	      (void) fprintf 
		(ccode, "((__bs%d *)__pb)->%s.namekind=__ADDRESS_NOTHUNK;",
			      rex->rd->encl->blno, rex->rd->ident);
#endif
	    }
	}
      else if (rex->left->token == MDOT)
	{
	  /* ADDRESS_THUNK */
	  send_to_formal_par (rex, TRUE, i = newlabel (), FALSE);

	  gen_thunk (rex, gen_thunk_array, i);

	}			/* END ADDRESS_THUNK */
      break;
    }				/* END SWITCH */
}				/* END-GENARRAYPAR */

/******************************************************************************
                                                         GEN_THUNK_PROCEDURE */

static gen_thunk_procedure (rex)
     struct EXP *rex;
{
  (void) fprintf (ccode, "__sl=");
  if (nonetest == ON)
    (void) fprintf (ccode, "((__bp=");
  genvalue (rex->left->left);
  if (nonetest == ON)
    (void) fprintf (ccode,
		    ")==__NULL?(__dhp)__rerror(__errnone):__bp)");
  (void) fprintf (ccode, ";");

  (void) fprintf (ccode, "__pp= ");
  if (rex->left->rd->categ == CVIRT)
    (void) fprintf 
      (ccode, "__sl->pp->virt[%d];", rex->left->right->rd->virtno - 1);
  else
    {
      gen_adr_prot (rex->left->right->rd);
      (void) fprintf (ccode, ";");
    }
}

/******************************************************************************
                                                             GENPROCEDUREPAR */

static genprocedurepar (rex)
     struct EXP *rex;
{
  int i;
  /* OVERF\RING AV PROSEDYRER SOM PARAMETERE */

  if (rex->rd->categ == CNAME)
    {
      /* FORMELL NAME-PARAMETER. */

      if (rex->left->token == MIDENTIFIER)
	{
	  switch (rex->left->rd->categ)
	    {
	    case CDEFLT:
	    case CVAR:
	      /* ADDRESS_NOTHUNK  Kopiere psl (prosedyrens statiske
	       * omgivelse) og pp (prosedyrens prototype). Setter namekind og
	       * overf|rer evt. __conv og q. */

	      (void) fprintf (ccode, "__bp=");
	      genchainrdencl (rex->left, FALSE);
	      (void) fprintf (ccode, ";");
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.h.psl=",
			      rex->rd->encl->blno, rex->rd->ident);
	      (void) fprintf (ccode, "((__bs%d *)__bp)->%s.psl;",
			   rex->left->rd->encl->blno, rex->left->rd->ident);
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.h.pp=",
			      rex->rd->encl->blno, rex->rd->ident);
	      (void) fprintf (ccode, "((__bs%d *)__bp)->%s.pp;",
			   rex->left->rd->encl->blno, rex->left->rd->ident);
#if ADDNOTH
	      (void) fprintf 
		(ccode, "((__bs%d *)__pb)->%s.h.namekind=__ADDRESS_NOTHUNK;",
			      rex->rd->encl->blno, rex->rd->ident);
#endif
	      gen_conv (rex, TRUE, FALSE);
	      break;
	    case CNAME:
	      /* Videresending av NAME-par. Kopierer hele den aktuelle
	       * beskrivelsen, dvs. sl,pp,psl,adr.men, adr.ent og namekind
	       * pluss evt. __conv og q. */

	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s=",
			      rex->rd->encl->blno, rex->rd->ident);
	      genchainrdencl (rex->left, TRUE);
	      (void) fprintf (ccode, "%s;", rex->left->value.ident);
	      gen_conv (rex, TRUE, TRUE);
	      break;
	    case CVIRT:
	    case CLOCAL:
	      /* ADDRESS_NOTHUNK */
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.h.psl=",
			      rex->rd->encl->blno, rex->rd->ident);
	      gensl (rex->left, FALSE, OFF);
	      (void) fprintf (ccode, ";");
	      if (rex->left->rd->categ == CVIRT)
		{		/* M} teste under RUN-TIME om
				 * virtuell-tabbelen er null */
		  (void) fprintf 
		    (ccode, 
		     "if((__pp=((__bs%d *)__pb)->%s.h.psl->pp->virt[%d])==__NULL)",
				  rex->rd->encl->blno, rex->rd->ident,
				  rex->left->rd->virtno - 1);
		  (void) fprintf (ccode, "__rerror(__errvirt);");
		}
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.h.pp= ",
			      rex->rd->encl->blno, rex->rd->ident);
	      if (rex->left->rd->categ == CVIRT)
		(void) fprintf (ccode, "__pp;");
	      else
		{
		  gen_adr_prot (rex->left->rd);
		  (void) fprintf (ccode, ";");
		}
#if ADDNOTH
	      (void) fprintf 
		(ccode, "((__bs%d *)__pb)->%s.h.namekind=__ADDRESS_NOTHUNK;",
			      rex->rd->encl->blno, rex->rd->ident);
#endif
	      gen_conv (rex, TRUE, FALSE);
	      break;
	    }			/* END-SWITCH */
	}			/* END aktuell par.token = MIDENTIFIER */
      else
	{
	  /* Aktuell par.token = MDOT, dvs. ADDRESS_THUNK */
	  send_to_formal_par (rex, TRUE, i = newlabel (), TRUE);

	  gen_thunk (rex, gen_thunk_procedure, i);

	}			/* END ADDRESS_THUNK */
    }				/* END FORMELL NAME-PAR */
  else
    {				/* FORMELL CVAR eller CDEFLT */

      if (rex->left->token == MIDENTIFIER)
	{
	  switch (rex->left->rd->categ)
	    {
	    case CDEFLT:
	    case CVAR:
	      /* Kopiere psl (prosedyrens statiske omgivelse) og  pp
	       * (prosedyrens prototype) og overf|rer evt. __conv og q */
	      (void) fprintf (ccode, "__bp=");
	      genchainrdencl (rex->left, FALSE);
	      (void) fprintf (ccode, ";");
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.psl=",
			      rex->rd->encl->blno, rex->rd->ident);
	      (void) fprintf (ccode, "((__bs%d *)__bp)->%s.psl;",
			   rex->left->rd->encl->blno, rex->left->rd->ident);
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.pp=",
			      rex->rd->encl->blno, rex->rd->ident);
	      (void) fprintf (ccode, "((__bs%d *)__bp)->%s.pp;",
			   rex->left->rd->encl->blno, rex->left->rd->ident);
	      gen_conv (rex, TRUE, FALSE);
	      break;
	    case CNAME:
	      /* AKTUELL PARAMETER ER EN NAME-PAR I EN YTRE PROSEDYRE Kaller
	       * p} gencall som skriver ut koden for kallet __rgetproc. Den
	       * rutinen returnerer med statisk omgivelse i sl og prototypen
	       * i pp Disse overf|res til den formelle parameteren */
	      gencall (rex, rex->left);
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.psl=__sl;", 
			      rex->rd->encl->blno,
			      rex->rd->ident);
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.pp=__pp;", 
			      rex->rd->encl->blno,
			      rex->rd->ident);
	      gen_conv (rex, TRUE, FALSE);
	      break;
	    case CVIRT:
	    case CLOCAL:
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.psl=",
			      rex->rd->encl->blno, rex->rd->ident);
	      gensl (rex->left, FALSE, OFF);
	      (void) fprintf (ccode, ";");
	      if (rex->left->rd->categ == CVIRT)
		{		/* M} teste under RUN-TIME om
				 * virtuell-tabbelen er null */
		  (void) fprintf 
		    (ccode, 
		     "if((__pp=((__bs%d *)__pb)->%s.psl->pp->virt[%d])==__NULL)",
				  rex->rd->encl->blno, rex->rd->ident,
				  rex->left->rd->virtno - 1);
		  (void) fprintf (ccode, "__rerror(__errvirt);");
		}
	      (void) fprintf (ccode, "((__bs%d *)__pb)->%s.pp= ",
			      rex->rd->encl->blno, rex->rd->ident);
	      if (rex->left->rd->categ == CVIRT)
		(void) fprintf (ccode, "__pp;");
	      else
		{
		  gen_adr_prot (rex->left->rd);
		  (void) fprintf (ccode, ";");
		}
	      gen_conv (rex, TRUE, FALSE);
	      break;
	    }			/* END-SWITCH */
	}			/* END aktuell par.token = MIDENTIFIER */
      else
	{
	  /* Aktuell par.token = MDOT */
	  gencall (rex, rex->left);

	  (void) fprintf 
	    (ccode, "((__bs%d *)__pb)->%s.psl=", rex->rd->encl->blno,
			  rex->rd->ident);
	  if (nonetest == ON)
	    (void) fprintf (ccode, "((__bp=");
	  genvalue (rex->left->left);
	  if (nonetest == ON)
	    (void) fprintf (ccode,
			    ")==__NULL?(__dhp)__rerror(__errnone):__bp)");
	  (void) fprintf (ccode, ";");

	  if (rex->left->rd->categ == CVIRT)
	    {			/* M} teste under RUN-TIME om
				 * virtuell-tabbelen er null */
	      (void) fprintf 
		(ccode, 
		 "if((__pp=((__bs%d *)__pb)->%s.psl->pp->virt[%d])==__NULL)",
			      rex->rd->encl->blno, rex->rd->ident,
			      rex->left->right->rd->virtno - 1);
	      (void) fprintf (ccode, "__rerror(__errvirt);");
	    }
	  (void) fprintf (ccode, "((__bs%d *)__pb)->%s.pp= ",
			  rex->rd->encl->blno, rex->rd->ident);
	  if (rex->left->right->rd->categ == CVIRT)
	    (void) fprintf (ccode, "__pp;");
	  else
	    {
	      gen_adr_prot (rex->left->right->rd);
	      (void) fprintf (ccode, ";");
	    }
	  gen_conv (rex, TRUE, FALSE);
	}			/* END DOT'et AKTUELL PAR */
    }				/* END FORMELL.CATEG = CVAR eller CDEFLT */
}

/******************************************************************************
                                                                GENPROCPARAM */

genprocparam (re)
     struct EXP *re;
{
  struct EXP *rex;

  for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
    {

      if (rex->rd->kind == KSIMPLE)
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

genpredefproccall (ret, re, danger)
     struct EXP *ret,
       *re;
     char danger;
{
  int i;
  /* Hvis danger = TRUE s} skal returverdien legges p} stakken */
  /* Denne rutinen er da kalt fra gencall og ikke genvalue */
  
  struct EXP *rex;
  
  exinfop p;
  char properproc = FALSE;
  
  if (re->type == TNOTY)
    properproc = TRUE;
  
  p = (exinfop) re->rd->descr->hiprot;
  
  if (danger)
    {
      for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
	gencall (ret, rex->left);
      
      if (p->codeclass == CCRANDOMRUTDANGER)
	{
	  /* Leter etter siste aktuelle parameter, som */
	  /* er en NAME parameter. Det skal sjekkes om */
	  /* denne variabelen er brukt flere ganger i uttrykket */
	  for (rex = re; rex->right->token != MENDSEP; rex = rex->right);
	  (void) savepar (ret, re, rex->left->rd, 0);
	}
      
      switch (re->type)
	{
	case TREF:
	  (void) fprintf (ccode, "__r[%d]=(__dhp)", (int) (re->value.ival =
			  findallentry (ret, re, USEDREF)));
	  break;
	case TNOTY:
	  break;		/* Skal ikke forekomme som 'danger' */
	case TTEXT:
	  (void) fprintf (ccode, "__t[%d]= *", (int) (re->value.ival =
			  findallentry (ret, re, USEDTXT)));
	  break;
	case TREAL:
	  (void) fprintf (ccode, "__v[%d].f=", (int) (re->value.ival =
			  findallentry (ret, re, USEDVAL)));
	  break;
	case TINTG:
	  (void) fprintf (ccode, "__v[%d].i=", (int) (re->value.ival =
			  findallentry (ret, re, USEDVAL)));
	  break;
	default:
	  (void) fprintf (ccode, "__v[%d].c=", (int) (re->value.ival =
			  findallentry (ret, re, USEDVAL)));
	}
    }			/* END-if(danger */
  
  if (p->codeclass != CCEXIT)
    (void) fprintf (ccode, "%s(", p->rtname);
  
  switch (p->codeclass)
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

      (void) fprintf (ccode, ",");

      /* No break at end of this case */
    case CCCALLRESUME:	/* CALL  og RESUME  */
      for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
	{
	  genvalue (rex->left);
	  (void) fprintf (ccode, ",");
	}
      if (separat_comp)
#if SPLIT_MODUL
	(void) fprintf (ccode, "0,__l%d%s);", i = newlabel (), timestamp);
#else
      (void) fprintf (ccode, "%d,__m_%s);", i = newlabel (), timestamp);
#endif
      else
#if SPLIT_MODUL
	(void) fprintf (ccode, "0,__l%d);", i = newlabel ());
#else
      (void) fprintf (ccode, "%d,__NULL);", i = newlabel ());
#endif
      exitlabel (i);	/* Reentrings punkt */
      return;
      break;
    case CCEXIT:		/* TERMINATE_PROGRAM */
      if (separat_comp)
	(void) fprintf 
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
	(void) fprintf (ccode, ",");
      break;
    case CCBLANKSCOPY:
    case CCFILEBLANKSCOPY:
      gen_ant_stack (ret, re);
      if (re->right->token != MENDSEP 
	  || p->codeclass == CCFILEBLANKSCOPY)
	(void) fprintf (ccode, ",");
      if (p->codeclass == CCBLANKSCOPY)
	break;
    case CCFILEDANGER:
    case CCFILE:
      /* En av fil-prosedyrene. F|rste parameter er peker til fil
       * klasse objektet */
      gensl (re, FALSE, nonetest);
      
      if (re->right->token != MENDSEP)
	(void) fprintf (ccode, ",");
      break;
    }			/* END-SWITCH */
  
  /* Overf|rer bruker parameterene */
  
  for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
    {
      if (rex->rd->categ == CVAR)
	{
	  /* Siste parameter til random drawing */
	  /* Som er en NAME (VAR) parameter */
	  (void) fprintf (ccode, "&");
	}
      genvalue (rex->left);
      
      if (rex->right->token != MENDSEP)
	(void) fprintf (ccode, ",");
    }
  (void) fprintf (ccode, ")");
  
  if (danger)
    {
      (void) fprintf (ccode, ";");
      /* Hvis dette er et dottet kall s} skal ikke doten ses n}r */
      /* genvalue kalles. (den informasjonen er ikke n|dvendig da */

      remove_dot (&re);
      
      if (properproc)
	return;
      
      /* H}ndterer returverdiene */
      re->token = MSTACK;
    }			/* END-if(danger */
}				/* END-Genpredefproccall */


/******************************************************************************
  GETFIRSTCLASSATTRIBUT */

/* Hjelperutine som retunerer med en peker til DECL-objektet til f|rste
 * attributtet i klassen som parameteren peker p}. Den leter f|rst rekursivt
 * i prefiks klassene. Rutinen brukes under overf|ring av referanse
 * parametere til eksterne C-prosedyrer. */


static struct DECL *
getfirstclassattribut (rd) struct DECL *rd;
{
  struct DECL *rdd;
  
  if (rd->idplev.plev != 0 
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

static par_to_cproc (rex) struct EXP *rex;
{
  struct DECL *rd;
  switch (rex->rd->kind)
    {
    case KSIMPLE:
      if (rex->rd->type == TTEXT)
	{
	  if (rex->rd->categ == CVALUE)
	    {
	      /* By value, Kopierer teksten over i C-space */
	      (void) fprintf (ccode, "__rcopytexttoc(");
	      genvalue (rex->left);
	      (void) fprintf (ccode, ")");
	    }
	  else if (rex->rd->categ == CDEFLT)
	    {
	      /* By referanse, Overf|rer en peker til f|rste character. 
	       * (dette gj|res av rt-rutienen raddroffirstchar */
	      (void) fprintf (ccode, "__raddroffirstchar(");
	      genvalue (rex->left);
	      (void) fprintf (ccode, ")");
	    }
	}
      else if (rex->rd->categ == CVAR || rex->rd->categ == CNAME)
	{			/* Enkel parameter (ikke TEXT) by name */
	  (void) fprintf (ccode, "&");
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
		(void) fprintf (ccode, "__NULL");
	      else
		{
		  (void) fprintf (ccode, "&((__bs%d *)", rd->encl->blno);
		  genvalue (rex->left);
		  (void) fprintf (ccode, ")->%s", rd->ident);
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
	  (void) fprintf (ccode, "__rcopytextarrtoc(");
	  genvalue (rex->left);
	  if (rex->rd->categ == CVALUE)
	    (void) fprintf (ccode, ",__TRUE)");
	  else
	    (void) fprintf (ccode, ",__FALSE)");
	}
      else
	{
	  if (rex->rd->categ == CVALUE)
	    (void) fprintf (ccode, "__rcopyarrtoc(");
	  else
	    (void) fprintf (ccode, "__raddroffirstelem(");
	  genvalue (rex->left);
	  (void) fprintf (ccode, ")");
	}
      break;
    case KPROC:
      /* Bare lovlig } overf|re C-prosedyrer */
      (void) fprintf (ccode, "(&%s)()",
		      (rex->left->token == MDOT ?
		       ((exinfop) rex->left->right->rd->descr->hiprot)->rtname :
		       ((exinfop) rex->left->rd->descr->hiprot)->rtname));
      break;
      
    }
}



/******************************************************************************
  GENCPROCCALL      */
gencproccall (re) struct EXP *re;
{
  exinfop p;
  struct EXP *rex;
  
  p = (exinfop) re->rd->descr->hiprot;
  
  (void) fprintf (ccode, "%s(", p->rtname);
  
  /* Overf|rer parameterene */
  
  for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
    {
      par_to_cproc (rex);
      if (rex->right->token != MENDSEP)
	(void) fprintf (ccode, ",");
      
    }			/* END-OVERF\RING AV PARAMETERE */
  
  (void) fprintf (ccode, ")");
  
}

/******************************************************************************
  GENCALL      */

genctextproccall (ret, re) struct EXP *ret,
       *re;
{
  /* Eksterne C-prosedyrer av type TEXT behandles
   * ikke av genvalue siden det m} genereres et
   * tekst-objekt til } legge returteksten i */
  struct EXP *rex;
  (void) fprintf (ccode, "__ctext=%s(", ((exinfop) re->rd->descr->hiprot)->rtname);
  re->token = MSTACK;
  
  /* Overf|rer parameterene */
  for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
    {
      par_to_cproc (rex);
      if (rex->right->token != MENDSEP)
	(void) fprintf (ccode, ",");
    }
  (void) fprintf (ccode, ");");
  
  re->value.ival = findallentry (ret, re, USEDTXT);
  
  remove_dot(&re);
  
  /* Genererer et tekst-objekt ved hjelp av blanks og kopierer over
   * stringen. Kall p} blanks kan f|re til gbc. Den trenger da
   * informasjon om evt. pekere p} stakken Looktext - 1 angir antall
   * pekere p} text stakken siden det allerede er tatt av plass til det 
   * nye tekst-objektet */
  (void) fprintf (ccode, "__t[%d]= *__rblanks(", (int) re->value.ival);
  gen_ant_stack (ret, re);
  (void) fprintf (ccode, ",strlen(__ctext));");
  (void) fprintf (ccode, "(void)strcpy(__t[%d].obj->string,__ctext);", 
		  (int) re->value.ival);
}

