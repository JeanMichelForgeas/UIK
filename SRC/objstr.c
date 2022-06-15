/****************************************************************
 *
 *      Project:   UIK
 *      Function:  Gestion de l'objet StringGadget
 *
 *      Created:   30/05/90    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikmacros.h"
#include "uiki:uikglobal.h"
#include "lci:uik_protosint.h"

#include "uiki:objstring.h"


/****** Imported ************************************************/

extern void IntUIK_DrawBox();
extern int StdGadInit();
extern void StdGadResize();
extern void StopUIKGadget();
extern void Hdl_ActivateKeyTank();
extern void Hdl_InactivateKeyTank();
extern void HdlUIK_ActivateStrGad();
extern void HdlUIK_NewKey2StrGad();
extern void HdlUIK_DigitalInt();
extern void HdlUIK_DigitalHex();

extern struct LayersBase * __far LayersBase;
extern struct GfxBase * __far GfxBase;
extern struct UIKBase * __far UIKBase;
extern struct DOSBase * __far DOSBase;

extern struct TagItem __far stdboxflags_boolmap[];

extern UWORD __far AutoDrawString_Under[];
extern UWORD __far AutoDrawStringSys_Under[];


/****** Exported ************************************************/


/****** Statics *************************************************/

static struct TagItem __far flags_boolmap[] =
    {
    { UIKTAG_OBJStrFl_CenLeft,     UIKST_LEFT       },
    { UIKTAG_OBJStrFl_CenJustify,  UIKST_JUSTIFY    },
    { UIKTAG_OBJStrFl_CenCenter,   UIKST_CENTER     },
    { UIKTAG_OBJStrFl_CenRight,    UIKST_RIGHT      },
    { UIKTAG_OBJStrFl_AutoHeight,  UIKST_AUTOHEIGHT },
    { UIKTAG_OBJStrFl_DigitalInt,  UIKST_DIGITALINT },
    { UIKTAG_OBJStrFl_DigitalHex,  UIKST_DIGITALHEX },
    { UIKTAG_OBJStrFl_Activate,    UIKST_ACTIVATE   },
    { UIKTAG_OBJStrFl_NoFilter,    UIKST_NOFILTER   },
    { TAG_DONE }
    };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void RemoveObjString( struct UIKObjStrGad *sobj )
{
    BufFree( sobj->Buffer ); sobj->Buffer = 0;
    BufFree( sobj->UndoBuffer ); sobj->UndoBuffer = 0;
}

static void StopObjString( struct UIKObjStrGad *sobj )
{
    UIK_InactivateKeyTank( sobj );
    StopUIKGadget( sobj );
}

static int StartObjString( struct UIKObjStrGad *sobj )
{
    StdGadInit( sobj );
    if (sobj->StrFlags & UIKST_ACTIVATE) UIK_ActivateKeyTank( sobj );
    return(1);
}

static void RefreshObjString( struct UIKObjStrGad *sobj )
{
    IntUIK_DrawBox( sobj );
    IntUIK_DrawObjStrText( sobj );
}
/*
static void ResizeObjString( struct UIKObjStrGad *sobj )
{
  struct Rectangle *rect = &sobj->Obj.ObjExt->InnerRect;

    sobj->Gad.LeftEdge = rect->MinX;
    sobj->Gad.TopEdge  = rect->MinY;
    sobj->Gad.Width    = rect->MaxX - rect->MinX + 1;
    sobj->Gad.Height   = rect->MaxY - rect->MinY + 1;
}
*/

