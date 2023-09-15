#include QMK_KEYBOARD_H

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

/* LAYOUT_60_iso_5x1u
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐
 * │00 │01 │02 │03 │04 │05 │06 │07 │08 │09 │0a │0b │0c │0e     │
 * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤
 * │10   │12 │13 │14 │15 │16 │17 │18 │19 │1a │1b │1c │1d │     │
 * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┐2d  │
 * │20    │22 │23 │24 │25 │26 │27 │28 │29 │2a │2b │2c │1e │    │
 * ├────┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┴┬───┤
 * │30  │31 │32 │33 │34 │35 │36 │37 │38 │39 │3a │3b │3d    │3e │
 * ├────┼───┴┬──┴─┬─┴───┴───┴┬──┴──┬┴───┴──┬┴──┬┴──┬┴──┬───┼───┤
 * │40  │41  │43  │46        │     │       │4a │4b │4c │4d │4e │
 * └────┴────┴────┴──────────┴─────┴───────┴───┴───┴───┴───┴───┘
*/
#define LAYOUT_RKUSA( \
    k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, k0c,      k0e, \
    k10,      k12, k13, k14, k15, k16, k17, k18, k19, k1a, k1b, k1c, k1d,      \
    k20,      k22, k23, k24, k25, k26, k27, k28, k29, k2a, k2b, k2c, k1e, k2d, \
    k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k3a, k3b,      k3d, k3e,  \
    k40, k41,      k43, k44,      k46,      k48,      k4a, k4b, k4c, k4d, k4e  \
) { \
    { k00,  k01,   k02,   k03,  k04,   k05,   k06,  k07,   k08,   k09,   k0a,  k0b,  k0c,   KC_NO, k0e   }, \
    { k10,  KC_NO, k12,   k13,  k14,   k15,   k16,  k17,   k18,   k19,   k1a,  k1b,  k1c,   k1d,   k1e   }, \
    { k20,  KC_NO, k22,   k23,  k24,   k25,   k26,  k27,   k28,   k29,   k2a,  k2b,  k2c,   k2d,   KC_NO }, \
    { k30,  k31,   k32,   k33,  k34,   k35,   k36,  k37,   k38,   k39,   k3a,  k3b,  KC_NO, k3d,   k3e   }, \
    { k40,  k41,   KC_NO, k43,  k44,   KC_NO, k46,  KC_NO, k48,  KC_NO,  k4a,  k4b,  k4c,   k4d,   k4e   }  \
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT_RKUSA(
		LT(1,KC_ESC), KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC,
		KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC,
		KC_ESC, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_BSLS, KC_ENT,
		KC_LSFT, KC_GRV, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_DEL,
		KC_LCTL, KC_LALT, KC_LGUI, KC_SPC, KC_F13, KC_SPC, KC_RGUI, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT),
	[1] = LAYOUT_RKUSA(
		KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DEL,
		KC_NO, KC_BRID, KC_BRIU, KC_NO, KC_MPRV, KC_MPLY, KC_MFFD, KC__MUTE, KC__VOLDOWN, KC__VOLUP, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_SLEP, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, RGB_TOG, KC_NO, KC_NO, RESET, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_N, KC_N),
};
