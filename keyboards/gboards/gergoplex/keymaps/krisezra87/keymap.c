/* Good on you for modifying your layout! if you don't have
 * time to read the QMK docs, a list of keycodes can be found at
 * https://github.com/qmk/qmk_firmware/blob/master/docs/keycodes.md
 */

#include "gergoplex.h"

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

enum {
    _ALPHA,     // default (Colemak DHm)
    _SPECIAL,   // special characters
    _NUMBERS,   // numbers/function/motion
    T_CTL
};

td_state_t cur_dance(qk_tap_dance_state_t *state);

// For the x tap dance. Put it here so it can be used in any keymap
void t_finished(qk_tap_dance_state_t *state, void *user_data);
void t_reset(qk_tap_dance_state_t *state, void *user_data);

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
     *               |SPECIAL|ESC |BSPC|      |SPC|ENT META|TAB|
     *               '-----------------'      '----------------'
     */
    [_ALPHA] = LAYOUT_split_3x5_3(
         KC_Q        , KC_W      , KC_F        , KC_P        , KC_B      , /**/ KC_J, KC_L        , KC_U        , KC_Y  , KC_SCLN,
         KC_A        , KC_R      , LCTL_T(KC_S), LALT_T(KC_T), KC_G      , /**/ KC_M, RALT_T(KC_N), RCTL_T(KC_E), KC_I  , KC_O,
         LSFT_T(KC_Z), LT(0,KC_X), LT(0,KC_C)  , KC_D        , LT(0,KC_V), /**/ KC_K, KC_H        , KC_COMMA    , KC_DOT, RSFT_T(KC_SLSH),
                                           OSL(_SPECIAL), KC_ESC, KC_BSPC, /**/ KC_SPC, LGUI_T(KC_ENT), KC_ESC),


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
     *             |NUMBERS| ESC |     |      |     |     |     |
     *             '-------------------'      '-----------------'
     */

    [_SPECIAL] = LAYOUT_split_3x5_3(
         LT(_SPECIAL,S(KC_EXLM)), KC_AT  , KC_HASH   , KC_DLR , KC_PERC, /**/ KC_CIRC, KC_AMPR     , KC_ASTR     , LT(_SPECIAL,KC_MINS), KC_BSLS,
         KC_TAB                 , KC_TRNS, KC_LCTL   , KC_LALT, KC_DEL , /**/ KC_H   , RALT_T(KC_J), RCTL_T(KC_K), KC_L   , KC_QUOT,
         LSFT_T(KC_GRV)         , KC_TRNS, S(KC_LBRC), KC_LPRN, KC_LBRC, /**/ KC_RBRC, KC_RPRN     , S(KC_RBRC)  , KC_MINS, RSFT_T(KC_EQL),
                                      TO(_NUMBERS), TO(_ALPHA), KC_TRNS, /**/ KC_TRNS, KC_TRNS, KC_TRNS),

    /* Keymap 2: Numbers/Motion layer
     * NEED TAPDANCE TO ADD ALT TO NUMBERS ON HOLD
     * FIX UP LEFT HAND
     * ,-------------------------------.      ,---------------------------------------.
     * |   1   |  2  |  3  |  4  |  5  |      |  6  |    7    |    8    |  9  |   0   |
     * |-------+-----+-----+-----+-----|      |-----+---------+---------+-----+-------|
     * |       | LEFT|CTL D|ALT R| UP  |      | H   | ALT J   | CTRL K  | L   |       |
     * |-------+-----+-----+-----+-----|      |-----+---------+---------+-----+-------|
     * |       | RMB | MMB | LMB |     |      | MLFT|  MDWN   |  MUP    | MRGT|       |
     * `-------------------------------'      `---------------------------------------'
     *            .--------------------.      .-----------------.
     *            |SPECIAL|ALPHA|      |      |     |     |     |
     *            '--------------------'      '-----------------'
     */
    [_NUMBERS] = LAYOUT_split_3x5_3(
         KC_1   , TD(T_CTL)   , KC_3           , KC_4         , KC_5    , /**/ KC_6   , KC_7        , KC_8        , KC_9   , KC_0   ,
         KC_TRNS, KC_LEFT, LCTL_T(KC_DOWN), LALT_T(KC_UP), KC_RIGHT, /**/ KC_H   , RALT_T(KC_J), RCTL_T(KC_K), KC_L   , KC_TRNS,
         KC_TRNS, KC_BTN2, KC_BTN3        , KC_BTN1      , KC_TRNS , /**/ KC_MS_L, KC_MS_D     , KC_MS_U     , KC_MS_R, KC_TRNS,
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
        case LT(_SPECIAL,S(KC_EXLM)):
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

td_state_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }

    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TD_TRIPLE_SINGLE_TAP', and define that enum just like 'TD_DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TD_TRIPLE_TAP;
        else return TD_TRIPLE_HOLD;
    } else return TD_UNKNOWN;
}

// Create an instance of 'td_tap_t' for the 'x' tap dance.
static td_tap_t ttap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void t_finished(qk_tap_dance_state_t *state, void *user_data) {
    ttap_state.state = cur_dance(state);
    switch (ttap_state.state) {
        case TD_SINGLE_TAP: register_code(KC_2); break;
        case TD_SINGLE_HOLD: register_code16(LALT(KC_2)); break;
        case TD_DOUBLE_TAP: tap_code(KC_2); register_code(KC_2); break;
        case TD_DOUBLE_HOLD: register_code16(LSA(KC_2)); break;
        // Last case is for fast typing. Assuming your key is `f`:
        // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
        // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
        case TD_DOUBLE_SINGLE_TAP: tap_code(KC_2); register_code(KC_2); break;
        default: break;
    }
}

void t_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (ttap_state.state) {
        case TD_SINGLE_TAP: unregister_code(KC_2); break;
        case TD_SINGLE_HOLD: unregister_code16(LALT(KC_2)); break;
        case TD_DOUBLE_TAP: unregister_code(KC_2); break;
        case TD_DOUBLE_HOLD: unregister_code16(LSA(KC_2)); break;
        case TD_DOUBLE_SINGLE_TAP: unregister_code(KC_2); break;
        default: break;
    }
    ttap_state.state = TD_NONE;
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [T_CTL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, t_finished, t_reset)
};
