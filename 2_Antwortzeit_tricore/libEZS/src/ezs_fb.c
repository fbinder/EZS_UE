#include "libEZS/include/ezs_fb.h"
typedef struct {
    int x;
    int y;
} cursor_t;

static cursor_t cursor;
void ezs_fb_init(void) {
    CYG_FB_ON(FRAMEBUF);
    CYG_FB_FILL_BLOCK(FRAMEBUF, 0, 0, FB_WIDTH, FB_HEIGHT, 0);
    cursor.x = 0;
    cursor.y = 0;
}


void ezs_fb_fill_block(cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 width,
                       cyg_ucount16 height, cyg_fb_colour color) {
    CYG_FB_FILL_BLOCK(FRAMEBUF, x, y, width, height, color);
}


void ezs_fb_hline(cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 len,
                  cyg_fb_colour color) {
    CYG_FB_WRITE_HLINE(FRAMEBUF, x, y, len, color);
}


void ezs_fb_vline(cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 len,
                  cyg_fb_colour color) {

    CYG_FB_WRITE_VLINE(FRAMEBUF, x, y, len, color);
}

void ezs_fb_clear(cyg_fb_colour color) {
    ezs_fb_fill_block(0, 0, FB_WIDTH, FB_HEIGHT, color);
}


void ezs_fb_print_char( int ch, cyg_ucount16 x, cyg_ucount16 y,
                        cyg_fb_colour color) {
    int i, j;

    for (i = ch * 8; i < (ch * 8) + 8; i++) {
        unsigned char row = g_8x8_font[i];

        for (j = 0; j < 8; j++) {
            if ((row & (0x80 >> j)) != 0) {
                ezs_fb_fill_block(x + j, y + (i - ch * 8), 1, 1, color);
            }
        }
    }

}

void ezs_fb_print_string(char* c,  cyg_fb_colour color) {
    ezs_fb_print_string_cur(c, cursor.x, cursor.y, color);
}

void ezs_fb_print_string_cur(char* c, cyg_ucount16 x, cyg_ucount16 y,
                             cyg_fb_colour color) {
    char* ch = c;

    while (*ch != '\0') {
        if (*ch == '\n') {
            ezs_fb_newline();
        }
        else {
            ezs_fb_print_char((*ch) + 0, x , y, color);
            x = x + 8;

            if ( x >= FB_WIDTH - 8) {
                y += 8;
                x = 0;
            }
        }

        ch++;
    }

    cursor.x = x;
    cursor.y = y;
}

void ezs_fb_newline(void) {
    cursor.x = 0;
    cursor.y += 8;
}

void ezs_fb_setpos(cyg_ucount16 x, cyg_ucount16 y) {
    cursor.x = x;
    cursor.y = y;
}

void ezs_fb_drawCoordSystem(void) {
    // y-axis
    ezs_fb_vline(1, 0, FB_HEIGHT, CYG_FB_DEFAULT_PALETTE_BLACK);
    ezs_fb_hline(0, 1, 3, CYG_FB_DEFAULT_PALETTE_BLACK);

    // x-axis
    ezs_fb_hline(0, FB_HEIGHT - 2, FB_WIDTH, CYG_FB_DEFAULT_PALETTE_BLACK);
    ezs_fb_vline(FB_WIDTH - 2, FB_HEIGHT - 3, 3, CYG_FB_DEFAULT_PALETTE_BLACK);
}

void ezs_fb_markXaxis(short numberOfMarks) {

    int delta = (FB_WIDTH - 1) / (numberOfMarks + 1);
    int shift = delta + 1;

    int i;

    for (i = 0; i < numberOfMarks; ++i) {
        ezs_fb_vline(shift, FB_HEIGHT - 4, 3, CYG_FB_DEFAULT_PALETTE_BLACK);
        shift += delta;
    }
}

void ezs_fb_markYaxis(short numberOfMarks) {

    int delta = (FB_HEIGHT - 1) / (numberOfMarks + 1);
    int shift = FB_HEIGHT - 1 - delta;

    int i;

    for (i = 0; i < numberOfMarks + 1; ++i) {
        ezs_fb_hline(0, shift, 3, CYG_FB_DEFAULT_PALETTE_BLACK);
        shift -= delta;
    }
}

void ezs_fb_drawBarOnCoordSystem(float upperBound, float lowerBound,
                                 float inputArray[], int sizeOfArray) {

    // Clear FrameBuffer
    ezs_fb_clear(CYG_FB_DEFAULT_PALETTE_WHITE);

    // Flag for bufferOverflow/Underflow
    bool overflow;
    bool underflow;

    // Define max / min Area Y-Axis
    int maxValueY = FB_HEIGHT - ((FB_HEIGHT - 1) / 11) * 10 - 1; //=21
    int minValueY = FB_HEIGHT - 1; //=239
    int area = minValueY - maxValueY; //=218

    // Define half block width
    int halfBlockWidth = ((FB_WIDTH - 1) / (sizeOfArray + 1)) / 3;

    // Temp variables for scaling
    assert(upperBound > lowerBound);
    float scaleTo = upperBound - lowerBound;
    float offset = 0 - lowerBound;
    float scaledValue;
    int scaledValueInt;

    // Bar drawing loop
    int delta = (FB_WIDTH - 1) / (sizeOfArray + 1);
    int shift = delta + 1;
    int i;

    for (i = 0; i < sizeOfArray; ++i) {

        scaledValue = (inputArray[i] + offset) / scaleTo;

        // Overflow check!
        if (scaledValue > 1.0) {
            scaledValue = 1.0;
            overflow = true;
        }
        else {
            overflow = false;
        }

        // Underflow check!
        if (scaledValue < 0.0) {
            scaledValue = 0.0;
            underflow = true;
        }
        else {
            underflow = false;
        }

        scaledValueInt = scaledValue * area;

        if (!overflow) {
            if (!underflow) {
                ezs_fb_fill_block(shift - halfBlockWidth, minValueY - scaledValueInt,
                                  halfBlockWidth * 2, scaledValueInt, CYG_FB_DEFAULT_PALETTE_BLUE);
            }
            else {
                ezs_fb_fill_block(shift - halfBlockWidth, minValueY, halfBlockWidth * 2, 2,
                                  CYG_FB_DEFAULT_PALETTE_YELLOW);
            }
        }
        else {
            ezs_fb_fill_block(shift - halfBlockWidth, minValueY - scaledValueInt,
                              halfBlockWidth * 2, scaledValueInt, CYG_FB_DEFAULT_PALETTE_RED);
        }

        shift += delta;
    }

    // Draw coord system
    ezs_fb_drawCoordSystem();
    ezs_fb_markYaxis(10);
    ezs_fb_markXaxis(sizeOfArray);

    /*!
    *  \brief Fill block.
    *  \param x X-Axis start position.
    *  \param y Y-Axis start position.
    *  \param width Width of the box.
    *  \param height Height of the box.
    *  \param color Color.
    */
//void ezs_fb_fill_block(cyg_ucount16 x, cyg_ucount16 y, cyg_ucount16 width,cyg_ucount16 height,cyg_fb_colour color);


}
