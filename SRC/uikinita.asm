    OPT O+,W-
        OPT d+
****************************************************************
*
*       Project:    UIK
*       Function:   init UIK
*
*       Created:    10/05/90    Jean-Michel Forgeas
*
****************************************************************

        IDNT    uikinita


******* Includes ************************************************

        INCDIR  "include:"

        INCLUDE "lib/exec_lib.i"
        INCLUDE "lib/dos_lib.i"
        INCLUDE "lib/locale_lib.i"
        INCLUDE "exec/tasks.i"
        INCLUDE "exec/execbase.i"
        INCLUDE "exec/memory.i"
        INCLUDE "libraries/dos.i"
        INCLUDE "libraries/dosextens.i"
        INCLUDE "intuition/intuition.i"
        INCLUDE "uiki:uikbase.i"
        INCLUDE "uiki:uikobj.i"
        INCLUDE "uiki:uikgfx.i"
        INCLUDE "uiki:uikmacros.i"
        INCLUDE "uiki:uikglobal.i"
        INCLUDE "uiki:objwindow.i"


******* Imported ************************************************

        XREF    _InitUIK
        XREF    _CleanupUIK


        XREF    _UIK_Input_Set
        XREF    _UIK_Input_Flush
        XREF    _UIK_Input_SetTime
        XREF    _Input_UninstallTask
        XREF    _IntUIK_PushBox
        XREF    _IntUIK_PopBox
        XREF    _UIK_LockObjVars
        XREF    _UIK_UnlockObjVars
        XREF    _UIK_ClipObject
        XREF    _UIK_UnclipObject
        XREF    _UIK_DeadKeyConvert
        XREF    _UIK_MemSpAlloc
        XREF    _UIK_MemSpFree
        XREF    _UIK_AllocBMapRPort
        XREF    _UIK_FreeBMapRPort
        XREF    _UIK_InactivateKeyTank

        XREF    _zo_UnmarkZone

        ;XREF    _TRUE_Stop
        XREF    _TRUE_Remove
        XREF    _FALSE_Remove

        XREF    _DOSBase
        XREF    _UIKBase
        XREF    _LocaleBase
        XREF    _intena

        XREF    _UIKPrefsFileName

        XREF    _AddTaskList
        XREF    _RemoveTaskList
        XREF    _menukey_create_msg


******* Exported ************************************************

        XDEF    _UIK_Install
        XDEF    _UIK_RemoveAll
        XDEF    _UIK_Enable
        XDEF    _UIK_Disable

        XDEF    _IntUIK_OpenLocale
        XDEF    _IntUIK_CloseLocale

        XDEF    makeport
        XDEF    newlist


****************************************************************
*
*       Macros
*
****************************************************************/

;DISABLE MACRO
;        move.w  #$4000,_intena
;        addq.b  #1,IDNestCnt(a6)
;        ENDM
;
;ENABLE  MACRO
;        subq.b  #1,IDNestCnt(a6)
;        bge.s   .ENABLE\@
;        move.w  #$C000,_intena
;.ENABLE\@:
;        ENDM


****************************************************************
*
*       Routines
*
****************************************************************/

        SECTION text,CODE


****************************************************************
*
*       UIKGlobal
*
****************************************************************/

makeport
    move.b  #NT_MSGPORT,LN_TYPE(a0)
    move.b  #PA_SIGNAL,MP_FLAGS(a0)
    lea     MP_SIGTASK(a0),a0
    move.l  a1,(a0)+            a0 = MP_MSGLIST
newlist
    NEWLIST a0                  a1 preserved
    rts


makesignal
    moveq   #-1,d0
    move.l  a0,-(sp)
    jsr     _LVOAllocSignal(a6)         alloc a signal
    move.l  (sp)+,a0
    cmp.b   #-1,d0
    beq.b   .end
    move.b  d0,(a0)
    addq.l  #4,a0
    moveq   #0,d1
    bset    d0,d1
    move.l  d1,d0
