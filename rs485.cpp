#include "global.h"

#define BAUD_RATE 9600
#define SERIAL_CONFIG SERIAL_8N1

uint16_t convetHexToDec(byte hexByte)
{
  return ((hexByte / 16) * 10 + (hexByte % 16));
}

double accumulatedValueConvert(byte *byteArray)
{
  double total_number = 0;
  for (int i = 0; i < 5; i++)
  {
    // printf("%02X ", byteArray[i + 4]);
    total_number = total_number * 100;
    uint16_t dec = convetHexToDec(*(byteArray + i));
    printf("%d ", dec);
    total_number += dec;
  }
  printf("\n");
  printf("%f\n", total_number);
  int point_index = (int)*(byteArray + 6);
  printf("point_index: %d\n", point_index);

  for (int i = 0; i < point_index; i++)
  {
    total_number = total_number / 10;
  }
  printf("\n");

  printf("%f\n", total_number);
  return total_number;
}

int16_t daysValueConvert(byte *byteArray)
{
  int16_t total_number = 0;
  for (int i = 0; i < 2; i++)
  {
    // printf("%02X ", byteArray[i + 4]);
    total_number = total_number * 100;
    uint16_t dec = convetHexToDec(*(byteArray + i));
    printf("%d ", dec);
    total_number += dec;
  }
  printf("\n");
  printf("%d\n", total_number);
  return total_number;
}

void rs485_init()
{
  Serial.println("RS485 init");
  pinMode(pVext, OUTPUT);
  digitalWrite(pVext, HIGH);
  Serial1.begin(BAUD_RATE, SERIAL_CONFIG, RO, DI);
  pinMode(RE_DE, OUTPUT);
  digitalWrite(RE_DE, HIGH);
}

bool rs485_process(void)
{
  Serial.println("RS485 process");
  byte modbusQuery[] = {0x01, 0x03, 0x03, 0x04, 0x00, 0x09, 0xc4, 0x49};
  Serial.print("Sent:");
  printHex(modbusQuery, sizeof(modbusQuery));
  uint8_t try_count = 0;
  while (try_count < 3)
  {
    digitalWrite(RE_DE, HIGH); // Enable transmission
    Serial1.write(modbusQuery, sizeof(modbusQuery));
    Serial1.flush();
    digitalWrite(RE_DE, LOW); // Disable transmission
    byte responseBuffer[256]; // Adjust the buffer size as needed
    int bytesRead = Serial1.readBytes(responseBuffer, sizeof(responseBuffer));
    uint32_t startTime = millis();
    // Wait for the response
    while (Serial1.available() == 0 && (millis() - startTime) < 500)
    {
    }
    // Check if there is any response
    if (bytesRead > 0)
    {
      // Process the received bytes here
      Serial.print("Received:");
      printHex(responseBuffer, bytesRead);
    }
    Serial.println("fetch data failed, try again");
    try_count++;
  }
  return false;
}

bool fetch_data_process(void)
{
  rs485_init();
  if (!rs485_process())
  {
    return false;
  }
  Serial.println();
  Serial.flush();
  return true;
}