/****************************************************************
 *
 *      File:      ListStr.c
 *      Project:   test
 *
 *      Created:   08-04-92     Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include <stdio.h>

#include <libraries/dos.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikmacros.h"
#include "uiki:uikdrawauto.h"
#include "uiki:uikglobal.h"
#include "uiki:uikcolors.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objwindow.h"
#include "uiki:objlist.h"
#include "uiki:objstring.h"

#define TEXT_WindowTitle    1
#define TEXT_ScreenTitle    2
#define TEXT_ListTitle      3
#define TEXT_StringTitle    4
#define TEXT_ListEntry      5

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

static UBYTE *LangEnglish[] =
    {
    "",
    "ListStr",
    "ListStr Copyright © 1992 Jean-Michel Forgeas",
    "The List",
    "The String",
    "Hello1\nHello2\nHello3\nHello4\nHello5\nHello6\nHello7\nHello8\nHello9\n",
    0
    };

static UBYTE *LangOther[] =
    {
    "",
    "ListStr",
    "ListStr Copyright © 1992 Jean-Michel Forgeas",
    "La liste",
    "La chaine",
    "bonjour1\n"       /* tous les "bonjourN\n" sont concaténés par le compilateur */
        "bonjour2\n"
        "bonjour3\n"
        "bonjour4\n"
        "bonjour5\n"
        "bonjour6\n"
        "bonjour7\n"
        "bonjour8\n"
        "bonjour9\n",
    0
    };

static UWORD AutoBlackRect[] = { COL_PEN, RECTFILL, x_ADD_N, 2, X_SUB_N, 2, y_ADD_1, Y_SUB_1, COL_BLACK, END_AUTO };
static UWORD AutoWhiteRect[] = { COL_PEN, RECTFILL, x_ADD_N, 2, X_SUB_N, 2, y_ADD_1, Y_SUB_1, COL_LIGHT, END_AUTO };

#include "uiki:uikgfx.h"

static UWORD chip BrUpData[5][5][1] =
   {
   0x0c00,    /* ....##.......... */
   0x1e00,    /* ...####......... */
   0x3300,    /* ..##..##........ */
   0x6180,    /* .##....##....... */
   0xc0c0,    /* ##......##...... */

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

   0x0c00,    /* ....##.......... */
   0x1e00,    /* ...####......... */
   0x3300,    /* ..##..##........ */
   0x6180,    /* .##....##....... */
   0xc0c0,    /* ##......##...... */
   };
static struct UIKBitMap UBM_BrUp = { &BrUpData[4][0][0], UIKBMAP_MASKED|UIKBMAP_CONTIG,
    10, 2,5, 0,4,0, &BrUpData[0][0][0], &BrUpData[1][0][0], &BrUpData[2][0][0], &BrUpData[3][0][0], 0, 0, 0, 0 };

static UWORD chip BrDownData[5][5][1] =
   {
   0xc0c0,    /* ##......##...... */
   0x6180,    /* .##....##....... */
   0x3300,    /* ..##..##........ */
   0x1e00,    /* ...####......... */
   0x0c00,    /* ....##.......... */

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

   0xc0c0,    /* ##......##...... */
   0x6180,    /* .##....##....... */
   0x3300,    /* ..##..##........ */
   0x1e00,    /* ...####......... */
   0x0c00,    /* ....##.......... */
   };
static struct UIKBitMap UBM_BrDown = { &BrDownData[4][0][0], UIKBMAP_MASKED|UIKBMAP_CONTIG,
    10, 2,5, 0,4,0, &BrDownData[0][0][0], &BrDownData[1][0][0], &BrDownData[2][0][0], &BrDownData[3][0][0], 0, 0, 0, 0 };


static struct UIKBitMap *UIKBitMapTable[] = {
    &UBM_BrUp,
    &UBM_BrDown,
    0
    };

/****************************************************************
 *
 *      Vectors
 *
 ****************************************************************/

static void String2List( struct UIKObjStrGad *ostr, struct UIKObjList *olist )
{
  ULONG len;
  UBYTE *ptr, *nul="";
  struct IntuiMessage *imsg = ostr->Obj.UIK->IntuiMsg;

    if (imsg->Code & 0x80) return; /* touche relevée */

    if (ostr->Obj.UIK->KeySupp.KeyBuffer[0] == 0x0d)  /* touche RETURN */
        {
        UIK_LockObjVars( ostr );
        ptr = UIK_OSt_Get( ostr, &len ); /* ptr = buffer dynamique de l'objet UIKObj_String */
            {
            if (len) UIK_OLi_AddLine( olist, ptr, -1, 0 );  /* vers la liste */
            UIK_OSt_Set( ostr, nul, 0 );            /* efface le buffer */
            }
        UIK_UnlockObjVars( ostr );

        UIK_ActivateKeyTank( ostr );  /* remet le curseur */
        }
}

static void List2String( struct UIKObjList *olist, struct UIKObjStrGad *ostr )
{
  UBYTE *ptr=0;
  WORD sel;

    sel = olist->Sel;
    if (sel != -1) ptr = UIK_OLi_LineBuffer( olist, sel ); /* retourne le buffer de la ligne indiquée */
    if (ptr)
        UIK_OSt_Set( ostr, ptr, StrLen( ptr ) );
}

/****************************************************************
 *
 *      Code principal
 *
 ****************************************************************/

static void Cleanup()
{
    if (UIK)     UIK_Remove( UIK );
    if (UIKBase) CloseLibrary( UIKBase );
    exit(0);
}

