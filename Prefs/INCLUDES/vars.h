struct Vars {
    struct UIKBase      *DOSBase;
    struct UIKBase      *UIKBase;
    struct GfxBase      *GfxBase;
    struct Library      *IntuitionBase;
    struct Library      *LocaleBase;
    struct UIKGlobal    *UIK;

    struct UIKObjWinHelp *wh;
    struct UIKObj       *WO;
    struct UIKObj       *FS;
    ULONG               DLock;
    ULONG               OldLock;
    ULONG               Nil;
    struct FileInfoBlock FIB;

    struct UIKPrefs     Prefs;
    struct UIKPrefs     OldPrefs;

    WORD                WLeft, WTop, WWidth, WHeight;
    WORD                HLeft, HTop, HWidth, HHeight;

    UBYTE               Continue;
    UBYTE               PadB;
    UBYTE               PreName[150];
    };
typedef struct Vars VARS;
