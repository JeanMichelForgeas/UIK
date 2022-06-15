
#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikglobal.h"
#include "uiki:uikbox.h"
#include "uiki:uikcolors.h"
#include "lci:uik_protosint.h"

extern ULONG DOSBase;

static UBYTE buf[100];

void printc( char c )
{
    buf[0] = c;
    Write( Output(), buf, 1 );
}

void printi( int i )
{
    sprintf( buf, "%ld", i );
    Write( Output(), buf, StrLen( buf ) );
}

void printn()
{
    Write( Output(), "\n", 1 );
}

/*-------------------------
#include <intuition/intuitionbase.h>

extern struct IntuitionBase *IntuitionBase;
extern struct GfxBase *GfxBase;

static y=20;

void __asm textvars( register __a0 ULONG src, register __a1 ULONG dst, register __d0 ULONG len )
{
  struct RastPort *srp = &IntuitionBase->FirstScreen->RastPort;

    SetAPen( srp, 3 );
    Move( srp, 0, 20 );
    sprintf( buf, "src = %lx, dst = %lx, len = %ld\n", src, dst, len );
    Text( srp, buf, StrLen(buf) );
}

void __asm printregs( register __d0 UWORD d0, register __d1 UWORD d1 )
{
    kprintf( "(%ld) (%ld) ", d0, d1 );
}


void __asm printval( register __d0 ULONG d0 ) { kprintf( "%lx ", d0 ); }

void __asm printdis( register __d0 UBYTE dis )
{
  struct RastPort *srp = &IntuitionBase->FirstScreen->RastPort;

    sprintf( buf,"%ld",(int)dis);
    Move( srp, 0, y );
    Text( srp, buf, StrLen(buf) );
    if (y < 240) y+=9; else y=20;
}

ULONG savemsg;

void __asm printinvalid( register __a1 struct UIKObj *obj )
{
  struct RastPort *srp = &IntuitionBase->FirstScreen->RastPort;

    if (obj)
        sprintf( buf,"--invalid : class=%lx, obj=%lx, Status=%ld",savemsg,obj,obj->Status);
    else
        sprintf( buf,"--invalid : class=%lx, obj=%lx",savemsg,obj);
    Move( srp, 0, 20 );
    Text( srp, buf, StrLen(buf) );
}

void __asm printzero( register __a2 struct UIKObj *obj, register __a3 int rp )
{
  struct RastPort *srp = &IntuitionBase->FirstScreen->RastPort;

    sprintf( buf,"----zero  : class=%lx, rp=%lx, task=%lx, obj=%lx, Status=%ld",savemsg,rp,FindTask(0),obj,obj->Status);
    Move( srp, 0, 30 );
    Text( srp, buf, StrLen(buf) );
}

void __asm printimpair( register __a2 struct UIKObj *obj, register __a3 int rp )
{
  struct RastPort *srp = &IntuitionBase->FirstScreen->RastPort;

    sprintf( buf,"----impair: rp=%lx, task=%lx, obj=%lx",rp,FindTask(0),obj);
    Move( srp, 0, 50 );
    Text( srp, buf, StrLen(buf) );
}
-----------------*/
