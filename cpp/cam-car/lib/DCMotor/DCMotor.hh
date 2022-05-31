#ifndef __DC_MOTOR_H__
#define __DC_MOTOR_H__

#include <stdint.h>

#include "PWMGenerator.hh"

class DCMotor {

    public:
        DCMotor(PWMGenerator *pwm_generator, uint8_t pin_EN, uint8_t pin_A, uint8_t pin_B);
        ~DCMotor();

        void set_speed(int8_t speed);

    private:
        const char *_tag = "DCMotor";
        PWMGenerator *_pwm_generator;
        uint8_t _pin_A;
        uint8_t _pin_B;
        uint8_t _pin_EN;

};

#endif /* __DC_MOTOR_H__ */