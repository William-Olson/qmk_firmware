#pragma once

typedef enum {
    LC_AMBER = 0,
    LC_RED,
    LC_BLUE,
    LC_WHITE,
    LC_GREEN,
    LC_PURPLE,
} LIGHTING_COLOR;

void init_lighting(void);
bool is_boot_animation_done(void);
void set_hsv_from_eeprom(void);
void check_boot_animation(void);
void set_lighting_color_temporarily(LIGHTING_COLOR color);

#include_next <willko_rgb_helper.h>

