#include "oled.h"
#include "hardware/spi.h" // Include necessary header for spi0

// OLED Command Set
#define SPI_PORT spi0
#define PIN_DC 16
#define PIN_CS_1 17 // Chip select is active-low
#define PIN_SCLK 18
#define PIN_MOSI 19
#define PIN_RES 22

int main()
{
    stdio_init_all();

    PmodOLED oled = create_oled(SPI_PORT, PIN_CS_1, PIN_DC, PIN_RES, PIN_MOSI, PIN_SCLK);

    spi_initilize(&oled); // Initialize SPI

    reset_oled(&oled);
    clear_oled(&oled);
    init_oled(&oled);

    int i = 0;
    while (true)
    {

        // printf("Current value: %d\n", i);
        char message[32]; // Buffer to store the formatted message

        // Format the message
        sprintf(message, "Current value: %d", i);

        // Draw the message at row 0, column 10
        draw_string(&oled, message, 2, 0);
        sleep_ms(100);

        i++;
    }

    return 0;
}
