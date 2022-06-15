#include "uiki:uikgfx.h"

static UWORD chip PicCheckData[5][7][1] =
   {
   0x0038,    /* ..........###... */
   0x0060,    /* .........##..... */
   0x00c0,    /* ........##...... */
   0xe180,    /* ###....##....... */
   0x7300,    /* .###..##........ */
   0x3e00,    /* ..#####......... */
   0x1c00,    /* ...###.......... */

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
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0038,    /* ..........###... */
   0x0060,    /* .........##..... */
   0x00c0,    /* ........##...... */
   0xe180,    /* ###....##....... */
   0x7300,    /* .###..##........ */
   0x3e00,    /* ..#####......... */
   0x1c00,    /* ...###.......... */
   };
struct UIKBitMap __far UBM_PicCheck = { &PicCheckData[4][0][0], UIKBMAP_MASKED|UIKBMAP_CONTIG,
    13, 2,7, 0,4,0, &PicCheckData[0][0][0], &PicCheckData[1][0][0], &PicCheckData[2][0][0], &PicCheckData[3][0][0], 0, 0, 0, 0 };

static UWORD chip PicImage1Data[5][11][1] =
   {
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0040,    /* .........#...... */
   0x0020,    /* ..........#..... */
   0x0020,    /* ..........#..... */
   0x0020,    /* ..........#..... */
   0x0020,    /* ..........#..... */
   0x0020,    /* ..........#..... */
   0x0040,    /* .........#...... */
   0x2080,    /* ..#.....#....... */
   0x1f00,    /* ...#####........ */

   0x1f00,    /* ...#####........ */
   0x2080,    /* ..#.....#....... */
   0x4000,    /* .#.............. */
   0x8000,    /* #............... */
   0x8000,    /* #............... */
   0x8000,    /* #............... */
   0x8000,    /* #............... */
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

   0x1f00,    /* ...#####........ */
   0x2080,    /* ..#.....#....... */
   0x4040,    /* .#.......#...... */
   0x8020,    /* #.........#..... */
   0x8020,    /* #.........#..... */
   0x8020,    /* #.........#..... */
   0x8020,    /* #.........#..... */
   0x8020,    /* #.........#..... */
   0x4040,    /* .#.......#...... */
   0x2080,    /* ..#.....#....... */
   0x1f00,    /* ...#####........ */
   };
struct UIKBitMap __far UBM_PicImage1 = { &PicImage1Data[4][0][0], UIKBMAP_MASKED|UIKBMAP_CONTIG,
    11, 2,11, 0,4,0, &PicImage1Data[0][0][0], &PicImage1Data[1][0][0], &PicImage1Data[2][0][0], &PicImage1Data[3][0][0], 0, 0, 0, 0 };

static UWORD chip PicAltImage1Data[5][11][1] =
   {
   0x1f00,    /* ...#####........ */
   0x2080,    /* ..#.....#....... */
   0x4e00,    /* .#..###......... */
   0x9f00,    /* #..#####........ */
   0xbf80,    /* #.#######....... */
   0xbf80,    /* #.#######....... */
   0xbf80,    /* #.#######....... */
   0x9f00,    /* #..#####........ */
   0x4e00,    /* .#..###......... */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0e40,    /* ....###..#...... */
   0x1f20,    /* ...#####..#..... */
   0x3fa0,    /* ..#######.#..... */
   0x3fa0,    /* ..#######.#..... */
   0x3fa0,    /* ..#######.#..... */
   0x1f20,    /* ...#####..#..... */
   0x0e40,    /* ....###..#...... */
   0x2080,    /* ..#.....#....... */
   0x1f00,    /* ...#####........ */

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
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */
   0x0000,    /* ................ */

   0x1f00,    /* ...#####........ */
   0x2080,    /* ..#.....#....... */
   0x4e40,    /* .#..###..#...... */
   0x9f20,    /* #..#####..#..... */
   0xbfa0,    /* #.#######.#..... */
   0xbfa0,    /* #.#######.#..... */
   0xbfa0,    /* #.#######.#..... */
   0x9f20,    /* #..#####..#..... */
   0x4e40,    /* .#..###..#...... */
   0x2080,    /* ..#.....#....... */
   0x1f00,    /* ...#####........ */
   };
struct UIKBitMap __far UBM_PicAltImage1 = { &PicAltImage1Data[4][0][0], UIKBMAP_MASKED|UIKBMAP_CONTIG,
    11, 2,11, 0,4,0, &PicAltImage1Data[0][0][0], &PicAltImage1Data[1][0][0], &PicAltImage1Data[2][0][0], &PicAltImage1Data[3][0][0], 0, 0, 0, 0 };

