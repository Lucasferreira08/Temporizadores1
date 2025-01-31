#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

// Definição dos pinos dos LEDs
#define LED_RED 11
#define LED_YELLOW 12
#define LED_GREEN 13

// Variável para controlar o estado do semáforo
typedef enum {
    RED,
    YELLOW,
    GREEN
} TrafficLightState;

TrafficLightState current_state = RED;

// Função de callback do temporizador
bool repeating_timer_callback(struct repeating_timer *t) {
    switch (current_state) {
        case RED:
            gpio_put(LED_RED, 0);    // Apaga LED vermelho
            gpio_put(LED_YELLOW, 1); // Acende LED amarelo
            current_state = YELLOW;
            break;
        case YELLOW:
            gpio_put(LED_YELLOW, 0); // Apaga LED amarelo
            gpio_put(LED_GREEN, 1);  // Acende LED verde
            current_state = GREEN;
            break;
        case GREEN:
            gpio_put(LED_GREEN, 0);  // Apaga LED verde
            gpio_put(LED_RED, 1);    // Acende LED vermelho
            current_state = RED;
            break;
    }
    return true;
}

int main() {
    stdio_init_all();

    // Configuração dos GPIOs
    gpio_init(LED_RED);
    gpio_init(LED_YELLOW);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_YELLOW, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);

    // Inicializa o semáforo com o LED vermelho aceso
    gpio_put(LED_RED, 1);
    gpio_put(LED_YELLOW, 0);
    gpio_put(LED_GREEN, 0);

    // Configuração do temporizador periódico
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    // Loop principal
    while (true) {
        printf("Semáforo em funcionamento...\n");
        sleep_ms(1000);
    }

    return 0;
}