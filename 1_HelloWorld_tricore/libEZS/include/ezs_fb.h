/*!
 * @file ezs_fb.h
 * @brief eCos framebuffer helper
 * @author Martin Hoffmann
 * \ingroup hw
 * Some helper functions for the eCos framebuffer driver.
 * \note You can still use the eCos functions directly, of course
 *
 */

#ifndef __FB_H_
#define __FB_H_

#include <cyg/infra/cyg_type.h>
#include <cyg/infra/diag.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/io/framebuf.h>
#include <string.h>
#include <stdio.h>

#define FRAMEBUF fb0
#define FB_WIDTH    CYG_FB_WIDTH(FRAMEBUF)
#define FB_HEIGHT   CYG_FB_HEIGHT(FRAMEBUF)

#define FB_WHITE ((16*4)-1)

#define FB_FONT_SMILE 1

/*!
 *  \brief Initialize the framebuffer. Call this function exactly once before
 *  using any of the other framebuffer related functions.
 */
void ezs_fb_init(void);

/*!
 *  \brief Fill block.
 *  \param x X-Axis start position.
 *  \param y Y-Axis start position.
 *  \param width Width of the box.
 *  \param height Height of the box.
 *  \param color Color.
 */
void ezs_fb_fill_block(cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 width,
                       cyg_ucount16 height, cyg_fb_colour color);

/*!
 *  \brief Draw horizontal line.
 *  \param x X-Axis start position.
 *  \param y Y-Axis start position.
 *  \param len length of the line.
 *  \param color Color.
 */
void ezs_fb_hline(cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 len,
                  cyg_fb_colour color);

/*!
 *  \brief Draw vertical line.
 *  \param x X-Axis start position.
 *  \param y Y-Axis start position.
 *  \param len length of the line.
 *  \param color Color.
 */
void ezs_fb_vline(cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 len,
                  cyg_fb_colour color);

/*!
 *  \brief Clear the framebuffer.
 *  \param color Color.
 */
void ezs_fb_clear(cyg_fb_colour color);

/*  and so on.. see: http://ecos.sourceware.org/docs-latest/ref/framebuf-drawing.html   */

/*!
 * \brief Prints a character at a given position.
 * \param c Character to print.
 * \param x X-Axis position.
 * \param y Y-Axis position.
 * \param color Color.
 */
void ezs_fb_print_char(int c, cyg_ucount16 x, cyg_ucount16 y,
                       cyg_fb_colour color);

/*!
 * \brief Prints a string at a given position.
 * \param c Pointer to null terminated string.
 * \param x X-Axis position.
 * \param y Y-Axis position.
 * \param color Color.
 */
void ezs_fb_print_string_cur(char* c, cyg_ucount16 x, cyg_ucount16 y,
                             cyg_fb_colour color);

/*!
 * \brief Prints a string.
 * \param c Pointer to null terminated string.
 * \param color String color.
 */
void ezs_fb_print_string(char* c, cyg_fb_colour color);

/*!
 * \brief Sets the cursor to a new line.
 */
void ezs_fb_newline(void);

/*!
 * \brief Sets the cursor to an arbitrary position.
 * \param x X-axis position (0 .. FB_WIDTH)
 * \param y Y-axis position (0 .. FB_HEIGHT)
 */
void ezs_fb_setpos(cyg_ucount16 x, cyg_ucount16 y);

#endif

