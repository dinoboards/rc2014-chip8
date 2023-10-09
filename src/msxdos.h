#ifndef __MSX_DOS
#define __MSX_DOS

/**
 * Function: initialises the screen to TEXT1 mode (40 x 24). In this routine, the palette is not initialised. To initialise the palette, call INIPLT in SUB-ROM after this call.
 * Input:
 * TXTNAM for the pattern name table
 * TXTCGP for the pattern generator table
 * LINL40 for the length of one line
 * Output: none
 * Registers: all
 */
extern void msxbiosInitxt(void);

/**
 * Function: restores the palette from VRAM
 * Input: none
 * Output: none
 */
extern void msxbiosRestorePalette(void);

/**
 * Function: initialises the palette (the current palette is saved in VRAM)
 * Input: none
 * Output: none
 */
extern void msxbiosInitPalette(void);

#endif
