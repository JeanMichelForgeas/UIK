*************************************************
*                                               *
*               minitel.demo                    *
*               by Olivier Dewee                *
*               August 1991                     *
*************************************************

    opt o+,w-

    IDNT    TestWithUIK

    SECTION TestWithUIK,CODE        ; DATA BSS CHIP ...

    NOLIST


******* Included Files  *************************

    incdir  "Includeg:"

    include "exec/types.i"
    include "exec/initializers.i"
    include "exec/memory.i"
    include "exec/ports.i"
    include "exec/devices.i"
    include "exec/io.i"
    include "exec/errors.i"
    include "exec/libraries.i"
FUNC_CNT         SET    LIB_USERDEF     * Skip 4 standard vectors
FUNCDEF          MACRO
_LVO\1           EQU    FUNC_CNT
FUNC_CNT         SET    FUNC_CNT-6      * Standard offset-6 bytes each
                 ENDM
    include "exec/exec_lib.i"

;    INCLUDE "workbench/startup.i"

    INCLUDE "libraries/dos_lib.i"
    INCLUDE "libraries/dos.i"
    INCLUDE "libraries/dosextens.i"


;    INCDIR  "Projects_Disk:Minitel/Device/includes/"
;    include "minitel.i"
;    include "mntlbase.i"


    include "UIKI:uik_lib.i"
    include "UIKI:uikbase.i"
    include "UIKI:uikobj.i"
    include "UIKI:uiktree.i"
    include "UIKI:uikglobal.i"
    include "UIKI:uikcolors.i"
    include "UIKI:uikmacros.i"

    include "UIKI:objfileselector.i"
    include "UIKI:objwindow.i"


******* Equates  ********************************

ABSEXECBASE equ 4   ;Absolute location of the pointer to exec.library base

NULL        equ 0
TRUE        equ -1
FALSE       equ 0

BUFFERSIZE  equ 256


******* Exports  ********************************


******* Imports  ********************************

    XREF    _CreatePort
    XREF    _DeletePort
    XREF    _CreateExtIO
    XREF    _DeleteExtIO



******* Macros  *********************************

CALLSYS MACRO   * function      ; call a library via A6 without having to see _LVO
        JSR     _LVO\1(A6)
        ENDM

TEXT    MACRO   * textptr
        dc.l    \1
        ENDM

STRING  MACRO   * string
        dc.b    \1
        dc.b    0
        cnop    0,2
        ENDM


******* Code  ***********************************

Main:
    move.l  4.w,_SysBase
    move.l  #VARS_SIZE,d0
    move.l  #MEMF_CLEAR,d1
    movea.l (ABSEXECBASE).w,a6
    CALLSYS AllocMem
    tst.l   d0
    beq.w   .NoMemory
    movea.l d0,a5
    move.l  d0,(Global).l

;    suba.l  a1,a1
;    CALLSYS FindTask
;    movea.l d0,a0               ; struct Task *a0
;    move.l  TC_TRAPCODE(a0),OldTrapCode(a5)

;    lea     TrapCode(pc),a1
;    move.l  a1,TC_TRAPCODE(a0)

    moveq   #0,d0               ; Pas de version particuliere
    lea     UIKname(pc),a1
    CALLSYS OpenLibrary
    tst.l   d0
    beq.w   .LibNotOpen
    move.l  d0,UIKbase(a5)

    lea     UIKTree(pc),a0
    movea.l d0,a1               ; struct UIKObj *a1
    movea.l a5,a2
    CALLUIK MakeUIKTree
    tst.l   d0
    beq.s   .ObjsNotAdded

.Loop:
    movea.l UIK(a5),a0          ; struct UIKObj *a0
    movea.l UIKbase(a5),a6
    CALLUIK Start
    tst.l   d0
    beq.s   .ObjNotStarted      ; error

    clr.w   Flag(a5)

    movea.l UIK(a5),a0          ; struct UIKObj *a0
