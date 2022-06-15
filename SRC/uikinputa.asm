    OPT O+,W-
        OPT d+
****************************************************************
*
*       Project:    UIK
*       Function:   uik input modes
*
*       Created:    16/04/92    Jean-Michel Forgeas
*
****************************************************************

        IDNT    uikinput


******* Includes ************************************************

        INCDIR  "include:"

        INCLUDE "lib/exec_lib.i"
        INCLUDE "exec/execbase.i"
        INCLUDE "exec/ables.i"

        INCLUDE "uiki:uikbase.i"
        INCLUDE "uiki:uikmacros.i"
        INCLUDE "uiki:uikglobal.i"
        INCLUDE "uiki:uikzone.i"


******* Imported ************************************************

        XREF    _UIK_AccessPrefs

        XREF    _zo_UnmarkZone
        XREF    _zo_InstallZoneList
        XREF    _zo_MarkFirstZone
        XREF    _zo_ChangeActiveZoneList

        XREF    _VBlankSend

        XREF    _intena
        XREF    _UIKBase

        XREF    _IPCode_Mouse
        XREF    _IPCode_Bouris
        XREF    _IPCode_JoyMouse
        XREF    _IPCode_1Contact
        XREF    _IPCode_2Contacts
        XREF    _IPCode_5Contacts


******* Exported ************************************************

        XDEF    Input_VblankCode

        XDEF    _Input_InstallTask
        XDEF    _Input_UninstallTask

        XDEF    _UIK_Input_AddJoyEvent
        XDEF    _UIK_Input_SetTime
        XDEF    _UIK_Input_Flush
        XDEF    _UIK_Input_ChangeActiveZoneList
        XDEF    _UIK_Input_CycleZoneList
        XDEF    _IntUIK_Input_Cycle
        XDEF    _UIK_Input_JamFrame
        XDEF    _UIK_Input_ReleaseFrame
        XDEF    _UIK_Input_Get
        XDEF    _UIK_Input_Set


****************************************************************
*
*       Routines
*
****************************************************************/

        SECTION text,CODE


Input_VblankCode
    addq.l  #1,lb_Ticks(a1)

    move.l  lb_ZonedUIK(a1),d0
    beq.b   .end

    movem.l d2-d5/a0/a2/a6,-(sp)
    move.l  d0,a2

    move.l  a1,a6
    bsr     Input_JoyTest
    move.l  a2,a0
    bsr     _VBlankSend

    move.b  uike_JoyVal(a2),uike_OldJoyVal(a2)
    move.b  uike_JoyButs(a2),uike_OldJoyButs(a2)

    movem.l (sp)+,d2-d5/a0/a2/a6
.end
    moveq   #0,d0
    rts


*******************************************************
*   UIK_Input_AddJoyEvent( uik, buts, flush )
*                          a1    d0    d1
*******************************************************

_UIK_Input_AddJoyEvent
    movem.l d2-d5/a0/a2/a6,-(sp)
    move.l  a1,a2
    move.l  d0,d2

    DISABLE a0

    tst.l   d1
    beq.b   .noflush
    bsr.b   _UIK_Input_Flush
.noflush
    bsr     AddEvent

    move.l  a2,a0
    bsr     _VBlankSend

    move.b  uike_JoyVal(a2),uike_OldJoyVal(a2)
    move.b  uike_JoyButs(a2),uike_OldJoyButs(a2)

    ENABLE  a0

    movem.l (sp)+,d2-d5/a0/a2/a6
    rts


*******************************************************
*   UIK_Input_SetTime( uik, speed, valid, wait )
*                      a1    d0     d1     a0
*******************************************************

_UIK_Input_SetTime
    move.w  d0,uike_TempoSpeed(a1)
    swap    d1
    move.w  a0,d1                       d1 = valid:wait
    move.l  d1,uike_TempoValidRight(a1) uike_TempoValid + uike_TempoWait
    move.l  d1,uike_TempoValidLeft(a1)
    move.l  d1,uike_TempoValidBack(a1)
    move.l  d1,uike_TempoValidForw(a1)
    move.l  d1,uike_TempoValidFire(a1)
    move.l  _UIKBase,a1
    move.l  lb_UIKPrefs(a1),a1
    move.w  d0,uikp_ZoneSpeed(a1)
    move.l  d1,uikp_ValidTime(a1)       uikp_ValidTime + uikp_WaitTime
    rts


*******************************************************
*   UIK_Input_Flush( uik )
*                    a1
*******************************************************

