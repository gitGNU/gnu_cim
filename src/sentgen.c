/* $Id: $ */

/* Copyright (C) 1998 Sverre Hvammen Johansen,
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

#include <stdio.h>
#include "gen.h"
#include "extspec.h"
#include "error.h"
#include "passes.h"

char not_reached;

char *mainroutine = "main";

/******************************************************************************
                                                                 SENTLISTGEN */

static void sent_list_gen (sent_t *parent_sent, int lab)
{
  sent_t *sent;

  for (sent= parent_sent->first; sent!=NULL; sent= sent->next)
    {
      lineno= sent->line;
      nonetest= sent->nonetest;
      indextest= sent->indextest;
      stripsideeffects= sent->stripsideeffects;
      sent_gen (sent, lab);
      cblock= parent_sent->cblock;
      if (cblock) cblev= cblock->blev;
    }
  lineno= parent_sent->last_line;
}




/******************************************************************************
                                                                     GENINIT */

static void gen_init (void)
{
#if 0
  fprintf (ccode, "/*Cim_ccode*/\n");
  if (!separat_comp)
    {
      fprintf (ccode, "int __start_data_segment=1;\n");
    }
  fprintf (ccode, "#include \"%s/cim.h\"\n", 
	   includedir);
#endif
}

/******************************************************************************
                                                                   MODULEGEN */

static void module_gen (sent_t *sent)
{
  int i;

  fprintf (ccode, "/*Cim_ccode*/\n");
  if (!separat_comp)
    {
      fprintf (ccode, "int __start_data_segment=1;\n");
    }
  fprintf (ccode, "#include \"%s/cim.h\"\n", 
	   includedir);

#ifdef DEBUG
  if (option_dump_build)
    sent_dump (main_sent, 0);
#endif

  sent_check (sent, TRUE);

  if (anterror) return;

#ifdef DEBUG
  if (option_dump_check)
    sent_dump (main_sent, 0);
#endif

  sent_trans (main_sent);

#ifdef DEBUG
  if (option_dump_trans)
    sent_dump (main_sent, 0);
#endif

  structure ();

  if (separat_comp)
    {
      fprintf (ccode, "void ");
      genmodulemark(NULL);
      fprintf (ccode, "(){");
      if (!option_line)
	fprintf (ccode, "__curent_map=__map%s;", timestamp);
      fprintf (ccode, "goto __s;");
    }
  else
    {
      fprintf (ccode, "%s(__argc,__argv)int __argc;char *__argv[];{"
	       "__progname= __argv[0];"
	       ,mainroutine);

      if (option_init_poolsize)
	fprintf (ccode, "__poolsize= %d;",
			option_init_poolsize);
      if (option_max_poolsize)
	fprintf (ccode, "__maxsize= %d;",
			option_max_poolsize);
      if (option_dyn_poolsize)
	fprintf (ccode, "__dynsize= __TRUE;");

      fprintf (ccode, "goto __sw;__l0:__rstart(__argc,__argv);"
	       "goto __start;__l1:__slutt:__rslutt();"
	       "exit(0);__start:");
      if (!option_line)
	fprintf (ccode, "__curent_map=__mapmain;");
    }

  sent_list_gen (sent, 0);

  if (separat_comp)
    {
      fprintf (ccode, "__sw:if(__goto.ment!=(void (*)())");
      genmodulemark(NULL);
      fprintf (ccode, ")return;");
      i = STARTLABEL;		/* Skal ikke skrive ut hopp til labler for
				 * omgivelsene. */
    }
  else
    {
      fprintf (ccode, "__sw:if(__goto.ment!=__NULL)"
	       "{ do (*__goto.ment)(); while(__goto.ment!=__NULL);");
      if (!option_line)
	fprintf (ccode, "     __curent_map=__mapmain;");
      fprintf (ccode, "}");
      i = 0;			/* Skal skrive ut hopp til labler for
				 * omgivelsene.               */
    }
  fprintf (ccode, "__s:switch(__goto.ent){");
  for (; i < lediglabel; i++)
    {
      fprintf (ccode, "case %d: goto __l%d;\n", i, i);
    }
  fprintf (ccode, "}");


  if (!separat_comp)
    fprintf (ccode,
		    "__goto=__return;return(__vreturn);");
  fprintf (ccode, "}");
  stat_pointers ();
  fprintf (ccode, "\n");
}

