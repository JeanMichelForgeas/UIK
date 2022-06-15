    OPT O+,W-
        OPT d+
****************************************************************
*
*       Project:    UIK
*       Function:   envoi de commandes à un autre UIK
*
*       Created:    19/05/93    Jean-Michel Forgeas
*
****************************************************************

        IDNT    uikipa


******* Includes ************************************************

        INCDIR  "i:"

        INCLUDE "exec/ables.i"
        INCLUDE "lib/exec_lib.i"
        INCLUDE "exec/tasks.i"
        INCLUDE "exec/execbase.i"
        INCLUDE "uiki:uikbase.i"
        INCLUDE "uiki:uikglobal.i"
        INCLUDE "uiki:uikmacros.i"
        INCLUDE "uiki:uik_lib.i"


******* Imported ************************************************

;        XREF    _UIKBase


******* Exported ************************************************

        XDEF    __SearchUIK
        XDEF    __SendIPUIKRequest
        XDEF    _IPUIK_AddObjectTags
        XDEF    _IPUIK_AddPrivateObjectTags
        XDEF    _IPUIK_CallObjectFuncTags


****************************************************************
*
*       Routines
*
****************************************************************/

        SECTION text,CODE

libname
    dc.b    "uik.library",0
    CNOP    0,4

GetTaskUIK
    move.l  a6,-(sp)
    move.l  4.w,a6

    moveq   #1,d0
    lea     libname(pc),a1
    move.l  a0,-(sp)
    jsr     _LVOOpenLibrary(a6)
    move.l  (sp)+,a0
    tst.l   d0
    beq.b   .end
    move.l  d0,a1
    movem.l a0-a1,-(sp)
    jsr     _LVOCloseLibrary(a6)
    movem.l (sp)+,a0-a1

    lea     lb_TaskList(a1),a1
    moveq   #0,d0
    FORBID
    move.l  (a1),d1
.loop
    move.l  d1,a1
    move.l  (a1),d1
    beq.b   .error
    cmp.l   8(a1),a0
    bne.b   .loop
    move.l  12(a1),d0
    move.l  d0,a0
.error
    jsr     _LVOPermit(a6)
.end
    move.l  (sp)+,a6
    tst.l   d0
    rts


***********************************************************
*   __SearchUIK()
*
***********************************************************

__SearchUIK
    move.l  4.w,a0
    move.l  ThisTask(a0),a0
    bsr.b   GetTaskUIK
    bne.b   .ok
    addq.w  #4,sp       dépile l'adresse de retour dans le glue
.ok rts

;    move.l  4.w,a0
;    move.l  ThisTask(a0),a0
;    move.l  TC_Userdata(a0),d1      DestUIK = task->UserData?
;    beq.b   .error
;    btst    #0,d1                   Adresse paire?
;    bne.b   .error
;    move.l  d1,a0                   a2=UIK
;    cmp.l   uo_UIK(a0),d1           UIK->UIK == UIK?
;    bne.b   .error                  si non: pas UIK
;    rts
;
;.error
;    moveq   #0,d0
;    addq.w  #4,sp       dépile l'adresse de retour dans le glue
;    rts

***********************************************************
*   __SendIPUIKRequest( task, ... )
*                       4(sp)
***********************************************************

__SendIPUIKRequest
    movem.l d2/a2-a3/a5-a6,-(sp)
    move.l  4.w,a6
    moveq   #0,d0

    move.l  ThisTask(a6),a0
    ;move.l  TC_Userdata(a0),d1      DestUIK = task->UserData?
    ;beq     .no
    ;btst    #0,d1                   Adresse paire?
    ;bne     .no
    ;move.l  d1,a2                   a2=UIK
    ;cmp.l   uo_UIK(a2),d1           UIK->UIK == UIK?
    ;bne     .no                     si non: pas UIK
    bsr.b   GetTaskUIK
    beq     .no
    move.l  d0,a2                   a2=UIK

    movem.l 24(sp),a3               a3=task
    cmp.l   uike_Task(a2),a3
    beq     .no
    sub.w   #MP_SIZE,sp
    move.l  sp,d3                   d3=port

    move.b  #NT_MSGPORT,LN_TYPE(sp)
    move.b  #PA_SIGNAL,MP_FLAGS(sp)
    move.l  uike_Task(a2),MP_SIGTASK(sp)
    lea     MP_MSGLIST(sp),a0
    NEWLIST a0
    move.b  #SIGB_SINGLE,MP_SIGBIT(sp)

    FORBID

    ;move.l  TC_Userdata(a3),d1      DestUIK = task->UserData?
    ;beq.b   .end
    ;btst    #0,d1                   Adresse paire?
    ;bne.b   .end
    ;move.l  d1,a5                   a5=DestUIK
    ;cmp.l   uo_UIK(a5),d1           UIK->UIK == UIK?
    ;bne.b   .end                    si non: pas UIK
    move.l  a3,a0
    bsr     GetTaskUIK
    beq.b   .end
    move.l  d0,a5                   a5=UIK
    tst.b   uo_Status(a5)           UIK->Status == STARTED?
    bne.b   .end                    si non: rien a faire avec

    moveq   #0,d0
    move.l  #SIGF_SINGLE,d1
    jsr     _LVOSetSignal(a6)

    lea     uike_CmdReq(a2),a1
    move.l  sp,MN_REPLYPORT(a1)
    lea     uike_CmdPort(a5),a0     port distant
    jsr     _LVOPutMsg(a6)

