/****************************************************************
 *
 *      Project:   UIK
 *      Function:  gestion des zones de sélection
 *
 *      Created:   13/03/92    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include <intuition/intuitionbase.h>

#include "uiki:uikbase.h"
#include "uiki:uikglobal.h"
#include "uiki:uikbuf.h"
#include "lci:uik_protosint.h"

#include "uiki:objjoystick.h"
#include "uiki:objscreen.h"


/****** Macros **************************************************/


/****** Imported ****** ******************************************/

struct UIKObj * __asm win_or_req( register __a0 struct Window *w );

extern void * __asm Input_InstallTask( register __a1 struct UIKGlobal *uik );
extern struct RastPort * __asm IntUIK_GetRastPort( register __a0 struct UIKobj *obj );
extern void __asm IntUIK_Input_Cycle( register __a0 struct UIKZoneList *uzl, register __a1 struct UIKGlobal *uik, register __d0 UBYTE on );

extern struct UIKBase * __far UIKBase;
extern struct DOSBase * __far DOSBase;
extern struct GfxBase * __far GfxBase;
extern struct IntuitionBase * __far IntuitionBase;


/****** Exported ************************************************/

ULONG __asm zo_ChangeActiveZoneList( register __a0 struct UIKGlobal *uik, register __a1 struct UIKZoneList *uzl );
struct UIKZoneList * __asm zo_InstallZoneList( register __a0 struct UIKGlobal *uik, register __a1 struct UIKZoneList *uzl );
void __asm zo_MarkZone( register __a0 struct UIKGlobal *uik, register __a1 UZone *zo );
UZone * __asm zo_UnmarkZone( register __a0 struct UIKGlobal *uik );
UZone * __asm zo_FindNextZone( register __a0 struct UIKGlobal *uik );
void __asm zo_MarkNextZone( register __a0 struct UIKGlobal *uik );
void __asm zo_MarkNextZoneDir( register __a0 struct UIKGlobal *uik, register __d0 UBYTE buts );
void __asm zo_MarkFirstZone( register __a0 struct UIKGlobal *uik );
struct UIKObj * __asm zo_FindActiveObject( register __a0 struct UIKGlobal *uik );


/****** Statics *************************************************/


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

#define UBUFPTR(b) ((struct UIKBuffer *) ((ULONG)b - sizeof(struct UIKBuffer)))
#define UBUFLEN(b) (UBUFPTR(b)->CurSize)

static UZone * __asm RightZone( register __a0 UZList *uzl, register __a1 UZone *uz, register __d0 UBYTE want_started )
{
  UZone *last = (UZone *) ((ULONG)uzl->ZoneStart + UBUFLEN( uzl->ZoneStart ));

    uz++;
    while (uz < last)
        {
        if (uz->ZObj)
            {
            if (want_started) { if (uz->ZObj->Status == UIKS_STARTED) break; }
            else break;
            }
        uz++;
        }
    return( uz < last ? uz : 0 );
}

static UZone * __asm LeftZone( register __a0 UZList *uzl, register __a1 UZone *uz )
{
    uz--;
    while (uz >= uzl->ZoneStart)
        {
        if (uz->ZObj)
            if (uz->ZObj->Status == UIKS_STARTED) break;
        uz--;
        }
    return( uz >= uzl->ZoneStart ? uz : 0 );
}

static UZone * __asm FirstZone( register __a0 struct UIKGlobal *uik )
{
  struct UIKZoneList *uzl = uik->ActiveZoneList;
  UZone *uz = 0;

    if (uzl) uz = RightZone( uzl, uzl->ZoneStart - 1, 1 );
    return( uz );
}

static UZone * __asm LastZone( register __a0 struct UIKGlobal *uik )
{
  struct UIKZoneList *uzl = uik->ActiveZoneList;
  UZone *uz = 0;

    if (uzl) uz = LeftZone( uzl, (UZone *) ((ULONG)uzl->ZoneStart + UBUFLEN( uzl->ZoneStart )) );
    return( uz );
}

