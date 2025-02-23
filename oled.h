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

typedef struct
{
    spi_inst_t *spi; // SPI instance (spi0 or spi1)
    uint cs_pin;     // Chip select pin
    uint dc_pin;     // Data/Command control pin
    uint res_pin;    // Reset pin
    uint mosi_pin;   //
    uint sclk_pin;   //
} PmodOLED;

// OLED Register Definitions
#undef SPI0_BASE
#define SPI0_BASE 0x4003c000
#define SSPR0_OFFSET (volatile uint32_t *)(SPI0_BASE + 0x000)
#define SSPR1_OFFSET (volatile uint32_t *)(SPI0_BASE + 0x004)
#define SSPDR_OFFSET (volatile uint32_t *)(SPI0_BASE + 0x008)
#define SSPSR_OFFSET (volatile uint32_t *)(SPI0_BASE + 0x00c)
#define SSPCPSR_OFFSET (volatile uint32_t *)(SPI0_BASE + 0x010)

#define GPIO_BASE 0x40014000
#define GPIO18_CTRL_OFFSET (volatile uint32_t *)(GPIO_BASE + 0x094) // SCLK
#define GPIO19_CTRL_OFFSET (volatile uint32_t *)(GPIO_BASE + 0x09c) // MOSI
#define GPIO17_CTRL_OFFSET (volatile uint32_t *)(GPIO_BASE + 0x08c) // CS_1
#define GPIO6_CTRL_OFFSET (volatile uint32_t *)(GPIO_BASE + 0x034)  // CS_2
#define GPIO16_CTRL_OFFSET (volatile uint32_t *)(GPIO_BASE + 0x084) // DC
#define GPIO22_CTRL_OFFSET (volatile uint32_t *)(GPIO_BASE + 0x0b4) // RES

#undef SIO_BASE
#define SIO_BASE 0xd0000000
#define GPIO_OUT_SET_OFFSET (volatile uint32_t *)(SIO_BASE + 0x14)
#define GPIO_OUT_CLR_OFFSET (volatile uint32_t *)(SIO_BASE + 0x18)
#define GPIO_OE_SET_OFFSET (volatile uint32_t *)(SIO_BASE + 0x24)

void init_oled();
void reset_oled();

void send_command_1(uint8_t cmd);
void send_command_2(uint8_t cmd);
void send_data_1(uint8_t *data, size_t len);
void send_data_2(uint8_t *data, size_t len);
void clear_1();
void clear_2();
void draw_string_1(const char *str, uint8_t page, uint8_t col);
void draw_string_2(const char *str, uint8_t page, uint8_t col);

#endif // OLED_H