/****************************************************************
 *
 *      Project:   UIK
 *      Function:  gestion des modes d'entr�e
 *
 *      Created:   16/04/92    Jean-Michel Forgeas
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikbase.h"
#include "uiki:uikglobal.h"
#include "lci:uik_protosint.h"


/****** Macros **************************************************/


/****** Imported ************************************************/

extern void __asm circbuf_put( register __a2 struct UIKGlobal *uik, register __d0 UBYTE b, register __d1 UBYTE c );

extern ULONG __asm zo_ChangeActiveZoneList( register __a0 struct UIKGlobal *uik, register __a1 struct UIKZoneList *uzl );
extern void __asm zo_InstallZoneList( register __a0 struct UIKGlobal *uik, register __a1 struct UIKZoneList *uzl );
extern UZone * __asm zo_UnmarkZone( register __a0 struct UIKGlobal *uik );
extern void __asm zo_MarkZone( register __a0 struct UIKGlobal *uik, register __a1 UZone *zo );
extern UZone * __asm zo_FindNextZone( register __a0 struct UIKGlobal *uik );
extern void __asm zo_MarkNextZone( register __a0 struct UIKGlobal *uik );
extern void __asm zo_MarkNextZoneDir( register __a0 struct UIKGlobal *uik, register __d0 UBYTE buts );
extern void __asm zo_MarkFirstZone( register __a0 struct UIKGlobal *uik );
extern struct UIKObj * __asm zo_FindActiveObject( register __a0 struct UIKGlobal *uik );

extern struct UIKBase * __far UIKBase;
extern struct DOSBase * __far DOSBase;
extern struct GfxBase * __far GfxBase;
extern struct IntuitionBase * __far IntuitionBase;


/****** Exported ************************************************/

void IPCode_1Contact( struct UIKGlobal *uik, UBYTE buts, UBYTE changes );
void IPCode_2Contacts( struct UIKGlobal *uik, UBYTE buts, UBYTE changes );
void IPCode_5Contacts( struct UIKGlobal *uik, UBYTE buts, UBYTE changes );


/****** Statics *************************************************/


/****************************************************************
 *
 *      Interruption
 *
 *  JoyVal ne sert que dans la partie assembleur et prend en
 *  charge le mode maintient (InputHeld). Il range ses r�sultats
 *  dans JoyButs.
 *
 *  JoyButs ne sert que dans cette partie et prend en charge
 *  les temporisations. Elle range ses r�sultats *  dans la table
 *  par circbuf_put().
 *
 *  La partie tournant sur le process ne prendra en compte que
 *  les datas obtenues dans la table par circbuf_get().
 *
 *
 *      Les modes
 *
 *  2 contacts:
 *      - Auto:     [1] valide + arr�te le d�filement
 *                  [2] amorce le case � case
 *      - Manuel:   [1] valide
 *                  [2] amorce le case � case
 *          Si maintient -> similaire au mode Auto
 *
 *  5 contacts:
 *      - Auto1:
 *          Donner une impulsion amorce le case � case dans la
 *          direction indiqu�e, en restant dans sa ligne ou dans
 *          sa colone.
 *      - Auto2:
 *          Pareil, sauf qu'on change de ligne ou de colone
 *          quand on arrive au bout (de gauche � droite et de
 *          haut en bas).
 *      La diff�rence entre Auto1 et Auto2 se fait par l'option
 *      maintient: l'option maintient n'a de sens que dans les
 *      modes manuels.
 *
 ****************************************************************/

