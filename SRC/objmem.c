/****************************************************************
 *
 *      Project:   UIK
 *      Function:  routines de gestion des objets 'mémoire'
 *
 *      Created:   30/05/90    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikobj.h"
#include "uiki:objmemory.h"
#include "lci:uik_protosint.h"


/****** Imported ************************************************/


/****** Exported ************************************************/


/****** Statics *************************************************/


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void RemoveObjMem( struct UIKObjMem *obj )
{
    if (obj->CleanupFunc) (*obj->CleanupFunc)( obj, obj->MemPtr );
    if (obj->MemPtr) UIK_MemSpFree( obj->MemPtr );
}

static int AddObjMem( struct UIKObjMem *obj, struct TagItem *taglist )
{
  ULONG size = UIK_GetTagData( UIKTAG_OBJMem_MemSize, 256, taglist );
  ULONG type = UIK_GetTagData( UIKTAG_OBJMem_MemType, MEMF_ANY | MEMF_CLEAR, taglist );
  ULONG func = UIK_GetTagData( UIKTAG_OBJMem_UserCleanup, 0, taglist );

    if (! (obj->MemPtr = UIK_MemSpAlloc( size, type ))) return(0);
    obj->CleanupFunc = (void *) func;
    return(1);
}

UBYTE * __asm UIK_OMe_MemPtr( register __a0 struct UIKObjMem *obj )
{
    return( obj->MemPtr );
}

struct UIKObjGerm __far MemoryGerm =
    {
    "UIKObj_Memory",
    AddObjMem,                  // AddVector
    NULL,                       // StartVector
    NULL,                       // StopVector
    RemoveObjMem,               // RemoveVector
    NULL,                       // RefreshVector
    NULL,                       // ResizeVector
    NULL,                       // ExceptActVector
    NULL,                       // ExceptInaVector
    NULL,                       // ExceptMovVector
    NULL,                       // ExceptKeyVector
    0,0,0,0,
    0, 0,
    0,
    UIKID_MEM,
    sizeof(struct UIKObjMem),
    0,
    };
