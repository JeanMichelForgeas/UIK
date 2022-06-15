/****************************************************************
 *
 *      Project:   UIKPrefs
 *      Function:  Objet modes de selection
 *
 *      Creation:  06/05/92    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikbase.h"
#include "uiki:uikglobal.h"
#include "uiki:uikobj.h"
#include "uiki:uikbox.h"
#include "uiki:uikmacros.h"
#include "uiki:uikcolors.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"
#include "uiki:objwindow.h"
#include "uiki:objrequest.h"
#include "uiki:objstring.h"

#include "lci:vars.h"
#include "lci:objreqmode.h"


/****** Macros **************************************************/


/****** Imported ************************************************/

extern VARS __near V; /* magic workaround */

extern struct GfxBase * __far GfxBase;
extern struct DOSBase * __far DOSBase;
extern struct UIKBase * __far UIKBase;

extern UBYTE * __far STitle;


/****** Exported ************************************************/


/****** Statics *************************************************/

static struct TTextAttr __far FrameTTA[] = { 0, 0, FSF_ITALIC, 0, 0 };
static struct TTextAttr __far NormTTA[] = { 0, 0, 0, 0, 0 };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static void DoOK( struct UIKObj *obj, struct UIKObjReqMode *orm )
{
    UIK_SetPrefs( &V.Prefs, sizeof(struct UIKPrefs) );
    UIK_Stop( orm );
}

static void DoCancel( struct UIKObj *wo, struct UIKObjReqMode *orm )
{
    V.Prefs.InputMode = orm->MPrefs.InputMode;
    V.Prefs.InputHeld = orm->MPrefs.InputHeld;
    V.Prefs.InputAuto = orm->MPrefs.InputAuto;
    V.Prefs.ZoneSpeed = orm->MPrefs.ZoneSpeed;
    V.Prefs.ValidTime = orm->MPrefs.ValidTime;
    V.Prefs.WaitTime  = orm->MPrefs.WaitTime;
    V.Prefs.ZoneThick = UIKBase->UIKPrefs->ZoneThick = orm->MPrefs.ZoneThick;
    UIK_SetPrefs( &V.Prefs, sizeof(struct UIKPrefs) );

    UIK_Input_Set( wo->UIK, V.Prefs.InputMode, V.Prefs.InputHeld, V.Prefs.InputAuto );
    UIK_Input_SetTime( wo->UIK, V.Prefs.ZoneSpeed, V.Prefs.ValidTime, V.Prefs.WaitTime );
    UIK_Stop( orm );
}

static void set_new_mode( struct UIKObj *obj, ULONG i )
{
    UIK_Input_Set( obj->UIK, i, (UBYTE)-1, (UBYTE)-1 );
    V.Prefs.InputMode = i;
}

static void set_held( struct UIKObj *obj, struct UIKObjReqMode *orm )
{
  UBYTE i = obj->Flags & BOX_OVER ? 0 : 1;

    UIK_Input_Set( obj->UIK, (UBYTE)-1, i, (UBYTE)-1 );
    V.Prefs.InputHeld = i;
}

static void set_auto( struct UIKObj *obj, struct UIKObjReqMode *orm )
{
  UBYTE i = obj->Flags & BOX_OVER ? 0 : 1;

    UIK_Input_Set( obj->UIK, (UBYTE)-1, (UBYTE)-1, i );
    V.Prefs.InputAuto = i;
}

static void set_times( struct UIKObj *obj, struct UIKObjReqMode *orm )
{
  register struct IntuiMessage *imsg = obj->UIK->IntuiMsg;

    if (imsg->Code == 0x43 || imsg->Code == 0x44)
        {
        UBYTE buf[20];
        UWORD val1 = orm->Str1Res, val2 = orm->Str2Res, val3 = orm->Str3Res;

        if (val1 > 1000) val1 = 1000;
        sprintf( buf, "%ld", val1 );
        UIK_OSt_Set( (struct UIKObjStrGad *) orm->Str1, buf, StrLen( buf ) );

        if (val2 > 1000) val2 = 1000;
        sprintf( buf, "%ld", val2 );
        UIK_OSt_Set( (struct UIKObjStrGad *) orm->Str2, buf, StrLen( buf ) );

        if (val3 > 1000) val3 = 1000;
        sprintf( buf, "%ld", val3 );
        UIK_OSt_Set( (struct UIKObjStrGad *) orm->Str3, buf, StrLen( buf ) );

        UIK_Input_SetTime( obj->UIK, val1, val2, val3 );
        V.Prefs.ZoneSpeed = val1;
        V.Prefs.ValidTime = val2;
        V.Prefs.WaitTime  = val3;
        }
}

