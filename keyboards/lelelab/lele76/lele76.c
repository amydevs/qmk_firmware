#include QMK_KEYBOARD_H
#include "screen_app.h"
#include "via_protocol.h"
#include "app_eeconfig.h"
#include "timeout.h"

void eeconfig_init_kb(void) {  // EEPROM is getting reset!
    app_eeconfig_init();
}

void keyboard_post_init_kb(void) {
    app_eeconfig_load();
}

void housekeeping_task_kb(void) {
    timeout_task();
}

void post_process_record_kb(uint16_t keycode, keyrecord_t *record) {
    refresh_timeout();
}

void post_encoder_update_kb(uint8_t index, bool clockwise) {
    refresh_timeout();
}

#ifdef OLED_ENABLE
// Used to draw on to the oled screen
bool oled_task_kb(void) {
    app_draw();
    return false;
}

// Display seems to be a 64x128 mounted horizontally.
oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    return OLED_ROTATION_90;
}
#endif

#ifdef VIA_ENABLE
static void backlight_custom_set_value(uint8_t *data) {
    // data = [ value_id, value_data ]
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch (*value_id) {
        case id_backlight_brightness: {
            eecfg.rgb.bright = *value_data;
            break;
        }
        case id_backlight_effect: {
            eecfg.rgb.mode = *value_data;
            break;
        }
        case id_backlight_effect_speed: {
            eecfg.rgb.speed = *value_data;
            break;
        }
        case id_backlight_hue: {
            eecfg.rgb.hue = *value_data;
            break;
        }
        case id_backlight_hue2: {
            eecfg.rgb.hue2 = *value_data;
            break;
        }
        case id_backlight_saturation: {
            eecfg.rgb.satu = *value_data;
            break;
        }
    }
}


static void backlight_custom_get_value(uint8_t *data) {
    // data = [ value_id, value_data ]
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch (*value_id) {
        case id_backlight_brightness: {
            *value_data = eecfg.rgb.bright;
            break;
        }
        case id_backlight_effect: {
            *value_data = eecfg.rgb.mode;
            break;
        }
        case id_backlight_effect_speed: {
            *value_data = eecfg.rgb.speed;
            break;
        }
        case id_backlight_hue: {
            *value_data = eecfg.rgb.hue;
            break;
        }
        case id_backlight_hue2: {
            *value_data = eecfg.rgb.hue2;
            break;
        }
        case id_backlight_saturation: {
            *value_data = eecfg.rgb.satu;
            break;
        }
    }
}

void via_custom_value_command_kb(uint8_t *data, uint8_t length) {
    // data = [ command_id, channel_id, value_id, value_data ]
    uint8_t *command_id        = &(data[0]);
    uint8_t *channel_id        = &(data[1]);
    uint8_t *value_id_and_data = &(data[2]);

    if (*channel_id == id_custom_channel) {
        switch (*command_id) {
            case id_custom_set_value: {
                backlight_custom_set_value(value_id_and_data);
                break;
            }
            case id_custom_get_value: {
                backlight_custom_get_value(value_id_and_data);
                break;
            }
            case id_custom_save: {
                app_eeconfig_save();
                break;
            }
            default: {
                // Unhandled message.
                *command_id = id_unhandled;
                break;
            }
        }
        return;
    }

    // Return the unhandled state
    *command_id = id_unhandled;

    // DO NOT call raw_hid_send(data,length) here, let caller do this
}
#endif
