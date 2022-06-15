/*****************************************************************
 *
 *       Project:    UIK
 *       Function:   Tags processing
 *
 *       Created:    05/02/91    Jean-Michel Forgeas
 *
 *       This code is CopyRight © 1991 Jean-Michel Forgeas
 *       I hope it is compatible with corresponding function
 *       calls from Commodore in utility.library (2.0).
 *
 ****************************************************************/

/* Note : j'ai gardé la passage de paramètre dans les registres
 * afin de pouvoir tester en remplaçant directement les routines
 * dans la librairie, mais vous pouvez les enlever sans problème
 * et mettre des passages de paramètres normaux.
 *
 * Note : j'ai mis en tête du source, le fichier include de Commodore
 * qui s'appelle <utility/tagitem.h> et qui est appelé par le mien
 * qui est "uiki:uiktag.h".
 *
 * Note : j'ai été obligé de rajouter un paramètre à la fonction
 * UIK_ApplyFindTag() qui le recevait dans un registre que j'avais
 * oublié.
 *
 * Note : le programme de test doit être changé lui aussi car
 * d'une part j'avais un bug dans une valeur de retour, et d'autre
 * part une fonction de test comptait sur les valeurs des registres.
 */


/******* Includes ************************************************/

#include <exec/memory.h>
#include "uiki:uiktag.h"


/******* Imported ************************************************/


/******* Exported ************************************************/

ULONG __asm UIK_ApplyTagItems( register __a0 struct TagItem *TagList,
        register __a1 ULONG (*Func)(), register __d2 ULONG FuncParm,
        register __d0 ULONG TestRet, register __d1 ULONG TestValue );

ULONG __asm UIK_ApplyFindTag( register __a0 struct TagItem *ATagList,
        register __a1 struct TagItem *FTagList, register __d0 ULONG (*Perform)(),
        register __d1 ULONG parm );

struct TagItem  * __asm UIK_NextTagItem( register __a0 struct TagItem **TagItemPtr );

struct TagItem  * __asm UIK_GetUserTag( register __a0 struct TagItem *TagItem );

struct TagItem  * __asm UIK_FindTagItem( register __d0 Tag TagVal,
        register __a0 struct TagItem *TagList );

ULONG __asm UIK_GetTagData( register __d0 Tag TagVal,
        register __d1 ULONG Default, register __a0 struct TagItem *TagList );

struct TagItem * __asm UIK_SetTagData( register __d0 Tag TagVal,
        register __d1 ULONG newdata, register __a0 struct TagItem *TagList );

BOOL __asm UIK_TagInArray( register __d0 Tag TagVal,
        register __a0 Tag *TagArray );

ULONG __asm UIK_FilterTagItems( register __a0 struct TagItem *TagList,
        register __a1 Tag *TagArray, register __d0 ULONG Logic );

ULONG __asm UIK_PackBoolTags( register __d0 ULONG InitialFlags,
        register __a0 struct TagItem *TagList, register __a1 struct TagItem *BoolMap );

extern void __asm UIK_FilterTagChanges( register __a0 struct TagItem *ChangeList,
        register __a1 struct TagItem *OldValues, register __d0 ULONG Apply );

void __asm UIK_MapTags( register __a0 struct TagItem *TagList,
        register __a1 struct TagItem *MapList, register __d0 ULONG IncludeMiss );

struct TagItem  * __asm UIK_AllocateTagItems( register __d0 ULONG NumItems );

struct TagItem  * __asm UIK_CloneTagItems( register __a0 struct TagItem *TagList );

void __asm UIK_RefreshTagItemClones( register __a0 struct TagItem *CloneTagItems,
        register __a1 struct TagItem *OriginalTagItems );

void __asm UIK_CopyTagItems( register __a0 struct TagItem *OriginalTagItems,
        register __a1 struct TagItem *CloneTagItems );

void __asm UIK_FreeTagItems( register __a0 struct TagItem *TagList );

ULONG __asm UIK_HowManyTagItems( register __a0 struct TagItem *TagList );

void __asm UIK_JoinTagItems( register __a0 struct TagItem *TagList1,
        register __a1 struct TagItem *TagList2 );


/***************************************************************************
 *
 *      Code
 *
 ***************************************************************************/

