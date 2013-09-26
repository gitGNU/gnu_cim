/* $Id: cgenstr.c,v 1.11 1995/12/21 15:13:30 cim Exp $ */

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

/* Legger ut C-kode for hvert blokk objekt. */

#include "const.h"
#include "cimcomp.h"
#include "error.h"
#include "extspec.h"
#include "mapline.h"
#include "name.h"

static short plevnull;		/* Hvis en blokks prefiksniv} er 0 s} er 
				 * plevnull=TRUE.Brukes for } initsialisere
				 * offset adressene til pekerne.M} vite om 
				 * structen til denne blokken inneholder   
				 * deklarasjonen struct dhp h.Ellers s} m} 
				 * .s f}lges plev ganger for } komme til h.pp 
				 */


static int naref;

/******************************************************************************
								  WRITE_DECL */

static void write_decl (decl_t *rd, char *type, char output_refs)
{
  if (!output_refs)
    {
      fprintf (ccode, "\t%s %s;\n", type, rd->ident);
    }
}


/******************************************************************************
								  WRITE_REFS */

static void write_refs (block_t *rb, decl_t *rd, char *atrib, char output_refs)
{

  /* TBD Hvis rd alltil hadde vært forskjellig fra NULL kunne rb sløyfes
     og i stedet for rb->blno brukte man rd->encl->blno, Dersom
     de laged DECL objekter or et, er samt connester kan det gjøres.*/

  if (output_refs)
    fprintf (ccode, "(short)((char *)&((__bs%d *)0)->%s%s-(char *)0),",
	     rb->blno, rd==NULL?"":rd->ident, atrib);
  else
    naref++;
}




/******************************************************************************
                                                        DECLSTRUCTURE        */

