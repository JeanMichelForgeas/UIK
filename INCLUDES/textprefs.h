#ifndef UIKPREFS_STR_H
#define UIKPREFS_STR_H


/****************************************************************************/


/* This file was created automatically by CatComp.
 * Do NOT edit by hand!
 */


#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifdef CATCOMP_ARRAY
#undef CATCOMP_NUMBERS
#undef CATCOMP_STRINGS
#define CATCOMP_NUMBERS
#define CATCOMP_STRINGS
#endif

#ifdef CATCOMP_BLOCK
#undef CATCOMP_STRINGS
#define CATCOMP_STRINGS
#endif


/****************************************************************************/


#ifdef CATCOMP_NUMBERS

#define MSG_NULLSTRING 0
#define MSG_FRAME_GAD 1
#define MSG_LOCALE_GAD 2
#define MSG_SAVE_GAD 3
#define MSG_HELP_GAD 4
#define MSG_TEST_GAD 5
#define MSG_LIGHT_GAD 6
#define MSG_DARK_GAD 7
#define MSG_SPECIAL_GAD 8
#define MSG_MEDIUM_GAD 9
#define MSG_TITLE_HELPWINDOW 10
#define MSG_LANGUAGE_FRAME 11
#define MSG_ENGLISH_GAD 12
#define MSG_OTHER_GAD 13
#define MSG_COLORS_FRAME 14
#define MSG_DEFSCREEN_FRAME 15
#define MSG_WORKBENCH_GAD 16
#define MSG_UIK_GAD 17
#define MSG_MODE_FRAME 18
#define MSG_FONT_FRAME 19
#define MSG_UIKSCREEN_FRAME 20
#define MSG_USE_GAD 21
#define MSG_CANCEL_GAD 22
#define MSG_NULLSTRING2 23
#define MSG_COLORS_PAL 24
#define MSG_R_PAL 25
#define MSG_G_PAL 26
#define MSG_B_PAL 27
#define MSG_RESTORE_PAL 28
#define MSG_NULLSTRING3 29
#define MSG_PROJECT_MENU 30
#define MSG_PROJECT_OPEN_MENU 31
#define MSG_PROJECT_SAVEAS_MENU 32
#define MSG_PROJECT_QUIT_MENU 33
#define MSG_EDIT_MENU 34
#define MSG_EDIT_RESET_MENU 35
#define MSG_EDIT_LAST_MENU 36
#define MSG_EDIT_RESTORE_MENU 37
#define MSG_SETTINGS_MENU 38
#define MSG_SETTINGS_CRICON_MENU 39
#define MSG_ENTER_FONT_NAME 40
#define MSG_INPUT_MODES 41
#define MSG_INPUT_MOUSE 42
#define MSG_INPUT_BOURIS 43
#define MSG_INPUT_JOYMOUSE 44
#define MSG_INPUT_1 45
#define MSG_INPUT_2 46
#define MSG_INPUT_3 47
#define MSG_INPUT_AUTO 48
#define MSG_INPUT_HOLD 49
#define MSG_INPUT_TIMES 50
#define MSG_INPUT_SPEED 51
#define MSG_INPUT_VALID 52
#define MSG_INPUT_UNSELECT 53
#define MSG_INPUT_THICK 54
#define MSG_HELP_TEXT 55
#define MSG_SELECT_FONT 56
#define MSG_LOOK 57
#define MSG_SYSLOOK 58
#define MSG_UIKLOOK 59

#endif /* CATCOMP_NUMBERS */


/****************************************************************************/


#ifdef CATCOMP_STRINGS

