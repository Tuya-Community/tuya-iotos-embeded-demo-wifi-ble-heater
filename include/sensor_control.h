/*
 * @Author: zgw
 * @email: liang.zhang@tuya.com
 * @LastEditors: zgw
 * @file name: sensor_control.h
 * @Description:
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-1-5 16:21:15
 * @LastEditTime: 2021-1-5 16:21:15
 */

#ifndef __SENSOR_CONTROL_H__
#define __SENSOR_CONTROL_H__

#include "uni_log.h"
#include "tuya_cloud_error_code.h"


typedef struct {
    float temperature;
    float max_temp;
    float min_temp;
    BOOL_T dev_switch;
}DEVICE_DATA_T;


typedef struct {
    UINT16_T Temp_current;
    UINT16_T max_temp;
    UINT16_T min_temp;
}APP_REPORT_DATA_T;

VOID app_device_init(VOID);
VOID app_ctrl_handle(VOID);
VOID app_ctrl_all_off(VOID);

OPERATE_RET app_get_all_sensor_data(VOID);

#endif