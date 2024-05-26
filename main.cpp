#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

int Time_Period_of_Wave = 33;
int number_of_steps_to_devide_pulse = 20;
int duty_cycle = Time_Period_of_Wave / number_of_steps_to_devide_pulse; // For a wave of 30 ms T.P., increase the duty cycle by 10%
int i;

void pwm_init() {
	// Set PWM pins (OC1A and OC1B) as output
	DDRB |= (1 << DDB1) | (1 << DDB2);
	// Fast PWM mode with ICR1 as TOP, non-inverted output
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS10);
	ICR1 = Time_Period_of_Wave; // Set TOP value for 33kHz frequency, this value equals the time period
}

int main(void) {
	DDRB &= ~(1 << DDB0);
	PORTB |= (1 << PORTB0);
	DDRC = 0xFF;
	PORTC &= ~(1 << PORTC0); // Output for in2
	PORTC &= ~(1 << PORTC1); // Output for in4
	PORTC &= ~(1 << PORTC2); // Output for buzzer
	i = 0; // Initial duty cycle for 50% speed
	pwm_init(); // Initialize PWM
	DDRD &= ~(1 << DDD2);
	PORTD |= (1 << PORTD2);
	DDRD &= ~(1 << DDD3);
	PORTD |= (1 << PORTD3);

	// Falling edge of INT0 and INT1 generates an interrupt request
	EICRA = (1 << ISC01) | (1 << ISC11);

	// Enable external interrupts INT0 and INT1
	EIMSK = (1 << INT0) | (1 << INT1);

	sei();  // Enable global interrupts

	while (1) {
		OCR1A = i; // Set PWM duty cycle for OCR1A
		OCR1B = i * 2; // Set PWM duty cycle for OCR1B to be 30% more than OCR1A
		// _delay_ms(100); // Delay to observe the changes in speed if the time period is in ms
		_delay_us(5); // Delay to observe the changes in speed if the time period is in us
	}

	return 0;
}

// Interrupt Service Routine for INT0
ISR(INT0_vect) {
	if (i >= 0) {
		PORTC &= ~(1 << PORTC0);
		PORTC &= ~(1 << PORTC1);
		PORTC &= ~(1 << PORTC2);
		i += duty_cycle; // Increase duty cycle for higher speed
		} else if (i < 0) {
		PORTC |= (1 << PORTC0);
		PORTC |= (1 << PORTC1);
		PORTC |= (1 << PORTC2);
		i += duty_cycle; // Increase duty cycle for higher speed
		} else {
		i += duty_cycle; // Increase duty cycle for higher speed
	}

	if (i == ((Time_Period_of_Wave + duty_cycle) / 2)) {
		PORTC |= (1 << PORTC2);
		} else if (i >= (Time_Period_of_Wave + duty_cycle)) {
		PORTC |= (1 << PORTC2);
		i = Time_Period_of_Wave;
	}
}

// Interrupt Service Routine for INT1
ISR(INT1_vect) {
	i -= duty_cycle; // Decrease duty cycle for lower speed
	if (i < duty_cycle) {
		PORTC |= (1 << PORTC2);
		i = 0;
		} else if (i <= Time_Period_of_Wave) {
		PORTC &= ~(1 << PORTC2);
	}
}
