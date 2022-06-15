#ifndef UIKDEFS_SERIAL

#define UIKDEFS_SERIAL
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif


struct TimerSupp {
    ULONG task;         /* task to signal */
    ULONG mask;         /* event mask (1L<<sig) */
    LONG  sig;          /* signal */
    ULONG alarm;        /* time to signal the task */
    ULONG curr_time;    /* curent time */
    struct Interrupt *inter;
};

struct UIKObjSerial
    {
    struct UIKObj   Obj;

    /*---recieve part */
    UBYTE           *RxBuf;
    LONG            RxLen;
    LONG            RxAct;
    void            (*f_ReadOK)();
    void            (*f_Timeout)();
    void            (*f_ReadERR)();
    void            (*f_Break)();
    BPTR            Capture;

    /*---send part */
    UBYTE           *TxBuf;
    LONG            TxLen;
    void            (*f_SendOK)();
    void            (*f_SendERR)();

    /*---setpar */
    UWORD   sp_baud;
    UWORD   sp_hand;
    UBYTE   sp_rbits;
    UBYTE   sp_wbits;
    UBYTE   sp_sbits;
    UBYTE   sp_parity;
    ULONG   sp_buflen;

    /*---divers */
    ULONG           EventMask;
    struct MsgPort  *wport,*rport,*qport;
    struct IOExtSer *wioser,*rioser,*qioser;
    struct TimerSupp *ts;
    LONG            TimeLim;
    LONG            RetryLim;
    LONG            RetryCurr;
    LONG            InUse;
    LONG            MsgNum;
    UBYTE           BreakChar;
    UBYTE           EndChar;
    UBYTE           Rx;
    UBYTE           pad;
};

/* appel: UIK_CallObjectFunc( obj, UIKFUNC_Serial_ClearSer, 0, 0 );
 */
#define UIKFUNC_Serial_ClearSer         0

/* appel: status = UIK_CallObjectFunc( obj, UIKFUNC_Serial_QuerySer, 0, 0 );
 */
#define UIKFUNC_Serial_QuerySer         4

/* appel: UIK_CallObjectFunc( obj, UIKFUNC_Serial_StartReadSer, 0, 0 );
 */
#define UIKFUNC_Serial_StartReadSer     8

/* appel: UIK_CallObjectFunc( obj, UIKFUNC_Serial_PurgeSer, func, 0 );
 */
#define UIKFUNC_Serial_PurgeSer        12

/* appel: error = UIK_CallObjectFunc( obj, UIKFUNC_Serial_WriteSer, buf, len );
 */
#define UIKFUNC_Serial_WriteSer        16

/* appel: UIK_CallObjectFunc( obj, UIKFUNC_Serial_StartWriteSer, 0, 0 );
 */
#define UIKFUNC_Serial_StartWriteSer   20

/* appel: error = UIK_CallObjectFunc( obj, UIKFUNC_Serial_SetParams, struct TagItem *taglist, 0 );
 */
#define UIKFUNC_Serial_SetParams       24


#define SERIAL_NOPARITY     0
#define SERIAL_EVENPARITY   1
#define SERIAL_ODDPARITY    2

#define SERIAL_XDISABLE     0
#define SERIAL_XENABLE      1
#define SERIAL_WIRE7        2


/*---------Obj Serial Tags---------- */
#define UIKTAG_Serial_Bauds     (UIKTAG_USER|0x0001) /*  */
#define UIKTAG_Serial_ReadBits  (UIKTAG_USER|0x0002) /* 7 ou 8 */
#define UIKTAG_Serial_WriteBits (UIKTAG_USER|0x0003) /* 7 ou 8 */
#define UIKTAG_Serial_StopBits  (UIKTAG_USER|0x0004) /* 1 ou 2 */
#define UIKTAG_Serial_Parity    (UIKTAG_USER|0x0005) /* *SERIAL_NOPARITY* ou SERIAL_EVENPARITY ou SERIAL_ODDPARITY */
#define UIKTAG_Serial_Handshake (UIKTAG_USER|0x0006) /* *SERIAL_XDISABLE* ou SERIAL_XENABLE ou SERIAL_WIRE7 */
#define UIKTAG_Serial_RBufLen   (UIKTAG_USER|0x0007) /* (défaut 4096) taille du buffer de reception */

/*---------Obj Serial Flags---------- */

/*-- */

#endif
