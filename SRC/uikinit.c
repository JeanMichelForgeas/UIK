/****************************************************************
 *
 *      Project:   UIK
 *      Function:  init
 *
 *      Created:   21/01/91    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikbase.h"
#include "uiki:uikglobal.h"
#include "uiki:uikobj.h"
#include "uiki:uikbox.h"
#include "uiki:uikgfx.h"
#include "lci:uik_protosint.h"

#include "uiki:objwindow.h"
#include "uiki:objmenu.h"
#include "uiki:objstring.h"

#include <clib/locale_protos.h>
#include <pragmas/locale_pragmas.h>


/****** Macros **************************************************/


/****** Imported ************************************************/

extern void __asm IntUIK_CloseLocale( register __a0 struct Locale *locale );

extern struct GfxBase * __far GfxBase;
extern struct UIKBase * __far UIKBase;
extern struct DOSBase * __far DOSBase;
extern ULONG __far LocaleBase;
extern ULONG __far IntuitionBase;

extern struct UIKBitMap __far UBM_Direc, __far UBM_CranP;


/****** Exported ************************************************/

ULONG __far SysBase;


/****** Statics *************************************************/


/****************************************************************
 *
 *      UIKBase
 *
 ****************************************************************/

/*
extern void * __asm AsmAllocPooled(register __a0 void *, register __d0 ULONG, register __a6 APTR);
extern void __asm AsmFreePooled(register __a0 void *, register __a1 void *, register __d0 ULONG, register __a6 APTR);

APTR __asm MWAsmAllocPooled( register __a0 APTR pool, register __d0 ULONG memsize )
{
  ULONG i;
  UBYTE *ptr, *mem;

    if (! (mem = AsmAllocPooled( pool, 16 + memsize + 16, (APTR)*((ULONG*)4) ))) return(0);
    ptr = mem;
    for (i=0; i < 16; i++) *ptr++ = 0xA5;
    ptr = mem + 16 + memsize;
    for (i=0; i < 16; i++) *ptr++ = 0xA5;
    return( mem + 16 );
}

static void test_mem( APTR memory, UBYTE *mem, ULONG len, ULONG before, ULONG memsize )
{
  ULONG i=0, changed=0;
  UBYTE *ptr=mem;

    if (before)
        { for (; i < len; i++, ptr++) { if (*ptr != 0xA5) changed++; } }
    else{ for (ptr+=len-1; i < len; i++, ptr--) { if (*ptr != 0xA5) changed++; } }

    if (changed)
        {
        kprintf( "\nMWAsmFreePooled: %ld bytes %ls allocation at 0x%lx, size %ld were hit!\n", changed, before?"before":"after", memory, memsize );
        kprintf( "%08.8lx: ", mem );
        for (ptr=mem, i=0; i < len; i++) kprintf( "%02.2lx", ptr[i] );
        kprintf( "\n" );
        }
}

void __asm MWAsmFreePooled( register __a0 APTR pool, register __a1 APTR memory, register __d0 ULONG memsize )
{
  ULONG i, totlen;
  UBYTE *mem;

    totlen = 16 + memsize + 16;
    mem = (UBYTE*)memory - 16;
    test_mem( memory, mem, 16, 1, memsize );
    test_mem( memory, mem + 16 + memsize, 16, 0, memsize );
    for( i=0; i < totlen; ) { mem[i++] = 0xDE; mem[i++] = 0xAD; mem[i++] = 0xBE; mem[i++] = 0xEF; }
    AsmFreePooled( pool, mem, totlen, (APTR)*((ULONG*)4) );
}
*/

//-----------------------------------------------------------------

/*
void __asm debug_std_line_rect( register __a0 UBYTE *a, register __a1 UBYTE *b, register __d0 ULONG l )
{
    if (UIKBase->UIKPrefs->Padw)
        {
        kprintf( "%ld %lx %lx %ld\n", UIKBase->padw, a, b, l );
        Delay(50);
        }
}
*/

LONG __far NumScreens;

static void ClosePubScreens( void )
{
  struct List *list;
  struct PubScreenNode *node;
  struct Task *me;
  struct Screen *sc=0;
  struct UIKObj *obj, *uik;

    me = FindTask(0);

    if (list = LockPubScreenList())
        {
        for (node=(struct PubScreenNode *)list->lh_Head; node->psn_Node.ln_Succ; node = (struct PubScreenNode *)node->psn_Node.ln_Succ)
            {
            if (node->psn_SigBit == SIGBREAKB_CTRL_F && node->psn_SigTask == me && node->psn_VisitorCount == 0)
                {
                sc = node->psn_Screen;
                break;
                }
            }
        UnlockPubScreenList();
        }

    if (sc)
        {
        if (sc == node->psn_Screen)
            {
            Forbid();
            if (screen_exists( sc, 0 ))
                {
                obj = (struct UIKObj *)sc->UserData;
                if (obj && !(((ULONG)obj) & 1))
                    {
                    if (uik = obj->UIK)
                        {
                        if (uik && !(((ULONG)uik) & 1))
                            {
                            if (uik == uik->UIK) // alors objet valide
                                {
                                if (obj->Status == UIKS_STARTED)
                                    {
                                    sc = 0;
                                    }
                                }
                            }
                        }
                    }
                if (sc)
                    {
                    CloseScreen( sc );
                    NumScreens--;
//kprintf( "ClosePubScreens() : NumScreens-- %ld\n", NumScreens );
                    }
                }
            if (sc == UIKBase->DefaultScreen) UIKBase->DefaultScreen = 0;
            Permit();
            }
        }
}