.end
    rts

***********************************************************
*   UIK = UIK_Install( english, other )
*   d0                 a0       a1
***********************************************************

_UIK_Install
REGOFFS equ 4*10
    movem.l d0/d2-d4/a0-a3/a5-a6,-(sp)
    ;move.l  _UIKBase,a2
    move.l  a6,a2                       a2 = UIKBase
    move.l  lb_UIKPrefs(a2),a3          a3 = UIKPrefs
    move.l  4.w,a6

    move.l  #UIKGLOBAL_SIZE+UIKOBJEXT_SIZE+ula_SIZEOF+SS_SIZE,d0
    move.l  #MEMF_PUBLIC!MEMF_CLEAR,d1
    bsr     _UIK_MemSpAlloc
    tst.l   d0
    beq     insterror
    move.l  d0,a5                       a5 = UIK global datas
    suba.l  a1,a1
    jsr     _LVOFindTask(a6)
    move.l  d0,uike_Task(a5)            remember this task
    move.l  d0,a0

    moveq   #5,d0
    move.l  d0,uo_List+uikl_BlockSize(a5) init blocksize to 5
    move.l  d0,uo_Hook+uikl_BlockSize(a5)

    move.l  a5,uo_UIK(a5)               for object compatibility
    move.b  #UIKS_STOPPED,uo_Status(a5)
    moveq   #22,d0
    move.w  d0,uo_XAspect(a5)           for object default aspect
    move.w  d0,uo_YAspect(a5)           for object default aspect
    move.b  #1,uo_AutoRatio(a5)         for object default ratio

    ;move.l  #UIKOBJEXT_SIZE,d0          for object extension
    ;move.l  #MEMF_PUBLIC!MEMF_CLEAR,d1
    ;bsr     _UIK_MemSpAlloc
    ;move.l  d0,uo_ObjExt(a5)
    ;beq     extension_error

    ;move.l  #ula_SIZEOF,d0              for object based langage
    ;move.l  #MEMF_PUBLIC!MEMF_CLEAR,d1
    ;bsr     _UIK_MemSpAlloc
    ;move.l  uo_ObjExt(a5),a0
    ;move.l  d0,uoe_Lang(a0)
    ;beq     sigerror

    lea     UIKGLOBAL_SIZE(a5),a0
    move.l  a0,uo_ObjExt(a5)
    lea     UIKGLOBAL_SIZE+UIKOBJEXT_SIZE(a5),a0
    move.l  a0,UIKGLOBAL_SIZE+uoe_Lang(a5)
    lea     UIKGLOBAL_SIZE+UIKOBJEXT_SIZE+ula_SIZEOF(a5),a0
    move.l  a0,uo_SigSemaphore(a5)
    jsr     _LVOInitSemaphore(a6)

    lea     uike_SigBit(a5),a0              signal pour recevoir Intuition
    bsr     makesignal
    beq     sigerror
    move.l  d0,(a0)

    lea     uike_ActiveSigBit(a5),a0
    bsr     makesignal
    beq     sig0error
    move.l  d0,uike_ActiveMask(a5)          utilisé pour input joystick

    lea     uike_CmdPort+MP_SIGBIT(a5),a0   pour communication inter-process
    bsr     makesignal
    beq     sig1error
    move.l  d0,uike_CmdMask(a5)
    lea     uike_CmdReq(a5),a1
    lea     uike_ArgRegs(a5),a0
    move.l  a0,IO_DEVICE(a1)
    move.b  #NT_MESSAGE,LN_TYPE(a1)
    lea     uike_CmdPort(a5),a0
    ;move.l  a0,MN_REPLYPORT(a1)
    move.l  uike_Task(a5),a1
    bsr     makeport

    move.l  a5,a0
    bsr     _UIK_Disable                    disable all sigexcepts

    lea     uike_UPort+MP_SIGBIT(a5),a0
    bsr     makesignal
    beq     sig2error
    move.l  d0,uike_ExcMask(a5)

    ;------------------------- pfaction version
    move.l  a5,uike_UPort+LN_NAME(a5)
    move.b  #NT_MSGPORT,uike_UPort+LN_TYPE(a5)      create a user port
    move.b  #PF_ACTION,uike_UPort+MP_FLAGS(a5)
    move.l  #UIK_ExceptionHandler,uike_UPort+MP_SIGTASK(a5)
    lea     uike_UPort+MP_MSGLIST(a5),a0
    bsr     newlist
    ;----------------------------------

    lea     uike_IMSList(a5),a0               task list
    bsr     newlist

    ;------ Langage
