#include QMK_KEYBOARD_H

// ----- global variables -----
bool is_alt_tab_active = false;
bool is_alt_shift_tab_active = false;
uint16_t alt_tab_timer = 0;

#define TAPPING_TOGGLE 1

#define _QWERTY 0
#define _SYMB_ARW 1
#define _NUM_MOUSE 2
#define _MACROS 3

#define KC_COPY LCTL(KC_INS)
#define KC_PASTE LSFT(KC_INS)

// browser tabs
#define P_TAB LSFT(LCTL(KC_TAB))
#define N_TAB LCTL(KC_TAB)

// workspaces
#define P_WRKS LCA(KC_LEFT)
#define N_WRKS LCA(KC_RGHT)

// windows
#define N_WIN LSA(KC_TAB)

// apps
#define TMUX LCTL(KC_A)

#define CTL_ESC LCTL_T(KC_ESC)
#define SFT_ENT LSFT_T(KC_ENT)

enum custom_keycodes {
    ALT_TAB = SAFE_RANGE,
    SALT_TAB,
};
// ----- global variables -----

// ----- functions -----
#define AH_STATIC_LIGHT(COLOR)                        \
  rgblight_enable_noeeprom();                         \
  rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT); \
  rgblight_sethsv_noeeprom(COLOR);

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case ALT_TAB:
      if (record->event.pressed) {
        if (!is_alt_tab_active) {
          if (is_alt_shift_tab_active) {
            unregister_code(KC_LSFT);
            is_alt_shift_tab_active = false;
          }

          is_alt_tab_active = true;
          register_code(KC_LALT);
        }
        alt_tab_timer = timer_read();
        register_code(KC_TAB);
      } else {
        unregister_code(KC_TAB);
      }
      break;
    case SALT_TAB:
      if (record->event.pressed) {
        if (!is_alt_shift_tab_active) {
          if (is_alt_tab_active) {
            is_alt_tab_active = false;
          }
          is_alt_shift_tab_active = true;
          register_code(KC_LALT);
          register_code(KC_LSFT);
        }
        alt_tab_timer = timer_read();
        register_code(KC_TAB);
      } else {
        unregister_code(KC_TAB);
      }
      break;
  }
  return true;
}

void matrix_scan_user(void) {
  if (is_alt_tab_active && timer_elapsed(alt_tab_timer) > 250) {
    unregister_code(KC_LALT);
    is_alt_tab_active = false;
  }
  if (is_alt_shift_tab_active && timer_elapsed(alt_tab_timer) > 250) {
    unregister_code(KC_LALT);
    unregister_code(KC_LSFT);
    is_alt_shift_tab_active = false;
  }
}

// ----- end functions -----

// ----- rgb configuration -----
#undef RGBLED_NUM
#define RGBLIGHT_ANIMATIONS
#define RGBLED_NUM 14

#ifdef RGBLIGHT_ENABLE
// Change LED colors depending on the layer.
uint32_t layer_state_set_user(uint32_t state) {
  switch (biton32(state)) {
    case _QWERTY:
      AH_STATIC_LIGHT(HSV_GREEN);
      break;
    case _SYMB_ARW:
      AH_STATIC_LIGHT(HSV_ORANGE);
      break;
    case _NUM_MOUSE:
      AH_STATIC_LIGHT(HSV_BLUE);
      break;
    case _MACROS:
      AH_STATIC_LIGHT(HSV_MAGENTA);
      break;
    default:
      rgblight_disable_noeeprom();
      break;
  }
  return state;
};
#endif
// ----- not rgb configuration -----

// ----- tap dance -----
enum {
    BS_CBS,
};

qk_tap_dance_action_t tap_dance_actions[] = {
    [BS_CBS] = ACTION_TAP_DANCE_DOUBLE(KC_BSPC, LCTL(KC_BSPC)),
};

// ----- end tap dance -----
// ----- mouse -----

#define MK_3_SPEED
#define MOUSEKEY_INTERVAL 16
#define MOUSEKEY_DELAY 0
#define MOUSEKEY_TIME_TO_MAX 60
#define MOUSEKEY_MAX_SPEED 7
#define MOUSEKEY_WHEEL_DELAY 0