static UZone * __asm MidZone( register __a0 struct UIKGlobal *uik )
{
  struct UIKZoneList *uzl = uik->ActiveZoneList;
  UZone *uz = 0;
  UZone *z = 0;
  ULONG n, max;

    if (uzl)
        {
        if (uz = FirstZone( uik ))
            {
            for (z=uz, n=1; (z = RightZone( uzl, z, 1 )); n++);
            max = n / 2;
            for (n=0; n < max; n++)
                {
                z = RightZone( uzl, uz, 1 );
                if (z) uz = z;
                }
            }
        }
    return( uz );
}

static UZone * __asm OnlyLeftZone( register __a0 UZList *uzl, register __a1 UZone *uz )
{
  struct UIKObj *zo;
  UWORD X, y, Y;

    zo = uz->ZObj;
    X = zo->Box.Left + zo->Box.Width-1;
    y = zo->Box.Top;
    Y = zo->Box.Top + zo->Box.Height-1;

    while (uz = LeftZone( uzl, uz ))
        {
        zo = uz->ZObj;

        if (zo->Box.Top <= Y && zo->Box.Top + zo->Box.Height-1 >= y && zo->Box.Left + zo->Box.Width-1 <= X)
            break;
        }
    return( uz );
}

static UZone * __asm OnlyRightZone( register __a0 UZList *uzl, register __a1 UZone *uz )
{
  struct UIKObj *zo;
  UWORD x, y, Y;

    zo = uz->ZObj;
    x = zo->Box.Left;
    y = zo->Box.Top;
    Y = zo->Box.Top + zo->Box.Height-1;

    while (uz = RightZone( uzl, uz, 1 ))
        {
        zo = uz->ZObj;

        if (zo->Box.Top + zo->Box.Height-1 >= y && zo->Box.Top <= Y && zo->Box.Left > x)
            break;
        }
    return( uz );
}

static UZone * __asm OnlyUpZone( register __a0 UZList *uzl, register __a1 UZone *uz )
{
  struct UIKObj *zo;
  UWORD x, X, Y;

    zo = uz->ZObj;
    x = zo->Box.Left;
    X = zo->Box.Left + zo->Box.Width-1;
    Y = zo->Box.Top + zo->Box.Height-1;

    while (uz = LeftZone( uzl, uz ))
        {
        zo = uz->ZObj;

        if (zo->Box.Left <= X && zo->Box.Left + zo->Box.Width-1 >= x && zo->Box.Top + zo->Box.Height-1 <= Y)
            break;
        }
    return( uz );
}

static UZone * __asm OnlyDownZone( register __a0 UZList *uzl, register __a1 UZone *uz )
{
  struct UIKObj *zo;
  UWORD x, X, y;

    zo = uz->ZObj;
    x = zo->Box.Left;
    X = zo->Box.Left + zo->Box.Width-1;
    y = zo->Box.Top;

    while (uz = RightZone( uzl, uz, 1 ))
        {
        zo = uz->ZObj;

        if (zo->Box.Left + zo->Box.Width-1 >= x && zo->Box.Left <= X && zo->Box.Top > y)
            break;
        }
    return( uz );
}

//--------------------

void zo_DeleteZoneList( struct UIKObj *obj )
{
  UZList *uzl;

    if (uzl = obj->ObjExt->ZoneList)
        {
        BufFree( (UIKBUF*) uzl->ZoneStart );
        UIK_MemSpFree( uzl );
        obj->ObjExt->ZoneList = 0;
        }
}

void zo_DeleteZone( struct UIKGlobal *uik, UZone *zo )
{
    zo->ZObj = 0;
    if (uik->ActiveZoneList && uik->ActiveZoneList->ActiveZone == zo)
        uik->ActiveZoneList->ActiveZone = 0;
}

