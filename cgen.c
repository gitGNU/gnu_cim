/* $Id: cgen.c,v 1.18 1997/01/08 09:49:17 cim Exp $ */

/* Copyright (C) 1994 Sverre Hvammen Johansen, Stein Krogdahl and Terje Mjøs
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

/* Generere C-kode for de ulike simula konstruksjoner. */

#include "cgen.h"

long last_line;

extern int option_init_poolsize,
  option_max_poolsize;
extern char option_dyn_poolsize;

char mainroutine[MAINROUTINELENGTH + 1] = "main";

extern char *includedir;
extern struct BLOCK *nextblock ();

FILE *positionarch ();

int stakk[MAXLABELSTAKK];
int cur = 0;

/* JUMP SOM ING]R I RUNTIME-SYSTEMET */
int lediglabel = STARTLABEL;

/******************************************************************************
                                                        INITGEN  & FINGEN    */

initgen ()
{
  (void) fprintf (CKOD, "/*Cim_ckod*/\n");
  if (!separat_comp)
    {
      (void) fprintf (CKOD, "int __start_data_segment=1;\n");
    }
  (void) fprintf (CKOD, "#include \"%s%scim.h\"\n", includedir, DIR_MARKER);
  (void) fprintf (CKOD, "#include \"%s\"\n", hkodnavn);
  if (separat_comp)
    {
      (void) fprintf (CKOD, "void __m_%s()\n{", timestamp);
      if (!option_line)
	(void) fprintf (CKOD, "__curent_map=__map%s;", timestamp);
      (void) fprintf (CKOD, "goto __s;\n");
    }
  else
    {
      (void) fprintf (CKOD, "%s(__argc,__argv)int __argc;char *__argv[];\n{\n"
		      ,mainroutine);
      if (option_init_poolsize)
	(void) fprintf (CKOD, "__poolsize= %d;\n",
			option_init_poolsize);
      if (option_max_poolsize)
	(void) fprintf (CKOD, "__maxsize= %d;\n",
			option_max_poolsize);
      if (option_dyn_poolsize)
	(void) fprintf (CKOD, "__dynsize= __TRUE;\n");

      (void) fprintf (CKOD, "goto __sw;\n__l0:__rstart(__argc,__argv);\n");

      (void) fprintf (CKOD, "goto __start;\n__l1:__slutt:\n__rslutt();");

      (void) fprintf (CKOD, "exit(%d);\n__start:\n", SH_TRUE);
      if (!option_line)
	(void) fprintf (CKOD, "__curent_map=__mapmain;\n");
    }
#if SPLIT_MODUL
  (void) fprintf (CKOD, "__first%s();\n", timestamp);
#else
}
fingen ()
{
#endif
  if (!separat_comp)
    {
#if SPLIT_MODUL
#else
      (void) fprintf (CKOD, "goto __slutt;\n");
#endif
    }
  typeswitch ();
  if (!separat_comp)
    (void) fprintf (CKOD,
		    "__goto=__return;\nreturn(__vreturn);");
  (void) fprintf (CKOD, "}\n");
#if SPLIT_MODUL
  (void) fprintf (CKOD, "__first%s(){\n", timestamp);
}
fingen ()
{
  (void) fprintf (CKOD, "__goto.ent=15;__goto.ment=__NULL;}\n");
#endif
  structure ();
}
/******************************************************************************
                             NEWLABEL & TYPELABEL & EXITLABEL & GOTOLABEL    */

int 
newlabel ()
{
  return (lediglabel++);
}

typelabel (l)
     int l;
{
#if SPLIT_MODUL
  (void) fprintf (CKOD, "__goto.ment= __l%d%s;}\n__l%d%s(){\n", l, timestamp
		  ,l, timestamp);
  (void) fprintf (HKOD, "__l%d%s();\n", l, timestamp);
#else
  (void) fprintf (CKOD, "__l%d:\n", l);
#endif
  not_reached = FALSE;
  last_line = 0;
  genline ();
}

gotoswitch ()
{
#if SPLIT_MODUL
  (void) fprintf (CKOD, "return;\n");
#else
  (void) fprintf (CKOD, "goto __sw;\n");
#endif
}

exitlabel (l)
     int l;
{
#if SPLIT_MODUL
  (void) fprintf (CKOD, "}\n__l%d%s(){\n", l, timestamp);
  (void) fprintf (HKOD, "__l%d%s();\n", l, timestamp);
#else
  (void) fprintf (CKOD, "goto __sw;__l%d:\n", l);
#endif
  not_reached = FALSE;
  last_line = 0;
  genline ();
}
exitcondlabel (l)
     int l;
{
#if SPLIT_MODUL
  (void) fprintf (CKOD, ";else __goto.ment=__l%d%s;}\n__l%d%s(){\n"
		  ,l, timestamp, l, timestamp);
  (void) fprintf (HKOD, "__l%d%s();\n", l, timestamp);
#else
  (void) fprintf (CKOD, "goto __sw;__l%d:\n", l);
#endif
  not_reached = FALSE;
  last_line = 0;
  genline ();
}
gotolabel (l)
     int l;
{
#if SPLIT_MODUL
  (void) fprintf (CKOD, "{__goto.ment= __l%d%s;return;}\n", l, timestamp);
#else
  (void) fprintf (CKOD, "goto __l%d;\n", l);
#endif
}

