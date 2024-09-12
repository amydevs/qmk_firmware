#include "eeprom.h"
#include "app_eeconfig.h"

app_eeconfig_t eecfg;

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