#define MSG_NULLSTRING_STR ""
#define MSG_FRAME_GAD_STR "Frame"
#define MSG_LOCALE_GAD_STR "Localized"
#define MSG_SAVE_GAD_STR "Save"
#define MSG_HELP_GAD_STR "Help"
#define MSG_TEST_GAD_STR "Test"
#define MSG_LIGHT_GAD_STR "Light"
#define MSG_DARK_GAD_STR "Dark"
#define MSG_SPECIAL_GAD_STR "Special"
#define MSG_MEDIUM_GAD_STR "Medium"
#define MSG_TITLE_HELPWINDOW_STR "UIKPrefs Help"
#define MSG_LANGUAGE_FRAME_STR " Language\x20"
#define MSG_ENGLISH_GAD_STR "English"
#define MSG_OTHER_GAD_STR "Other"
#define MSG_COLORS_FRAME_STR " Colors\x20"
#define MSG_DEFSCREEN_FRAME_STR " Default screen\x20"
#define MSG_WORKBENCH_GAD_STR "Workbench"
#define MSG_UIK_GAD_STR "UIK"
#define MSG_MODE_FRAME_STR " Mode\x20"
#define MSG_FONT_FRAME_STR " Font\x20"
#define MSG_UIKSCREEN_FRAME_STR " UIK screen\x20"
#define MSG_USE_GAD_STR "Use"
#define MSG_CANCEL_GAD_STR "Cancel"
#define MSG_NULLSTRING2_STR ""
#define MSG_COLORS_PAL_STR "Colors"
#define MSG_R_PAL_STR "R"
#define MSG_G_PAL_STR "G"
#define MSG_B_PAL_STR "B"
#define MSG_RESTORE_PAL_STR "Restore"
#define MSG_NULLSTRING3_STR ""
#define MSG_PROJECT_MENU_STR ""
#define MSG_PROJECT_OPEN_MENU_STR ""
#define MSG_PROJECT_SAVEAS_MENU_STR ""
#define MSG_PROJECT_QUIT_MENU_STR ""
#define MSG_EDIT_MENU_STR ""
#define MSG_EDIT_RESET_MENU_STR ""
#define MSG_EDIT_LAST_MENU_STR ""
#define MSG_EDIT_RESTORE_MENU_STR ""
#define MSG_SETTINGS_MENU_STR ""
#define MSG_SETTINGS_CRICON_MENU_STR ""
#define MSG_ENTER_FONT_NAME_STR ""
#define MSG_INPUT_MODES_STR ""
#define MSG_INPUT_MOUSE_STR ""
#define MSG_INPUT_BOURIS_STR ""
#define MSG_INPUT_JOYMOUSE_STR ""
#define MSG_INPUT_1_STR ""
#define MSG_INPUT_2_STR ""
#define MSG_INPUT_3_STR ""
#define MSG_INPUT_AUTO_STR ""
#define MSG_INPUT_HOLD_STR ""
#define MSG_INPUT_TIMES_STR ""
#define MSG_INPUT_SPEED_STR ""
#define MSG_INPUT_VALID_STR ""
#define MSG_INPUT_UNSELECT_STR ""
#define MSG_INPUT_THICK_STR ""
#define MSG_HELP_TEXT_STR ""
#define MSG_SELECT_FONT_STR ""
#define MSG_LOOK_STR ""
#define MSG_SYSLOOK_STR ""
#define MSG_UIKLOOK_STR ""

#endif /* CATCOMP_STRINGS */


/****************************************************************************/


#ifdef CATCOMP_ARRAY

struct CatCompArrayType
{
    LONG   cca_ID;
    STRPTR cca_Str;
};

