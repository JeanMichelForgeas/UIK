/****************************************************************
 *
 *      Project:   UIK
 *      Function:  routines de gestion des objets 'window'
 *
 *      Created:   11/05/90    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include <intuition/intuitionbase.h>
#include <graphics/gfxbase.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikglobal.h"
#include "uiki:uikbox.h"
#include "uiki:uikmacros.h"
#include "uiki:uikcolors.h"
#include "uiki:uikdrawauto.h"
#include "lci:uik_protosint.h"

#include "uiki:objscreen.h"
#include "uiki:objwindow.h"

#define UIK_IDCMP (IDCMP_REFRESHWINDOW|INTUITICKS|CLOSEWINDOW|ACTIVEWINDOW|MOUSEMOVE|\
INACTIVEWINDOW|GADGETDOWN|GADGETUP|REQSET|REQCLEAR|RAWKEY|DISKREMOVED|DISKINSERTED|MENUPICK|\
MENUVERIFY|IDCMP_MENUHELP|IDCMP_CHANGEWINDOW|IDCMP_IDCMPUPDATE|IDCMP_GADGETHELP|MOUSEBUTTONS|\
IDCMP_REQVERIFY|IDCMP_NEWPREFS)

// pas de VANILLAKEY, DELTAMOVE, WBENCHMESSAGE
// SIZEVERIFY|NEWSIZE mis après

#define SYSGAD_WIDTH 22

struct MaxBox { WORD maxl, maxt, maxw, maxh; };


/****** Imported ************************************************/

extern ULONG __asm StrNCSub( register __a0 UBYTE *sub, register __a1 UBYTE *str );

extern struct UIKObjScreen * __asm parent_screen( register __a0 struct UIKObj *obj );
extern struct UIKObjWindow * __asm parent_window( register __a0 struct UIKObj *obj );
extern struct UIKZoneList * __asm zo_InstallZoneList( register __a0 struct UIKGlobal *uik, register __a1 struct UIKZoneList *uzl );

extern struct UIKBase * __far UIKBase;
extern struct Library * __far GfxBase;
extern struct IntuitionBase * __far IntuitionBase;
extern ULONG __far DOSBase, __far stdout;

extern UWORD __far AutoDrawBox_Over[], __far AutoDrawBox_Under[];
extern UWORD __far DraggAuto[];
extern UWORD __far WinTitleAuto[];
extern UWORD __far WinTitleAltAuto[];
extern UWORD __far AutoDrawWindow[];
extern UWORD __far AutoDrawWindowNoFill[];
extern UWORD __far AutoFillWindow[];
extern UWORD __far DepthAuto[], __far DepthAltAuto[];
extern UWORD __far ZoomAuto[], __far ZoomAltAuto[];
extern UWORD __far IconAuto[], __far IconAltAuto[];
extern UWORD __far CloseAuto[], __far CloseAltAuto[];

/****** Exported ************************************************/


/****** Statics *************************************************/

static ULONG AddWindowObjects( struct UIKObjWindow *wobj, struct TagItem *taglist, struct UIKWinScrInfo *si );

//static UWORD __far pens[] = { ~0 };

static struct TagItem __far flags_boolmap[] = {
    { UIKTAG_OBJWindowFl_IconWindow, UIKW_ICONWINDOW },
    { UIKTAG_OBJWindowFl_With_Depth, UIKW_DEPTH      },
    { UIKTAG_OBJWindowFl_With_Size,  UIKW_SIZE       },
    { UIKTAG_OBJWindowFl_With_Close, UIKW_CLOSE      },
    { UIKTAG_OBJWindowFl_With_Zoom,  UIKW_ZOOM       },
    { UIKTAG_OBJWindowFl_With_Icon,  UIKW_ICON       },
    { UIKTAG_OBJWindowFl_With_Drag,  UIKW_DRAG       },
    { UIKTAG_OBJWindowFl_AutoAdjustSize,    UIKW_AUTOADJUSTSIZE    },
    { UIKTAG_OBJWindowFl_AutoAdjustLeftTop, UIKW_AUTOADJUSTLEFTTOP },
    { UIKTAG_OBJWindowFl_NoDefScreen,       UIKW_NODEFSCREEN },
    { UIKTAG_OBJWindowFl_NoBlackPerimeter,  UIKW_NOPERIMETER },
    { UIKTAG_OBJWindowFl_DefScreenOnly,     UIKW_DEFSCREENONLY },
    { UIKTAG_OBJWindowFl_Backdrop,          UIKW_BACKDROP },
    { UIKTAG_OBJWindowFl_NewLookMenu3,      UIKW_MENULOOK },
    { UIKTAG_OBJWindowFl_SystemLook,        UIKW_SYSTEMLOOK },
    { UIKTAG_OBJWindowFl_CloseStop,         UIKW_CLOSESTOP },
    { UIKTAG_OBJWindowFl_CloseRemove,       UIKW_CLOSEREMOVE },
    { UIKTAG_OBJWindowFl_NoFill,            UIKW_NOFILL },
    { UIKTAG_OBJWindowFl_SimpleRefresh,     UIKW_SIMPLEREFRESH },
    { UIKTAG_OBJWindowFl_StartInactive,     UIKW_STARTINACTIVE },

    { UIKTAG_OBJWindowFl_FullScreenWidth,   UIKW_FULLWIDTH },
    { UIKTAG_OBJWindowFl_FullScreenHeight,  UIKW_FULLHEIGHT },
    { UIKTAG_OBJWindowFl_NearFullScHeight,  UIKW_FULLHEIGHT | UIKW_NEARFULL },
    { UIKTAG_OBJWindowFl_FullDisplayWidth,  UIKW_FULLDISPL | UIKW_FULLWIDTH },
    { UIKTAG_OBJWindowFl_FullDisplayHeight, UIKW_FULLDISPL | UIKW_FULLHEIGHT },
    { UIKTAG_OBJWindowFl_NearFullDiHeight,  UIKW_FULLDISPL | UIKW_FULLHEIGHT | UIKW_NEARFULL },
    { UIKTAG_OBJWindowFl_FullScreen,        UIKW_FULLWIDTH | UIKW_FULLHEIGHT },
    { UIKTAG_OBJWindowFl_NearFullScreen,    UIKW_FULLWIDTH | UIKW_FULLHEIGHT | UIKW_NEARFULL },
    { UIKTAG_OBJWindowFl_FullDisplay,       UIKW_FULLWIDTH | UIKW_FULLHEIGHT | UIKW_FULLDISPL },
    { UIKTAG_OBJWindowFl_NearFullDisplay,   UIKW_FULLWIDTH | UIKW_FULLHEIGHT | UIKW_NEARFULL | UIKW_FULLDISPL },
    { UIKTAG_OBJWindowFl_AutoShrink,        UIKW_AUTOSHRINK },
    { TAG_DONE } };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static int __asm is_syslook( register __a0 struct UIKObjWindow *wobj )
{
    return( (((wobj->WinFlags & UIKW_SYSTEMLOOK) /*|| (UIKBase->UIKPrefs->PrefsFlags & UIKF_SYSLOOK)*/) && !(wobj->WinFlags & UIKW_ICONWINDOW)) ? 1 : 0 );
}

