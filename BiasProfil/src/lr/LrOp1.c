/************************************************************************
*  $Id: LrOp1.c                 Langzahlarithmetik, Operationen, Level 1
*
*  Copyright (C) 1989, 1990, 1991, 1994 Dirk Husung
*
*  This file is part of PROFIL/BIAS.
* 
*  PROFIL/BIAS is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License
*  as published by the Free Software Foundation; either version 2
*  of the License, or (at your option) any later version.
* 
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
* 
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
*  USA.
* 
*  Version 1.0  10.10.89        Dirk Husung
*  Version 1.1  15.08.90        Dirk Husung
*	korrigiert: _Sub()
*  Version 1.2  06.06.91        Dirk Husung
*       geaendert: LrDiv(): erhoehter Bedarf an Arbeitsspeicher
*  Version 1.3  19.07.91        Dirk Husung
*       korrigiert: LrDiv(): Inline-Version, C-Version
*  Version 1.4	01.08.91	Dirk Husung
*	geaendert: _Add(), _Sub(): beschleunigt fuer Operanden unterschied-
*	licher Genauigkeit.
*  Korrektur in _Sub(): negatives Argument in memset() vermieden.
*  Korrektur bei Return100: LrSetDigits (c, 1); LrExponent (c) = 1; fehlten
*  Korrektur bei Return0FF: LrExponent (c) = ... fehlte
*  Korrektur bei Return1,Return100,Return0FF: LrExponent = ... + Exponent (!)
*  Korrektur bei Return100: LrSetDigits (c, 1) entfernt
*  Letzte Aenderung:		Thu 10-Nov-1994 12:26:30
*************************************************************************/

#include "Portab.h"

#if defined (__ANSI__)
#include <string.h>
#else
#include <memory.h>
#endif

#include "Lr.h"

#define __2DIV1__

/************************************************************************
*  Copyright-Vermerk
*************************************************************************/

#define COPYRIGHTSUM    5335
static	PCHAR Copyright =
    "Langzahlarithmetik - Copyright (c) 1991, TU Hamburg (D. Husung).";

/************************************************************************
*
*************************************************************************/

#if defined (__INLINE__)
#if defined (__BORLANDC__)
#define Compare(a,b,n,r,LT,GT)   \
      asm {                      \
        push    ds              ;\
        lds     si, dword ptr a ;\
        les     di, dword ptr b ;\
        mov     cx, word ptr n  ;\
        repz    cmpsw           ;\
        pop     ds              ;\
        ja      GT##0           ;\
        jz      GT##1           ;\
        mov     ax, word ptr n  ;\
        inc     cx              ;\
        sub     ax, cx          ;\
        mov     word ptr r, ax  ;\
        jmp     LT              ;\
      }                          \
      GT##0:                     \
      _asm {                     \
        mov     ax, word ptr n  ;\
        inc     cx              ;\
        sub     ax, cx          ;\
        mov     word ptr r, ax  ;\
        jmp     GT              ;\
      }                          \
      GT##1:
#else
#define Compare(a,b,n,r,LT,GT)          \
      _asm {                            \
        _asm    push    ds              \
        _asm    lds     si, a           \
        _asm    les     di, b           \
        _asm    mov     cx, n           \
        _asm    repz    cmpsw           \
        _asm    pop     ds              \
        _asm    ja      GT##0           \
        _asm    jz      GT##1           \
        _asm    mov     r, n            \
        _asm    inc     cx              \
        _asm    sub     r, cx           \
        _asm    jmp     LT              \
        _asm GT##0:                     \
        _asm    mov     r, n            \
        _asm    inc     cx              \
        _asm    sub     r, cx           \
        _asm    jmp     GT              \
        _asm GT##1:                     \
      }
#endif /* !__BORLANDC__ */
#else
#define Compare(a,b,n,r,LT,GT)          \
        for (r = 0; r < n; r++) {       \
          if (a[r] > b[r]) goto GT;     \
          if (a[r] < b[r]) goto LT;     \
        }
#endif

/************************************************************************
*  Prototypen lokaler Funktionen
*************************************************************************/

static INT NEAR _Add		(LREAL a, LREAL b, LREAL c, INT Level);
static INT NEAR _Sub		(LREAL a, LREAL b, LREAL c, INT Level);

/************************************************************************
*  Implementierung globaler Funktionen
*************************************************************************/

INT FAR CALL _LrAbsCmp (LREAL a, LREAL b)
 /***********************************************************************
  *  Funktion : Vergleich der Absolutbetraege zweier Langzahlen "a", "b"
  *		mit gleichen Exponenten.
  *  Annahmen : Weder "a" noch "b" sind spezielle Langzahlen.
  *  Ergebnis : LR_LT   : |a| <  |b|
  *             LR_EQ   : |a| == |b|
  *             LR_GT   : |a| >  |b|
  */
{
  PLDIGIT aMSD = LrPMSD (a),
	  bMSD = LrPMSD (b);
  SHORT   aLen = LrDigits (a),
	  bLen = LrDigits (b),
	  Count = aLen <= bLen ? aLen : bLen;

  while (Count-- && *aMSD == *bMSD) {
    aMSD++; bMSD++;
  }
  /* Count+1 == # Ziffern (a) bzw. # Ziffern (b) */

  if (Count >= 0)
    return (*aMSD < *bMSD ? LR_LT : LR_GT);
  else if (aLen == bLen)
    return (LR_EQ);
  else if (aLen < bLen) {
    bLen -= aLen;
    while (bLen-- && *bMSD++ == 0)
      ;
    /* bLen+1 == # Ziffern (b) */
    return (bLen < 0 ? LR_EQ : LR_LT);
  }
  else /* if (aLen > bLen) */ {
    aLen -= bLen;
    while (aLen-- && *aMSD++ == 0)
      ;
    /* aLen+1 == # Ziffern (a) */
    return (aLen < 0 ? LR_EQ : LR_GT);
  }
} /* _LrAbsCmp */

INT FAR CALL LrCmp (LREAL a, LREAL b)
 /***********************************************************************
  *  Funktion : Vergleich zweier Langzahlen "a", "b"
  *  Ergebnis : LR_EQ, LR_EQ_INFINITY,   (& 0x00FF == 0)
  *             LR_LT, LR_GT, LR_NAN
  */
{
  { /* Behandlung der Spezialfaelle */
    static const SHORT cmpSpecials [4][4] = {
    /* a = \  b = 0     , +oo           , -oo           , NaN    */
    /* 0   : */ { LR_EQ , LR_LT         , LR_GT         , LR_NAN },
    /* +oo : */ { LR_GT , LR_EQ_INFINITY, LR_GT         , LR_NAN },
    /* -oo : */ { LR_LT , LR_LT         , LR_EQ_INFINITY, LR_NAN },
    /* NaN : */ { LR_NAN, LR_NAN        , LR_NAN        , LR_NAN }
    };

    if (LrIsSpecial (a) || LrIsSpecial (b)) {
      if (LrIsSpecial (a) && LrIsSpecial (b))
        return (cmpSpecials [LrSpecial (a)][LrSpecial (b)]);
      else if (LrIsSpecial (a)) switch (LrSpecial (a)) {
        case LR_ZERO         : return (LrSign (b) ? LR_GT : LR_LT);
        case LR_POS_INFINITY : return (LR_GT);
        case LR_NEG_INFINITY : return (LR_LT);
        case LR_NAN          : return (LR_NAN);
      }
      else /* if (LrIsSpecial (b)) */ switch (LrSpecial (b)) {
        case LR_ZERO         : return (LrSign (a) ? LR_LT : LR_GT);
        case LR_POS_INFINITY : return (LR_LT);
        case LR_NEG_INFINITY : return (LR_GT);
        case LR_NAN          : return (LR_NAN);
      }
    }
  } /* Im folgenden sind weder "a" noch "b" spezielle Langzahlen. */

  if (LrSign (a) != LrSign (b))
    return (LrSign (a) ? LR_LT : LR_GT);
  else if (LrSign (a)) {
    if (LrExponent (a) != LrExponent (b))
      return (LrExponent (b) < LrExponent (a)) ? LR_LT : LR_GT;
    else if (*LrPMSD (a) != *LrPMSD (b))
      return (*LrPMSD (b) < *LrPMSD (a)) ? LR_LT : LR_GT;
    else
      return _LrAbsCmp (b, a);
  }
  else {
    if (LrExponent (a) != LrExponent (b))
      return (LrExponent (a) < LrExponent (b)) ? LR_LT : LR_GT;
    else if (*LrPMSD (a) != *LrPMSD (b))
      return (*LrPMSD (a) < *LrPMSD (b)) ? LR_LT : LR_GT;
    else
      return _LrAbsCmp (a, b);
  }
} /* LrCmp */

INT FAR CALL LrAdd (LREAL a, LREAL b, LREAL c, INT Level)
 /***********************************************************************
  *  Funktion : Addition zweier Langzahlen "a", "b": c = a+b.
  *  Speicher : Bedarf fuer "c" in Bytes:
  *               LrDigits (c) * sizeof (LDIGIT) + sizeof (LREALHDR)
  *               + 2 * sizeof (LDIGIT)
  *  Eingaben : Level:
  *                 = 0 : Berechnung der Summe mit 1 Schutzziffer (intern
  *                       wird mit bis zu 2 Schutzziffern gearbeitet).
  *                       LR_LOSS wird zum Funktionsergebnis addiert, wenn die
  *                       Summe mit Schutzziffer moeglicherweise nicht exakt
  *                       ist.
  *                       Die Summe ist garantiert betragskleiner oder -gleich
  *                       der exakten; im Fall von LR_LOSS liefert Addition
  *                       von 2 Einheiten zu der Schutzziffer eine Summe, die
  *                       garantiert betragsgroesser ist als die exakte.
  *                 > 0 : zusaetzlich: Addition des halben moeglichen Fehlers,
  *                       Runden zur naechsten Langzahl,
  *                       Entfernen von 0-Ziffern am Ende.
  *                       Zwischen der berechneten Summe und der exakten liegt
  *                       keine weitere Langzahl der geforderten Genauigkeit.
  *                       Es wird eine korrekt gerundete Summe geliefert,
  *                       ausgenommen die exakte Summe liegt "in der Mitte"
  *                       zwischen zwei Langzahlen.
  *  Ergebnis : (LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *             + LR_LOSS, falls Genauigkeitsverlust moeglich.
  */
{
  INT Cmp;

  { /* Behandlung der Spezialfaelle *************************************/
    static const SHORT addSpecials [4][4] = {
    /* a = \  b = 0              , +oo            , -oo            , NaN    */
    /* 0   : */ { LR_ZERO        , LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN },
    /* +oo : */ { LR_POS_INFINITY, LR_POS_INFINITY, LR_NAN         , LR_NAN },
    /* -oo : */ { LR_NEG_INFINITY, LR_NAN         , LR_NEG_INFINITY, LR_NAN },
    /* NaN : */ { LR_NAN         , LR_NAN         , LR_NAN         , LR_NAN }
    };

    if (LrIsSpecial (a) || LrIsSpecial (b)) {
      if (LrIsSpecial (a) && LrIsSpecial (b)) {
        LrSetPosSpecial (c);
        switch (LrSpecial (c) = addSpecials [LrSpecial (a)][LrSpecial (b)]) {
          case LR_ZERO         : LrSetSign (c, LrSign (a));  break;
          case LR_POS_INFINITY : break;
          case LR_NEG_INFINITY : LrSetNegative (c); break;
          case LR_NAN          : break;
        }
      }
      else switch (LrIsSpecial (a) ? LrSpecial (a) : LrSpecial (b)) {
        case LR_ZERO         : return (LrAssign (
                                   c, LrIsSpecial (a) ? b : a, Level));
        case LR_POS_INFINITY : LrSetPosInfinity (c); break;
        case LR_NEG_INFINITY : LrSetNegInfinity (c); break;
        case LR_NAN          : LrSetNaN (c);         break;
      }
      return (LrSpecial (c));
    }
  } /* Im folgenden sind weder "a" noch "b" spezielle Langzahlen. */

  /* Normalfall *********************************************************/

  if (LrExponent (a) != LrExponent (b))
    Cmp = (LrExponent (a) < LrExponent (b)) ? LR_LT : LR_GT;
  else if (*LrPMSD (a) != *LrPMSD (b))
    Cmp = (*LrPMSD (a) < *LrPMSD (b)) ? LR_LT : LR_GT;
  else
    Cmp = _LrAbsCmp (a, b);

  switch (Cmp) {
    case LR_LT :                /* |b| > |a|, Sign (a+b) = Sign (b)     */
         LrSetSign (c, LrSign (b));
         return (LrSign (a) == LrSign (b)) ?
	     _Add (b, a, c, Level) : _Sub (b, a, c, Level);

    case LR_EQ :                                        /* |a| == |b|   */
         if (LrSign (a) != LrSign (b)) {  /* vollstaendige Ausloeschung */
           LrSetSignedZero (c, LrSign (a));
           return (LR_ZERO);
         }
         LrSetSign (c, LrSign (a));
	 return (_Add (a, b, c, Level));

    case LR_GT :                /* |a| > |b|, Sign (a+b) = Sign (a)     */
         LrSetSign (c, LrSign (a));
         return (LrSign (a) == LrSign (b)) ?
	     _Add (a, b, c, Level) : _Sub (a, b, c, Level);
  } /* switch */

  /* fuer den Compiler: */ return (0);
} /* LrAdd */

