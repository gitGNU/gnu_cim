/* $Id: cgen.c,v 1.18 1997/01/08 09:49:17 cim Exp $ */

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

/* Generere C-kode for de ulike simula konstruksjoner. */

#include "gen.h"
#include "lex.h"
#include "extspec.h"

static long last_line;
static long true_last_line;

char mainroutine[MAINROUTINELENGTH + 1] = "main";

int stakk[MAXLABELSTAKK];
int cur = 0;

/* JUMP SOM ING]R I RUNTIME-SYSTEMET */
static int lediglabel = STARTLABEL;

/******************************************************************************
                                                                TYPESWITCH   */

static typeswitch ()
{
  int i;
  if (separat_comp)
    {
      (void) fprintf (ccode, "__sw:if(__goto.ment!=(void (*)())__m_%s)return;", timestamp);
      i = STARTLABEL;		/* Skal ikke skrive ut hopp til labler for
				 * omgivelsene. */
    }
  else
    {
      (void) fprintf (ccode, "__sw:if(__goto.ment!=__NULL)");
      (void) fprintf (ccode, "   { do (*__goto.ment)(); while(__goto.ment!=__NULL);");
      if (!option_line)
	(void) fprintf (ccode, "     __curent_map=__mapmain;");
      (void) fprintf (ccode, "}");
      i = 0;			/* Skal skrive ut hopp til labler for
				 * omgivelsene.               */
    }
  (void) fprintf (ccode, "__s:switch(__goto.ent){");
  for (; i < lediglabel; i++)
    {
      (void) fprintf (ccode, "case %d: goto __l%d;", i, i);
      if (i / 3 * 3 == i - 2)
	(void) fprintf (ccode, "");
    }
  (void) fprintf (ccode, "}");
}

/******************************************************************************
                                                        INITGEN  & FINGEN    */

initgen ()
{
  (void) fprintf (ccode, "/*Cim_ccode*/\n");
  if (!separat_comp)
    {
      (void) fprintf (ccode, "int __start_data_segment=1;\n");
    }
  (void) fprintf (ccode, "#include \"%s/cim.h\"\n", includedir);
  (void) fprintf (ccode, "#include \"%s\"\n", hcodename);
  if (separat_comp)
    {
      (void) fprintf (ccode, "void __m_%s(){", timestamp);
      if (!option_line)
	(void) fprintf (ccode, "__curent_map=__map%s;", timestamp);
      (void) fprintf (ccode, "goto __s;");
    }
  else
    {
      (void) fprintf (ccode, "%s(__argc,__argv)int __argc;char *__argv[];{"
		      ,mainroutine);
      (void) fprintf (ccode, "__progname= __argv[0];");

      if (option_init_poolsize)
	(void) fprintf (ccode, "__poolsize= %d;",
			option_init_poolsize);
      if (option_max_poolsize)
	(void) fprintf (ccode, "__maxsize= %d;",
			option_max_poolsize);
      if (option_dyn_poolsize)
	(void) fprintf (ccode, "__dynsize= __TRUE;");

      (void) fprintf (ccode, "goto __sw;__l0:__rstart(__argc,__argv);");

      (void) fprintf (ccode, "goto __start;__l1:__slutt:__rslutt();");

      (void) fprintf (ccode, "exit(0);__start:");
      if (!option_line)
	(void) fprintf (ccode, "__curent_map=__mapmain;");
    }
#if SPLIT_MODUL
  (void) fprintf (ccode, "__first%s();", timestamp);
#else
}
fingen ()
{
#endif
  if (!separat_comp)
    {
#if SPLIT_MODUL
#else
      (void) fprintf (ccode, "goto __slutt;");
#endif
    }
  typeswitch ();
  if (!separat_comp)
    (void) fprintf (ccode,
		    "__goto=__return;return(__vreturn);");
  (void) fprintf (ccode, "}");
#if SPLIT_MODUL
  (void) fprintf (ccode, "__first%s(){", timestamp);
}
fingen ()
{
  (void) fprintf (ccode, "__goto.ent=15;__goto.ment=__NULL;}");
#endif
  structure ();
  (void) fprintf (ccode, "\n");
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
  (void) fprintf (ccode, "__goto.ment= __l%d%s;}__l%d%s(){", l, timestamp
		  ,l, timestamp);
  (void) fprintf (hcode, "__l%d%s();\n", l, timestamp);
#else
  (void) fprintf (ccode, "__l%d:", l);
#endif
  not_reached = FALSE;
  last_line = 0;
  genline ();
}

