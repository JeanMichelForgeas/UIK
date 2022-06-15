    OPT O+,W-

        INCLUDE "uiki:objarexxsim.i"
        INCLUDE "uiki:objdial.i"
        INCLUDE "uiki:objdispatch.i"
        INCLUDE "uiki:objdisplaymode.i"
        INCLUDE "uiki:objfileselector.i"
        INCLUDE "uiki:objhelp.i"
        INCLUDE "uiki:objilbm.i"
        INCLUDE "uiki:objinputick.i"
        INCLUDE "uiki:objjoystick.i"
        INCLUDE "uiki:objlist.i"
        INCLUDE "uiki:objmemory.i"
        INCLUDE "uiki:objmenu.i"
        INCLUDE "uiki:objprogress.i"
        INCLUDE "uiki:objproplift.i"
        INCLUDE "uiki:objreqstring.i"
        INCLUDE "uiki:objrequest.i"
        INCLUDE "uiki:objscreen.i"
        INCLUDE "uiki:objsmus.i"
        INCLUDE "uiki:objstring.i"
        INCLUDE "uiki:objwindow.i"
        INCLUDE "uiki:objwinhelp.i"
        INCLUDE "uiki:uikbase.i"
        INCLUDE "uiki:uikbox.i"
        INCLUDE "uiki:uikbuf.i"
        INCLUDE "uiki:uikcolors.i"
        INCLUDE "uiki:uikdrawauto.i"
        INCLUDE "uiki:uikgfx.i"
        INCLUDE "uiki:uikglobal.i"
        INCLUDE "uiki:uikkey.i"
        INCLUDE "uiki:uiklang.i"
        INCLUDE "uiki:uiklist.i"
        INCLUDE "uiki:uikmacros.i"
        INCLUDE "uiki:uikobj.i"
        INCLUDE "uiki:uiktag.i"
        INCLUDE "uiki:uiktree.i"
        INCLUDE "uiki:uikzone.i"

        INCLUDE "uiki:objchecktoggle.i"
        INCLUDE "uiki:objradioexclude.i"
        INCLUDE "uiki:objtimer.i"
        INCLUDE "uiki:objwfilebut.i"
        INCLUDE "uiki:objwfontbut.i"
        INCLUDE "uiki:objwinfilesel.i"
        INCLUDE "uiki:objwinfontsel.i"

SIZ MACRO
    XDEF    _\1
_\1 move.l  #\1_SIZE,d0
    rts
    XDEF    _Str\1
_Str\1
    move.l  #.\1,d0
    rts
.\1 dc.b    '\1',0
    EVEN
    ENDM

    SIZ UIKObjARexxSim
    SIZ UIKObjDial
    SIZ PrEntry
    SIZ Processing
    SIZ UIKObjDispatch
    SIZ ModeInfo
    SIZ UIKObjDisplayMode
    SIZ UIKObjFileSelector
    SIZ UIKObjHelp
    SIZ UIKObjILBM
    SIZ UIKIlbmFrame
    SIZ UIKObjInputick
    SIZ UIKObjJoystick
    SIZ TextEntry
    SIZ UIKObjList
    SIZ UIKObjMem
    SIZ UIKMenuEntry
    SIZ UIKObjMenu
    SIZ UIKObjProgress
    SIZ UIKObjPropGad
    SIZ UIKObjReqString
    SIZ UIKObjReq
    SIZ UIKObjScreen
    SIZ VectArg
    SIZ UIKObjSMUS
    SIZ UIKObjStrGad
    SIZ UIKObjWindow
    SIZ UIKObjWinHelp
    SIZ UIKPrefs
    SIZ UIKBase
    SIZ UIKBox
    SIZ UIKBuffer
    SIZ UIKTColors
    SIZ UIKColors
    SIZ UIKBitMap
    SIZ UIKGlobal
    SIZ UIKKeySupp
    SIZ UIKLang
    SIZ UIKNode
    SIZ UIKList
    SIZ UIKTreeItem
    SIZ UIKHookItem
    SIZ UIKObjGerm
    SIZ UIKObjRegGerm
    SIZ UIKObjExtension
    SIZ UIKObj
    SIZ UIKObjGad
    SIZ UIKHook
    SIZ UIKLink
    SIZ UIKZone
    SIZ UIKZoneList

    SIZ UIKObjCheckToggle
    SIZ UIKObjRadioExclude
    SIZ UIKObjTimer
    SIZ UIKObjWFileBut
    SIZ UIKObjWFontBut
    SIZ WinFileSelPrefs
    SIZ UIKObjWinFileSel
    SIZ WinFontSelPrefs
    SIZ UIKObjWinFontSel


    END
