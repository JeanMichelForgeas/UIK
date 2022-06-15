/****************************************************************
 *
 *      Project:   UIK
 *      Function:  gestion des objets
 *
 *      Created:   13/05/90    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikbase.h"
#include "uiki:uikglobal.h"
#include "uiki:uikbox.h"
#include "uiki:uikmacros.h"
#include "uiki:uikcolors.h"
#include "lci:uik_protosint.h"

#include "uiki:objwindow.h"
#include "uiki:objrequest.h"

/* macro offsetof() */
#include <stddef.h>

#include <clib/locale_protos.h>
#include <pragmas/locale_pragmas.h>


/****** Macros **************************************************/


/****** Imported ************************************************/

extern struct TextFont * __asm IntUIK_OpenFontFillTTA( register __a0 UBYTE *name, register __d0 UWORD height, register __a1 struct TTextAttr *tta );
extern void IntUIK_DrawBox();
extern void __asm Auto2InnerBox( register __a0 struct UIKObj *obj );

extern struct IntuitionBase * __far IntuitionBase;
extern struct GfxBase * __far GfxBase;
extern struct UIKBase * __far UIKBase;
extern struct UIKBase * __far DOSBase;
extern ULONG __far LocaleBase;

extern UWORD __far AutoDrawBox_Over[], __far AutoDrawBox_Under[], __far AutoDrawFrame[], __far NullAuto[];
extern UWORD __far DefaultPattern[];


/****** Exported ************************************************/


/****** Statics *************************************************/

extern struct UIKObjGerm __far MemoryGerm;
extern struct UIKObjGerm __far BoxGerm;
extern struct UIKObjGerm __far FrameGerm;
extern struct UIKObjGerm __far ScreenGerm;
extern struct UIKObjGerm __far WindowGerm;
extern struct UIKObjGerm __far RequestGerm;
extern struct UIKObjGerm __far UpGerm;
extern struct UIKObjGerm __far DownGerm;
extern struct UIKObjGerm __far RightGerm;
extern struct UIKObjGerm __far LeftGerm;
extern struct UIKObjGerm __far OKGerm;
extern struct UIKObjGerm __far CancelGerm;
extern struct UIKObjGerm __far BooleanGerm;
extern struct UIKObjGerm __far ToggleGerm;
extern struct UIKObjGerm __far StringGerm;
extern struct UIKObjGerm __far ReqStringGerm;
extern struct UIKObjGerm __far PropLiftGerm;
extern struct UIKObjGerm __far ListGerm;
extern struct UIKObjGerm __far MutualExcludeGerm;
extern struct UIKObjGerm __far WindowDepthGerm;
extern struct UIKObjGerm __far WindowSizeGerm;
extern struct UIKObjGerm __far WindowCloseGerm;
extern struct UIKObjGerm __far WindowZoomGerm;
extern struct UIKObjGerm __far WindowIconifyGerm;
extern struct UIKObjGerm __far WindowDraggGerm;

static struct UIKObjGerm * __far ObjGermTable[] =
    {
    &BooleanGerm,
    &ToggleGerm,
    &MutualExcludeGerm,
    &BoxGerm,
    &FrameGerm,
    &WindowGerm,
    &RequestGerm,
    &UpGerm,
    &DownGerm,
    &RightGerm,
    &LeftGerm,
    &StringGerm,
    &ReqStringGerm,
    &PropLiftGerm,
    &ListGerm,
    &OKGerm,
    &CancelGerm,
    &WindowDepthGerm,
    &WindowSizeGerm,
    &WindowCloseGerm,
    &WindowZoomGerm,
    &WindowIconifyGerm,
    &WindowDraggGerm,
    &MemoryGerm,
    0
    };

