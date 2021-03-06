
#include "uiki:uikgfx.h"

static UWORD chip DownData[5][5][1] =
   {
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0xFE00,    /* #######......... */
   0x7C00,    /* .#####.......... */
   0x3800,    /* ..###........... */
   0x1000,    /* ...#............ */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0xFE00,    /* #######......... */
   0x7C00,    /* .#####.......... */
   0x3800,    /* ..###........... */
   0x1000,    /* ...#............ */
   };
struct UIKBitMap __far UBM_Down = { &DownData[4][0][0], UIKBMAP_BWSYS1|UIKBMAP_MASKED|UIKBMAP_CONTIG,
    7, 2,5, 0,4,0, &DownData[0][0][0], &DownData[1][0][0], &DownData[2][0][0], &DownData[3][0][0], 0, 0, 0, 0 };

static UWORD chip UpData[5][4][1] =
   {
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x1000,    /* ...#............ */
   0x3800,    /* ..###........... */
   0x7C00,    /* .#####.......... */
   0xFE00,    /* #######......... */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x1000,    /* ...#............ */
   0x3800,    /* ..###........... */
   0x7C00,    /* .#####.......... */
   0xFE00,    /* #######......... */
   };
struct UIKBitMap __far UBM_Up = { &UpData[4][0][0], UIKBMAP_BWSYS1|UIKBMAP_MASKED|UIKBMAP_CONTIG,
    7, 2,4, 0,4,0, &UpData[0][0][0], &UpData[1][0][0], &UpData[2][0][0], &UpData[3][0][0], 0, 0, 0, 0 };

static UWORD chip LeftData[5][5][1] =
   {
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0C00,    /* ....##.......... */
   0x3C00,    /* ..####.......... */
   0xFC00,    /* ######.......... */
   0x3C00,    /* ..####.......... */
   0x0C00,    /* ....##.......... */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0C00,    /* ....##.......... */
   0x3C00,    /* ..####.......... */
   0xFC00,    /* ######.......... */
   0x3C00,    /* ..####.......... */
   0x0C00,    /* ....##.......... */
   };
struct UIKBitMap __far UBM_Left = { &LeftData[4][0][0], UIKBMAP_BWSYS1|UIKBMAP_MASKED|UIKBMAP_CONTIG,
    8, 2,5, 0,4,0, &LeftData[0][0][0], &LeftData[1][0][0], &LeftData[2][0][0], &LeftData[3][0][0], 0, 0, 0, 0 };

static UWORD chip RightData[5][5][1] =
   {
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x3000,    /* ..##............ */
   0x3C00,    /* ..####.......... */
   0x3F00,    /* ..######........ */
   0x3C00,    /* ..####.......... */
   0x3000,    /* ..##............ */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x3000,    /* ..##............ */
   0x3C00,    /* ..####.......... */
   0x3F00,    /* ..######........ */
   0x3C00,    /* ..####.......... */
   0x3000,    /* ..##............ */
   };
struct UIKBitMap __far UBM_Right = { &RightData[4][0][0], UIKBMAP_BWSYS1|UIKBMAP_MASKED|UIKBMAP_CONTIG,
    8, 2,5, 0,4,0, &RightData[0][0][0], &RightData[1][0][0], &RightData[2][0][0], &RightData[3][0][0], 0, 0, 0, 0 };

static UWORD chip OKData[5][8][1] =
   {
   0x0000,    /* ................ */
   0x00E0,    /* ........###..... */
   0x0060,    /* .........##..... */
   0x0060,    /* .........##..... */
   0x0C60,    /* ....##...##..... */
   0x3FE0,    /* ..#########..... */
   0x0C00,    /* ....##.......... */
   0x0000,    /* ................ */

   0x01F0,    /* .......#####.... */
   0x0110,    /* .......#...#.... */
   0x0090,    /* ........#..#.... */
   0x0C90,    /* ....##..#..#.... */
   0x3390,    /* ..##..###..#.... */
   0xC010,    /* ##.........#.... */
   0x33F0,    /* ..##..######.... */
   0x0C00,    /* ....##.......... */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x01F0,    /* .......#####.... */
   0x01F0,    /* .......#####.... */
   0x00F0,    /* ........####.... */
   0x0CF0,    /* ....##..####.... */
   0x3FF0,    /* ..##########.... */
   0xFFF0,    /* ############.... */
   0x3FF0,    /* ..##########.... */
   0x0C00,    /* ....##.......... */
   };
