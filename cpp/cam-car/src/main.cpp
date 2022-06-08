#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_event.h"

#include "defines.hh"
#include "WifiHandler.hh"
#include "HTTPServer.hh"
#include "PWMGenerator.hh"
#include "ServoMotor.hh"
#include "DCMotor.hh"

extern "C" {
    void app_main(void);
}

const char *tag = "Main";

void app_init() {
    // Initialize the Non-Volatile Storage for storing configurations
    ESP_ERROR_CHECK(nvs_flash_init());

    // Initialize NETIF layer
    ESP_ERROR_CHECK(esp_netif_init());

    // Initialize the default event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());
}

void app_main() {

    status_t status = success; 

    app_init();

    // Initialize the WifiHandler and connect to Wifi
    WifiHandler wifi_handler = WifiHandler();
    status = wifi_handler.Connect("Not a 2G Wifi!", "connectanyway");

    // No point in continuing if Wifi connection fails
    if (status == failure) {
        return;
    }

    // Start the HTTP server
    HTTPServer server = HTTPServer();
    ESP_ERROR_CHECK(server.start());

    // PWMGenerator* pwm_gen_dc_m_left = new PWMGenerator(
    //     DC_M_LEFT_MCPWM_UNIT,
    //     DC_M_LEFT_MCPWM_GENERATOR,
    //     DC_M_LEFT_MCPWM_SIGNAL,
    //     DC_M_LEFT_MCPWM_TIMER,
    //     GPIO_DC_M_LEFT_EN,
    //     DC_M_PWM_FREQ
    // );

    // PWMGenerator* pwm_gen_dc_m_right = new PWMGenerator(
    //     DC_M_RIGHT_MCPWM_UNIT,
    //     DC_M_RIGHT_MCPWM_GENERATOR,
    //     DC_M_RIGHT_MCPWM_SIGNAL,
    //     DC_M_RIGHT_MCPWM_TIMER,
    //     GPIO_DC_M_RIGHT_EN,
    //     DC_M_PWM_FREQ
    // );

    // PWMGenerator* pwm_gen_servo_m_horiz = new PWMGenerator(
    //     SERVO_M_HORIZ_MCPWM_UNIT,
    //     SERVO_M_HORIZ_MCPWM_GENERATOR,
    //     SERVO_M_HORIZ_MCPWM_SIGNAL,
    //     SERVO_M_HORIZ_MCPWM_TIMER,
    //     GPIO_SERVO_M_HORIZ_PIN,
    //     SERVO_M_PWM_FREQ
    // );

    // PWMGenerator* pwm_gen_servo_m_vert = new PWMGenerator(
    //     SERVO_M_VERT_MCPWM_UNIT,
    //     SERVO_M_VERT_MCPWM_GENERATOR,
    //     SERVO_M_VERT_MCPWM_SIGNAL,
    //     SERVO_M_VERT_MCPWM_TIMER,
    //     GPIO_SERVO_M_VERT_PIN,
    //     SERVO_M_PWM_FREQ
    // );

    // DCMotor dc_motor_left = DCMotor(pwm_gen_dc_m_left, GPIO_DC_M_LEFT_EN, GPIO_DC_M_LEFT_A, GPIO_DC_M_LEFT_B);
    // DCMotor dc_motor_right = DCMotor(pwm_gen_dc_m_right, GPIO_DC_M_RIGHT_EN, GPIO_DC_M_RIGHT_A, GPIO_DC_M_RIGHT_B);

    // ServoMotor servo_horiz = ServoMotor(pwm_gen_servo_m_horiz, GPIO_SERVO_M_HORIZ_PIN);
    // ServoMotor servo_vert = ServoMotor(pwm_gen_servo_m_vert, GPIO_SERVO_M_VERT_PIN);

    // servo_vert.set_angle(SERVO_M_VERT_ANGLE_MID);
    // servo_horiz.set_angle(SERVO_M_HORIZ_ANGLE_MID);

    // while (1) {

    //     // forward
    //     dc_motor_left.set_speed(85);
    //     dc_motor_right.set_speed(85);
    //     servo_vert.set_angle(SERVO_M_VERT_ANGLE_MID);
    //     vTaskDelay(pdMS_TO_TICKS(2000));

    //     // stop dc, move servo
    //     dc_motor_left.set_speed(0);
    //     dc_motor_right.set_speed(0);
    //     servo_vert.set_angle(SERVO_M_VERT_ANGLE_UP);
    //     servo_horiz.set_angle(-10);
    //     vTaskDelay(pdMS_TO_TICKS(2000));
        
    //     // backward
    //     dc_motor_left.set_speed(-85);
    //     dc_motor_right.set_speed(-85);
    //     servo_vert.set_angle(SERVO_M_VERT_ANGLE_MID);
    //     servo_vert.set_angle(65); // vertical
    //     vTaskDelay(pdMS_TO_TICKS(2000));
        
    //     // stop dc, move servo
    //     dc_motor_left.set_speed(0);
    //     dc_motor_right.set_speed(0);
    //     servo_horiz.set_angle(10);
    //     servo_vert.set_angle(SERVO_M_VERT_ANGLE_DOWN);
    //     vTaskDelay(pdMS_TO_TICKS(2000));
    // }
    
}