/******************************************************************************
                                                                TYPESWITCH   */

typeswitch ()
{
  int i;
  if (separat_comp)
    {
      (void) fprintf (CKOD, "__sw:if(__goto.ment!=(void (*)())__m_%s)return;\n", timestamp);
      i = STARTLABEL;		/* Skal ikke skrive ut hopp til labler for
				 * omgivelsene. */
    }
  else
    {
      (void) fprintf (CKOD, "__sw:if(__goto.ment!=__NULL)");
      (void) fprintf (CKOD, "   {\n do (*__goto.ment)();\n while(__goto.ment!=__NULL);\n");
      if (!option_line)
	(void) fprintf (CKOD, "     __curent_map=__mapmain;");
      (void) fprintf (CKOD, "}\n");
      i = 0;			/* Skal skrive ut hopp til labler for
				 * omgivelsene.               */
    }
  (void) fprintf (CKOD, "__s:switch(__goto.ent){\n");
  for (; i < lediglabel; i++)
    {
      (void) fprintf (CKOD, "case %d: goto __l%d;", i, i);
      if (i / 3 * 3 == i - 2)
	(void) fprintf (CKOD, "\n");
    }
  if (i >= maxantcase)
    maxantcase = i + 1;
  (void) fprintf (CKOD, "}\n");
}

/******************************************************************************
                                        NEWLLABEL & TYPELLABEL & GOTOLLABEL  */

/* LOKALE HOPP */
int ledigllabel = 0;

int 
newllabel ()
{
  return (ledigllabel++);
}

typellabel (l)
     int l;
{
#if SPLIT_MODUL
  (void) fprintf (CKOD, "__goto.ment= __ll%d%s;}\n__ll%d%s(){\n", l, timestamp
		  ,l, timestamp);
  (void) fprintf (HKOD, "__ll%d%s();\n", l, timestamp);
#else
  (void) fprintf (CKOD, "__ll%d:\n", l);
#endif
  not_reached = FALSE;
  last_line = 0;
}

gotollabel (l)
     int l;
{
#if SPLIT_MODUL
  (void) fprintf (CKOD, "{__goto.ment= __ll%d%s;return;}\n", l, timestamp);
#else
  (void) fprintf (CKOD, "goto __ll%d;\n", l);
#endif
}


/******************************************************************************
                                                        GENLINE              */

genline ()
{
  if (!option_line && last_line != yylineno)
    {
      fprintf (CKOD, "__cline=%ld;", yylineno);
      if (option_gen_trace)
	(void) fprintf (CKOD, "__rtrace();");
    }
  last_line = yylineno;
#ifdef DEBUG
  if (option_gen_test)
    (void) fprintf (CKOD, "__rtest();");
#endif
}


/******************************************************************************
                                                        SKRIVENDDECL         */

skrivenddecl ()
{				/* Hvis vi er inne i en klasse og er ferdig
				 * med deklarasjonene, s} skal renddecl
				 * skrives ut. Dette gj|res evt. av denne
				 * prosedyren. */
  if (!cblock->enddecl && (cblock->quant.kind == KCLASS 
			   || cblock->quant.kind == KPRBLK))
    {
      (void) fprintf (CKOD, "__renddecl(%d);", cblock->quant.idplev.plev);
      gotoswitch ();
      typelabel (pop);
      cblock->enddecl = TRUE;
    }
}

/******************************************************************************
                                                        GBLOCK & GENDBLOCK   */

gblock ()
{
  genline ();
  if (cblock->stat)
    (void) fprintf (CKOD, "__sto= (__dhp)&__blokk%d%s;", cblock->blno
		    ,timestamp);
  (void) fprintf (CKOD, "__rb(");
  gen_adr_prot (&cblock->quant);
  (void) fprintf (CKOD, ");\n");
#if 0
  typelabel (cblock->ent = newlabel ());
#endif
}

gendblock ()
{
  if (not_reached == FALSE)
    (void) fprintf (CKOD, "__rbe();\n");
}

/******************************************************************************
                                                GPRBLOCK & GENDPRBLOCK       */

gprblock (re)
     struct EXP *re;
{
  if (cblock->stat)
    (void) fprintf (CKOD, "__sto= (__dhp)&__blokk%d%s;", cblock->blno
		    ,timestamp);
  (void) fprintf (CKOD, "__rcprb(");
  gen_adr_prot (&cblock->quant);
  (void) fprintf (CKOD, ");\n");
  /* Sender parameterne */
  gencall (re->up, re);
  if (separat_comp)
#if SPLIT_MODUL
    (void) fprintf (CKOD, "__rcprbb(0,__l%d%s);", 
		    push = newlabel (), timestamp);
#else
    (void) fprintf (CKOD, "__rcprbb(%d,__m_%s);", 
		    push = newlabel (), timestamp);
#endif
  else
#if SPLIT_MODUL
    (void) fprintf (CKOD, "__rcprbb(0,__l%d);", push = newlabel ());
#else
    (void) fprintf (CKOD, "__rcprbb(%d,__NULL);", push = newlabel ());
#endif
  exitlabel (cblock->ent = newlabel ());
  push = newlabel ();

}