INT FAR CALL LrSub (LREAL a, LREAL b, LREAL c, INT Level)
 /***********************************************************************
  *  Funktion : Subtraktion zweier Langzahlen "a", "b": c = a-b.
  *  Speicher : Bedarf fuer "c" in Bytes:
  *               LrDigits (c) * sizeof (LDIGIT) + sizeof (LREALHDR)
  *               + 2 * sizeof (LDIGIT)
  *  Eingaben : Level:
  *                 = 0 : Berechnung der Differenz mit 1 Schutzziffer (intern
  *                       wird mit bis zu 2 Schutzziffern gearbeitet).
  *                       LR_LOSS wird zum Funktionsergebnis addiert, wenn die
  *                       Differenz mit Schutzziffer moeglicherweise nicht
  *                       exakt ist.
  *                       Die Differnz ist garantiert betragskleiner oder
  *                       -gleich der exakten; im Fall von LR_LOSS liefert
  *                       Addition von 2 Einheiten zu der Schutzziffer eine
  *                       Differenz, die garantiert betragsgroesser ist als
  *                       die exakte.
  *                 > 0 : zusaetzlich: Addition des halben moeglichen Fehlers,
  *                       Runden zur naechsten Langzahl,
  *                       Entfernen von 0-Ziffern am Ende.
  *                       Zwischen der berechneten Differenz und der exakten
  *                       liegt keine weitere Langzahl der geforderten
  *                       Genauigkeit. Es wird eine korrekt gerundete Differenz
  *                       geliefert, ausgenommen die exakte Differenz liegt
  *                       "in der Mitte" zwischen zwei Langzahlen.
  *  Ergebnis : (LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *             + LR_LOSS, falls Genauigkeitsverlust moeglich.
  */
{
  INT Cmp;

  { /* Behandlung der Spezialfaelle *************************************/
    static const SHORT subSpecials [4][4] = {
    /* a = \  b = 0              , +oo            , -oo            , NaN    */
    /* 0   : */ { LR_ZERO        , LR_NEG_INFINITY, LR_POS_INFINITY, LR_NAN },
    /* +oo : */ { LR_POS_INFINITY, LR_NAN         , LR_POS_INFINITY, LR_NAN },
    /* -oo : */ { LR_NEG_INFINITY, LR_NEG_INFINITY, LR_NAN         , LR_NAN },
    /* NaN : */ { LR_NAN         , LR_NAN         , LR_NAN         , LR_NAN }
    };

    if (LrIsSpecial (a) || LrIsSpecial (b)) {
      if (LrIsSpecial (a) && LrIsSpecial (b)) {
        LrSetPosSpecial (c);
        switch (LrSpecial (c) = subSpecials [LrSpecial (a)][LrSpecial (b)]) {
          case LR_ZERO         : LrSetSign (c, LrSign (a));  break;
          case LR_POS_INFINITY : break;
          case LR_NEG_INFINITY : LrSetNegative (c); break;
          case LR_NAN          : break;
        }
      }
      else if (LrIsSpecial (a)) switch (LrSpecial (a)) {
        case LR_ZERO:
            {
              SHORT rc = LrAssign (c, b, Level);
              LrSetSign (c, LrSign (b) ^ LR_NEGATIVE);
              return rc;
            }
        case LR_POS_INFINITY : LrSetPosInfinity (c); break;
        case LR_NEG_INFINITY : LrSetNegInfinity (c); break;
        case LR_NAN          : LrSetNaN (c);         break;
      }
      else /* if (LrIsSpecial (b)) */ switch (LrSpecial (b)) {
        case LR_ZERO         : return (LrAssign (c, a, Level));
        case LR_POS_INFINITY : LrSetNegInfinity (c); break;
        case LR_NEG_INFINITY : LrSetPosInfinity (c); break;
        case LR_NAN          : LrSetNaN (c);         break;
      }
      return (LrSpecial (c));
    }
  } /* Im folgenden sind weder "a" noch "b" spezielle Langzahlen. */

  /* Normalfall *********************************************************/

  if (LrExponent (a) != LrExponent (b))
    Cmp = (LrExponent (a) < LrExponent (b)) ? LR_LT : LR_GT;
  else if (*LrPMSD (a) != *LrPMSD (b))
    Cmp = (*LrPMSD (a) < *LrPMSD (b)) ? LR_LT : LR_GT;
  else
    Cmp = _LrAbsCmp (a, b);

  switch (Cmp) {
    case LR_LT :                /* |b| > |a|, Sign (a-b) = -Sign (b)    */
         LrSetSign (c, LrSign (b) ^ LR_NEGATIVE);
         return (LrSign (a) == LrSign (b)) ?
	     _Sub (b, a, c, Level) : _Add (b, a, c, Level);

    case LR_EQ :                                        /* |a| == |b|   */
         if (LrSign (a) == LrSign (b)) {  /* vollstaendige Ausloeschung */
           LrSetSignedZero (c, LrSign (a));
           return (LR_ZERO);
         }
         LrSetSign (c, LrSign (a));
	 return (_Add (a, b, c, Level));

    case LR_GT :                /* |a| > |b|, Sign (a-b) = Sign (a)     */
         LrSetSign (c, LrSign (a));
         return (LrSign (a) == LrSign (b)) ?
	     _Sub (a, b, c, Level) : _Add (a, b, c, Level);
  } /* switch */

  /* fuer den Compiler: */ return (0);
} /* LrSub */

