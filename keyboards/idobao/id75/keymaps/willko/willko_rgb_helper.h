/* Copyright 2018 MechMerlin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

typedef enum {
    LC_AMBER = 0,
    LC_RED,
    LC_BLUE,
    LC_WHITE,
    LC_GREEN,
    LC_PURPLE,
} LIGHTING_COLOR;

typedef enum {
    BAM_DEFAULT_ON = 1,
    BAM_SOLID_ON = 2,
    BAM_VERTICAL_RAINBOW = 3,
    BAM_HORIZONTAL_RAINBOW = 4,
    BAM_PULSE = 5,
    BAM_STRIPE_RUNNER_ON_WHITE = 6,
    BAM_STRIPE_RUNNER = 7,
    BAM_WINDMILL_ON_WHITE = 8,
    BAM_WINDMILL = 9,
    BAM_SWIRL_ON_WHITE = 10,
    BAM_SWIRL = 11,
    BAM_COLOR_SEQUENCE_FLASH = 12,
    BAM_HORIZONTAL_RAINBOW_SLIDE = 13,
    BAM_VERTICAL_RAINBOW_SLIDE = 14,
    BAM_DIAGONAL_RAINBOW = 15,
    BAM_SINKHOLE_RAINBOW = 16,
    BAM_COLOR_SEQUENCE_SHIMMER = 17,
    BAM_SWIRL_RAINBOW = 18,
    BAM_OFF = 19,
    BAM_MATRIX_RAIN = 20,
} BOOT_ANIMATION_MODE;

bool check_boot_animation(void);
void init_lighting(void);
void set_boot_duration(int duration_ms);
void set_boot_animation_mode(BOOT_ANIMATION_MODE mode);
void set_lighting_color_temporarily(LIGHTING_COLOR color);
void set_hsv_from_eeprom(void);

