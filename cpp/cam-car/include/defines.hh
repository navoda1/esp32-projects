#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#include <stdint.h>

typedef enum {
    success,
    failure
} status_t;

typedef enum {
    dc,
    servo
} motor_type_t;


/* GPIO Pins */
// Left DC motor
// Right DC motor
// Horizontal Servo
// Vertical Servo
// Other
#define GPIO_CAM_FLASH                  4

/* DC motor config */
#define DC_M_PWM_FREQ                   30000
// Left DC Motor
#define GPIO_DC_M_LEFT_EN               2
#define GPIO_DC_M_LEFT_A                15
#define GPIO_DC_M_LEFT_B                14
#define DC_M_LEFT_MCPWM_UNIT            MCPWM_UNIT_0      
#define DC_M_LEFT_MCPWM_GENERATOR       MCPWM_GEN_A
#define DC_M_LEFT_MCPWM_SIGNAL          MCPWM0A
#define DC_M_LEFT_MCPWM_TIMER           MCPWM_TIMER_0
// Right DC Motor
#define GPIO_DC_M_RIGHT_EN              3
#define GPIO_DC_M_RIGHT_A               13
#define GPIO_DC_M_RIGHT_B               12
#define DC_M_RIGHT_MCPWM_UNIT           MCPWM_UNIT_0      
#define DC_M_RIGHT_MCPWM_GENERATOR      MCPWM_GEN_B
#define DC_M_RIGHT_MCPWM_SIGNAL         MCPWM0B
#define DC_M_RIGHT_MCPWM_TIMER          MCPWM_TIMER_0

/* Servo config */
#define SERVO_M_PWM_FREQ                50
#define SERVO_M_MAX_DEGREES             90
// Horizontal Servo
#define GPIO_SERVO_M_HORIZ_PIN          1
#define SERVO_M_HORIZ_MCPWM_UNIT        MCPWM_UNIT_1
#define SERVO_M_HORIZ_MCPWM_GENERATOR   MCPWM_GEN_A
#define SERVO_M_HORIZ_MCPWM_SIGNAL      MCPWM0A
#define SERVO_M_HORIZ_MCPWM_TIMER       MCPWM_TIMER_0
#define SERVO_M_HORIZ_ANGLE_MID         0
// Vertical Servo
#define GPIO_SERVO_M_VERT_PIN           16
#define SERVO_M_VERT_MCPWM_UNIT         MCPWM_UNIT_1
#define SERVO_M_VERT_MCPWM_GENERATOR    MCPWM_GEN_B
#define SERVO_M_VERT_MCPWM_SIGNAL       MCPWM0B
#define SERVO_M_VERT_MCPWM_TIMER        MCPWM_TIMER_0
#define SERVO_M_VERT_ANGLE_MID          70
#define SERVO_M_VERT_ANGLE_UP           50
#define SERVO_M_VERT_ANGLE_DOWN         90

#endif /* __TYPEDEF_H__ */