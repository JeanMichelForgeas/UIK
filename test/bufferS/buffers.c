
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
        UBYTE *buf1=0, *buf2=0;

        if (buf1 = BufAlloc( 10, StrLen( str1 ), MEMF_PUBLIC ))
            {
            printf( "Alloc ok: %ld\n", BufLength( buf1 ) );
            StrCpy( buf1, str1 );
            if (BufCopyS( buf1, &buf2, 3, 11 ))
                {
                printf( "copy: abcdefghijk '%ls'\n", buf2 );
                if (BufCutS( &buf2, 3, 4 ))
                    {
                    printf( "cut: abchijk '%ls'\n", buf2 );
                    if (BufPasteS( str2, 3, &buf2, 3 ))
                        {
                        printf( "paste: abcDEFhijk '%ls'\n", buf2 );
                        if (BufResizeS( &buf2, 4 ))
                            {
                            printf( "resize: abcD '%ls'\n", buf2 );
                            if (BufPrintf( &buf2, -1, "(%ls%ld)", "num=", 10 ))
                                {
                                printf( "BufPrintf: abcD(num=10) '%ls'\n", buf2 );
                                }
                            }
                        }
                    }
                }
            }
        BufFreeS( &buf1 );
        BufFreeS( &buf2 );
        CloseLibrary( UIKBase );
        }
}
