#ifndef UIKDEFS_BOX

#define UIKDEFS_BOX
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits R�serv�s
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

struct UIKBox {
    WORD    Left, Top;
    WORD    Width, Height;
    };

#endif
