#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 1000

int main(void) {
  DDRB |= _BV(DDB5); // set pin 5 of PORTB for output

  for (;;) {
    PORTB |= _BV(PORTB5); // set pin 5 high to turn led on
    _delay_ms(BLINK_DELAY_MS);

    PORTB &= ~_BV(PORTB5); // set pin 5 low to turn led off
    _delay_ms(BLINK_DELAY_MS);
  }
}
