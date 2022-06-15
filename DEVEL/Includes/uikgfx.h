#ifndef UIK_GFX_H

#define UIK_GFX_H
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

struct UIKBitMap {
    APTR    Mask;
    UWORD   UFlags;
    UWORD   Width;
    UWORD   BytesPerRow;
    UWORD   Rows;
    UBYTE   Flags;
    UBYTE   Depth;
    UWORD   pad;
    APTR    Planes[8];
    };

#define UIK2BM(ubm) ((struct BitMap *)(((ULONG)ubm)+8))
#define BM2UIK(bm) ((struct UIKBitMap *)(((ULONG)bm)-8))

#define UIKBMAP_CONTIG  0x0001
#define UIKBMAP_MASKED  0x0002
#define UIKBMAP_PUBLIC  0x0004
#define UIKBMAP_BWSYS1  0x0008
#define UIKBMAP_NOPLANE 0x0010

#define UIKBMAP_INVDONE 0x8000

#endif
