#include "auto_capitalise_i.h"
#include "quantum.h"
#include "send_string.h"

static bool prev_was_space        = false;
static bool i_pressed_after_space = false;

static uint16_t extract_tap_keycode(uint16_t keycode) {
	if (IS_QK_MOD_TAP(keycode)) {
		return QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
	}
	if (IS_QK_LAYER_TAP(keycode)) {
		return QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
	}
	return keycode;
}

bool auto_capitalise_i_process_record(uint16_t keycode, keyrecord_t *record) {
	if (!record->event.pressed) {
		return true;
	}

	const uint16_t base_keycode = extract_tap_keycode(keycode);

	switch (base_keycode) {
		case KC_SPC:
			if (i_pressed_after_space) {
				// We just typed: space -> i -> space
				tap_code(KC_BSPC); // Delete the "i"
				SEND_STRING("I ");
				i_pressed_after_space = false;
				prev_was_space        = true;
				return false;
			}
			prev_was_space        = true;
			i_pressed_after_space = false;
			break;

		case KC_I:
			if (prev_was_space) {
				i_pressed_after_space = true;
				prev_was_space        = false;
			} else {
				i_pressed_after_space = false;
				prev_was_space        = false;
			}
			break;

		default:
			prev_was_space        = false;
			i_pressed_after_space = false;
			break;
	}

	return true;
}