gotoswitch ()
{
#if SPLIT_MODUL
  (void) fprintf (ccode, "return;");
#else
  (void) fprintf (ccode, "goto __sw;");
#endif
}

exitlabel (l)
     int l;
{
#if SPLIT_MODUL
  (void) fprintf (ccode, "}__l%d%s(){", l, timestamp);
  (void) fprintf (hcode, "__l%d%s();\n", l, timestamp);
#else
  (void) fprintf (ccode, "goto __sw;__l%d:", l);
#endif
  not_reached = FALSE;
  last_line = 0;
  genline ();
}

exitcondlabel (l)
     int l;
{
#if SPLIT_MODUL
  (void) fprintf (ccode, ";else __goto.ment=__l%d%s;}__l%d%s(){"
		  ,l, timestamp, l, timestamp);
  (void) fprintf (hcode, "__l%d%s();\n", l, timestamp);
#else
  (void) fprintf (ccode, "goto __sw;__l%d:", l);
#endif
  not_reached = FALSE;
  last_line = 0;
  genline ();
}

gotolabel (l)
     int l;
{
#if SPLIT_MODUL
  (void) fprintf (ccode, "{__goto.ment= __l%d%s;return;}", l, timestamp);
#else
  (void) fprintf (ccode, "goto __l%d;", l);
#endif
}

/******************************************************************************
                                        NEWLLABEL & TYPELLABEL & GOTOLLABEL  */

/* LOKALE HOPP */
static int ledigllabel;

int 
newllabel ()
{
  return (ledigllabel++);
}

typellabel (l)
     int l;
{
#if SPLIT_MODUL
  (void) fprintf (ccode, "__goto.ment= __ll%d%s;}__ll%d%s(){", l, timestamp
		  ,l, timestamp);
  (void) fprintf (hcode, "__ll%d%s();\n", l, timestamp);
#else
  (void) fprintf (ccode, "__ll%d:", l);
#endif
  not_reached = FALSE;
  last_line = 0;
}

gotollabel (l)
     int l;
{
#if SPLIT_MODUL
  (void) fprintf (ccode, "{__goto.ment= __ll%d%s;return;}", l, timestamp);
#else
  (void) fprintf (ccode, "goto __ll%d;", l);
#endif
}


/******************************************************************************
                                                        GENLINE              */

genline ()
{
  if (true_last_line != yylineno)
    {
      fprintf (ccode, "\n# %d \"\%s\"\n",getmapline(yylineno), getmapfile(yylineno));
    }
  if (!option_line && last_line != yylineno)
    {
      fprintf (ccode, "__cline=%ld;", yylineno);
      if (option_gen_trace)
	(void) fprintf (ccode, "__rtrace();");
    }
  true_last_line= last_line= yylineno;
#ifdef DEBUG
  if (option_gen_test)
    (void) fprintf (ccode, "__rtest();");
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
      (void) fprintf (ccode, "__renddecl(%d);", cblock->quant.idplev.plev);
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
    (void) fprintf (ccode, "__sto= (__dhp)&__blokk%d%s;", cblock->blno
		    ,timestamp);
  (void) fprintf (ccode, "__rb(");
  gen_adr_prot (&cblock->quant);
  (void) fprintf (ccode, ");");
#if 0
  typelabel (cblock->ent = newlabel ());
#endif
}

gendblock ()
{
  if (not_reached == FALSE)
    (void) fprintf (ccode, "__rbe();");
}

/******************************************************************************
                                                GPRBLOCK & GENDPRBLOCK       */