static void ClosePubScreensTask( void )
{
  ULONG events, cont=1;

    while (TRUE)
        {
//kprintf( "Wait...\n" );
        events = Wait( SIGBREAKF_CTRL_C | SIGBREAKF_CTRL_D | SIGBREAKF_CTRL_E | SIGBREAKF_CTRL_F );

        if (events & SIGBREAKF_CTRL_E)
            {
            NumScreens++;
//kprintf( "SIGBREAKF_CTRL_E : NumScreens++ %ld\n", NumScreens );
            }

        if (events & SIGBREAKF_CTRL_F)
            {
            ClosePubScreens();
//kprintf( "SIGBREAKF_CTRL_F : NumScreens=%ld\n", NumScreens );
            if (cont == 0 && NumScreens == 0) break;
            }

        if (events & SIGBREAKF_CTRL_D)
            {
            NumScreens--;
//kprintf( "SIGBREAKF_CTRL_D : NumScreens-- %ld\n", NumScreens );
            if (cont == 0 && NumScreens == 0) break;
            }

        if (events & SIGBREAKF_CTRL_C)
            {
//kprintf( "SIGBREAKF_CTRL_C : NumScreens=%ld\n", NumScreens );
            if (NumScreens == 0) break;
            else cont = 0;
            }
        }
//kprintf( "Ended\n" );
    UIKBase->ScrPubTask = 0;
}

void AddPubScrTask( void )
{
    SysBase = *((ULONG*)4);
    UIKBase->ScrPubTask = (struct Task *) CreateTask( "UIK_PublicScreensManager", 0, ClosePubScreensTask, 4096 );
}

void RemPubScrTask( void )
{
    if (UIKBase->ScrPubTask)
        {
        SetTaskPri( UIKBase->ScrPubTask, 127 );
//kprintf( "-- Signaling PubTask Ctrl+C\n" );
        Signal( UIKBase->ScrPubTask, SIGBREAKF_CTRL_C );
//kprintf( "-- Waiting for PubTask\n" );
        while (UIKBase->ScrPubTask)
            {
//kprintf( "> Waiting for PubTask..........\n" );
            Delay(1);
            }
//kprintf( "-- PubTask ended ok\n" );
        }
}

/****************************************************************
 *
 *      UIKGlobal
 *
 ****************************************************************/

int __asm InitUIK( register __a0 struct UIKGlobal *uik )
{
    if (OpenDevice( "console.device", -1, &uik->KeySupp.ConsoleMsg, 0 )) return(0);
    if (OpenDevice( "console.device", -1, &uik->SCutSupp.ConsoleMsg, 0 )) return(0);
    return(1);
}

void __asm CleanupUIK( register __a0 struct UIKGlobal *uik )
{
  register struct IOStdReq *io;

    io = &uik->KeySupp.ConsoleMsg;
    if (io->io_Device) CloseDevice( io );
    io->io_Device = 0;
    io = &uik->SCutSupp.ConsoleMsg;
    if (io->io_Device) CloseDevice( io );
    io->io_Device = 0;
}

//-----------------------------------------------------------------------------------------

static void __asm free_one_langage( register __a0 UBYTE **tab )
{
    if (tab)
        {
        UBYTE **ptr; for (ptr=tab; *ptr; ptr++) UIK_MemSpFree( *ptr );
        UIK_MemSpFree( tab );
        }
}

void FreeLangage( struct UIKObj *obj )
{
  struct UIKLang *lang = obj->ObjExt->Lang;

    if (lang->Catalog) CloseCatalog( lang->Catalog );
    lang->Catalog = 0;

//kprintf( "FreeLangage... " );
    if (lang->Flags & UIKLANG_OWNER)
        {
//kprintf( "Freed\n" );
        free_one_langage( lang->English );
        free_one_langage( lang->Other );
        }
    lang->Langage = lang->English = lang->Other = 0;
}

void Remove2( struct UIKGlobal *uik )
{
//kprintf( "Remove2\n" );
    FreeLangage( uik );
    IntUIK_CloseLocale( uik->Locale ); uik->Locale = 0;
    UIK_RemoveAll( uik );
}

//------------------

extern ULONG __asm langage_num( register __a0 UBYTE **tab );

static UBYTE ** __asm init_one_langage( register __a0 UBYTE **tab )
{
  UBYTE *ptr, *np, **new=0, **newtab;
  ULONG num, len;

    if (! tab) return(0);
    for (num=0; tab[num]; num++);

    if (newtab = new = (UBYTE **) UIK_MemSpAlloc( (num+1) * 4, MEMF_ANY | MEMF_CLEAR ))
        {
        while (ptr = *tab++)
            {
            len = StrLen( ptr );
            if (! (np = UIK_MemSpAlloc( len + 1, MEMF_ANY | MEMF_CLEAR ))) return(0);
            *new++ = np;
            StrCpy( np, ptr );
            }
        }
    return( newtab );
}