struct TagItem __far stdboxflags_boolmap[] =
    {
    { UIKTAG_OBJ_BoxFl_NotDrawn,        BOX_NDRAWN  },
    { UIKTAG_OBJ_BoxFl_Frame,           BOX_FRAME   },
    { UIKTAG_OBJ_BoxFl_NoAct,           BOX_NOACT   },
    { UIKTAG_OBJ_BoxFl_Over,            BOX_OVER    },
    { UIKTAG_OBJ_BoxFl_Filled,          BOX_FILLED  },
    { UIKTAG_OBJ_BoxFl_Clipped,         BOX_CLIPPED },
    { UIKTAG_OBJ_BoxFl_Semaphore,       BOX_SEMAPHORE },
    { UIKTAG_OBJ_BoxFl_NoPush,          BOX_NOPUSH  },
    { UIKTAG_OBJ_BoxFl_TranslateLeftTop,BOX_TRANSLATELEFTTOP },
    { UIKTAG_OBJ_BoxFl_UseBitMap,       BOX_USEBITMAP },
    { UIKTAG_OBJ_BoxFl_Zoneable,        BOX_ZONEABLE },
    { UIKTAG_OBJ_BoxFl_Displayable,     BOX_DISPLAYABLE },
    { UIKTAG_OBJ_BoxFl_StartSpecify,    BOX_STARTSPEC },
    { UIKTAG_OBJ_BoxFl_ExtGadgetUp,     BOX_EXTGADUP },
    { UIKTAG_OBJ_BoxFl_Disabled,        BOX_DISABLED },
    { UIKTAG_OBJ_BoxFl_CannotDisable,   BOX_CANTDISABLE },
    { TAG_DONE }
    };

struct TagItem __far titleflags_boolmap[] =
    {
    { UIKTAG_OBJ_TitleFl_Top,       TITLE_TOP     },
    { UIKTAG_OBJ_TitleFl_Left,      TITLE_LEFT    },
    { UIKTAG_OBJ_TitleFl_Right,     TITLE_RIGHT   },
    { UIKTAG_OBJ_TitleFl_Center,    TITLE_CENTER  },
    { UIKTAG_OBJ_TitleFl_Addr,      TITLE_ADDR    },
    { UIKTAG_OBJ_TitleFl_Clipped,   TITLE_CLIPPED },
    { UIKTAG_OBJ_TitleFl_Image,     TITLE_IMAGE   },
    { UIKTAG_OBJ_TitleFl_Above,     TITLE_ABOVE   },
    { UIKTAG_OBJ_TitleFl_InsRight,  TITLE_INSRIGHT},
    { UIKTAG_OBJ_TitleFl_InsLeft,   TITLE_INSLEFT },
    { TAG_DONE }
    };

static struct TagItem __far resizeflags_boolmap[] =
    {
    { UIKTAG_OBJ_Sz_AttachedFl_Left,  UO_ATTACHED_LEFT  },
    { UIKTAG_OBJ_Sz_AttachedFl_Top,   UO_ATTACHED_TOP   },
    { UIKTAG_OBJ_Sz_AttachedFl_Right, UO_ATTACHED_RIGHT },
    { UIKTAG_OBJ_Sz_AttachedFl_Bot,   UO_ATTACHED_BOT   },
    { TAG_DONE }
    };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/


void __asm IntUIK_ShowRequest( register __a0 UBYTE *fmt, register __d0 ULONG p1, register __d1 ULONG p2, register __a1 ULONG p3 )
{
  UBYTE buf[100];
  struct IntuiText msg = { -1, -1, JAM1, 10, 10, NULL, NULL, NULL },
                   ok  = { -1, -1, JAM1, 6, 3, NULL, "OK", NULL };

    sprintf( buf, fmt, p1, p2, p3 );
    msg.IText = buf;
    AutoRequest( NULL, &msg, 0, &ok, 0, 0, (__builtin_strlen( buf ) + 13) * 8, 50 );
}

static struct UIKObjRegGerm *FindRegGerm( UBYTE *objname, LONG *pind )
{
  register struct UIKList *L;
  register LONG ind = -1;
  register struct UIKObjRegGerm *rg;

    L = &UIKBase->RegisteredObjList;
    objname = UIK_BaseName( objname );
    Forbid();
    while (++ind < L->NodeNum)
        {
        rg = (struct UIKObjRegGerm *) L->UNode[ind].Buf;
        if (! StrNCCmp( objname, &rg->Germ->ObjName[0] ))
            {
            if (pind) *pind = ind;
            Permit();
            return( rg );
            }
        }
    Permit();
    return(0);
}

