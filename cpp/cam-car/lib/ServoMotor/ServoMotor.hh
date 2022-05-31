#ifndef __SERVO_MOTOR_H__
#define __SERVO_MOTOR_H__

#include <stdint.h>

#include "esp_log.h"

#include "defines.hh"
#include "PWMGenerator.hh"

class ServoMotor {

    public:
        ServoMotor(PWMGenerator *pwm_generator, int8_t gpio_pin);
        ~ServoMotor();

        int set_angle(int8_t angle);

    private:
        /* Local config */
        const char *_tag = "ServoMotor";
        const int _c_min_pulse_width_us = 200;
        const int _c_max_pulse_width_us = 2500;
        const int8_t _c_max_degrees = 90;
        PWMGenerator *_pwm_generator;
        int8_t _gpio_pin;

        inline uint32_t angle_to_duty_us(int8_t angle);

};

#endif /* __SERVO_MOTOR_H__ */