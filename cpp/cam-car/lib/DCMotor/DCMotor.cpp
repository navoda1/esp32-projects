
#include "driver/mcpwm.h"
#include "DCMotor.hh"

DCMotor::DCMotor(PWMGenerator *pwm_generator, uint8_t pin_EN, uint8_t pin_A, uint8_t pin_B) {
    // Set local parameter
    _pin_EN = pin_EN;
    _pin_A = pin_A;
    _pin_B = pin_B;
    _pwm_generator = pwm_generator;

    // Initialize GPIO pins 
    gpio_pad_select_gpio(_pin_A);
    gpio_pad_select_gpio(_pin_B);

    gpio_set_direction((gpio_num_t) _pin_A, GPIO_MODE_OUTPUT);
    gpio_set_direction((gpio_num_t) _pin_B, GPIO_MODE_OUTPUT);

    gpio_set_level((gpio_num_t) _pin_A, 0);
    gpio_set_level((gpio_num_t) _pin_B, 0);

}

/* Set speed of the motor, with values between -100 and 100. Negative: backward, Postive: forward */
void DCMotor::set_speed(int8_t speed) {
    
    uint8_t abs_speed = std::abs(speed);

    /* Limit speed to 100% */
    if (abs_speed > 100) {
        abs_speed = 100;
    }

    /* Set speed */
    _pwm_generator->set_duty_cycle_per(abs_speed);

    /* Set direction */
    if (speed > 0) {
        gpio_set_level((gpio_num_t) _pin_A, 1);
        gpio_set_level((gpio_num_t) _pin_B, 0);
    }
    else if (speed < 0) {
        gpio_set_level((gpio_num_t) _pin_A, 0);
        gpio_set_level((gpio_num_t) _pin_B, 1);
    }
    else {
        gpio_set_level((gpio_num_t) _pin_A, 0);
        gpio_set_level((gpio_num_t) _pin_B, 0);
    }
}