# HiFive1-PWM_interrupt

Demonstrate using the PWM module on SiFive's FE310 chip, as used on the HiFive1 dev board, to generate regular timer interrupts.

## Results

Trying to get a feel for what's a reasonable usage of the PWM module as an internal interrupt source. We can calculate an intended interrupt frequency via:

(Clock source / 2^divisor) / PWM comparator = frequency (Hz)

See the [Freedom E300 Platform Reference Manual](https://www.sifive.com/documentation/freedom-soc/freedom-e300-platform-reference-manual/) for more information about the PWM module. 

To test, this program toggles a GPIO pin in the ISR. There are other effects in play than the PWM interrupt firing but we should be able to measure the pin oscillating roughly in line with the following calculations:

Top row: divisor (PWM prescaler). Column: PWM comparator value. 

|       |  0         |  1  |  10 | 
| ---   |    ---     | --- | --- |
| 10000 | 38.17 uS (26,200 Hz)  | 76.33 uS (13,200 Hz)    | 39062.5 uS (25.6 Hz) |
| 1000  | 3.817 uS (262,000 Hz) | 7.633 uS (132,000 Hz) | 3906.25 uS (256Hz) |
| 100   | 0.3168 uS (2,620,000 Hz) | 0.7633 uS (1,320,000 Hz) | 390.625 uS (2,560 Hz) |
| 10    | 0.03168 uS (26,200,000 Hz) | 0.07633 uS (13,200,000 Hz) | 39.0625 uS (25,600 Hz) |