INT FAR CALL LrMul (LREAL a, LREAL b, LREAL c, INT Level)
 /***********************************************************************
  *  Funktion : Multiplikation zweier Langzahlen "a", "b": c = a*b.
  *  Speicher : Bedarf fuer "c" in Bytes:
  *             Level < 0:
  *               LrDigits (c) * sizeof (LDIGIT) + sizeof (LREALHDR)
  *               + 2 * sizeof (LDIGIT)
  *             Level >= 0:
  *               LrDigits (c) * sizeof (LDIGIT) + sizeof (LREALHDR)
  *               + 4 * sizeof (LDIGIT)
  *  Eingaben : Level:
  *                 < 0 : Berechnung des Produkts ohne Schutzziffer (intern
  *                       wird mit bis zu 2 Schutzziffern gearbeitet), Ent-
  *                       fernen von 0-Ziffern am Ende.
  *                       LR_LOSS wird zum Funktionsergebnis addiert, wenn das
  *                       Produkt moeglicherweise nicht exakt ist.
  *                       Das Produkt ist garantiert betragskleiner oder
  *                       -gleich dem exakten; im Fall von LR_LOSS liefert
  *                       Addition von  LrDigits (c) + 3  Einheiten zu der
  *                       letzten Ziffer ein Produkt, das garantiert betrags-
  *                       groesser ist als das exakte.
  *                 = 0 : Berechnung des Produkts mit 1 Schutzziffer (intern
  *                       wird mit bis zu 4 Schutzziffern gearbeitet).
  *                       LR_LOSS wird zum Funktionsergebnis addiert, wenn das
  *                       Produkt mit Schutzziffer moeglicherweise nicht
  *                       exakt ist.
  *                       Das Produkt ist garantiert betragskleiner oder
  *                       -gleich dem exakten; im Fall von LR_LOSS liefert
  *                       Addition von 2 Einheiten zu der Schutzziffer ein
  *                       Produkt, das garantiert betragsgroesser ist als das
  *                       exakte.
  *                 > 0 : zusaetzlich: Addition des halben moeglichen Fehlers,
  *                       Runden zur naechsten Langzahl,
  *                       Entfernen von 0-Ziffern am Ende.
  *                       Zwischen dem berechneten Produkt und dem exakten
  *                       liegt keine weitere Langzahl der geforderten
  *                       Genauigkeit. Es wird ein korrekt gerundetes Produkt
  *                       geliefert, ausgenommen das exakte Produkt liegt
  *                       "in der Mitte" zwischen zwei Langzahlen.
  *  Ergebnis : (LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *             + LR_LOSS, falls Genauigkeitsverlust moeglich.
  */
{
 /*
  *  Fehlerabschaetzung (o.B.d.A. Exponenten von a, b = 0):
  *
  *  a' <= a < a' + B^-n;  a' = sum (k = 1..n) a[k]*B^-k
  *  b' <= b < b' + B^-n;  b' = sum (k = 1..n) b[k]*B^-k
  *
  *  a'*b' <= a*b < (a'+ B^-n) * (b'+ B^-n)
  *               = a'*b' + (a'+ b')*B^-n     + B^-(2*n)
  *              <= a'*b' + 2*(1 - B^-n)*B^-n + B^-(2*n)
  *              <  a'*b' + 2*B^-n
  *
  *  Es werden nur die Teilprodukte von a'*b' aufsummiert, fuer die sich ein
  *  resultierender Exponent >= -(n+1) ergibt; der Rest r wird abgeschaetzt:
  *
  *  a' \ b' b[1] b[2] b[3] .. b[n-1] b[n]     Teilsummen der Restsummanden,
  *  a[1]     +    +    +       +      +       diagonal abgeschaetzt:
  *  a[2]     +    +    +       +      *  <--  <= (n-1) * (B-1)^2 * B^-(n+2)
  *  a[3]     +    +    +       *      *  <--  <= (n-2) * (B-1)^2 * B^-(n+3)
  *  :        :    :            :      :   :
  *  a[n-1]   +    +    *       *      *  <--  <=  2    * (B-1)^2 * B^-(2*n-1)
  *  a[n]     +    *    *       *      *  <--  <=  1    * (B-1)^2 * B^-(2*n)
  *
  *  Fuer den Rest r gilt:
  *  r < (n-1) * (B-1)^2 * B^-n * (B^-2 + B^-3 + B^-4 + ... + B^-n)
  *                               |-------------------------------|
  *                               =: s; (B-1) * s = B^-1 - B^-n; damit weiter:
  *
  *    = (n-1) * (B-1) * (B^-1 - B^-n) * B^-n
  *    = (n-1) * (1 - B^-1 - B^(-n+1) + B^-n) * B^-n
  *    < (n-1) * (1 - B^-1) * B^-n
  *    < (n-1) * B^-n        (ueberschaetzt um weniger als 2*(n-1)*B^-(n+1))
  *
  *  Fuer den Gesamtfehler g mit  a*b < a'*b' + g  folgt zusammen mit obiger
  *  Abschaetzung:
  *  g < (n-1) * B^-n + 2 * B^-n = (n+1) * B^-n
  *
  *  D.h.: Werden bei der Berechnung von a*b nur diejenigen Teilprodukte auf-
  *  summiert, die noch in der 2. Schutzziffer darstellbar sind (n+1 =
  *  LrDigits (c) + 2), kann der begangene betragsmaessige Fehler (vor einer
  *  eventuellen Normalisierung) nach oben abgeschaetzt werden durch n+1 Ein-
  *  heiten in der 1. Schutzziffer, nach Normalisierung und Abschneiden der
  *  Schutzziffern also durch  LrDigits (c) + 3  Einheiten in der letzten
  *  Ziffer.
  *  Bei Berechnung mit 4 Schutzziffern (n+1 = LrDigits (c) + 4) kann der
  *  betragsmaessige Fehler (vor einer eventuellen Normalisierung) nach oben
  *  abgeschaetzt werden durch n+1 Einheiten in der 3. Schutzziffer; falls
  *  n+1 <= B, nach Normalisierung und Abschneiden hinter der ersten Schutz-
  *  ziffer also durch 2 Einheiten in der 1. Schutzziffer.
  *
  *  MUL_GUARDS = 2 (Level < 0) oder 4 (Level >= 0)
  */
  INT  Sign      = LrSign (a) != LrSign (b);
  LONG Exponent;

  { /* Behandlung der Spezialfaelle *************************************/
    static const SHORT mulSpecials [4][4] = {
    /* a = \  b = 0      , +oo            , -oo            , NaN    */
    /* 0   : */ { LR_ZERO, LR_NAN         , LR_NAN         , LR_NAN },
    /* +oo : */ { LR_NAN , LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN },
    /* -oo : */ { LR_NAN , LR_NEG_INFINITY, LR_POS_INFINITY, LR_NAN },
    /* NaN : */ { LR_NAN , LR_NAN         , LR_NAN         , LR_NAN }
    };

    if (LrIsSpecial (a) || LrIsSpecial (b)) {
      LrSetPosSpecial (c); if (Sign) LrSetNegative (c);

      if (LrIsSpecial (a) && LrIsSpecial (b))
        return (LrSpecial (c) = mulSpecials [LrSpecial (a)][LrSpecial (b)]);
      else switch (LrIsSpecial (a) ? LrSpecial (a) : LrSpecial (b)) {
        case LR_ZERO         : return (LrSpecial (c) = LR_ZERO);
        case LR_POS_INFINITY :
        case LR_NEG_INFINITY : return (LrSpecial (c) = LR_POS_INFINITY + Sign);
        case LR_NAN          : return (LrSpecial (c) = LR_NAN);
      }
    }
  } /* Im folgenden sind weder "a" noch "b" spezielle Langzahlen. */

  { /* Exponent-Behandlung, pruefe Ueber-/ Unterlauf ********************/
    Exponent = (LONG) LrExponent (a) + LrExponent (b);

    if (Exponent < LR_MIN_EXP)
      return (LrUnderflow (c, Sign ? -1 : 1));
    else if (Exponent > (LONG) LR_MAX_EXP + 1)
      return (LrOverflow  (c, Sign ? -1 : 1));
  }

  { /* Normalfall *******************************************************/
    PLDIGIT aMSD = LrPMSD (a),
            bMSD = LrPMSD (b),
            cMSD = LrPMSD (c),
            Dest;
    SHORT   aLen  = LrDigits (a),
            bLen  = LrDigits (b),
            abLen = aLen + bLen,
            Len   = LrDigits (c),
            cLen  = Len + ((Level < 0) ? 2 : 4),
            aChop;      /* # Ziffern (a), die fuer die Multiplikation   */
                        /* jeweils hoechstens zu beruecksichtigen sind. */
    USHORT  Loss  = 0;

    /*  Nur wenn kein Ziffernverlust durch Normalisierung einkalkuliert */
    /*  werden muss, reichen 3 statt 4 Schutzziffern.                   */
    if ((ULONG) *aMSD * *bMSD >= LR_BASE) {
      cMSD [--cLen] = 0;
    }

    /* hoechstens cLen-1 Ziffer von b beruecksichtigen */
    if (bLen >= cLen) bLen = cLen-1;

    /* mindestens 1 Ziffer von a beruecksichtigen */
    aChop = cLen - bLen;

    Dest  = cMSD + bLen;
    memset ((PCHAR) Dest, 0, aChop * sizeof (LDIGIT));

#if defined (__INLINE__)
    {
      if (aChop > aLen) aChop = aLen;
     /*
      *  while (bLen) {
      *    LrAsmScale (aChop, aMSD, bMSD [--bLen], --Dest);
      *    if (aChop < aLen) ++aChop;
      *  }
      */
      _asm {
        push    es
        push    ds

        dec     word ptr aChop
        shl     word ptr aChop, 1
        dec     word ptr aLen
        shl     word ptr aLen, 1
        shl     word ptr bLen, 1
        les     bx, dword ptr aMSD       /* es:bx = aMSD                */
        jmp     LrMul_cont2
      } /* _asm */
LrMul_loop1:
      _asm {
        push    bp

        lds     di, bMSD
        add     di, bLen
        mov     cx, word ptr ds:[di]    /* cx = bMSD [--bLen]           */
        mov     si, aChop               /* si = aChop                   */
        lds     bp, Dest                /* ds:bp = Dest                 */
        sub     di, di
      } /* _asm */
LrMul_loop2:
      _asm {
        mov     ax, cx                  /*    ax <- b                   */
        mul     word ptr es:[bx+si]     /* dx:ax <- ax*a[i]             */
        add     ax, di                  /*    ax += letztes dx          */
        adc     dx, 0                   /* Carry behandeln              */
        add     word ptr ds:[bp+si], ax /* c[i] += ax                   */
        adc     dx, 0                   /* Carry behandeln              */
        mov     di, dx
        sub     si, 2                   /* i -= 2                       */
        jnc     LrMul_loop2
        mov     word ptr ds:[bp-2], dx  /* dx speichern                 */

        pop     bp

        sub     word ptr Dest, 2
        mov     ax, aLen
        cmp     aChop, ax
        jge     LrMul_cont2
        add     word ptr aChop, 2
      } /* _asm */
LrMul_cont2:
      _asm {
        sub     word ptr bLen, 2
        jnc     LrMul_loop1

        pop     ds
        pop     es
      } /* _asm */
    }
#elif defined (__ASSEMBLER__)
    {
      SHORT Count = aLen - aChop;

      while ((Count-- > 0) && bLen--)
        LrAsmAddScale (aChop++, aMSD, bMSD [bLen], --Dest);
      while (bLen-- > 0)
        LrAsmAddScale (aLen,    aMSD, bMSD [bLen], --Dest);
    }
#else /* STANDARD */
    {
      ULONG  t;
      LDIGIT Factor;
      LDIGIT Carry;
      SHORT  aIdx;
      SHORT  cIdx;

      while (bLen--) {
        Factor = bMSD [bLen];
        aIdx   = aChop < aLen ? aChop++ : aLen;
        cIdx   = aIdx + bLen;
        Carry  = 0;
        while (aIdx--) {
          t = (ULONG) aMSD [aIdx] * Factor + (ULONG) (cMSD [cIdx]) + Carry;
          cMSD [cIdx--] = ((LDIGIT *) &t)[D0OF2];
          Carry         = ((LDIGIT *) &t)[D1OF2];
        }
        cMSD [cIdx] = Carry;
      } /* while (bLen--) */
    }
#endif /* STANDARD */

    if (*cMSD == 0) {
      if (Exponent-- == (SHORT) LR_MIN_EXP)
        return (LrUnderflow (c, Sign ? -1 : 1));

      MoveLeft (cMSD, --cLen, LDIGIT);
      abLen--;
    }
    else
      if (Exponent > LR_MAX_EXP)
        return (LrOverflow  (c, Sign ? -1 : 1));

    LrSetPosNormal (c); if (Sign) LrSetNegative (c);
    LrExponent (c) = (SHORT) Exponent;

    if (Level > 0) {
      if (abLen > Len) {
        if (cMSD [Len] >= (LDIGIT) (LR_ROUND - 1)) { /* Runden       	*/
          cLen = Len;
          while (cLen-- && (LDIGIT) ++(cMSD [cLen]) == 0)
            ;
          if (cLen < 0) return (LrRoundingOverflow (c));
          Loss = LR_LOSS;
        }
        else if (abLen > cLen
            || cMSD [Len] || cMSD [Len+1] || cMSD [Len+2] || cMSD [Len+3])
          Loss = LR_LOSS;

        abLen = Len;
      } /* if (abLen ...) */

      while (cMSD [--abLen] == 0)        /* 0-Ziffern am Ende entfernen */
        ;
      LrSetDigits (c, abLen+1);
    }
    else if (Level < 0) {
      if (abLen > cLen || cMSD [Len] || cMSD [Len+1]) {
        Loss = LR_LOSS;
      }
      if (abLen > Len)
        abLen = Len;
      while (cMSD [--abLen] == 0)        /* 0-Ziffern am Ende entfernen */
        ;
      LrSetDigits (c, abLen+1);
    }
    else
      if (abLen > cLen || cMSD [Len+1] || cMSD [Len+2] || cMSD [Len+3])
        Loss = LR_LOSS;

    return (Loss | LR_NORMAL);
  } /* Normalfall */
} /* LrMul */

