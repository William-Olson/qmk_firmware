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
#define TAPPING_TOGGLE 2

#define _MAIN_LAYER   0
#define _LOWER_LAYER  1
#define _UPPER_LAYER  2
#define _ADJUST_LAYER 3
#define _UTIL_LAYER   4

#include QMK_KEYBOARD_H

typedef enum {
    LC_AMBER = 0,
    LC_RED,
    LC_BLUE,
    LC_WHITE,
    LC_GREEN,
    LC_PURPLE,
} LIGHTING_COLOR;

enum CUSTOM_OSX_MACRO {
    MACRO_REFRESH  = SAFE_RANGE,
    MACRO_COPY,
    MACRO_PASTE,
    MACRO_CUT,
    MACRO_REDO,
    MACRO_UNDO,
    MACRO_CP_URL,
    MACRO_APP_SWITCHER
};

bool is_app_switcher_active = false;
uint16_t app_switcher_timer = 0;
uint16_t app_switcher_keep_open_threshold_ms = 1200;

#define LOWER MO(_LOWER_LAYER)
#define UPPER MO(_UPPER_LAYER)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* Main Layer
    * ,--------------------------------------------------------------------------------------------------------.
    * | Esc  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  |   -  |  F24 |  Ins |  Del |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |  ~   |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |  F23 | Home | PgUp |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * | Tab  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |   '  |  F22 |  End | PgDn |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |  Ent |  F21 |  Up  |  F20 |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |AppSwt| Ctrl | Alt  | GUI  |Lower |  Spc |  Spc |Raise |  GUI | Down |  Up  |  Up  | Left | Down |Right |
    * `--------------------------------------------------------------------------------------------------------'
    */
    [_MAIN_LAYER] = LAYOUT_ortho_5x15(
      KC_ESC,              KC_1,     KC_2,     KC_3,     KC_4,  KC_5,   KC_6,   KC_7,  KC_8,     KC_9,         KC_0,        KC_MINS,            KC_F24,   KC_INS,   KC_DEL, 
      KC_GRV,              KC_Q,     KC_W,     KC_E,     KC_R,  KC_T,   KC_Y,   KC_U,  KC_I,     KC_O,         KC_P,        KC_BSPC,            KC_F23,   KC_HOME,  KC_PGUP,
      KC_TAB,              KC_A,     KC_S,     KC_D,     KC_F,  KC_G,   KC_H,   KC_J,  KC_K,     KC_L,         KC_SCLN,     KC_QUOT,            KC_F22,   KC_END,   KC_PGDN,
      KC_LSFT,             KC_Z,     KC_X,     KC_C,     KC_V,  KC_B,   KC_N,   KC_M,  KC_COMM,  KC_DOT,       KC_SLSH,     KC_ENT,             KC_F21,   KC_UP,    KC_F20,
      MACRO_APP_SWITCHER,  KC_LCTL,  KC_LALT,  KC_LGUI,  LOWER, KC_SPC, KC_SPC, UPPER, KC_RGUI,  MACRO_COPY,   MACRO_PASTE, TT(_UTIL_LAYER),    KC_LEFT,  KC_DOWN,  KC_RGHT
    ),

    /* Lower Layer
    * ,--------------------------------------------------------------------------------------------------------.
    * |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |   ▼  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  |   ▼  |   ▼  |   ▼  |   ▼  |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |   ▼  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   _  |   +  |   {  |   }  |  |   |   ▼  |   ▼  |   ▼  |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |   ▼  |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  | Vol+ |   ▼  |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  | Next | Vol- | Play |
    * `--------------------------------------------------------------------------------------------------------'
    */
    [_LOWER_LAYER] = LAYOUT_ortho_5x15(
      _______, _______, _______,  _______,  _______,  _______,  _______,  _______, _______, _______, _______, _______, _______, _______, _______, 
      _______, KC_EXLM, KC_AT,    KC_HASH,  KC_DLR,   KC_PERC,  KC_CIRC,  KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______, _______, _______, _______,
      _______, KC_F1,   KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, _______, _______, _______,
      _______, KC_F7,   KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______, _______, _______, _______, _______, _______, KC_VOLU, KC_MNXT,
      _______, _______, _______,  _______,  _______,  _______,  _______,  _______, _______, _______, _______, _______, KC_MSTP, KC_VOLD, KC_MPLY
    ),

    /* Upper Layer
    * ,--------------------------------------------------------------------------------------------------------.
    * |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  |   ▼  |   ▼  |   ▼  |   ▼  |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |   ▼  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   -  |   =  |   [  |   ]  |   \  |   ▼  |   ▼  |   ▼  |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |   ▼  |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  | Vol+ |   ▼  |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  | Next | Vol- | Play |
    * `--------------------------------------------------------------------------------------------------------'
    */
    [_UPPER_LAYER] = LAYOUT_ortho_5x15(
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
      _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______, _______, _______, _______,
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_MINS, KC_PEQL, KC_LBRC, KC_RBRC, KC_BSLS, _______, _______, _______,
      _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, _______, _______, _______, _______, KC_VOLU, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_MPLY
    ),

    /* Adjust RBG (Lower + Raise)
    * ,--------------------------------------------------------------------------------------------------------.
    * |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |Debug |   ▼  | Reset|
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |   ▼  |BLCyc | Brt+ |RgbTgl|   ▼  |   ▼  | Sat- | Sat+ |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |   ▼  | Hue- | Brt- | Hue+ |  BL+ |  BL- |Mode- |Mode+ |Speed-|Speed+| BLTgl|   ▼  |   ▼  |   ▼  |   ▼  |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |
    * `--------------------------------------------------------------------------------------------------------'
    */
    [_ADJUST_LAYER] = LAYOUT_ortho_5x15(
      _______, _______, _______, _______, _______,  _______,  _______, _______,   _______, _______, _______, _______, DEBUG,   _______, RESET, 
      _______, BL_STEP, RGB_VAI, RGB_TOG, _______,  _______,  RGB_SAI, RGB_SAD,   _______, _______, BL_TOGG, _______, _______, _______, _______,
      _______, RGB_HUD, RGB_VAD, RGB_HUI, BL_INC,   BL_DEC,   RGB_MOD, RGB_RMOD,  RGB_SPI, RGB_SPD, BL_BRTG, _______, _______, _______, _______,
      _______, _______, _______, _______, _______,  _______,  _______, _______,   _______, _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______,  _______,  _______, _______,   _______, _______, _______, _______, _______, _______, _______
    ),

    /* Layer 4 - (utility layer)
    * ,--------------------------------------------------------------------------------------------------------.
    * |  Clr |  F13 |  F14 |  F15 |  F16 |  F17 |  F18 |  F19 |  F20 |  F21 |  F22 |  F23 |  F24 |   ▼  | Sleep|
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  | Undo |   ▼  |   ▼  |   ▼  |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |   ▼  |   ▼  | Srch |   ▼  | Find |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  | Redo |   ▼  |   ▼  |   ▼  |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |   ▼  |   ▼  | Cut  | Copy | Paste|   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |Refrsh|   ▼  |   ▼  |   ▼  |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |
    * `--------------------------------------------------------------------------------------------------------'
    */
    [_UTIL_LAYER] = LAYOUT_ortho_5x15(
      KC_CLR,   KC_F13,   KC_F14,         KC_F15,      KC_F16,      KC_F17,   KC_F18,   KC_F19,   KC_F20,   KC_F21,   KC_F22,  KC_F23,        KC_F24,       _______, KC_SLEP, 
      _______,  _______,  _______,        _______,     _______,     _______,  _______,  _______,  _______,  _______,  _______, MACRO_UNDO,    _______,      _______, _______,
      _______,  _______,  KC_WWW_SEARCH,  _______,     KC_FIND,     _______,  _______,  _______,  _______,  _______,  _______, MACRO_REDO,    _______,      _______, _______,
      _______,  _______,  MACRO_CUT,      MACRO_COPY,  MACRO_PASTE, _______,  _______,  _______,  _______,  _______,  _______, MACRO_REFRESH, MACRO_CP_URL, _______, _______,
      _______,  _______,  _______,        _______,     _______,     _______,  _______,  _______,  _______,  _______,  _______, _______,       _______,      _______, _______
    )

};




