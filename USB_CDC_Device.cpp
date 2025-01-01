#include "USB_CDC_Device.h"
#include "usbd_desc.h"

// ################################################################################################

// Define the static members
uint8_t USB_CDC_Device::_UserRxBufferFS[USB_RX_DATA_SIZE] = {0}; // Initialize to zero
uint8_t USB_CDC_Device::_UserTxBufferFS[USB_TX_DATA_SIZE] = {0}; // Initialize to zero
USBD_HandleTypeDef USB_CDC_Device::_hUsbDeviceFS = {}; // Zero-initialize the structure

// ##################################################################################################

USB_CDC_Device::USB_CDC_Device()
{
    _USBD_Interface_fops_FS =
    {
    _CDC_Init_FS,
    _CDC_DeInit_FS,
    _CDC_Control_FS,
    _CDC_Receive_FS,
    _CDC_TransmitCplt_FS
    };
}

bool USB_CDC_Device::init(void)
{
  /* Init Device Library, add supported class and start the library. */
  if (USBD_Init(&_hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK)
  {
    Error_Handler();
    return false;
  }
  if (USBD_RegisterClass(&_hUsbDeviceFS, &USBD_CDC) != USBD_OK)
  {
    Error_Handler();
    return false;
  }
  if (USBD_CDC_RegisterInterface(&_hUsbDeviceFS, &_USBD_Interface_fops_FS) != USBD_OK)
  {
    Error_Handler();
    return false;
  }
  if (USBD_Start(&_hUsbDeviceFS) != USBD_OK)
  {
    Error_Handler();
    return false;
  }

  return true;
}

int8_t USB_CDC_Device::_CDC_Init_FS(void)
{
  /* Set Application Buffers */
  USBD_CDC_SetTxBuffer(&_hUsbDeviceFS, _UserTxBufferFS, 0);
  USBD_CDC_SetRxBuffer(&_hUsbDeviceFS, _UserRxBufferFS);
  return (USBD_OK);
}

int8_t USB_CDC_Device::_CDC_DeInit_FS(void)
{
  return (USBD_OK);
}

int8_t USB_CDC_Device::_CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  switch(cmd)
  {
    case CDC_SEND_ENCAPSULATED_COMMAND:

    break;

    case CDC_GET_ENCAPSULATED_RESPONSE:

    break;

    case CDC_SET_COMM_FEATURE:

    break;

    case CDC_GET_COMM_FEATURE:

    break;

    case CDC_CLEAR_COMM_FEATURE:

    break;

  /*******************************************************************************/
  /* Line Coding Structure                                                       */
  /*-----------------------------------------------------------------------------*/
  /* Offset | Field       | Size | Value  | Description                          */
  /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
  /* 4      | bCharFormat |   1  | Number | Stop bits                            */
  /*                                        0 - 1 Stop bit                       */
  /*                                        1 - 1.5 Stop bits                    */
  /*                                        2 - 2 Stop bits                      */
  /* 5      | bParityType |  1   | Number | Parity                               */
  /*                                        0 - None                             */
  /*                                        1 - Odd                              */
  /*                                        2 - Even                             */
  /*                                        3 - Mark                             */
  /*                                        4 - Space                            */
  /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
  /*******************************************************************************/
    case CDC_SET_LINE_CODING:

    break;

    case CDC_GET_LINE_CODING:

    break;

    case CDC_SET_CONTROL_LINE_STATE:

    break;

    case CDC_SEND_BREAK:

    break;

  default:
    break;
  }

  return (USBD_OK);
}

int8_t USB_CDC_Device::_CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
{
  USBD_CDC_SetRxBuffer(&_hUsbDeviceFS, &Buf[0]);
  USBD_CDC_ReceivePacket(&_hUsbDeviceFS);
  return (USBD_OK);
}

uint8_t USB_CDC_Device::CDC_Transmit_FS(uint8_t* Buf, uint16_t Len)
{
  uint8_t result = USBD_OK;

  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)_hUsbDeviceFS.pClassData;
  if (hcdc->TxState != 0){
    return USBD_BUSY;
  }
  USBD_CDC_SetTxBuffer(&_hUsbDeviceFS, Buf, Len);
  result = USBD_CDC_TransmitPacket(&_hUsbDeviceFS);

  return result;
}

int8_t USB_CDC_Device::_CDC_TransmitCplt_FS(uint8_t *Buf, uint32_t *Len, uint8_t epnum)
{
  uint8_t result = USBD_OK;

  UNUSED(Buf);
  UNUSED(Len);
  UNUSED(epnum);

  return result;
}