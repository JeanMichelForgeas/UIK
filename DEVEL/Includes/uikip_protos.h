#ifndef  CLIB_IP_PROTOS_H
#define  CLIB_IP_PROTOS_H

#ifndef CLIB_UIK_BASE_H
#include "uiki:uikbase.h"
#endif
#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif
#ifndef UIKDEFS_MACRO
#include "uiki:uikmacros.h"
#endif
#ifndef UIKDEFS_GLOBAL
#include "uiki:uikglobal.h"
#endif

/* "uik.library" */
/* */
/*      (C) Copyright 1991,1992,1993 Jean-Michel Forgeas" */
/*                  Tous Droits Réservés" */
/* */
/* */
/* First function is always at an offset of -30 from lib base */
/******** */
/* ------------------------------public------------------------------- */
extern int IPUIK_IsSystemVersion2( struct Task * );
extern int IPUIK_DeadKeyConvert( struct Task *, struct UIKKeySupp *uks, struct IntuiMessage *imsg, struct KeyMap *keymap );
extern void IPUIK_InvertBlackWhite( struct Task *, struct UIKGlobal *uik, struct UIKBitMap **table, UBYTE iv );
extern void IPUIK_EnableDOSReq( struct Task *, ULONG null, ULONG old );
extern ULONG IPUIK_DisableDOSReq( struct Task * );
extern STRPTR IPUIK_BaseName( struct Task *, STRPTR pathname );
extern struct TextFont * IPUIK_OpenFont( struct Task *, STRPTR name, UWORD height );
extern struct TextFont * IPUIK_ReopenFont( struct Task *, struct TextFont *font );
extern ULONG IPUIK_AccessPrefs( struct Task *, int (*func)(), ULONG parm1, ULONG parm2 );
extern ULONG IPUIK_ReadPrefs( struct Task * );
extern ULONG IPUIK_SavePrefs( struct Task * );
extern void IPUIK_GetPrefs( struct Task *, struct UIKPrefs *buf, int len );
extern void IPUIK_SetPrefs( struct Task *, struct UIKPrefs *buf, int len );
/* */
/* */
extern void IPUIK_Disable( struct Task *, struct UIKGlobal *uik );
extern void IPUIK_Enable( struct Task *, struct UIKGlobal *uik );
/* */
extern int IPUIK_InsideObj( struct Task *, struct UIKObj *obj, ULONG xy );
extern void IPUIK_LockObjVars( struct Task *, struct UIKObj *obj );
extern void IPUIK_UnlockObjVars( struct Task *, struct UIKObj *obj );
extern STRPTR IPUIK_ObjTitle( struct Task *, struct UIKObj *obj );
extern STRPTR IPUIK_ObjAltTitle( struct Task *, struct UIKObj *obj );
extern STRPTR IPUIK_LangString( struct Task *, struct UIKObj *obj, ULONG index );
extern STRPTR IPUIK_OMe_MemPtr( struct Task *, struct UIKObj *obj );
/* */
extern struct UIKHook * IPUIK_AddVector( struct Task *, struct UIKObj *obj, void (*vect)(), ULONG mask, ULONG parm );
extern void IPUIK_RemoveVector( struct Task *, struct UIKObj *obj, struct UIKHook *hook );
extern void IPUIK_RemAllVectors( struct Task *, struct UIKObj *obj );
extern void IPUIK_Stop( struct Task *, struct UIKObj *obj );
extern void IPUIK_Remove( struct Task *, struct UIKObj *obj );
extern void IPUIK_Refresh( struct Task *, struct UIKObj *obj, int n );
extern void IPUIK_RefreshWindow( struct Task *, struct UIKObj *obj );
extern void IPUIK_CauseObjHook( struct Task *, struct UIKObj *obj, ULONG intuiclass );
extern void IPUIK_CauseObjLink( struct Task *, struct UIKObj *obj, ULONG intuiclass );
extern int IPUIK_Link( struct Task *, struct UIKObj *obj1, struct UIKObj *obj2, int (*func)(), ULONG intuiclass, ULONG parm );
extern int IPUIK_Start( struct Task *,struct UIKObj *obj );
extern ULONG IPUIK_Do( struct Task *, struct UIKObj *obj, ULONG EXTEVENT );
/* */
extern struct UIKObj * IPUIK_AddObject( struct Task *, STRPTR ObjName, struct UIKObj *Parent, struct TagItem *taglist );
extern int IPUIK_GetObjectValues( struct Task *, struct UIKObj *Obj, struct TagItem *taglist );
extern int IPUIK_SetObjectValues( struct Task *, struct UIKObj *Obj, struct TagItem *taglist );
/* */
extern void IPUIK_OPr_ChangePropGadget( struct Task *, struct UIKObjPropGad *obj, int Total, int Visible, int Position );
extern void IPUIK_OPr_ChangePropNoDraw( struct Task *, struct UIKObjPropGad *obj, int Total, int Visible, int Position );
/* */
extern void IPUIK_OLi_DrawList( struct Task *, struct UIKObj *obj );
extern void IPUIK_OLi_DrawListFrom( struct Task *, struct UIKObj *obj, LONG from, LONG num );
extern void IPUIK_OLi_DrawLine( struct Task *, struct UIKObj *obj, LONG line );
/* */
extern struct UIKList * IPUIK_nl_NewList( struct Task *, int n );
extern struct UIKList * IPUIK_nl_ListPtr( struct Task *, struct UIKList *L );
extern void IPUIK_nl_FreeList( struct Task *, struct UIKList *L );
extern void IPUIK_nl_DeleteList( struct Task *, struct UIKList *L );
extern void IPUIK_nl_AttachList( struct Task *, struct UIKList *from, struct UIKList *to );
extern void IPUIK_nl_DetachList( struct Task *, struct UIKList *from, ULONG n );
extern void IPUIK_nl_FreeNodes( struct Task *, struct UIKList *L, LONG from, LONG to );
extern void IPUIK_nl_SortList( struct Task *, struct UIKList *L, int (*sortfunc)() );
extern STRPTR IPUIK_nl_PasteToNode( struct Task *, struct UIKList *L, LONG ind, LONG start, STRPTR buf, LONG len );
extern STRPTR IPUIK_nl_CutFromNode( struct Task *, struct UIKList *L, LONG ind, LONG start, LONG len );
/* */
extern ULONG IPUIK_nl_ListNodeNum( struct Task *, struct UIKList *L );
extern ULONG IPUIK_nl_NodeBufLen( struct Task *, struct UIKList *L, LONG ind );
extern int IPUIK_nl_CrossList( struct Task *, struct UIKList *L, int (*func)(), int wret, ULONG parm );
extern int IPUIK_nl_AllocList( struct Task *, struct UIKList *L, int n );
extern LONG IPUIK_nl_FindNode( struct Task *, struct UIKList *L, STRPTR buf );
extern STRPTR IPUIK_nl_AllocNode( struct Task *, struct UIKList *L, LONG ind, LONG len, WORD flags );
extern STRPTR IPUIK_nl_ReAllocNode( struct Task *, struct UIKList *L, LONG ind, LONG len, LONG flags );
extern STRPTR IPUIK_nl_NodeBuf( struct Task *, struct UIKList *L, LONG ind );
/* */
extern int IPUIK_OLi_ChangeLine( struct Task *, struct UIKObjList *lobj, STRPTR str, LONG line, WORD flags );
extern int IPUIK_OLi_AddLineLen( struct Task *, struct UIKObjList *lobj, STRPTR str, LONG line, WORD flags, LONG len );
extern int IPUIK_OLi_AddLine( struct Task *, struct UIKObjList *lobj, STRPTR str, LONG line, WORD flags );
extern int IPUIK_OLi_TotLines( struct Task *, struct UIKObjList *lobj );
extern int IPUIK_OLi_NewText( struct Task *, struct UIKObjList *lobj, struct TextEntry *te );
extern void IPUIK_OLi_DelLine( struct Task *, struct UIKObjList *lobj, LONG line );
extern void IPUIK_OLi_UnselectList( struct Task *, struct UIKObjList *lobj );
extern void IPUIK_OLi_SelectList( struct Task *, struct UIKObjList *lobj, LONG line );
extern void IPUIK_OLi_ActualizeList( struct Task *, struct UIKObjList *lobj );
extern void IPUIK_OLi_SetListTop( struct Task *, struct UIKObjList *lobj, LONG line );
extern void IPUIK_OLi_MoveListFrame( struct Task *, struct UIKObjList *list, LONG n );
extern void IPUIK_OLi_MoveListSel( struct Task *, struct UIKObjList *list, LONG n );
extern STRPTR IPUIK_OLi_LineBuffer( struct Task *, struct UIKObjList *lobj, LONG line );
/* */
extern int IPStrNCCmp( struct Task *, char *s1, char *s2 );
extern int IPStcCpy( struct Task *, char *dest, char *source, int len );
extern int IPStr2Num( struct Task *, char *s );
extern int IPStrB2C( struct Task *, ULONG bs, char *cs );
extern void IPStrCat( struct Task *, char *dest, char *source );
extern int IPStrCmp( struct Task *, char *s1, char *s2 );
extern char * IPStrCpy( struct Task *, char *dest, char *source );
extern int IPStrLen( struct Task *, char *s );
/* */
extern struct TagItem  * IPUIK_NextTagItem( struct Task *, struct TagItem **TagItemPtr );
extern struct TagItem  * IPUIK_GetUserTag( struct Task *, struct TagItem *TagItem );
extern struct TagItem  * IPUIK_FindTagItem( struct Task *, Tag TagVal, struct TagItem *TagList );
extern struct TagItem  * IPUIK_AllocateTagItems( struct Task *, ULONG NumItems );
extern struct TagItem  * IPUIK_CloneTagItems( struct Task *, struct TagItem *TagList );
extern BOOL IPUIK_TagInArray( struct Task *, Tag TagVal, Tag *TagArray );
extern ULONG IPUIK_PackBoolTags( struct Task *, ULONG InitialFlags, struct TagItem *TagList, struct TagItem *BoolMap );
extern ULONG IPUIK_GetTagData( struct Task *, Tag TagVal, ULONG Default, struct TagItem *TagList );
extern ULONG IPUIK_ApplyTagItems( struct Task *, struct TagItem *TagList, ULONG (*Func)(), ULONG FuncParm, ULONG TestRet, ULONG TestValue );
extern ULONG IPUIK_FilterTagItems( struct Task *, struct TagItem *TagList, Tag *TagArray, ULONG Logic );
extern ULONG IPUIK_HowManyTagItems( struct Task *, struct TagItem *TagList );
extern void IPUIK_ApplyFindTag( struct Task *, struct TagItem *ATagList, struct TagItem *FTagList, void (*Perform)(), ULONG parm );
extern void IPUIK_FilterTagChanges( struct Task *, struct TagItem *ChangeList, struct TagItem *OldValues, ULONG Apply );
extern void IPUIK_MapTags( struct Task *, struct TagItem *TagList, struct TagItem *MapList, ULONG IncludeMiss );
extern void IPUIK_RefreshTagItemClones( struct Task *, struct TagItem *CloneTagItems, struct TagItem *OriginalTagItems );
extern void IPUIK_CopyTagItems( struct Task *, struct TagItem *OriginalTagItems, struct TagItem *CloneTagItems );
extern void IPUIK_FreeTagItems( struct Task *, struct TagItem *TagList );
extern void IPUIK_JoinTagItems( struct Task *, struct TagItem *TagList1, struct TagItem *TagList2 );
/* */
extern int IPUIK_AskIconPlace( struct Task *, struct UIKObj *wo, ULONG *newplace, UWORD icwidth );
/* */
extern STRPTR IPUIK_MemTAlloc( struct Task *, ULONG size, ULONG reqs );
extern STRPTR IPUIK_MemSpAlloc( struct Task *, ULONG size, ULONG reqs );
extern STRPTR IPUIK_MemSpResize( struct Task *, APTR old, ULONG newsize, ULONG reqs );
extern void IPUIK_MemTFree( struct Task *, APTR mem );
extern void IPUIK_MemSpFree( struct Task *, APTR mem );
extern void IPUIK_MemCopy( struct Task *, APTR src, APTR dst, ULONG len );
extern int IPUIK_MemSpLength( struct Task *, APTR mem );
/* */
extern void IPUIK_ClipObject( struct Task *, struct UIKObj *obj );
extern void IPUIK_UnclipObject( struct Task *, struct UIKObj *obj );
/* */
extern void IPUIK_OME_ActiveMExObj( struct Task *, struct UIKObjGad *obj );
extern int IPUIK_CharWidth( struct Task *, struct TextFont *font, UBYTE c );
extern STRPTR IPUIK_OSt_Get( struct Task *, struct UIKObjStrGad *obj, ULONG *plen );
extern STRPTR IPUIK_OSt_Set( struct Task *, struct UIKObjStrGad *obj, STRPTR Buf, ULONG len );
extern STRPTR IPUIK_OSt_Cut( struct Task *, struct UIKObjStrGad *obj, ULONG start, ULONG len );
extern STRPTR IPUIK_OSt_Paste( struct Task *, struct UIKObjStrGad *obj, STRPTR buf, ULONG start, ULONG len );
extern ULONG IPUIK_ActivateKeyTank( struct Task *, struct UIKObj *obj );
/* */
extern struct UIKObjRegGerm * IPUIK_RegisterObject( struct Task *, struct UIKObjGerm *g, int replace );
extern int IPUIK_UnregisterObject( struct Task *, STRPTR objname, int force );
/* */
extern struct UIKObj * IPUIK_IsActiveKeyTank( struct Task *, struct UIKObj *obj );
/* */
extern void IPUIK_InactivateKeyTank( struct Task *, struct UIKObj *obj );
/* */
extern char IPUIK_ToUpper( struct Task *, char c );
extern char IPUIK_ToLower( struct Task *, char c );
extern char * IPUIK_StrToUpper( struct Task *, char *str );
extern char * IPUIK_StrToLower( struct Task *, char *str );
/* */
/* */
extern LONG IPUIK_Match( struct Task *, STRPTR Pat, STRPTR Aux, STRPTR Str );
/* */
extern int IPUIK_StdGadStart( struct Task *, struct UIKObjGad *obj );
extern void IPUIK_StdGadStop( struct Task *, struct UIKObjGad *obj );
extern void IPUIK_StdGadResize( struct Task *, struct UIKObjGad *obj );
extern void IPUIK_StdGadRefresh( struct Task *, struct UIKObjGad *obj );
extern void IPUIK_StdGadAct( struct Task *, struct UIKObjGad *obj );
extern void IPUIK_StdGadInact( struct Task *, struct UIKObjGad *obj );
extern void IPUIK_StdGadMove( struct Task *, struct UIKObjGad *obj, struct IntuiMessage *imsg );
/* */
extern struct UIKObj * IPUIK_AddPrivateObject( struct Task *, struct UIKObjRegGerm *rg, struct UIKObj *Parent, struct TagItem *taglist );
/* */
extern void IPUIK_OTo_SelectToggle( struct Task *, struct UIKObjGad *obj );
extern void IPUIK_OTo_UnselectToggle( struct Task *, struct UIKObjGad *obj );
/* */
extern ULONG IPUIK_Do2( struct Task *, struct UIKObj *o, ULONG EXTEVENT, ULONG windowflag );
extern ULONG IPUIK_CallObjectFunc( struct Task *, struct UIKObj *obj, ULONG func, ULONG data1, ULONG data2 );
/* */
extern struct TagItem * IPUIK_SetTagData( struct Task *, Tag TagVal, ULONG newdata, struct TagItem *TagList );
/* */
extern struct UIKTreeItem * IPUIK_MakeUIKTree( struct Task *, struct UIKTreeItem *TreeItems, struct UIKObj *Parent, APTR Vars );
/* */
extern void IPUIK_DrawBox( struct Task *, struct UIKObj *obj );
extern void IPUIK_RefreshSons( struct Task *, struct UIKObj *obj );
/* */
extern ULONG IPUIK_StrWidth( struct Task *, struct TextFont *font, STRPTR string, ULONG numbytes );
/* */
extern void IPUIK_FreeBMapRPort( struct Task *, struct RastPort *rport );
extern void IPUIK_FreeRPort( struct Task *, struct RastPort *rport );
extern void IPUIK_FreeBMap( struct Task *, struct BitMap *bitmap );
extern struct RastPort * IPUIK_AllocBMapRPort( struct Task *, UBYTE depth, UWORD width, UWORD height, UWORD flags );
extern struct RastPort * IPUIK_AllocRPort( struct Task * );
extern struct BitMap * IPUIK_AllocBMap( struct Task *, UBYTE depth, UWORD width, UWORD height, UWORD flags );
/* */
extern void IPUIK_Select( struct Task *, struct UIKObj *obj, ULONG events_mask );
/* */
extern struct UIKObjWindow * IPUIK_Iconify( struct Task *, struct UIKObj *obj );
extern ULONG IPUIK_Uniconify( struct Task *, struct UIKObjWindow *obj );
/* */
extern void IPUIK_Input_Set( struct Task *, struct UIKGlobal *uik, UBYTE input, UBYTE held, UBYTE autom );
extern void IPUIK_Input_Get( struct Task *, struct UIKGlobal *uik, STRPTR input, STRPTR held, STRPTR autom );
extern struct UIKZone * IPUIK_AddZone( struct Task *, struct UIKObj *master, struct UIKObj* newobj, ULONG events, void (*func)() );
/* */
/****************** UIKBuffers management */
extern ULONG IPBufGetUDatas( struct Task *, STRPTR buf, UWORD *pw );
extern ULONG IPBufLength( struct Task *, STRPTR mem );
extern void IPBufSetUDatas( struct Task *, STRPTR buf, ULONG uw, ULONG ul );
extern void IPBufFree( struct Task *, STRPTR mem );
extern STRPTR IPBufAlloc( struct Task *, ULONG blksize, ULONG size, ULONG reqs );
extern STRPTR IPBufResize( struct Task *, STRPTR old, ULONG newsize );
extern STRPTR IPBufClone( struct Task *, STRPTR old );
extern STRPTR IPBufCopy( struct Task *, STRPTR buf, STRPTR dest, ULONG start, ULONG len );
extern STRPTR IPBufCut( struct Task *, STRPTR buf, ULONG start, ULONG len );
extern STRPTR IPBufPaste( struct Task *, STRPTR src, ULONG len, STRPTR dest, ULONG start );
/* */
extern void IPUIK_Input_JamFrame( struct Task *, struct UIKGlobal *uik );
extern void IPUIK_Input_ReleaseFrame( struct Task *, struct UIKGlobal *uik );
extern void IPUIK_Input_SetTime( struct Task *, struct UIKGlobal *uik, UWORD speed, UWORD valid, UWORD wait );
extern void IPUIK_Input_Flush( struct Task *, struct UIKGlobal *uik );
extern void IPUIK_Input_AddJoyEvent( struct Task *, struct UIKGlobal *uik, UBYTE buttons, ULONG flush );
/* */
extern void IPUIK_Font2TTA( struct Task *, struct TextFont *font, struct TTextAttr *tta );
extern struct TextFont * IPUIK_OpenFontTTA( struct Task *, struct TTextAttr *tta );
/* */
extern void IPUIK_DrawAuto( struct Task *, struct UIKObj *obj, UWORD *automate, UWORD xoffs, UWORD yoffs );
/* */
extern void IPUIK_Input_CycleZoneList( struct Task *, struct UIKObj *obj, UBYTE cycle_on );
extern ULONG IPUIK_Input_ChangeActiveZoneList( struct Task *, struct UIKObj *obj );
/* */
extern ULONG IPUIK_SaveEnvPrefs( struct Task * );
extern ULONG IPUIK_ReadArcPrefs( struct Task * );
extern ULONG IPUIK_OLi_NewTextPtr( struct Task *, struct UIKObjList *lobj, STRPTR buf );
/* */
extern STRPTR IPBufSet( struct Task *, STRPTR src, ULONG len, STRPTR dest );
/* */
extern ULONG IPUIK_ChangeAutomate( struct Task *, struct TagItem *TagList );
/* */
extern void IPBufFreeS( struct Task *, STRPTR *pmem );
extern STRPTR IPBufResizeS( struct Task *, STRPTR *pold, ULONG newsize );
extern STRPTR IPBufCloneS( struct Task *, STRPTR *pold );
extern STRPTR IPBufCopyS( struct Task *, STRPTR buf, STRPTR *pdest, ULONG start, ULONG len );
extern STRPTR IPBufCutS( struct Task *, STRPTR *pbuf, ULONG start, ULONG len );
extern STRPTR IPBufPasteS( struct Task *, STRPTR src, ULONG len, STRPTR *pdest, ULONG start );
extern STRPTR IPBufSetS( struct Task *, STRPTR src, ULONG len, STRPTR *pdest );
/* */
extern UBYTE IPUIK_GetColor( struct Task *, struct UIKObj *obj, UBYTE uikcolor );
extern void IPUIK_SetColor( struct Task *, struct UIKObj *obj, UBYTE uik_color, UBYTE palette_color );
/* */
extern ULONG IPUIK_ProcessPendingEvents( struct Task *, struct UIKObj *o, ULONG EXTEVENT );
extern ULONG IPUIK_UniconifyWindow( struct Task *, struct UIKObjWindow *obj );
extern ULONG IPUIK_WaitFirstEvents( struct Task *, struct UIKObj *o, ULONG EXTEVENT );
/* */
extern void IPUIK_StripWindowEvents( struct Task *, struct UIKObj *obj, ULONG intuiclass );
/* */
/* New for V1.40 */
/* */
extern void IPBufTruncate( struct Task *, STRPTR old, ULONG newsize );
extern void IPUIK_OLi_DeleteLine( struct Task *, struct UIKObjList *lobj, LONG line );
extern STRPTR IPBufAllocP( struct Task *, APTR pool, ULONG size, ULONG blksize );
extern struct UIKPList * IPUIK_nl_NewListP( struct Task *, APTR pool, int n, ULONG memFlags, ULONG puddleSize, ULONG threshSize );
extern int IPUIK_nl_AllocListP( struct Task *, APTR pool, struct UIKPList *L, int n, ULONG memFlags, ULONG puddleSize, ULONG threshSize );
extern struct UIKGlobal * IPUIK_GetTaskUIK( struct Task *, struct Task *task );
extern struct UIKObj * IPUIK_GetObject( struct Task *, struct UIKObj *root, struct UIKObj *after, UWORD id );
extern STRPTR IPVBufPrintf( struct Task *, STRPTR *ubuf, ULONG start, STRPTR format, APTR *argv );
extern void IPUIK_OME_InactiveMExObj( struct Task *, struct UIKObjGad *obj );
extern void IPUIK_ShortCutRemove( struct Task *, struct UIKObj *obj );
extern void IPUIK_ObjDisable( struct Task *, struct UIKObj *obj, ULONG n );
extern void IPUIK_ObjEnable( struct Task *, struct UIKObj *obj, ULONG n );
extern struct RastPort * IPUIK_GetRastPort( struct Task *, struct UIKObj *obj );
/* */
/* New for V1.42 */
/* */
extern LONG IPUIK_CmplPattern( struct Task *, STRPTR pattern, STRPTR dest, ULONG len );
/* */

/***********************************************************************
 *      Ces fonctions de uikip.lib correspondent à des fonctions qui
 *      sont dans uik.lib (donc qui ne sont pas dans le fichier fd).
 * ---------------------------------------------------------------------
 */
extern struct UIKObj * IPUIK_AddObjectTags( struct Task *task, UBYTE *ObjName, struct UIKObj *Parent, ... );
extern struct UIKObj * IPUIK_AddPrivateObjectTags( struct Task *task, struct UIKObjRegGerm *rg, struct UIKObj *Parent, ... );
extern ULONG IPUIK_CallObjectFuncTags( struct Task *task, struct UIKObj *obj, ULONG func, ULONG data1, ... );


#endif   /* CLIB_IPUIK_PROTOS_H */
