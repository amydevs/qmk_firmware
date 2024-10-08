#include "eeprom.h"
#include "tiny_mcu.h"
#include "tiny_mcu_protocol.h"
#include "eecfg.h"
#include "timeout.h"

eecfg_t eecfg;
static uint8_t side_effect_run_progress = 0;

bool eecfg_load(void) {
    // check magicbyte
    if (eeprom_read_byte((uint8_t*) EECFG_ADDR_MAGICBYTE)
        != EECFG_MAGICBYTE)
        return false;

    uint8_t *buf = (uint8_t*) &eecfg;
    for (int i=0;i<sizeof(eecfg_t);i++) {
        *buf = eeprom_read_byte((uint8_t*) (i + EECFG_ADDR_START));
        buf++;
    }
    return true;
}

void eecfg_save(void) {
    uint8_t *src = (uint8_t*) &eecfg;
    for (int i=0;i<sizeof(eecfg_t);i++) {
        eeprom_update_byte((uint8_t*)(i + EECFG_ADDR_START), *src);
        src++;
    }
    eeprom_update_byte((uint8_t*)EECFG_ADDR_MAGICBYTE, EECFG_MAGICBYTE);
}

void eecfg_init(void) {
    eecfg.rgb.mode = RGB_Mode_ripple;
    eecfg.rgb.bright = MAX_RGB_BRIGHT/2;
    eecfg.rgb.speed = MAX_RGB_SPEED;
    eecfg.rgb.satu = 0xff;
    eecfg.rgb.hue = 60;
    eecfg.rgb.hue2 = 200;
    eecfg.rgb.autooff = 0;

    eecfg.oled.bright = 180;
    eecfg.oled.invert = 0;
    eecfg.oled.autooff = 0;

    eecfg.side.mode = SideLed_Mode_keyflash;
    eecfg.side.bright = MAX_SIDE_LED_BRIGHT;
    eecfg.side.speed = MAX_SIDE_LED_SPEED;
    eecfg.side.autooff = 0;
}

void eecfg_oled_side_effect(void) {
    oled_set_brightness(eecfg.oled.bright);
    oled_invert(eecfg.oled.invert);
}

void eecfg_rgb_mode_side_effect(void) {
    if (!is_led_timeout) tiny85_i2c_tx_2b(CMD_RGB_eff, eecfg.rgb.mode);
}

void eecfg_rgb_satu_side_effect(void) {
    tiny85_i2c_tx_2b(CMD_RGB_satu, eecfg.rgb.satu);
}

void eecfg_rgb_bright_side_effect(void) {
    tiny85_i2c_tx_2b(CMD_RGB_bright, eecfg.rgb.bright);
}

void eecfg_rgb_hue_side_effect(void) {
    tiny85_i2c_tx_2b(CMD_RGB_hue, eecfg.rgb.hue);
}

void eecfg_rgb_hue2_side_effect(void) {
    tiny85_i2c_tx_2b(CMD_RGB_hue2, eecfg.rgb.hue2);
}

void eecfg_rgb_speed_side_effect(void) {
    tiny85_i2c_tx_2b(CMD_RGB_speed, 10+MAX_RGB_SPEED-eecfg.rgb.speed);
}

void eecfg_side_mode_side_effect(void) {
    if (!is_side_led_timeout && eecfg.side.mode <= SideLed_Mode_solid) {
        const uint8_t side_effect_data[] = {CMD_SIDE_LED_EFFECT, eecfg.side.mode};
        tiny85_i2c_tx(side_effect_data, sizeof(side_effect_data));
    }
}

void eecfg_side_bright_side_effect(void) {
    const uint8_t side_bright_data[] = {CMD_SIDE_LED_BRIGHT, eecfg.side.bright};
    tiny85_i2c_tx(side_bright_data, sizeof(side_bright_data));
}

void eecfg_side_speed_side_effect(void) {
    const uint8_t side_speed_data[] = {CMD_SIDE_LED_SPEED, eecfg.side.speed};
    tiny85_i2c_tx(side_speed_data, sizeof(side_speed_data));
}

void eecfg_task(void) {
    switch (side_effect_run_progress) {
        case 1:
        {
            eecfg_oled_side_effect();
            break;
        }
        case 2:
        {
            if (is_tiny_busy()) return;
            eecfg_rgb_mode_side_effect();
            break;
        }
        case 3:
        {
            if (is_tiny_busy()) return;
            eecfg_rgb_satu_side_effect();
            break;
        }
        case 4:
        {
            if (is_tiny_busy()) return;
            eecfg_rgb_bright_side_effect();
            break;
        }
        case 5:
        {
            if (is_tiny_busy()) return;
            eecfg_rgb_hue_side_effect();
            break;
        }
        case 6:
        {
            if (is_tiny_busy()) return;
            eecfg_rgb_hue2_side_effect();
            tiny85_i2c_tx_2b(CMD_RGB_hue2, eecfg.rgb.hue2);
            break;
        }
        case 7:
        {
            if (is_tiny_busy()) return;
            eecfg_rgb_speed_side_effect();
            tiny85_i2c_tx_2b(CMD_RGB_speed, 10+MAX_RGB_SPEED-eecfg.rgb.speed);
            break;
        }
        case 8:
        {
            if (is_tiny_busy()) return;
            eecfg_side_mode_side_effect();
            break;
        }
        case 9:
        {
            if (is_tiny_busy()) return;
            eecfg_side_bright_side_effect();
            break;
        }
        case 10:
        {
            if (is_tiny_busy()) return;
            eecfg_side_speed_side_effect();
            break;
        }
        default:
        {
            side_effect_run_progress = 0;
            return;
        }
    }
    side_effect_run_progress++;
}

void eecfg_all_side_effects_schedule(void) {
    side_effect_run_progress = 1;
}