_UIK_Input_Flush
    DISABLE a0
    bsr     circbuf_init
    moveq   #0,d0
    move.b  d0,uike_JoyVal(a1)      pas uike_OldJoyVal !!!
    move.b  d0,uike_JoyButs(a1)     pas uike_OldJoyButs
    move.b  d0,uike_ProcButs(a1)
    move.w  d0,uike_IptButs(a1)     et uike_IptChgs
    move.l  uike_InputMainVector(a1),uike_InputVector(a1)
    ENABLE  a0
    rts


*******************************************************
*   UIK_Input_ChangeActiveZoneList( obj )
*                             a0
*******************************************************
_UIK_Input_ChangeActiveZoneList
    move.l  uo_ObjExt(a0),a1
    move.l  uoe_ZoneList(a1),a1                 a1 = ZoneList
    move.l  uo_UIK(a0),a0                       a0 = UIK
    move.l  a1,d0                               la liste est valide ?
    bne     _zo_ChangeActiveZoneList ( register __a0 struct UIKGlobal *uik, register __a1 struct UIKZoneList *uzl )
    rts

*******************************************************
*   UIK_Input_CycleZoneList( obj, cycle_on )
*                            a0      d0
*******************************************************
* est appelée aussi chaque fois que l'on
* change de zone (deeper/upper/install)
* - si d0 == -1, on prend le on/off de la liste active

_UIK_Input_CycleZoneList
    move.l  uo_UIK(a0),a1                       a1 = UIK
    move.l  uo_ObjExt(a0),a0
    move.l  uoe_ZoneList(a0),a0                 a0 = ZoneList
    move.l  a0,d1                               la liste est valide ?
    beq     cycle_end

_IntUIK_Input_Cycle
    move.l  a2,-(sp)
    move.l  a1,a2

    cmp.b   #-1,d0                              doit-on déterminer le on/off ?
    bne.b   .do_onoff
    btst    #UZLB_NOCYCLE,uozl_Flags+1(a0)      si oui, test du flag
    seq     d0                                  -> positionnement de d0

.do_onoff
    btst    #INPUTB_JAMED,uike_InputFlags(a2)   si JAMED pas touche!
    bne.b   .end


    tst.b   d0
    beq.b   .do_cycle_off

.do_cycle_on
    bclr    #UZLB_NOCYCLE,uozl_Flags+1(a0)      doit être cyclée

    move.l  a0,-(sp)
    move.l  a2,a0
    bsr     _zo_UnmarkZone                      uik=a0  remet l'affichage normal
    move.l  (sp)+,uike_ActiveZoneList(a2)       active la ZoneList

    ; if (uik->InputMode >= INPUTMODE_1CONTACT && uik->InputMode <= INPUTMODE_5CONTACTS)
    cmp.b   #INPUTMODE_1CONTACT,uike_InputMode(a2)
    bcs.b   .end
    cmp.b   #INPUTMODE_5CONTACTS,uike_InputMode(a2)
    bhi.b   .end
    move.l  a2,a0                               a0 = UIK
    bsr     _zo_MarkFirstZone                   remet l'affichage sur la première zone
    move.l  a2,a1
    bsr     _UIK_Input_Flush
    bra.b   .end

.do_cycle_off
    bset    #UZLB_NOCYCLE,uozl_Flags+1(a0)      ne doit pas être cyclée
    cmp.l   uike_ActiveZoneList(a2),a0          si elle est celle qui est active
    bne.b   .end                                on arrête le cyclage

    move.l  a2,a0
    bsr     _zo_UnmarkZone                      uik=a0  remet l'affichage normal
    clr.l   uike_ActiveZoneList(a2)             inhibe le cyclage
    clr.l   uike_InputVector(a2)                inhibe le cyclage

.end
cycle_end
    move.l  (sp)+,a2
    rts


*******************************************************
*   UIK_Input_JamFrame( uik )
*                       a1
*******************************************************

_UIK_Input_JamFrame
    bset    #INPUTB_JAMED,uike_InputFlags+1(a1)
    rts


*******************************************************
*   UIK_Input_ReleaseFrame( uik )
*                           a1
*******************************************************

_UIK_Input_ReleaseFrame
    move.l  a2,-(sp)
    move.l  a1,a2
    ;bsr     _Input_InstallTask
    bclr    #INPUTB_JAMED,uike_InputFlags+1(a2)
    move.l  (sp)+,a2
    rts


