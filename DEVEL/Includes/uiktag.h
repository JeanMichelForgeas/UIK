#ifndef UIKDEFS_TAGITEM_H

#define UIKDEFS_TAGITEM_H TRUE
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef UTILITY_TAGITEM_H
#include <utility/tagitem.h>
#endif

#define UIKTAG_USER  (TAG_USER|(1L<<30))    /* differentiates uik user tags from uik tags*/

#define AllocateTagItems    UIK_AllocateTagItems
#define ApplyFindTag        UIK_ApplyFindTag
#define ApplyTagItems       UIK_ApplyTagItems
#define CloneTagItems       UIK_CloneTagItems
#define CopyTagItems        UIK_CopyTagItems
#define FilterTagChanges    UIK_FilterTagChanges
#define FilterTagItems      UIK_FilterTagItems
#define FindTagItem         UIK_FindTagItem
#define FreeTagItems        UIK_FreeTagItems
#define GetTagData          UIK_GetTagData
#define GetUserTag          UIK_GetUserTag
#define HowManyTagItems     UIK_HowManyTagItems
#define JoinTagItems        UIK_JoinTagItems
#define MapTags             UIK_MapTags
#define NextTagItem         UIK_NextTagItem
#define PackBoolTags        UIK_PackBoolTags
#define RefreshTagItemClones UIK_RefreshTagItemClones
#define SetTagData          UIK_SetTagData
#define TagInArray          UIK_TagInArray

#endif