static int __asm is_uikscreen( register __a0 struct UIKObjWindow *wobj )
{
    return( ((wobj->WinFlags & UIKW_DEFSCREENONLY) || ((UIKBase->UIKPrefs->PrefsFlags & UIKDEFAULT_SCREEN) && !(wobj->WinFlags & UIKW_NODEFSCREEN) && !wobj->ScrAdd)) ? 1 : 0 );
}

static int __asm is_sysborder( register __a0 struct UIKObjWindow *wobj )
{
    return( (!(wobj->WinFlags & UIKW_NOPERIMETER) || (wobj->WinFlags & (UIKW_DEPTH|UIKW_CLOSE/*|UIKW_DRAG*/|UIKW_SIZE)) || wobj->Obj.Title) ? 1 : 0 );
}

struct Screen *screen_exists( struct Screen *isc, UBYTE *name )
{
  ULONG ilock;
  struct Screen *sc;

    ilock = LockIBase( 0 );
    sc = IntuitionBase->FirstScreen;
    while (sc)
        {
        if (isc == sc) break;
        if (name && StrNCSub( name, sc->DefaultTitle )) break;
        sc = sc->NextScreen;
        }
    UnlockIBase( ilock );
    return( sc );
}

static void DrawWindow( struct UIKObjWindow *obj )
{
  UWORD innerw = obj->Obj.w->Width - (obj->LeftBorderWidth + obj->RightBorderWidth),
        innerh = obj->Obj.w->Height - (obj->TopBorderHeight + obj->BottomBorderHeight);

    if (! (obj->Obj.Flags & BOX_NDRAWN))
        {
        if (is_syslook( obj ))
            {
            if (! (obj->WinFlags & UIKW_NOFILL))
                {
                WORD X=obj->LeftBorderWidth+innerw-1, Y=obj->TopBorderHeight+innerh-1;
                if (X >= obj->LeftBorderWidth && Y >= obj->TopBorderHeight)
                    {
                    struct RastPort *rp = obj->Obj.rp;
                    UBYTE mask = rp->Mask;

                    rp->Mask = obj->Obj.sc ? ((struct UIKObjScreen *)obj->Obj.sc->UserData)->Mask : UIKBase->UIKPrefs->Mask;
                    SetAPen( obj->Obj.rp, UIK_GetColor( obj, UIKCOL_GREY ) );
                    SetDrMd( obj->Obj.rp, JAM2 );
                    RectFill( obj->Obj.rp, obj->LeftBorderWidth, obj->TopBorderHeight, X, Y );
                    rp->Mask = mask;
                    }
                }
            }
        else{
            if (obj->WinFlags & UIKW_NOFILL)
                { if (! (obj->WinFlags & UIKW_NOPERIMETER)) IntUIK_DrawAuto( obj, AutoDrawWindowNoFill, 0, 0 ); }
            else IntUIK_DrawAuto( obj, (obj->WinFlags & UIKW_NOPERIMETER) ? AutoFillWindow : AutoDrawWindow, 0, 0 );
            }
        }

    if (obj->WinFlags & UIKW_BITMAPED)
        {
        struct BitMap *bm;
        if (bm = obj->CustomBitMap)
            BltBitMapRastPort( obj->CustomBitMap, 0,0, obj->Obj.w->RPort, obj->LeftBorderWidth,obj->TopBorderHeight,
                    MIN( bm->BytesPerRow<<3, innerw ), MIN( bm->Rows, innerh ), 0xc0 );
        }
}

static void __asm CloseDefScreen( register __a1 struct Screen *s,
register __a2 struct UIKBase *base, register __a3 struct UIKPrefs *prefs )
{
    if (base->DefaultScreen == s)
        if (! base->DefaultScreen->FirstWindow)
            {
            Forbid(); /* à cause du Screen Manager */
            if (screen_exists( base->DefaultScreen, 0 ))
                {
                if (UIK_IsSystemVersion2())
                    {
                    if (TRUE == CloseScreen( base->DefaultScreen ))
                        Signal( base->ScrPubTask, SIGBREAKF_CTRL_D );
                    }
                else CloseScreen( base->DefaultScreen );
                }
            Permit();
            base->DefaultScreen = 0;
            }
}

static struct Screen * __asm OpenDefScreen(
register __a1 struct UIKObjWindow *wobj,
register __a2 struct UIKBase *base,
register __a3 struct UIKPrefs *prefs )
{
    if (is_uikscreen( wobj ))
        {
        if (! base->DefaultScreen)
            {
            if (UIK_IsSystemVersion2())
                //--------------------- 2.0 stuff
                {
                UWORD tpens[13], black, white, light;
                black = UIK_GetColor( wobj, UIKCOL_BLACK );
                white = UIK_GetColor( wobj, UIKCOL_WHITE );
                light = UIK_GetColor( wobj, UIKCOL_LIGHT );
                tpens[0] = black;
                tpens[1] = white;
                tpens[2] = black;
                tpens[3] = white;
                tpens[4] = black;
                tpens[5] = light;
                tpens[6] = black;
                tpens[7] = 0;
                tpens[8] = UIK_GetColor( wobj, UIKCOL_IMP );
                tpens[9] = black;
                tpens[10] = white;
                tpens[11] = black;
                tpens[12] = -1;
                if (base->DefaultScreen = OpenScreenTags( NULL,
                                SA_Width, prefs->DefScreen_Width,
                                SA_Height, prefs->DefScreen_Height,
                                SA_Depth, prefs->DefScreen_Depth,
                                SA_DisplayID, prefs->DefScreen_ModeId,
                                SA_Overscan, OSCAN_TEXT,
                                SA_Type, PUBLICSCREEN /*CUSTOMSCREEN*/,
                                SA_Behind, TRUE,
                                //SA_Quiet, TRUE,
                                SA_Font, &base->DefaultFontAttr,
                                SA_Pens, tpens, // pens,
                                SA_Title, "UIK Screen",
                                SA_PubName, "UIK Screen",
                                SA_PubSig, SIGBREAKB_CTRL_F,
                                SA_PubTask, base->ScrPubTask,
                                SA_AutoScroll, TRUE,
                                SA_MinimizeISG, TRUE,
                                SA_SharePens,   TRUE,  /* --> pour la roue des couleurs - Intuition V39 (3.0) */
                                SA_Interleaved, TRUE,  /* --> pour l'exemple - Intuition V39 (3.0) */
                                TAG_END ))
                    {
                    PubScreenStatus( base->DefaultScreen, 0 );
                    Signal( base->ScrPubTask, SIGBREAKF_CTRL_E );
                    if (GfxBase->lib_Version >= 39)
                        {
                        UBYTE i;
                        for (i=0; i<MAX_UIKDEFINEDPENS; i++)
                            ObtainPen( base->DefaultScreen->ViewPort.ColorMap,
                                        UIK_GetColor( wobj->Obj.UIK, i ),
                                        0, 0, 0, PEN_EXCLUSIVE | PEN_NO_SETCOLOR );
                        }
                    }
                }
            else//--------------------- 1.3 stuff
                {
                struct NewScreen ns;

                ns.LeftEdge = ns.TopEdge = 0;
                ns.Depth = prefs->DefScreen_Depth;
                ns.Width = prefs->DefScreen_Width;
                ns.Height = prefs->DefScreen_Height;
                ns.Type = CUSTOMSCREEN | SCREENBEHIND;
                ns.DetailPen = 0;
                ns.BlockPen = 1;
                ns.ViewModes = prefs->DefScreen_Modes;
                ns.Font = &base->DefaultFontAttr;
                ns.DefaultTitle = "UIK Screen";
                ns.Gadgets = 0;
                ns.CustomBitMap = 0;
                base->DefaultScreen = OpenScreen( &ns );
                }
            if (base->DefaultScreen)
                {
                LoadRGB4( &base->DefaultScreen->ViewPort, prefs->ColorTable, 2*MAX_UIKCOLORS );
                ScreenToFront( base->DefaultScreen );
                }
            }
        else ScreenToFront( base->DefaultScreen );
        }
    return( base->DefaultScreen );
}