;    move.l  16(sp),uike_LangEnglish(a5)
;    move.l  20(sp),uike_LangOther(a5)
;
;    move.l  uike_LangEnglish(a5),d0
;    btst    #UIKLANGB,uikp_PrefsFlags+1(a3)
;    beq.b   .setlang
;    move.l  uike_LangOther(a5),d0
;.setlang
;    move.l  d0,uike_Langage(a5)

    move.l  16(sp),UIKGLOBAL_SIZE+UIKOBJEXT_SIZE+ula_English(a5)
    move.l  20(sp),UIKGLOBAL_SIZE+UIKOBJEXT_SIZE+ula_Other(a5)

    move.l  UIKGLOBAL_SIZE+UIKOBJEXT_SIZE+ula_English(a5),d0
    btst    #UIKLANGB,uikp_PrefsFlags+1(a3)
    beq.b   .setlang
    move.l  UIKGLOBAL_SIZE+UIKOBJEXT_SIZE+ula_Other(a5),d0
.setlang
    move.l  d0,UIKGLOBAL_SIZE+UIKOBJEXT_SIZE+ula_Langage(a5)
    move.l  a5,UIKGLOBAL_SIZE+UIKOBJEXT_SIZE+ula_Master(a5)

    ;------ alloc zone rastport h
    moveq   #UIKBMAP_NOPLANE,d0
    move.l  d0,a1
    moveq   #1,d0
    move.l  d0,d1
    move.l  d0,a0
    bsr     _UIK_AllocBMapRPort
    tst.l   d0
    beq     rperror
    move.l  d0,uike_ZoneRPortH(a5)
    ;------ alloc zone rastport w
    moveq   #UIKBMAP_NOPLANE,d0
    move.l  d0,a1
    moveq   #1,d0
    move.l  d0,d1
    move.l  d0,a0
    bsr     _UIK_AllocBMapRPort
    tst.l   d0
    beq     rperror
    move.l  d0,uike_ZoneRPortW(a5)

    move.l  a5,a0
    bsr     _InitUIK
    tst.l   d0
    beq     initerror

    ;------ Place private func vectors
    movem.w uikp_ZoneSpeed(a3),d0-d1/a0
    move.l  a5,a1
    bsr     _UIK_Input_SetTime
    move.l  a5,a1
    bsr     _UIK_Input_Flush

    ;------ Install input mode
    ;move.b  uikp_Colors+UIKCOL_ZONE(a3),uike_ZoneColor(a5)
    move.l  a5,a0
    move.b  uikp_InputAuto(a3),d0
    move.w  d0,a1
    move.b  uikp_InputMode(a3),d0
    move.b  uikp_InputHeld(a3),d1
    bsr     _UIK_Input_Set
    move.w  uikp_InputFlags(a3),uike_InputFlags(a5)

    ;------ Place private func vectors
    ;lea     _TRUE_Stop,a0
    ;move.l  a0,uike_StopVector(a5)
    lea     _TRUE_Remove,a0
    move.l  a0,uike_RemoveVector(a5)

    ;move.l  uike_Task(a5),a0
    ;tst.l   TC_Userdata(a0)
    ;bne.b   .noset
    ;move.l  a5,TC_Userdata(a0)          task->UserData = UIK
    move.l  uike_Task(a5),a0
    move.l  a5,a1
    bsr     _AddTaskList                ( a0 task, a1 uik )

    move.l  a5,a0
    bsr     _UIK_Enable                 enable handler

    move.l  a5,(sp)                     return datas adress
    movem.l (sp)+,d0/d2-d4/a0-a3/a5-a6
    rts

