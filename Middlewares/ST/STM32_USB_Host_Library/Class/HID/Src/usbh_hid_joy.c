/*
 * usbh_hid_joystick.c
 *
 *  Created on: 2024年6月28日
 *      Author: Yan Zhongsheng
 *      used：根据抓包的消息，定义报告描述符
 */

#include "usbh_hid_joy.h"
#include "usbh_hid_parser.h"
#include "usb_host.h"

static USBH_StatusTypeDef USBH_HID_JoyDecode(USBH_HandleTypeDef *phost);

HID_JOY_Info_TypeDef joy_info;
uint32_t joy_report_data[2];
uint32_t joy_rx_report_buf[2];
extern USBH_HandleTypeDef hUsbHostFS;
extern ApplicationTypeDef Appli_state;

/* Structures defining how to access items in a HID mouse report */
/* Access ID state. */
static const HID_Report_ItemTypedef prop_id =
    {
        (uint8_t *)(void *)joy_report_data + 0, /*data 按字节偏移*/
        8,                                      /*size*/
        0,                                      /*shift*/
        0,                                      /*count (only for array items)*/
        1,                                      /*signed?*/
        0,                                      /*min value read can return*/
        0xFFFF,                                 /*max value read can return*/
        0,                                      /*min value device can report*/
        0xFFFF,                                 /*max value device can report*/
        1                                       /*resolution*/
};

/* Access Lx coordinate change. */
static const HID_Report_ItemTypedef prop_L_X =
    {
        (uint8_t *)(void *)joy_report_data + 1, /*data 按字节偏移*/
        8,                                      /*size*/
        0,                                      /*shift*/
        0,                                      /*count (only for array items)*/
        1,                                      /*signed?*/
        0,                                      /*min value read can return*/
        0xFFFF,                                 /*max value read can return*/
        0,                                      /*min vale device can report*/
        0xFFFF,                                 /*max value device can report*/
        1                                       /*resolution*/
};

/* Access Ly coordinate change. */
static const HID_Report_ItemTypedef prop_L_Y =
    {
        (uint8_t *)(void *)joy_report_data + 2, /*data 按字节偏移*/
        8,                                      /*size*/
        0,                                      /*shift*/
        0,                                      /*count (only for array items)*/
        1,                                      /*signed?*/
        0,                                      /*min value read can return*/
        0xFFFF,                                 /*max value read can return*/
        0,                                      /*min vale device can report*/
        0xFFFF,                                 /*max value device can report*/
        1                                       /*resolution*/
};

/* Access Rx coordinate change. */
static const HID_Report_ItemTypedef prop_R_X =
    {
        (uint8_t *)(void *)joy_report_data + 3, /*data 按字节偏移*/
        8,                                      /*size*/
        0,                                      /*shift*/
        0,                                      /*count (only for array items)*/
        1,                                      /*signed?*/
        0,                                      /*min value read can return*/
        0xFFFF,                                 /*max value read can return*/
        0,                                      /*min vale device can report*/
        0xFFFF,                                 /*max value device can report*/
        1                                       /*resolution*/
};

/* Access Ry coordinate change. */
static const HID_Report_ItemTypedef prop_R_Y =
    {
        (uint8_t *)(void *)joy_report_data + 4, /*data 按字节偏移*/
        8,                                      /*size*/
        0,                                      /*shift*/
        0,                                      /*count (only for array items)*/
        1,                                      /*signed?*/
        0,                                      /*min value read can return*/
        0xFFFF,                                 /*max value read can return*/
        0,                                      /*min vale device can report*/
        0xFFFF,                                 /*max value device can report*/
        1                                       /*resolution*/
};

/* Access Buttons_1 coordinate change. */
static const HID_Report_ItemTypedef prop_Buttons_1 =
    {
        (uint8_t *)(void *)joy_report_data + 5, /*data 按字节偏移*/
        8,                                      /*size*/
        0,                                      /*shift*/
        0,                                      /*count (only for array items)*/
        1,                                      /*signed?*/
        0,                                      /*min value read can return*/
        0xFFFF,                                 /*max value read can return*/
        0,                                      /*min vale device can report*/
        0xFFFF,                                 /*max value device can report*/
        1                                       /*resolution*/
};

/* Access Buttons_2 coordinate change. */
static const HID_Report_ItemTypedef prop_Buttons_2 =
    {
        (uint8_t *)(void *)joy_report_data + 6, /*data 按字节偏移*/
        8,                                      /*size*/
        0,                                      /*shift*/
        0,                                      /*count (only for array items)*/
        1,                                      /*signed?*/
        0,                                      /*min value read can return*/
        0xFFFF,                                 /*max value read can return*/
        0,                                      /*min vale device can report*/
        0xFFFF,                                 /*max value device can report*/
        1                                       /*resolution*/
};

