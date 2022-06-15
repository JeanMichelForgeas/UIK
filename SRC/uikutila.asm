    OPT O+,W-
        OPT d+
****************************************************************
*
*       Project:    UIK
*       Function:   diverses
*
*       Created:    11/03/91    Jean-Michel Forgeas
*
****************************************************************

        IDNT    uikutila


******* Includes ************************************************

        INCDIR  "include:"

        INCLUDE "exec/ables.i"
        INCLUDE "lib/graphics_lib.i"
        INCLUDE "lib/console_lib.i"
        INCLUDE "lib/dos_lib.i"
        INCLUDE "lib/exec_lib.i"
        INCLUDE "lib/diskfont_lib.i"
        INCLUDE "graphics/gfx.i"
        INCLUDE "graphics/gfxbase.i"
        INCLUDE "libraries/dos.i"
        INCLUDE "libraries/dosextens.i"
        INCLUDE "uiki:uikbase.i"
        INCLUDE "uiki:uikobj.i"
        INCLUDE "uiki:uikgfx.i"
        INCLUDE "uiki:uikmacros.i"
        INCLUDE "uiki:uikglobal.i"
        INCLUDE "uiki:uiktree.i"


******* Imported ************************************************

        XREF    _DOSBase
        XREF    _GfxBase
        XREF    _UIKBase
        XREF    _IntuitionBase

        XREF    _UIKPrefsFileName
        XREF    _UIK_AddObject
        XREF    _UIK_AddVector
        XREF    _IntUIK_DoMask
        XREF    DiskFontName


******* Exported ************************************************

        XDEF    _UIK_IsSystemVersion2
        XDEF    _UIK_InvertBlackWhite
        XDEF    _UIK_ChangeAutomate
        XDEF    _UIK_EnableDOSReq
        XDEF    _UIK_DisableDOSReq
        XDEF    _UIK_DeadKeyConvert
        XDEF    _UIK_ReopenFont
        XDEF    _UIK_OpenFont
        XDEF    _UIK_OpenFontTTA
        XDEF    _UIK_Font2TTA
        XDEF    _UIK_BaseName
        XDEF    _UIK_IsActiveKeyTank
        XDEF    _UIK_MakeUIKTree

        XDEF    _UIK_AccessPrefs
        XDEF    _UIK_GetPrefs
        XDEF    _UIK_SetPrefs
        XDEF    _UIK_ReadPrefs
        XDEF    _UIK_SavePrefs
        XDEF    _UIK_ReadArcPrefs
        XDEF    _UIK_SaveEnvPrefs

        XDEF    _UIK_GetTaskUIK

        XDEF    _parent_screen
        XDEF    _parent_window

        XDEF    _do_process_cmd
        XDEF    _do_process_warn
        XDEF    _do_send_warnremove

        XDEF    _IntUIK_OpenFontFillTTA


****************************************************************
*
*       Routines
*
****************************************************************/

        SECTION text,CODE


***********************************************************
*   BOOL UIK_IsSystemVersion2()
*
***********************************************************

_UIK_IsSystemVersion2
        move.l  4.w,a0
        moveq   #1,d0
        cmp.w   #36,LIB_VERSION(a0)
        bpl.b   .end
        moveq   #0,d0
.end
        tst.b   d0
        rts


******************************************
*   UIK_InvertBlackWhite( uik, table, iv )
*                         a0    a1    d0
******************************************

_UIK_InvertBlackWhite
    move.l  d0,d1
    bsr.b   _UIK_IsSystemVersion2
    beq.b   .1.3
.2.0
    tst.b   d1
    bne.b   .end                        1 = images faites sous 2.0
    bra.b   .do_it
.1.3
    tst.b   d1
    beq.b   .end                        0 = images faites sous 1.3
.do_it
    move.l  (a1)+,d0
    beq.b   .end
    move.l  d0,a0
    bset.b  #7,ubm_UFlags(a0)           UIKBMAP_INVDONE
    bne.b   .do_it
    add.w   #UIKTOBMAP_OFFSET,a0
    bsr     Invert
    bra.b   .do_it
