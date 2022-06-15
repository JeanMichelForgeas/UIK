/****************************************************************
 *
 *      Project:   UIK
 *      Function:  Test des objets de la librarie
 *
 *      Created:   12/05/90    Jean-Michel Forgeas
 *
 *      Hum, ce programme me sert à tester UIK, et il n'est pas
 *      clair du tout (c'est un vrai fouillis).
 *      Je vous conseille de voir les autres programmes d'exemple
 *      qui ne traitent que trés peu de fonctionnalités à la fois
 *      et qui sont écrits plus clairement.
 *
 ****************************************************************/


/****** Includes ************************************************/

#include <dos/dos.h>
#include <dos/dosextens.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikglobal.h"
#include "uiki:uikbox.h"
#include "uiki:uikcolors.h"
#include "uiki:uikdrawauto.h"
#include "uiki:uikmacros.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "uiki:objreqstring.h"
#include "uiki:objdial.h"
#include "uiki:objmenu.h"
#include "uiki:objlist.h"
#include "uiki:objstring.h"
#include "uiki:objproplift.h"
#include "uiki:objwindow.h"

#ifdef LATTICE
int CXBRK    ( void ) { return(0); }
int chkabort ( void ) { return(0); }
#endif
#ifdef SAS
int CXBRK    ( void ) { return(0); }
int _CXBRK   ( void ) { return(0); }
int chkabort ( void ) { return(0); }
#endif

/****** Imported ************************************************/

extern ULONG DOSBase;


/****** Exported ************************************************/

struct UIKBase * UIKBase;


/****** Statics *************************************************/

static void Options2Menus( struct UIKObjMenu *uom );

static struct UIKGlobal * UIK;

static void (*CleanupVector)();
static struct UIKObjWindow * wo;
static struct UIKObj * wo2, * req, * reqstr, * dial;
static struct UIKObj * ObjUp, * ObjDown, * ListFrame, * OptFrame, * Losange;
static struct UIKObjList * List;
static struct UIKObjStrGad * StrObj;
static struct UIKObjPropGad * LProp;

/* en fait pas activé car test du tag UIKTAG_OBJStrFl_DigitalInt */
static int __asm StrGadgetHandler();

static LONG Result;

static UWORD PatProp1[] = { 0xAAAA, 0x5555 };
static UWORD PatProp2[] = { 0x5555, 0x5555 };
static UWORD Pat1[] = { 0xFFFF, 0x0000 };
static UWORD Pat2[] = { 0x0404, 0x0404, 0x0c0c, 0x1212, 0x2121, 0xc0c0, 0x3030, 0x0808 };
static UWORD Pat3[] = { 0xccc0, 0xd8c0, 0x1818, 0x031b, 0x3303, 0x6030, 0x61b3, 0x0d83 };

static UBYTE * LangEnglish[] =
    {
    "",
    "Waouh!",
    "Requester",
    "Window removed...",
    "No active window",
    " List ",     /* 5 */
    " Option ",
    " Exclude ",
    "Toggle",
    "Active",
    "Start W",     /* 10 */
    "Start R",
    "Start R",
    "Pretty",
    "isn't it",
    "String",      /* 15 */
    "Initial text ATPjgpéâ}",
    "Requester",
    "Cancel",
    "SubW",
    "REMOVE",      /* 20 */
    "Project",
    "Quit",
    "Input",
    "Mouse",
    "Bouris",
    "JoyMouse",
    "1 contact",
    "2 contacts",
    "5 contacts",
    "Holding",     /* 30 */
    "Auto",
    "Type something\n(doesn't matter)",
    0
    };

static UBYTE * LangOther[] =
    {
    "",
    "Waouh!",
    "Requester",
    "Fenêtre enlevée...",
    "Pas de fenêtre active",
    " Liste ",     /* 5 */
    " Option ",
    " Exclue ",
    "Toggle",
    "Active",
    "Start W",     /* 10 */
    "Start R",
    "Start R",
    "Pas mal",
    "c'pas ?",
    "Saisie",      /* 15 */
    "Texte initial ATPjgpéâ}",
    "Requester",
    "Non",
    "Sous F",
    "ENLEVE",      /* 20 */
    "Projet",
    "Quitte",
    "Entrée",
    "Souris",
    "Bouris",
    "JoyMouse",
    "1 contact",
    "2 contacts",
    "5 contacts",
    "Maintien",    /* 30 */
    "Auto",
    "Entrez qlq chose\n(ça ne sert à rien)",
    0
    };

static struct TagItem postl[] = {
    { UIKTAG_OBJ_LeftTop,           0 },
    { UIKTAG_OBJ_WidthHeight,       0 },
    { UIKTAG_OBJ_Sz_Attached_Flags, 0 },
    { TAG_END } };