static void declstructure (decl_t *rd, char output_refs)
{
  char write = 0;
  if (rd->kind == KSIMPLE)
    {
      if (rd->categ == CVAR)
	{
	  if (rd->type == TREF)
	    write_decl (rd, "__refvarpar ", output_refs);
	  else if (rd->type == TINTG || rd->type == TREAL)
	    write_decl (rd, "__aritvarpar ", output_refs);
	  else if (rd->type == TLABEL)
	    write_decl (rd, "__labelswitchpar ", output_refs);
	  else
	    write_decl (rd, "__varpar ", output_refs);

	  if (rd->type == TLABEL)
	    write_refs (rd->encl, rd, ".ob", output_refs);
	  else
	    write_refs (rd->encl, rd, ".bp", output_refs);
	}
      else if (rd->categ == CNAME)
	{
	  if (rd->type == TREF)
	    write_decl (rd, "__refnamepar ", output_refs);
	  else if (rd->type == TINTG || rd->type == TREAL)
	    write_decl (rd, "__aritnamepar ", output_refs);
	  else if (rd->type == TTEXT)
	    write_decl (rd, "__textnamepar ", output_refs);
	  else if (rd->type == TLABEL)
	    write_decl (rd, "__labelnamepar ", output_refs);
	  else
	    write_decl (rd, "__charboolnamepar ", output_refs);

	  if (rd->type == TLABEL)
	    write_refs (rd->encl, rd, ".ob", output_refs);
	  else
	    write_refs (rd->encl, rd, ".bp", output_refs);

	  if (rd->type == TLABEL)
	    write_refs (rd->encl, rd, ".sl", output_refs);
	  else
	    write_refs (rd->encl, rd, ".sl", output_refs);
	}
      else if (rd->categ == CCONST);
      else
	{
	  switch (rd->type)
	    {
	    case TINTG:
	      write_decl (rd, "long ", output_refs);
	      break;
	    case TREAL:
	      write_decl (rd, "double ", output_refs);
	      break;
	    case TBOOL:
	      write_decl (rd, "char ", output_refs);
	      break;
	    case TCHAR:
	      write_decl (rd, "char ", output_refs);
	      break;
	    case TLABEL:
	      if (rd->categ != CLOCAL)
		{
		  write_decl (rd, "__labelswitchpar ", output_refs);
		  write_refs (rd->encl, rd, ".ob", output_refs);
		}
	      break;
	    case TTEXT:
	      write_decl (rd, "__txt ", output_refs);
	      write_refs (rd->encl, rd, ".obj", output_refs);
	      break;
	    case TREF:
	      write_decl (rd, "__dhp ", output_refs);
	      write_refs (rd->encl, rd, "", output_refs);
	      break;
	    case TVOIDP:
	      write_decl (rd, "void *", output_refs);
	      break;
	    }
	  /* END-SWITCH */
	}
    }
  else if (rd->kind == KARRAY)
    {
      if (rd->type == TLABEL && rd->categ != CLOCAL)
	{
	  write_decl (rd, "__labelswitchpar ", output_refs);
	  write_refs (rd->encl, rd, ".ob", output_refs);
	}
      else if (rd->categ == CNAME)
	{
	  write_decl (rd, "__arraynamepar ", output_refs);
	  write_refs (rd->encl, rd, ".sl", output_refs);
	  write_refs (rd->encl, rd, ".ap", output_refs);
	}
      else
	{
	  write_decl (rd, "__arrp ", output_refs);

	  write_refs (rd->encl, rd, "", output_refs);
	}
    }
  else if (rd->kind == KPROC)
    {
      if (rd->categ == CDEFLT || rd->categ == CVAR)
	{
	  if (rd->type == TINTG || rd->type == TREAL)
	    write_decl (rd, "__aritprocpar ", output_refs);
	  else if (rd->type == TREF)
	    write_decl (rd, "__refprocpar ", output_refs);
	  else
	    write_decl (rd, "__forprocpar ", output_refs);

	  write_refs (rd->encl, rd, ".psl", output_refs);
	}
      else if (rd->categ == CNAME)
	{
	  if (rd->type == TINTG || rd->type == TREAL)
	    write_decl (rd, "__aritprocnamepar ", output_refs);
	  else if (rd->type == TREF)
	    write_decl (rd, "__refprocnamepar ", output_refs);
	  else
	    write_decl (rd, "__simpleprocnamepar ", output_refs);
	  write_refs (rd->encl, rd, ".psl", output_refs);
	  write_refs (rd->encl, rd, ".sl", output_refs);
	}
    }
}


/******************************************************************************
                                                         skrivprefikspp()    */

/* Hjelpe prosedyre som g}r rekursift gjennom prefikskjeden helt til
 * plev=0,og mens den trekker seg tilbake skriver den ut &p<blno til klassen>
 * til klassen.Kalles fra blockstructure for } initsiere prefiksarrayet
 * i prototypene.Da blir det slik at en prototype for en klasse p}
 * prefiksniv} n vil i arrayet sitt ha en peker til seg selv p} plass
 * 0,til sin superklasse p} plass n-1,dens superklasse p} plass n-2
 * osv. */

static void skrivprefikspp (decl_t *rd)
{
  if (rd != NULL)
    {
      if (rd->plev > 0)
	{
	  skrivprefikspp (rd->prefqual);
	  fprintf (ccode, ",");
	}
    gen_adr_prot (ccode, rd);
    }
}



/******************************************************************************
                                                    BLOCKMAINSTRUCTURE       */

