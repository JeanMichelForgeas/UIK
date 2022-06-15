        OPT d+
*************************************
*
*           Automates
*
*************************************


        INCLUDE "uiki:uikdrawauto.i"


    XDEF    _NullAuto
    XDEF    _AutoDrawBox_Light
    XDEF    _AutoDrawBox_Over
    XDEF    _AutoDrawBox_Under
    XDEF    _AutoDrawProp_Over
    XDEF    _AutoDrawList
    XDEF    _AutoDrawList_Under
    XDEF    _AutoDrawProp_Under
    XDEF    _AutoDrawString_Under
    XDEF    _AutoDrawStringSys_Under
    XDEF    _AutoDrawFrame
    XDEF    _AutoDrawReqString
    XDEF    _AutoFillBox
    XDEF    _DraggAuto
    XDEF    _WinTitleAuto
    XDEF    _WinTitleAltAuto
    XDEF    _AutoDrawWindow
    XDEF    _AutoDrawWindowNoFill
    XDEF    _AutoFillWindow
    XDEF    _DepthAuto
    XDEF    _DepthAltAuto
    XDEF    _ZoomAuto
    XDEF    _ZoomAltAuto
    XDEF    _IconAuto
    XDEF    _IconAltAuto
    XDEF    _CloseAuto
    XDEF    _CloseAltAuto


****************************************************************
*
        SECTION data,DATA
*
****************************************************************/

;------------------- Window
_DraggAuto
_NullAuto
    dc.w        END_AUTO

_WinTitleAuto
    dc.w    COL_BLACK,MOVE_Xy,DRAW_XY
    dc.w    COL_WHITE,MOVE_xy,DRAW_xY
    dc.w    X_SUB_1
    dc.w    IF_NOTLOOK_SKIP_N,8
    dc.w        COL_LOOK,MOVE_xY,DRAW_XY,Y_SUB_1
    dc.w    x_ADD_1,COL_PEN,RECTFILL
    dc.w    END_AUTO
_WinTitleAltAuto
    dc.w    COL_BLACK,MOVE_xY,DRAW_xy,X_SUB_1,DRAW_Xy
    dc.w    COL_WHITE,X_ADD_1,MOVE_Xy,DRAW_XY,x_ADD_1,DRAW_xY
    dc.w    X_SUB_1,y_ADD_1,Y_SUB_1
    dc.w    COL_PEN,RECTFILL
    dc.w    END_AUTO

_AutoDrawWindow
    dc.w    COL_BLACK,MOVE_Xy,DRAW_xy,DRAW_xY,DRAW_XY,DRAW_Xy
    dc.w    COL_PEN,x_ADD_1,y_ADD_1,X_SUB_1,Y_SUB_1,RECTFILL
    dc.w    END_AUTO

_AutoDrawWindowNoFill
    dc.w    COL_BLACK,MOVE_Xy,DRAW_xy,DRAW_xY,DRAW_XY,DRAW_Xy
    dc.w    COL_PEN,x_ADD_1,y_ADD_1,X_SUB_1,Y_SUB_1
    dc.w    END_AUTO

_AutoFillWindow
    dc.w    COL_PEN,RECTFILL
    dc.w    END_AUTO

_DepthAuto
    dc.w    COL_BLACK,MOVE_Xy,DRAW_XY
    dc.w    COL_WHITE,MOVE_xy,DRAW_xY
    dc.w    X_SUB_1
    dc.w    IF_NOTLOOK_SKIP_N,20
    dc.w        COL_LOOK,MOVE_xY,DRAW_XY,Y_SUB_1
    dc.w        x_ADD_1,COL_PEN,RECTFILL,Y_ADD_1
    dc.w            SKIP_N,6
    dc.w        x_ADD_1,COL_PEN,RECTFILL
    dc.w    COL_BLACK
    dc.w    x_ADD_N,5,X_SUB_N,8,y_ADD_N,3,Y_SUB_N,4     ; x_ADD_N,5,X_SUB_N,8,y_ADD_N,3,Y_SUB_N,5
    dc.w    MOVE_Xy,DRAW_xy,DRAW_xY,x_ADD_N,3,DRAW_xY
    dc.w    X_ADD_N,3,y_ADD_1,Y_ADD_1
    dc.w    MOVE_xy,DRAW_xY,DRAW_XY,DRAW_Xy,DRAW_xy
    dc.w    COL_WHITE
    dc.w    x_ADD_1,X_SUB_1,y_ADD_1,Y_SUB_1,RECTFILL
    dc.w    END_AUTO
