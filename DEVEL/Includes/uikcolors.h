#ifndef UIKDEFS_COLORS

#define UIKDEFS_COLORS
/*
 *      Copyright (C) 1991,1992 Jean-Michel Forgeas
 *                  Tous Droits Réservés
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#define MAX_UIKCOLORS       32
#define MAX_UIKDEFINEDPENS  7

#define UIKCOL_GREY     0   /* couleur de fond des objets */
#define UIKCOL_MEDIUM   UIKCOL_GREY
#define UIKCOL_BLACK    1   /* couleur utilisée pour le noir */
#define UIKCOL_WHITE    2   /* couleur utilisée pour le blanc */
#define UIKCOL_LIGHT    3   /* couleur spéciale de sélection */
#define UIKCOL_ZONE     4   /* couleur du cadre mobile des modes de selection par contacts */
#define UIKCOL_LOOK     5   /* couleur de surlignage pour le look amélioré */
#define UIKCOL_IMP      6   /* couleur pour les choses importantes */

struct UIKTColors {
    UBYTE   c[MAX_UIKCOLORS];
    };

struct UIKColors {
    UBYTE   Grey;
    UBYTE   Black;
    UBYTE   White;
    UBYTE   Light;
    UBYTE   c4;
    UBYTE   c5;
    UBYTE   c6;
    UBYTE   c7;
    UBYTE   c8;
    UBYTE   c9;
    UBYTE   c10;
    UBYTE   c11;
    UBYTE   c12;
    UBYTE   c13;
    UBYTE   c14;
    UBYTE   c15;
    UBYTE   c16;
    UBYTE   c17;
    UBYTE   c18;
    UBYTE   c19;
    UBYTE   c20;
    UBYTE   c21;
    UBYTE   c22;
    UBYTE   c23;
    UBYTE   c24;
    UBYTE   c25;
    UBYTE   c26;
    UBYTE   c27;
    UBYTE   c28;
    UBYTE   c29;
    UBYTE   c30;
    UBYTE   c31;
    };

#define ColZone c4
#define ColLook c5
#define ColImp  c6

#endif
