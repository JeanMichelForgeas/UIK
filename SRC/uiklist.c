/****************************************************************
 *
 *      Project:   UIK
 *      Function:  list management
 *
 *      Created:   11/05/90    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include <exec/types.h>
#include <exec/memory.h>
#include <clib/macros.h>

#include "uiki:uikglobal.h"
#include "uiki:uiklist.h"
#include "uiki:uikmacros.h"
#include "lci:uik_protosint.h"

#define UNODESIZE       sizeof(struct UIKNode)

extern void * __asm AsmCreatePool(register __d0 ULONG,register __d1 ULONG,register __d2 ULONG,register __a6 struct ExecBase *);
extern void __asm AsmDeletePool(register __a0 void *,register __a6 struct ExecBase *);
extern void * __asm AsmAllocPooled(register __a0 void *,register __d0 ULONG,register __a6 struct ExecBase *);
extern void __asm AsmFreePooled(register __a0 void *,register __a1 void *,register __d0 ULONG,register __a6 struct ExecBase *);


/****** Macros **************************************************/

#define UTABLESIZE(n)   (sizeof(struct UIKNode) * (n))


/****** Imported ************************************************/


/****** Exported ************************************************/


/****** Statics *************************************************/


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

struct UIKList * __asm UIK_nl_NewList( register __d0 int n )
{
  register struct UIKList *L;

    if (! (L = (struct UIKList *) UIK_MemSpAlloc( sizeof(struct UIKList), MEMF_ANY|MEMF_CLEAR ))) return(0);
    if (! UIK_nl_AllocList( L, n ))
        { UIK_MemSpFree( L ); return(0); }
    return( L );
}

int __asm UIK_nl_AllocList( register __a0 struct UIKList *L, register __d0 int n )
{
    if (L->MaxNodes) return(0);
    L->MaxNodes = L->BlockSize = n;
    L->UNode = (struct UIKNode *) BufAlloc( sizeof(struct UIKNode), UTABLESIZE(L->BlockSize), MEMF_ANY | MEMF_CLEAR );
    return( (int)L->UNode );
}

//--------------------------------------------------------------

struct UIKPList * __asm UIK_nl_NewListP( register __a0 APTR pool, register __d0 int n, register __d1 ULONG memFlags, register __d2 ULONG puddleSize, register __d3 ULONG threshSize )
{
  register struct UIKPList *L;

    if (! (L = (struct UIKPList *) UIK_MemSpAlloc( sizeof(struct UIKPList), MEMF_ANY|MEMF_CLEAR ))) return(0);
    if (! UIK_nl_AllocListP( pool, L, n, memFlags, puddleSize, threshSize ))
        { UIK_nl_FreeList( L ); return(0); }
    return( L );
}

int __asm UIK_nl_AllocListP( register __a0 APTR pool, register __a1 struct UIKPList *L, register __d0 int n, register __d1 ULONG memFlags, register __d2 ULONG puddleSize, register __d3 ULONG threshSize )
{
    if (L->MaxNodes) return(0);
    L->MaxNodes = L->BlockSize = n;

    if (! pool) {
        if (! (pool = AsmCreatePool( memFlags, puddleSize, threshSize, *(APTR*)4 ))) return(0);
        L->Flags |= UIKPLF_POOLOWNER;
        }

    if (! (L->UNode = (struct UIKNode *) BufAllocP( pool, UTABLESIZE(L->BlockSize), sizeof(struct UIKNode) )))
        if (L->Flags & UIKPLF_POOLOWNER)
            {
            AsmDeletePool( pool, *(APTR*)4 );
            L->Flags &= ~UIKPLF_POOLOWNER;
            }

    return( (int)L->UNode );
}

//-----------------------------------------------------------------

void __asm UIK_nl_DeleteList( register __a0 struct UIKList *L )
{
    if (! L) return;
    UIK_nl_FreeList( L );
    UIK_MemSpFree( L );
}