static void blockmainstructure (block_t *rb, char output_refs)
{
  int i;
  decl_t *rd;
  if (rb->quant.kind == KPROC && rb->quant.type != TNOTY)
    {
      if (rb->quant.type == TTEXT)
	write_refs (rb, NULL, "et.obj", output_refs); 
      else if (rb->quant.type == TREF)
	write_refs (rb, NULL, "er", output_refs); 
    }
  
  {
    int mincon= 1;
    if ((rb->quant.kind == KCLASS || rb->quant.kind == KPRBLK) && 
	rb->quant.plev > 0)
      {
	if (rb->connest < rb->quant.prefqual->descr->connest)
	  rb->connest= rb->quant.prefqual->descr->connest;
	mincon= rb->quant.prefqual->descr->connest+1;
      } 
    for (i = mincon; i <= rb->connest; i++)
      {
	char s[10];
	sprintf (s, "c%d", i);
	write_refs (rb, NULL, s, output_refs);
      };
  }

#if ACSTACK_IN_OBJ
  {
    int minref= 1, mintxt= 1;
    if ((rb->quant.kind == KCLASS || rb->quant.kind == KPRBLK) && 
	rb->quant.plev > 0)
      {
	minref= rb->quant.prefqual->descr->maxusedref+1;
	mintxt= rb->quant.prefqual->descr->maxusedtxt+1; 
      }

      for (i= minref; i <= rb->maxusedref; i++)
	{
	  char s[20];
	  sprintf (s, "__r%d", i);
	  write_refs (rb, NULL, s, output_refs);
	}
    
    for (i= mintxt; i <= rb->maxusedtxt; i++)
      {
	char s[20];
	sprintf (s, "__t%d.obj", i);
	write_refs (rb, NULL, s, output_refs);
      }
  }
#endif

  for (rd = rb->parloc; rd != NULL; rd = rd->next)
    declstructure (rd, output_refs);
}

static void specifier_structure (block_t *rb);
/******************************************************************************
                                                        BLOCKSTRUCTURE       */