/******************************************************************************
                                                                    BLOCKGEN */

static void block_gen (sent_t *sent)
{
  cblock= sent->cblock;
  cblev= cblock->blev;

  genline ();
  if (sent->cblock->stat)
    fprintf (ccode, "__sto= (__dhp)&__blokk%d%s;", sent->cblock->blno
	     ,timestamp);
  fprintf (ccode, "__rb(");
  gen_adr_prot (ccode, &sent->cblock->quant);
  fprintf (ccode, ");");

  gotollabel (sent->cblock->ent = newllabel ());
  sent_list_gen (sent, 0);
  
  if (not_reached == FALSE)
    fprintf (ccode, "__rbe();");
}

/******************************************************************************
                                                                  PRBLOCKGEN */

static void prblock_gen (sent_t *sent)
{
  int labexit;
  if (not_reached == FALSE)
    genline ();
  else
    not_reached= FALSE;

  cblock= sent->cblock;
  cblev= cblock->blev;
  if (sent->cblock->stat)
    fprintf (ccode, "__sto= (__dhp)&__blokk%d%s;", sent->cblock->blno
		    ,timestamp);
  fprintf (ccode, "__rcprb(");
  gen_adr_prot (ccode, &sent->cblock->quant);
  fprintf (ccode, ");");
  /* Sender parameterne */
  genvalue (sent->iexp); fprintf (ccode, ";");
  genvalue (sent->exp); fprintf (ccode, ";");
  labexit = newlabel ();
  fprintf (ccode, "__rcprbb(%d,", labexit);
  genmodulemark(NULL);
  fprintf (ccode, ");");
  gotoswitch ();
  
  sent_list_gen (sent, sent->cblock->ent+1);
  if (not_reached == FALSE)
    fprintf (ccode, "__rendclass(%d);", sent->cblock->quant.plev);
  exitlabel (labexit);
}

/******************************************************************************
                                                                PROCEDUREGEN */

static void procedure_gen (sent_t *sent)
{
  cblock= sent->cblock;
  cblev= cblock->blev;
  fprintf (ccode, "/* START PROCEDURE %s */", sent->cblock->quant.ident);
  sent_list_gen (sent, 0);
  if (not_reached == FALSE)
    {
      genline ();
      if (sent->cblock->quant.type != TNOTY)
	{
	  if (sent->cblock->quant.type == TREF)
	    fprintf (ccode, "__er=((__bs%d *)__lb)->er;", sent->cblock->blno);
	  else if (sent->cblock->quant.type == TTEXT)
	    fprintf (ccode, "__et=((__bs%d *)__lb)->et;", sent->cblock->blno);
	  else if (sent->cblock->quant.type == TREAL)
	    fprintf (ccode, "__ev.f=((__bs%d *)__lb)->ef;", 
		     sent->cblock->blno);
	  else if (sent->cblock->quant.type == TINTG)
	    fprintf (ccode, "__ev.i=((__bs%d *)__lb)->ev;", 
			    sent->cblock->blno);
	  else
	    fprintf (ccode, "__ev.c=((__bs%d *)__lb)->ec;", 
			    sent->cblock->blno);
	  fprintf (ccode, "__rep();");
	}
      else
	fprintf (ccode, "__repp();");
      gotoswitch ();
    }
  fprintf (ccode, "/* SLUTT PROCEDURE %s */", sent->cblock->quant.ident);
}

/******************************************************************************
                                                                    CLASSGEN */

