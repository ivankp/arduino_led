#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

// _BV(N) is the same as (1 << N)

#define BTN (!(PIND & _BV(PIND6)))

int main(void) {
  DDRD |=  _BV(DDD7); // D7 is LED
  DDRD &= ~_BV(DDD6); // D6 is button

  PORTD &= ~_BV(PORTD7); // LED off

  bool button_state = false;
  bool led_state = false;

  for (;;) {
    if (!button_state && BTN) {

      button_state = true;
      led_state = !led_state;

      if (led_state) PORTD |= _BV(PORTD7); // LED on
      else PORTD &= ~_BV(PORTD7); // LED off

      _delay_ms(1000);

    } else if (button_state && !BTN) {
      button_state = false;
    }
  }
}
