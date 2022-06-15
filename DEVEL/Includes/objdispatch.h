#ifndef UIKDEFS_DISPATCH

#define UIKDEFS_DISPATCH
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

#ifndef UIKDEFS_TAGITEM_H
#include "uiki:uiktag.h"
#endif

struct PrEntry
    {
    APTR  parm;
    ULONG pri;
    };

struct Processing
    {
    struct Node PNode;
    ULONG       (*ProcFunc)();
    APTR        Parm;
    };

struct UIKObjDispatch
    {
    struct UIKObj           Obj;
    struct Library          *DOSBase;
    struct Library          *UIKBase;

    ULONG                   SigBit;
    struct UIKHook          *EventHook;
    void                    (*EventVect)();

    struct List             ProcList;
    };


/* fonctions publiques
 */

#define OBJFUNC_Dispatch_AddProcessing      0
#define OBJFUNC_Dispatch_RemProcessing      4

#define OBJFUNC_Dispatch_LastCmd            4


/*---------Obj Dispatch Tags---------- */
#define UIKTAG_Dispatch_    (UIKTAG_USER|0x0001) /*  */

/*---------Obj Dispatch Flags---------- */

/*-- */

#endif
