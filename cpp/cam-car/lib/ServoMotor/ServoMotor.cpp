#include "ServoMotor.hh"

ServoMotor::ServoMotor(PWMGenerator *pwm_generator, int8_t gpio_pin) {

    /* Set local config */
    _gpio_pin = gpio_pin;
    _pwm_generator = pwm_generator;
}

/* Set servo angle to a value between -90 and +90 degrees */
int ServoMotor::set_angle(int8_t angle) {
    
    if (angle < -SERVO_M_MAX_DEGREES || angle > SERVO_M_MAX_DEGREES) {
        ESP_LOGE(_tag, "Invalid servo motor angle: %d\n", angle);
        return -1;
    }

    _pwm_generator->set_duty_cycle_us(angle_to_duty_us(angle));

    return 0;
}

/* Get duty cycle (ms) from angle */
uint32_t ServoMotor::angle_to_duty_us(int8_t angle) {
    return (angle + _c_max_degrees) * (_c_max_pulse_width_us - _c_min_pulse_width_us) / (2 * _c_max_degrees) + _c_min_pulse_width_us;
}