static void __asm prefs_zone_thick( register __a1 struct UIKGlobal *uik, register __d1  ULONG thick,
register __a2 struct UIKBase *base, register __a3 struct UIKPrefs *prefs )
{
  UBYTE mode = uik->InputMode;

    UIK_Input_Set( uik, 0, (UBYTE)-1, (UBYTE)-1 );     // MOUSE -> efface le cadre
    V.Prefs.ZoneThick = prefs->ZoneThick = thick;
    UIK_Input_Set( uik, mode, (UBYTE)-1, (UBYTE)-1 );
}

static void set_zone_thick( struct UIKObj *obj, struct UIKObjReqMode *orm )
{
  ULONG thick = orm->Str4Res;
  register struct IntuiMessage *imsg = obj->UIK->IntuiMsg;
  UBYTE buf[20];

    if (imsg->Code == 0x43 || imsg->Code == 0x44)
        {
        if (thick < 1) thick = 1;
        if (thick > 20) thick = 20;
        sprintf( buf, "%ld", thick );
        UIK_OSt_Set( (struct UIKObjStrGad *) obj, buf, StrLen( buf ) );
        UIK_AccessPrefs( (void *) prefs_zone_thick, (ULONG)obj->UIK, thick );
        }
}

/****************************************************************
 *
 *      Object
 *
 ****************************************************************/

static int StartReqMode( struct UIKObjReqMode *orm )
{
    return(1);
}

static void StopReqMode( struct UIKObjReqMode *orm )
{
}

static void RemoveReqMode( struct UIKObjReqMode *orm )
{
}

