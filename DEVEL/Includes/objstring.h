#ifndef UIKDEFS_OBJSTRING

#define UIKDEFS_OBJSTRING
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

struct UIKObjStrGad
    {
    struct UIKObj       Obj;
    struct Gadget       Gad;
    UIKBUF              *Buffer;
    UIKBUF              *UndoBuffer;
    ULONG               Reserved1;
    ULONG               Reserved2;
    ULONG               StrFlags;
    ULONG               NumChars;
    ULONG               CursorPos;
    ULONG               CharOffset;
    ULONG               YOffset;
    UBYTE               ActTextPen;
    UBYTE               InactTextPen;
    UBYTE               ActBgPen;
    UBYTE               InactBgPen;
    ULONG               UserHook;
    ULONG               LongIntResult;
    ULONG               MaxChars;
    ULONG               ActiveFunc;
    ULONG               InactiveFunc;
    };

#define UIKST_ALLOCBLOCK    24

/*-----StringGad flags----*/
#define UIKST_LEFT          0x0000
#define UIKST_JUSTIFY       0x0001
#define UIKST_CENTER        0x0002
#define UIKST_RIGHT         0x0004
#define UIKST_AUTOHEIGHT    0x0008
#define UIKST_CURSORON      0x0010
#define UIKST_DIGITALINT    0x0020
#define UIKST_DIGITALHEX    0x0040
#define UIKST_ACTIVATE      0x0080
#define UIKST_NOFILTER      0x0100

/*----UIKObjStr values Tags---- */
#define UIKTAG_OBJStr_CursorPos     (UIKTAG_USER|0x0001)
#define UIKTAG_OBJStr_TextBuffer    (UIKTAG_USER|0x0002)
#define UIKTAG_OBJStr_TextPens      (UIKTAG_USER|0x0003)
#define UIKTAG_OBJStr_NumLines      (UIKTAG_USER|0x0004)
#define UIKTAG_OBJStr_UserHandler   (UIKTAG_USER|0x0005)
#define UIKTAG_OBJStr_ResultPointer (UIKTAG_USER|0x0006)
#define UIKTAG_OBJStr_TextPointer   (UIKTAG_USER|0x0007)
#define UIKTAG_OBJStr_MaxChars      (UIKTAG_USER|0x0008)
#define UIKTAG_OBJStr_ActiveFunc    (UIKTAG_USER|0x0009)
#define UIKTAG_OBJStr_InactiveFunc  (UIKTAG_USER|0x000a)

/*----UIKObjStr Flags Tags---- */
#define UIKTAG_OBJStr_Flags         (UIKTAG_USER|0x0020)
#define UIKTAG_OBJStrFl_CenLeft     (UIKTAG_USER|0x0021)
#define UIKTAG_OBJStrFl_CenJustify  (UIKTAG_USER|0x0022)
#define UIKTAG_OBJStrFl_CenCenter   (UIKTAG_USER|0x0023)
#define UIKTAG_OBJStrFl_CenRight    (UIKTAG_USER|0x0024)
#define UIKTAG_OBJStrFl_AutoHeight  (UIKTAG_USER|0x0025)
#define UIKTAG_OBJStrFl_DigitalInt  (UIKTAG_USER|0x0026)
#define UIKTAG_OBJStrFl_DigitalHex  (UIKTAG_USER|0x0027)
#define UIKTAG_OBJStrFl_Activate    (UIKTAG_USER|0x0028)
#define UIKTAG_OBJStrFl_NoFilter    (UIKTAG_USER|0x0029)

#endif /* UIKDEFS_OBJSTRING */
