        IFND UIK_ZONE_H
UIK_ZONE_H SET     1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**

    IFND EXEC_TYPES_I
    INCLUDE "exec/types.i"
    ENDC


 STRUCTURE  UIKZone,0
    APTR        uoz_ZObj;
    ULONG       uoz_ZClass;
    FPTR        uoz_ZFunc;
    LABEL   uoz_SIZE
    LABEL   UIKZone_SIZE


 STRUCTURE  UIKZoneList,0
    APTR        uozl_ZoneStart
    APTR        uozl_ActiveZone
    UWORD       uozl_Flags
    UWORD       uozl_padw
    LABEL   uozl_SIZE
    LABEL   UIKZoneList_SIZE


;---- ZoneList Flags ----
UZLB_NOCYCLE    equ     0

UZLF_NOCYCLE    equ     $0001

;---- allocation extention block size ----
UZONE_ALLOC_BLOCK equ  (32*uoz_SIZE)


        ENDC