void main()
{
  struct UIKObj *wo;
  struct UIKObj *olist;
  struct UIKObjStrGad *ostr;
  struct UIKObj *oprop, *up, *down;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) goto END_ERROR;
    if (! (UIK = (struct UIKGlobal *) UIK_AddObjectTags( "UIK", 0,
             UIKTAG_GEN_LangEnglish,    (ULONG) LangEnglish,
             UIKTAG_GEN_LangOther,      (ULONG) LangOther,
             TAG_END ))) goto END_ERROR;

    UIK_InvertBlackWhite( UIK, UIKBitMapTable, 1 );

    if (! (wo = UIK_AddObjectTags( "UIKObj_Window", UIK,
            UIKTAG_OBJ_LeftTop,       SETL(180,30),
            UIKTAG_OBJ_WidthHeight,   SETL(300,113),
            UIKTAG_OBJ_Title,         TEXT_WindowTitle,
            UIKTAG_OBJ_AltTitle,      TEXT_ScreenTitle,
            TAG_END ))) goto END_ERROR;

    if (! (olist = UIK_AddObjectTags( "UIKObj_List", wo,
            UIKTAG_OBJ_LeftTop,       SETL(10,15),
            UIKTAG_OBJ_WidthHeight,   SETL(200,70),
            UIKTAG_OBJ_Title,         TEXT_ListTitle,
            UIKTAG_OBJ_TitleFl_Above, TRUE,
            UIKTAG_OBJListFl_ClickSel,TRUE,
            UIKTAG_OBJListFl_SelDispl,TRUE,
            UIKTAG_OBJListFl_Sorted,  TRUE,
            UIKTAG_OBJList_TextPtr,   (ULONG) UIK_LangString( UIK, TEXT_ListEntry ),
            UIKTAG_OBJ_Sz_AttachedFl_Top,   TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Left,  TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Right, TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Bot,   TRUE,
            TAG_END ))) goto END_ERROR;

    if (! (oprop = UIK_AddObjectTags( "UIKObj_PropLift", olist,
            UIKTAG_OBJ_LeftTop,             SETL(209,15),
            UIKTAG_OBJ_WidthHeight,         SETL(20,50),
            UIKTAG_OBJ_Sz_AttachedFl_Right, TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Top,   TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Bot,   TRUE,
            UIKTAG_OBJProp_Vertical,        TRUE,
            UIKTAG_OBJProp_KnobAutomate,    AutoBlackRect,
            UIKTAG_OBJProp_KnobAltAutomate, AutoWhiteRect,
            UIKTAG_OBJProp_KnobImage1,      0,
            UIKTAG_OBJProp_KnobSpImage1,    0,
            UIKTAG_OBJProp_KnobImage2,      0,
            UIKTAG_OBJProp_KnobSpImage2,    0,
            TAG_END ))) goto END_ERROR;
    UIK_Link( oprop, olist, 0, MOUSEMOVE|GADGETDOWN, 0 );

    if (! (up = UIK_AddObjectTags( "UIKObj_Up", oprop,
            UIKTAG_OBJ_ParentRelative,  TRUE,
            UIKTAG_OBJ_LeftTop,         SETL(1,50),
            UIKTAG_OBJ_WidthHeight,     SETL(18,10),
            UIKTAG_OBJ_Title,           &UBM_BrUp,
            UIKTAG_OBJ_TitleFl_Image,   TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Right, TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Bot,   TRUE,
            TAG_END ))) goto END_ERROR;
    UIK_Link( up, oprop, 0, MOUSEMOVE|GADGETDOWN|INTUITICKS, 0 );

    if (! (down = UIK_AddObjectTags( "UIKObj_Down", oprop,
            UIKTAG_OBJ_ParentRelative,  TRUE,
            UIKTAG_OBJ_LeftTop,         SETL(1,60),
            UIKTAG_OBJ_WidthHeight,     SETL(18,10),
            UIKTAG_OBJ_Title,           &UBM_BrDown,
            UIKTAG_OBJ_TitleFl_Image,   TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Right, TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Bot,   TRUE,
            TAG_END ))) goto END_ERROR;
    UIK_Link( down, oprop, 0, MOUSEMOVE|GADGETDOWN|INTUITICKS, 0 );

    if (! (ostr = (struct UIKObjStrGad *) UIK_AddObjectTags( "UIKObj_String", olist,
            UIKTAG_OBJ_LeftTop,             SETL(10,90),
            UIKTAG_OBJ_WidthHeight,         SETL(200,20),
            UIKTAG_OBJ_Title,               TEXT_StringTitle,
            UIKTAG_OBJ_TitleFl_Right,       TRUE,
            UIKTAG_OBJStr_TextPointer,      "initstring",
            UIKTAG_OBJ_Sz_AttachedFl_Left,  TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Right, TRUE,
            UIKTAG_OBJ_Sz_AttachedFl_Bot,   TRUE,
            TAG_END ))) goto END_ERROR;

    UIK_AddVector( olist, (void*)List2String, GADGETDOWN, (ULONG)ostr );
    UIK_AddVector( ostr, (void*)String2List, RAWKEY, (ULONG)olist );

    if (! UIK_Start( UIK )) goto END_ERROR;

    UIK_Do( UIK, SIGBREAKF_CTRL_C );
kprintf( "result=%ld\n", result );

  END_ERROR:
    Cleanup();
}
