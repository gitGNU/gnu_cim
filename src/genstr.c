/* $Id: cgenstr.c,v 1.11 1995/12/21 15:13:30 cim Exp $ */

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

/* Legger ut C-kode for hvert blokk objekt. */

#include "const.h"
#include "dekl.h"
#include "cimcomp.h"
#include "feil.h"
#include "extspec.h"

static short plevnull;		/* Hvis en blokks prefiksniv} er 0 s} er 
				 * plevnull=TRUE.Brukes for } initsialisere
				 * offset adressene til pekerne.M} vite om 
				 * structen til denne blokken inneholder   
				 * deklarasjonen struct dhp h.Ellers s} m} 
				 * .s f}lges plev ganger for } komme til h.pp 
				 */


static int naref;

/******************************************************************************
                                                        DECLSTRUCTURE        */

static declstructure (rd, in_external_modul)
     struct DECL *rd;
     char in_external_modul;
{
  char write = 0;
  if (rd->kind == KSIMPLE)
    {
      if (rd->categ == CVAR)
	{
	  if (rd->type == TREF)
	    (void) fprintf (hcode, "        __refvarpar ");
	  else if (rd->type == TINTG || rd->type == TREAL)
	    (void) fprintf (hcode, "        __aritvarpar ");
	  else if (rd->type == TLABEL)
	    (void) fprintf (hcode, "        __labelswitchpar ");
	  else
	    (void) fprintf (hcode, "        __varpar ");

	  if (!in_external_modul)
	    {
	      (void) fprintf (ccode,
		       "__rl%d%s[%d]=(short)((char *)&((__bs%d *)__p)->%s.",
			 rd->encl->blno, timestamp, naref++, rd->encl->blno,
			      rd->ident);

	      if (rd->type == TLABEL)
		(void) fprintf (ccode, "ob-(char *)__p);");
	      else
		(void) fprintf (ccode, "bp-(char *)__p);");
	    }

	  write = TRUE;
	}
      else if (rd->categ == CNAME)
	{
	  if (rd->type == TREF)
	    (void) fprintf (hcode, "        __refnamepar ");
	  else if (rd->type == TINTG || rd->type == TREAL)
	    (void) fprintf (hcode, "        __aritnamepar ");
	  else if (rd->type == TTEXT)
	    (void) fprintf (hcode, "        __textnamepar ");
	  else if (rd->type == TLABEL)
	    (void) fprintf (hcode, "        __labelnamepar ");
	  else
	    (void) fprintf (hcode, "        __charboolnamepar ");

	  if (!in_external_modul)
	    {
	      (void) fprintf (ccode,
		       "__rl%d%s[%d]=(short)((char *)&((__bs%d *)__p)->%s.",
	      rd->encl->blno, timestamp, naref++, rd->encl->blno, rd->ident);

	      if (rd->type == TLABEL)
		(void) fprintf (ccode, "ob-(char *)__p);");
	      else
		(void) fprintf (ccode, "h.bp-(char *)__p);");

	      (void) fprintf (ccode,
		       "__rl%d%s[%d]=(short)((char *)&((__bs%d *)__p)->%s.",
			 rd->encl->blno, timestamp, naref++, rd->encl->blno,
			      rd->ident);

	      if (rd->type == TLABEL)
		(void) fprintf (ccode, "sl-(char *)__p);");
	      else
		(void) fprintf (ccode, "h.sl-(char *)__p);");

	    }
	  write = TRUE;
	}
      else
	{
	  switch (rd->type)
	    {
	    case TINTG:
	      (void) fprintf (hcode, "        long ");
	      write = TRUE;
	      break;
	    case TREAL:
	      (void) fprintf (hcode, "        double ");
	      write = TRUE;
	      break;
	    case TBOOL:
	      (void) fprintf (hcode, "        char ");
	      write = TRUE;
	      break;
	    case TCHAR:
	      (void) fprintf (hcode, "        char ");
	      write = TRUE;
	      break;
	    case TLABEL:
	      if (rd->categ != CLOCAL)
		{
		  (void) fprintf (hcode, "        __labelswitchpar ");
		  if (!in_external_modul)
		    (void) fprintf 
		      (ccode,
		       "__rl%d%s[%d]=(short)((char *)&((__bs%d *)__p)->%s.ob-(char *)__p);",
		       rd->encl->blno, timestamp, naref++, rd->encl->blno,
		       rd->ident);
		  write = TRUE;
		}
	      break;
	    case TTEXT:
	      (void) fprintf (hcode, "        __txt ");
	      if (!in_external_modul)
		(void) fprintf 
		  (ccode,
		   "__rl%d%s[%d]=(short)((char *)&((__bs%d *)__p)->%s.obj-(char *)__p);",
		   rd->encl->blno, timestamp, naref++, rd->encl->blno,
		   rd->ident);
	      write = TRUE;
	      break;
	    case TREF:
	      (void) fprintf (hcode, "        __dhp ");
	      if (!in_external_modul)
		(void) fprintf 
		  (ccode, 
		   "__rl%d%s[%d]=(short)((char *)&((__bs%d *)__p)->%s-(char *)__p);",
		   rd->encl->blno, timestamp, naref++, rd->encl->blno,
		   rd->ident);

	      write = TRUE;
	      break;
	    }
	  /* END-SWITCH */
	}
    }
  else if (rd->kind == KARRAY)
    {
      if (rd->type == TLABEL && rd->categ != CLOCAL)
	{
	  (void) fprintf (hcode, "        __labelswitchpar ");
	  if (!in_external_modul)
	    (void) fprintf 
	      (ccode,
	       "__rl%d%s[%d]=(short)((char *)&((__bs%d *)__p)->%s.ob-(char *)__p);",
	       rd->encl->blno, timestamp, naref++, rd->encl->blno,
	       rd->ident);
	}
      else if (rd->categ == CNAME)
	{
	  (void) fprintf (hcode, "        __arraynamepar ");
	  if (!in_external_modul)
	    {
	      (void) fprintf 
		(ccode, 
		 "__rl%d%s[%d]=(short)((char *)&((__bs%d *)__p)->%s.sl-(char *)__p);",
		 rd->encl->blno, timestamp, naref++, rd->encl->blno,
		 rd->ident);
	      (void) fprintf 
		(ccode, 
		 "__rl%d%s[%d]=(short)((char *)&((__bs%d *)__p)->%s.ap-(char *)__p);",
		 rd->encl->blno, timestamp, naref++, rd->encl->blno,
		 rd->ident);
	    }
	}
      else
	{
	  (void) fprintf (hcode, "        __arrp ");

	  if (!in_external_modul)
	    (void) fprintf 
	      (ccode, 
	       "__rl%d%s[%d]=(short)((char *)&((__bs%d *)__p)->%s-(char *)__p);",
	       rd->encl->blno, timestamp, naref++, rd->encl->blno,
	       rd->ident);
	}
      write = TRUE;
    }
  else if (rd->kind == KPROC)
    {
      if (rd->categ == CDEFLT || rd->categ == CVAR)
	{
	  if (rd->type == TINTG || rd->type == TREAL)
	    (void) fprintf (hcode, "        __aritprocpar ");
	  else if (rd->type == TREF)
	    (void) fprintf (hcode, "       __refprocpar ");
	  else
	    (void) fprintf (hcode, "       __forprocpar ");

	  if (!in_external_modul)
	    (void) fprintf 
	      (ccode, 
	       "__rl%d%s[%d]=(short)((char *)&((__bs%d *)__p)->%s.psl-(char *)__p);",
	       rd->encl->blno, timestamp, naref++, rd->encl->blno,
	       rd->ident);

	  write = TRUE;
	}
      else if (rd->categ == CNAME)
	{
	  if (rd->type == TINTG || rd->type == TREAL)
	    (void) fprintf (hcode, "        __aritprocnamepar ");
	  else if (rd->type == TREF)
	    (void) fprintf (hcode, "       __refprocnamepar ");
	  else
	    (void) fprintf (hcode, "       __simpleprocnamepar ");
	  if (!in_external_modul)
	    {
	      (void) fprintf 
		(ccode, 
		 "__rl%d%s[%d]=(short)((char *)&((__bs%d *)__p)->%s.h.psl-(char *)__p);",
		 rd->encl->blno, timestamp, naref++, rd->encl->blno,
		 rd->ident);
	      (void) fprintf 
		(ccode, 
		 "__rl%d%s[%d]=(short)((char *)&((__bs%d *)__p)->%s.h.sl-(char *)__p);",
		 rd->encl->blno, timestamp, naref++, rd->encl->blno,
		 rd->ident);
	    }
	  write = TRUE;
	}
    }

  if (write)
    (void) fprintf (hcode, "%s;\n", rd->ident);
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

static skrivprefikspp (rd)
     struct DECL *rd;
{
  if (rd != NULL)
    {
      if (rd->idplev.plev > 0)
	skrivprefikspp (rd->prefqual);
      (void) fprintf (hcode, "&__p%d", 
		      (rd->descr->externid == 0 ? rd->descr->blno :
		       rd->descr->ptypno));
      if (rd->descr->externid != 0)
	(void) fprintf (hcode, "%s", extract_timestamp (rd->descr->externid));
      else
	(void) fprintf (hcode, "%s", timestamp);
      (void) fprintf (hcode, ",");

    }
}

/******************************************************************************
                                                        BLOCKSTRUCTURE       */

static blockstructure (rb)
     struct BLOCK *rb;
{
  int i;
  struct DECL *rd;
  char forste_virtlab;

  if (rb->blno < 11)
    return;

  if (rb->blev == SETT)
    return;			/* Blokken er allerede behandlet */
  if ((rb->quant.kind == KCLASS || rb->quant.kind == KPROC) 
      && rb->externid != 0 &&
      rb->quant.encl->externid != rb->externid)
    {
      /* Definerer den eksterne modulen som extern p} .h filen */
      (void) fprintf (hcode, "extern void __m_%s();\n",
		      extract_timestamp (rb->externid));

    }


  if ((rb->quant.kind == KCLASS && rb->quant.idplev.plev > 0) 
      || (rb->quant.kind == KPRBLK))
    {				/* G}r gjennom prefikskjeden */
      char ttt[2];
      ttt[1] = '\0';
      ttt[0] = rb->quant.kind;
      blockstructure (rb->quant.prefqual->descr);
      plevnull = FALSE;
      naref += rb->connest;
      (void) fprintf (hcode, "typedef struct /* %s */\n    {\n"
		      ,rb->quant.ident == NULL ? ttt : rb->quant.ident);
      (void) fprintf (hcode, "        __bs%d s;\n",
		      rb->quant.prefqual->descr->blno);

    }
  else if (rb->quant.kind == KPROC && rb->quant.categ == CCPROC)
    {				/* Extern C-prosedyre */
      (void) fprintf (hcode, "extern ");
      switch (rb->quant.type)
	{
	case TTEXT:
	  (void) fprintf (hcode, "char *");
	  break;
	case TINTG:
	  (void) fprintf (hcode, "long ");
	  break;
	case TREAL:
	  (void) fprintf (hcode, "double ");
	  break;
	case TBOOL:
	case TCHAR:
	  (void) fprintf (hcode, "char ");
	  break;
	}
      (void) fprintf (hcode, "%s();\n", ((exinfop) rb->hiprot)->rtname);
      return;
    }
  else
    {
      char ttt[2];
      ttt[1] = '\0';
      ttt[0] = rb->quant.kind;
      plevnull = TRUE;
      (void) fprintf (hcode, "typedef struct /* %s */\n    {\n"
		      ,rb->quant.ident == NULL ? ttt : rb->quant.ident);
      (void) fprintf (hcode, "        __dh h;\n");

    };
  naref = rb->connest;		/* Inspect variablene ligger forst i ref
				 * tabellen */

  /* NB !!!. Deklarasjonene m} skrives ut f}r evt. hjelpe variable 
   * (for,inspect) og f|r returverdivariabelen. Slipper da } skrive
   * ut disse i structene for virtuelle og formelle prosedyre 
   * spesifikasjoner. Gjelder prosedyrer. */
  for (rd = rb->parloc; rd != NULL; rd = rd->next)
    declstructure (rd, rb->externid != 0);

  if (rb->quant.kind == KPROC && rb->quant.type != TNOTY)
    {
      if (rb->quant.type == TTEXT)
	{
	  if (rb->externid == 0)
	    (void) fprintf (ccode,
			    "__rl%d%s[%d]=(short)((char *)&((__bs%d *)__p)->et.obj-(char *)__p);",
			    rb->blno, timestamp, naref++, rb->blno);
	  (void) fprintf (hcode, "        __txt et;\n");
	}
      else if (rb->quant.type == TREF)
	{
	  if (rb->externid == 0)
	    (void) fprintf 
	      (ccode,
	       "__rl%d%s[%d]=(short)((char *)&((__bs%d *)__p)->er-(char *)__p);",
	       rb->blno, timestamp, naref++, rb->blno);
	  (void) fprintf (hcode, "        __dhp er;\n");
	}
      else if (rb->quant.type == TINTG)
	(void) fprintf (hcode, "        long ev;\n");
      else if (rb->quant.type == TREAL)
	(void) fprintf (hcode, "        double ef;\n");
      else
	(void) fprintf (hcode, "        char ec;\n");
    }

  for (i = 1; i <= rb->fornest; i++)
    (void) fprintf (hcode, "        short f%d;\n", i);
  for (i = 1; i <= rb->connest; i++)
    {
      (void) fprintf (hcode, "        __dhp c%d;\n", i);
      if (rb->externid == 0)
	(void) fprintf 
	  (ccode, 
	   "__rl%d%s[%d]=(short)((char *)&((__bs%d *)__p)->c%d-(char *)__p);",
	   rb->blno, timestamp, i - 1, rb->blno, i);


    };

  (void) fprintf (hcode, "    } __bs%d;\n", rb->blno, rb->blno);

  if (rb->stat)
    {
      (void) fprintf 
	(hcode, "__bs%d __blokk%d%s;\n", rb->blno, rb->blno, timestamp);
    }

  if (rb->externid != 0)
    {
      (void) fprintf (hcode, "extern __ptyp __p%d%s", rb->ptypno,
		      extract_timestamp (rb->externid));
      (void) fprintf (hcode, ";\n");
    }
  else
    {
      if (naref)
	(void) fprintf (hcode, "short __rl%d%s[%d];\n", 
			rb->blno, timestamp, naref);


      if ((rb->quant.kind == KCLASS || rb->quant.kind == KPRBLK) &&
	  (rb->navirt || rb->navirtlab))
	{
	  if (rb->navirt)
	    (void) fprintf (hcode, "__pty   __vl%d%s[%d];\n", 
			    rb->blno, timestamp, rb->navirt);
	  if (rb->navirtlab)
	    {
	      (void) fprintf (hcode, "__progadr   __labvl%d%s[%d]={", rb->blno,
			      timestamp, rb->navirtlab);
	      forste_virtlab = TRUE;
	    }

	  for (rd = rb->virt; rd != NULL; rd = rd->next)
	    {
	      if (rd->kind == KPROC)
		{
		  (void) fprintf (ccode, "__vl%d%s[%d]= ", rb->blno, timestamp,
				  rd->virtno - 1);
		  if (rd->match != NULL)
		    {
		      gen_adr_prot (rd->match);
		      (void) fprintf (ccode, ";");
		    }
		  else
		    (void) fprintf (ccode, "__NULL;");
		}
	      else
		{
		  if (!forste_virtlab)
		    (void) fprintf (hcode, ",");
		  else
		    forste_virtlab = FALSE;

		  if (rd->match != NULL)
		    {
		      if (rd->match->encl->externid != 0)
			(void) fprintf (hcode, "%d,__m_%s",
					rd->match->idplev.plev,
			     extract_timestamp (rd->match->encl->externid));

		      else if (separat_comp)
			(void) fprintf (hcode, "%d,__m_%s",
					rd->match->idplev.plev, timestamp);
		      else
			(void) fprintf (hcode, "%d,__NULL",
					rd->match->idplev.plev);
		    }
		  else
		    (void) fprintf (hcode, "0,__NULL");

		}
	    }
	  if (rb->navirtlab)
	    (void) fprintf (hcode, "};\n");

	}

      (void) fprintf (hcode, "__pty   __pl%d%s[%d]={",
		      rb->blno, timestamp,
                      (rb->quant.prefqual==NULL)?1:
		      ((rb->quant.idplev.plev + 1 > DEF_PLEV_TAB_SIZE) ?
		      rb->quant.idplev.plev + 1 : DEF_PLEV_TAB_SIZE));
      if (rb->quant.idplev.plev > 0)
	skrivprefikspp (rb->quant.prefqual);

      (void) fprintf (hcode, "0};\n");

      (void) fprintf (ccode, "__pl%d%s[%d] = ", rb->blno, timestamp,
		      rb->quant.idplev.plev);
      gen_adr_prot (&rb->quant);
      (void) fprintf (ccode, ";");

      (void) fprintf (hcode, "__ptyp __p%d%s", rb->blno, timestamp);

      (void) fprintf (hcode, "={'%c',%d,%d,sizeof(__bs%d)", rb->quant.kind
		      ,rb->quant.idplev.plev,
		      rb->blev, rb->blno);
      (void) fprintf (hcode, ",%d,0,%d,%d", rb->ent
		      ,rb->fornest, rb->connest);
      (void) fprintf (hcode, ",%d,%d", naref, rb->navirt);
      if (naref)
	(void) fprintf (hcode, ",__rl%d%s", rb->blno, timestamp);
      else
	(void) fprintf (hcode, ",0");
      if ((rb->quant.kind == KCLASS || rb->quant.kind == KPRBLK) && rb->navirt)
	(void) fprintf (hcode, ",__vl%d%s", rb->blno, timestamp);
      else
	(void) fprintf (hcode, ",0");

      (void) fprintf (hcode, ",__pl%d%s", rb->blno, timestamp, timestamp);
      if ((rb->quant.kind == KCLASS || rb->quant.kind == KPRBLK) 
	  && rb->navirtlab)
	(void) fprintf (hcode, ",__labvl%d%s};\n", rb->blno, timestamp);
      else
	(void) fprintf (hcode, ",__NULL};\n");

      if (separat_comp && (rb->quant.kind == KCLASS 
			   || rb->quant.kind == KPROC ||
			   rb->quant.kind == KPRBLK))
#if SPLIT_MODUL
	(void) fprintf 
	  (ccode, 
	   "__p%d%s.adr.ment=__l%d%s;", rb->blno, timestamp, rb->ent, timestamp);
#else
	(void) fprintf 
	  (ccode, 
	   "__p%d%s.adr.ment=__m_%s;", rb->blno, timestamp, timestamp);
#endif
    }
  rb->blev = SETT;		/* merker av at det er lagt ut type for denne 
				 * blokken */

  /* Sjekker om det m} skrives ut structer for virtuelle- og formelle
   * prosedyre spesifikasjoner */
  if (rb->quant.kind == KCLASS || rb->quant.kind == KPROC)
    specifier_structure (rb);
}

static param_structure (rd)
     struct DECL *rd;
{
  if (rd->kind == KSIMPLE)
    {
      if (rd->categ == CVAR)
	{
	  if (rd->type == TREF)
	    (void) fprintf (hcode, "        __refvarpar ");
	  else if (rd->type == TINTG || rd->type == TREAL)
	    (void) fprintf (hcode, "        __aritvarpar ");
	  else
	    (void) fprintf (hcode, "        __varpar ");

	}
      else if (rd->categ == CNAME)
	{
	  if (rd->type == TREF)
	    (void) fprintf (hcode, "        __refnamepar ");
	  else if (rd->type == TINTG || rd->type == TREAL)
	    (void) fprintf (hcode, "        __aritnamepar ");
	  else if (rd->type == TTEXT)
	    (void) fprintf (hcode, "        __textnamepar ");
	  else
	    (void) fprintf (hcode, "        __charboolnamepar ");

	}
      else
	{
	  switch (rd->type)
	    {
	    case TINTG:
	      (void) fprintf (hcode, "        long ");
	      break;
	    case TREAL:
	      (void) fprintf (hcode, "        double ");
	      break;
	    case TBOOL:
	      (void) fprintf (hcode, "        char ");
	      break;
	    case TCHAR:
	      (void) fprintf (hcode, "        char ");
	      break;
	    case TLABEL:
	      (void) fprintf (hcode, "        __lab ");
	      break;
	    case TTEXT:
	      (void) fprintf (hcode, "        __txt ");
	      break;
	    case TREF:
	      (void) fprintf (hcode, "        __dhp ");
	      break;
	    }
	}
    }
  else if (rd->kind == KARRAY)
    (void) fprintf (hcode, "        __arrp ");
  else if (rd->kind == KPROC && rd->categ == CDEFLT)
    {
      if (rd->type == TREF)
	(void) fprintf (hcode, "       __refprocpar ");
      else
	(void) fprintf (hcode, "       __forprocpar ");
    }
  (void) fprintf (hcode, "%s;\n", rd->ident);

}

specifier_structure (rb)
     struct BLOCK *rb;
{				/* Kaller p} param_structure som skriver ut
				 * structer for evt. parameterspesifikasjoner 
				 * til virtuelle  og formelle prosedyre-
				 * spesifikasjoner. Alts} kun for de som
				 * inneholder parametere. */

  struct DECL *rd,
   *rdi;
  int i,
    j;

  /* Ser forst etter formell prosedyre spesifikasjoner */
  for (rd = rb->parloc; rd != NULL && (rd->categ == CDEFLT 
				       || rd->categ == CNAME &&
				       rd->categ == CVAR 
				       && rd->categ == CVALUE); rd = rd->next)
    {
      if (rd->kind == KPROC)
	{
	  if (rd->descr->parloc != NULL)
	    {
	      (void) fprintf 
		(hcode, "typedef struct /* %s PARAM. SPEC*/\n    {\n"
			      ,rd->ident);
	      (void) fprintf (hcode, "        __dh h;\n");

	      /* Skriver alle parameterne */
	      for (rdi = rd->descr->parloc; rdi != NULL; rdi = rdi->next)
		param_structure (rdi);
	      (void) fprintf 
		(hcode, "    } __bs%d;\n", rd->descr->blno, rd->descr->blno);

	      /* Flere niv}er ? */
	      specifier_structure (rd->descr);
	    }
	  /* merker av at det er lagt ut type for denne blokken */
	  rd->descr->blev = SETT;
	}
    }
  /* G}r s} gjennom den virtuele listen, og skriver ut for evt.  virtuelle
   * prosedyre spesifikasjoner */

  if (rb->quant.kind == KCLASS && rb->virt != NULL)
    {
      /* Skal evt. bare skrive ut structene for de prosedyrene som ikke */
      /* er eller blir skrevet ut under behandlingen av prefiksklassen  */
      if (rb->quant.idplev.plev > 0)
	i = rb->quant.prefqual->descr->navirt +
	  rb->quant.prefqual->descr->navirtlab;
      else
	i = 0;
      j = 0;
      for (rd = rb->virt; j++ < i; rd = rd->next);
      /* Har n} funnet f|rste 'nye' virtuelle spesifikasjon. */

      for (; rd != NULL; rd = rd->next)
	if (rd->kind == KPROC)
	  {
	    if (rd->descr->parloc != NULL)
	      {
		(void) fprintf 
		  (hcode, "typedef struct /* %s VIRT SPEC.*/\n    {\n"
				,rd->ident);

		(void) fprintf (hcode, "        __dh h;\n");

		for (rdi = rd->descr->parloc; rdi != NULL; rdi = rdi->next)
		  param_structure (rdi);
		(void) fprintf 
		  (hcode, "    } __bs%d;\n", rd->descr->blno, rd->descr->blno);

		/* Flere niv}er ? */
		specifier_structure (rd->descr);

	      }
	    /* merker av at det er lagt ut type for denne blokken */
	    rd->descr->blev = SETT;
	  }
    }
}

/******************************************************************************
                                                           INIT_STRUCTURE    */

init_structure ()
{
  (void) fprintf (hcode, "/*Cim_hcode*/\n");
}

/******************************************************************************
                                                                STRUCTURE    */

structure ()
{
  struct BLOCK *block;
  struct stamp *st;
  genmap ();

  /* Definerer forst hjelpevariablene som brukes for } beregne */
  /* offset adresser.                                          */
  if (separat_comp)
    {
      (void) fprintf (ccode, "void __init_%s(){", timestamp);
      (void) fprintf (hcode, "void __m_%s();\n", timestamp);
    }
  else
    {
      (void) fprintf (ccode, "__init(){");
      for (st = first_stamp; st != NULL; st = st->next)
	(void) fprintf (ccode, "__init_%s();", st->timestamp);
      (void) fprintf (hcode, "__dh __aa;\n");
      (void) fprintf (hcode, "__dhp __p= &__aa;\n");
    }

  for (block = sblock; block != NULL; block = block->next_block)
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
  (void) fprintf (ccode, "}");

  if (!separat_comp)
    {
      fprintf 
	(ccode, 
	 "__do_for_each_stat_pointer(doit,doit_notest,force)void(*doit)(),(*doit_notest)();int force;{");
      for (block = sblock; block != NULL; block = block->next_block)
	switch (block->quant.kind)
	  {
	  case KPROC:
	  case KCLASS:
	  case KBLOKK:
	  case KPRBLK:
	    if (block->stat)
	      fprintf 
		(ccode, 
		 "if(((__dhp)&__blokk%d%s)->gl!=__NULL|force)__do_for_each_pointer(&__blokk%d%s,doit,doit_notest);"
		 ,block->blno, timestamp, block->blno, timestamp
		 ,block->blno, timestamp, block->blno, timestamp);
	  }
      fprintf (ccode, "}");


      fprintf (ccode, "__update_gl_to_obj(){");
      for (block = sblock; block != NULL; block = block->next_block)
	switch (block->quant.kind)
	  {
	  case KPROC:
	  case KCLASS:
	  case KBLOKK:
	  case KPRBLK:
	    if (block->stat)
	      fprintf 
		(ccode, 
		 "if(((__dhp)&__blokk%d%s)->gl)((__dhp)&__blokk%d%s)->gl=(__dhp)&__blokk%d%s;"
		 ,block->blno, timestamp
		 ,block->blno, timestamp, block->blno, timestamp);
	  }
      fprintf (ccode, "}");


      fprintf (ccode, "__update_gl_to_null(){");
      for (block = sblock; block != NULL; block = block->next_block)
	switch (block->quant.kind)
	  {
	  case KPROC:
	  case KCLASS:
	  case KBLOKK:
	  case KPRBLK:
	    if (block->stat)
	      fprintf (ccode, "((__dhp)&__blokk%d%s)->gl=(__dhp)0;"
		       ,block->blno, timestamp);
	  }
      fprintf (ccode, "}");
    }
}
