
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

#include <exec/execbase.h>
#include <clib/exec_protos.h>

#define UIK_LIBNAME "uik.library"

extern struct ExecBase * __far SysBase;

void main( int argc, char **argv )
{
  struct Node *here;
  struct Library *uik;
  ULONG force=0;

    if (argc == 2)
        {
        if (!strcmp( argv[1], "FORCE" ) || !strcmp( argv[1], "force" )) force = 1;
        }

    Forbid();
    here = FindName( &SysBase->LibList, UIK_LIBNAME );
    if (force) Remove( here );
    Permit();

    if (force)
        {
        printf( "%ls still in memory but unknown for System\n", UIK_LIBNAME );
        }
    else if (here)
        {
        if (! (uik = OpenLibrary( UIK_LIBNAME, 0 )))
            {
            printf( "Cannot open %ls\n", UIK_LIBNAME );
            exit(20);
            }
        RemLibrary( uik );
        CloseLibrary( uik );

        Forbid();
        if (here = FindName( &SysBase->LibList, UIK_LIBNAME ))
        RemLibrary( here );
        Permit();

        Forbid();
        here = FindName( &SysBase->LibList, UIK_LIBNAME );
        Permit();
        if (! here) printf( "%ls FLUSHED\n", UIK_LIBNAME );
        }

    exit(0);
}