gendprblock ()
{
  if (not_reached == FALSE)
    (void) fprintf (CKOD, "__rendclass(%d);", cblock->quant.idplev.plev);
  exitlabel (pop);
}

/******************************************************************************
                                                GPROCEDURE& GENDPROCEDURE   */

gprocedure ()
{
  struct DECL *rd;
  int i;
  (void) fprintf (CKOD, "/* START PROCEDURE %s */\n", cblock->quant.ident);
  gotollabel (push = newllabel ());
  typelabel (cblock->ent = newlabel ());
  rd = cblock->parloc;
  for(i = 1; i <= cblock->napar; i++)
    {
      if (rd->kind == KSIMPLE && rd->type == TREF && rd->categ != CNAME)
	{
	  (void) fprintf (CKOD, "(((__bp=((__bs%d *)__lb)->%s"
			  ,rd->encl->blno,rd->ident);
	  (void) fprintf (CKOD, ")!=__NULL && (");
	  if (rd->prefqual->idplev.plev >= DEF_PLEV_TAB_SIZE)
	    (void) fprintf (CKOD, "__bp->pp->plev<%d || ",
			    rd->prefqual->idplev.plev);
	  (void) fprintf (CKOD, "__bp->pp->pref[%d]!= ",
			  rd->prefqual->idplev.plev);
	  gen_adr_prot (rd->prefqual);
	  (void) fprintf (CKOD, "))?(__dhp)__rerror(__errqual):__bp);\n");
	}
      rd = rd->next;
    }
}

gendprocedure ()
{
  if (not_reached == FALSE)
    {
      genline ();
      if (cblock->quant.type != TNOTY)
	{
	  if (cblock->quant.type == TREF)
	    (void) fprintf (CKOD, "__er=((__bs%d *)__lb)->er;\n", 
			    cblock->blno);
	  else if (cblock->quant.type == TTEXT)
	    (void) fprintf 
	      (CKOD, "__et=((__bs%d *)__lb)->et;\n", 
	       cblock->blno);
	  else if (cblock->quant.type == TREAL)
	    (void) fprintf 
	      (CKOD, "__ev.f=((__bs%d *)__lb)->ef;\n", cblock->blno);
	  else if (cblock->quant.type == TINTG)
	    (void) fprintf (CKOD, "__ev.i=((__bs%d *)__lb)->ev;\n", 
			    cblock->blno);
	  else
	    (void) fprintf (CKOD, "__ev.c=((__bs%d *)__lb)->ec;\n", 
			    cblock->blno);
	  (void) fprintf (CKOD, "__rep();\n");
	}
      else
	(void) fprintf (CKOD, "__repp();");
      gotoswitch ();
    }
  typellabel (pop);
  (void) fprintf (CKOD, "/* SLUTT PROCEDURE %s */\n", cblock->quant.ident);
}
/******************************************************************************
                                                        GCLASS & GENDCLASS   */

gclass ()
{
  int i;
  (void) fprintf (CKOD, "/* START CLASS %s */\n", cblock->quant.ident);
  gotollabel (push = newllabel ());
  typelabel (cblock->ent = newlabel ());
  i = newlabel ();		/* Label etter dekl. del */
  push = newlabel ();		/* Etter kall p} rinner  */
  push = i;
  /* Disse labelene m} tas av her fordi label etter dekl.   m} v}re
   * entringspunkt + 1 og etter rinner m} v{re entringspunkt + 2. (Hvis
   * det er prosedyre kall i setningene i klassen s} vil flere labler
   * legges ut.) */
}

gendclass ()
{
  if (1 /* not_reached==FALSE */ )
    {
      genline ();
      if (!cblock->inner)
	{
	  (void) fprintf (CKOD, "__rinner(%d);", cblock->quant.idplev.plev);
	  gotoswitch ();
	  typelabel (pop);
	}
      (void) fprintf (CKOD, "__rendclass(%d);", cblock->quant.idplev.plev);
      gotoswitch ();
    }
  else if (!cblock->inner)
    {
      typelabel (pop);
      (void) fprintf (CKOD, "__rendclass(%d);", cblock->quant.idplev.plev);
      gotoswitch ();
    }
  typellabel (pop);
  (void) fprintf (CKOD, "/* SLUTT CLASS %s */\n", cblock->quant.ident);
}

/******************************************************************************
                                                GINSPECT & GENDINSPECT       */

ginspect (re)
     struct EXP *re;
{
  struct DECL *rd;
  gencall (re->up, re);
  cblock->naloc = FALSE;	/* Brukes som flagg for om pp er satt   */

