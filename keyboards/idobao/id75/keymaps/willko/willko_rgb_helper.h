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

void init_lighting(void);
void set_hsv_from_eeprom(void);
bool check_boot_animation(void);
void set_lighting_color_temporarily(LIGHTING_COLOR color);


