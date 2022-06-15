/*******
 Made reentrant by Paul Kienitz 12/31/90.  Used function prototypes just to
 avoid booboos.  Also replaced _toupper with real toupper.  I hope someday to
 eliminate the error return of CmplPat; make missing parens be assumed to be
 at the beginning or end as appropriate, and an empty half of | be treated
 like a %.  I guess ' at the end is ignored?  Actually it already ignores
 extra right parens at the end.  But with an extra right paren in the middle
 it ignores everything after it.
*******/

/* PatMatch.c - Implements AmigaDos Regular Expression Pattern Matching.
**
**  This program will test whether a string is an AmigaDos  regular expression
**  It may be used to implement wild expressions such as:
**
**    "copy #?.c to <dir>" to copy any file ending in .c
**
**  The program has two entry points: CmplPat, and Match.
**
**    CmplPat - takes a pattern and returns an auxilliary array of short
**              ints which is used by Match.  The pattern is not modified
**              in any way.  CmplPat returns 1 if no errors were detected
**              while compiling the pattern; otherwise it returns 0;
**
**    Match   - takes the pattern, the auxilliary vector, and the string
**              to be matched.  It returns 1 if the string matches the
**              pattern; otherwise it returns 0;
**
**  Translated from BCPL by:
**              Jeff Lydiatt
**              Richmond B.C. Canada
**              16 May 1986.
**
**  Source: "A Compact Function for Regular Expression Pattern Matching",
**           Software - Practice and Experience, September 1979.
**
**  Useage:
**     To test if "file.c" matches the regular expression "#?.c"
**     char *Pat = "#?.c";
**     char *Str = "file.c";
**     WORD Aux[128];
**
**     if ( CmplPat( Pat, Aux ) == 0 )
**        {
**           printf("Bad Wildcard Expression\n");
**           exit(1);
**        }
**     if ( Match( Pat, Aux, Str ) == 1 )
**        printf("String matches the pattern\n");
**     else
**        printf("String does NOT match the pattern\n");
**/
/****************************************************************
 *
 *      Project:   UIK
 *      Function:  Pattern matching
 *      Authors:   See above
 *
 *      Created:   17/05/91    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "lci:uik_protosint.h"

#define  EOS '\0'

struct CM {
    short *Aux;
    char  *Pat;
    short PatP, Patlen;
    BOOL  Errflag;
    char  Ch;
};

#define CC register struct CM *c


/****** Imported ************************************************/

extern struct UIKBase * __far UIKBase;
extern ULONG __far DOSBase;


/****** Exported ************************************************/


/****** Statics *************************************************/

static short Exp( short AltP, CC );


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

/*----------------------------------------------------------------*/
/*                   The Interpreter                              */
/*----------------------------------------------------------------*/


struct GM {
    long  Succflag;
    short Wp;
    short *Work;
};


static void Put(short N, register struct GM *m)
{
   register short *ip;
   register short *to;

   if ( N == 0 )
      m->Succflag = TRUE;
   else
      {
        for ( ip = m->Work + 1, to = m->Work + m->Wp; ip <= to; ip++)
           if ( *ip == N )
              return;
        m->Work[ ++m->Wp ] = N;
      }
}

long __asm Match( register __d0 char Pat[], register __a0 short Aux[], register __a1 char Str[] )
{
   struct GM m;
   short     W[ 128 ];
   ULONG     S = 0;
   ULONG     I, N, Strlength;
   char      Ch;
   register short Q, P;
   register char K;

   m.Work = W;
   m.Wp = 0;
   m.Succflag = FALSE;
   Strlength = StrLen( (UBYTE*)Str );
   Put( 1, &m );

   Aux++;
   if ( Aux[ 0 ] != 0 )
      Put( Aux[ 0 ], &m );

   for(;;)
      {
        /* First complete the closure */
        for( N=1; N <= m.Wp; N++ )
          {
             P = m.Work[ N ];
             K = Pat[ P-1 ];
             Q = Aux[ P ];
             switch( K )
                {
                  case '#': Put( P + 1, &m );
                  case '%': Put( Q, &m );
                  default : break;
                  case '(':
                  case '|': Put( P + 1, &m);
                            if ( Q != 0 )
                               Put( Q, &m );
                }
           }

        if ( S >= Strlength )
           return m.Succflag;
        if ( m.Wp == 0 )
           return FALSE;
        Ch = Str[ S++ ];

        /* Now deal with the match items */

        N = m.Wp;
        m.Wp = 0;
        m.Succflag = FALSE;

        for ( I = 1; I <= N; I++)
          {
             P = m.Work[ I ];
             K = Pat[ P - 1 ];
             switch( K )
               {
                 case '#':
                 case '|':
                 case '%':
                 case '(': break;
                 case '\'': K = Pat[ P ];
                 default : if ( UIK_ToUpper( Ch ) != UIK_ToUpper( K ) )
                              break;
                 case '?': /* Successful match */
                           Put( Aux[ P ], &m );
                } /* End Switch */
          } /* End For I */
     } /* End for(;;) */
}