  (void) fprintf (CKOD, "((__bs%d *)__lb)->c%d=", (rd = cblock->quant.match)->descr->blno,
		  cblock->connest);
  genvalue (re);
  (void) fprintf (CKOD, ";\n");

  (void) fprintf (CKOD, "if(((__bs%d *)__lb)->c%d==__NULL)", rd->descr->blno, cblock->connest);



  gotollabel (push = newllabel ());
  push = newllabel ();
}

gendinspect ()
{
  /* Denne kalles bare hvis det ikke er otherwise gren.Hvis det er otherwise
   * gren s} vil m-symbolet etter MENDOTHERWISE v{re MENDINSPECT.Det
   * som da skal gj|res ved MENDINPECT kan da gj|res av gendotherwise() */

  typellabel (pop);
  typellabel (pop);
  cblock->naloc = FALSE;
}

/******************************************************************************
                                                             GDO &    GENDDO */

gdo ()
{
}

genddo ()
{
  if (not_reached == FALSE)
    {
      genline ();
      gotollabel (look);
    }
}

/******************************************************************************
                                                        GWHEN & GENDWHEN     */

gwhen (rdi)
     struct DECL *rdi;
{
  if (!cblock->quant.prefqual->descr->naloc)	/* pp er ikke satt */
    {
      (void) fprintf (CKOD, "if((__pp=((__bs%d *)__lb)->c%d->pp)->plev < %d ",
       cblock->quant.match->descr->blno, cblock->connest, rdi->idplev.plev);
      cblock->quant.prefqual->descr->naloc = TRUE;
    }
  else
    (void) fprintf (CKOD, "if(__pp->plev < %d ", rdi->idplev.plev);

  (void) fprintf (CKOD, "|| __pp->pref[%d]!= ", rdi->idplev.plev);
  gen_adr_prot (rdi);
  (void) fprintf (CKOD, ")");

  gotollabel (push = newllabel ());
}

gendwhen ()
{
  int l;
  if (not_reached == FALSE)
    {
      genline ();
      l = pop;
      gotollabel (look);
    }
  else
    l = pop;
  typellabel (l);
}

/******************************************************************************
                                                GOTHERWISE & GENDOTHERWISE   */

gotherwise ()
{
  int l;
  l = pop;
  typellabel (pop);
  push = l;
}

gendotherwise ()
{
  if (not_reached == FALSE)
    genline ();
  typellabel (pop);
}

/******************************************************************************
                                                        GFOROPT              */
/* Genererer kode for en for l|kke med bare ett liste element                */
gforopt (re)
     struct EXP *re;
{
  int l1,
    l2,
    konst_step,
    notnegativ;
  struct EXP *rex;
  struct EXP *re2;

  rex = re->right;