static const struct CatCompArrayType CatCompArray[] =
{
    {MSG_NULLSTRING,(STRPTR)MSG_NULLSTRING_STR},
    {MSG_FRAME_GAD,(STRPTR)MSG_FRAME_GAD_STR},
    {MSG_LOCALE_GAD,(STRPTR)MSG_LOCALE_GAD_STR},
    {MSG_SAVE_GAD,(STRPTR)MSG_SAVE_GAD_STR},
    {MSG_HELP_GAD,(STRPTR)MSG_HELP_GAD_STR},
    {MSG_TEST_GAD,(STRPTR)MSG_TEST_GAD_STR},
    {MSG_LIGHT_GAD,(STRPTR)MSG_LIGHT_GAD_STR},
    {MSG_DARK_GAD,(STRPTR)MSG_DARK_GAD_STR},
    {MSG_SPECIAL_GAD,(STRPTR)MSG_SPECIAL_GAD_STR},
    {MSG_MEDIUM_GAD,(STRPTR)MSG_MEDIUM_GAD_STR},
    {MSG_TITLE_HELPWINDOW,(STRPTR)MSG_TITLE_HELPWINDOW_STR},
    {MSG_LANGUAGE_FRAME,(STRPTR)MSG_LANGUAGE_FRAME_STR},
    {MSG_ENGLISH_GAD,(STRPTR)MSG_ENGLISH_GAD_STR},
    {MSG_OTHER_GAD,(STRPTR)MSG_OTHER_GAD_STR},
    {MSG_COLORS_FRAME,(STRPTR)MSG_COLORS_FRAME_STR},
    {MSG_DEFSCREEN_FRAME,(STRPTR)MSG_DEFSCREEN_FRAME_STR},
    {MSG_WORKBENCH_GAD,(STRPTR)MSG_WORKBENCH_GAD_STR},
    {MSG_UIK_GAD,(STRPTR)MSG_UIK_GAD_STR},
    {MSG_MODE_FRAME,(STRPTR)MSG_MODE_FRAME_STR},
    {MSG_FONT_FRAME,(STRPTR)MSG_FONT_FRAME_STR},
    {MSG_UIKSCREEN_FRAME,(STRPTR)MSG_UIKSCREEN_FRAME_STR},
    {MSG_USE_GAD,(STRPTR)MSG_USE_GAD_STR},
    {MSG_CANCEL_GAD,(STRPTR)MSG_CANCEL_GAD_STR},
    {MSG_NULLSTRING2,(STRPTR)MSG_NULLSTRING2_STR},
    {MSG_COLORS_PAL,(STRPTR)MSG_COLORS_PAL_STR},
    {MSG_R_PAL,(STRPTR)MSG_R_PAL_STR},
    {MSG_G_PAL,(STRPTR)MSG_G_PAL_STR},
    {MSG_B_PAL,(STRPTR)MSG_B_PAL_STR},
    {MSG_RESTORE_PAL,(STRPTR)MSG_RESTORE_PAL_STR},
    {MSG_NULLSTRING3,(STRPTR)MSG_NULLSTRING3_STR},
    {MSG_PROJECT_MENU,(STRPTR)MSG_PROJECT_MENU_STR},
    {MSG_PROJECT_OPEN_MENU,(STRPTR)MSG_PROJECT_OPEN_MENU_STR},
    {MSG_PROJECT_SAVEAS_MENU,(STRPTR)MSG_PROJECT_SAVEAS_MENU_STR},
    {MSG_PROJECT_QUIT_MENU,(STRPTR)MSG_PROJECT_QUIT_MENU_STR},
    {MSG_EDIT_MENU,(STRPTR)MSG_EDIT_MENU_STR},
    {MSG_EDIT_RESET_MENU,(STRPTR)MSG_EDIT_RESET_MENU_STR},
    {MSG_EDIT_LAST_MENU,(STRPTR)MSG_EDIT_LAST_MENU_STR},
    {MSG_EDIT_RESTORE_MENU,(STRPTR)MSG_EDIT_RESTORE_MENU_STR},
    {MSG_SETTINGS_MENU,(STRPTR)MSG_SETTINGS_MENU_STR},
    {MSG_SETTINGS_CRICON_MENU,(STRPTR)MSG_SETTINGS_CRICON_MENU_STR},
    {MSG_ENTER_FONT_NAME,(STRPTR)MSG_ENTER_FONT_NAME_STR},
    {MSG_INPUT_MODES,(STRPTR)MSG_INPUT_MODES_STR},
    {MSG_INPUT_MOUSE,(STRPTR)MSG_INPUT_MOUSE_STR},
    {MSG_INPUT_BOURIS,(STRPTR)MSG_INPUT_BOURIS_STR},
    {MSG_INPUT_JOYMOUSE,(STRPTR)MSG_INPUT_JOYMOUSE_STR},
    {MSG_INPUT_1,(STRPTR)MSG_INPUT_1_STR},
    {MSG_INPUT_2,(STRPTR)MSG_INPUT_2_STR},
    {MSG_INPUT_3,(STRPTR)MSG_INPUT_3_STR},
    {MSG_INPUT_AUTO,(STRPTR)MSG_INPUT_AUTO_STR},
    {MSG_INPUT_HOLD,(STRPTR)MSG_INPUT_HOLD_STR},
    {MSG_INPUT_TIMES,(STRPTR)MSG_INPUT_TIMES_STR},
    {MSG_INPUT_SPEED,(STRPTR)MSG_INPUT_SPEED_STR},
    {MSG_INPUT_VALID,(STRPTR)MSG_INPUT_VALID_STR},
    {MSG_INPUT_UNSELECT,(STRPTR)MSG_INPUT_UNSELECT_STR},
    {MSG_INPUT_THICK,(STRPTR)MSG_INPUT_THICK_STR},
    {MSG_HELP_TEXT,(STRPTR)MSG_HELP_TEXT_STR},
    {MSG_SELECT_FONT,(STRPTR)MSG_SELECT_FONT_STR},
    {MSG_LOOK,(STRPTR)MSG_LOOK_STR},
    {MSG_SYSLOOK,(STRPTR)MSG_SYSLOOK_STR},
    {MSG_UIKLOOK,(STRPTR)MSG_UIKLOOK_STR},
};

