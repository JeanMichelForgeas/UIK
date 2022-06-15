#ifndef UIKDEFS_HELP

#define UIKDEFS_HELP
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

#ifndef UIKDEFS_TAGITEM_H
#include "uiki:uiktag.h"
#endif


struct UIKObjHelp
    {
    struct UIKObj       Obj;
    struct UIKObjList   *ObjList;
    struct UIKHook      *KeyHook;
    struct UIKBase      *UIKBase;
    };

#define HELP_BOX_STDWIDTH 408
#define HELP_BOX_STDHEIGHT 80

/*---------Obj help Tags---------- */
#define UIKTAG_Help_TextPtr      (UIKTAG_USER|0x0003)

/*---------Obj help Flags---------- */

/*-- */

#endif