static struct TagItem titl[] = {
    { UIKTAG_OBJ_Title,         0 },
    { UIKTAG_OBJ_AltTitle,      0 },
    { TAG_IGNORE /*UIKTAG_OBJ_BoxFl_Over*/, FALSE },
    { UIKTAG_OBJ_Title_Flags,   0 },
    { UIKTAG_OBJ_BoxFl_Filled,  TRUE },
    { TAG_MORE,                 (ULONG)postl } };

static struct TagItem pattl[] = {
    { UIKTAG_OBJ_LeftTop,           0 },
    { UIKTAG_OBJ_WidthHeight,       0 },
    { UIKTAG_OBJ_Sz_Attached_Flags, UO_ATTACHED_RIGHT },
    { UIKTAG_OBJ_Pattern,           0 },
    { UIKTAG_OBJ_PatternPower,      1 },
    { UIKTAG_OBJ_BoxFl_Filled,      TRUE },
    { TAG_IGNORE,                   TRUE },
    { TAG_END } };

static struct TagItem reqtl[] = {
    { UIKTAG_OBJ_WidthHeight, SETL(200,90) },
    { UIKTAG_OBJ_Sz_Attached_Flags, UO_ATTACHED_TOP | UO_ATTACHED_BOT | UO_ATTACHED_LEFT | UO_ATTACHED_RIGHT },
    { UIKTAG_OBJ_Title,         0 },
    { UIKTAG_OBJ_ActInactPens, UIKCOLS( UIKCOL_GREY, UIKCOL_GREY, UIKCOL_BLACK, UIKCOL_BLACK ) },
    { TAG_END } };

static struct TagItem booltl[] = {
    { UIKTAG_OBJ_Title,         0 },
    { UIKTAG_OBJ_ActInactPens, UIKCOLS( UIKCOL_GREY, UIKCOL_GREY, UIKCOL_BLACK, UIKCOL_WHITE ) },
    { TAG_MORE,                 (ULONG)postl } };

static struct TagItem wtl[] = {
    { UIKTAG_OBJ_LeftTop,     SETL(120,50) },
    { UIKTAG_OBJ_WidthHeight, SETL(396,141) },
    { UIKTAG_OBJ_Title,       1 },
    { UIKTAG_OBJ_AltTitle,    1 },
    { UIKTAG_OBJ_ActInactPens, UIKCOLS( UIKCOL_LIGHT, UIKCOL_GREY, UIKCOL_WHITE, UIKCOL_BLACK ) },
    { TAG_END } };

#define INITBOXHEIGHT 94

static struct TTextAttr FrameTTA = { "topaz.font", 8, FSF_ITALIC, 0, 0 };
static struct TagItem btl[] = {
    { UIKTAG_OBJ_LeftTop,           SETL(33,27) },
    { UIKTAG_OBJ_WidthHeight,       SETL(149,INITBOXHEIGHT) },
    { UIKTAG_OBJ_Title,             0 },
    { UIKTAG_OBJ_Sz_Attached_Flags, UO_ATTACHED_TOP | UO_ATTACHED_BOT | UO_ATTACHED_LEFT | UO_ATTACHED_RIGHT },
    { UIKTAG_OBJ_BoxFl_Zoneable,    TRUE },
    { UIKTAG_OBJ_TTextAttr,         (ULONG) &FrameTTA },
    { TAG_END } };

static struct TextEntry TextList[] = {
    0,"aaaaaaaaaaaa",
    0,"bbbbbbbbbbbbbbbbb",
    0,"cccccccccccccc",
    0,"dddddddddddddddddddddddddddddddddddddd",
    UIKN_IMAGEFIRST,"eeeeeeeeeeeeeeeeeeeeeeeeeee",
    UIKN_IMAGEFIRST,"ffffffffffffffffffffffffffff",
    UIKN_IMAGEFIRST,"gggggggggggg",
    0,"h",
    0,"iiiiiiiiiiiiiii",
    0,"jjjjjjjjjjjjjjjjjjjjjj",
    0,"kkkkkkkkkkkkkkkkkkkkkkkkk",
    0,"lllllllllllllll",
    0,"mmmmmmmmmmmmmmmmmmmm",
    0,"nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn",
    0,"oooooooooooooooooooo",
    0,"ppppppppppppppppp",
    0,"qqqqqqqqqqqqqqqqqqqqqqq",
    0,"rrrrrrrrrrrrrrrrrrrrrrrrr",
    0,"ssssss",
    0,"ttttttttttttt",
    0,"uuuuuuuu",
    0,"vvvvvvvvvvvvvvvvvvvvv",
    0,"wwwwwwwwwwwwwwwwwww",
    0,"xxxxxxxxxxxxxxxxxxxxxxxxxx",
    0,"yyyyyyyyyy",
    0,"zzzzzzzzzzzzzzzzzzzzzzzz",
    0,0
    };
