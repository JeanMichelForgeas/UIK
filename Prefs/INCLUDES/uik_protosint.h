#ifndef  CLIB_UIKINT_PROTOS_H
#define  CLIB_UIKINT_PROTOS_H

/* "uik.library" */
/* */
/*      (C) Copyright 1991,1992 Jean-Michel Forgeas" */
/*                  Tous Droits Réservés" */
/* */
/* */
/* First function is always at an offset of -30 from lib base */
/******** */
/* ------------------------------public------------------------------- */
extern int __asm UIK_IsSystemVersion2( void );
extern int __asm UIK_DeadKeyConvert( register __a0 struct UIKKeySupp *uks, 
        register __a1 struct IntuiMessage *imsg, register __d0 struct KeyMap *keymap );
extern void __asm UIK_InvertBlackWhite( register __a0 struct UIKGlobal *uik, 
        register __a1 struct UIKBitMap **table, register __d0 UBYTE iv );
extern void __asm UIK_EnableDOSReq( register __d1 ULONG null, 
        register __d0 ULONG old );
extern ULONG __asm UIK_DisableDOSReq(  );
extern UBYTE * __asm UIK_BaseName( register __a0 UBYTE *pathname );
extern struct TextFont * __asm UIK_OpenFont( register __a0 UBYTE *name, 
        register __d0 UWORD height );
extern struct TextFont * __asm UIK_ReopenFont( register __a1 struct TextFont *font );
extern ULONG __asm UIK_AccessPrefs( register __a0 int (*func)(), 
        register __a1 ULONG parm1, register __d1 ULONG parm2 );
extern ULONG __asm UIK_ReadPrefs(  );
extern ULONG __asm UIK_SavePrefs(  );
extern void __asm UIK_GetPrefs( register __a1 struct UIKPrefs *buf, 
        register __d1 int len );
extern void __asm UIK_SetPrefs( register __a1 struct UIKPrefs *buf, 
        register __d1 int len );
/* */
extern struct UIKGlobal * __asm UIK_Install( register __a0 UBYTE *english[], 
        register __a1 UBYTE *other[] );
extern void __asm UIK_RemoveAll( register __d0 struct UIKGlobal *uik );
/* */
extern void __asm UIK_Disable( register __a0 struct UIKGlobal *uik );
extern void __asm UIK_Enable( register __a0 struct UIKGlobal *uik );
/* */
extern int __asm UIK_InsideObj( register __a0 struct UIKObj *obj, 
        register __a1 ULONG xy );
extern void __asm UIK_LockObjVars( register __a0 struct UIKObj *obj );
extern void __asm UIK_UnlockObjVars( register __a0 struct UIKObj *obj );
extern UBYTE * __asm UIK_ObjTitle( register __a0 struct UIKObj *obj );
extern UBYTE * __asm UIK_ObjAltTitle( register __a0 struct UIKObj *obj );
extern UBYTE * __asm UIK_LangString( register __a0 struct UIKObj *obj, 
        register __d0 ULONG index );
extern UBYTE * __asm UIK_OMe_MemPtr( register __a0 struct UIKObj *obj );
/* */
extern struct UIKHook * __asm UIK_AddVector( register __a0 struct UIKObj *obj, 
        register __a1 int (*vect)(), register __d0 ULONG mask, 
        register __d1 ULONG parm );
extern void __asm UIK_RemoveVector( register __a1 struct UIKObj *obj, 
        register __a0 struct UIKHook *hook );
extern void __asm UIK_RemAllVectors( register __a1 struct UIKObj *obj );
extern void __asm UIK_Stop( register __a0 struct UIKObj *obj );
extern void __asm UIK_Remove( register __a0 struct UIKObj *obj );
extern void __asm UIK_Refresh( register __a0 struct UIKObj *obj, 
        register __d0 int n );
extern void __asm UIK_RefreshWindow( register __a0 struct UIKObj *obj );
extern int __asm UIK_CauseObjHook( register __a0 struct UIKObj *obj, 
        register __d0 ULONG class );
extern int __asm UIK_CauseObjLink( register __a0 struct UIKObj *obj, 
        register __d0 ULONG class );