.end
    rts

Invert
                movem.l d2-d7/a1-a4,-(a7)

                move            (a0)+,d0        d0 Nbs d'octets
                move            (a0)+,d1        d1 Nbs de lignes
                move            (a0)+,d2        d2 Nbs plans
                and             #$ff,d2

                subq            #2,d2           Nbs plans-2
                bmi.s           fin
                addq.l          #2,a0
                move.l          (a0)+,a1        plan0
                move.l          (a0)+,a2        plan1
                lsr             #1,d0
                subq            #1,d0           mots
                subq            #1,d1           lignes
                subq            #1,d2           plans
                moveq           #0,d3
                move            d0,d4

ligne           move            d4,d0
octet           move            (a1),d7
                move            (a2),d6
                eor             d6,d7
                move            d2,d5
                bmi.s           Que2
                moveq           #0,d6
                lea             (a0),a4
AutresPlans     move.l          (a4)+,a3
                or              (a3,d3),d6
                dbf             d5,AutresPlans
                not             d6
                and             d6,d7
Que2            eor             d7,(a1)+
                eor             d7,(a2)+
                addq.l          #2,d3
                dbf             d0,octet
                dbf             d1,ligne

fin             movem.l (a7)+,d2-d7/a1-a4
                rts


******************************************
*    UIK_ChangeAutomate( taglist )
*                          a0
******************************************
_UIK_ChangeAutomate
    rts


******************************************
*   old = UIK_DisableDOSReq()
*   d0
******************************************

_UIK_DisableDOSReq
    movem.l d1/a0/a1/a6,-(sp)
    sub.l   a1,a1
    move.l  4.w,a6
    jsr     _LVOFindTask(a6)
    move.l  d0,a0
    move.l  pr_WindowPtr(a0),d0
    moveq   #-1,d1
    move.l  d1,pr_WindowPtr(a0)
    movem.l (sp)+,d1/a0/a1/a6
    rts


******************************************
*   UIK_EnableDOSReq( nul, old )
*                          d0
******************************************

_UIK_EnableDOSReq
    movem.l d0/d1/a0/a1/a6,-(sp)
    sub.l   a1,a1
    move.l  4.w,a6
    jsr     _LVOFindTask(a6)
    move.l  d0,a0
    move.l  (sp),pr_WindowPtr(a0)
    movem.l (sp)+,d0/d1/a0/a1/a6
    rts


***********************************************
*   UIK_DeadKeyConvert( keysupp, imsg, keymap )
*                       a0       a1    d0
***********************************************

_UIK_DeadKeyConvert
    movem.l a2/a3/a6,-(sp)
    move.l  a0,a3
    move.l  d0,a2                               a2 = keymap
    sub.l   #ie_SIZEOF,sp
    move.l  sp,a0                               a0 = event

    moveq   #0,d0
    move.l  d0,ie_NextEvent(a0)                 ievent.ie_NextEvent = NULL;
    move.b  #IECLASS_RAWKEY,ie_Class(a0)        ievent.ie_Class = IECLASS_RAWKEY;
    move.w  im_Code(a1),d0
    bclr    #7,d0                               pas de UP
    move.w  d0,ie_Code(a0)                      ievent.ie_Code = imsg->Code;
    move.w  im_Qualifier(a1),d0
    ;bclr    #IEQUALIFIERB_REPEAT,d0
    move.w  d0,ie_Qualifier(a0)                 ievent.ie_Qualifier = imsg->Qualifier & ~(IEQUALIFIER_REPEAT);
    move.l  im_IAddress(a1),a1
    move.l  (a1),ie_EventAddress(a0)            ievent.ie_position.ie_addr = *(APTR*)imsg->IAddress;

    lea     uks_KeyBuffer(a3),a1                a1 = buffer
    move.l  #UIK_KEYBUFLEN-1,d1                   d1 = length
    move.l  uks_ConsoleMsg+IO_DEVICE(a3),a6     a6 = ConsoleDevice
    jsr     _LVORawKeyConvert(a6)
    cmp.l   #-1,d0
    bne.b   .p
    moveq   #UIK_KEYBUFLEN-1,d0
