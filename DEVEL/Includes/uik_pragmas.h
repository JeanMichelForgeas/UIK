#ifndef UIKBase_INLINE
#define UIKBase_INLINE

#ifndef CLIB_UIK_PROTOS_H
#include "uiki:uik_protos.h"
#endif

#ifdef __DICE_INLINE
#ifndef UIKBase_DECLARED
#define UIKBase_DECLARED
extern struct Library *UIKBase;
#endif
#endif

/* "uik.library" */
/* */
/*      (C) Copyright 1991,1992,1993 Jean-Michel Forgeas" */
/*                  Tous Droits Réservés" */
/* */
/* */
/* uik.library */
/* First function is always at an offset of -30 from lib base */
/******** */
/* ------------------------------public------------------------------- */
#pragma libcall UIKBase UIK_IsSystemVersion2 1E 0
#pragma libcall UIKBase UIK_DeadKeyConvert 24 09803
#pragma libcall UIKBase UIK_InvertBlackWhite 2A 09803
#pragma libcall UIKBase UIK_EnableDOSReq 30 0102
#pragma libcall UIKBase UIK_DisableDOSReq 36 0
#pragma libcall UIKBase UIK_BaseName 3C 801
#pragma libcall UIKBase UIK_OpenFont 42 0802
#pragma libcall UIKBase UIK_ReopenFont 48 901
#pragma libcall UIKBase UIK_AccessPrefs 4E 19803
#pragma libcall UIKBase UIK_ReadPrefs 54 0
#pragma libcall UIKBase UIK_SavePrefs 5A 0
#pragma libcall UIKBase UIK_GetPrefs 60 1902
#pragma libcall UIKBase UIK_SetPrefs 66 1902
/* */
/* */
#pragma libcall UIKBase UIK_Disable 78 801
#pragma libcall UIKBase UIK_Enable 7E 801
/* */
#pragma libcall UIKBase UIK_InsideObj 84 9802
#pragma libcall UIKBase UIK_LockObjVars 8A 801
#pragma libcall UIKBase UIK_UnlockObjVars 90 801
#pragma libcall UIKBase UIK_ObjTitle 96 801
#pragma libcall UIKBase UIK_ObjAltTitle 9C 801
#pragma libcall UIKBase UIK_LangString A2 0802
#pragma libcall UIKBase UIK_OMe_MemPtr A8 801
/* */
#pragma libcall UIKBase UIK_AddVector AE 109804
#pragma libcall UIKBase UIK_RemoveVector B4 8902
#pragma libcall UIKBase UIK_RemAllVectors BA 901
#pragma libcall UIKBase UIK_Stop C0 801
#pragma libcall UIKBase UIK_Remove C6 801
#pragma libcall UIKBase UIK_Refresh CC 0802
#pragma libcall UIKBase UIK_RefreshWindow D2 801
#pragma libcall UIKBase UIK_CauseObjHook D8 0802
#pragma libcall UIKBase UIK_CauseObjLink DE 0802
#pragma libcall UIKBase UIK_Link E4 2109805
#pragma libcall UIKBase UIK_Start EA 801
#pragma libcall UIKBase UIK_Do F0 0802
/* */
#pragma libcall UIKBase UIK_AddObject F6 90803
#pragma libcall UIKBase UIK_GetObjectValues FC 9802
#pragma libcall UIKBase UIK_SetObjectValues 102 9802
/* */
#pragma libcall UIKBase UIK_OPr_ChangePropGadget 108 109804
#pragma libcall UIKBase UIK_OPr_ChangePropNoDraw 10E 109804
/* */
#pragma libcall UIKBase UIK_OLi_DrawList 114 801
#pragma libcall UIKBase UIK_OLi_DrawListFrom 11A 10803
#pragma libcall UIKBase UIK_OLi_DrawLine 120 0802
/* */
#pragma libcall UIKBase UIK_nl_NewList 126 001
#pragma libcall UIKBase UIK_nl_ListPtr 12C 801
#pragma libcall UIKBase UIK_nl_FreeList 132 801
#pragma libcall UIKBase UIK_nl_DeleteList 138 801
#pragma libcall UIKBase UIK_nl_AttachList 13E 9802
#pragma libcall UIKBase UIK_nl_DetachList 144 0802
#pragma libcall UIKBase UIK_nl_FreeNodes 14A 10803
#pragma libcall UIKBase UIK_nl_SortList 150 9802
#pragma libcall UIKBase UIK_nl_PasteToNode 156 2910805
#pragma libcall UIKBase UIK_nl_CutFromNode 15C 210804
/* */
#pragma libcall UIKBase UIK_nl_ListNodeNum 162 801
#pragma libcall UIKBase UIK_nl_NodeBufLen 168 0802
#pragma libcall UIKBase UIK_nl_CrossList 16E 109804
#pragma libcall UIKBase UIK_nl_AllocList 174 0802
#pragma libcall UIKBase UIK_nl_FindNode 17A 0802
#pragma libcall UIKBase UIK_nl_AllocNode 180 910804
#pragma libcall UIKBase UIK_nl_ReAllocNode 186 910804
#pragma libcall UIKBase UIK_nl_NodeBuf 18C 0802
/* */
#pragma libcall UIKBase UIK_OLi_ChangeLine 192 109804
#pragma libcall UIKBase UIK_OLi_AddLineLen 198 2109805
#pragma libcall UIKBase UIK_OLi_AddLine 19E 109804
#pragma libcall UIKBase UIK_OLi_TotLines 1A4 801
#pragma libcall UIKBase UIK_OLi_NewText 1AA 9802
#pragma libcall UIKBase UIK_OLi_DelLine 1B0 0802
#pragma libcall UIKBase UIK_OLi_UnselectList 1B6 801
#pragma libcall UIKBase UIK_OLi_SelectList 1BC 0802
#pragma libcall UIKBase UIK_OLi_ActualizeList 1C2 801
#pragma libcall UIKBase UIK_OLi_SetListTop 1C8 0802
#pragma libcall UIKBase UIK_OLi_MoveListFrame 1CE 0802
#pragma libcall UIKBase UIK_OLi_MoveListSel 1D4 0802
#pragma libcall UIKBase UIK_OLi_LineBuffer 1DA 0802
/* */
#pragma libcall UIKBase StrNCCmp 1E0 9802
#pragma libcall UIKBase StcCpy 1E6 98003
#pragma libcall UIKBase Str2Num 1EC 801
#pragma libcall UIKBase StrB2C 1F2 9802
#pragma libcall UIKBase StrCat 1F8 9802
#pragma libcall UIKBase StrCmp 1FE 9802
#pragma libcall UIKBase StrCpy 204 9802
#pragma libcall UIKBase StrLen 20A 801
/* */
#pragma libcall UIKBase UIK_NextTagItem 210 801
#pragma libcall UIKBase UIK_GetUserTag 216 801
#pragma libcall UIKBase UIK_FindTagItem 21C 8002
#pragma libcall UIKBase UIK_AllocateTagItems 222 001
#pragma libcall UIKBase UIK_CloneTagItems 228 801
#pragma libcall UIKBase UIK_TagInArray 22E 8002
#pragma libcall UIKBase UIK_PackBoolTags 234 98003
#pragma libcall UIKBase UIK_GetTagData 23A 81003
#pragma libcall UIKBase UIK_ApplyTagItems 240 1029805
#pragma libcall UIKBase UIK_FilterTagItems 246 09803
#pragma libcall UIKBase UIK_HowManyTagItems 24C 801
#pragma libcall UIKBase UIK_ApplyFindTag 252 109804
#pragma libcall UIKBase UIK_FilterTagChanges 258 09803
#pragma libcall UIKBase UIK_MapTags 25E 09803
#pragma libcall UIKBase UIK_RefreshTagItemClones 264 9802
#pragma libcall UIKBase UIK_CopyTagItems 26A 9802
#pragma libcall UIKBase UIK_FreeTagItems 270 801
#pragma libcall UIKBase UIK_JoinTagItems 276 9802
/* */
#pragma libcall UIKBase UIK_AskIconPlace 27C 09803
/* */
#pragma libcall UIKBase UIK_MemTAlloc 282 1002
#pragma libcall UIKBase UIK_MemSpAlloc 288 1002
#pragma libcall UIKBase UIK_MemSpResize 28E 10803
#pragma libcall UIKBase UIK_MemTFree 294 801
#pragma libcall UIKBase UIK_MemSpFree 29A 901
#pragma libcall UIKBase UIK_MemCopy 2A0 09803
#pragma libcall UIKBase UIK_MemSpLength 2A6 801
/* */
#pragma libcall UIKBase UIK_ClipObject 2AC 901
#pragma libcall UIKBase UIK_UnclipObject 2B2 901
/* */
#pragma libcall UIKBase UIK_OME_ActiveMExObj 2B8 801
#pragma libcall UIKBase UIK_CharWidth 2BE 0802
#pragma libcall UIKBase UIK_OSt_Get 2C4 9802
#pragma libcall UIKBase UIK_OSt_Set 2CA 09803
#pragma libcall UIKBase UIK_OSt_Cut 2D0 10803
#pragma libcall UIKBase UIK_OSt_Paste 2D6 109804
#pragma libcall UIKBase UIK_ActivateKeyTank 2DC 801
/* */
#pragma libcall UIKBase UIK_RegisterObject 2E2 0802
#pragma libcall UIKBase UIK_UnregisterObject 2E8 0802
/* */
#pragma libcall UIKBase UIK_IsActiveKeyTank 2EE 801
/* */
#pragma libcall UIKBase UIK_InactivateKeyTank 2F4 801
/* */
#pragma libcall UIKBase UIK_ToUpper 2FA 001
#pragma libcall UIKBase UIK_ToLower 300 001
#pragma libcall UIKBase UIK_StrToUpper 306 801
#pragma libcall UIKBase UIK_StrToLower 30C 801
/* */
/* */
#pragma libcall UIKBase UIK_Match 318 98003
/* */
#pragma libcall UIKBase UIK_StdGadStart 31E 801
#pragma libcall UIKBase UIK_StdGadStop 324 801
#pragma libcall UIKBase UIK_StdGadResize 32A 801
#pragma libcall UIKBase UIK_StdGadRefresh 330 801
#pragma libcall UIKBase UIK_StdGadAct 336 801
#pragma libcall UIKBase UIK_StdGadInact 33C 801
#pragma libcall UIKBase UIK_StdGadMove 342 B802
/* */
#pragma libcall UIKBase UIK_AddPrivateObject 348 90803
/* */
#pragma libcall UIKBase UIK_OTo_SelectToggle 34E 801
#pragma libcall UIKBase UIK_OTo_UnselectToggle 354 801
/* */
#pragma libcall UIKBase UIK_Do2 35A 10803
#pragma libcall UIKBase UIK_CallObjectFunc 360 180904
/* */
#pragma libcall UIKBase UIK_SetTagData 366 81003
/* */
#pragma libcall UIKBase UIK_MakeUIKTree 36C A9803
/* */
#pragma libcall UIKBase UIK_DrawBox 372 801
#pragma libcall UIKBase UIK_RefreshSons 378 801
/* */
#pragma libcall UIKBase UIK_StrWidth 37E 09803
/* */
#pragma libcall UIKBase UIK_FreeBMapRPort 384 801
#pragma libcall UIKBase UIK_FreeRPort 38A 801
#pragma libcall UIKBase UIK_FreeBMap 390 801
#pragma libcall UIKBase UIK_AllocBMapRPort 396 981004
#pragma libcall UIKBase UIK_AllocRPort 39C 0
#pragma libcall UIKBase UIK_AllocBMap 3A2 981004
/* */
#pragma libcall UIKBase UIK_Select 3A8 0802
/* */
#pragma libcall UIKBase UIK_Iconify 3AE 801
#pragma libcall UIKBase UIK_Uniconify 3B4 801
/* */
#pragma libcall UIKBase UIK_Input_Set 3BA 910804
#pragma libcall UIKBase UIK_Input_Get 3C0 910804
#pragma libcall UIKBase UIK_AddZone 3C6 109804
/* */
/****************** UIKBuffers management */
#pragma libcall UIKBase BufGetUDatas 3CC 9802
#pragma libcall UIKBase BufLength 3D2 801
#pragma libcall UIKBase BufSetUDatas 3D8 10803
#pragma libcall UIKBase BufFree 3DE 901
#pragma libcall UIKBase BufAlloc 3E4 10803
#pragma libcall UIKBase BufResize 3EA 0802
#pragma libcall UIKBase BufClone 3F0 801
#pragma libcall UIKBase BufCopy 3F6 109804
#pragma libcall UIKBase BufCut 3FC 10803
#pragma libcall UIKBase BufPaste 402 190804
/* */
#pragma libcall UIKBase UIK_Input_JamFrame 408 901
#pragma libcall UIKBase UIK_Input_ReleaseFrame 40E 901
#pragma libcall UIKBase UIK_Input_SetTime 414 810904
#pragma libcall UIKBase UIK_Input_Flush 41A 901
#pragma libcall UIKBase UIK_Input_AddJoyEvent 420 10903
/* */
#pragma libcall UIKBase UIK_Font2TTA 426 8902
#pragma libcall UIKBase UIK_OpenFontTTA 42C 801
/* */
#pragma libcall UIKBase UIK_DrawAuto 432 109804
/* */
#pragma libcall UIKBase UIK_Input_CycleZoneList 438 0802
#pragma libcall UIKBase UIK_Input_ChangeActiveZoneList 43E 801
/* */
#pragma libcall UIKBase UIK_SaveEnvPrefs 444 0
#pragma libcall UIKBase UIK_ReadArcPrefs 44A 0
#pragma libcall UIKBase UIK_OLi_NewTextPtr 450 9802
/* */
#pragma libcall UIKBase BufSet 456 90803
/* */
#pragma libcall UIKBase UIK_ChangeAutomate 45C 801
/* */
#pragma libcall UIKBase BufFreeS 462 901
#pragma libcall UIKBase BufResizeS 468 0802
#pragma libcall UIKBase BufCloneS 46E 801
#pragma libcall UIKBase BufCopyS 474 109804
#pragma libcall UIKBase BufCutS 47A 10803
#pragma libcall UIKBase BufPasteS 480 190804
#pragma libcall UIKBase BufSetS 486 90803
/* */
#pragma libcall UIKBase UIK_GetColor 48C 0802
#pragma libcall UIKBase UIK_SetColor 492 10803
/* */
#pragma libcall UIKBase UIK_ProcessPendingEvents 498 0802
#pragma libcall UIKBase UIK_UniconifyWindow 49E 801
#pragma libcall UIKBase UIK_WaitFirstEvents 4A4 0802
/* */
#pragma libcall UIKBase UIK_StripWindowEvents 4AA 0802
/* */
/* New for V1.40 */
/* */
#pragma libcall UIKBase BufTruncate 4B0 0802
#pragma libcall UIKBase UIK_OLi_DeleteLine 4B6 0802
#pragma libcall UIKBase BufAllocP 4BC 10803
#pragma libcall UIKBase UIK_nl_NewListP 4C2 3210805
#pragma libcall UIKBase UIK_nl_AllocListP 4C8 32109806
#pragma libcall UIKBase UIK_GetTaskUIK 4CE 801
#pragma libcall UIKBase UIK_GetObject 4D4 09803
#pragma libcall UIKBase VBufPrintf 4DA 190804
#pragma libcall UIKBase UIK_OME_InactiveMExObj 4E0 801
#pragma libcall UIKBase UIK_ShortCutRemove 4E6 801
#pragma libcall UIKBase UIK_ObjDisable 4EC 0802
#pragma libcall UIKBase UIK_ObjEnable 4F2 0802
#pragma libcall UIKBase UIK_GetRastPort 4F8 801
/* */
/* New for V1.42 */
/* */
#pragma libcall UIKBase UIK_CmplPattern 4FE 09803
/* */

#ifdef __SASC
#if __VERSION__ >= 6

#pragma tagcall UIKBase UIK_AddObjectTags F6 90803
#pragma tagcall UIKBase UIK_AddPrivateObjectTags 348 90803
#pragma tagcall UIKBase UIK_CallObjectFuncTags 360 180904
#pragma tagcall UIKBase BufPrintf 4DA 190804

#endif
#endif

#endif
