#include QMK_KEYBOARD_H

enum unicode_names {
    ue,
		UE,
		oe,
		OE,
		ae,
		AE,
		SZ
};

const uint32_t PROGMEM unicode_map[] = {
    [ue] = 0X00FC, // ü
    [UE] = 0X00DC, // Ü
    [oe] = 0X00F6, // ö
    [OE] = 0X00D6, // Ö
    [ae] = 0X00E4, // ä
    [AE] = 0X00C4, // Ä
    [SZ] = 0X00DF, // ß
};

// void keyboard_post_init_user(void) {
//   debug_enable = true;
// }

#define MODS_LSHIFT  (get_mods() & MOD_BIT(KC_LSHIFT))
#define MODS_RSHIFT  (get_mods() & MOD_BIT(KC_RSHIFT))
#define MODS_SHIFT  (MODS_LSHIFT || MODS_RSHIFT)

// we are going to unregister the shift keys, when shift+backspace is pressed and use those
// variables to keep track of them instead
static bool bspc_del_lshift = false;
static bool bspc_del_rshift = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	// #ifdef CONSOLE_ENABLE
  //   uprintf("KL: kc: %u, pressed: %u, shift: %u\n", keycode, record->event.pressed, MODS_SHIFT);
	// #endif

	// ctrl + backspace = del (forward delete)
	if (keycode == KC_BSPC) {
		if ((MODS_SHIFT || (bspc_del_lshift || bspc_del_rshift)) && record->event.pressed) {
			// some programs (especially code editors), don't execute a forward delete for shift+del,
			// which is why we have to unregister the shift key
			if (MODS_LSHIFT) {
				bspc_del_lshift = true;
				unregister_code(KC_LSHIFT);
			}
			if (MODS_RSHIFT) {
				bspc_del_rshift = true;
				unregister_code(KC_RSHIFT);
			}

			register_code(KC_DEL);

			return false;
		} else if (!record->event.pressed) {
			unregister_code(KC_DEL);

			// if shift keys are still pressed, re-send their code
			if (bspc_del_lshift) {
				bspc_del_lshift = false;
				register_code(KC_LSHIFT);
			}
			if (bspc_del_rshift) {
				bspc_del_rshift = false;
				register_code(KC_RSHIFT);
			}
		}
	}

	// track if we stopped pressing the shift modifiers
	if (keycode == KC_LSHIFT && !record->event.pressed) {
		bspc_del_lshift = false;
	}
	if (keycode == KC_RSHIFT && !record->event.pressed) {
		bspc_del_rshift = false;
	}

	return true;
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT_60_iso_5x1u(LT(1,KC_ESC), KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_ESC, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_BSLS, KC_ENT, KC_LSFT, LT(2,KC_GRV), KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_LCTL, KC_LALT, KC_LGUI, KC_SPC, KC_RGUI, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT),
	[1] = LAYOUT_60_iso_5x1u(KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DEL, KC_NO, KC_BRID, KC_BRIU, KC_NO, KC_MPRV, KC_MPLY, KC_MFFD, KC__MUTE, KC__VOLDOWN, KC__VOLUP, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_SLEP, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RGB_TOG, KC_NO, KC_NO, RESET, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO),
	[2] = LAYOUT_60_iso_5x1u(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, X(SZ), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, XP(ue,UE), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, XP(oe,OE), XP(ae,AE), KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO)
};
