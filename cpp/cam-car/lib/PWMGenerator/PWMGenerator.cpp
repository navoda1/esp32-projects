
#include "PWMGenerator.hh"

PWMGenerator::PWMGenerator(
    mcpwm_unit_t mcpwm_unit, 
    mcpwm_operator_t mcpwm_generator, 
    mcpwm_io_signals_t mcpwm_io_signal, 
    mcpwm_timer_t mcpwm_timer, 
    uint8_t pin, 
    uint32_t freq
) {
    _mcpwm_unit = mcpwm_unit;
    _mcpwm_generator = mcpwm_generator;
    _mcpwm_io_signal = mcpwm_io_signal;
    _mcpwm_timer = mcpwm_timer;
    _pin = pin;
    _freq = freq;
    
    mcpwm_gpio_init(mcpwm_unit, mcpwm_io_signal, pin);

    mcpwm_config_t pwm_config = {
        .frequency = freq,
        .cmpr_a = 0,     // duty cycle of PWMxA = 0
        .cmpr_b = 0,     // duty cycle of PWMxB = 0
        .duty_mode = MCPWM_DUTY_MODE_0,
        .counter_mode = MCPWM_UP_COUNTER,
    };

    mcpwm_init(mcpwm_unit, mcpwm_timer, &pwm_config);
}

PWMGenerator::~PWMGenerator() {}

void PWMGenerator::set_duty_cycle_per(float percentage) {
    ESP_ERROR_CHECK(
        mcpwm_set_duty(_mcpwm_unit, _mcpwm_timer, _mcpwm_generator, percentage)
    );
}

void PWMGenerator::set_duty_cycle_us(uint32_t us) {
    ESP_ERROR_CHECK(
        mcpwm_set_duty_in_us(_mcpwm_unit, _mcpwm_timer, _mcpwm_generator, us)
    );
}
