/**
 * Copyright 2023 Stefano Cottafavi <stefano.cottafavi@gmail.com>.
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include <zephyr/kernel.h>

#include "display.h"

#define STACKSIZE 1024
#define THREAD_LOW 9

void thread_lcd(void * id, void * unused1, void * unused2)
{
    display_init();
    display_play();
}

K_THREAD_DEFINE(lcd, STACKSIZE, thread_lcd, NULL, NULL, NULL, THREAD_LOW, 0, 0);