*******************************************************
*   UIK_Input_Get( uik, pinput, pheld, pauto )
*                  a0     d0     d1     a1
*******************************************************
_UIK_Input_Get
    cmp.l   #-1,a1
    beq.b   .getinput

    move.b  uike_InputAuto(a0),(a1)

.getinput
    cmp.l   #-1,d0
    beq.b   .getheld

    move.l  d0,a1
    move.b  uike_InputMode(a0),(a1)

.getheld
    cmp.l   #-1,d1
    beq.b   .end

    move.l  d1,a1
    move.b  uike_InputHeld(a0),(a1)

.end
    rts


*******************************************************
*   UIK_Input_Set( uik, input, held, auto )
*                  a0    d0     d1    a1
*******************************************************

_UIK_Input_Set
    movem.l d1-d2/a1/a2,-(sp)
D1OFFS  equ 0
A1OFFS  equ 8
    move.l  a0,a2
    moveq   #-1,d2

    cmp.b   d2,d0
    beq     .setheld

    ;--------------------------------
    pea     .setheld(pc)
    tst.b   d0
    beq     IPStart_Mouse
    subq.b  #1,d0
    beq     IPStart_Bouris
    subq.b  #1,d0
    beq     IPStart_JoyMouse
    subq.b  #1,d0
    beq     IPStart_1Contact
    subq.b  #1,d0
    beq     IPStart_2Contacts
    subq.b  #1,d0
    beq     IPStart_5Contacts
    addq.w  #4,sp                   erreur: dépile le pea et passe
    bra.b   .end

    ;--------------------------------
.setheld
    move.b  D1OFFS+3(sp),d1         ; held
    cmp.b   d2,d1
    beq.b   .setauto

    move.b  d1,uike_InputHeld(a2)

    ;--------------------------------
.setauto
    move.b  A1OFFS+3(sp),d1         ; auto
    cmp.b   d2,d1
    beq.b   .end

    move.b  d1,uike_InputAuto(a2)

    ;--------------------------------
.end
    move.l  a2,a1
    bsr     _UIK_Input_Flush
    movem.l (sp)+,d1-d2/a1/a2
    rts


*******************************************************
IPStart_Mouse
    bsr     IPStop_All
    move.b  #INPUTMODE_MOUSE,uike_InputMode(a2)
    bsr     CommonInstallZoneList
    move.l  #_IPCode_Mouse,d0
    move.l  d0,uike_InputVector(a2)
    move.l  d0,uike_InputMainVector(a2)
    rts

IPStop_Mouse
    rts

*******************************************************
IPStart_Bouris
    bsr     IPStop_All
    move.b  #INPUTMODE_BOURIS,uike_InputMode(a2)
    bsr     CommonInstallZoneList
    move.l  #_IPCode_Bouris,d0
    move.l  d0,uike_InputVector(a2)
    move.l  d0,uike_InputMainVector(a2)
    rts

IPStop_Bouris
    rts

*******************************************************
IPStart_JoyMouse
    bsr     IPStop_All
    move.b  #INPUTMODE_JOYMOUSE,uike_InputMode(a2)
    bsr     CommonInstallZoneList
    move.l  #_IPCode_JoyMouse,d0
    move.l  d0,uike_InputVector(a2)
    move.l  d0,uike_InputMainVector(a2)
    rts

IPStop_JoyMouse
    rts

*******************************************************
IPStart_1Contact
    bsr     IPStop_All
    move.b  #INPUTMODE_1CONTACT,uike_InputMode(a2)
    ;move.b  #1,uike_InputAuto(a2)           force le mode auto pour le cyclage
    bsr     CommonInstallZoneList
    move.l  #_IPCode_1Contact,d0
    move.l  d0,uike_InputVector(a2)
    move.l  d0,uike_InputMainVector(a2)
    rts

IPStop_1Contact
    rts

*******************************************************
IPStart_2Contacts
    bsr     IPStop_All
    move.b  #INPUTMODE_2CONTACTS,uike_InputMode(a2)
    bsr     CommonInstallZoneList
    move.l  #_IPCode_2Contacts,d0
    move.l  d0,uike_InputVector(a2)
    move.l  d0,uike_InputMainVector(a2)
    rts

IPStop_2Contacts
    rts

*******************************************************
IPStart_5Contacts
    bsr     IPStop_All
    move.b  #INPUTMODE_5CONTACTS,uike_InputMode(a2)
    bsr     CommonInstallZoneList
    move.l  #_IPCode_5Contacts,d0
    move.l  d0,uike_InputVector(a2)
    move.l  d0,uike_InputMainVector(a2)
    rts

