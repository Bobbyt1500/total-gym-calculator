#include <avr/io.h>
#include <util/delay.h>

#include "sevenSeg.h"

enum COLOR {
    RED,
    GREEN,
    BLUE
};

enum STEPS {
    WEIGHTSTEP,
    LEVELSTEP,
    SHOWSTEP
};

void ADC_INIT() {
  ADMUX = 0b01000001;
  ADCSRA = 0b10000111;
}

int ADC_READ() {
  ADCSRA |= 0b01000000;
  while ((ADCSRA & 0b01000000) != 0);
  return ADC;
}

void setRGB(int color) {
  PORTC &= ~(0b00011100);
  PORTC |= (1 << (color + 2));
}

int buttonPushed() {
  if (!(PINB & 1)) {
    // Debounce
    _delay_ms(50);
    if (!(PINB & 1)) return 1;
  }
  return 0;
}

int main() {

  // Set RGB Pins as output
  DDRC |= 0b00000100;
  // Set PB0 internal pullup resistor
  PORTB = 0b00000001;

  initDisplay();
  ADC_INIT();

  int weight, level = 0;
  int cur_step = WEIGHTSTEP;

  while (1) {

    if (buttonPushed()) {
        ++cur_step;
        _delay_ms(1000);
    }

    if (cur_step == WEIGHTSTEP) {

      setRGB(RED);

      displayNum(weight);
      weight = (ADC_READ() * 0.19) + 100;
      
    } else if (cur_step == LEVELSTEP) {

      setRGB(BLUE);

      displayNum(level);
      level = (ADC_READ() * 0.013) + 1;

    } else if (cur_step == SHOWSTEP) {

      setRGB(GREEN);

      int height = (2.05 * level) + 12;
      // Weight lifted = (height of level / length of hypotenuse) * body weight
      float weight_lifted = (height / 92.0) * weight;

      displayNum((int) weight_lifted);

    } else {
      // Return to weight step if not in any of the other defined steps
      cur_step = WEIGHTSTEP;
    }
    
  }


  return 0;
}