void IntUIK_UnregisterFreeObjects() // appelée sous Forbid() par LibExpunge()
{
  register struct UIKList *L;
  register LONG ind = -1;
  register struct UIKObjRegGerm *rg;

    L = &UIKBase->RegisteredObjList;
    while (++ind < L->NodeNum)
        {
        rg = (struct UIKObjRegGerm *) L->UNode[ind].Buf;
        if (rg->UsageCount == 0 && rg->Segment) // seulement ceux qui ont été chargés
            {
            if ((rg->Flags & UIKRGF_UNLOAD) && rg->UnloadFunc)
                (*rg->UnloadFunc)( rg );
            UnLoadSeg( rg->Segment );
            rg->Segment = 0;
            UIK_nl_FreeNodes( &UIKBase->RegisteredObjList, ind, ind );
            ind--; // pour pointer sur le noeud suivant au prochain tour
            }
        }
}

static int put_bstr( ULONG badd, UBYTE *buf )
{
  UBYTE *nameptr;
  UBYTE *p;
  UWORD len;
  UWORD i;

    nameptr = (UBYTE*) BADDR(badd);
    len = *nameptr++;
    for (i=0, p=buf; i<len; i++) *p++ = *nameptr++;
    *p++ = ':'; *p = '\0';
    return( len+1 );
}

static int drive_exist( UBYTE *name )
{
  struct DeviceList *devlist, *startlist;
  struct DosInfo *dosinfo;
  LONG len, yuck;
  UBYTE buf[110];

    dosinfo = (struct DosInfo *)(((ULONG)((struct RootNode *)((struct DosLibrary *)DOSBase)->dl_Root)->rn_Info) << 2);
    startlist = devlist = (struct DeviceList *)(((ULONG)dosinfo->di_DevInfo) << 2);
    while (devlist)
        {
        if ((devlist->dl_Type == DLT_DEVICE) && (devlist->dl_Task))
            {
            len = put_bstr( devlist->dl_Name, buf );
            if (! StrNCCmp( name, buf )) return(1);
            }
        yuck = devlist->dl_Next;
        devlist = (struct DeviceList *)(yuck << 2);
        }
    return(0);
}

static ULONG __asm load_disk_object(  /* doit être appelée sous semaphore */
register __a0 UBYTE *name,
register __a1 struct UIKObjRegGerm *rg,
register __d0 ULONG request )
{
  ULONG save, lock=0, ok=0, disabled=1;
  UBYTE filename[100];

    if (! StrNCCmp( name, "UIKObj_ReqString" )) return(0); // empêche le chargement du viel objet

    save = UIK_DisableDOSReq();     //--- le fichier existe?
    /* Uniquement pour distribution avec "Décollages" pour ADFI
    if (! drive_exist( "df1:" ))    // si un 2ème lecteur, on laisse le DisableDOSReq()
        {
        if (! drive_exist( "df2:" ))
            {
            UIK_EnableDOSReq( 0, save );
            disabled = 0;
            }
        }
    */

    if (! (lock = Lock( name, SHARED_LOCK )))
        {
        sprintf( filename, "UIK:Objects/%-.80ls", name );
        if (lock = Lock( filename, SHARED_LOCK )) name = filename;
        else{
            sprintf( filename, "SYS:UIK/Objects/%-.84ls", name );
            if (lock = Lock( filename, SHARED_LOCK )) name = filename;
            else{
                sprintf( filename, "UIK/Objects/%-.80ls", name );
                if (lock = Lock( filename, SHARED_LOCK )) name = filename;
                }
            }
        }
    if (disabled) UIK_EnableDOSReq( 0, save );

    if (!lock)
        {
        if (request) IntUIK_ShowRequest( "WANTED: %ls", (ULONG) name, 0, 0 );
        }
    else{
        UnLock( lock );                 //--- assez de mémoire?

        if (! (rg->Segment = LoadSeg( name )))
            {
            IntUIK_UnregisterFreeObjects();
            rg->Segment = LoadSeg( name );
            }
        if (rg->Segment)
            {
            rg->Germ = *((struct UIKObjGerm **) ((rg->Segment << 2) + 8)); // 4 + moveq #20,d0 + rts
            ok = 1;
            }
        else if (request) IntUIK_ShowRequest( "NO MEMORY: %ls", (ULONG) name, 0, 0 );
        }
    return( ok );
}

struct UIKObjRegGerm * __asm UIK_RegisterObject( register __a0 struct UIKObjGerm *g, register __d0 int replace )
{
  struct UIKObjRegGerm *rg;
  LONG ind;

