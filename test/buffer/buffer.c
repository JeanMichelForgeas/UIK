
#include <stdio.h>

#ifdef LATTICE
int CXBRK(void) { return(0); }
int chkabort(void) { return(0); }
#endif
#ifdef SAS
int CXBRK    ( void ) { return(0); }
int _CXBRK   ( void ) { return(0); }
int chkabort ( void ) { return(0); }
#endif

#include <exec/memory.h>
#include <libraries/dos.h>

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikbuf.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

extern ULONG DOSBase;
struct UIKBase *UIKBase;
struct UIKGlobal *UIK;

static UBYTE *str1 = "123abcdefghijk456";
static UBYTE *str2 = "DEF";

void main()
{
    if (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))
        {
        UBYTE *buf1=0, *buf2=0, *b;

        if (buf1 = BufAlloc( 10, StrLen( str1 ), MEMF_PUBLIC ))
            {
            printf( "Alloc ok: %ld\n", BufLength( buf1 ) );
            StrCpy( buf1, str1 );
            if (b = BufCopy( buf1, 0, 3, 11 ))
                {
                buf2 = b;
                printf( "copy: abcdefghijk '%ls'\n", buf2 );
                if (b = BufCut( buf2, 3, 4 ))
                    {
                    buf2 = b;
                    printf( "cut: abchijk '%ls'\n", buf2 );
                    if (b = BufPaste( str2, 3, buf2, 3 ))
                        {
                        buf2 = b;
                        printf( "paste: abcDEFhijk '%ls'\n", buf2 );
                        if (b = BufResize( buf2, 4 ))
                            {
                            buf2 = b;
                            printf( "resize: abcD '%ls'\n", buf2 );
                            if (b = BufClone( buf2 ))
                                {
                                printf( "clone: abcD '%ls'\n", b );
                                BufFree( b );
                                if (BufPrintFStack( &buf2, 2, "(%ls%ld)", "num=", 10 ))
                                    {
                                    printf( "BufPrintF: ab(num=10)cD '%ls'\n", buf2 );
                                    }
                                }
                            }
                        }
                    }
                }
            }
        BufFree( buf1 );
        BufFree( buf2 );
        CloseLibrary( UIKBase );
        }
}
