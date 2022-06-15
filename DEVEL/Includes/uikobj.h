#ifndef UIKDEFS_OBJ

#define UIKDEFS_OBJ
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef INTUITION_INTUITION_H
#include <intuition/intuition.h>
#endif

#ifndef GRAPHICS_REGIONS_H
#include <graphics/regions.h>
#endif

#ifndef DEVICES_TIMER_H
#include devices/timer.h
#endif

#ifndef UIKDEFS_LIST
#include "uiki:uiklist.h"
#endif

#ifndef UIK_ZONE_H
#include "uiki:uikzone.h"
#endif

#ifndef UIKDEFS_BOX
#include "uiki:uikbox.h"
#endif

#ifndef UIKDEFS_TAGITEM
#include "uiki:uiktag.h"
#endif

#ifndef UIKDEFS_BUFFER
#include "uiki:uikbuf.h"
#endif

#ifndef UIKDEFS_LANGAGE
#include "uiki:uiklang.h"
#endif

#ifndef UIKDEFS_MACRO
#include "uiki:uikmacros.h"
#endif


/* long names shortcuts */
#define UIK_Call            UIK_CallObjectFunc
#define UIK_CallTags        UIK_CallObjectFuncTags

/* obsolete
fonctions étendues
#define UIK_OSt_Activate    UIK_ActivateKeyTank
#define UIK_OSt_Inactivate  UIK_InactivateKeyTank
#define UIK_IsActiveString  UIK_IsActiveKeyTank
*/

struct UIKObjGerm
{
    UBYTE           ObjName[32];
    int             (*AddVector)();
    int             (*StartVector)();
    void            (*StopVector)();
    void            (*RemoveVector)();
    void            (*RefreshVector)();
    void            (*ResizeVector)();
    void            (*ExceptActVector)();
    void            (*ExceptInaVector)();
    void            (*ExceptMovVector)();
    void            (*ExceptKeyVector)();
    int             (*StartAfterVector)();
    int             (*SelectVector)();
    ULONG           *ExtentedVectors;
    struct TagItem  *GermTagList;
    UWORD           MinWidth;
    UWORD           MinHeight;
    ULONG           DefaultWidthHeight;
    UWORD           ObjId;
    UWORD           ObjSize;
    ULONG           DefaultFlags;
};

struct UIKObjRegGerm
{
    struct UIKObjGerm   *Germ;
    ULONG               UsageCount;
    ULONG               Segment;
    ULONG               Flags;
    void                (*UnloadFunc)(); // if Flags & UIKRGF_UNLOAD
};

#define UIKRGF_UNLOAD 0x0001

struct UIKObjExtension
    {
    void                (*UserStopFunc)();
    void                (*UserRemoveFunc)();
    struct TextFont     *Font;
    APTR                UFrame;
    void                (*CustomDraw)();
    ULONG               CustomDrawData;
    struct UIKZoneList  *ZoneList;
    struct TTextAttr    TTA;
    struct UIKLang      *Lang;
    ULONG               LastFunc;
    UBYTE               SCKey;
    UBYTE               SCCode;
    UWORD               SCQual;
    struct Rectangle    InnerRect;
    UWORD               MultiDelay;
    UWORD               MultiNum;
    UWORD               MultiCount;
    UBYTE               SCWidth;
    UBYTE               SCPos;
    struct UIKList      *SCList;
    struct UIKHook      *SCHook;
    void                (*ActKeyTank)();
    void                (*InactKeyTank)();
    struct Task         *IPMother;
    void                (*IPRemoveFunc)();
    };