.p  move.l  d0,uks_Length(a3)
    lea     uks_KeyBuffer(a3),a1                a1 = buffer
    clr.b   (a1,d0.l)                           buffer[length] = 0
    add.l   #ie_SIZEOF,sp
    movem.l (sp)+,a2/a3/a6
    tst.l   d0
    rts


****************************************************
*   font = IntUIK_OpenFontFillTTA( UBYTE *name, UWORD height, struct TTextAttr *tta )
*   D0                                     a0           d0                      a1
*   devrait être toujours un succes
****************************************************

_IntUIK_OpenFontFillTTA
    movem.l a2/a6,-(sp)
    sub.w   #tta_SIZEOF,sp
    move.l  a1,a2
    bra.b   FillTTA

****************************************************
*   font = UIK_OpenFontTTA( struct TTextAttr *tta )
*   D0                                        a0
*   devrait être toujours un succes
****************************************************

_UIK_OpenFontTTA
    movem.l a2/a6,-(sp)
    sub.w   #tta_SIZEOF,sp
    move.l  a0,a2
    bra.b   OpenFont

****************************************************
*   font = UIK_OpenFont( UBYTE *name, UWORD height )
*   D0                          a0          d0
*   devrait être toujours un succes
****************************************************

_UIK_OpenFont
    movem.l a2/a6,-(sp)
    sub.w   #tta_SIZEOF,sp       spare for TextAttr
    move.l  sp,a2
FillTTA
    move.l  a0,tta_Name(a2)
    move.w  d0,tta_YSize(a2)
    moveq   #0,d0
    move.w  d0,tta_Style(a2)     style *and* flags
    move.l  d0,tta_Tags(a2)

    bset    #FPB_ROMFONT,tta_Flags(a2)
    move.l  a2,a0
OpenFont
    move.l  _GfxBase,a6
    jsr     _LVOOpenFont(a6)    Font = (struct TextFont *) OpenFont( &TextAttr )
    tst.l   d0
    beq.b   .diskfont

    move.l  d0,a0
    move.w  tta_YSize(a2),d1
    cmp.w   tf_YSize(a0),d1     Font->tf_YSize == TTextAttr.ta_YSize ?
    beq   .end                si oui, OK!
    move.l  a0,a1
    jsr     _LVOCloseFont(a6)
    moveq   #0,d0

.diskfont
    lea     DiskFontName,a1     ouvre librairie
    moveq   #0,d0
    move.l  4.w,a6
    jsr     _LVOOpenLibrary(a6)
    tst.l   d0
    beq.b   .error

    exg     d0,a6                   juste pour ouvrir une fonte
    bset    #FPB_DISKFONT,tta_Flags(a2)
    move.l  a2,a0
    jsr     _LVOOpenDiskFont(a6)    Font = (struct TextFont *) OpenDiskFont( &TextAttr )
    move.l  d0,-(sp)                sauvegarde la fonte
    ;
    move.l  a6,a1                   et referme la librairie
    move.l  4.w,a6
    jsr     _LVOCloseLibrary(a6)
    ;
    move.l  (sp)+,d0                restore la fonte
    beq.b   .error
.end
    add.w   #tta_SIZEOF,sp
    movem.l (sp)+,a2/a6
    tst.l   d0
    rts

.error
    move.l  _GfxBase,a6
    move.l  gb_DefaultFont(a6),a1
    bsr.b   _UIK_ReopenFont
    bra.b   .end


*************************************
*   font = UIK_ReopenFont( font )
*   d0                     a1
*************************************

_UIK_ReopenFont
    move.l  a6,-(sp)
    sub.w   #tta_SIZEOF,sp       spare for TextAttr
    move.l  sp,a0

    move.l  4.w,a6
    jsr     _LVOForbid(a6)       ne touche pas à d0-d1/a0-a1

    bsr.b   _UIK_Font2TTA        ne touche pas à a1

    move.l  _GfxBase,a6
    jsr     _LVOOpenFont(a6)

    move.l  4.w,a6
    jsr     _LVOPermit(a6)

    add.w   #tta_SIZEOF,sp
    move.l  (sp)+,a6
    tst.l   d0
    rts