_DepthAltAuto
    dc.w    COL_BLACK,MOVE_Xy,DRAW_XY
    dc.w    COL_WHITE,MOVE_xy,DRAW_xY
    dc.w    X_SUB_1
    dc.w    IF_NOTLOOK_SKIP_N,20
    dc.w        COL_LOOK,MOVE_xY,DRAW_XY,Y_SUB_1
    dc.w        x_ADD_1,COL_WHITE,RECTFILL,Y_ADD_1
    dc.w            SKIP_N,6
    dc.w        x_ADD_1,COL_WHITE,RECTFILL
    dc.w    COL_BLACK
    dc.w    x_ADD_N,5,X_SUB_N,8,y_ADD_N,3,Y_SUB_N,4     ; x_ADD_N,5,X_SUB_N,8,y_ADD_N,3,Y_SUB_N,5
    dc.w    MOVE_xy,DRAW_xY,DRAW_XY,DRAW_Xy,DRAW_xy
    dc.w    x_ADD_N,3,X_ADD_N,3,y_ADD_1,Y_ADD_1
    dc.w    MOVE_xy,DRAW_xY,DRAW_XY,DRAW_Xy,DRAW_xy
    dc.w    END_AUTO

_ZoomAuto
    dc.w    COL_BLACK,MOVE_Xy,DRAW_XY
    dc.w    COL_WHITE,MOVE_xy,DRAW_xY
    dc.w    X_SUB_1
    dc.w    IF_NOTLOOK_SKIP_N,20
    dc.w        COL_LOOK,MOVE_xY,DRAW_XY,Y_SUB_1
    dc.w        x_ADD_1,COL_PEN,RECTFILL,Y_ADD_1
    dc.w            SKIP_N,6
    dc.w        x_ADD_1,COL_PEN,RECTFILL
    dc.w    COL_BLACK
    dc.w    x_ADD_N,8,y_ADD_N,3,MOVE_xy,x_SUB_N,2,DRAW_xy,y_ADD_N,2,DRAW_xy ; x_ADD_N,8,y_ADD_N,4,MOVE_xy,x_SUB_N,2,DRAW_xy,y_ADD_N,2,DRAW_xy
    dc.w    x_ADD_N,2,Y_SUB_N,3,MOVE_xY,x_SUB_N,2,DRAW_xY,Y_SUB_N,2,DRAW_xY ; x_ADD_N,2,Y_SUB_N,4,MOVE_xY,x_SUB_N,2,DRAW_xY,Y_SUB_N,2,DRAW_xY
    dc.w    x_ADD_N,5,y_SUB_N,2,MOVE_xy,x_ADD_N,2,DRAW_xy,y_ADD_N,2,DRAW_xy
    dc.w    x_SUB_N,2,Y_ADD_N,2,MOVE_xY,x_ADD_N,2,DRAW_xY,Y_SUB_N,2,DRAW_xY
    dc.w    END_AUTO
_ZoomAltAuto
    dc.w    COL_BLACK,MOVE_Xy,DRAW_XY
    dc.w    COL_WHITE,MOVE_xy,DRAW_xY
    dc.w    X_SUB_1
    dc.w    IF_NOTLOOK_SKIP_N,20
    dc.w        COL_LOOK,MOVE_xY,DRAW_XY,Y_SUB_1
    dc.w        x_ADD_1,COL_WHITE,RECTFILL,Y_ADD_1
    dc.w            SKIP_N,6
    dc.w        x_ADD_1,COL_WHITE,RECTFILL
    dc.w    COL_BLACK
    dc.w    x_ADD_N,8,y_ADD_N,3,MOVE_xy,x_SUB_N,2,y_ADD_N,2,DRAW_xy
    dc.w    x_ADD_N,2,Y_SUB_N,3,MOVE_xY,x_SUB_N,2,Y_SUB_N,2,DRAW_xY
    dc.w    x_ADD_N,5,y_SUB_N,2,MOVE_xy,x_ADD_N,2,y_ADD_N,2,DRAW_xy
    dc.w    x_SUB_N,2,Y_ADD_N,2,MOVE_xY,x_ADD_N,2,Y_SUB_N,2,DRAW_xY
    dc.w    END_AUTO