;    move.l  #SIGBREAKF_CTRL_C,d0        ; EXTEVENTS
    moveq   #0,d0               ; EXTEVENTS
    CALLUIK Do
    ; d0 : ULONG ??

    tst.w   Flag(a5)
    beq.s   .NoFileName         ; FALSE

    bsr     OpenDev
    tst.l   d0
    beq.s   .NoDevice

    lea     Buffer(a5),a0
    bsr     LoadFile
    tst.l   d0
    beq.s   .NoFile

    movea.l iorequest(a5),a1    ; struct IORequest *a1
;    move.w  #MD_PRI,IO_COMMAND(a1)
    move.l  #5,IO_LENGTH(a1)    ; Met priorite 5
    movea.l (ABSEXECBASE).w,a6
    CALLSYS DoIO
    tst.b   d0
    bne.s   .DeviceError

    movea.l iorequest(a5),a1    ; struct IORequest *a1
    move.w  #CMD_WRITE,IO_COMMAND(a1)
    move.l  FileBase(a5),IO_DATA(a1)
    move.l  LenFile(a5),IO_LENGTH(a1)
    CALLSYS DoIO
;    tst.b   d0
;    bne.s   ---------

.DeviceError:
    bsr     Exit2

.NoFile:
    bsr     CloseDev            ; ferme minitel.device

    bra.w   .Loop               ; .s ?

.ObjsNotAdded:
.ObjNotAdded:
.NoDevice:
.NoFileName:
.ObjNotStarted:
    move.l  UIK(a5),a0          ; struct UIKObj *a0
    movea.l UIKbase(a5),a6
    CALLUIK Remove

.NotInstalled:
    move.l  UIKbase(a5),d0
    beq.s   .LibNotOpen
    movea.l d0,a1
    movea.l (ABSEXECBASE).w,a6
    CALLSYS CloseLibrary

.LibNotOpen:
    movea.l a5,a1
    move.l  #VARS_SIZE,d0
    CALLSYS FreeMem

.NoMemory:
    moveq   #0,d0
    rts


******* Constantes ******************************

    cnop    0,4

Global:     dc.l    0           ; ptr sur vars globals

; !!! si variable referencé ulterieurement en adressage absolu long !!!
; !!! et option o+ a l'assemblage : transformation en adressage     !!!
; !!! absolu court (cause : l'assemblage est effectue en relatif    !!!
; !!! par rapport a 0, la reference a la variable en adressage      !!!
; !!! absolu se trouve a une valeur proche de 0 et <65535           !!!
; !!! donc considere comme convertible en adressage absolu court)   !!!

EL0:    STRING  ""
EL1:    STRING  <"Test uik.library and minitel.device">
EL2:    STRING  <"uik.library by Jean-Michel Forgeas / minitel.device by Olivier Dewee">
EL3:    STRING  "Speed"
EL4:    STRING  "1200"
EL5:    STRING  "4800"
EL6:    STRING  "9600"
EL7:    STRING  "Max."
EL8:    STRING  "Mixte"
EL9:    STRING  "Scroll"
EL10:   STRING  "Maskag"
EL11:   STRING  <"Black & White">
EL12:   STRING  "Intro"

OL0:    STRING  ""
OL1:    STRING  <"Test de uik.library et de minitel.device">
OL2:    STRING  <"uik.library par Jean-Michel Forgeas / minitel.device par Olivier Dewee">
OL3:    STRING  "Speed"
OL4:    STRING  "1200"
OL5:    STRING  "4800"
OL6:    STRING  "9600"
OL7:    STRING  "Max."
OL8:    STRING  "Mixte"
OL9:    STRING  "Rouleau"
OL10:   STRING  "Masquage"
OL11:   STRING  <"Noir & Blanc">
OL12:   STRING  "Intro"

    cnop    0,4

LangEnglish:
    TEXT    EL0
    TEXT    EL1
    TEXT    EL2
    TEXT    EL3
    TEXT    EL4
    TEXT    EL5
    TEXT    EL6
    TEXT    EL7
    TEXT    EL8
    TEXT    EL9
    TEXT    EL10
    TEXT    EL11
    TEXT    EL12
    dc.l    0

