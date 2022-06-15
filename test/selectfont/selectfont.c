
#include <stdio.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikmacros.h"
#include "uiki:uikglobal.h"
#include "uiki:uikcolors.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objfontselector.h"
#include "uiki:objwindow.h"

#ifdef LATTICE
int CXBRK(void) { return(0); }      /* disable SASC CTRLC handling */
int chkabort(void) { return(0); }   /* really */
#endif
#ifdef SAS
int CXBRK    ( void ) { return(0); }
int _CXBRK   ( void ) { return(0); }
int chkabort ( void ) { return(0); }
#endif

extern ULONG DOSBase;

struct UIKBase *UIKBase;
struct UIKGlobal *UIK;


/*---------------------------------------------------------------------------*/

static void CancelFunc(struct UIKObjFontSelect *ofs)
{
   printf("Cancel!\n");
}

static void OKFunc(struct UIKObjFontSelect *ofs, struct TextFont *tf, UWORD Style )
{
   printf( "Font : '%ls', size %ld, style %lx\n", tf->tf_Message.mn_Node.ln_Name, tf->tf_YSize, Style
);
}

void main()
{
  struct UIKObj *wo, *obj;

    if (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))
        {
        if (UIK = (struct UIKGlobal *) UIK_AddObject( "UIK", 0, 0 ))
            {
            if (wo = (struct UIKObj *) UIK_AddObjectTags( "UIKObj_Window", UIK,
                        UIKTAG_OBJ_LeftTop,     SETL(150,50),
                        UIKTAG_OBJ_WidthHeight, SETL(330,110),
                        UIKTAG_OBJWindow_MinimumWidthHeight, SETL(100,50),
                        UIKTAG_OBJ_Title,       "FontSelector v1.0",
                        UIKTAG_OBJ_TitleFl_Addr,TRUE,
                        TAG_END ))
                {
                if (obj = (struct UIKObj *) UIK_AddObjectTags( "UIKObj_FontSelector", wo,
                            UIKTAG_OBJ_LeftTop,     SETL(5,4),
                            UIKTAG_OBJ_WidthHeight, SETL(320,97),
                            UIKTAG_FOS_OKFUNC,      OKFunc,
                            UIKTAG_FOS_CANCELFUNC,  CancelFunc,
                            UIKTAG_FOS_HighlightFont, &wo->ObjExt->TTA,
                            TAG_END ))
                    {
                    if (UIK_Start( UIK ))
                        UIK_Do( UIK, SIGBREAKF_CTRL_C );
                    }
                }
            UIK_Remove( UIK );
            }
        CloseLibrary( UIKBase );
        }
}