struct UIKObj
    {
    struct UIKList          List;
    struct UIKList          Link;
    struct UIKList          Hook;
    struct UIKGlobal        *UIK;
    struct Screen           *sc;
    struct Window           *w;
    struct Requester        *rq;
    struct RastPort         *rp;
    struct UIKBox           Box;
    ULONG                   UserULong1;
    ULONG                   UserULong2;
    UBYTE                   Status;
    UBYTE                   MouseIn;
    WORD                    MouseX;
    WORD                    MouseY;
    WORD                    RegisterId;
    struct UIKObj           *Icon;
    struct UIKObj           *Parent;
    ULONG                   Flags;
    ULONG                   TitleFlags;
    ULONG                   ResizeFlags;

    UBYTE                   ActBoxPen;
    UBYTE                   InactBoxPen;
    UBYTE                   ActTitlePen;
    UBYTE                   InactTitlePen;
    UBYTE                   BoxPen;
    UBYTE                   TitlePen;
    UWORD                   Id;

    ULONG                   Title;
    ULONG                   AltTitle;
    UWORD                   *Automate;
    UWORD                   *AltAutomate;

    int                     (*StartVector)();
    void                    (*StopVector)();
    void                    (*RemoveVector)();
    void                    (*RefreshVector)();
    void                    (*ResizeVector)();
    void                    (*ExceptActVector)();
    void                    (*ExceptInaVector)();
    void                    (*ExceptMovVector)();
    void                    (*ExceptKeyVector)();
    int                     (*StartAfterVector)();
    void                    (*SelectVector)();
    ULONG                   *ExtentedVectors;

    struct TagItem          *GermTagList;
    UWORD                   MinWidth;
    UWORD                   MinHeight;

    struct SignalSemaphore  *SigSemaphore;
    struct RastPort         *irp;
    struct Region           *Region;
    struct Region           *SpareRegion;
    UBYTE                   ClipCount;
    UBYTE                   PatternPower;
    UBYTE                   EraseColor;
    UBYTE                   AutoRatio;
    struct UIKObjRegGerm    *RegGerm;
    ULONG                   Pattern;
    UWORD                   XAspect;
    UWORD                   YAspect;
    struct UIKObjExtension  *ObjExt;
    };

#define UIKObjBox UIKObj

struct UIKObjGad
    {
    struct UIKObj           Obj;
    struct Gadget           Gad;
    };

/*----UIKObj Declencheurs---- */
struct UIKHook
    {
    int                     (*Vect)();
    struct UIKObj           *Obj;
    ULONG                   Mask;
    ULONG                   Parm;
    };

struct UIKLink
    {
    int                     (*Vect)();
    struct UIKObj           *Obj;
    struct UIKObj           *Dest;
    ULONG                   Mask;
    ULONG                   Parm;
    };

/*----- object status ------*/

/* flag */
#define UIKSF_OUT_OF_THE_LAW  0x80

#define UIKS_STARTED        0  /* l'utilisateur peut utiliser cet objet */
#define UIKS_STOPPING       1  /* l'objet est en train d'être arrêté */
#define UIKS_REMOVING       2  /* l'objet est en train d'être supprimé */
#define UIKS_STOPPED        3  /* l'objet est arrêté */
#define UIKS_ICONIFIED      4  /* l'objet est iconifié */
#define UIKS_OLDICONIFIED   5  /* l'objet iconifié est en train d'être stoppé */
#define UIKS_REMOVED        6  /* l'objet va être supprimé effectivement */

/*------ objects ids -------*/

#define OBJECTID_USER       0x8000

#define UIKID_NULL          0
#define UIKID_BOX           1
#define UIKID_SCREEN        2
#define UIKID_WINDOW        3
#define UIKID_REQUEST       4
#define UIKID_UPGAD         5
#define UIKID_DOWNGAD       6

#define UIKID_SYSGAD        7
#define UIKID_DEPTGAD       7
#define UIKID_SIZEGAD       8
#define UIKID_CLOSGAD       9
#define UIKID_ZOOMGAD       10
#define UIKID_ICONGAD       11
#define UIKID_DRAGGAD       12

#define UIKID_BOOLGAD       13
#define UIKID_TOGLGAD       14
#define UIKID_STRGAD        15
#define UIKID_PROPGAD       16
#define UIKID_LIST          17
#define UIKID_MEXGAD        18
#define UIKID_MEM           19
#define UIKID_RIGHTGAD      20
#define UIKID_LEFTGAD       21

#define UIKID_FILESELECT    22

#define UIKID_OKGAD         23
#define UIKID_CANCELGAD     24

#define UIKID_HELP          25
#define UIKID_WINHELP       26
#define UIKID_REQSTRING     27
#define UIKID_DIAL          28
#define UIKID_AREXXSIM      29

#define UIKID_PROGRESS      30
#define UIKID_MENU          31
#define UIKID_JOYSTICK      32
#define UIKID_INPUTICK      33
#define UIKID_PALETTE       34
#define UIKID_FONTSELECT    35

#define UIKID_DISPLAYMODE   36
#define UIKID_DISPATCH      37
#define UIKID_TEXTVIEW      38
#define UIKID_CXBROKER      39
#define UIKID_APPWBENCH     40

