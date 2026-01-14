// Copyright 2025 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "os_detection.h"

#include "features/auto_capitalise_i.h"
#include "features/auto_camelcase.h"

#pragma region Layers
enum layer_number {
	_QWERTY = 0,
	_SYMBOLS,
	_NAV,
	_MOUSE,
	_GAMING,
	_UNUSED,
};

enum custom_keycodes {
	/* Toggles qwerty layer until ESC or ENTER are pressed - Used for in-game chat */
	CHAT_MODE = QK_KB_0,
	/* Toggles camel case typing mode */
	CAMELCASE,
	MACRO_TILDE,
};

// Home row mods
#define HRM_A LGUI_T(KC_A)
#define HRM_S LALT_T(KC_S)
#define HRM_D LSFT_T(KC_D)
#define HRM_F LCTL_T(KC_F)
#define HRM_J RCTL_T(KC_J)
#define HRM_K RSFT_T(KC_K)
#define HRM_L RALT_T(KC_L)
#define HRM_SCLN RGUI_T(KC_SCLN)

// Layer-tap and mod-tap keys
#define SFT_SPC LSFT_T(KC_SPC)
#define NAV_BSPC LT(_NAV, KC_BSPC)
#define MS_ENT LT(_MOUSE, KC_ENT)
#define SYM MO(_SYMBOLS)

