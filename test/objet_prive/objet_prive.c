/****************************************************************
 *
 *      File:      objet_prive.c
 *      Project:   UIK: exemple d'objet privé
 *
 *      Created:   31-07-91     Jean-Michel Forgeas
 *
 *      Ce source n'est pas directement compilable. Il sert juste
 *      à montrer comment ajouter un objet privé à un arbre UIK.
 *      L'objet de cet exemple ne sert pas à grand chose. A chaque
 *      fois qu'il est démarré il ouvre 2 fichiers, et à chaque
 *      fois qu'il est stoppé il les ferme.
 *      Notez que l'ouverture et la fermeture auraient pu se faire
 *      dans Generic_Add() et Generic_Remove() aussi bien...
 *
 *      Attention: ce code n'a pas été compilé / testé
 *
 ****************************************************************/


/****** Includes ************************************************/

#include "uiki:uikbase.h"
#include "uiki:uikobj.h"
#include "uiki:uikglobal.h"
#include "uiki:uikcolors.h"
#include "uiki:uikmacros.h"
#include "uiki:uiktag.h"
#include "uiki:uik_protos.h"
#include "uiki:uik_pragmas.h"

#include "objtoto.h"

#ifdef SAS
int CXBRK    ( void ) { return(0); }
int _CXBRK   ( void ) { return(0); }
int chkabort ( void ) { return(0); }
#endif


/****** Imported ************************************************/

extern ULONG DOSBase;
extern struct UIKBase *UIKBase;


/****** Exported ***********************************************/


/****** Statics ************************************************/

/* pour faire la traductions des tags en flags */
static struct TagItem flags_boolmap[] = {
    { UIKTAG_TOTOFl_NewFile1, UIKTOTO_NEWFILE1 },
    { UIKTAG_TOTOFl_NewFile2, UIKTOTO_NEWFILE2 },
    { TAG_DONE } };


/****************************************************************
 *
 *      Code
 *
 ****************************************************************/

/* Fonction appelée par UIK_Refresh().
 */
static void Generic_Refresh( struct ObjToto *toto )
{
}

/* Fonction appelée lors d'un évènement NEWSIZE seulement si les
 * dimensions de la fenêtre ont changées.
 * Il est possible ici d'appliquer tout changement de dimension
 * aux objets internes à la fenêtre. Avant d'appeler cette fonction
 * UIK a déjà appliqué des changements relatif aux ResizeFlags de
 * l'objet.
 * Aprés avoir appelé cette fonction et aprés avoir traité les
 * vecteurs greffés à l'objet fenêtre sur NEWSIZE, UIK fait un
 * Refresh de l'objet fenêtre et de tous les sous-objets placés
 * dans la fenêtre.
 */
static void Generic_Resize( struct ObjToto *toto )
{
}

/* Fonction appelée par UIK_Start() = mise en activité de l'objet.
 * Si la fonction retourne 0, UIK_Start() retourne 0.
 */
static int Generic_Start( struct ObjToto *toto )
{
    if (toto->Name1)
        if (! (toto->File1 = Open( toto->Name1, (toto->Flags & UIKTOTO_NEWFILE1) ? MODE_NEWFILE : MODE_OLDFILE ))) goto END_ERROR;
    if (toto->Name2)
        if (! (toto->File2 = Open( toto->Name2, (toto->Flags & UIKTOTO_NEWFILE2) ? MODE_NEWFILE : MODE_OLDFILE ))) goto END_ERROR;
    return(1);

  END_ERROR:
    return(0);
}

/* Fonction appelée par UIK_Stop() = mise en sommeil de l'objet.
 */
static void Generic_Stop( struct ObjToto *toto )
{
    if (toto->File1) Close( toto->File1 );
    toto->File1 = 0;
    if (toto->File2) Close( toto->File2 );
    toto->File2 = 0;
}

/* Fonction appelée par UIK_Remove(), 1 seule fois dans la
 * vie de l'objet (mort).
 * La fonction Generic_Stop() est appelée juste avant.
 * Il faut mettre ici toutes les désallocations de mémoire qui n'ont
 * pas été faites dans Generic_Stop().
 */
static void Generic_Remove( struct ObjToto *toto )
{
}

/* Fonction appelée par UIK_AddPrivateObject(), 1 seule fois dans la
 * vie de l'objet (naissance).
 * A ce moment l'objet n'a pas de fenêtre, pas de RastPort, etc...
 * puisqu'il n'est pas démarré.
 * Les seules choses valides qu'il ait sont UIKBase et UIKGlobal.
 * Si la fonction retourne 0, UIK_AddPrivateObject() retourne 0.
 */