UZone * __asm UIK_AddZone( register __a0 struct UIKObj *master, register __a1 struct UIKObj* newobj, register __d0 ULONG events, register __d1 void (*func)() )
{
  UZList *uzl;
  UZone *zp, *uz;

    if (!master || !newobj) return(0);

    uzl = master->ObjExt->ZoneList;
    if (! uzl)
        {
        if (! (uzl = (UZList *) UIK_MemSpAlloc( sizeof(UZList), MEMF_ANY | MEMF_CLEAR ))) return(0);
        if (! (uz = (UZone *) BufAlloc( UZONE_ALLOC_BLOCK, sizeof(UZone), MEMF_ANY | MEMF_CLEAR )))
            { UIK_MemSpFree( uzl ); return(0); }
        master->ObjExt->ZoneList = uzl;
        uzl->ZoneStart = uz;
        }
    else{
        //zo_UnmarkZone( master->UIK );

        zp = uz = uzl->ZoneStart - 1;
        while (uz = RightZone( uzl, uz, 0 ))
            {
            struct UIKObj *zo = uz->ZObj;

            if (newobj->Box.Top + newobj->Box.Height-1 < zo->Box.Top)
                break;

            if (newobj->Box.Left < zo->Box.Left && newobj->Box.Top <= zo->Box.Top + zo->Box.Height-1)
                break;

            zp = uz;
            }
        zp++;

        if (zp->ZObj || !uz)  // place libre ou à la fin
            {
            UBYTE *newstart;
            ULONG inspos = (ULONG)zp - (ULONG)uzl->ZoneStart;

            if (! (newstart = BufPaste( (UBYTE*) zp, sizeof(UZone), (UIKBUF*) uzl->ZoneStart, inspos ))) return(0);
            uz = (UZone *) ((ULONG)newstart + inspos);
            uzl->ZoneStart = (UZone *) newstart;
            }
        else uz = zp;
        }
    uz->ZObj = newobj;
    uz->ZClass = events;
    uz->ZFunc = func;
    return( uz );
}

//----------------------------------------------------------------------

static int cross_findstartedobj( struct UIKObj *obj, struct UIKObj *findobj )
{
    if (obj == findobj && obj->Status == UIKS_STARTED) return(0); // trouvé, on arrête là
    return( UIK_nl_CrossList( obj, (void*)cross_findstartedobj, 1, (ULONG) findobj ) );
}
static int cross_findobj( struct UIKObj *obj, struct UIKObj *findobj )
{
    if (obj == findobj && obj->Status != UIKS_REMOVING) return(0); // trouvé, on arrête là
    return( UIK_nl_CrossList( obj, (void*)cross_findobj, 1, (ULONG) findobj ) );
}
static int VerifZoneObj( struct UIKGlobal *uik, UZone *zo )
{
    if (! UIK_nl_CrossList( uik, (void*)cross_findobj, 1, (ULONG) zo->ZObj )) return(1);
    zo_DeleteZone( uik, zo );
    return(0);
}

UZone * __asm zo_UnmarkZone( register __a0 struct UIKGlobal *uik )
{
  UZone *zo=0;

    if (uik->ActiveZoneList && (zo = uik->ActiveZoneList->ActiveZone))
        {
        if (VerifZoneObj( uik, zo ))
            {
            struct UIKObj *obj = zo->ZObj;
            struct RastPort *rp;
            UWORD hthick = UIKBase->UIKPrefs->ZoneThick;
            UWORD wthick = hthick * obj->AutoRatio;
            WORD  l, t;
            UWORD w, h;
            WORD  X, Y;

            if (obj->Flags & BOX_INNERZONE)
                {
                struct Rectangle *rect = &obj->ObjExt->InnerRect;
                l = rect->MinX - wthick;
                t = rect->MinY - hthick;
                X = rect->MaxX + wthick;
                Y = rect->MaxY + hthick;
                w = X - l + 1;
                h = Y - t + 1;
                }
            else{
                l = obj->Box.Left - wthick;
                t = obj->Box.Top - hthick;
                w = obj->Box.Width + 2 * wthick;
                h = obj->Box.Height + 2 * hthick;
                X = l + w - 1;
                Y = t + h - 1;
                }

            if (uik->ZonePlanesH && uik->ZonePlanesW)
                if (rp = IntUIK_GetRastPort( obj))
                    {
                    WaitBOVP( &obj->w->WScreen->ViewPort );
                    ClipBlit( uik->ZoneRPortH, 0,0, rp, l,t, wthick,h, 0xC0 );
                    ClipBlit( uik->ZoneRPortH, wthick,0, rp, X-(wthick-1),t, wthick,h, 0xC0 );
                    ClipBlit( uik->ZoneRPortW, 0,0, rp, l+wthick,t, w-2*wthick,hthick, 0xC0 );
                    ClipBlit( uik->ZoneRPortW, 0,hthick, rp, l+wthick,Y-(hthick-1), w-2*wthick,hthick, 0xC0 );
                    }
            uik->ActiveZoneList->ActiveZone = 0;
            }
        else zo = 0;
        }
    return( zo );
}

