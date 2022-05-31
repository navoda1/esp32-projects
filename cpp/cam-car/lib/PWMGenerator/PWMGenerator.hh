#ifndef __PWM_GENERATOR_H__
#define __PWM_GENERATOR_H__

#include "esp_log.h"
#include "driver/mcpwm.h"

class PWMGenerator {

    public:
        PWMGenerator(
            mcpwm_unit_t mcpwm_unit, 
            mcpwm_operator_t generator, 
            mcpwm_io_signals_t mcpwm_io_signal, 
            mcpwm_timer_t timer, 
            uint8_t pin, 
            uint32_t freq
        );
        ~PWMGenerator();

        void set_duty_cycle_per(float percentage);
        void set_duty_cycle_us(uint32_t us);

    private:
        mcpwm_unit_t _mcpwm_unit;
        mcpwm_io_signals_t _mcpwm_io_signal;
        mcpwm_timer_t _mcpwm_timer;
        mcpwm_operator_t _mcpwm_generator;

        uint8_t _pin;
        uint32_t _freq;
};

#endif /* __PWM_GENERATOR_H__ */