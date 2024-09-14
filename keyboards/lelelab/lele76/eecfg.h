#pragma once
#include <stdbool.h>

#define EECFG_ADDR_MAGICBYTE 5

#define EECFG_MAGICBYTE  0x06

#define EECFG_ADDR_START 6

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
} eecfg_t;

extern eecfg_t eecfg;

bool eecfg_load(void);
void eecfg_save(void);
void eecfg_init(void);
void eecfg_oled_side_effect(void);
void eecfg_rgb_mode_side_effect(void);
void eecfg_rgb_satu_side_effect(void);
void eecfg_rgb_bright_side_effect(void);
void eecfg_rgb_hue_side_effect(void);
void eecfg_rgb_hue2_side_effect(void);
void eecfg_rgb_speed_side_effect(void);
void eecfg_side_mode_side_effect(void);
void eecfg_side_bright_side_effect(void);
void eecfg_side_speed_side_effect(void);
void eecfg_task(void);
void eecfg_all_side_effects_schedule(void);
