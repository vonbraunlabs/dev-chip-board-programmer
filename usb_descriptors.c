/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "dirtyJtagConfig.h"
#include "tusb.h"
#include "get_serial.h"

#if ( CDC_UART_INTF_COUNT > 0 )
#define USB_BCD   0x0210
#else
#define USB_BCD   0x0110
#endif

//--------------------------------------------------------------------+
// Device Descriptors
//--------------------------------------------------------------------+
tusb_desc_device_t const desc_device =
{
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0210,
    .bDeviceClass       = 0x00, // Each interface specifies its own
    .bDeviceSubClass    = 0x00, // Each interface specifies its own
    .bDeviceProtocol    = 0x00,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

	  .idVendor = 0x1209,
	  .idProduct = 0xC0CA,
	  .bcdDevice = 0x0111,
    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,
    .bNumConfigurations = 0x01
};

// Invoked when received GET DEVICE DESCRIPTOR
// Application return pointer to descriptor
uint8_t const * tud_descriptor_device_cb(void)
{
  return (uint8_t const *) &desc_device;
}

//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+

enum
{
  ITF_NUM_PROBE = 0,
#if ( CDC_UART_INTF_COUNT > 0 )
  ITF_NUM_CDC_1 = 1,
  ITF_NUM_CDC_1_DATA,
#endif
#if (CDC_UART_INTF_COUNT > 1)
  ITF_NUM_CDC_2 = 3,
  ITF_NUM_CDC_2_DATA,
#endif 
  ITF_NUM_TOTAL
};

#define PROBE_OUT_EP_NUM 0x01
#define PROBE_IN_EP_NUM  0x82
#if ( CDC_UART_INTF_COUNT > 0 )
#define CDC_NOTIF_EP1_NUM 0x83
#define CDC_OUT_EP1_NUM   0x03
#define CDC_IN_EP1_NUM    0x84
#endif
#if (CDC_UART_INTF_COUNT > 1)
#define CDC_NOTIF_EP2_NUM 0x85
#define CDC_OUT_EP2_NUM   0x05
#define CDC_IN_EP2_NUM    0x86
#endif 

#define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_VENDOR_DESC_LEN + (TUD_CDC_DESC_LEN * (CFG_TUD_CDC)))

static uint8_t const desc_ms_os_20[] = 
{
  // DESCRIPTOR SET
  10, 0,                  // length = 10
  0x00, 0x00,             // MS_OS_20_SET_HEADER_DESCRIPTOR
  0x00, 0x00, 0x03, 0x06, // Windows version >= 0x06030000
  174, 0,                 // total length
  // CONFIGURATION SUBSET
  8, 0,                   // length = 8
  0x01, 0x00,             // MS_OS_20_SUBSET_HEADER_CONFIGURATION
  0x00,                   // configuration index
  0x00,                   // reserved
  164, 0,                 // total length
  // FUNCTION SUBSET
  8, 0,                   // length = 8
  0x02, 0x00,             // MS_OS_20_SUBSET_HEADER_FUNCTION
  ITF_NUM_PROBE,          // interface index
  0x00,                   // reserved
  156, 0,                 // total length
  // FEATURE COMPATIBLE ID
  20, 0,                  // length = 20
  0x03, 0x00,             // MS_OS_20_FEATURE_COMPATIBLE_ID
  'W', 'I', 'N', 'U', 'S', 'B', 0, 0,   // compatible ID
  0, 0, 0, 0, 0, 0, 0, 0,               // sub compatible ID
  // REGISTRY PROPERTY
  128, 0,                 // length = 128
  0x04, 0x00,             // MS_OS_20_FEATURE_REG_PROPERTY
  0x01, 0x00,             // property type = STRING
  40, 0,                  // property name length
  'D', 0, 'e', 0, 'v', 0, 'i', 0, // property name = DeviceInterfaceGUID\0 
  'c', 0, 'e', 0, 'I', 0, 'n', 0, //  
  't', 0, 'e', 0, 'r', 0, 'f', 0, //  
  'a', 0, 'c', 0, 'e', 0, 'G', 0, //  
  'U', 0, 'I', 0, 'D', 0,   0, 0, // 
  78, 0,                  // property data length
  '{', 0, 'A', 0, '5', 0, 'D', 0, // property data = {A5DCBF10-6530-11D2-901F-00C04FB951ED}\0 
  'C', 0, 'B', 0, 'F', 0, '1', 0, //  
  '0', 0, '-', 0, '6', 0, '5', 0, //  
  '3', 0, '0', 0, '-', 0, '1', 0, //  
  '1', 0, 'D', 0, '2', 0, '-', 0, //  
  '9', 0, '0', 0, '1', 0, 'F', 0, //  
  '-', 0, '0', 0, '0', 0, 'C', 0, //  
  '0', 0, '4', 0, 'F', 0, 'B', 0, //  
  '9', 0, '5', 0, '1', 0, 'E', 0, //  
  'D', 0, '}', 0,   0, 0,         // 
};