_IconAuto
    dc.w    COL_BLACK,MOVE_Xy,DRAW_XY
    dc.w    COL_WHITE,MOVE_xy,DRAW_xY
    dc.w    X_SUB_1
    dc.w    IF_NOTLOOK_SKIP_N,20
    dc.w        COL_LOOK,MOVE_xY,DRAW_XY,Y_SUB_1
    dc.w        x_ADD_1,COL_PEN,RECTFILL,Y_ADD_1
    dc.w            SKIP_N,6
    dc.w        x_ADD_1,COL_PEN,RECTFILL
    dc.w    COL_BLACK
    dc.w    x_ADD_N,10,y_ADD_N,3,X_SUB_N,5,MOVE_xy,DRAW_Xy  ; x_ADD_N,10,y_ADD_N,4,X_SUB_N,5,MOVE_xy,DRAW_Xy
    dc.w    y_ADD_1,MOVE_xy,DRAW_Xy
    dc.w    x_SUB_N,5,y_ADD_N,2,Y_SUB_N,3                   ; x_SUB_N,5,y_ADD_N,2,Y_SUB_N,4
    dc.w    MOVE_xy,DRAW_xY,DRAW_XY,DRAW_Xy,DRAW_xy
    dc.w    END_AUTO
_IconAltAuto
    dc.w    COL_BLACK,MOVE_Xy,DRAW_XY
    dc.w    COL_WHITE,MOVE_xy,DRAW_xY
    dc.w    X_SUB_1
    dc.w    IF_NOTLOOK_SKIP_N,20
    dc.w        COL_LOOK,MOVE_xY,DRAW_XY,Y_SUB_1
    dc.w        x_ADD_1,COL_WHITE,RECTFILL,Y_ADD_1
    dc.w            SKIP_N,6
    dc.w        x_ADD_1,COL_WHITE,RECTFILL
    dc.w    COL_BLACK
    dc.w    x_ADD_N,10,y_ADD_N,3,X_SUB_N,5,MOVE_xy,DRAW_Xy
    dc.w    y_ADD_1,MOVE_xy,DRAW_Xy
    dc.w    x_SUB_N,5,y_ADD_N,2,Y_SUB_N,3
    dc.w    MOVE_xy,DRAW_xY,DRAW_XY,DRAW_Xy,DRAW_xy
    dc.w    END_AUTO

_CloseAuto
    dc.w    COL_BLACK,MOVE_Xy,DRAW_XY
    dc.w    COL_WHITE,MOVE_xy,DRAW_xY
    dc.w    X_SUB_1
    dc.w    IF_NOTLOOK_SKIP_N,20
    dc.w        COL_LOOK,MOVE_xY,DRAW_XY,Y_SUB_1
    dc.w        x_ADD_1,COL_PEN,RECTFILL,Y_ADD_1
    dc.w            SKIP_N,6
    dc.w        x_ADD_1,COL_PEN,RECTFILL
    dc.w    COL_BLACK
    dc.w    x_ADD_N,7,X_SUB_N,7,y_ADD_N,4,Y_SUB_N,4
    dc.w    MOVE_xy,DRAW_xY,DRAW_XY,DRAW_Xy,DRAW_xy
    dc.w    COL_WHITE
    dc.w    x_ADD_1,X_SUB_1,y_ADD_1,Y_SUB_1,RECTFILL
    dc.w    END_AUTO
_CloseAltAuto
    dc.w    COL_BLACK,MOVE_Xy,DRAW_XY
    dc.w    COL_WHITE,MOVE_xy,DRAW_xY
    dc.w    X_SUB_1
    dc.w    IF_NOTLOOK_SKIP_N,20
    dc.w        COL_LOOK,MOVE_xY,DRAW_XY,Y_SUB_1
    dc.w        x_ADD_1,COL_WHITE,RECTFILL,Y_ADD_1
    dc.w            SKIP_N,6
    dc.w        x_ADD_1,COL_WHITE,RECTFILL
    dc.w    COL_BLACK
    dc.w    x_ADD_N,8,X_SUB_N,8,y_ADD_N,5,Y_SUB_N,5
    dc.w    RECTFILL
    dc.w    END_AUTO

;------------------- Light Box
_AutoDrawBox_Light
    dc.w  IF_RATIO2_SKIP_N,(.2-.1)
.1  dc.w    COL_WHITE
    dc.w    MOVE_Xy
    dc.w    DRAW_xy
    dc.w    DRAW_xY
    dc.w    COL_BLACK
    dc.w    x_ADD_1
    dc.w    MOVE_xY
    dc.w    DRAW_XY
    dc.w    IF_LOOK_SKIP_N,8
    dc.w        y_ADD_1
    dc.w        DRAW_Xy
    dc.w            SKIP_N,4
    dc.w        DRAW_Xy
    dc.w        y_ADD_1
    dc.w    Y_SUB_1
    dc.w    X_SUB_1
    dc.w    COL_WHITE
    dc.w    END_AUTO
    ;---------------