*************************************
*   void UIK_Font2TTA( font, tta )     ne touche pas à a1
*   d0                  a1   a0
*************************************

_UIK_Font2TTA
    move.l  LN_NAME(a1),tta_Name(a0)
    move.l  tf_YSize(a1),tta_YSize(a0)

    move.l  tf_Extension(a1),d0
    beq.b   .clear
    exg     d0,a1
    move.l  tfe_Tags(a1),tta_Tags(a0)
    exg     d0,a1
    beq.b   .clear
    bset    #FSB_TAGGED,tta_Style(a0)
    rts
.clear
    bclr    #FSB_TAGGED,tta_Style(a0)
    rts

;   screen.c..
;   157         ta->ta_Name = sobj->SFont->tf_Message.mn_Node.ln_Name;
;   158         *((ULONG*)&ta->ta_YSize) = *((ULONG*)&sobj->SFont->tf_YSize);


*************************************
*   ptr = UIK_BaseName( filename )
*   D0                     a0
*************************************

_UIK_BaseName
    move.l  a0,a1
.loop
    move.b  (a1)+,d0
    beq.b   .end
    cmp.b   #':',d0
    beq.b   .found
    cmp.b   #'/',d0
    bne.b   .loop
.found
    move.l  a1,a0
    bra.b   .loop
.end
    move.l  a0,d0
    rts


*************************************
*   obj = UIK_IsActiveKeyTank( obj ) window ou requester
*   D0                         a0
*************************************

_UIK_IsActiveKeyTank
    move.l  uo_UIK(a0),a1
    move.l  uike_ActiveKeyTank(a1),d0
    beq.s   .no
    move.l  d0,a1

    move.l  uo_rq(a0),d1
    beq.s   .win
    cmp.l   uo_rq(a1),d1
    beq.s   .ok
.win
    move.l  uo_w(a0),d1
    beq.s   .no
    cmp.l   uo_w(a1),d1
    beq.s   .ok
.no
    moveq   #0,d0
    rts
.ok
    move.l  a1,d0
    rts


*****************************************
*                                       *
*   APTR MakeUIKTree()                  *
*                                       *
*   reentrant function                  *
*                                       *
*   return NULL if fail                 *
*                                       *
*****************************************

    ; a0 : struct TreeItem[] *
    ; a1 : struct UIK *  (first parent) struct UIKObj *
    ; a2 : struct Vars * (or NULL if absolute location)
    ; a6 : uikbase *

_UIK_MakeUIKTree:
    movem.l d2/d3/a3,-(sp)

    moveq   #0,d0
    move.l  a1,d2               ; struct UIKObj * (current parent)
    ;beq     .End

    move.w  tri_Level(a0),d0
    movea.l a0,a3               ; struct TreeItem *
    moveq   #0,d3               ; d3 = last object added  (??)

.Loop:
    cmp.w   tri_Level(a3),d0
    bne.w   .LevelChange        ; changement de niveau

    move.l  tri_Before(a3),d0
    beq.s   .NoFunction

    ; parametres dans les registres pour asm
    movea.l a3,a0               ; struct TreeItem *a0

    ; dans la pile pour c
    move.l  a3,-(sp)

    movea.l d0,a1
    jsr     (a1)                ; bool=func(APTR)

    addq.w  #4,sp

    tst.l   d0
    beq.w   .End

.NoFunction:
    move.l  d2,d0               ; struct UIKobj *d0 (current parent object)
    movea.l tri_ObjName(a3),a0  ; UBYTE *
    movea.l tri_TagList(a3),a1  ; struct TagItem * (taglist)
    bsr     _UIK_AddObject
    move.l  d0,d3               ; d3=last object added
    beq     .End

    move.l  tri_Var(a3),d0
    cmpi.l  #NOSTORE,d0
    beq.s   .NoStore
    move.l  d3,(a2,d0.l)

