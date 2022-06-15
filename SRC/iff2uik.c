#include <stdio.h>
//#include <exec/types.h>
//#include <graphics/gfx.h>
//#include <libraries/dos.h>
#include <iff/ilbm.h>
#include <iff/readpict.h>
#include <iff/remalloc.h>

struct   GfxBase  *GfxBase;
extern ULONG DOSBase;

void BMap2C( stream, bm, name, base, x, y, width, height, depth, flags, mask )
FILE *stream;
struct BitMap *bm;
UBYTE *name, *base;
WORD x, y;
UWORD width, height, depth;
UBYTE flags, mask;
{
  UBYTE i, j;
  UWORD rows, bpr, d_word, d, r, b;      /* bpr = bytes per row   */

    if ( (bpr=((width&7? width+8 : width)>>3))&1 ) bpr++;  /* word align   */
    rows = height;

    fprintf(stream, "\n%sUWORD %s%sData[%ld][%ld][%ld] = \n   {\n", "static ", "chip ", name, depth+mask, rows, bpr/2 );

    for (d=0; d<depth; d++)
        {
        if (d) fputc('\n', stream);
        for(r=0; r<rows; r++)
            {
            fputs("   ", stream);
            for (b=0; b<bpr; b+=2)
                {
                d_word = *(UWORD *)(base+d*rows*bpr+r*bpr+b);
                fprintf(stream, "0x%04.4x, ", d_word);
                }
            if (!flags)
                {
                fputs("   /* ", stream);
                for (b=0; b<bpr; b+=2)
                    {
                    d_word = *(UWORD *)(base+d*rows*bpr+r*bpr+b);
                    for (i=0xf; i!=0xff; i--) fputc( (d_word & (1<<i)) ? '#':'.', stream );
                    }
                fputs(" */", stream);
                }
            fputc('\n', stream);
            }
        }
    if (mask)
        {
        fputc('\n', stream);
        for(r=0; r<rows; r++)
            {
            fputs("   ", stream);
            for (b=0; b<bpr; b+=2)
                {
                UBYTE *t = base;
                d_word = 0;
                for (j=0; j<depth; j++)
                    {
                    d_word |= *(UWORD *)(t+r*bpr+b);
                    t += rows*bpr;
                    }
                fprintf(stream, "0x%04.4x, ", d_word);
                }
            if (!flags)
                {
                fputs("   /* ", stream);
                for (b=0; b<bpr; b+=2)
                    {
                    UBYTE *t = base;
                    d_word = 0;
                    for (j=0; j<depth; j++)
                        {
                        d_word |= *(UWORD *)(t+r*bpr+b);
                        t += rows*bpr;
                        }
                    for (i=0xf; i!=0xff; i--) fputc( (d_word & (1<<i)) ? '#':'.', stream );
                    }
                fputs(" */", stream);
                }
            fputc('\n', stream);
            }
        }

    fputs("   };\n", stream);

    fprintf( stream, "struct UIKBitMap __far UBM_%s = { ", name );
    if (mask)
        {
        fprintf( stream, "&%sData[%ld][0][0]", name, depth );
        fprintf( stream, ", UIKBMAP_MASKED|UIKBMAP_CONTIG,\n    %d, %d,%d, %d,%d,%d", width, bpr, rows, 0, depth, 0 );
        }
    else{
        fprintf( stream, "NULL" );
        fprintf( stream, ", UIKBMAP_CONTIG,\n    %d, %d,%d, %d,%d,%d", width, bpr, rows, 0, depth, 0 );
        }
    for (d=0; d<depth; d++)
        fprintf( stream, ", &%sData[%ld][0][0]", name, d );
    for (d=depth; d<8; d++)
        fprintf( stream, ", 0" );
    fprintf( stream, " };\n" );
}

/*
_UIK_BaseName
    move.l  a0,a1
.loop
    move.b  (a1)+,d0
    beq.b   .end
    cmp.b   #':',d0
    beq.b   .found
    cmp.b   #'/',d0
    bne.b   .loop
.found
    move.l  a1,a0
    bra.b   .loop
.end
    move.l  a0,d0
    rts
*/
static UBYTE *BaseName( UBYTE *filename )
{
  UBYTE ch, *name = filename;

LOOP:
    ch = *name++;
    if (ch == 0) goto END;
    if (ch == ':') goto FOUND;
    if (ch == '/') goto FOUND;
    goto LOOP;
FOUND:
    filename = name;
    goto LOOP;
END:
    return( filename );
}

void main( argc, argv )
UBYTE argc, **argv;
{
  UBYTE arg=1, flags=0, mask=1;
  LONG file;
  IFFP iffp = NO_FILE;
  ILBMFrame iFrame;
  struct BitMap bitmap;

    if (!(GfxBase = (struct GfxBase *)OpenLibrary("graphics.library", 0))) exit(1);

    if (argc==1)
        {
        fputs("Usage: IFF2UIK [-c] [-m] <file> [<file>...]\n", stderr);
        fputs("       -c: switches comment output, default on\n", stderr);
        fputs("       -m: switches mask plane output, default on\n", stderr);
        goto END_CLEANUP;
        }

    fprintf( stdout, "#include \"uiki:uikgfx.h\"\n" );

    while (arg < argc)
        {
        switch(argv[arg][0])
            {
            case '-':
                if (argc==2)
                    {
                    fputs("IFF2UIK: input file name not specified\n", stderr);
                    goto END_CLEANUP;
                    }

                switch(argv[arg][1])
                    {
                    case 'm': mask = 0; break;
                    case 'c': flags = 1; break;
                    default:
                       fputs("IFF2UIK: invalid arguments, try IFF2UIK for help\n", stderr);
                       goto END_CLEANUP;
                    }
                 break;

            default:
                if (file = Open(argv[arg], MODE_OLDFILE))
                    {
                    iffp = ReadPicture(file, &bitmap, &iFrame, ChipAlloc);
                    Close(file);
                    }
                else
                    {
                    fprintf(stderr, "IFF2UIK: wrong file name %s\n", argv[arg]);
                    goto END_CLEANUP;
                    }
                if (iffp != IFF_DONE)
                    {
                    fprintf(stderr, "IFF2UIK: wrong file format %s\n", argv[arg]);
                    goto END_CLEANUP;
                    }

                BMap2C( stdout, &bitmap, BaseName( argv[arg] ), bitmap.Planes[0],
                        iFrame.bmHdr.x, iFrame.bmHdr.y,
                        iFrame.bmHdr.w, iFrame.bmHdr.h,
                        bitmap.Depth, flags, mask );

                if (bitmap.Planes[0]) RemFree(bitmap.Planes[0]);
                break;
            }
        arg++;
        }

    fprintf( stdout, "\n\nstruct UIKBitMap * __far UIKBitMapTable[] = {\n" );
    arg = 1;
    while (arg < argc)
        {
        switch(argv[arg][0])
            {
            case '-': break;

            default:
                fprintf( stdout, "    &UBM_%s,\n", BaseName( argv[arg] ) );
                break;
            }
        arg++;
        }
    fprintf( stdout, "    0\n    };\n" );

  END_CLEANUP:
    CloseLibrary(GfxBase);
    exit(0);
}