    ObtainSemaphore( &UIKBase->RegObjSemaphore );
    rg = FindRegGerm( &g->ObjName[0], &ind );
    if (rg && replace)
        {
        if (rg->Segment)
            {
            if ((rg->Flags & UIKRGF_UNLOAD) && rg->UnloadFunc)
                (*rg->UnloadFunc)( rg );
            UnLoadSeg( rg->Segment );
            }
        rg->Segment = 0;
        rg->Germ = g;
        }
    else if (!rg)
        {
        Forbid();
        if (rg = (struct UIKObjRegGerm *) UIK_nl_AllocNode( &UIKBase->RegisteredObjList, -1, sizeof(struct UIKObjRegGerm), 0 ))
            {
            rg->Germ = g;
            }
        Permit();
        }
    ReleaseSemaphore( &UIKBase->RegObjSemaphore );
    return( rg );
}

int __asm UIK_UnregisterObject( register __a0 UBYTE *objname, register __d0 int force )
{
  struct UIKObjRegGerm *rg;
  LONG ind;
  int rc;

    ObtainSemaphore( &UIKBase->RegObjSemaphore );
    rc = 1;
    if (rg = FindRegGerm( objname, &ind ))
        {
        rc = 0;
        if (force || rg->UsageCount == 0)
            {
            if (rg->Segment)
                {
                if ((rg->Flags & UIKRGF_UNLOAD) && rg->UnloadFunc)
                    (*rg->UnloadFunc)( rg );
                UnLoadSeg( rg->Segment );
                }
            Forbid();
            UIK_nl_FreeNodes( &UIKBase->RegisteredObjList, ind, ind );
            Permit();
            rc = 1;
            }
        }
    ReleaseSemaphore( &UIKBase->RegObjSemaphore );
    return( rc );
}

void IntUIK_DecRegGermCounter( struct UIKObjRegGerm *rg )
{
    ObtainSemaphore( &UIKBase->RegObjSemaphore );
    //if (UIK_nl_FindNode( &UIKBase->RegisteredObjList, (UBYTE *) rg ))
        if (rg->UsageCount != 0)
            --rg->UsageCount;
            //if (--rg->UsageCount == 0)
                ; //UIK_UnregisterObject( &rg->Germ->ObjName[0], 0 );
    ReleaseSemaphore( &UIKBase->RegObjSemaphore );
}

int IntUIK_RegisterInternalObjects()
{
  struct UIKObjGerm *g, **p = ObjGermTable;
  struct UIKObjRegGerm *rg;

    while (g = *p++)
        {
        ObtainSemaphore( &UIKBase->RegObjSemaphore );
        Forbid();
        if (rg = (struct UIKObjRegGerm *) UIK_nl_AllocNode( &UIKBase->RegisteredObjList, -1, sizeof(struct UIKObjRegGerm), 0 ))
            {
            Permit();
            if (! load_disk_object( &g->ObjName[0], rg, 0 ))
                rg->Germ = g; // si pas sur disk, on prend le germe g de la librairie.
            }
        else Permit();
        ReleaseSemaphore( &UIKBase->RegObjSemaphore );
        if (!rg) return(0);
        }
    return(1);
}

void IntUIK_UnregisterAllObjects()
{
  register struct UIKList *L;

    ObtainSemaphore( &UIKBase->RegObjSemaphore );
    L = &UIKBase->RegisteredObjList;
    while (L->NodeNum)
        {
        struct UIKObjRegGerm *rg = (struct UIKObjRegGerm *) L->UNode[0].Buf;
        UIK_UnregisterObject( rg->Germ->ObjName, 1 ); // force
        }
    ReleaseSemaphore( &UIKBase->RegObjSemaphore );
}

