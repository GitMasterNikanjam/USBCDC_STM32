#pragma once

// ##############################################################################################
// MCU Select:

#include "mcu_select.h"

/*
    If there is not exist mcu_select.h at beside of this header file, Create it and put this bellow following content. 
    Then select your desired MCU that want work with.
*/
// ----------------------------------------------------------------
// mcu_select.h file:

// Define the target MCU family here
// Uncomment the desired MCU family definition below:

// #define STM32F1
// #define STM32F4
// #define STM32H7

// ----------------------------------------------------------------

// ##################################################################################
// Include libraries:

#if defined(STM32F1)
#include "stm32f1xx_hal.h"      // HAL library for STM32F1 series
#elif defined(STM32F4)
#include "stm32f4xx_hal.h"      // HAL library for STM32F4 series
#elif defined(STM32H7)
#include "stm32h7xx_hal.h"      // HAL library for STM32H7 series
#else
#error "Unsupported MCU family. Please define a valid target (e.g., STM32F1, STM32F4, STM32H7)."
#endif

#include <string>       // Include the standard string library for error handling and messages
#include "usb_device.h"
#include "usbd_cdc.h"
#include "usbd_def.h"

// ##################################################################################

/* Define size for the receive and transmit buffer over CDC */
#define USB_RX_DATA_SIZE  2048
#define USB_TX_DATA_SIZE  2048

// ###################################################################################

class USB_CDC_Device
{
    public:

    /**
     * @brief Constructor. Init some parameters and variables.
     */
    USB_CDC_Device();

    /**
     * @brief Init object. Check parameters validation.
     * @note - Init USB device Library, add supported class and start the library
     * @return true if successful.
     */
    bool init(void);

    /**
     * @brief  CDC_Transmit_FS
     *         Data to send over USB IN endpoint are sent over CDC interface
     *         through this function.
     *         @note
     *
     *
     * @param  Buf: Buffer of data to be sent
     * @param  Len: Number of data to be sent (in bytes)
     * @retval USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
     */
    uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);

    private:

        /** Received data over USB are stored in this buffer      */
        static uint8_t _UserRxBufferFS[USB_RX_DATA_SIZE];

        /** Data to send over USB CDC are stored in this buffer   */
        static uint8_t _UserTxBufferFS[USB_TX_DATA_SIZE];

        static USBD_HandleTypeDef _hUsbDeviceFS;

        USBD_CDC_ItfTypeDef _USBD_Interface_fops_FS;

        /**
         * @brief  Initializes the CDC media low layer over the FS USB IP
         * @retval USBD_OK if all operations are OK else USBD_FAIL
         */
        static int8_t _CDC_Init_FS(void);

        /**
         * @brief  DeInitializes the CDC media low layer
         * @retval USBD_OK if all operations are OK else USBD_FAIL
         */
        static int8_t _CDC_DeInit_FS(void);

        /**
         * @brief  Manage the CDC class requests
         * @param  cmd: Command code
         * @param  pbuf: Buffer containing command data (request parameters)
         * @param  length: Number of data to be sent (in bytes)
         * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
         */
        static int8_t _CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length);

        /**
         * @brief  Data received over USB OUT endpoint are sent over CDC interface
         *         through this function.
         *
         *         @note
         *         This function will issue a NAK packet on any OUT packet received on
         *         USB endpoint until exiting this function. If you exit this function
         *         before transfer is complete on CDC interface (ie. using DMA controller)
         *         it will result in receiving more data while previous ones are still
         *         not sent.
         *
         * @param  Buf: Buffer of data to be received
         * @param  Len: Number of data received (in bytes)
         * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
         */
        static int8_t _CDC_Receive_FS(uint8_t* pbuf, uint32_t *Len);

        /**
         * @brief  CDC_TransmitCplt_FS
         *         Data transmitted callback
         *
         *         @note
         *         This function is IN transfer complete callback used to inform user that
         *         the submitted Data is successfully sent over USB.
         *
         * @param  Buf: Buffer of data to be received
         * @param  Len: Number of data received (in bytes)
         * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
         */
        static int8_t _CDC_TransmitCplt_FS(uint8_t *pbuf, uint32_t *Len, uint8_t epnum);
};