static struct TextEntry TextList2[] = {
    0,"aaaaaaaaaaaa",
    0,"bbbbbbbbbbbbbbbbb",
    0,"cccccccccccccc",
    0,"dddddddddddddddddddddddddddddddddddddd",
    0,0
    };

static struct TagItem ltl[] = {   /* liste */
    { UIKTAG_OBJ_LeftTop,           SETL(43,39) },
    { UIKTAG_OBJ_WidthHeight,       SETL(100,63) },
    { UIKTAG_OBJ_Title,             5 },
    { UIKTAG_OBJ_Title_Flags,       TITLE_ABOVE },
    { UIKTAG_OBJ_ActInactPens,      UIKCOLS( UIKCOL_GREY, UIKCOL_GREY, UIKCOL_WHITE, UIKCOL_WHITE ) },
    { UIKTAG_OBJ_Sz_Attached_Flags, UO_ATTACHED_TOP | UO_ATTACHED_BOT | UO_ATTACHED_LEFT | UO_ATTACHED_RIGHT },
    { UIKTAG_OBJList_TextEntry,     (ULONG)TextList },
    { UIKTAG_OBJList_TopPosition,   4 },
    { UIKTAG_OBJList_SelectedLine,  6 },
    /* { UIKTAG_OBJList_ListPen,       UIKCOL_WHITE }, */
    { UIKTAG_OBJList_Flags,         UIKL_CLICKSEL | UIKL_SELDISPL },
    { TAG_END } };

static UWORD PropAuto[] =
    {
    X_SUB_1,
    y_ADD_1, Y_SUB_1,
    COL_BLACK, MOVE_Xy, DRAW_XY,
    x_ADD_1, x_ADD_1, X_SUB_1,
    END_AUTO
    };
static struct TagItem ptl[] = {   /* proplift */
    { UIKTAG_OBJ_LeftTop,           SETL(150,39) },
    { UIKTAG_OBJ_WidthHeight,       SETL(20,63) },
    { UIKTAG_OBJ_Sz_Attached_Flags, UO_ATTACHED_TOP | UO_ATTACHED_BOT | UO_ATTACHED_RIGHT },
    { UIKTAG_OBJProp_Vertical,      TRUE },
    { UIKTAG_OBJProp_Total,         0 },
    { UIKTAG_OBJProp_Visible,       0 },
    { UIKTAG_OBJProp_Position,      0 },
    { UIKTAG_OBJ_BoxFl_Over,        FALSE },
    { UIKTAG_OBJ_Pattern,           (ULONG)PatProp2 },
    { UIKTAG_OBJ_PatternPower,      1 },
    { UIKTAG_OBJ_BoxFl_Filled,      TRUE },
    { TAG_IGNORE },
    { TAG_END } };

static struct TagItem strtl[] = {  /* stringgadget */
    { UIKTAG_OBJ_LeftTop,           SETL(90,3) },
    { UIKTAG_OBJ_WidthHeight,       SETL(190,18) },
    { UIKTAG_OBJ_Title,             15 },
    { UIKTAG_OBJ_AltTitle,          1 },
    { UIKTAG_OBJ_ActInactPens,      UIKCOLS( UIKCOL_WHITE, UIKCOL_WHITE, UIKCOL_BLACK, UIKCOL_BLACK ) },
    { UIKTAG_OBJ_Title_Flags,       TITLE_LEFT },
    { UIKTAG_OBJ_Sz_Attached_Flags, UO_ATTACHED_LEFT | UO_ATTACHED_RIGHT },
    { UIKTAG_OBJStr_TextBuffer,     16 },
    { UIKTAG_OBJ_ActInactPens,      UIKCOLS( UIKCOL_BLACK, UIKCOL_LIGHT, UIKCOL_WHITE, UIKCOL_GREY ) },
    /*
    { UIKTAG_OBJStr_Flags,          UIKST_CENTER },
    { UIKTAG_OBJStr_UserHandler,    (ULONG)StrGadgetHandler },
    */
    { UIKTAG_OBJStr_ResultPointer,  (ULONG)&Result },
    { UIKTAG_OBJStrFl_DigitalInt,   TRUE },
    { UIKTAG_OBJStrFl_Activate,     TRUE },
    { TAG_END } };

static struct TagItem tl_dial[] = {  /* dial */
    { UIKTAG_OBJ_LeftTop,           SETL(304,3) },
    { UIKTAG_OBJ_WidthHeight,       SETL(67,33) },
    { UIKTAG_OBJ_Sz_Attached_Flags, UO_ATTACHED_TOP | UO_ATTACHED_RIGHT },
    { UIKTAG_OBJDial_Total,         100 },
    { UIKTAG_OBJDial_Position,      30 },
    { UIKTAG_OBJDialFl_SupHalf,     TRUE },
    { TAG_END } };

