#include QMK_KEYBOARD_H
#include "screen_app.h"
#include "via_protocol.h"

#ifdef OLED_ENABLE
// Used to draw on to the oled screen
bool oled_task_user(void) {
    app_draw();
    return false;
}

// Display seems to be a 64x128 mounted horizontally.
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_90;
}
#endif

#ifdef VIA_ENABLE
void via_custom_value_command_kb(uint8_t *data, uint8_t length) {

    // DO NOT call raw_hid_send(data,length) here, let caller do this
}
#endif
