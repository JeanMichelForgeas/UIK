/****************************************************************
 *
 *      Project:   UIKObj_Palette
 *      Function:  Objet Palette
 *
 *      Object Creation:   20/05/91    Jean-Michel Forgeas
 *      Avec la permission de Frédéric DENIS:
 *
 *      Code from: Palette Editor for UIK Prefs file.
 *      Created 24/9/91 by Frédéric DENIS, The Software Laboratory
 *      All rights reserved.
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikbase.h"
#include "uiki:uikglobal.h"
#include "uiki:uikobj.h"
#include "uiki:uikbox.h"
#include "uiki:uikmacros.h"
#include "uiki:uikcolors.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objwindow.h"
#include "uiki:objproplift.h"

#include "lci:vars.h"
#include "lci:objpalette.h"


/****** Macros **************************************************/


/****** Imported ************************************************/

extern VARS __near V; /* magic workaround */

extern struct GfxBase * __far GfxBase;
extern struct DOSBase * __far DOSBase;
extern struct UIKBase * __far UIKBase;

extern UBYTE * __far STitle;

/****** Exported ************************************************/


/****** Statics *************************************************/

static UWORD __far PatProp[] = { 0xAAAA, 0x5555 };

static struct TTextAttr __far FrameTTA[] = { 0, 0, FSF_ITALIC, 0, 0 };

static UBYTE * __far WTitle = "Palette";


/*****************************************************************************
 *
 *   Definition de l'objet prive
 *
 *****************************************************************************/

struct UIKObjPaletteDispl
    {
    struct UIKObj     Obj;
    struct UIKObj     *ColGads[32];
    struct UIKHook    *GadsHooks[32];
    struct UIKPrefs   UIKPrefs;
    UWORD             Color;
    };

static UBYTE __far NrGadX[] = { 1, 2, 2, 2, 4 };
static UBYTE __far NrGadY[] = { 2, 2, 4, 8, 8 };

static int CreateColGads( struct UIKObjPaletteDispl *opd )
{
  UWORD XSize, YSize;
  UWORD XNum, YNum;
  UWORD StartX = opd->Obj.Box.Left;
  UWORD XPos = 0, YPos = opd->Obj.Box.Top;
  UBYTE GadNum = 0;
  UWORD i, j, depth;
  struct TagItem ColGadTL[] = {
     { UIKTAG_OBJ_LeftTop,         SETL(0,0) },
     { UIKTAG_OBJ_WidthHeight,     SETL(0,0) },
     { UIKTAG_OBJ_BoxFl_Over,      FALSE },
     { UIKTAG_OBJ_UserValue1,      0 },
     { UIKTAG_OBJ_ShortCutKey,     0 },
     { TAG_END } };

    depth = MIN(5,opd->UIKPrefs.DefScreen_Depth);
    XNum = NrGadX[ (depth - 1)];
    YNum = NrGadY[ (depth - 1)];
    XSize = (opd->Obj.Box.Width - (XNum - 1)) / XNum;
    YSize = (opd->Obj.Box.Height - (YNum - 1)) / YNum;
    ColGadTL[1].ti_Data = SETL(XSize,YSize);

    for(j = 0; j<YNum; j++)
        {
        XPos = StartX;
        for(i = 0; i<XNum; i++)
            {
            ColGadTL[0].ti_Data = SETL(XPos,YPos);
            ColGadTL[3].ti_Data = GadNum;
            ColGadTL[4].ti_Data = B2L(0x30+GadNum,0,0,0);
            XPos += XSize + 1;
            if (opd->ColGads[GadNum] = UIK_AddObject( "UIKObj_MutualExclude", opd, ColGadTL ))
                {
                ColGadTL[2].ti_Data = TRUE;
                /*Links*/
                }
            else{
                return(0);
                }
            GadNum++;
            }
        YPos += YSize + 1;
        }
    return(1);
}

