#ifndef UIK_ZONE_H

#define UIK_ZONE_H
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif


struct UIKZone {
    struct UIKObj   *ZObj;
    ULONG           ZClass;
    void            (*ZFunc)();
    };

typedef struct UIKZone UZone;


struct UIKZoneList {
    struct UIKZone *ZoneStart;
    struct UIKZone *ActiveZone;    /* 0 ou adresse */
    UWORD           Flags;
    UWORD           padw;
    };

typedef struct UIKZoneList UZList;

/*---- ZoneList Flags ----*/
#define UZLF_NOCYCLE        0x0001

/*---- allocation extention block size ----*/
#define UZONE_ALLOC_BLOCK   (32*sizeof(struct UIKZone))


#endif
