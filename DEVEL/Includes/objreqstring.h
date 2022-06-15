#ifndef UIKDEFS_REQSTRING

#define UIKDEFS_REQSTRING
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef EXEC_TYPES_H
#include "exec/types.h"
#endif

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif


struct UIKObjReqString
    {
    struct UIKObj       Obj;
    struct UIKObj       *Req; /* peut être un UIKObjRequest ou un UIKObjWindow */
    struct UIKObjStrGad *Str;
    struct UIKHook      *KeyHook;
    int                 (*OKFunc)();
    void                (*CancelFunc)();
    void                (*MiddleFunc)();
    struct UIKObjStrGad *Str2;
    struct UIKHook      *KeyHook2;
    struct UIKObjList   *Text;
    ULONG               Flags;
    };


/*--------- ReqString Flags ---------- */
#define ORSF_WINDOW     0x0001
#define ORSF_ACTIVATE   0x0002
#define ORSF_RETQUIET   0x0004

/*--------- ReqString Tags ---------- */
#define UIKTAG_ReqString_OKFunc         (UIKTAG_USER|0x0001) /* fonction à executer sur OK */
#define UIKTAG_ReqString_CancelFunc     (UIKTAG_USER|0x0002) /* fonction à executer sur Cancel, -1 donne une fonction par défaut (UIK_Stop) */

/* Ces deux tags sont à éviter maintenant. Ils fonctionnent encore pour
 * raison de compatibilité avec les anciens programmes.
 * #define UIKTAG_ReqString_EnglishEntry   (UIKTAG_USER|0x0003)  struct TextEntry * version anglaise
 * #define UIKTAG_ReqString_OtherEntry     (UIKTAG_USER|0x0004)  struct TextEntry * version autre langue
 */
/* pointeur sur chaine par défaut pour le string gadget */
#define UIKTAG_ReqString_StringPtr          (UIKTAG_USER|0x0005)

/* Pointeur sur la chaine à afficher au-dessus de l'objet String.
 * Peut contenir des '\n' pour afficher plusieurs lignes.
 * Pour tenir compte de la localisation il suffit de lui passer
 * en paramètre le résultat d'un UIK_LangString().
 */
#define UIKTAG_ReqString_TextPtr            (UIKTAG_USER|0x0006)

/* Pointeur sur une TagList donnée directement à l'objet String */
#define UIKTAG_ReqString_StringTagList      (UIKTAG_USER|0x0007)

/* Index de chaines dans tableaux de langage */
#define UIKTAG_ReqString_OKTitle            (UIKTAG_USER|0x0008)
#define UIKTAG_ReqString_CancelTitle        (UIKTAG_USER|0x0009)
#define UIKTAG_ReqString_MiddleTitle        (UIKTAG_USER|0x000a)

/* fonction sur un troisième bouton */
#define UIKTAG_ReqString_MiddleFunc         (UIKTAG_USER|0x000b)

/* pointeur sur chaine par défaut pour un deuxième objet String */
#define UIKTAG_ReqString_StringPtr2         (UIKTAG_USER|0x000c)

/* Pointeur sur une TagList donnée à un deuxième objet String */
#define UIKTAG_ReqString_StringTagList2     (UIKTAG_USER|0x000d)

/* Pointeur sur une TagList donnée à l'UIKObj_List qui affiche le texte */
#define UIKTAG_ReqString_TextListTagList    (UIKTAG_USER|0x000e)

/* Pointeurs sur chaines de caractères */
#define UIKTAG_ReqString_OKTitlePtr         (UIKTAG_USER|0x000f)
#define UIKTAG_ReqString_CancelTitlePtr     (UIKTAG_USER|0x0010)
#define UIKTAG_ReqString_MiddleTitlePtr     (UIKTAG_USER|0x0011)

#define UIKTAG_ReqString_CloseFunc          (UIKTAG_USER|0x0012) /* fonction à executer sur CLOSEWINDOW */

/*--------- ReqString Tag Flags ---------- */
/* si TRUE, utilise un UIKObj_Window au lieu de UIKObj_Request */
#define UIKTAG_ReqStringFl_Window             (UIKTAG_USER|0x0041)

/* si FALSE, la zone de saisie ne sera pas activée */
#define UIKTAG_ReqStringFl_Activate           (UIKTAG_USER|0x0042)

/* si TRUE, la touche RETURN si la dernière zone est active, ne ferme pas la requête */
#define UIKTAG_ReqStringFl_StringQuiet        (UIKTAG_USER|0x0043)

/*-- */

#endif
