#include "oled.h"

PmodOLED create_oled(spi_inst_t *spi, uint cs_pin, uint dc_pin, uint res_pin, uint mosi_pin, uint sclk_pin)
{
    PmodOLED oled = {
        .spi = spi,
        .cs_pin = cs_pin,
        .dc_pin = dc_pin,
        .res_pin = res_pin,
        .mosi_pin = mosi_pin,
        .sclk_pin = sclk_pin};

    return oled;
}

void spi_initilize(PmodOLED *oled)
{
    spi_init(oled->spi, 5000 * 1000); // 5MHz
    gpio_set_function(oled->sclk_pin, GPIO_FUNC_SPI);
    gpio_set_function(oled->mosi_pin, GPIO_FUNC_SPI);
    gpio_init(oled->cs_pin);
    gpio_init(oled->dc_pin);
    gpio_init(oled->res_pin);
    // Set direction of GPIO pins
    gpio_set_dir(oled->cs_pin, GPIO_OUT);
    gpio_set_dir(oled->dc_pin, GPIO_OUT);
    gpio_set_dir(oled->res_pin, GPIO_OUT);
    gpio_put(oled->cs_pin, 1); // Deselect OLED
}

void send_command(PmodOLED *oled, uint8_t cmd)
{
    gpio_put(oled->dc_pin, 0); // Command mode
    gpio_put(oled->cs_pin, 0); // Select OLED
    spi_write_blocking(oled->spi, &cmd, 1);
    gpio_put(oled->cs_pin, 1); // Deselect OLED
}

void send_data(PmodOLED *oled, uint8_t *data, size_t len)
{
    gpio_put(oled->dc_pin, 1); // Data mode
    gpio_put(oled->cs_pin, 0); // Select OLED
    spi_write_blocking(oled->spi, data, len);
    gpio_put(oled->cs_pin, 1); // Deselect OLED
}

void reset_oled(PmodOLED *oled)
{
    gpio_put(oled->res_pin, 0);
    sleep_ms(100);
    gpio_put(oled->res_pin, 1);
    sleep_ms(100);
}

void init_oled(PmodOLED *oled)
{
    reset_oled(oled);

    for (size_t i = 0; i < sizeof(init_cmds); i++)
    {
        send_command(oled, init_cmds[i]);
    }
}

void clear_oled(PmodOLED *oled)
{
    uint8_t buffer[128]; // Buffer for one row (128 columns)
    memset(buffer, 0, sizeof(buffer));

    for (uint8_t i = 0; i < 4; i++)
    {                                 // 4 pages for 128x32 display (32 rows ÷ 8 = 4 pages)
        send_command(oled, 0xB0 + i); // Set page address
        send_command(oled, 0x00);     // Set lower column address
        send_command(oled, 0x10);     // Set higher column address
        send_data(oled, buffer, 128); // Clear the row
    }
}

void draw_string(PmodOLED *oled, const char *str, uint8_t page, uint8_t col)
{
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        uint8_t char_index = str[i] - ' '; // Convert character to font index (starting from space ' ')
        if (char_index < 128)
        {                                                       // Ensure the character is within the font array bounds
            send_command(oled, 0xB0 + page);                    // Set page (row)
            send_command(oled, 0x00 + (col & 0x0F));            // Lower column address
            send_command(oled, 0x10 + (col >> 4));              // Higher column address
            send_data(oled, (uint8_t *)font5x8[char_index], 5); // Send character pixels
            col += 6;                                           // Move cursor (5 pixels + 1 spacing)
        }
    }
}