void __asm VBlankSend( register __a0 struct UIKGlobal *uik, register __a6 struct UIKBase *ub )
{
  struct UIKPrefs *prefs = ub->UIKPrefs;
  UBYTE signal=0, changes=0;

    //-------------------------------------------------------------------
    if (uik->TempoWaitFire)  --uik->TempoWaitFire;

    if (uik->JoyButs & VBJOYF_FIRE)
        {
        if (! (uik->OldJoyButs & VBJOYF_FIRE))
            {                               // vient d'�tre appuy�
            if (uik->TempoWaitFire == 0)            // a-t-on le rel�chement minimum ?
                uik->TempoValidFire = prefs->ValidTime;  // oui: on se pr�pare � la validation
            }
        else if (uik->TempoValidFire)
            {
            if (--uik->TempoValidFire == 0)         // a-t-on l'appui minimum ?
                {
                uik->ProcButs |= VBJOYF_FIRE;       // oui: validation appui !
                changes |= VBJOYF_FIRE;
                signal = 1;
                uik->TempoSpeed = prefs->ZoneSpeed;
                }
            }
        }
    else{
        if (uik->OldJoyButs & VBJOYF_FIRE)
            {                               // vient d'�tre rel�ch�
            if (uik->TempoValidFire == 0)           // a-t-on l'appui minimum ?
                {
                uik->ProcButs &= ~VBJOYF_FIRE;      // oui: validation rel�chement
                changes |= VBJOYF_FIRE;
                signal = 1;
                }
            uik->TempoSpeed = prefs->ZoneSpeed; // on recompte apr�s le rel�chement
            uik->TempoWaitFire = prefs->WaitTime;
            }
        }

    //-------------------------------------------------------------------
    if (uik->TempoWaitRight)  --uik->TempoWaitRight;

    if (uik->JoyButs & VBJOYF_RIGHT)
        {
        if (! (uik->OldJoyButs & VBJOYF_RIGHT))
            {                               // vient d'�tre appuy�
            if (uik->TempoWaitRight == 0)            // a-t-on le rel�chement minimum ?
                uik->TempoValidRight = prefs->ValidTime;  // oui: on se pr�pare � la validation
            }
        else if (uik->TempoValidRight)
            {
            if (--uik->TempoValidRight == 0)         // a-t-on l'appui minimum ?
                {
                uik->ProcButs |= VBJOYF_RIGHT;       // oui: validation appui !
                changes |= VBJOYF_RIGHT;
                signal = 1;
                uik->TempoSpeed = prefs->ZoneSpeed;
                }
            }
        }
    else{
        if (uik->OldJoyButs & VBJOYF_RIGHT)
            {                               // vient d'�tre rel�ch�
            if (uik->TempoValidRight == 0)           // a-t-on l'appui minimum ?
                {
                uik->ProcButs &= ~VBJOYF_RIGHT;      // oui: validation rel�chement
                changes |= VBJOYF_RIGHT;
                signal = 1;
                }
            uik->TempoSpeed = prefs->ZoneSpeed; // on recompte apr�s le rel�chement
            uik->TempoWaitRight = prefs->WaitTime;
            }
        }

    //-------------------------------------------------------------------
    if (uik->TempoWaitLeft)  --uik->TempoWaitLeft;

    if (uik->JoyButs & VBJOYF_LEFT)
        {
        if (! (uik->OldJoyButs & VBJOYF_LEFT))
            {                               // vient d'�tre appuy�
            if (uik->TempoWaitLeft == 0)            // a-t-on le rel�chement minimum ?
                uik->TempoValidLeft = prefs->ValidTime;  // oui: on se pr�pare � la validation
            }
        else if (uik->TempoValidLeft)
            {
            if (--uik->TempoValidLeft == 0)         // a-t-on l'appui minimum ?
                {
                uik->ProcButs |= VBJOYF_LEFT;       // oui: validation appui !
                changes |= VBJOYF_LEFT;
                signal = 1;
                uik->TempoSpeed = prefs->ZoneSpeed;
                }
            }
        }
    else{
        if (uik->OldJoyButs & VBJOYF_LEFT)
            {                               // vient d'�tre rel�ch�
            if (uik->TempoValidLeft == 0)           // a-t-on l'appui minimum ?
                {
                uik->ProcButs &= ~VBJOYF_LEFT;      // oui: validation rel�chement
                changes |= VBJOYF_LEFT;
                signal = 1;
                }
            uik->TempoSpeed = prefs->ZoneSpeed; // on recompte apr�s le rel�chement
            uik->TempoWaitLeft = prefs->WaitTime;
            }
        }

    //-------------------------------------------------------------------
    if (uik->TempoWaitBack)  --uik->TempoWaitBack;

    if (uik->JoyButs & VBJOYF_BACK)
        {
        if (! (uik->OldJoyButs & VBJOYF_BACK))
            {                               // vient d'�tre appuy�
            if (uik->TempoWaitBack == 0)            // a-t-on le rel�chement minimum ?
                uik->TempoValidBack = prefs->ValidTime;  // oui: on se pr�pare � la validation
            }
        else if (uik->TempoValidBack)
            {
            if (--uik->TempoValidBack == 0)         // a-t-on l'appui minimum ?
                {
                uik->ProcButs |= VBJOYF_BACK;       // oui: validation appui !
                changes |= VBJOYF_BACK;
                signal = 1;
                uik->TempoSpeed = prefs->ZoneSpeed;
                }
            }
        }
    else{
        if (uik->OldJoyButs & VBJOYF_BACK)
            {                               // vient d'�tre rel�ch�
            if (uik->TempoValidBack == 0)           // a-t-on l'appui minimum ?
                {
                uik->ProcButs &= ~VBJOYF_BACK;      // oui: validation rel�chement
                changes |= VBJOYF_BACK;
                signal = 1;
                }
            uik->TempoSpeed = prefs->ZoneSpeed; // on recompte apr�s le rel�chement
            uik->TempoWaitBack = prefs->WaitTime;
            }
        }

    //-------------------------------------------------------------------
    if (uik->TempoWaitForw)  --uik->TempoWaitForw;

    if (uik->JoyButs & VBJOYF_FORW)
        {
        if (! (uik->OldJoyButs & VBJOYF_FORW))
            {                               // vient d'�tre appuy�
            if (uik->TempoWaitForw == 0)            // a-t-on le rel�chement minimum ?
                uik->TempoValidForw = prefs->ValidTime;  // oui: on se pr�pare � la validation
            }
        else if (uik->TempoValidForw)
            {
            if (--uik->TempoValidForw == 0)         // a-t-on l'appui minimum ?
                {
                uik->ProcButs |= VBJOYF_FORW;       // oui: validation appui !
                changes |= VBJOYF_FORW;
                signal = 1;
                uik->TempoSpeed = prefs->ZoneSpeed;
                }
            }
        }
    else{
        if (uik->OldJoyButs & VBJOYF_FORW)
            {                               // vient d'�tre rel�ch�
            if (uik->TempoValidForw == 0)           // a-t-on l'appui minimum ?
                {
                uik->ProcButs &= ~VBJOYF_FORW;      // oui: validation rel�chement
                changes |= VBJOYF_FORW;
                signal = 1;
                }
            uik->TempoSpeed = prefs->ZoneSpeed; // on recompte apr�s le rel�chement
            uik->TempoWaitForw = prefs->WaitTime;
            }
        }

    //-------------------------------------------------------------------
    //if (uik->InputAuto || (uik->ProcButs || changes))
    if (uik->TempoSpeed)
        if (--uik->TempoSpeed <= 0)
            {
            signal = 1;
            uik->TempoSpeed = prefs->ZoneSpeed;
            }

    if (signal)
        {
        circbuf_put( uik, uik->ProcButs, changes );
        Signal( uik->Task, uik->ActiveMask );
        }
}

