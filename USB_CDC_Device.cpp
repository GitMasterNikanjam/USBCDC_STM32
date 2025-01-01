#include "USB_CDC_Device.h"
#include "usbd_desc.h"

// ################################################################################################
// Define the static members

uint8_t USB_CDC_Device::_USBTxBuffer[USB_TX_DATA_SIZE] = {0};
uint8_t USB_CDC_Device::_USBRxBuffer[USB_RX_DATA_SIZE] = {0};
size_t USB_CDC_Device::_USBTxPosition = 0;
size_t USB_CDC_Device::_USBRxPosition = 0;
USBD_HandleTypeDef USB_CDC_Device::_hUsbDeviceFS = {}; // Zero-initialize the structure
volatile bool USB_CDC_Device::_isTransmitting = false;

// ##################################################################################################

USB_CDC_Device::USB_CDC_Device()
{   
    _streamTxBufferSize = 1000;
    _streamRxBufferSize = 1000;

    _stream.setTxBufferSize(_streamTxBufferSize);
    _stream.setRxBufferSize(_streamRxBufferSize);

    _USBD_Interface_fops_FS =
    {
    _CDC_Init_FS,
    _CDC_DeInit_FS,
    _CDC_Control_FS,
    _CDC_Receive_FS,
    _CDC_TransmitCplt_FS
    };

    _initState = false;
    _isTransmitting = false;
}

bool USB_CDC_Device::init(void)
{
  if(_checkParameters() == false)
  {
    return false;
  }

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

  _initState = true;
  return true;
}

size_t USB_CDC_Device::getTxBufferSize() 
{
    return USB_TX_DATA_SIZE;
}

size_t USB_CDC_Device::getRxBufferSize() 
{
    return USB_RX_DATA_SIZE;
}

uint16_t USB_CDC_Device::available() 
{
    return strlen((char*)_USBRxBuffer);
}

uint16_t USB_CDC_Device::write(uint8_t data)
{
  return write(&data, 1);
}

uint16_t USB_CDC_Device::write(uint8_t* data, uint16_t length)
{

  _stream.pushBackTxBuffer((char*)data, length);

  // if (_USBTxPosition + length > USB_TX_DATA_SIZE) // Check buffer size
  // {
  //     return 0; // Not enough space
  // }

  // memcpy(&_UserTxBufferFS[_txPosition], data, length);
  // _txPosition += length;

  // // Attempt transmission immediately
  // if(_transmitProcessQueue() == USBD_FAIL)
  // {
  //   return 0;
  // }

  return length;
}

// ------------------------------------------------------------------------
// print/println methods:

uint16_t USB_CDC_Device::print(const char* data)
{
  uint16_t dataSize = strlen(data);

  return write((uint8_t*)data, dataSize);
}

uint16_t USB_CDC_Device::print(const std::string& data)
{
  return print(data.c_str());
}

uint16_t USB_CDC_Device::print(uint32_t data)
{
    char buffer[12]; // Buffer to hold the ASCII representation of the number (up to 3 digits + null terminator)
    int length = snprintf(buffer, sizeof(buffer), "%d", data); // Convert the uint8_t to string
  
    return print(buffer);
}

uint16_t USB_CDC_Device::print(int32_t data)
{
    char buffer[12]; // Buffer to hold the ASCII representation of the number (up to 3 digits + null terminator)
    int length = snprintf(buffer, sizeof(buffer), "%d", data); // Convert the uint8_t to string
  
    return print(buffer);
}

uint16_t USB_CDC_Device::print(uint64_t data)
{
    char buffer[30]; // Buffer to hold the ASCII representation of the number (up to 3 digits + null terminator)
    int length = snprintf(buffer, sizeof(buffer), "%llu", data); // Convert the uint8_t to string
  
    return print(buffer);
}

uint16_t USB_CDC_Device::print(int64_t data)
{
    char buffer[30]; // Buffer to hold the ASCII representation of the number (up to 3 digits + null terminator)
    int length = snprintf(buffer, sizeof(buffer), "%lld", data); // Convert the uint8_t to string
  
    return print(buffer);
}

uint16_t USB_CDC_Device::print(double data, uint8_t precision)
{
    if (precision > 10) 
    { // Limit precision to avoid excessive output
        precision = 10;
    }
    char buffer[30]; // Buffer to hold the ASCII representation of the number (up to 3 digits + null terminator)
    int length = snprintf(buffer, sizeof(buffer), "%.*lf", precision, data); // Convert the uint8_t to string
  
     // Ensure snprintf was successful and print the result
    if (length <= 0 || length >= (int)sizeof(buffer)) 
    {
        return 0;
    } 

    return print(buffer);
}

uint16_t USB_CDC_Device::println(const char* data)
{
  uint16_t dataSize = print(data) + print("\n");
  return dataSize ;
}

uint16_t USB_CDC_Device::println(const std::string& data)
{
  return print(data + "\n");
}

uint16_t USB_CDC_Device::println(uint32_t data)
{
  size_t dataSize = print(data) + print("\n");
  return dataSize + 1;
}

uint16_t USB_CDC_Device::println(int32_t data)
{
  size_t dataSize = print(data) + print("\n");
  return dataSize + 1;
}

uint16_t USB_CDC_Device::println(uint64_t data)
{
  size_t dataSize = print(data) + print("\n");
  return dataSize + 1;
}

uint16_t USB_CDC_Device::println(int64_t data)
{
  size_t dataSize = print(data) + print("\n");
  return dataSize + 1;
}

uint16_t USB_CDC_Device::println(double data, uint8_t precision)
{
  size_t dataSize = print(data) + print("\n");
  return dataSize + 1;
}

// -------------------------------------------------------------------------

void USB_CDC_Device::updateProccess(void)
{
  size_t len = 0;

  if(_stream.availableTx() > USB_TX_DATA_SIZE)
  {
    len = USB_TX_DATA_SIZE;
  }

  if(_isTransmitting == false)
  {
    _stream.popFrontTxBuffer((char*)_USBTxBuffer, len);
    _isTransmitting = true;
    _CDC_Transmit_FS(_USBTxBuffer, len);
  }
  
}

int8_t USB_CDC_Device::_CDC_Init_FS(void)
{
  /* Set Application Buffers */
  USBD_CDC_SetTxBuffer(&_hUsbDeviceFS, _USBTxBuffer, 0);
  USBD_CDC_SetRxBuffer(&_hUsbDeviceFS, _USBRxBuffer);
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

uint8_t USB_CDC_Device::_transmitProcessQueue(void)
{
  uint8_t state = USBD_OK;

  if (!_isTransmitting && (_USBTxPosition != 0))
  {
    state = _CDC_Transmit_FS(_USBTxBuffer, _USBTxPosition);
    if (state == USBD_OK)
    {
        _isTransmitting = true;
        // _txPosition = 0; // Reset the position after successful transmission
    }
  }
  else
  {
    state = USBD_BUSY;
  }

  return state;
}

uint8_t USB_CDC_Device::_CDC_Transmit_FS(uint8_t* Buf, uint16_t Len)
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
  _isTransmitting = false;
  uint8_t result = USBD_OK;

  UNUSED(Buf);
  UNUSED(Len);
  UNUSED(epnum);

  return result;
}

bool USB_CDC_Device::_checkParameters(void)
{

  return true;
}