.2  dc.w    X_SUB_1
    dc.w    COL_WHITE
    dc.w    MOVE_Xy
    dc.w    DRAW_xy
    dc.w    DRAW_xY
    dc.w    x_ADD_1
    dc.w    MOVE_xy
    dc.w    DRAW_xY
    dc.w    COL_BLACK
    dc.w    X_ADD_1
    dc.w    DRAW_XY
    dc.w    DRAW_Xy
    dc.w    X_SUB_1
    dc.w    y_ADD_1
    dc.w    MOVE_Xy
    dc.w    DRAW_XY
    dc.w    Y_SUB_1
    dc.w    X_SUB_1
    dc.w    x_ADD_1
    dc.w    COL_WHITE
    dc.w    END_AUTO

;------------------- Box
_AutoDrawBox_Over
    dc.w  IF_RATIO2_SKIP_N,(.2-.1)
.1  dc.w    COL_WHITE
    dc.w    MOVE_Xy
    dc.w    DRAW_xy
    dc.w    IF_LOOK_SKIP_N,18
    dc.w        DRAW_xY
    dc.w        x_ADD_1
    dc.w        COL_BLACK
    dc.w        MOVE_xY
    dc.w        DRAW_XY
    dc.w        y_ADD_1
    dc.w        DRAW_Xy
    dc.w            SKIP_N,30
    dc.w        Y_SUB_1
    dc.w        DRAW_xY
    dc.w        Y_ADD_1
    dc.w        COL_BLACK
    dc.w        MOVE_xY
    dc.w        DRAW_XY
    dc.w        DRAW_Xy
    dc.w        y_ADD_1
    dc.w        Y_SUB_1
    dc.w        X_SUB_1
    dc.w        COL_LOOK
    dc.w        x_ADD_1
    dc.w        MOVE_xY
    dc.w        DRAW_XY
    dc.w        DRAW_Xy
    dc.w    Y_SUB_1
    dc.w    X_SUB_1
    dc.w    COL_PEN
    dc.w    IF_NOTLOOK_SKIP_N,6
    dc.w        MOVE_Xy
    dc.w        DRAW_xy
    dc.w        DRAW_xY
    dc.w    END_AUTO
    ;---------------
.2  dc.w    X_SUB_1
    dc.w    COL_WHITE
    dc.w    MOVE_Xy
    dc.w    DRAW_xy
    dc.w    DRAW_xY
    dc.w    x_ADD_1
    dc.w    MOVE_xy
    dc.w    DRAW_xY
    dc.w    COL_BLACK
    dc.w    X_ADD_1
    dc.w    DRAW_XY
    dc.w    DRAW_Xy
    dc.w    X_SUB_1
    dc.w    y_ADD_1
    dc.w    MOVE_Xy
    dc.w    DRAW_XY
    dc.w    Y_SUB_1
    dc.w    X_SUB_1
    dc.w    x_ADD_1
    dc.w    COL_PEN
    dc.w    END_AUTO

_AutoDrawBox_Under
    dc.w  IF_RATIO2_SKIP_N,(.2-.1)
.1  dc.w    COL_BLACK
    dc.w    IF_NOTLOOK_SKIP_N,2
    dc.w        COL_LOOK
    dc.w    MOVE_Xy
    dc.w    DRAW_xy
    dc.w    DRAW_xY
    dc.w    COL_WHITE
    dc.w    x_ADD_1
    dc.w    MOVE_xY
    dc.w    DRAW_XY
    dc.w    y_ADD_1
    dc.w    DRAW_Xy
    dc.w    Y_SUB_1
    dc.w    X_SUB_1
    dc.w    IF_NOTWHITE_SKIP_N,18
    dc.w        SET_BPEN
    dc.w        COL_GREY
    dc.w        MOVE_xY
    dc.w        PATT_ON
    dc.w        DRAW_XY
    dc.w        PATT_OFF
    dc.w        DRAW_Xy
    dc.w        X_SUB_1
    dc.w        Y_SUB_1
    dc.w    COL_PEN
    dc.w    IF_NOTLOOK_SKIP_N,18
    dc.w        MOVE_xY
    dc.w        DRAW_XY
    dc.w        DRAW_Xy
    dc.w        COL_BLACK
    dc.w        MOVE_Xy
    dc.w        DRAW_xy
    dc.w        DRAW_xY
    dc.w        x_ADD_1
    dc.w        y_ADD_1
    dc.w    END_AUTO
    ;---------------