static bool chat_active = false;

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_QWERTY] = LAYOUT(
		   KC_GRV,     KC_1,     KC_2,        KC_3,     KC_4,     KC_5,                                   KC_6,           KC_7,         KC_8,           KC_9,      KC_0,     KC_GRV,
		   KC_TAB,     KC_Q,     KC_W,        KC_E,     KC_R,     KC_T,                                   KC_Y,           KC_U,         KC_I,           KC_O,      KC_P,    KC_MINS,
		  KC_LCTL,    HRM_A,    HRM_S,       HRM_D,    HRM_F,     KC_G,                                   KC_H,          HRM_J,        HRM_K,          HRM_L,  HRM_SCLN,    KC_QUOT,
		  KC_LSFT,     KC_Z,     KC_X,        KC_C,     KC_V,     KC_B,    TD(1),        TD(1),           KC_N,           KC_M,      KC_COMM,         KC_DOT,   KC_SLSH,    KC_RSFT,
		                       KC_MEH,      KC_ESC,      SYM,  SFT_SPC,                                 MS_ENT,       NAV_BSPC,       KC_DEL,         KC_MEH
	),
	[_SYMBOLS] = LAYOUT(
		   KC_F12,    KC_F1,    KC_F2,       KC_F3,    KC_F4,    KC_F5,                                  KC_F6,          KC_F7,        KC_F8,          KC_F9,    KC_F10,     KC_F11,
		  A(KC_D),  KC_QUOT,  KC_DQUO,     KC_CIRC,  KC_BSLS,  KC_TILD,                                KC_LBRC,        KC_LABK,       KC_EQL,        KC_RABK,   KC_RBRC,    KC_PSLS,
		    KC_NO,  KC_EXLM,    KC_AT,     KC_HASH,   KC_DLR,  KC_PERC,                                KC_LCBR,        KC_LPRN,      KC_COLN,        KC_RPRN,   KC_RCBR,    KC_PMNS,
		    KC_NO,    KC_NO,  KC_HOME,     KC_PGUP,  KC_PGDN,   KC_END,    KC_NO,  MACRO_TILDE,        KC_PIPE,        KC_AMPR,      KC_ASTR,        KC_PLUS,   KC_UNDS,     KC_GRV,
		                      _______,     _______,  _______,  _______,                                _______,        _______,      _______,        _______
	),
	[_NAV] = LAYOUT(
		    KC_NO,  A(KC_1),  A(KC_2),     A(KC_3),  A(KC_4),  A(KC_5),                                A(KC_6),        A(KC_7),      A(KC_8),        A(KC_9),     KC_NO,      KC_NO,
		  A(KC_D),    KC_NO,    KC_NO,     A(KC_1),    KC_NO,  A(KC_7),                          RALT(KC_LEFT),  RALT(KC_DOWN),  RALT(KC_UP),  RALT(KC_RGHT),     KC_NO,      KC_NO,
		   KC_MEH,  KC_LGUI,  KC_LALT,     KC_LSFT,  KC_LCTL,    KC_NO,                                KC_LEFT,        KC_DOWN,        KC_UP,        KC_RGHT,     KC_NO,      KC_NO,
		    KC_NO,    KC_NO,    KC_NO,       KC_NO,    KC_NO,  A(KC_2),    KC_NO,        KC_NO,     C(KC_LEFT),     C(KC_DOWN),     C(KC_UP),     C(KC_RGHT),     KC_NO,      KC_NO,
		                      _______,     _______,  _______,  _______,                                _______,        _______,      _______,        _______
	),
	[_MOUSE] = LAYOUT(
		    KC_NO,    KC_NO,    KC_NO,       KC_NO,    KC_NO,  KC_HOME,                                KC_HOME,          KC_NO,        KC_NO,          KC_NO,     KC_NO,      KC_NO,
		    KC_NO,    KC_NO,    KC_NO,     KC_WH_U,    KC_NO,  KC_PGUP,                                KC_PGUP,        KC_BTN1,      KC_MS_U,        KC_BTN2,     KC_NO,      KC_NO,
		    KC_NO,    KC_NO,  KC_WH_L,     KC_WH_D,  KC_WH_R,  KC_PGDN,                                KC_PGDN,        KC_MS_L,      KC_MS_D,        KC_MS_R,   KC_BTN1,      KC_NO,
		    KC_NO,    KC_NO,    KC_NO,       KC_NO,    KC_NO,   KC_END,  QK_LLCK,      QK_LLCK,         KC_END,        KC_BTN4,      KC_BTN3,        KC_BTN5,     KC_NO,      KC_NO,
		                      _______,     _______,  _______,  _______,                                _______,        _______,      _______,    TG(_GAMING)
	),
	[_GAMING] = LAYOUT(
		   KC_ESC,     KC_1,     KC_2,        KC_3,     KC_4,   KC_GRV,                                   KC_I,           KC_E,         KC_C,           KC_S,      KC_K,    KC_HOME,
		   KC_TAB,     KC_6,     KC_Q,        KC_W,     KC_E,     KC_R,                                  KC_NO,        KC_LALT,        KC_UP,          KC_NO,     KC_NO,    KC_MINS,
		  KC_BSPC,  KC_LSFT,     KC_A,        KC_S,     KC_D,     KC_F,                                  KC_NO,        KC_LEFT,      KC_DOWN,        KC_RGHT,      KC_F,      KC_NO,
		     KC_5,  KC_LCTL,     KC_Z,        KC_X,     KC_C,     KC_V,     KC_G,       KC_EQL,          KC_NO,           KC_M,        KC_NO,          KC_NO,     KC_NO,  CHAT_MODE,
		                       KC_TAB,  RCS(KC_NO),  KC_LALT,   KC_SPC,                                 KC_SPC,        KC_LSFT,         KC_Z,    TG(_GAMING)
	),
	[_UNUSED] = LAYOUT(
		  _______,  _______,  _______,     _______,  _______,  _______,                                _______,        _______,      _______,        _______,   _______,    _______,
		  _______,  _______,  _______,     _______,  _______,  _______,                                _______,        _______,      _______,        _______,   _______,    _______,
		  _______,  _______,  _______,     _______,  _______,  _______,                                _______,        _______,      _______,        _______,   _______,    _______,
		  _______,  _______,  _______,     _______,  _______,  _______,  _______,      _______,        _______,        _______,      _______,        _______,   _______,    _______,
		                      _______,     _______,  _______,  _______,                                _______,        _______,      _______,        _______
	),
};
// clang-format on

// clang-format off
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT(
        '*', '*', '*', '*', '*', '*',             '*', '*', '*', '*', '*', '*',
        '*', 'L', 'L', 'L', 'L', 'L',             'R', 'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L', 'L',             'R', 'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L', 'L', '*',   '*', 'R', 'R', 'R', 'R', 'R', 'R',
                       '*', '*', '*', '*',   '*', '*', '*', '*'
);
// clang-format on

#pragma endregion

#pragma region Process_Record

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	const bool isGaming = (get_highest_layer(layer_state | default_layer_state) == _GAMING);

	// Do not run auto capitalise or auto camel case when the gaming layer is active.
	if (!isGaming) {
		if (!auto_capitalise_i_process_record(keycode, record)) return false;
		if (!auto_camelcase_process_record(keycode, record)) return false;
	}

	if (chat_active && record->event.pressed) {
		if (
		    // Regular key presses
		    (keycode == KC_ESC || keycode == KC_ENT)) {
			chat_active = false;
			layer_clear();
			layer_on(_GAMING);
			return true;
		} else if (keycode == MS_ENT && record->tap.count > 0) {
			// Layer toggle key presses
			chat_active = false;
			layer_clear();
			layer_on(_GAMING);
			tap_code(KC_ENT); // Send Enter keycode, else it will send gaming layer key (probably space)
			return false;
		}
	}

	switch (keycode) {
		case CHAT_MODE:
			if (record->event.pressed) {
				chat_active = true;
				layer_off(_GAMING);
				layer_on(_QWERTY);
			}
			return false;
		case CAMELCASE:
			if (record->event.pressed) {
				auto_camelcase_toggle();
			}
			return false;
		case MACRO_TILDE:
			if (record->event.pressed) {
				SEND_STRING("~/");
			}
			return false;
	}

	return true;
}