extern int __asm UIK_Link( register __a0 struct UIKObj *obj1, 
        register __a1 struct UIKObj *obj2, register __d0 int (*func)(), 
        register __d1 ULONG class, register __d2 ULONG parm );
extern int __asm UIK_Start( register __a0 struct UIKObj *obj );
extern ULONG __asm UIK_Do( register __a0 struct UIKObj *obj, 
        register __d0 ULONG EXTEVENT );
/* */
extern struct UIKObj * __asm UIK_AddObject( register __a0 UBYTE *ObjName, 
        register __d0 struct UIKObj *Parent, register __a1 struct TagItem *taglist );
extern int __asm UIK_GetObjectValues( register __a0 struct UIKObj *Obj, 
        register __a1 struct TagItem *taglist );
extern int __asm UIK_SetObjectValues( register __a0 struct UIKObj *Obj, 
        register __a1 struct TagItem *taglist );
/* */
extern void __asm UIK_OPr_ChangePropGadget( register __a0 struct UIKObjPropGad *obj, 
        register __a1 int Total, register __d0 int Visible, 
        register __d1 int Position );
extern void __asm UIK_OPr_ChangePropNoDraw( register __a0 struct UIKObjPropGad *obj, 
        register __a1 int Total, register __d0 int Visible, 
        register __d1 int Position );
/* */
extern void __asm UIK_OLi_DrawList( register __a0 struct UIKObj *obj );
extern void __asm UIK_OLi_DrawListFrom( register __a0 struct UIKObj *obj, 
        register __d0 LONG from, register __d1 LONG num );
extern void __asm UIK_OLi_DrawLine( register __a0 struct UIKObj *obj, 
        register __d0 LONG line );
/* */
extern struct UIKList * __asm UIK_nl_NewList( register __d0 int n );
extern struct UIKList * __asm UIK_nl_ListPtr( register __a0 struct UIKList *L );
extern void __asm UIK_nl_FreeList( register __a0 struct UIKList *L );
extern void __asm UIK_nl_DeleteList( register __a0 struct UIKList *L );
extern void __asm UIK_nl_AttachList( register __a0 struct UIKList *from, 
        register __a1 struct UIKList *to );
extern void __asm UIK_nl_DetachList( register __a0 struct UIKList *from, 
        register __d0 ULONG n );
extern void __asm UIK_nl_FreeNodes( register __a0 struct UIKList *L, 
        register __d0 LONG from, register __d1 LONG to );
extern void __asm UIK_nl_SortList( register __a0 struct UIKList *L, 
        register __a1 int (*sortfunc)() );
extern UBYTE * __asm UIK_nl_PasteToNode( register __a0 struct UIKList *L, 
        register __d0 LONG ind, register __d1 LONG start, 
        register __a1 UBYTE *buf, register __d2 LONG len );
extern UBYTE * __asm UIK_nl_CutFromNode( register __a0 struct UIKList *L, 
        register __d0 LONG ind, register __d1 LONG start, 
        register __d2 LONG len );
/* */
extern ULONG __asm UIK_nl_ListNodeNum( register __a0 struct UIKList *L );
extern ULONG __asm UIK_nl_NodeBufLen( register __a0 struct UIKList *L, 
        register __d0 LONG ind );
extern int __asm UIK_nl_CrossList( register __a0 struct UIKList *L, 
        register __a1 int (*func)(), register __d0 int wret, 
        register __d1 ULONG parm );
extern int __asm UIK_nl_AllocList( register __a0 struct UIKList *L, 
        register __d0 int n );
extern LONG __asm UIK_nl_FindNode( register __a0 struct UIKList *L, 
        register __d0 UBYTE *buf );
extern UBYTE * __asm UIK_nl_AllocNode( register __a0 struct UIKList *L, 
        register __d0 LONG ind, register __d1 LONG len, 
        register __a1 WORD flags );
extern UBYTE * __asm UIK_nl_ReAllocNode( register __a0 struct UIKList *L, 
        register __d0 LONG ind, register __d1 LONG len, 
        register __a1 LONG flags );
extern UBYTE * __asm UIK_nl_NodeBuf( register __a0 struct UIKList *L, 
        register __d0 LONG ind );