.2  dc.w    X_SUB_1
    dc.w    COL_BLACK
    dc.w    MOVE_Xy
    dc.w    DRAW_xy
    dc.w    DRAW_xY
    dc.w    x_ADD_1
    dc.w    MOVE_xy
    dc.w    DRAW_xY
    dc.w    COL_WHITE
    dc.w    X_ADD_1
    dc.w    DRAW_XY
    dc.w    DRAW_Xy
    dc.w    X_SUB_1
    dc.w    y_ADD_1
    dc.w    MOVE_Xy
    dc.w    DRAW_XY
    dc.w    Y_SUB_1
    dc.w    X_SUB_1
    dc.w    x_ADD_1
    dc.w    IF_NOTWHITE_SKIP_N,18
    dc.w        SET_BPEN
    dc.w        COL_GREY
    dc.w        MOVE_xY
    dc.w        PATT_ON
    dc.w        DRAW_XY
    dc.w        PATT_OFF
    dc.w        DRAW_Xy
    dc.w        Y_SUB_1
    dc.w        X_SUB_1
    dc.w    COL_PEN
    dc.w    END_AUTO

;------------------- Liste
_AutoDrawList
    dc.w  IF_RATIO2_SKIP_N,(.2-.1)
.1  dc.w    COL_WHITE
    dc.w    MOVE_Xy
    dc.w    DRAW_xy
    dc.w    IF_LOOK_SKIP_N,18
    dc.w        DRAW_xY
    dc.w        x_ADD_1
    dc.w        COL_BLACK
    dc.w        MOVE_xY
    dc.w        DRAW_XY
    dc.w        y_ADD_1
    dc.w        DRAW_Xy
    dc.w            SKIP_N,30
    dc.w        Y_SUB_1
    dc.w        DRAW_xY
    dc.w        Y_ADD_1
    dc.w        COL_BLACK
    dc.w        MOVE_xY
    dc.w        DRAW_XY
    dc.w        DRAW_Xy
    dc.w        y_ADD_1
    dc.w        Y_SUB_1
    dc.w        X_SUB_1
    dc.w        COL_LOOK
    dc.w        x_ADD_1
    dc.w        MOVE_xY
    dc.w        DRAW_XY
    dc.w        DRAW_Xy
    dc.w    Y_SUB_1
    dc.w    X_SUB_1
    dc.w    COL_PEN
    dc.w    MOVE_Xy,DRAW_xy,DRAW_xY,DRAW_XY,DRAW_Xy
    dc.w    x_ADD_1,y_ADD_1,X_SUB_1,Y_SUB_1
    dc.w    END_AUTO
    ;---------------
.2  dc.w    X_SUB_1
    dc.w    COL_WHITE
    dc.w    MOVE_Xy
    dc.w    DRAW_xy
    dc.w    DRAW_xY
    dc.w    x_ADD_1
    dc.w    MOVE_xy
    dc.w    DRAW_xY
    dc.w    COL_BLACK
    dc.w    X_ADD_1
    dc.w    DRAW_XY
    dc.w    DRAW_Xy
    dc.w    X_SUB_1
    dc.w    y_ADD_1
    dc.w    MOVE_Xy
    dc.w    DRAW_XY
    dc.w    Y_SUB_1
    dc.w    X_SUB_1
    dc.w    x_ADD_1
    dc.w    COL_PEN
    dc.w    MOVE_Xy,DRAW_xy,DRAW_xY,DRAW_XY,DRAW_Xy
    dc.w    x_ADD_1,y_ADD_1,X_SUB_1,Y_SUB_1
    dc.w    END_AUTO

_AutoDrawList_Under
    dc.w  IF_RATIO2_SKIP_N,(.2-.1)
.1  dc.w    COL_BLACK
    dc.w    IF_NOTLOOK_SKIP_N,2
    dc.w        COL_LOOK
    dc.w    MOVE_Xy
    dc.w    DRAW_xy
    dc.w    DRAW_xY
    dc.w    COL_WHITE
    dc.w    x_ADD_1
    dc.w    MOVE_xY
    dc.w    DRAW_XY
    dc.w    y_ADD_1
    dc.w    DRAW_Xy
    dc.w    Y_SUB_1
    dc.w    X_SUB_1
    dc.w    IF_NOTLOOK_SKIP_N,12
    dc.w        COL_BLACK
    dc.w        MOVE_Xy
    dc.w        DRAW_xy
    dc.w        DRAW_xY
    dc.w        x_ADD_1
    dc.w        y_ADD_1
    dc.w    IF_NOTWHITE_SKIP_N,18
    dc.w        SET_BPEN
    dc.w        COL_GREY
    dc.w        MOVE_xY
    dc.w        PATT_ON
    dc.w        DRAW_XY
    dc.w        PATT_OFF
    dc.w        DRAW_Xy
    dc.w        Y_SUB_1
    dc.w        X_SUB_1
    dc.w    COL_PEN
    dc.w    MOVE_Xy,DRAW_xy,DRAW_xY,DRAW_XY,DRAW_Xy
    dc.w    x_ADD_1,y_ADD_1,X_SUB_1,Y_SUB_1
    dc.w    END_AUTO
    ;---------------