void __asm zo_MarkZone( register __a0 struct UIKGlobal *uik, register __a1 UZone *zo )
{
    zo_UnmarkZone( uik );

    if (uik->ActiveZoneList && zo && VerifZoneObj( uik, zo ))
        {
        struct UIKObj *obj = zo->ZObj;
        struct RastPort *rp = obj->rp;
        if (rp)
            {
            struct Screen *sc = obj->w->WScreen;
            UBYTE d = sc->BitMap.Depth;
            UWORD hthick = UIKBase->UIKPrefs->ZoneThick;
            UWORD wthick = hthick * obj->AutoRatio;
            WORD  l, t;
            UWORD w, h;
            WORD  X, Y;
            ULONG oldlen, newlen;
            struct BitMap *bm;
            UBYTE i, *p, pen;
            ULONG sizeh, sizew;

            if (obj->Flags & BOX_INNERZONE)
                {
                struct Rectangle *rect = &obj->ObjExt->InnerRect;
                l = rect->MinX - wthick;
                t = rect->MinY - hthick;
                X = rect->MaxX + wthick;
                Y = rect->MaxY + hthick;
                w = X - l + 1;
                h = Y - t + 1;
                }
            else{
                l = obj->Box.Left - wthick;
                t = obj->Box.Top - hthick;
                w = obj->Box.Width + 2 * wthick;
                h = obj->Box.Height + 2 * hthick;
                X = l + w - 1;
                Y = t + h - 1;
                }
            sizeh = d * RASSIZE(wthick,h);           // gauche et droit
            sizew = d * RASSIZE(w-2*wthick,hthick);  // haut et bas

            oldlen = UIK_MemSpLength( uik->ZonePlanesH );
            newlen = 2 * sizeh;
            if (newlen > oldlen)
                {
                if (! (p = UIK_MemSpAlloc( newlen, MEMF_CHIP ))) return;
                UIK_MemSpFree( uik->ZonePlanesH );
                uik->ZonePlanesH = p;
                }
            bm = uik->ZoneRPortH->BitMap;
            InitBitMap( bm, d, 2*wthick, h );   // gauche et droit cote à cote
            for (i=0, p=uik->ZonePlanesH; i<d; i++) { bm->Planes[i] = p; p += (bm->Rows * bm->BytesPerRow); }
            ClipBlit( rp, l,t, uik->ZoneRPortH, 0,0, wthick,h, 0xc0 );
            ClipBlit( rp, X-(wthick-1),t, uik->ZoneRPortH, wthick,0, wthick,h, 0xc0 );

            oldlen = UIK_MemSpLength( uik->ZonePlanesW );
            newlen = 2 * sizew;
            if (newlen > oldlen)
                {
                if (! (p = UIK_MemSpAlloc( newlen, MEMF_CHIP ))) return;
                UIK_MemSpFree( uik->ZonePlanesW );
                uik->ZonePlanesW = p;
                }
            bm = uik->ZoneRPortW->BitMap;
            InitBitMap( bm, d, w-2*wthick, 2*hthick ); // haut et bas en dessous
            for (i=0, p=uik->ZonePlanesW; i<d; i++) { bm->Planes[i] = p; p += (bm->Rows * bm->BytesPerRow); }
            ClipBlit( rp, l+wthick,t, uik->ZoneRPortW, 0,0, w-2*wthick,hthick, 0xc0 );
            ClipBlit( rp, l+wthick,Y-(hthick-1), uik->ZoneRPortW, 0,hthick, w-2*wthick,hthick, 0xc0 );

            pen = obj->sc ? ((struct UIKObjScreen *)obj->sc->UserData)->Colors.ColZone
                     : UIKBase->UIKPrefs->Colors.ColZone;
            SetAPen( rp, pen );

            WaitBOVP( &sc->ViewPort );
            RectFill( rp, l, t, l+(wthick-1), Y );
            RectFill( rp, X-(wthick-1), t, X, Y );
            RectFill( rp, l+wthick, t, X-wthick, t+(hthick-1) );
            RectFill( rp, l+wthick, Y-(hthick-1), X-wthick, Y );
            }
        uik->ActiveZoneList->ActiveZone = zo;
        }
}

