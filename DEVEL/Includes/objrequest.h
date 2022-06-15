#ifndef UIKDEFS_OBJREQUEST

#define UIKDEFS_OBJREQUEST
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

struct UIKObjReq
    {
    struct UIKObj       Obj;
    struct Requester    Req;
    ULONG               ReqFlags;
    struct BitMap       *CustomBitMap;
    struct BitMap       *CustomAltBitMap;
    WORD                OldLeft;
    WORD                OldTop;
    struct UIKList      ShortCutList;
    };

/*------Req flags------*/
#define UIKREQ_BITMAPED 0x0001
#define UIKREQ_CENTERED 0x0002

/*----UIKObjReq values Tags---- */
#define UIKTAG_OBJReq_CustomBitMap       (UIKTAG_USER|0x0001)
#define UIKTAG_OBJReq_CustomAltBitMap    (UIKTAG_USER|0x0002)

/*----UIKObjReq ReqFlags Tags---- */
#define UIKTAG_OBJReq_Flags              (UIKTAG_USER|0x0020)

#endif /* UIKDEFS_OBJREQUEST */