struct UIKBitMap __far UBM_OK = { &OKData[4][0][0], UIKBMAP_BWSYS1|UIKBMAP_MASKED|UIKBMAP_CONTIG,
    12, 2,8, 0,4,0, &OKData[0][0][0], &OKData[1][0][0], &OKData[2][0][0], &OKData[3][0][0], 0, 0, 0, 0 };

/*
static UWORD chip CancelData[5][6][1] =
   {
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x8040,    /* #........#...... */
   0x6180,    /* .##....##....... */
   0x3F00,    /* ..######........ */
   0x3F00,    /* ..######........ */
   0x6180,    /* .##....##....... */
   0x8040,    /* #........#...... */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x8040,    /* #........#...... */
   0x6180,    /* .##....##....... */
   0x3F00,    /* ..######........ */
   0x3F00,    /* ..######........ */
   0x6180,    /* .##....##....... */
   0x8040,    /* #........#...... */
   };
struct UIKBitMap __far UBM_Cancel = { &CancelData[4][0][0], UIKBMAP_BWSYS1|UIKBMAP_MASKED|UIKBMAP_CONTIG,
    10, 2,6, 0,4,0, &CancelData[0][0][0], &CancelData[1][0][0], &CancelData[2][0][0], &CancelData[3][0][0], 0, 0, 0, 0 };
*/

static UWORD chip CancelData[5][6][1] =
   {
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0xe038,    /* ###.......###... */
   0x38e0,    /* ..###...###..... */
   0x0f80,    /* ....#####....... */
   0x0f80,    /* ....#####....... */
   0x38e0,    /* ..###...###..... */
   0xe038,    /* ###.......###... */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0xe038,    /* ###.......###... */
   0x38e0,    /* ..###...###..... */
   0x0f80,    /* ....#####....... */
   0x0f80,    /* ....#####....... */
   0x38e0,    /* ..###...###..... */
   0xe038,    /* ###.......###... */
   };
struct UIKBitMap __far UBM_Cancel = { &CancelData[4][0][0], UIKBMAP_MASKED|UIKBMAP_CONTIG,
    13, 2,6, 0,4,0, &CancelData[0][0][0], &CancelData[1][0][0], &CancelData[2][0][0], &CancelData[3][0][0], 0, 0, 0 };


static UWORD chip DirecData[5][6][1] =
   {
   0x0600,    /* .....##......... */
   0x0580,    /* .....#.##....... */
   0x0460,    /* .....#...##..... */
   0x0400,    /* .....#.......... */
   0x0400,    /* .....#.......... */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0060,    /* .........##..... */
   0x0180,    /* .......##....... */
   0x0600,    /* .....##......... */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0600,    /* .....##......... */
   0x0580,    /* .....#.##....... */
   0x0460,    /* .....#...##..... */
   0x0460,    /* .....#...##..... */
   0x0580,    /* .....#.##....... */
   0x0600,    /* .....##......... */
   };
struct UIKBitMap __far UBM_Direc = { &DirecData[4][0][0], UIKBMAP_BWSYS1|UIKBMAP_MASKED|UIKBMAP_CONTIG,
    16, 2,6, 0,4,0, &DirecData[0][0][0], &DirecData[1][0][0], &DirecData[2][0][0], &DirecData[3][0][0], 0, 0, 0, 0 };

/*
static UWORD chip CranPData[5][2][1] =
   {
   0x2000,    /* ..#............. */
   0x6000,    /* .##............. */

   0xC000,    /* ##.............. */
   0x8000,    /* #............... */

   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   };
struct UIKBitMap __far UBM_CranP = { NULL, UIKBMAP_BWSYS1|UIKBMAP_CONTIG,
    3, 2,2, 0,4,0, &CranPData[0][0][0], &CranPData[1][0][0], &CranPData[2][0][0], &CranPData[3][0][0], 0, 0, 0, 0 };
