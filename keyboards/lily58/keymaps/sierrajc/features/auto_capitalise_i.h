#pragma once

#include "community_modules.h"
/**
 * Auto-capitalise standalone "i" to "I"
 *
 * Automatically capitalises the letter "i" when typed as a standalone word
 * (i.e., when surrounded by spaces: "space i space").
 *
 * Usage:
 *   Call auto_capitalise_i_process_record() from your process_record_user()
 *
 * Example:
 *   bool process_record_user(uint16_t keycode, keyrecord_t *record) {
 *       if (!auto_capitalise_i_process_record(keycode, record)) {
 *           return false;
 *       }
 *       // ... rest of your code
 *       return true;
 *   }
 */

/**
 * Process key events for auto-capitalization
 * Returns false if the event was handled and should not be processed further
 * Returns true if the event should continue normal processing
 */
bool auto_capitalise_i_process_record(uint16_t keycode, keyrecord_t *record);