void get_UIKWinScrInfo( struct UIKObjWindow *wobj, struct UIKWinScrInfo *si )
{
  struct UIKObjScreen *os;
  struct Screen sc, *scr=0;

    os = parent_screen( wobj );

    if (wobj->ScrAdd = screen_exists( wobj->ScrAdd, 0 ))
        {
        scr = wobj->ScrAdd;
        if (UIK_IsSystemVersion2()) si->ModeId = GetVPModeID( &scr->ViewPort );
        else si->ModeId = scr->ViewPort.Modes;
        si->Font = scr->RastPort.Font;
        si->Depth = scr->RastPort.BitMap->Depth;
        si->Scr = scr;
        *((struct UIKBox *)&si->Left) = *((struct UIKBox *)&scr->LeftEdge);
        }
    else{
        if (os)
            {
            si->ModeId = os->ModeId;
            si->Font = os->Obj.ObjExt->Font;
            si->Depth = os->NS.Depth;
            if (si->Scr = os->Obj.sc)
                *((struct UIKBox *)&si->Left) = *((struct UIKBox *)&si->Scr->LeftEdge);
            else
                *((struct UIKBox *)&si->Left) = *((struct UIKBox *)&os->NS.LeftEdge);
            }
        else{
            struct UIKPrefs *prefs = UIKBase->UIKPrefs;

            si->ModeId = prefs->DefScreen_ModeId;
            si->Font = UIKBase->DefaultFont;
            si->Depth = prefs->DefScreen_Depth;
            if (si->Scr = UIKBase->DefaultScreen)
                *((struct UIKBox *)&si->Left) = *((struct UIKBox *)&si->Scr->LeftEdge);
            else{
                *((ULONG *)&si->Left) = 0;
                *((ULONG *)&si->Width) = *((ULONG *)&prefs->DefScreen_Width);
                }

            if (! is_uikscreen( wobj ))
                {
                if (UIK_IsSystemVersion2())
                    {
                    if (scr = LockPubScreen( "Workbench" ))
                        {
                        si->ModeId = GetVPModeID( &scr->ViewPort );
                        UnlockPubScreen( NULL, scr );
                        }
                    }
                if (!scr)
                    {
                    ULONG ilock;
                    struct Screen *sc;

                    ilock = LockIBase( 0 );
                    sc = IntuitionBase->FirstScreen;
                    while (sc)
                        {
                        if (sc->Flags & WBENCHSCREEN) { scr = sc; break; }
                        sc = sc->NextScreen;
                        }
                    UnlockIBase( ilock );

                    if (scr) si->ModeId = scr->ViewPort.Modes;
                    }
                if (scr)
                    {
                    si->Font = scr->RastPort.Font;
                    si->Depth = scr->RastPort.BitMap->Depth;
                    si->Scr = scr;
                    *((struct UIKBox *)&si->Left) = *((struct UIKBox *)&scr->LeftEdge);
                    }
                }
            }
        }
}

struct TextFont *IconFont( struct UIKObjWindow *wobj )
{
  struct UIKWinScrInfo si;

    get_UIKWinScrInfo( wobj, &si );
    return( si.Font );
}

static void unlock_screen( struct UIKObjWindow *wobj )
{
    if (wobj->ScrAdd && (wobj->WinFlags & UIKW_SCREENLOCKED))
        {
        UnlockPubScreen( NULL, wobj->ScrAdd );
        wobj->WinFlags &= ~UIKW_SCREENLOCKED;
        }
}

static int StartWindow( struct UIKObjWindow *wobj )
{
  struct ExtNewWindow enw;
  register struct Window *w=0;
  register struct UIKGlobal *uik = wobj->Obj.UIK;
  register struct UIKObj *t;
  struct Screen sc, *s=0, *locked=0;
  ULONG idcmp, defscreen=0;
  struct TagItem wtl[] =
    {
    WA_PubScreen,           0,
    WA_PubScreenFallBack,   TRUE,
    TAG_END, 0
    };

    if (wobj->ScrAdd && screen_exists( wobj->ScrAdd, 0 ))
        {
        wobj->NW.Type = CUSTOMSCREEN;
        s = wobj->NW.Screen = wobj->ScrAdd;
        }
    else{
        if (wobj->Obj.sc)
            {
            wobj->NW.Type = CUSTOMSCREEN;
            s = wobj->NW.Screen = wobj->Obj.sc;
            }
        else{
            if (s = (struct Screen *) UIK_AccessPrefs( (void *) OpenDefScreen, (ULONG)wobj, 0 ))
                {
                wobj->NW.Type = CUSTOMSCREEN;
                wobj->NW.Screen = s;
                defscreen = 1;
                }
            else{
                if (UIK_IsSystemVersion2())
                    {
                    locked = s = LockPubScreen( "Workbench" );
                    }
                if (! s)
                    if (GetScreenData( (char*)&sc, sizeof(sc), WBENCHSCREEN, NULL ))
                        {
                        wobj->NW.Type = WBENCHSCREEN;
                        wobj->NW.Screen = NULL;
                        s = &sc;
                        }
                }
            }
        }
    if (! s) goto END_ERROR;

    if (wobj->NW.MaxWidth == -1 || wobj->NW.MaxWidth > s->Width) wobj->NW.MaxWidth = s->Width;
    if (wobj->NW.MaxHeight == -1 || wobj->NW.MaxHeight > s->Height) wobj->NW.MaxHeight = s->Height;

    if (wobj->WinFlags & UIKW_AUTOADJUSTLEFTTOP)
        {
        if (wobj->NW.LeftEdge + wobj->Obj.Box.Width > s->Width) wobj->NW.LeftEdge = s->Width - wobj->Obj.Box.Width;
        if (wobj->NW.TopEdge + wobj->Obj.Box.Height > s->Height) wobj->NW.TopEdge = s->Height - wobj->Obj.Box.Height;
        if (wobj->NW.LeftEdge < 0) wobj->NW.LeftEdge = 0;
        if (wobj->NW.TopEdge < 0) wobj->NW.TopEdge = 0;
        }

/* remplacé par ajust_window_size
    if (wobj->Obj.Status == UIKS_STOPPED)
        {
        wobj->OldLeft   = wobj->NW.LeftEdge;
        wobj->OldTop    = wobj->NW.TopEdge;
        wobj->OldWidth  = wobj->NW.MinWidth;
        wobj->OldHeight = wobj->NW.MinHeight;
        }
*/

    wobj->NW.DetailPen = 0; wobj->NW.BlockPen = 1;

    CopyMem( &wobj->NW, &enw, sizeof(struct NewWindow) );
    if (UIK_IsSystemVersion2())
        {
        wtl[0].ti_Data = (ULONG)enw.Screen;
        enw.Screen = 0;
        enw.Type = 0;
        enw.Flags |= WFLG_NW_EXTENDED;
        enw.Extension = wtl;
        }

    if (! (w = (struct Window *) OpenWindow( &enw ))) goto END_ERROR;
    wobj->ScrAdd = w->WScreen;
    unlock_screen( wobj );
    w->UserData = (BYTE *) wobj;
    if (! (wobj->Obj.irp = (struct RastPort *) UIK_CloneRPort( w->RPort ))) goto END_ERROR;
    wobj->Obj.w = w; wobj->Obj.rq = 0; wobj->Obj.rp = w->RPort;
    wobj->Obj.Status = UIKS_STARTED;

    if (t = wobj->TitleBox)
        {
        if (w->Flags & WINDOWACTIVE) t->Flags &= ~BOX_OVER; else t->Flags |= BOX_OVER;
        t->BoxPen = (w->Flags & WINDOWACTIVE ? t->ActBoxPen : t->InactBoxPen /*UIKCOL_LIGHT : UIKCOL_GREY*/);
        }

    uik->WindowNum++;
    w->UserPort = &uik->UPort;
    idcmp = ModifyIDCMP( w, UIK_IDCMP | ((wobj->WinFlags & UIKW_SIZE) ? SIZEVERIFY|NEWSIZE : 0) );
    if ((IntuitionBase->LibNode.lib_Version >= 37) && !idcmp) goto END_ERROR;

    DrawWindow( wobj );

    if (wobj->Obj.AltTitle != -1)
        {
        UBYTE *titleptr = UIK_ObjAltTitle( wobj );
        SetWindowTitles( w, (char*)-1, titleptr?titleptr:"" );
        }

    if (locked) UnlockPubScreen( NULL, locked );

    if (UIK_IsSystemVersion2())
        {
        if (! (wobj->DrawInfo = GetScreenDrawInfo( w->WScreen ))) goto END_ERROR;
        }

    return(1);

  END_ERROR:
    if (w)
        {
        if (wobj->Obj.irp) UIK_MemSpFree( wobj->Obj.irp );
        wobj->Obj.irp = 0;
        CloseWindow( w );
        }
    if (defscreen) UIK_AccessPrefs( (void *) CloseDefScreen, (ULONG) s, 0 );
    unlock_screen( wobj );
    return(0);
}

