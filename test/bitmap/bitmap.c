/****************************************************************
 *
 *      Project:   UIK
 *      Function:  Test des tags ...CustomBitmap
 *
 *      Created:   14/02/92    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include <stdio.h>

#include <libraries/dos.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikmacros.h"
#include "uiki:uikglobal.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objscreen.h"
#include "uiki:objilbm.h"
#include "uiki:objwindow.h"
#include "uiki:objrequest.h"

#ifdef LATTICE
int CXBRK(void) { return(0); }
int chkabort(void) { return(0); }
#endif
#ifdef SAS
int CXBRK    ( void ) { return(0); }
int _CXBRK   ( void ) { return(0); }
int chkabort ( void ) { return(0); }
#endif


/****** Imported ************************************************/

extern ULONG DOSBase;


/****** Exported ************************************************/

struct UIKBase *UIKBase;


/****** Statics *************************************************/

static struct UIKGlobal *UIK;
static struct UIKIlbmFrame UFrameWin0;
static struct UIKIlbmFrame UFrameWin1;
static struct UIKIlbmFrame UFrameReq0;
static struct UIKIlbmFrame UFrameReq1;


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void startreq( struct UIKObj *obj, struct UIKObj *req )
{
    UIK_Start( req );
}

static void stopreq( struct UIKObj *obj, struct UIKObj *req )
{
    UIK_Stop( req );
}

/*---------------------------------------------------------------*/

static void Cleanup()
{
    if (UIK)     UIK_Remove( UIK );
    if (UIKBase) CloseLibrary( UIKBase );
    exit(0);
}