#define UIKID_WINFILESEL    41
#define UIKID_WFILEBUTTON   42
#define UIKID_WINFONTSEL    43
#define UIKID_WFONTBUTTON   44
#define UIKID_CHECKTOGGLE   45
#define UIKID_RADIOEXCLUDE  46
#define UIKID_IMAGEVIEW     47
#define UIKID_TIMER         48
#define UIKID_CYCLE         49
#define UIKID_IPOL          50
#define UIKID_NOTIFY        51
#define UIKID_SPROPLIFT     52
#define UIKID_COLORSLIDE    53

#define UIKID_SERIAL        54
#define UIKID_BOXNBOX       55
#define UIKID_OBJNULL       56
#define UIKID_PROCESS       57
#define UIKID_ASYNCFILE     58
#define UIKID_NARRATOR      59
#define UIKID_JOYMOUSE      60

#define UIKID_FBHELP        61
#define UIKID_FBWINHELP     62
#define UIKID_FBSCROLLER    63
#define UIKID_FB2C          64
#define UIKID_FB3C          65
#define UIKID_FBFILEPP      66
#define UIKID_FBWINTV       67
#define UIKID_FBMED         68
#define UIKID_FBPAUSE       69
#define UIKID_FBPLAY        70
#define UIKID_FBSTOP        71
#define UIKID_FBMEDPLAYER   72
#define UIKID_FBVOLUME      73
#define UIKID_FBCHOICE      74
#define UIKID_FBLISTER      75
#define UIKID_TEXTEDITOR    76
#define UIKID_CALENDAR      77
#define UIKID_FBBUTTONS     78
#define UIKID_FBBUTTONLIST  79

#define UIKID_DCLOCK        80
#define UIKID_MDRAW         81

/*----- objects flags -----*/
#define BOX_NDRAWN              0x0001  /* 1 = la boite n'est pas dessinée */
#define BOX_INNERZONE           0x0002  /* rectangle de sélection autour de l'intérieur */
#define BOX_FRAME               0x0004  /* pour un cadre gravé */
#define BOX_NOACT               0x0008  /* 1 = objet dessiné mais pas d'activation par le handler */
#define BOX_OVER                0x0010  /* 1 = boite convexe, sinon concave */
#define BOX_FILLED              0x0020  /* rempli avec la couleur de fond */
#define BOX_reserved            0x0040
#define BOX_CLIPPED             0x0080  /* l'objet est clippé quand il est actif */
#define BOX_SEMAPHORE           0x0100  /* pour 'UIK_LockVars()' et éviter des conflits */
#define BOX_NOPUSH              0x0200  /* provoque AltAutomate = Automate */
#define BOX_TRANSLATELEFTTOP    0x0400  /* origines transférées dans l'intérieur de la fenêtre */
#define BOX_VISUALERASE         0x0800  /* couleur d'effacement */
#define BOX_OPENEDFONT          0x1000  /* le font de cet objet a été ouvert par lui */
#define BOX_CUSTOMDRAW          0x2000  /* l'objet se redessine lui-même */
#define BOX_USEBITMAP           0x4000  /* UIK utilise les CustomBitmaps de la window parente */
#define BOX_TRANSLATED          0x8000  /* l'objet a été translaté (voir BOX_TRANSLATELEFTTOP) */
#define BOX_ZONEABLE        0x00010000  /* l'objet peut être entouré du rectangle de sélection */
#define BOX_DISPLAYABLE     0x00020000  /* l'objet a une apparence visuelle */
#define BOX_STARTSPEC       0x00040000  /* l'objet est démarré si spécifié dans UIK_Start() */
#define BOX_EXTGADUP        0x00080000  /* GADGETUP en dehors du gadget pris quand même */
#define BOX_DISABLED        0x00100000  /* objet indisponible : inactivable */
#define BOX_CANTDISABLE     0x00200000  /* ne peut être désactivé */
#define BOX_GROUPABLE       0x00400000  /* peut contenir d'autres objets */
#define BOX_VGROUP          0x00800000  /* est un groupe vertical */
#define BOX_HGROUP          0x01000000  /* est un groupe horizontal */