/* */
extern int __asm UIK_OLi_ChangeLine( register __a0 struct UIKObjList *lobj, 
        register __a1 UBYTE *str, register __d0 LONG line, 
        register __d1 WORD flags );
extern int __asm UIK_OLi_AddLineLen( register __a0 struct UIKObjList *lobj, 
        register __a1 UBYTE *str, register __d0 LONG line, 
        register __d1 WORD flags, register __d2 LONG len );
extern int __asm UIK_OLi_AddLine( register __a0 struct UIKObjList *lobj, 
        register __a1 UBYTE *str, register __d0 LONG line, 
        register __d1 WORD flags );
extern int __asm UIK_OLi_TotLines( register __a0 struct UIKObjList *lobj );
extern int __asm UIK_OLi_NewText( register __a0 struct UIKObjList *lobj, 
        register __a1 struct TextEntry *te );
extern void __asm UIK_OLi_DelLine( register __a0 struct UIKObjList *lobj, 
        register __d0 LONG line );
extern void __asm UIK_OLi_UnselectList( register __a0 struct UIKObjList *lobj );
extern void __asm UIK_OLi_SelectList( register __a0 struct UIKObjList *lobj, 
        register __d0 LONG line );
extern void __asm UIK_OLi_ActualizeList( register __a0 struct UIKObjList *lobj );
extern void __asm UIK_OLi_SetListTop( register __a0 struct UIKObjList *lobj, 
        register __d0 LONG line );
extern void __asm UIK_OLi_MoveListFrame( register __a0 struct UIKObjList *list, 
        register __d0 LONG n );
extern void __asm UIK_OLi_MoveListSel( register __a0 struct UIKObjList *list, 
        register __d0 LONG n );
extern UBYTE * __asm UIK_OLi_LineBuffer( register __a0 struct UIKObjList *lobj, 
        register __d0 LONG line );
/* */
extern int __asm StrNCCmp( register __a0 char *s1, 
        register __a1 char *s2 );
extern int __asm StcCpy( register __d0 char *dest, 
        register __a0 char *source, register __a1 int len );
extern int __asm Str2Num( register __a0 char *s );
extern int __asm StrB2C( register __a0 ULONG bs, 
        register __a1 char *cs );
extern void __asm StrCat( register __a0 char *dest, 
        register __a1 char *source );
extern int __asm StrCmp( register __a0 char *s1, 
        register __a1 char *s2 );
extern char * __asm StrCpy( register __a0 char *dest, 
        register __a1 char *source );
extern int __asm StrLen( register __a0 char *s );
/* */
extern struct TagItem  * __asm NextTagItem( register __a0 struct TagItem **TagItemPtr );
extern struct TagItem  * __asm GetUserTag( register __a0 struct TagItem *TagItem );
extern struct TagItem  * __asm FindTagItem( register __d0 Tag TagVal, 
        register __a0 struct TagItem *TagList );
extern struct TagItem  * __asm AllocateTagItems( register __d0 ULONG NumItems );
extern struct TagItem  * __asm CloneTagItems( register __a0 struct TagItem *TagList );
extern BOOL __asm TagInArray( register __d0 Tag, 
        register __a0 Tag *TagArray );
extern ULONG __asm PackBoolTags( register __d0 ULONG InitialFlags, 
        register __a0 struct TagItem *TagList, register __a1 struct TagItem *BoolMap );
extern ULONG __asm GetTagData( register __d0 Tag TagVal, 
        register __d1 ULONG Default, register __a0 struct TagItem *TagList );
extern ULONG __asm ApplyTagItems( register __a0 struct TagItem *TagList, 
        register __a1 ULONG (*Func)(), register __d2 ULONG FuncParm, 
        register __d0 ULONG TestRet, register __d1 ULONG TestValue );
extern ULONG __asm FilterTagItems( register __a0 struct TagItem *TagList, 
        register __a1 Tag *TagArray, register __d0 ULONG Logic );
extern ULONG __asm HowManyTagItems( register __a0 struct TagItem *TagList );
extern void __asm ApplyFindTag( register __a0 struct TagItem *ATagList, 
        register __a1 struct TagItem *FTagList, register __d0 void (*Perform)() );
