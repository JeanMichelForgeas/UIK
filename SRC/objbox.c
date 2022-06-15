/****************************************************************
 *
 *      Project:   UIK
 *      Function:  box
 *
 *      Created:   12/05/90    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikglobal.h"
#include "uiki:uikobj.h"
#include "uiki:uikbox.h"
#include "uiki:uikdrawauto.h"
#include "uiki:uikcolors.h"
#include "lci:uik_protosint.h"


/****** Macros **************************************************/


/****** Imported ************************************************/

extern void IntUIK_DrawBox();

extern struct TagItem __far titleflags_boolmap[];

extern UWORD __far AutoDrawFrame[];


/****** Exported ************************************************/


/****** Statics *************************************************/


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static int AddBox( struct UIKObj *obj, struct TagItem *taglist )
{
    *((ULONG*)&obj->ActBoxPen) = UIK_GetTagData( UIKTAG_OBJ_ActInactPens, UIKCOL_GREY<<24 | UIKCOL_GREY<<16 | UIKCOL_BLACK<<8 | UIKCOL_BLACK, taglist );
    return(1);
}

struct UIKObjGerm __far BoxGerm =
    {
    "UIKObj_Box",
    AddBox,                     // AddVector
    NULL,                       // StartVector
    NULL,                       // StopVector
    NULL,                       // RemoveVector
    IntUIK_DrawBox,             // RefreshVector
    NULL,                       // ResizeVector
    NULL,                       // ExceptActVector
    NULL,                       // ExceptInaVector
    NULL,                       // ExceptMovVector
    NULL,                       // ExceptKeyVector
    0,0,0,0,
    4,2,
    (40<<16)|18,
    UIKID_BOX,
    sizeof(struct UIKObjBox),
    BOX_DISPLAYABLE|BOX_INNERZONE|BOX_OVER|BOX_TRANSLATELEFTTOP,
    };

static int AddFrame( struct UIKObj *obj, struct TagItem *taglist )
{
  ULONG flags;

    *((ULONG*)&obj->ActBoxPen) = UIK_GetTagData( UIKTAG_OBJ_ActInactPens, UIKCOL_GREY<<24 | UIKCOL_GREY<<16 | UIKCOL_BLACK<<8 | UIKCOL_BLACK, taglist );
    obj->TitleFlags = UIK_GetTagData( UIKTAG_OBJ_Title_Flags, TITLE_TOP, taglist );
    flags = UIK_PackBoolTags( 0, taglist, titleflags_boolmap );
    if (flags & (TITLE_INSLEFT|TITLE_INSRIGHT|TITLE_ABOVE|TITLE_TOP|TITLE_LEFT|TITLE_RIGHT|TITLE_CENTER))
        obj->TitleFlags &= ~(TITLE_INSLEFT|TITLE_INSRIGHT|TITLE_ABOVE|TITLE_TOP|TITLE_LEFT|TITLE_RIGHT|TITLE_CENTER);
    obj->TitleFlags |= flags;
    obj->Automate = obj->AltAutomate = AutoDrawFrame;
    return(1);
}

struct UIKObjGerm __far FrameGerm =
    {
    "UIKObj_Frame",
    AddFrame,                   // AddVector
    NULL,                       // StartVector
    NULL,                       // StopVector
    NULL,                       // RemoveVector
    IntUIK_DrawBox,             // RefreshVector
    NULL,                       // ResizeVector
    NULL,                       // ExceptActVector
    NULL,                       // ExceptInaVector
    NULL,                       // ExceptMovVector
    NULL,                       // ExceptKeyVector
    0,0,0,0,
    4,2,
    (60<<16)|28,
    UIKID_BOX,
    sizeof(struct UIKObjBox),
    BOX_DISPLAYABLE|BOX_INNERZONE|BOX_FRAME|BOX_TRANSLATELEFTTOP,
    };