  switch (rex->left->token)
    {
    case MSTEP:
      re2 = copytree (re);
      gencall (rex->left, rex->left->left);
      genvalue (re->left);
      (void) fprintf (CKOD, "=");
      genvalue (rex->left->left);
      (void) fprintf (CKOD, ";\n");
      gencall (rex->left->right, rex->left->right->left);
      gencall (rex->left->right, rex->left->right->right);

      notnegativ = !(rex->left->right->left->token == MUSUBI ||
		     (rex->left->right->left->token == MINTEGERKONST &&
		      rex->left->right->left->value.ival < 0));
      konst_step = rex->left->right->left->token == MINTEGERKONST;
      (void) fprintf (CKOD, "if((");
      if (konst_step)
	(void) fprintf (CKOD, "(");
      else
	{
	  if (notnegativ)
	    genvalue (rex->left->right->left);
	  else
	    genvalue (rex->left->right->left->left);
	  (void) fprintf (CKOD, "*(");
	}
#if 0
      genvalue (re->left);

      (void) fprintf (CKOD, " - ");
      genvalue (rex->left->right->right);
#else
#if 0
      genvalue (re->left);

      (void) fprintf (CKOD, " > ");
      genvalue (rex->left->right->right);
      (void) fprintf (CKOD, " ?1:( ");
      genvalue (re->left);

      (void) fprintf (CKOD, " == ");
      genvalue (rex->left->right->right);
      (void) fprintf (CKOD, " ?0: -1) ");
#else
      if(re->type==TINTG)fprintf(CKOD, "__rsigndi(");
        else fprintf(CKOD, "__rsigndr(");
      genvalue (re->left);

      (void) fprintf (CKOD, " , ");
      genvalue (rex->left->right->right);
      (void) fprintf (CKOD, ")");
#endif
#endif
      if (!notnegativ)
	(void) fprintf (CKOD, ")) < 0)");
      else
	(void) fprintf (CKOD, ")) > 0)");
      gotollabel (push = newllabel ());
      gotollabel (l1 = newllabel ());
      typellabel (l2 = newllabel ());
      re = re2;
      rex = re2->right;
      gencall (rex->left->right, rex->left->right->left);
      gencall (rex->left->right, rex->left->right->right);
      genvalue (re->left);
      (void) fprintf (CKOD, " += ");
      if (!konst_step)
	genvalue (rex->left->right->left);
      else
	genvalue (rex->left->right->left);
      (void) fprintf (CKOD, ";\n");
      (void) fprintf (CKOD, "if((");
      if (konst_step)
	(void) fprintf (CKOD, "(");
      else
	{
	  if (notnegativ)
	    genvalue (rex->left->right->left);
	  else
	    genvalue (rex->left->right->left->left);
	  (void) fprintf (CKOD, "*(");
	}
#if 0
      genvalue (re->left);
      (void) fprintf (CKOD, " - ");
      genvalue (rex->left->right->right);
#else
#if 0
      genvalue (re->left);

      (void) fprintf (CKOD, " > ");
      genvalue (rex->left->right->right);
      (void) fprintf (CKOD, " ?1:( ");
      genvalue (re->left);

      (void) fprintf (CKOD, " == ");
      genvalue (rex->left->right->right);
      (void) fprintf (CKOD, " ?0: -1) ");
#else
      if(re->type==TINTG)
	if(rex->left->right->right->type==TREAL)
	  fprintf(CKOD, "__rsigndx(");
	  else fprintf(CKOD, "__rsigndi(");
        else fprintf(CKOD, "__rsigndr(");
      genvalue (re->left);
      (void) fprintf (CKOD, " , ");
      genvalue (rex->left->right->right);
      (void) fprintf (CKOD, ")");
#endif
#endif
      if (!notnegativ)
	(void) fprintf (CKOD, ")) < 0)");
      else
	(void) fprintf (CKOD, ")) > 0)");
      gotollabel (look);
      push = l2;
      push = l1;
      break;
    case MFORWHILE:
      typellabel (l1 = newllabel ());
      gencall (rex->left, rex->left->left);
      if (re->left->type == TTEXT)
	{
	  if (re->token == MFOR)
	    (void) fprintf (CKOD, "__rtextvalassign(");
	  else
	    (void) fprintf (CKOD, "__rtextassign(");
	  genvalue (re->left);
	  (void) fprintf (CKOD, ",");
	  genvalue (rex->left->left);
	  (void) fprintf (CKOD, ");");
	}
      else
	{
	  genvalue (re->left);
	  (void) fprintf (CKOD, "=");
	  genvalue (rex->left->left);
	  (void) fprintf (CKOD, ";");
	}
      gencall (rex->left, rex->left->right);
      (void) fprintf (CKOD, "if(!");
      genvalue (rex->left->right);
      (void) fprintf (CKOD, ")");
      gotollabel (push = newllabel ());
      push = l1;
      push = newllabel ();

      break;
    default:
      gencall (rex, rex->left);
      if (re->left->type == TTEXT)
	{
	  if (re->token == MFOR)
	    (void) fprintf (CKOD, "__rtextvalassign(");
	  else
	    (void) fprintf (CKOD, "__rtextassign(");
	  genvalue (re->left);
	  (void) fprintf (CKOD, ",");
	  genvalue (rex->left);
	  (void) fprintf (CKOD, ");\n");
	}
      else
	{
	  genvalue (re->left);
	  (void) fprintf (CKOD, "=");
	  genvalue (rex->left);
	  (void) fprintf (CKOD, ";\n");
	}
      l1 = newllabel ();
      push = l2 = newllabel ();
      push = l2;
      push = l1;
    }

}

/******************************************************************************
                                                        GFOR                 */
/* Genererer kode for en for l|kke med flere liste elementer                 */
gfor (re)
     struct EXP *re;
{
  struct EXP *rex;
  struct EXP *rey;
  struct EXP *re2;

  int l1,
    l2,
    listnr,
    notnegativ,
    konst_step;
  struct DECL *rd;

  /* Hvis for l|kka bare har en i listen kalles gforopt som er en */
  /* optimalisering for dette                                     */
  if (re->right->right->token == MENDSEP)
    {
      gforopt (re);
      return;
    }
  l1 = cblock->quant.idplev.plev;
  (void) fprintf (CKOD, "((__bs%d *)__lb)->f%d=1;\n", 
		  (rd = cblock->quant.match)->descr->blno,
		  cblock->fornest);

  typellabel (l1 = newllabel ());	/* Etter forl|kka skal det hoppes til 
					 * l1: */
  l2 = newllabel ();		/* l2 er labelen til for blokken         */
  (void) fprintf (CKOD, "switch (((__bs%d *)__lb)->f%d)\n", rd->descr->blno,
		  cblock->fornest);
  (void) fprintf (CKOD, "{\n");
  listnr = 0;