static void class_gen (sent_t *sent)
{
  cblock= sent->cblock;
  cblev= cblock->blev;
  fprintf (ccode, "/* START CLASS %s */", sent->cblock->quant.ident);
  sent_list_gen (sent, sent->cblock->ent+1);
  genline ();
  fprintf (ccode, "__rendclass(%d);", sent->cblock->quant.plev);
  gotoswitch ();
  fprintf (ccode, "/* SLUTT CLASS %s */", sent->cblock->quant.ident);
}

/******************************************************************************
                                                                  INSPECTGEN */

static void inspect_gen (sent_t *sent)
{
  int labnull, labexit;
  if (not_reached == FALSE)
    genline ();
  else
    not_reached= FALSE;

  cblock= sent->cblock;
  cblev= cblock->blev;
  genvalue (sent->iexp); fprintf (ccode, ";");

  gen_con_ref (sent->cblock->connest);
  fprintf (ccode, "=");
  genvalue (sent->exp);
  fprintf (ccode, ";");

  fprintf (ccode, "if(");
  gen_con_ref (sent->cblock->connest);
  fprintf (ccode, "==__NULL)");

  labexit = newllabel ();

  if (sent->last!=NULL && sent->last->token == MOTHERWISE)
    labnull= newllabel ();
  else
    labnull= labexit;

  gotollabel (labnull);


  if (sent->first!=NULL && sent->first->token == MWHEN)
    {
      fprintf (ccode, "__pp=");
      gen_con_ref (sent->cblock->connest);
      fprintf (ccode, "->pp; ");
    }
  sent_list_gen (sent, labexit);

  if (sent->last!=NULL && sent->last->token == MOTHERWISE)
    {
      typellabel (labnull);
      cblock= sent->last->cblock;
      cblev= cblock->blev;
      
      sent_list_gen (sent->last, 0);
  
      if (not_reached == FALSE)
	genline ();
    }
  typellabel (labexit);
}

/******************************************************************************
                                                                       DOGEN */

static void do_gen (sent_t *sent, int labexit)
{
  cblock= sent->cblock;
  cblev= cblock->blev;

  sent_list_gen (sent, 0);

  if (not_reached == FALSE)
    {
      genline ();
      gotollabel (labexit);
    }
}

/******************************************************************************
                                                                     WHENGEN */

static void when_gen (sent_t *sent, int labexit)
{
  int labnextcase;
  cblock= sent->cblock;
  cblev= cblock->blev;

  fprintf (ccode, "if(__pp->plev < %d || __pp->pref[%d]!= ", 
	   sent->exp->rd->plev, sent->exp->rd->plev);

  gen_adr_prot (ccode, sent->exp->rd);
  fprintf (ccode, ")");

  gotollabel (labnextcase = newllabel ());

  sent_list_gen (sent, 0);

  if (not_reached == FALSE)
    {
      genline ();
      gotollabel (labexit);
    }
  typellabel (labnextcase);
}

/******************************************************************************
                                                                OTHERWISEGEN */

static void otherwise_gen (sent_t *sent)
{
}

/******************************************************************************
                                                       GFORELEM              */