/*------ Title flags ------*/
#define TITLE_TOP               0x0001  /* à cheval sur le bord du haut */
#define TITLE_LEFT              0x0002  /* à gauche du bord gauche */
#define TITLE_RIGHT             0x0004  /* à droite du bord droit */
#define TITLE_CENTER            0x0008  /* centré dans l'objet */
#define TITLE_reserved          0x0010
#define TITLE_ADDR              0x0020  /* si 1, les titres donnés à l'objet doivent être des pointeurs sur du texte */
#define TITLE_CLIPPED           0x0040  /* si 1, les titres ne dépassent pas de l'objet */
#define TITLE_IMAGE             0x0080  /* si 1, les titres sont des UIKBitMap */
#define TITLE_ABOVE             0x0100  /* au-dessus du bord du haut */
#define TITLE_INSRIGHT          0x0200  /* à l'intérieur à droite */
#define TITLE_INSLEFT           0x0400  /* à l'intérieur à gauche */

/*------ Sp Flags ----------*/
#define UO_ATTACHED_LEFT        0x01    /* solidaire du bord gauche */
#define UO_ATTACHED_TOP         0x02    /* solidaire du bord haut */
#define UO_ATTACHED_RIGHT       0x04    /* solidaire du bord droit */
#define UO_ATTACHED_BOT         0x08    /* solidaire du bord bas */


/*--------------------------------------------------------- */
/*---------------------------- T A G S -------------------- */
/*--------------------------------------------------------- */

/*---------------------------------------------- GENERIQUES */
#define UIKTAG_GEN_LangEnglish          (TAG_USER|0x0001)
#define UIKTAG_GEN_LangOther            (TAG_USER|0x0002)
#define UIKTAG_GEN_LastFuncId           (TAG_USER|0x0003)
#define UIKTAG_GEN_Catalog              (TAG_USER|0x0004)
#define UIKTAG_GEN_ActInactPens         (TAG_USER|0x0005)
#define UIKTAG_GEN_BoxAutomate          (TAG_USER|0x0006)
#define UIKTAG_GEN_BoxAltAutomate       (TAG_USER|0x0007)
#define UIKTAG_GEN_ShortCutKey          (TAG_USER|0x0008)
#define UIKTAG_GEN_ActKeyTank           (TAG_USER|0x0009)
#define UIKTAG_GEN_InactKeyTank         (TAG_USER|0x000a)
#define UIKTAG_GEN_VersionRevision      (TAG_USER|0x000b)
#define UIKTAG_GEN_LangDuplicate        (TAG_USER|0x000c)
#define UIKTAG_GEN_LoadFunc             (TAG_USER|0x000d)
#define UIKTAG_GEN_UnloadFunc           (TAG_USER|0x000e)
#define UIKTAG_GEN_FilterIEventsFunc    (TAG_USER|0x000f)
#define UIKTAG_GEN_SysLookGenTagList    (TAG_USER|0x0010)

/*---------------------------------------------------UIKObj */
/*----UIKObj values Tags---- */
#define UIKTAG_OBJ_ParentRelative       (TAG_USER|0x0000)
#define UIKTAG_OBJ_LeftTop              (TAG_USER|0x0001)
#define UIKTAG_OBJ_WidthHeight          (TAG_USER|0x0002)
#define UIKTAG_OBJ_HitMask              (TAG_USER|0x0003)
#define UIKTAG_OBJ_TitleUnderscore      (TAG_USER|0x0004)
#define UIKTAG_OBJ_ActInactPens         (TAG_USER|0x0005)
#define UIKTAG_OBJ_Title                (TAG_USER|0x0006)
#define UIKTAG_OBJ_AltTitle             (TAG_USER|0x0007)
#define UIKTAG_OBJ_BoxAutomate          (TAG_USER|0x0008)
#define UIKTAG_reserved1                (TAG_USER|0x0009)
#define UIKTAG_OBJ_UserValue1           (TAG_USER|0x000a)
#define UIKTAG_OBJ_UserValue2           (TAG_USER|0x000b)
#define UIKTAG_OBJ_BoxAltAutomate       (TAG_USER|0x000c)
#define UIKTAG_OBJ_Pattern              (TAG_USER|0x000d)
#define UIKTAG_OBJ_PatternPower         (TAG_USER|0x000e)
#define UIKTAG_OBJ_VisualEraseColor     (TAG_USER|0x000f)
#define UIKTAG_OBJ_UserStopFunc         (TAG_USER|0x0010)
#define UIKTAG_OBJ_UserRemoveFunc       (TAG_USER|0x0011)
#define UIKTAG_OBJ_FontName             (TAG_USER|0x0012)
#define UIKTAG_OBJ_FontHeight           (TAG_USER|0x0013)
#define UIKTAG_OBJ_CustomDraw           (TAG_USER|0x0014)
#define UIKTAG_OBJ_CustomDrawData       (TAG_USER|0x0015)
#define UIKTAG_OBJ_IPRemoveFunc         (TAG_USER|0x0016)
#define UIKTAG_OBJ_ZoneFunc             (TAG_USER|0x0017)
#define UIKTAG_OBJ_TTextAttr            (TAG_USER|0x0018)
#define UIKTAG_OBJ_LangMaster           (TAG_USER|0x0019)
#define UIKTAG_OBJ_ShortCutKey          (TAG_USER|0x001a)
#define UIKTAG_reserved2                (TAG_USER|0x001b)
#define UIKTAG_OBJ_MinVerRev            (TAG_USER|0x001c)
#define UIKTAG_OBJ_MultiTitleDelay      (TAG_USER|0x001d)
#define UIKTAG_OBJ_MultiTitleNum        (TAG_USER|0x001e)