ULONG AllocLangage( struct UIKObj *obj, struct TagItem *taglist )
{
  struct UIKLang *lang = obj->ObjExt->Lang;
  APTR english, other;

    lang->Langage = lang->English = lang->Other = 0; // cause compatibilité avec UIK_Install()
    english = (UBYTE **) UIK_GetTagData( UIKTAG_GEN_LangEnglish, 0, taglist );
    other = (UBYTE **) UIK_GetTagData( UIKTAG_GEN_LangOther, 0, taglist );
    if (english || other)
        {
        lang->Flags |= UIKLANG_OWNER;
        if (lang->English = init_one_langage( english ))
            {
            if (lang->Other = init_one_langage( other ))
                {
                lang->Langage = (UIKBase->UIKPrefs->PrefsFlags & UIKLANG_OTHER) ? lang->Other : lang->English;
                }
            }
        if (! lang->Langage) goto END_ERROR;
        }

    if (LocaleBase && (lang->CatName = (UBYTE *) UIK_GetTagData( UIKTAG_GEN_Catalog, 0, taglist )))
        {
        lang->Catalog = OpenCatalogA( NULL, lang->CatName, NULL );
        }

    if (english || lang->Catalog) lang->Master = obj;
    return(1);

  END_ERROR:
    FreeLangage( obj );
    return(0);
}

struct UIKGlobal *Install2( struct TagItem *taglist )
{
  struct UIKGlobal *uik=0;
  APTR english, other;

    english = (UBYTE **) UIK_GetTagData( UIKTAG_GEN_LangEnglish, 0, taglist );
    other = (UBYTE **) UIK_GetTagData( UIKTAG_GEN_LangOther, 0, taglist );
    if (! (uik = UIK_Install( english, other ))) goto END_ERROR;

    if (! (uik->Locale = (struct Locale *) IntUIK_OpenLocale()))
        { if (LocaleBase) goto END_ERROR; }

    if (UIK_GetTagData( UIKTAG_GEN_LangDuplicate, TRUE, taglist ))
        if (! AllocLangage( (struct UIKObj *) uik, taglist )) goto END_ERROR;

    uik->FilterIE = UIK_GetTagData( UIKTAG_GEN_FilterIEventsFunc, 0, taglist );

    return( uik );

  END_ERROR:
    if (uik) UIK_RemoveAll( uik );
    return(0);
}

//-----------------------------------------------------------------------------------------

struct UIKTaskNode
    {
    struct  UIKTaskNode *ln_Succ;
    struct  UIKTaskNode *ln_Pred;
    APTR    Task;
    APTR    UIK;
    };


void __asm RemoveTaskList( register __a0 struct Task *task )
{
  struct UIKTaskNode *node;

    Forbid();
    for (node=(struct UIKTaskNode*)UIKBase->TaskList.lh_Head; node->ln_Succ; node = node->ln_Succ)
        {
        if (node->Task == (APTR)task)
            {
            Remove( (struct Node *)node );
            FreeMem( node, sizeof(struct UIKTaskNode) );
            break;
            }
        }
    Permit();
}

void __asm AddTaskList( register __a0 struct Task *task, register __a1 struct UIKGlobal *uik )
{
  struct UIKTaskNode *node;

    if (node = (APTR) AllocMem( sizeof(struct UIKTaskNode), MEMF_ANY | MEMF_CLEAR ))
        {
        node->Task = task;
        node->UIK = uik;
        Forbid();
        AddTail( &UIKBase->TaskList, (struct Node *)node );
        Permit();
        }
}

//-----------------------------------------------------------------------------------------

struct IntuiMessage * __asm menukey_create_msg( register __a0 struct UIKGlobal *uik, register __a1 struct UIKObjStrGad *ostr, register __a3 struct IntuiMessage *imsg )
{
  struct IntuiMessage *msg;
  struct Window *w = imsg->IDCMPWindow;

//    if (w == ostr->Obj.w && w->UserPort == &uik->UPort) // bien la fenêtre du String, et une fenêtre UIK
        {
        if (msg = (struct IntuiMessage *) UIK_MemSpAlloc( 4+sizeof(struct IntuiMessage), MEMF_PUBLIC|MEMF_CLEAR ))
            {
            CopyMem( imsg, msg, sizeof(struct IntuiMessage) );
            msg->ExecMessage.mn_ReplyPort = 0; // sera effacé par le process destinataire
            msg->Code     &= 0x7f;
//            msg->Qualifier = 0x8080; // IEQUALIFIER_RELATIVEMOUSE | IEQUALIFIER_RCOMMAND
            msg->IAddress  = (APTR) (((ULONG)msg) + sizeof(struct IntuiMessage));
            Disable();
            AddHead( &uik->UPort.mp_MsgList, msg );
            Enable();
            return( msg );
            }
        }
    return(0);
}