*************************************
*   UIK_RemoveAll( UIK )
*                  d0
*************************************

_UIK_RemoveAll
    tst.l   d0
    beq     endRemoveAll
RREGOFFS equ 4*10
    movem.l d0/d2-d4/a0-a3/a5-a6,-(sp)  doit avoir les mêmes registre que Install()
    move.l  d0,a5                       a5 = datas
    move.l  a6,a2                       a2 = UIKBase
    move.l  4.w,a6                      a6 = ExecBase

    move.l  a5,a1
    bsr     _Input_UninstallTask

    move.l  a5,a0
    bsr     _UIK_Disable                disable all sigexcepts

    ;move.l  uike_Task(a5),a0
    ;cmp.l   TC_Userdata(a0),a5
    ;bne.b   .repmsg
    ;clr.l   TC_Userdata(a0)             task->UserData = 0
    move.l  uike_Task(a5),a0
    bsr     _RemoveTaskList             ( a0 task )

.repmsg
    lea     uike_CmdPort(a5),a0
    jsr     _LVOGetMsg(a6)
    tst.l   d0
    beq.b   .nomsg
    move.l  d0,a1
    jsr     _LVOReplyMsg(a6)
    bra.b   .repmsg

.nomsg
    move.l  a5,a0
    bsr     _CleanupUIK

initerror
    move.l  uike_ZoneRPortW(a5),a0
    bsr     _UIK_FreeBMapRPort
    move.l  uike_ZonePlanesW(a5),a1
    bsr     _UIK_MemSpFree
    move.l  uike_ZoneRPortH(a5),a0
    bsr     _UIK_FreeBMapRPort
    move.l  uike_ZonePlanesH(a5),a1
    bsr     _UIK_MemSpFree

rperror
    ;------ Free signals
    move.b  uike_UPort+MP_SIGBIT(a5),d0
    jsr     _LVOFreeSignal(a6)          free user port signal

sig2error
    move.b  uike_CmdPort+MP_SIGBIT(a5),d0
    jsr     _LVOFreeSignal(a6)          free signal
sig1error
    move.b  uike_ActiveSigBit(a5),d0
    jsr     _LVOFreeSignal(a6)          free signal
sig0error
    move.b  uike_SigBit(a5),d0
    jsr     _LVOFreeSignal(a6)          free signal
sigerror
;    move.l  uo_ObjExt(a5),a3
;    move.l  uoe_Lang(a3),a1
;    bsr     _UIK_MemSpFree
;    move.l  a3,a1
;    bsr     _UIK_MemSpFree
extension_error
    move.l  a5,a1
    bsr     _UIK_MemSpFree              free datas

insterror
    movem.l (sp)+,d0/d2-d4/a0-a3/a5-a6
endRemoveAll
    moveq   #0,d0                       for Install() zero ret code
    rts

_IntUIK_OpenLocale        ;  locale = IntUIK_OpenLocale()
    move.l  _LocaleBase,d0
    beq.b   .end
    move.l  a6,-(sp)
    move.l  d0,a6
    sub.l   a0,a0
    jsr     _LVOOpenLocale(a6)
    move.l  (sp)+,a6
.end rts

_IntUIK_CloseLocale       ;  IntUIK_CloseLocale( locale ) (A0)
    move.l  a0,d0
    beq.b   .end
    move.l  _LocaleBase,d0
    beq.b   .end
    move.l  a6,-(sp)
    move.l  d0,a6
    jsr     _LVOCloseLocale(a6)
    move.l  (sp)+,a6