.NoStore:
    move.l  tri_After(a3),d0
    beq.s   .NoFunction2

    ; parametres dans les registres pour asm
    movea.l d3,a0               ; struct UIKObj *a1
    movea.l a3,a1               ; struct TreeItem *a0

    ; dans la pile pour c
    movem.l d3/a3,-(sp)

    movea.l d0,a2
    jsr     (a2)                ; ulong=func(UIKObj,TreeItem)

    addq.w  #8,sp

.NoFunction2:
    movem.l d2/a2,-(sp)

    move.l  d0,d2

    move.l  tri_Vectors(a3),d0
    beq.s   .NoVectors

    movea.l d0,a2               ; struct HookItem *a2

.Loop2:
    tst.l   hi_Vector(a2)       ; test a placer en fin de boucle ??
    beq.s   .Finish

    movea.l d3,a0               ; struct UIKObj *a0
    movea.l hi_Vector(a2),a1    ; vector
    move.l  hi_Events(a2),d0    ; mask
    move.l  d2,d1               ; parm

    bsr     _UIK_AddVector      ; struct UIKHook * = AddVector
    tst.l   d0
    bne.s   .ok
    movem.l (sp)+,d2/a2
    bra.s   .End
.ok
    lea     hi_SIZE(a2),a2      ; next HookItem structure
    bra.s   .Loop2

.Finish:
.NoVectors:
    movem.l (sp)+,d2/a2

    move.w  tri_Level(a3),d0
    lea     tri_SIZE(a3),a3     ; next TreeItem structure
    bra.w   .Loop

.LevelChange:
    bgt.s   .BackLevel          ; d0>Level (come back)

    ; Level>d0
    move.w  d0,-(sp)

    movea.l a3,a0               ; struct TreeItem *
    movea.l d3,a1               ; struct UIKObj *  (parent object)
    bsr.w   _UIK_MakeUIKTree    ; a2=vars / a6=uikbase
    move.w  (sp)+,d1
    tst.l   d0
    beq.s   .End

    movea.l d0,a3               ; struct TreeItem *
    move.w  d1,d0
    bra.w   .Loop

.BackLevel:
    move.l  a3,d0

.End:
    movem.l (sp)+,d2/d3/a3
    rts


****************************************************************
*
*       Preferences
*
****************************************************************/

***********************************************************
*   ret = UIK_AccessPrefs( func, parm1, parm2 )
*                          a0    a1     d1
*   will do:
*   ret = func( parm1, parm2, UIKBase, UIKPrefs, ExecBase )
*               a1     d1     a2       a3        a6
*   all registers are available inside func() code
***********************************************************

_UIK_AccessPrefs
        movem.l d1-d7/a0-a3/a5-a6,-(sp)
        move.l  _UIKBase,a2
        lea     lb_PrefsSemaphore(a2),a0
        move.l  4.w,a6
        jsr     _LVOObtainSemaphore(a6)

        move.l  lb_UIKPrefs(a2),a3
        move.l  (sp),d1
        movem.l 28(sp),a0-a1
        jsr     (a0)

        move.l  d0,-(sp)
        move.l  _UIKBase,a2
        lea     lb_PrefsSemaphore(a2),a0
        move.l  4.w,a6
        jsr     _LVOReleaseSemaphore(a6)
        movem.l (sp)+,d0/d1-d7/a0-a3/a5-a6        d0: restore code retour
        tst.l   d0
        rts


***********************************************************
*   UIK_GetPrefs( buf, len)   copy prefs vers buffer
*                 a1   d1
***********************************************************

_UIK_GetPrefs
        lea     .getprefs(pc),a0
        bra.b   _UIK_AccessPrefs
.getprefs
        move.l  a3,a0
        bra.b   doprefscopy     src=prefs, dest=buf, d1=len

***********************************************************
*   UIK_SetPrefs( buf, len)   copy buffer vers prefs
*                 a1   d1
***********************************************************

