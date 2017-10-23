#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

// _BV(N) is the same as (1 << N)

#define BTN (!(PIND & _BV(PIND6)))

bool button() {
  static bool button_state = true;
  if (!button_state && BTN) {
    _delay_ms(10);
    if (BTN) button_state = true;
    return true;
  } else if (button_state && !BTN) {
    button_state = false;
  }
  return false;
}

int main(void) {
  DDRD |=  _BV(DDD7); // D7 is LED
  DDRD &= ~_BV(DDD6); // D6 is button

  bool led_state = false;

  for (;;) {
    if (button())
      led_state = !led_state;

    if (led_state) PORTD |= _BV(PORTD7); // LED on
    else PORTD &= ~_BV(PORTD7); // LED off
  }
}
