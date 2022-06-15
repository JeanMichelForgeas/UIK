#ifndef UIKDEFS_WINHELP

#define UIKDEFS_WINHELP
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits R�serv�s
 */

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif


struct UIKObjWinHelp
    {
    struct UIKObj       Obj;
    struct UIKObjWindow *WO;
    struct UIKObjHelp   *HO;
    struct UIKBase      *UIKBase;
    };

#define WINHELP_BOX_STDWIDTH 408
#define WINHELP_BOX_STDHEIGHT 80

/*---------Obj help Tags---------- */
/* titre de la fen�tre (UBYTE *) */
#define UIKTAG_WinHelp_WindowTitle  (UIKTAG_USER|0x0001)

/* Ces deux tags sont � �viter maintenant. Ils fonctionnent encore pour
 * raison de compatibilit� avec les anciens programmes.
 * #define UIKTAG_WinHelp_EnglishEntry (UIKTAG_USER|0x0002)
 * #define UIKTAG_WinHelp_OtherEntry   (UIKTAG_USER|0x0003)
 */

/* UIK_Stop() par defaut */
#define UIKTAG_WinHelp_CloseWindowAction (UIKTAG_USER|0x0004)

/* Pointeur sur la chaine � afficher dans la liste.
 * Peut contenir des '\n' pour afficher plusieurs lignes.
 * Pour tenir compte de la localisation il suffit de lui passer
 * en param�tre le r�sultat d'un UIK_LangString().
 */
#define UIKTAG_WinHelp_TextPtr      (UIKTAG_USER|0x0005)

/*---------Obj help Flags---------- */

/*-- */

#endif
