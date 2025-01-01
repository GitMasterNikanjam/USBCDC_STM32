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
#include "Stream.h"

// ##################################################################################
// Define global macros:

/* Define size for the receive and transmit buffer over CDC */
#define USB_RX_DATA_SIZE  256
#define USB_TX_DATA_SIZE  256

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
     * @brief Return current TxBuffer size.
     */
    size_t getTxBufferSize();

    /**
     * @brief Return currect RxBuffer size.
     */
    size_t getRxBufferSize();

    /**
     * @brief Get the number of bytes (characters) available for reading from the serial port. 
     * This is data that’s already arrived and stored in the serial receive buffer
     */
    uint16_t available(void);

    /**
     * @brief Writes binary data to the serial port.
     * @return The number of bytes written, though reading that number is optional.
     */
    uint16_t write(uint8_t data);

    /**
     * @brief Writes binary data to the serial port.
     * @return The number of bytes written, though reading that number is optional.
     */
    uint16_t write(uint8_t* data, uint16_t length);

    /**
     * @brief Prints data to the serial port as human-readable ASCII text. This command can take many forms. 
     * Numbers are printed using an ASCII character for each digit. Floats are similarly printed as ASCII digits, 
     * defaulting to two decimal places.
     * @return number of characters that written.
     * @note Return value can be 0 or number of character of data. If any error for transmitting data occurred it returns 0.
     */
    uint16_t print(const char* data);

    /**
     * @brief Prints data to the serial port as human-readable ASCII text. This command can take many forms. 
     * Numbers are printed using an ASCII character for each digit. Floats are similarly printed as ASCII digits, 
     * defaulting to two decimal places.
     * @return number of characters that written.
     * @note Return value can be 0 or number of character of data. If any error for transmitting data occurred it returns 0.
     */
    uint16_t print(const std::string& data);

    /**
     * @brief Prints data to the serial port as human-readable ASCII text. This command can take many forms. 
     * Numbers are printed using an ASCII character for each digit. Floats are similarly printed as ASCII digits, 
     * defaulting to two decimal places.
     * @return number of characters that written.
     * @note Return value can be 0 or number of character of data. If any error for transmitting data occurred it returns 0.
     */
    uint16_t print(uint32_t data);

    /**
     * @brief Prints data to the serial port as human-readable ASCII text. This command can take many forms. 
     * Numbers are printed using an ASCII character for each digit. Floats are similarly printed as ASCII digits, 
     * defaulting to two decimal places.
     * @return number of characters that written.
     * @note Return value can be 0 or number of character of data. If any error for transmitting data occurred it returns 0.
     */
    uint16_t print(int32_t data);

    /**
     * @brief Prints data to the serial port as human-readable ASCII text. This command can take many forms. 
     * Numbers are printed using an ASCII character for each digit. Floats are similarly printed as ASCII digits, 
     * defaulting to two decimal places.
     * @return number of characters that written.
     * @note Return value can be 0 or number of character of data. If any error for transmitting data occurred it returns 0.
     */
    uint16_t print(uint64_t data);

    /**
     * @brief Prints data to the serial port as human-readable ASCII text. This command can take many forms. 
     * Numbers are printed using an ASCII character for each digit. Floats are similarly printed as ASCII digits, 
     * defaulting to two decimal places.
     * @return number of characters that written.
     * @note Return value can be 0 or number of character of data. If any error for transmitting data occurred it returns 0.
     */
    uint16_t print(int64_t data);

    /**
     * @brief Prints data to the serial port as human-readable ASCII text. This command can take many forms. 
     * Numbers are printed using an ASCII character for each digit. Floats are similarly printed as ASCII digits, 
     * defaulting to two decimal places.
     * @return number of characters that written.
     * @note Return value can be 0 or number of character of data. If any error for transmitting data occurred it returns 0.
     */
    uint16_t print(double data, uint8_t precision= 2);

    /**
     * @brief Prints data to the serial port as human-readable ASCII text. This command can take many forms. 
     * Numbers are printed using an ASCII character for each digit. Floats are similarly printed as ASCII digits, 
     * defaulting to two decimal places.
     * @note It sends newline character automatically end of data.
     * @return number of characters that written.
     * @note Return value can be 0 or number of character of data. If any error for transmitting data occurred it returns 0.
     */
    uint16_t println(const char* data);

    /**
     * @brief Prints data to the serial port as human-readable ASCII text. This command can take many forms. 
     * Numbers are printed using an ASCII character for each digit. Floats are similarly printed as ASCII digits, 
     * defaulting to two decimal places.
     * @note It sends newline character automatically end of data.
     * @return number of characters that written.
     * @note Return value can be 0 or number of character of data. If any error for transmitting data occurred it returns 0.
     */
    uint16_t println(const std::string& data);

    /**
     * @brief Prints data to the serial port as human-readable ASCII text. This command can take many forms. 
     * Numbers are printed using an ASCII character for each digit. Floats are similarly printed as ASCII digits, 
     * defaulting to two decimal places.
     * @note It sends newline character automatically end of data.
     * @return number of characters that written.
     * @note Return value can be 0 or number of character of data. If any error for transmitting data occurred it returns 0.
     */
    uint16_t println(uint32_t data);

    /**
     * @brief Prints data to the serial port as human-readable ASCII text. This command can take many forms. 
     * Numbers are printed using an ASCII character for each digit. Floats are similarly printed as ASCII digits, 
     * defaulting to two decimal places.
     * @note It sends newline character automatically end of data.
     * @return number of characters that written.
     * @note Return value can be 0 or number of character of data. If any error for transmitting data occurred it returns 0.
     */
    uint16_t println(int32_t data);

    /**
     * @brief Prints data to the serial port as human-readable ASCII text. This command can take many forms. 
     * Numbers are printed using an ASCII character for each digit. Floats are similarly printed as ASCII digits, 
     * defaulting to two decimal places.
     * @note It sends newline character automatically end of data.
     * @return number of characters that written.
     * @note Return value can be 0 or number of character of data. If any error for transmitting data occurred it returns 0.
     */
    uint16_t println(uint64_t data);

    /**
     * @brief Prints data to the serial port as human-readable ASCII text. This command can take many forms. 
     * Numbers are printed using an ASCII character for each digit. Floats are similarly printed as ASCII digits, 
     * defaulting to two decimal places.
     * @note It sends newline character automatically end of data.
     * @return number of characters that written.
     * @note Return value can be 0 or number of character of data. If any error for transmitting data occurred it returns 0.
     */
    uint16_t println(int64_t data);

    /**
     * @brief Prints data to the serial port as human-readable ASCII text. This command can take many forms. 
     * Numbers are printed using an ASCII character for each digit. Floats are similarly printed as ASCII digits, 
     * defaulting to two decimal places.
     * @note It sends newline character automatically end of data.
     * @return number of characters that written.
     * @note Return value can be 0 or number of character of data. If any error for transmitting data occurred it returns 0.
     */
    uint16_t println(double data, uint8_t precision= 2);

    void updateProccess(void);

    private:

        Stream _stream;

        /// @brief Data to send over USB CDC are stored in this buffer
        static uint8_t _USBTxBuffer[USB_TX_DATA_SIZE];

        /**
         * @brief Received data over USB are stored in this buffer
         */
        static uint8_t _USBRxBuffer[USB_RX_DATA_SIZE];

        /// @brief The last character position + 1 in the _txBuffer.
        static size_t _USBTxPosition;

        /// @brief The last character position in the _rxBuffer.
        static size_t _USBRxPosition;

        size_t _streamTxBufferSize;

        size_t _streamRxBufferSize; 

        /**
         * @brief USB Device Handle for Full-Speed Communication.
         *
         * Represents the USB device handle used to manage the USB CDC operations
         * for the Full-Speed configuration.
         */
        static USBD_HandleTypeDef _hUsbDeviceFS;

        /**
         * @brief USB CDC Interface Function Pointers for Full-Speed.
         *
         * Defines the interface functions implemented for the USB CDC Full-Speed
         * communication, such as data transmission and reception handlers.
         */
        USBD_CDC_ItfTypeDef _USBD_Interface_fops_FS;

        /**
         * @brief Indicate init state. It is true if init successfully finished.
         */
        bool _initState;

        /**
         * @brief Tracks the transmission state of the USB CDC.
         *
         * Indicates whether a USB CDC transmission is currently in progress.
         */
        static volatile bool _isTransmitting;

        /**
         * @brief Handles USB CDC data transmission.
         *
         * Sends data if not already transmitting and the buffer has data. 
         * Resets `_txPosition` after a successful send.
         * 
         * @return USBD_STATE.
         */
        static uint8_t _transmitProcessQueue(void);

        /**
         * @brief  CDC_Transmit_FS
         *         Data to send over USB IN endpoint are sent over CDC interface
         *         through this function.
         * @param  Buf: Buffer of data to be sent
         * @param  Len: Number of data to be sent (in bytes)
         * @retval USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
         */
        static uint8_t _CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);

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

        /**
         * @brief Check parameters validation.
         * @return true if successful.
         */
        bool _checkParameters(void);
};