static int AfterStartWindow( struct UIKObjWindow *wobj )
{
  register struct UIKGlobal *uik = wobj->Obj.UIK;

    UIK_CauseObjHook( wobj, ACTIVEWINDOW );

    if (wobj->Obj.ObjExt->ZoneList && (wobj->Obj.w->Flags & WFLG_WINDOWACTIVE))
        zo_InstallZoneList( uik, wobj->Obj.ObjExt->ZoneList );
    return(1);
}

void get_overscan( struct UIKObjWindow *wobj, struct MaxBox *b, struct UIKBox *sc, ULONG ModeId, WORD *piconheight )
{
  struct Rectangle rect;

    b->maxl = b->maxt = 0;
    b->maxw = sc->Width; b->maxh = sc->Height;
    if (UIK_IsSystemVersion2())
        {
        if (QueryOverscan( ModeId, &rect, OSCAN_TEXT ))
            {
            b->maxl = MAX(0,-sc->Left);
            b->maxt = MAX(0,-sc->Top);
            b->maxw = rect.MaxX - rect.MinX + 1;
            b->maxh = rect.MaxY - rect.MinY + 1;
            if (sc->Top > 0) b->maxh -= sc->Top;
            else if (sc->Top < 0 && piconheight && *piconheight) // si iconheight == 0 pas de calcul non plus
                {
                *piconheight += sc->Top;
                if (*piconheight < 0) *piconheight = 0;
                }
            b->maxw = MIN(b->maxw,sc->Width);
            b->maxh = MIN(b->maxh,sc->Height);
            }
        }
}

//------------------------------------------------------------------------------------

void ZipWindowObj( struct UIKObjWindow *wobj, ULONG wanted, ULONG full )
{
  struct Window *w = wobj->Obj.w;
  struct Screen *s;
  WORD n, grandir, disp = 1, iconheight;
  struct IntuiMessage *imsg = wobj->Obj.UIK->IntuiMsg;
  struct MaxBox b;
  ULONG vpmid;

    if (!w || !(wobj->WinFlags & UIKW_SIZE)) return;
    wobj->NW.MinWidth  = w->MinWidth;
    wobj->NW.MinHeight = w->MinHeight;
    wobj->NW.MaxWidth  = w->MaxWidth;
    wobj->NW.MaxHeight = w->MaxHeight;

    s = w->WScreen;
    iconheight = (full ? 0 : ((struct TextFont *)IconFont( wobj ))->tf_YSize + 3);

    b.maxl = 0;
    b.maxt = iconheight;
    b.maxw = s->Width;
    b.maxh = s->Height - iconheight;
    if (wobj->NW.MaxWidth != s->Width/*-1*/)
        { b.maxl = w->LeftEdge; if (wobj->NW.MaxWidth < b.maxw) b.maxw = wobj->NW.MaxWidth; }
    if (wobj->NW.MaxHeight != s->Height/*-1*/)
        { b.maxt = w->TopEdge; if (wobj->NW.MaxHeight < b.maxh) b.maxh = wobj->NW.MaxHeight; }

    if (UIK_IsSystemVersion2())
        {
        if (imsg)
            {
            if (imsg->Qualifier & (IEQUALIFIER_LSHIFT|IEQUALIFIER_RSHIFT)) disp = 0;
            }

        if (disp)
            {
            if ((vpmid = GetVPModeID( &s->ViewPort )) != INVALID_ID)
                get_overscan( wobj, &b, (struct UIKBox *)&s->LeftEdge, vpmid, &iconheight );

            if (wobj->NW.MaxWidth != s->Width/*-1*/)
                { b.maxl = w->LeftEdge; if (wobj->NW.MaxWidth < b.maxw) b.maxw = wobj->NW.MaxWidth; }
            if (wobj->NW.MaxHeight != s->Height/*-1*/)
                { b.maxt = w->TopEdge; if (wobj->NW.MaxHeight < b.maxh) b.maxh = wobj->NW.MaxHeight; }
            else { b.maxt += iconheight; b.maxh -= iconheight; }
            }
        }

    UIK_LockObjVars( wobj );
    if ((w->Width != b.maxw) || (w->Height != b.maxh))
        {
        grandir = 1;
        wobj->OldLeft   = w->LeftEdge;
        wobj->OldTop    = w->TopEdge;
        wobj->OldWidth  = w->Width;
        wobj->OldHeight = w->Height;
        }
    else{
        grandir = 0;
        b.maxl = wobj->OldLeft;
        b.maxt = wobj->OldTop;
        b.maxw = wobj->OldWidth;
        b.maxh = wobj->OldHeight;
        }
    if (b.maxw < w->MinWidth) b.maxw = wobj->NW.MinWidth;
    if (b.maxh < w->MinHeight) b.maxh = wobj->NW.MinHeight;
    if ((n = b.maxl+b.maxw-s->Width) > 0) b.maxl -= n;
    if ((n = b.maxt+b.maxh-s->Height) > 0) b.maxt -= n;
    UIK_UnlockObjVars( wobj );

    switch (wanted)
        {
        case 0: if (grandir) return; break;   // veut petite taille
        case 1: if (! grandir) return; break; // veut grande taille
        }

    if (UIK_IsSystemVersion2())
        {
        ChangeWindowBox( w, b.maxl, b.maxt, b.maxw, b.maxh );
        }
    else{
        b.maxl -= w->LeftEdge;
        b.maxt -= w->TopEdge;
        if (grandir) MoveWindow( w, b.maxl, b.maxt );
        SizeWindow( w, b.maxw - w->Width, b.maxh - w->Height );
        if (! grandir) MoveWindow( w, b.maxl, b.maxt );
        }
}