  for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
    {
      (void) fprintf (CKOD, "\n   case %d:\n\n", ++listnr);
      rey = rex->left;
      switch (rey->token)
	{
	case MSTEP:
	  re2 = copytree (rey);
	  gencall (rey, rey->left);
	  genvalue (re->left);
	  (void) fprintf (CKOD, "=");
	  genvalue (rey->left);
	  (void) fprintf (CKOD, ";");
	  (void) fprintf (CKOD, "((__bs%d *)__lb)->f%d++;\n", rd->descr->blno,
			  cblock->fornest);
	  gencall (rey->right, rey->right->left);
	  gencall (rey->right, rey->right->right);

	  notnegativ = !(rey->right->left->token == MUSUBI ||
			 (rey->right->left->token == MINTEGERKONST &&
			  rey->right->left->value.ival < 0));
	  konst_step = rey->right->left->token == MINTEGERKONST;
	  (void) fprintf (CKOD, "if((");
	  if (konst_step)
	    (void) fprintf (CKOD, "(");
	  else
	    {
	      if (notnegativ)
		genvalue (rey->right->left);
	      else
		genvalue (rey->right->left->left);
	      (void) fprintf (CKOD, "*(");
	    }
#if 0
	  genvalue (re->left);
	  (void) fprintf (CKOD, " - ");
	  genvalue (rey->right->right);
#else
#if 0
          genvalue (re->left);

          (void) fprintf (CKOD, " > ");
          genvalue (rey->right->right);
          (void) fprintf (CKOD, " ?1:( ");
          genvalue (re->left);
          (void) fprintf (CKOD, " == ");
          genvalue (rey->right->right);
          (void) fprintf (CKOD, " ?0: -1) ");
#else
          if(re->type==TINTG)
	    if(rex->left->right->right->type==TREAL)
	      fprintf(CKOD, "__rsigndx(");
	      else fprintf(CKOD, "__rsigndi(");
            else fprintf(CKOD, "__rsigndr(");
	  genvalue (re->left);
	  (void) fprintf (CKOD, " , ");
	  genvalue (rey->right->right);
	  (void) fprintf (CKOD, ")");
#endif
#endif
	  if (!notnegativ)
	    (void) fprintf (CKOD, ")) >= 0)");
	  else
	    (void) fprintf (CKOD, ")) <= 0)");
	  gotollabel (l2);
	  (void) fprintf (CKOD, "((__bs%d *)__lb)->f%d++;", 
			  rd->descr->blno, cblock->fornest);
	  gotollabel (l1);
	  (void) fprintf (CKOD, "\n   case %d:\n\n", ++listnr);
	  rey = re2;
	  gencall (rey->right, rey->right->left);
	  gencall (rey->right, rey->right->right);
	  genvalue (re->left);
	  (void) fprintf (CKOD, " += ");
	  if (!konst_step)
	    genvalue (rey->right->left);
	  else
	    genvalue (rey->right->left);
	  (void) fprintf (CKOD, ";");
	  (void) fprintf (CKOD, "if((");
	  if (konst_step)
	    (void) fprintf (CKOD, "(");
	  else
	    {
	      if (notnegativ)
		genvalue (rey->right->left);
	      else
		genvalue (rey->right->left->left);
	      (void) fprintf (CKOD, "*(");
	    }
#if 0
	  genvalue (re->left);
	  (void) fprintf (CKOD, " - ");
	  genvalue (rey->right->right);
#else
#if 0
          genvalue (re->left);

          (void) fprintf (CKOD, " > ");
          genvalue (rey->right->right);
          (void) fprintf (CKOD, " ?1:( ");
          genvalue (re->left);
          (void) fprintf (CKOD, " == ");
          genvalue (rey->right->right);
          (void) fprintf (CKOD, " ?0: -1) ");
#else
          if(re->type==TINTG)
	    if(rex->left->right->right->type==TREAL)
	      fprintf(CKOD, "__rsigndx(");
	      else fprintf(CKOD, "__rsigndi(");
            else fprintf(CKOD, "__rsigndr(");
	  genvalue (re->left);
	  (void) fprintf (CKOD, " , ");
	  genvalue (rey->right->right);
	  (void) fprintf (CKOD, ")");
#endif
#endif
	  if (!notnegativ)
	    (void) fprintf (CKOD, ")) >= 0)");
	  else
	    (void) fprintf (CKOD, ")) <= 0)");
	  gotollabel (l2);
	  (void) fprintf (CKOD, "((__bs%d *)__lb)->f%d++;\n", 
			  rd->descr->blno, cblock->fornest);
	  break;
	case MFORWHILE:
	  gencall (rey, rey->left);
	  if (re->left->type == TTEXT)
	    {
	      if (re->token == MFOR)
		(void) fprintf (CKOD, "__rtextvalassign(");
	      else
		(void) fprintf (CKOD, "__rtextassign(");
	      genvalue (re->left);
	      (void) fprintf (CKOD, ",");
	      genvalue (rey->left);
	      (void) fprintf (CKOD, ");");
	    }
	  else
	    {
	      genvalue (re->left);
	      (void) fprintf (CKOD, "=");
	      genvalue (rey->left);
	      (void) fprintf (CKOD, ";");
	    }
	  gencall (rey, rey->right);
	  (void) fprintf (CKOD, "if(");
	  genvalue (rey->right);
	  (void) fprintf (CKOD, ")");
	  gotollabel (l2);
	  (void) fprintf (CKOD, "((__bs%d *)__lb)->f%d++;\n", 
			  rd->descr->blno, cblock->fornest);
	  break;
	default:
	  gencall (rex, rey);
	  if (re->left->type == TTEXT)
	    {
	      if (re->token == MFOR)
		(void) fprintf (CKOD, "__rtextvalassign(");
	      else
		(void) fprintf (CKOD, "__rtextassign(");
	      genvalue (re->left);
	      (void) fprintf (CKOD, ",");
	      genvalue (rey);
	      (void) fprintf (CKOD, ");");
	    }
	  else
	    {
	      genvalue (re->left);
	      (void) fprintf (CKOD, "=");
	      genvalue (rey);
	      (void) fprintf (CKOD, ";");
	    }
	  (void) fprintf (CKOD, "((__bs%d *)__lb)->f%d++;", rd->descr->blno,
			  cblock->fornest);
	  gotollabel (l2);
	}
    }
  if (listnr >= maxantcase)
    maxantcase = listnr + 1;
  (void) fprintf (CKOD, "\n   default:");
  gotollabel (push = newllabel ());
  (void) fprintf (CKOD, "}\n");
  push = l1;
  push = l2;

}

