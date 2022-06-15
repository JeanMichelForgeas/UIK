    IFND  UIK_GFX_H
UIK_GFX_H SET 1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**

    IFND    EXEC_TYPES_I
    INCLUDE "exec/types.i"
    ENDC


 STRUCTURE  UIKBitMap,0
    LONG        ubm_Mask
    WORD        ubm_UFlags
    WORD        ubm_Width
    WORD        ubm_BytesPerRow
    WORD        ubm_Rows
    BYTE        ubm_Flags
    BYTE        ubm_Depth
    WORD        ubm_Pad
    STRUCT      ubm_Planes,8*4
    LABEL   ubm_SIZEOF
    LABEL   UIKBitMap_SIZE

UIKTOBMAP_OFFSET equ 8

UIKBMAPB_CONTIG  equ 0
UIKBMAPB_MASKED  equ 1
UIKBMAPB_PUBLIC  equ 2
UIKBMAPB_BWSYS1  equ 3
UIKBMAPB_NOPLANE equ 4
UIKBMAPB_INVDONE equ 15

UIKBMAP_CONTIG  equ $0001
UIKBMAP_MASKED  equ $0002
UIKBMAP_PUBLIC  equ $0004
UIKBMAP_BWSYS1  equ $0008
UIKBMAP_NOPLANE equ $0010
UIKBMAP_INVDONE equ $8000

    ENDC