static void RedrawColBoxes( struct UIKObjPaletteDispl *opd )
{
  UWORD i, depth = MIN(5,opd->UIKPrefs.DefScreen_Depth);
  UWORD maxi = 1 << depth;
  struct UIKObj *Gad;
  struct RastPort *rp;

    for (i = 0; i < maxi; i++)
        {
        Gad = opd->ColGads[i];
        rp = Gad->rp;
        SetAPen( rp, i);
        RectFill( rp,
            Gad->Box.Left + 4,
            Gad->Box.Top + 2,
            Gad->Box.Left + Gad->Box.Width - 5,
            Gad->Box.Top + Gad->Box.Height - 3 );
        }
}

static int AddPalDispl( struct UIKObjPaletteDispl *opd )
{
    CopyMem( &V.Prefs, &opd->UIKPrefs, sizeof(struct UIKPrefs) );
    if (! CreateColGads( opd)) return (0);
    return(1);
}

static void RemovePalDispl( struct UIKObjPaletteDispl *opd )
{
}

static void StopPalDispl( struct UIKObjPaletteDispl *opd )
{
  UWORD i, depth = MIN(5,opd->UIKPrefs.DefScreen_Depth);
  UWORD maxi = 1 << depth;

    for (i = 0; i < maxi; i++)
        UIK_RemoveVector( opd->ColGads[i], opd->GadsHooks[i] );
}

static void RefreshPalDispl( struct UIKObjPaletteDispl *opd )
{
}

static void GadgetDown( struct UIKObj *gad, struct UIKObjPaletteDispl *opd)
{
    opd->Color = gad->UserULong1;
    UIK_CauseObjHook( opd, GADGETDOWN);
}

static int StartAfterPalDispl( struct UIKObjPaletteDispl *opd )
{
  UWORD i, depth = MIN(5,opd->UIKPrefs.DefScreen_Depth);
  UWORD maxi = 1 << depth;
  struct UIKObj *Gad;

    RedrawColBoxes( opd );

    for (i = 0; i < maxi; i++)
        {
        Gad = opd->ColGads[i];
        if (! (opd->GadsHooks[i] = UIK_AddVector( Gad, (void *)GadgetDown, GADGETDOWN, (ULONG)opd))) return(0);
        }

    return( 1 );
}

static struct UIKObjGerm __far PalDisplGerm =
{
    "UIKObj_PaletteDisp",
    AddPalDispl,
    NULL,
    StopPalDispl,
    RemovePalDispl,
    RefreshPalDispl,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    StartAfterPalDispl,
    NULL,
    NULL,
    NULL,
    71,
    87,
    71<<16|87,
    0,
    sizeof(struct UIKObjPaletteDispl),
    BOX_SEMAPHORE,
};

static struct UIKObjRegGerm __far ObjPalDisplRegGerm = { &PalDisplGerm, 0, 0, 0 };

/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static int lnk_prop2disp(struct UIKObjPropGad *opg, struct UIKObj *disp)
{
    sprintf( UIK_ObjTitle( disp ), "  %ld  ", 15 - opg->Pos );
    UIK_Refresh( disp, 1 );
    return(1);
}

static int lnk_pal2red( struct UIKObjPaletteDispl *opd, struct UIKObjPropGad *opg)
{
  WORD Pos = 15 - ((opd->UIKPrefs.ColorTable[opd->Color]) >> 8);

    UIK_OPr_ChangePropGadget( opg, -1, -1, Pos );
    UIK_CauseObjHook( opg, GADGETDOWN );
    return(1);
}
static int lnk_pal2grn( struct UIKObjPaletteDispl *opd, struct UIKObjPropGad *opg )
{
  WORD Pos = 15 - (((opd->UIKPrefs.ColorTable[opd->Color]) & 0xf0) >> 4);

    UIK_OPr_ChangePropGadget( opg, -1, -1, Pos );
    UIK_CauseObjHook( opg, GADGETDOWN );
    return(1);
}
static int lnk_pal2blu( struct UIKObjPaletteDispl *opd, struct UIKObjPropGad *opg )
{
  WORD Pos = 15 - ((opd->UIKPrefs.ColorTable[opd->Color]) & 0xf);

    UIK_OPr_ChangePropGadget( opg, -1, -1, Pos );
    UIK_CauseObjHook( opg, GADGETDOWN );
    return(1);
}

