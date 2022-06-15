    OPT O+,W-
        OPT O1+                 ; Tells when a branch could be optimised to short
        OPT i+                  ; Tells when '#' is probably missing
        OPT d+
****************************************************************
*
*       Project:    UIK
*       Function:   list gadget
*
*       Created:    17/09/90    Jean-Michel Forgeas
*
****************************************************************

        IDNT    objlista


******* Includes ************************************************

        INCDIR  "include:"

        INCLUDE "intuition/intuition.i"
        INCLUDE "uiki:uikobj.i"
        INCLUDE "uiki:uikbox.i"
        INCLUDE "uiki:objlist.i"


******* Imported ************************************************

        XREF    LockObjVars
        XREF    UnlockObjVars
        XREF    _UIK_OLi_SelectList
        ;XREF    _UIK_OLi_DrawLine
        XREF    _UIK_nl_ListPtr


******* Exported ************************************************

        XDEF    _HdlUIK_ListSelect


        SECTION text,CODE


*************************************
*   HdlUIK_ListSelect( obj, msg )
*                      a1   a3
*************************************

_HdlUIK_ListSelect              ; uo_Status(a1) déja testé dans valid_object
    bsr     LockObjVars
    pea     UnlockObjVars(pc)
    moveq   #0,d1
    move.w  im_MouseY(a3),d1

    move.l  uo_rq(a1),d0                d0 = requester
    beq.b   .win                        si pas de requester
    move.l  d0,a0
    sub.w   rq_TopEdge(a0),d1           offset du requester
.win
    sub.w   uol_Gad+gg_TopEdge(a1),d1       d1 = offset dans le gadget
    move.l  uo_ObjExt(a1),a0
    move.l  uoe_Font(a0),a0                 a0 = fonte de l'objet
    divu    tf_YSize(a0),d1
    add.w   uol_Pos(a1),d1                  d1 = numéro de ligne

    lea     uol_ListText(a1),a0             a0 = UIKList*
    LISTPTR     ; modifie a0,d0
    cmp.w   uikl_NodeNum+2(a0),d1           (c'est un long)
    bmi.b   .10                             si on click aprés la dernière
    moveq   #0,d2                           ligne de la liste, le message
    bra.b   .end                            est annulé
.10
    move.l  a1,a0
    ext.l   d1
    move.l  d1,d0
    bsr     _UIK_OLi_SelectList
.end
    rts



*************************************
        END
*************************************



    btst    #UIKLB_SELDISPL,uol_ListFlags+3(a1)
    bne.b   .20                             si pas de display de sélection
    move.w  d0,uol_Sel(a1)                  enregistrer la sélection
    bra.b   .end                            terminé
.20
    cmp.w   uol_Sel(a1),d0                  si même sélection qu'avant
    beq.b   .end                            terminé

    movem.l a2/d2,-(sp)
    move.w  d0,d2
    ext.l   d2
    move.l  a1,a2

    move.w  uol_Sel(a2),d0
    ext.l   d0

    moveq   #-1,d1
    cmp.w   d0,d1                           ancienne sélection == -1 ?
    beq.b   .trace                          oui: on efface pas

    move.w  d1,uol_Sel(a2)
    move.l  a2,a0                           d0 déjà initialisé
    bsr     _UIK_OLi_DrawLine
.trace
    move.w  d2,uol_Sel(a2)
    move.l  d2,d0
    move.l  a2,a0
    bsr     _UIK_OLi_DrawLine
    movem.l (sp)+,a2/d2
.end
    rts


