
#include "uiki:objarexxsim.h"
#include "uiki:objdial.h"
#include "uiki:objdispatch.h"
#include "uiki:objdisplaymode.h"
#include "uiki:objfileselector.h"
#include "uiki:objhelp.h"
#include "uiki:objilbm.h"
#include "uiki:objinputick.h"
#include "uiki:objjoystick.h"
#include "uiki:objlist.h"
#include "uiki:objmemory.h"
#include "uiki:objmenu.h"
#include "uiki:objprogress.h"
#include "uiki:objproplift.h"
#include "uiki:objreqstring.h"
#include "uiki:objrequest.h"
#include "uiki:objscreen.h"
#include "uiki:objsmus.h"
#include "uiki:objstring.h"
#include "uiki:objwindow.h"
#include "uiki:objwinhelp.h"
#include "uiki:uikbase.h"
#include "uiki:uikbox.h"
#include "uiki:uikbuf.h"
#include "uiki:uikcolors.h"
#include "uiki:uikdrawauto.h"
#include "uiki:uikgfx.h"
#include "uiki:uikglobal.h"
#include "uiki:uikkey.h"
#include "uiki:uiklang.h"
#include "uiki:uiklist.h"
#include "uiki:uikmacros.h"
#include "uiki:uikobj.h"
#include "uiki:uiktag.h"
#include "uiki:uiktree.h"
#include "uiki:uikzone.h"

#include "uiki:objchecktoggle.h"
#include "uiki:objradioexclude.h"
#include "uiki:objtimer.h"
#include "uiki:objwfilebut.h"
#include "uiki:objwfontbut.h"
#include "uiki:objwinfilesel.h"
#include "uiki:objwinfontsel.h"

#define SIZ(abcd) { int a=sizeof(struct abcd), b=abcd(); printf( "%-30.30ls: %5.5ld - %5.5ld    %ls\n", Str##abcd(), a, b, a != b ? "!!!" : "" ); }

void main()
{
    SIZ(UIKObjARexxSim)
    SIZ(UIKObjDial)
    SIZ(PrEntry)
    SIZ(Processing)
    SIZ(UIKObjDispatch)
    SIZ(ModeInfo)
    SIZ(UIKObjDisplayMode)
    SIZ(UIKObjFileSelector)
    SIZ(UIKObjHelp)
    SIZ(UIKObjILBM)
    SIZ(UIKIlbmFrame)
    SIZ(UIKObjInputick)
    SIZ(UIKObjJoystick)
    SIZ(TextEntry)
    SIZ(UIKObjList)
    SIZ(UIKObjMem)
    SIZ(UIKMenuEntry)
    SIZ(UIKObjMenu)
    SIZ(UIKObjProgress)
    SIZ(UIKObjPropGad)
    SIZ(UIKObjReqString)
    SIZ(UIKObjReq)
    SIZ(UIKObjScreen)
    SIZ(VectArg)
    SIZ(UIKObjSMUS)
    SIZ(UIKObjStrGad)
    SIZ(UIKObjWindow)
    SIZ(UIKObjWinHelp)
    SIZ(UIKPrefs)
    SIZ(UIKBase)
    SIZ(UIKBox)
    SIZ(UIKBuffer)
    SIZ(UIKTColors)
    SIZ(UIKColors)
    SIZ(UIKBitMap)
    SIZ(UIKGlobal)
    SIZ(UIKKeySupp)
    SIZ(UIKLang)
    SIZ(UIKNode)
    SIZ(UIKList)
    SIZ(UIKTreeItem)
    SIZ(UIKHookItem)
    SIZ(UIKObjGerm)
    SIZ(UIKObjRegGerm)
    SIZ(UIKObjExtension)
    SIZ(UIKObj)
    SIZ(UIKObjGad)
    SIZ(UIKHook)
    SIZ(UIKLink)
    SIZ(UIKZone)
    SIZ(UIKZoneList)

    SIZ(UIKObjCheckToggle)
    SIZ(UIKObjRadioExclude)
    SIZ(UIKObjTimer)
    SIZ(UIKObjWFileBut)
    SIZ(UIKObjWFontBut)
    SIZ(WinFileSelPrefs)
    SIZ(UIKObjWinFileSel)
    SIZ(WinFontSelPrefs)
    SIZ(UIKObjWinFontSel)
}
