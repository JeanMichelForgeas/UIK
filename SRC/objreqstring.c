/****************************************************************
 *
 *      Project:   UIK
 *      Function:  UIKObj_ReqString
 *
 *      Created:   17/05/91    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikbase.h"
#include "uiki:uikglobal.h"
#include "uiki:uikobj.h"
#include "uiki:uikdrawauto.h"
#include "uiki:uikbox.h"
#include "uiki:uikmacros.h"
#include "uiki:uikcolors.h"

#include "lci:uik_protosint.h"

#include "uiki:objstring.h"
#include "uiki:objreqstring.h"
#include "uiki:objlist.h"
#include "uiki:objwindow.h"


/****** Macros **************************************************/


/****** Imported ************************************************/

extern struct UIKBase * __far UIKBase;

extern UWORD __far AutoDrawBox_Over[], __far AutoDrawBox_Light[], __far AutoDrawReqString[];


/****** Exported ************************************************/


/****** Statics *************************************************/

static UWORD __far NullAuto[] = {
    END_AUTO
    };
static UWORD __far TitleBarAutoU[] = {
    COL_WHITE, MOVE_xY, DRAW_xy, DRAW_Xy, x_ADD_1,
    COL_BLACK,
    MOVE_xY, DRAW_XY, DRAW_Xy,
    END_AUTO
    };
static UWORD __far TitleBarAutoD[] = {
    IF_LOOK_SKIP_N, 6,
        COL_BLACK,
        SKIP_N, 2,
        COL_LOOK,
    MOVE_xy, DRAW_Xy,
    COL_WHITE, MOVE_xY, DRAW_XY,
    END_AUTO
    };

static struct TagItem __far flags_boolmap[] =
    {
    { UIKTAG_ReqStringFl_Window,        ORSF_WINDOW },
    { UIKTAG_ReqStringFl_Activate,      ORSF_ACTIVATE },
    { UIKTAG_ReqStringFl_StringQuiet,   ORSF_RETQUIET },
    { TAG_DONE }
    };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void default_gads( int nil, struct UIKObjReqString *rs )
{
    UIK_Stop( rs );
}

static void ReqKeyFunc( struct UIKObj *obj, struct UIKObjReqString *rs )
{
    switch (rs->Obj.UIK->IntuiMsg->Code)
        {
        case 0x43:                                                      /* ent */
        case 0x44: if (rs->OKFunc) (*rs->OKFunc)( obj, rs );            /* ret */
            break;

        case 0x45: if (rs->CancelFunc) (*rs->CancelFunc)( obj, rs );    /* esc */
            break;

        default:
            break;
        }
}

static void StrKeyFunc( struct UIKObj *obj, struct UIKObjReqString *rs )
{
    switch (rs->Obj.UIK->IntuiMsg->Code)
        {
        case 0x44:
            if (obj == rs->Str)
                if (rs->Str2)
                    {
                    UIK_ActivateKeyTank( rs->Str2 );
                    break;
                    }
        case 0x43:                                                      /* ent */
            if (rs->OKFunc && !(rs->Flags & ORSF_RETQUIET)) (*rs->OKFunc)( obj, rs );            /* ret */
            break;

        default:
            break;
        }
}

static int StartAfterReqString( struct UIKObjReqString *rs )
{
    if (rs->Str && (rs->Flags & ORSF_ACTIVATE)) UIK_ActivateKeyTank( rs->Str );
    return(1);
}

static ULONG verif_title_width( ULONG *func, UBYTE *title, struct TextFont *font )
{
  ULONG w=0;

    if (*func == -1) *func = (ULONG) default_gads;

    if (*func)
        {
        if (title && *title)
            {
            w = UIK_StrWidth( font, title, StrLen( title ) ) + 8 + 2 * UIK_CharWidth( font, ' ' );
            }
        else w = 40;
        }
    return(w);
}