IPStop_5Contacts
    rts

*******************************************************

CommonInstallZoneList
    move.l  a2,a0
    move.l  uike_ZoneListBase(a2),a1
    bsr     _zo_InstallZoneList     (uik=a0,uzl=a1) appelle _Input_InstallTask
    rts

;DoNoCycle
;    cmp.l   lb_ZonedUIK(a2),a1      c'est moi qui suis actif ?
;    bne.b   .end                    sinon rien
;    move.l  a1,a2
;    move.l  a1,a0
;    bsr     _zo_UnmarkZone          remet l'affichage normal
;    clr.l   uike_ActiveZoneList(a2) inhibe le cyclage
;    clr.l   uike_InputVector(a2)    inhibe l'appel du code
;.end
;    rts

Uncycle_Sem ; (uikbase=a2)
    move.l  lb_ZonedUIK(a2),d0      un uik est-il actif ?
    beq.b   .end
    movem.l a2/a6,-(sp)
    move.l  a2,a6
    move.l  d0,a2
    move.l  d0,a0
    bsr     _zo_UnmarkZone          remet l'affichage normal
    clr.l   lb_ZonedUIK(a6)         oui: inactive
    clr.l   uike_ActiveZoneList(a2) inhibe le cyclage
    clr.l   uike_InputVector(a2)    inhibe l'appel du code
    movem.l (sp)+,a2/a6
.end
    rts

IPStop_All ; (uik=a2,uikbase=a6)
    clr.l   uike_InputVector(a2)
    bclr    #INPUTB_KZAKZ_ON,uike_InputFlags+1(a2)
    lea     Uncycle_Sem(pc),a0
    bsr     _UIK_AccessPrefs     enlève le cyclage de n'importe quelle tâche

    bsr     IPStop_Mouse
    bsr     IPStop_Bouris
    bsr     IPStop_JoyMouse
    bsr     IPStop_1Contact
    bsr     IPStop_2Contacts
    bsr     IPStop_5Contacts
    rts

*******************************************************
*   Input_InstallTask( uik )
*                      a1
*******************************************************

_Input_InstallTask
    lea     InstallMe_Sem(pc),a0
    bsr     _UIK_AccessPrefs
    rts

InstallMe_Sem ; (uikbase=a2,uik=a1,a3=uikprefs) all protected, under _UIK_AccessPrefs

    ;---- d'abord initialiser le buffer circulaire et inputVector
    bsr     _UIK_Input_Flush        a1 est préservé

    ;---- doit-on s'installer ?
    cmp.l   lb_ZonedUIK(a2),a1      j'y suis déjà ?
    beq.b   .end                    alors rien
    move.l  a1,-(sp)
    bsr     Uncycle_Sem (base=a2)   enlève le cyclage de l'autre tâche
    move.l  (sp)+,a1
    ;---- puis installer ce uik
    move.l  a1,lb_ZonedUIK(a2)
.end
    rts

_Input_UninstallTask
    lea     UninstallMe_Sem(pc),a0
    bsr     _UIK_AccessPrefs
    rts

UninstallMe_Sem ; (uikbase=a2,uik=a1) all protected, under _UIK_AccessPrefs
    cmp.l   lb_ZonedUIK(a2),a1      est-ce moi ?
    bne.b   .end                    sinon rien
    clr.l   lb_ZonedUIK(a2)         si oui, je n'y suis plus.
.end
    rts


*******************************************************
*   Input_JoyTest()
*
*******************************************************

* joy.asm
* JOYSTICK(1) READ
* by Alex Livshits & jmf
    include "exec/types.i"
    include "hardware/custom.i"
    include "hardware/cia.i"

    xref    _custom
    xref    _ciaa
    xref    _ciaapra

* -------------------------------------
* test joystick 1 position
* -------------------------------------

Input_JoyTest:
D2_JOY  equr    d2
D3_OLD  equr    d3
D4_BUT  equr    d4
D5_CHG  equr    d5

        lea     _custom,a0
        moveq   #0,D2_JOY

* test FIRE
        btst.b  #CIAB_GAMEPORT1,_ciaapra
        bne.s   .10
        bset    #VBJOYB_FIRE,D2_JOY

* test RIGHT
.10     move.w  joy1dat(a0),d1
        btst    #1,d1
        beq.s   .20
        bset    #VBJOYB_RIGHT,D2_JOY

* test LEFT
.20     btst    #9,d1
        beq.s   .30
        bset    #VBJOYB_LEFT,D2_JOY

