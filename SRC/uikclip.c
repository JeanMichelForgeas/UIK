/****************************************************************
 *
 *      File:      uikclip.c    routines de gestion du clipboard
 *      Project:   UIK
 *
 *      Created:   27-06-88     from CBM example cbio.c
 *
 ****************************************************************/


/****** Includes ************************************************/

#include <devices/clipboard.h>


/****** Imported ************************************************/

extern struct GfxBase * __far GfxBase;
extern struct UIKBase * __far UIKBase;
extern struct DOSBase * __far DOSBase;
extern ULONG __far LocaleBase;
extern ULONG __far IntuitionBase;


/****** Exported ************************************************/


/****** Statics *************************************************/


/****************************************************************
 *
 *      Routines
 *
 ****************************************************************/

LONG func_CBWrite( struct IOClipReq *clip, char *ptr, LONG len )
{
    if (!clip) return(0);
    if (!len) return(0);
    clip->io_Command = CMD_WRITE;
    clip->io_Data    = ptr;
    clip->io_Length  = len;
    //clip->io_Offset  = 0;
    DoIO( (APTR)clip );
    return( (LONG)clip->io_Actual );
}

struct IOClipReq *func_CBWriteStart( LONG unit )
{
  LONG len=0;
  struct MsgPort *port=0;
  struct IOClipReq *clip=0;
  ULONG dev=1;

    if (! (port = (struct MsgPort *) CreatePort( 0, 0 ))) goto END_ERROR2;
    if (! (clip = (struct IOClipReq *) CreateExtIO( port, sizeof(struct IOClipReq) ))) goto END_ERROR2;
    if (dev = OpenDevice( "clipboard.device", unit, (APTR)clip, 0 ) != 0) goto END_ERROR2;

    clip->io_Offset = 0;
    clip->io_ClipID = 0;
    if (func_CBWrite( clip, "FORM", 4 ) < 4) goto END_ERROR;
    if (func_CBWrite( clip, (char*)&len  , 4 ) < 4) goto END_ERROR;
    if (func_CBWrite( clip, "FTXTCHRS", 8 ) < 8) goto END_ERROR;
    if (func_CBWrite( clip, (char*)&len  , 4 ) < 4) goto END_ERROR;
    return( clip );

  END_ERROR:
    clip->io_Command = CMD_UPDATE;
    DoIO( (APTR)clip );
  END_ERROR2:
    if (!dev) CloseDevice( (APTR)clip );
    if (clip) DeleteExtIO( clip );
    if (port) DeletePort( port );
    return(0);
}

void func_CBWriteEnd( struct IOClipReq *clip, LONG len )
{
  LONG real=len;

    if (!clip) return;
    if (len & 1) { func_CBWrite( clip, "", 1 ); len++; }

    //if (func_CBWrite( clip, 16, (char*)&real, 4 ) < 4) goto FIN;
    clip->io_Command = CMD_WRITE;
    clip->io_Data    = (char*)&real;
    clip->io_Length  = 4;
    clip->io_Offset  = 16;
    DoIO( (APTR)clip );
    if (clip->io_Actual == 4)
        {
        len += 12;
        //func_CBWrite( clip, 4, (char*)&len, 4 );
        clip->io_Command = CMD_WRITE;
        clip->io_Data    = (char*)&len;
        clip->io_Length  = 4;
        clip->io_Offset  = 4;
        DoIO( (APTR)clip );
        }
    clip->io_Command = CMD_UPDATE;
    DoIO( (APTR)clip );
    CloseDevice( (APTR)clip );
    DeletePort( clip->io_Message.mn_ReplyPort );
    DeleteExtIO( clip );
}

LONG func_CBRead( struct IOClipReq *clip, char *ptr, LONG len )
{
    if (!clip) return(0);
    clip->io_Command = CMD_READ;
    clip->io_Data    = ptr;
    clip->io_Length  = len;
    //clip->io_Offset  = 0;
    DoIO( (APTR)clip );
    return( (LONG)clip->io_Actual );
}

struct IOClipReq *func_CBReadStart( LONG unit, ULONG *plen ) /* return length of text available */
{
  struct MsgPort *port=0;
  struct IOClipReq *clip=0;
  ULONG dev=1;
  LONG buf[5], len;

    if (! (port = (struct MsgPort *) CreatePort( 0, 0 ))) goto END_ERROR2;
    if (! (clip = (struct IOClipReq *) CreateExtIO( port, sizeof(struct IOClipReq) ))) goto END_ERROR2;
    if (dev = OpenDevice( "clipboard.device", unit, (APTR)clip, 0 ) != 0) goto END_ERROR2;

    clip->io_Offset = 0;
    clip->io_ClipID = 0;
    if (func_CBRead( clip, (char*)&buf[0], 20 ) < 20) goto END_ERROR;
    if (buf[0] != 0x464f524dL) goto END_ERROR; /*FORM*/
    if (buf[2] != 0x46545854L) goto END_ERROR; /*FTXT*/

    buf[0] = buf[3];
    len = buf[4];
    while (TRUE)
        {
        if (buf[0] == 0x43485253L) break;  /*CHRS*/
        if (len & 1) len += 1;
        if (func_CBRead( clip, NULL, len ) < len) goto END_ERROR;
        if (func_CBRead( clip, (char*)&buf[0], 8 ) < 8) goto END_ERROR;
        len = buf[1];
        }
    if (len)
        {
        *plen = len;
        return( clip );
        }

  END_ERROR:
    func_CBRead( clip, NULL, 0x7fffffff ); /* read total length in clip */
    func_CBRead( clip, NULL, 1 );          /* just say ok for me (end of clip) */
  END_ERROR2:
    if (!dev) CloseDevice( (APTR)clip );
    if (clip) DeleteExtIO( clip );
    if (port) DeletePort( port );
    return(0);
}

void func_CBReadEnd( struct IOClipReq *clip ) /* free the access for other process to write */
{
    if (!clip) return;
    func_CBRead( clip, NULL, 0x7fffffff ); /* read total length in clip */
    func_CBRead( clip, NULL, 1 );          /* just say ok for me (end of clip) */
    CloseDevice( (APTR)clip );
    DeletePort( clip->io_Message.mn_ReplyPort );
    DeleteExtIO( clip );
}