_UIK_SetPrefs
        lea     .setprefs(pc),a0
        bsr.b   _UIK_AccessPrefs
        move.l  lb_UIKPrefs(a6),a0
        lea     uikp_Mask(a0),a1
        lea     uikp_Colors(a0),a0
        bsr     _IntUIK_DoMask
        rts

.setprefs
        move.l  a1,a0
        move.l  a3,a1           src=buf, dest=prefs, d1=len

doprefscopy
        move.l  d1,d0           len
        ;movem.l d0-d1/a0-a1,-(sp)
        ;XREF    _textvars
        ;bsr     _textvars
        ;movem.l (sp)+,d0-d1/a0-a1
        jmp     _LVOCopyMem(a6)


***********************************************************
*   length = UIK_ReadPrefs()
*
***********************************************************

        XREF    _doprefsfile

_UIK_ReadPrefs
    moveq   #0,d1
    bra.b   common_read


***********************************************************
*   length = UIK_ReadArcPrefs()
*
***********************************************************

_UIK_ReadArcPrefs
    moveq   #1,d1
common_read
    bsr     _UIK_DisableDOSReq
    move.l  d0,-(sp)

    lea     _doprefsfile,a0
    move.l  #MODE_OLDFILE,a1
    bsr     _UIK_AccessPrefs
    move.l  d0,d1

    move.l  (sp)+,d0
    bsr     _UIK_EnableDOSReq

    move.l  d1,d0   return( size );
    rts


***********************************************************
*   length = UIK_SaveEnvPrefs()
*
***********************************************************

_UIK_SaveEnvPrefs
    moveq   #0,d1
    bra.b   common_save

***********************************************************
*   length = UIK_SavePrefs()
*
***********************************************************

_UIK_SavePrefs
    moveq   #1,d1
common_save
    lea     _doprefsfile,a0
    move.l  #MODE_NEWFILE,a1
    bsr     _UIK_AccessPrefs
    rts


***********************************************************
* struct UIKGlobal *UIK_GetTaskUIK( struct Task *task );
*                                                A0
***********************************************************

_UIK_GetTaskUIK
    move.l  a6,-(sp)
    move.l  4.w,a6
    FORBID
    ;jsr     _LVOForbid(a6)

    moveq   #0,d0
    move.l  _UIKBase,a1
    lea     lb_TaskList(a1),a1
    move.l  (a1),d1
.loop
    move.l  d1,a1
    move.l  (a1),d1
    beq.b   .end
    cmp.l   8(a1),a0
    bne.b   .loop
    move.l  12(a1),d0
.end
    jsr     _LVOPermit(a6)
    move.l  (sp)+,a6
    rts

    ;moveq   #0,d0
    ;move.l  TC_Userdata(a0),d1      DestUIK = task->UserData?
    ;beq.b   .end
    ;btst    #0,d1                   Adresse paire?
    ;bne.b   .end
    ;move.l  d1,a1                   a1=DestUIK
    ;cmp.l   uo_UIK(a1),d1           UIK->UIK == UIK?
    ;bne.b   .end                    si non: pas UIK
    ;move.l  d1,d0

***********************************************************
* so = parent_screen( struct UIKObj *obj );
*                                    A0
***********************************************************

_parent_screen
    move.l  uo_Parent(a0),d0
    beq.b   .end
    move.l  d0,a0
    cmp.w   #UIKID_SCREEN,uo_Id(a0)
    bne.b   _parent_screen
.end
    rts

***********************************************************
* so = parent_window( struct UIKObj *obj );
*                                    A0
***********************************************************

_parent_window
    move.l  uo_Parent(a0),d0
    beq.b   .end
    move.l  d0,a0
    cmp.w   #UIKID_SCREEN,uo_Id(a0)
    beq.b   .no
    cmp.w   #UIKID_WINDOW,uo_Id(a0)
    bne.b   _parent_window
.end
    rts
.no
    moveq   #0,d0
    rts

