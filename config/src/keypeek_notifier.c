/*
 * Crosses 42 KeyPeek notifier.
 * Derived from srwi/zmk-keypeek-layer-notifier at ad688638517baf527e66a20596aa152cb42a1cd4.
 * Local modifier packets were originally added in commit
 * 3c1df661217e45a740500f373621f52d7c83fea6.
 *
 * Copyright (c) 2024 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 */

#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/events/position_state_changed.h>
#include <zmk/keymap.h>
#include <zmk/keys.h>

#include <raw_hid/events.h>

#include <stdint.h>
#include <string.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#define KEYPEEK_LAYER_PACKET_MARKER 0xFF
#define KEYPEEK_KEY_PACKET_MARKER 0xF1
#define KEYPEEK_MOD_PACKET_MARKER 0xF2
#define MAX_LAYER_CHECK 32

#define KEYPEEK_HID_LCTL 0xE0
#define KEYPEEK_HID_LSFT 0xE1
#define KEYPEEK_HID_LALT 0xE2
#define KEYPEEK_HID_LGUI 0xE3
#define KEYPEEK_HID_RCTL 0xE4
#define KEYPEEK_HID_RSFT 0xE5
#define KEYPEEK_HID_RALT 0xE6
#define KEYPEEK_HID_RGUI 0xE7

static uint8_t hid_buf[CONFIG_RAW_HID_REPORT_SIZE];
static uint8_t current_mod_state;

static void keypeek_send_layer_state(void) {
    uint32_t layer_state = 0;
    for (uint8_t i = 0; i < MAX_LAYER_CHECK; i++) {
        if (zmk_keymap_layer_active(i)) {
            layer_state |= BIT(i);
        }
    }

    uint32_t default_layer_state = BIT(0);

    memset(hid_buf, 0, sizeof(hid_buf));
    hid_buf[0] = KEYPEEK_LAYER_PACKET_MARKER;
    hid_buf[1] = sizeof(uint32_t);
    memcpy(&hid_buf[2], &default_layer_state, sizeof(uint32_t));
    memcpy(&hid_buf[2 + sizeof(uint32_t)], &layer_state, sizeof(uint32_t));

    raise_raw_hid_sent_event(
        (struct raw_hid_sent_event){.data = hid_buf, .length = sizeof(hid_buf)});
}

static void keypeek_send_key_event(uint32_t position, bool pressed) {
    if (position > UINT8_MAX) {
        LOG_WRN("Position %u exceeds KeyPeek packet format", position);
        return;
    }

    memset(hid_buf, 0, sizeof(hid_buf));
    hid_buf[0] = KEYPEEK_KEY_PACKET_MARKER;
    hid_buf[1] = 0;
    hid_buf[2] = (uint8_t)position;
    hid_buf[3] = pressed ? 1 : 0;

    raise_raw_hid_sent_event(
        (struct raw_hid_sent_event){.data = hid_buf, .length = sizeof(hid_buf)});
}

static void keypeek_send_mod_state(uint8_t mods) {
    memset(hid_buf, 0, sizeof(hid_buf));
    hid_buf[0] = KEYPEEK_MOD_PACKET_MARKER;
    hid_buf[1] = mods;

    raise_raw_hid_sent_event(
        (struct raw_hid_sent_event){.data = hid_buf, .length = sizeof(hid_buf)});
}

static uint8_t keypeek_mod_bit_for_keycode(uint32_t keycode) {
    switch (keycode) {
    case KEYPEEK_HID_LCTL:
        return BIT(0);
    case KEYPEEK_HID_LSFT:
        return BIT(1);
    case KEYPEEK_HID_LALT:
        return BIT(2);
    case KEYPEEK_HID_LGUI:
        return BIT(3);
    case KEYPEEK_HID_RCTL:
        return BIT(4);
    case KEYPEEK_HID_RSFT:
        return BIT(5);
    case KEYPEEK_HID_RALT:
        return BIT(6);
    case KEYPEEK_HID_RGUI:
        return BIT(7);
    default:
        return 0;
    }
}

static int layer_state_changed_listener(const zmk_event_t *eh) {
    ARG_UNUSED(eh);
    keypeek_send_layer_state();
    return ZMK_EV_EVENT_BUBBLE;
}

static int position_state_changed_listener(const zmk_event_t *eh) {
    const struct zmk_position_state_changed *ev = as_zmk_position_state_changed(eh);
    if (ev == NULL) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    keypeek_send_key_event(ev->position, ev->state);
    return ZMK_EV_EVENT_BUBBLE;
}

static int keycode_state_changed_listener(const zmk_event_t *eh) {
    const struct zmk_keycode_state_changed *ev = as_zmk_keycode_state_changed(eh);
    if (ev == NULL || ev->usage_page != HID_USAGE_KEY) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    uint8_t bit = keypeek_mod_bit_for_keycode(ev->keycode);
    if (bit == 0) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    uint8_t new_state = current_mod_state;
    if (ev->state) {
        new_state |= bit;
    } else {
        new_state &= ~bit;
    }

    if (new_state != current_mod_state) {
        current_mod_state = new_state;
        keypeek_send_mod_state(current_mod_state);
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(keypeek_layer_notifier, layer_state_changed_listener);
ZMK_SUBSCRIPTION(keypeek_layer_notifier, zmk_layer_state_changed);

ZMK_LISTENER(keypeek_key_notifier, position_state_changed_listener);
ZMK_SUBSCRIPTION(keypeek_key_notifier, zmk_position_state_changed);

ZMK_LISTENER(keypeek_mod_notifier, keycode_state_changed_listener);
ZMK_SUBSCRIPTION(keypeek_mod_notifier, zmk_keycode_state_changed);