static blockstructure (block_t *rb)
{
  int i;
  decl_t *rd;

#if 0
  if (rb->blno < 11)
    return;
#endif

  if (rb->structure_written)
    return;			/* Blokken er allerede behandlet */


  switch (rb->quant.kind)
    {
    case KPROC:
    case KCLASS:
    case KBLOKK:
    case KPRBLK:
      if (rb->quant.kind == KPROC)
	{
	  if (rb->quant.categ == CCPROC)
	    {
	      /* Extern C-prosedyre */
	      fprintf (ccode, "extern ");
	      switch (rb->quant.type)
		{
		case TTEXT:
		  fprintf (ccode, "char *");
		  break;
		case TINTG:
		  fprintf (ccode, "long ");
		  break;
		case TREAL:
		  fprintf (ccode, "double ");
		  break;
		case TBOOL:
		case TCHAR:
		  fprintf (ccode, "char ");
		  break;
		}
	      fprintf (ccode, "%s();\n", rb->rtname);
	      break;
	    }
	  else if (rb->codeclass != CCNO) break;
	}
      
      if ((rb->quant.kind == KCLASS || rb->quant.kind == KPROC) 
	  && rb->timestamp != 0 &&
	  rb->quant.encl->timestamp != rb->timestamp)
	{
	  
	  if (&rb->quant == classtext || &rb->quant == commonprefiks) break;
	  /* Definerer den eksterne modulen som extern på .h filen */
	  fprintf (ccode, "extern void __m_%s();\n",
		   rb->timestamp);

	}

      if ((rb->quant.kind == KCLASS || rb->quant.kind == KPRBLK) 
	  && rb->timestamp == 0)
	{
	  fprintf (ccode, "extern __ptyp __p%d%s;\n", rb->blno,
		   timestamp);
	  for (rd = rb->virt; rd != NULL; rd = rd->next)
	    if (rd->kind == KPROC && rd->match!= NULL) 
	      blockstructure (rd->match->descr);
	}
      
      if ((rb->quant.kind == KCLASS && rb->quant.plev > 0) 
	  || (rb->quant.kind == KPRBLK))
	{				/* Går gjennom prefikskjeden */
	  blockstructure (rb->quant.prefqual->descr);
	  plevnull = FALSE;
	} 
      else
	plevnull= TRUE;

      fprintf (ccode, "typedef struct /* %s */\n    {\n"
	       ,rb->quant.ident == NULL ? "" : rb->quant.ident);

      if ((rb->quant.kind == KCLASS && rb->quant.plev > 0) 
	  || (rb->quant.kind == KPRBLK))
	fprintf (ccode, "        __bs%d s;\n",
		 rb->quant.prefqual->descr->blno);
      else
	fprintf (ccode, "        __dh h;\n");

      naref = 0;
      /* NB !!!. Deklarasjonene må skrives ut før evt. hjelpe variable 
   * (for,inspect) og før returverdivariabelen. Slipper da å skrive
   * ut disse i structene for virtuelle og formelle prosedyre 
   * spesifikasjoner. Gjelder prosedyrer. */
      blockmainstructure (rb, FALSE);

      if (rb->quant.kind == KPROC && rb->quant.type != TNOTY)
	{
	  if (rb->quant.type == TTEXT)
	    fprintf (ccode, "        __txt et;\n");
	  else if (rb->quant.type == TREF)
	    fprintf (ccode, "        __dhp er;\n");
	  else if (rb->quant.type == TINTG)
	    fprintf (ccode, "        long ev;\n");
	  else if (rb->quant.type == TREAL)
	    fprintf (ccode, "        double ef;\n");
	  else
	    fprintf (ccode, "        char ec;\n");
	}

      {
	int minfor= 1, mincon=1;
	if ((rb->quant.kind == KCLASS || rb->quant.kind == KPRBLK) && 
	    rb->quant.plev > 0)
	  {
	    if (rb->fornest < rb->quant.prefqual->descr->fornest)
	      rb->fornest= rb->quant.prefqual->descr->fornest;
	    if (rb->connest < rb->quant.prefqual->descr->connest)
	      rb->connest= rb->quant.prefqual->descr->connest;
	    minfor= rb->quant.prefqual->descr->fornest+1;
	    mincon= rb->quant.prefqual->descr->connest+1;
	  } 
	for (i = minfor; i <= rb->fornest; i++)
	  fprintf (ccode, "        short f%d;\n", i);
	for (i = mincon; i <= rb->connest; i++)
	  fprintf (ccode, "        __dhp c%d;\n", i);
      }

#if ACSTACK_IN_OBJ
      {
	int minval= 1, minref= 1, mintxt= 1;
	if ((rb->quant.kind == KCLASS || rb->quant.kind == KPRBLK) && 
	    rb->quant.plev > 0)
	  {
	    if (rb->maxusedref < rb->quant.prefqual->descr->maxusedref)
	      rb->maxusedref= rb->quant.prefqual->descr->maxusedref;
	    if (rb->maxusedtxt < rb->quant.prefqual->descr->maxusedtxt)
	      rb->maxusedtxt= rb->quant.prefqual->descr->maxusedtxt;
	    if (rb->maxusedval < rb->quant.prefqual->descr->maxusedval)
	      rb->maxusedval= rb->quant.prefqual->descr->maxusedval;
	    minref= rb->quant.prefqual->descr->maxusedref+1;
	    mintxt= rb->quant.prefqual->descr->maxusedtxt+1; 
	    minval= rb->quant.prefqual->descr->maxusedval+1; 
	  } 
	for (i= minref; i<=rb->maxusedref; i++) 
	  fprintf (ccode, "        __dhp __r%d;\n", i);
	for (i= mintxt; i<=rb->maxusedtxt; i++) 
	  fprintf (ccode, "        __txt __t%d;\n", i);
	for (i= minval; i<=rb->maxusedval; i++) 
	  fprintf (ccode, "        __valuetype __v%d;\n", i);
      }

#endif

      fprintf (ccode, "    } __bs%d;\n", rb->blno);

      if (rb->stat)
	{
	  if (rb->timestamp) fprintf (ccode, "extern ");
	  fprintf 
	    (ccode, "__bs%d __blokk%d%s;\n", rb->blno, rb->blno, 
	     rb->timestamp?rb->timestamp:timestamp);
	}

      if (rb->timestamp != 0)
	{
	  fprintf (ccode, "extern __ptyp __p%d%s;\n", rb->ptypno,
		   rb->timestamp);
	}
      else
	{
	  if (naref)
	    {
	      fprintf (ccode, "short __rl%d%s[%d]={", 
		       rb->blno, timestamp, naref);

	      blockmainstructure (rb, TRUE);

	      fprintf (ccode, "};\n");
	    }

	  if ((rb->quant.kind == KCLASS || rb->quant.kind == KPRBLK) &&
	      (rb->navirt || rb->navirtlab))
	    {
	      if (rb->navirt)
		{
		  fprintf (ccode, "__pty   __vl%d%s[%d]={", 
			   rb->blno, timestamp, rb->navirt);
		  for (rd = rb->virt; rd != NULL; rd = rd->next)
		    {
		      if (rd->kind == KPROC)
			{
			  if (rd->match != NULL)
			    {
			      gen_adr_prot (ccode, rd->match);
			      fprintf (ccode, ",");
			    }
			  else
			    fprintf (ccode, "__NULL,");
			}
		    }
		  fprintf (ccode, "};\n");
		}

	      if (rb->navirtlab)
		{
		  fprintf (ccode, "__progadr   __labvl%d%s[%d]={", rb->blno,
			   timestamp, rb->navirtlab);
		  for (rd = rb->virt; rd != NULL; rd = rd->next)
		    {
		      if (rd->kind != KPROC)
			{
			  if (rd->match != NULL)
			    {
			      if (rd->match->plev == 0)
				rd->match->plev = newlabel ();

			      if (rd->match->encl->timestamp != 0)
				fprintf (ccode, "%d,__m_%s,",
					 rd->match->plev,
					 rd->match->encl->timestamp);
			  
			      else if (separat_comp)
				fprintf (ccode, "%d,__m_%s,",
					 rd->match->plev, timestamp);
			      else
				fprintf (ccode, "%d,__NULL,",
					 rd->match->plev);
			    }
			  else
			    fprintf (ccode, "0,__NULL,");
			}
		    }
		  fprintf (ccode, "};\n");
		}
	    }

	  fprintf (ccode, "extern __ptyp __p%d%s;__pty   __pl%d%s[%d]={", 
		   rb->blno, timestamp,
		   rb->blno, timestamp,
		   (rb->quant.prefqual==NULL)?1:
		   ((rb->quant.plev + 1 > DEF_PLEV_TAB_SIZE) ?
		    rb->quant.plev + 1 : DEF_PLEV_TAB_SIZE));
	  skrivprefikspp (&rb->quant);

	  fprintf (ccode, "};\n__ptyp __p%d%s={'%c',%d,%d,sizeof(__bs%d),%d,",
		   rb->blno, timestamp,
		   rb->quant.kind,
		   rb->quant.plev,
		   rb->blev, rb->blno,
		   rb->ent);

	  if (separat_comp && (rb->quant.kind == KCLASS 
			       || rb->quant.kind == KPROC ||
			       rb->quant.kind == KPRBLK))
	    fprintf (ccode, "__m_%s", timestamp);
	  else
	    fprintf (ccode, "0");

	  fprintf (ccode, ",%d,%d,%d,%d",
		   rb->fornest, 
		   rb->connest,
		   naref, 
		   rb->navirt);

	  if (naref)
	    fprintf (ccode, ",__rl%d%s", rb->blno, timestamp);
	  else
	    fprintf (ccode, ",0");

	  if ((rb->quant.kind == KCLASS || rb->quant.kind == KPRBLK) && rb->navirt)
	    fprintf (ccode, ",__vl%d%s", rb->blno, timestamp);
	  else
	    fprintf (ccode, ",0");

	  fprintf (ccode, ",__pl%d%s", rb->blno, timestamp);

	  if ((rb->quant.kind == KCLASS || rb->quant.kind == KPRBLK) 
	      && rb->navirtlab)
	    fprintf (ccode, ",__labvl%d%s};\n", rb->blno, timestamp);
	  else
	    fprintf (ccode, ",__NULL};\n");

	}
      rb->structure_written = TRUE;	/* merker av at det er lagt ut type for denne 
				 * blokken */

  /* Sjekker om det må skrives ut structer for virtuelle- og formelle
   * prosedyre spesifikasjoner */
      if (rb->quant.kind == KCLASS || rb->quant.kind == KPROC)
	specifier_structure (rb);
      break;
    }
  for (rd= rb->parloc; rd!= NULL; rd= rd->next)
    {
      switch (rd->kind)
	{
	case KPROC:
	case KCLASS:
	case KBLOKK:
	case KPRBLK:
	case KFOR:
	case KINSP:
	case KCON:
	  blockstructure (rd->descr);
	  break;
	}
    }
}