static struct TagItem tl_dialdispl[] = {
    { UIKTAG_OBJ_LeftTop,           SETL(0,33) },
    { UIKTAG_OBJ_WidthHeight,       SETL(67,13) },
    { UIKTAG_OBJ_Sz_Attached_Flags, UO_ATTACHED_RIGHT | UO_ATTACHED_TOP },
    { UIKTAG_OBJ_ParentRelative,    TRUE },
    //{ UIKTAG_OBJ_BoxFl_Filled,      TRUE },
    { UIKTAG_OBJ_BoxFl_Over,        FALSE },
    { UIKTAG_OBJ_TitleFl_Addr,      TRUE },
    { UIKTAG_OBJ_Title,             (ULONG)"                    " },
    { UIKTAG_OBJ_ActInactPens,      UIKCOLS(UIKCOL_GREY,UIKCOL_GREY,UIKCOL_BLACK,UIKCOL_BLACK) },
    { TAG_END } };

static struct TagItem tl_reqstr[] = {
    { UIKTAG_ReqString_TextPtr      },
    { UIKTAG_ReqString_StringPtr,   (ULONG)"init string" },
    { UIKTAG_ReqString_CancelFunc,   -1 },
    { TAG_END } };

static UWORD InactLosangeAuto[] =
    {
    COL_BLACK, MOVE_MID_xX_y, DRAW_MID_yY_X, DRAW_MID_xX_Y,
    COL_WHITE, MOVE_MID_xX_y, DRAW_MID_yY_x, DRAW_MID_xX_Y,
    END_AUTO
    };
static UWORD ActLosangeAuto[] =
    {
    COL_WHITE, MOVE_MID_xX_y, DRAW_MID_yY_X, DRAW_MID_xX_Y,
    COL_BLACK, MOVE_MID_xX_y, DRAW_MID_yY_x, DRAW_MID_xX_Y,
    END_AUTO
    };

/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void Project_Quit( struct UIKObjMenu *mo, struct UIKMenuEntry *ume, struct MenuItem *item, UWORD menunum, UWORD itemnum, UWORD subnum )
{
    Signal( FindTask(0), SIGBREAKF_CTRL_C );
}

static void Input_Change( struct UIKObjMenu *uom, struct UIKMenuEntry *ume, struct MenuItem *item, UWORD menunum, UWORD itemnum, UWORD subnum )
{
    UIK_Input_Set( uom->Obj.UIK, (UBYTE)itemnum, (UBYTE)-1, (UBYTE)-1 );
    Options2Menus( uom );
}

static void Input_Holding( struct UIKObjMenu *uom, struct UIKMenuEntry *ume, struct MenuItem *item, UWORD menunum, UWORD itemnum, UWORD subnum )
{
    UIK_Input_Set( uom->Obj.UIK, (UBYTE)-1, (item->Flags & CHECKED) ? 1 : 0, (UBYTE)-1 );
    Options2Menus( uom );
}

static void Input_Auto( struct UIKObjMenu *uom, struct UIKMenuEntry *ume, struct MenuItem *item, UWORD menunum, UWORD itemnum, UWORD subnum )
{
    UIK_Input_Set( uom->Obj.UIK, (UBYTE)-1, (UBYTE)-1, (item->Flags & CHECKED) ? 1 : 0 );
    Options2Menus( uom );
}

static struct UIKMenuEntry __far MenuEntry[] = {
    { 0,  0, 21,        NULL,               0,   0, 0, MENUENABLED },
    { 1,'Q',    22,     Project_Quit,       0,   0, 0, ITEMENABLED },
    { 0,  0, 23,        NULL,               0,   0, 0, MENUENABLED },
    { 1,'S',    24,     Input_Change,       0,0x7e, 0, ITEMENABLED|CHECKIT },
    { 1,'B',    25,     Input_Change,       0,0x7d, 0, ITEMENABLED|CHECKIT },
    { 1,'J',    26,     Input_Change,       0,0x7b, 0, ITEMENABLED|CHECKIT },
    { 1,'1',    27,     Input_Change,       0,0x77, 0, ITEMENABLED|CHECKIT },
    { 1,'2',    28,     Input_Change,       0,0x6f, 0, ITEMENABLED|CHECKIT },
    { 1,'5',    29,     Input_Change,       0,0x5f, 0, ITEMENABLED|CHECKIT },
    { 1,  0,    ITEM_SEPARATION },
    { 1,'M',    30,     Input_Holding,      0,   0, 0, ITEMENABLED|MENUTOGGLE|CHECKIT },
    { 1,  0,    ITEM_SEPARATION },
    { 1,'A',    31,     Input_Auto,         0,   0, 0, ITEMENABLED|MENUTOGGLE|CHECKIT },
    { 0,0,0,0,0,0 },  // marqueur de fin: Title doit être à 0
    };