static int Generic_Add( struct ObjToto *toto, struct TagList *taglist )
{
    /*-------remplace les défauts d'un seul coup */
    toto->Flags = UIK_GetTagData( UIKTAG_TOTO_Flags, 0, taglist );
    /*-------modifie un par un les défauts */
    toto->Flags = UIK_PackBoolTags( toto->Flags, taglist, flags_boolmap );

    /*-------prend les noms de fichier */
    toto->Name1 = UIK_GetTagData( UIKTAG_TOTO_File1, 0, taglist );
    toto->Name2 = UIK_GetTagData( UIKTAG_TOTO_File2, 0, taglist );

    .................
    if (! (...autres initialisations...)) goto END_ERROR;
    .................

    return(1);

  END_ERROR:
    return(0);
}

/* Fonctions publiques: le code fait partie de l'objet et est rendu
 * public. Le "client" appellera chaque fonction par UIK_CallObjFunc()
 * exemple:
 *    length = UIK_CallObjFunc( OBJFUNC_TOTO_ReadFromFile, 1, buffer );
 */

static int ReadFromFile( struct UIKObjToto *obj, int file, UBYTE *dest )
{
  ULONG handle;

    handle = (file == 1 ? obj->File1 : obj->File2);
    return( Read( handle, dest, RECORD_LENGTH ) );
}

static int WriteToFile( struct UIKObjToto *obj, int file, UBYTE *buffer )
{
  ULONG handle;

    handle = (file == 1 ? obj->File1 : obj->File2);
    return( Write( handle, buffer, RECORD_LENGTH ) );
}

static ULONG FuncTable[] = {
    (ULONG) ReadFromFile,   /* fonction placée à 0 depuis début de table */
    (ULONG) WriteToFile,    /* fonction placée à 4 depuis début de table */
    };


static struct UIKObjGerm Germ = {
    "",                 /* pas de nom */
    Generic_Add,        /* int     (*AddVector)(); */
    Generic_Start,      /* int     (*StartVector)(); */
    Generic_Stop,       /* void    (*StopVector)(); */
    Generic_Remove,     /* void    (*RemoveVector)(); */
    NULL,               /* void    (*RefreshVector)(); */
    NULL,               /* void    (*ResizeVector)(); */
    NULL,               /* void    (*ExceptActVector)(); */
    NULL,               /* void    (*ExceptInaVector)(); */
    NULL,               /* void    (*ExceptMovVector)(); */
    NULL,               /* void    (*ExceptKeyVector)(); */
    NULL,               /* int     (*StartAfterVector)(); */
    NULL,               /* int     (*SetGetValues)(); */
    FuncTable,          /* ULONG   *ExtentedVectors; */
    NULL,               /* ULONG   ExReserved[1]; */
    0,                  /* UWORD   MinWidth; */
    0,                  /* UWORD   MinHeight; */
    0,                  /* ULONG   DefaultWidthHeight; */
    OBJECTID_USER | 0,  /* UWORD   ObjId; */
    sizeof(struct ObjToto), /* UWORD   ObjSize; */
    BOX_SEMAPHORE,      /* ULONG   DefaultFlags */
    };

struct UIKObjRegGerm ObjTotoRegGerm = { &Germ, 0, 0 ,0 };

************************** fichier objtoto.h *****************************

#include "uiki:uikobj.h"
#include "uiki:uiktag.h"

struct ObjToto
    {
    struct UIKObj   Obj;

    ULONG           Flags;
    UBYTE           *Name1;
    UBYTE           *Name2;
    ULONG           File1;
    ULONG           File2;
    ...........
    };

/*---------Obj TOTO Tags-----------*/
#define UIKTAG_TOTO_File1 (UIKTAG_USER|0x0001) /* pointeur sur nom du fichier1 */
#define UIKTAG_TOTO_File2 (UIKTAG_USER|0x0002) /* pointeur sur nom du fichier2 */

/*---------Obj TOTO Tag Flags----------*/
#define UIKTAG_TOTO_Flags (UIKTAG_USER|0x0020) /* pour remplacer les défauts */
 /* sinon un par un... */
#define UIKTAG_TOTOFl_NewFile1 (UIKTAG_USER|0x0021) /* FALSE */
#define UIKTAG_TOTOFl_NewFile2 (UIKTAG_USER|0x0022) /* FALSE */

/*---------Flags----------*/
#define UIKTOTO_NEWFILE1    0x01
#define UIKTOTO_NEWFILE2    0x02

/*---------Functions------*/
#define OBJFUNC_TOTO_ReadFromFile   0  /* offset 0 dans FuncTable */
#define OBJFUNC_TOTO_WriteToFile    4  /* offset 4 dans FuncTable */

/*---------Divers---------*/
#define RECORD_LENGTH  100


************************** programme principal ***************************

extern struct UIKObjRegGerm ObjTotoRegGerm;

et_si_jajoutais_un_objet_toto_hein()
{
  struct TagItem taglist_toto[] = {
        { UIKTAG_TOTO_File1 },
        { UIKTAG_TOTO_File2 },
        { TAG_END },
        }
    tl_toto[0].ti_Data = (ULONG) "ram:x";
    tl_toto[1].ti_Data = (ULONG) "ram:y";
    if (! UIK_AddPrivateObject( &ObjTotoRegGerm, parent, tl_toto )) goto END_ERROR;
    ...........
}
