#ifndef UIKDEFS_UIKTREE

#define UIKDEFS_UIKTREE
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif


/*------ automatic tree macros -------*/

struct UIKHookItem
    {
    void    (*hi_Vector)();
    ULONG   hi_Events;
    };

struct UIKTreeItem {
    WORD                tri_Level;
    ULONG               tri_Var;            /* si -1 pas de sauvegarde du ptr */
    UBYTE               *tri_ObjName;
    struct TagItem      *tri_TagList;
    ULONG               (*tri_Before)();    /* user function called before AddObject */
    ULONG               (*tri_After)();     /* idem but called after Addobject */
    struct UIKHookItem  *tri_Vectors;       /* ptr on array of events/vectors */
    };

#define NOSTORE -1

#define OBJECT( Level, Var, ObjName, TagList, Before, After, Vectors ) { Level, Var, ObjName, TagList, Before, After, Vectors }
#define OBJEND { -1 }

#include <stddef.h>


#endif