static void Options2Menus( struct UIKObjMenu *uom )
{
  UBYTE i, input, held, autom;

    UIK_Input_Get( uom->Obj.UIK, &input, &held, &autom );

    for (i=0; i<INPUTMODE_NUMBER; i++) uom->MenuEntry[3+i].Flags &= ~CHECKED;

    uom->MenuEntry[3+input].Flags |= CHECKED;

    if (held) uom->MenuEntry[10].Flags |= CHECKED;
    else uom->MenuEntry[10].Flags &= ~CHECKED;

    if (autom) uom->MenuEntry[12].Flags |= CHECKED;
    else uom->MenuEntry[12].Flags &= ~CHECKED;
}

/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void ToggleGad( struct UIKObjStrGad *obj )
    {
    if (obj->Gad.Flags & SELECTED) printf("Gadget active\n");
    else printf("Gadget inactive\n");
    }
static void StartReqString()
    {
    if (!reqstr->rp) UIK_Start( reqstr );
    }
static void StartRequest()
    {
    if (!req->rp) UIK_Start( req );
    }
static void StopRequest( struct UIKObj *obj )
    {
    if (req->rp)
        {
        UIK_Stop( obj->Parent );
        UIK_OSt_Activate( StrObj );
        }
    }
static void StartWindow( struct UIKObj *gad )
    {
    struct UIKObj *o;
    o = (struct UIKObj *) UIK_AddObjectTags( "UIKObj_Window", gad,
                                UIKTAG_OBJ_LeftTop,     SETL(130,50),
                                UIKTAG_OBJ_WidthHeight, SETL(170,1),
                                UIKTAG_OBJ_Title,       1,
                                UIKTAG_OBJWindowFl_CloseRemove, TRUE,
                                TAG_END );
    if (o) UIK_Start( o );
    }
static void startreq()
    {
    UIK_Start( req );
    }

static void WindowKeys( int nil, struct UIKObjStrGad *obj )
{
  struct IntuiMessage *imsg = obj->Obj.UIK->IntuiMsg;
  char *str;

    str = UIK->KeySupp.KeyBuffer;
    if (str[0] == ' ') UIK_OSt_Activate( obj );
    else printf( "Code=%lx, Qual=%lx, ASCII=%lx: '%lc'\n", imsg->Code, imsg->Qualifier, str[0], str[0] );
}

static int __asm StrGadgetHandler( register __a1 UBYTE *buf )
{
    if (*buf < '0' || *buf > '9') return(0);
    return( (int)buf );
}
static void ObjStringkeys( struct UIKObjStrGad *obj )
{
    struct IntuiMessage *imsg = obj->Obj.UIK->IntuiMsg;
    register UWORD code = imsg->Code;

    if (code & 0x80) return;
    switch (code)
        {
        case 0x43: case 0x44:
            printf( "String number = %ld\n", Result );
            break;
        }
}
static void ObjStringFunc( struct UIKObjStrGad *obj )
    {
    printf( "String object hit!\n" );
    }
static void WActInact()
    {
    printf("Window active or inactive\n");
    }
static void GadgetDown()
    {
    printf("gadget active\n");
    }
static void GadgetUp()
    {
    printf("gadget inactive\n");
    }
static void GadgetMove( struct UIKObj *obj )
    {
    printf("mouse moved %ls\n", obj->MouseIn ? "inside" : "outside" );
    }
static void PropGadgetMove( struct UIKObjPropGad *obj )
    {
    printf("Position=%ld, Total=%ld, Visible=%ld\n", obj->Pos, obj->Tot, obj->Vis );
    }
static void SelectUp( struct UIKObj *obj, struct IntuiMessage **imsg )
    {
    if ((*imsg)->Code == SELECTUP || (*imsg)->Code == MENUUP)
        {
        UIK_OSt_Activate( StrObj );
        }
    else if ((*imsg)->Code == MENUDOWN)
        {
        int i;
        for (i=0; i<400; i+=8)
            {
            UIK_CallObjectFunc( dial, UIKFUNC_Dial_ChangeDialGadget, 100, i );
            }
        }
    }

static void SelectList( struct UIKObjList *obj )
{
  UBYTE *ptr=0;
  WORD sel;

    UIK_LockObjVars( obj );
    sel = obj->Sel;
    printf("-> Sel=%ld",sel);
    if (sel != -1) ptr = UIK_OLi_LineBuffer( obj, sel );
    if (ptr) printf(", Text=%ls",ptr);
    printf("\n");
    if (sel != -1)
        {
        UIK_OLi_DeleteLine( obj, sel );
        /*
        UBYTE *ptr2 = UIK_OLi_LineBuffer( obj, sel );
        UIK_OLi_ChangeLine( obj, ptr2, sel, 0 );
        */
        }
    UIK_UnlockObjVars( obj );
}

