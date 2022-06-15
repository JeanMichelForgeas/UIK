/****************************************************************
 *
 *      Project:   UIK
 *      Function:  uikutil
 *
 *      Created:   09/08/92    Jean-Michel Forgeas
 *
 ****************************************************************/


/********** Includes ********************************************/

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikglobal.h"
#include "lci:uik_protosint.h"

#include "pr:aps/includes/aps.h"
#include "pr:aps/includes/aps_protos.h"


/********** Imported ********************************************/

extern ULONG __far DOSBase;
extern UBYTE __far UIKPrefsFileName;


/********** Exported ********************************************/


/********** Statics *********************************************/


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

static ULONG VerifyDir( UBYTE *name )
{
  ULONG lock, ok=0;

    if (lock = Lock( name, ACCESS_READ ))
        {
        UnLock( lock );
        ok = 1;
        }
    else{
        if (lock = CreateDir( name ))
            {
            UnLock( lock );
            ok = 1;
            }
        }
    return( ok );
}

ULONG __asm doprefsfile( register __d1 ULONG arc, register __a1 ULONG mode, register __a3 struct UIKPrefs *prefs )  // returns length
{
  struct APSHandle *ah;
  UBYTE *envname, *arcname, *loadname;
  ULONG lock, size;
  struct UIKPrefs *newprefs;
  UBYTE *Name1, *Name2, *Name3;

    Name1 = "S:uik.prefs";
    Name2 = "ENV:UIK/uik.prefs";
    Name3 = "ENVARC:UIK/uik.prefs";

    envname = arcname = Name1;
    size = 0;

    if (ah = APS_RegisterAppl( "UIK", 0, 0, 0, 0, 0, 0 ))
        {
        if (VerifyDir( "ENV:UIK" )) envname = Name2;

        if (UIK_IsSystemVersion2())
            if (VerifyDir( "ENVARC:UIK" )) arcname = Name3;

        loadname = arc ? arcname : envname;
        while (! (lock = Lock( loadname, ACCESS_READ )))
            {
            if (loadname == Name2) loadname = Name1;
            else if (loadname == Name3) loadname = Name1;
            else break;
            }
        if (lock) UnLock( lock );

        newprefs = arc ?
            APS_OpenArcPrefs( ah, loadname, "uik.prefs", sizeof(struct UIKPrefs), 0, prefs, sizeof(struct UIKPrefs) )
          : APS_OpenPrefs( ah, loadname, "uik.prefs", sizeof(struct UIKPrefs), 0, prefs, sizeof(struct UIKPrefs) );

        if (newprefs)
            {
            if (mode == MODE_OLDFILE) /* ReadPrefs */
                {
                UIK_MemCopy( newprefs, prefs, sizeof(struct UIKPrefs) );
                size = sizeof(struct UIKPrefs);
                }
            else{                    /* SavePrefs */
                UIK_MemCopy( prefs, newprefs, sizeof(struct UIKPrefs) );
                if (arc)
                    {
                    if (APS_SavePrefs( ah, arcname, envname, "uik.prefs", 0 )) size = sizeof(struct UIKPrefs);
                    }
                else{
                    if (APS_SaveEnvPrefs( ah, envname, "uik.prefs", 0 )) size = sizeof(struct UIKPrefs);
                    }
                }
            }

        APS_UnregisterAppl( ah );
        }

    return( size );
}
