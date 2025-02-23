#include "oled.h"

// OLED1
#define SPI_PORT spi0
#define PIN_DC 16
#define PIN_CS_1 17 // Chip select is active-low
#define PIN_SCLK 18
#define PIN_MOSI 19
#define PIN_RES 22

// OLED2
#define SPI_PORT2 spi0
#define PIN_DC2 16
#define PIN_CS_2 6 // Chip select is active-low
#define PIN_SCLK2 18
#define PIN_MOSI2 19
#define PIN_RES2 22

void send_command_1(uint8_t cmd) {
    gpio_put(PIN_DC, 0);  // Command mode
    gpio_put(PIN_CS_1, 0);  // Select OLED
    spi_write_blocking(SPI_PORT, &cmd, 1);
    gpio_put(PIN_CS_1, 1);  // Deselect OLED
}

void send_command_2(uint8_t cmd) {
    gpio_put(PIN_DC, 0);  // Command mode
    gpio_put(PIN_CS_2, 0);  // Select OLED
    spi_write_blocking(SPI_PORT, &cmd, 1);
    gpio_put(PIN_CS_2, 1);  // Deselect OLED
}

void send_data_1(uint8_t *data, size_t len) {
    gpio_put(PIN_DC, 1);  // Data mode
    gpio_put(PIN_CS_1, 0);  // Select OLED
    spi_write_blocking(SPI_PORT, data, len);
    gpio_put(PIN_CS_1, 1);  // Deselect OLED
}

void send_data_2(uint8_t *data, size_t len) {
    gpio_put(PIN_DC, 1);  // Data mode
    gpio_put(PIN_CS_2, 0);  // Select OLED
    spi_write_blocking(SPI_PORT, data, len);
    gpio_put(PIN_CS_2, 1);  // Deselect OLED
}

void reset_oled() {
    gpio_put(PIN_RES, 0);
    sleep_ms(100);
    gpio_put(PIN_RES, 1);
    sleep_ms(100);
}


void init_oled() {
    reset_oled();

    for (size_t i = 0; i < sizeof(init_cmds); i++) {
        send_command_1(init_cmds[i]);
        send_command_2(init_cmds[i]);
    }
}

void clear_1() {
    uint8_t buffer[128]; // Buffer for one row (128 columns)
    memset(buffer, 0, sizeof(buffer));

    for (uint8_t i = 0; i < 4; i++) {  // 4 pages for 128x32 display (32 rows ÷ 8 = 4 pages)
        send_command_1(0xB0 + i); // Set page address
        send_command_1(0x00);     // Set lower column address
        send_command_1(0x10);     // Set higher column address
        send_data_1(buffer, 128); // Clear the row
    }
}

void clear_2() {
    uint8_t buffer[128]; // Buffer for one row (128 columns)
    memset(buffer, 0, sizeof(buffer));

    for (uint8_t i = 0; i < 4; i++) {  // 4 pages for 128x32 display (32 rows ÷ 8 = 4 pages)
        send_command_2(0xB0 + i); // Set page address
        send_command_2(0x00);     // Set lower column address
        send_command_2(0x10);     // Set higher column address
        send_data_2(buffer, 128); // Clear the row
    }
}

void draw_string_1(const char *str, uint8_t page, uint8_t col) {
    clear_1();
    for (size_t i = 0; str[i] != '\0'; i++) {
        uint8_t char_index = str[i] - ' '; // Convert character to font index (starting from space ' ')
        if (char_index < 128) { // Ensure the character is within the font array bounds
            send_command_1(0xB0 + page);  // Set page (row)
            send_command_1(0x00 + (col & 0x0F));  // Lower column address
            send_command_1(0x10 + (col >> 4));   // Higher column address
            send_data_1((uint8_t *)font5x8[char_index], 5); // Send character pixels
            col += 6; // Move cursor (5 pixels + 1 spacing)
        }
    }
}

void draw_string_2(const char *str, uint8_t page, uint8_t col) {
    clear_2();
    for (size_t i = 0; str[i] != '\0'; i++) {
        uint8_t char_index = str[i] - ' '; // Convert character to font index (starting from space ' ')
        if (char_index < 128) { // Ensure the character is within the font array bounds
            send_command_2(0xB0 + page);  // Set page (row)
            send_command_2(0x00 + (col & 0x0F));  // Lower column address
            send_command_2(0x10 + (col >> 4));   // Higher column address
            send_data_2((uint8_t *)font5x8[char_index], 5); // Send character pixels
            col += 6; // Move cursor (5 pixels + 1 spacing)
        }
    }
}