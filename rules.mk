MCU = atmega32u4 # Cambia esto si usas otro microcontrolador

LAYOUTS = default

SPLIT_KEYBOARD = yes
MASTER_RIGHT = yes

SERIAL_DRIVER = hardware

SRC += split_util.c