void main()
{
  struct UIKObj *oilbm=0, *or, *o;
  struct UIKObjWindow *ow=0;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (UIK = (struct UIKGlobal *) UIK_AddObject( "UIK", 0, 0 ))) goto END_ERROR;

    /*------lecture des brosses */
    if (! (oilbm = UIK_AddObject( "UIKObj_ILBM", UIK, 0 ))) goto END_ERROR;
    if (IFF_DONE != UIK_CallObjectFunc( oilbm, UIKFUNC_ILBM_File2BitMap, (ULONG)"bitmapwin0.pic", (ULONG)&UFrameWin0 )) goto END_ERROR;
    if (IFF_DONE != UIK_CallObjectFunc( oilbm, UIKFUNC_ILBM_File2BitMap, (ULONG)"bitmapwin1.pic", (ULONG)&UFrameWin1)) goto END_ERROR;
    if (IFF_DONE != UIK_CallObjectFunc( oilbm, UIKFUNC_ILBM_File2BitMap, (ULONG)"bitmapreq0.pic", (ULONG)&UFrameReq0 )) goto END_ERROR;
    if (IFF_DONE != UIK_CallObjectFunc( oilbm, UIKFUNC_ILBM_File2BitMap, (ULONG)"bitmapreq1.pic", (ULONG)&UFrameReq1 )) goto END_ERROR;

    /*------une fenêtre sans la bordure noire et avec des bitmaps */
    if (! (ow = (struct UIKObjWindow *) UIK_AddObjectTags( "UIKObj_Window", UIK,
                            UIKTAG_OBJ_LeftTop,               SETL(130,20),
                            UIKTAG_OBJ_WidthHeight,           SETL(373,130),
                            UIKTAG_OBJWindow_CustomBitMap,    (ULONG) UFrameWin0.BMap,
                            UIKTAG_OBJWindow_CustomAltBitMap, (ULONG) UFrameWin1.BMap,
                            UIKTAG_OBJWindowFl_NoBlackPerimeter, TRUE,
                            UIKTAG_OBJ_Title, (ULONG)"Exemple avec BitMap",
                            UIKTAG_OBJ_TitleFl_Addr, TRUE,
                            TAG_END ))) goto END_ERROR;

    /*------des gadgets de contrôle */
    if (! UIK_AddObjectTags( "UIKObj_WindowClose", ow,
                            UIKTAG_OBJ_LeftTop,             SETL(5,26),
                            UIKTAG_OBJ_WidthHeight,         SETL(35,17),
                            UIKTAG_OBJ_BoxFl_UseBitMap,     TRUE,
                            TAG_END )) goto END_ERROR;
    if (! UIK_AddObjectTags( "UIKObj_WindowDragg", ow,
                            UIKTAG_OBJ_LeftTop,             SETL(300,43),
                            UIKTAG_OBJ_WidthHeight,         SETL(60,27),
                            UIKTAG_OBJ_BoxFl_UseBitMap,     TRUE,
                            TAG_END )) goto END_ERROR;
    if (! UIK_AddObjectTags( "UIKObj_WindowSize", ow,
                            UIKTAG_OBJ_LeftTop,             SETL(227,94),
                            UIKTAG_OBJ_WidthHeight,         SETL(120,34),
                            UIKTAG_OBJ_BoxFl_UseBitMap,     TRUE,
                            TAG_END )) goto END_ERROR;
    if (! UIK_AddObjectTags( "UIKObj_WindowZoom", ow,
                            UIKTAG_OBJ_LeftTop,             SETL(8,96),
                            UIKTAG_OBJ_WidthHeight,         SETL(125,34),
                            UIKTAG_OBJ_BoxFl_UseBitMap,     TRUE,
                            TAG_END )) goto END_ERROR;

    /*------le gadget d'appel du requester */
    if (! (o = (struct UIKObj *) UIK_AddObjectTags( "UIKObj_Boolean", ow,
                            UIKTAG_OBJ_LeftTop,             SETL(88,58),
                            UIKTAG_OBJ_WidthHeight,         SETL(198,32),
                            UIKTAG_OBJ_BoxFl_UseBitMap,     TRUE,
                            TAG_END ))) goto END_ERROR;

    /*------le requuester qui suit a le tag UIKTAG_OBJ_BoxFl_StartSpecify donc */
    /*      il ne sera démarré que explicitement si on met son pointeur a UIK_Start() */
    if (! (or = (struct UIKObj *) UIK_AddObjectTags( "UIKObj_Request", ow,
                            UIKTAG_OBJ_LeftTop,            SETL(83,47),
                            UIKTAG_OBJ_WidthHeight,        SETL(207,54),
                            UIKTAG_OBJReq_CustomBitMap,    (ULONG) UFrameReq0.BMap,
                            UIKTAG_OBJReq_CustomAltBitMap, (ULONG) UFrameReq1.BMap,
                            UIKTAG_OBJ_BoxFl_StartSpecify, TRUE,
                            TAG_END ))) goto END_ERROR;
    /*------un vecteur sur le gadget d'appel du requester */
    if (! UIK_AddVector( o, (void*)startreq, GADGETUP, (ULONG) or )) goto END_ERROR;

    /*------un gadget pour quitter le requester */
    if (! (o = (struct UIKObj *) UIK_AddObjectTags( "UIKObj_Boolean", or,
                            UIKTAG_OBJ_LeftTop,             SETL(85,17),
                            UIKTAG_OBJ_WidthHeight,         SETL(40,19),
                            UIKTAG_OBJ_BoxFl_UseBitMap,     TRUE,
                            TAG_END ))) goto END_ERROR;
    /*------un vecteur pour la fonction qui stoppe le requester */
    if (! UIK_AddVector( o, (void*)stopreq, GADGETUP, (ULONG) or )) goto END_ERROR;

    /*=================== on démarre le tout ======================*/
    if (! UIK_Start( UIK )) goto END_ERROR;

    UIK_Do( UIK, SIGBREAKF_CTRL_C );

  END_ERROR:

    if (ow == 0)
        printf(  "Démarrez ce programme depuis son répertoire\n" );

    /*------on libère les bitmaps et d'éventuelles allocations */
    if (oilbm)
        {
        UIK_CallObjectFunc( oilbm, UIKFUNC_ILBM_CleanupFrame, (ULONG)ow, (ULONG)&UFrameWin0 );
        UIK_CallObjectFunc( oilbm, UIKFUNC_ILBM_CleanupFrame, (ULONG)ow, (ULONG)&UFrameWin1 );
        UIK_CallObjectFunc( oilbm, UIKFUNC_ILBM_CleanupFrame, (ULONG)ow, (ULONG)&UFrameReq0 );
        UIK_CallObjectFunc( oilbm, UIKFUNC_ILBM_CleanupFrame, (ULONG)ow, (ULONG)&UFrameReq1 );
        }
    Cleanup();
}