gprblock (re)
     struct EXP *re;
{
  if (cblock->stat)
    (void) fprintf (ccode, "__sto= (__dhp)&__blokk%d%s;", cblock->blno
		    ,timestamp);
  (void) fprintf (ccode, "__rcprb(");
  gen_adr_prot (&cblock->quant);
  (void) fprintf (ccode, ");");
  /* Sender parameterne */
  gencall (re->up, re);
  if (separat_comp)
#if SPLIT_MODUL
    (void) fprintf (ccode, "__rcprbb(0,__l%d%s);", 
		    push = newlabel (), timestamp);
#else
    (void) fprintf (ccode, "__rcprbb(%d,__m_%s);", 
		    push = newlabel (), timestamp);
#endif
  else
#if SPLIT_MODUL
    (void) fprintf (ccode, "__rcprbb(0,__l%d);", push = newlabel ());
#else
    (void) fprintf (ccode, "__rcprbb(%d,__NULL);", push = newlabel ());
#endif
  exitlabel (cblock->ent = newlabel ());
  push = newlabel ();

}

gendprblock ()
{
  if (not_reached == FALSE)
    (void) fprintf (ccode, "__rendclass(%d);", cblock->quant.idplev.plev);
  exitlabel (pop);
}

/******************************************************************************
                                                GPROCEDURE& GENDPROCEDURE   */

gprocedure ()
{
  struct DECL *rd;
  int i;
  (void) fprintf (ccode, "/* START PROCEDURE %s */", cblock->quant.ident);
  gotollabel (push = newllabel ());
  typelabel (cblock->ent = newlabel ());
  rd = cblock->parloc;
  for(i = 1; i <= cblock->napar; i++)
    {
      if (rd->kind == KSIMPLE && rd->type == TREF && rd->categ != CNAME)
	{
	  (void) fprintf (ccode, "(((__bp=((__bs%d *)__lb)->%s"
			  ,rd->encl->blno,rd->ident);
	  (void) fprintf (ccode, ")!=__NULL && (");
	  if (rd->prefqual->idplev.plev >= DEF_PLEV_TAB_SIZE)
	    (void) fprintf (ccode, "__bp->pp->plev<%d || ",
			    rd->prefqual->idplev.plev);
	  (void) fprintf (ccode, "__bp->pp->pref[%d]!= ",
			  rd->prefqual->idplev.plev);
	  gen_adr_prot (rd->prefqual);
	  (void) fprintf (ccode, "))?(__dhp)__rerror(__errqual):__bp);");
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
	    (void) fprintf (ccode, "__er=((__bs%d *)__lb)->er;", 
			    cblock->blno);
	  else if (cblock->quant.type == TTEXT)
	    (void) fprintf 
	      (ccode, "__et=((__bs%d *)__lb)->et;", 
	       cblock->blno);
	  else if (cblock->quant.type == TREAL)
	    (void) fprintf 
	      (ccode, "__ev.f=((__bs%d *)__lb)->ef;", cblock->blno);
	  else if (cblock->quant.type == TINTG)
	    (void) fprintf (ccode, "__ev.i=((__bs%d *)__lb)->ev;", 
			    cblock->blno);
	  else
	    (void) fprintf (ccode, "__ev.c=((__bs%d *)__lb)->ec;", 
			    cblock->blno);
	  (void) fprintf (ccode, "__rep();");
	}
      else
	(void) fprintf (ccode, "__repp();");
      gotoswitch ();
    }
  typellabel (pop);
  (void) fprintf (ccode, "/* SLUTT PROCEDURE %s */", cblock->quant.ident);
}
/******************************************************************************
                                                        GCLASS & GENDCLASS   */

gclass ()
{
  int i;
  (void) fprintf (ccode, "/* START CLASS %s */", cblock->quant.ident);
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
	  (void) fprintf (ccode, "__rinner(%d);", cblock->quant.idplev.plev);
	  gotoswitch ();
	  typelabel (pop);
	}
      (void) fprintf (ccode, "__rendclass(%d);", cblock->quant.idplev.plev);
      gotoswitch ();
    }
  else if (!cblock->inner)
    {
      typelabel (pop);
      (void) fprintf (ccode, "__rendclass(%d);", cblock->quant.idplev.plev);
      gotoswitch ();
    }
  typellabel (pop);
  (void) fprintf (ccode, "/* SLUTT CLASS %s */", cblock->quant.ident);
}

/******************************************************************************
                                                GINSPECT & GENDINSPECT       */

ginspect (re)
     struct EXP *re;
{
  struct DECL *rd;
  gencall (re->up, re);
  cblock->naloc = FALSE;	/* Brukes som flagg for om pp er satt   */