.end rts

;_langage_num        ;( register __a0 UBYTE **tab )
;    move.l  a0,d0
;    beq.b   .end
;    moveq   #0,d0
;.loop
;    move.l  (a0)+,d1
;    beq.b   .end
;    addq.l  #1,d0
;    bra.b   .loop
;.end
;    rts


*************************************
*   oldsigs = UIK_Disable( uik )
*                          a0
*************************************

_UIK_Disable
    addq.b  #1,uike_Exable(a0)
    rts


*************************************
*   UIK_Enable( uik )
*               a0
*************************************

_UIK_Enable
    subq.b  #1,uike_Exable(a0)
;    bne.b   .10
;    DISABLE
;    lea     uike_UPort(a0),a1
;    bsr.b   UIK_ExceptionHandler
;    ENABLE
;.10
    rts


*************************************
*   Global exception handler
*   a1=data
*************************************

MENUMSG     equ MENUVERIFY!MENUPICK!MENUHELP
GADGETMSG   equ GADGETDOWN!GADGETUP!MOUSEMOVE|CLOSEWINDOW
WINDOWMSG   equ ACTIVEWINDOW!INACTIVEWINDOW!MOUSEBUTTONS
;WANTEDMSG   equ (GADGETMSG!WINDOWMSG!RAWKEY)

UIK_ExceptionHandler
    movem.l d2-d3/a2/a3/a5,-(sp)           d2 used by exe_func
    move.l  LN_NAME(a1),a5              task's UIKDatas
    move.l  4.w,a6
    ENABLE
    tst.b   uike_Exable(a5)
    beq.b   .10
    move.l  uike_ExcMask(a5),d0         signaler la tache
    move.l  uike_Task(a5),a1
    jsr     _LVOSignal(a6)
    bra     return
.10
    ;move.l  a5,a0
    ;bsr     _UIK_LockObjVars

    lea     uike_UPort+MP_MSGLIST(a5),a2 a2 = header = &port->mp_MsgList
    move.l  (a2),a3                     a3 = lh_head pointer = msg
    lea     uike_IMSList(a5),a2         a2 = &IMSList

loop
        move.l  (a3),d0
        beq.b   end                     si plus de message -> fin

        move.l  im_Class(a3),d3
        move.l  d3,d0
        and.l   #WINDOWMSG,d0
        bne     do_window
        move.l  d3,d0
        and.l   #GADGETMSG,d0           on veut cette class ?
        bne     do_gadget               oui: -> traitement
        move.l  d3,d0
        and.l   #RAWKEY,d0
        bne     do_rawkey
        move.l  d3,d0
        and.l   #MENUMSG,d0
        bne     do_menus

attach  ; attacher le msg a l'autre liste pour la tache
        bsr     removemsg               REMOVE from Intuition port list
        move.l  a2,a0                   a0 = list, a1 = msg
        DISABLE
        jsr     _LVOAddTail(a6)         ADDTAIL to task list
        ENABLE
        bra.b   loop                    continue

end ;------ reste-t-il des messages attachés a la liste ?
    cmp.l   LH_TAILPRED(a2),a2
    beq.b   return                      non: la tache n'a rien a faire

    move.l  LH_TAILPRED(a2),a3                     a3 = head pointer
    moveq   #0,d3
    DISABLE
mmloop
        move.l  LN_PRED(a3),d0
        beq.b   sigtask                 si plus de message -> fin

        btst    #4,im_Class+3(a3)       MOUSEMOVE ?
        beq.b   .20

        tst.b   d3                      on en a déjà un ?
        beq.b   .10
        bsr     replymsg2               oui: on enleve celui-la
.20
        move.l  LN_PRED(a3),a3          on repart du dernier
        bra.b   mmloop                  continue
