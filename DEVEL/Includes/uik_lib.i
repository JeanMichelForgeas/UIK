**
**      (C) Copyright 1991,1992,1993 Jean-Michel Forgeas
**                  Tous Droits R�serv�s
**
_LVOUIK_IsSystemVersion2 equ -30
_LVOUIK_DeadKeyConvert equ -36
_LVOUIK_InvertBlackWhite equ -42
_LVOUIK_EnableDOSReq equ -48
_LVOUIK_DisableDOSReq equ -54
_LVOUIK_BaseName equ -60
_LVOUIK_OpenFont equ -66
_LVOUIK_ReopenFont equ -72
_LVOUIK_AccessPrefs equ -78
_LVOUIK_ReadPrefs equ -84
_LVOUIK_SavePrefs equ -90
_LVOUIK_GetPrefs equ -96
_LVOUIK_SetPrefs equ -102
_LVOUIK_Disable equ -120
_LVOUIK_Enable equ -126
_LVOUIK_InsideObj equ -132
_LVOUIK_LockObjVars equ -138
_LVOUIK_UnlockObjVars equ -144
_LVOUIK_ObjTitle equ -150
_LVOUIK_ObjAltTitle equ -156
_LVOUIK_LangString equ -162
_LVOUIK_OMe_MemPtr equ -168
_LVOUIK_AddVector equ -174
_LVOUIK_RemoveVector equ -180
_LVOUIK_RemAllVectors equ -186
_LVOUIK_Stop equ -192
_LVOUIK_Remove equ -198
_LVOUIK_Refresh equ -204
_LVOUIK_RefreshWindow equ -210
_LVOUIK_CauseObjHook equ -216
_LVOUIK_CauseObjLink equ -222
_LVOUIK_Link equ -228
_LVOUIK_Start equ -234
_LVOUIK_Do equ -240
_LVOUIK_AddObject equ -246
_LVOUIK_GetObjectValues equ -252
_LVOUIK_SetObjectValues equ -258
_LVOUIK_OPr_ChangePropGadget equ -264
_LVOUIK_OPr_ChangePropNoDraw equ -270
_LVOUIK_OLi_DrawList equ -276
_LVOUIK_OLi_DrawListFrom equ -282
_LVOUIK_OLi_DrawLine equ -288
_LVOUIK_nl_NewList equ -294
_LVOUIK_nl_ListPtr equ -300
_LVOUIK_nl_FreeList equ -306
_LVOUIK_nl_DeleteList equ -312
_LVOUIK_nl_AttachList equ -318
_LVOUIK_nl_DetachList equ -324
_LVOUIK_nl_FreeNodes equ -330
_LVOUIK_nl_SortList equ -336
_LVOUIK_nl_PasteToNode equ -342
_LVOUIK_nl_CutFromNode equ -348
_LVOUIK_nl_ListNodeNum equ -354
_LVOUIK_nl_NodeBufLen equ -360
_LVOUIK_nl_CrossList equ -366
_LVOUIK_nl_AllocList equ -372
_LVOUIK_nl_FindNode equ -378
_LVOUIK_nl_AllocNode equ -384
_LVOUIK_nl_ReAllocNode equ -390
_LVOUIK_nl_NodeBuf equ -396
_LVOUIK_OLi_ChangeLine equ -402
_LVOUIK_OLi_AddLineLen equ -408
_LVOUIK_OLi_AddLine equ -414
_LVOUIK_OLi_TotLines equ -420
_LVOUIK_OLi_NewText equ -426
_LVOUIK_OLi_DelLine equ -432
_LVOUIK_OLi_UnselectList equ -438
_LVOUIK_OLi_SelectList equ -444
_LVOUIK_OLi_ActualizeList equ -450
_LVOUIK_OLi_SetListTop equ -456
_LVOUIK_OLi_MoveListFrame equ -462
_LVOUIK_OLi_MoveListSel equ -468
_LVOUIK_OLi_LineBuffer equ -474
_LVOStrNCCmp equ -480
_LVOStcCpy equ -486
_LVOStr2Num equ -492
_LVOStrB2C equ -498
_LVOStrCat equ -504
_LVOStrCmp equ -510
_LVOStrCpy equ -516
_LVOStrLen equ -522
_LVOUIK_NextTagItem equ -528
_LVOUIK_GetUserTag equ -534
_LVOUIK_FindTagItem equ -540
_LVOUIK_AllocateTagItems equ -546
_LVOUIK_CloneTagItems equ -552
_LVOUIK_TagInArray equ -558
_LVOUIK_PackBoolTags equ -564
_LVOUIK_GetTagData equ -570
_LVOUIK_ApplyTagItems equ -576
_LVOUIK_FilterTagItems equ -582
_LVOUIK_HowManyTagItems equ -588
_LVOUIK_ApplyFindTag equ -594
_LVOUIK_FilterTagChanges equ -600
_LVOUIK_MapTags equ -606
_LVOUIK_RefreshTagItemClones equ -612
_LVOUIK_CopyTagItems equ -618
_LVOUIK_FreeTagItems equ -624
_LVOUIK_JoinTagItems equ -630
_LVOUIK_AskIconPlace equ -636
_LVOUIK_MemTAlloc equ -642
_LVOUIK_MemSpAlloc equ -648
_LVOUIK_MemSpResize equ -654
_LVOUIK_MemTFree equ -660
_LVOUIK_MemSpFree equ -666
_LVOUIK_MemCopy equ -672
_LVOUIK_MemSpLength equ -678
_LVOUIK_ClipObject equ -684
_LVOUIK_UnclipObject equ -690
_LVOUIK_OME_ActiveMExObj equ -696
_LVOUIK_CharWidth equ -702
_LVOUIK_OSt_Get equ -708
_LVOUIK_OSt_Set equ -714
_LVOUIK_OSt_Cut equ -720
_LVOUIK_OSt_Paste equ -726
_LVOUIK_ActivateKeyTank equ -732
_LVOUIK_RegisterObject equ -738
_LVOUIK_UnregisterObject equ -744
_LVOUIK_IsActiveKeyTank equ -750
_LVOUIK_InactivateKeyTank equ -756
_LVOUIK_ToUpper equ -762
_LVOUIK_ToLower equ -768
_LVOUIK_StrToUpper equ -774
_LVOUIK_StrToLower equ -780
_LVOUIK_Match equ -792
_LVOUIK_StdGadStart equ -798
_LVOUIK_StdGadStop equ -804
_LVOUIK_StdGadResize equ -810
_LVOUIK_StdGadRefresh equ -816
_LVOUIK_StdGadAct equ -822
_LVOUIK_StdGadInact equ -828
_LVOUIK_StdGadMove equ -834
_LVOUIK_AddPrivateObject equ -840
_LVOUIK_OTo_SelectToggle equ -846
_LVOUIK_OTo_UnselectToggle equ -852
_LVOUIK_Do2 equ -858
_LVOUIK_CallObjectFunc equ -864
_LVOUIK_SetTagData equ -870
_LVOUIK_MakeUIKTree equ -876
_LVOUIK_DrawBox equ -882
_LVOUIK_RefreshSons equ -888
_LVOUIK_StrWidth equ -894
_LVOUIK_FreeBMapRPort equ -900
_LVOUIK_FreeRPort equ -906
_LVOUIK_FreeBMap equ -912
_LVOUIK_AllocBMapRPort equ -918
_LVOUIK_AllocRPort equ -924
_LVOUIK_AllocBMap equ -930
_LVOUIK_Select equ -936
_LVOUIK_Iconify equ -942
_LVOUIK_Uniconify equ -948
_LVOUIK_Input_Set equ -954
_LVOUIK_Input_Get equ -960
_LVOUIK_AddZone equ -966
_LVOBufGetUDatas equ -972
_LVOBufLength equ -978
_LVOBufSetUDatas equ -984
_LVOBufFree equ -990
_LVOBufAlloc equ -996
_LVOBufResize equ -1002
_LVOBufClone equ -1008
_LVOBufCopy equ -1014
_LVOBufCut equ -1020
_LVOBufPaste equ -1026
_LVOUIK_Input_JamFrame equ -1032
_LVOUIK_Input_ReleaseFrame equ -1038
_LVOUIK_Input_SetTime equ -1044
_LVOUIK_Input_Flush equ -1050
_LVOUIK_Input_AddJoyEvent equ -1056
_LVOUIK_Font2TTA equ -1062
_LVOUIK_OpenFontTTA equ -1068
_LVOUIK_DrawAuto equ -1074
_LVOUIK_Input_CycleZoneList equ -1080
_LVOUIK_Input_ChangeActiveZoneList equ -1086
_LVOUIK_SaveEnvPrefs equ -1092
_LVOUIK_ReadArcPrefs equ -1098
_LVOUIK_OLi_NewTextPtr equ -1104
_LVOBufSet equ -1110
_LVOUIK_ChangeAutomate equ -1116
_LVOBufFreeS equ -1122
_LVOBufResizeS equ -1128
_LVOBufCloneS equ -1134
_LVOBufCopyS equ -1140
_LVOBufCutS equ -1146
_LVOBufPasteS equ -1152
_LVOBufSetS equ -1158
_LVOUIK_GetColor equ -1164
_LVOUIK_SetColor equ -1170
_LVOUIK_ProcessPendingEvents equ -1176
_LVOUIK_UniconifyWindow equ -1182
_LVOUIK_WaitFirstEvents equ -1188
_LVOUIK_StripWindowEvents equ -1194
_LVOBufTruncate equ -1200
_LVOUIK_OLi_DeleteLine equ -1206
_LVOBufAllocP equ -1212
_LVOUIK_nl_NewListP equ -1218
_LVOUIK_nl_AllocListP equ -1224
_LVOUIK_GetTaskUIK equ -1230
_LVOUIK_GetObject equ -1236
_LVOVBufPrintf equ -1242
_LVOUIK_OME_InactiveMExObj equ -1248
_LVOUIK_ShortCutRemove equ -1254
_LVOUIK_ObjDisable equ -1260
_LVOUIK_ObjEnable equ -1266
_LVOUIK_GetRastPort equ -1272
_LVOUIK_CmplPattern equ -1278
