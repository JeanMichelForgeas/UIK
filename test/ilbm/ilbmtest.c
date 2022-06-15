/****************************************************************
 *
 *      Project:   UIK
 *      Function:  Test de l'objet UIKObj_ILBM
 *
 *      Created:   22/01/92    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include <stdio.h>

#ifdef LATTICE
int CXBRK(void) { return(0); }
int chkabort(void) { return(0); }
#endif
#ifdef SAS
int CXBRK    ( void ) { return(0); }
int _CXBRK   ( void ) { return(0); }
int chkabort ( void ) { return(0); }
#endif

#include <libraries/dos.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikmacros.h"
#include "uiki:uikglobal.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objscreen.h"
#include "uiki:objilbm.h"


/****** Imported ************************************************/

extern ULONG DOSBase;


/****** Exported ************************************************/

struct UIKBase *UIKBase;


/****** Statics *************************************************/

static struct UIKGlobal *UIK;
static struct UIKIlbmFrame UFrame;


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void Cleanup()
{
    if (UIK)     UIK_Remove( UIK );
    if (UIKBase) CloseLibrary( UIKBase );
    exit(0);
}

void main( int argc, char *argv[] )
{
  struct UIKObj *oilbm=0, *os;
  UBYTE *pictname;
  ULONG iffp;

    pictname = (argc > 1) ? argv[1] : "ilbmtest.pic";

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (UIK = (struct UIKGlobal *) UIK_AddObject( "UIK", 0,0 ))) goto END_ERROR;

    if (! (oilbm = UIK_AddObject( "UIKObj_ILBM", UIK, 0 ))) goto END_ERROR;
    if (IFF_DONE != (iffp = UIK_CallObjectFunc( oilbm, UIKFUNC_ILBM_File2BitMap, (ULONG)pictname, (ULONG)&UFrame )))
        {
        printf( "Erreur : iffp=%ld\n", iffp );
        goto END_ERROR;
        }
    if (! (os = (struct UIKObj *) UIK_AddObjectTags( "UIKObj_Screen", UIK,
                            UIKTAG_OBJ_LeftTop, 0,
                            UIKTAG_OBJ_WidthHeight,        SETL(UFrame.W,UFrame.H),
                            UIKTAG_ObjScreen_ViewModes,    UFrame.ViewMode,
                            UIKTAG_ObjScreen_CustomBitMap, UFrame.BMap,
                            UIKTAG_ObjScreen_Depth,        UFrame.Depth,
                            UIKTAG_OBJScreen_ScreenQuiet,  TRUE,
                            TAG_END ))) goto END_ERROR;

    /*----on ouvre l'écran avec la bitmap de l'image */
    if (! UIK_Start( UIK )) goto END_ERROR; /*-----------------------*/

    /*----on met ses bonnes couleurs */
    UIK_CallObjectFunc( oilbm, UIKFUNC_ILBM_Colors2Screen, (ULONG)os, (ULONG)&UFrame );
    /*----active éventuellement un chunk PCHG en installant une copperlist */
    UIK_CallObjectFunc( oilbm, UIKFUNC_ILBM_StartPCHG, (ULONG)os, (ULONG)&UFrame );

    /*----On attend qlq secondes */
    Delay( 200 );

    /*----on ferme l'écran */
    UIK_Stop( os );
    /*----puis on libère sa bitmap et d'éventuelles allocations */
    UIK_CallObjectFunc( oilbm, UIKFUNC_ILBM_CleanupFrame, (ULONG)os, (ULONG)&UFrame );

  END_ERROR:
    Cleanup();
}