/******************************************************************************
                                                        GFORDO & GENDFORDO   */

gfordo ()
{
  typellabel (pop);
}

gendfordo ()
{
  if (not_reached == FALSE)
    {
      genline ();
      gotollabel (pop);
    }
  else
    pop;
  typellabel (pop);
}

/******************************************************************************
                                                        GWHILE & GENDWHILE   */

gwhile (re)
     struct EXP *re;
{
  typellabel (push = newllabel ());
  gencall (re->up, re);
  (void) fprintf (CKOD, "if(!(");
  genvalue (re);
  (void) fprintf (CKOD, "))");
  gotollabel (push = newllabel ());
}

gendwhile ()
{
  int i;
  if (not_reached == FALSE)
    {
      genline ();
      i = pop;
      gotollabel (pop);
    }
  else
    {
      i = pop;
      pop;
    }
  typellabel (i);
}

/******************************************************************************
                                                        GIF & GELSE & GENDIF */

gif (re)
     struct EXP *re;
{
  gencall (re->up, re);
  (void) fprintf (CKOD, "if(!(");
  genvalue (re);
  (void) fprintf (CKOD, "))");
  gotollabel (push = newllabel ());
}

gelse ()
{
  int i;
  i = look;
  if (not_reached == FALSE)
    {
      genline ();
      gotollabel (look = newllabel ());
    }
  else
    look = newllabel ();
  typellabel (i);
}

gendif ()
{
  if (not_reached == FALSE)
    genline ();
  typellabel (pop);
}

/******************************************************************************
                                                                GLABEL       */

glabel (re)
     struct EXP *re;
{
  (void) fprintf (CKOD, "/*%s_%d*/", re->value.ident, re->rd->encl->blno);
  if (re->rd->categ == CVIRT)
    {
      if (re->rd->match->idplev.plev == 0)
	re->rd->match->idplev.plev = newlabel ();
    }
  else
    {
      if (re->rd->idplev.plev == 0)
	re->rd->idplev.plev = newlabel ();
    }
  typelabel ((re->rd->categ == CVIRT ? re->rd->match : re->rd)->idplev.plev);
}

/******************************************************************************
                                                                GGOTO        */

ggoto (re)
     struct EXP *re;
{
  gencall (re->up, re);
  genvalue (re);
}

/******************************************************************************
                                                                GINNER       */

ginner ()
{
  genline ();
  (void) fprintf (CKOD, "__rinner(%d);", cblock->quant.idplev.plev);
  gotoswitch ();
  typelabel (pop);
}

/******************************************************************************
                                                                GENARRAY     */

genarray (re)
     struct EXP *re;
{
  struct EXP *re1,
   *re2;
  int i;
  char ifskrevet = FALSE;
  /* Sjekke at re!=NULL */

  gencall (re, re->right);

