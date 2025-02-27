#pragma once

#include "quantum.h"

// Matriz de teclas
#define MATRIX_ROWS 4
#define MATRIX_COLS 5
#define MATRIX_ROW_PINS { B0, B1, B2, B3 }
#define MATRIX_COL_PINS { D0, D1, D2, D3, D4 }

// Comunicación dividida (TRRS serial)
#define SPLIT_KEYBOARD
#define MASTER_RIGHT
#define SERIAL_DRIVER SERIAL_DRIVER_MASTER
#define SERIAL_RX_PIN D6
#define SERIAL_TX_PIN D7

// Sensor TTP223
#define TTP223_SENSOR_PIN D5

// LED WS2812B
#define RGB_MATRIX_ENABLE
#define RGB_MATRIX_DRIVER ws2812
#define RGB_MATRIX_PIN C6
#define RGB_MATRIX_LED_COUNT 1

// Definición del Layout
#define LAYOUT_split_4x5_4( \
    k00, k01, k02, k03, k04, \
    k10, k11, k12, k13, k14, \
    k20, k21, k22, k23, k24, \
    k30, k31, k32, k33, k34, \
    k40, k41, k42, k43, \
    k50, k51, k52, k53 \
) \
{ \
    { k00, k01, k02, k03, k04 }, \
    { k10, k11, k12, k13, k14 }, \
    { k20, k21, k22, k23, k24 }, \
    { k30, k31, k32, k33, k34 }, \
    { k40, k41, k42, k43 }, \
    { k50, k51, k52, k53 } \
}