static ULONG ChangeWindowTitle( struct UIKObjWindow *wobj, UBYTE *title )
{
  struct UIKObj *box = wobj->TitleBox;
  ULONG len = StrLen( title );

    if ((wobj->Obj.TitleFlags & (TITLE_ADDR | TITLE_IMAGE)) != TITLE_ADDR) return(0);
    if (! BufSetS( title, len, (UBYTE**)&wobj->Obj.Title )) return(0);
    title = (UBYTE*)wobj->Obj.Title;

    if (box)
        {
        BufSetS( title, len, (UBYTE**)&box->Title );
        UIK_Refresh( box, 1 );
        }

    if (wobj->WinFlags & UIKW_SYSTEMLOOK)
        {
        wobj->NW.Title = (UBYTE*)wobj->Obj.Title;
        if (wobj->Obj.Status == UIKS_STARTED)
            SetWindowTitles( wobj->Obj.w, title, (char*)-1 );
        }

    return(1);
}

//------------------------------------------------------------------------------------

static void StopWindow( struct UIKObjWindow *wobj )
{
  register struct Window *w=wobj->Obj.w;

    unlock_screen( wobj );

    if (w)
        {
        struct Screen *s = w->WScreen;
        struct UIKGlobal *uik = wobj->Obj.UIK;

        Forbid();
        if (uik->IntuiMsg)
            {
            if (uik->IntuiMsg->IDCMPWindow == w)
                {
                ReplyIntuiMsg( uik->IntuiMsg );
                uik->IntuiMsg = 0;
                }
            }
        UIK_StripWindowMsg( wobj );
        w->UserPort = 0;
        ModifyIDCMP( w, 0 );
        Permit();
        wobj->NW.LeftEdge = w->LeftEdge;
        wobj->NW.TopEdge = w->TopEdge;
        CloseWindow( w ); wobj->Obj.w = 0;
        wobj->Obj.UIK->WindowNum--;
        if (wobj->DrawInfo) FreeScreenDrawInfo( s, wobj->DrawInfo );
        UIK_AccessPrefs( (void *) CloseDefScreen, (ULONG) s, 0 );
        }
    UIK_FreeCloneRPort( wobj->Obj.irp );
}

static void RemoveWindow( struct UIKObjWindow *wobj )
{
    unlock_screen( wobj );

    if (wobj->WFont) CloseFont( wobj->WFont );
    UIK_nl_FreeList( &wobj->ShortCutList );
}

static void GenericResize( struct UIKObjWindow *wobj )
{
  struct Rectangle *rec = &wobj->Obj.ObjExt->InnerRect;

//kprintf( "GenericResize\n" );
    rec->MinX = wobj->LeftBorderWidth;
    rec->MinY = wobj->TopBorderHeight;
    rec->MaxX = wobj->Obj.Box.Width - wobj->RightBorderWidth - 1;
    rec->MaxY = wobj->Obj.Box.Height - wobj->BottomBorderHeight - 1;
}