static int lnk_pal2col( struct UIKObjPaletteDispl *opd, struct UIKObj *obj )
{
  struct RastPort *rp;

    rp = obj->rp;
    SetAPen(rp, opd->Color );
    RectFill( rp, obj->Box.Left + 4,
                  obj->Box.Top + 2,
                  obj->Box.Left + obj->Box.Width - 5,
                  obj->Box.Top + obj->Box.Height - 3 );
    return(1);
}

static void DoRestore( struct UIKObj *obj, struct UIKObjPaletteDispl *opd )
{
  struct ViewPort *vp = &opd->Obj.w->WScreen->ViewPort;
  struct UIKObjPalette *pobj = (struct UIKObjPalette *) obj->UserULong1;
  UWORD depth = MIN(5,opd->UIKPrefs.DefScreen_Depth);

    CopyMem( &V.Prefs, &opd->UIKPrefs, sizeof(struct UIKPrefs) );
    LoadRGB4( vp, &opd->UIKPrefs.ColorTable[0], 1 << depth );
    UIK_CauseObjHook( pobj->paldsp, GADGETDOWN );
}

static void ActiveCols( struct UIKObj *obj, struct UIKObjPaletteDispl *opd )
{
    UIK_CauseObjHook( opd, GADGETDOWN );
}

static void DoOK( struct UIKObj *obj, struct UIKObjPaletteDispl *opd )
{
  struct UIKObjPalette *pobj = (struct UIKObjPalette *) obj->UserULong1;

    UIK_SetPrefs( &opd->UIKPrefs, sizeof(struct UIKPrefs) );
    CopyMem( &opd->UIKPrefs, &V.Prefs, sizeof(struct UIKPrefs) );
    UIK_Remove( pobj );
}

static void DoCancel( struct UIKObj *wo, struct UIKObjPaletteDispl *opd )
{
  struct ViewPort *vp;
  struct UIKObjPalette *pobj = (struct UIKObjPalette *) wo->UserULong1;
  UWORD depth = MIN(5,opd->UIKPrefs.DefScreen_Depth);

    if (wo->w && wo->w->WScreen)
        {
        vp = &wo->w->WScreen->ViewPort;
        LoadRGB4( vp, &V.Prefs.ColorTable[0], 1 << depth );
        }
    UIK_Remove( pobj );
}

static void updatered( struct UIKObjPropGad *opg, struct UIKObjPaletteDispl *opd )
{
  UWORD Red = 15 - opg->Pos;
  UWORD Grn = (opd->UIKPrefs.ColorTable[opd->Color] & 0x0f0) >> 4;
  UWORD Blu = opd->UIKPrefs.ColorTable[opd->Color] & 0x00f;
  struct ViewPort *vp = &opg->Obj.w->WScreen->ViewPort;

    opd->UIKPrefs.ColorTable[opd->Color] = Red << 8 | Grn << 4 | Blu;
    SetRGB4( vp, opd->Color, Red, Grn, Blu );
}
static void updategrn( struct UIKObjPropGad *opg, struct UIKObjPaletteDispl *opd )
{
  UWORD Grn = 15 - opg->Pos;
  UWORD Red = (opd->UIKPrefs.ColorTable[opd->Color] & 0xf00) >> 8;
  UWORD Blu = opd->UIKPrefs.ColorTable[opd->Color] & 0x00f;
  struct ViewPort *vp = &opg->Obj.w->WScreen->ViewPort;

    opd->UIKPrefs.ColorTable[opd->Color] = Red << 8 | Grn << 4 | Blu;
    SetRGB4( vp, opd->Color, Red, Grn, Blu );
}
static void updateblu( struct UIKObjPropGad *opg, struct UIKObjPaletteDispl *opd )
{
  UWORD Blu = 15 - opg->Pos;
  UWORD Grn = (opd->UIKPrefs.ColorTable[opd->Color] & 0x0f0) >> 4;
  UWORD Red = (opd->UIKPrefs.ColorTable[opd->Color] & 0xf00) >> 8;
  struct ViewPort *vp = &opg->Obj.w->WScreen->ViewPort;

    opd->UIKPrefs.ColorTable[opd->Color] = Red << 8 | Grn << 4 | Blu;
    SetRGB4( vp, opd->Color, Red, Grn, Blu );
}

/****************************************************************
 *
 *      Object
 *
 ****************************************************************/