static void reqmbuttons() { printf("MOUSEBUTTONS\n"); }
static void reqwindow() { printf("IN/ACTIVEWINDOW\n"); }
static void reqticks() { printf("INTUITICKS\n"); }
static void reqset() { printf("REQSET\n"); }
static void reqclear() { printf("REQCLEAR\n"); }

static void ResizeWindow( struct UIKObjWindow *wo )
{
  int size;

    /*--------- élargir les flêches sous la liste ---------*/
    size = List->Obj.Box.Width;
    ObjDown->Box.Left = ObjUp->Box.Left + size/2;
    ObjUp->Box.Width = ObjDown->Box.Left - ObjUp->Box.Left;
    ObjDown->Box.Width = size - ObjUp->Box.Width;

    (*ObjDown->ResizeVector)( ObjDown );
    (*ObjUp->ResizeVector)( ObjUp );
}

static int lnk_dial2box( struct UIKObjDial *od, struct UIKObj *box )
{
    sprintf( UIK_ObjTitle( box ), "  %ld  ", od->Pos );
    UIK_Refresh( box, 1 );
    //printf("angle=%ld\n",od->Angle);
    return(1);
}

/****************************************************************
 *
 *      Code principal
 *
 ****************************************************************/

static void Cleanup()
{
    if (UIK)            UIK_Remove( UIK );
    if (UIKBase)        CloseLibrary( UIKBase );
    exit(0);
}

