#ifndef UIKDEFS_KEYSUPP

#define UIKDEFS_KEYSUPP
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef EXEC_IO_H
#include    <exec/io.h>
#endif

#define UIK_KEYBUFLEN   256

struct UIKKeySupp {
    struct IOStdReq ConsoleMsg;
    ULONG           Length;
    UBYTE           KeyBuffer[UIK_KEYBUFLEN];
    };

#endif