//----------------------------------------------------------------------

void __asm ZoneUpdateStart( register __a0 struct UIKObj *obj )
{
  struct UIKGlobal *uik = obj->UIK;
  struct UIKZoneList *uzl = uik->ActiveZoneList;
  UZone *oz;

    if (uzl)
        {
        oz = uzl->ActiveZone;
        if (oz && oz->ZObj == obj)
            {
            zo_UnmarkZone( uik );
            uzl->ActiveZone = oz;
            }
        }
}

void __asm ZoneUpdateEnd( register __a0 struct UIKObj *obj )
{
  struct UIKGlobal *uik = obj->UIK;
  struct UIKZoneList *uzl = uik->ActiveZoneList;
  UZone *oz;

    if (uzl)
        {
        oz = uzl->ActiveZone;
        if (oz && oz->ZObj == obj)
            {
            uzl->ActiveZone = 0;
            zo_MarkZone( uik, oz );
            }
        }
}

//----------------------------------------------------------------------

void zo_ObjStopping( struct UIKObj *obj )
{
  UZone *zo;
  struct UIKGlobal *uik = obj->UIK;
  struct UIKZoneList *uzl = uik->ActiveZoneList;
  struct UIKZoneList *ozl = obj->ObjExt->ZoneList;

    if (uik->ZoneListBase == ozl) uzl = ozl;

    if (uzl)
        {
        if (uzl == ozl) // par exemple objet fenêtre
            {
            zo_UnmarkZone( uik );
            uik->ActiveZoneList = 0;
            if (uik->ZoneListBase == uzl) uik->ZoneListBase = 0;
            }
        else if (zo = uzl->ActiveZone) // gadget entouré
            {
            if (obj == zo->ZObj)
                {
                zo_UnmarkZone( uik );
                }
            }
        }
}

ULONG __asm zo_ChangeActiveZoneList( register __a0 struct UIKGlobal *uik, register __a1 struct UIKZoneList *uzl )
{
  ULONG rc=0;

    if (! (uik->InputFlags & INPUTF_JAMED))
        {
        if (uzl && !(uzl->Flags & UZLF_NOCYCLE)) // test ici car il faut peut-être laisser l'ancienne zone
            {
            //zo_UnmarkZone( uik );
            //uik->ActiveZoneList = uzl;
            IntUIK_Input_Cycle( uzl, uik, 1 );  // appellera zo_MarkFirstZone() si pas de zone active
            rc = 1;
            }
        }
    return( rc );
}