extern void __asm FilterTagChanges( register __a0 struct TagItem *ChangeList, 
        register __a1 struct TagItem *OldValues, register __d0 ULONG Apply );
extern void __asm MapTags( register __a0 struct TagItem *TagList, 
        register __a1 struct TagItem *MapList, register __d0 ULONG IncludeMiss );
extern void __asm RefreshTagItemClones( register __a0 struct TagItem *CloneTagItems, 
        register __a1 struct TagItem *OriginalTagItems );
extern void __asm CopyTagItems( register __a0 struct TagItem *OriginalTagItems, 
        register __a1 struct TagItem *CloneTagItems );
extern void __asm FreeTagItems( register __a0 struct TagItem *TagList );
extern void __asm JoinTagItems( register __a0 struct TagItem *TagList1, 
        register __a1 struct TagItem *TagList2 );
/* */
extern int __asm UIK_AskIconPlace( register __a0 struct UIKObj *wo, 
        register __a1 ULONG *new, register __d0 UWORD icwidth );
/* */
extern UBYTE * __asm UIK_MemTAlloc( register __d0 ULONG size, 
        register __d1 ULONG reqs );
extern UBYTE * __asm UIK_MemSpAlloc( register __d0 ULONG size, 
        register __d1 ULONG reqs );
extern UBYTE * __asm UIK_MemSpResize( register __a0 APTR old, 
        register __d0 ULONG newsize, register __d1 ULONG reqs );
extern void __asm UIK_MemTFree( register __a0 APTR mem );
extern void __asm UIK_MemSpFree( register __a1 APTR mem );
extern void __asm UIK_MemCopy( register __a0 APTR src, 
        register __a1 APTR dst, register __d0 ULONG len );
extern int __asm UIK_MemSpLength( register __a0 APTR mem );
/* */
extern struct Region * __asm UIK_ClipObject( register __a1 struct UIKObj *obj );
extern struct Region * __asm UIK_UnclipObject( register __a1 struct UIKObj *obj );
/* */
extern void __asm UIK_OME_ActiveMExObj( register __a0 struct UIKObjGad *obj );
extern int __asm UIK_CharWidth( register __a0 struct TextFont *font, 
        register __d0 UBYTE c );
extern UBYTE * __asm UIK_OSt_Get( register __a0 struct UIKObjStrGad *obj, 
        register __a1 ULONG *plen );
extern UBYTE * __asm UIK_OSt_Set( register __a0 struct UIKObjStrGad *obj, 
        register __a1 UBYTE *Buf, register __d0 ULONG len );
extern UBYTE * __asm UIK_OSt_Cut( register __a0 struct UIKObjStrGad *obj, 
        register __d0 ULONG start, register __d1 ULONG len );
extern UBYTE * __asm UIK_OSt_Paste( register __a0 struct UIKObjStrGad *obj, 
        register __a1 UBYTE *buf, register __d0 ULONG start, 
        register __d1 ULONG len );
extern void __asm UIK_OSt_Activate( register __a0 struct UIKObjStrGad *obj );
/* */
extern struct UIKObjRegGerm * __asm UIK_RegisterObject( register __a0 struct UIKObjGerm *g, 
        register __d0 int replace );
extern int __asm UIK_UnregisterObject( register __a0 UBYTE *objname, 
        register __d0 int force );
/* */
extern struct UIKObj * __asm UIK_IsActiveString( register __a0 struct UIKObj *obj );
/* */
extern void __asm UIK_OSt_Inactivate( register __a0 struct UIKObjStrGad *obj );
/* */
extern char __asm UIK_ToUpper( register __d0 char c );
extern char __asm UIK_ToLower( register __d0 char c );
extern char * __asm UIK_StrToUpper( register __a0 char *str );
extern char * __asm UIK_StrToLower( register __a0 char *str );
/* */
extern long __asm UIK_CmplPat( register __a0 char Pattern[], 
        register __a1 short CmplPattern[] );
extern long __asm UIK_Match( register __d0 char Pat[], 
        register __a0 short Aux[], register __a1 char Str[] );
