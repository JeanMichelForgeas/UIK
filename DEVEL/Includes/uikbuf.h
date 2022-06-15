#ifndef UIKDEFS_BUFFER

#define UIKDEFS_BUFFER
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif


struct UIKBuffer {
    ULONG   TotSize;
    ULONG   CurSize;
    ULONG   MemReqs;
    UWORD   BlkSize;
    UWORD   UserUWord;
    ULONG   UserULong;
    };

struct UIKPBuffer {
    APTR    Pool;
    ULONG   TotSize;
    ULONG   CurSize;
    ULONG   MemReqs;
    UWORD   BlkSize;
    UWORD   UserUWord;
    ULONG   UserULong;
    };

typedef UBYTE UIKBUF;

#endif