static int AddWindow( struct UIKObjWindow *wobj, struct TagItem *taglist )
{
  struct TextFont *wantedfont;
  ULONG refresfflags;
  struct UIKWinScrInfo si;
  UBYTE *name;
  struct UIKObjWindow *wo;
  APTR scradd=0;

    /*---------------- screen --------------*/
    if (wo = parent_window( wobj )) scradd = wo->ScrAdd;

    if (scradd = (APTR) UIK_GetTagData( UIKTAG_OBJWindow_ScreenAddr, (ULONG)scradd, taglist ))
        {
        scradd = screen_exists( scradd, 0 );
        }
    else if (TRUE == UIK_GetTagData( UIKTAG_OBJWindowFl_FrontScreen, FALSE, taglist ))
        {
        scradd = IntuitionBase->FirstScreen;
        }
    else if (name = (APTR) UIK_GetTagData( UIKTAG_OBJWindow_PubScreenName, 0, taglist ))
        {
        if (UIK_IsSystemVersion2())
            { if (scradd = LockPubScreen( name )) wobj->WinFlags |= UIKW_SCREENLOCKED; }
        else{
            scradd = screen_exists( 0, name );
            }
        }
    wobj->ScrAdd = scradd;
    get_UIKWinScrInfo( wobj, &si );

    /*---------------- aspect --------------*/
    wobj->Obj.XAspect = 0;  /* So we can tell when we've got it */
    if (UIK_IsSystemVersion2())
        {
        struct DisplayInfo DI;
        if (GetDisplayInfoData( NULL, (UBYTE *) &DI, sizeof(struct DisplayInfo), DTAG_DISP, si.ModeId ))
            {
            wobj->Obj.XAspect = DI.Resolution.x;
            wobj->Obj.YAspect = DI.Resolution.y;
            }
        }
    if (! wobj->Obj.XAspect) /* If running under 1.3 or GetDisplayInfoData failed, use old method of guessing aspect ratio */
        {
        wobj->Obj.XAspect = 44;
        wobj->Obj.YAspect = ((struct GfxBase *)GfxBase)->DisplayFlags & PAL ? 44 : 52;
        if (si.ModeId & HIRES) wobj->Obj.XAspect = wobj->Obj.XAspect >> 1;
        if (si.ModeId & LACE)  wobj->Obj.YAspect = wobj->Obj.YAspect >> 1;
        }
    wobj->Obj.AutoRatio = wobj->Obj.YAspect / wobj->Obj.XAspect;
    if (wobj->Obj.AutoRatio == 0) wobj->Obj.AutoRatio = 1;
    if (wobj->Obj.AutoRatio > 2) wobj->Obj.AutoRatio = 2;

    /*--------------------------------------*/
    wobj->IconColors = *((ULONG*)&wobj->Obj.ActBoxPen) = UIK_GetTagData( UIKTAG_OBJ_ActInactPens, (UIKCOL_LIGHT<<24 | UIKCOL_GREY<<16 | UIKCOL_BLACK<<8 | UIKCOL_WHITE), taglist );

    wobj->WinFlags |= UIK_GetTagData( UIKTAG_OBJWindow_Flags,
        UIKW_DRAG | UIKW_ICON | UIKW_DEPTH | UIKW_CLOSE | UIKW_SIZE |
        UIKW_ZOOM | UIKW_AUTOADJUSTSIZE | UIKW_AUTOADJUSTLEFTTOP |
        UIKW_MENULOOK,
        taglist );
    wobj->WinFlags |= UIKW_CLOSESTOP; // force le défaut pour compatibilité
    wobj->WinFlags = UIK_PackBoolTags( wobj->WinFlags, taglist, flags_boolmap );
    //--- force le look System si non spécifé
    if ((UIKBase->UIKPrefs->PrefsFlags & UIKF_SYSLOOK) && !UIK_FindTagItem( UIKTAG_OBJWindowFl_SystemLook, taglist ))
        wobj->WinFlags |= UIKW_SYSTEMLOOK;

    refresfflags = (wobj->WinFlags & UIKW_SIMPLEREFRESH) ?
        WFLG_SIMPLE_REFRESH : WFLG_SMART_REFRESH | WFLG_NOCAREREFRESH;
    wobj->NW.Flags     = (wobj->WinFlags & UIKW_ICONWINDOW)
        ? refresfflags|RMBTRAP|BORDERLESS
        : refresfflags|RMBTRAP|BORDERLESS|ACTIVATE|REPORTMOUSE;
    if (wobj->WinFlags & UIKW_STARTINACTIVE) wobj->NW.Flags &= ~WFLG_ACTIVATE;
    if (is_syslook( wobj ))
        if (is_sysborder( wobj ))
            wobj->NW.Flags &= ~BORDERLESS;

    wobj->NW.Type      = WBENCHSCREEN;
    if (wobj->WinFlags & UIKW_BACKDROP) wobj->NW.Flags |= WFLG_BACKDROP;
    if (wobj->WinFlags & UIKW_MENULOOK) wobj->NW.Flags |= WFLG_NEWLOOKMENUS;

    wantedfont = wobj->Obj.ObjExt->Font;
    if (! (wobj->WFont = UIK_ReopenFont( wantedfont ? wantedfont : UIKBase->DefaultFont ))) return(0);
    if (! wantedfont)
        {
        wobj->Obj.ObjExt->Font = wobj->WFont;
        UIK_Font2TTA( wobj->WFont, &wobj->Obj.ObjExt->TTA );
        }

    if (wobj->CustomBitMap = (struct BitMap *) UIK_GetTagData( UIKTAG_OBJWindow_CustomBitMap, 0, taglist ))
        wobj->WinFlags |= UIKW_BITMAPED;
    wobj->CustomAltBitMap = (struct BitMap *) UIK_GetTagData( UIKTAG_OBJWindow_CustomAltBitMap, (ULONG) wobj->CustomBitMap, taglist );

    *((ULONG*)&wobj->NW.MaxWidth) = UIK_GetTagData( UIKTAG_OBJWindow_MaximumWidthHeight, -1, taglist );
    *((ULONG*)&wobj->NW.MinWidth) = UIK_GetTagData( UIKTAG_OBJWindow_MinimumWidthHeight, SETL(wobj->Obj.Box.Width,wobj->Obj.Box.Height), taglist );

    if (! AddWindowObjects( wobj, taglist, &si )) return(0);

    wobj->NW.LeftEdge = wobj->Obj.Box.Left;
    wobj->NW.TopEdge  = wobj->Obj.Box.Top;
    wobj->NW.Width    = wobj->Obj.Box.Width;
    wobj->NW.Height   = wobj->Obj.Box.Height;
    if (UIK_GetTagData( UIKTAG_OBJWindowFl_AutoCenter, 0, taglist ))
        {
        struct MaxBox b;
        WORD iconheight = ((struct TextFont *)IconFont( wobj ))->tf_YSize + 3;
//kprintf( "\nsiw=%ld, sih=%ld\n", si.Width, si.Height );
        get_overscan( wobj, &b, (struct UIKBox *)&si.Left, si.ModeId, &iconheight );
        b.maxt += iconheight; b.maxh -= iconheight;
//kprintf( "mw=%ld, mh=%ld\n", b.maxw, b.maxh );
        wobj->NW.LeftEdge = b.maxl + (b.maxw - wobj->NW.Width) / 2;
        wobj->NW.TopEdge = b.maxt + (b.maxh - wobj->NW.Height) / 2;
//kprintf( "l=%ld, t=%ld, w=%ld, h=%ld\n", wobj->NW.LeftEdge, wobj->NW.TopEdge, wobj->NW.Width, wobj->NW.Height );
        }

    GenericResize( wobj );
    wobj->Obj.Box.Left = wobj->Obj.Box.Top = 0;
    return(1);
}

static int SelectFunc( struct UIKObj *ow )
{
    if (ow->w) ActivateWindow( ow->w );
    return(0);
}

static ULONG __far FuncTable[] = {
    (ULONG) ZipWindowObj,
    (ULONG) ChangeWindowTitle,
    (ULONG) get_UIKWinScrInfo,
    };

static struct TagItem __far GermTagList[] =
{
   UIKTAG_GEN_LastFuncId,   UIKFUNC_Window_LastCmd,
   TAG_END
};

struct UIKObjGerm __far WindowGerm =
    {
    "UIKObj_Window",
    AddWindow,                  // AddVector
    StartWindow,                // StartVector
    StopWindow,                 // StopVector
    RemoveWindow,               // RemoveVector
    DrawWindow,                 // RefreshVector
    GenericResize,              // ResizeVector
    NULL,                       // ExceptActVector
    NULL,                       // ExceptInaVector
    NULL,                       // ExceptMovVector
    NULL,                       // ExceptKeyVector
    AfterStartWindow,           // StartAfterVector
    SelectFunc,                 // SelectVector
    FuncTable,                  // ExtentedVectors
    GermTagList,                /*struct TagItem *GermTagList; */
    0,0,
    (300<<16)|150,
    UIKID_WINDOW,
    sizeof(struct UIKObjWindow),
    BOX_DISPLAYABLE | BOX_OVER | BOX_SEMAPHORE | BOX_CANTDISABLE,
    };

static void ajust_window_size( struct UIKObjWindow *wobj, WORD *woffs, WORD *hoffs, struct MaxBox *b )
{
    *woffs = wobj->LeftBorderWidth + wobj->RightBorderWidth;
    *hoffs = wobj->TopBorderHeight + wobj->BottomBorderHeight;
    if (wobj->WinFlags & UIKW_AUTOADJUSTSIZE)
        {
        if (wobj->NW.MaxWidth != -1) wobj->NW.MaxWidth += *woffs;
        if (wobj->NW.MaxHeight != -1) wobj->NW.MaxHeight += *hoffs;
        if (wobj->NW.MinWidth != -1) wobj->NW.MinWidth += *woffs;
        if (wobj->NW.MinHeight != -1) wobj->NW.MinHeight += *hoffs;
        }
    wobj->OldLeft   = wobj->Obj.Box.Left; wobj->OldTop    = wobj->Obj.Box.Top;
    wobj->OldWidth  = wobj->NW.MinWidth;  wobj->OldHeight = wobj->NW.MinHeight;
    if (wobj->WinFlags & UIKW_FULLWIDTH)
        {
        wobj->Obj.Box.Left = b->maxl;
        wobj->Obj.Box.Width = b->maxw;
        if (wobj->WinFlags & UIKW_AUTOADJUSTSIZE) wobj->Obj.Box.Width -= *woffs;
        }
    if (wobj->WinFlags & UIKW_FULLHEIGHT)
        {
        wobj->Obj.Box.Top = b->maxt;
        wobj->Obj.Box.Height = b->maxh;
        if (wobj->WinFlags & UIKW_AUTOADJUSTSIZE) wobj->Obj.Box.Height -= *hoffs;
        }
    if (wobj->WinFlags & UIKW_AUTOADJUSTSIZE)
        {
        wobj->Obj.Box.Width += *woffs; wobj->Obj.Box.Height += *hoffs;
        }
    if (wobj->WinFlags & UIKW_AUTOSHRINK)
        {
        if (wobj->Obj.Box.Width > b->maxw)
            { wobj->ShrunkWidth = b->maxw - wobj->Obj.Box.Width; wobj->Obj.Box.Width = b->maxw; }
        if (wobj->Obj.Box.Height > b->maxh)
            { wobj->ShrunkHeight = b->maxh - wobj->Obj.Box.Height; wobj->Obj.Box.Height = b->maxh; }
        if (wobj->NW.MinWidth > b->maxw) { wobj->NW.MinWidth = b->maxw; }
        if (wobj->NW.MinHeight > b->maxh) { wobj->NW.MinHeight = b->maxh; }
        }
    if (wobj->Obj.Box.Width < wobj->NW.MinWidth) wobj->Obj.Box.Width = wobj->NW.MinWidth;
    if (wobj->Obj.Box.Height < wobj->NW.MinHeight) wobj->Obj.Box.Height = wobj->NW.MinHeight;
}