/* */
extern int __asm UIK_StdGadStart( register __a0 struct UIKObjGad *obj );
extern void __asm UIK_StdGadStop( register __a0 struct UIKObjGad *obj );
extern void __asm UIK_StdGadResize( register __a0 struct UIKObjGad *obj );
extern void __asm UIK_StdGadRefresh( register __a0 struct UIKObjGad *obj );
extern void __asm UIK_StdGadAct( register __a0 struct UIKObjGad *obj );
extern void __asm UIK_StdGadInact( register __a0 struct UIKObjGad *obj );
extern void __asm UIK_StdGadMove( register __a0 struct UIKObjGad *obj, 
        register __a3 struct IntuiMessage *imsg );
/* */
extern struct UIKObj * __asm UIK_AddPrivateObject( register __a0 struct UIKObjRegGerm *rg, 
        register __d0 struct UIKObj *Parent, register __a1 struct TagItem *taglist );
/* */
extern void __asm UIK_OTo_SelectToggle( register __a0 struct UIKObjGad *obj );
extern void __asm UIK_OTo_UnselectToggle( register __a0 struct UIKObjGad *obj );
/* */
extern ULONG __asm UIK_Do2( register __a0 struct UIKObj *o, 
        register __d0 ULONG EXTEVENT, register __d1 ULONG windowflag );
extern ULONG __asm UIK_CallObjectFunc( register __a1 struct UIKObj *obj, 
        register __d0 ULONG func, register __a0 ULONG data1, 
        register __d1 ULONG data2 );
/* */
extern struct TagItem * __asm SetTagData( register __d0 Tag TagVal, 
        register __d1 ULONG newdata, register __a0 struct TagItem *TagList );
/* */
extern struct UIKTreeItem * __asm UIK_MakeUIKTree( register __a0 struct UIKTreeItem *TreeItems, 
        register __a1 struct UIKObj *Parent, register __a2 APTR Vars );
/* */
extern void __asm UIK_DrawBox( register __a0 struct UIKObj *obj );
extern void __asm UIK_RefreshSons( register __a0 struct UIKObj *obj );
/* */
extern ULONG __asm UIK_StrWidth( register __a0 struct TextFont *font, 
        register __a1 UBYTE *string, register __d0 ULONG numbytes );
/* */
extern void __asm UIK_FreeBMapRPort( register __a0 struct RastPort *rport );
extern void __asm UIK_FreeRPort( register __a0 struct RastPort *rport );
extern void __asm UIK_FreeBMap( register __a0 struct BitMap *bitmap );
extern struct RastPort * __asm UIK_AllocBMapRPort( register __d0 UBYTE depth, 
        register __d1 UWORD width, register __a0 UWORD height, 
        register __a1 UWORD flags );
extern struct RastPort * __asm UIK_AllocRPort( void );
extern struct BitMap * __asm UIK_AllocBMap( register __d0 UBYTE depth, 
        register __d1 UWORD width, register __a0 UWORD height, 
        register __a1 UWORD flags );
/* */
extern void __asm UIK_Select( register __a0 struct UIKObj *obj, 
        register __d0 ULONG events_mask );
/* */
extern struct UIKObjWindow * __asm UIK_Iconify( register __a0 struct UIKObj *obj );
extern void __asm UIK_Uniconify( register __a0 struct UIKObjWindow *obj );
/* */
extern void __asm UIK_Input_Set( register __a0 struct UIKGlobal *uik, 
        register __d0 UBYTE input, register __d1 UBYTE held, 
        register __a1 UBYTE autom );
extern void __asm UIK_Input_Get( register __a0 struct UIKGlobal *uik, 
        register __d0 UBYTE *input, register __d1 UBYTE *held, 
        register __a1 UBYTE *autom );
extern struct UIKZone * __asm UIK_AddZone( register __a0 struct UIKObj *master, 
        register __a1 struct UIKObj* newobj, register __d0 ULONG events, 
        register __d1 void (*func)() );
/* */
/******************Gestion des UIKBuffers */
extern ULONG __asm BufGetUDatas( register __a0 UBYTE *buf, 
        register __a1 UWORD *pw );
extern ULONG __asm BufLength( register __a0 UBYTE *mem );
extern void __asm BufSetUDatas( register __a0 UBYTE *buf, 
        register __d0 ULONG uw, register __d1 ULONG ul );