/****************************************************************
 *
 *      Code commun
 *
 ****************************************************************/

static void Up_ListBase( struct UIKGlobal *uik, struct UIKObj *obj )
{
  ULONG class = GADGETUP; //(obj->Id == UIKID_CLOSGAD) ? CLOSEWINDOW : GADGETUP;

    if (! (uik->InputFlags & INPUTF_JAMED))
        {
        UZone *zo = zo_UnmarkZone( uik );

        UIK_Select( obj, class );  // le flag peut �tre mis pendant UIK_Select()
        if (uik->InputFlags & INPUTF_JAMED) zo_MarkZone( uik, zo );

        zo_ChangeActiveZoneList( uik, uik->ZoneListBase );
        }
    else UIK_Select( obj, class );
}

/****************************************************************
 *
 *      Code pour Mouse
 *
 ****************************************************************/

void IPCode_Mouse( struct UIKGlobal *uik, UBYTE buts, UBYTE changes )
{
    //kprintf("Mouse: %lx %lx\n", buts, changes );
}

/****************************************************************
 *
 *      Code pour Bouris
 *
 ****************************************************************/

void IPCode_Bouris( struct UIKGlobal *uik, UBYTE buts, UBYTE changes )
{
    //kprintf("Bouris: %lx %lx\n", buts, changes );
}

/****************************************************************
 *
 *      Code pour JoyMouse
 *
 ****************************************************************/

void IPCode_JoyMouse( struct UIKGlobal *uik, UBYTE buts, UBYTE changes )
{
    //kprintf("JoyMouse: %lx %lx\n", buts, changes );
}

/****************************************************************
 *
 *      Code pour 1 contact
 *
 ****************************************************************/

static void IPCode_1C_DoNoChange( struct UIKGlobal *uik, UBYTE buts, UBYTE changes )
{
    //---- si appuy�, rien; si relev�, zone suivante
    if (! (buts & CLIC_BUTTON))
        zo_MarkNextZone( uik );
}

