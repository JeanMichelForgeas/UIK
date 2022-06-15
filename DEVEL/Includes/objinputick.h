#ifndef UIKDEFS_INPUTICK

#define UIKDEFS_INPUTICK
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

struct UIKObjInputick
    {
    struct UIKObj           Obj;
    struct Library          *DOSBase;
    struct Library          *UIKBase;
    void                    (*ClientFunc)();

    struct UIKHook          *EventHook;
    void                    (*EventVect)();
    };


/* fonctions publiques
 */
#define OBJFUNC_Inputick_


/*---------Obj Inputick Tags---------- */
#define UIKTAG_Inputick_EventFunc  (UIKTAG_USER|0x0001)

/*---------Obj Inputick Flags---------- */

/*-- */

#endif