  (void) fprintf (ccode, "((__bs%d *)__lb)->c%d=", (rd = cblock->quant.match)->descr->blno,
		  cblock->connest);
  genvalue (re);
  (void) fprintf (ccode, ";");

  (void) fprintf (ccode, "if(((__bs%d *)__lb)->c%d==__NULL)", rd->descr->blno, cblock->connest);



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
      (void) fprintf (ccode, "if((__pp=((__bs%d *)__lb)->c%d->pp)->plev < %d ",
       cblock->quant.match->descr->blno, cblock->connest, rdi->idplev.plev);
      cblock->quant.prefqual->descr->naloc = TRUE;
    }
  else
    (void) fprintf (ccode, "if(__pp->plev < %d ", rdi->idplev.plev);

  (void) fprintf (ccode, "|| __pp->pref[%d]!= ", rdi->idplev.plev);
  gen_adr_prot (rdi);
  (void) fprintf (ccode, ")");

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
static gforopt (re)
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
      (void) fprintf (ccode, "=");
      genvalue (rex->left->left);
      (void) fprintf (ccode, ";");
      gencall (rex->left->right, rex->left->right->left);
      gencall (rex->left->right, rex->left->right->right);

      notnegativ = !(rex->left->right->left->token == MUSUBI ||
		     (rex->left->right->left->token == MINTEGERKONST &&
		      rex->left->right->left->value.ival < 0));
      konst_step = rex->left->right->left->token == MINTEGERKONST;
      (void) fprintf (ccode, "if((");
      if (konst_step)
	(void) fprintf (ccode, "(");
      else
	{
	  if (notnegativ)
	    genvalue (rex->left->right->left);
	  else
	    genvalue (rex->left->right->left->left);
	  (void) fprintf (ccode, "*(");
	}
#if 0
      genvalue (re->left);

      (void) fprintf (ccode, " - ");
      genvalue (rex->left->right->right);
#else
#if 0
      genvalue (re->left);

      (void) fprintf (ccode, " > ");
      genvalue (rex->left->right->right);
      (void) fprintf (ccode, " ?1:( ");
      genvalue (re->left);

      (void) fprintf (ccode, " == ");
      genvalue (rex->left->right->right);
      (void) fprintf (ccode, " ?0: -1) ");
#else
      if(re->type==TINTG)fprintf(ccode, "__rsigndi(");
        else fprintf(ccode, "__rsigndr(");
      genvalue (re->left);

      (void) fprintf (ccode, " , ");
      genvalue (rex->left->right->right);
      (void) fprintf (ccode, ")");
#endif
#endif
      if (!notnegativ)
	(void) fprintf (ccode, ")) < 0)");
      else
	(void) fprintf (ccode, ")) > 0)");
      gotollabel (push = newllabel ());
      gotollabel (l1 = newllabel ());
      typellabel (l2 = newllabel ());
      re = re2;
      rex = re2->right;
      gencall (rex->left->right, rex->left->right->left);
      gencall (rex->left->right, rex->left->right->right);
      genvalue (re->left);
      (void) fprintf (ccode, " += ");
      if (!konst_step)
	genvalue (rex->left->right->left);
      else
	genvalue (rex->left->right->left);
      (void) fprintf (ccode, ";");
      (void) fprintf (ccode, "if((");
      if (konst_step)
	(void) fprintf (ccode, "(");
      else
	{
	  if (notnegativ)
	    genvalue (rex->left->right->left);
	  else
	    genvalue (rex->left->right->left->left);
	  (void) fprintf (ccode, "*(");
	}
#if 0
      genvalue (re->left);
      (void) fprintf (ccode, " - ");
      genvalue (rex->left->right->right);
#else
#if 0
      genvalue (re->left);

      (void) fprintf (ccode, " > ");
      genvalue (rex->left->right->right);
      (void) fprintf (ccode, " ?1:( ");
      genvalue (re->left);

      (void) fprintf (ccode, " == ");
      genvalue (rex->left->right->right);
      (void) fprintf (ccode, " ?0: -1) ");
#else
      if(re->type==TINTG)
	if(rex->left->right->right->type==TREAL)
	  fprintf(ccode, "__rsigndx(");
	  else fprintf(ccode, "__rsigndi(");
        else fprintf(ccode, "__rsigndr(");
      genvalue (re->left);
      (void) fprintf (ccode, " , ");
      genvalue (rex->left->right->right);
      (void) fprintf (ccode, ")");
