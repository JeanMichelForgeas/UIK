#ifndef UIKDEFS_OBJILBM

#define UIKDEFS_OBJILBM
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef UIKDEFS_OBJ
#include "uiki:uikobj.h"
#endif

#ifndef UIKDEFS_MACRO
#include "uiki:uikmacros.h"
#endif

struct UIKObjILBM {
    struct UIKObj           Obj;
    struct IntuitionBase    *IntuitionBase;
    struct GfxBase          *GfxBase;
    struct DosLibrary       *DOSBase;
    struct UIKBase          *UIKBase;
    };

/* FoundFlags */
#define IFFFOUNDF_BMHD  0x01
#define IFFFOUNDF_CAMG  0x02
#define IFFFOUNDF_CMAP  0x04
#define IFFFOUNDF_PCHG  0x08
#define IFFFOUNDF_BODY  0x10

/* Flags */
#define UIF_FORCE_UIKBITMAP 0x01    /* Force l'allocation par UIK_AllocBMap() même sous V39 ou plus */

struct UIKIlbmFrame {
    /*---------correspond à ilbmFrame */
    UWORD           *ColorTable256;       /* table des couleurs : jusqu'à 256 couleurs */
    ULONG           Colors;               /* nombre de couleurs */
    ULONG           ViewMode;             /* Contenu du hunk CAMG */
    ULONG           Reserved;             /* *** */
    UBYTE           FoundBMHD;            /* indicateur obsolete : gardé pour compatibilité */
    UBYTE           FoundFlags;           /* *** */
    UWORD           W, H;                 /* Image width & height en pixels */
    WORD            X, Y;                 /* *** */
    UBYTE           Depth;                /* nombre de bitplanes */
    UBYTE           Masking;              /* technique de masquage */
    UBYTE           Compression;          /* algorithme de compression */
    UBYTE           Flags;                /* *** */
    UWORD           TransparentColor;     /* numéro de la couleur transparente */
    UBYTE           XAspect, YAspect;     /* aspect ratio, a rational number x/y */
    WORD            PageWidth, PageHeight;/* source "page" size in pixels */
    UWORD           ColorTable[32];       /* les 32 premières couleurs sont recopiées ici pour compatibilité */

    /*---------autres */
    struct BitMap   *(*BMAllocator)();    /* fonction d'allocation... */
    struct BitMap   *BMap;                /* résultat: BitMap allouée */
    ULONG           UserData;             /* pas touché: pour le programmeur */
    UWORD           xb, yb, wb, hb;       /* *** */
    UWORD           xi, yi, wi, hi;       /* *** */

    /*---------PCHG */
    UWORD           PCHGCompression;      /* struct PCHGHeader */
    UWORD           PCHGFlags;
    WORD            PCHGStartLine;
    UWORD           PCHGLineCount;
    UWORD           PCHGChangedLines;
    UWORD           PCHGMinReg;
    UWORD           PCHGMaxReg;
    UWORD           PCHGMaxChanges;
    ULONG           PCHGTotalChanges;

    UBYTE           *PCHGDatas;           /* datas allouées */
    };

#define IFF_OKAY   0L   /* (interne aux routines IFF) */
#define END_MARK  -1L   /* As if there was a chunk at end of group.*/
#define IFF_DONE  -2L   /* Tout est fait, l'image a été bien lue */
#define DOS_ERROR -3L   /* par exemple si l'objet n'arrive pas à ouvrir le fichier */
#define NOT_IFF   -4L   /* not an IFF file.*/
#define NO_FILE   -5L   /* Tried to open file, DOS didn't find it */
#define CLIENT_ERROR -6L/* Client made invalid request, for instance, write a negative size chunk.*/
#define BAD_FORM  -7L   /* A client read proc complains about FORM semantics; e.g. valid IFF, but missing a required chunk.*/
#define SHORT_CHUNK -8L /* Client asked to IFFReadBytes more bytes than left in the chunk. Could be client bug or bad form.*/
#define BAD_IFF   -9L   /* mal-formed IFF file. [TBD] Expand this into a range of error codes.*/

/*-------Fonctions internes publiques------*/

#define UIKFUNC_ILBM_File2BitMap    0
#define UIKFUNC_ILBM_Colors2Screen  4
#define UIKFUNC_ILBM_StartPCHG      8
#define UIKFUNC_ILBM_StopPCHG       12
#define UIKFUNC_ILBM_CleanupFrame   16

#define UIKFUNC_ILBM_LastCmd        16

/*-----------------------------------------*/

/*----ILBMFlags----*/
/*#define UIKF_ILBM_   0x0001*/

/*---------Obj ILBM Tags---------- */
/*#define UIKTAG_OBJILBM_       (UIKTAG_USER|0x0001) */

/*---------Obj ILBM Flags---------- */
#define UIKTAG_OBJILBM_Flags        (UIKTAG_USER|0x0020)
/*#define UIKTAG_OBJILBMFl_    (UIKTAG_USER|0x0021)*/

/*-- */

#endif