#pragma endregion

#pragma region OS_detection

bool process_detected_host_os_kb(os_variant_t detected_os) {
	switch (detected_os) {
		case OS_MACOS:
			keymap_config.swap_lctl_lgui = true;
			keymap_config.swap_rctl_rgui = true;
			break;
		case OS_WINDOWS:
		case OS_LINUX:
		default:
			keymap_config.swap_lctl_lgui = false;
			keymap_config.swap_rctl_rgui = false;
			break;
	}
	return true;
}

#pragma endregion

#pragma region OLED
#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
	// Vertical orientation for both screens.
	return OLED_ROTATION_270;
}

void render_space(void) {
	oled_write_P(PSTR("     "), false);
}

void render_mod_status_gui_alt(uint8_t modifiers) {
	static const char PROGMEM gui_off_1[] = {0x80, 0x81, 0};
	static const char PROGMEM gui_off_2[] = {0xa0, 0xa1, 0};
	static const char PROGMEM gui_on_1[]  = {0x88, 0x89, 0};
	static const char PROGMEM gui_on_2[]  = {0xa8, 0xa9, 0};

	static const char PROGMEM alt_off_1[] = {0x82, 0x83, 0};
	static const char PROGMEM alt_off_2[] = {0xa2, 0xa3, 0};
	static const char PROGMEM alt_on_1[]  = {0x8a, 0x8b, 0};
	static const char PROGMEM alt_on_2[]  = {0xaa, 0xab, 0};

	// fillers between the modifier icons bleed into the icon frames
	static const char PROGMEM off_off_1[] = {0xc0, 0};
	static const char PROGMEM off_off_2[] = {0xc1, 0};
	static const char PROGMEM on_off_1[]  = {0xc2, 0};
	static const char PROGMEM on_off_2[]  = {0xc3, 0};
	static const char PROGMEM off_on_1[]  = {0xc4, 0};
	static const char PROGMEM off_on_2[]  = {0xc5, 0};
	static const char PROGMEM on_on_1[]   = {0xc6, 0};
	static const char PROGMEM on_on_2[]   = {0xc7, 0};

	if (modifiers & MOD_MASK_GUI) {
		oled_write_P(gui_on_1, false);
	} else {
		oled_write_P(gui_off_1, false);
	}

	if ((modifiers & MOD_MASK_GUI) && (modifiers & MOD_MASK_ALT)) {
		oled_write_P(on_on_1, false);
	} else if (modifiers & MOD_MASK_GUI) {
		oled_write_P(on_off_1, false);
	} else if (modifiers & MOD_MASK_ALT) {
		oled_write_P(off_on_1, false);
	} else {
		oled_write_P(off_off_1, false);
	}

	if (modifiers & MOD_MASK_ALT) {
		oled_write_P(alt_on_1, false);
	} else {
		oled_write_P(alt_off_1, false);
	}

	if (modifiers & MOD_MASK_GUI) {
		oled_write_P(gui_on_2, false);
	} else {
		oled_write_P(gui_off_2, false);
	}

	if ((modifiers & MOD_MASK_GUI) && (modifiers & MOD_MASK_ALT)) {
		oled_write_P(on_on_2, false);
	} else if (modifiers & MOD_MASK_GUI) {
		oled_write_P(on_off_2, false);
	} else if (modifiers & MOD_MASK_ALT) {
		oled_write_P(off_on_2, false);
	} else {
		oled_write_P(off_off_2, false);
	}

	if (modifiers & MOD_MASK_ALT) {
		oled_write_P(alt_on_2, false);
	} else {
		oled_write_P(alt_off_2, false);
	}
}