static int AddReqMode( struct UIKObjReqMode *orm, struct TagItem *taglist )
{
  ULONG title_num;
  struct UIKObj *obj, *frame;
  UBYTE buf[50];

    if (! (title_num = GetTagData( UIKTAG_ObjReqMode_FirstTitleNum, 0, taglist ))) goto END_ERROR;

    CopyMem( &V.Prefs, &orm->MPrefs, sizeof(struct UIKPrefs) );

    //----------------- Window
    if (! (orm->WO = (struct UIKObjWindow *) UIK_AddObjectTags( "UIKObj_Window", orm,
                UIKTAG_OBJ_LeftTop,        SETL(40,20),
                UIKTAG_OBJ_WidthHeight,    SETL(391,142),
                UIKTAG_OBJ_Title,          (ULONG) UIK_LangString( orm, title_num ),
                UIKTAG_OBJ_AltTitle,       (ULONG) STitle,
                UIKTAG_OBJ_TitleFl_Addr,   TRUE,
                UIKTAG_OBJWindowFl_With_Size, FALSE,
                TAG_END ))) goto END_ERROR;

    if (! UIK_AddVector( orm->WO, (void *) DoCancel, CLOSEWINDOW, (ULONG) orm )) goto END_ERROR;

    //----------------- Entrées
    { struct TagItem tl_modgad[] = {
        { UIKTAG_OBJ_LeftTop },
        { UIKTAG_OBJ_WidthHeight },
        { UIKTAG_OBJ_BoxFl_Over },
        { UIKTAG_OBJ_Title },
        { UIKTAG_OBJ_TitleFl_Right, TRUE },
        { UIKTAG_OBJ_BoxFl_Filled,  TRUE },
        { UIKTAG_OBJ_ActInactPens,  UIKCOLS(UIKCOL_LIGHT,UIKCOL_GREY,UIKCOL_BLACK,UIKCOL_BLACK) },
        { UIKTAG_OBJ_BoxFl_Zoneable },
        { UIKTAG_OBJ_ShortCutKey,   0 },
        { TAG_END } };
    ULONG i;
    UBYTE keys[] = { 'S','B','J','1','2','5' };

    for (i=0; i<6; i++)
        {
        tl_modgad[0].ti_Data = SETL(26,9+13*i);
        tl_modgad[1].ti_Data = SETL(20,11);
        tl_modgad[2].ti_Data = V.Prefs.InputMode == i ? FALSE : TRUE;
        tl_modgad[3].ti_Data = title_num + 1 + i;
        tl_modgad[7].ti_Data = i < 3 ? FALSE : TRUE;
        tl_modgad[8].ti_Data = B2L(keys[i],0,0,0);
        if (! (obj = UIK_AddObject( "UIKObj_RadioExclude", orm->WO, tl_modgad ))) goto END_ERROR;

        if (! UIK_AddVector( obj, (void*) set_new_mode, GADGETDOWN, i )) goto END_ERROR;
        }
    }

    //----------------- Auto
    if (! (obj = UIK_AddObjectTags( "UIKObj_CheckToggle", orm->WO,
                UIKTAG_OBJ_LeftTop,     SETL(23,95),      // SETL(16,91)
                UIKTAG_OBJ_WidthHeight, SETL(25,14),     // SETL(131,17)
                UIKTAG_OBJ_TitleFl_Right,   TRUE,
                UIKTAG_OBJ_ActInactPens,    UIKCOLS(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_BLACK,UIKCOL_BLACK),
                UIKTAG_OBJ_BoxFl_NoPush,  TRUE,
                UIKTAG_OBJ_BoxFl_Over,  V.Prefs.InputAuto ? FALSE : TRUE,
                UIKTAG_OBJ_Title,       title_num+7,
                UIKTAG_OBJ_BoxFl_Filled,TRUE,
                UIKTAG_OBJ_ShortCutKey, B2L('a',0,0,0),
                TAG_END ))) goto END_ERROR;

    if (! UIK_AddVector( obj, (void*) set_auto, GADGETUP, (ULONG) orm )) goto END_ERROR;

    //----------------- Maintient
    if (! (obj = UIK_AddObjectTags( "UIKObj_CheckToggle", orm->WO,
                UIKTAG_OBJ_LeftTop,         SETL(23,114),  // SETL(16,112)
                UIKTAG_OBJ_WidthHeight,     SETL(25,14),  // SETL(131,17)
                UIKTAG_OBJ_TitleFl_Right,   TRUE,
                UIKTAG_OBJ_BoxFl_NoPush,  TRUE,
                UIKTAG_OBJ_ActInactPens,    UIKCOLS(UIKCOL_WHITE,UIKCOL_GREY,UIKCOL_BLACK,UIKCOL_BLACK),
                UIKTAG_OBJ_BoxFl_Over,      V.Prefs.InputHeld ? FALSE : TRUE,
                UIKTAG_OBJ_Title,           title_num+8,
                UIKTAG_OBJ_BoxFl_Filled,    TRUE,
                UIKTAG_OBJ_ShortCutKey,     B2L('m',0,0,0),
                TAG_END ))) goto END_ERROR;

    if (! UIK_AddVector( obj, (void*) set_held, GADGETUP, (ULONG) orm )) goto END_ERROR;

    //----------------- Vitesses
    if (! (frame = UIK_AddObjectTags( "UIKObj_Frame", orm->WO,
                UIKTAG_OBJ_LeftTop,         SETL(185,9),
                UIKTAG_OBJ_WidthHeight,     SETL(190,76),
                UIKTAG_OBJ_Title,           title_num+9,
                UIKTAG_OBJ_TTextAttr,       (ULONG)&FrameTTA,
                UIKTAG_OBJ_BoxFl_Zoneable,  TRUE,
                TAG_END ))) goto END_ERROR;

    sprintf( buf, "%ld", V.Prefs.ZoneSpeed );
    if (! (orm->Str1 = UIK_AddObjectTags( "UIKObj_String", frame,
                UIKTAG_OBJ_LeftTop,         SETL(114,8),
                UIKTAG_OBJ_WidthHeight,     SETL(43,0),
                UIKTAG_OBJ_Title,           title_num+10,
                UIKTAG_OBJ_TitleFl_Left,    TRUE,
                UIKTAG_OBJ_ParentRelative,  TRUE,
                UIKTAG_OBJStrFl_DigitalInt, TRUE,
                UIKTAG_OBJStr_TextPointer,  (ULONG) buf,
                UIKTAG_OBJStr_ResultPointer,(ULONG) &orm->Str1Res,
                UIKTAG_OBJ_ShortCutKey,     B2L('d',0,0,0),
                UIKTAG_OBJ_TTextAttr,       (ULONG)&NormTTA,
                TAG_END ))) goto END_ERROR;
    if (! UIK_AddVector( orm->Str1, (void*) set_times, RAWKEY, (ULONG) orm )) goto END_ERROR;

    sprintf( buf, "%ld", V.Prefs.ValidTime );
    if (! (orm->Str2 = UIK_AddObjectTags( "UIKObj_String", frame,
                UIKTAG_OBJ_LeftTop,         SETL(114,29),
                UIKTAG_OBJ_WidthHeight,     SETL(43,0),
                UIKTAG_OBJ_Title,           title_num+11,
                UIKTAG_OBJ_TitleFl_Left,    TRUE,
                UIKTAG_OBJ_ParentRelative,  TRUE,
                UIKTAG_OBJStrFl_DigitalInt, TRUE,
                UIKTAG_OBJStr_TextPointer,  (ULONG) buf,
                UIKTAG_OBJStr_ResultPointer, (ULONG) &orm->Str2Res,
                UIKTAG_OBJ_ShortCutKey,     B2L('v',0,0,0),
                UIKTAG_OBJ_TTextAttr,       (ULONG)&NormTTA,
                TAG_END ))) goto END_ERROR;
    if (! UIK_AddVector( orm->Str2, (void*) set_times, RAWKEY, (ULONG) orm )) goto END_ERROR;

    sprintf( buf, "%ld", V.Prefs.WaitTime );
    if (! (orm->Str3 = UIK_AddObjectTags( "UIKObj_String", frame,
                UIKTAG_OBJ_LeftTop,         SETL(114,50),
                UIKTAG_OBJ_WidthHeight,     SETL(43,0),
                UIKTAG_OBJ_Title,           title_num+12,
                UIKTAG_OBJ_TitleFl_Left,    TRUE,
                UIKTAG_OBJ_ParentRelative,  TRUE,
                UIKTAG_OBJStrFl_DigitalInt, TRUE,
                UIKTAG_OBJStr_TextPointer,  (ULONG) buf,
                UIKTAG_OBJStr_ResultPointer, (ULONG) &orm->Str3Res,
                UIKTAG_OBJ_ShortCutKey,     B2L('r',0,0,0),
                UIKTAG_OBJ_TTextAttr,       (ULONG)&NormTTA,
                TAG_END ))) goto END_ERROR;
    if (! UIK_AddVector( orm->Str3, (void*) set_times, RAWKEY, (ULONG) orm )) goto END_ERROR;

    //----------------- Epaisseur du cadre
    sprintf( buf, "%ld", V.Prefs.ZoneThick );
    if (! (obj = UIK_AddObjectTags( "UIKObj_String", orm->WO,
                UIKTAG_OBJ_LeftTop,         SETL(338,90),
                UIKTAG_OBJ_WidthHeight,     SETL(37,0),
                UIKTAG_OBJ_Title,           title_num+13,
                UIKTAG_OBJ_TitleFl_Left,    TRUE,
                UIKTAG_OBJStrFl_DigitalInt, TRUE,
                UIKTAG_OBJStr_TextPointer,  (ULONG) buf,
                UIKTAG_OBJStr_ResultPointer,(ULONG) &orm->Str4Res,
                UIKTAG_OBJ_ShortCutKey,     B2L('e',0,0,0),
                TAG_END ))) goto END_ERROR;
    if (! UIK_AddVector( obj, (void*) set_zone_thick, RAWKEY, (ULONG) orm )) goto END_ERROR;

    //----------------- OK
    if (! (obj = UIK_AddObjectTags ( "UIKObj_OK", orm->WO,
                UIKTAG_OBJ_LeftTop,         SETL(290,118),
                UIKTAG_OBJ_WidthHeight,     SETL(69,17),
                //UIKTAG_OBJ_ShortCutKey, B2L(0x0d,0,0,0),
                TAG_END ))) goto END_ERROR;
    if (! UIK_AddVector( obj, (void *)DoOK, GADGETUP, (ULONG) orm )) goto END_ERROR;

    return(1);

  END_ERROR:
    return(0);
}