/* Genererer kode for en for løkke for ett liste element                     */
static int forelemgen (exp_t *re, exp_t *rex,
		   int labcontinue, int labdo, int labexit, int single, int notlastdefault,
		   int *listnrp)
{
  exp_t *rey;
  int labnext;

  exp_t *reconc, *red;

  rey= rex->left;
  switch (rey->token)
    {
    case MSTEP:
      genvalue (rey->left); fprintf (ccode, ";");

      fprintf (ccode, "if(");
      genvalue (rey->right->left);
      fprintf (ccode, ")");

      if (single)
	{
	  gotollabel (labdo);
	  gotollabel (labexit);
	  typellabel (labcontinue);
	}
      else
	{
	  fprintf (ccode, "{");
	  gen_for_val(cblock->fornest);
	  fprintf (ccode, "= %d;", ++*listnrp);
	  gotollabel (labdo);
	  fprintf (ccode, "}");
	  
	  gotollabel ( labnext= newllabel ());
	  
	  fprintf (ccode, "   case %d:", *listnrp);
	}
      genvalue (rey->right->right); fprintf (ccode, ";");

      fprintf (ccode, "if(");
      if (single) fprintf (ccode, "!");
      genvalue (rey->right->left);
      fprintf (ccode, ")");
      if (single)
	gotollabel (labexit);
      else
	{
	  gotollabel (labdo);
	  typellabel (labnext);
	}
      break;
    case MFORWHILE:
      if (single)
	typellabel (labcontinue);
      else if (notlastdefault)
	{
	  gen_for_val(cblock->fornest);
	  fprintf (ccode, "= %d;", ++*listnrp);
	}
      genvalue (rey->left); fprintf (ccode, ";");
      fprintf (ccode, "if(");
      if (single) fprintf (ccode, "!");
      genvalue (rey->right);
      fprintf (ccode, ")");
      if (single)
	gotollabel (labexit);
      else
	gotollabel (labdo);
      break;
    default:
      genvalue (rey); fprintf (ccode, ";");
      if (!single)
	{
	  gen_for_val(cblock->fornest);
	  fprintf (ccode, "= %d;", ++*listnrp);
	  gotollabel (labdo);
	  fprintf (ccode, "   case %d:", *listnrp);
	}
      return (FALSE);
      break;
    }
  return (TRUE);
}

/******************************************************************************
                                                      FORGEN                 */
/* Genererer kode for en for l|kke med et antall liste elementer             */
static int forgen (exp_t *re, int labcontinue, int labdo, int labexit)
{
  int notlastdefault=FALSE;
  exp_t *rex;
  int labnext;

  int listnr=0;

  if (re->right->right->token == MENDSEP)
    return forelemgen (re, re->right, labcontinue, labdo, labexit, TRUE, 0, NULL);

  gen_for_val(cblock->fornest);
  fprintf (ccode, "=1;");

  gotollabel (labnext= newllabel ());
  typellabel (labcontinue);
				
  fprintf (ccode, "switch (");
  gen_for_val(cblock->fornest);
  fprintf (ccode, " ){"); 

  typellabel (labnext);

  for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
    {
      notlastdefault= 
	forelemgen (re, rex, labcontinue, labdo, labexit, FALSE, 
		    notlastdefault, &listnr);
    }
  gotollabel (labexit);
  fprintf (ccode, "}");
  return (TRUE);
}

/******************************************************************************
                                                                    FORDOGEN */

static void fordo_gen (sent_t *sent)
{
  int labcontinue, labdo, labexit, iterate;
  if (not_reached == FALSE)
    genline ();
  else
    not_reached= FALSE;

  cblock= sent->cblock;
  cblev= cblock->blev;

  labcontinue= newllabel ();
  labdo= newllabel (); 
  labexit= newllabel ();

  iterate= forgen (sent->exp, labcontinue, labdo, labexit);

  typellabel (labdo);

  sent_list_gen (sent, 0);

  if (iterate)
    {
      if (not_reached == FALSE)
	{
	  genline ();
	  gotollabel (labcontinue);
	}
      typellabel (labexit);
    }
}

/******************************************************************************
                                                                    WHILEGEN */

static void while_gen (sent_t *sent)
{
  int labstart, labexit;
  if (not_reached == FALSE)
    genline ();
  else
    not_reached= FALSE;

  typellabel (labstart= newllabel ());
  genvalue (sent->iexp); fprintf (ccode, ";");
  fprintf (ccode, "if(!(");
  genvalue (sent->exp);
  fprintf (ccode, "))");
  gotollabel (labexit= newllabel ());

  sent_list_gen (sent, 0);

  if (not_reached == FALSE)
    {
      genline ();
      gotollabel (labstart);
    }
  typellabel (labexit);
}

/******************************************************************************
                                                                       IFGEN */

