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

enum ortho_layers {
  _MAIN_LAYER,
  _UTIL_LAYER,
  _GAME_LAYER,
  _LOWER_LAYER,
  _UPPER_LAYER,
  _ADJUST_LAYER
};

#define LOWER MO(_LOWER_LAYER)
#define UPPER MO(_UPPER_LAYER)
#define UTILS TT(_UTIL_LAYER)

/*
  Operating System Specific Modifiers
*/

// for Mac
#define OSS_MOD(string) SS_LGUI(string)
#define OSS_MOD_KC KC_LGUI

// (for Windows, just comment out the previous 2 lines and uncomment the following lines instead)

// // for Windows
// #define OSS_MOD(string) SS_LCTL(string)
// #define OSS_MOD_KC KC_LCTL

/*
  Enums for RGB Light Colors and Custom Macros
*/

typedef enum {
    LC_AMBER = 0,
    LC_RED,
    LC_BLUE,
    LC_WHITE,
    LC_GREEN,
    LC_PURPLE,
} LIGHTING_COLOR;

enum CUSTOM_MACROS {
    MACRO_REFRESH  = SAFE_RANGE,
    MACRO_COPY,
    MACRO_PASTE,
    MACRO_CUT,
    MACRO_PALETTE,
    MACRO_REDO,
    MACRO_UNDO,
    MACRO_CP_URL,
    MACRO_APP_SWITCHER,
    MACRO_INC_HUE,
    MACRO_DEC_HUE
};

/*
  Global variables for tracking various things
*/

// app switcher state
bool is_app_switcher_active = false;
uint16_t app_switcher_timer = 0;
uint16_t app_switcher_keep_open_threshold_ms = 1200;

// game mode layer state
bool can_enable_game_mode = true;

// variable for allowing color change while on the ADJUST layer
bool isUserChangingRgb = false;


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
int rgbBootMode = RGB_MODE_SNAKE;
// int rgbBootMode = RGB_MODE_SWIRL;
// int rgbBootMode = RGB_MODE_XMAS;
// int rgbBootMode = RGB_MODE_FORWARD;
// int rgbBootMode = RGB_MODE_RAINBOW;
// int rgbBootMode = RGB_MODE_BREATHE;

// boot timeout vars
bool bootComplete = false;
int bootTimeoutDuration = 5000;
int bootTimeout;


// fetch what the hsv was last session and set it
void init_hsv(void) {
	h = rgblight_get_hue();
	s = rgblight_get_sat();
	v = rgblight_get_val();
	rgbMode = rgblight_get_mode();
  rgblight_sethsv(h,s,v);
}

// load and set HSV values from eeprom
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
void checkBootupAnimation(void) {
  if (bootComplete) {
    return;
  }

  bootComplete = (timer_elapsed(bootTimeout) > bootTimeoutDuration) ? true : false;

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

  // set rgb color / brightness
  init_hsv();

  // init rgb
  rgblight_enable();

  // animate with boot sequence
  rgblight_mode_noeeprom(rgbBootMode);
}

