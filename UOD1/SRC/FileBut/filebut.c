/****************************************************************
 *
 *      Project:   UIK
 *      Function:  objet WFileButton
 *
 *      Created:   01/03/93     Jean-Michel Forgeas
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

#include "uiki:objwinfilesel.h"
#include "uiki:objwfilebut.h"
#include "uiki:objstring.h"


/****** Macros **************************************************/


/****** Imported ************************************************/



/****** Exported ************************************************/

ULONG __far SysBase;
struct UIKBase * __far UIKBase;
struct DOSBase * __far DOSBase;


/****** Statics *************************************************/

static struct TagItem __far TagBoolMap[] =
    {
    { UIKTAG_WFB_AssignLst,         UIK_WFB_ASSIGNLIST },
    { UIKTAG_WFBFl_ReturnReturn,    UIK_WFB_RETURNRETURN },
    { UIKTAG_WFBFl_OK_IfNotExists,  UIK_WFB_OK_IFNOTEXISTS },
    { UIKTAG_WFBFl_DirsOnly,        UIK_WFB_DIRSONLY },
    { UIKTAG_WFBFl_ShowIcons,       UIK_WFB_SHOWICONS },
    { UIKTAG_WFBFl_StringRight,     UIK_WFB_STRINGRIGHT },
    { UIKTAG_WFBFl_AutoHeight,      UIK_WFB_AUTOHEIGHT },
    { UIKTAG_WFBFl_ButtonNoPush,    UIK_WFB_BUTTONNOPUSH },
    { UIKTAG_WFBFl_StringsTop,      UIK_WFB_STRINGSTOP },
    { UIKTAG_WFBFl_LeavePath,       UIK_WFB_LEAVEPATH },
    { TAG_DONE }
    };

static UWORD chip BrFileData[5][9][1] =
   {
   0x03c0,    /* ......####...... */
   0x0420,    /* .....#....#..... */
   0xf810,    /* #####......#.... */
   0xfc10,    /* ######.....#.... */
   0xc3f0,    /* ##....######.... */
   0xc010,    /* ##.........#.... */
   0xc010,    /* ##.........#.... */
   0xc010,    /* ##.........#.... */
   0xfff0,    /* ############.... */

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
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x03c0,    /* ......####...... */
   0x0420,    /* .....#....#..... */
   0xf810,    /* #####......#.... */
   0xfc10,    /* ######.....#.... */
   0xc3f0,    /* ##....######.... */
   0xc010,    /* ##.........#.... */
   0xc010,    /* ##.........#.... */
   0xc010,    /* ##.........#.... */
   0xfff0,    /* ############.... */
   };
static struct UIKBitMap __far UBM_BrFile = { &BrFileData[4][0][0], UIKBMAP_MASKED|UIKBMAP_CONTIG,
    12, 2,9, 0,4,0, &BrFileData[0][0][0], &BrFileData[1][0][0], &BrFileData[2][0][0], &BrFileData[3][0][0], 0, 0, 0, 0 };


static struct UIKBitMap * __far ObjBitMapTable[] = {
    &UBM_BrFile,
    0
    };


/****************************************************************
 *
 *      Callbacks
 *
 ****************************************************************/

static void OKFunc( struct UIKObjWinFileSel *wfs, UBYTE *totname )
{
  struct UIKObjWFileBut *wfb = (struct UIKObjWFileBut *) wfs->Obj.Parent->Parent;

    UIK_OSt_Set( wfb->OS, totname, StrLen( totname ) );
    UIK_Remove( wfs ); wfb->WFS = 0;
    UIK_OTo_UnselectToggle( wfb->OG );

    if (wfb->OKFunc) (*wfb->OKFunc)( wfb, totname );
}

static void CancelFunc( int nil, struct UIKObjWinFileSel *wfs )
{
  struct UIKObjWFileBut *wfb = (struct UIKObjWFileBut *) wfs->Obj.Parent->Parent;

    UIK_Remove( wfs ); wfb->WFS = 0;
    UIK_OTo_UnselectToggle( wfb->OG );
}


/****************************************************************
 *
 *      Methodes
 *
 ****************************************************************/

static void unselect_toggle( struct UIKObj *wo, struct UIKObjWFileBut *wfb ) // CLOSEWINDOW
{
    if (wfb->OG) UIK_OTo_UnselectToggle( wfb->OG );
    if (wfb->WFS) { UIK_Remove( wfb->WFS ); wfb->WFS = 0; }
}