***********************************************************
* rc = do_process_cmd( struct IORequest *req );
*                                        A0
***********************************************************

_do_process_cmd
    movem.l d2-d7/a2-a6,-(sp)

    move.l  _UIKBase,a6
    move.l  a6,a1
    sub.w   IO_COMMAND(a0),a1       a1 = LVO(a6)

;  moveq     #0,d0
;  move.w    IO_COMMAND(a0),d0
;  DBUG      <'offs=0x%04.4lx, req=0x%08.8lx',10>,d0,a0

    pea     .ret(pc)                adresse de retour
    move.l  a1,-(sp)                adresse de saut

    move.l  IO_DEVICE(a0),a0        a0 = uike_ArgRes
    movem.l (a0),d0-d7/a0-a5        a6 = _UIKBase
    rts                             saut
.ret
    movem.l (sp)+,d2-d7/a2-a6
    rts


***********************************************************
* do_process_warn( struct IORequest *req );
*                                    A0
***********************************************************

_do_process_warn
    move.l  IO_UNIT(a0),d1          d1 = obj
    beq.b   .end
    move.l  d1,a1                   a1 = obj
    move.l  uo_ObjExt(a1),a1        a1 = ObjExt
    move.l  uoe_IPRemoveFunc(a1),d0
    beq.b   .end
    move.l  d0,a1                   a1 = IPRemoveFunc

    move.l  d1,-(sp)                obj
    move.l  MN_REPLYPORT(a0),a0
    move.l  MP_SIGTASK(a0),-(sp)    task
    jsr     (a1)                    warnfunc( task, obj );
    addq.w  #8,sp
.end
    rts


***********************************************************
* do_send_warnremove( struct UIKObj *obj );
*                                    A0
***********************************************************

_do_send_warnremove
    move.l  uo_ObjExt(a0),a1        a1 = ObjExt
    move.l  uoe_IPMother(a1),d0
    bne.b   .send
    rts
.send
    movem.l a2-a6,-(sp)
    move.l  4.w,a6
    sub.w   #MP_SIZE,sp
    move.l  sp,a4                       a4 = port
    move.l  uo_UIK(a0),a2               a2 = UIK
    move.l  d0,a3                       a3 = task
    move.l  a0,uike_CmdReq+IO_UNIT(a2)  req->io_Unit = obj

    move.b  #NT_MSGPORT,LN_TYPE(a4)
    move.b  #PA_SIGNAL,MP_FLAGS(a4)
    move.l  uike_Task(a2),MP_SIGTASK(a4)
    lea     MP_MSGLIST(a4),a1
    NEWLIST a1
    move.b  #SIGB_SINGLE,MP_SIGBIT(a4)

    FORBID

    ;move.l  TC_Userdata(a3),d1      DestUIK = task->UserData?
    ;beq.b   .end
    ;btst    #0,d1                   Adresse paire?
    ;bne.b   .end
    ;move.l  d1,a5                   a5=DestUIK
    ;cmp.l   uo_UIK(a5),d1           UIK->UIK == UIK?
    ;bne.b   .end                    si non: pas UIK

    move.l  a3,a0
    bsr     _UIK_GetTaskUIK
    tst.l   d0
    beq.b   .end
    move.l  d0,a5
    tst.b   uo_Status(a5)           UIK->Status == STARTED?
    bne.b   .end                    si non: rien a faire avec

    moveq   #0,d0
    move.l  #SIGF_SINGLE,d1
    jsr     _LVOSetSignal(a6)

    lea     uike_CmdReq(a2),a1
    move.l  a4,MN_REPLYPORT(a1)
    move.w  #$8000,IO_COMMAND(a1)
    lea     uike_CmdPort(a5),a0     port distant
    jsr     _LVOPutMsg(a6)
    move.l  a4,a0
    jsr     _LVOWaitPort(a6)
    move.l  a4,a0
    jsr     _LVOGetMsg(a6)

.end
    jsr     _LVOPermit(a6)

    add.w   #MP_SIZE,sp
    movem.l (sp)+,a2-a6
    rts


        END