static ULONG strtitle_width( struct UIKObj *rs, struct TextFont *font, struct TagItem *taglist )
{
  ULONG w=0;

    if (taglist)
        {
        UBYTE *ptr = (UBYTE *) UIK_GetTagData( UIKTAG_OBJ_Title, 0, taglist );
        if (! UIK_GetTagData( UIKTAG_OBJ_TitleFl_Addr, 0, taglist ))
            ptr = (UBYTE *) UIK_LangString( rs, (ULONG)ptr );
        if (ptr) w = 5 + UIK_StrWidth( font, ptr, StrLen( ptr ) );
        }
    return(w);
}

int AddReqString( struct UIKObjReqString *rs, struct TagItem *taglist )
{
  ULONG TEntry, TEntryE, TEntryO=0, nlines=0, fh, listwidth, stringptr1, stringptr2;
  ULONG height_deb, height_title, height_list, height_strings, height_sep, height_gads;
  ULONG after_title, after_list, after_strings, after_sep;
  ULONG minispace, smallspace, bigspace;
  struct TextEntry *te;
  struct UIKObj *req, *o;
  ULONG lefttop=-1, tag=TAG_IGNORE;
  struct TextFont *font = rs->Obj.ObjExt->Font;
  ULONG iswindow, istitle, islist, len, maxwidth=100;
  ULONG gadwidth, widthok, widthmid, widthcan;
  UBYTE *titleok=0, *titlemid=0, *titlecan=0;
  ULONG YY, HH, leftoffs, widthoffs;
  void (*closefunc)()=0;
  struct TagItem TL[] = {
        { UIKTAG_OBJ_LeftTop            },
        { UIKTAG_OBJ_WidthHeight,       },
        { UIKTAG_OBJ_Title,             },
        { 0,      TRUE },
        { UIKTAG_OBJ_BoxFl_Filled,      TRUE },
        { UIKTAG_OBJ_ActInactPens,      UIKCOLS(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_BLACK,UIKCOL_WHITE) },
        { UIKTAG_OBJ_BoxAutomate,       (ULONG) AutoDrawBox_Over },
        { UIKTAG_OBJ_BoxAltAutomate,    (ULONG) AutoDrawBox_Light },
        { UIKTAG_OBJ_TitleUnderscore,   (ULONG) '_' },
        { TAG_END } };

    rs->Flags = UIK_PackBoolTags( ORSF_ACTIVATE, taglist, flags_boolmap );
    if (UIK_FindTagItem( UIKTAG_OBJ_LeftTop, taglist )) lefttop = *((ULONG*)&rs->Obj.Box.Left);

    stringptr1 = UIK_GetTagData( UIKTAG_ReqString_StringPtr, 0, taglist );
    stringptr2 = UIK_GetTagData( UIKTAG_ReqString_StringPtr2, 0, taglist );
    iswindow = rs->Flags & ORSF_WINDOW;
    istitle = rs->Obj.Title;
    islist = 1;
    if (TEntryE = UIK_GetTagData( (UIKTAG_USER|0x0003), 0, taglist )) // compatibilité pré 1.0
        { if (! (TEntryO = UIK_GetTagData( (UIKTAG_USER|0x0004), 0, taglist ))) islist = 0; }
    else if (! (TEntry = UIK_GetTagData( UIKTAG_ReqString_TextPtr, 0, taglist ))) islist = 0;

    fh = font->tf_YSize;
    if (rs->Obj.AutoRatio == 1)
         { minispace = 3; smallspace = 4; bigspace = 7; }
    else { minispace = 2; smallspace = 3; bigspace = 5; }

    height_deb = minispace;
    height_title = after_title = 0;
    height_list = after_list = 0;
    height_strings = after_strings = 0;
    height_sep = 2; after_sep = minispace;
    height_gads = fh + 5;

    /*--------------titre */
    if (!iswindow && istitle)
        {
        UBYTE *str = UIK_ObjTitle( rs );
        maxwidth = UIK_StrWidth( font, str, StrLen( str ) );
        height_title = fh + minispace + 2;
        after_title = bigspace;
        }

    /*--------------liste */
    if (islist)
        {
        if (TEntryO) // old style
            {
            TEntry = (UIKBase->UIKPrefs->PrefsFlags & UIKLANG_OTHER ? TEntryO : TEntryE);
            for (te=(struct TextEntry *)TEntry; te->String; nlines++, te++);
            tag = UIKTAG_OBJList_TextEntry;
            }
        else if (TEntry)
            {
            UBYTE *str, *ptr;
            ptr = str = (UBYTE*) TEntry;
            while (*ptr)
                {
                len = 0;
                while (*ptr && *ptr != '\n')
                    {
                    len += UIK_CharWidth( font, *ptr );
                    ptr++;
                    }
                if (len > maxwidth) maxwidth = len;
                nlines++;
                if (*ptr) ptr++; // skip newline
                str = ptr;
                }
            tag = UIKTAG_OBJList_TextPtr;
            }
        height_list = nlines * fh;
        after_list = smallspace;
        if (!height_title) height_deb = after_list;
        }

    /*--------------strings */
    if (stringptr1 || stringptr2)
        {
        if (!height_list && !height_title) height_deb = bigspace;
        height_strings = fh + 5;
        if (stringptr1 && stringptr2) height_strings = height_strings * 2 + smallspace;
        after_strings = bigspace;
        }

    /*------------hauteur du requester */
    YY = HH = (iswindow?0:2) + height_deb;

    HH += height_title + after_title +
          height_list + after_list +
          height_strings + after_strings +
          height_sep + after_sep +
          height_gads;

    HH += minispace + (iswindow?0:2);

    rs->Obj.Box.Height = HH;

    /*------------largeur du requester */
    rs->OKFunc = (void*) UIK_GetTagData( UIKTAG_ReqString_OKFunc, (ULONG)default_gads, taglist );
    rs->MiddleFunc = (void*) UIK_GetTagData( UIKTAG_ReqString_MiddleFunc, 0, taglist );
    rs->CancelFunc = (void*) UIK_GetTagData( UIKTAG_ReqString_CancelFunc, 0, taglist );

    if (! (titleok = (UBYTE*)UIK_LangString( rs, UIK_GetTagData( UIKTAG_ReqString_OKTitle, 0, taglist ) )))
        titleok = (UBYTE*)UIK_GetTagData( UIKTAG_ReqString_OKTitlePtr, 0, taglist );
    if (! (titlemid = (UBYTE*)UIK_LangString( rs, UIK_GetTagData( UIKTAG_ReqString_MiddleTitle, 0, taglist ) )))
        titlemid = (UBYTE*)UIK_GetTagData( UIKTAG_ReqString_MiddleTitlePtr, 0, taglist );
    if (! (titlecan = (UBYTE*)UIK_LangString( rs, UIK_GetTagData( UIKTAG_ReqString_CancelTitle, 0, taglist ) )))
        titlecan = (UBYTE*)UIK_GetTagData( UIKTAG_ReqString_CancelTitlePtr, 0, taglist );

    widthok = verif_title_width( (ULONG*)&rs->OKFunc, titleok, font );
    widthmid = verif_title_width( (ULONG*)&rs->MiddleFunc, titlemid, font );
    widthcan = verif_title_width( (ULONG*)&rs->CancelFunc, titlecan, font );

    gadwidth = widthok;
    if (widthmid) { if (gadwidth < widthmid) gadwidth = widthmid; else widthmid = gadwidth; }
    if (widthcan) { if (gadwidth < widthcan) gadwidth = widthcan; else widthcan = gadwidth; }

    if (gadwidth > widthok) widthok = gadwidth;
    if (widthmid) { if (gadwidth > widthmid) widthmid = gadwidth; }
    if ((widthok+widthmid+widthcan+16) > maxwidth) maxwidth = (widthok+widthmid+widthcan+16);

    maxwidth += (20 + 10);
    if (rs->Obj.Box.Width < maxwidth) rs->Obj.Box.Width = maxwidth;

    /*------------ajout du requester------------*/
    leftoffs = iswindow ? 0 : 2;
    widthoffs = iswindow ? 0 : 4;

    if (iswindow)
        {
        struct Window *w = rs->Obj.Parent->w;
        struct UIKObj *to = rs;
        WORD left=0, top=0;

        if (w)
            {
            if (! istitle) to = (struct UIKObj *) w->UserData;
            if (lefttop == -1)
                {
                left = w->LeftEdge + (w->Width - rs->Obj.Box.Width) / 2;
                top = w->TopEdge + (w->Height - rs->Obj.Box.Height) / 2;
                }
            }
        if (! (req = (struct UIKObj *) IntUIK_AddObjectTags( "UIKObj_Window", rs,
                UIKTAG_OBJ_LeftTop,     lefttop == -1 ? W2L(left,top) : lefttop,
                UIKTAG_OBJ_WidthHeight, *((ULONG*)&rs->Obj.Box.Width),
                UIKTAG_OBJ_LangMaster,  to->ObjExt->Lang->Master,
                UIKTAG_OBJ_Title,       to->Title,
                UIKTAG_OBJ_AltTitle,    to->AltTitle,
                UIKTAG_OBJ_Title_Flags, to->TitleFlags,
                UIKTAG_OBJWindowFl_With_Size, FALSE,
                UIKTAG_OBJWindowFl_With_Icon, FALSE,
                UIKTAG_OBJWindowFl_CloseStop, FALSE,
                UIKTAG_OBJWindowFl_AutoShrink, TRUE,
                TAG_END ))) goto END_ERROR;
        if (closefunc = (rs->CancelFunc) ? rs->CancelFunc : UIK_GetTagData( UIKTAG_ReqString_CloseFunc, 0, taglist ))
            if (! UIK_AddVector( req, (void*)closefunc, CLOSEWINDOW, (ULONG)rs )) goto END_ERROR;
        }
    else{
        if (! (req = (struct UIKObj *) IntUIK_AddObjectTags( "UIKObj_Request", rs,
                UIKTAG_OBJ_WidthHeight, *((ULONG*)&rs->Obj.Box.Width),
                UIKTAG_OBJ_LangMaster,  rs->Obj.ObjExt->Lang->Master,
                UIKTAG_OBJ_BoxAutomate, AutoDrawReqString,
                TAG_END ))) goto END_ERROR;
        if (istitle)
            {
            if (! IntUIK_AddObjectTags( "UIKObj_Box", req,
                    UIKTAG_OBJ_LeftTop,     W2L(8,YY),
                    UIKTAG_OBJ_WidthHeight, W2L(rs->Obj.Box.Width-16,fh),
                    UIKTAG_OBJ_LangMaster,  rs->Obj.ObjExt->Lang->Master,
                    UIKTAG_OBJ_Title,       rs->Obj.Title,
                    UIKTAG_OBJ_Title_Flags, rs->Obj.TitleFlags,
                    UIKTAG_OBJ_BoxAutomate, NullAuto,
                    UIKTAG_OBJ_BoxFl_Filled, FALSE,
                    TAG_END )) goto END_ERROR;
            if (! IntUIK_AddObjectTags( "UIKObj_Box", req,
                    //UIKTAG_OBJ_LeftTop,     W2L(3,YY+height_title-2),
                    //UIKTAG_OBJ_WidthHeight, W2L(rs->Obj.Box.Width-6,2),
                    UIKTAG_OBJ_LeftTop,     W2L(leftoffs,YY+height_title-2),
                    UIKTAG_OBJ_WidthHeight, W2L(rs->Obj.Box.Width-widthoffs,2),
                    UIKTAG_OBJ_BoxAutomate, TitleBarAutoD,
                    TAG_END )) goto END_ERROR;
            }
        }
    rs->Req = req;
    if (! UIK_AddVector( req, (void*)ReqKeyFunc, RAWKEY, (ULONG)rs )) goto END_ERROR;
    YY += (height_title + after_title);

    /*------------ajout de la liste------------*/
    listwidth = rs->Obj.Box.Width - 20;
    {
    struct TagItem *tll, ListTL[] = {
        { UIKTAG_OBJ_LeftTop        },
        { UIKTAG_OBJ_WidthHeight    },
        { UIKTAG_OBJList_TextEntry  },
        { UIKTAG_OBJ_BoxAltAutomate    },
        { UIKTAG_OBJListFl_ClickSel, FALSE },
        { TAG_END, 0 } };

    ListTL[0].ti_Data = W2L(10,YY);
    ListTL[1].ti_Data = W2L(listwidth,height_list);
    ListTL[2].ti_Tag = tag; ListTL[2].ti_Data = TEntry;
    ListTL[3].ti_Data = (ULONG)NullAuto;
    if (tll = (struct TagItem *) UIK_GetTagData( UIKTAG_ReqString_TextListTagList, 0, taglist ))
        {
        ListTL[5].ti_Tag = TAG_MORE;
        ListTL[5].ti_Data = (ULONG)tll;
        }
    if (! (rs->Text = UIK_AddObject( "UIKObj_List", req, ListTL ))) goto END_ERROR;
    //rs->Text->Obj.Automate =
    }
    YY += (height_list + after_list);

    /*------------ajout des string gadgets------------*/
    {
    if (stringptr1 || stringptr2)
        {
        ULONG w1=0, w2=0;
        struct TagItem *tl1, *tl2;
        struct TagItem StringTL[] = {
            { UIKTAG_OBJ_LeftTop        },
            { UIKTAG_OBJ_WidthHeight    },
            { UIKTAG_OBJStr_TextPointer },
            { UIKTAG_OBJ_TitleFl_Left, TRUE },
            { TAG_END, 0 } };

        w1 = strtitle_width( rs, font, tl1 = (struct TagItem *) UIK_GetTagData( UIKTAG_ReqString_StringTagList, 0, taglist ) );
        w2 = strtitle_width( rs, font, tl2 = (struct TagItem *) UIK_GetTagData( UIKTAG_ReqString_StringTagList2, 0, taglist ) );
        if (w1 < w2) w1 = w2;
        if (w1 && w1 < 40) w1 = 40;

        if (StringTL[2].ti_Data = stringptr1)
            {
            StringTL[0].ti_Data = W2L(w1+10,YY);
            StringTL[1].ti_Data = W2L(listwidth - w1,0);
            if (StringTL[4].ti_Data = (ULONG)tl1) StringTL[4].ti_Tag = TAG_MORE;

            if (! (rs->Str = (struct UIKObjString *) UIK_AddObject( "UIKObj_String", req, StringTL ))) goto END_ERROR;
            rs->Str->CursorPos = StrLen( (UBYTE*)stringptr1 );
            if (! (rs->KeyHook = UIK_AddVector( rs->Str, (void*)StrKeyFunc, RAWKEY, (ULONG)rs ))) goto END_ERROR;
            YY += (fh + 5);
            }
        if (StringTL[2].ti_Data = stringptr2)
            {
            if (stringptr1) YY += smallspace;
            StringTL[0].ti_Data = W2L(w1+10,YY);
            StringTL[1].ti_Data = W2L(listwidth - w1,0);
            if (StringTL[4].ti_Data = (ULONG)tl2) StringTL[4].ti_Tag = TAG_MORE;

            if (! (rs->Str2 = (struct UIKObjString *) UIK_AddObject( "UIKObj_String", req, StringTL ))) goto END_ERROR;
            rs->Str2->CursorPos = StrLen( (UBYTE*)stringptr2 );
            if (! (rs->KeyHook2 = UIK_AddVector( rs->Str2, (void*)StrKeyFunc, RAWKEY, (ULONG)rs ))) goto END_ERROR;
            YY += (fh + 5);
            }
        YY += after_strings;
        }
    }

    /*------------Boite de séparation-----------*/
    if (! IntUIK_AddObjectTags( "UIKObj_Box", req,
            //UIKTAG_OBJ_LeftTop,     W2L(4,YY),
            //UIKTAG_OBJ_WidthHeight, W2L(rs->Obj.Box.Width-8,height_sep),
            UIKTAG_OBJ_LeftTop,     W2L(leftoffs,YY),
            UIKTAG_OBJ_WidthHeight, W2L(rs->Obj.Box.Width-widthoffs,height_sep),
            UIKTAG_OBJ_BoxAutomate, TitleBarAutoD,
            TAG_END )) goto END_ERROR;
    YY += (height_sep + after_sep);

    /*------------ajout de Cancel-----------*/
    if (rs->CancelFunc)
        {
        TL[0].ti_Data = W2L((10+listwidth-widthcan),YY);
        TL[1].ti_Data = W2L(widthcan,height_gads);
        if (titlecan)
            {
            TL[2].ti_Data = (ULONG)titlecan;
            TL[3].ti_Tag = UIKTAG_OBJ_TitleFl_Addr;
            }
        else TL[2].ti_Tag = TAG_END;
        if (! (o = UIK_AddObject( "UIKObj_Cancel", req, TL ))) goto END_ERROR;
        if (! UIK_AddVector( o, (void*)rs->CancelFunc, GADGETUP, (ULONG)rs )) goto END_ERROR;
        }
    TL[2].ti_Tag = UIKTAG_OBJ_Title;

    /*------------ajout de Middle-----------*/
    if (rs->CancelFunc && rs->MiddleFunc)
        {
        TL[0].ti_Data = W2L((rs->Obj.Box.Width-widthmid)/2,YY);
        TL[1].ti_Data = W2L(widthmid,height_gads);
        TL[2].ti_Data = (ULONG) "...";
        TL[3].ti_Tag = UIKTAG_OBJ_TitleFl_Addr;
        if (titlemid)
            {
            TL[2].ti_Data = (ULONG)titlemid;
            }
        if (! (o = UIK_AddObject( "UIKObj_Boolean", req, TL ))) goto END_ERROR;
        if (! UIK_AddVector( o, (void*)rs->MiddleFunc, GADGETUP, (ULONG)rs )) goto END_ERROR;
        }

    /*------------ajout de OK-----------*/
    if (rs->CancelFunc) TL[0].ti_Data = W2L(10,YY);
    else{
        TL[0].ti_Data = W2L((rs->Obj.Box.Width-widthok)/2,YY);
        rs->CancelFunc = (void*)rs->OKFunc;
        if (iswindow && !closefunc)
            if (! UIK_AddVector( req, (void*)rs->OKFunc, CLOSEWINDOW, (ULONG)rs )) goto END_ERROR;
        }
    TL[1].ti_Data = W2L(widthok,height_gads);
    if (titleok)
        {
        TL[2].ti_Data = (ULONG)titleok;
        TL[3].ti_Tag = UIKTAG_OBJ_TitleFl_Addr;
        }
    else TL[2].ti_Tag = TAG_END;
    if (! (o = UIK_AddObject( "UIKObj_OK", req, TL ))) goto END_ERROR;
    if (! UIK_AddVector( o, (void*)rs->OKFunc, GADGETUP, (ULONG)rs )) goto END_ERROR;

    return(1);

  END_ERROR:
    return(0);
}