#endif
#endif
      if (!notnegativ)
	(void) fprintf (ccode, ")) < 0)");
      else
	(void) fprintf (ccode, ")) > 0)");
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
	    (void) fprintf (ccode, "__rtextvalassign(");
	  else
	    (void) fprintf (ccode, "__rtextassign(");
	  genvalue (re->left);
	  (void) fprintf (ccode, ",");
	  genvalue (rex->left->left);
	  (void) fprintf (ccode, ");");
	}
      else
	{
	  genvalue (re->left);
	  (void) fprintf (ccode, "=");
	  genvalue (rex->left->left);
	  (void) fprintf (ccode, ";");
	}
      gencall (rex->left, rex->left->right);
      (void) fprintf (ccode, "if(!");
      genvalue (rex->left->right);
      (void) fprintf (ccode, ")");
      gotollabel (push = newllabel ());
      push = l1;
      push = newllabel ();

      break;
    default:
      gencall (rex, rex->left);
      if (re->left->type == TTEXT)
	{
	  if (re->token == MFOR)
	    (void) fprintf (ccode, "__rtextvalassign(");
	  else
	    (void) fprintf (ccode, "__rtextassign(");
	  genvalue (re->left);
	  (void) fprintf (ccode, ",");
	  genvalue (rex->left);
	  (void) fprintf (ccode, ");");
	}
      else
	{
	  genvalue (re->left);
	  (void) fprintf (ccode, "=");
	  genvalue (rex->left);
	  (void) fprintf (ccode, ";");
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
  (void) fprintf (ccode, "((__bs%d *)__lb)->f%d=1;", 
		  (rd = cblock->quant.match)->descr->blno,
		  cblock->fornest);

  typellabel (l1 = newllabel ());	/* Etter forl|kka skal det hoppes til 
					 * l1: */
  l2 = newllabel ();		/* l2 er labelen til for blokken         */
  (void) fprintf (ccode, "switch (((__bs%d *)__lb)->f%d)", rd->descr->blno,
		  cblock->fornest);
  (void) fprintf (ccode, "{");
  listnr = 0;


  for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
    {
      (void) fprintf (ccode, "   case %d:", ++listnr);
      rey = rex->left;
      switch (rey->token)
	{
	case MSTEP:
	  re2 = copytree (rey);
	  gencall (rey, rey->left);
	  genvalue (re->left);
	  (void) fprintf (ccode, "=");
	  genvalue (rey->left);
	  (void) fprintf (ccode, ";");
	  (void) fprintf (ccode, "((__bs%d *)__lb)->f%d++;", rd->descr->blno,
			  cblock->fornest);
	  gencall (rey->right, rey->right->left);
	  gencall (rey->right, rey->right->right);

	  notnegativ = !(rey->right->left->token == MUSUBI ||
			 (rey->right->left->token == MINTEGERKONST &&
			  rey->right->left->value.ival < 0));
	  konst_step = rey->right->left->token == MINTEGERKONST;
	  (void) fprintf (ccode, "if((");
	  if (konst_step)
	    (void) fprintf (ccode, "(");
	  else
	    {
	      if (notnegativ)
		genvalue (rey->right->left);
	      else
		genvalue (rey->right->left->left);
	      (void) fprintf (ccode, "*(");
	    }
#if 0
	  genvalue (re->left);
	  (void) fprintf (ccode, " - ");
	  genvalue (rey->right->right);
#else
#if 0
          genvalue (re->left);

          (void) fprintf (ccode, " > ");
          genvalue (rey->right->right);
          (void) fprintf (ccode, " ?1:( ");
          genvalue (re->left);
          (void) fprintf (ccode, " == ");
          genvalue (rey->right->right);
          (void) fprintf (ccode, " ?0: -1) ");
#else
          if(re->type==TINTG)
	    if(rex->left->right->right->type==TREAL)
	      fprintf(ccode, "__rsigndx(");
	      else fprintf(ccode, "__rsigndi(");
            else fprintf(ccode, "__rsigndr(");
	  genvalue (re->left);
	  (void) fprintf (ccode, " , ");
	  genvalue (rey->right->right);
	  (void) fprintf (ccode, ")");
#endif
#endif
	  if (!notnegativ)
	    (void) fprintf (ccode, ")) >= 0)");
	  else
	    (void) fprintf (ccode, ")) <= 0)");
	  gotollabel (l2);
	  (void) fprintf (ccode, "((__bs%d *)__lb)->f%d++;", 
			  rd->descr->blno, cblock->fornest);
	  gotollabel (l1);
	  (void) fprintf (ccode, "   case %d:", ++listnr);
	  rey = re2;
	  gencall (rey->right, rey->right->left);
	  gencall (rey->right, rey->right->right);
	  genvalue (re->left);
	  (void) fprintf (ccode, " += ");
	  if (!konst_step)
	    genvalue (rey->right->left);
	  else
	    genvalue (rey->right->left);
	  (void) fprintf (ccode, ";");
	  (void) fprintf (ccode, "if((");
	  if (konst_step)
	    (void) fprintf (ccode, "(");
	  else
	    {
	      if (notnegativ)
		genvalue (rey->right->left);
	      else
		genvalue (rey->right->left->left);
	      (void) fprintf (ccode, "*(");
	    }
#if 0
	  genvalue (re->left);
	  (void) fprintf (ccode, " - ");
	  genvalue (rey->right->right);
#else
#if 0
          genvalue (re->left);

          (void) fprintf (ccode, " > ");
          genvalue (rey->right->right);
          (void) fprintf (ccode, " ?1:( ");
          genvalue (re->left);
          (void) fprintf (ccode, " == ");
          genvalue (rey->right->right);
          (void) fprintf (ccode, " ?0: -1) ");
#else
          if(re->type==TINTG)
	    if(rex->left->right->right->type==TREAL)
	      fprintf(ccode, "__rsigndx(");
	      else fprintf(ccode, "__rsigndi(");
            else fprintf(ccode, "__rsigndr(");
	  genvalue (re->left);
	  (void) fprintf (ccode, " , ");
	  genvalue (rey->right->right);
	  (void) fprintf (ccode, ")");
#endif
#endif
	  if (!notnegativ)
	    (void) fprintf (ccode, ")) >= 0)");
	  else
	    (void) fprintf (ccode, ")) <= 0)");
	  gotollabel (l2);
	  (void) fprintf (ccode, "((__bs%d *)__lb)->f%d++;", 
			  rd->descr->blno, cblock->fornest);
	  break;
	case MFORWHILE:
	  gencall (rey, rey->left);
	  if (re->left->type == TTEXT)
	    {
	      if (re->token == MFOR)
		(void) fprintf (ccode, "__rtextvalassign(");
	      else
		(void) fprintf (ccode, "__rtextassign(");
	      genvalue (re->left);
	      (void) fprintf (ccode, ",");
	      genvalue (rey->left);
	      (void) fprintf (ccode, ");");
	    }
	  else
	    {
	      genvalue (re->left);
	      (void) fprintf (ccode, "=");
	      genvalue (rey->left);
	      (void) fprintf (ccode, ";");
	    }
	  gencall (rey, rey->right);
	  (void) fprintf (ccode, "if(");
	  genvalue (rey->right);
	  (void) fprintf (ccode, ")");
	  gotollabel (l2);
	  (void) fprintf (ccode, "((__bs%d *)__lb)->f%d++;", 
			  rd->descr->blno, cblock->fornest);
	  break;
	default:
	  gencall (rex, rey);
	  if (re->left->type == TTEXT)
	    {
	      if (re->token == MFOR)
		(void) fprintf (ccode, "__rtextvalassign(");
	      else
		(void) fprintf (ccode, "__rtextassign(");
	      genvalue (re->left);
	      (void) fprintf (ccode, ",");
	      genvalue (rey);
	      (void) fprintf (ccode, ");");
	    }
	  else
	    {
	      genvalue (re->left);
	      (void) fprintf (ccode, "=");
	      genvalue (rey);
	      (void) fprintf (ccode, ";");
	    }
	  (void) fprintf (ccode, "((__bs%d *)__lb)->f%d++;", rd->descr->blno,
			  cblock->fornest);
	  gotollabel (l2);
	}
    }
  (void) fprintf (ccode, "   default:");
  gotollabel (push = newllabel ());
  (void) fprintf (ccode, "}");
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
  (void) fprintf (ccode, "if(!(");
  genvalue (re);
  (void) fprintf (ccode, "))");
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
  (void) fprintf (ccode, "if(!(");
  genvalue (re);
  (void) fprintf (ccode, "))");
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
  (void) fprintf (ccode, "/*%s_%d*/", re->value.ident, re->rd->encl->blno);
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
  (void) fprintf (ccode, "__rinner(%d);", cblock->quant.idplev.plev);
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
      (void) fprintf (ccode, "        /* Array %s  */", 
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
		      (void) fprintf (ccode, "if(");
		      ifskrevet = TRUE;
		    }
		  else
		    (void) fprintf (ccode, " || ");

		  (void) fprintf (ccode, "(");
		  genvalue (re2->left->left);
		  (void) fprintf (ccode, " > ");
		  genvalue (re2->left->right);
		  (void) fprintf (ccode, "+1)");

		}
	    }
	  if (ifskrevet)
	    (void) fprintf (ccode, ")__rerror(__errlim);");
	}
      /* Generer arrayobjektet */
      (void) fprintf (ccode, "__ap=(__arrp)__ralloc((long)");
      (void) fprintf (ccode, "sizeof(__ah)+(sizeof(__arrlimit)*%d)+(sizeof(",
		      re1->left->rd->dim);
      gentype (re1->left);
      (void) fprintf (ccode, ")");
      for (re2 = re->right; re2->token != MENDSEP; re2 = re2->right)
	{
	  (void) fprintf (ccode, "*(");
	  genvalue (re2->left->right);
	  (void) fprintf (ccode, " - ");
	  genvalue (re2->left->left);
	  (void) fprintf (ccode, "+1)");
	}
      (void) fprintf (ccode, "));");
      (void) fprintf (ccode, "((__bs%d *)__pb)->%s=__ap;"
		      ,re1->left->rd->encl->blno
		      ,re1->left->value.ident);
      /* Initserer objektet -> Setter nedre  bounds og denne dimmensjonens
       * size */
      i = 0;
      for (re2 = re->right; re2->token != MENDSEP; re2 = re2->right)
	{
	  (void) fprintf (ccode, "__ap->limits[%d].low=", i);
	  genvalue (re2->left->left);
	  (void) fprintf (ccode, ";");
	  (void) fprintf (ccode, "__ap->limits[%d].size=1+", i++);
	  genvalue (re2->left->right);
	  (void) fprintf (ccode, " - ");
	  genvalue (re2->left->left);
	  (void) fprintf (ccode, ";");
	}
      /* -> Merker at det er et ARRAY */
      (void) fprintf (ccode, "__ap->h.pp=(__pty)__ARRAY;");
      /* -> SIZE */
      (void) fprintf (ccode, "__ap->h.size=");
      (void) fprintf (ccode, "(sizeof(__ah)+(sizeof(__arrlimit)*%d)+(sizeof(",
		      re1->left->rd->dim);
      gentype (re1->left);
      (void) fprintf (ccode, ")");
      for (re2 = re->right; re2->token != MENDSEP; re2 = re2->right)
	{
	  (void) fprintf (ccode, "*(");
	  genvalue (re2->left->right);
	  (void) fprintf (ccode, " - ");
	  genvalue (re2->left->left);
	  (void) fprintf (ccode, "+1)");
	}
      (void) fprintf (ccode, "));");
      /* -> DIMMENSJON */
      (void) fprintf (ccode, "__ap->h.dim=%d;", re1->left->rd->dim);
      /* -> TYPE */
      (void) fprintf (ccode, "__ap->h.type='%c';", re1->left->type);
    }
  (void) fprintf (ccode, "/*Slutt: kode for genering av ARRAY*/");
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
  (void) fprintf (ccode, "switch(__swv)");
  (void) fprintf (ccode, "{");

  for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
    {
      (void) fprintf (ccode, "   case %d:", i++);
      gencall (rex, rex->left);
      genvalue (rex->left);
    }
  (void) fprintf (ccode, "   default:__rerror(__errswitch);");
  (void) fprintf (ccode, "}");
  typellabel (pop);
}
