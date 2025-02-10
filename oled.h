#ifndef OLED_H
#define OLED_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <hardware/gpio.h>
#include <hardware/irq.h>
#include "hardware/uart.h"
#include <string.h>
#include "hardware/spi.h"
#include "font.h"

typedef struct {
    spi_inst_t *spi;  // SPI instance (spi0 or spi1)
    uint cs_pin;      // Chip select pin
    uint dc_pin;      // Data/Command control pin
    uint res_pin;     // Reset pin
    uint mosi_pin;      //
    uint sclk_pin;     // 
} PmodOLED;

// OLED Register Definitions
#undef SPI0_BASE
#define SPI0_BASE 0x4003c000
#define SSPR0_OFFSET (volatile uint32_t*)(SPI0_BASE + 0x000)
#define SSPR1_OFFSET (volatile uint32_t*)(SPI0_BASE + 0x004)
#define SSPDR_OFFSET (volatile uint32_t*)(SPI0_BASE + 0x008)
#define SSPSR_OFFSET (volatile uint32_t*)(SPI0_BASE + 0x00c)
#define SSPCPSR_OFFSET (volatile uint32_t*)(SPI0_BASE + 0x010)

#define GPIO_BASE 0x40014000
#define GPIO18_CTRL_OFFSET (volatile uint32_t*)(GPIO_BASE + 0x094)  // SCLK
#define GPIO19_CTRL_OFFSET (volatile uint32_t*)(GPIO_BASE + 0x09c)  // MOSI
#define GPIO17_CTRL_OFFSET (volatile uint32_t*)(GPIO_BASE + 0x08c)  // CS_1
#define GPIO6_CTRL_OFFSET (volatile uint32_t*)(GPIO_BASE + 0x034)   // CS_2
#define GPIO16_CTRL_OFFSET (volatile uint32_t*)(GPIO_BASE + 0x084)  // DC
#define GPIO22_CTRL_OFFSET (volatile uint32_t*)(GPIO_BASE + 0x0b4)  // RES

#undef SIO_BASE
#define SIO_BASE 0xd0000000
#define GPIO_OUT_SET_OFFSET (volatile uint32_t*)(SIO_BASE + 0x14)
#define GPIO_OUT_CLR_OFFSET (volatile uint32_t*)(SIO_BASE + 0x18)
#define GPIO_OE_SET_OFFSET (volatile uint32_t*)(SIO_BASE + 0x24)


// Function Declarations
PmodOLED create_oled(spi_inst_t *spi, uint cs_pin, uint dc_pin, uint res_pin, uint mosi_pin, uint sclk_pin);
void spi_initilize(PmodOLED *oled);
void send_command(PmodOLED *oled, uint8_t cmd);
void send_data(PmodOLED *oled, uint8_t *data, size_t len);
void draw_string(PmodOLED *oled, const char *str, uint8_t page, uint8_t col);
void reset_oled(PmodOLED *oled);
void init_oled(PmodOLED *oled);
void clear_oled(PmodOLED *oled);

#endif // OLED_H