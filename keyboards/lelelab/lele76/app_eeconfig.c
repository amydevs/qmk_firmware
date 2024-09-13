#include "eeprom.h"
#include "tiny_mcu.h"
#include "tiny_mcu_protocol.h"
#include "app_eeconfig.h"

app_eeconfig_t eecfg;
static uint8_t side_effect_run_progress = 0;

bool app_eeconfig_load(void) {
    // check magicbyte
    if (eeprom_read_byte((uint8_t*) APP_EECONFIG_ADDR_MAGICBYTE)
        != APP_EECONFIG_MAGICBYTE)
        return false;

    uint8_t *buf = (uint8_t*) &eecfg;
    for (int i=0;i<sizeof(app_eeconfig_t);i++) {
        *buf = eeprom_read_byte((uint8_t*) (i + APP_EECONFIG_ADDR_START));
        buf++;
    }
    return true;
}

void app_eeconfig_save(void) {
    uint8_t *src = (uint8_t*) &eecfg;
    for (int i=0;i<sizeof(app_eeconfig_t);i++) {
        eeprom_update_byte((uint8_t*)(i + APP_EECONFIG_ADDR_START), *src);
        src++;
    }
    eeprom_update_byte((uint8_t*)APP_EECONFIG_ADDR_MAGICBYTE, APP_EECONFIG_MAGICBYTE);
}

void app_eeconfig_init(void) {
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

void app_eeconfig_task(void) {
    if (is_tiny_busy()) return;
    switch (side_effect_run_progress) {
        case 1:
        {
            oled_set_brightness(eecfg.oled.bright);
            oled_invert(eecfg.oled.invert);
            break;
        }
        case 2:
        {
            tiny85_i2c_tx_2b(CMD_RGB_eff, eecfg.rgb.mode);
            break;
        }
        case 3:
        {
            tiny85_i2c_tx_2b(CMD_RGB_satu, eecfg.rgb.satu);
            break;
        }
        case 4:
        {
            tiny85_i2c_tx_2b(CMD_RGB_bright, eecfg.rgb.bright);
            break;
        }
        case 5:
        {
            tiny85_i2c_tx_2b(CMD_RGB_hue, eecfg.rgb.hue);
            break;
        }
        case 6:
        {
            tiny85_i2c_tx_2b(CMD_RGB_hue2, eecfg.rgb.hue2);
            break;
        }
        case 7:
        {
            tiny85_i2c_tx_2b(CMD_RGB_speed, 10+MAX_RGB_SPEED-eecfg.rgb.speed);
            break;
        }
        case 8:
        {
            if (eecfg.side.mode <= SideLed_Mode_solid) {
                const uint8_t side_effect_data[] = {CMD_SIDE_LED_EFFECT, eecfg.side.mode};
                tiny85_i2c_tx(side_effect_data, sizeof(side_effect_data));
            }
            break;
        }
        case 9:
        {
            const uint8_t side_bright_data[] = {CMD_SIDE_LED_BRIGHT, eecfg.side.bright};
            tiny85_i2c_tx(side_bright_data, sizeof(side_bright_data));
            break;
        }
        case 10:
        {
            const uint8_t side_speed_data[] = {CMD_SIDE_LED_SPEED, eecfg.side.speed};
            tiny85_i2c_tx(side_speed_data, sizeof(side_speed_data));
            break;
        }
        default:
        {
            return;
        }
    }
    side_effect_run_progress++;
}

void app_eeconfig_side_effects_run(void) {
    side_effect_run_progress = 1;
}