struct UIKObj * __asm UIK_AddPrivateObject(
register __a0 struct UIKObjRegGerm *rg,
register __d0 struct UIKObj *parent,
register __a1 struct TagItem *taglist )
{
  register struct UIKObj *obj=0;

    if (parent == 0) return(0);

    if (rg)
        {
        ULONG flags, titleflags, resizeflags, len, pens, norauto, altauto, key;
        struct UIKObjGerm *g;
        UBYTE *mem, *title, *alttitle;
        UBYTE relative;
        struct TagItem *ti;
        struct UIKObjExtension *ext;
        struct UIKLang *lang;
        UBYTE ok=1, *fontname;
        struct TagItem *GenTagList;
        ULONG VerRev;

        g = rg->Germ;
        if (GenTagList = g->GermTagList)
            if (UIKBase->UIKPrefs->PrefsFlags & UIKF_SYSLOOK)
                GenTagList = (struct TagItem *) UIK_GetTagData( UIKTAG_GEN_SysLookGenTagList, (ULONG)GenTagList, GenTagList );

        if (VerRev = UIK_GetTagData( UIKTAG_OBJ_MinVerRev, 0, taglist ))
            if (VerRev > UIK_GetTagData( UIKTAG_GEN_VersionRevision, 0, GenTagList ))
                {
                IntUIK_ShowRequest( "WANTED: %ls %ld.%ld minimum", (ULONG)g->ObjName, VerRev>>16, VerRev&0xffff );
                goto END_ERROR;
                }

        flags = UIK_GetTagData( UIKTAG_OBJ_Box_Flags, g->DefaultFlags, taglist );
        titleflags = UIK_GetTagData( UIKTAG_OBJ_Title_Flags, 0, taglist );
        resizeflags = UIK_GetTagData( UIKTAG_OBJ_Sz_Attached_Flags, 0, taglist );

        if (parent->Flags & BOX_DISABLED) flags |= BOX_DISABLED;

        flags = UIK_PackBoolTags( flags, taglist, stdboxflags_boolmap );
        titleflags = UIK_PackBoolTags( titleflags, taglist, titleflags_boolmap );
        resizeflags = UIK_PackBoolTags( resizeflags, taglist, resizeflags_boolmap );

        title = (UBYTE *) UIK_GetTagData( UIKTAG_OBJ_Title, 0, taglist );
        alttitle = (UBYTE *) UIK_GetTagData( UIKTAG_OBJ_AltTitle, 0, taglist );
        if (title || alttitle)
            {
            if (! (titleflags & (TITLE_TOP|TITLE_LEFT|TITLE_RIGHT|TITLE_ABOVE)))
                titleflags |= TITLE_CENTER | TITLE_CLIPPED;
            titleflags = UIK_PackBoolTags( titleflags, taglist, titleflags_boolmap );
            if (titleflags & (TITLE_TOP|TITLE_LEFT|TITLE_RIGHT|TITLE_ABOVE))
                titleflags &= ~(TITLE_CLIPPED);
            }

        len = g->ObjSize + sizeof(struct UIKObjExtension) + sizeof(struct UIKLang);
        if (flags & BOX_SEMAPHORE) len += sizeof(struct SignalSemaphore);
        if (mem = UIK_nl_AllocNode( parent, -1, len, 0 ))
            {
            struct UIKObjWindow *wo;
            struct UIKObj *rpobj, *parentbox;
            UBYTE ch;

            obj = (struct UIKObj *) mem; mem += g->ObjSize;
            obj->Status = UIKS_STOPPED;
            obj->ObjExt = ext = (struct UIKObjExtension *) mem; mem += sizeof(struct UIKObjExtension);
            ext->LastFunc = -1;
            obj->ObjExt->Lang = lang = (struct UIKLang *) mem; mem += sizeof(struct UIKLang);
            lang->Master = parent->UIK;

            if (flags & BOX_SEMAPHORE)
                {
                obj->SigSemaphore = (struct SignalSemaphore *) mem;
                InitSemaphore( obj->SigSemaphore );
                }

            obj->List.BlockSize = 5;
            obj->Hook.BlockSize = 1;
            obj->Link.BlockSize = 1;
            obj->UIK            = parent->UIK;
            obj->Parent         = parent;
            obj->ClipCount      = -1;
            obj->Flags          = flags;
            obj->TitleFlags     = titleflags;
            obj->ResizeFlags    = resizeflags;
            obj->Id             = g->ObjId;

            obj->AutoRatio      = parent->AutoRatio;
            obj->XAspect        = parent->XAspect;
            obj->YAspect        = parent->YAspect;

            if (obj->Pattern = UIK_GetTagData( UIKTAG_OBJ_Pattern, 0, taglist ))
                obj->Flags |= BOX_FILLED; // important pour pens
            obj->PatternPower = UIK_GetTagData( UIKTAG_OBJ_PatternPower, 0, taglist );

            pens = (obj->Flags & BOX_FILLED) ? UIKCOLS(UIKCOL_LIGHT,UIKCOL_GREY,UIKCOL_BLACK,UIKCOL_WHITE) : UIKCOLS(UIKCOL_GREY,UIKCOL_GREY,UIKCOL_WHITE,UIKCOL_WHITE);
            if (obj->Flags & (BOX_USEBITMAP | BOX_CUSTOMDRAW)) norauto = altauto = (ULONG) NullAuto;
            else{
                norauto = (ULONG) AutoDrawBox_Over;
                altauto = (ULONG) AutoDrawBox_Under;
                }
            key = 0;
            if (GenTagList)
                {
                if (! AllocLangage( obj, GenTagList )) goto END_ERROR;
                pens = UIK_GetTagData( UIKTAG_GEN_ActInactPens, pens, GenTagList );
                norauto = UIK_GetTagData( UIKTAG_GEN_BoxAutomate, norauto, GenTagList );
                altauto = UIK_GetTagData( UIKTAG_GEN_BoxAltAutomate, altauto, GenTagList );
                key = UIK_GetTagData( UIKTAG_GEN_ShortCutKey, key, GenTagList );
                ext->ActKeyTank = (APTR) UIK_GetTagData( UIKTAG_GEN_ActKeyTank, 0, GenTagList );
                ext->InactKeyTank = (APTR) UIK_GetTagData( UIKTAG_GEN_InactKeyTank, 0, GenTagList );
                }

            ext->IPRemoveFunc = (APTR) UIK_GetTagData( UIKTAG_OBJ_IPRemoveFunc, 0, taglist );

            *((ULONG*)&obj->ActBoxPen) = UIK_GetTagData( UIKTAG_OBJ_ActInactPens, pens, taglist ); // tous les Pens en même temps

            obj->Automate       = (UWORD*) UIK_GetTagData( UIKTAG_OBJ_BoxAutomate, norauto, taglist );
            obj->AltAutomate    = (UWORD*) UIK_GetTagData( UIKTAG_OBJ_BoxAltAutomate, altauto, taglist );
            if (obj->Flags & BOX_FRAME) obj->Automate = obj->AltAutomate = AutoDrawFrame;
            else if (obj->Flags & BOX_NOPUSH) obj->AltAutomate = obj->Automate;
            if (! obj->Automate) obj->Automate = (UWORD*) NullAuto;
            if (! obj->AltAutomate) obj->AltAutomate = (UWORD*) NullAuto;

            obj->Title    = (ULONG) title;
            obj->AltTitle = (ULONG) alttitle;
            if (ext->MultiNum = (UWORD) UIK_GetTagData( UIKTAG_OBJ_MultiTitleNum, 0, taglist ))
                {
                ext->MultiDelay = UIK_GetTagData( UIKTAG_OBJ_MultiTitleDelay, ((struct ExecBase *)*((ULONG*)4))->VBlankFrequency/10, taglist );
                }

            if (ti = UIK_FindTagItem( UIKTAG_OBJ_VisualEraseColor, taglist ))
                {
                obj->Flags |= BOX_VISUALERASE;
                obj->EraseColor = (UBYTE) ti->ti_Data;
                }
            ext->UserStopFunc = (void*) UIK_GetTagData( UIKTAG_OBJ_UserStopFunc, 0, taglist );
            ext->UserRemoveFunc = (void*) UIK_GetTagData( UIKTAG_OBJ_UserRemoveFunc, 0, taglist );

            lang->Master = (void*) UIK_GetTagData( UIKTAG_OBJ_LangMaster, (ULONG) lang->Master, taglist );

            { struct TextFont *font = 0;
            if (fontname = (UBYTE *) UIK_GetTagData( UIKTAG_OBJ_FontName, NULL, taglist ))
                {
                ULONG fontheight = UIK_GetTagData( UIKTAG_OBJ_FontHeight, 8, taglist );
                font = IntUIK_OpenFontFillTTA( fontname, fontheight, &ext->TTA );
                }
            else{
                struct TTextAttr *tta;
                if (parent->ObjExt->Font)
                    {
                    //UIK_Font2TTA( parent->ObjExt->Font, &ext->TTA );
                    ext->TTA = parent->ObjExt->TTA;
                    }
                if (tta = (struct TTextAttr *) UIK_GetTagData( UIKTAG_OBJ_TTextAttr, NULL, taglist ))
                    {
                    ext->TTA.tta_Style = tta->tta_Style;
                    if (ext->TTA.tta_Tags = tta->tta_Tags) ext->TTA.tta_Style |= FSF_TAGGED;
                    if (tta->tta_Name)
                        {
                        ext->TTA = *tta;
                        font = UIK_OpenFontTTA( &ext->TTA );
                        }
                    }
                }
            if (font) obj->Flags |= BOX_OPENEDFONT;
            ext->Font = font ? font : parent->ObjExt->Font;
            if (! ext->Font)
                {
                ext->Font = UIKBase->DefaultFont;
                ext->TTA = UIKBase->DefaultFontAttr;
                }
            }

            if (ext->CustomDraw = (void *) UIK_GetTagData( UIKTAG_OBJ_CustomDraw, NULL, taglist ))
                {
                ext->CustomDrawData = UIK_GetTagData( UIKTAG_OBJ_CustomDrawData, NULL, taglist );
                obj->Flags |= BOX_CUSTOMDRAW;
                }

            wo = (struct UIKObjWindow *) parent;
            rpobj = 0;
            parentbox = 0;
            while (wo && wo->Obj.Id != UIKID_WINDOW)
                {
                if (wo->Obj.Id == UIKID_REQUEST && !rpobj)
                    { rpobj = wo; break; }
                if (wo->Obj.Id == UIKID_BOX && !parentbox && (wo->Obj.Flags & BOX_ZONEABLE))
                    { parentbox = wo; }
                wo = (struct UIKObjWindow *) wo->Obj.Parent;
                }
            if (wo && !rpobj) rpobj = wo;
            if (! parentbox) parentbox = rpobj;

            relative = UIK_GetTagData( UIKTAG_OBJ_ParentRelative, FALSE, taglist );
            *((ULONG*)&obj->Box.Left) = UIK_GetTagData( UIKTAG_OBJ_LeftTop, 0, taglist ); // + Box.Top en même temps !
            if (obj->Flags & BOX_TRANSLATELEFTTOP && !relative)
                {
                if (wo && wo->Obj.Id != UIKID_REQUEST)
                    {
                    obj->Box.Left += wo->LeftBorderWidth;
                    obj->Box.Top += wo->TopBorderHeight;
                    obj->Flags |= BOX_TRANSLATED;
                    }
                }
            if (relative == TRUE)
                { obj->Box.Left += parent->Box.Left; obj->Box.Top += parent->Box.Top; }

            *((ULONG*)&obj->Box.Width) = UIK_GetTagData( UIKTAG_OBJ_WidthHeight, g->DefaultWidthHeight, taglist ); // + Box.Height en même temps !
            if (obj->Box.Width < g->MinWidth) obj->Box.Width = g->MinWidth;
            if (obj->Box.Height < g->MinHeight) obj->Box.Height = g->MinHeight;
            obj->UserULong1 = UIK_GetTagData( UIKTAG_OBJ_UserValue1, 0, taglist );
            obj->UserULong2 = UIK_GetTagData( UIKTAG_OBJ_UserValue2, 0, taglist );

            key = UIK_GetTagData( UIKTAG_OBJ_ShortCutKey, key, taglist );
            if (ch = UIK_GetTagData( UIKTAG_OBJ_TitleUnderscore, 0, taglist ))
                {
                UBYTE *ptr, pos;
                if (ptr = UIK_ObjTitle( obj )) // titre au repos
                    {
                    for (pos=0; *ptr && *ptr != ch; pos++, ptr++);
                    if (*ptr++)   // skip underscore
                        if (*ptr) // pointe shortcut
                            {
                            key = W2L(B2W(*ptr,0),0);
                            ext->SCWidth = UIK_CharWidth( ext->Font, ch );
                            ext->SCPos = pos;
                            }
                    }
                }

            *((ULONG*)&ext->SCKey) = key;
            if ((ext->SCKey || ext->SCCode) && rpobj && obj->Id != UIKID_WINDOW && obj->Id != UIKID_REQUEST)
                {
                struct UIKHook *hook;
                struct UIKList *list=0;

                list = (rpobj->Id == UIKID_REQUEST) ? &((struct UIKObjReq *)rpobj)->ShortCutList : &((struct UIKObjWindow *)rpobj)->ShortCutList;
                ext->SCKey = UIK_ToUpper( ext->SCKey );
                //if (ext->SCQual & IEQUALIFIER_LSHIFT || ext->SCQual & IEQUALIFIER_RSHIFT) ext->SCQual |= IEQUALIFIER_LSHIFT|IEQUALIFIER_RSHIFT;
                if (ext->SCQual & IEQUALIFIER_LALT || ext->SCQual & IEQUALIFIER_RALT) ext->SCQual |= IEQUALIFIER_LALT|IEQUALIFIER_RALT;
                if (hook = (struct UIKHook *)UIK_nl_AllocNode( list, -1, sizeof(struct UIKHook), 0 ))
                    {
                    ext->SCList = list;
                    ext->SCHook = hook;
                    hook->Obj = obj;
                    }
                }

            CopyMem( (UBYTE*) (((ULONG)g) + offsetof(struct UIKObjGerm, StartVector)),
                     (UBYTE*) (((ULONG)obj) + offsetof(struct UIKObj, StartVector)),
                     offsetof(struct UIKObjGerm, DefaultWidthHeight) - offsetof(struct UIKObjGerm, StartVector)
                     );

            ext->LastFunc = UIK_GetTagData( UIKTAG_GEN_LastFuncId, -1, GenTagList );

            IntUIK_SetActiveColors( obj );
            Auto2InnerBox( obj );

            obj->RegGerm = rg;
            rg->UsageCount++;

            if ((titleflags & (TITLE_ADDR | TITLE_IMAGE)) == TITLE_ADDR)
                {
                if (title)
                    {
                    if (obj->Title = (ULONG) BufAlloc( 4, StrLen( title ), MEMF_ANY | MEMF_CLEAR ))
                        StrCpy( (UBYTE*)obj->Title, title );
                    else ok = 0;
                    }
                if (ok && alttitle)
                    {
                    if (obj->AltTitle = (ULONG) BufAlloc( 4, StrLen( alttitle ), MEMF_ANY | MEMF_CLEAR ))
                        StrCpy( (UBYTE*)obj->AltTitle, alttitle );
                    else ok = 0;
                    }
                }

            if (ok && obj->Flags & BOX_ZONEABLE)
                {
                if (! UIK_AddZone( parentbox, obj,
                    0 /*UIK_GetTagData( UIKTAG_OBJ_ZoneClass, GADGETDOWN | MOUSEMOVE | GADGETUP, taglist )*/,
                    (void *) UIK_GetTagData( UIKTAG_OBJ_ZoneFunc, 0, taglist )
                    )) ok = 0;
                }
            if (ok && g->AddVector)
                {
                if (! g->AddVector( obj, taglist )) ok = 0;
                }

            if (ok) return( obj );
            }
        }

  END_ERROR:
    if (obj) TRUE_Remove( obj );
    return(0);
}