static int StartPalette( struct UIKObjPalette *pobj )
{
    return(1);
}

static int AfterStartPalette( struct UIKObjPalette *pobj )
{
    if (pobj->Wino->Obj.w)
        {
        UIK_CauseObjHook( pobj->paldsp, GADGETDOWN);
        UIK_CauseObjHook( pobj->rprop, GADGETDOWN);
        UIK_CauseObjHook( pobj->gprop, GADGETDOWN);
        UIK_CauseObjHook( pobj->bprop, GADGETDOWN);
        }
    return(1);
}

static void StopPalette( struct UIKObjPalette *pobj )
{
}

static void RemovePalette( struct UIKObjPalette *pobj )
{
    /*
    if (pobj->UIKBase) CloseLibrary( pobj->UIKBase );
    if (pobj->DOSBase) CloseLibrary( pobj->DOSBase );
    if (pobj->GfxBase) CloseLibrary( pobj->GfxBase );
    */
}

static int AddPalette( struct UIKObjPalette *pobj, struct TagItem *taglist )
{
  ULONG title_num;

    /*
    SysBase = (*(ULONG*)4);
    if (! (UIKBase = pobj->UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (DOSBase = pobj->DOSBase = (struct DOSBase *) OpenLibrary( DOSNAME, 0 ))) goto END_ERROR;
    if (! (GfxBase = pobj->GfxBase = (struct GfxBase *) OpenLibrary( "graphics.library", 0 ))) goto END_ERROR;
    */

    if (! (title_num = GetTagData( UIKTAG_ObjPalette_FirstTitleNum, 0, taglist ))) goto END_ERROR;
    //title_num = 1;

    //----------------- window
    if (! (pobj->Wino = (struct UIKObjWindow *) UIK_AddObjectTags( "UIKObj_Window", pobj,
                UIKTAG_OBJ_LeftTop,         SETL(300,50),
                UIKTAG_OBJ_WidthHeight,     SETL(199,152),
                UIKTAG_OBJ_Title,           (ULONG) WTitle,
                UIKTAG_OBJ_AltTitle,        (ULONG) STitle,
                UIKTAG_OBJ_TitleFl_Addr,    TRUE,
                UIKTAG_OBJWindowFl_With_Size, FALSE,
                UIKTAG_OBJWindowFl_DefScreenOnly, TRUE,
                UIKTAG_OBJ_UserValue1,      (ULONG) pobj,
                TAG_END ))) goto END_ERROR;

    //----------------- frames
    if (! (pobj->ColFram = UIK_AddObjectTags( "UIKObj_Frame", pobj->Wino,
                UIKTAG_OBJ_LeftTop,        SETL(9,5),
                UIKTAG_OBJ_WidthHeight,    SETL(84,124),
                UIKTAG_OBJ_BoxFl_Zoneable, TRUE,
                TAG_END ))) goto END_ERROR;
    if (! (pobj->RVBFram = UIK_AddObjectTags( "UIKObj_Frame", pobj->Wino,
                UIKTAG_OBJ_LeftTop,        SETL(100,5),
                UIKTAG_OBJ_WidthHeight,    SETL(90,124),
                UIKTAG_OBJ_BoxFl_Zoneable, TRUE,
                TAG_END ))) goto END_ERROR;

    //----------------- props
    {struct TagItem prop_tl[] = {
        { UIKTAG_OBJ_LeftTop,        SETL(107,21) },
        { UIKTAG_OBJ_WidthHeight,    SETL(24,87) },
        { UIKTAG_OBJ_Pattern,        (ULONG)PatProp },
        { UIKTAG_OBJ_PatternPower,   1 },
        { UIKTAG_OBJProp_Vertical,   TRUE },
        { UIKTAG_OBJProp_Total,      16 },
        { UIKTAG_OBJProp_Visible,    1 },
        { UIKTAG_OBJProp_Position,   0 },
        { UIKTAG_OBJ_BoxFl_Over,     FALSE },
        { UIKTAG_OBJ_BoxFl_Filled,   TRUE },
        { TAG_END } };

    if (! (pobj->rprop = (struct UIKObjProp *) UIK_AddObject( "UIKObj_PropLift", pobj->RVBFram, prop_tl ))) goto END_ERROR;
    prop_tl[0].ti_Data = SETL(133,21);
    if (! (pobj->gprop = (struct UIKObjProp *) UIK_AddObject( "UIKObj_PropLift", pobj->RVBFram, prop_tl ))) goto END_ERROR;
    prop_tl[0].ti_Data = SETL(159,21);
    if (! (pobj->bprop = (struct UIKObjProp *) UIK_AddObject( "UIKObj_PropLift", pobj->RVBFram, prop_tl ))) goto END_ERROR;
    }

    //----------------- boutons r g b
    { struct TagItem tl[] = {
        {  UIKTAG_OBJ_LeftTop,        SETL(0,91) },
        {  UIKTAG_OBJ_WidthHeight,    SETL(24,12) },
        {  UIKTAG_OBJ_ParentRelative, TRUE },
        {  UIKTAG_OBJ_BoxFl_Over,     FALSE },
        {  UIKTAG_OBJ_TitleFl_Addr,   TRUE },
        {  UIKTAG_OBJ_Title,          (ULONG)"      " },
        {  UIKTAG_OBJ_ActInactPens,   UIKCOLS(UIKCOL_GREY,UIKCOL_GREY,UIKCOL_BLACK,UIKCOL_BLACK) },
        {  TAG_END } };

    tl[7].ti_Data = (ULONG) pobj;
    if (! (pobj->rdisp = UIK_AddObject( "UIKObj_Box", pobj->rprop, tl ))) goto END_ERROR;
    tl[5].ti_Data = (ULONG) "      ";
    if (! (pobj->gdisp = UIK_AddObject( "UIKObj_Box", pobj->gprop, tl ))) goto END_ERROR;
    tl[5].ti_Data = (ULONG) "      ";
    if (! (pobj->bdisp = UIK_AddObject( "UIKObj_Box", pobj->bprop, tl ))) goto END_ERROR;

    if (! (pobj->colname = UIK_AddObjectTags( "UIKObj_Box", pobj->Wino,
                UIKTAG_OBJ_LeftTop,         SETL(11,8),
                UIKTAG_OBJ_WidthHeight,     SETL(79,12),
                UIKTAG_OBJ_BoxFl_NotDrawn,  TRUE,
                UIKTAG_OBJ_Title,           title_num++,
                UIKTAG_OBJ_TTextAttr,       (ULONG)&FrameTTA,
                TAG_END ))) goto END_ERROR;

    tl[0].ti_Data = SETL(0,-12); tl[1].ti_Data = SETL(24,13); tl[3].ti_Data = TRUE; tl[4].ti_Data = FALSE;
    tl[5].ti_Data = title_num++;
    if (! (pobj->rdisp2 = UIK_AddObject( "UIKObj_Box", pobj->rprop, tl ))) goto END_ERROR;
    tl[5].ti_Data = title_num++;
    if (! (pobj->gdisp2 = UIK_AddObject( "UIKObj_Box", pobj->gprop, tl ))) goto END_ERROR;
    tl[5].ti_Data = title_num++;
    if (! (pobj->bdisp2 = UIK_AddObject( "UIKObj_Box", pobj->bprop, tl ))) goto END_ERROR;
    }
    if (! UIK_AddVector( pobj->rprop, lnk_prop2disp, GADGETDOWN|MOUSEMOVE, (ULONG)pobj->rdisp )) goto END_ERROR;
    if (! UIK_AddVector( pobj->gprop, lnk_prop2disp, GADGETDOWN|MOUSEMOVE, (ULONG)pobj->gdisp )) goto END_ERROR;
    if (! UIK_AddVector( pobj->bprop, lnk_prop2disp, GADGETDOWN|MOUSEMOVE, (ULONG)pobj->bdisp )) goto END_ERROR;

    //----------------- objet privé ObjPalDisplRegGerm
    if (! (pobj->paldsp = (struct UIKObjPaletteDispl *) UIK_AddPrivateObjectTags( &ObjPalDisplRegGerm, pobj->ColFram,
                UIKTAG_OBJ_LeftTop,         SETL(15,21),
                UIKTAG_OBJ_WidthHeight,     SETL(71,103),
                TAG_END ))) goto END_ERROR;

    if (! UIK_AddVector( pobj->paldsp, lnk_pal2red, GADGETDOWN, (ULONG)pobj->rprop )) goto END_ERROR;
    if (! UIK_AddVector( pobj->paldsp, lnk_pal2grn, GADGETDOWN, (ULONG)pobj->gprop )) goto END_ERROR;
    if (! UIK_AddVector( pobj->paldsp, lnk_pal2blu, GADGETDOWN, (ULONG)pobj->bprop )) goto END_ERROR;
    if (! UIK_AddVector( pobj->rprop, (void *)updatered, GADGETDOWN|MOUSEMOVE, (ULONG)pobj->paldsp )) goto END_ERROR;
    if (! UIK_AddVector( pobj->gprop, (void *)updategrn, GADGETDOWN|MOUSEMOVE, (ULONG)pobj->paldsp )) goto END_ERROR;
    if (! UIK_AddVector( pobj->bprop, (void *)updateblu, GADGETDOWN|MOUSEMOVE, (ULONG)pobj->paldsp )) goto END_ERROR;

    //----------------- close
    if (! UIK_AddVector( pobj->Wino, (void *) DoCancel, CLOSEWINDOW, (ULONG) pobj->paldsp )) goto END_ERROR;

    //----------------- ok
    if (! (pobj->ok = UIK_AddObjectTags ( "UIKObj_OK", pobj->Wino,
                UIKTAG_OBJ_LeftTop,         SETL(143,132),
                UIKTAG_OBJ_WidthHeight,     SETL(46,16),
                UIKTAG_OBJ_UserValue1,      (ULONG) pobj,
                //UIKTAG_OBJ_ShortCutKey,     B2L(0x0d,0,0,0),
                TAG_END ))) goto END_ERROR;

    if (! UIK_AddVector( pobj->ok, (void *)DoOK, GADGETUP, (ULONG)pobj->paldsp)) goto END_ERROR;

    //----------------- restore
    if (! (pobj->restore = UIK_AddObjectTags ( "UIKObj_Boolean", pobj->Wino,
                UIKTAG_OBJ_LeftTop,         SETL(9,132),
                UIKTAG_OBJ_WidthHeight,     SETL(84,16),
                UIKTAG_OBJ_Title,           title_num++,
                UIKTAG_OBJ_UserValue1,      (ULONG) pobj,
                //UIKTAG_OBJ_ShortCutKey,     B2L('r',0,0,0),
                TAG_END ))) goto END_ERROR;

    if (! UIK_AddVector( pobj->restore, (void *)DoRestore, GADGETUP, (ULONG)pobj->paldsp)) goto END_ERROR;

    if (! UIK_AddVector( pobj->Wino, (void *) ActiveCols, ACTIVEWINDOW, (ULONG) pobj->paldsp )) goto END_ERROR;

    return(1);

  END_ERROR:
    return(0);
}