void IPCode_1C_WaitClickUp( struct UIKGlobal *uik, UBYTE buts, UBYTE changes )
{
    if (! (changes & CLIC_BUTTON))
        {
        IPCode_1C_DoNoChange( uik, buts, changes );
        UIK_Select( zo_FindActiveObject( uik ), INTUITICKS|MOUSEMOVE );
        }
    else{
        struct UIKObj *obj;

        if (obj = zo_FindActiveObject( uik ))
            {
            if (! (buts & CLIC_BUTTON))
                {
                if (obj->ObjExt->ZoneList)
                    {
                    zo_ChangeActiveZoneList( uik, obj->ObjExt->ZoneList );
                    }
                else{
                    Up_ListBase( uik, obj );
                    }

                uik->InputVector = IPCode_1Contact;
                }
            }
        }
}

void IPCode_1Contact( struct UIKGlobal *uik, UBYTE buts, UBYTE changes )
{

    if (! (changes & CLIC_BUTTON))
        {
        IPCode_1C_DoNoChange( uik, buts, changes );
        }
    else{
        struct UIKObj *obj;

        if (obj = zo_FindActiveObject( uik ))
            {
            if (buts & CLIC_BUTTON)
                {
                if (! obj->ObjExt->ZoneList)
                    {
                    UZone *z = uik->ActiveZoneList->ActiveZone;  // oblig� d'�tre ok
                    UIK_Select( obj, GADGETDOWN );
                    if (! uik->ActiveZoneList) return;
                    if (z != uik->ActiveZoneList->ActiveZone) return;
                    }
                uik->InputVector = IPCode_1C_WaitClickUp;
                }
            }
        }
}

/****************************************************************
 *
 *      Code pour 2 contacts
 *
 ****************************************************************/

static void IPCode_2C_DoNoChange( struct UIKGlobal *uik, UBYTE buts, UBYTE changes )
{
    if (uik->InputAuto)
        {
        //---- appuy� ou relev�, zone suivante
        if (uik->InputFlags & INPUTF_KZAKZ_ON) zo_MarkNextZone( uik );
        }
    else{
        //---- si appuy�, zone suivante; si relev�, rien
        if (buts & MOVE_BUTTON)
            {
            if (uik->InputFlags & INPUTF_KZAKZ_ON) zo_MarkNextZone( uik );
            }
        }
}

static void IPCode_2C_WaitMoveUp( struct UIKGlobal *uik, UBYTE buts, UBYTE changes )
{
    if (! changes)
        {
        IPCode_2C_DoNoChange( uik, buts, changes );
        UIK_Select( zo_FindActiveObject( uik ), INTUITICKS|MOUSEMOVE );
        }
    else{
        if ((changes & MOVE_BUTTON) && !(buts & MOVE_BUTTON))
            {
            if (! uik->InputAuto)                       // manuel: arr�te le KZAKZ
                uik->InputFlags &= ~INPUTF_KZAKZ_ON;

            uik->InputVector = IPCode_2Contacts;
            }
        }
}

void IPCode_2C_WaitClickUp( struct UIKGlobal *uik, UBYTE buts, UBYTE changes )
{
  struct UIKObj *obj = zo_FindActiveObject( uik );

    if (! changes)
        {
        IPCode_2C_DoNoChange( uik, buts, changes );
        UIK_Select( obj, INTUITICKS|MOUSEMOVE );
        }
    else{
        if (obj)
            {
            if ((changes & CLIC_BUTTON) && !(buts & CLIC_BUTTON))
                {
                if (obj->ObjExt->ZoneList)
                    {
                    uik->InputFlags |= INPUTF_KZAKZ_ON;
                    zo_ChangeActiveZoneList( uik, obj->ObjExt->ZoneList );
                    }
                else{
                    uik->InputFlags &= ~INPUTF_KZAKZ_ON; // arr�te le d�filement
                    Up_ListBase( uik, obj );
                    }

                uik->InputVector = IPCode_2Contacts;
                }
            }
        }
}

void IPCode_2Contacts( struct UIKGlobal *uik, UBYTE buts, UBYTE changes )
{
    //----------------------------------------------------------------------
    if (! changes)
        {
        IPCode_2C_DoNoChange( uik, buts, changes );
        }
    else{
        //----------------------------------------------------------------------
        if ((changes & MOVE_BUTTON) && (buts & MOVE_BUTTON))
            {
            uik->InputFlags |= INPUTF_KZAKZ_ON;
            if (! uik->InputAuto)                       // manuel: zone suivante + amorce KZAKZ
                zo_MarkNextZone( uik );

            uik->InputVector = IPCode_2C_WaitMoveUp;
            }

        //----------------------------------------------------------------------
        if (changes & CLIC_BUTTON)
            {
            struct UIKObj *obj;

            if (obj = zo_FindActiveObject( uik ))
                {
                if (buts & CLIC_BUTTON)
                    {
                    uik->InputFlags &= ~INPUTF_KZAKZ_ON; // arr�te le d�filement
                    if (! obj->ObjExt->ZoneList)
                        {
                        UZone *z = uik->ActiveZoneList->ActiveZone;  // oblig� d'�tre ok
                        UIK_Select( obj, GADGETDOWN );
                        if (! uik->ActiveZoneList) return;
                        if (z != uik->ActiveZoneList->ActiveZone) return;
                        }
                    uik->InputVector = IPCode_2C_WaitClickUp;
                    }
                }
            }
        }
}