LONG __asm UIK_Match( register __d0 UBYTE *Pat, register __a0 UBYTE *Aux, register __a1 UBYTE *Str )
{
  UWORD version = ((struct Library *)(*(ULONG*)4))->lib_Version;

    if (version < 37 || (*Aux == 0))
        {
        return( Match( (APTR)Pat, (APTR)Aux, (APTR)Str ) );
        }
    else{
        BOOL res;
        res = MatchPatternNoCase( (STRPTR)Aux+2, (STRPTR)Str );
        return( (long)res );
        }
}

/*----------------------------------------------------------------*/
/*                     The Compiler                               */
/*----------------------------------------------------------------*/


static void Rch( CC ) /* Read next character from Pat */
{
   if ( c->PatP >= c->Patlen )
      c->Ch = EOS;
   else
      c->Ch = c->Pat[ c->PatP++ ];
}



static void Nextitem( CC ) /* Get next char from Pat; recognize ' escape char */
{
   if ( c->Ch == '\'' )
      Rch( c );
   Rch( c );
}


static void Setexits( short List, short Val, short *Aux )
{
   register short A;

   do
     {
        A = Aux[ List ];
        Aux[ List ] = Val;
        List = A;
     }
        while ( List != 0 );
}

static short Join( short A, short B, short *Aux )
{
    register short T = A;

    if ( A == 0 )
        return B;
    while ( Aux[ A ] != 0 )
        A = Aux[ A ];
    Aux[ A ] = B;
    return T;
}

static short Prim( CC )      /* Parse a Prim symbol */
{
   register short   A = c->PatP;
   register char Op = c->Ch;
   short  Exp( short A, CC );

   Nextitem( c );
   switch( Op )
     {
        case EOS:                               /* empty string after | ? */
        case ')':                               /* missing ( ? */
        case '|': c->Errflag = TRUE;            /* empty string before | ? */
        default : return A;
        case '#': Setexits( Prim( c ), A, c->Aux );
                  return A;
        case '(': A = Exp( A, c );
                  if ( c->Ch != ')' )
                        c->Errflag = TRUE;      /* missing ) ? */
                  Nextitem( c );
                  return A;
     }
}

static short Exp( short AltP, CC )    /* Parse an expression */
{
   short Exits = 0;
   register short A;

   for (;;)
        {
           A = Prim( c );
           if ( c->Ch == '|' || c->Ch == ')' || c->Ch == EOS )
              {
                Exits = Join( Exits, A, c->Aux );
                if ( c->Ch != '|' )
                   return Exits;
                c->Aux[ AltP ] = c->PatP;
                AltP = c->PatP;
                Nextitem( c );
              }
           else
              Setexits( A, c->PatP, c->Aux );
        }
}

long __asm UIK_CmplPat( register __a0 char Pattern[], register __a1 short CmplPattern[] )
{
  register short i;
  struct CM c;

    c.Pat = (char*)Pattern;
    c.Aux = (short*)CmplPattern + 1;
    c.PatP = 0;
    c.Patlen = StrLen( c.Pat );
    c.Errflag = FALSE;

    for ( i = 0; i <= c.Patlen; i++ )
       c.Aux[ i ] = 0;
    Rch( &c );
    Setexits( Exp( 0, &c ), 0, &c.Aux[0] );
    return (!c.Errflag);
}

long __asm UIK_CmplPattern( register __a0 UBYTE *pattern, register __a1 UBYTE *dest, register __d0 ULONG len )
{
  UWORD version = ((struct Library *)(*(ULONG*)4))->lib_Version;

    if (version < 37)
        {
        return( UIK_CmplPat( pattern, dest ) );
        }
    else{
        long res;
        if (version < 39) UIK_StrToUpper( pattern ); // bug Système
        *dest++ = 1; *dest++ = 1; if (len) len -= 2;
        res = ParsePatternNoCase( pattern, dest, len );
        return( res >= 0 ? 1 : 0 );
        }
}