.2  dc.w    X_SUB_1
    dc.w    COL_BLACK
    dc.w    MOVE_Xy
    dc.w    DRAW_xy
    dc.w    DRAW_xY
    dc.w    x_ADD_1
    dc.w    MOVE_xy
    dc.w    DRAW_xY
    dc.w    COL_WHITE
    dc.w    X_ADD_1
    dc.w    DRAW_XY
    dc.w    DRAW_Xy
    dc.w    X_SUB_1
    dc.w    y_ADD_1
    dc.w    MOVE_Xy
    dc.w    DRAW_XY
    dc.w    Y_SUB_1
    dc.w    X_SUB_1
    dc.w    x_ADD_1
    dc.w    IF_NOTWHITE_SKIP_N,18
    dc.w        SET_BPEN
    dc.w        COL_GREY
    dc.w        MOVE_xY
    dc.w        PATT_ON
    dc.w        DRAW_XY
    dc.w        PATT_OFF
    dc.w        DRAW_Xy
    dc.w        Y_SUB_1
    dc.w        X_SUB_1
    dc.w    COL_PEN
    dc.w    MOVE_Xy,DRAW_xy,DRAW_xY,DRAW_XY,DRAW_Xy
    dc.w    x_ADD_1,y_ADD_1,X_SUB_1,Y_SUB_1
    dc.w    END_AUTO

;------------------- Prop
_AutoDrawProp_Over
    dc.w  IF_RATIO2_SKIP_N,(.2-.1)
.1  dc.w    x_ADD_1
    dc.w    X_SUB_1
    dc.w    COL_WHITE
    dc.w    MOVE_Xy
    dc.w    DRAW_xy
    dc.w    DRAW_xY
    dc.w    COL_BLACK
    dc.w    x_ADD_1
    dc.w    MOVE_xY
    dc.w    DRAW_XY
    dc.w    y_ADD_1
    dc.w    DRAW_Xy
    dc.w    Y_SUB_1
    dc.w    X_SUB_1
    dc.w    COL_PEN
    dc.w    END_AUTO
    ;---------------
.2  dc.w    X_SUB_1
    dc.w    COL_WHITE
    dc.w    MOVE_Xy
    dc.w    DRAW_xy
    dc.w    DRAW_xY
    dc.w    x_ADD_1
    dc.w    MOVE_xy
    dc.w    DRAW_xY
    dc.w    COL_BLACK
    dc.w    X_ADD_1
    dc.w    DRAW_XY
    dc.w    DRAW_Xy
    dc.w    X_SUB_1
    dc.w    y_ADD_1
    dc.w    MOVE_Xy
    dc.w    DRAW_XY
    dc.w    Y_SUB_1
    dc.w    X_SUB_1
    dc.w    x_ADD_1
    dc.w    COL_PEN
    dc.w    END_AUTO

_AutoDrawProp_Under
    dc.w  IF_RATIO2_SKIP_N,(.2-.1)
.1  dc.w    x_ADD_1
    dc.w    X_SUB_1
    dc.w    COL_BLACK
    dc.w    MOVE_Xy
    dc.w    DRAW_xy
    dc.w    DRAW_xY
    dc.w    COL_WHITE
    dc.w    x_ADD_1
    dc.w    MOVE_xY
    dc.w    DRAW_XY
    dc.w    y_ADD_1
    dc.w    DRAW_Xy
    dc.w    Y_SUB_1
    dc.w    X_SUB_1
    dc.w    COL_PEN
    dc.w    END_AUTO
    ;---------------
.2  dc.w    X_SUB_1
    dc.w    COL_BLACK
    dc.w    MOVE_Xy
    dc.w    DRAW_xy
    dc.w    DRAW_xY
    dc.w    x_ADD_1
    dc.w    MOVE_xy
    dc.w    DRAW_xY
    dc.w    COL_WHITE
    dc.w    X_ADD_1
    dc.w    DRAW_XY
    dc.w    DRAW_Xy
    dc.w    X_SUB_1
    dc.w    y_ADD_1
    dc.w    MOVE_Xy
    dc.w    DRAW_XY
    dc.w    Y_SUB_1
    dc.w    X_SUB_1
    dc.w    x_ADD_1
    dc.w    COL_PEN
    dc.w    END_AUTO

;------------------- Frame
_AutoDrawFrame
    dc.w  IF_RATIO2_SKIP_N,(.2-.1)
