#include QMK_KEYBOARD_H
#include "app_eeconfig.h"
#include "tiny_mcu.h"
#include "tiny_mcu_protocol.h"

static uint16_t key_timer;
bool is_oled_timeout = false;
bool is_led_timeout = false;
bool is_sideled_timeout = false;


static void handle_led_suspend(void) {
    tiny85_i2c_tx_2b(CMD_RGB_eff, RGB_Mode_off);
}

static void handle_wakeup(void) {
    tiny85_i2c_tx_2b(CMD_RGB_eff, eecfg.rgb.mode);

    if (eecfg.side.mode <= SideLed_Mode_solid) {
        const uint8_t side_effect_data[] = {CMD_SIDE_LED_EFFECT, eecfg.side.mode};
        tiny85_i2c_tx(side_effect_data, sizeof(side_effect_data));
    }
}

void timeout_task(void) {
    if (!is_led_timeout && timer_elapsed(key_timer) > eecfg.rgb.autooff * 1000) {
        handle_led_suspend();
        is_led_timeout = true;
    }
}

void refresh_timeout(void) {
    key_timer = timer_read();
    if (is_led_timeout) {
        is_led_timeout = false;
        handle_wakeup();
    }
}
