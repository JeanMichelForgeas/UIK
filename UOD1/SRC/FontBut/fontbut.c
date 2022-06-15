/****************************************************************
 *
 *      Project:   UIK
 *      Function:  objet WFontButton
 *
 *      Created:   28/03/93     Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikbase.h"
#include "uiki:uikglobal.h"
#include "uiki:uikobj.h"
#include "uiki:uikmacros.h"
#include "uiki:uikgfx.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objwfontbut.h"
#include "uiki:objstring.h"


/****** Macros **************************************************/


/****** Imported ************************************************/



/****** Exported ************************************************/

ULONG __far SysBase;
struct UIKBase * __far UIKBase;
struct DOSBase * __far DOSBase;
struct GfxBase * __far GfxBase;
struct IntuitionBase * __far IntuitionBase;


/****** Statics *************************************************/

static struct TagItem __far TagBoolMap[] =
    {
    { UIKTAG_WFOBFl_PropWidth,      UIKF_WFOB_WPROP },
    { UIKTAG_WFOBFl_FixedWidth,     UIKF_WFOB_WFIXED },
    { UIKTAG_WFOBFl_StringRight,    UIK_WFOB_STRINGRIGHT },
    { UIKTAG_WFOBFl_AutoHeight,     UIK_WFOB_AUTOHEIGHT },
    { UIKTAG_WFOBFl_ButtonNoPush,   UIK_WFOB_BUTTONNOPUSH },
    { TAG_DONE }
    };

#include "uiki:uikgfx.h"

static UWORD chip BrFontData[5][9][1] =
   {
   0x0002,    /* ..............#. */
   0x33fe,    /* ..##..#########. */
   0xc200,    /* ##....#......... */
   0x0020,    /* ..........#..... */
   0x03e0,    /* ......#####..... */
   0x0200,    /* ......#......... */
   0x0200,    /* ......#......... */
   0x0e00,    /* ....###......... */
   0xf800,    /* #####........... */

   0x7ffc,    /* .#############.. */
   0xcc00,    /* ##..##.......... */
   0x0c00,    /* ....##.......... */
   0x0fc0,    /* ....######...... */
   0x0c00,    /* ....##.......... */
   0x0c00,    /* ....##.......... */
   0x1c00,    /* ...###.......... */
   0xf000,    /* ####............ */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x7ffe,    /* .##############. */
   0xfffe,    /* ###############. */
   0xce00,    /* ##..###......... */
   0x0fe0,    /* ....#######..... */
   0x0fe0,    /* ....#######..... */
   0x0e00,    /* ....###......... */
   0x1e00,    /* ...####......... */
   0xfe00,    /* #######......... */
   0xf800,    /* #####........... */
   };
struct UIKBitMap __far UBM_BrFont = { &BrFontData[4][0][0], UIKBMAP_MASKED|UIKBMAP_CONTIG,
    15, 2,9, 0,4,0, &BrFontData[0][0][0], &BrFontData[1][0][0], &BrFontData[2][0][0], &BrFontData[3][0][0], 0, 0, 0, 0 };

struct UIKBitMap * __far ObjBitMapTable[] = {
    &UBM_BrFont,
    0
    };

/****************************************************************
 *
 *      Callbacks
 *
 ****************************************************************/

static void OKFunc( struct UIKObjWinFontSel *wfs, struct TextFont *font, UBYTE style )
{
  struct UIKObjWFontBut *wfb = (struct UIKObjWFontBut *) wfs->Obj.Parent->Parent;
  UBYTE buf[10], *name = font->tf_Message.mn_Node.ln_Name;

    UIK_OSt_Set( wfb->OS, name, StrLen( name ) );
    sprintf( buf, "%ld", font->tf_YSize );
    UIK_OSt_Set( wfb->OSH, buf, StrLen( buf ) );
    UIK_Remove( wfs ); wfb->WFS = 0;
    UIK_OTo_UnselectToggle( wfb->OG );

    StrCpy( wfb->FontName, name );
    wfb->TTA.tta_YSize = font->tf_YSize;
    wfb->TTA.tta_Style = style;
    if (wfb->OKFunc) (*wfb->OKFunc)( wfb, &wfb->TTA );
}