static void specifier_proc_structure (decl_t *rd)
{
  decl_t *rdi;
  if (rd->kind == KPROC)
    {
      if (rd->descr->parloc != NULL)
	{
	  fprintf 
	    (ccode, "typedef struct /* %s SPEC*/\n    {\n", rd->ident);
	  fprintf (ccode, "        __dh h;\n");
	  
	  /* Skriver alle parameterne */
	  for (rdi = rd->descr->parloc; rdi != NULL; rdi = rdi->next)
	    declstructure (rdi, FALSE);
	  fprintf (ccode, "    } __bs%d;\n", rd->descr->blno);
	  
	  /* Flere nivåer ? */
	  specifier_structure (rd->descr);
	}
      /* merker av at det er lagt ut type for denne blokken */
      rd->descr->structure_written = TRUE;
    }
}

static void specifier_structure (block_t *rb)
{				/* Kaller på param_structure som skriver ut
				 * structer for evt. parameterspesifikasjoner 
				 * til virtuelle  og formelle prosedyre-
				 * spesifikasjoner. Altså kun for de som
				 * inneholder parametere. */

  decl_t *rd,
   *rdi;

  /* Ser forst etter formell prosedyre spesifikasjoner */
  for (rd = rb->parloc; rd != NULL && (rd->categ == CDEFLT 
				       || rd->categ == CNAME &&
				       rd->categ == CVAR 
				       && rd->categ == CVALUE); rd = rd->next)
    specifier_proc_structure (rd);

  /* Går så gjennom den virtuele listen, og skriver ut for evt.  virtuelle
   * prosedyre spesifikasjoner */

  if (rb->quant.kind == KCLASS && rb->virt != NULL)
    {
      int i;
      /* Skal evt. bare skrive ut structene for de prosedyrene som ikke */
      /* er eller blir skrevet ut under behandlingen av prefiksklassen  */
      if (rb->quant.plev > 0)
	i = rb->quant.prefqual->descr->navirt +
	  rb->quant.prefqual->descr->navirtlab;
      else
	i = 0;
      for (rd = rb->virt; i-- > 0; rd = rd->next);
      /* Har nå funnet første 'nye' virtuelle spesifikasjon. */

      for (; rd != NULL; rd = rd->next)
	specifier_proc_structure (rd);
    }
}