struct UIKObj * __asm UIK_AddObject(
register __a0 UBYTE *name,
register __d0 struct UIKObj *parent,
register __a1 struct TagItem *taglist )
{
  struct UIKObjRegGerm *rg;
  void (*func)();
  ULONG ok=0;

    if (name[0] == 'U' && name[1] == 'I' && name[2] == 'K' && name[3] == 0)
        return( (struct UIKObj *) Install2( taglist ) );

    if (parent == 0) return(0);

    ObtainSemaphore( &UIKBase->RegObjSemaphore );
    if (! (rg = FindRegGerm( name, 0 )))
        {
        Forbid();
        if (rg = (struct UIKObjRegGerm *) UIK_nl_AllocNode( &UIKBase->RegisteredObjList, -1, sizeof(struct UIKObjRegGerm), 0 ))
            {
            Permit();

            if (! (ok = load_disk_object( name, rg, 1 )))
                {
                UIK_nl_FreeNodes( &UIKBase->RegisteredObjList, -1, -1 );
                rg = 0;
                }
            }
        else Permit();

        if (ok && rg->Germ->GermTagList)
            {
            if (func = (APTR) GetTagData( UIKTAG_GEN_LoadFunc, 0, rg->Germ->GermTagList ))
                (*func)( rg, taglist );
            if (rg->UnloadFunc = (APTR) GetTagData( UIKTAG_GEN_UnloadFunc, 0, rg->Germ->GermTagList ))
                rg->Flags |= UIKRGF_UNLOAD;
            }
        }
    ReleaseSemaphore( &UIKBase->RegObjSemaphore );

    return( UIK_AddPrivateObject( rg, parent, taglist ) );
}
