/*
 * Crosses 42 motion-gated temporary layer processor
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_input_processor_motion_gated_temp_layer

#include <stdlib.h>

#include <zephyr/device.h>
#include <zephyr/input/input.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>

#include <drivers/input_processor.h>
#include <zmk/keymap.h>

struct motion_gate_config {
    const struct device *processor;
    uint16_t activation_threshold;
    uint16_t activation_window_ms;
};

struct motion_gate_data {
    int64_t window_started_at;
    uint32_t accumulated_movement;
};

static bool is_xy_motion(const struct input_event *event) {
    return event->type == INPUT_EV_REL &&
           (event->code == INPUT_REL_X || event->code == INPUT_REL_Y);
}

static void reset_gate(struct motion_gate_data *data) {
    data->window_started_at = 0;
    data->accumulated_movement = 0;
}

static int motion_gate_handle_event(const struct device *dev, struct input_event *event,
                                    uint32_t layer, uint32_t timeout_ms,
                                    struct zmk_input_processor_state *state) {
    const struct motion_gate_config *config = dev->config;
    struct motion_gate_data *data = dev->data;

    if (!is_xy_motion(event)) {
        return ZMK_INPUT_PROC_CONTINUE;
    }

    /* Once Mouse is active, every real motion event refreshes its timeout. */
    if (zmk_keymap_layer_active(zmk_keymap_layer_index_to_id(layer))) {
        reset_gate(data);
        return zmk_input_processor_handle_event(config->processor, event, layer, timeout_ms, state);
    }

    if (event->value == 0) {
        return ZMK_INPUT_PROC_CONTINUE;
    }

    const int64_t now = k_uptime_get();
    if (data->window_started_at == 0 ||
        now - data->window_started_at > config->activation_window_ms) {
        data->window_started_at = now;
        data->accumulated_movement = 0;
    }

    data->accumulated_movement =
        MIN(UINT32_MAX, data->accumulated_movement + (uint32_t)abs(event->value));

    if (data->accumulated_movement < config->activation_threshold) {
        return ZMK_INPUT_PROC_CONTINUE;
    }

    reset_gate(data);
    return zmk_input_processor_handle_event(config->processor, event, layer, timeout_ms, state);
}

static const struct zmk_input_processor_driver_api motion_gate_driver_api = {
    .handle_event = motion_gate_handle_event,
};

#define MOTION_GATE_INST(n)                                                                       \
    BUILD_ASSERT(DT_INST_PROP(n, activation_threshold) > 0,                                      \
                 "activation-threshold must be greater than zero");                             \
    BUILD_ASSERT(DT_INST_PROP(n, activation_window_ms) > 0,                                      \
                 "activation-window-ms must be greater than zero");                            \
    static struct motion_gate_data motion_gate_data_##n = {};                                    \
    static const struct motion_gate_config motion_gate_config_##n = {                            \
        .processor = DEVICE_DT_GET(DT_INST_PHANDLE(n, processor)),                               \
        .activation_threshold = DT_INST_PROP(n, activation_threshold),                           \
        .activation_window_ms = DT_INST_PROP(n, activation_window_ms),                           \
    };                                                                                            \
    DEVICE_DT_INST_DEFINE(n, NULL, NULL, &motion_gate_data_##n, &motion_gate_config_##n,          \
                          POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                       \
                          &motion_gate_driver_api);

DT_INST_FOREACH_STATUS_OKAY(MOTION_GATE_INST)