.1  dc.w    SET_BPEN
    dc.w    IF_NOTLOOK_SKIP_N,6
    dc.w        COL_LOOK
    dc.w            SKIP_N,2
    dc.w        COL_BLACK
    dc.w    MOVE_Xy
    dc.w    DRAW_xy
    dc.w    DRAW_xY
    dc.w    x_ADD_1
    dc.w    Y_SUB_1
    dc.w    MOVE_xY
    dc.w    X_SUB_1
    dc.w    DRAW_XY
    dc.w    y_ADD_1
    dc.w    y_ADD_1
    dc.w    PATT_OFF
    dc.w    DRAW_Xy
    dc.w    COL_WHITE
    dc.w    y_SUB_1
    dc.w    MOVE_Xy
    dc.w    IF_LOOK_PATT_ON
    dc.w    DRAW_xy
    dc.w    Y_ADD_1
    dc.w    PATT_OFF
    dc.w    DRAW_xY
    dc.w    x_ADD_1
    dc.w    X_ADD_1
    dc.w    MOVE_xY
    dc.w    IF_LOOK_PATT_ON
    dc.w    DRAW_XY
    dc.w    PATT_OFF
    dc.w    DRAW_Xy
    dc.w    y_ADD_1
    dc.w    Y_SUB_1
    dc.w    Y_SUB_1
    dc.w    X_SUB_1
    dc.w    X_SUB_1
    dc.w    END_AUTO
    ;---------------
.2  dc.w    SET_BPEN
    dc.w    IF_NOTLOOK_SKIP_N,6
    dc.w        COL_LOOK
    dc.w            SKIP_N,2
    dc.w        COL_BLACK
    dc.w    MOVE_Xy
    dc.w    IF_NOTLOOK_PATT_ON
    dc.w    DRAW_xy
    dc.w    PATT_OFF
    dc.w    DRAW_xY
    dc.w    x_ADD_1
    dc.w    Y_SUB_1
    dc.w    MOVE_xY
    dc.w    X_SUB_1
    dc.w    IF_NOTLOOK_PATT_ON
    dc.w    DRAW_XY
    dc.w    y_ADD_1
    dc.w    y_ADD_1
    dc.w    PATT_OFF
    dc.w    DRAW_Xy
    dc.w    COL_WHITE
    dc.w    y_SUB_1
    dc.w    MOVE_Xy
    dc.w    IF_LOOK_PATT_ON
    dc.w    DRAW_xy
    dc.w    Y_ADD_1
    dc.w    PATT_OFF
    dc.w    DRAW_xY
    dc.w    x_ADD_1
    dc.w    X_ADD_1
    dc.w    MOVE_xY
    dc.w    IF_LOOK_PATT_ON
    dc.w    DRAW_XY
    dc.w    PATT_OFF
    dc.w    DRAW_Xy
    dc.w    y_ADD_1
    dc.w    Y_SUB_1
    dc.w    Y_SUB_1
    dc.w    X_SUB_1
    dc.w    X_SUB_1
    dc.w    END_AUTO

;---------------------------------

_AutoDrawString_Under
    dc.w  IF_RATIO2_SKIP_N,(.2-.1)
.1  dc.w    COL_BLACK
    dc.w    MOVE_Xy
    dc.w    DRAW_xy
    dc.w    DRAW_xY
    dc.w    COL_WHITE
    dc.w    x_ADD_1
    dc.w    MOVE_xY
    dc.w    DRAW_XY
    dc.w    y_ADD_1
    dc.w    DRAW_Xy
    dc.w    Y_SUB_1
    dc.w    X_SUB_1
    dc.w    COL_WHITE
    dc.w    IF_NOTWHITE_SKIP_N,18
    dc.w        SET_BPEN
    dc.w        COL_GREY
    dc.w        MOVE_xY
    dc.w        PATT_ON
    dc.w        DRAW_XY
    dc.w        PATT_OFF
    dc.w        DRAW_Xy
    dc.w        X_SUB_1
    dc.w        Y_SUB_1
    dc.w    COL_PEN
    dc.w    MOVE_xy
    dc.w    DRAW_xY
    dc.w    x_ADD_1
    dc.w    END_AUTO
    ;---------------