////////////////////////////////////////////////////////////////
///////////////////////    RGB Helpers   ///////////////////////
////////////////////////////////////////////////////////////////

// Default LED colors
uint8_t h = 170;
uint8_t s = 255;
uint8_t v = 0;

// default animation
int rgbMode = RGB_MODE_PLAIN;

// boot animation
int rgbBootMode = RGB_MODE_GRADIENT;

// boot timeout vars
uint8_t bootComplete = 0;
int bootTimeoutDuration = 4000;
int bootTimeout;


void init_hsv_brightness(void) {
  // fetch what the brightness was last session
	h = rgblight_get_hue();
	s = rgblight_get_sat();
	v = rgblight_get_val();
	rgbMode = rgblight_get_mode();
  rgblight_sethsv(h,s,v);
}

// reset HSV va
void reset_hsv(void) {
  uint8_t currentV = rgblight_get_val();
	rgblight_sethsv(h,s,currentV);
}

/*
  Deterimes when to stop bootup animation
*/
void checkBootupAnimation(void) {
  if (bootComplete == 1) {
    return;
  }

  bootComplete = (timer_elapsed(bootTimeout) > bootTimeoutDuration) ? 1 : 0;

  if (bootComplete) {
    rgblight_mode(rgbMode);
  }
}

/*
  Starts the boot animation and timers
*/
void init_lighting(void) {
    // start a timeout
  bootTimeout = timer_read();

  // set rgb color
  init_hsv_brightness();

  // init rgb
  rgblight_enable();

  // animate with boot sequence
  rgblight_mode(rgbBootMode);
}