static int cross_findwindow( struct UIKObj *obj, struct UIKZoneList **tmp )
{
    if (obj->Id == UIKID_WINDOW && obj->Status == UIKS_STARTED && obj->w)
        {
        obj = win_or_req( obj->w );
        if (*tmp = obj->ObjExt->ZoneList) return(0); // trouvé, on arrête là
        }
    return( UIK_nl_CrossList( obj, (void*)cross_findwindow, 1, (ULONG) tmp ) );
}
struct UIKZoneList * __asm zo_InstallZoneList( register __a0 struct UIKGlobal *uik, register __a1 struct UIKZoneList *uzl )
{
    if (! (uik->InputFlags & INPUTF_JAMED))
        {
        zo_UnmarkZone( uik );

        uik->ZoneListBase = uik->ActiveZoneList = 0;

        if (! uzl)
            {
            struct Window *w;
            struct UIKObj *o;
            struct UIKZoneList *tmp=0;

            if (w = IntuitionBase->ActiveWindow)
                {
                if (! UIK_nl_CrossList( uik, (void*)cross_findstartedobj, 1, (ULONG) w->UserData ))
                    {
                    o = win_or_req( w );
                    tmp = o->ObjExt->ZoneList;
                    }
                }
            if (! tmp)
                {
                UIK_nl_CrossList( uik, (void*)cross_findwindow, 1, (ULONG) &tmp );
                }
            uzl = tmp;
            }

        if (uzl)
            {
            uik->ZoneListBase = uzl;
            if (uik->InputMode >= INPUTMODE_1CONTACT && uik->InputMode <= INPUTMODE_5CONTACTS)
                if (uik->ActiveZoneList != uzl || !uzl->ActiveZone)
                    if (! (uzl->Flags & UZLF_NOCYCLE))
                        {
                        uik->ActiveZoneList = uzl;
                        zo_MarkFirstZone( uik );
                        }
            }
        }
    else uzl = 0;

    Input_InstallTask( uik ); // l'objet Inputick doit fonctionner même si uzl == 0

    return( uzl );
}

//----------------------------------------------------------------------

/*
UZone * __asm zo_FindPrevZone( register __a0 struct UIKGlobal *uik )
{
  struct UIKZoneList *uzl = uik->ActiveZoneList;
  UZone *uz=0;

    if (uzl)
        {
        if (uz = uzl->ActiveZone) uz = LeftZone( uzl, uz );
        if (! uz) uz = LastZone( uik );
        }
    return( uz );
}
*/

UZone * __asm zo_FindPrevDirZone( register __a0 struct UIKGlobal *uik )
{
  struct UIKZoneList *uzl = uik->ActiveZoneList;
  UZone *start, *zs, *uz=0;

    if (uzl)
        {
        if (start = uz = uzl->ActiveZone)
            {
            uz = OnlyLeftZone( uzl, uz );
            if (! uz) // repart à droite
                {
                uz = start;

                if (!uik->InputAuto || !uik->InputHeld)
                    {
                    if (uz = LeftZone( uzl, uz ))
                        {
                        while (zs = OnlyRightZone( uzl, uz )) uz = zs;
                        }
                    else uz = LastZone( uik );
                    }
                else while (zs = OnlyRightZone( uzl, uz )) uz = zs;
                }
            }
        }
    return( uz );
}

UZone * __asm zo_FindNextZone( register __a0 struct UIKGlobal *uik )
{
  struct UIKZoneList *uzl = uik->ActiveZoneList;
  UZone *uz=0;

    if (uzl)
        {
        if (uz = uzl->ActiveZone) uz = RightZone( uzl, uz, 1 );
        if (! uz) uz = FirstZone( uik );
        }
    return( uz );
}

UZone * __asm zo_FindNextDirZone( register __a0 struct UIKGlobal *uik )
{
  struct UIKZoneList *uzl = uik->ActiveZoneList;
  UZone *start, *zs, *uz=0;

    if (uzl)
        {
        if (start = uz = uzl->ActiveZone)
            {
            uz = OnlyRightZone( uzl, uz );
            if (! uz) // repart d'en haut
                {
                uz = start;

                if (!uik->InputAuto || !uik->InputHeld)
                    {
                    if (uz = RightZone( uzl, uz, 1 ))
                        {
                        while (zs = OnlyLeftZone( uzl, uz )) uz = zs;
                        }
                    else uz = FirstZone( uik );
                    }
                else while (zs = OnlyLeftZone( uzl, uz )) uz = zs;
                }
            }
        }
    return( uz );
}