uint8_t const desc_configuration[CONFIG_TOTAL_LEN] =
{
  // Config number, interface count, string index, total length, attribute, power in mA
  TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

  // Interface 2 : Interface number, string index, EP Out & IN address, EP size
  TUD_VENDOR_DESCRIPTOR(ITF_NUM_PROBE, 0, PROBE_OUT_EP_NUM, PROBE_IN_EP_NUM, 64),
#if ( CDC_UART_INTF_COUNT > 0 )
  // Interface 3 : Interface number, string index, EP notification address and size, EP data address (out, in) and size.
  TUD_CDC_DESCRIPTOR(ITF_NUM_CDC_1, 4, CDC_NOTIF_EP1_NUM, 8, CDC_OUT_EP1_NUM, CDC_IN_EP1_NUM, 64),
#endif
#if ( CDC_UART_INTF_COUNT > 1 )
  TUD_CDC_DESCRIPTOR(ITF_NUM_CDC_2, 5, CDC_NOTIF_EP2_NUM, 8, CDC_OUT_EP2_NUM, CDC_IN_EP2_NUM, 64),
#endif
};

uint8_t const desc_bos[] = 
{
  // MS OS 20 descriptor
  TUD_BOS_DESCRIPTOR(TUD_BOS_DESC_LEN + TUD_BOS_MICROSOFT_OS_DESC_LEN, 1),
  TUD_BOS_MS_OS_20_DESCRIPTOR(sizeof(desc_ms_os_20), 0x01)
};

// Invoked when received GET CONFIGURATION DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const * tud_descriptor_configuration_cb(uint8_t index)
{
  (void) index; // for multiple configurations
  return desc_configuration;
}

//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+

// array of pointer to string descriptors
char const *string_desc_arr[] =
{
    (const char[]){0x09, 0x04}, // 0: is supported language is English (0x0409)
    "Chipinventor LTDA",              // 1: Manufacturer
    "DevChipBoard programmer",                // 2: Product
    usb_serial,                 // 3: Serial, uses flash unique ID
#if ( CDC_UART_INTF_COUNT > 0 )
    "DirtyJTAG CDC 0", // 4: CDC Interface 0
#endif
#if ( CDC_UART_INTF_COUNT > 1 )
    "DirtyJTAG CDC 1"  // 5: CDC Interface 1
#endif
};

static uint16_t _desc_str[32];

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
  (void) langid;

  uint8_t chr_count;

  if ( index == 0)
  {
    memcpy(&_desc_str[1], string_desc_arr[0], 2);
    chr_count = 1;
  }else
  {
    // Convert ASCII string into UTF-16

    if ( !(index < sizeof(string_desc_arr)/sizeof(string_desc_arr[0])) ) return NULL;

    const char* str = string_desc_arr[index];

    // Cap at max char
    chr_count = strlen(str);
    if ( chr_count > 31 ) chr_count = 31;

    for(uint8_t i=0; i<chr_count; i++)
    {
      _desc_str[1+i] = str[i];
    }
  }

  // first byte is length (including header), second byte is string type
  _desc_str[0] = (TUSB_DESC_STRING << 8 ) | (2*chr_count + 2);

  return _desc_str;
}

uint8_t const* tud_descriptor_bos_cb(void)
{
  return desc_bos;
}

// Invoked when control request is arrived.
bool tud_vendor_control_xfer_cb(uint8_t rhport, uint8_t stage, tusb_control_request_t const * request)
{
  if (stage != CONTROL_STAGE_SETUP) return true;

  if( request->bmRequestType_bit.type == TUSB_REQ_TYPE_VENDOR && request->bRequest == 0x01 && request->wValue == 0x0000 && request->wIndex == 0x0007 ) {
    // MS OS 2.0 descriptor request
    return tud_control_xfer(rhport, request, (void*)desc_ms_os_20, sizeof(desc_ms_os_20));
  }
  return false;
}