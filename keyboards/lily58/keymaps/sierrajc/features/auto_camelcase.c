#include "auto_camelcase.h"
#include "quantum.h"

static bool camel_case_active = false;
static bool space_pressed     = false;

static uint16_t extract_tap_keycode(uint16_t keycode) {
	if (IS_QK_MOD_TAP(keycode)) {
		return QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
	}
	if (IS_QK_LAYER_TAP(keycode)) {
		return QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
	}
	return keycode;
}

static bool is_letter_key(uint16_t keycode) {
	const uint16_t base = extract_tap_keycode(keycode);
	return base >= KC_A && base <= KC_Z;
}

void auto_camelcase_toggle(void) {
	camel_case_active = !camel_case_active;
	space_pressed     = false;
}

bool auto_camelcase_is_active(void) {
	return camel_case_active;
}

bool auto_camelcase_process_record(uint16_t keycode, keyrecord_t *record) {
	if (!record->event.pressed || !camel_case_active) {
		return true;
	}

	const uint16_t base_keycode = extract_tap_keycode(keycode);

	if (base_keycode == KC_ESC || base_keycode == KC_ENT || base_keycode == KC_TAB) {
		camel_case_active = false;
		space_pressed     = false;
		return true;
	}

	if (base_keycode == KC_SPC) {
		space_pressed = true;
		return true;
	}

	const bool is_letter = is_letter_key(keycode);

	if (space_pressed && is_letter) {
		tap_code(KC_BSPC);
		add_oneshot_mods(MOD_BIT(KC_LSFT));
		space_pressed = false;
		return true;
	}

	if (!is_letter) {
		camel_case_active = false;
		space_pressed     = false;
		return true;
	}

	space_pressed = false;
	return true;
}