UZone * __asm zo_FindUpZone( register __a0 struct UIKGlobal *uik )
{
  struct UIKZoneList *uzl = uik->ActiveZoneList;
  UZone *start, *zs, *uz=0;

    if (uzl)
        {
        if (start = uz = uzl->ActiveZone)
            {
            uz = OnlyUpZone( uzl, uz );
            if (! uz) // repart d'en bas
                {
                uz = start;

                if (!uik->InputAuto || !uik->InputHeld)
                    {
                    if (uz = LeftZone( uzl, uz ))
                        {
                        while (zs = OnlyDownZone( uzl, uz )) uz = zs;
                        }
                    else uz = LastZone( uik );
                    }
                else while (zs = OnlyDownZone( uzl, uz )) uz = zs;
                }
            }
        }
    return( uz );
}

UZone * __asm zo_FindDownZone( register __a0 struct UIKGlobal *uik )
{
  struct UIKZoneList *uzl = uik->ActiveZoneList;
  UZone *start, *zs, *uz=0;

    if (uzl)
        {
        if (start = uz = uzl->ActiveZone)
            {
            uz = OnlyDownZone( uzl, uz );
            if (! uz) // repart d'en haut
                {
                uz = start;

                if (!uik->InputAuto || !uik->InputHeld)
                    {
                    if (uz = RightZone( uzl, uz, 1 ))
                        {
                        while (zs = OnlyUpZone( uzl, uz )) uz = zs;
                        }
                    else uz = FirstZone( uik );
                    }
                else while (zs = OnlyUpZone( uzl, uz )) uz = zs;
                }
            }
        }
    return( uz );
}

UZone * __asm zo_FindDirZone( register __a0 struct UIKGlobal *uik, register __d0 UBYTE buttons )
{
  struct UIKZoneList *uzl = uik->ActiveZoneList;
  UZone *uz = 0;

    if (uzl)
        {
        if (uz = uzl->ActiveZone)
            {
            if (buttons & VBJOYF_RIGHT) uz = zo_FindNextDirZone( uik );
            else if (buttons & VBJOYF_LEFT) uz = zo_FindPrevDirZone( uik );
            else if (buttons & VBJOYF_BACK) uz = zo_FindDownZone( uik );
            else if (buttons & VBJOYF_FORW) uz = zo_FindUpZone( uik );
            }
        if (! uz) uz = MidZone( uik );
        }
    return( uz );
}

static void __asm ex_zone_func( register __a0 struct UIKGlobal *uik )
{
  struct UIKZoneList *uzl;
  UZone *az;

    if (uzl = uik->ActiveZoneList)
        {
        if (az = uzl->ActiveZone)
            {
            if (az->ZObj && az->ZFunc) (*az->ZFunc)( az->ZObj );
            }
        }
}

void __asm zo_MarkNextZone( register __a0 struct UIKGlobal *uik )
{
    if (! (uik->InputFlags & INPUTF_JAMED))
        {
        zo_MarkZone( uik, zo_FindNextZone( uik ) );
        ex_zone_func( uik );
        }
}

void __asm zo_MarkFirstZone( register __a0 struct UIKGlobal *uik )
{
  struct UIKPrefs *p = UIKBase->UIKPrefs;

    if (! (uik->InputFlags & INPUTF_JAMED))
        {
        zo_MarkZone( uik, (uik->InputMode == INPUTMODE_5CONTACTS) ? MidZone( uik ) : FirstZone( uik ) );
        UIK_Input_SetTime( uik, p->ZoneSpeed, p->ValidTime, p->WaitTime );
        ex_zone_func( uik );
        }
}

void __asm zo_MarkNextZoneDir( register __a0 struct UIKGlobal *uik, register __d0 UBYTE buttons )
{
    if (! (uik->InputFlags & INPUTF_JAMED))
        {
        zo_MarkZone( uik, zo_FindDirZone( uik, buttons ) );
        ex_zone_func( uik );
        }
}

struct UIKObj * __asm zo_FindActiveObject( register __a0 struct UIKGlobal *uik )
{
  UZone *zo;
  struct UIKObj *obj;

    if (uik->ActiveZoneList && (zo = uik->ActiveZoneList->ActiveZone))
        if (VerifZoneObj( uik, zo ))
            if (obj = zo->ZObj)
                if (obj && obj->Status == UIKS_STARTED)
                    return( obj );
    return(0);
}