.2  dc.w    X_SUB_1
    dc.w    COL_BLACK
    dc.w    MOVE_Xy
    dc.w    DRAW_xy
    dc.w    DRAW_xY
    dc.w    x_ADD_1
    dc.w    MOVE_xy
    dc.w    DRAW_xY
    dc.w    COL_WHITE
    dc.w    X_ADD_1
    dc.w    DRAW_XY
    dc.w    DRAW_Xy
    dc.w    X_SUB_1
    dc.w    y_ADD_1
    dc.w    MOVE_Xy
    dc.w    DRAW_XY
    dc.w    Y_SUB_1
    dc.w    X_SUB_1
    dc.w    x_ADD_1
    dc.w    IF_NOTWHITE_SKIP_N,18
    dc.w        SET_BPEN
    dc.w        COL_GREY
    dc.w        MOVE_xY
    dc.w        PATT_ON
    dc.w        DRAW_XY
    dc.w        PATT_OFF
    dc.w        DRAW_Xy
    dc.w        X_SUB_1
    dc.w        Y_SUB_1
    dc.w    COL_PEN
    dc.w    MOVE_xy
    dc.w    DRAW_xY
    dc.w    x_ADD_1
    dc.w    END_AUTO

_AutoDrawStringSys_Under
    dc.w    Y_ADD_1
    ;dc.w  IF_RATIO2_SKIP_N,(.2-.1)
    dc.w    COL_WHITE   -----------
    dc.w    X_SUB_1
    dc.w    MOVE_Xy
    dc.w    DRAW_xy     horiz   haut
    dc.w    DRAW_xY     vert    gauche
    dc.w    x_ADD_1
    dc.w    y_ADD_1
    dc.w    Y_SUB_1
    dc.w    MOVE_xy
    dc.w    DRAW_xY
    dc.w    x_ADD_N,2   horiz   haut
    dc.w    X_SUB_1
    dc.w    MOVE_xY
    dc.w    DRAW_XY
    dc.w    DRAW_Xy     vert    droit
    dc.w    X_SUB_1
    dc.w    y_ADD_1
    dc.w    MOVE_Xy
    dc.w    DRAW_XY
    dc.w    COL_BLACK   ------------
    dc.w    X_ADD_N,2   vert    droit
    dc.w    y_SUB_1
    dc.w    MOVE_Xy
    dc.w    DRAW_XY
    dc.w    X_ADD_1
    dc.w    y_SUB_1
    dc.w    Y_ADD_1
    dc.w    MOVE_Xy
    dc.w    DRAW_XY
    dc.w    x_SUB_N,2   horiz   bas
    dc.w    DRAW_xY
    dc.w    x_ADD_1     vert    gauche
    dc.w    y_ADD_1
    dc.w    MOVE_xY
    dc.w    DRAW_xy
    dc.w    x_ADD_1
    dc.w    Y_SUB_N,2
    dc.w    MOVE_xY
    dc.w    DRAW_xy
    dc.w    X_SUB_N,3   horiz   haut
    dc.w    DRAW_Xy
    dc.w    COL_LIGHT ;PEN     ------------
    dc.w    x_ADD_N,2
    dc.w    y_ADD_1
    dc.w    X_SUB_N,2
    dc.w    END_AUTO

;---------------------------------

_AutoFillBox
    dc.w    COL_PEN
    dc.w    FILLPAT_ON
    dc.w    RECTFILL
    dc.w    FILLPAT_OFF
    dc.w    END_AUTO

;---------------------------------

_AutoDrawReqString
    dc.w  COL_BLACK,MOVE_Xy,DRAW_xy,DRAW_xY,DRAW_XY,DRAW_Xy
    dc.w  x_ADD_1,y_ADD_1,X_SUB_1,Y_SUB_1
    dc.w  IF_RATIO2_SKIP_N,(.2-.1)
.1  dc.w    COL_WHITE
    dc.w    MOVE_Xy
    dc.w    DRAW_xy
    dc.w    DRAW_xY
    dc.w    x_ADD_1
    dc.w    COL_BLACK
    dc.w    MOVE_xY
    dc.w    DRAW_XY
    dc.w    y_ADD_1
    dc.w    DRAW_Xy
    dc.w    Y_SUB_1
    dc.w    X_SUB_1
    dc.w    COL_PEN
    dc.w    END_AUTO
    ;---------------
.2  dc.w    X_SUB_1
    dc.w    COL_WHITE
    dc.w    MOVE_Xy
    dc.w    DRAW_xy
    dc.w    DRAW_xY
    dc.w    x_ADD_1
    dc.w    MOVE_xy
    dc.w    DRAW_xY
    dc.w    COL_BLACK
    dc.w    X_ADD_1
    dc.w    DRAW_XY
    dc.w    DRAW_Xy
    dc.w    X_SUB_1
    dc.w    y_ADD_1
    dc.w    MOVE_Xy
    dc.w    DRAW_XY
    dc.w    Y_SUB_1
    dc.w    X_SUB_1
    dc.w    x_ADD_1
    dc.w    COL_PEN
    dc.w    END_AUTO



        END
