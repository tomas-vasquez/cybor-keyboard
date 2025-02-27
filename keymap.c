#include QMK_KEYBOARD_H // Incluye el encabezado principal de QMK

// Definición de los mapas de teclas para cada capa
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Capa 0: Capa base (QWERTY)
    [0] = LAYOUT_split_4x5_4(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, 
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, 
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, 
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                KC_6,    KC_7,    KC_8,    KC_9,    KC_0, 
                          MO(1), KC_SPC, KC_ENT, MO(2),    KC_BSPC, KC_ESC, KC_LGUI, KC_RALT 
    ),

    // Capa 1: Símbolos y navegación
    [1] = LAYOUT_split_4x5_4(
        KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC,             KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, 
        KC_MINS, KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS,           KC_GRV, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, 
        KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE,           KC_TILD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
                      _______, _______, _______, MO(3),     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX 
    ),

    // Capa 2: Funciones y navegación avanzada
    [2] = LAYOUT_split_4x5_4(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,               KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10, 
        KC_F11,  KC_F12, XXXXXXX, XXXXXXX, XXXXXXX,           XXXXXXX, KC_HOME, KC_PGDN, KC_PGUP, KC_END, 
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
                    _______, _______, _______, _______,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX 
    ),

    // Capa 3: Funciones especiales (reinicio, RGB matrix control)
    [3] = LAYOUT_split_4x5_4(
        QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
        RM_TOGG, RM_HUEU, RM_SATU, RM_VALU, XXXXXXX,           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
        RM_NEXT, RM_HUED, RM_SATD, RM_VALD, XXXXXXX,           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
                   _______, _______, _______, _______,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX 
    )
};

//---------------------------------------------------------------------
// Sensor TTP223 para evitar activaciones accidentales
//---------------------------------------------------------------------

// Variable para almacenar el estado del sensor TTP223
bool is_ttp223_touched = false;

// Función para verificar si el sensor TTP223 está activado
bool check_ttp223_touch(void) {
    return digitalRead(TTP223_SENSOR_PIN) == HIGH;  // Retorna verdadero si el sensor está tocado
}

//---------------------------------------------------------------------
// LED WS2812 para indicar la capa activa del teclado y la pulsacion de una tecla
//---------------------------------------------------------------------

// Definir colores para cada capa
#define LAYER0_COLOR 0x0000FF // Azul
#define LAYER1_COLOR 0xFF0000 // Rojo
#define LAYER2_COLOR 0x00FF00 // Verde
#define LAYER3_COLOR 0x800080 // Violeta

// Variable para almacenar el tiempo de inicio del parpadeo
uint32_t blink_start_time = 0;

// Función para iniciar el parpadeo del LED
void start_blink_led(void) {
    blink_start_time = timer_read32(); // Obtener el tiempo actual
    rgb_matrix_set_color(0, 0, 0, 0); // Apagar el LED inmediatamente
}

// Función para manejar el parpadeo no bloqueante
void handle_blink_led(void) {
    if (timer_elapsed32(blink_start_time) >= 50) { // Verificar si han pasado 50 ms
        update_led_color(layer_state_get_current()); // Restaurar el color del LED
    }
}

// Función para actualizar el color del LED según la capa activa
void update_led_color(uint32_t state) {
    uint32_t color;
    switch (get_highest_layer(state)) {
        case 0:  color = LAYER0_COLOR; break;
        case 1:  color = LAYER1_COLOR; break;
        case 2:  color = LAYER2_COLOR; break;
        case 3:  color = LAYER3_COLOR; break;
        default: color = 0x000000; // Apagar LED para capas desconocidas
    }
    rgb_matrix_set_color(0, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
}

//---------------------------------------------------------------------
// Sobreescritura de funciones en QMK
//---------------------------------------------------------------------

// Procesar pulsaciones de teclas
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Si el sensor TTP223 no está activo, ignorar las pulsaciones
    if (is_ttp223_touched) {
        if (record->event.pressed) {
            start_blink_led(); // Parpadear LED al presionar una tecla
        }
        return true;
    } 
    return false;
}

// Escaneo de la matriz del teclado
void matrix_scan_user(void) {
    is_ttp223_touched = check_ttp223_touch(); // Leer el estado del sensor TTP223
    handle_blink_led(); // Manejar el parpadeo no bloqueante
}

// Actualizar color del LED al cambiar de capa
void layer_state_set_user(uint32_t state) {
    update_led_color(state);
}