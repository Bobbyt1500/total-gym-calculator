#include "sevenSeg.h"

#include <stdint.h>
#include <util/delay.h>

// 0bGFEDCBA0 
// 0 is on - 1 is off
int char_mapping[10] = {
    0b10000000,
    0b11110010,
    0b01001000,
    0b01100000,
    0b00110010,
    0b00100100,
    0b00000100,
    0b11110000,
    0b00000000,
    0b00100000
};

void displayNum(int num) {
    // Pointer to Port D Register
    uint8_t *pd = 0x2B;
    // Pointer to Port B Register
    uint8_t *pb = 0x25;

    int display_buf[4] = {-1, -1, -1, -1};
    
    int i = 0;
    while (num > 0) {
        display_buf[i++] = num%10;
        num /= 10;
    }

    
    for (int i = 0; i < 4; ++i) {
        if (display_buf[i] == -1) break;
        // Set pins to low
        *pb &= ~(0b00011110);

        // Set the correct digit pin to high
        *pb |= (1 << (4 - i));

        // Write the correct character to the digit
        *pd = char_mapping[display_buf[i]];

        _delay_ms(5);
    }
    
    
    *pb &= ~(0b00011110);
    
}

void initDisplay() {
    // Pointer to Data Direction Register D
    uint8_t *ddrd = 0x2A;
    // Pointer to Port D Register
    uint8_t *pd = 0x2B;

    // Pointer to DDRB
    uint8_t *ddrb = 0x24;

    // Set all of the used pins on PORTD to outputs
    *ddrd = 0b11111110;

    // Set all of the used pins on PORTB to outputs
    *ddrb |= 0b00011110;

    // Set all segment pins to high initially
    *pd = 0b11111110;
}