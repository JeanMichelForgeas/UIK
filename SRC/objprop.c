/****************************************************************
 *
 *      Project:   UIK
 *      Function:  propgadget
 *
 *      Created:   19/09/90    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikglobal.h"
#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikbox.h"
#include "uiki:uikgfx.h"
#include "uiki:uikcolors.h"
#include "lci:uik_protosint.h"

#include "uiki:objproplift.h"


/****** Macros **************************************************/


/****** Imported ************************************************/

extern void HdlUIK_ActivePropGad(), HdlUIK_InactivePropGad();
extern void HdlUIK_MovePropGad();
extern void IntUIK_DrawBox(), StopUIKGadget();
extern int StdGadInit();

extern struct UIKBase * __far UIKBase;
extern struct GfxBase * __far GfxBase;
extern struct IntuitionBase * __far IntuitionBase;

extern UWORD __far AutoDrawBox_Over[];
extern UWORD __far AutoDrawBox_Light[];
extern UWORD __far AutoDrawProp_Over[];
extern UWORD __far AutoDrawProp_Under[];

extern struct UIKBitMap __far UBM_Prop1;
extern struct UIKBitMap __far UBM_Prop1Sp;
extern struct UIKBitMap __far UBM_Prop2;
extern struct UIKBitMap __far UBM_Prop2Sp;


/****** Exported ************************************************/


/****** Statics *************************************************/


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void IntUIK_Longs2Words( struct UIKObjPropGad *gobj )
{
  ULONG val;

    if (gobj->LTot > 0xffffL)
        {
        gobj->Tot = 0xffff;
        val = gobj->LVis / (gobj->LTot / 0xffff); gobj->Vis = (val > 0xffff) ? 0xffff : val;
        val = gobj->LPos / (gobj->LTot / 0xffff); gobj->Pos = (val > 0xffff) ? 0xffff : val;
        if (gobj->Vis == 0) gobj->Vis = 1;
        }
    else{
        gobj->Tot = (UWORD)gobj->LTot;
        gobj->Vis = (UWORD)gobj->LVis;
        gobj->Pos = (UWORD)gobj->LPos;
        }
}

void IntUIK_SetPropPos( struct UIKObjPropGad *gobj ) // Objet déjà locké quand appelée
{
  ULONG OffPC;

    if (gobj->KnobOffs + gobj->KnobWidth < gobj->ContWidth)
        {
        gobj->LPos = (gobj->KnobOffs * (gobj->LTot - gobj->LVis + 1)) / (gobj->ContWidth - gobj->KnobWidth + 1);
        }
    else gobj->LPos = gobj->LTot - gobj->LVis;

    gobj->OldLPos = gobj->LPos;
    IntUIK_Longs2Words( gobj );
}

void IntUIK_SetPropVars( struct UIKObjPropGad *gobj )
{
  register ULONG tv, inf;

    if (! gobj->LVis) { gobj->LVis = 0xffffffff; }
    if (gobj->LTot > gobj->LVis)
        {
        tv = gobj->LTot - gobj->LVis;
        if (gobj->LPos > tv) gobj->LPos = tv;

        IntUIK_Longs2Words( gobj );

        gobj->VisPC = 0; // pour routine assembleur

        gobj->KnobWidth = (gobj->LVis * gobj->ContWidth) / (gobj->LTot ? gobj->LTot : 1);
        inf = ((UIKBase->UIKPrefs->PrefsFlags & UIKF_SPLOOK) ? 1 : 0) + (gobj->Ver ? 7 : 9);
        if (gobj->KnobWidth < inf) gobj->KnobWidth = inf;
        gobj->KnobOffs = (gobj->LPos * (gobj->ContWidth - gobj->KnobWidth)) / tv;
        }
    else{
        gobj->VisPC = 0xffff; // pour routine assembleur
        gobj->LPos = 0;
        gobj->KnobOffs = 0;
        gobj->KnobWidth = gobj->ContWidth;

        IntUIK_Longs2Words( gobj );
        }
}