/******************************************************************************
                                                                STRUCTURE    */

void structure (void)
{
  block_t *block;
  if (separat_comp)
    {
      fprintf (ccode, "void __m_%s();\n", timestamp);
    }
  genmap ();
  ssblock->timestamp= tag("FILE");
#if 0
  for (block = ssblock; block != NULL; block = block->next_block)
    {
      switch (block->quant.kind)
	{
	case KPROC:
	case KCLASS:
	case KBLOKK:
	case KPRBLK:
	  blockstructure (block);
	  break;
	default:
	  /* IKKE NOE UTLEGG */ ;
	}
    }
#else
    blockstructure (ssblock);
#endif
}

/******************************************************************************
								UPDATEGLNULL */

static void do_for_each_stat_pointer (block_t *block)
{
  decl_t *rd;
  switch (block->quant.kind)
    {
    case KPROC:
    case KCLASS:
    case KBLOKK:
    case KPRBLK:
      if (block->stat)
	fprintf (ccode, "if(((__dhp)&__blokk%d%s)->gl!=__NULL|force)"
		 "__do_for_each_pointer(&__blokk%d%s,doit,doit_notest);\n"
		 ,block->blno, timestamp, block->blno, 
		 block->timestamp?block->timestamp:timestamp);
    }
  for (rd= block->parloc; rd!= NULL; rd= rd->next)
    {
      switch (rd->kind)
	{
	case KPROC:
	case KCLASS:
	case KBLOKK:
	case KPRBLK:
	case KFOR:
	case KINSP:
	case KCON:
	  do_for_each_stat_pointer (rd->descr);
	  break;
	}
    }
}

