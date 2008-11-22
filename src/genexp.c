/* $Id: cgenexp.c,v 1.24 1995/12/02 15:24:43 cim Exp $ */

/* Copyright (C) 1994, 1998 Sverre Hvammen Johansen and Terje Mj�s,
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

#include "config.h"
#include "gen.h"
#include "extspec.h"

int stack;
static int anttext;
int inthunk;			/* Brukes i forbindelse med uttrykk i
				 * thunker. Sier at statisk link (sl) m}
				 * f|lges en gang ekstra for      variable
				 * som aksesseres ifra thunken. */



/******************************************************************************
                                                                         GEN */

gen (re)
     struct EXP *re;
{
  genvalue (transcall (re->up, re));
  gen_sent_marker();
  genvalue (re);
  gen_sent_marker();
}

/******************************************************************************
                                                                GENSL        */

gensl (re, atr, nonetest)
     struct EXP *re;
     char atr,
       nonetest;
{
  if (is_after_dot (re))
    {
      if (re->up->left->token == MQUA || re->up->left->token == MQUANOTNONE)
	nonetest= OFF;
      if (atr)
	fprintf (ccode, "((__bs%d *)", re->rd->encl->blno);
      if (nonetest == ON)
	fprintf (ccode, "((__bp=");
      genvalue (re->up->left);
      if (nonetest == ON)
	fprintf (ccode,
			")==__NULL?(__dhp)__rerror(__errnone):__bp)");
      if (atr)
	fprintf (ccode, ")->");
    }
  else if (seen_th_insp (re))
    {
      if (atr)
	fprintf (ccode, "((__bs%d *)", re->rd->encl->blno);
      genchain (re->seenthrough->quant.match->descr, TRUE);
      fprintf (ccode, "c%d", re->seenthrough->connest);
      if (atr)
	fprintf (ccode, ")->");
    }
  else
    {
      genchain (re->rd->encl, atr);
    }
}

/******************************************************************************
                                                               GENCHAIN     */

genchain (rb, atr)
     struct BLOCK *rb;
     char atr;
{
  int i;
  if (rb->stat)
    if (atr)
      fprintf (ccode, "(__blokk%d%s).", rb->blno, 
	       rb->timestamp?rb->timestamp:timestamp);
#if 0
    else if (rb == sblock && separat_comp)
#else
    else if (rb->blev==EXTERNALGLOBALBLEV && separat_comp)
#endif
      fprintf (ccode, "__NULL");
    else
      fprintf (ccode, "((__dhp)&__blokk%d%s)", rb->blno, 
	       rb->timestamp?rb->timestamp:timestamp);
  else
    {
      struct BLOCK *rbx;
      /*      rbx = display[rb->blev];*/
      for (rbx= cblock; rbx->blev != rb->blev; rbx= rbx->quant.encl);
      
      while (rbx->quant.kind == KFOR || rbx->quant.kind == KINSP
	     || rbx->quant.kind == KCON)
	rbx = rbx->quant.prefqual->descr;
      if (rbx->stat)
	{
	  if (atr)
	    {
	      fprintf (ccode, "((__bs%d *)&__blokk%d%s)->",
		       rb->blno, rbx->blno, 
		       rbx->timestamp?rbx->timestamp:timestamp);
	    }
	  else
	    fprintf (ccode, "((__dhp)&__blokk%d%s)", 
		     rbx->blno, 
		     rbx->timestamp?rbx->timestamp:timestamp);
	}
      else
	{
	  if (atr)
	    fprintf (ccode, "((__bs%d *)__lb", rb->blno);
	  else
	    fprintf (ccode, "__lb", rb->blno);
	  for (i = cblev + (inthunk ? 1 : 0); i > rb->blev; i--)
	    fprintf (ccode, "->sl");
	  if (atr)
	    fprintf (ccode, ")->", rb->blno);
	}
    }
}

/******************************************************************************
                                                                GENTYPE      */

gentype (re)
     struct EXP *re;
{
  switch (re->type)
    {
    case TINTG:
      fprintf (ccode, "long");
      break;
    case TREAL:
      fprintf (ccode, "double");
      break;
    case TBOOL:
    case TCHAR:
      fprintf (ccode, "char");
      break;
    case TLABEL:
      fprintf (ccode, "__lab");
      break;
    case TTEXT:
      fprintf (ccode, "__txt");
      break;
    case TREF:
      fprintf (ccode, "__dhp");
      break;
    };

}

/******************************************************************************
                                                            GEN_ADR_PROT     */

gen_adr_prot (code, rd) FILE *code;
     struct DECL *rd;
{
  fprintf (code, "&__p%d%s"
	   ,rd->descr->timestamp == 0 ? rd->descr->blno : rd->descr->ptypno
	   ,rd->descr->timestamp == 0 ?
	   (rd->encl->blev == SYSTEMGLOBALBLEV && 
	    rd->encl->quant.plev == 0 
	    ? "" :timestamp) : rd->descr->timestamp);
}

/******************************************************************************
							       GENMODULEMARK */

genmodulemark(maintimestamp) char *maintimestamp;
{
  if (maintimestamp)
    fprintf (ccode, "__m_%s", maintimestamp);
  else if (separat_comp)
    fprintf (ccode, "__m_%s", timestamp);
  else fprintf (ccode, "__NULL");
}

/******************************************************************************
                                                                GENVALUE     */

genvalue (re)
     struct EXP *re;
{
  struct EXP *rex;
  static struct EXP *ree;

  if (re == NULL)
    return;

