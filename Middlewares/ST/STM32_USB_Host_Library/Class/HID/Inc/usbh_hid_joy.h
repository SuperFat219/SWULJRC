/*
 * usbh_hid_joystick.h
 *
 *  Created on: 2024年6月28日
 *      Author: Yan Zhongsheng
 *      Used：	专门应用于罗技F710游戏手柄，或可以兼容XBOX360手柄
 */

#ifndef ST_STM32_USB_HOST_LIBRARY_CLASS_HID_INC_USBH_HID_JOY_H_
#define ST_STM32_USB_HOST_LIBRARY_CLASS_HID_INC_USBH_HID_JOY_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbh_hid.h"

    typedef struct _HID_JOY_Info
    {
        uint8_t ID; // X/D mode
        uint8_t L_X;
        uint8_t L_Y;
        uint8_t R_X;
        uint8_t R_Y;
        uint8_t Buttons_1; // 摇杆上的ABXY/up/right/down/left
        uint8_t Buttons_2; // 摇杆上的START/BACK/RT/LT/RB/LB
        uint8_t Mode;      // 0/1 将左摇杆和左按键调换
    } HID_JOY_Info_TypeDef;

/******************************  D模式的按键定义（8字节）    ****************************************/
#define JOY_MODE_0 0x74 // 0x74为mode灭灯
#define JOY_MODE_1 0x7C // 0x7C为mode亮灯

#define JOY_L_X_INIT 0x80 // 0x80为左摇杆X轴零点（不一定精确回正，有2以内误差）
#define JOY_L_Y_INIT 0x7F // 0x7F为左摇杆Y轴零点（不一定精确回正，有2以内误差）
#define JOY_R_X_INIT 0x80 // 0x80为右摇杆X轴零点（不一定精确回正，有2以内误差）
#define JOY_R_Y_INIT 0x7F // 0x7F为右摇杆Y轴零点（不一定精确回正，有2以内误差）

// BUTTONS1使用同一个字节，按键可叠加读取
#define BUTTONS1_L_INIT 0x08 // 左方向键初始状态
#define BUTTONS1_LEFT 0x06   // 左方向键向左
#define BUTTONS1_RIGHT 0x02  // 左方向键向右
#define BUTTONS1_UP 0x00     // 左方向键向上
#define BUTTONS1_DOWN 0x04   // 左方向键向下

#define BUTTONS1_R_INIT 0x00 // 右键初始状态
#define BUTTONS1_A 0x20      // 右A
#define BUTTONS1_B 0x40      // 右B
#define BUTTONS1_X 0x10      // 右X
#define BUTTONS1_Y 0x80      // 右Y

// BUTTONS2使用同一个字节，按键可叠加读取
#define BUTTONS2_INIT 0x00
#define BUTTONS2_LT 0x04
#define BUTTONS2_LB 0x01
#define BUTTONS2_RT 0x08
#define BUTTONS2_RB 0x02
#define BUTTONS2_BACK 0x10
#define BUTTONS2_START 0x20

    USBH_StatusTypeDef USBH_HID_JoyInit(USBH_HandleTypeDef *phost);
    HID_JOY_Info_TypeDef *USBH_HID_GetJoyInfo(USBH_HandleTypeDef *phost);

#ifdef __cplusplus
}
#endif

#endif /* ST_STM32_USB_HOST_LIBRARY_CLASS_HID_INC_USBH_HID_JOY_H_ */