/* Access MODE coordinate change. */
static const HID_Report_ItemTypedef prop_Mode =
    {
        (uint8_t *)(void *)joy_report_data + 7, /*data 按字节偏移*/
        8,                                      /*size*/
        0,                                      /*shift按位偏移 */
        0,                                      /*count (only for array items)*/
        1,                                      /*signed?*/
        0,                                      /*min value read can return*/
        0xFFFF,                                 /*max value read can return*/
        0,                                      /*min vale device can report*/
        0xFFFF,                                 /*max value device can report*/
        1                                       /*resolution*/
};

/**
 * @}
 */

/** @defgroup USBH_HID_MOUSE_Private_Functions
 * @{
 */

/**
 * @brief  USBH_HID_JoyInit
 *         The function init the HID mouse.
 * @param  phost: Host handle
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_HID_JoyInit(USBH_HandleTypeDef *phost)
{
    uint32_t i;
    HID_HandleTypeDef *HID_Handle = (HID_HandleTypeDef *)phost->pActiveClass->pData;

    USBH_UsrLog("DEBUG: USBH_HID_JoyInit!!!");

    joy_info.ID = 0U;
    joy_info.L_X = 128U;
    joy_info.L_Y = 128U;
    joy_info.R_X = 128U;
    joy_info.R_X = 128U;
    joy_info.Mode = 0U;
    joy_info.Buttons_1 = 0U;
    joy_info.Buttons_2 = 0U;

    for (i = 0U; i < (sizeof(joy_report_data) / sizeof(uint32_t)); i++)
    {
        joy_report_data[i] = 0U;
        joy_rx_report_buf[i] = 0U;
    }

    if (HID_Handle->length > sizeof(joy_report_data))
    {
        HID_Handle->length = sizeof(joy_report_data);
    }
    HID_Handle->pData = (uint8_t *)(void *)joy_rx_report_buf;
    USBH_HID_FifoInit(&HID_Handle->fifo, phost->device.Data, HID_QUEUE_SIZE * sizeof(joy_report_data));

    return USBH_OK;
}

/**
 * @brief  USBH_HID_GetJoyInfo
 *         The function return mouse information.
 * @param  phost: Host handle
 * @retval mouse information
 */
HID_JOY_Info_TypeDef *USBH_HID_GetJoyInfo(USBH_HandleTypeDef *phost)
{
    if (USBH_HID_JoyDecode(phost) == USBH_OK)
    {
        return &joy_info;
    }
    else
    {
        return NULL;
    }
}

/**
 * @brief  USBH_HID_JoyDecode
 *         The function decode mouse data.
 * @param  phost: Host handle
 * @retval USBH Status
 */
static USBH_StatusTypeDef USBH_HID_JoyDecode(USBH_HandleTypeDef *phost)
{
    HID_HandleTypeDef *HID_Handle = (HID_HandleTypeDef *)phost->pActiveClass->pData;

    if (HID_Handle->length == 0U)
    {
        return USBH_FAIL;
    }
    //  USBH_UsrLog("DEBUG: USBH_HID_JoyDecode!!!");	//解码为while轮询，这句不要加

    /*Fill report */
    if (USBH_HID_FifoRead(&HID_Handle->fifo, &joy_report_data, HID_Handle->length) == HID_Handle->length)
    {
        /*Decode report */

        joy_info.ID = (uint8_t)HID_ReadItem((HID_Report_ItemTypedef *)&prop_id, 0U);
        joy_info.L_X = (uint8_t)HID_ReadItem((HID_Report_ItemTypedef *)&prop_L_X, 0U);
        joy_info.L_Y = (uint8_t)HID_ReadItem((HID_Report_ItemTypedef *)&prop_L_Y, 0U);
        joy_info.R_X = (uint8_t)HID_ReadItem((HID_Report_ItemTypedef *)&prop_R_X, 0U);
        joy_info.R_Y = (uint8_t)HID_ReadItem((HID_Report_ItemTypedef *)&prop_R_Y, 0U);
        joy_info.Buttons_1 = (uint8_t)HID_ReadItem((HID_Report_ItemTypedef *)&prop_Buttons_1, 0U);
        joy_info.Buttons_2 = (uint8_t)HID_ReadItem((HID_Report_ItemTypedef *)&prop_Buttons_2, 0U);
        joy_info.Mode = (uint8_t)HID_ReadItem((HID_Report_ItemTypedef *)&prop_Mode, 0U);

        return USBH_OK;
    }
    return USBH_FAIL;
}

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost)
{
    if (Appli_state == APPLICATION_READY)
    {
        USBH_HID_GetJoyInfo(&hUsbHostFS);
    }
}