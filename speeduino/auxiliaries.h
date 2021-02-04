#ifndef AUX_H
#define AUX_H

#include "globals.h"

void initialiseAuxPWM();
void boostControl();
void boostDisable();
void idleControl();
void vvtControl();
void initialiseFan();
void nitrousControl();
void fanControl();
void wmiControl();

#define SIMPLE_BOOST_P  1
#define SIMPLE_BOOST_I  1
#define SIMPLE_BOOST_D  1

#define BOOST_PIN_LOW()  *boost_pin_port &= ~(boost_pin_mask)
#define BOOST_PIN_HIGH() *boost_pin_port |= (boost_pin_mask)
#define VVT1_PIN_LOW()    *vvt1_pin_port &= ~(vvt1_pin_mask)
#define VVT1_PIN_HIGH()   *vvt1_pin_port |= (vvt1_pin_mask)
#define VVT2_PIN_LOW()    *vvt2_pin_port &= ~(vvt2_pin_mask)
#define VVT2_PIN_HIGH()   *vvt2_pin_port |= (vvt2_pin_mask)
#define VVT1_PIN_ON()     if (configPage6.vvtPWMdir == 0) { VVT1_PIN_HIGH(); } else { VVT1_PIN_LOW(); } 
#define VVT1_PIN_OFF()    if (configPage6.vvtPWMdir == 0) { VVT1_PIN_LOW() ; } else { VVT1_PIN_HIGH(); }
//#define VVT2_PIN_ON()     if (configPage6.vvtPWMdir == 0) { VVT2_PIN_HIGH(); } else { VVT2_PIN_LOW(); } 
//#define VVT2_PIN_OFF()    if (configPage6.vvtPWMdir == 0) { VVT2_PIN_LOW() ; } else { VVT2_PIN_HIGH(); }
#define VVT2_PIN_ON()     {} //Disabled as VVT2 not yet complete
#define VVT2_PIN_OFF()    {} //Disabled as VVT2 not yet complete
#define FAN_PIN_LOW()    *fan_pin_port &= ~(fan_pin_mask)
#define FAN_PIN_HIGH()   *fan_pin_port |= (fan_pin_mask)
#define N2O_STAGE1_PIN_LOW()  *n2o_stage1_pin_port &= ~(n2o_stage1_pin_mask)
#define N2O_STAGE1_PIN_HIGH() *n2o_stage1_pin_port |= (n2o_stage1_pin_mask)
#define N2O_STAGE2_PIN_LOW()  *n2o_stage2_pin_port &= ~(n2o_stage2_pin_mask)
#define N2O_STAGE2_PIN_HIGH() *n2o_stage2_pin_port |= (n2o_stage2_pin_mask)
#define READ_N2O_ARM_PIN()    ((*n2o_arming_pin_port & n2o_arming_pin_mask) ? true : false)

#define FAN_ON()         ((configPage6.fanInv) ? FAN_PIN_LOW() : FAN_PIN_HIGH())
#define FAN_OFF()        ((configPage6.fanInv) ? FAN_PIN_HIGH() : FAN_PIN_LOW())

#define WMI_TANK_IS_EMPTY() ((configPage10.wmiEmptyEnabled) ? ((configPage10.wmiEmptyPolarity) ? !digitalRead(pinWMIEmpty) : digitalRead(pinWMIEmpty)) : 0)

extern volatile PORT_TYPE *boost_pin_port;
extern volatile PINMASK_TYPE boost_pin_mask;
extern volatile PORT_TYPE *vvt1_pin_port;
extern volatile PINMASK_TYPE vvt1_pin_mask;
extern volatile PORT_TYPE *vvt2_pin_port;
extern volatile PINMASK_TYPE vvt2_pin_mask;
extern volatile PORT_TYPE *fan_pin_port;
extern volatile PINMASK_TYPE fan_pin_mask;
extern volatile PORT_TYPE *n2o_stage1_pin_port;
extern volatile PINMASK_TYPE n2o_stage1_pin_mask;
extern volatile PORT_TYPE *n2o_stage2_pin_port;
extern volatile PINMASK_TYPE n2o_stage2_pin_mask;
extern volatile PORT_TYPE *n2o_arming_pin_port;
extern volatile PINMASK_TYPE n2o_arming_pin_mask;

extern volatile bool boost_pwm_state;
extern unsigned int boost_pwm_max_count; //Used for variable PWM frequency
extern volatile unsigned int boost_pwm_cur_value;
extern long boost_pwm_target_value;
extern long boost_cl_target_boost;
extern byte boostCounter;
extern byte vvtCounter;

extern byte fanHIGH;             // Used to invert the cooling fan output
extern byte fanLOW;               // Used to invert the cooling fan output

extern volatile bool vvt1_pwm_state;
extern volatile bool vvt2_pwm_state;
extern volatile bool vvt1_max_pwm;
extern volatile bool vvt2_max_pwm;
extern volatile char nextVVT;
extern unsigned int vvt_pwm_max_count; //Used for variable PWM frequency
extern volatile unsigned int vvt1_pwm_cur_value;
extern volatile unsigned int vvt2_pwm_cur_value;
extern long vvt1_pwm_value;
extern long vvt2_pwm_value;
extern long vvt_pid_target_angle;
//long vvt_pid_current_angle;
void boostInterrupt();
void vvtInterrupt();


#endif
