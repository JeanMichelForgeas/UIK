#ifndef UIKDEFS_LIST

#define UIKDEFS_LIST
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

struct UIKNode {    /* sizeof() must be a multiple of 4 */
    UBYTE    *Buf;   /* pointer to data                  */
    UWORD    Reserved;
    UWORD    Flags;
    };

struct UIKList {
    ULONG           NodeNum, MaxNodes; /* number of actual/max      */
    ULONG           BlockSize;         /* size increment            */
    struct UIKNode  *UNode;            /* pointers to UIKNode table */
    };

struct UIKPList {
    ULONG           NodeNum, MaxNodes; /* number of actual/max      */
    ULONG           BlockSize;         /* size increment            */
    struct UIKNode  *UNode;            /* pointers to UIKNode table */
    UWORD           Flags;
    UWORD           Reserved1;
    APTR            Reserved2;
    };

#define UIKPLB_POOLOWNER    0
#define UIKPLF_POOLOWNER    (1L<<UIKPLB_POOLOWNER)

#endif