struct UIKObjGerm __far ReqStringGerm = {
    "UIKObj_ReqString",
    AddReqString,       /*int     (*AddVector)(); */
    NULL,               /*int     (*StartVector)(); */
    NULL,               /*void    (*StopVector)(); */
    NULL,               /*void    (*RemoveVector)(); */
    NULL,               /*void    (*RefreshVector)(); */
    NULL,               /*void    (*ResizeVector)(); */
    NULL,               /*void    (*ExceptActVector)(); */
    NULL,               /*void    (*ExceptInaVector)(); */
    NULL,               /*void    (*ExceptMovVector)(); */
    NULL,               /*void    (*ExceptKeyVector)(); */
    StartAfterReqString,/*int     (*StartAfterVector)(); */
    NULL,               /*int     (*SelectVector)(); */
    NULL,               /*ULONG   *ExtentedVectors; */
    NULL,               /*ULONG   ExReserved[1]; */
    0,                  /*UWORD   MinWidth; */
    0,                  /*UWORD   MinHeight; */
    0,                  /*ULONG   DefaultWidthHeight; */
    UIKID_REQSTRING,                /*UWORD   ObjId; */
    sizeof(struct UIKObjReqString), /*UWORD   ObjSize; */
    BOX_DISPLAYABLE | BOX_TRANSLATELEFTTOP | BOX_CANTDISABLE,
    };