ULONG __asm UIK_ApplyTagItems(
register __a0 struct TagItem *TagList,
register __a1 ULONG (*Func)(),
register __d2 ULONG FuncParm,
register __d0 ULONG TestRet,
register __d1 ULONG TestValue )
{
  struct TagItem *ti;
  ULONG retcode = 0;

    if (! (ti = TagList)) return(0); // test pointeur null

    for (; ; ti++)
        {
        if (! (ti = UIK_GetUserTag( ti ))) return(0);  // saute les tags Système éventuels
        retcode = (*Func)( ti, FuncParm );
        if (TestRet)
            {
            if (retcode == TestValue)
                break;
            }
        }
    return( retcode );
}

//--------------------------------------------------------------------------

struct TagItem  * __asm UIK_NextTagItem(
register __a0 struct TagItem **TagItemPtr )
{
  struct TagItem *ti;

    if (! (ti = *TagItemPtr)) return(0); // test pointeur null
    ti++;
    ti = UIK_GetUserTag( ti );
    *TagItemPtr = ti;
    return( ti );
}

//--------------------------------------------------------------------------

struct TagItem  * __asm UIK_GetUserTag(
register __a0 struct TagItem *TagItem )
{
  struct TagItem *ti;

    for (ti=TagItem; ; )
        {
        switch( ti->ti_Tag )
            {
            case TAG_DONE:   return(0);               /* end of taglist */
            case TAG_IGNORE: break;                   /* goto next tag */
            case TAG_MORE:   ti = (struct TagItem *)ti->ti_Data; continue; /* adress of next tag is in ti_Data */
            case TAG_SKIP:   ti++; break;             /* skip 2 tags (this & the next) */
            default: return( ti );
            }
        ti++;
        }
}

//--------------------------------------------------------------------------

struct TagItem  * __asm UIK_FindTagItem(
register __d0 Tag TagVal,
register __a0 struct TagItem *TagList )
{
  struct TagItem *ti;

    if (! (ti = TagList)) return(0);

    for (; ti = UIK_GetUserTag(ti); ti++)
        {
        if (ti->ti_Tag == TagVal) break;
        }
    return( ti );
}

//--------------------------------------------------------------------------

ULONG __asm UIK_GetTagData(
register __d0 Tag TagVal,
register __d1 ULONG Default,
register __a0 struct TagItem *TagList )
{
  struct TagItem *ti;

    return( (ti = UIK_FindTagItem( TagVal, TagList )) ? ti->ti_Data : Default );
}

//--------------------------------------------------------------------------

struct TagItem * __asm UIK_SetTagData(
register __d0 Tag TagVal,
register __d1 ULONG newdata,
register __a0 struct TagItem *TagList )
{
  struct TagItem *ti;

    if (ti = UIK_FindTagItem( TagVal, TagList ))
        {
        ti->ti_Data = newdata;
        }
    return( ti );
}

//--------------------------------------------------------------------------

ULONG __asm UIK_ApplyFindTag(
register __a0 struct TagItem *ATagList,
register __a1 struct TagItem *FTagList,
register __d0 ULONG (*Perform)(),
register __d1 ULONG parm )
{
  struct TagItem *ti, *fti;

    if (! (ti = ATagList)) return( parm ); // test pointeur null

    for (; ti = UIK_GetUserTag( ti ); ti++)
        {
        fti = UIK_FindTagItem( ti->ti_Tag, FTagList );
        parm = (*Perform)( ti, fti, parm );    // fti is null if ti->ti_Tag was not found
        }
    return( parm );
}

//--------------------------------------------------------------------------

ULONG __asm UIK_FilterTagItems(
register __a0 struct TagItem *TagList,
register __a1 Tag *TagArray,
register __d0 ULONG Logic )
{
  ULONG num;
  struct TagItem *ti;

    if (! (ti = TagList)) return(0); // test pointeur null

    for (num=0; ti = UIK_GetUserTag( ti ); ti++)
        {
        if (UIK_TagInArray( ti->ti_Tag, TagArray ))
            {
            if (Logic == TAGFILTER_NOT) ti->ti_Tag = TAG_IGNORE; else num++;
            }
        else{
            if (Logic == TAGFILTER_NOT) num++; else ti->ti_Tag = TAG_IGNORE;
            }
        }
    return( num );
}

//--------------------------------------------------------------------------

BOOL __asm UIK_TagInArray(
register __d0 Tag TagVal,
register __a0 Tag *TagArray )
{
    for (; *TagArray != TAG_DONE; TagArray++)
        {
        if (*TagArray == TagVal) return(1);
        }
    return(0);
}

//--------------------------------------------------------------------------

ULONG each_PackBoolTags( struct TagItem *ti, struct TagItem *fti, ULONG flags )
{
  ULONG mask;

    if (fti)
        {
        mask = fti->ti_Data;
        if (ti->ti_Data == TRUE) flags |= mask;
        else flags &= ~mask;
        }
    return( flags );
}

