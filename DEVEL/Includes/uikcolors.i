    IFND    UIKDEFS_COLORS
UIKDEFS_COLORS  SET 1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**

    IFND    EXEC_TYPES_I
    INCLUDE "exec/types.i"
    ENDC


MAX_UIKCOLORS       equ 32
MAX_UIKDEFINEDPENS  equ 7

UIKCOL_GREY     equ 0
UIKCOL_BLACK    equ 1
UIKCOL_WHITE    equ 2
UIKCOL_LIGHT    equ 3
UIKCOL_ZONE     equ 4
UIKCOL_LOOK     equ 5
UIKCOL_IMP      equ 6

 STRUCTURE  UIKTColors,0
    STRUCT      uikcol,MAX_UIKCOLORS
    LABEL   UIKCOLORS_SIZE
    LABEL   UIKTColors_SIZE

 STRUCTURE  UIKColors,0
    UBYTE       Grey
    UBYTE       Black
    UBYTE       White
    UBYTE       Light
    LABEL       ColZone
    UBYTE       c4
    LABEL       ColLook
    UBYTE       c5
    LABEL       ColImp
    UBYTE       c6
    UBYTE       c7
    UBYTE       c8
    UBYTE       c9
    UBYTE       c10
    UBYTE       c11
    UBYTE       c12
    UBYTE       c13
    UBYTE       c14
    UBYTE       c15
    UBYTE       c16
    UBYTE       c17
    UBYTE       c18
    UBYTE       c19
    UBYTE       c20
    UBYTE       c21
    UBYTE       c22
    UBYTE       c23
    UBYTE       c24
    UBYTE       c25
    UBYTE       c26
    UBYTE       c27
    UBYTE       c28
    UBYTE       c29
    UBYTE       c30
    UBYTE       c31
    LABEL   UIKColors_SIZE ; pareil que UIKCOLORS_SIZE ci-dessus

    ENDC