static UWORD chip PicImage2Data[5][9][2] =
   {
   0x0000, 0x0000,    /* ................................ */
   0x0003, 0x0000,    /* ..............##................ */
   0x0001, 0x8000,    /* ...............##............... */
   0x0001, 0x8000,    /* ...............##............... */
   0x0001, 0x8000,    /* ...............##............... */
   0x0001, 0x8000,    /* ...............##............... */
   0x0001, 0x8000,    /* ...............##............... */
   0x0003, 0x0000,    /* ..............##................ */
   0x1ffe, 0x0000,    /* ...############................. */

   0x3ffe, 0x0000,    /* ..#############................. */
   0x6000, 0x0000,    /* .##............................. */
   0xc000, 0x0000,    /* ##.............................. */
   0xc000, 0x0000,    /* ##.............................. */
   0xc000, 0x0000,    /* ##.............................. */
   0xc000, 0x0000,    /* ##.............................. */
   0xc000, 0x0000,    /* ##.............................. */
   0x6000, 0x0000,    /* .##............................. */
   0x2000, 0x0000,    /* ..#............................. */

   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */

   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */

   0x3ffe, 0x0000,    /* ..#############................. */
   0x6003, 0x0000,    /* .##...........##................ */
   0xc001, 0x8000,    /* ##.............##............... */
   0xc001, 0x8000,    /* ##.............##............... */
   0xc001, 0x8000,    /* ##.............##............... */
   0xc001, 0x8000,    /* ##.............##............... */
   0xc001, 0x8000,    /* ##.............##............... */
   0x6003, 0x0000,    /* .##...........##................ */
   0x3ffe, 0x0000,    /* ..#############................. */
   };
struct UIKBitMap __far UBM_PicImage2 = { &PicImage2Data[4][0][0], UIKBMAP_MASKED|UIKBMAP_CONTIG,
    17, 4,9, 0,4,0, &PicImage2Data[0][0][0], &PicImage2Data[1][0][0], &PicImage2Data[2][0][0], &PicImage2Data[3][0][0], 0, 0, 0, 0 };

static UWORD chip PicAltImage2Data[5][9][2] =
   {
   0x3ffe, 0x0000,    /* ..#############................. */
   0x6000, 0x0000,    /* .##............................. */
   0xc7f0, 0x0000,    /* ##...#######.................... */
   0xcff8, 0x0000,    /* ##..#########................... */
   0xcff8, 0x0000,    /* ##..#########................... */
   0xcff8, 0x0000,    /* ##..#########................... */
   0xc7f0, 0x0000,    /* ##...#######.................... */
   0x6000, 0x0000,    /* .##............................. */
   0x2000, 0x0000,    /* ..#............................. */

   0x0000, 0x0000,    /* ................................ */
   0x0003, 0x0000,    /* ..............##................ */
   0x07f1, 0x8000,    /* .....#######...##............... */
   0x0ff9, 0x8000,    /* ....#########..##............... */
   0x0ff9, 0x8000,    /* ....#########..##............... */
   0x0ff9, 0x8000,    /* ....#########..##............... */
   0x07f1, 0x8000,    /* .....#######...##............... */
   0x0003, 0x0000,    /* ..............##................ */
   0x1ffe, 0x0000,    /* ...############................. */

   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */

   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */
   0x0000, 0x0000,    /* ................................ */

   0x3ffe, 0x0000,    /* ..#############................. */
   0x6003, 0x0000,    /* .##...........##................ */
   0xc7f1, 0x8000,    /* ##...#######...##............... */
   0xcff9, 0x8000,    /* ##..#########..##............... */
   0xcff9, 0x8000,    /* ##..#########..##............... */
   0xcff9, 0x8000,    /* ##..#########..##............... */
   0xc7f1, 0x8000,    /* ##...#######...##............... */
   0x6003, 0x0000,    /* .##...........##................ */
   0x3ffe, 0x0000,    /* ..#############................. */
   };
struct UIKBitMap __far UBM_PicAltImage2 = { &PicAltImage2Data[4][0][0], UIKBMAP_MASKED|UIKBMAP_CONTIG,
    17, 4,9, 0,4,0, &PicAltImage2Data[0][0][0], &PicAltImage2Data[1][0][0], &PicAltImage2Data[2][0][0], &PicAltImage2Data[3][0][0], 0, 0, 0, 0 };


struct UIKBitMap * __far UIKBitMapTable[] = {
    &UBM_PicCheck,
    &UBM_PicImage1,
    &UBM_PicAltImage1,
    &UBM_PicImage2,
    &UBM_PicAltImage2,
    0
    };