LangOther:
    TEXT    OL0
    TEXT    OL1
    TEXT    OL2
    TEXT    OL3
    TEXT    OL4
    TEXT    OL5
    TEXT    OL6
    TEXT    OL7
    TEXT    OL8
    TEXT    OL9
    TEXT    OL10
    TEXT    OL11
    TEXT    OL12
    dc.l    0

UIKName:
    STRING  <"UIK">

WindowName:
    STRING  <"UIKObj_Window">

FileSelName:
    STRING  <"UIKObj_FileSelector">

BoolName:
    STRING  <"UIKObj_Boolean">

MutExcName:
    STRING  <"UIKObj_MutualExclude">

ToggleName:
    STRING  <"UIKObj_Toggle">


    cnop    0,4

* structures TagItem pour Window

    SETL        lefttop,100,25
    SETL        widthheight,450,150
    SETL        minwidthheight,200,100
    UIKCOLS     colors,UIKCOL_LIGHT,UIKCOL_GREY,UIKCOL_WHITE,UIKCOL_BLACK

uiktl:
    TAGITEM     UIKTAG_GEN_LangEnglish,LangEnglish
    TAGITEM     UIKTAG_GEN_LangOther,LangOther
    TAGITEM     TAG_END,0

wtl:    ; window tag list
    TAGITEM     UIKTAG_OBJ_LeftTop,lefttop
    TAGITEM     UIKTAG_OBJ_WidthHeight,widthheight
    TAGITEM     UIKTAG_OBJ_Title,1
    TAGITEM     UIKTAG_OBJ_AltTitle,2
    TAGITEM     UIKTAG_OBJ_ActInactPens,colors
    TAGITEM     UIKTAG_OBJWindow_MinimumWidthHeight,minwidthheight
;    TAGITEM     UIKTAG_OBJWindowFl_AutoAdjustSize,FALSE
    TAGITEM     TAG_END,0


* structures TagItem pour Toggle

    SETL        lefttop,5,5
;    SETL        widthheight,20,10
    UIKCOLS     colors,UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_BLACK,UIKCOL_WHITE

ttl:    ; toggle tag list
    TAGITEM     UIKTAG_OBJ_LeftTop,lefttop
;    TAGITEM     UIKTAG_OBJ_WidthHeight,widthheight
    TAGITEM     UIKTAG_OBJ_Title,11
;    TAGITEM     UIKTAG_OBJ_ParentRelative,TRUE
    TAGITEM     UIKTAG_OBJ_ActInactPens,colors
;    TAGITEM     UIKTAG_OBJ_BoxFl_Filled,TRUE
    TAGITEM     UIKTAG_OBJ_BoxFl_NoPush,TRUE
;    TAGITEM     UIKTAG_OBJ_BoxFl_TranslateLeftTop,TRUE
    TAGITEM     UIKTAG_OBJ_TitleFl_Top,FALSE
    TAGITEM     UIKTAG_OBJ_TitleFl_Above,TRUE
    TAGITEM     TAG_END,0


* structure TagItem pour FileSelector

DEVSWIDTH   equ 100
FILESWIDTH  equ 150

    SETL        lefttop,40,10
    SETL        widthheight,400,130
    SETL        widths,DEVSWIDTH,FILESWIDTH

fstl:   ; file selector tag list
    TAGITEM     UIKTAG_OBJ_LeftTop,lefttop
    TAGITEM     UIKTAG_OBJ_WidthHeight,widthheight
    TAGITEM     UIKTAG_FS_OKFUNC,okfunc         * fonction appelée si OK ou double-click
    TAGITEM     UIKTAG_FS_CANCELFUNC,cancelfunc * fonction appelée si Cancel
