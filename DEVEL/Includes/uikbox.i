    IFND    UIKDEFS_BOX
UIKDEFS_BOX     SET 1
**
**      Copyright (C) 1991,1992 Jean-Michel Forgeas
**                  Tous Droits Réservés
**

    IFND    EXEC_TYPES_I
    INCLUDE "exec/types.i"
    ENDC


 STRUCTURE  UIKBox,0
    UWORD       box_Left
    UWORD       box_Top
    UWORD       box_Width
    UWORD       box_Height
    LABEL   UIKBOX_SIZE
    LABEL   UIKBox_SIZE

    ENDC
