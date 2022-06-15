#ifndef  CLIB_UIK_PROTOS_H
#define  CLIB_UIK_PROTOS_H

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

#ifdef __cplusplus
extern "C" {
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
extern int UIK_IsSystemVersion2( void );
extern int UIK_DeadKeyConvert( struct UIKKeySupp *uks, struct IntuiMessage *imsg, struct KeyMap *keymap );
extern void UIK_InvertBlackWhite( struct UIKGlobal *uik, struct UIKBitMap **table, UBYTE iv );
extern void UIK_EnableDOSReq( ULONG null, ULONG old );
extern ULONG UIK_DisableDOSReq( void );
extern STRPTR UIK_BaseName( STRPTR pathname );
extern struct TextFont * UIK_OpenFont( STRPTR name, UWORD height );
extern struct TextFont * UIK_ReopenFont( struct TextFont *font );
extern ULONG UIK_AccessPrefs( int (*func)(), ULONG parm1, ULONG parm2 );
extern ULONG UIK_ReadPrefs( void );
extern ULONG UIK_SavePrefs( void );
extern void UIK_GetPrefs( struct UIKPrefs *buf, int len );
extern void UIK_SetPrefs( struct UIKPrefs *buf, int len );
/* */
/* */
extern void UIK_Disable( struct UIKGlobal *uik );
extern void UIK_Enable( struct UIKGlobal *uik );
/* */
extern int UIK_InsideObj( struct UIKObj *obj, ULONG xy );
extern void UIK_LockObjVars( struct UIKObj *obj );
extern void UIK_UnlockObjVars( struct UIKObj *obj );
extern STRPTR UIK_ObjTitle( struct UIKObj *obj );
extern STRPTR UIK_ObjAltTitle( struct UIKObj *obj );
extern STRPTR UIK_LangString( struct UIKObj *obj, ULONG index );
extern STRPTR UIK_OMe_MemPtr( struct UIKObj *obj );
/* */
extern struct UIKHook * UIK_AddVector( struct UIKObj *obj, void (*vect)(), ULONG mask, ULONG parm );
extern void UIK_RemoveVector( struct UIKObj *obj, struct UIKHook *hook );
extern void UIK_RemAllVectors( struct UIKObj *obj );
extern void UIK_Stop( struct UIKObj *obj );
extern void UIK_Remove( struct UIKObj *obj );
extern void UIK_Refresh( struct UIKObj *obj, int n );
extern void UIK_RefreshWindow( struct UIKObj *obj );
extern void UIK_CauseObjHook( struct UIKObj *obj, ULONG intuiclass );
extern void UIK_CauseObjLink( struct UIKObj *obj, ULONG intuiclass );
extern int UIK_Link( struct UIKObj *obj1, struct UIKObj *obj2, int (*func)(), ULONG intuiclass, ULONG parm );
extern int UIK_Start(struct UIKObj *obj );
extern ULONG UIK_Do( struct UIKObj *obj, ULONG EXTEVENT );
/* */
extern struct UIKObj * UIK_AddObject( STRPTR ObjName, struct UIKObj *Parent, struct TagItem *taglist );
extern int UIK_GetObjectValues( struct UIKObj *Obj, struct TagItem *taglist );
extern int UIK_SetObjectValues( struct UIKObj *Obj, struct TagItem *taglist );
/* */
extern void UIK_OPr_ChangePropGadget( struct UIKObjPropGad *obj, int Total, int Visible, int Position );
extern void UIK_OPr_ChangePropNoDraw( struct UIKObjPropGad *obj, int Total, int Visible, int Position );
/* */
extern void UIK_OLi_DrawList( struct UIKObj *obj );
extern void UIK_OLi_DrawListFrom( struct UIKObj *obj, LONG from, LONG num );
extern void UIK_OLi_DrawLine( struct UIKObj *obj, LONG line );
/* */
extern struct UIKList * UIK_nl_NewList( int n );
extern struct UIKList * UIK_nl_ListPtr( struct UIKList *L );
extern void UIK_nl_FreeList( struct UIKList *L );
extern void UIK_nl_DeleteList( struct UIKList *L );
extern void UIK_nl_AttachList( struct UIKList *from, struct UIKList *to );
extern void UIK_nl_DetachList( struct UIKList *from, ULONG n );
extern void UIK_nl_FreeNodes( struct UIKList *L, LONG from, LONG to );
extern void UIK_nl_SortList( struct UIKList *L, int (*sortfunc)() );
extern STRPTR UIK_nl_PasteToNode( struct UIKList *L, LONG ind, LONG start, STRPTR buf, LONG len );
extern STRPTR UIK_nl_CutFromNode( struct UIKList *L, LONG ind, LONG start, LONG len );
/* */
extern ULONG UIK_nl_ListNodeNum( struct UIKList *L );
extern ULONG UIK_nl_NodeBufLen( struct UIKList *L, LONG ind );
extern int UIK_nl_CrossList( struct UIKList *L, int (*func)(), int wret, ULONG parm );
extern int UIK_nl_AllocList( struct UIKList *L, int n );
extern LONG UIK_nl_FindNode( struct UIKList *L, STRPTR buf );
extern STRPTR UIK_nl_AllocNode( struct UIKList *L, LONG ind, LONG len, WORD flags );
extern STRPTR UIK_nl_ReAllocNode( struct UIKList *L, LONG ind, LONG len, LONG flags );
extern STRPTR UIK_nl_NodeBuf( struct UIKList *L, LONG ind );
/* */
extern int UIK_OLi_ChangeLine( struct UIKObjList *lobj, STRPTR str, LONG line, WORD flags );
extern int UIK_OLi_AddLineLen( struct UIKObjList *lobj, STRPTR str, LONG line, WORD flags, LONG len );
extern int UIK_OLi_AddLine( struct UIKObjList *lobj, STRPTR str, LONG line, WORD flags );
extern int UIK_OLi_TotLines( struct UIKObjList *lobj );
extern int UIK_OLi_NewText( struct UIKObjList *lobj, struct TextEntry *te );
extern void UIK_OLi_DelLine( struct UIKObjList *lobj, LONG line );
extern void UIK_OLi_UnselectList( struct UIKObjList *lobj );
extern void UIK_OLi_SelectList( struct UIKObjList *lobj, LONG line );
extern void UIK_OLi_ActualizeList( struct UIKObjList *lobj );
extern void UIK_OLi_SetListTop( struct UIKObjList *lobj, LONG line );
extern void UIK_OLi_MoveListFrame( struct UIKObjList *list, LONG n );
extern void UIK_OLi_MoveListSel( struct UIKObjList *list, LONG n );
extern STRPTR UIK_OLi_LineBuffer( struct UIKObjList *lobj, LONG line );
/* */
extern int StrNCCmp( char *s1, char *s2 );
extern int StcCpy( char *dest, char *source, int len );
extern int Str2Num( char *s );
extern int StrB2C( ULONG bs, char *cs );
extern void StrCat( char *dest, char *source );
extern int StrCmp( char *s1, char *s2 );
extern char * StrCpy( char *dest, char *source );
extern int StrLen( char *s );
/* */
extern struct TagItem  * UIK_NextTagItem( struct TagItem **TagItemPtr );
extern struct TagItem  * UIK_GetUserTag( struct TagItem *TagItem );
extern struct TagItem  * UIK_FindTagItem( Tag TagVal, struct TagItem *TagList );
extern struct TagItem  * UIK_AllocateTagItems( ULONG NumItems );
extern struct TagItem  * UIK_CloneTagItems( struct TagItem *TagList );
extern BOOL UIK_TagInArray( Tag TagVal, Tag *TagArray );
extern ULONG UIK_PackBoolTags( ULONG InitialFlags, struct TagItem *TagList, struct TagItem *BoolMap );
extern ULONG UIK_GetTagData( Tag TagVal, ULONG Default, struct TagItem *TagList );
extern ULONG UIK_ApplyTagItems( struct TagItem *TagList, ULONG (*Func)(), ULONG FuncParm, ULONG TestRet, ULONG TestValue );
extern ULONG UIK_FilterTagItems( struct TagItem *TagList, Tag *TagArray, ULONG Logic );
extern ULONG UIK_HowManyTagItems( struct TagItem *TagList );
extern void UIK_ApplyFindTag( struct TagItem *ATagList, struct TagItem *FTagList, void (*Perform)(), ULONG parm );
extern void UIK_FilterTagChanges( struct TagItem *ChangeList, struct TagItem *OldValues, ULONG Apply );
extern void UIK_MapTags( struct TagItem *TagList, struct TagItem *MapList, ULONG IncludeMiss );
extern void UIK_RefreshTagItemClones( struct TagItem *CloneTagItems, struct TagItem *OriginalTagItems );
extern void UIK_CopyTagItems( struct TagItem *OriginalTagItems, struct TagItem *CloneTagItems );
extern void UIK_FreeTagItems( struct TagItem *TagList );
extern void UIK_JoinTagItems( struct TagItem *TagList1, struct TagItem *TagList2 );
/* */
extern int UIK_AskIconPlace( struct UIKObj *wo, ULONG *newplace, UWORD icwidth );
/* */
extern STRPTR UIK_MemTAlloc( ULONG size, ULONG reqs );
extern STRPTR UIK_MemSpAlloc( ULONG size, ULONG reqs );
extern STRPTR UIK_MemSpResize( APTR old, ULONG newsize, ULONG reqs );
extern void UIK_MemTFree( APTR mem );
extern void UIK_MemSpFree( APTR mem );
extern void UIK_MemCopy( APTR src, APTR dst, ULONG len );
extern int UIK_MemSpLength( APTR mem );
/* */
extern void UIK_ClipObject( struct UIKObj *obj );
extern void UIK_UnclipObject( struct UIKObj *obj );
/* */
extern void UIK_OME_ActiveMExObj( struct UIKObjGad *obj );
extern int UIK_CharWidth( struct TextFont *font, UBYTE c );
extern STRPTR UIK_OSt_Get( struct UIKObjStrGad *obj, ULONG *plen );
extern STRPTR UIK_OSt_Set( struct UIKObjStrGad *obj, STRPTR Buf, ULONG len );
extern STRPTR UIK_OSt_Cut( struct UIKObjStrGad *obj, ULONG start, ULONG len );
extern STRPTR UIK_OSt_Paste( struct UIKObjStrGad *obj, STRPTR buf, ULONG start, ULONG len );
extern ULONG UIK_ActivateKeyTank( struct UIKObj *obj );
/* */
extern struct UIKObjRegGerm * UIK_RegisterObject( struct UIKObjGerm *g, int replace );
extern int UIK_UnregisterObject( STRPTR objname, int force );
/* */
extern struct UIKObj * UIK_IsActiveKeyTank( struct UIKObj *obj );
/* */
extern void UIK_InactivateKeyTank( struct UIKObj *obj );
/* */
extern char UIK_ToUpper( char c );
extern char UIK_ToLower( char c );
extern char * UIK_StrToUpper( char *str );
extern char * UIK_StrToLower( char *str );
/* */
/* */
extern LONG UIK_Match( STRPTR Pat, STRPTR Aux, STRPTR Str );
/* */
extern int UIK_StdGadStart( struct UIKObjGad *obj );
extern void UIK_StdGadStop( struct UIKObjGad *obj );
extern void UIK_StdGadResize( struct UIKObjGad *obj );
extern void UIK_StdGadRefresh( struct UIKObjGad *obj );
extern void UIK_StdGadAct( struct UIKObjGad *obj );
extern void UIK_StdGadInact( struct UIKObjGad *obj );
extern void UIK_StdGadMove( struct UIKObjGad *obj, struct IntuiMessage *imsg );
/* */
extern struct UIKObj * UIK_AddPrivateObject( struct UIKObjRegGerm *rg, struct UIKObj *Parent, struct TagItem *taglist );
/* */
extern void UIK_OTo_SelectToggle( struct UIKObjGad *obj );
extern void UIK_OTo_UnselectToggle( struct UIKObjGad *obj );
/* */
extern ULONG UIK_Do2( struct UIKObj *o, ULONG EXTEVENT, ULONG windowflag );
extern ULONG UIK_CallObjectFunc( struct UIKObj *obj, ULONG func, ULONG data1, ULONG data2 );
/* */
extern struct TagItem * UIK_SetTagData( Tag TagVal, ULONG newdata, struct TagItem *TagList );
/* */
extern struct UIKTreeItem * UIK_MakeUIKTree( struct UIKTreeItem *TreeItems, struct UIKObj *Parent, APTR Vars );
/* */
extern void UIK_DrawBox( struct UIKObj *obj );
extern void UIK_RefreshSons( struct UIKObj *obj );
/* */
extern ULONG UIK_StrWidth( struct TextFont *font, STRPTR string, ULONG numbytes );
/* */
extern void UIK_FreeBMapRPort( struct RastPort *rport );
extern void UIK_FreeRPort( struct RastPort *rport );
extern void UIK_FreeBMap( struct BitMap *bitmap );
extern struct RastPort * UIK_AllocBMapRPort( UBYTE depth, UWORD width, UWORD height, UWORD flags );
extern struct RastPort * UIK_AllocRPort( void );
extern struct BitMap * UIK_AllocBMap( UBYTE depth, UWORD width, UWORD height, UWORD flags );
/* */
extern void UIK_Select( struct UIKObj *obj, ULONG events_mask );
/* */
extern struct UIKObjWindow * UIK_Iconify( struct UIKObj *obj );
extern ULONG UIK_Uniconify( struct UIKObjWindow *obj );
/* */
extern void UIK_Input_Set( struct UIKGlobal *uik, UBYTE input, UBYTE held, UBYTE autom );
extern void UIK_Input_Get( struct UIKGlobal *uik, STRPTR input, STRPTR held, STRPTR autom );
extern struct UIKZone * UIK_AddZone( struct UIKObj *master, struct UIKObj* newobj, ULONG events, void (*func)() );
/* */
/****************** UIKBuffers management */
extern ULONG BufGetUDatas( STRPTR buf, UWORD *pw );
extern ULONG BufLength( STRPTR mem );
extern void BufSetUDatas( STRPTR buf, ULONG uw, ULONG ul );
extern void BufFree( STRPTR mem );
extern STRPTR BufAlloc( ULONG blksize, ULONG size, ULONG reqs );
extern STRPTR BufResize( STRPTR old, ULONG newsize );
extern STRPTR BufClone( STRPTR old );
extern STRPTR BufCopy( STRPTR buf, STRPTR dest, ULONG start, ULONG len );
extern STRPTR BufCut( STRPTR buf, ULONG start, ULONG len );
extern STRPTR BufPaste( STRPTR src, ULONG len, STRPTR dest, ULONG start );
/* */
extern void UIK_Input_JamFrame( struct UIKGlobal *uik );
extern void UIK_Input_ReleaseFrame( struct UIKGlobal *uik );
extern void UIK_Input_SetTime( struct UIKGlobal *uik, UWORD speed, UWORD valid, UWORD wait );
extern void UIK_Input_Flush( struct UIKGlobal *uik );
extern void UIK_Input_AddJoyEvent( struct UIKGlobal *uik, UBYTE buttons, ULONG flush );
/* */
extern void UIK_Font2TTA( struct TextFont *font, struct TTextAttr *tta );
extern struct TextFont * UIK_OpenFontTTA( struct TTextAttr *tta );
/* */
extern void UIK_DrawAuto( struct UIKObj *obj, UWORD *automate, UWORD xoffs, UWORD yoffs );
/* */
extern void UIK_Input_CycleZoneList( struct UIKObj *obj, UBYTE cycle_on );
extern ULONG UIK_Input_ChangeActiveZoneList( struct UIKObj *obj );
/* */
extern ULONG UIK_SaveEnvPrefs( void );
extern ULONG UIK_ReadArcPrefs( void );
extern ULONG UIK_OLi_NewTextPtr( struct UIKObjList *lobj, STRPTR buf );
/* */
extern STRPTR BufSet( STRPTR src, ULONG len, STRPTR dest );
/* */
extern ULONG UIK_ChangeAutomate( struct TagItem *TagList );
/* */
extern void BufFreeS( STRPTR *pmem );
extern STRPTR BufResizeS( STRPTR *pold, ULONG newsize );
extern STRPTR BufCloneS( STRPTR *pold );
extern STRPTR BufCopyS( STRPTR buf, STRPTR *pdest, ULONG start, ULONG len );
extern STRPTR BufCutS( STRPTR *pbuf, ULONG start, ULONG len );
extern STRPTR BufPasteS( STRPTR src, ULONG len, STRPTR *pdest, ULONG start );
extern STRPTR BufSetS( STRPTR src, ULONG len, STRPTR *pdest );
/* */
extern UBYTE UIK_GetColor( struct UIKObj *obj, UBYTE uikcolor );
extern void UIK_SetColor( struct UIKObj *obj, UBYTE uik_color, UBYTE palette_color );
/* */
extern ULONG UIK_ProcessPendingEvents( struct UIKObj *o, ULONG EXTEVENT );
extern ULONG UIK_UniconifyWindow( struct UIKObjWindow *obj );
extern ULONG UIK_WaitFirstEvents( struct UIKObj *o, ULONG EXTEVENT );
/* */
extern void UIK_StripWindowEvents( struct UIKObj *obj, ULONG intuiclass );
/* */
/* New for V1.40 */
/* */
extern void BufTruncate( STRPTR old, ULONG newsize );
extern void UIK_OLi_DeleteLine( struct UIKObjList *lobj, LONG line );
extern STRPTR BufAllocP( APTR pool, ULONG size, ULONG blksize );
extern struct UIKPList * UIK_nl_NewListP( APTR pool, int n, ULONG memFlags, ULONG puddleSize, ULONG threshSize );
extern int UIK_nl_AllocListP( APTR pool, struct UIKPList *L, int n, ULONG memFlags, ULONG puddleSize, ULONG threshSize );
extern struct UIKGlobal * UIK_GetTaskUIK( struct Task *task );
extern struct UIKObj * UIK_GetObject( struct UIKObj *root, struct UIKObj *after, UWORD id );
extern STRPTR VBufPrintf( STRPTR *ubuf, ULONG start, STRPTR format, APTR *argv );
extern void UIK_OME_InactiveMExObj( struct UIKObjGad *obj );
extern void UIK_ShortCutRemove( struct UIKObj *obj );
extern void UIK_ObjDisable( struct UIKObj *obj, ULONG n );
extern void UIK_ObjEnable( struct UIKObj *obj, ULONG n );
extern struct RastPort * UIK_GetRastPort( struct UIKObj *obj );
/* */
/* New for V1.42 */
/* */
extern LONG UIK_CmplPattern( STRPTR pattern, STRPTR dest, ULONG len );
/* */

/***********************************************************************
 *             Fonctions externes à la librairie
 * ---------------------------------------------------------------------
 * Ces fonctions sont dans uik.lib car leurs paramètres sont dans la pile.
 */
extern struct UIKObj * UIK_AddObjectTags( STRPTR ObjName, struct UIKObj *Parent, ... );
extern struct UIKObj * UIK_AddObjectTagsB( struct UIKBase *base, STRPTR ObjName, struct UIKObj *Parent, ... );

extern struct UIKObj * UIK_AddPrivateObjectTags( struct UIKObjRegGerm *rg, struct UIKObj *Parent, ... );
extern struct UIKObj * UIK_AddPrivateObjectTagsB( struct UIKBase *base, struct UIKObjRegGerm *rg, struct UIKObj *Parent, ... );

extern ULONG UIK_CallObjectFuncTags( struct UIKObj *obj, ULONG func, ULONG data1, ... );
extern ULONG UIK_CallObjectFuncTagsB( struct UIKBase *base, struct UIKObj *obj, ULONG func, ULONG data1, ... );

extern STRPTR  BufPrintf( STRPTR *ubuf, ULONG start, STRPTR fmt, void *arg1, ... );
extern STRPTR  BufPrintfB( struct UIKBase *base, STRPTR *ubuf, ULONG start, STRPTR fmt, void *arg1, ... );

#ifdef __cplusplus
}
#endif

#endif   /* CLIB_UIK_PROTOS_H */

