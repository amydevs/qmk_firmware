MCU = atmega32a
F_CPU = 16000000
BOOTLOADER = usbasploader

LTO_ENABLE = yes
I2C_DRIVER_REQUIRED = yes

SRC += screen_app.c tiny_mcu.c app_eeconfig.c timeout.c