void render_mod_status_ctrl_shift(uint8_t modifiers) {
	static const char PROGMEM ctrl_off_1[] = {0x84, 0x85, 0};
	static const char PROGMEM ctrl_off_2[] = {0xa4, 0xa5, 0};
	static const char PROGMEM ctrl_on_1[]  = {0x8c, 0x8d, 0};
	static const char PROGMEM ctrl_on_2[]  = {0xac, 0xad, 0};

	static const char PROGMEM shift_off_1[] = {0x86, 0x87, 0};
	static const char PROGMEM shift_off_2[] = {0xa6, 0xa7, 0};
	static const char PROGMEM shift_on_1[]  = {0xc8, 0xc9, 0};
	static const char PROGMEM shift_on_2[]  = {0xca, 0xcb, 0};

	// fillers between the modifier icons bleed into the icon frames
	static const char PROGMEM off_off_1[] = {0xc0, 0};
	static const char PROGMEM off_off_2[] = {0xc1, 0};
	static const char PROGMEM on_off_1[]  = {0xc2, 0};
	static const char PROGMEM on_off_2[]  = {0xc3, 0};
	static const char PROGMEM off_on_1[]  = {0xc4, 0};
	static const char PROGMEM off_on_2[]  = {0xc5, 0};
	static const char PROGMEM on_on_1[]   = {0xc6, 0};
	static const char PROGMEM on_on_2[]   = {0xc7, 0};

	if (modifiers & MOD_MASK_CTRL) {
		oled_write_P(ctrl_on_1, false);
	} else {
		oled_write_P(ctrl_off_1, false);
	}

	if ((modifiers & MOD_MASK_CTRL) && (modifiers & MOD_MASK_SHIFT)) {
		oled_write_P(on_on_1, false);
	} else if (modifiers & MOD_MASK_CTRL) {
		oled_write_P(on_off_1, false);
	} else if (modifiers & MOD_MASK_SHIFT) {
		oled_write_P(off_on_1, false);
	} else {
		oled_write_P(off_off_1, false);
	}

	if (modifiers & MOD_MASK_SHIFT) {
		oled_write_P(shift_on_1, false);
	} else {
		oled_write_P(shift_off_1, false);
	}

	if (modifiers & MOD_MASK_CTRL) {
		oled_write_P(ctrl_on_2, false);
	} else {
		oled_write_P(ctrl_off_2, false);
	}

	if ((modifiers & MOD_MASK_CTRL) && (modifiers & MOD_MASK_SHIFT)) {
		oled_write_P(on_on_2, false);
	} else if (modifiers & MOD_MASK_CTRL) {
		oled_write_P(on_off_2, false);
	} else if (modifiers & MOD_MASK_SHIFT) {
		oled_write_P(off_on_2, false);
	} else {
		oled_write_P(off_off_2, false);
	}

	if (modifiers & MOD_MASK_SHIFT) {
		oled_write_P(shift_on_2, false);
	} else {
		oled_write_P(shift_off_2, false);
	}
}

void render_paw(void) {
	static const char PROGMEM paw[] = {0x8e, 0x8f, 0x90, 0x91, 0x92, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0};
	oled_write_P(paw, false);
}

void render_qr(void) {
	static const char PROGMEM qr[] = {
	    0x93, 0x94, 0x95, 0x96, 0x97, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0,
	};
	oled_write_P(qr, false);
}

void render_layer_state(void) {
	oled_write_P(PSTR("Layer"), false);
	// Be aware that length of layer captions should be always 5 characters, otherwise the layout will look eerie.
	switch (get_highest_layer(layer_state | default_layer_state)) {
		case _QWERTY:
			oled_write_P(PSTR("QWERT"), false);
			break;
		case _SYMBOLS:
			oled_write_P(PSTR("SYM  "), false);
			break;
		case _NAV:
			oled_write_P(PSTR("NAV  "), false);
			break;
		case _MOUSE:
			oled_write_P(PSTR("MOUSE"), false);
			break;
		case _GAMING:
			oled_write_P(PSTR("GAME "), false);
			break;
		case _UNUSED:
			oled_write_P(PSTR("WIP  "), false);
			break;
		default:
			oled_write_P(PSTR("?????"), false);
			break;
	}
}

void render_camelcase_status(void) {
	if (auto_camelcase_is_active()) {
		oled_write_P(PSTR("CAMEL"), false);
	} else {
		oled_write_P(PSTR("     "), false);
	}
}

bool oled_task_user(void) {
	if (is_keyboard_master()) {
		render_paw();
		render_space();
		render_space();
		render_layer_state();
		render_space();
		render_camelcase_status();
		render_space();
		render_mod_status_gui_alt(get_mods() | get_oneshot_mods());
		render_mod_status_ctrl_shift(get_mods() | get_oneshot_mods());
	} else {
		render_paw();
		render_space();
		render_space();
		render_qr();
	}

	return false;
}

#endif // OLED_ENABLE

#pragma endregion
