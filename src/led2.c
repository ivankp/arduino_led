#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  DDRD |= _BV(DDD7);

  for (unsigned i=0; ; ++i, _delay_ms(50)) {
    if (!(i%20)) {
      PORTD |= _BV(PORTD7);
      i = 0;
    } else {
      PORTD &= ~_BV(PORTD7);
    }
  }
}
