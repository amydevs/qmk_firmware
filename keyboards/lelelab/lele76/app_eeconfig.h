#pragma once
#include <stdbool.h>

#define APP_EECONFIG_ADDR_MAGICBYTE 5

#define APP_EECONFIG_MAGICBYTE  0x06

#define APP_EECONFIG_ADDR_START 6

typedef struct {
    uint8_t mode;
    uint8_t bright;
    uint8_t satu;
    uint8_t hue;
    uint8_t hue2;
    uint8_t speed;
    uint8_t autooff;
} reg_rgb_t;

typedef struct {
    struct {
        uint8_t bright;
        uint8_t invert;
        uint8_t autooff;
    } oled;

    reg_rgb_t rgb;

    struct {
        uint8_t mode;
        uint8_t bright;
        uint8_t speed;
        uint8_t autooff;
    } side;

    // uint8_t homeart_id;
} app_eeconfig_t;

extern app_eeconfig_t eecfg;

bool app_eeconfig_load(void);
void app_eeconfig_save(void);
void app_eeconfig_init(void);
void app_eeconfig_oled_side_effect(void);
void app_eeconfig_rgb_mode_side_effect(void);
void app_eeconfig_rgb_satu_side_effect(void);
void app_eeconfig_rgb_bright_side_effect(void);
void app_eeconfig_rgb_hue_side_effect(void);
void app_eeconfig_rgb_hue2_side_effect(void);
void app_eeconfig_rgb_speed_side_effect(void);
void app_eeconfig_side_mode_side_effect(void);
void app_eeconfig_side_bright_side_effect(void);
void app_eeconfig_side_speed_side_effect(void);
void app_eeconfig_task(void);
void app_eeconfig_all_side_effects_schedule(void);
