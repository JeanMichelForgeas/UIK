/****************************************************************
 *
 *      Project:   UIK
 *      Function:  routines de gestion des liens
 *
 *      Created:   25/09/90    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikglobal.h"
#include "uiki:uikobj.h"
#include "lci:uik_protosint.h"

#include "uiki:objlist.h"
#include "uiki:objproplift.h"


/****** Imported ************************************************/

extern struct GfxBase * __far GfxBase;


/****** Exported ************************************************/


/****** Statics *************************************************/


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static int Up2Prop( obj1, obj2 )
struct UIKObj *obj1;
struct UIKObjPropGad *obj2;
{
    LONG p, rc=0;

    //UIK_LockObjVars( obj2 );
    p = obj2->LPos - 1;
    if (obj1->MouseIn && (p >= 0) && (p != obj2->LPos))
        {
        UIK_OPr_ChangePropGadget( obj2, -1, -1, p );
        rc = 1;
        }
    //UIK_UnlockObjVars( obj2 );
    return( rc );
}

static int Up2List( obj1, obj2 )
struct UIKObj *obj1;
struct UIKObjList *obj2;
{
    LONG p, rc=0;

    UIK_LockObjVars( obj2 );
    p = obj2->Pos - 1;
    if (obj1->MouseIn && (p >= 0) && (p != obj2->Pos))
        {
        obj2->Pos = p;
        UIK_OLi_ScrollListFrom( obj2, 1, -1, p );
        rc = 1;
        }
    UIK_UnlockObjVars( obj2 );
    return( rc );
}

static int Down2Prop( obj1, obj2 )
struct UIKObj *obj1;
struct UIKObjPropGad *obj2;
{
    LONG p, max, rc=0;

    //UIK_LockObjVars( obj2 );
    p = obj2->LPos + 1;
    max = obj2->LTot - obj2->LVis;
    if (obj1->MouseIn && (p <= max) && (p != obj2->LPos))
        {
        UIK_OPr_ChangePropGadget( obj2, -1, -1, p );
        rc = 1;
        }
    //UIK_UnlockObjVars( obj2 );
    return( rc );
}

static int Down2List( obj1, obj2 )
struct UIKObj *obj1;
struct UIKObjList *obj2;
{
  struct UIKList *ul = UIK_nl_ListPtr( &obj2->ListText );
  LONG p, rc=0, max;

    UIK_LockObjVars( obj2 );
    p = obj2->Pos + 1;
    max = ul->NodeNum - obj2->Vis;
    if (obj1->MouseIn && (p <= max) && (p != obj2->Pos))
        {
        obj2->Pos = p;
        UIK_OLi_ScrollListFrom( obj2, 1, 1, p );
        rc = 1;
        }
    UIK_UnlockObjVars( obj2 );
    return( rc );
}

static int Prop2List( obj1, obj2 )
struct UIKObjPropGad *obj1;
struct UIKObjList *obj2;
{
  LONG newpos, len, alen;

    //UIK_LockObjVars( obj1 );
    newpos = obj1->Pos;
    UIK_LockObjVars( obj2 );
    if (len = newpos - obj2->Pos)
        {
        obj2->Pos = newpos;
        if ((alen = (len > 0 ? len : -len)) >= obj2->Vis) UIK_OLi_DrawList( obj2 );
        else UIK_OLi_ScrollListFrom( obj2, alen, len, newpos );
        }
    UIK_UnlockObjVars( obj2 );
    //UIK_UnlockObjVars( obj1 );
    return(1);
}

static int Win2ToglGad( win, tog, class )
struct UIKObjWindow *win;
struct UIKObjGad *tog;
ULONG class;
{
    if (class == CLOSEWINDOW) UIK_OTo_UnselectToggle( tog );
    else if (class == ACTIVEWINDOW) UIK_OTo_SelectToggle( tog );
/*
    UIK_LockObjVars( tog );
    if (class == CLOSEWINDOW) tog->Gad.Flags &= ~SELECTED;
    else if (class == ACTIVEWINDOW) tog->Gad.Flags |= SELECTED;
    UIK_Refresh( tog, 1 );
    UIK_UnlockObjVars( tog );
*/
    return(1);
}

int __asm UIK_Link( register __a0 struct UIKObj *obj1, register __a1 struct UIKObj *obj2,
    register __d0 int (*func)(), register __d1 ULONG class, register __d2 ULONG parm )
{
  register UWORD id2;
  register struct UIKLink *link;

    if (!obj1 || !obj2) return(0);

    id2 = obj2->Id;
    if (! func)
        {
        switch (obj1->Id)
            {
            case UIKID_LEFTGAD:
            case UIKID_UPGAD:
                if (id2 == UIKID_PROPGAD) func = Up2Prop;
                else if (id2 == UIKID_LIST) func = Up2List;
                else goto END_ERROR;
                break;
            case UIKID_RIGHTGAD:
            case UIKID_DOWNGAD:
                if (id2 == UIKID_PROPGAD) func = Down2Prop;
                else if (id2 == UIKID_LIST) func = Down2List;
                else goto END_ERROR;
                break;
            case UIKID_PROPGAD:
                if (id2 == UIKID_LIST)
                    {
                    struct UIKList *ul = UIK_nl_ListPtr( &((struct UIKObjList *)obj2)->ListText );
                    func = Prop2List;
                    UIK_OPr_ChangePropGadget( (struct UIKObjPropGad *)obj1, ul->NodeNum, ((struct UIKObjList *)obj2)->Vis, ((struct UIKObjList *)obj2)->Pos );
                    ((struct UIKObjList *)obj2)->FeedBack = obj1;
                    }
                else goto END_ERROR;
                break;
            case UIKID_WINDOW:
                if (id2 == UIKID_TOGLGAD) func = Win2ToglGad;
                else goto END_ERROR;
                break;
            default: goto END_ERROR;
            }
        }
    if (link = (struct UIKLink *)UIK_nl_AllocNode( &obj1->Link, -1, sizeof(struct UIKLink), 0 ))
        {
        link->Vect = func;
        link->Obj  = obj1;
        link->Dest = obj2;
        link->Mask = class;
        link->Parm = parm;
        return(1);
        }

  END_ERROR:
    return(0);
}
