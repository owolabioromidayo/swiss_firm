// create a config.h from this (change all the values to yours)
#ifndef PROJ_MAIN_CONF_H_
#define PROJ_MAIN_CONF_H_

#include "driver/adc.h"
#include "driver/gpio.h"

#define TEST_MODE 1
#define HOSTNAME "https://www.google.com" // must be https

#define ca_cert \ // this is just an example.
"-----BEGIN CERTIFICATE-----\n" \
"MIIESTCCAzGgAwIBAgITBn+UV4WH6Kx33rJTMlu8mYtWDTANBgkqhkiG9w0BAQsF\n" \
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n" \
"b24gUm9vdCBDQSAxMB4XDTE1MTAyMjAwMDAwMFoXDTI1MTAxOTAwMDAwMFowRjEL\n" \
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEVMBMGA1UECxMMU2VydmVyIENB\n" \
"IDFCMQ8wDQYDVQQDEwZBbWF6b24wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK\n" \
"AoIBAQDCThZn3c68asg3Wuw6MLAd5tES6BIoSMzoKcG5blPVo+sDORrMd4f2AbnZ\n" \
"cMzPa43j4wNxhplty6aUKk4T1qe9BOwKFjwK6zmxxLVYo7bHViXsPlJ6qOMpFge5\n" \
"blDP+18x+B26A0piiQOuPkfyDyeR4xQghfj66Yo19V+emU3nazfvpFA+ROz6WoVm\n" \
"B5x+F2pV8xeKNR7u6azDdU5YVX1TawprmxRC1+WsAYmz6qP+z8ArDITC2FMVy2fw\n" \
"0IjKOtEXc/VfmtTFch5+AfGYMGMqqvJ6LcXiAhqG5TI+Dr0RtM88k+8XUBCeQ8IG\n" \
"KuANaL7TiItKZYxK1MMuTJtV9IblAgMBAAGjggE7MIIBNzASBgNVHRMBAf8ECDAG\n" \
"AQH/AgEAMA4GA1UdDwEB/wQEsdfshjAdBgNVHQ4EFgQUWaRmBlKge5WSPKOUByeW\n" \
"dFv5PdAwHwYDVR0jBBgwFoAUBjMhTTsvAyUlC4IWZzHshBOCggwewYIKwYBBQUH\n" \
"AQEEbzBtMC8GCCsGAQUFBzABhiNddHRwOi8vb2NzcC5yb290Y2ExLmFtYXpvbnRy\n" \
"dXN0LmNvbTA6BggrBgEFBQcwAoYuaHR0cDovL2NydC5yb290Y2ExLmFtYXpvbnRy\n" \
"dXN0LmNvbS9yb290Y2ExLmNlcjA/BgNVHR8EODA2MDSgMqAwhi5odHRwOi8vY3Js\n" \
"LnJvb3RjYTEuYW1hem9udHJ1c3QuY29tL3Jvb3RjYTEuY3JsMBMGA1UdIAQMMAow\n" \
"CAYGZ4EMAQIBMA0GCSqGSIb3DQEBCwUAA4IBAQCFkr41u3nPo4FCHOTjY3NTOVI1\n" \
"59Gt/a6ZiqyJEi+752+a1U5y6iAwYfmXss2lJwJFqMp2PphKg5625kXg8kP2CN5t\n" \
"6G7bMQcT8C8xDZNtYTd7WPD8UZiRKAJPBXa30/AbwuZe0GaFEQ8ugcYQgSn+IGBI\n" \
"8/LwhBNTZTUVEWuCUUBVV18YtbAiPq3yXqMB48Oz+ctBWuZSkbvkNodPLamkB2g1\n" \
"upRyzQ7qDn1X8nn8N8V7YJ6y68AtkHcNSRAnpTitxBKjtKPISLMVCx7i4hncxHZS\n" \
"yLyKQXhw2W2Xs0qLeC1etA+jTGDK4UfLeC0SF7FSi8o5LL21L8IzApar2pR/\n" \
"-----END CERTIFICATE-----\n"
// typedef enum {
//     ADC1_CHANNEL_0 = 0, /*!< ADC1 channel 0 is GPIO36 */
//     ADC1_CHANNEL_1,     /*!< ADC1 channel 1 is GPIO37 */
//     ADC1_CHANNEL_2,     /*!< ADC1 channel 2 is GPIO38 */
//     ADC1_CHANNEL_3,     /*!< ADC1 channel 3 is GPIO39 */
//     ADC1_CHANNEL_4,     /*!< ADC1 channel 4 is GPIO32 */
//     ADC1_CHANNEL_5,     /*!< ADC1 channel 5 is GPIO33 */
//     ADC1_CHANNEL_6,     /*!< ADC1 channel 6 is GPIO34 */
//     ADC1_CHANNEL_7,     /*!< ADC1 channel 7 is GPIO35 */
//     ADC1_CHANNEL_MAX,
// } adc1_channel_t;
//use this for reference when deciding on your ADC pins. 

#define ADC_WIND_VANE_PIN ADC_CHANNEL_0
#define ADC_BATTERY_PIN ADC_CHANNEL_3
#define ADC_UV_PIN ADC_CHANNEL_6

#define GPIO_ANEMOMETER GPIO_NUM_19
#define GPIO_ANEMOMETER_SEL (1ULL << GPIO_ANEMOMETER)

#define GPIO_RAINGAUGE GPIO_NUM_32
#define GPIO_RAINGAUGE_SEL (1ULL << GPIO_RAINGAUGE)

#define GPIO_DSB GPIO_NUM_26

#define GPIO_SDA GPIO_NUM_17
#define GPIO_SCL GPIO_NUM_4
#endif //PROJ_MAIN_CONF_H_

