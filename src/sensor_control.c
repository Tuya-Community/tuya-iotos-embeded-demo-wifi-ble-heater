/*
 * @Author: zgw
 * @email: liang.zhang@tuya.com
 * @LastEditors: zgw
 * @file name: sensor_control.c
 * @Description: 
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-1-5 16:21:15
 * @LastEditTime: 2021-1-5 16:21:15
 */

#include "sensor_control.h"
#include "sht21.h"
#include "tuya_gpio.h"

/***********************************************************
*************************types define***********************
***********************************************************/
typedef enum
{
    LOW = 0,
    HIGH,
}default_level;



DEVICE_DATA_T device_data = {
    .temperature = 0,
    .max_temp = 30,
    .min_temp = 20,
    .dev_switch = FALSE
};

APP_REPORT_DATA_T app_report_data = {0};

/***********************************************************
*************************IO control device define***********
***********************************************************/
#define IIC_SDA_PORT                     (6)
#define IIC_SCL_PORT                     (7)


#define HEATING_ROD_PORT                    (8)
#define HEATING_ROD_LEVEL                   LOW

#define COOL_DOWN_FAN_PORT                  (9)
#define COOL_DOWN_FAN_LEVEL                 LOW
/***********************************************************
*************************about adc init*********************
***********************************************************/



/***********************************************************
*************************about iic init*********************
***********************************************************/
STATIC sht21_init_t sht21_int_param = {IIC_SDA_PORT, IIC_SCL_PORT, SHT2x_RES_10_13BIT};


/***********************************************************
*************************function***************************
***********************************************************/

STATIC VOID __ctrl_gpio_init(CONST TY_GPIO_PORT_E port, CONST BOOL_T high)
{
    tuya_gpio_inout_set(port, FALSE);
    tuya_gpio_write(port, high);
}

VOID app_device_init(VOID)
{
    INT_T op_ret = 0;
    
    // SHT21 IIC driver init 
    tuya_sht21_init(&sht21_int_param);
    
    __ctrl_gpio_init(HEATING_ROD_PORT, HEATING_ROD_LEVEL); 

    __ctrl_gpio_init(COOL_DOWN_FAN_PORT, COOL_DOWN_FAN_LEVEL);

}



OPERATE_RET app_get_all_sensor_data(VOID)
{
    OPERATE_RET ret = 0;

    SHORT_T hum;
    SHORT_T temp;

    temp = tuya_sht21_measure(TEMP);
    device_data.temperature = tuya_sht21_cal_temperature(temp);

    app_report_data.Temp_current = (UINT16_T)(10*device_data.temperature);
    PR_NOTICE("tempre = %d",app_report_data.Temp_current);
    
    return ret;
}


STATIC VOID __passive_ctrl_module_temp(VOID)
{   
    if(device_data.temperature < device_data.min_temp) {
        tuya_gpio_write(HEATING_ROD_PORT, !HEATING_ROD_LEVEL);
        tuya_gpio_write(COOL_DOWN_FAN_PORT, COOL_DOWN_FAN_LEVEL);
    }else if(device_data.temperature > device_data.max_temp) {
        tuya_gpio_write(HEATING_ROD_PORT, HEATING_ROD_LEVEL);
        tuya_gpio_write(COOL_DOWN_FAN_PORT, !COOL_DOWN_FAN_LEVEL);
    }else {
        tuya_gpio_write(HEATING_ROD_PORT, HEATING_ROD_LEVEL);
        tuya_gpio_write(COOL_DOWN_FAN_PORT, COOL_DOWN_FAN_LEVEL);
    }
    
}

VOID app_ctrl_handle(VOID)
{   
    PR_DEBUG("ctrl handle");
    __passive_ctrl_module_temp();

}

VOID app_ctrl_all_off(VOID)
{   
    tuya_gpio_write(HEATING_ROD_PORT, HEATING_ROD_LEVEL);
    tuya_gpio_write(COOL_DOWN_FAN_PORT, COOL_DOWN_FAN_LEVEL);
}