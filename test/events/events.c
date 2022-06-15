/****************************************************************
 *
 *      File:      events.c
 *      Project:   UIK example code
 *
 *      Created:   09-05-94     Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include <stdio.h>

#include <libraries/dos.h>
#include <intuition/intuition.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikmacros.h"
#include "uiki:uikglobal.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

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


/****** Imported ************************************************/

extern ULONG DOSBase;


/****** Exported ***********************************************/

struct UIKBase *UIKBase;
struct UIKGlobal *UIK;


/****** Statics ************************************************/

static UBYTE debug_active = 0xff;


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

UBYTE *class_name( ULONG class )
{
  UBYTE *name;

    switch (class)
        {
        case IDCMP_SIZEVERIFY    : name = "IDCMP_SIZEVERIFY    "; break;
        case IDCMP_NEWSIZE       : name = "IDCMP_NEWSIZE       "; break;
        case IDCMP_REFRESHWINDOW : name = "IDCMP_REFRESHWINDOW "; break;
        case IDCMP_MOUSEBUTTONS  : name = "IDCMP_MOUSEBUTTONS  "; break;
        case IDCMP_MOUSEMOVE     : name = "IDCMP_MOUSEMOVE     "; break;
        case IDCMP_GADGETDOWN    : name = "IDCMP_GADGETDOWN    "; break;
        case IDCMP_GADGETUP      : name = "IDCMP_GADGETUP      "; break;
        case IDCMP_REQSET        : name = "IDCMP_REQSET        "; break;
        case IDCMP_MENUPICK      : name = "IDCMP_MENUPICK      "; break;
        case IDCMP_CLOSEWINDOW   : name = "IDCMP_CLOSEWINDOW   "; break;
        case IDCMP_RAWKEY        : name = "IDCMP_RAWKEY        "; break;
        case IDCMP_REQVERIFY     : name = "IDCMP_REQVERIFY     "; break;
        case IDCMP_REQCLEAR      : name = "IDCMP_REQCLEAR      "; break;
        case IDCMP_MENUVERIFY    : name = "IDCMP_MENUVERIFY    "; break;
        case IDCMP_NEWPREFS      : name = "IDCMP_NEWPREFS      "; break;
        case IDCMP_DISKINSERTED  : name = "IDCMP_DISKINSERTED  "; break;
        case IDCMP_DISKREMOVED   : name = "IDCMP_DISKREMOVED   "; break;
        case IDCMP_WBENCHMESSAGE : name = "IDCMP_WBENCHMESSAGE "; break;
        case IDCMP_ACTIVEWINDOW  : name = "IDCMP_ACTIVEWINDOW  "; break;
        case IDCMP_INACTIVEWINDOW: name = "IDCMP_INACTIVEWINDOW"; break;
        case IDCMP_DELTAMOVE     : name = "IDCMP_DELTAMOVE     "; break;
        case IDCMP_VANILLAKEY    : name = "IDCMP_VANILLAKEY    "; break;
        case IDCMP_INTUITICKS    : name = "IDCMP_INTUITICKS    "; break;
        case IDCMP_IDCMPUPDATE   : name = "IDCMP_IDCMPUPDATE   "; break;
        case IDCMP_MENUHELP      : name = "IDCMP_MENUHELP      "; break;
        case IDCMP_CHANGEWINDOW  : name = "IDCMP_CHANGEWINDOW  "; break;
        case IDCMP_GADGETHELP    : name = "IDCMP_GADGETHELP    "; break;
        default                  : name = "IDCMP non répertorié"; break;
        }
    return( name );
}

static ULONG FilterIE( struct UIKObj *obj, ULONG class, UWORD qual, UWORD code )
{
    /* "obj" est l'objet vers lequel UIK dirigera l'évènement
     * une fois qu'il sera en cours de traitement.
     * Si cette fonction retourne 0 l'évènement ne sera pas
     * traité : UIK fera tout de suite ReplyMsg().
     *
     * NB : certains évènements peuvent être transformés par UIK,
     * comme les raccourcis clavier d'activation de boutons.
     * Si vous voulez avoir accès au message Intuition avant
     * cette transformation, lisez directement obj->UIK->IntuiMsg.
     */

    if ((class == IDCMP_RAWKEY) && !(code & IECODE_UP_PREFIX))
        {
        if ((qual & IEQUALIFIER_CONTROL) && (code == 0x45)) /* CTRL-Escape */
            debug_active = ~debug_active;
        }

    if (debug_active && (class != IDCMP_INTUITICKS))
        {
        printf( "Evènement=%ls Qual=%4.4lx Code=%4.4lx Objet=%ls\n",
                class_name( class ),
                qual,
                code,
                obj->RegGerm->Germ->ObjName
                );
        }

    return(1);
}

static void Cleanup()
{
    if (UIK)     UIK_Remove( UIK );
    if (UIKBase) CloseLibrary( UIKBase );
    exit(0);
}

void main()
{
  struct UIKObj *wo;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (UIK = (struct UIKGlobal *) UIK_AddObjectTags( "UIK", 0,
            UIKTAG_GEN_FilterIEventsFunc,   FilterIE,   /* ««««««««« c'est là! */
            TAG_END ))) goto END_ERROR;

    if (! (wo = UIK_AddObjectTags( "UIKObj_Window", UIK, /* fenêtre */
            UIKTAG_OBJ_LeftTop,         W2L(180,30),
            UIKTAG_OBJ_WidthHeight,     W2L(250,104),
            TAG_END ))) goto END_ERROR;

    if (! (UIK_AddObjectTags( "UIKObj_Boolean", wo, /* bouton */
            UIKTAG_OBJ_LeftTop,         W2L(20,13),
            UIKTAG_OBJ_WidthHeight,     W2L(50,15),
            UIKTAG_OBJ_Title,           "_XXX",
            UIKTAG_OBJ_TitleFl_Addr,    TRUE,
            UIKTAG_OBJ_TitleUnderscore, '_',
            UIKTAG_OBJ_BoxFl_Filled,    TRUE,
            TAG_END ))) goto END_ERROR;

    if (! UIK_Start( UIK )) goto END_ERROR;

    UIK_Do( UIK, SIGBREAKF_CTRL_C );

  END_ERROR:
    Cleanup();
}