void __asm UIK_OPr_ChangePropNoDraw( register __a0 struct UIKObjPropGad *obj,
register __a1 int Total, register __d0 int Visible, register __d1 int Position )
{
    UIK_LockObjVars( obj );
    if (Total != -1) obj->LTot = Total;
    if (Visible != -1) obj->LVis = Visible;
    if (! obj->Moving)
        if (Position != -1) { obj->OldLPos = obj->LPos; obj->LPos = Position; }

    IntUIK_Longs2Words( obj );

    if (! obj->Moving)
        IntUIK_SetPropVars( obj );
    UIK_UnlockObjVars( obj );
}

void __asm UIK_OPr_ChangePropGadget( register __a0 struct UIKObjPropGad *obj,
register __a1 int Total, register __d0 int Visible, register __d1 int Position )
{
    UIK_LockObjVars( obj );
    if (Total != -1) obj->LTot = Total;
    if (Visible != -1) obj->LVis = Visible;
    if (! obj->Moving)
        if (Position != -1) { obj->OldLPos = obj->LPos; obj->LPos = Position; }

    IntUIK_Longs2Words( obj );

    if (! obj->Moving)
        {
        IntUIK_SetPropVars( obj );
        IntUIK_DrawKnob( obj );
        }
    UIK_UnlockObjVars( obj );
}

void PropGadRefresh( obj )
struct UIKObjPropGad *obj;
{
    if (!obj->Obj.Status)
        {
        UIK_LockObjVars( obj );
        IntUIK_DrawBox( obj );
        *((double *)obj->SaveKnob) = 0;      // simule la première fois pour redessiner le knob
        IntUIK_DrawKnob( obj );
        UIK_UnlockObjVars( obj );
        }
}

void PropGadResize( obj )
struct UIKObjPropGad *obj;
{
    // quand resize, plus de gadget dans la fenetre donc le handler
    // ne peut plus etre activé donc pas besoin de lock sur l'objet

    obj->SaveKnob[0] = obj->SaveKnob[1] = obj->SaveKnob[2] = obj->SaveKnob[3] = 0; // == première fois
    StdGadResize( obj );
    if (obj->Ver) obj->ContWidth = obj->Gad.Height - 2;
    else obj->ContWidth = obj->Gad.Width - 4;
    IntUIK_SetPropVars( obj );
}

static int AddPropLift( struct UIKObjPropGad *gobj, struct TagItem *taglist )
{
    *((ULONG*)&gobj->Obj.ActBoxPen) = UIK_GetTagData( UIKTAG_OBJ_ActInactPens, UIKCOL_GREY<<24 | UIKCOL_GREY<<16 | UIKCOL_BLACK<<8 | UIKCOL_BLACK, taglist );
    gobj->Gad.LeftEdge = gobj->Obj.Box.Left;
    gobj->Gad.TopEdge  = gobj->Obj.Box.Top;
    gobj->Gad.Width    = gobj->Obj.Box.Width;
    gobj->Gad.Height   = gobj->Obj.Box.Height;
    gobj->Gad.UserData = (APTR) gobj;
    gobj->Gad.Flags      = GADGHNONE;
    gobj->Gad.Activation = FOLLOWMOUSE | RELVERIFY | GADGIMMEDIATE;
    gobj->Gad.GadgetType = BOOLGADGET;

    //gobj->Obj.Automate    = (UWORD*) UIK_GetTagData( UIKTAG_OBJ_BoxAutomate, (ULONG) AutoDrawProp_Over, taglist );
    //gobj->Obj.AltAutomate = (UWORD*) UIK_GetTagData( UIKTAG_OBJ_BoxAltAutomate, (ULONG) AutoDrawProp_Under, taglist );
    gobj->KnobNorAutomate = (UBYTE*)UIK_GetTagData( UIKTAG_OBJProp_KnobAutomate, (ULONG)AutoDrawBox_Over, taglist );
    gobj->KnobAltAutomate = (UBYTE*)UIK_GetTagData( UIKTAG_OBJProp_KnobAltAutomate, (ULONG)AutoDrawBox_Light, taglist );
    gobj->KnobAutomate = gobj->KnobNorAutomate;

    gobj->KnobImage1 = (APTR)UIK_GetTagData( UIKTAG_OBJProp_KnobImage1, (ULONG) &UBM_Prop1, taglist );
    gobj->KnobSpecialImage1 = (APTR)UIK_GetTagData( UIKTAG_OBJProp_KnobSpImage1, (ULONG) &UBM_Prop1Sp, taglist );
    gobj->KnobImage2 = (APTR)UIK_GetTagData( UIKTAG_OBJProp_KnobImage2, (ULONG) &UBM_Prop2, taglist );
    gobj->KnobSpecialImage2 = (APTR)UIK_GetTagData( UIKTAG_OBJProp_KnobSpImage2, (ULONG) &UBM_Prop2Sp, taglist );

    gobj->LTot = UIK_GetTagData( UIKTAG_OBJProp_Total, 1, taglist );
    gobj->LVis = UIK_GetTagData( UIKTAG_OBJProp_Visible, 1, taglist );
    gobj->LPos = UIK_GetTagData( UIKTAG_OBJProp_Position, 0, taglist );
    IntUIK_Longs2Words( gobj );

    gobj->Ver = UIK_GetTagData( UIKTAG_OBJProp_Vertical, 0, taglist );
    if (gobj->Ver) gobj->ContWidth = gobj->Gad.Height - 2;
    else gobj->ContWidth = gobj->Gad.Width - 4;
    IntUIK_SetPropVars( gobj );

    return(1);
}