/****************************************************************
 *
 *      Code pour 5 contacts
 *
 ****************************************************************/

static void IPCode_5C_DoNoChange( struct UIKGlobal *uik, UBYTE buts, UBYTE changes )
{
    if (uik->InputAuto)
        {
        //---- appuy� ou relev�, zone suivante
        if (buts & DIR_BUTTONS) uik->DirButs = buts;
        if (uik->InputFlags & INPUTF_KZAKZ_ON) zo_MarkNextZoneDir( uik, uik->DirButs );
        }
    else{
        //---- si appuy�, zone suivante; si relev�, rien
        if (buts & DIR_BUTTONS)
            {
            if (uik->InputFlags & INPUTF_KZAKZ_ON) zo_MarkNextZoneDir( uik, buts );
            }
        }
}

static void IPCode_5C_WaitMoveUp( struct UIKGlobal *uik, UBYTE buts, UBYTE changes )
{
    if (! changes)
        {
        IPCode_5C_DoNoChange( uik, buts, changes );
        UIK_Select( zo_FindActiveObject( uik ), INTUITICKS|MOUSEMOVE );
        }
    else{
        if ((changes & DIR_BUTTONS) && !(buts & DIR_BUTTONS))
            {
            if (! uik->InputAuto)                       // manuel: arr�te le KZAKZ
                uik->InputFlags &= ~INPUTF_KZAKZ_ON;

            uik->InputVector = IPCode_5Contacts;
            }
        }
}

void IPCode_5C_WaitClickUp( struct UIKGlobal *uik, UBYTE buts, UBYTE changes )
{
  struct UIKObj *obj = zo_FindActiveObject( uik );

    if (! changes)
        {
        IPCode_5C_DoNoChange( uik, buts, changes );
        UIK_Select( obj, INTUITICKS|MOUSEMOVE );
        }
    else{
        if (obj)
            {
            if ((changes & CLIC_BUTTON) && !(buts & CLIC_BUTTON))
                {
                if (obj->ObjExt->ZoneList)
                    {
                    uik->InputFlags |= INPUTF_KZAKZ_ON;
                    zo_ChangeActiveZoneList( uik, obj->ObjExt->ZoneList );
                    }
                else{
                    uik->InputFlags &= ~INPUTF_KZAKZ_ON; // arr�te le d�filement
                    Up_ListBase( uik, obj );
                    }

                uik->InputVector = IPCode_5Contacts;
                }
            }
        }
}

void IPCode_5Contacts( struct UIKGlobal *uik, UBYTE buts, UBYTE changes )
{
    if (buts & DIR_BUTTONS) uik->DirButs = buts;

    if (! changes)
        {
        IPCode_5C_DoNoChange( uik, buts, changes );
        }
    else{
        //----------------------------------------------------------------------
        if ((changes & DIR_BUTTONS) && (buts & DIR_BUTTONS))
            {
            if (uik->InputAuto)                         // auto: amorce KZAKZ
                {
                uik->InputFlags |= INPUTF_KZAKZ_ON;
                }
            else{                                       // manuel: zone suivante + amorce KZAKZ
                uik->InputFlags |= INPUTF_KZAKZ_ON;
                zo_MarkNextZoneDir( uik, buts );
                }

            uik->InputVector = IPCode_5C_WaitMoveUp;
            }
        //----------------------------------------------------------------------
        if (changes & CLIC_BUTTON)
            {
            struct UIKObj *obj;

            if (obj = zo_FindActiveObject( uik ))
                {
                if (buts & CLIC_BUTTON)
                    {
                    uik->InputFlags &= ~INPUTF_KZAKZ_ON; // arr�te le d�filement
                    if (! obj->ObjExt->ZoneList)
                        {
                        UZone *z = uik->ActiveZoneList->ActiveZone;  // oblig� d'�tre ok
                        UIK_Select( obj, GADGETDOWN );
                        if (! uik->ActiveZoneList) return;
                        if (z != uik->ActiveZoneList->ActiveZone) return;
                        }
                    uik->InputVector = IPCode_5C_WaitClickUp;
                    }
                }
            }
        }
}