static struct UIKObjGerm __far Germ = {
    "",
    AddReqMode,         /*int     (*AddVector)(); */
    StartReqMode,       /*int     (*StartVector)(); */
    StopReqMode,        /*void    (*StopVector)(); */
    RemoveReqMode,      /*void    (*RemoveVector)(); */
    NULL,               /*void    (*RefreshVector)(); */
    NULL,               /*void    (*ResizeVector)(); */
    NULL,               /*void    (*ExceptActVector)(); */
    NULL,               /*void    (*ExceptInaVector)(); */
    NULL,               /*void    (*ExceptMovVector)(); */
    NULL,               /*void    (*ExceptKeyVector)(); */
    NULL,               /*int     (*StartAfterVector)(); */
    NULL,               /*int     (*SelectVector)(); */
    NULL,               /*ULONG   *ExtentedVectors; */
    NULL,               /*struct TagItem *GermTagList; */
    0,                  /*UWORD   MinWidth; */
    0,                  /*UWORD   MinHeight; */
    0,                  /*ULONG   DefaultWidthHeight; */
    0,                  /*UWORD   ObjId; */
    sizeof(struct UIKObjReqMode), /*UWORD   ObjSize; */
    0,
    };

struct UIKObjRegGerm __far ObjReqModeRegGerm = { &Germ, 0, 0 ,0 };