extern void __asm BufFree( register __a1 UBYTE *mem );
extern UBYTE * __asm BufAlloc( register __a0 ULONG blksize, 
        register __d0 ULONG size, register __d1 ULONG reqs );
extern UBYTE * __asm BufResize( register __a0 UBYTE *old, 
        register __d0 ULONG newsize );
extern UBYTE * __asm BufClone( register __a0 UBYTE *old );
extern UBYTE * __asm BufCopy( register __a0 UBYTE *buf, 
        register __a1 UBYTE *dest, register __d0 ULONG start, 
        register __d1 ULONG len );
extern UBYTE * __asm BufCut( register __a0 UBYTE *buf, 
        register __d0 ULONG start, register __d1 ULONG len );
extern UBYTE * __asm BufPaste( register __a0 UBYTE *src, 
        register __d0 ULONG len, register __a1 UBYTE *dest, 
        register __d1 ULONG start );
/* */
extern void __asm UIK_Input_JamFrame( register __a1 struct UIKGlobal *uik );
extern void __asm UIK_Input_ReleaseFrame( register __a1 struct UIKGlobal *uik );
extern void __asm UIK_Input_SetTime( register __a1 struct UIKGlobal *uik, 
        register __d0 UWORD speed, register __d1 UWORD valid, 
        register __a0 UWORD wait );
extern void __asm UIK_Input_Flush( register __a1 struct UIKGlobal *uik );
extern void __asm UIK_Input_AddJoyEvent( register __a1 struct UIKGlobal *uik, 
        register __d0 UBYTE buttons, register __d1 ULONG flush );
/* */
extern void __asm UIK_Font2TTA( register __a1 struct TextFont *font, 
        register __a0 struct TTextAttr *tta );
extern struct TextFont * __asm UIK_OpenFontTTA( register __a0 struct TTextAttr *tta );
/* */
extern void __asm UIK_DrawAuto( register __a0 struct UIKObj *obj, 
        register __a1 UWORD *automate, register __d0 UWORD xoffs, 
        register __d1 UWORD yoffs );
/* */
extern void __asm UIK_Input_CycleZoneList( register __a0 struct UIKObj *obj, 
        register __d0 UBYTE cycle_on );
extern ULONG __asm UIK_Input_ChangeActiveZoneList( register __a0 struct UIKObj *obj );
/* */
extern ULONG __asm UIK_SaveEnvPrefs(  );
extern ULONG __asm UIK_ReadArcPrefs(  );
extern ULONG __asm UIK_OLi_NewTextPtr( register __a0 struct UIKObjList *lobj, 
        register __a1 UBYTE *buf );
/* */
extern UBYTE * __asm BufSet( register __a0 UBYTE *src, 
        register __d0 ULONG len, register __a1 UBYTE *dest );
/* */
extern ULONG __asm UIK_ChangeAutomate( register __a0 struct TagItem *TagList );
/* */
extern void __asm BufFreeS( register __a1 UBYTE **mem );
extern UBYTE * __asm BufResizeS( register __a0 UBYTE **old, 
        register __d0 ULONG newsize );
extern UBYTE * __asm BufCloneS( register __a0 UBYTE **old );
extern UBYTE * __asm BufCopyS( register __a0 UBYTE *buf, 
        register __a1 UBYTE **dest, register __d0 ULONG start, 
        register __d1 ULONG len );
extern UBYTE * __asm BufCutS( register __a0 UBYTE **buf, 
        register __d0 ULONG start, register __d1 ULONG len );
extern UBYTE * __asm BufPasteS( register __a0 UBYTE *src, 
        register __d0 ULONG len, register __a1 UBYTE **dest, 
        register __d1 ULONG start );
extern UBYTE * __asm BufSetS( register __a0 UBYTE *src, 
        register __d0 ULONG len, register __a1 UBYTE **dest );
/* */
extern UBYTE __asm UIK_GetColor( register __a0 struct UIKObj *obj, 
        register __d0 UBYTE uikcolor );
extern void __asm UIK_SetColor( register __a0 struct UIKObj *obj, 
        register __d0 UBYTE uik_color, register __d1 UBYTE palette_color );
/* */
/********* */
/* ------------------------------private------------------------------- */

#endif   /* CLIB_UIKINT_PROTOS_H */