/******************************************************************************
								UPDATEGLNULL */

static void update_gl_null (block_t *block)
{
  decl_t *rd;
  switch (block->quant.kind)
    {
    case KPROC:
    case KCLASS:
    case KBLOKK:
    case KPRBLK:
      if (block->stat)
	fprintf (ccode, "((__dhp)&__blokk%d%s)->gl=(__dhp)0;\n",block->blno, 
		 block->timestamp?block->timestamp:timestamp);
    }
  for (rd= block->parloc; rd!= NULL; rd= rd->next)
    {
      switch (rd->kind)
	{
	case KPROC:
	case KCLASS:
	case KBLOKK:
	case KPRBLK:
	case KFOR:
	case KINSP:
	case KCON:
	  update_gl_null (rd->descr);
	  break;
	}
    }
}

/******************************************************************************
								UPDATEGLOBJ */

static void update_gl_obj (block_t *block)
{
  decl_t *rd;
  switch (block->quant.kind)
    {
    case KPROC:
    case KCLASS:
    case KBLOKK:
    case KPRBLK:
      if (block->stat)
	fprintf 
	  (ccode, 
	   "if(((__dhp)&__blokk%d%s)->gl)((__dhp)&__blokk%d%s)->gl=(__dhp)&__blokk%d%s;\n"
	   ,block->blno, block->timestamp?block->timestamp:timestamp
	   ,block->blno, block->timestamp?block->timestamp:timestamp, 
	   block->blno, block->timestamp?block->timestamp:timestamp);
    }

  for (rd= block->parloc; rd!= NULL; rd= rd->next)
    {
      switch (rd->kind)
	{
	case KPROC:
	case KCLASS:
	case KBLOKK:
	case KPRBLK:
	case KFOR:
	case KINSP:
	case KCON:
	  update_gl_obj (rd->descr);
	  break;
	}
    }
}

/******************************************************************************
                                                            STAT_POINTERS    */

void stat_pointers (void)
{
  block_t *block;
  struct stamp *st;

  if (!separat_comp)
    { /* TBD __init(){__init_FILE();__init_SIMENVIR(); should be removed */
      fprintf (ccode, "\n__init(){__init_FILE();__init_SIMENVIR();}\n");
      fprintf 
	(ccode, 
	 "__do_for_each_stat_pointer(doit,doit_notest,force)void(*doit)(),(*doit_notest)();int force;{\n");

      do_for_each_stat_pointer (sblock);

      fprintf (ccode, "}\n__update_gl_to_obj(){\n");
      update_gl_obj (sblock);


      fprintf (ccode, "}\n__update_gl_to_null(){\n");
      update_gl_null (sblock);

      fprintf (ccode, "}\n");
    }
}