static void if_gen (sent_t *sent)
{
  int labelse, labexit;
  if (not_reached == FALSE)
    genline ();
  else
    not_reached= FALSE;

  genvalue (sent->iexp); fprintf (ccode, ";");

  fprintf (ccode, "if(!(");
  genvalue (sent->exp);
  fprintf (ccode, "))");
  labexit= newllabel ();
  if (sent->last->token == MTHEN)
    {
      gotollabel (labexit);
      sent_list_gen (sent->first, 0);      
    }
  else
    {
      gotollabel (labelse= newllabel ());

      sent_list_gen (sent->first, 0);      

      if (not_reached == FALSE)
      {
	genline ();
	gotollabel (labexit);
      }
      typellabel (labelse);

      sent_list_gen (sent->last, 0);      
    }
  if (not_reached == FALSE)
    genline ();
  typellabel (labexit);
}

/******************************************************************************
                                                                     GOTOGEN */

static void goto_gen (sent_t *sent)
{
  if (not_reached == FALSE)
    genline ();
  else
    not_reached= FALSE;

  genvalue (sent->iexp); fprintf (ccode, ";");
  genvalue (sent->exp);
}

/******************************************************************************
                                                                    INNERGEN */

static void inner_gen (sent_t *sent, int lab)
{
  genline ();
  fprintf (ccode, "__rinner(%d);", sent->cblock->quant.plev);
  exitlabel (lab+1);
}

/******************************************************************************
                                                                    ENTRYGEN */

static void entry_gen (sent_t *sent)
{
  typelabel (sent->cblock->ent);
}

/******************************************************************************
                                                               BLOCKENTRYGEN */

static void block_entry_gen (sent_t *sent)
{
  typellabel (sent->cblock->ent);
}

/******************************************************************************
                                                           PROCEDUREENTRYGEN */

static void procedure_entry_gen (sent_t *sent)
{
  decl_t *rd= sent->cblock->parloc;
  int i;

  typelabel (sent->cblock->ent);

  for(i = sent->cblock->napar; i>0; i--)
    {
      if (rd->kind == KSIMPLE && rd->type == TREF && rd->categ != CNAME)
	{
	  fprintf (ccode, "(((__bp=((__bs%d *)__lb)->%s"
			  ,rd->encl->blno,rd->ident);
	  fprintf (ccode, ")!=__NULL && (");
	  if (rd->prefqual->plev >= DEF_PLEV_TAB_SIZE)
	    fprintf (ccode, "__bp->pp->plev<%d || ",
			    rd->prefqual->plev);
	  fprintf (ccode, "__bp->pp->pref[%d]!= ",
			  rd->prefqual->plev);
	  gen_adr_prot (ccode, rd->prefqual);
	  fprintf (ccode, "))?(__dhp)__rerror(__errqual):__bp);");
	}
      rd = rd->next;
    }
  
}

/******************************************************************************
                                                                ENDSWITCHGEN */

static void endswitch_gen (sent_t *sent)
{
  exp_t *re= sent->exp, *rex;
  int i = 1;
  if (not_reached == FALSE)
    genline ();
  else
    not_reached= FALSE;

  if (re->left->rd->categ == CVIRT)
    {
      if (re->left->rd->match->plev == 0)
	re->left->rd->match->plev = newlabel ();
    }
  else
    {
      if (re->left->rd->plev == 0)
	re->left->rd->plev = newlabel ();
    }
  typelabel ((re->left->rd->categ == CVIRT ? re->left->rd->match : re->left->rd)
	     ->plev);
  fprintf (ccode, "switch(__swv)");
  fprintf (ccode, "{");

  for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
    {
      fprintf (ccode, "   case %d:", i++);
      genvalue (rex->left);
    }
  fprintf (ccode, "   default:__rerror(__errswitch);");
  fprintf (ccode, "}");
}

/******************************************************************************
                                                                ENDASSIGNGEN */

