#ifndef UIKDEFS_OBJMEMORY

#define UIKDEFS_OBJMEMORY
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

struct UIKObjMem
    {
    struct UIKObj       Obj;
    UBYTE               *MemPtr;
    void                (*CleanupFunc)();
    };

/*----UIKObjMem values Tags---- */
#define UIKTAG_OBJMem_MemSize       (UIKTAG_USER|0x0001)
#define UIKTAG_OBJMem_MemType       (UIKTAG_USER|0x0002)
#define UIKTAG_OBJMem_UserCleanup   (UIKTAG_USER|0x0003)

#endif /* UIKDEFS_OBJMEMORY */
