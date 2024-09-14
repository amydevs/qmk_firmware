#include QMK_KEYBOARD_H
#include "eecfg.h"
#include "tiny_mcu.h"
#include "tiny_mcu_protocol.h"

static uint32_t key_timer;
bool is_oled_timeout = false;
bool is_led_timeout = false;
bool is_side_led_timeout = false;


static void handle_led_suspend(void) {
    tiny85_i2c_tx_2b(CMD_RGB_eff, RGB_Mode_off);
}

static void handle_led_wakeup(void) {
    tiny85_i2c_tx_2b(CMD_RGB_eff, eecfg.rgb.mode);
}


static void handle_side_led_suspend(void) {
    const uint8_t side_effect_data[] = {CMD_SIDE_LED_EFFECT, SideLed_Mode_off};
    tiny85_i2c_tx(side_effect_data, sizeof(side_effect_data));
}

static void handle_side_led_wakeup(void) {
    const uint8_t side_effect_data[] = {CMD_SIDE_LED_EFFECT, eecfg.side.mode};
    tiny85_i2c_tx(side_effect_data, sizeof(side_effect_data));
}

static void handle_oled_suspend(void) {
    oled_off();
}

static void handle_oled_wakeup(void) {
    oled_on();
}

void timeout_task(void) {
    int32_t elapsed = timer_elapsed(key_timer);
    if (eecfg.oled.autooff != 0) {
        if (!is_oled_timeout && elapsed > eecfg.oled.autooff * 1000) {
            handle_oled_suspend();
            is_oled_timeout = true;
        }
    }
    else if (is_oled_timeout) {
        handle_oled_wakeup();
        is_oled_timeout = false;
    }
    if (eecfg.rgb.autooff != 0) {
        if (!is_led_timeout && elapsed > eecfg.rgb.autooff * 1000 && !is_tiny_busy()) {
            handle_led_suspend();
            is_led_timeout = true;
        }
    }
    else if (is_led_timeout && !is_tiny_busy()) {
        handle_led_wakeup();
        is_led_timeout = false;
    }
    if (eecfg.side.autooff != 0) {
        if (!is_side_led_timeout && elapsed > eecfg.side.autooff * 1000 && !is_tiny_busy()) {
            handle_side_led_suspend();
            is_side_led_timeout = true;
        }
    }
    else if (is_side_led_timeout && !is_tiny_busy()) {
        handle_side_led_wakeup();
        is_side_led_timeout = false;
    }
}

void refresh_timeout(void) {
    key_timer = timer_read32();
    if (is_oled_timeout) {
        is_oled_timeout = false;
        handle_oled_wakeup();
    }
    if (is_led_timeout) {
        is_led_timeout = false;
        handle_led_wakeup();
    }
    if (is_side_led_timeout) {
        is_side_led_timeout = false;
        handle_side_led_wakeup();
    }
}