static void endassign_gen (sent_t *sent)
{
  if (not_reached == FALSE)
    genline ();
  else
    not_reached= FALSE;

  genvalue (sent->iexp); fprintf (ccode, ";");
  genvalue (sent->exp); fprintf (ccode, ";");
}

/******************************************************************************
                                                                 ENDARRAYGEN */

static void endarray_gen (sent_t *sent)
{
  exp_t *re= sent->exp, *re1, *re2;
  int i;
  char ifskrevet = FALSE;

  if (not_reached == FALSE)
    genline ();
  else
    not_reached= FALSE;

  genvalue (sent->iexp); fprintf (ccode, ";");
  for (re1 = re->left; re1->token != MENDSEP; re1 = re1->right)
    {
      fprintf (ccode, "        /* Array %s  */", 
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
		      fprintf (ccode, "if(");
		      ifskrevet = TRUE;
		    }
		  else
		    fprintf (ccode, " || ");

		  fprintf (ccode, "(");
		  genvalue (re2->left->left);
		  fprintf (ccode, " > ");
		  genvalue (re2->left->right);
		  fprintf (ccode, "+1)");

		}
	    }
	  if (ifskrevet)
	    fprintf (ccode, ")__rerror(__errlim);");
	}
      /* Generer arrayobjektet */
      fprintf (ccode, "__ap=(__arrp)__ralloc((long)");
      fprintf (ccode, "sizeof(__ah)+(sizeof(__arrlimit)*%d)+(sizeof(",
		      re1->left->rd->dim);
      gentype (re1->left);
      fprintf (ccode, ")");
      for (re2 = re->right; re2->token != MENDSEP; re2 = re2->right)
	{
	  fprintf (ccode, "*(");
	  genvalue (re2->left->right);
	  fprintf (ccode, " - ");
	  genvalue (re2->left->left);
	  fprintf (ccode, "+1)");
	}
      fprintf (ccode, "));");
      fprintf (ccode, "((__bs%d *)__pb)->%s=__ap;"
		      ,re1->left->rd->encl->blno
		      ,re1->left->value.ident);
      /* Initserer objektet -> Setter nedre  bounds og denne dimmensjonens
       * size */
      i = 0;
      for (re2 = re->right; re2->token != MENDSEP; re2 = re2->right)
	{
	  fprintf (ccode, "__ap->limits[%d].low=", i);
	  genvalue (re2->left->left);
	  fprintf (ccode, ";");
	  fprintf (ccode, "__ap->limits[%d].size=1+", i++);
	  genvalue (re2->left->right);
	  fprintf (ccode, " - ");
	  genvalue (re2->left->left);
	  fprintf (ccode, ";");
	}
      /* -> Merker at det er et ARRAY */
      fprintf (ccode, "__ap->h.pp=(__pty)__ARRAY;");
      /* -> SIZE */
      fprintf (ccode, "__ap->h.size=");
      fprintf (ccode, "(sizeof(__ah)+(sizeof(__arrlimit)*%d)+(sizeof(",
		      re1->left->rd->dim);
      gentype (re1->left);
      fprintf (ccode, ")");
      for (re2 = re->right; re2->token != MENDSEP; re2 = re2->right)
	{
	  fprintf (ccode, "*(");
	  genvalue (re2->left->right);
	  fprintf (ccode, " - ");
	  genvalue (re2->left->left);
	  fprintf (ccode, "+1)");
	}
      fprintf (ccode, "));");
      /* -> DIMMENSJON */
      fprintf (ccode, "__ap->h.dim=%d;", re1->left->rd->dim);
      /* -> TYPE */
      fprintf (ccode, "__ap->h.type='%c';", re1->left->type);
    }
  fprintf (ccode, "/*Slutt: kode for genering av ARRAY*/");
}

/******************************************************************************
                                                                  ENDDECLGEN */