static void StopProp( struct UIKObjPropGad *prop )
{
    prop->SaveKnob[0] = 0; prop->SaveKnob[1] = 0; prop->SaveKnob[2] = 0; prop->SaveKnob[3] = 0;
    prop->KnobAutomate = prop->KnobNorAutomate;
    StopUIKGadget( prop );
}

static int SelectPropGad( struct UIKObjPropGad *obj, ULONG class )
{
    if (obj)
        if (class & (INTUITICKS|GADGETDOWN))
            {
            ULONG pos = obj->LPos;
            if (obj->Obj.UIK->IptButs & (VBJOYF_FORW|VBJOYF_LEFT))
                { if (pos >= obj->LVis) pos -= obj->LVis; else pos = 0; }
            else if (obj->Obj.UIK->IptButs & (VBJOYF_BACK|VBJOYF_RIGHT))
                { pos += obj->LVis; }
            else return(1);
            UIK_OPr_ChangePropGadget( obj, -1, -1, pos );
            }
    return(1);
}

static struct TagItem __far GermTagList[] =
    {
    UIKTAG_GEN_BoxAutomate,  (ULONG) AutoDrawProp_Over,
    UIKTAG_GEN_BoxAltAutomate,  (ULONG) AutoDrawProp_Under,
    TAG_END
    };

struct UIKObjGerm __far PropLiftGerm =
    {
    "UIKObj_PropLift",
    AddPropLift,                // AddVector
    StdGadInit,                 // StartVector
    StopProp,                   // StopVector
    NULL,                       // RemoveVector
    PropGadRefresh,             // RefreshVector
    PropGadResize,              // ResizeVector
    HdlUIK_ActivePropGad,       // ExceptActVector
    HdlUIK_InactivePropGad,     // ExceptInaVector
    HdlUIK_MovePropGad,         // ExceptMovVector
    NULL,                       // ExceptKeyVector
    0,                          // StartAfterVector
    SelectPropGad,              // SelectVector
    0,                          // ExtentedVectors
    GermTagList,                // GermTagList
    11,6,
    (60<<16)|13,
    UIKID_PROPGAD,
    sizeof(struct UIKObjPropGad),
    BOX_EXTGADUP|BOX_DISPLAYABLE|BOX_INNERZONE|BOX_OVER | BOX_SEMAPHORE | BOX_TRANSLATELEFTTOP | BOX_ZONEABLE,
    };