;    TAGITEM     UIKTAG_FS_INITPATH,             * pointeur sur chaine de caractères pour le path initial
    TAGITEM     UIKTAG_FS_DEVFILES,widths       * (Width pour liste Devices <<16) | (Width pour list Files) (Width en pixels)
    TAGITEM     UIKTAG_FS_ASSIGNLST,TRUE        * BOOL flag : liste des devices ou liste des Assign
;    TAGITEM     UIKTAG_FS_PATTERN,              * pointeur sur pattern initiale
    TAGITEM     TAG_END,0

    cnop    0,4

UIKTree:
    OBJSTART
    OBJECT  UIK,UIKName,uiktl,0,0,0
        CHILD
        OBJECT  wo,WindowName,wtl,0,0,0
            CHILD
            OBJECT  to,ToggleName,ttl,0,0,0
            OBJECT  fso,FileSelName,fstl,0,0,0
            CHILDEND
        CHILDEND
    OBJEND

******* fonctions 'greffees' aux objets *********

*   void okfunc(struct UIKObjFileSelector *fso , UBYTE *filename)
*  Pile uniquement

uikobj      SET     3*4         ; APTR
filename    SET     3*4+4       ; APTR

okfunc:
    movem.l a5/a6,-(sp)

;    movea.l uikobj(sp),a0       ; struct UIKObjFileSelector *a0

    movea.l Global(pc),a5
    move.w  #TRUE,Flag(a5)

    movea.l filename(sp),a0     ; UBYTE *a0
    lea     Buffer(a5),a1
    move.l  a1,d0               ; dest
    movea.w #BUFFERSIZE,a1      ; len
    movea.l UIKbase(a5),a6
    CALLSYS StcCpy

    movea.l UIK(a5),a0          ; struct UIKObj *a0
    CALLUIK Stop

    movem.l (sp)+,a5/a6
    rts

*   void cancelfunc(struct UIKObjFileSelector *fso)
*   Pile uniquement

uikobj      SET     2*4         ; APTR

cancelfunc:
    movem.l a5/a6,-(sp)

;    movea.l uikobj(sp),a0       ; struct UIKObjFileSelector *a0

    movea.l Global(pc),a5
    clr.w   Flag(a5)

    movea.l UIK(a5),a0          ; struct UIKObj *a0
    movea.l UIKbase(a5),a6
    CALLUIK Stop

    movem.l (sp)+,a5/a6
    rts

;    suba.l  a1,a1
;    movea.l (ABSEXECBASE).w,a6
;    CALLSYS FindTask

;    movea.l d0,a1               ; struct Task *a1
;    move.l  #SIGBREAKF_CTRL_C,d0
;    CALLSYS Signal

; #define TEXT_WindowTitle    1
; #define TEXT_ScreenTitle    2

*********************************

OpenDev:    ; BOOL (VOID)
    move.l  a6,-(sp)

    move.b  #-1,DevError(a5)

    moveq   #0,d0               ; pri
    suba.l  a0,a0               ; name
    bsr     _CreatePort
    tst.l   d0
    beq.s   .Error
    move.l  d0,msgport(a5)

    movea.l d0,a0               ; struct MsgPort *a0
    move.l  #IOSTD_SIZE,d0
    bsr     _CreateExtIO
    tst.l   d0
    beq.s   .Error
    move.l  d0,iorequest(a5)

;    movea.l d0,a1               ; iorequest
;    lea     MntlName(pc),a0     ; devnameptr
;    moveq   #0,d0               ; unit number
;    moveq   #0,d1               ; flags
;    bset    #7,d1
;    movea.l (ABSEXECBASE).w,a6
;    CALLSYS OpenDevice
;    move.b  d0,DevError(a5)
    bra.s   .Error

.End
    movea.l (sp)+,a6
    rts

.Error:
    moveq   #FALSE,d0
    bra.s   .End


*********************************