* test BACK
.30     btst    #0,d1
        bne.s   .32
        btst    #1,d1
        beq.s   .40
        bset    #VBJOYB_BACK,D2_JOY
        bra.s   .40
.32     btst    #1,d1
        bne.s   .40
        bset    #VBJOYB_BACK,D2_JOY

* test FORW
.40     btst    #8,d1
        bne.s   .42
        btst    #9,d1
        beq.s   AddEvent
        bset    #VBJOYB_FORW,D2_JOY
        bra.s   AddEvent
.42     btst    #9,d1
        bne.s   AddEvent
        bset    #VBJOYB_FORW,D2_JOY

AddEvent
    move.b  uike_OldJoyVal(a2),D3_OLD
    move.b  uike_JoyButs(a2),D4_BUT

    cmp.b   #INPUTMODE_5CONTACTS,uike_InputMode(a2)
    beq.b   .right_down
    tst.b   uike_InputHeld(a2)
    bne     .Held

    ;=========================pas de maintien=======================
.right_down
    btst    #VBJOYB_RIGHT,D2_JOY
        beq.b   .right_up                           if (uik->JoyVal & VBJOYF_RIGHT)
        ;-------------------------- état bas            { //-------------------------- état bas
        btst    #VBJOYB_RIGHT,D3_OLD                    if (! (uik->OldJoyVal & VBJOYF_RIGHT)) // était-il haut ?
        bne.b   .left_down                                  { //---- il vient juste de passer en bas -> on vient d'appuyer
            bset    #VBJOYB_RIGHT,D4_BUT                    uik->JoyButs |= VBJOYF_RIGHT;
            bra.b   .left_down                              }
.right_up                                               ;}
        ;-------------------------- état haut       else{ //-------------------------- état haut
        btst    #VBJOYB_RIGHT,D3_OLD                    if (uik->OldJoyVal & VBJOYF_RIGHT) // était-il bas ?
        beq.b   .left_down
            bclr    #VBJOYB_RIGHT,D4_BUT                    uik->JoyButs &= ~VBJOYF_RIGHT;
        bra.b   .back_down
                                                            ;}
    ;--------------------------------------------------------------
.left_down
    btst    #VBJOYB_LEFT,D2_JOY
        beq.b   .left_up
        ;-------------------------- état bas
        btst    #VBJOYB_LEFT,D3_OLD
        bne.b   .back_down
            bset    #VBJOYB_LEFT,D4_BUT
            bra.b   .back_down
.left_up
        ;-------------------------- état haut
        btst    #VBJOYB_LEFT,D3_OLD
        beq.b   .back_down
            bclr    #VBJOYB_LEFT,D4_BUT

    ;--------------------------------------------------------------
.back_down
    btst    #VBJOYB_BACK,D2_JOY
        beq.b   .back_up
        ;-------------------------- état bas
        btst    #VBJOYB_BACK,D3_OLD
        bne.b   .forw_down
            bset    #VBJOYB_BACK,D4_BUT
            bra.b   .forw_down
.back_up
        ;-------------------------- état haut
        btst    #VBJOYB_BACK,D3_OLD
        beq.b   .forw_down
            bclr    #VBJOYB_BACK,D4_BUT
        bra.b   .fire_down

    ;--------------------------------------------------------------
.forw_down
    btst    #VBJOYB_FORW,D2_JOY
        beq.b   .forw_up
        ;-------------------------- état bas
        btst    #VBJOYB_FORW,D3_OLD
        bne.b   .fire_down
            bset    #VBJOYB_FORW,D4_BUT
            bra.b   .fire_down
.forw_up
        ;-------------------------- état haut
        btst    #VBJOYB_FORW,D3_OLD
        beq.b   .fire_down
            bclr    #VBJOYB_FORW,D4_BUT

    ;--------------------------------------------------------------

.fire_down
    btst    #VBJOYB_FIRE,D2_JOY
        beq.b   .fire_up
        ;-------------------------- état bas
        btst    #VBJOYB_FIRE,D3_OLD
        bne     .end
            bset    #VBJOYB_FIRE,D4_BUT
            bra     .end
.fire_up
        ;-------------------------- état haut
        btst    #VBJOYB_FIRE,D3_OLD
        beq     .end
            bclr    #VBJOYB_FIRE,D4_BUT

    ;===============================================================
    bra.b   .end
.Held
    ;============================maintien===========================