static ULONG AddWindowObjects( struct UIKObjWindow *wobj, struct TagItem *taglist, struct UIKWinScrInfo *si )
{
  UWORD left, top, width, height;
  ULONG master, dlt, dwh, ic = wobj->WinFlags & UIKW_ICONWINDOW;
  WORD width_offs, height_offs, givenheight, hires=0, iconheight=0;
  struct MaxBox b;
  /*
  struct UIKObjScreen *so;
  struct Screen *scr=0;
  struct TextAttr *ta=0;
  UWORD scwidth=640, scheight=200;
  */
  struct TagItem ti[] = {
      { UIKTAG_OBJ_LeftTop,           },  // 0
      { UIKTAG_OBJ_WidthHeight,       },  // 1
      { UIKTAG_OBJ_Sz_Attached_Flags, },  // 2
      { UIKTAG_OBJ_BoxAutomate,       },  // 3
      { UIKTAG_OBJ_BoxAltAutomate,    },  // 4
      { UIKTAG_OBJ_BoxFl_TranslateLeftTop, FALSE },
      { TAG_IGNORE,                   },
      { TAG_END },
      };

    b = *((struct MaxBox *)&si->Left);
    if (wobj->WinFlags & UIKW_NEARFULL)
        {
        iconheight = ((struct TextFont *)IconFont( wobj ))->tf_YSize + 3;
        }
    if (wobj->WinFlags & UIKW_FULLDISPL)
        {
        get_overscan( wobj, &b, (struct UIKBox *)&si->Left, si->ModeId, &iconheight );
        }
    b.maxt += iconheight; b.maxh -= iconheight;

    master = (ULONG) wobj->Obj.ObjExt->Lang->Master;
    hires = si->ModeId & HIRES ? 1 : 0;

    if (is_syslook( wobj ))
        {
        if (is_sysborder( wobj ))
            {
            givenheight = wobj->Obj.Box.Height;

            //------épaisseur du périmètre
            wobj->LeftBorderWidth = wobj->RightBorderWidth = 4;
            wobj->TopBorderHeight = wobj->BottomBorderHeight = 2;
            if (si->Scr)
                {
                wobj->LeftBorderWidth    = si->Scr->WBorLeft;
                wobj->RightBorderWidth   = si->Scr->WBorRight;
                wobj->TopBorderHeight    = si->Scr->WBorTop;
                wobj->BottomBorderHeight = si->Scr->WBorBottom;
                }

            //------dimension du bord haut
            if ((wobj->WinFlags & (UIKW_DEPTH|UIKW_CLOSE|UIKW_DRAG)) || wobj->Obj.Title)
                wobj->TopBorderHeight += /*ta->ta_YSize*/ si->Font->tf_YSize + 1;

            //------dimension du bord droit
            //if (wobj->WinFlags & UIKW_SIZE)       // @@@A
            //    wobj->RightBorderWidth += (hires ? 16 : 11) - 2;

            //------dimension du bord bas
            if (wobj->WinFlags & UIKW_SIZE)
                wobj->BottomBorderHeight += (hires ? 8 : 9);
            else if ((wobj->WinFlags & UIKW_AUTOADJUSTSIZE) && (givenheight == 0))
                wobj->BottomBorderHeight = 0;

            //------ajuste la taille de la fenêtre
            ajust_window_size( wobj, &width_offs, &height_offs, &b );

            //------si on a les gadgets system
            if (wobj->WinFlags & UIKW_CLOSE) wobj->NW.Flags |= WFLG_CLOSEGADGET;
            if (wobj->WinFlags & UIKW_DEPTH) wobj->NW.Flags |= WFLG_DEPTHGADGET;
            if (wobj->WinFlags & UIKW_DRAG)  wobj->NW.Flags |= WFLG_DRAGBAR;
            if (wobj->WinFlags & UIKW_SIZE)  wobj->NW.Flags |= WFLG_SIZEGADGET|WFLG_SIZEBBOTTOM; // @@@A
            if (wobj->Obj.Title)  wobj->NW.Title = UIK_ObjTitle( wobj );
            }
        else{
            //------ajuste la taille de la fenêtre
            ajust_window_size( wobj, &width_offs, &height_offs, &b );

            if (wobj->WinFlags & UIKW_DRAG)  wobj->NW.Flags |= WFLG_DRAGBAR;
            }
        }
    else{
        //------épaisseur du périmètre
        wobj->LeftBorderWidth = wobj->RightBorderWidth =  wobj->TopBorderHeight = wobj->BottomBorderHeight
            = ((wobj->Obj.Flags & BOX_NDRAWN) || (wobj->WinFlags & UIKW_NOPERIMETER)) ? 0 : 1;

        //------dimensions de la barre de titre
        left = wobj->LeftBorderWidth;
        top = wobj->TopBorderHeight;
        height = 0;
        if (ic) height = wobj->Obj.Box.Height;
        else if ((wobj->WinFlags & (UIKW_DEPTH|UIKW_ZOOM|UIKW_ICON|UIKW_CLOSE|UIKW_DRAG)) || wobj->Obj.Title)
            height = wobj->WFont->tf_YSize + 6;

        //------ajuste l'épaisseur des bords
        wobj->TopBorderHeight += height;
        if (wobj->WinFlags & UIKW_SIZE)
            wobj->BottomBorderHeight = wobj->WFont->tf_YSize / 2 + 1; // recouvre le perimètre (s'il y en a un...)
        else wobj->WinFlags &= ~UIKW_ZOOM;

        //------ajuste la taille de la fenêtre
        ajust_window_size( wobj, &width_offs, &height_offs, &b );

        //------si on a une barre de titre...
        if (height)
            {
            WORD offs1=0, offs2=0;

            width = wobj->Obj.Box.Width - width_offs; // dimension de la barre de titre

            //------sauvegarde pour l'objet dragg/box
            dlt = SETL(left,top);
            dwh = SETL(width,height);

            //------dimensions de l'intérieur du dragg
            if (! ic) { left += 1; top += 1; width -= 1; height -= 2; }

            //------ajout des objets
            if (! IntUIK_AddObjectTags( "UIKObj_Box", wobj,   // dessin dur dragg
                    UIKTAG_OBJ_LeftTop,           dlt,
                    UIKTAG_OBJ_WidthHeight,       dwh,
                    UIKTAG_OBJ_Sz_Attached_Flags, UO_ATTACHED_LEFT | UO_ATTACHED_TOP | UO_ATTACHED_RIGHT,
                    UIKTAG_OBJ_BoxFl_TranslateLeftTop, FALSE,
                    UIKTAG_OBJ_BoxFl_NotDrawn,    (ic ? TRUE : FALSE),
                    UIKTAG_OBJ_BoxFl_CannotDisable, TRUE,
                    TAG_END )) goto END_ERROR;

            if (wobj->WinFlags & UIKW_CLOSE)
                {
                ti[0].ti_Data = SETL(left,top); ti[1].ti_Data = SETL(SYSGAD_WIDTH,height);
                ti[2].ti_Data = 0;
                ti[3].ti_Data = (ULONG) CloseAuto; ti[4].ti_Data = (ULONG) CloseAltAuto;
                ti[6].ti_Tag = UIKTAG_OBJ_ShortCutKey; ti[6].ti_Data = UIK_GetTagData( UIKTAG_OBJWindow_Close_ShortKey, B2L(0,0x45,0,0), taglist );
                if (! UIK_AddObject( "UIKObj_WindowClose", wobj, ti )) goto END_ERROR;
                offs1 += SYSGAD_WIDTH;
                }
            if (wobj->WinFlags & UIKW_ICON)
                {
                ti[0].ti_Data = SETL(left+offs1,top); ti[1].ti_Data = SETL(SYSGAD_WIDTH,height);
                ti[2].ti_Data = 0;
                ti[3].ti_Data = (ULONG) IconAuto; ti[4].ti_Data = (ULONG) IconAltAuto;
                ti[6].ti_Tag = UIKTAG_OBJ_ShortCutKey; ti[6].ti_Data = UIK_GetTagData( UIKTAG_OBJWindow_Icon_ShortKey, B2L(0,0,0,0), taglist );
                if (! UIK_AddObject( "UIKObj_WindowIconify", wobj, ti )) goto END_ERROR;
                offs1 += SYSGAD_WIDTH;
                }
            if (wobj->WinFlags & UIKW_DEPTH)
                {
                ti[0].ti_Data = SETL(left+width-SYSGAD_WIDTH,top); ti[1].ti_Data = SETL(SYSGAD_WIDTH,height);
                ti[2].ti_Data = UO_ATTACHED_TOP | UO_ATTACHED_RIGHT;
                ti[3].ti_Data = (ULONG) DepthAuto; ti[4].ti_Data = (ULONG) DepthAltAuto;
                ti[6].ti_Tag = UIKTAG_OBJ_ShortCutKey; ti[6].ti_Data = UIK_GetTagData( UIKTAG_OBJWindow_Depth_ShortKey, B2L(0,0,0,0), taglist );
                if (! UIK_AddObject( "UIKObj_WindowDepth", wobj, ti )) goto END_ERROR;
                offs2 += SYSGAD_WIDTH;
                }
            if (wobj->WinFlags & UIKW_ZOOM)
                {
                ti[0].ti_Data = SETL(left+width-SYSGAD_WIDTH-offs2,top); ti[1].ti_Data = SETL(SYSGAD_WIDTH,height);
                ti[2].ti_Data = UO_ATTACHED_TOP | UO_ATTACHED_RIGHT;
                ti[3].ti_Data = (ULONG) ZoomAuto; ti[4].ti_Data = (ULONG) ZoomAltAuto;
                ti[6].ti_Tag = UIKTAG_OBJ_ShortCutKey; ti[6].ti_Data = UIK_GetTagData( UIKTAG_OBJWindow_Zoom_ShortKey, B2L(0,0,0,0), taglist );
                if (! UIK_AddObject( "UIKObj_WindowZoom", wobj, ti )) goto END_ERROR;
                offs2 += SYSGAD_WIDTH;
                }

            if (wobj->WinFlags & UIKW_DRAG)
                if (! IntUIK_AddObjectTags( "UIKObj_WindowDragg", wobj,
                        UIKTAG_OBJ_LeftTop,           dlt,
                        UIKTAG_OBJ_WidthHeight,       dwh,
                        UIKTAG_OBJ_Sz_Attached_Flags, UO_ATTACHED_LEFT | UO_ATTACHED_TOP | UO_ATTACHED_RIGHT,
                        UIKTAG_OBJ_BoxFl_TranslateLeftTop, FALSE,
                        UIKTAG_OBJ_BoxAutomate,       DraggAuto,
                        TAG_END )) goto END_ERROR;

            if (wobj->Obj.Title)
                {
                WORD offs=MAX(offs1,offs2);
                /*
                UBYTE *title = wobj->Obj.Title;
                ULONG flags = wobj->Obj.TitleFlags;

                if ((flags & TITLE_IMAGE) == 0)
                    {
                    flags |= TITLE_ADDR;
                    title = UIK_ObjTitle( wobj );
                    }
                */

                left += offs;
                width -= (2*offs);
                }

            if (ic || wobj->Obj.Title)
                {
                if (! (wobj->TitleBox = (struct UIKObj *) IntUIK_AddObjectTags( "UIKObj_Box", wobj,
                        UIKTAG_OBJ_LeftTop,           SETL(left,top),
                        UIKTAG_OBJ_WidthHeight,       SETL(width,height),
                        UIKTAG_OBJ_Box_Flags,         BOX_FILLED,
                        UIKTAG_OBJ_Sz_Attached_Flags, UO_ATTACHED_LEFT | UO_ATTACHED_TOP | UO_ATTACHED_RIGHT,
                        UIKTAG_OBJ_Title_Flags,       wobj->Obj.TitleFlags,
                        UIKTAG_OBJ_Title,             wobj->Obj.Title,
                        UIKTAG_OBJ_ActInactPens,      *((ULONG*)&wobj->Obj.ActBoxPen),
                        UIKTAG_OBJ_BoxAutomate,       (ic ? AutoDrawBox_Over : WinTitleAuto),
                        UIKTAG_OBJ_BoxAltAutomate,    (ic ? AutoDrawBox_Under : WinTitleAltAuto),
                        UIKTAG_OBJ_BoxFl_TranslateLeftTop, FALSE,
                        UIKTAG_OBJ_LangMaster,        master,
                        UIKTAG_OBJ_BoxFl_CannotDisable, TRUE,
                        TAG_END ))) goto END_ERROR;
                }
            }

        if (wobj->WinFlags & UIKW_SIZE)
            {
            if (! IntUIK_AddObjectTags( "UIKObj_WindowSize", wobj,
                    UIKTAG_OBJ_LeftTop,           SETL(wobj->LeftBorderWidth, wobj->Obj.Box.Height - wobj->BottomBorderHeight),
                    UIKTAG_OBJ_WidthHeight,       SETL(wobj->Obj.Box.Width - wobj->LeftBorderWidth - wobj->RightBorderWidth, wobj->BottomBorderHeight),
                    UIKTAG_OBJ_Sz_Attached_Flags, UO_ATTACHED_LEFT | UO_ATTACHED_RIGHT | UO_ATTACHED_BOT,
                    UIKTAG_OBJ_BoxFl_TranslateLeftTop, FALSE,
                    TAG_END )) goto END_ERROR;
            }
        }

    return(1);

    END_ERROR: return(0);
}
