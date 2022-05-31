#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/mcpwm.h"
 
#define GPIO_PWM 33

// You can get these value from the datasheet of servo you use, in general pulse width varies between 1000 to 2000 mocrosecond
#define SERVO_MIN_PULSEWIDTH_US (1000) // Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH_US (2000) // Maximum pulse width in microsecond
#define SERVO_MAX_DEGREE        (90)   // Maximum angle in degree upto which servo can rotate

static const char *TAG = "pwm";

void ledc_pwm(void *pvParameter) {
    int bitSize         = 15;
	int minValue        = 500;  // micro seconds (uS)
	int maxValue        = 2500; // micro seconds (uS)
	int sweepDuration   = 4000; // milliseconds (ms)
	int duty            = (1<<bitSize) * minValue / 20000 ;//1638
	int direction       = 1; // 1 = up, -1 = down
	int valueChangeRate = 20; // msecs


    ledc_timer_config_t timer_conf;
    timer_conf.duty_resolution = LEDC_TIMER_15_BIT;
    timer_conf.freq_hz = 50;
    timer_conf.speed_mode = LEDC_HIGH_SPEED_MODE;
    timer_conf.timer_num = LEDC_TIMER_0;
    ledc_timer_config(&timer_conf);

    ledc_channel_config_t channel_conf;
    channel_conf.channel = LEDC_CHANNEL_0;
    channel_conf.duty = duty;
    channel_conf.gpio_num = GPIO_PWM;
    channel_conf.intr_type = LEDC_INTR_DISABLE;
    channel_conf.speed_mode = LEDC_HIGH_SPEED_MODE;
    channel_conf.timer_sel = LEDC_TIMER_0;
    ledc_channel_config(&channel_conf);

    int changesPerSweep = sweepDuration / valueChangeRate;// 1500/20 -> 75
	int changeDelta = (maxValue-minValue) / changesPerSweep;// 2000/75 -> 26
	int i;

    while(1) {
		for (i=0; i<changesPerSweep; i++) {
			if (direction > 0) {
				duty += changeDelta;
			} else {
				duty -= changeDelta;
			}
			ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, duty);
			ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
			vTaskDelay(valueChangeRate/portTICK_PERIOD_MS);
		}
		direction = -direction;
	} 

}

static inline uint32_t example_convert_servo_angle_to_duty_us(int angle)
{
    return (angle + SERVO_MAX_DEGREE) * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) / (2 * SERVO_MAX_DEGREE) + SERVO_MIN_PULSEWIDTH_US;
}

void mcpwm_servo(void * vParamaters) {
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_PWM); // To drive a RC servo, one MCPWM generator is enough

    mcpwm_config_t pwm_config = {
        .frequency = 50, // frequency = 50Hz, i.e. for every servo motor time period should be 20ms
        .cmpr_a = 0,     // duty cycle of PWMxA = 0
        .counter_mode = MCPWM_UP_COUNTER,
        .duty_mode = MCPWM_DUTY_MODE_0,
    };
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);

    while (1) {
        for (int angle = -SERVO_MAX_DEGREE; angle < SERVO_MAX_DEGREE; angle++) {
            ESP_LOGI(TAG, "Angle of rotation: %d", angle);
            ESP_ERROR_CHECK(mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, example_convert_servo_angle_to_duty_us(angle)));
            vTaskDelay(pdMS_TO_TICKS(100)); //Add delay, since it takes time for servo to rotate, generally 100ms/60degree rotation under 5V power supply
        }
    }
}
 
void app_main() {

    // xTaskCreate(&ledc_pwm, "ledc_pwm", 2048, NULL, 5, NULL);
    xTaskCreate(&mcpwm_servo, "mcpwm_servo", 2048, NULL, 5, NULL);
}