void main()
{
  struct UIKObj *obj, *of2;
  struct UIKObjMenu *mo;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) exit(20);

    if (! (UIK = (struct UIKGlobal *) UIK_AddObjectTags( "UIK", 0,
             UIKTAG_GEN_LangEnglish,    (ULONG) LangEnglish,
             UIKTAG_GEN_LangOther,      (ULONG) LangOther,
             TAG_END ))) goto ERROR;

    /*------------fenetre------------*/
    if (! (wo = (struct UIKObjWindow *) UIK_AddObject( "UIKObj_Window", UIK, wtl ))) goto ERROR;
    if (! (mo = (struct UIKObjMenu *) UIK_AddObjectTags( "UIKObj_Menu", wo,
                UIKTAG_Menu_Entry,   (ULONG) MenuEntry,
                TAG_END ))) goto ERROR;
    Options2Menus( mo );
    if (! UIK_AddVector( wo, (void *)reqclear, REQCLEAR, 0 )) goto ERROR;

    /*------------cadres------------*/
    if (! (ListFrame = UIK_AddObject( "UIKObj_Frame", wo, btl ))) goto ERROR;
    btl[0].ti_Data = SETL(196,27); btl[1].ti_Data = SETL(91,INITBOXHEIGHT);
    btl[2].ti_Data = 6; btl[3].ti_Data = UO_ATTACHED_RIGHT;
    if (! (OptFrame = UIK_AddObject( "UIKObj_Frame", wo, btl ))) goto ERROR;
    btl[0].ti_Data = SETL(295,56); btl[1].ti_Data = SETL(91,65); btl[2].ti_Data = 7;
    if (! (of2 = UIK_AddObject( "UIKObj_Frame", wo, btl ))) goto ERROR;

    /*------------liste------------*/
    if (! (List = (struct UIKObjList *) UIK_AddObject( "UIKObj_List", ListFrame, ltl ))) goto ERROR;
    if (! UIK_AddVector( List, (void *)SelectList, GADGETDOWN, 0 )) goto ERROR;

    /*------------prop------------*/
    if (! (LProp = (struct UIKObjProp *) UIK_AddObject( "UIKObj_PropLift", ListFrame, ptl ))) goto ERROR;
    if (! UIK_Link( LProp, List, 0, MOUSEMOVE|GADGETDOWN|GADGETUP, 0 )) goto ERROR;

    ptl[0].ti_Data = SETL(35,125); ptl[1].ti_Data = SETL(350,15);
    ptl[2].ti_Data = UO_ATTACHED_BOT | UO_ATTACHED_LEFT | UO_ATTACHED_RIGHT;
    ptl[3].ti_Data = 0; ptl[4].ti_Data = 100; ptl[5].ti_Data = 20;
    ptl[6].ti_Data = 40; ptl[7].ti_Data = TRUE; ptl[8].ti_Data = 0;
    if (! (obj = UIK_AddObject( "UIKObj_PropLift", wo, ptl ))) goto ERROR;
    if (! UIK_AddVector( obj, (void *)PropGadgetMove, MOUSEMOVE, 0 )) goto ERROR;
    if (! UIK_AddVector( obj, (void *)GadgetDown, GADGETDOWN, 0 )) goto ERROR;
    if (! UIK_AddVector( obj, (void *)GadgetUp, GADGETUP, 0 )) goto ERROR;

    ptl[0].ti_Data = SETL(-2,-1); ptl[1].ti_Data = SETL(25,143);
    ptl[2].ti_Data = UO_ATTACHED_TOP | UO_ATTACHED_BOT;
    ptl[3].ti_Data = 1; ptl[4].ti_Data = 100; ptl[5].ti_Data = 80;
    ptl[6].ti_Data = 10;
    /* ptl[7].ti_Tag = UIKTAG_OBJ_BoxFl_NotDrawn; ptl[7].ti_Data = TRUE; */
    ptl[7].ti_Tag = UIKTAG_OBJ_BoxAutomate; ptl[7].ti_Data = (ULONG)PropAuto;
    ptl[8].ti_Data = (ULONG) PatProp1;
    if (! (obj = UIK_AddObject( "UIKObj_PropLift", wo, ptl ))) goto ERROR;

    /*-----------paire de flêches---------*/
    postl[0].ti_Data = SETL(43,103); postl[1].ti_Data = SETL(50,12);
    postl[2].ti_Data = UO_ATTACHED_BOT;
    if (! (ObjUp = UIK_AddObject( "UIKObj_Up", ListFrame, postl ))) goto ERROR;
    if (! UIK_Link( ObjUp, LProp, 0, GADGETDOWN|INTUITICKS|MOUSEMOVE, 0 )) goto ERROR;

    postl[0].ti_Data = SETL(93,103);
    if (! (ObjDown = UIK_AddObject( "UIKObj_Down", ListFrame, postl ))) goto ERROR;
    if (! UIK_Link( ObjDown, LProp, 0, GADGETDOWN|INTUITICKS|MOUSEMOVE, 0 )) goto ERROR;

    /*-----------colone de gadgets---------*/
    postl[0].ti_Data = SETL(205,34); postl[1].ti_Data = SETL(72,16);
    postl[2].ti_Data = UO_ATTACHED_RIGHT;
    titl[0].ti_Data = 8; titl[1].ti_Data = 9;
    if (! (obj = UIK_AddObject( "UIKObj_Toggle", OptFrame, titl ))) goto ERROR;
    if (! UIK_AddVector( obj, (void *)ToggleGad, GADGETUP, 0 )) goto ERROR;
    titl[1].ti_Data = 0;  /* AltTitle = 0 */

    postl[0].ti_Data = SETL(205,52); titl[0].ti_Data = 10;
    if (! (obj = UIK_AddObject( "UIKObj_Boolean", OptFrame, titl ))) goto ERROR;
    if (! UIK_AddVector( obj, (void *)StartWindow, GADGETUP, 0 )) goto ERROR;

    postl[0].ti_Data = SETL(205,68); titl[0].ti_Data = 11;
    if (! (obj = UIK_AddObject( "UIKObj_Boolean", OptFrame, titl ))) goto ERROR;
    if (! UIK_AddVector( obj, (void *)startreq, GADGETUP, 0 )) goto ERROR;

    postl[0].ti_Data = SETL(205,84); titl[0].ti_Data = 12;
    if (! (obj = UIK_AddObject( "UIKObj_Boolean", OptFrame, titl ))) goto ERROR;
    if (! UIK_AddVector( obj, (void *)StartReqString, GADGETUP, 0 )) goto ERROR;

    /* new.Flags |= BOX_FILLED; */
    postl[0].ti_Data = SETL(205,100); titl[0].ti_Data = 13; titl[1].ti_Data = 14;
    if (! (obj = UIK_AddObject( "UIKObj_Boolean", OptFrame, titl ))) goto ERROR;
    if (! UIK_AddVector( obj, (void *)GadgetDown, GADGETDOWN, 0 )) goto ERROR;
    if (! UIK_AddVector( obj, (void *)GadgetUp, GADGETUP, 0 )) goto ERROR;
    if (! UIK_AddVector( obj, (void *)GadgetMove, MOUSEMOVE, 0 )) goto ERROR;
    titl[1].ti_Data = 0;  /* AltTitle = 0 */

    /*-----------colone de gadgets 2---------*/
    pattl[1].ti_Data = SETL(72,13);
    pattl[0].ti_Data = SETL(304,64); pattl[3].ti_Data = (ULONG)Pat1;
    if (! UIK_AddObject( "UIKObj_MutualExclude", of2, pattl )) goto ERROR;
    pattl[0].ti_Data = SETL(304,77); pattl[3].ti_Data = (ULONG)Pat2;
    pattl[4].ti_Data = 3;
    pattl[6].ti_Tag = UIKTAG_OBJ_BoxFl_Frame;
    if (! UIK_AddObject( "UIKObj_MutualExclude", of2, pattl )) goto ERROR;
    pattl[0].ti_Data = SETL(304,90); pattl[3].ti_Data = (ULONG)Pat3;
    if (! UIK_AddObject( "UIKObj_MutualExclude", of2, pattl )) goto ERROR;

    postl[1].ti_Data = SETL(72,13);
    titl[2].ti_Tag = UIKTAG_OBJ_BoxFl_Over;
    postl[0].ti_Data = SETL(304,103);
    titl[0].ti_Data = (ULONG)"TitreTropLong";
    titl[3].ti_Data = TITLE_ADDR | TITLE_CLIPPED;
    if (! (obj = UIK_AddObject( "UIKObj_MutualExclude", of2, titl ))) goto ERROR;
    if (! UIK_AddVector( obj, (void *)GadgetDown, GADGETDOWN, 0 )) goto ERROR;

    /*------------string gadget------------*/
    if (! (StrObj = (struct UIKObjStrGad *) UIK_AddObject( "UIKObj_String", wo, strtl ))) goto ERROR;
    if (! UIK_AddVector( StrObj, (void *)ObjStringFunc, GADGETDOWN, 0 )) goto ERROR;
    if (! UIK_AddVector( StrObj, (void *)ObjStringkeys, RAWKEY, 0 )) goto ERROR;

    if (! UIK_AddVector( wo, (void *)SelectUp, MOUSEBUTTONS, (ULONG)&UIK->IntuiMsg )) goto ERROR;
    if (! UIK_AddVector( wo, (void *)GadgetMove, MOUSEMOVE, 0 )) goto ERROR;
    if (! UIK_AddVector( wo, (void *)WActInact, ACTIVEWINDOW|INACTIVEWINDOW, 0 )) goto ERROR;
    if (! UIK_AddVector( wo, (void *)WindowKeys, RAWKEY, (ULONG)StrObj )) goto ERROR;
    if (! UIK_AddVector( wo, (void *)ResizeWindow, NEWSIZE, 0 )) goto ERROR;

    /*------------dial gadget------------*/
    if (! (dial = UIK_AddObject( "UIKObj_Dial", wo, tl_dial ))) goto ERROR;
    if (! (of2 = UIK_AddObject( "UIKObj_Box", dial, tl_dialdispl ))) goto ERROR;
    if (! UIK_AddVector( dial, lnk_dial2box, GADGETDOWN|MOUSEMOVE, (ULONG)of2 )) goto ERROR;

    if (! UIK_Start( UIK )) goto ERROR;
    UIK_CauseObjHook( dial, GADGETDOWN );

    /*------------requester------------*/
    if (! (req = UIK_AddObject( "UIKObj_Request", wo, reqtl ))) goto ERROR;
    if (! UIK_AddVector( req, (void *)GadgetMove, MOUSEMOVE, 0 )) goto ERROR;
    if (! UIK_AddVector( req, (void *)reqmbuttons, MOUSEBUTTONS, 0 )) goto ERROR;
    if (! UIK_AddVector( req, (void *)reqwindow, ACTIVEWINDOW, 0 )) goto ERROR;
    if (! UIK_AddVector( req, (void *)reqwindow, INACTIVEWINDOW, 0 )) goto ERROR;
    if (! UIK_AddVector( req, (void *)reqset, REQSET, 0 )) goto ERROR;
    /*
    if (! UIK_AddVector( req, (void *)reqticks, INTUITICKS, 0 )) goto ERROR;
    */

    postl[0].ti_Data = SETL(60,70); postl[1].ti_Data = SETL(80,15);
    postl[2].ti_Data = UO_ATTACHED_BOT | UO_ATTACHED_LEFT | UO_ATTACHED_RIGHT;
    booltl[0].ti_Data = 18;
    if (! (obj = UIK_AddObject( "UIKObj_Boolean", req, booltl ))) goto ERROR;
    if (! UIK_AddVector( obj, (void *)StopRequest, GADGETUP, 0 )) goto ERROR;
    ltl[0].ti_Data = SETL(20,5);
    ltl[1].ti_Data = SETL(160,60);
    ltl[2].ti_Data = 0;
    ltl[6].ti_Data = (ULONG) TextList2;
    if (! (obj = (struct UIKObjList *) UIK_AddObject( "UIKObj_List", req, ltl ))) goto ERROR;
    if (! UIK_AddVector( obj, (void *)SelectList, GADGETDOWN, 0 )) goto ERROR;

    tl_reqstr[0].ti_Data = (ULONG) UIK_LangString( UIK, 32 );
    if (! (reqstr = UIK_AddObject( "UIKObj_ReqString", wo, tl_reqstr ))) goto ERROR;

    CleanupVector = Cleanup;
    if (UIK_AddVector( UIK, (void *)&CleanupVector, SIGBREAKF_CTRL_E, 0 ))
        UIK_Do( UIK, SIGBREAKF_CTRL_C );

    ERROR: Cleanup();
}