  ree = re;
  switch (re->token)
    {
    case MARGUMENT:
      genprocparam (re);
      break;
    case MNEWARG:
      fprintf (ccode, "__sl=");
      gensl (re, FALSE, ON);
      fprintf (ccode, ";");
      if (re->rd->descr->stat)
	fprintf (ccode, "__sto= (__dhp)&__blokk%d%s;"
			,re->rd->descr->blno, timestamp);
      fprintf (ccode, "__rcp(");
      gen_adr_prot (ccode, re->rd);

      fprintf (ccode, ",%ldL);",re->value.n_of_stack_elements);
      genprocparam (re);

      {
	long l;
	fprintf (ccode, "__rccb(%d,", l = newlabel ());
	genmodulemark(NULL);
	fprintf (ccode, ");");
	if (separat_comp && re->rd->descr->timestamp)
	  {
	    fprintf (ccode, "return;");
	    typelabel (l);
	  }
	else
	  {
	    exitlabel (l);
	  }
      }

      break;

    case MPROCARG:
      /* Predefinerte prosedyrer, C-prosedyrer eller vanlige 
       * proper-procedures, som er behandlet av transcall. De
       * predefinerte og C-prosedyrene skal behandles her, mens vanlige
       * proper-procedures allerede er behandlet i transcall. */
      if (re->rd->descr->codeclass == CCNO)
	{
	  /* Statisk link overf|res i den globale variabelen sl. 
	   * Genererer kallet p} rcp. */

	  if (re->rd->categ != CNAME)
	    {
	      fprintf (ccode, "__sl=");
	      if (re->rd->categ == CDEFLT)
		{
		  gensl (re, TRUE, ON);
		  fprintf (ccode, "%s.psl;", re->rd->ident);
		}
	      else
		{
		  gensl (re, FALSE, ON);
		}
	      fprintf (ccode, ";");
	    }

	  if (re->rd->categ == CVIRT)
	    {
	      /* Kall p} en virtuell prosedyre. 
               * Prosedyrens prototype  er gitt i virtuell tabellen. 
	       * M} teste at den ikke er  NULL, som gir
	       * run-time error. */
	      fprintf (ccode, "if((__pp=");
	      gensl (re, FALSE, OFF);
	      fprintf (ccode, "->pp->virt[%d])==__NULL)__rerror(__errvirt);", 
		       re->rd->virtno - 1);
	    }

	  if (re->rd->descr->stat)
	    fprintf (ccode, "__sto= (__dhp)&__blokk%d%s;"
		     ,re->rd->descr->blno, timestamp);

	  if (re->type == TNOTY)
	    fprintf (ccode, "__rcpp(");
	  else
	    fprintf (ccode, "__rcp(");

	  if (re->rd->categ == CNAME || re->rd->categ == CVIRT)
	    fprintf (ccode, "__pp");
	  else if (re->rd->categ == CDEFLT)
	    {
	      gensl (re, TRUE, OFF);
	      fprintf (ccode, "%s.pp", re->rd->ident);
	    }
	  else
	    gen_adr_prot (ccode, re->rd);

	  if (re->type == TNOTY)
	    fprintf (ccode, ");");
	  else
	    fprintf (ccode, ",%ldL);", 
		     re->value.combined_stack.n_of_stack_elements);

	  /* Kaller p} genprocparam som genererer kode for parameter-
	   * overf|ringen. */

	  genprocparam (re);

	  /* Hvis dette er et dot'et kall s} skal ikke  doten ses n}r genvalue
	   * kalles.(Den informasjonen trengs ikke da) */


	  /* N} er alle parameterene overf}rt, 
	   * og prosedyren kan  settes i gang. */

	  { 
	    int l;
	    fprintf (ccode, "__rcpb(%d,", l= newlabel ());
	    genmodulemark(NULL);
	    fprintf (ccode, ");");
	    if (separat_comp && re->rd->descr->timestamp)
	      {
		fprintf (ccode, "return;");
		typelabel (l);
	      }
	    else
	      {
		exitlabel (l);
	      }
	  }

	  /* H}ndterer evt. funksjonsverdier. Sjekker om det
	   * er n|dvendig med konvertering av aritm.  returverier eller
	   * kvalifikasjonskontroll for type REF   Dette gjelder formelle
	   * prosedyrer med categ lik CVAR og CNAME (type = TREF, 
	   * TINTG og TREAL) 
	   */

	  switch (re->type)
	    {
	    case TREF:
	      fprintf (ccode, "__r[%d]=__er;", re->value.combined_stack.entry);
	      if (re->rd->categ == CVAR || re->rd->categ == CNAME)
		{
		  fprintf (ccode, "__bp=");
		  gensl (re, FALSE, ON);
		  fprintf (ccode, ";if(((__bs%d *)__bp)->%s.conv==__READTEST "
			   "&& !__rin(__er,((__bs%d *)__bp)->%s.q))"
			   "__rerror(__errqual);",
			   re->rd->encl->blno, re->rd->ident,
			   re->rd->encl->blno, re->rd->ident);

		}
	      break;
	    case TTEXT:
	      fprintf (ccode, "__t[%d]=__et;", re->value.combined_stack.entry);
	      break;
	    case TREAL:
	      if (re->rd->categ == CVAR || re->rd->categ == CNAME)
		{			/* Tre muligheter : ingen, int -> real,
					 * real->int->real */
		  fprintf (ccode, "__v[%d].f=((__conv=", 
			   re->value.combined_stack.entry);
		  gensl (re, TRUE, ON);
		  fprintf (ccode, "%s.conv)==__NOCONV?__ev.f:__conv==__INTREAL?"
			   "(double)__ev.i:(double)__rintrea(__ev.f));",
			   re->rd->ident);
		}
	      else
		fprintf (ccode, "__v[%d].f=__ev.f;", 
			 re->value.combined_stack.entry);
	      break;
	    case TINTG:
	      if (re->rd->categ == CNAME || re->rd->categ == CVAR)
		{	           /* To muligheter : ingen konvertering eller
			            * real->int */
		  fprintf (ccode, "__v[%d].i=(", 
			   re->value.combined_stack.entry);
		  gensl (re, TRUE, ON);
		  fprintf (ccode, "%s.conv==__NOCONV?",
			   re->rd->ident);
		  fprintf (ccode, "__ev.i:__rintrea(__ev.f));");
		}
	      else
		fprintf (ccode, "__v[%d].i=__ev.i;", 
			 re->value.combined_stack.entry );
	      break;
	    case TNOTY:
	      break;
	    default:
	      fprintf (ccode, "__v[%d].c=__ev.c;", 
		       re->value.combined_stack.entry);
	      break;
	    }

	  break;
	}

      if (re->right == NULL)
	break;
      /* Det siste tilfelle skal egentlig aldri sl} til. Det ville i s}
       * fall bety at en predefinert properprocedure ville v{rt spesifisert
       * som DANGER, noe som ikke er riktig. */

      if (re->rd->categ == CCPROC)
	if (re->type == TTEXT)
	  {
	    fprintf (ccode, "__ctext= ");
	    gencproccall (re);
	      fprintf (ccode, ";__t[%d]= *__rblanks(%ldL,"
		       "__ctext==__NULL?0:strlen(__ctext));"
		       "(void)strcpy(__t[%d].obj->string,__ctext);",
		       re->value.combined_stack.entry, 
		       re->value.combined_stack.n_of_stack_elements, 
		       re->value.combined_stack.entry);
	  }
	else
	  gencproccall (re);
      else
	genpredefproccall (re);
      break;

    case MNOT:
      fprintf (ccode, "(!");
      genvalue (re->left);
      putc (')', ccode);
      break;
    case MIMP:
      fprintf (ccode, "(!");
      genvalue (re->left);
      fprintf (ccode, "|");
      genvalue (re->right);
      fprintf (ccode, ")");
      break;
    case MIS:
      fprintf (ccode, "__ris(");
      genvalue (re->left);
      fprintf (ccode, ",");

      gen_adr_prot (ccode, re->rd);
      fprintf (ccode, ")");
      break;
    case MINS:
      fprintf (ccode, "__rin(");
      genvalue (re->left);
      fprintf (ccode, ",");
      gen_adr_prot (ccode, re->rd);
      fprintf (ccode, ")");
      break;
    case MEQT:
      fprintf (ccode, "__reqtext(");
      genvalue (re->left);
      fprintf (ccode, ",");
      genvalue (re->right);
      fprintf (ccode, ")");
      break;
    case MNET:
      fprintf (ccode, "!__reqtext(");
      genvalue (re->left);
      fprintf (ccode, ",");
      genvalue (re->right);
      fprintf (ccode, ")");
      break;
    case MLTT:
      fprintf (ccode, "__rlttext(");
      genvalue (re->left);
      fprintf (ccode, ",");
      genvalue (re->right);
      fprintf (ccode, ")");
      break;
    case MLET:
      fprintf (ccode, "__rletext(");
      genvalue (re->left);
      fprintf (ccode, ",");
      genvalue (re->right);
      fprintf (ccode, ")");
      break;
    case MGTT:
      fprintf (ccode, "__rlttext(");
      genvalue (re->right);
      fprintf (ccode, ",");
      genvalue (re->left);
      fprintf (ccode, ")");
      break;
    case MGET:
      fprintf (ccode, "__rletext(");
      genvalue (re->right);
      fprintf (ccode, ",");
      genvalue (re->left);
      fprintf (ccode, ")");
      break;
    case MEQRT:
      fprintf (ccode, "__reqrtext(");
      genvalue (re->left);
      fprintf (ccode, ",");
      genvalue (re->right);
      fprintf (ccode, ")");
      break;
    case MNERT:
      fprintf (ccode, "!__reqrtext(");
      genvalue (re->left);
      fprintf (ccode, ",");
      genvalue (re->right);
      fprintf (ccode, ")");
      break;
    case MSIGNDX:
      fprintf (ccode, "__rsigndx(");
      genvalue (re->left);
      fprintf (ccode, ",");
      genvalue (re->right);
      fprintf (ccode, ")");
      break;
    case MSIGNDI:
      fprintf (ccode, "__rsigndi(");
      genvalue (re->left);
      fprintf (ccode, ",");
      genvalue (re->right);
      fprintf (ccode, ")");
      break;
    case MSIGNDR:
      fprintf (ccode, "__rsigndr(");
      genvalue (re->left);
      fprintf (ccode, ",");
      genvalue (re->right);
      fprintf (ccode, ")");
      break;
    case MIFE:
      if (re->type == TLABEL)
	{
	  fprintf (ccode, "if(!");
	  genvalue (re->left);
	  fprintf (ccode, ")");
	  gotollabel ((int) (re->right->value.ival = newllabel ()));
	  genvalue (re->right);
	  break;
	}
      putc ('(', ccode);
      genvalue (re->left);
      fprintf (ccode, "?");
      genvalue (re->right);
      putc (')', ccode);
      break;
    case MELSEE:
      if (re->type == TLABEL)
	{
	  genvalue (re->left);
	  typellabel ((int) re->value.ival);
	  genvalue (re->right);
	  break;
	}
      genvalue (re->left);
      fprintf (ccode, ":");
      genvalue (re->right);
      break;
    case MIF:
      fprintf (ccode, "if(!");
      genvalue (re->left);
      fprintf (ccode, ")");
      gotollabel (re->right->value.ival= newllabel ());
      genvalue (re->right);
      fprintf (ccode, ";");
      typellabel (re->value.ival);
      break;
    case MELSE:
      genvalue (re->left);
      fprintf (ccode, ";");
      gotollabel (re->up->value.ival= newllabel ());
      typellabel (re->value.ival);
      genvalue (re->right);
      break;
    case MORELSE:
    case MANDTHEN:
      fprintf (ccode, "if(");
      if (re->token == MANDTHEN)
	fprintf (ccode, "!");
      genvalue (re->left);
      fprintf (ccode, ")");
      gotollabel ((int) (re->value.ival = newllabel ()));
      genvalue (re->right);
      fprintf (ccode, ";");
      typellabel ((int) re->value.ival);
      break;
    case MUADD:
    case MUADDI:
      genvalue (re->left);
      break;
    case MUSUB:
    case MUSUBI:
      fprintf (ccode, "(-");
      genvalue (re->left);
      fprintf (ccode, ")");
      break;
    case MPRIMARY:
      fprintf (ccode, "__rpow(");
      genvalue (re->left);
      fprintf (ccode, ",");
      genvalue (re->right);
      fprintf (ccode, ")");
      break;
    case MPRIMARYII:
      fprintf (ccode, "__rpowii(");
      genvalue (re->left);
      fprintf (ccode, ",");
      genvalue (re->right);
      fprintf (ccode, ")");
      break;
    case MPRIMARYRI:
      fprintf (ccode, "__rpowri(");
      genvalue (re->left);
      fprintf (ccode, ",");
      genvalue (re->right);
      fprintf (ccode, ")");
      break;
    case MREAINT:
      fprintf (ccode, "(double)");
      genvalue (re->left);
      break;
    case MINTREA:
      fprintf (ccode, "__rintrea(");
      genvalue (re->left);
      fprintf (ccode, ")");
      break;
    case MCONC:
      fprintf (ccode, "__t[%d]= *__rconc(%ldL,",
	       re->value.combined_stack.entry, 
	       re->value.combined_stack.n_of_stack_elements);
      genvalue (re->left);
      fprintf (ccode, ",");
      genvalue (re->right);
      fprintf (ccode, ");");
      
      break;
    case MTEXTKONST:
      fprintf (ccode, "(__txtvp)&__tk%d%s", re->value.tval.id,
		      re->value.tval.id == NOTEXT ? "" : timestamp);
      break;
    case MCHARACTERKONST:
    case MBOOLEANKONST:
      if (re->value.ival < 0)
	fprintf (ccode, " ");
      fprintf (ccode, "%d", (int) re->value.ival);
      break;
    case MINTEGERKONST:
      if (re->value.ival < 0)
	fprintf (ccode, " ");
      if (re->value.ival == (-MAX_INT - 1))
	{
	  fprintf (ccode, "(%ldL-1L)", re->value.ival+1);
	} else
	{
	  fprintf (ccode, "%ldL", re->value.ival);
	}
      break;
    case MREALKONST:
      if (re->value.rval <= 0.0)
	fprintf (ccode, " ");
      fprintf (ccode, "%.16le", re->value.rval);
      break;
    case MNONE:
      fprintf (ccode, "__NULL");
      break;
    case MSTACK:
      switch (re->type)
	{
	case TREF:
	  fprintf (ccode, "__r[%d]", re->value.entry);
	  break;
	case TINTG:
	  fprintf (ccode, "__v[%d].i", re->value.entry);
	  break;
	case TREAL:
	  fprintf (ccode, "__v[%d].f", re->value.entry);
	  break;
	case TTEXT:
	  fprintf (ccode, "&__t[%d]", re->value.entry);
	  break;
	case TNOTY:
	  break;
	default:
	  fprintf (ccode, "__v[%d].c", re->value.entry);
	  break;
	}
      break;
    case MEXITARGUMENT:
      switch (re->type)
	{
	case TREF:
	  fprintf (ccode, "__er");
	  break;
	case TINTG:
	  fprintf (ccode, "__ev.i");
	  break;
	case TREAL:
	  fprintf (ccode, "__ev.f");
	  break;
	case TTEXT:
	  fprintf (ccode, "&__et");
	  break;
	default:
	  fprintf (ccode, "__ev.c");
	  break;
	}
      break;
    case MIDENTIFIER:
      if (re->type != TLABEL)
	{
	  if (re->rd->categ == CVAR || re->rd->categ == CNAME)
	    {
	      if (re->rd->categ == CVAR && re->rd->kind != KARRAY &&
		  (re->type == TREAL || re->type == TINTG) &&
		  (!(re->up->token == MASSIGN && re->up->left == re)))
		{		/* Lese aksess av aritm. var-parameter. For
				 *   bare er gjort RT-call for skrive-aksess. 
				 */
		  if (re->type == TINTG)
		    {	/* To muligheter : ingen eller real -> int */
		      fprintf (ccode, "((__vvp= &(");
		      gensl (re, TRUE, ON);
		      fprintf (ccode, "%s))->conv==__NOCONV?"
			       " *(long *)(((char *)__vvp->bp)+"
			       "__vvp->ofs):__rintrea("
			       " *(double *)(((char *)__vvp->bp)"
			       "+__vvp->ofs)))",
			       re->rd->ident);
		    }
		  else
		    {	/* Tre muligheter : ingen, int -> real,
			 * real->int->real */
		      fprintf (ccode, "((__vvp= &(");
		      gensl (re, TRUE, ON);
		      fprintf (ccode, "%s))->conv==__NOCONV?"
			       " *(double *)(((char *)__vvp->bp)+__vvp->ofs):"
			       "(__vvp->conv==__INTREAL?(double)"
			       " *(long *)(((char *)__vvp->bp)+__vvp->ofs):"
			       "(double)__rintrea( *(double *)"
			       "(((char *)__vvp->bp)+__vvp->ofs))))",
			       re->rd->ident);
		    }
		}
	      else if (re->rd->categ == CNAME 
		       && re->up->token == MASSIGN &&
		       re->up->right == re)
		{
		  /* Lese-aksess av en name-parameter som det    nettopp
		   * er gjort skrive-aksess p}. Vanligvis  gj|res
		   * konvertering av NAME-parametere av   RT-rutiene, men 
		   * ikke i tilfelle med multippel assignment. Det gj|res 
		   * da her. Noden er     omd|pt fra MNAMEADR til
		   * MIDENTIFER i case   MASSIGN grenen i genvalue. */
			  
		  if (re->type == TINTG)
		    {	/* To muligheter : ingen eller real -> int */
		      fprintf (ccode, "((");
		      gensl (re, TRUE, ON);
		      fprintf (ccode, "%s)->conv==__NOCONV?"
			       " *(long *)(((char *)__r[%d])+__v[%d].i):"
			       "__rintrea("
			       " *(double *)(((char *)__r[%d])+__v[%d].i)))",
			       re->rd->ident,
			       re->value.stack.ref_entry, re->value.stack.val_entry,
			       re->value.stack.ref_entry, (int) re->value.stack.val_entry);
		    }
		  else
		    {       /* Tre muligheter : ingen,int ->
			     * real,real->int->real */
		      fprintf (ccode, "((__nvp= &(");
		      gensl (re, TRUE, ON);
		      fprintf (ccode, "%s))->conv==__NOCONV?"
			       " *(double *)(((char *)__r[%d])+__v[%d].i):"
			       "(__nvp->conv==__INTREAL?(double)"
			       " *(long *)(((char *)__r[%d])+__v[%d].i):"
			       "(double)__rintrea( *(double *)"
			       "(((char *)__r[%d])+__v[%d].i))))",
			       re->rd->ident,
			       re->value.stack.ref_entry, re->value.stack.val_entry,
			       re->value.stack.ref_entry, re->value.stack.val_entry,
			       re->value.stack.ref_entry, re->value.stack.val_entry);
		    }
		}
	      else if (re->type == TREF && re->rd->categ == CVAR &&
		       !(re->up->token == MASSIGNR && re->up->left == re))
		{
		  /* Lese-aksess av referanse var-parametere. Legger inn
		   * kode som sjekker om re er "in" strengeste
		   * kvalifikasjon p} aksessveien. */
			  
		  fprintf (ccode, "((((__vrp= &");
		  gensl (re, TRUE, ON);
		  fprintf (ccode, "%s)->conv==__READTEST "
			   "|| __vrp->conv==__READWRITETEST) &&"
			   " !__rin((__bp= *(__dhp *)(((char *)__vrp->bp)+"
			   "__vrp->ofs)),__vrp->q))?(__dhp)__rerror(__errqual)"
			   ":(__bp= *(__dhp *)(((char *)__vrp->bp)+"
			   "__vrp->ofs)))",
			   re->rd->ident);
		}
	      else
		{
		  /* For parametere av type Character, Boolean,   LESE og 
		   * SKRIVE-AKSESS AV B]DE VAR OG NAME-   PARAMETERE som
		   * ikke er behandlet lengre oppe */
			  
		  if (re->rd->kind == KARRAY) 
		    if (re->rd->categ ==CNAME)
		      fprintf (ccode, "(__arrp)__er");
		    else
		      {
			gensl (re, TRUE, ON);
			fprintf (ccode, "%s", re->rd->ident);
		      }
		  else
		    {
		      if (re->type == TTEXT)
			fprintf (ccode, " (");
		      else
			fprintf (ccode, " *(");
		      gentype (re);
		      fprintf (ccode, " *)(((char *)");
			      
		      gensl (re, TRUE, ON);
		      fprintf (ccode, "%s.", re->rd->ident);
			      
		      if (re->rd->categ == CVAR)
			fprintf (ccode,
				 "bp)+");
		      else
			fprintf (ccode, "bp)+",
				 re->rd->ident);
			      
		      gensl (re, TRUE, ON);
		      fprintf (ccode, "%s.", re->rd->ident);
			      
		      if (re->rd->categ == CVAR)
			fprintf (ccode, "ofs)", re->rd->ident);
		      else
			fprintf (ccode, "v.ofs)",
				 re->rd->ident);
		    }
		}
	    }		/* End Var eller Name-parameter */
	  else
	    {
	      if (re->type == TTEXT && re->rd->kind != KARRAY)
		fprintf (ccode, "(__txtvp)&");
	      gensl (re, TRUE, ON);
	      fprintf (ccode, "%s", re->rd->ident);
	    }
	  break;
	}
      /* Ingen break her */
    case MARRAYARG:
      if (re->type == TLABEL)
	{
	  if (re->token == MARRAYARG)
	    {
	      fprintf (ccode, "__swv=");
	      genvalue (re->right->left);
	      fprintf (ccode, ";");
	    }
	  switch (re->rd->categ)
	    {
	    case CNAME:
	      if (re->token == MIDENTIFIER)
		{
		  /* Transcall har skrevet ut kallet p} rgetlab, slik at adressen
		   * ligger i modul og ev, og objekt-   pekeren ligger i er. */
		  fprintf (ccode, "__rgoto(__er);__goto=__ev.adr;");
		  gotoswitch ();
		  break;
		}
	      /* Inge break her */
	    case CDEFLT:
	    case CVAR:
	      /* Setter bp, en hjelpevariabel, til } peker p} den aktuelle
	       * parameterens blokk. Dermed blir aksessveien kortere
	       * for de etterf|lgende aksessene */
	      fprintf (ccode, "__bp=");
	      gensl (re, FALSE, ON);
	      fprintf (ccode, ";__rgoto(((__bs%d *)__bp)->%s.ob);" 
		       "__goto=((__bs%d *)__bp)->%s.adr;",
		       re->rd->encl->blno, re->rd->ident,
		       re->rd->encl->blno, re->rd->ident);
	      gotoswitch ();
	      break;
	    case CVIRT:
	      if (cblock->blev > re->rd->encl->blev)
		{
		  fprintf (ccode, "__rgoto(");
		  gensl (re, FALSE, ON);
		  fprintf (ccode, ");");
		  fprintf (ccode, "if((__pp=__lb");
		}
	      else
		{
		  fprintf (ccode, "if((__pp=");
		  genchain (re->rd->encl, FALSE);
		}

	      fprintf (ccode, "->pp)->virtlab[%d].ent==0)"
		       "__rerror(__errvirt);__goto=__pp->virtlab[%d];",
		       re->rd->virtno - 1, re->rd->virtno - 1);
	      gotoswitch ();
	      break;
	    case CLOCAL:
	      if (cblock->blev > re->rd->encl->blev)
		{
		  fprintf (ccode, "__rgoto(");
		  gensl (re, FALSE, ON);
		  fprintf (ccode, ");");
		}
	      if (re->rd->plev == 0)
		re->rd->plev = newlabel ();
	      if (re->rd->encl->timestamp != 0)
		{
		  /* Skal hoppe til en label i en annen modul */
		  fprintf (ccode, "__goto.ent=%d;__goto.ment=", 
			   re->rd->plev);
		  genmodulemark(re->rd->encl->timestamp);
		  fprintf (ccode, ";");
		  gotoswitch ();
		}
	      else
		gotolabel (re->rd->plev);
	      break;
	    }
	  not_reached = TRUE;
	} 
      else
	{
	  int i, dim;
	  /* Legger ut kode som sjekker indeksene og at det antall indekser
	   * stemmer med dimmensjonen */

	  if (re->rd->categ != CNAME)	/* Name er behandlet ovenfor */
	    {
	      fprintf (ccode, "__r[%d]=(__dhp)", re->value.stack.ref_entry);
	      gensl (re, TRUE, ON);
	      fprintf (ccode, "%s;", re->rd->ident);
	    }

	  if (re->rd->dim == 0)
	    {
	      /* Array som parameter.M} legge ut kode som sjekker dimmensjonen
	       * ved Run-time */
	      dim= 0;
	      for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
		dim++;

	      fprintf 
		(ccode, "((__arrp)__r[%d])->h.dim!=%d?__rerror(__errarr):1;", 
		 re->value.stack.ref_entry, dim, re->rd->ident);
	    }
	  dim= 0;
	  for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
	    {
	      if (dim == MAX_ARRAY_DIM)
		gerror (85);
	      fprintf (ccode, "__h[%d]=", dim++);
	      genvalue (rex->left);
	      fprintf (ccode, "-((__arrp)__r[%d])->limits[%d].low;", 
		       re->value.stack.ref_entry, dim - 1);
	    }
	  fprintf (ccode, "if(");
	  for (i = 0; i < dim; i++)
	    {
	      fprintf (ccode, "__h[%d]<0 || __h[%d]>=((__arrp)"
		       "__r[%d])->limits[%d].size",
		       i, i, re->value.stack.ref_entry, i);
	      if (i < dim - 1)
		fprintf (ccode, " ||   ");
	    }
	  fprintf (ccode, ")__rerror(__errbound);"
		   "__v[%d].i=sizeof(__ah)+sizeof(__arrlimit)*%d+((",
		   re->value.stack.val_entry, dim);
	  for (i = dim - 1; i > 0; i--)
	    {
	      fprintf (ccode, "((__arrp)__r[%d])->limits[%d].size*(",
		       re->value.stack.ref_entry, i);
	    }

	  fprintf (ccode, "__h[0])");

	  for (i = 1; i < dim; i++)
	    fprintf (ccode, "+__h[%d])", i);
	  fprintf (ccode, "*sizeof(");
	  gentype (re);
	  fprintf (ccode, "));");
	}
      break;
    case MNAMEREADACESS:
    case MNAMEWRITEACESS:
      fprintf (ccode, "if(");
      if (re->rd->kind == KPROC)
	fprintf (ccode, "__rgetproc(");
      else if (re->rd->kind == KARRAY)
	fprintf (ccode, "__rgeta(");
      else if (re->token == MNAMEWRITEACESS)
	fprintf (ccode, "__rgetsa(");
      else
	switch (re->type)
	  {
	  case TINTG:
	    fprintf (ccode, "__rgetav(__TINTG,");
	    break;
	  case TREAL:
	    fprintf (ccode, "__rgetav(__TREAL,");
	    break;
	  case TCHAR:
	  case TBOOL:
	    fprintf (ccode, "__rgetcbv(");
	    break;
	  case TREF:
	    fprintf (ccode, "__rgetrv(");
	    break;
	  case TTEXT:
	    fprintf (ccode, "__rgetta(");
	    break;
	  case TLABEL:
	    fprintf (ccode, "__rgetlab(");
	    break;
	  }
      fprintf (ccode, "&");
      gensl (re, TRUE, ON);
      {
	int i;
	fprintf (ccode, "%s,%ldL,%d,", re->rd->ident, 
		 re->value.n_of_stack_elements, i = newlabel ());
	genmodulemark(NULL);
	fprintf (ccode, "))");
	exitcondlabel (i);
      }
      break;
    case MNAMEREADTEXT:
      fprintf (ccode, "switch (");
      gensl (re, TRUE, ON);
      fprintf (ccode, "%s.namekind){"
	       "case __ADDRESS_THUNK: case __ADDRESS_NOTHUNK: "
	       "__v[%d].i=__ev.i;__r[%d]=__er;" 
	       "break; case __VALUE_THUNK: case __VALUE_NOTHUNK: "
	       "__t[%d]=__et;__r[%d] = __NULL;"
	       "__v[%d].i = ((char *)&__t[%d])-((char *) 0);}",
	       re->rd->ident, re->value.stack.val_entry, re->value.stack.ref_entry,
	       re->value.stack.txt_entry, re->value.stack.ref_entry,
	       re->value.stack.val_entry, re->value.stack.txt_entry);
      break;
    case MPROCASSIGN:
      if (re->type == TNOTY)
	break;
      if (re->type == TTEXT)
	fprintf (ccode, "&");
      genchain (re->rd->descr, TRUE);
      if (re->type == TREF)
	fprintf (ccode, "er");
      else if (re->type == TTEXT)
	fprintf (ccode, "et");
      else if (re->type == TREAL)
	fprintf (ccode, "ef");
      else if (re->type == TINTG)
	fprintf (ccode, "ev");
      else
	fprintf (ccode, "ec");
      break;
    case MTHIS:
      if (seen_th_insp (re))
	{
	  genchain (re->seenthrough->quant.match->descr, TRUE);
	  fprintf (ccode, "c%d", re->seenthrough->connest);
	}
      else
	genchain (re->qual->descr, FALSE);
      break;
    case MQUA:
      /* Sjekker om det er n\dvendig } utf\re en none-test, eller om den er
       * utf\rt lengre ned i treet. */
      if (re->left->token != MDOT && re->left->token != MQUA &&
	  re->left->token != MQUANOTNONE && nonetest == ON)
	{
	  fprintf (ccode, "((__bp=");
	  genvalue (re->left);
	  fprintf (ccode, ")==__NULL?(__dhp)__rerror(__errnone):(__bp");
	}
      else
	{
	  fprintf (ccode, "(((__bp=");
	  genvalue (re->left);
	  fprintf (ccode, ")");
	}
      if (re->qual->plev >= DEF_PLEV_TAB_SIZE)
	fprintf (ccode, "->pp->plev<%d || __bp",
			re->qual->plev);

      fprintf (ccode, "->pp->pref[%d] != ",
		      re->qual->plev);
      gen_adr_prot (ccode, re->qual);
      fprintf (ccode, ")?(__dhp)__rerror(__errqual):__bp)");
      break;
    case MQUANOTNONE:
      /* Sjekker om det er n\dvendig } utf\re en none-test, eller om den er
       * utf\rt lengre ned i treet. */
      if (re->left->token != MDOT && re->left->token != MQUA &&
	  re->left->token != MQUANOTNONE)
	{
	  if (nonetest == ON)
	    fprintf (ccode, "((__bp=");
	  genvalue (re->left);
	  if (nonetest == ON)
	    fprintf (ccode,
			    ")==__NULL?(__dhp)__rerror(__errnone):__bp)");
	}
      else
	genvalue (re->left);
      break;
    case MQUANONEAND:
      if (re->left->token == MNONE)
	fprintf (ccode, "__NULL");
      else
	{
	  fprintf (ccode, "(((__bp=");
	  genvalue (re->left);
	  fprintf (ccode, ")!=__NULL && (");
	  if (re->qual->plev >= DEF_PLEV_TAB_SIZE)
	    fprintf (ccode, "__bp->pp->plev<%d || ",
			    re->qual->plev);
	  fprintf (ccode, "__bp->pp->pref[%d]!= ",
			  re->qual->plev);
	  gen_adr_prot (ccode, re->qual);
	  fprintf (ccode, "))?(__dhp)__rerror(__errqual):__bp)");
	}
      break;
    case MDOT:
      genvalue (re->right);
      break;
    case MDOTCONST:
      if (nonetest == ON)
	fprintf (ccode, "((");
      if (nonetest == ON)
	genvalue (re->left);
      if (nonetest == ON)
	fprintf (ccode, ")==__NULL?(");
      gentype (re);
      if (re->type == TTEXT)
	fprintf (ccode, " *");
      if (nonetest == ON)
	fprintf (ccode, ")__rerror(__errnone):");
      genvalue (re->right);
      if (nonetest == ON)
	fprintf (ccode, ")");

      break;
    case MTEXTADR:
    case MNAMEADR:
    case MARRAYADR:
      if (re->type == TTEXT)
	fprintf (ccode, " ((");
      else
	fprintf (ccode, " (*(");
      gentype (re);
      fprintf (ccode, " *)(((char *)__r[%d])+__v[%d].i))", 
	       re->value.stack.ref_entry, re->value.stack.val_entry);
      break;
    case MREFASSIGNT:
      fprintf (ccode, "__rtextassign(");
      genvalue (re->left);
      fprintf (ccode, ",");
      genvalue (re->right);
      fprintf (ccode, ")");
      break;
    case MVALASSIGNT:
      fprintf (ccode, "__rtextvalassign(");
      genvalue (re->left);
      fprintf (ccode, ",");
      genvalue (re->right);
      fprintf (ccode, ")");
      break;
    case MASSIGND:
       genvalue (re->left);
       fprintf(ccode, "= ");
       genvalue (re->right);
       break;
    case MASSIGNADD:
       genvalue (re->left);
       fprintf(ccode, "+= ");
       genvalue (re->right);
       break;
    case MASSIGN:
      if (re->right->token == MASSIGN)
	{
	  genvalue (re->right);
	  fprintf (ccode, ";");
	}
      fprintf (ccode, "(");

      if (re->left->rd!=NULL && re->left->rd->kind == KSIMPLE &&
	  (re->left->rd->categ == CVAR || re->left->rd->categ == CNAME)
	  && (re->left->type == TINTG || re->left->type == TREAL))
	{			/* For aritmetiske, for } h{ndtere
				 * konvertering. */
	  if (re->type == TREAL)
	    fprintf (ccode, "__ev.f");
	  else if (re->type == TINTG)
	    fprintf (ccode, "__ev.i");
	}
      else
	genvalue (re->left);
      fprintf (ccode, "=");
      if (re->left->type == TINTG && re->right->type == TREAL)
	fprintf (ccode, "__rintrea");
      fprintf (ccode, "(");
      if (re->right->token == MASSIGN)
	{
	  if (re->right->left->token == MNAMEADR 
	      || re->right->left->token == MTEXTADR)
	    {
	      if (re->right->left->type == TREAL)
		fprintf (ccode, "__ev.f");
	      else if (re->right->left->type == TINTG)
		fprintf (ccode, "__ev.i");
	      else genvalue (re->right->left);
	    }
	  else
	    genvalue (re->right->left);
	}
      else
	genvalue (re->right);
      fprintf (ccode, "))");
      if (re->left->rd!= NULL && re->left->rd->kind == KSIMPLE)
	{
	  if (re->left->rd->categ == CVAR &&
	      (re->type == TINTG || re->type == TREAL))

	    {
	      /* SKRIVE-AKSESS AV ARIT. VAR-PARAMETER */
	      fprintf (ccode, ";");

	      /* M| sjekke ved runtime om det skal gjores
	       * konvertering. */

	      if (re->type == TINTG)
		{
		  /* To muligheter : ingen, real -> int */
		  fprintf (ccode, "if((__vvp= &");
		  gensl (re->left, TRUE, ON);
		  fprintf (ccode, "%s)->conv==__NOCONV)"
			   " *(long *)(((char *)__vvp->bp)+"
			   "__vvp->ofs)=__ev.i;"
			   "else *(double *)(((char *)__vvp->bp)+"
			   "__vvp->ofs)=__ev.i",
			   re->left->rd->ident);
		}
	      else
		{		/* Tre muligheter : ingen, int -> real, og 
				 * real ->int ->real */
		  fprintf (ccode, "if((__vvp= &");
		  gensl (re->left, TRUE, ON);
		  fprintf (ccode, "%s)->conv==__NOCONV)"
			   "if(__vvp->conv==__INTREAL)"
			   " *(double *)(((char *)__vvp->bp)+"
			   "__vvp->ofs)=__ev.f;else "
			   " *(long *)(((char *)__vvp->bp)+"
			   "__vvp->ofs)=__ev.f;else "
			   " *(double *)(((char *)__vvp->bp)+"
			   "__vvp->ofs)=__rintrea(__ev.f)",
			   re->left->rd->ident);
		}
	    }
	  else if (re->left->rd->categ == CNAME &&
		   (re->type == TINTG || re->type == TREAL))
	    {
	      /* SKRIVE-AKSESS AV ARIT. NAME-PARAMETER */
	      fprintf (ccode, ";");

	      /* M| sjekke ved runtime om det skal gjores konvertering. */

	      if (re->type == TINTG)
		{		/* To muligheter : ingen, real -> int */
		  fprintf (ccode, "if(");
		  gensl (re->left, TRUE, ON);
		  fprintf (ccode, "%s.conv==__NOCONV)"
			   " *(long *)(((char *)__r[%d])+__v[%d].i)=__ev.i;"
			   "else *(double *)(((char *)__r[%d])+__v[%d].i)="
			   "__ev.i",
			   re->left->rd->ident,
			   re->left->value.stack.ref_entry, 
			   re->left->value.stack.val_entry,
			   re->left->value.stack.ref_entry, 
			   re->left->value.stack.val_entry);
		}
	      else
		{		/* Tre muligheter : ingen, int -> real og
				 * real ->int ->real */
		  fprintf (ccode, "if((__nvp= &");
		  gensl (re->left, TRUE, ON);
		  fprintf (ccode, "%s)->conv==__NOCONV)"
			   " *(double *)(((char *)__r[%d])+"
			   "__v[%d].i)=__ev.f;else "
			   "if(__nvp->conv==__INTREAL)"
			   " *(long *)(((char *)__r[%d])+"
			   "__v[%d].i)=__ev.f;else "
			   " *(double *)(((char *)__r[%d])+"
			   "__v[%d].i)=__rintrea(__ev.f)",
			   re->left->rd->ident,
			   re->left->value.stack.ref_entry, 
			   re->left->value.stack.val_entry,
			   re->left->value.stack.ref_entry, 
			   re->left->value.stack.val_entry,
			   re->left->value.stack.ref_entry, 
			   re->left->value.stack.val_entry);
		}
	    }
	}
      break;
    case MINSTRONGEST:
      fprintf (ccode, "if(((__nrp= &(");
      gensl (re->left, TRUE, ON);
      fprintf (ccode, "%s))->conv==__WRITETEST || __nrp->conv=="
	       "__READWRITETEST) && !__rin(", re->left->rd->ident);
      genvalue (re->right);
      fprintf (ccode, ",__nrp->q))__rerror(__errqual);");
      break;
    case MASSIGNR:
      genvalue (re->left);
      fprintf (ccode, "=");
      if ((rex = re->left)->token == MIDENTIFIER && rex->rd->categ == CVAR)
	{
	  /* SKRIVE-AKSESS P} REFERANSE VAR-PARAMETER              M} da
	   * legge inn kode som ,hvis n|dvendig , sjekker om h|yre side er
	   * 'in' strengeste kvalifikasjon p}        aksessveien */

	  fprintf (ccode, "((((__vrp= &");
	  gensl (rex, TRUE, ON);
	  fprintf (ccode, "%s)->conv==__WRITETEST "
		   "|| __vrp->conv==__READWRITETEST) && !__rin((__bp= ",
		   rex->rd->ident);
	  genvalue (re->right);
	  fprintf 
	    (ccode, "),__vrp->q))?(__dhp)__rerror(__errqual):(__bp=");
	  genvalue (re->right);
	  fprintf (ccode, "))");
	}
      else
	genvalue (re->right);
      break;
    case MNOOP:
      if (re->type == TTEXT)
	{
	  /* Parantes i forbindelse med tekster. Venstre-siden skal legges p} 
	   * en anonym tekst-variabel. */
	  fprintf (ccode, "__rtextassign(&__et,");
	  genvalue (re->left);
	  fprintf (ccode, ")");
	}
      else
	genvalue (re->left);
      break;
    case MSL:
      
      break;
    case MSENTCONC:
      genvalue (re->left);
      fprintf (ccode, ";");
      genvalue (re->right);
      break;
    case MDIV:
    case MINTDIV:
      putc ('(', ccode);
      genvalue (re->left);
      fprintf (ccode, "/");

#ifdef DIV0
      if (re->token == MINTDIV) 
	fprintf (ccode, "__ridiv0 (");
      else
	fprintf (ccode, "__rrdiv0 (");
#endif /* DIV0 */

      genvalue (re->right);
      putc (')', ccode);

#ifdef DIV0
      putc (')', ccode); 
#endif /* DIV0 */
      break;
    default:
      putc ('(', ccode);
      if (re->left->type == TCHAR)
	fprintf (ccode, "(unsigned char)");
      genvalue (re->left);
      switch (re->token)
	{
	case MORELSEE:
	  fprintf (ccode, "||");
	  break;
	case MANDTHENE:
	  fprintf (ccode, "&&");
	  break;
	case MOR:
	  fprintf (ccode, "|");
	  break;
	case MAND:
	  fprintf (ccode, "&");
	  break;
	case MEQV:
	case MEQ:
	case MEQR:
	  fprintf (ccode, "==");
	  break;
	case MNE:
	case MNER:
	  fprintf (ccode, "!=");
	  break;
	case MLT:
	  fprintf (ccode, "<");
	  break;
	case MLE:
	  fprintf (ccode, "<=");
	  break;
	case MGT:
	  fprintf (ccode, ">");
	  break;
	case MGE:
	  fprintf (ccode, ">=");
	  break;
	case MADD:
	case MADDI:
	  fprintf (ccode, "+");
	  break;
	case MSUB:
	case MSUBI:
	  fprintf (ccode, "-");
	  break;
	case MMUL:
	case MMULI:
	  fprintf (ccode, "*");
	  break;
	default:
#ifdef DEBUG
	  fprintf (stderr, "Illegal token:%s\n"
			  ,texttoken (re->token));
#else
	  fprintf (stderr, "Illegal token:%d\n"
			  ,re->token);
#endif
	  break;
	}
      if (re->left->type == TCHAR)
	fprintf (ccode, "(unsigned char)");
      genvalue (re->right);
      putc (')', ccode);
    }
}

/*****************************************************************************
						              GEN_TEXTCONST */

gen_textconst (re) struct EXP *re;
{
  if (re->value.tval.id == NOTEXT)
    {
      char *t;
      static int it = 0;
      int antchar;
      t = re->value.tval.txt;
      antchar = sstrlen (t);
      if (antchar == 0)
	re->value.tval.id = NOTEXT;
      else
	{
	  anttext++;
	  fprintf (ccode, "struct __tt%d {__txt tvar;__th h;"
		   "char string[%d];}\n__tk%d%s={(__textref)"
		   "&__tk%d%s.h.pp,%d,1,1,(__pty)__TEXT,"
		   "(__dhp)&__tk%d%s.h.pp,__CONSTANT,%d,\"%s\"};\n",
		   anttext, antchar + 1,
		   anttext, timestamp, anttext, timestamp,
		   antchar, anttext, timestamp, antchar, t);
	  
	  re->value.tval.id = anttext;
	}
    }
}