.10
        moveq   #1,d3                   non: alors maintenant oui
        bra.b   .20                     continue

sigtask
    ENABLE

    ;move.l  a5,a0
    ;bsr     _UIK_UnlockObjVars

    move.l  uike_SigMask(a5),d0         oui: prévenir la tache
    move.l  uike_Task(a5),a1            task
    jsr     _LVOSignal(a6)
return
    movem.l (sp)+,d2-d3/a2/a3/a5
    DISABLE
    rts


****** menus ******

do_menus
    ;------ Solution pour Intuition qui bloque toutes les fenêtres
    cmp.l   #MENUVERIFY,d3
    bne.s   .no
    move.l  im_IDCMPWindow(a3),a0
    move.l  wd_Flags(a0),d0
    and.l   #WFLG_WINDOWACTIVE,d0
    beq     replymsg
.no
    ;------ Est-ce un raccourcis clavier ?
    btst    #IEQUALIFIERB_RCOMMAND,im_Qualifier+1(a3)   si pas _RCOMMAND c'est un vrai menu
    bne.s   .shortcut
    bsr     inact_keytank                   vrai menu : désactive le réservoir de touches
    bra.s   .end

.shortcut
    ;------ Oui : raccourcis clavier
    move.l  uike_ActiveKeyTank(a5),d0       réservoir de touches actif ?
    beq.s   .end                            si non ok : on laisse l'appel des menus
    move.l  d0,a0
    cmp.w   #UIKID_STRGAD,uo_Id(a0)         objet UIKObj_String ?
    bne.s   .end                            si non ok : on laisse l'appel des menus
    cmp.l   #MENUVERIFY,d3
    beq.s   .sc_menuverify
    cmp.l   #MENUPICK,d3
    beq.s   .sc_menupick
.end
    bra     attach

    ;------ MENUVERIFY pour raccourcis clavier --> on ignore
.sc_menuverify
    bset    #UIKGB_DEADMENU,uike_Flags(a5)  le prochain RAWKEY UP sera doublé
    move.w  #MENUCANCEL,im_Code(a3)
    bra     replymsg

.sc_menupick
    bra     replymsg        on ignore ce message


****** rawkeys ******

do_rawkey
;    btst.b  #UIKGF_RAWCTRLC,uike_Flags(a5)      si 1 on est forcement sur un UIKObj_String
;    beq.s   .suite
;    btst.b  #IEQUALIFIERB_CONTROL,im_Qualifer+1(a3)
;    beq.s   .suite
;    move.w  im_Code(a3),d0
;    btst.b  #7,d0
;    bne.s   .suite
;    cmp.b   #$33,d0     touches CTRL+C
;    bne.s   .suite
;  DBUG <'CTRL+C',10>,D0,D0

.suite
    move.l  uike_ActiveKeyTank(a5),d0   reservoir de touches actif ?
    beq.s .end
    move.l  d0,a1

    ;------ Si le précédent était un MENUVERIFY on dédouble ce RAWKEY qui doit être théoriquement un UP
    bclr    #UIKGB_DEADMENU,uike_Flags(a5)      si 1 on est forcement sur un UIKObj_String
    beq.s   .normal
    move.l  a1,-(sp)
    move.l  a5,a0   à cause bug SASC link A5
    jsr     _menukey_create_msg  (a0,a1,a3)
    move.l  (sp)+,a1
    tst.l   d0
    beq.s   .normal
    move.l  d0,a3   le message créé est avant, on recommence par lui
    bra     loop    ;------------------------------------------>>>

    ;------ opérations normales