/*
  Allows setting an rgb lighting color
*/
void set_lighting_color_temporarily(LIGHTING_COLOR color) {
  if (bootComplete == 0) {
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


////////////////////////////////////////////////////////////////
///////////////////////    Tap Dances    ///////////////////////
////////////////////////////////////////////////////////////////

// Tap Dance Declarations
enum {
    TD_KC_FN,
};

/*
  Tap dance handler for catching two taps and triggering the command palette macro or five
  to trigger the game mode default layer.
  Palette refers to a text editor's command dropdown menu (for example VS Code's or Atom's).
*/
void fn_key_tapped(tap_dance_state_t *state, void *user_data) {
  switch (state->count) {
    case 2: // tapping 2 times opens the command palette
      SEND_STRING(OSS_MOD(SS_LSFT("p")));
      reset_tap_dance(state);
      return;
    case 5: // tapping 5 times toggles game mode
      if (can_enable_game_mode) {
        set_single_persistent_default_layer(_GAME_LAYER);
        set_lighting_color_temporarily(LC_BLUE);
      } else {
        set_single_persistent_default_layer(_MAIN_LAYER);
        set_lighting_color_temporarily(LC_RED);
      }
      can_enable_game_mode = !can_enable_game_mode;
      reset_tap_dance(state);
      return;
    default:
      return;
  }
}

// Tap Dances Available
tap_dance_action_t tap_dance_actions[] = {
    // tap dance handler for 'Fn' key presses
    [TD_KC_FN] = ACTION_TAP_DANCE_FN(fn_key_tapped),
};



////////////////////////////////////////////////////////////////
///////////////////////    The Keymap    ///////////////////////
////////////////////////////////////////////////////////////////

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

   /* Main Layer
    * ,--------------------------------------------------------------------------------------------------------.
    * | Esc  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  |   -  | Undo |AppSwt| Redo |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |  ~   |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |  Ins | Home | PgUp |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * | Tab  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |   '  |  Del |  End | PgDn |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |  Ent |  F21 |  Up  |  F22 |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * | Fn*  | Ctrl | Alt  | GUI  |Lower |  Spc |  Spc |Raise |  GUI | Down |  Up  |  Up  | Left | Down |Right |
    * `--------------------------------------------------------------------------------------------------------'
    */
    [_MAIN_LAYER] = LAYOUT_ortho_5x15(
      KC_ESC,          KC_1,     KC_2,     KC_3,     KC_4,  KC_5,   KC_6,   KC_7,  KC_8,     KC_9,         KC_0,        KC_MINS,   MACRO_UNDO, MACRO_APP_SWITCHER, MACRO_REDO,
      KC_GRV,          KC_Q,     KC_W,     KC_E,     KC_R,  KC_T,   KC_Y,   KC_U,  KC_I,     KC_O,         KC_P,        KC_BSPC,   KC_INS,     KC_HOME,            KC_PGUP,
      KC_TAB,          KC_A,     KC_S,     KC_D,     KC_F,  KC_G,   KC_H,   KC_J,  KC_K,     KC_L,         KC_SCLN,     KC_QUOT,   KC_DEL,     KC_END,             KC_PGDN,
      KC_LSFT,         KC_Z,     KC_X,     KC_C,     KC_V,  KC_B,   KC_N,   KC_M,  KC_COMM,  KC_DOT,       KC_SLSH,     KC_ENT,    KC_F21,     KC_UP,              KC_F22,
      TD(TD_KC_FN),    KC_LCTL,  KC_LALT,  KC_LGUI,  LOWER, KC_SPC, KC_SPC, UPPER, KC_RGUI,  MACRO_COPY,   MACRO_PASTE, UTILS,     KC_LEFT,    KC_DOWN,            KC_RGHT
    ),

   /* Utility layer
    * ,--------------------------------------------------------------------------------------------------------.
    * |  Clr |  F13 |  F14 |  F15 |  F16 |  F17 |  F18 |  F19 |  F20 |  F21 |  F22 |  F23 |  F24 |   ▼  | Sleep|
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  | Undo |   ▼  |   ▼  |   ▼  |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |   ▼  |   ▼  | Srch |   ▼  | Find |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  | Redo |   ▼  |   ▼  |   ▼  |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |   ▼  |   ▼  | Cut  | Copy | Paste|   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |Refrsh|UrlCpy|   W  |   ▼  |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   ▼  |   A  |   S  |   D  |
    * `--------------------------------------------------------------------------------------------------------'
    */
    [_UTIL_LAYER] = LAYOUT_ortho_5x15(
      KC_CLR,   KC_F13,   KC_F14,         KC_F15,      KC_F16,      KC_F17,   KC_F18,   KC_F19,   KC_F20,   KC_F21,   KC_F22,  KC_F23,        KC_F24,       _______, KC_SLEP,
      _______,  _______,  _______,        _______,     _______,     _______,  _______,  _______,  _______,  _______,  _______, MACRO_UNDO,    _______,      _______, _______,
      _______,  _______,  KC_WWW_SEARCH,  _______,     KC_FIND,     _______,  _______,  _______,  _______,  _______,  _______, MACRO_REDO,    _______,      _______, _______,
      _______,  _______,  MACRO_CUT,      MACRO_COPY,  MACRO_PASTE, _______,  _______,  _______,  _______,  _______,  _______, MACRO_REFRESH, MACRO_CP_URL, KC_W,    _______,
      _______,  _______,  _______,        _______,     _______,     _______,  _______,  _______,  _______,  _______,  _______, UTILS,         KC_A,         KC_S,    KC_D
    ),

   /* Game Layer
    * ,--------------------------------------------------------------------------------------------------------.
    * | Esc  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  |   -  | Undo |AppSwt| Redo |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * |  ~   |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |  Ins | Home | PgUp |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * | Tab  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |   '  |  Del |  End | PgDn |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |  Ent |  F21 |  Up  |  F22 |
    * |------+------+------+------+------+------+------+------+------+------+------+------+------+------+------|
    * | Shift| Ctrl | Alt  | GUI  |Lower |  Spc |  Spc |Raise |  GUI | Copy | Paste| Util | Left | Down |Right |
    * `--------------------------------------------------------------------------------------------------------'
    */
    [_GAME_LAYER] = LAYOUT_ortho_5x15(
      KC_ESC,        KC_1,     KC_2,     KC_3,     KC_4,  KC_5,   KC_6,   KC_7,  KC_8,     KC_9,         KC_0,        KC_MINS,   MACRO_UNDO, MACRO_APP_SWITCHER, MACRO_REDO,
      KC_GRV,        KC_Q,     KC_W,     KC_E,     KC_R,  KC_T,   KC_Y,   KC_U,  KC_I,     KC_O,         KC_P,        KC_BSPC,   KC_INS,     KC_HOME,    KC_PGUP,
      KC_TAB,        KC_A,     KC_S,     KC_D,     KC_F,  KC_G,   KC_H,   KC_J,  KC_K,     KC_L,         KC_SCLN,     KC_QUOT,   KC_DEL,     KC_END,     KC_PGDN,
      KC_LSFT,       KC_Z,     KC_X,     KC_C,     KC_V,  KC_B,   KC_N,   KC_M,  KC_COMM,  KC_DOT,       KC_SLSH,     KC_ENT,    KC_F21,     KC_UP,      TD(TD_KC_FN),
      KC_LSFT,       KC_LCTL,  KC_LALT,  KC_LGUI,  LOWER, KC_SPC, KC_SPC, UPPER, KC_RGUI,  MACRO_COPY,   MACRO_PASTE, UTILS,     KC_LEFT,    KC_DOWN,    KC_RGHT
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
      QK_BOOT,   _______, _______,  _______,  _______,  _______,  _______,  _______, _______, _______, _______, _______,  _______, _______, _______,
      _______, KC_EXLM, KC_AT,    KC_HASH,  KC_DLR,   KC_PERC,  KC_CIRC,  KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______,  _______, _______, _______,
      _______, KC_F1,   KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE,  _______, _______, _______,
      _______, KC_F7,   KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______, _______, _______, _______, _______,  _______, KC_VOLU, KC_MNXT,
      _______, _______, _______,  _______,  _______,  _______,  _______,  _______, _______, _______, _______, KC_TRNS, KC_MSTP, KC_VOLD, KC_MPLY
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
      QK_BOOT,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______,
      _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,  _______, _______, _______,
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_MINS, KC_PEQL, KC_LBRC, KC_RBRC, KC_BSLS,  _______, _______, _______,
      _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, _______, _______, _______,  _______, KC_VOLU, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_TRNS, KC_MNXT, KC_VOLD, KC_MPLY
    ),

   /* Adjust Layer (Lower + Raise)
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
      _______, _______,       _______, _______,       _______,  _______,  _______, _______,   _______, _______, _______, _______,  DB_TOGG, _______, QK_BOOT,
      _______, BL_STEP,       RGB_VAI, RGB_TOG,       _______,  _______,  RGB_SAI, RGB_SAD,   _______, _______, BL_TOGG, _______,  _______, _______, _______,
      _______, MACRO_DEC_HUE, RGB_VAD, MACRO_INC_HUE, BL_UP,    BL_DOWN,  RGB_MOD, RGB_RMOD,  RGB_SPI, RGB_SPD, BL_BRTG, _______,  _______, _______, _______,
      _______, _______,       _______, _______,       _______,  _______,  _______, _______,   _______, _______, _______, _______,  _______, _______, _______,
      _______, _______,       _______, _______,       _______,  _______,  _______, _______,   _______, _______, _______, KC_TRNS,  _______, _______, _______
    )
};





////////////////////////////////////////////////////////////////
////////////////////      QMK Callbacks    /////////////////////
////////////////////////////////////////////////////////////////


// macro handler
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case MACRO_REFRESH:
      if (record->event.pressed) {
        SEND_STRING(OSS_MOD("r"));
      }
      return false;
      break;
    case MACRO_COPY:
      if (record->event.pressed) {
        SEND_STRING(OSS_MOD("c"));
      }
      return false;
      break;
    case MACRO_PASTE:
      if (record->event.pressed) {
        SEND_STRING(OSS_MOD("v"));
      }
      return false;
      break;
    case MACRO_CUT:
      if (record->event.pressed) {
        SEND_STRING(OSS_MOD("x"));
      }
      return false;
      break;
    case MACRO_PALETTE:
      if (record->event.pressed) {
        SEND_STRING(OSS_MOD("p"));
      }
      return false;
      break;
    case MACRO_REDO:
      if (record->event.pressed) {
        SEND_STRING(OSS_MOD(SS_LSFT("z")));
      }
      return false;
      break;
    case MACRO_UNDO:
      if (record->event.pressed) {
        SEND_STRING(OSS_MOD("z"));
      }
      return false;
      break;
    case MACRO_CP_URL:
      if (record->event.pressed) {
        SEND_STRING(OSS_MOD("l")OSS_MOD("c"));
      }
      return false;
      break;
    case MACRO_INC_HUE:
      if (record->event.pressed) {
        isUserChangingRgb = true;
        set_hsv_from_eeprom();
        rgblight_increase_hue();
      }
      return false;
      break;
    case MACRO_DEC_HUE:
      if (record->event.pressed) {
        isUserChangingRgb = true;
        set_hsv_from_eeprom();
        rgblight_decrease_hue();
      }
      return false;
      break;
    case MACRO_APP_SWITCHER:
      if (record->event.pressed) {
        if (!is_app_switcher_active) {
          is_app_switcher_active = true;
          register_code(OSS_MOD_KC);
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

  // activate the ADJUST layer if the RAISE and LOWER are active
  state = update_tri_layer_state(state, _LOWER_LAYER, _UPPER_LAYER, _ADJUST_LAYER);

  // trigger color change based on layer state
  switch (get_highest_layer(
    state
  )) {
    case _MAIN_LAYER:
    case _GAME_LAYER:
        set_hsv_from_eeprom();
        // turn off the rgb change flag once we get back to the default layer
        isUserChangingRgb = false;
        break;
    case _LOWER_LAYER:
        set_lighting_color_temporarily(LC_AMBER);
        break;
    case _UPPER_LAYER:
        set_lighting_color_temporarily(LC_PURPLE);
        break;
    case _ADJUST_LAYER:
        // Note: if we did't want a specific color for our ADJUST layer it would make
        // things a little easier. We would just use set_hsv_from_eeprom() call below
        // instead of having an if block and isUserChangingRgb state. But since we
        // want this layer to have its own color, we have a couple things to consider:
        // 1) We have to use this layer's color as a baseline (or starting point)
        //    everytime we want to update the Hue Saturation and/or Brightness levels.
        //    This is annoying when you have a nice hue and want to just adjust the brightness,
        //    you will have to scroll through the hues (and satutation values if needed) from the
        //    base layer color again first, then scroll the brightness. Possible way around this
        //    might be a macro (mapped to this layer) that sets the boot-time loaded hsv values.
        // 2) We need to temporarily show the "updated" lighting instead of our layer's base color
        //    while we are switching between hsv values. Hence tracking the isUserChangingRgb state.
        if (isUserChangingRgb) {
          // show the color that is saved/changing instead of fixed layer color
          set_hsv_from_eeprom();
        } else {
          set_lighting_color_temporarily(LC_RED);
        }
        break;
    case _UTIL_LAYER:
        set_lighting_color_temporarily(LC_GREEN);
        break;
    default:
        break;
  }

  // allow three layer acces with the LOWER and UPPER keys
  return state;
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
  if (!bootComplete) {
    checkBootupAnimation();
  }

  // release app switcher check
  if (is_app_switcher_active) {
    if (timer_elapsed(app_switcher_timer) > app_switcher_keep_open_threshold_ms) {
      unregister_code(OSS_MOD_KC);
      is_app_switcher_active = false;
    }
  }
}