/*
  Allows setting an rgb color
*/
void set_lighting_color(LIGHTING_COLOR color) {
  if (bootComplete == 0) {
    return;
  }
  switch (color) {
    case LC_AMBER:
        rgblight_sethsv(25, s, rgblight_get_val());
        break;
    case LC_RED:
        rgblight_sethsv(0, s, rgblight_get_val());
        break;
    case LC_BLUE:
        rgblight_sethsv(148, s, rgblight_get_val());
        break;
    case LC_WHITE:
        rgblight_sethsv(155, 50, rgblight_get_val());
        break;
    case LC_GREEN:
	      rgblight_sethsv(80, s, rgblight_get_val());
        break;
    case LC_PURPLE:
        rgblight_sethsv(170, s, rgblight_get_val());
        break;
    default:
        break;
  }
}



////////////////////////////////////////////////////////////////
////////////////////      QMK Callbacks    /////////////////////
////////////////////////////////////////////////////////////////


// macro handler
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case MACRO_REFRESH:
      if (record->event.pressed) {
        SEND_STRING(SS_LGUI("r"));
      }
      return false;
      break;
    case MACRO_COPY:
      if (record->event.pressed) {
        SEND_STRING(SS_LGUI("c"));
      }
      return false;
      break;
    case MACRO_PASTE:
      if (record->event.pressed) {
        SEND_STRING(SS_LGUI("v"));
      }
      return false;
      break;
    case MACRO_CUT:
      if (record->event.pressed) {
        SEND_STRING(SS_LGUI("x"));
      }
      return false;
      break;
    case MACRO_REDO:
      if (record->event.pressed) {
        SEND_STRING(SS_LGUI(SS_LSFT("z")));
      }
      return false;
      break;
    case MACRO_UNDO:
      if (record->event.pressed) {
        SEND_STRING(SS_LGUI("z"));
      }
      return false;
      break;
    case MACRO_CP_URL:
      if (record->event.pressed) {
        SEND_STRING(SS_LGUI("l")SS_LGUI("c"));
      }
      return false;
      break;
    case MACRO_APP_SWITCHER:
      if (record->event.pressed) {
        if (!is_app_switcher_active) {
          is_app_switcher_active = true;
          register_code(KC_LGUI);
        }
        app_switcher_timer = timer_read();
        register_code(KC_TAB);
      } else {
        unregister_code(KC_TAB);
      }
      break;
  }
  return true;
}

/*
  Callback for layer functions. Triggered on layer changes.
*/
layer_state_t layer_state_set_user(layer_state_t state) {

  // trigger color change based on layer state
  switch (get_highest_layer(state)) {
    case _MAIN_LAYER:
        reset_hsv();
        break;
    case _LOWER_LAYER:
        set_lighting_color(LC_AMBER);
        break;
    case _UPPER_LAYER:
        set_lighting_color(LC_PURPLE);
        break;
    case _ADJUST_LAYER:
        set_lighting_color(LC_WHITE);
        break;
    case _UTIL_LAYER:
        set_lighting_color(LC_GREEN);
        break;
    default:
        break;
  }

  // allow three layer acces with the LOWER and UPPER keys
  return update_tri_layer_state(state, _LOWER_LAYER, _UPPER_LAYER, _ADJUST_LAYER);
}


/*
  Callback for default layer functions. Called on keyboard initialization.
*/
void keyboard_post_init_user(void) {
  // init boot animation
  init_lighting();
};

/*
  Called very frequently (many times per second). Take caution around
  performance inside this function.
*/
void matrix_scan_user(void) {
  
  // disable the bootup animation after time elapsed
  checkBootupAnimation();

  // release app switcher check
  if (is_app_switcher_active) {
    if (timer_elapsed(app_switcher_timer) > app_switcher_keep_open_threshold_ms) {
      unregister_code(KC_LGUI);
      is_app_switcher_active = false;
    }
  }
}