// ----- end mouse -----


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_QWERTY] = LAYOUT(
 //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                                KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
 //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    P_WRKS,                            N_WRKS,  KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_EQL,
 //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
    TMUX,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    SALT_TAB,                          ALT_TAB, KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
 //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_NO,   KC_DEL,          KC_F12,  N_WIN,   KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
 //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
    KC_LGUI, KC_WBAK, KC_WFWD, KC_LALT,   MO(_SYMB_ARW),CTL_ESC,LALT_T(KC_BSPC),    KC_ENT,   KC_SPC,   MO(_NUM_MOUSE),  KC_RALT, KC_LBRC, KC_RBRC, MO(3)
 //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

	[_NUM_MOUSE] = LAYOUT(
 //┌────────┬────────┬────────┬────────┬────────┬────────┐                                            ┌────────┬────────┬────────┬────────┬────────┬────────┐
    KC_NO,   RGB_SPI, RGB_SPD, RGB_HUD, RGB_HUI, RGB_MOD,                                              KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,
 //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                          ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
    KC_NO,   KC_NO,   KC_P7,   KC_P8,   KC_P9,   KC_NO,   RGB_RMOD,                           KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_F12,
 //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
    KC_NO,   KC_NO,   KC_P4,   KC_P5,   KC_P6,   KC_P0,   KC_COPY,                            KC_PASTE,KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_NO,   KC_NO,
 //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
    KC_LSFT, KC_NO,   KC_P1,   KC_P2,   KC_P3,   KC_NO,   KC_NO,   LCTL(KC_DEL),     KC_NO,   KC_NO,   KC_HOME,KC_PGDOWN,KC_PGUP, KC_END,  KC_NO,   KC_RSFT,
 //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤        ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
    KC_LGUI, KC_ACL0, KC_ACL1, KC_ACL2,      KC_NO,       KC_LCTL, KC_LALT,          KC_LCTL, KC_LALT,     KC_LSFT,      KC_NO,   KC_NO,   KC_NO,   KC_NO
 //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘        └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

	[_SYMB_ARW] = LAYOUT(
 //┌────────┬────────┬────────┬────────┬────────┬────────┐                                            ┌────────┬────────┬────────┬────────┬────────┬────────┐
    KC_NO,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                                KC_MUTE, KC_VOLU, KC_VOLD, KC_BRIU, KC_BRID, KC_NO,
 //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                          ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
 //├        ┼   !    ┼   @    ┼   [/{  ┼  ]/}   ┼  \|   ┼        ┐
    KC_NO,   KC_EXLM, KC_AT,   KC_LBRC, KC_RBRC, KC_BSLS, KC_NO,                              KC_NO,   RGB_TOG, RGB_VAI, RGB_VAD, RGB_SAI, RGB_SAD, KC_NO,
 //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
 //├        ┼   #    ┼   $    ┼   _-   ┼   +=   ┼   `    ┼        ┐
    KC_NO,   KC_HASH, KC_DLR,  KC_MINS, KC_EQL, KC_GRV,  KC_NO,                               KC_NO,   KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, KC_NO,   KC_NO,
 //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
 //├        ┼   %    ┼   ^    ┼   (    ┼   )    ┼   ~    ┼        ┐
    KC_NO,   KC_PERC, KC_CIRC, KC_LPRN, KC_RPRN, KC_TILD, KC_NO,   LCTL(KC_DEL),     KC_NO,   KC_NO,   KC_NO, KC_MS_BTN1,KC_MS_BTN2, KC_NO,   KC_NO,   KC_NO,
 //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤        ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
    KC_LGUI, KC_NO,   KC_NO,   KC_NO,        KC_NO,       KC_NO,   LCTL(KC_BSPC),    KC_LSFT, KC_NO,       KC_LSFT,      KC_NO,   KC_NO,   KC_NO,   KC_NO
 //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘        └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    ),

	[_MACROS] = LAYOUT(
 //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                                               KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
 //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
    KC_NO,   KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_NO,   KC_NO,                             KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
 //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
    KC_NO,   KC_F17,  KC_F17,  KC_F18,  KC_F20,  KC_NO,   KC_NO,                             KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
 //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
    KC_NO,   KC_F21,  KC_F22,  KC_F23,  KC_F24,  KC_NO,   KC_NO,   KC_NO,           KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
 //├────────┼────────┼────────┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
    KC_LGUI, KC_NO,   KC_NO,   KC_NO,        KC_NO,       KC_NO,   KC_NO,           KC_NO,   KC_NO,       KC_NO,        KC_NO,   KC_NO,   KC_NO,   KC_TRNS
 //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
    )};
