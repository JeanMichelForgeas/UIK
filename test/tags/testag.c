/****************************************************************
 *
 *      Project:   UIK
 *      Function:  Essai des fonctions UIK pour les Tags
 *
 *      Created:   12/05/90    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include <stdio.h>

#ifdef LATTICE
int CXBRK(void) { return(0); }
int chkabort(void) { return(0); }
#endif

#include <pragmas/utility_pragmas.h>
#include <clib/utility_protos.h>

#include "uiki:uiktag.h"
#include "uiki:uikbase.h"

#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"


/****** Imported ************************************************/


/****** Exported ************************************************/

struct GfxBase *GfxBase;
struct UIKBase *UIKBase;


/****** Statics *************************************************/

static struct TagItem TagsB[] =
    {
    { TAG_SKIP },
    { 3000,     4000 },
    { 3001,     4001 },
    { 3002,     4002 },
    { 3003,     4003 },
    { 3004,     4004 },
    { 3005,     4005 },
    { 3006,     4006 },
    { TAG_IGNORE },
    { TAG_DONE }
    };

static struct TagItem TagsA[] =
    {
    { 1000,     2000 },
    { TAG_SKIP },
    { 1001,     2001 },
    { TAG_IGNORE },
    { 1002,     2002 },
    { TAG_DONE }
    };

/*-----------------*/

static Tag tagarray[] =
    { 1001, 1003, 1005, 1007, 3001, 3003, 3005,3007 };

static enum mytags { tag1 = TAG_USER+1, tag2, tag3, tag4, tag5 };

static struct TagItem boolmap[] =
    {
    { tag1,  0x0001 },
    { tag2,  0x0002 },
    { tag3,  0x0004 },
    { tag4,  0x0008 },
    { TAG_DONE }
    };

static struct TagItem boolexample[] =
    {
    { tag1,  TRUE },
    { tag2,  FALSE },
    { tag5,  45 },
    { tag3,  TRUE },
    { TAG_DONE }
    };

/*-----------------*/

static enum  mytags2 { MY_SIZE = 7000, MY_WEIGHT, HIS_TALL };
/*       Consider this source list: */
static struct TagItem list[] = {
               { MY_SIZE,      71 },
               { MY_WEIGHT,    200 },
               { TAG_END,      } };
/*       And the mapping list: */
static struct TagItem map[] = {
               { MY_SIZE,      HIS_TALL },
               { TAG_END,      } };

/*       Then after UIK_MapTags( list, map, 0 ), 'list' will become: */
/*             { HIS_TALL, 71 },                                 */
/*             { TAG_IGNORE, },                                  */
/*             { TAG_END, }                                      */
/*       Then after UIK_MapTags( list, map, 1 ), 'list' will become: */
/*             { HIS_TALL, 71 },                                 */
/*             { MY_WEIGHT, 200 },                               */
/*             { TAG_END, }                                      */

/*-----------------*/

static enum  mytags3 { ATTR_Size = 8000, ATTR_Color, ATTR_Shape };

static struct TagItem ChangeList[] = {
               { ATTR_Size,  100 },
               { ATTR_Shape, 400 },
               { TAG_END,      } };

static struct TagItem OldList[] = {
               { ATTR_Size,  100 },
               { ATTR_Color, 200 },
               { ATTR_Shape, 300 },
               { TAG_END,      } };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static int print_tagitem( struct TagItem *ti )
{
    printf( "     { %ld,\t%ld },\n", ti->ti_Tag, ti->ti_Data );
    return(10);
}

void main()
{
  struct TagItem *ti, *tagptr;
  struct TagItem *tic;

    if (! (UIKBase = (struct UIKBase *) OpenLibrary( UIKNAME, 0 ))) exit(20);

    printf("UIK_NextTagItem...\n");
    tagptr = &TagsA[0];
    for (ti=tagptr; ti; ti = UIK_NextTagItem( &tagptr )) print_tagitem( ti );

    printf("UIK_JoinTagItems...\n");
    UIK_JoinTagItems( TagsA, TagsB );
    UIK_ApplyTagItems( TagsA, (void*)print_tagitem, 0, 0, 0 );

    printf("UIK_FindTagItem...\n");
    if (ti = UIK_FindTagItem( 3001, TagsA )) print_tagitem( ti ); else printf("    %ld not found\n", 3001);
    if (ti = UIK_FindTagItem( 3001, TagsB )) print_tagitem( ti ); else printf("    %ld not found\n", 3001);
    if (ti = UIK_FindTagItem( 3011, TagsA )) print_tagitem( ti ); else printf("    %ld not found\n", 3011);
    if (ti = UIK_FindTagItem( 3000, TagsB )) print_tagitem( ti ); else printf("    %ld not found\n", 3000);

    printf("UIK_GetTagData...\n");
    printf("    (3001/1) %ld\n", UIK_GetTagData( 3001, 1, TagsA ) );
    printf("    (3004/4) %ld\n", UIK_GetTagData( 3004, 4, TagsA ) );

    printf("UIK_TagInArray...\n");
    printf("    (3001) %ld\n", UIK_TagInArray( 3001, tagarray ) );
    printf("    (3004) %ld\n", UIK_TagInArray( 3004, tagarray ) );

    printf("UIK_PackBoolTags...\n");
    printf("    Flags (0x800002) => %lx\n", UIK_PackBoolTags( 0x800002, boolexample, boolmap ) );

    printf("UIK_MapTags...\n");
    UIK_MapTags( list, map, 1 );
    UIK_ApplyTagItems( list, (void*)print_tagitem, 0, 0, 0 );

    printf("UIK_CloneTagItems...\n");
    if (tic = UIK_CloneTagItems( TagsA ))
        {
        UIK_ApplyTagItems( tic, (void*)print_tagitem, 0, 1, 10 );

        printf("UIK_FilterTagItems TAGFILTER_AND...\n");
        printf( "    %ld valids\n", UIK_FilterTagItems( tic, tagarray, TAGFILTER_AND ) );
        UIK_ApplyTagItems( tic, (void*)print_tagitem, 0, 0, 0 );

        printf("UIK_RefreshTagItemClones...\n");
        UIK_RefreshTagItemClones( tic, TagsA );
        printf("UIK_FilterTagItems TAGFILTER_NOT...\n");
        printf( "    %ld valids\n", UIK_FilterTagItems( tic, tagarray, TAGFILTER_NOT ) );
        UIK_ApplyTagItems( tic, (void*)print_tagitem, 0, 0, 0 );
        UIK_FreeTagItems( tic );
        }

    printf("UIK_FilterTagChanges...\n");
    if (tic = UIK_CloneTagItems( ChangeList ))
        {
        printf("UIK_FilterTagChanges 0...\n");
        UIK_FilterTagChanges( tic, OldList, 0 );
        printf("    ChangeList:\n");
        UIK_ApplyTagItems( tic, (void*)print_tagitem, 0, 0, 0 );
        printf("    OldList:\n");
        UIK_ApplyTagItems( OldList, (void*)print_tagitem, 0, 0, 0 );

        UIK_RefreshTagItemClones( tic, ChangeList );
        printf("UIK_FilterTagChanges 1...\n");
        UIK_FilterTagChanges( tic, OldList, 1 );
        printf("    ChangeList:\n");
        UIK_ApplyTagItems( tic, (void*)print_tagitem, 0, 0, 0 );
        printf("    OldList:\n");
        UIK_ApplyTagItems( OldList, (void*)print_tagitem, 0, 0, 0 );

        UIK_FreeTagItems( tic );
        }

    CloseLibrary( UIKBase );
}