void __asm UIK_nl_AttachList( register __a0 struct UIKList *from,  register __a1 struct UIKList *to )
{
    if (!from || !to) return;
    from->BlockSize = ~0;
    from->UNode = (struct UIKNode *) to;
}

void __asm UIK_nl_DetachList( register __a0 struct UIKList *from, register __d0 ULONG n )
{
    if (from)
        {
        from->MaxNodes = from->BlockSize = n;
        from->UNode = 0;
        }
}

int __asm IntUIK_ExtendList( register __a0 struct UIKList *L )
{
  struct UIKNode *node;

    if (! (node = (struct UIKNode *) BufResize( (UBYTE *) L->UNode, UTABLESIZE(L->MaxNodes+L->BlockSize) ))) return(0);
    L->UNode = node;
    L->MaxNodes += L->BlockSize;
    return(1);
}

UBYTE * __asm UIK_nl_ReAllocNode( register __a0 struct UIKList *L, register __d0 LONG ind, register __d1 LONG len, register __a1 LONG flags )
{
  register UBYTE *ptr;
  register struct UIKNode *node;
  register LONG source;

    L = UIK_nl_ListPtr( L );
    source = L->NodeNum;
    if (ind > source || ind < 0) return(0);
    node = &L->UNode[ind];
    if (ptr = BufResize( node->Buf, len ))
        {
        node->Buf = ptr;
        node->Reserved = len;
        if (flags != 0xffffffff) node->Flags = flags;
        }
    return( ptr );
}

UBYTE * __asm UIK_nl_PasteToNode( register __a0 struct UIKList *L, register __d0 LONG ind,  register __d1 LONG start, register __a1 UBYTE *buf, register __d2 LONG len )
{
  UBYTE *p;
  struct UIKNode *node;
  register LONG source;

    L = UIK_nl_ListPtr( L );
    source = L->NodeNum - 1;
    if (ind > source || ind < 0) ind = source;
    node = &L->UNode[ind];

    if (p = BufPaste( buf, len, node->Buf, start ))
        {
        node->Buf = p;
        node->Reserved = BufLength( p );
        }
    return( p );
}

UBYTE * __asm UIK_nl_CutFromNode( register __a0 struct UIKList *L, register __d0 LONG ind, register __d1 LONG start, register __d2 LONG len )
{
  struct UIKNode *node;
  register LONG source;

    L = UIK_nl_ListPtr( L );
    source = L->NodeNum - 1;
    if (ind > source || ind < 0) ind = source;
    node = &L->UNode[ind];

    node->Buf = BufCut( node->Buf, start, len );
    node->Reserved = BufLength( node->Buf );
    return( node->Buf );
}

//----------- Tri par tas (Heapsort)

static void __asm glisser( register __a0 struct UIKNode *an,
register __a1 int (*func)(),
register __d0 LONG ga,
register __d1 LONG dr )
{
  register LONG i, j;
  struct UIKNode y;

    i = ga; j = 2 * ga; y = an[ga];
    if ((j < dr) && ((*func)( an[j].Buf, an[j+1].Buf ) < 0)) j++;
    while ((j <= dr) && ((*func)( y.Buf, an[j].Buf ) < 0))
        {
        y = an[i]; an[i] = an[j]; an[j] = y;
        i = j; j = 2 * j;
        if ((j < dr) && ((*func)( an[j].Buf, an[j+1].Buf ) < 0)) j++;
        }
}

void __asm UIK_nl_SortList( register __a0 struct UIKList *L, register __a1 int (*func)() )
{
  register LONG G, D, n;
  struct UIKNode x, *a;
  extern int SStrNCCmp();

    if (func == 0) func = SStrNCCmp;
    if ((n = UIK_nl_ListNodeNum( L )) < 2) return;
    a = (UIK_nl_ListPtr( L ))->UNode;
    G = n / 2; D = n - 1;
    while (G) glisser( a, func, --G, D );
    while (D)
        {
        x = a[0]; a[0] = a[D]; a[D] = x;
        glisser( a, func, G, --D );
        }
}