.normal
    tst.b   uo_Status(a1)           obj->Status == UIKS_STARTED ?
    bne     replymsg                si non: objet stopped ou removed
    btst    #(BOXB_DISABLED-16),uo_Flags+1(a1)
    bne     replymsg                si != 0, objet disabled
    move.l  uo_ExceptKeyVector(a1),d0   utilise son vecteur
    beq.b   .end                    pas de fonction

    move.l  a1,-(sp)   object String ou autre
    ;-------------
    move.l  _UIKBase,a0
    lea     lb_KeySupp(a0),a0
    move.l  a3,a1
    moveq   #0,d0
    bsr     _UIK_DeadKeyConvert
    beq.b   .re
    ;-------------
    move.l  (sp),a1
    move.l  uo_ExceptKeyVector(a1),a0
    moveq   #1,d2
    jsr     (a0)                    appel de la fonction objet
    move.l  (sp)+,a1
    tst.b   d2
    bne.b   .end
    bra     replymsg                l'objet cancel le message
.re
    move.l  (sp)+,a1
.end
    bra     attach

****** gadgets ******

do_gadget
.mmove
    cmp.l   #MOUSEMOVE,d3
    bne.b   .close
    move.l  uike_ActiveObj(a5),d0
    beq     replymsg                reply et continue sur loop
    move.l  d0,a1
    move.l  uo_ExceptMovVector(a1),a0
    bra.b   exe_func
.close
    cmp.l   #CLOSEWINDOW,d3
    bne.b   others
    move.l  uike_ActiveObj(a5),d0
    beq     attach                  attach to process list
    move.l  d0,a1
    btst    #(BOXB_DISABLED-16),uo_Flags+1(a1)
    bne     replymsg                si != 0, objet disabled
    bra.b   inactgadget
others
    move.l  im_IAddress(a3),a0      a0 = gadget
    move.l  gg_UserData(a0),a1      a1 = object
    bsr.b   valid_object            !!! ne doit pas modifier a0 !!!
    beq     replymsg                reply et continue sur loop
    btst    #(BOXB_DISABLED-16),uo_Flags+1(a1)
    bne     replymsg                si != 0, objet disabled
.gadown
    cmp.l   #GADGETDOWN,d3
    bne.b   .gadup
    move.l  a1,uike_ActiveObj(a5)
    bsr     gad_inact_keytank
    move.l  uike_ActiveObj(a5),d0
    beq     replymsg                objet n'est plus valide... reply et continue sur loop
    move.l  d0,a1   restore
    move.l  uo_ExceptActVector(a1),a0
    bra.b   exe_func
.gadup
    cmp.l   #GADGETUP,d3
    bne     attach
inactgadget
    clr.l   uike_ActiveObj(a5)
    move.l  uo_ExceptInaVector(a1),a0

exe_func
    tst.b   uo_Status(a1)           obj->Status == UIKS_STARTED ?
    bne.b   replymsg                si non: objet stopped ou removed
    btst    #(BOXB_DISABLED-16),uo_Flags+1(a1)
    bne     replymsg                si != 0, objet disabled
    move.l  a0,d0
    beq.b   .re                     pas de fonction
    moveq   #1,d2
    move.l  a1,-(sp)   object       ok
    jsr     (a0)                    appel de la fonction objet
    move.l  (sp)+,a1
    tst.b   d2
    beq.b   replymsg                l'objet cancel le message
.re

taskhook ;------ si l'objet n'a pas d'action, la tâche n'a pas a être signalée
    ;move.l  uo_Hook+uikl_NodeNum(a1),d0
    ;or.l    uo_Link+uikl_NodeNum(a1),d0
    ;bne     attach
    ;bra.b   replymsg                pas de fonction: reply et continue sur loop
    bra     attach

****** général ******

removemsg
    move.l  a3,a1                   a1 = message
    ;DISABLE
    move.l  (a1),a3                 a3 = node suivant (car Remove apres)
    move.l  LN_PRED(a1),a0          REMOVE
    move.l  a3,(a0)                 .
    move.l  a0,LN_PRED(a3)          .
    ;ENABLE
    rts                             a1 = message, a3 = nextmsg

