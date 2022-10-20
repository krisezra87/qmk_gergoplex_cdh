/* Good on you for modifying your layout! if you don't have
 * time to read the QMK docs, a list of keycodes can be found at
 * https://github.com/qmk/qmk_firmware/blob/master/docs/keycodes.md
 */

#include "gergoplex.h"
#include "g/keymap_combo.h"

enum {
    _ALPHA,     // default (Colemak DHm)
    _SPECIAL,   // special characters
    _NUMBERS    // numbers/function/motion
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Keymap 0: Alpha layer / Colemak DHm
     *
     * ,-------------------------------.      ,-------------------------------.
     * |   Q   |  W  |  F  |  P  |  B  |      |  J  |  L  |  U  |  Y  |   ;   |
     * |-------+-----+-----+-----+-----|      |-----+-----+-----+-----+-------|
     * |   A   |  R  |CTL S|ALT T|  G  |      |  M  |ALT N|CTL E|  I  |   O   |
     * |-------+-----+-----+-----+-----|      |-----+-----+-----+-----+-------|
     * | SHFT Z|  X  |  C  |  D  |  V  |      |  K  |  H  |  <  |  >  |SHFT / |
     * `-------------------------------'      `-------------------------------'
     *               .-----------------.      .----------------.
     *               |SPECIAL|SHFT|BSPC|      |SPC|ENT META|TAB|
     *               '-----------------'      '----------------'
     *   TODO: TAPDANCE on AA?
     */
    [_ALPHA] = LAYOUT_split_3x5_3(
         KC_Q        , KC_W      , KC_F        , KC_P        , KC_B      , /**/ KC_J, KC_L        , KC_U        , KC_Y  , KC_SCLN,
         KC_A        , KC_R      , LCTL_T(KC_S), LALT_T(KC_T), KC_G      , /**/ KC_M, RALT_T(KC_N), RCTL_T(KC_E), KC_I  , KC_O,
         LSFT_T(KC_Z), LT(0,KC_X), LT(0,KC_C)  , KC_D        , LT(0,KC_V), /**/ KC_K, KC_H        , KC_COMMA    , KC_DOT, RSFT_T(KC_SLSH),
                                     TO(_SPECIAL), OSM(MOD_LSFT), KC_BSPC, /**/ KC_SPC, LGUI_T(KC_ENT), KC_ESC),

    /* TODO practice using dedicated shift */

    /* Keymap 1: Special characters layer
     *
     * ,-------------------------------.      ,-------------------------------.
     * |  ` !  |  @  |  #  |  $  |  %  |      |  ^  |  &  |  *  | - = |   \   |
     * |-------+-----+-----+-----+-----|      |-----+-----+-----+-----+-------|
     * |  TAB  |     | LCTL| LALT| DEL |      |  H  |  J  |  K  |  L  |   '   |
     * |-------+-----+-----+-----+-----|      |-----+-----+-----+-----+-------|
     * | SHFT `|     |  {  |  (  |  [  |      |  ]  |  )  |  }  |  -  | SHFT =|
     * `-------------------------------'      `-------------------------------'
     *             .-------------------.      .-----------------.
     *             |NUMBERS|ALPHA|     |      |     |     |     |
     *             '-------------------'      '-----------------'
     */

    [_SPECIAL] = LAYOUT_split_3x5_3(
         LT(_SPECIAL,S(KC_EXLM)), KC_AT  , KC_HASH   , KC_DLR , KC_PERC, /**/ KC_CIRC, KC_AMPR     , KC_ASTR     , LT(_SPECIAL,KC_MINS), KC_BSLS,
         KC_TAB                 , KC_TRNS, KC_LCTL   , KC_LALT, KC_DEL , /**/ KC_H   , RALT_T(KC_J), RCTL_T(KC_K), KC_L   , KC_QUOT,
         LSFT_T(KC_GRV)         , KC_TRNS, S(KC_LBRC), KC_LPRN, KC_LBRC, /**/ KC_RBRC, KC_RPRN     , S(KC_RBRC)  , KC_MINS, RSFT_T(KC_EQL),
                                      TO(_NUMBERS), TO(_ALPHA), KC_TRNS, /**/ KC_TRNS, KC_TRNS, KC_TRNS),


    /* Keymap 2: Numbers/Motion layer
     * NEED TAPDANCE TO ADD ALT TO NUMBERS ON HOLD
     *
     * ,-------------------------------.      ,---------------------------------------.
     * |   1   |  2  |  3  |  4  |  5  |      |  6  |    7    |    8    |  9  |   0   |
     * |-------+-----+-----+-----+-----|      |-----+---------+---------+-----+-------|
     * |       |     |LCTL |LALT |     |      | LFT | ALT DWN | CTRL UP | RGT |       |
     * |-------+-----+-----+-----+-----|      |-----+---------+---------+-----+-------|
     * |       | RMB | MMB | LMB |     |      | MLFT|  MDWN   |  MUP    | MRGT|       |
     * `-------------------------------'      `---------------------------------------'
     *            .--------------------.      .-----------------.
     *            |SPECIAL|ALPHA|      |      |     |     |     |
     *            '--------------------'      '-----------------'
     */
    [_NUMBERS] = LAYOUT_split_3x5_3(
         KC_1   , KC_2   , KC_3   , KC_4   , KC_5   , /**/ KC_6   , KC_7           , KC_8         , KC_9   , KC_0   ,
         KC_TRNS, KC_TRNS, KC_LCTL, KC_LALT, KC_TRNS, /**/ KC_LEFT, RALT_T(KC_DOWN), RALT_T(KC_UP), KC_RGHT, KC_TRNS,
         KC_TRNS, KC_BTN2, KC_BTN3, KC_BTN1, KC_TRNS, /**/ KC_MS_L, KC_MS_D        , KC_MS_U      , KC_MS_R, KC_TRNS,
                   TO(_SPECIAL), TO(_ALPHA), KC_TRNS, /**/ KC_TRNS, KC_TRNS, KC_TRNS)
};

/* Abuse the layer trigger function to sort out hold vs tap for some keys */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(0,KC_X):
            if (!record->tap.count && record->event.pressed) {
                tap_code16(C(KC_X)); // Intercept hold function to send Ctrl-X
                return false;
            }
            return true;             // Return true for normal processing of tap keycode
        case LT(0,KC_C):
            if (!record->tap.count && record->event.pressed) {
                tap_code16(C(KC_C)); // Intercept hold function to send Ctrl-C
                return false;
            }
            return true;             // Return true for normal processing of tap keycode
        case LT(0,KC_V):
            if (!record->tap.count && record->event.pressed) {
                tap_code16(C(KC_V)); // Intercept hold function to send Ctrl-V
                return false;
            }
            return true;             // Return true for normal processing of tap keycode
        case LT(_SPECIAL,KC_MINS):
            if (record->tap.count && record->event.pressed) {
                return true; // Return true for normal processing of tap keycode
                break;
            } else if (record->event.pressed) {
                tap_code16(KC_EQUAL); // Intercept hold function to send +
                return false;
            }
        case LT(_SPECIAL,KC_EXLM):
            if (record->tap.count && record->event.pressed) {
                return true; // Return true for normal processing of tap keycode
                break;
            } else if (record->event.pressed) {
                tap_code16(KC_GRV); // Intercept hold function to send `
                return false;
            }
    }
    return true;
}
