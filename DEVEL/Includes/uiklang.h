#ifndef UIKDEFS_LANGAGE

#define UIKDEFS_LANGAGE
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#define UIK_KEYBUFLEN   256

struct UIKLang
    {
    UBYTE           **English;  /* adresse du tableau des textes en anglais */
    UBYTE           **Other;    /* adresse du tableau des textes de l'autre langue */
    UBYTE           **Langage;  /* pointe sur l'un des deux précédents */
    ULONG           Flags;
    struct UIKObj   *Master;    /* objet contenant les chaines de langage */
    UBYTE           *CatName;   /* pointeur passé par l'application */
    struct Catalog  *Catalog;   /* catalogue (si objet master) */
    };

#define UIKLANG_OWNER   0x0001

#endif