static void CancelFunc( struct UIKObjWinFontSel *wfs )
{
  struct UIKObjWFontBut *wfb = (struct UIKObjWFontBut *) wfs->Obj.Parent->Parent;

    UIK_Remove( wfs ); wfb->WFS = 0;
    UIK_OTo_UnselectToggle( wfb->OG );
}


/****************************************************************
 *
 *      Methodes
 *
 ****************************************************************/

static void unselect_toggle( struct UIKObj *wo, struct UIKObjWFontBut *wfb ) // CLOSEWINDOW
{
    if (wfb->OG) UIK_OTo_UnselectToggle( wfb->OG );
    if (wfb->WFS) { UIK_Remove( wfb->WFS ); wfb->WFS = 0; }
}

static void toggle_select( struct UIKObjGad *tog, struct UIKObjWFontBut *wfb )
{
    if (! wfb->WFS)
        {
        UIK_OTo_SelectToggle( tog );

        StrCpy( wfb->FontName, UIK_OSt_Get( wfb->OS, 0 ) );
        wfb->TTA.tta_YSize = Str2Num( UIK_OSt_Get( wfb->OSH, 0 ) );

        if (wfb->WFS = (struct UIKObjWinFontSel *) UIK_AddObjectTags( "UIKObj_WinFontSel", tog,
            UIKTAG_OBJ_LangMaster,      wfb->Obj.ObjExt->Lang->Master,
            UIKTAG_OBJ_Title,           wfb->SelTitle,
            UIKTAG_OBJ_AltTitle,        wfb->SelAltTitle,
            UIKTAG_WFOS_OKFUNC,         OKFunc,
            UIKTAG_WFOS_CANCELFUNC,     CancelFunc,
            UIKTAG_WFOS_FONTSPATH,      wfb->FontPath,
            UIKTAG_WFOS_HighlightFont,  (wfb->TTA.tta_Name ? &wfb->TTA : 0),
            UIKTAG_WFOS_Flags,          wfb->Flags,
            TAG_DONE ))
            {
            wfb->WFS->FS->DispStatus = MONODISPLAY;
            UIK_OME_ActiveMExObj( wfb->WFS->FS->ObjVars.meo2 );
            if (UIK_AddVector( wfb->WFS->WO, (void*)unselect_toggle, CLOSEWINDOW, (ULONG)wfb ))
                if (UIK_Start( wfb->WFS ))
                    return;
            UIK_Remove( wfb->WFS ); wfb->WFS = 0;
            }
        UIK_OTo_UnselectToggle( tog );
        }
    else{
        if (wfb->WFS) { UIK_Remove( wfb->WFS ); wfb->WFS = 0; }
        UIK_OTo_UnselectToggle( tog );
        }
}

static UBYTE * __asm StringHandler( register __a1 UBYTE *buf ) /* sous contexte input.device */
{
    if (*buf < '0' || *buf > '9') return(0);
    return(buf);
}

static void ObjString_Keys( struct UIKObjStrGad *objstr, struct UIKObjWFontBut *wfb )
{
  struct UIKGlobal *uik = objstr->Obj.UIK;
  struct IntuiMessage *imsg = uik->IntuiMsg;
  struct TextFont *tf;
  UBYTE ok;

    if (imsg->Code & 0x80)
        return; /* touche relâchée */

    if (uik->KeySupp.Length == 1 && uik->KeySupp.KeyBuffer[0] == 0x0d)
        {
        StrCpy( wfb->FontName, UIK_OSt_Get( wfb->OS, 0 ) );
        wfb->TTA.tta_YSize = Str2Num( UIK_OSt_Get( wfb->OSH, 0 ) );
        wfb->TTA.tta_Style = 0;

        ok = 0;
        if (tf = UIK_OpenFontTTA( &wfb->TTA ))
            {
            if (tf->tf_YSize == wfb->TTA.tta_YSize)
                if (! StrNCCmp( tf->tf_Message.mn_Node.ln_Name, wfb->FontName ))
                    ok = 1;
            CloseFont( tf );
            }

        if (ok)
            { if (wfb->OKFunc) (*wfb->OKFunc)( wfb, &wfb->TTA ); }
        else{
            DisplayBeep( NULL );
            UIK_ActivateKeyTank( objstr );
            }
        }
}

