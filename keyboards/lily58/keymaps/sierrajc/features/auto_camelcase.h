#pragma once

#include "community_modules.h"
/**
 * Auto camel case mode
 *
 * When activated, automatically converts space-separated words into camelCase.
 * Typing "my variable name" becomes "myVariableName" by deleting spaces and
 * capitalising the first letter of each subsequent word.
 *
 * Mode exits automatically when space is followed by a non-letter character,
 * or can be manually toggled off by pressing the CAMELCASE key again.
 *
 * Usage:
 *   1. Call auto_camelcase_process_record() from your process_record_user()
 *   2. Call auto_camelcase_toggle() when your CAMELCASE key is pressed
 *
 * Example:
 *   bool process_record_user(uint16_t keycode, keyrecord_t *record) {
 *       if (!auto_camelcase_process_record(keycode, record)) {
 *           return false;
 *       }
 *       
 *       switch (keycode) {
 *           case CAMELCASE:
 *               if (record->event.pressed) {
 *                   auto_camelcase_toggle();
 *               }
 *               return false;
 *       }
 *       
 *       return true;
 *   }
 */

/**
 * Toggle camel case mode on/off
 */
void auto_camelcase_toggle(void);

/**
 * Check if camel case mode is currently active
 */
bool auto_camelcase_is_active(void);

/**
 * Process key events for auto camel case mode
 * Returns false if the event was handled and should not be processed further
 * Returns true if the event should continue normal processing
 */
bool auto_camelcase_process_record(uint16_t keycode, keyrecord_t *record);