#endif /* CATCOMP_ARRAY */


/****************************************************************************/


#ifdef CATCOMP_BLOCK

static const char CatCompBlock[] =
{
    "\x00\x00\x00\x00\x00\x02"
    MSG_NULLSTRING_STR "\x00\x00"
    "\x00\x00\x00\x01\x00\x06"
    MSG_FRAME_GAD_STR "\x00"
    "\x00\x00\x00\x02\x00\x0A"
    MSG_LOCALE_GAD_STR "\x00"
    "\x00\x00\x00\x03\x00\x06"
    MSG_SAVE_GAD_STR "\x00\x00"
    "\x00\x00\x00\x04\x00\x06"
    MSG_HELP_GAD_STR "\x00\x00"
    "\x00\x00\x00\x05\x00\x06"
    MSG_TEST_GAD_STR "\x00\x00"
    "\x00\x00\x00\x06\x00\x06"
    MSG_LIGHT_GAD_STR "\x00"
    "\x00\x00\x00\x07\x00\x06"
    MSG_DARK_GAD_STR "\x00\x00"
    "\x00\x00\x00\x08\x00\x08"
    MSG_SPECIAL_GAD_STR "\x00"
    "\x00\x00\x00\x09\x00\x08"
    MSG_MEDIUM_GAD_STR "\x00\x00"
    "\x00\x00\x00\x0A\x00\x0E"
    MSG_TITLE_HELPWINDOW_STR "\x00"
    "\x00\x00\x00\x0B\x00\x0C"
    MSG_LANGUAGE_FRAME_STR "\x00\x00"
    "\x00\x00\x00\x0C\x00\x08"
    MSG_ENGLISH_GAD_STR "\x00"
    "\x00\x00\x00\x0D\x00\x06"
    MSG_OTHER_GAD_STR "\x00"
    "\x00\x00\x00\x0E\x00\x0A"
    MSG_COLORS_FRAME_STR "\x00\x00"
    "\x00\x00\x00\x0F\x00\x12"
    MSG_DEFSCREEN_FRAME_STR "\x00\x00"
    "\x00\x00\x00\x10\x00\x0A"
    MSG_WORKBENCH_GAD_STR "\x00"
    "\x00\x00\x00\x11\x00\x04"
    MSG_UIK_GAD_STR "\x00"
    "\x00\x00\x00\x12\x00\x08"
    MSG_MODE_FRAME_STR "\x00\x00"
    "\x00\x00\x00\x13\x00\x08"
    MSG_FONT_FRAME_STR "\x00\x00"
    "\x00\x00\x00\x14\x00\x0E"
    MSG_UIKSCREEN_FRAME_STR "\x00\x00"
    "\x00\x00\x00\x15\x00\x04"
    MSG_USE_GAD_STR "\x00"
    "\x00\x00\x00\x16\x00\x08"
    MSG_CANCEL_GAD_STR "\x00\x00"
    "\x00\x00\x00\x17\x00\x02"
    MSG_NULLSTRING2_STR "\x00\x00"
    "\x00\x00\x00\x18\x00\x08"
    MSG_COLORS_PAL_STR "\x00\x00"
    "\x00\x00\x00\x19\x00\x02"
    MSG_R_PAL_STR "\x00"
    "\x00\x00\x00\x1A\x00\x02"
    MSG_G_PAL_STR "\x00"
    "\x00\x00\x00\x1B\x00\x02"
    MSG_B_PAL_STR "\x00"
    "\x00\x00\x00\x1C\x00\x08"
    MSG_RESTORE_PAL_STR "\x00"
    "\x00\x00\x00\x1D\x00\x02"
    MSG_NULLSTRING3_STR "\x00\x00"
    "\x00\x00\x00\x1E\x00\x02"
    MSG_PROJECT_MENU_STR "\x00\x00"
    "\x00\x00\x00\x1F\x00\x02"
    MSG_PROJECT_OPEN_MENU_STR "\x00\x00"
    "\x00\x00\x00\x20\x00\x02"
    MSG_PROJECT_SAVEAS_MENU_STR "\x00\x00"
    "\x00\x00\x00\x21\x00\x02"
    MSG_PROJECT_QUIT_MENU_STR "\x00\x00"
    "\x00\x00\x00\x22\x00\x02"
    MSG_EDIT_MENU_STR "\x00\x00"
    "\x00\x00\x00\x23\x00\x02"
    MSG_EDIT_RESET_MENU_STR "\x00\x00"
    "\x00\x00\x00\x24\x00\x02"
    MSG_EDIT_LAST_MENU_STR "\x00\x00"
    "\x00\x00\x00\x25\x00\x02"
    MSG_EDIT_RESTORE_MENU_STR "\x00\x00"
    "\x00\x00\x00\x26\x00\x02"
    MSG_SETTINGS_MENU_STR "\x00\x00"
    "\x00\x00\x00\x27\x00\x02"
    MSG_SETTINGS_CRICON_MENU_STR "\x00\x00"
    "\x00\x00\x00\x28\x00\x02"
    MSG_ENTER_FONT_NAME_STR "\x00\x00"
    "\x00\x00\x00\x29\x00\x02"
    MSG_INPUT_MODES_STR "\x00\x00"
    "\x00\x00\x00\x2A\x00\x02"
    MSG_INPUT_MOUSE_STR "\x00\x00"
    "\x00\x00\x00\x2B\x00\x02"
    MSG_INPUT_BOURIS_STR "\x00\x00"
    "\x00\x00\x00\x2C\x00\x02"
    MSG_INPUT_JOYMOUSE_STR "\x00\x00"
    "\x00\x00\x00\x2D\x00\x02"
    MSG_INPUT_1_STR "\x00\x00"
    "\x00\x00\x00\x2E\x00\x02"
    MSG_INPUT_2_STR "\x00\x00"
    "\x00\x00\x00\x2F\x00\x02"
    MSG_INPUT_3_STR "\x00\x00"
    "\x00\x00\x00\x30\x00\x02"
    MSG_INPUT_AUTO_STR "\x00\x00"
    "\x00\x00\x00\x31\x00\x02"
    MSG_INPUT_HOLD_STR "\x00\x00"
    "\x00\x00\x00\x32\x00\x02"
    MSG_INPUT_TIMES_STR "\x00\x00"
    "\x00\x00\x00\x33\x00\x02"
    MSG_INPUT_SPEED_STR "\x00\x00"
    "\x00\x00\x00\x34\x00\x02"
    MSG_INPUT_VALID_STR "\x00\x00"
    "\x00\x00\x00\x35\x00\x02"
    MSG_INPUT_UNSELECT_STR "\x00\x00"
    "\x00\x00\x00\x36\x00\x02"
    MSG_INPUT_THICK_STR "\x00\x00"
    "\x00\x00\x00\x37\x00\x02"
    MSG_HELP_TEXT_STR "\x00\x00"
    "\x00\x00\x00\x38\x00\x02"
    MSG_SELECT_FONT_STR "\x00\x00"
    "\x00\x00\x00\x39\x00\x02"
    MSG_LOOK_STR "\x00\x00"
    "\x00\x00\x00\x3A\x00\x02"
    MSG_SYSLOOK_STR "\x00\x00"
    "\x00\x00\x00\x3B\x00\x02"
    MSG_UIKLOOK_STR "\x00\x00"
};

#endif /* CATCOMP_BLOCK */


/****************************************************************************/


struct LocaleInfo
{
    APTR li_LocaleBase;
    APTR li_Catalog;
};


#ifdef CATCOMP_CODE

STRPTR GetString(struct LocaleInfo *li, LONG stringNum)
{
LONG   *l;
UWORD  *w;
STRPTR  builtIn;

    l = (LONG *)CatCompBlock;

    while (*l != stringNum)
    {
        w = (UWORD *)((ULONG)l + 4);
        l = (LONG *)((ULONG)l + (ULONG)*w + 6);
    }
    builtIn = (STRPTR)((ULONG)l + 6);

#define XLocaleBase LocaleBase
#define LocaleBase li->li_LocaleBase
    
    if (LocaleBase)
        return(GetCatalogStr(li->li_Catalog,stringNum,builtIn));
#define LocaleBase XLocaleBase
#undef XLocaleBase

    return(builtIn);
}


#endif /* CATCOMP_CODE */


/****************************************************************************/


#endif /* UIKPREFS_STR_H */