/****************************************************************
 *
 *      Init/Cleanup
 *
 ****************************************************************/

static void GenericRefresh( struct UIKObjWFontBut *wfb )
{
    UIK_DrawBox( wfb );
}

static void GenericRemove( struct UIKObjWFontBut *wfb )
{
    BufFreeS( &wfb->FontPath );
    if (wfb->IntuitionBase) CloseLibrary( wfb->IntuitionBase );
    if (wfb->GfxBase) CloseLibrary( wfb->GfxBase );
    if (wfb->DOSBase) CloseLibrary( wfb->DOSBase );
    if (wfb->UIKBase) CloseLibrary( wfb->UIKBase );
}

static int GenericAdd( struct UIKObjWFontBut *wfb, struct TagItem *taglist )
{
  WORD sl, sw, shw, sh;
  WORD bl, bw, bh;
  UBYTE *ptr, buf[12];
  ULONG sizeflags, strsizeflags;
  ULONG extraspace, buttitle, butalttitle, butcolors, butshcut, butimage;
  struct TTextAttr *tta;

    SysBase = (*(ULONG*)4);
    if (! (wfb->UIKBase = UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (wfb->DOSBase = DOSBase = (struct DOSBase *) OpenLibrary( DOSNAME, 0 ))) goto END_ERROR;
    if (! (wfb->GfxBase = GfxBase = (struct GfxBase *) OpenLibrary( "graphics.library", 0 ))) goto END_ERROR;
    if (! (wfb->IntuitionBase = IntuitionBase = (struct IntuitionBase *) OpenLibrary( "intuition.library", 0 ))) goto END_ERROR;

    wfb->Obj.Flags |= BOX_NDRAWN;
    wfb->Obj.ObjExt->SCQual = 0xffff; // annule l'activation en laissant le soulignement

    UIK_InvertBlackWhite( wfb->Obj.UIK, ObjBitMapTable, 1 );

    //------ tags
    wfb->OKFunc = (void *) UIK_GetTagData( UIKTAG_WFOB_OKFunc, 0, taglist );
    extraspace  = UIK_GetTagData( UIKTAG_WFOB_ExtraSpace, 2, taglist );
    buttitle    = UIK_GetTagData( UIKTAG_WFOB_ButtonTitle, (ULONG)&UBM_BrFont, taglist );
    butalttitle = UIK_GetTagData( UIKTAG_WFOB_ButtonAltTitle, 0, taglist );
    butcolors   = UIK_GetTagData( UIKTAG_WFOB_ButtonActInactPens, UIKCOLS(UIKCOL_LIGHT,UIKCOL_GREY,UIKCOL_BLACK,UIKCOL_WHITE), taglist );
    butshcut    = UIK_GetTagData( UIKTAG_WFOB_ButtonShortCut, 0, taglist );
    butimage    = UIK_GetTagData( UIKTAG_WFOBFl_ButtonImage, (buttitle == (ULONG)&UBM_BrFont) ? TRUE : FALSE, taglist );
    wfb->SelTitle = UIK_GetTagData( UIKTAG_WFOB_SelectorTitle, 0, taglist );
    wfb->SelAltTitle = UIK_GetTagData( UIKTAG_WFOB_SelectorAltTitle, 0, taglist );
    wfb->Flags  = UIK_GetTagData( UIKTAG_WFOB_Flags, UIK_WFOB_AUTOHEIGHT|UIKF_WFOS_WPROP|UIKF_WFOS_WFIXED, taglist );
    wfb->Flags  = (ULONG) UIK_PackBoolTags( (ULONG)wfb->Flags, taglist, TagBoolMap );

    if (! (wfb->FontPath = BufAlloc( 8, 0, MEMF_ANY ))) goto END_ERROR;
    ptr = (UBYTE*) UIK_GetTagData( UIKTAG_WFOB_FONTSPATH, (ULONG)"FONTS:", taglist );
    BufSetS( ptr, StrLen( ptr ), &wfb->FontPath );

    if (tta = (struct TTextAttr *) UIK_GetTagData( UIKTAG_WFOB_HighlightFont, (ULONG)&wfb->Obj.ObjExt->TTA, taglist ))
        {
        StrCpy( wfb->FontName, tta->tta_Name );
        wfb->TTA = *tta;
        wfb->TTA.tta_Name = wfb->FontName;
        }

    //------ tailles
    if (wfb->Flags & UIK_WFOB_AUTOHEIGHT) wfb->Obj.Box.Height = wfb->Obj.ObjExt->Font->tf_YSize + 5;
    bh = sh = wfb->Obj.Box.Height;
    bw = 0;
    if (buttitle)
        {
        if (butimage == TRUE) bw = ((struct UIKBitMap*)buttitle)->Width;
        else{
            UBYTE *ptr = UIK_LangString( wfb, buttitle );
            bw = UIK_StrWidth( wfb->Obj.ObjExt->Font, ptr, StrLen( ptr ) );
            }
        }
    bw += 9;
    bw = UIK_GetTagData( UIKTAG_WFOB_ButtonWidth, (ULONG)bw, taglist );
    sw = wfb->Obj.Box.Width - (bw + extraspace);

    if (wfb->Flags & UIK_WFOB_STRINGRIGHT)
        {
        bl = 0; sl = bw + extraspace;
        }
    else{
        sl = 0; bl = sw + extraspace;
        }

    //------ ajout des objets
    strsizeflags = sizeflags = wfb->Obj.ResizeFlags;
    shw = 10 + (7 * UIK_CharWidth( wfb->Obj.ObjExt->Font, '8' )) / 2;

    if (! (wfb->OS = (struct UIKObjStrGad *) UIK_AddObjectTags( "UIKObj_String", wfb,
            UIKTAG_OBJ_ParentRelative,  TRUE,
            UIKTAG_OBJ_LeftTop,         W2L(sl,0),
            UIKTAG_OBJ_WidthHeight,     W2L(sw-shw-1,sh),
            UIKTAG_OBJ_ShortCutKey,     UIK_GetTagData( UIKTAG_OBJ_ShortCutKey, 0, taglist ),
            UIKTAG_OBJ_Sz_Attached_Flags, sizeflags,
            UIKTAG_OBJStr_TextPointer,  wfb->FontName,
            UIKTAG_OBJStrFl_AutoHeight, (wfb->Flags & UIK_WFOB_AUTOHEIGHT) ? TRUE : FALSE,
            UIKTAG_OBJStr_MaxChars,     31,
            TAG_DONE ))) goto END_ERROR;
    if (! UIK_AddVector( wfb->OS, (void*)ObjString_Keys, RAWKEY, (ULONG) wfb )) goto END_ERROR;

    if (sizeflags & UO_ATTACHED_RIGHT) sizeflags &= ~UO_ATTACHED_LEFT;

    sprintf( buf, "%ld", tta->tta_YSize );
    if (! (wfb->OSH = (struct UIKObjStrGad *) UIK_AddObjectTags( "UIKObj_String", wfb,
            UIKTAG_OBJ_ParentRelative,  TRUE,
            UIKTAG_OBJ_LeftTop,         W2L(sl+sw-shw,0),
            UIKTAG_OBJ_WidthHeight,     W2L(shw,sh),
            UIKTAG_OBJ_Sz_Attached_Flags, sizeflags,
            UIKTAG_OBJStr_TextPointer,  buf,
            UIKTAG_OBJStrFl_AutoHeight, (wfb->Flags & UIK_WFOB_AUTOHEIGHT) ? TRUE : FALSE,
            UIKTAG_OBJStr_MaxChars,     3,
            UIKTAG_OBJStr_UserHandler,  StringHandler,
            TAG_DONE ))) goto END_ERROR;
    if (! UIK_AddVector( wfb->OSH, (void*)ObjString_Keys, RAWKEY, (ULONG) wfb )) goto END_ERROR;

    if ((strsizeflags & (UO_ATTACHED_LEFT|UO_ATTACHED_RIGHT)) == UO_ATTACHED_LEFT|UO_ATTACHED_RIGHT)
        {
        if (wfb->Flags & UIK_WFOB_STRINGRIGHT) strsizeflags &= ~UO_ATTACHED_RIGHT;
        else strsizeflags &= ~UO_ATTACHED_LEFT;
        }
    if (! (wfb->OG = (struct UIKObjStrGad *) UIK_AddObjectTags( "UIKObj_Toggle", wfb,
            UIKTAG_OBJ_ParentRelative,  TRUE,
            UIKTAG_OBJ_LeftTop,         W2L(bl,0),
            UIKTAG_OBJ_WidthHeight,     W2L(bw,bh),
            UIKTAG_OBJ_LangMaster,      wfb->Obj.ObjExt->Lang->Master,
            UIKTAG_OBJ_Title,           buttitle,
            UIKTAG_OBJ_AltTitle,        butalttitle,
            UIKTAG_OBJ_TitleFl_Image,   butimage,
            UIKTAG_OBJ_BoxFl_Filled,    TRUE,
            UIKTAG_OBJ_ActInactPens,    butcolors,
            UIKTAG_OBJ_ShortCutKey,     butshcut,
            UIKTAG_OBJ_BoxFl_NoPush,    (wfb->Flags & UIK_WFOB_BUTTONNOPUSH) ? TRUE : FALSE,
            UIKTAG_OBJ_Sz_Attached_Flags, strsizeflags,
            TAG_DONE ))) goto END_ERROR;
    if (! UIK_AddVector( wfb->OG, (void*)toggle_select, GADGETUP, (ULONG)wfb )) goto END_ERROR;

    return(1);

  END_ERROR:
    return(0);
}

static struct TagItem __far GermTagList[] =
{
   { UIKTAG_GEN_VersionRevision, W2L(1,3) },
   { TAG_END }
};

struct UIKObjGerm __far Germ = {
    "UIKObj_WFontButton",
    GenericAdd,         /*int     (*AddVector)(); */
    NULL,               /*int     (*StartVector)(); */
    NULL,               /*void    (*StopVector)(); */
    GenericRemove,      /*void    (*RemoveVector)(); */
    GenericRefresh,     /*void    (*RefreshVector)(); */
    NULL,               /*void    (*ResizeVector)(); */
    NULL,               /*void    (*ExceptActVector)(); */
    NULL,               /*void    (*ExceptInaVector)(); */
    NULL,               /*void    (*ExceptMovVector)(); */
    NULL,               /*void    (*ExceptKeyVector)(); */
    NULL,               /*int     (*StartAfterVector)(); */
    NULL,               /*int     (*SelectVector)(); */
    NULL,               /*ULONG   *ExtentedVectors; */
    GermTagList,        /*struct TagItem *GermTagList; */
    50,                 /*UWORD   MinWidth; */
    10,                 /*UWORD   MinHeight; */
    W2L(WFOB_STDWIDTH,WFOB_STDHEIGHT), /*ULONG   DefaultWidthHeight; */
    UIKID_WFONTBUTTON,  /*UWORD   ObjId; */
    sizeof(struct UIKObjWFontBut),   /*UWORD   ObjSize; */
    BOX_DISPLAYABLE|BOX_TRANSLATELEFTTOP | BOX_CANTDISABLE, /* flags par defaut */
    };