.wait
    move.l  uike_CmdMask(a2),d0
    or.l    #SIGF_SINGLE,d0
    jsr     _LVOWait(a6)            Wait( CmdMask | SIGF_SINGLE )
    move.l  d0,d2                   d2 = events

.cmdmask
    move.l  d2,d0
    and.l   uike_CmdMask(a2),d0     if (events & CmdMask)
    beq.b   .single
        jsr     _LVOPermit(a6)
.while
        lea     uike_CmdPort(a2),a0
        jsr     _LVOGetMsg(a6)          while (msg = GetMsg( CmdPort ))
        tst.l   d0
        beq.b   .endwhile

        move.l  d0,-(sp)
        move.l  d0,a0
        bsr.b   do_process_warn             do_process_warn( msg )
        move.l  (sp)+,a1
        jsr     _LVOReplyMsg(a6)

        bra.b   .while
.endwhile
        jsr     _LVOForbid(a6)

.single
    ;move.l  d2,d0
    ;and.l   #SIGF_SINGLE,d0         if (events & SIGF_SINGLE)
    ;beq.b   .wait

        move.l  sp,a0
        jsr     _LVOGetMsg(a6)          if (GetMsg( tmpport )) break;
        tst.l   d0
        beq.b   .wait

    move.l  d0,a0
    move.l  IO_UNIT(a0),d0          résultat de la fonction

.end
    jsr     _LVOPermit(a6)
    add.w   #MP_SIZE,sp
.no
    movem.l (sp)+,d2/a2-a3/a5-a6
    rts

;------------------------------------------------------------

do_process_warn
    move.w  IO_COMMAND(a0),d1
    and.w   #$8000,d1
    beq.b   .end
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

****************************************************************
* obj = IPUIK_AddObjectTags( task, objname, parent, tag, data, etc...)
*                                       pile
* obj = IPUIK_AddPrivateObjectTags( task, rg, parent, tag, data, etc...)
*                                       pile
* obj = IPUIK_CallObjectFuncTags( task, obj, func, parm, tag, data, etc...)
*                                       pile
****************************************************************

_IPUIK_AddObjectTags
    ;DBUG    <'name %ls %lx',10,0>,8(sp),4(sp)
    bsr     __SearchUIK
    move.w  #-_LVOUIK_AddObject,uike_CmdReq+IO_COMMAND(a0)
    move.l  a4,uike_ArgRegs+uar_A4(a0)
    move.l  8(sp),uike_ArgRegs+uar_A0(a0)
    move.l  12(sp),uike_ArgRegs+uar_D0(a0)
    lea     16(sp),a1
    move.l  a1,uike_ArgRegs+uar_A1(a0)
    bra     __SendIPUIKRequest

_IPUIK_AddPrivateObjectTags
    bsr     __SearchUIK
    move.w  #-_LVOUIK_AddPrivateObject,uike_CmdReq+IO_COMMAND(a0)
    move.l  a4,uike_ArgRegs+uar_A4(a0)
    move.l  8(sp),uike_ArgRegs+uar_A0(a0)
    move.l  12(sp),uike_ArgRegs+uar_D0(a0)
    lea     16(sp),a1
    move.l  a1,uike_ArgRegs+uar_A1(a0)
    bra     __SendIPUIKRequest

_IPUIK_CallObjectFuncTags
    bsr     __SearchUIK
    move.w  #-_LVOUIK_AddPrivateObject,uike_CmdReq+IO_COMMAND(a0)
    move.l  a4,uike_ArgRegs+uar_A4(a0)
    move.l  8(sp),uike_ArgRegs+uar_A1(a0)
    move.l  12(sp),uike_ArgRegs+uar_D0(a0)
    move.l  16(sp),uike_ArgRegs+uar_A0(a0)
    lea     20(sp),a1
    move.l  a1,uike_ArgRegs+uar_D1(a0)
    bra     __SendIPUIKRequest



    END
