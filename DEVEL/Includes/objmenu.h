#ifndef UIKDEFS_MENU

#define UIKDEFS_MENU
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits R�serv�s
 */

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

/* Ceci permet de mettre une s�paration entre deux Items, d'une hauteur
 * de la moiti� de la fonte utilis�e + 1.
 */
#define ITEM_SEPARATION (-1)
#define ITEM_SYSSEPARATION (-2)

struct UIKMenuEntry
    {
    UBYTE       Depth;          /* 0=Menu, 1=Item, 2=SubItem */
    UBYTE       ComKey;         /* touche d'activation de cet item */
    ULONG       Title;          /* num�ro du titre dans tableaux de chaines */
    void        (*Func)();      /* fonction � appeler quand l'item est s�lectionn� */
    ULONG       UserData;       /* UIK ne touche pas � ce champ */
    ULONG       MutualExclude;  /* voir doc Intuition pour ce champ */
    struct MenuItem *IntuiData; /* structure Intuition associ�e (si objet start�) */
    UWORD       Flags;          /* voir "objets.doc" */
    };

#define MENUTEXTADDR    0x8000

struct UIKObjMenu
    {
    struct UIKObj       Obj;
    ULONG               Flags;
    struct UIKMenuEntry *EnglishMenu;   /* priv� */
    struct UIKMenuEntry *OtherMenu;     /* priv� */
    struct UIKMenuEntry *MenuEntry;     /* allocation pour copie des UIKMenuEntry */
    struct Menu         *Menu;          /* structure Menu allou�e par l'objet */
    void                (*MenuFunc)();  /* priv� */
    struct TTextAttr    TTAttr;         /* priv� */

    struct UIKBase      *UIKBase;
    struct DosLibrary   *DOSBase;
    struct GfxBase      *GfxBase;
    struct IntuitionBase *IntuitionBase;

    UWORD               TitleSep;       /* largeur entre les titres de menu */
    UBYTE               TitleStart;     /* abscisse du premier menu */
    UBYTE               TitleExtra;     /* ajout � la largeur de chaque titre */

    struct UIKList      Strings;
    UWORD               LifeId;         /* quand les allocations ne sont plus valides (objet stopp�) ce compteur est incr�ment� */
    UWORD               Padw;
    };

/*-------Fonctions internes publiques------*/
#define UIKFUNC_Menu_AddEntry       0
#define UIKFUNC_Menu_RemoveEntry    4
#define UIKFUNC_Menu_EnableEntry    8
#define UIKFUNC_Menu_DisableEntry   12
#define UIKFUNC_Menu_GetEntryPos    16
#define UIKFUNC_Menu_GetPosEntry    20

#define UIKFUNC_Menu_LastCmd        20

/*---------Obj Menu Tags---------- */
#define UIKTAG_Menu_Entry           (UIKTAG_USER|0x0003)
#define UIKTAG_Menu_TitleStart      (UIKTAG_USER|0x0004) /* defaut 10 */
#define UIKTAG_Menu_TitleSeparation (UIKTAG_USER|0x0005) /* defaut 20 */
#define UIKTAG_Menu_TitleExtra      (UIKTAG_USER|0x0006) /* defaut 10 */

/*---------Obj Menu Flags---------- */
#define MENUSTRIP_SET   0x0001
#define ONEENTRY        0x0002
#define MENUCHANGED     0x0004
#define SSEP_FONTS      0x0008
#define TITLETEXTSINGLE 0x0010

#define UIKTAG_MenuFl_SysSepFont    (UIKTAG_USER|0x0040)
#define UIKTAG_MenuFl_TitleSingle   (UIKTAG_USER|0x0041)

/*-- */

#endif