/*----UIKObj Flags Tags---- */
#define UIKTAG_OBJ_Box_Flags            (TAG_USER|0x0020)
#define UIKTAG_OBJ_BoxFl_NotDrawn       (TAG_USER|0x0021)
#define UIKTAG_OBJ_BoxFl_ExtGadgetUp    (TAG_USER|0x0022)
#define UIKTAG_OBJ_BoxFl_Frame          (TAG_USER|0x0023)
#define UIKTAG_OBJ_BoxFl_NoAct          (TAG_USER|0x0024)
#define UIKTAG_OBJ_BoxFl_Over           (TAG_USER|0x0025)
#define UIKTAG_OBJ_BoxFl_Filled         (TAG_USER|0x0026)
#define UIKTAG_OBJ_BoxFl_Clipped        (TAG_USER|0x0027)
#define UIKTAG_OBJ_BoxFl_Semaphore      (TAG_USER|0x0028)
#define UIKTAG_OBJ_BoxFl_NoPush         (TAG_USER|0x0029)
#define UIKTAG_OBJ_BoxFl_TranslateLeftTop (TAG_USER|0x002a)
#define UIKTAG_OBJ_BoxFl_UseBitMap      (TAG_USER|0x002b)
#define UIKTAG_OBJ_BoxFl_Zoneable       (TAG_USER|0x002c)
#define UIKTAG_OBJ_BoxFl_Displayable    (TAG_USER|0x002d)
#define UIKTAG_OBJ_BoxFl_StartSpecify   (TAG_USER|0x002e)
#define UIKTAG_OBJ_BoxFl_Disabled       (TAG_USER|0x002f)
#define UIKTAG_OBJ_BoxFl_CannotDisable  (TAG_USER|0x0030)

#define UIKTAG_OBJ_Title_Flags          (TAG_USER|0x0040)
#define UIKTAG_OBJ_TitleFl_Top          (TAG_USER|0x0042)
#define UIKTAG_OBJ_TitleFl_Left         (TAG_USER|0x0043)
#define UIKTAG_OBJ_TitleFl_Right        (TAG_USER|0x0044)
#define UIKTAG_OBJ_TitleFl_Center       (TAG_USER|0x0045)
#define UIKTAG_reserved4                (TAG_USER|0x0046)
#define UIKTAG_OBJ_TitleFl_Addr         (TAG_USER|0x0047)
#define UIKTAG_OBJ_TitleFl_Clipped      (TAG_USER|0x0048)
#define UIKTAG_OBJ_TitleFl_Image        (TAG_USER|0x0049)
#define UIKTAG_OBJ_TitleFl_Above        (TAG_USER|0x004a)
#define UIKTAG_OBJ_TitleFl_InsRight     (TAG_USER|0x004b)
#define UIKTAG_OBJ_TitleFl_InsLeft      (TAG_USER|0x004c)

#define UIKTAG_OBJ_Sz_Attached_Flags    (TAG_USER|0x0060)
#define UIKTAG_OBJ_Sz_AttachedFl_Left   (TAG_USER|0x0061)
#define UIKTAG_OBJ_Sz_AttachedFl_Top    (TAG_USER|0x0062)
#define UIKTAG_OBJ_Sz_AttachedFl_Right  (TAG_USER|0x0063)
#define UIKTAG_OBJ_Sz_AttachedFl_Bot    (TAG_USER|0x0064)

#endif