LoadFile:       ; d0:BOOL (a0:filename)
    movem.l d2-d4/a6,-(sp)
    move.l  #FALSE,-(sp)

    move.l  a0,d2

    moveq   #0,d0
    lea     DosName(pc),a1
    movea.l (ABSEXECBASE).w,a6
    CALLSYS OpenLibrary
    tst.l   d0
    beq.w   .LibNotOpen
    move.l  d0,DosBase(a5)

    move.l  d2,d1               ; FileName
    move.l  #MODE_OLDFILE,d2
    movea.l DosBase(a5),a6
    CALLSYS Open                ; ouverture du fichier 'Videotext
    tst.l   d0
    beq.s   .FileNotOpen
    move.l  d0,FileHdle(a5)
    move.l  d0,d4               ; d4:FileHdle

    move.l  d4,d1
    move.l  #0,d2
    move.l  #OFFSET_END,d3
    CALLSYS Seek                ; se positionne a la fin du fichier

    move.l  d4,d1
    move.l  #0,d2
    move.l  #OFFSET_BEGINNING,d3
    CALLSYS Seek                ; se positionne au debut du fichier
    move.l  d0,d2               ; taille du fichier dans d2

    move.l  #MEMF_CLEAR,d1
    movea.l (ABSEXECBASE).w,a6
    CALLSYS AllocMem            ; d0:size
    tst.l   d0
    beq.s   .NoMemory
    move.l  d0,FileBase(a5)

    move.l  d2,d3               ; length:d3
    move.l  d4,d1               ; filehandle:d1
    move.l  d0,d2               ; buffer:d2
    movea.l DosBase(a5),a6
    CALLSYS Read                ; d0=nombre d'octets lus
    move.l  d0,LenFile(a5)

    move.l  #TRUE,(sp)

.NoMemory:
    move.l  d4,d1               ; fermeture du fichier
    movea.l DosBase(a5),a6
    CALLSYS Close

.FileNotOpen:
    movea.l DosBase(a5),a1      ; struct DosBase *a1
    movea.l (ABSEXECBASE).w,a6
    CALLSYS CloseLibrary

.LibNotOpen:
    move.l  (sp)+,d0            ; BOOL d0
    movem.l (sp)+,d2-d4/a6
    rts


*********************************

CloseDev:
    move.l  a6,-(sp)

    tst.b   DevError(a5)
    bne.s   .DeviceNotOpen
    movea.l iorequest(a5),a1    ; struct IORequest *a1
    movea.l (ABSEXECBASE).w,a6
    CALLSYS CloseDevice

.DeviceNotOpen:
    move.l  iorequest(a5),d0
    beq.s   .NoIORequest
    movea.l d0,a0               ; struct IORequest *a0
    bsr     _DeleteExtIO

.NoIORequest:
    move.l  msgport(a5),d0
    beq.s   .NoPort
    movea.l d0,a0               ; struct MsgPort *a0
    bsr     _DeletePort

.NoPort:
    movea.l (sp)+,a6
    rts


*********************************

Exit2:
    move.l  a6,-(sp)

    movea.l FileBase(a5),a1
    move.l  LenFile(a5),d0
    movea.l (ABSEXECBASE).w,a6
    CALLSYS FreeMem

    movea.l (sp)+,a6
    rts


*********************************

;TrapCode:
;    cmpi.l  #3,(sp)             ; adress error
;    bne.s


;------------------------------------------------------------------------

UIKname:    UIKNAME
DosName:    DOSNAME
    XDEF _SysBase
_SysBase  dc.l 0


 STRUCTURE  VARS,0
    APTR    DosBase
    APTR    FileHdle
    APTR    FileBase
    ULONG   LenFile
    APTR    UIKbase             ; struct UIKBase     *UIKbase
    APTR    UIK                 ; struct UIKGlobal   *UIK
    APTR    msgport
    APTR    iorequest
    BYTE    DevError
    BYTE    Pad
    BOOL    Flag
    APTR    wo                  * struct UIKObj *wo  (window object)
    APTR    fso                 * struct UIKObj *fso (file selector object)
    APTR    to                  * struct UIKObj *to  (toggle object)
;    APTR    OldTrapCode
    STRUCT  Buffer,BUFFERSIZE
    LABEL   VARS_SIZE


    END
