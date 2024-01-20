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

#include QMK_KEYBOARD_H
#include "willko_rgb_helper.h"

////////////////////////////////////////////////////////////////
///////////////////////    RGB Helpers   ///////////////////////
////////////////////////////////////////////////////////////////

// Default LED colors
static uint8_t h = 170;
static uint8_t s = 255;
static uint8_t v = 0;

// default animation
static uint8_t rgb_mode = 1;

// can be set to another mode via set_boot_animation_mode() function
static uint8_t rgb_boot_mode = (uint8_t) BAM_STRIPE_RUNNER;


// boot timeout vars
static bool boot_complete = false;
static int boot_duration_ms = 5000;
static int bootTimeout = 0;


// fetch what the hsv was last session and set it
static void init_hsv(void) {
	h = rgblight_get_hue();
	s = rgblight_get_sat();
	v = rgblight_get_val();
	rgb_mode = rgblight_get_mode();
    rgblight_sethsv(h,s,v);
}

/*
  Starts the boot animation and timers
*/
void init_lighting(void) {
  // start a timeout
  bootTimeout = timer_read();

  // set rgb color / brightness
  init_hsv();

  // init rgb
  rgblight_enable();

  // animate with boot sequence
  rgblight_mode_noeeprom(rgb_boot_mode);
}

/*
  Set mode for boot sequence before calling init_lighting
*/
void set_boot_animation_mode(BOOT_ANIMATION_MODE mode) {
    rgb_boot_mode = (uint8_t)mode;
}

/*
  Set max duration of boot sequence before calling init_lighting
*/
void set_boot_duration(int duration_ms) {
    boot_duration_ms = duration_ms;
}

/*
  Load and set HSV values from eeprom.
*/
void set_hsv_from_eeprom(void) {
  rgblight_reload_from_eeprom();

  // set lighting based on loaded values
	rgblight_sethsv_noeeprom(
    rgblight_get_hue(),
    rgblight_get_sat(),
    rgblight_get_val()
  );
}

/*
  Deterimes when to stop bootup animation
*/
bool check_boot_animation(void) {
  if (boot_complete) {
    return boot_complete;
  }

  boot_complete = (timer_elapsed(bootTimeout) > boot_duration_ms) ? true : false;

  if (boot_complete) {
    rgblight_mode(rgb_mode);
  }

  return boot_complete;
}


/*
  Allows setting an rgb lighting color
*/
void set_lighting_color_temporarily(LIGHTING_COLOR color) {
  if (boot_complete == 0) {
    return;
  }
  switch (color) {
    case LC_AMBER:
        rgblight_sethsv_noeeprom(25, s, rgblight_get_val());
        break;
    case LC_RED:
        rgblight_sethsv_noeeprom(0, s, rgblight_get_val());
        break;
    case LC_BLUE:
        rgblight_sethsv_noeeprom(148, s, rgblight_get_val());
        break;
    case LC_WHITE:
        rgblight_sethsv_noeeprom(155, 50, rgblight_get_val());
        break;
    case LC_GREEN:
	      rgblight_sethsv_noeeprom(80, s, rgblight_get_val());
        break;
    case LC_PURPLE:
        rgblight_sethsv_noeeprom(170, s, rgblight_get_val());
        break;
    default:
        break;
  }
}
