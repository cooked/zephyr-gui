/**
 * Copyright 2023 Stefano Cottafavi <stefano.cottafavi@gmail.com>.
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/display/cfb.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(display, 3);

#define SELECTED_FONT_INDEX  1  // perhaps make this a config parameter

static struct device *dev;
uint16_t rows;
uint8_t ppt;
uint8_t font_width;
uint8_t font_height;

void display_play(void)
{
    uint8_t x_offset = 0;
    uint8_t y_offset;

    while (1) {

        for (int i=0; i < rows; i++) {

            y_offset = i * ppt;

            switch (i) {
                case 0:
                    cfb_print(dev, " average", x_offset, y_offset);
                    break;
                case 1:
                    cfb_print(dev, " good",    x_offset, y_offset);
                    break;
                case 2:
                    cfb_print(dev, " better",  x_offset, y_offset);
                    break;
                case 3:
                    cfb_print(dev, " best",    x_offset, y_offset);
                    break;
                default:
                    break;
            }

            cfb_framebuffer_finalize(dev);

            k_sleep(K_MSEC(300));
        }

        cfb_framebuffer_clear(dev, false);

        if (x_offset > 50)	x_offset = 0;
        else				x_offset += 5;
    }
}

void display_init(void)
{
	
    dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

	if (!device_is_ready(dev)) {
		printf("Device %s not ready\n", dev->name);
		return;
	}

    if (display_set_pixel_format(dev, PIXEL_FORMAT_MONO10) != 0) {
		printf("Failed to set required pixel format\n");
		return;
	}

    if (cfb_framebuffer_init(dev)) {
		printf("Framebuffer initialization failed!\n");
		return;
	}

    cfb_framebuffer_clear(dev, true);
    display_blanking_off(dev);

    rows = cfb_get_display_parameter(dev, CFB_DISPLAY_ROWS);
    ppt  = cfb_get_display_parameter(dev, CFB_DISPLAY_PPT);


	// load available fonts
	for (int idx = 0; idx < 42; idx++) {
		if (cfb_get_font_size(dev, idx, &font_width, &font_height)) {
			break;
		}
		cfb_framebuffer_set_font(dev, idx);
		printf("font width %d, font height %d\n",
		       font_width, font_height);
	}


    cfb_framebuffer_set_font(dev, SELECTED_FONT_INDEX);
    // dark mode
	cfb_framebuffer_invert(dev);

    printf("x_res %d, y_res %d, ppt %d, rows %d, cols %d\n",
	       cfb_get_display_parameter(dev, CFB_DISPLAY_WIDTH),
	       cfb_get_display_parameter(dev, CFB_DISPLAY_HEIGH),
	       ppt,
	       rows,
	       cfb_get_display_parameter(dev, CFB_DISPLAY_COLS));


	printf("Initialized %s\n", dev->name);

}