valid_object ;!!!!!!!!!!NE DOIT PAS MODIFIER A0!!!!!!!!!!!!
    move.l  a1,d0                   UserData == 0 ?
    beq.b   .end                    si oui: pas un objet UIK
    btst    #0,d0                   valeur impaire ?
    bne.b   .no                     si oui: pas un objet UIK
    cmpa.l  uo_UIK(a1),a5           Obj->UIK == bonne valeur ?
    bne.b   .no                     si non: pas un objet UIK
    tst.b   uo_Status(a1)           Obj->Status == STARTED ?
    beq.b   .set                    si non: rien a faire avec
.no
    moveq   #0,d0
.set
    tst.l   d0
.end
    rts

replymsg
    pea     loop(pc)                retour à loop aprés jmp
replymsg2
    tst.l   MN_REPLYPORT(a3)
    bne.s   .real_reply
    move.l  a3,a1
    jmp     _UIK_MemSpFree          puis free   (replyport = 0 --> alloué pour remplacé MENUPICK)
.real_reply
    bsr.b   removemsg
    jmp     _LVOReplyMsg(a6)        puis REPLY  (a1 = message)


****** window ******

do_window
    move.l  im_IDCMPWindow(a3),a0   a0 = window
    move.l  wd_UserData(a0),a1      a1 = object
    bsr.b   valid_object
    beq.b   replymsg

    btst    #3,im_Class+3(a3)       MOUSEBUTTONS bit
    beq.b   .actinact
    btst    #(BOXB_DISABLED-16),uo_Flags+1(a1)
    bne     replymsg                si != 0, objet disabled

.buttons
    btst    #IECODEB_UP_PREFIX,im_Code+1(a3)
    bne.b   .up
.down
    move.l  a1,uike_ActiveObj(a5)
    bsr.b   inact_keytank
    bra.b   taskhook
.up
    move.l  uike_ActiveObj(a5),d0   y a-t-il un objet actif ?
    beq.b   .upok
    exg     d0,a1                   a1 est l'objet actif
    cmp.l   d0,a1                   est-ce le meme qui est desactivé? (fenetre)
    bne     inactgadget             si non, gadget ou requester
.upok
    moveq   #0,d0
    move.l  d0,uike_ActiveObj(a5)
    bra     taskhook

.actinact
    move.l  a1,-(sp)
    lea     _IntUIK_PushBox(pc),a0
    cmp.l   #ACTIVEWINDOW,d3
    beq.b   .jsr
    bsr.b   inact_keytank
    lea     _IntUIK_PopBox(pc),a0
.jsr
    move.l  a0,-(sp)

    move.l  a5,a0
    bsr     _zo_UnmarkZone
    clr.l   uike_ActiveZoneList(a5)
    movem.l (sp),a0-a1              restore a0 et a1

    move.l  uow_TitleBox(a1),d1
    beq.b   .notitle
    move.l  d1,-(sp)                obj dans la pile
    jsr     (a0)
    addq.w  #4,sp

.notitle
    movem.l (sp)+,a0-a1
    btst    #(BOXB_DISABLED-16),uo_Flags+1(a1)
    bne     replymsg                si != 0, objet disabled
    bra     attach                  le process en a besoin dans tous les cas.


gad_inact_keytank
    move.l  uike_ActiveKeyTank(a5),d0
    beq.b   .end            un keytank actif ?
    cmp.l   a1,d0
    bne.s   do_inact
.end
    rts

inact_keytank
    move.l  uike_ActiveKeyTank(a5),d0
    bne.s   do_inact            un keytank actif ?
    rts

do_inact
    move.l  d0,a0
    move.l  uike_RemoveVector(a5),-(sp)
    move.l  #_FALSE_Remove,uike_RemoveVector(a5)
    jsr     _UIK_InactivateKeyTank
    move.l  (sp)+,uike_RemoveVector(a5)
    rts


****************************************************************
*
*       Datas
*
****************************************************************/

        END