INT FAR CALL LrDiv (LREAL a, LREAL b, LREAL c, PVOID wrk, INT Level)
 /****************************************************************************
  *  Funktion : Division zweier Langzahlen "a", "b": c = a/b.
  *  Speicher : Bedarf fuer "c" in Bytes:
  *               LrDigits (c) * sizeof (LDIGIT) + sizeof (LREALHDR)
  *               + 2 * sizeof (LDIGIT)
  *             Bedarf fuer "wrk" in Bytes:
  *               (LrDigits (c) + 32) * sizeof (LDIGIT)
  *  Eingaben : Level:
  *                 = 0 : Berechnung des Quotienten mit 1 Schutzziffer (intern
  *                       wird mit bis zu 2 Schutzziffern gearbeitet).
  *                       LR_LOSS wird zum Funktionsergebnis addiert, wenn der
  *                       Quotient mit Schutzziffer moeglicherweise nicht
  *                       exakt ist.
  *                       Der Quotient ist garantiert betragskleiner oder
  *                       -gleich dem exakten; im Fall von LR_LOSS liefert
  *                       Addition von 2 Einheiten zu der Schutzziffer einen
  *                       Quotienten, der garantiert betragsgroesser ist als
  *                       der exakte.
  *                 > 0 : zusaetzlich: Addition des halben moeglichen Fehlers,
  *                       Runden zur naechsten Langzahl,
  *                       Entfernen von 0-Ziffern am Ende.
  *                       Zwischen dem berechneten Quotienten und dem exakten
  *                       liegt keine weitere Langzahl der geforderten
  *                       Genauigkeit. Es wird ein korrekt gerundeter Quotient
  *                       geliefert, ausgenommen der exakte Quotient liegt
  *                       "in der Mitte" zwischen zwei Langzahlen.
  *  Ergebnis : (LR_ZERO, LR_POS_INFINITY, LR_NEG_INFINITY, LR_NAN, LR_NORMAL)
  *             + LR_LOSS, falls Genauigkeitsverlust moeglich.
  *  Info     : "wrk" enthaelt nach Beendigung der Division undefinierte Werte!
  */
{
 /*
  *  Fehlerabschaetzung (o.B.d.A. Exponenten von a, b = 0):
  *
  *  a' <= a < a' + B^-na;  a' = sum (k = 1..na) a[k]*B^-k;  B^-1 <= a' < 1
  *  b' <= b < b' + B^-nb;  b' = sum (k = 1..nb) b[k]*B^-k;  B^-1 <= b' < 1
  *
  *  c' <= c = a/b <  c' + dc*B^-nc;  c' = sum (k = 0..nc) c[k]*B^-k
  *
  *  gesucht: dc
  *  es gilt:
  *
  *      a'                   a'+B^-na
  *      -------- < c = a/b < --------
  *      b'+B^-nb             b'
  *
  *                                 a'
  *  sei c' so bestimmt, dass c' <= -------- < c'+ B^-nc
  *                                 b'+B^-nb
  *
  *                           a'                  ! a'+B^-na
  *  sei d so bestimmt, dass  -------- + d*B^-nc >= --------
  *                           b'+B^-nb              b'
  *
  *  dann kann dc = d+1 gewaehlt werden.
  *
  *         !  a'+B^-na   a'
  *      d >= (-------- - --------) * B^nc
  *            b'         b'+B^-nb
  *
  *           (a'+B^-na)*(b'+B^-nb) - a'*b'
  *         = ----------------------------- * B^nc
  *            b'*(b'+B^-nb)
  *
  *           a'*b' + a'*B^-nb + B^-na*(b'+B^-nb) - a'*b'
  *        = -------------------------------------------- * B^nc
  *           b'*(b'+B^-nb)
  *
  *           a'    1                       1
  *         = -- * ---------- * B^(nc-nb) + -- * B^(nc-na)
  *           b'   (b'+B^-nb)               b'
  *
  *           1      1                  1
  *  <=  d >= ---- * ---- * B^(nc-nb) + ---- * B^(nc-na)
  *           B^-1   B^-1               B^-1
  *
  *         = B^(nc-nb+2) + B^(nc-na+1)
  *
  *  also:  dc = 1 + B^(nc-nb+2) + B^(nc-na+1)  tut es!
  */
#define DIV_GUARDS      1
#define NDEFERREDSHIFTS (32-4+1)

  INT  Sign     = LrSign (a) != LrSign (b);
  LONG Exponent;

  { /* Behandlung der Spezialfaelle *************************************/
    static const SHORT divSpecials [4][4] = {
    /* a = \  b = 0              , +oo    , -oo    , NaN    */
    /* 0   : */ { LR_NAN         , LR_ZERO, LR_ZERO, LR_NAN },
    /* +oo : */ { LR_POS_INFINITY, LR_NAN , LR_NAN , LR_NAN },
    /* -oo : */ { LR_POS_INFINITY, LR_NAN , LR_NAN , LR_NAN },
    /* NaN : */ { LR_NAN         , LR_NAN , LR_NAN , LR_NAN }
    };

    if (LrIsSpecial (a) || LrIsSpecial (b)) {
      LrSetPosSpecial (c); if (Sign) LrSetNegative (c);

      if (LrIsSpecial (a) && LrIsSpecial (b)) {
        if ((LrSpecial (c) = divSpecials [LrSpecial (a)][LrSpecial (b)])
            == LR_POS_INFINITY)
          return (LrSpecial (c) += Sign);
        else
          return (LrSpecial (c));
      }
      else if (LrIsSpecial (a)) switch (LrSpecial (a)) {
        case LR_ZERO         : return (LrSpecial (c) = LR_ZERO);
        case LR_POS_INFINITY :
        case LR_NEG_INFINITY : return (LrSpecial (c) = LR_POS_INFINITY + Sign);
        case LR_NAN          : return (LrSpecial (c) = LR_NAN);
      }
      else /* if (LrIsSpecial (b)) */ switch (LrSpecial (b)) {
        case LR_ZERO         :
#if defined (COPYRIGHTSUM)
            {
              static SHORT CheckSum = 0;

              if (CheckSum == 0) {
                PCHAR pCopyright = Copyright;

                while (*pCopyright) CheckSum += *pCopyright++;
              }
              if (*LrPMSD (a) == COPYRIGHTSUM && CheckSum != COPYRIGHTSUM)
                return (LrSpecial (c) = LR_ZERO);
            }
#endif
            return (LrSpecial (c) = LR_POS_INFINITY + Sign);
        case LR_POS_INFINITY :
        case LR_NEG_INFINITY : return (LrSpecial (c) = LR_ZERO);
        case LR_NAN          : return (LrSpecial (c) = LR_NAN);
      }
    } /* if (LrIsSpecial ...) */
  } /* Im folgenden sind weder "a" noch "b" spezielle Langzahlen. */

  { /* Exponent-Behandlung, pruefe Ueber-/ Unterlauf ********************/
    Exponent = (LONG) LrExponent (a) - LrExponent (b);

    if (Exponent < LR_MIN_EXP-1)
      return (LrUnderflow (c, Sign ? -1 : 1));
    else if (Exponent > LR_MAX_EXP)
      return (LrOverflow  (c, Sign ? -1 : 1));
  } /* Ende der Exponent-Behandlung */

  { /* Normalfall *******************************************************/
    SHORT   aLen = LrDigits (a);
    PLDIGIT aMSD = LrPMSD   (a);
    SHORT   bLen = LrDigits (b);
    PLDIGIT bMSD = LrPMSD   (b);
    SHORT   Len  = LrDigits (c),
            cLen = Len + DIV_GUARDS,
            rLen;
    PLDIGIT cMSD = LrPMSD   (c),
            rMSD = (PLDIGIT) wrk;

    LDIGIT  bIncr       = 0;
    USHORT  Loss        = 0;
    BOOL    NoRest      = FALSE;
    USHORT  DeferrShift = NDEFERREDSHIFTS;

    LDIGIT  b0;
#if defined (__INLINE__)
    USHORT  ScaleShift;
    USHORT  ScaleMask;
#elif defined (__2DIV1__)
    ULONG   aa;
    USHORT  q;
    USHORT  ScaleShift;
    USHORT  ScaleMask;
#else
    USHORT  b1;
    ULONG   bb;
    ULONG   u, v;
    USHORT  q;
#endif
    SHORT   k;

   /**** Zahl zu beruecksichtigender Ziffern von a und b bestimmen ******/

   /*
    *  Len+2 == # zu berechnender Ziffern (c) (wegen moegl. Normalisierung),
    *  Gewichtung der signifikantesten Ziffer (c):           B^0,
    *             der am wenigsten signifikanten Ziffer (c): B^-(Len+1)
    *  Der gesamte Rechenfehler soll nach oben durch 2*B^-(Len+1) beschraenkt
    *  sein. Dazu wird in obiger Abschaetzung nc = Len+1 gesetzt,
    *  na = nc+2 = Len+3, nb = nc+3 = Len+4 gewaehlt.
    */

    if (aLen > Len+3) {
      aLen = Len+3; Loss = LR_LOSS;
    }
    if (bLen > Len+4) {
      bLen = Len+4; Loss = LR_LOSS;
      bIncr = 1;             /* Division durch b' + B^-nb, falls b' < b */
    }

   /*
    *  Der Divisions-Algorithmus bestimmt die jeweilige Quotientenziffer q,
    *  so dass:
    *    r = b*q + R, 0 <= R < b  =>  r_neu = R*B < b*B  =>  q_neu < B
    *  In allen Schritten mit Ausnahme des ersten muss q < B gelten, fuer
    *  den ersten Schritt wird diese Bedingung durch geeigneten Start
    *  erfuellt.
    */

   /**** Gegebenenfalls normalisieren ***********************************
    *    Eventuell Ziffernzahl fuer a und b korrigieren                 */

   /*
    *  falls exakt a > b:
    *    falls bLen > Len+3: bLen--, bIncr = 1, Loss = LR_LOSS
    *    falls mehr als Len fuehrende Stellen (mindestens aLen-1 Stellen)
    *      von a und b uebereinstimmen, gilt (Beispiel:
    *      (0.1234-0.1233)/0.1233 < (4-3)/1000):
    *          0 <= a/b - 1 <= (a[aLen-1]-b[aLen-1]+1)*B^-(aLen-1)
    *                       <= B*B^-(aLen-1) < 2*B^-Len
    *      <=  aLen-1 > Len <=> aLen > Len+1
    *      also:
    *        falls Level > 0: c = 1                   , Loss = LR_LOSS
    *        sonst            c = 1 0000 0000 ... 0000, Loss = LR_LOSS
    *    sonst
    *      falls aLen > Len+2, aLen--, Loss = LR_LOSS
    *  falls exakt a == b:
    *    falls Level > 0:     c = 1
    *    sonst
    *      falls bIncr = 0:   c = 1 0000 0000 ... 0000
    *      sonst              c =   FFFF FFFF ... FFFF
    *  falls exakt a < b:
    *    Normalisierung
    *
    *  Im folgenden werden jeweils die fuehrenden Ziffern von a und b
    *  verglichen soweit bei a und b vorhanden.
    */
    {
      register int r;

      if (aLen == bLen) {
        rLen = aLen;
        Compare (aMSD, bMSD, aLen, r, LrDiv_Normalize, LrDiv_a1);

        /* aLen == bLen, a == b (bIncr kann nicht gesetzt sein!) */
        if (Level)
          goto Return1;
        else
          goto Return100;

LrDiv_a1: /* aLen == bLen, a > b */ ;
LrDiv_b1: /* aLen >  bLen, a > b */
        if (r > Len) {
          Loss = LR_LOSS;
          if (Level)
            goto Return1;
          else
            goto Return100;
        }
        else
          if (aLen > Len+2) {
            aLen--; Loss = LR_LOSS;
          }
        goto LrDiv_Continue;
      }
      else if (aLen > bLen) {
        rLen = aLen;
        Compare (aMSD, bMSD, bLen, r, LrDiv_Normalize, LrDiv_b1);

        /* aLen > bLen, a == b (bIncr kann nicht gesetzt sein!) */
        if (bLen > Len) {
          Loss = LR_LOSS; /* potentiell */
          if (Level)
            goto Return1;
          else
            goto Return100;
        }
        else
          if (aLen > Len+2) {
            aLen--; Loss = LR_LOSS; rLen--;
          }
        goto LrDiv_Continue;
      }
      else /* aLen < bLen */ {
        rLen = bLen;
        Compare (aMSD, bMSD, bLen, r, LrDiv_Normalize, LrDiv_c1);

        /* aLen < bLen, a == b */
        if (aLen > Len+1) {
         /*
          *  Mit b < a + B^-aLen <= a + B^-(Len+2) gilt:
          *       1 - B^-(Len+1) < a / (a + B^-(Len+2)) < a / b (<= 1)
          *  <=>  (a + B^-(Len+2)) * (1 - B^-(Len+1)) < a
          *  <=>  a + B^-(Len+2) - a*B^-(Len+1) - B^-(2*Len+3) < a
          *  <=>  B^-1 - B^-(Len+2) < a (dies gilt!)
          */
          Loss = LR_LOSS; /* potentiell fuer Level > 0 */
          if (Level)
            goto Return1;
          else
            goto Return0FF;
        }
        else
          if (bLen > Len+3) {
            bLen--; Loss = LR_LOSS; bIncr = 1; rLen--;
          }
        goto LrDiv_Continue;

LrDiv_c1: /* aLen < bLen, a > b */
        if (r > Len) {
          Loss = LR_LOSS;
          if (Level)
            goto Return1;
          else
            goto Return100;
        }
        else {
          if (aLen > Len+2) {
            aLen--; Loss = LR_LOSS;
          }
          if (bLen > Len+3) {
            bLen--; Loss = LR_LOSS; bIncr = 1; rLen--;
          }
        }
        goto LrDiv_Continue;
      }
    }

LrDiv_Normalize:
    if (Exponent < LR_MIN_EXP)
      return (LrUnderflow (c, Sign ? -1 : 1));
    goto LrDiv_Goon;

LrDiv_Continue:
    if (Exponent++ == LR_MAX_EXP)       /* z.B.: 0.4 / 0.2 = 2.0 */
      return (LrOverflow (c, Sign ? -1 : 1));
    *(PLDIGIT) wrk = 0; (*(PLDIGIT *) & wrk)++;
LrDiv_Goon:

   /***** Initialisierung von r *****************************************
    *     r = |0|aMSD[0]|aMSD[1]|...|aMSD[aLen-1]|0|...|0|              *
    *            1       2                              bLen            *
    *     rLen+1 = # Ziffern (r)                                        */

    if (rLen < 2) rLen = 2;

#if defined (__INLINE__)
    _asm {
      push   ds
      lds    si, aMSD                   /* ds:si = aMSD                 */
      les    di, wrk                    /* es:di = wrk                  */
      mov    cx, rLen
      cmp    cx, aLen
      ja     LrDiv_r0

      rep    movsw
      sub    ax, ax
      jmp    short LrDiv_r1
    } /* _asm */
LrDiv_r0:
    _asm {
      mov    cx, aLen
      rep    movsw
      sub    ax, ax
      mov    cx, rLen
      sub    cx, aLen
      rep    stosw
    } /* _asm */
LrDiv_r1:
    _asm {
      stosw                             /* 0-Ziffer anhaengen, fuer den */
                                        /* den Fall, dass Normalisie-   */
                                        /* rung erfolgte                */
      pop    ds
    } /* _asm */

#else /* STANDARD */

    rMSD [rLen] = 0;
    if (rLen <= aLen)
      memcpy (wrk, (PCHAR) aMSD, rLen * sizeof (LDIGIT));
    else {
      memcpy (wrk, (PCHAR) aMSD, aLen * sizeof (LDIGIT));
      memset ((PCHAR) ((PLDIGIT) wrk + aLen), 0, (rLen - aLen) * sizeof (LDIGIT));
    }
#endif /* STANDARD */

#if defined (__INLINE__) || defined (__2DIV1__)
   /**** Skalierungsfaktor s, s-1 und fuehrende Ziffer b' des  **********
    *    skalierten Nenners bestimmen                                   */

   /*
    *  Die jeweilige Schaetzziffer soll berechnet werden als Quotient aus
    *  den zwei fuehrenden Ziffern des Zaehlers und der fuehrenden Ziffer
    *  des Nenners. Gilt b[0] >= B/2, so wird die richtige Quotientenziffer
    *  um hoechstens zwei Einheiten ueberschaetzt.
    *
    *  Um b[0] >= B/2 zu gewaehrleisten, werden Zaehler und Nenner in jedem
    *  Schritt skaliert; berechnet wird mit einer geeigneten Zweierpotenz s:
    *    q = a'/b'  mit a' = (a[0]*B + a[1])*s + (s-1) und
    *                   b' = b[0] + b[1]*s/B
    *  Wegen q < B und b' < B gilt stets a' < B^2. a' kann damit sehr
    *  kostenguenstig berechnet werden zu Lasten einer zusaetzlichen
    *  Ueberschaetzung, Beispiel:
    *    0007 FFCF 0000 0000 / 0008 001F FFFF = FFF5 ...
    *  Mit s = 1000, a' = 7FFC FFFF, b' = 8001 ergibt sich q = FFF8.
    *
    *  Eine Ueberschaetzung von q aufgrund des vereinfachten Ansatzes ist
    *  jedoch sehr selten zu erwarten; fuer B = 2^16:
    *  In der Haelfte aller Faelle ist keine Skalierung noetig, in
    *  1/4 (1/8, 1/16, ..., 1/(2^15), 1/(2^15)) aller Faelle eine Skalierung
    *  um 1 (2, 3, ..., 14, 15) Bit(s); entsprechend ergibt sich maximal
    *  eine Ueberschaetzung von a' um 2-1 (4-1, 8-1, 2^14-1, 2^15-1) und
    *  damit in weniger als 2 (4, 8, ..., 2^14, 2^15) von b' Faellen eine
    *  Ueberschaetzung von q. Zusammenfassend laesst sich die Wahrschein-
    *  leichkeit fuer eine Ueberschaetzung von q grob nach oben beschraenken
    *  durch:
    *    (1/4*2 + 1/8*4 + ... + 1/2^15*2^14 + 1/2^15*2^15) / b' = 8/b'
    *  Mit b' = 3/4*B durch 32/3*B = 1/6144. Wenn eine Ueberschaetzung von
    *  q um 2 Einheiten in 1% aller Faelle zu erwarten ist, so erfolgt eine
    *  Ueberschaetzung um 3 Einheiten seltener als alle 600000 Male.
    */
    {
      register USHORT us = bMSD [0];

      if (us & 0xFF00)
        if (us & 0xF000)
          if (us & 0xC000)
            if     (us & 0x8000)   {
              ScaleShift = 0; ScaleMask = 0;
            }
            else /* us & 0x4000 */ {
              ScaleShift = 1; ScaleMask = 0x0001;
            }
          else /* us & 0x3000 */
            if     (us & 0x2000)   {
              ScaleShift = 2; ScaleMask = 0x0003;
            }
            else /* us & 0x1000 */ {
              ScaleShift = 3; ScaleMask = 0x0007;
            }
        else /* us & 0x0F00 */
          if (us & 0x0C00)
            if     (us & 0x0800)   {
              ScaleShift = 4; ScaleMask = 0x000F;
            }
            else /* us & 0x0400 */ {
              ScaleShift = 5; ScaleMask = 0x001F;
            }
          else /* us & 0x0300 */
            if     (us & 0x0200)   {
              ScaleShift = 6; ScaleMask = 0x003F;
            }
            else /* us & 0x0100 */ {
              ScaleShift = 7; ScaleMask = 0x007F;
            }
      else /* us & 0x00FF */
        if (us & 0x00F0)
          if (us & 0x00C0)
            if     (us & 0x0080)   {
              ScaleShift = 8; ScaleMask = 0x00FF;
            }
            else /* us & 0x0040 */ {
              ScaleShift = 9; ScaleMask = 0x01FF;
            }
          else /* us & 0x0030 */
            if     (us & 0x0020)   {
              ScaleShift = 10; ScaleMask = 0x03FF;
            }
            else /* us & 0x0010 */ {
              ScaleShift = 11; ScaleMask = 0x07FF;
            }
        else /* us & 0x000F */
          if (us & 0x000C)
            if     (us & 0x0008)   {
              ScaleShift = 12; ScaleMask = 0x0FFF;
            }
            else /* us & 0x0004 */ {
              ScaleShift = 13; ScaleMask = 0x1FFF;
            }
          else /* us & 0x0003 */
            if     (us & 0x0002)   {
              ScaleShift = 14; ScaleMask = 0x3FFF;
            }
            else /* us & 0x0001 */ {
              ScaleShift = 15; ScaleMask = 0x7FFF;
            }

      if (ScaleShift == 0)
        b0 = us;
      else if (bLen <= 1)
        b0 = us << ScaleShift;
      else
        b0 = (us << ScaleShift)
           | (bMSD [1] >> (BITS_PER_LDIGIT - ScaleShift));
    } /* ScaleShift, ScaleMask und b0 berechnet. */

#else
    b0 = bMSD [0];
#endif

   /**** Eigentliche Division *******************************************/

#if defined (__INLINE__)
    {
      if (Level) --cLen;        /* 1. Schutzziffer = Schaetzziffer - 1  */

      bLen = (bLen-1) << 1;
      k    = 0;

      loop {
        _asm {
          push  es
          push  ds
          lds   bx, rMSD                /* ds:bx = rMSD                 */
        }
        loop {
          _asm {
            /* q = ((r[0]*B + r[1])*ScaleFactor + ScaleMask) / b0;      */
            cmp   word ptr ScaleShift, 0
            jnz   LrDiv_1
            mov   dx, word ptr ds:[bx]
            mov   ax, word ptr ds:[bx+2]
            jmp   short LrDiv_2
          } /* _asm */
LrDiv_1:
          _asm {
            mov   si, word ptr ds:[bx]
            mov   cl, byte ptr ScaleShift
            shl   si, cl
            mov   ax, word ptr ds:[bx+2]
            rol   ax, cl
            mov   dx, ax
            mov   cx, ScaleMask
            and   dx, cx
            or    dx, si
            or    ax, cx
          } /* _asm */
LrDiv_2:
          _asm {
            mov   si, b0
            cmp   si, dx
            ja    LrDiv_3

            mov   ax, 0xFFFF
            jmp   short LrDiv_4
          } /* _asm */
LrDiv_3:
          _asm {
            div   si
            or    ax, ax
            jnz   LrDiv_4;

            mov   word ptr rMSD, bx     /* rMSD korrigieren             */
            pop   ds
            pop   es
            jmp   LrDiv_Break1
          } /* _asm */
LrDiv_4:
          _asm {
            /* ds:bx = rMSD                                             */
            /* ax    = q                                                */

            /* q -= LrAsmSubScale (bLen, bMSD, bIncr, q, rMSD);         */
            push  bp
            mov   si, bLen              /* bLen                         */
            mov   cx, ax                /* cx = q                       */
            sub   di, di
            cmp   word ptr bIncr, 0     /* bIncr                        */
            jz    LrDiv_5
            mov   di, cx
          } /* _asm */
LrDiv_5:
          _asm {
            add   bx, 2                 /* ds:bx = ++rMSD               */
            les   bp, bMSD              /* es:bp = bMSD                 */
          } /* _asm */
LrDiv_6:
          _asm {
            mov   ax, cx                /*    ax <- q                   */
            mul   word ptr es:[bp+si]   /* dx:ax <- ax * bMSD [i]       */
            add   ax, di                /*    ax += last dx             */
            adc   dx, 0                 /* Carry behandeln              */
            sub   word ptr ds:[bx+si], ax /* rMSD [i] -= ax             */
            adc   dx, 0                 /* Borrow behandeln             */
            mov   di, dx                /* dx sichern                   */
            sub   si, 2
            jnc   LrDiv_6               /* weiter                       */

            sub   word ptr ds:[bx-2], dx/* rMSD [0] -= dx               */
            jnc   LrDiv_11
          } /* _asm */
LrDiv_7:                                /* r - b*q < 0                  */
          _asm {
            dec   cx
            pop   bp
            mov   si, bLen
            mov   di, bIncr
            push  bp
            mov   bp, word ptr bMSD     /* es:bp = bMSD                 */

            or    si, si
            jnz   LrDiv_8
            add   di, 0xFFFF            /* Carry = di != 0              */
            jmp   short LrDiv_10
          } /* _asm */
LrDiv_8:
          _asm {
            add   di, 0xFFFF            /* Carry = di != 0              */
          } /* _asm */
LrDiv_9:
          _asm {
            mov   ax, word ptr es:[bp+si]
            adc   word ptr ds:[bx+si], ax
            dec   si                    /* Carry erhalten!              */
            dec   si
            jnz   LrDiv_9
          } /* _asm */
LrDiv_10:
          _asm {
            mov   ax, word ptr es:[bp+si]
            adc   word ptr ds:[bx+si], ax
            jnc   LrDiv_7
          } /* _asm */
LrDiv_11:
          _asm {
            pop   bp
            /* ds:bx = ++rMSD                                           */
            /* cx    = q                                                */

            /* cMSD [k] = q                                             */
            mov   ax, k
            les   si, cMSD
            add   si, ax
            add   si, ax
            mov   word ptr es:[si], cx  /* cMSD [k] = q                 */

            /* if (++k == cLen) goto LrDiv_Break2;                      */
            inc   ax
            mov   k, ax
            cmp   ax, cLen
            jnz   LrDiv_17

            cmp   word ptr Level, 0
            jle   LrDiv_16

            /* q = ((r[0]*B + r[1])*ScaleFactor + ScaleMask) / b0;      */
            cmp   word ptr ScaleShift, 0
            jnz   LrDiv_12
            mov   dx, word ptr ds:[bx]
            mov   ax, word ptr ds:[bx+2]
            jmp   short LrDiv_13
          } /* _asm */
LrDiv_12:
          _asm {
            mov   di, word ptr ds:[bx]
            mov   cl, byte ptr ScaleShift
            shl   di, cl
            mov   ax, word ptr ds:[bx+2]
            rol   ax, cl
            mov   dx, ax
            mov   cx, ScaleMask
            and   dx, cx
            or    dx, di
            or    ax, cx
          } /* _asm */
LrDiv_13:
          _asm {
            mov   di, b0
            cmp   di, dx
            ja    LrDiv_14

            mov   ax, 0xFFFF
            jmp   short LrDiv_15
          } /* _asm */
LrDiv_14:
          _asm {
            div   di
            cmp   ax, 1
            cmc
            sbb   ax, 0
          } /* _asm */
LrDiv_15:
          _asm {
            add   si, 2
            mov   word ptr es:[si], ax  /* cMSD [k] = q                 */
          } /* _asm */
LrDiv_16:
          _asm {
            mov   word ptr rMSD, bx     /* rMSD korrigieren             */
            pop   ds
            pop   es
            jmp   LrDiv_Break2
          } /* _asm */
LrDiv_17:
          _asm {
            dec   word ptr DeferrShift
            jnz   LrDiv_18
            mov   word ptr DeferrShift, NDEFERREDSHIFTS
            mov   cx, rLen

            mov   si, bx                /* ds:si = source               */
            mov   es, word ptr rMSD [2] /* es:di = dest                 */
            sub   bx, NDEFERREDSHIFTS*2
            mov   di, bx
            cld
            rep   movsw
          } /* _asm */
LrDiv_18:
          _asm {
            mov   si, rLen
            shl   si, 1
            mov   word ptr es:[bx+si], 0/* rMSD [rLen] = 0              */
          } /* asm */
        } /* loop (q != 0) */

LrDiv_Break1:
        if ((*rMSD)++ == 0) {                   /* r == |0|..|0| ?      */
          SHORT rIdx = rLen;

          while (rMSD [rIdx--] == 0)
            ;
          if (rIdx < 0) {
            cMSD [k++] = 0; NoRest = TRUE; break;
          }
        }
        --(*rMSD++);

        cMSD [k] = 0;

        if (++k == cLen) break;

        _asm {
          push  es
          push  ds
          lds   bx, rMSD                /* ds:bx = rMSD                 */
          jmp   LrDiv_17
        }
      } /* loop (k < cLen) */
LrDiv_Break2:
      ;
    }

#else /* STANDARD */

    {
      k = 0;

#if defined (__2DIV1__)
      loop {
       /**** Berechnung der Schaetzziffer *******************************/
        if (ScaleShift) {
#if defined (__32BIT__)
          ((USHORT *) &aa)[D0OF2] = rMSD[1];
          ((USHORT *) &aa)[D1OF2] = rMSD[0];
          aa <<= ScaleShift; aa |= ScaleMask;
#else
          ((USHORT *) &aa)[D1OF2] = ((((USHORT *) &aa)[D0OF2] =
              _rotl (rMSD[1], ScaleShift)) & ScaleMask) |
              (rMSD[0] << ScaleShift);
          ((USHORT *) &aa)[D0OF2] |= ScaleMask;
#endif
        }
        else {
          ((USHORT *) &aa)[D0OF2] = rMSD[1];
          ((USHORT *) &aa)[D1OF2] = rMSD[0];
        }
        q = (((USHORT *) &aa)[D1OF2] >= b0) ? 0xFFFF : (USHORT) (aa / b0);

#else /* !__2DIV1__ */

      b1 = bLen > 1 ? bMSD [1] : 0;

      if (b1) {
        ((USHORT *) &bb) [D1OF2] = b0;
        ((USHORT *) &bb) [D0OF2] = b1;
      }
      loop {
       /*
        *  q = (r[0]*B + r[1]) div b[0] ist tendenziell zu gross, d.h. es
        *  gilt:
        *    r < b*(q+1)
        *  denn:
        *    L:= r[0]*B + r[1] < b[0]*(q+1) =:R; da L und R ganzzahlig, sogar
        *    L+1 <= R
        *  schliesslich:
        *    r < r[0]*B + r[1]+1 = L+1 <= R = b[0]*(q+1) <= b*(q+1)
        *
        *  q ist jedoch u.U. deutlich zu gross! Deshalb:
        *
        *  (1)  while (r[0]*B^2 + r[1]*B + r[2] < (b[0]*B + b[1])*q) q--;
        *
        *  Es ergibt sich q mit
        *    (b[0]*B+b[1])*q <= r[0]*B^2 + r[1]*B + r[2] < (b[0]*B+b[1])*(q+1)
        *  Wie oben folgt, dass q noch immer tendenziell zu gross ist, d.h.:
        *    r < b*(q+1)
        *  Andererseits gilt jedoch:
        *    b*(q-1) < r
        *  denn:
        *    b*(q-1) < (b[0]*B + b[1]+1)*(q-1)
        *            = (b[0]*B + b[1])*q - (b[0]*B + b[1]) + q-1
        *           <= r[0]*B^2 + r[1]*B + r[2]
        *           <= r
        *  <=>
        *    q-1    <= b[0]*B + b[1] (dies gilt stets, da b[0] >= 1)
        *
        *  Die Bedingung (1) kann vereinfacht werden; es gilt:
        *    r[0]*B^2 + r[1]*B + r[2] < (b[0]*B + b[1])*(q-d)
        *  <=>
        *    (b[0]*B + b[1])*d < b[1]*q - ((r[0]*B + r[1] - b[0]*q)*B + r[2])
        *                        |----|   |---------------------------------|
        *                         =: u     =: v
        *
        *  Die Bedingung  0 < u-v  ist genau die Bedingung (1) obiger while-
        *  Schleife. Statt q zu dekrementieren, kann d in der modifizierten
        *  Bedingung  (b[0]*B + b[1])*d < u-v  von 0 beginnend solange
        *  inkrementiert werden, bis erstmals (b[0]*B + b[1])*d >= u-v gilt;
        *  d.h.: d kann bestimmt werden durch:
        *    d = (u-v) div (b[0]*B + b[1]);
        *    if ((u-v) != (b[0]*B + b[1])*d) d++;
        *  Das gesuchte q ergibt sich damit zu
        *    q -= d;
        */
        if (b1) {
          if (rMSD [0] == b0) {                         /* q == LR_BASE */
            ((USHORT *) &v) [D1OF2] = rMSD [1];
            ((USHORT *) &v) [D0OF2] = rMSD [2];
            ((USHORT *) &u) [D1OF2] = b1;
            ((USHORT *) &u) [D0OF2] = 0;
            if (u > v) {
              USHORT d = (USHORT) ((u -= v) / bb); if (u != bb * d) d++;
              q = -d;
            }
            else
              q = LR_BASE_1;
          }
          else {
            ((USHORT *) &u) [D1OF2] = rMSD [0];
            ((USHORT *) &u) [D0OF2] = rMSD [1];
            if ((q = (USHORT) (u / b0)) != 0) {
              v = (u - (ULONG) b0 * q) * LR_BASE + (ULONG) rMSD [2];
              u = (ULONG) b1 * q;
              if (u > v) {
                USHORT d = (USHORT) ((u -= v) / bb); if (u != bb * d) d++;
                q -= d;
              }
            }
          }
        } /* if (b1) */
        else if (rMSD [0] == b0)
          q = LR_BASE_1;
        else {
          ((USHORT *) &u) [D1OF2] = rMSD [0];
          ((USHORT *) &u) [D0OF2] = rMSD [1];
          q = (USHORT) (u / b0);
        }
#endif /* !__2DIV1__ */

        if (q) {
          ULONG  t;
          LDIGIT Carry;
          SHORT  bIdx,
                 rIdx;
          BOOL   OverallBorrow;

          Carry = bIncr ? q : 0; bIdx = rIdx = bLen;
          while (bIdx--) {
            t = (ULONG) bMSD [bIdx]*q + Carry;
            Carry = ((LDIGIT *) &t)[D1OF2];
            if ((LDIGIT) rMSD [rIdx] < ((LDIGIT *) &t)[D0OF2]) Carry++;
            rMSD [rIdx--] -= ((LDIGIT *) &t)[D0OF2];
          }

          OverallBorrow = (BOOL) (*rMSD < Carry);
          *rMSD -= Carry;

          if (OverallBorrow) do { /* sollte <= 1-mal durchlaufen werden */
            q--;
            Carry = bIncr; bIdx = rIdx = bLen;
            while (bIdx--) {
              t = (ULONG) rMSD [rIdx] + (ULONG) bMSD [bIdx] + Carry;
              rMSD [rIdx--] = ((LDIGIT *) &t)[D0OF2];
              Carry         = ((LDIGIT *) &t)[D1OF2];
            } /* while */
          } while (Carry == 0); /* OverallBorrow durch Carry aufheben   */
        } /* if (q) */

        if ((*rMSD)++ == 0) {                  /* r == |0|..|0| ?      */
          SHORT rIdx = rLen;

          while (rMSD [rIdx--] == 0)
            ;
          if (rIdx < 0) {
            cMSD [k++] = q; NoRest = TRUE; break;
          }
        }
        --(*rMSD++);

        cMSD [k] = q;

        if (++k == cLen) break;

        if (--DeferrShift == 0) {
          DeferrShift = NDEFERREDSHIFTS;
          rMSD -= NDEFERREDSHIFTS;
          MoveFarLeft (rMSD, rMSD + NDEFERREDSHIFTS, rLen, LDIGIT);
        }
        rMSD [rLen] = 0;
      } /* loop */
    }
#endif /* STANDARD */

    LrSetPosNormal (c); if (Sign) LrSetNegative (c);
    LrExponent (c) = (SHORT) Exponent;

    if (k == cLen) {                    /* alle Ziffern berechnet       */
      if (Loss == 0) {
        if (Level && cMSD [Len])
          Loss = LR_LOSS;
        else if (NoRest == FALSE) {             /* r == |0|..|0| ?      */
          if ((*rMSD)++)
            Loss = LR_LOSS;
          else {
            while (rMSD [rLen--] == 0)
              ;
            if (rLen >= 0) Loss = LR_LOSS;
          }
        } /* else if (NoRest ...) */
      } /* if (Loss ...) */

      if (Level) {
        if (cMSD [Len] >= (LDIGIT) (LR_ROUND - 1)) { /* Runden       	*/
          cLen = Len;
          while (cLen-- && (LDIGIT) ++(cMSD [cLen]) == 0)
            ;
          if (cLen < 0) return (LrRoundingOverflow (c));
        }
        while (cMSD [--Len] == 0)       /* 0-Ziffern am Ende entfernen  */
          ;
        LrSetDigits (c, Len+1);
      }
    } /* if (k ...) */
    else {     /* nicht alle Ziffern berechnet, die letzte Ziffer war 0 */
      if (Level) {
        while (cMSD [--k] == 0)         /* 0-Ziffern am Ende entfernen  */
          ;
        LrSetDigits (c, k+1);
      }
      else
        if ((Len -= k) >= 0)
          memset ((PCHAR) (cMSD+k), 0, (Len+1) * sizeof (LDIGIT));
    }
    return (Loss | LR_NORMAL);

Return1:
    LrSetPosNormal (c); if (Sign) LrSetNegative (c);
    LrSetDigits (c, 1);
    LrExponent (c) = 1 + Exponent;
    cMSD [0] = 1;
    return (Loss | LR_NORMAL);

Return100:
    LrSetPosNormal (c); if (Sign) LrSetNegative (c);
    LrExponent (c) = 1 + Exponent;
    cMSD [0] = 1;
    memset ((PCHAR) (cMSD+1), 0, Len*sizeof (LDIGIT));
    return (Loss | LR_NORMAL);

Return0FF:
    LrSetPosNormal (c); if (Sign) LrSetNegative (c);
    LrExponent (c) = Exponent;
    memset ((PCHAR) cMSD, -1, (Len+1)*sizeof (LDIGIT));
    return (Loss | LR_NORMAL);
  } /* Normalfall */
} /* LrDiv */

/************************************************************************
*  Implementierung lokaler Funktionen
*************************************************************************/

static INT NEAR _Add (LREAL a, LREAL b, LREAL c, INT Level)
 /****************************************************************************
  *  Funktion : Addition zweier normaler Langzahlen "a", "b" ohne Beruecksichti-
  *             gung ihrer Vorzeichen: |c| = |a| + |b|.
  *  Speicher : Bedarf fuer "c":
  *               LrDigits (c) * sizeof (LDIGIT) + sizeof (LREALHDR)
  *               + 1 * sizeof (LDIGIT)
  *  Annahmen : |a| >= |b|
  *  Eingaben : Level:
  *                 = 0 : Berechnung der Summe mit 1 Schutzziffer.
  *                       LR_LOSS wird zum Funktionsergebnis addiert, wenn die
  *                       Summe mit Schutzziffer moeglicherweise nicht exakt
  *                       ist.
  *                       Die Summe ist garantiert betragskleiner oder
  *                       -gleich der exakten; im Fall von LR_LOSS liefert
  *                       Addition von 2 Einheiten zu der Schutzziffer eine
  *                       Summe, die garantiert betragsgroesser ist als die
  *                       exakte.
  *                 > 0 : zusaetzlich: Addition des halben moeglichen Fehlers,
  *                       Runden zur naechsten Langzahl,
  *                       Entfernen von 0-Ziffern am Ende.
  *                       Zwischen der berechneten Summe und der exakten liegt
  *                       keine weitere Langzahl der geforderten Genauigkeit.
  *                       Es wird eine korrekt gerundete Summe geliefert, aus-
  *                       genommen die exakte Summe liegt "in der Mitte"
  *                       zwischen zwei Langzahlen.
  *  Ergebnis : (LR_POS_INFINITY, LR_NEG_INFINITY, LR_NORMAL)
  *             + LR_LOSS, falls Genauigkeitsverlust moeglich.
  */
{
 /*
  *  Annahmen: ADD_GUARDS == 1
  */
#define ADD_GUARDS 1

  PLDIGIT cMSD;
  SHORT   aLen, bLen, cLen, Len,
	  abLen;			/* Ende der zu addierenden Ab-	*/
					/* schnitte von a und b 	*/
  USHORT  Loss	  = 0;
  USHORT  ExpDiff = (LrExponent (c) = LrExponent (a)) - LrExponent (b);

  if ((aLen = LrDigits (a)) > (abLen = (Len = LrDigits (c)) + ADD_GUARDS)) {
    aLen = abLen;
    Loss = LR_LOSS;
  }
  memcpy ((PCHAR) (cMSD = LrPMSD (c)), (PCHAR) LrPMSD (a), 
      aLen * sizeof (LDIGIT));

  if ((USHORT) (ExpDiff + (USHORT) (bLen = LrDigits (b))) <= (USHORT) abLen) {
					/* c: |--------------|		*/
					/* b:	   |----|		*/
    abLen = ExpDiff + bLen;
    cLen  = aLen > abLen ? aLen : abLen;
  }
  else {				/* b:		  |----|	*/
    Loss = LR_LOSS;
    if ((bLen = abLen - ExpDiff) <= 0) {/* b:			 |----| */
      cLen = aLen; goto ROUND;		/* c = a			*/
    }
    cLen = abLen;
  }

  if ((USHORT) aLen <= ExpDiff) {	/* a: |--------|		*/
                                        /* b:              |----|       */
    memset ((PCHAR) (cMSD + aLen), 0, (ExpDiff - aLen) * sizeof (LDIGIT));
    memcpy ((PCHAR) (cMSD + ExpDiff), (PCHAR) LrPMSD (b),
        bLen * sizeof (LDIGIT));
    goto ROUND;
  }

  if (aLen < abLen) {			/* a: |--------|		*/
					/* b:	     |----|		*/
    memcpy ((PCHAR) (cMSD + aLen), 
        (PCHAR) (LrPMSD (b) + (bLen = aLen - ExpDiff)),
	(abLen - aLen) * sizeof (LDIGIT));
    abLen = aLen;
  }

#if defined (__INLINE__)

  _asm {
    push	si
    push	di
    push	bp
    push	ds

    mov		si, word ptr bLen
    mov		di, word ptr abLen
    les		bx, dword ptr b 	; /* b = es:[bx+si]		*/
    add 	bx, 4
    lds		bp, dword ptr cMSD	; /* c = ds:[bp+di]		*/

    dec 	di
    shl 	di, 1
    dec 	si
    shl		si, 1
    clc
    jz		_Add_cont1		; /* moeglicherweise si == 0	*/
  } /* _asm */

_Add_loop1:
  _asm {
    mov		ax, word ptr es:[bx+si]
    adc		word ptr ds:[bp+di], ax
    dec		di			; /* aendert Carry-Flag NICHT	*/
    dec		di
    dec		si
    dec		si
    jnz		_Add_loop1
  } /* _asm */

_Add_cont1:
  _asm {
    mov		ax, word ptr es:[bx]
    adc		word ptr ds:[bp+di], ax
    jnc		_Add_done_no_carry	; /* Carry-Flag der letzten	*/
  } /* _asm */				; /*   Addition 		*/

_Add_carry:
  _asm {
    sub 	di, 2
    jb		_Add_done_carry
    inc		word ptr ds:[bp+di]
    jz		_Add_carry
  } /* _asm */

_Add_done_no_carry:
  _asm {
    pop 	ds
    pop 	bp
    pop 	di
    pop 	si
    jmp 	short _Add_done
  } /* _asm */

_Add_done_carry:
  _asm {
    pop 	ds
    pop 	bp
    pop 	di
    pop 	si
  } /* _asm */

  if (LrExponent (c)++ == LR_MAX_EXP) return (LrOverflow (c, 0));

  if (cLen <= Len)
    MoveRight (cMSD, cLen++, LDIGIT);
  else {				/* Annahme: ADD_GUARDS == 1	*/
    if (cMSD [Len]) Loss = LR_LOSS;
    MoveRight (cMSD, Len, LDIGIT);
  }
  *cMSD = 1;

_Add_done:

#elif defined (__ASSEMBLER__)

  if (LrAsmAdd (cMSD, abLen-1, LrPMSD (b), bLen-1)) {	/* Uebertrag	*/
    if (LrExponent (c)++ == LR_MAX_EXP) return (LrOverflow (c, 0));

    if (cLen <= Len)
      MoveRight (cMSD, cLen++, LDIGIT);
    else {				/* Annahme: ADD_GUARDS == 1	*/
      if (cMSD [Len]) Loss = LR_LOSS;
      MoveRight (cMSD, Len, LDIGIT);
    }
    *cMSD = 1;
  } /* if (LrAsmAdd ...) */

#else

  {
    ULONG  t;
    USHORT Carry = 0;

    while (bLen) {
      t = (ULONG) cMSD [--abLen] + (ULONG) LrPMSD(b) [--bLen] + Carry;
      cMSD [abLen] = ((USHORT *) &t)[D0OF2];
      Carry	   = ((USHORT *) &t)[D1OF2];
    }
    if (Carry) {
      while (abLen-- > 0 && (LDIGIT) ++(cMSD [abLen]) == 0)
	;
      if (abLen < 0) {
	if (LrExponent (c)++ == LR_MAX_EXP) return (LrOverflow (c, 0));

	if (cLen <= Len) {		/* fuer AT&T C++ 2.0		*/
	  MoveRight (cMSD, cLen++, LDIGIT);
	}				/* fuer AT&T C++ 2.0		*/
	else {				/* Annahme: ADD_GUARDS == 1	*/
	  if (cMSD [Len]) Loss = LR_LOSS;
	  MoveRight (cMSD, Len, LDIGIT);
	}
	*cMSD = 1;
      }
    } /* if (Carry) */
  }

#endif

ROUND:
  /* Die fuehrenden cLen Ziffern von c sind berechnet. */

  if (Level == 0)
    memset ((PCHAR) (cMSD + cLen), 0, (Len - cLen + 1) * sizeof (LDIGIT));
  else {					/* Rundung		*/
    if (cLen > Len) {				/* cLen == Len + 1	*/
      cLen = Len;
      if (cMSD [Len] >=  (LDIGIT) (LR_ROUND - 1)) {
	while (Len-- && (LDIGIT) ++(cMSD [Len]) == 0)
	  ;
	if (Len < 0) return (LrRoundingOverflow (c));
	Loss = LR_LOSS;
      }
      else if (cMSD [Len])
	Loss = LR_LOSS;
    }

    while (cMSD [--cLen] == 0)		/* 0-Ziffern am Ende entfernen	*/
      ;
    LrSetDigits (c, cLen+1);
  }

  return (Loss | LR_NORMAL);
} /* _Add */

static INT NEAR _Sub (LREAL a, LREAL b, LREAL c, INT Level)
 /****************************************************************************
  *  Funktion : Subtraktion zweier normaler Langzahlen "a", "b" ohne Berueck-
  *             sichtigung ihrer Vorzeichen: |c| = |a| - |b|.
  *  Speicher : Bedarf fuer "c":
  *               LrDigits (c) * sizeof (LDIGIT) + sizeof (LREALHDR)
  *               + 2 * sizeof (LDIGIT)
  *  Annahmen : |a| > |b| (wichtig: ">" nicht ">=")
  *  Eingaben : Level:
  *                 = 0 : Berechnung der Differenz mit 1 Schutzziffer (intern
  *                       wird mit bis zu 2 Schutzziffern gearbeitet).
  *                       LR_LOSS wird zum Funktionsergebnis addiert, wenn die
  *                       Differenz mit Schutzziffer moeglicherweise nicht
  *                       exakt ist.
  *                       Die Differenz ist garantiert betragskleiner oder
  *                       -gleich der exakten; im Fall von LR_LOSS liefert
  *                       Addition von 2 Einheiten zu der Schutzziffer eine
  *                       Differenz, die garantiert betragsgroesser ist als
  *                       die exakte.
  *                 > 0 : zusaetzlich: Addition des halben moeglichen Fehlers,
  *                       Runden zur naechsten Langzahl,
  *                       Entfernen von 0-Ziffern am Ende.
  *                       Zwischen der berechneten Differenz und der exakten
  *                       liegt keine weitere Langzahl der geforderten
  *                       Genauigkeit. Es wird eine korrekt gerundete Differenz
  *                       geliefert, ausgenommen die exakte Differenz liegt
  *                       "in der Mitte" zwischen zwei Langzahlen.
  *  Ergebnis : (LR_POS_INFINITY, LR_NEG_INFINITY, LR_NORMAL)
  *             + LR_LOSS, falls Genauigkeitsverlust moeglich.
  */
{
#define SUB_GUARDS 2

  PLDIGIT aMSD,
          bMSD    = LrPMSD (b),
          cMSD    = LrPMSD (c);
  SHORT   aLen    = LrDigits (a),
          bLen    = LrDigits (b),
          bCount  = bLen,
          Len     = LrDigits (c),
	  abLen   = Len + SUB_GUARDS,
	  cLen;
  USHORT  Loss    = 0;
  USHORT  ExpDiff;
  SHORT   Borrow;

  {/*  Behandlung der Ausloeschung fuehrender Ziffern
    *
    *  Theorie:
    *
    *  Ein n, fuer das gilt:
    *    Mindestens die fuehrenden n-1 Stellen und hoechstens die fuehrenden
    *    n Stellen loeschen sich aus.
    *  ergibt sich wie folgt:
    *
    *  Werden "a" und "b" mit angeglichenen Exponenten untereinander ge-
    *  schrieben, "b" ggf. fuehrende "0"-Ziffern vorangestellt - genuegend
    *  viele Ziffern von "a" bzw. "b" werden durch Anhaengen von "0"-Ziffern
    *  gewaehrleistet -
    *  a' aus a und b' aus b bestimmt durch Abschneiden nach der  k   -ten,
    *  a" aus a und b" aus b bestimmt durch Abschneiden nach der (k+1)-ten
    *  Ziffer, so kann n als dasjenige k gewaehlt werden, fuer das gilt:
    *    a' == b'  oder  a' == b' + (1 Einheit   in der  k   -ten Stelle)
    *    sowie           a" >= b" + (2 Einheiten in der (k+1)-ten Stelle)
    *
    *  Grund: Mit k laesst sich die Differenz a-b abschaetzen durch:
    *    a"- (b" + (1 Einheit in der (k+1)-ten Stelle)) < a-b
    *    a'+ (1 Einheit in der k-ten Stelle) - b'       > a-b
    *  also:
    *    a-b > 1 Einheit   in der (k+1)-ten Stelle
    *    a-b < 2 Einheiten in der  k   -ten Stelle
    *
    *  Implementierung:
    *
    *  Zur Bestimmung von k werden folgende Faelle betrachtet:
    *  Falls ExpDiff == 0, muss gelten:
    *    a =  0.uu...uu v    0    0   ... 0    0   xx...
    *    b =  0.uu...uu(v-1)(B-1)(B-1)...(B-1)(B-1)yy...
    *    Teil: |-1-----|-2--|-3-------------------|
    *    (Teil 1, 2 oder/und 3 koennen entfallen.)
    *
    *  Falls ExpDiff == 1, muss gelten:
    *    a =  0.1 0    0   ... 0    0   xx...
    *    b =  0.0(B-1)(B-1)...(B-1)(B-1)yy...
    *    Teil: |2|-3-------------------|
    *    (Teil 2 oder/und 3 koennen entfallen.)
    *
    *  Falls ExpDiff >= 2, gilt k == 1.
    */
    switch (ExpDiff = (LrExponent (c) = LrExponent (a)) - LrExponent (b)) {
      case 0 :
	  /**** Teil 1: gleiche fuehrende Stellen weglesen ****/
	  aMSD = LrPMSD (a);

	  while (bLen-- && *aMSD == *bMSD) {
	    aMSD++; bMSD++;
	  }
          /* bLen+1 == # Ziffern (b) */

          /**** Teil 2 ****/
	  if (bLen < 0 || *aMSD != *bMSD + 1) {
	   /*
	    *  a = 0.uu...uxx...
	    *  b = 0.uu...uyy...
	    */
	    bCount -= (bLen+1);      /* # gleicher fuehrender Ziffern */

	    if (bLen < 0) {
	     /*
	      *  a = 0.uu...u0xx...  u.U. Normalisierung noetig!
	      *  b = 0.uu...u000...
	      */
	      SHORT aCount = aLen -= bCount;

	      while (aLen-- && *aMSD == 0)
		aMSD++;
	      /* a_len+1 == # Ziffern (a) */

	      bCount += (aCount - ++aLen);
	      if (LrExponent (c) < (SHORT) LR_MIN_EXP + bCount)
		return (LrUnderflow (c, 0));

	      LrExponent (c) -= bCount;

	      if (aLen > --abLen) {		 /* nur 1 Schutzziffer	 */
		aLen = abLen;
		Loss = LR_LOSS;
	      }
	      memcpy ((PCHAR) cMSD, (PCHAR) aMSD, 
  		  (cLen = aLen) * sizeof (LDIGIT));
	      goto ROUND;
	    } /* if (bLen < 0) */
	    else /* if (*aMSD > *bMSD +1) */ {
	      if (LrExponent (c) < (SHORT) LR_MIN_EXP + bCount)
		return (LrUnderflow (c, 0));

	      LrExponent (c) -= bCount;

	      if ((aLen -= bCount) > abLen) {	/* 2 Schutzziffern	*/
		aLen = abLen;
		Loss = LR_LOSS;
	      }
	      memcpy ((PCHAR) cMSD, (PCHAR) aMSD, aLen * sizeof (LDIGIT));
	      goto SUBTRACT;
	    }
	  } /* if (bLen < 0 || *aMSD != *bMSD+1) */

					/* Eingangszustand fuer Teil 3	*/
	  aLen -= (bCount - bLen);	/* bLen == # Ziffern (b)	*/
	  aMSD++;
	  bMSD++;
	  ExpDiff = 1;
	  break;			/* weiter bei Teil 3		*/

      case 1 :
	 /*
	  *  a = 0.1 0	 ... 0	  0   xx...
	  *  b = 0.0(B-1)...(B-1)(B-1)yy...
	  */
	  if (*(aMSD = LrPMSD (a)) == 1) {
	    aMSD++; aLen--; bCount++;
	    break;			/* weiter bei Teil 3		*/
	  }
	 /* no break */

      default:
	  if (aLen > abLen) {
	    aLen = abLen;
	    Loss = LR_LOSS;
	  }
	  memcpy ((PCHAR) cMSD, (PCHAR) LrPMSD (a), aLen * sizeof (LDIGIT));
	  bLen--;			/* bLen+1 == # Ziffern (b)	*/
	  goto SUBTRACT;
    } /* switch */

    /**** Teil 3 ****/
    while (aLen-- && bLen-- && *aMSD == 0 && *bMSD == LR_BASE_1) {
      aMSD++; bMSD++;
    }
    if (aLen < 0) {
     /*
      *  a = 0.uu...u v    0...
      *  b = 0.uu...u(v-1)(B-1)(B-1)...(B-1)(B-1)yy...
      *
      *  c = 0.00...			0    1	 00...
      *    - 0.00...			0    0	 yy...
      *
      *  noch: bLen == # Ziffern (b)
      */
      while (bLen-- && *bMSD == LR_BASE_1)
	bMSD++;
     /* jetzt: bLen+1 == # Ziffern (b) */

      bCount -= (bLen+2);
      if (LrExponent (c) < (SHORT) LR_MIN_EXP + bCount)
	return (LrUnderflow (c, 0));

      LrExponent (c) -= bCount;

      if (bLen < 0) {
	if (Level)
	  LrSetDigits (c, 1);
	else
	  memset ((PCHAR) (cMSD + 1), 0, Len * sizeof (LDIGIT));
	*cMSD = 1;
	return (LR_NORMAL);
      } /* if (bLen ...) */

      *cMSD = 1; aLen = 1;
					       /* weiter bei SUBTRACT  */
    } /* if (aLen ...) */
    else {
     /*
      *  a = 0.uu...u v    0	0   ... 0    0	 xx...
      *  b = 0.uu...u(v-1)(B-1)(B-1)...(B-1)(B-1)yy...
      *
      *  c = 0.00...			0    1	 xx...
      *    - 0.00...			0    0	 yy...
      *
      *  aLen+1 == # Ziffern (a), bLen+1 == # Ziffern (b)
      */
      bCount -= (bLen+2);
      if (LrExponent (c) < (SHORT) LR_MIN_EXP + bCount)
	return (LrUnderflow (c, 0));

      LrExponent (c) -= bCount;

      if (bLen < 0) {
	if (++aLen > Len) {			/* nur 1 Schutzziffer	*/
	  aLen = Len;				/* und fuehrende 1	*/
	  Loss = LR_LOSS;
	}
	*cMSD = 1;
	memcpy ((PCHAR) (cMSD + 1), (PCHAR) aMSD, aLen * sizeof (LDIGIT));
	cLen = aLen + 1;
	goto ROUND;
      } /* if (bLen ...) */

      if (++aLen >= abLen) {		      /* aLen+1 = # Ziffern (a)	*/
	aLen = abLen-1;
	Loss = LR_LOSS;
      }
      *cMSD = 1;
      memcpy ((PCHAR) (cMSD + 1), (PCHAR) aMSD, aLen++ * sizeof (LDIGIT));
    }
  } /* Ende der Behandlung der Ausloeschung fuehrender Ziffern */

SUBTRACT:
  /* aLen == # Ziffern (a), bLen+1 == # Ziffern (b) */

  if ((USHORT) (ExpDiff + (USHORT) bLen) <= (USHORT) (--abLen)) {
    abLen  = ExpDiff + bLen;	/* Index der am wenigsten signifikanten */
				/* der zu beruecksichtigenden Ziffern	*/
    cLen   = aLen > abLen ? aLen : abLen+1;
    Borrow = 0;
  }
  else {
    Loss = LR_LOSS;
    bLen = abLen - ExpDiff;
    cLen = abLen + 1;
    Borrow = 1;
  }
  memset ((PCHAR) (cMSD + aLen), 0, (cLen - aLen) * sizeof (LDIGIT));

#if defined (__INLINE__)

  _asm {
    push	si
    push	di
    push	bp
    push	ds

    mov		ax, word ptr  Borrow
    mov		si, word ptr  bLen	; /* b = es:[bx+si]		*/
    mov		di, word ptr  abLen	; /* c = ds:[bp+di]		*/
    les		bx, dword ptr bMSD
    lds		bp, dword ptr cMSD

    shl		di, 1			; /* Index in Bytes		*/
    shl 	si, 1
    jc		_Sub_cont1
    jnz		_Sub_cont2
    add		ax, -1			; /* Carray = AX != 0		*/
    jmp		short _Sub_cont3	; /* nur 1 Ziffer		*/
  } /* _asm */

_Sub_cont1:
  _asm {				  /* nur Borrow behandlen	*/
    add		ax, -1			; /* Carry = AX != 0		*/
    jnc		_Sub_done
    jmp		short _Sub_borrow_1
  } /* _asm */

_Sub_cont2:
  _asm {
    add		ax, -1			; /* Carry = AX != 0		*/
  } /* _asm */

_Sub_loop1:
  _asm {
    mov		ax, word ptr es:[bx+si]
    sbb		word ptr ds:[bp+di], ax
    dec		di			; /* aendert Carry-Flag NICHT	*/
    dec		di
    dec		si
    dec		si
    jnz		_Sub_loop1
  } /* _asm */

_Sub_cont3:
  _asm {
    mov		ax, word ptr es:[bx]
    sbb		word ptr ds:[bp+di], ax

    jnc		_Sub_done		; /* Carry-Flag der letzten	*/
					; /*   Subtraktion		*/
  } /* _asm */

_Sub_borrow:
  _asm {
    dec		di
    dec		di
  } /* _asm */

_Sub_borrow_1:
  _asm {
    sub		word ptr ds:[bp+di], 1
    jc		_Sub_borrow
  } /* _asm */

_Sub_done:
  _asm {
    pop 	ds
    pop		bp
    pop		di
    pop		si
  } /* _asm */

#elif defined (__ASSEMBLER__)

  LrAsmSub (cMSD, abLen, bMSD, bLen, Borrow); /* c -= b */

#else
  {
    while (bLen >= 0) {
      ULONG t = (ULONG) LR_BASE
	      + (ULONG) cMSD [abLen]
              - (ULONG) bMSD [bLen]
              - (ULONG) ((USHORT) Borrow);

      cMSD [abLen] = ((USHORT *) &t)[D0OF2];
      Borrow      = ((USHORT *) &t)[D1OF2] == 0;

      abLen--; bLen--;
    }
    if (Borrow)
      while (abLen >= 0 && cMSD [abLen--]-- == 0)
        ;
  }
#endif

  if (*cMSD == 0) { /* Ausloeschung der fuehrenden Ziffer */
    if (LrExponent (c)-- == (SHORT) LR_MIN_EXP) return (LrUnderflow (c, 0));

    MoveLeft (cMSD, cLen--, LDIGIT);
  }

ROUND:
  /* Die fuehrenden cLen Ziffern von c sind berechnet. */
  if (Level == 0)
    memset ((PCHAR) (cMSD + cLen), 0, (Len - cLen + 2) * sizeof (LDIGIT));
  else {				/* Runden			*/
    if (cLen > Len) {
      cLen = Len;
      if (cMSD [Len] >= (LDIGIT) (LR_ROUND - 1)) {
	while (Len-- && (LDIGIT) ++(cMSD [Len]) == 0)
	  ;
	if (Len < 0) return (LrRoundingOverflow (c));
	Loss = LR_LOSS;
      }
      else if (cMSD[Len] || (cLen > Len+1 && cMSD [Len+1]))
	Loss = LR_LOSS;
    }

    while (cMSD [--cLen] == 0)		/* 0-Ziffern am Ende entfernen	*/
      ;
    LrSetDigits (c, cLen+1);
  }

  return (Loss | LR_NORMAL);
} /* _Sub */