.hright_down
    btst    #VBJOYB_RIGHT,D2_JOY
    beq.b   .hleft_down
        ;-------------------------- état bas
        btst    #VBJOYB_RIGHT,D3_OLD
        bne.b   .hleft_down
            btst    #VBJOYB_LEFT,D4_BUT
            bne.b   .hleft_trans
.hright_trans
            bchg    #VBJOYB_RIGHT,D4_BUT
        bra.b   .hback_down

    ;--------------------------------------------------------------
.hleft_down
    btst    #VBJOYB_LEFT,D2_JOY
    beq.b   .hback_down
        ;-------------------------- état bas
        btst    #VBJOYB_LEFT,D3_OLD
        bne.b   .hback_down
            btst    #VBJOYB_RIGHT,D4_BUT
            bne.b   .hright_trans
.hleft_trans
            bchg    #VBJOYB_LEFT,D4_BUT

    ;--------------------------------------------------------------
.hback_down
    btst    #VBJOYB_BACK,D2_JOY
    beq.b   .hforw_down
        ;-------------------------- état bas
        btst    #VBJOYB_BACK,D3_OLD
        bne.b   .hforw_down
            btst    #VBJOYB_FORW,D4_BUT
            bne.b   .hforw_trans
.hback_trans
            bchg    #VBJOYB_BACK,D4_BUT
        bra.b   .hfire_down

    ;--------------------------------------------------------------
.hforw_down
    btst    #VBJOYB_FORW,D2_JOY
    beq.b   .hfire_down
        ;-------------------------- état bas
        btst    #VBJOYB_FORW,D3_OLD
        bne.b   .hfire_down
            btst    #VBJOYB_BACK,D4_BUT
            bne.b   .hback_trans
.hforw_trans
            bchg    #VBJOYB_FORW,D4_BUT

    ;--------------------------------------------------------------

.hfire_down
    btst    #VBJOYB_FIRE,D2_JOY
    beq.b   .end
        ;-------------------------- état bas
        btst    #VBJOYB_FIRE,D3_OLD
        bne.b   .end
            bchg    #VBJOYB_FIRE,D4_BUT

    ;===============================================================
.end
    move.b  D2_JOY,uike_JoyVal(a2)
    move.b  D2_JOY,uike_OldJoyVal(a2)
    move.b  D4_BUT,uike_JoyButs(a2)
    rts

************************************************
circbuf_init    ; (uik=a1) appelée par process sous disable ou interrupt
    lea     uike_JoyEvents(a1),a0
    move.l  a0,uike_JTBeg(a1)
    move.l  a0,uike_JTEnd(a1)
    rts

************************************************
    XDEF    _circbuf_put
_circbuf_put     ; (uik=a2) appelée par interrupt
    ;------ range les valeurs
    move.l  uike_JTEnd(a2),a1
    move.b  d0,(a1)+
    move.b  d1,(a1)+                    a1 = new end

    ;------ incremente la fin du buffer
    lea     uike_JoyEvents(a2),a0
    move.l  a0,d0                       d0 = début de la table
    lea     uike_JoyEvents+40(a2),a0    a0 = aprés la fin de la table
    cmp.l   a1,a0                       on pointe apres la fin de la table ?
    bne.b   .incok
    move.l  d0,a1                       oui: on revient sur le debut
.incok
    cmp.l   uike_JTBeg(a2),a1           est-ce qu'on pointe sur le début ?
    beq.b   .full                       oui: le buffer est plein
    move.l  a1,uike_JTEnd(a2)           non: une entrée en plus
.full
    rts

************************************************
    XDEF    _circbuf_get
_circbuf_get     ; (uik=a1) appelée par process sous disable
    ;------ retourne 0 si buffer vide
    moveq   #0,d0

    ;------ y-a-il qlq chose ?
    move.l  uike_JTBeg(a1),a0
    cmp.l   uike_JTEnd(a1),a0
    beq.b   .empty

    ;------ prend les valeurs
    move.l  a0,d0                       d0 = adresse des données valides

    ;------ incrémente le début
    move.l  d0,d1
    addq.l  #2,d1                       d1 = new début
    lea     uike_JoyEvents+40(a1),a0    a0 = aprés la fin de la table
    cmp.l   a0,d1                       on pointe apres la fin de la table ?
    bne.b   .incok
    lea     uike_JoyEvents(a1),a0       oui: on revient sur le debut
    move.l  a0,d1                       d1 = new début = début de la table
.incok
    move.l  d1,uike_JTBeg(a1)
.empty
    rts