static int AddObjString( struct UIKObjStrGad *sobj, struct TagItem *taglist )
{
  ULONG initstr;
  struct TextFont *font;

    //*((ULONG*)&sobj->Obj.ActBoxPen) = UIK_GetTagData( UIKTAG_OBJ_ActInactPens, UIKCOL_WHITE<<24 | UIKCOL_WHITE<<16 | UIKCOL_BLACK<<8 | UIKCOL_BLACK, taglist );

    *((ULONG*)&sobj->ActTextPen) = UIK_GetTagData( UIKTAG_OBJStr_TextPens, UIKCOL_BLACK<<24 | UIKCOL_BLACK<<16 | UIKCOL_WHITE<<8 | UIKCOL_WHITE, taglist );
    if (UIKBase->UIKPrefs->PrefsFlags & UIKF_SYSLOOK)
        {
        *((ULONG*)&sobj->ActTextPen) = UIK_GetTagData( UIKTAG_OBJStr_TextPens, UIKCOL_BLACK<<24 | UIKCOL_BLACK<<16 | UIKCOL_GREY<<8 | UIKCOL_GREY, taglist );
        //sobj->Obj.ActBoxPen = sobj->Obj.InactBoxPen = UIKCOL_GREY;
        }
    sobj->UserHook = UIK_GetTagData( UIKTAG_OBJStr_UserHandler, 0, taglist );
    sobj->LongIntResult = UIK_GetTagData( UIKTAG_OBJStr_ResultPointer, 0, taglist );
    sobj->StrFlags = UIK_GetTagData( UIKTAG_OBJStr_Flags, UIKST_LEFT | UIKST_AUTOHEIGHT, taglist );
    sobj->MaxChars = UIK_GetTagData( UIKTAG_OBJStr_MaxChars, 0, taglist );
    sobj->ActiveFunc = UIK_GetTagData( UIKTAG_OBJStr_ActiveFunc, 0, taglist );
    sobj->InactiveFunc = UIK_GetTagData( UIKTAG_OBJStr_InactiveFunc, 0, taglist );
    sobj->StrFlags = UIK_PackBoolTags( sobj->StrFlags, taglist, flags_boolmap );

    if (sobj->StrFlags & UIKST_AUTOHEIGHT) sobj->Obj.Box.Height = 6;
    if (sobj->StrFlags & UIKST_DIGITALINT) sobj->UserHook = (ULONG) HdlUIK_DigitalInt;
    if (sobj->StrFlags & UIKST_DIGITALHEX) sobj->UserHook = (ULONG) HdlUIK_DigitalHex;
    if (sobj->StrFlags & UIKST_ACTIVATE)   sobj->Obj.Flags &= ~BOX_OVER;

    sobj->Gad.UserData   = (APTR) sobj;
    sobj->Gad.Flags      = GADGHNONE;
    sobj->Gad.Activation = FOLLOWMOUSE | RELVERIFY | GADGIMMEDIATE;
    sobj->Gad.GadgetType = BOOLGADGET;

    font = sobj->Obj.ObjExt->Font;
    if (sobj->StrFlags & UIKST_AUTOHEIGHT)
        {
        sobj->Gad.Height = sobj->Obj.Box.Height = font->tf_YSize + 5;
        sobj->YOffset = font->tf_Baseline + 1;
        }
    else{
        WORD y = sobj->Obj.ObjExt->InnerRect.MinY, h = sobj->Obj.ObjExt->InnerRect.MaxY - sobj->Obj.ObjExt->InnerRect.MinY + 1;
        sobj->YOffset = ((h - font->tf_YSize) / 2) + font->tf_Baseline;
        }
    //ResizeObjString( sobj );
    StdGadResize( sobj );


    if (sobj->Buffer = BufAlloc( 16, 0, MEMF_ANY|MEMF_CLEAR ))
        {
        UBYTE *str;
        if ((initstr = UIK_GetTagData( UIKTAG_OBJStr_TextBuffer, -1, taglist )) != -1)
            {
            str = UIK_LangString( sobj, initstr );
            }
        else{
            str = (UBYTE *) UIK_GetTagData( UIKTAG_OBJStr_TextPointer, 0, taglist );
            }
        if (str)
            {
            if (! UIK_OSt_Set( sobj, str, StrLen( str ) ))
                return(0);
            }
        }
    else return(0);
    return(1);
}

static int SelectStrGad( struct UIKObjStrGad *obj, ULONG class )
{
    if (obj && (class & GADGETDOWN)) // une seule fois
        {
        UIK_ActivateKeyTank( obj );
        }
    return(1);
}

static struct TagItem __far GermSysTagList[] =
    {
    UIKTAG_GEN_ActInactPens,    UIKCOLS(UIKCOL_GREY,UIKCOL_GREY,UIKCOL_BLACK,UIKCOL_BLACK),
    UIKTAG_GEN_BoxAutomate,     (ULONG) AutoDrawStringSys_Under,
    UIKTAG_GEN_BoxAltAutomate,  (ULONG) AutoDrawStringSys_Under,
    UIKTAG_GEN_ActKeyTank,      (ULONG) Hdl_ActivateKeyTank,
    UIKTAG_GEN_InactKeyTank,    (ULONG) Hdl_InactivateKeyTank,
    TAG_END
    };
static struct TagItem __far GermTagList[] =
    {
    UIKTAG_GEN_ActInactPens,        UIKCOLS(UIKCOL_WHITE,UIKCOL_WHITE,UIKCOL_BLACK,UIKCOL_BLACK),
    UIKTAG_GEN_BoxAutomate,         (ULONG) AutoDrawString_Under,
    UIKTAG_GEN_BoxAltAutomate,      (ULONG) AutoDrawString_Under,
    UIKTAG_GEN_ActKeyTank,          (ULONG) Hdl_ActivateKeyTank,
    UIKTAG_GEN_InactKeyTank,        (ULONG) Hdl_InactivateKeyTank,
    UIKTAG_GEN_SysLookGenTagList,   (ULONG) GermSysTagList,
    TAG_END
    };

struct UIKObjGerm __far StringGerm =
    {
    "UIKObj_String",
    AddObjString,               // AddVector
    StartObjString,             // StartVector
    StopObjString,              // StopVector
    RemoveObjString,            // RemoveVector
    RefreshObjString,           // RefreshVector
    StdGadResize,               // ResizeVector
    HdlUIK_ActivateStrGad,      // ExceptActVector
    NULL,                       // ExceptInaVector
    NULL,                       // ExceptMovVector
    HdlUIK_NewKey2StrGad,       // ExceptKeyVector
    0,                          // StartAfterVector
    SelectStrGad,               // SelectVector
    0,                          // ExtentedVectors
    GermTagList,                // GermTagList
    6,6,
    (60<<16)|12,
    UIKID_STRGAD,
    sizeof(struct UIKObjStrGad),
    BOX_DISPLAYABLE|BOX_INNERZONE|BOX_SEMAPHORE | BOX_OVER | BOX_FILLED | BOX_CLIPPED | BOX_TRANSLATELEFTTOP /*| BOX_ZONEABLE*/,
    };