static void enddecl_gen (sent_t *sent, int lab)
{
  if (not_reached == FALSE)
    genline ();
  else
    not_reached= FALSE;

  fprintf (ccode, "__renddecl(%d);", sent->cblock->quant.plev);
  exitlabel (lab);
}

/******************************************************************************
                                                                 ENDLABELGEN */

static void endlabel_gen (sent_t *sent)
{
  exp_t *re= sent->exp;
  if (not_reached == FALSE)
    genline ();
  else
    not_reached= FALSE;

  fprintf (ccode, "/*%s_%d*/", re->value.ident, re->rd->encl->blno);
  if (re->rd->categ == CVIRT)
    {
      if (re->rd->match->plev == 0)
	re->rd->match->plev = newlabel ();
    }
  else
    {
      if (re->rd->plev == 0)
	re->rd->plev = newlabel ();
    }
  typelabel ((re->rd->categ == CVIRT ? re->rd->match : re->rd)->plev);
}

/******************************************************************************
                                                                 GOTOSTOPGEN */
static void goto_stop_gen (sent_t *sent)
{
  fprintf (ccode, "goto __slutt;");
}


/******************************************************************************
                                                                    THUNKGEN */

static void thunk_gen (sent_t *sent)
{
  int labbypass;
  cblock= sent->cblock;
  if (cblock) cblev= cblock->blev;
  inthunk= sent->exp->value.thunk.inthunk;
  typelabel (sent->exp->value.thunk.label);
  genvalue (sent->iexp);fprintf (ccode, ";");

  switch (sent->token)
    {
    case MTHUNKSIMPLEADDRESS:
      gen_thunk_simple_address (sent->exp);
      break;
    case MTHUNKSIMPLEVALUE:
      gen_thunk_simple_value (sent->exp);
      break;
    case MTHUNKLABLE:
      gen_thunk_lable (sent->exp);
      break;
    case MTHUNKARRAY:
      gen_thunk_array (sent->exp);
      break;
    case MTHUNKPROCEDURE:
      gen_thunk_procedure (sent->exp);
      break;
    }
  gotoswitch ();
}

/******************************************************************************
                                                                     SENTGEN */

void sent_gen (sent_t *sent, int lab)
{
  switch (sent->token)
    {
    case MMODULE:
      module_gen (sent);
      break;
    case MBLOCK:
      block_gen (sent);
      break;
    case MPRBLOCK:
      prblock_gen (sent);
      break;
    case MPROCEDURE:
      procedure_gen (sent);
      break;
    case MCLASS:
      class_gen (sent);
      break;
    case MINSPECT:
      inspect_gen (sent);
      break;
    case MDO:
      do_gen (sent, lab);
      break;
    case MWHEN:
      when_gen (sent, lab);
      break;
    case MOTHERWISE:
      otherwise_gen (sent);
      break;
    case MFORDO:
      fordo_gen (sent);
      break;
    case MWHILE:
      while_gen (sent);
      break;
    case MIF:
      if_gen (sent);
      break;
    case MGOTO:
      goto_gen (sent);
      break;
    case MINNER:
      inner_gen (sent, lab);
      break;
    case MENTRY:
      entry_gen (sent);
      break;
    case MBLOCKENTRY:
      block_entry_gen (sent);
      break;
    case MPROCEDUREENTRY:
      procedure_entry_gen (sent);
      break;
    case MENDSWITCH:
      endswitch_gen (sent);
      break;
    case MENDASSIGN:
      endassign_gen (sent);
      break;
    case MENDARRAY:
      endarray_gen (sent);
      break;
    case MENDDECL:
      enddecl_gen (sent, lab);
      break;
    case MENDLABEL:
      endlabel_gen (sent);
      break;
    case MGOTOSTOP:
      goto_stop_gen (sent);
      break;
    case MTHUNKSIMPLEADDRESS:
    case MTHUNKSIMPLEVALUE:
    case MTHUNKLABLE:
    case MTHUNKARRAY:
    case MTHUNKPROCEDURE:
      thunk_gen (sent);
      break;
    }
}