*/

static UWORD chip Prop1Data[5][5][1] =
   {
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0800,    /* ....#........... */
   0x1800,    /* ...##........... */
   0x3000,    /* ..##............ */

   0x7000,    /* .###............ */
   0xe800,    /* ###.#........... */
   0xc000,    /* ##.............. */
   0x8000,    /* #............... */
   0x4000,    /* .#.............. */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x7000,    /* .###............ */
   0xe800,    /* ###.#........... */
   0xc800,    /* ##..#........... */
   0x9800,    /* #..##........... */
   0x7000,    /* .###............ */
   };
struct UIKBitMap __far UBM_Prop1 = { &Prop1Data[4][0][0], UIKBMAP_MASKED|UIKBMAP_CONTIG,
    5, 2,5, 0,4,0, &Prop1Data[0][0][0], &Prop1Data[1][0][0], &Prop1Data[2][0][0], &Prop1Data[3][0][0], 0, 0, 0, 0 };

static UWORD chip Prop1SpData[5][5][1] =
   {
   0x0000,    /* ................ */
   0x3800,    /* ..###........... */
   0x6800,    /* .##.#........... */
   0x5800,    /* .#.##........... */
   0x7000,    /* .###............ */

   0x7000,    /* .###............ */
   0xf800,    /* #####........... */
   0xe000,    /* ###............. */
   0xc000,    /* ##.............. */
   0x4000,    /* .#.............. */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x7000,    /* .###............ */
   0xf800,    /* #####........... */
   0xe800,    /* ###.#........... */
   0xd800,    /* ##.##........... */
   0x7000,    /* .###............ */
   };
struct UIKBitMap __far UBM_Prop1Sp = { &Prop1SpData[4][0][0], UIKBMAP_MASKED|UIKBMAP_CONTIG,
    5, 2,5, 0,4,0, &Prop1SpData[0][0][0], &Prop1SpData[1][0][0], &Prop1SpData[2][0][0], &Prop1SpData[3][0][0], 0, 0, 0, 0 };

static UWORD chip Prop2Data[5][3][1] =
   {
   0x0000,    /* ................ */
   0x0800,    /* ....#........... */
   0x3800,    /* ..###........... */

   0x7000,    /* .###............ */
   0xc000,    /* ##.............. */
   0x4000,    /* .#.............. */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x7000,    /* .###............ */
   0xc800,    /* ##..#........... */
   0x7800,    /* .####........... */
   };
struct UIKBitMap __far UBM_Prop2 = { &Prop2Data[4][0][0], UIKBMAP_MASKED|UIKBMAP_CONTIG,
    5, 2,3, 0,4,0, &Prop2Data[0][0][0], &Prop2Data[1][0][0], &Prop2Data[2][0][0], &Prop2Data[3][0][0], 0, 0, 0, 0 };

static UWORD chip Prop2SpData[5][3][1] =
   {
   0x1000,    /* ...#............ */
   0x2800,    /* ..#.#........... */
   0x7000,    /* .###............ */

   0x7000,    /* .###............ */
   0xe000,    /* ###............. */
   0x4000,    /* .#.............. */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x7000,    /* .###............ */
   0xe800,    /* ###.#........... */
   0x7000,    /* .###............ */
   };
struct UIKBitMap __far UBM_Prop2Sp = { &Prop2SpData[4][0][0], UIKBMAP_MASKED|UIKBMAP_CONTIG,
    5, 2,3, 0,4,0, &Prop2SpData[0][0][0], &Prop2SpData[1][0][0], &Prop2SpData[2][0][0], &Prop2SpData[3][0][0], 0, 0, 0, 0 };


struct UIKBitMap * __far BitMapTable[] = {
    &UBM_Down,
    &UBM_Up,
    &UBM_Left,
    &UBM_Right,
    &UBM_OK,
    &UBM_Cancel,
    &UBM_Direc,
    &UBM_Prop1,
    &UBM_Prop1Sp,
    &UBM_Prop2,
    &UBM_Prop2Sp,
    0
    };