static struct UIKObjGerm __far Germ = {
    "",
    AddPalette,         /*int     (*AddVector)(); */
    StartPalette,       /*int     (*StartVector)(); */
    StopPalette,        /*void    (*StopVector)(); */
    RemovePalette,      /*void    (*RemoveVector)(); */
    NULL,               /*void    (*RefreshVector)(); */
    NULL,               /*void    (*ResizeVector)(); */
    NULL,               /*void    (*ExceptActVector)(); */
    NULL,               /*void    (*ExceptInaVector)(); */
    NULL,               /*void    (*ExceptMovVector)(); */
    NULL,               /*void    (*ExceptKeyVector)(); */
    AfterStartPalette,  /*int     (*StartAfterVector)(); */
    NULL,               /*int     (*SelectVector)(); */
    NULL,               /*ULONG   *ExtentedVectors; */
    NULL,               /*struct TagItem *GermTagList; */
    0,                  /*UWORD   MinWidth; */
    0,                  /*UWORD   MinHeight; */
    0,                  /*ULONG   DefaultWidthHeight; */
    0,                  /*UWORD   ObjId; */
    sizeof(struct UIKObjPalette), /*UWORD   ObjSize; */
    BOX_SEMAPHORE,
    };

struct UIKObjRegGerm __far ObjPaletteRegGerm = { &Germ, 0, 0 ,0 };