static void toggle_select( struct UIKObjGad *tog, struct UIKObjWFileBut *wfb )
{
    if (! wfb->WFS)
        {
        UIK_OTo_SelectToggle( tog );

        if (wfb->WFS = (struct UIKObjWinFileSel *) UIK_AddObjectTags( "UIKObj_WinFileSel", tog,
            UIKTAG_WFSFl_ReturnReturn,  TRUE,
            UIKTAG_OBJ_Title,           wfb->SelTitle,
            UIKTAG_OBJ_AltTitle,        wfb->SelAltTitle,
            UIKTAG_OBJ_LangMaster,      wfb->Obj.ObjExt->Lang->Master,
            UIKTAG_WFS_OKFunc,          OKFunc,
            UIKTAG_WFS_CancelFunc,      CancelFunc,
            UIKTAG_WFS_InitPath,        UIK_OSt_Get( wfb->OS, 0 ),
            UIKTAG_WFS_Pattern,         wfb->Pattern,
            UIKTAG_WFS_Flags,           wfb->Flags,
            TAG_DONE ))
            {
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

static void ObjString_Keys( struct UIKObjStrGad *objstr, struct UIKObjWFileBut *wfb )
{
  struct UIKGlobal *uik = objstr->Obj.UIK;

    if (uik->IntuiMsg->Code & 0x80)
        return; /* touche relâchée */

    if (uik->KeySupp.Length == 1 && uik->KeySupp.KeyBuffer[0] == 0x0d)
        {
        if (wfb->OKFunc)
            {
            ULONG len;
            UBYTE *ptr, *buf;

            UIK_LockObjVars( wfb->OS );
            ptr = UIK_OSt_Get( wfb->OS, &len );
            if (buf = UIK_MemSpAlloc( len+1, MEMF_ANY )) StrCpy( buf, ptr );
            UIK_UnlockObjVars( wfb->OS );

            if (buf)
                {
                (*wfb->OKFunc)( wfb, buf );
                UIK_MemSpFree( buf );
                }
            }
        }
}

/****************************************************************
 *
 *      Init/Cleanup
 *
 ****************************************************************/

static void GenericRefresh( struct UIKObjWFileBut *wfb )
{
    UIK_DrawBox( wfb );
}

static void GenericRemove( struct UIKObjWFileBut *wfb )
{
    BufFreeS( &wfb->Pattern );
    if (wfb->DOSBase) CloseLibrary( wfb->DOSBase );
    if (wfb->UIKBase) CloseLibrary( wfb->UIKBase );
}

static int GenericAdd( struct UIKObjWFileBut *wfb, struct TagItem *taglist )
{
  WORD sl, sw, sh;
  WORD bl, bw, bh;
  UBYTE *ptr;
  ULONG sizeflags;
  ULONG extraspace, buttitle, butalttitle, butcolors, butshcut, butimage;

    SysBase = (*(ULONG*)4);
    if (! (wfb->UIKBase = UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (wfb->DOSBase = DOSBase = (struct DOSBase *) OpenLibrary( DOSNAME, 0 ))) goto END_ERROR;

    wfb->Obj.Flags |= BOX_NDRAWN;
    wfb->Obj.ObjExt->SCQual = 0xffff; // annule l'activation en laissant le soulignement

    UIK_InvertBlackWhite( wfb->Obj.UIK, ObjBitMapTable, 1 );

    //------ tags
    wfb->OKFunc = (void *) UIK_GetTagData( UIKTAG_WFB_OKFunc, 0, taglist );
    extraspace  = UIK_GetTagData( UIKTAG_WFB_ExtraSpace, 2, taglist );
    buttitle    = UIK_GetTagData( UIKTAG_WFB_ButtonTitle, (ULONG)&UBM_BrFile, taglist );
    butalttitle = UIK_GetTagData( UIKTAG_WFB_ButtonAltTitle, 0, taglist );
    butcolors   = UIK_GetTagData( UIKTAG_WFB_ButtonActInactPens, UIKCOLS(UIKCOL_LIGHT,UIKCOL_GREY,UIKCOL_BLACK,UIKCOL_WHITE), taglist );
    butshcut    = UIK_GetTagData( UIKTAG_WFB_ButtonShortCut, 0, taglist );
    butimage    = UIK_GetTagData( UIKTAG_WFBFl_ButtonImage, (buttitle == (ULONG)&UBM_BrFile) ? TRUE : FALSE, taglist );
    wfb->SelTitle = UIK_GetTagData( UIKTAG_WFB_SelectorTitle, 0, taglist );
    wfb->SelAltTitle = UIK_GetTagData( UIKTAG_WFB_SelectorAltTitle, 0, taglist );
    wfb->Flags  = UIK_GetTagData( UIKTAG_WFB_Flags, UIK_WFB_LEAVEPATH|UIK_WFB_AUTOHEIGHT/*|UIK_WFB_RETURNRETURN*/, taglist );
    wfb->Flags  = (ULONG) UIK_PackBoolTags( (ULONG)wfb->Flags, taglist, TagBoolMap );

    if (! (wfb->Pattern = BufAlloc( 8, 0, MEMF_ANY ))) goto END_ERROR;
    ptr = (UBYTE*) UIK_GetTagData( UIKTAG_WFB_Pattern, (ULONG)"#?", taglist );
    BufSetS( ptr, StrLen( ptr ), &wfb->Pattern );

    //------ tailles
    if (wfb->Flags & UIK_WFB_AUTOHEIGHT) wfb->Obj.Box.Height = wfb->Obj.ObjExt->Font->tf_YSize + 5;
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
    bw = UIK_GetTagData( UIKTAG_WFB_ButtonWidth, (ULONG)bw, taglist );
    sw = wfb->Obj.Box.Width - (bw + extraspace);

    if (wfb->Flags & UIK_WFB_STRINGRIGHT)
        {
        bl = 0; sl = bw + extraspace;
        }
    else{
        sl = 0; bl = sw + extraspace;
        }

    //------ ajout des objets
    sizeflags = wfb->Obj.ResizeFlags;

    if (! (wfb->OS = (struct UIKObjStrGad *) UIK_AddObjectTags( "UIKObj_String", wfb,
            UIKTAG_OBJ_ParentRelative,  TRUE,
            UIKTAG_OBJ_LeftTop,         W2L(sl,0),
            UIKTAG_OBJ_WidthHeight,     W2L(sw,sh),
            UIKTAG_OBJ_ShortCutKey,     UIK_GetTagData( UIKTAG_OBJ_ShortCutKey, 0, taglist ),
            UIKTAG_OBJ_Sz_Attached_Flags, sizeflags,
            UIKTAG_OBJStr_TextPointer,  UIK_GetTagData( UIKTAG_WFB_InitPath, (ULONG)"", taglist ),
            UIKTAG_OBJStrFl_AutoHeight, (wfb->Flags & UIK_WFB_AUTOHEIGHT) ? TRUE : FALSE,
            TAG_DONE ))) goto END_ERROR;
    if (! UIK_AddVector( wfb->OS, (void*)ObjString_Keys, RAWKEY, (ULONG) wfb )) goto END_ERROR;

    if ((sizeflags & (UO_ATTACHED_LEFT|UO_ATTACHED_RIGHT)) == UO_ATTACHED_LEFT|UO_ATTACHED_RIGHT)
        {
        if (wfb->Flags & UIK_WFB_STRINGRIGHT) sizeflags &= ~UO_ATTACHED_RIGHT;
        else sizeflags &= ~UO_ATTACHED_LEFT;
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
            UIKTAG_OBJ_BoxFl_NoPush,    (wfb->Flags & UIK_WFB_BUTTONNOPUSH) ? TRUE : FALSE,
            UIKTAG_OBJ_Sz_Attached_Flags, sizeflags,
            TAG_DONE ))) goto END_ERROR;
    if (! UIK_AddVector( wfb->OG, (void*)toggle_select, GADGETUP, (ULONG)wfb )) goto END_ERROR;

    return(1);

  END_ERROR:
    return(0);
}

static struct TagItem __far GermTagList[] =
{
   { UIKTAG_GEN_VersionRevision, W2L(1,6) },
   { TAG_END }
};

struct UIKObjGerm __far Germ = {
    "UIKObj_WFileButton",
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
    W2L(WFB_STDWIDTH,WFB_STDHEIGHT), /*ULONG   DefaultWidthHeight; */
    UIKID_WFILEBUTTON,  /*UWORD   ObjId; */
    sizeof(struct UIKObjWFileBut),   /*UWORD   ObjSize; */
    BOX_DISPLAYABLE|BOX_TRANSLATELEFTTOP | BOX_CANTDISABLE, /* flags par defaut */
    };