  for (re1 = re->left; re1->token != MENDSEP; re1 = re1->right)
    {
      (void) fprintf (CKOD, "        /* Array %s  */\n", 
		      re1->left->value.ident);
      /* Legger inn kode som sjekker at ovre grense > nedre grense Hvis dette 
       * er en deklarasjon av flere array,  f.eks integer array a,b(1:10), s} 
       * er det ikke n|dvendig } foreta sjekkingen av grensene mer enn en
       * gang. */
      if (re1->up == re)
	{
	  for (re2 = re->right; re2->token != MENDSEP; re2 = re2->right)
	    {
	      if ((MINTEGERKONST == (
				      re2->left->left->token == MUSUBI 
				     ? re2->left->left->left->token :
				      re2->left->left->token)) &&
		  (MINTEGERKONST == (
				      re2->left->right->token == MUSUBI 
				     ? re2->left->right->left->token :
				      re2->left->right->token)))
		{		/* KONSTANTER (kan ogs} v{re med minus foran
				 * f.eks -3:10 Kompilatoren sjekker disse */
		  if ((re2->left->left->token == MUSUBI ?
		       (-1) * re2->left->left->left->value.ival :
		       re2->left->left->value.ival) >
		      (re2->left->right->token == MUSUBI ?
		       (-1) * re2->left->right->left->value.ival :
		       re2->left->right->value.ival)+1)

		    gerror (82, re1->left->rd->ident);
		}
	      else
		{
		  if (!ifskrevet)
		    {
		      (void) fprintf (CKOD, "if(");
		      ifskrevet = TRUE;
		    }
		  else
		    (void) fprintf (CKOD, " || ");

		  (void) fprintf (CKOD, "(");
		  genvalue (re2->left->left);
		  (void) fprintf (CKOD, " > ");
		  genvalue (re2->left->right);
		  (void) fprintf (CKOD, "+1)");

		}
	    }
	  if (ifskrevet)
	    (void) fprintf (CKOD, ")__rerror(__errlim);\n");
	}
      /* Generer arrayobjektet */
      (void) fprintf (CKOD, "__ap=(__arrp)__ralloc((long)");
      (void) fprintf (CKOD, "sizeof(__ah)+(sizeof(__arrlimit)*%d)+(sizeof(",
		      re1->left->rd->dim);
      gentype (re1->left);
      (void) fprintf (CKOD, ")");
      for (re2 = re->right; re2->token != MENDSEP; re2 = re2->right)
	{
	  (void) fprintf (CKOD, "*(");
	  genvalue (re2->left->right);
	  (void) fprintf (CKOD, " - ");
	  genvalue (re2->left->left);
	  (void) fprintf (CKOD, "+1)");
	}
      (void) fprintf (CKOD, "));");
      (void) fprintf (CKOD, "((__bs%d *)__pb)->%s=__ap;\n"
		      ,re1->left->rd->encl->blno
		      ,re1->left->value.ident);
      /* Initserer objektet -> Setter nedre  bounds og denne dimmensjonens
       * size */
      i = 0;
      for (re2 = re->right; re2->token != MENDSEP; re2 = re2->right)
	{
	  (void) fprintf (CKOD, "__ap->limits[%d].low=", i);
	  genvalue (re2->left->left);
	  (void) fprintf (CKOD, ";\n");
	  (void) fprintf (CKOD, "__ap->limits[%d].size=1+", i++);
	  genvalue (re2->left->right);
	  (void) fprintf (CKOD, " - ");
	  genvalue (re2->left->left);
	  (void) fprintf (CKOD, ";\n");
	}
      /* -> Merker at det er et ARRAY */
      (void) fprintf (CKOD, "__ap->h.pp=(__pty)__ARRAY;\n");
      /* -> SIZE */
      (void) fprintf (CKOD, "__ap->h.size=");
      (void) fprintf (CKOD, "(sizeof(__ah)+(sizeof(__arrlimit)*%d)+(sizeof(",
		      re1->left->rd->dim);
      gentype (re1->left);
      (void) fprintf (CKOD, ")");
      for (re2 = re->right; re2->token != MENDSEP; re2 = re2->right)
	{
	  (void) fprintf (CKOD, "*(");
	  genvalue (re2->left->right);
	  (void) fprintf (CKOD, " - ");
	  genvalue (re2->left->left);
	  (void) fprintf (CKOD, "+1)");
	}
      (void) fprintf (CKOD, "));\n");
      /* -> DIMMENSJON */
      (void) fprintf (CKOD, "__ap->h.dim=%d;\n", re1->left->rd->dim);
      /* -> TYPE */
      (void) fprintf (CKOD, "__ap->h.type='%c';\n", re1->left->type);
    }
  (void) fprintf (CKOD, "/*Slutt: kode for genering av ARRAY*/\n");
}

/******************************************************************************
                                                                GENSWITCH    */

genswitch (re)
     struct EXP *re;
{
  int i = 1;
  struct EXP *rex;
  gotollabel (push = newllabel ());
  if (re->left->rd->categ == CVIRT)
    {
      if (re->left->rd->match->idplev.plev == 0)
	re->left->rd->match->idplev.plev = newlabel ();
    }
  else
    {
      if (re->left->rd->idplev.plev == 0)
	re->left->rd->idplev.plev = newlabel ();
    }
  typelabel ((re->left->rd->categ == CVIRT ? re->left->rd->match : re->left->rd)
	     ->idplev.plev);
  (void) fprintf (CKOD, "switch(__swv)\n");
  (void) fprintf (CKOD, "{\n");

  for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
    {
      (void) fprintf (CKOD, "   case %d:", i++);
      gencall (rex, rex->left);
      genvalue (rex->left);
    }
  if (i > maxantcase)
    maxantcase = i;
  (void) fprintf (CKOD, "   default:__rerror(__errswitch);\n");
  (void) fprintf (CKOD, "}\n");
  typellabel (pop);
}