ULONG __asm UIK_PackBoolTags(
register __d0 ULONG InitialFlags,
register __a0 struct TagItem *TagList,
register __a1 struct TagItem *BoolMap )
{
    return( UIK_ApplyFindTag( TagList, BoolMap, (void*)each_PackBoolTags, InitialFlags ) );
}

//--------------------------------------------------------------------------

ULONG each_FilterTagChanges( struct TagItem *ChangeList, struct TagItem *OldList, ULONG apply )
{
    if (OldList)
        {
        if (ChangeList->ti_Data == OldList->ti_Data)
            {
            ChangeList->ti_Tag = TAG_IGNORE;
            }
        else if (apply)
            {
            OldList->ti_Data = ChangeList->ti_Data;
            }
        }
    else{
        ChangeList->ti_Tag = TAG_IGNORE;
        }
    return( apply );
}

void __asm UIK_FilterTagChanges(
register __a0 struct TagItem *ChangeList,
register __a1 struct TagItem *OldValues,
register __d0 ULONG Apply )
{
    UIK_ApplyFindTag( ChangeList, OldValues, (void*)each_FilterTagChanges, Apply );
}

//--------------------------------------------------------------------------

ULONG each_MapTags( struct TagItem *TagList, struct TagItem *MapList, ULONG IncludeMiss )
{
    if (MapList)
        {
        TagList->ti_Tag = MapList->ti_Data;
        }
    else{
        if (IncludeMiss == MAP_REMOVE_NOT_FOUND)
            {
            TagList->ti_Tag = TAG_IGNORE;
            }
        }
    return( IncludeMiss );
}

void __asm UIK_MapTags(
register __a0 struct TagItem *TagList,
register __a1 struct TagItem *MapList,
register __d0 ULONG IncludeMiss )
{
    UIK_ApplyFindTag( TagList, MapList, (void*)each_MapTags, IncludeMiss );
}

//--------------------------------------------------------------------------

struct TagItem * __asm UIK_AllocateTagItems(
register __d0 ULONG NumItems )
{
  ULONG size, *ptr;

    size = 4 + ((NumItems + 1) * sizeof(struct TagItem));
    if (ptr = (ULONG *) AllocMem( size, MEMF_ANY|MEMF_CLEAR ))
        {
        *ptr++ = size;        // remember size into the first long word
        ((struct TagItem *)ptr)[NumItems].ti_Tag = TAG_DONE;
        }
    return( (struct TagItem *)ptr );
}

//--------------------------------------------------------------------------

struct TagItem  * __asm UIK_CloneTagItems(
register __a0 struct TagItem *TagList )
{
  ULONG numtags;
  struct TagItem *ti;

    numtags = UIK_HowManyTagItems( TagList );
    if (ti = UIK_AllocateTagItems( numtags ))
        {
        UIK_CopyTagItems( TagList, ti );
        }
    return( ti );
}

//--------------------------------------------------------------------------

void __asm UIK_RefreshTagItemClones(
register __a0 struct TagItem *CloneTagItems,
register __a1 struct TagItem *OriginalTagItems )
{
    return( UIK_CopyTagItems( OriginalTagItems, CloneTagItems ) );
}

//--------------------------------------------------------------------------

void __asm UIK_CopyTagItems(
register __a0 struct TagItem *OriginalTagItems,
register __a1 struct TagItem *CloneTagItems )
{
    if (! OriginalTagItems) return;

    for (; OriginalTagItems = UIK_GetUserTag( OriginalTagItems ); )
        {
        *CloneTagItems++ = *OriginalTagItems++;
        }
}

//--------------------------------------------------------------------------

void __asm UIK_FreeTagItems( register __a0 struct TagItem *TagList )
{
    if (TagList)
        {
        ULONG *ptr = (((ULONG*)TagList) - 1);
        FreeMem( ptr, *ptr );
        }
}

//--------------------------------------------------------------------------

ULONG __asm UIK_HowManyTagItems( register __a0 struct TagItem *TagList )
{
  ULONG num;

    for (num=0; TagList = UIK_GetUserTag( TagList ); num++, TagList++) ;
    return( num );
}

//--------------------------------------------------------------------------

void __asm UIK_JoinTagItems(
register __a0 struct TagItem *TagList1,
register __a1 struct TagItem *TagList2 )
{
    for (; TagList1->ti_Tag != TAG_DONE; TagList1++) ;
    TagList1->ti_Tag = TAG_MORE;
    TagList1->ti_Data = (ULONG)TagList2;
}
