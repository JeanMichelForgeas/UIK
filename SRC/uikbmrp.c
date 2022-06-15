/**********************************************************************
 *
 *   fichier:  uikbmrp.c
 *
 *   fonction: Alloc & Free de BitMap + RPort
 *
 *   creation: 08-Avr-89  jm forgeas
 *   revision: 08-Jan-92  jm forgeas pour UIK
 *
 **********************************************************************/


/******* Included Files ***********************************************/

#include "uiki:uikgfx.h"

#include "lci:uik_protosint.h"

#include <stddef.h>


/****** Imported ************************************************/

extern struct GfxBase * __far GfxBase;


/****** Exported ************************************************/


/**********************************************************************
 *
 *  Routines
 *
 **********************************************************************/

void __asm UIK_FreeRPort( register __a0 struct RastPort *rport )
{
    UIK_MemSpFree( rport );
}

struct RastPort * __asm UIK_AllocRPort( void )
{
  struct RastPort *rport;

    if (rport = (struct RastPort *) UIK_MemSpAlloc( sizeof(struct RastPort), MEMF_ANY | MEMF_CLEAR))
        InitRastPort( rport );
    return( rport );
}

void __asm UIK_FreeBMap( register __a0 struct BitMap *bitmap )
{
  UBYTE i;
  struct UIKBitMap *ubm;

    if (!bitmap) return;
    ubm = (struct UIKBitMap *) (((ULONG)bitmap) - offsetof(struct UIKBitMap,BytesPerRow));
    if (! (ubm->UFlags & UIKBMAP_NOPLANE))
        {
        if (ubm->UFlags & UIKBMAP_CONTIG) UIK_MemSpFree( ubm->Planes[0] );
        else for (i=0; i<ubm->Depth; i++) UIK_MemSpFree( ubm->Planes[i] );
        UIK_MemSpFree( ubm->Mask );
        }
    UIK_MemSpFree( ubm );
}

struct BitMap * __asm UIK_AllocBMap( register __d0 UBYTE depth,
register __d1 UWORD width, register __a0 UWORD height, register __a1 UWORD UFlags )
{
  struct UIKBitMap *ubm;
  struct BitMap *bitmap=0;
  UWORD size, i;
  UBYTE *ptr;
  ULONG reqs = (UFlags & UIKBMAP_PUBLIC ? MEMF_PUBLIC : MEMF_CHIP) | MEMF_CLEAR;

    if ((ubm = (struct UIKBitMap *) UIK_MemSpAlloc( sizeof(struct UIKBitMap), MEMF_ANY | MEMF_CLEAR)))
        {
        ubm->UFlags = UFlags;
        ubm->Width = width;
        bitmap = (struct BitMap *) (((ULONG)ubm) + offsetof(struct UIKBitMap,BytesPerRow));
        InitBitMap( bitmap, depth, width, height );
        if (! (ubm->UFlags & UIKBMAP_NOPLANE))
            {
            size = bitmap->BytesPerRow * bitmap->Rows;
            if (UFlags & UIKBMAP_CONTIG)
                {
                if (! (ptr = UIK_MemSpAlloc( size * depth, reqs ))) return(0);
                for (i=0; i<depth; i++) { bitmap->Planes[i] = ptr; ptr += size; }
                }
            else{
                for (i=0; i<depth; i++)
                    {
                    if (! (bitmap->Planes[i] = (PLANEPTR) UIK_MemSpAlloc( size, reqs )))
                        {
                        UIK_FreeBMap( bitmap );
                        return(0);
                        }
                    }
                }
            if (UFlags & UIKBMAP_MASKED)
                {
                if (! (ubm->Mask = UIK_MemSpAlloc( size, MEMF_CHIP | MEMF_CLEAR ))) return(0);
                }
            }
        }
    return( bitmap );
}

void __asm UIK_FreeBMapRPort( register __a0 struct RastPort *rport )
{
    if (rport)
        {
        UIK_FreeBMap( rport->BitMap );
        UIK_FreeRPort( rport );
        }
}


struct RastPort * __asm UIK_AllocBMapRPort( register __d0 UBYTE depth,
register __d1 UWORD width, register __a0 UWORD height, register __a1 UWORD UFlags )
{
  struct RastPort *rport=0;

    if (rport = UIK_AllocRPort())
        {
        if (! (rport->BitMap = UIK_AllocBMap( depth, width, height, UFlags )))
            {
            UIK_FreeBMapRPort( rport );
            rport = 0;
            }
        }
    return( rport );
}


