#ifndef UIKDEFS_MACRO

#define UIKDEFS_MACRO
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#define Alloc(n)         ((UBYTE *) AllocMem( n, MEMF_PUBLIC | MEMF_CLEAR ))
#define UIKCOLS(a,b,c,d) ((ULONG)((((UBYTE)(a))<<24)|(((UBYTE)(b))<<16)|(((UBYTE)(c))<<8)|((UBYTE)(d))))
#define SETL(a,b)       ((ULONG)((((UWORD)(a))<<16)|((UWORD)(b))))
#define W2L(a,b)        ((ULONG)((((UWORD)(a))<<16)|((UWORD)(b))))
#define B2L(a,b,c,d)    ((ULONG)((((UBYTE)(a))<<24)|(((UBYTE)(b))<<16)|(((UBYTE)(c))<<8)|((UBYTE)(d))))
#define B2W(a,b)        ((UWORD)((((UBYTE)(a))<<8)|((UBYTE)(b))))

#define UIK_nl_FreeNode( L, line ) (UIK_nl_FreeNodes( L, line, line ))

#endif
