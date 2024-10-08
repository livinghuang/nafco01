#include "global.h"
bool fetch_data_success = false;
static void prepareTxFrame(uint8_t port);
/* OTAA para*/
uint8_t devEui[] = {0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x02, 0x02};
uint8_t appEui[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t appKey[] = {0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88};

/* ABP para*/
uint8_t nwkSKey[] = {0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x02, 0x02};
uint8_t appSKey[] = {0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x02, 0x02};
uint32_t devAddr = (uint32_t)0x88880202;
/*LoraWan channelsmask, default channels 0-7*/
uint16_t userChannelsMask[6] = {0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
/*LoraWan region,`1qw
    LORAMAC_REGION_AS923        //  AS band on 923MHz
    LORAMAC_REGION_AU915        //  Australian band on 915MHz
    LORAMAC_REGION_CN470        //  Chinese band on 470MHz
    LORAMAC_REGION_CN779        //  Chinese band on 779MHz
    LORAMAC_REGION_EU433        //  European band on 433MHz
    LORAMAC_REGION_EU868        //  European band on 868MHz
    LORAMAC_REGION_KR920        //  South korean band on 920MHz
    LORAMAC_REGION_IN865        //  India band on 865MHz
    LORAMAC_REGION_US915        //  North american band on 915MHz
    LORAMAC_REGION_US915_HYBRID //  North american band on 915MHz with a maximum of 16 channels
    LORAMAC_REGION_AU915_SB2    //  Australian band on 915MHz Subband 2
    LORAMAC_REGION_AS923_AS1    //  AS band on 922.0-923.4MHz
    LORAMAC_REGION_AS923_AS2    //  AS band on 923.2-924.6MHz
*/
LoRaMacRegion_t loraWanRegion = LORAMAC_REGION_AS923_AS2;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t loraWanClass = CLASS_A;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 15000;

/*OTAA or ABP*/
bool overTheAirActivation = false;

/*ADR enable*/
bool loraWanAdr = true;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = true;

/* Application port */
uint8_t appPort = 2;
/*!
 * Number of trials to transmit the frame, if the LoRaMAC layer did not
 * receive an acknowledgment. The MAC performs a datarate adaptation,
 * according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
 * to the following table:
 *
 * Transmission nb | Data Rate
 * ----------------|-----------
 * 1 (first)       | DR
 * 2               | DR
 * 3               | max(DR-1,0)
 * 4               | max(DR-1,0)
 * 5               | max(DR-2,0)
 * 6               | max(DR-2,0)
 * 7               | max(DR-3,0)
 * 8               | max(DR-3,0)
 *
 * Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
 * the datarate, in case the LoRaMAC layer did not receive an acknowledgment
 */
uint8_t confirmedNbTrials = 4;
static void prepareTxFrame(uint8_t port);
void generate_lorawan_settings_by_chip_id()
{
  uint64_t chipid = ESP.getEfuseMac();
  Serial.printf("ESP32ChipID=%04X%08X\n", (uint16_t)(chipid >> 32), (uint32_t)chipid);

  devAddr = (uint32_t)(chipid >> 32) * (uint32_t)chipid;
  // 将MAC地址转换为字符串形式
  char chipidStr[17];
  snprintf(chipidStr, sizeof(chipidStr), "%016llx", chipid);

  Serial.print("devEUI:");
  memcpy(&devEui[2], &chipid, sizeof(devEui) - 2);
  print_bytes((uint8_t *)&devEui, sizeof(devEui));
  Serial.print("devAddr:");
  print_bytes_reverse((uint8_t *)&devAddr, sizeof(devAddr));
  memcpy(appKey, chipidStr, 16);
  memcpy(appSKey, chipidStr, 16);
  memcpy(nwkSKey, chipidStr, 16);
  Serial.print("appKey:");
  print_bytes((uint8_t *)&appKey, sizeof(appKey));
  Serial.print("nwkSKey:");
  print_bytes((uint8_t *)&nwkSKey, sizeof(nwkSKey));
  Serial.print("appSKey:");
  print_bytes((uint8_t *)&appSKey, sizeof(appSKey));
}

void setup()
{
  Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);
  Serial.begin(115200);
  Serial.println("Hello xda002!");
  generate_lorawan_settings_by_chip_id();
  deviceState = DEVICE_STATE_INIT;
  rs485_init();
  delay(100);
}
void loop()
{
  switch (deviceState)
  {
  case DEVICE_STATE_INIT:
  {
    LoRaWAN.init(loraWanClass, loraWanRegion);
    // both set join DR and DR when ADR off
    if (!loraWanAdr)
    {
      LoRaWAN.setDefaultDR(3);
    }
    break;
  }
  case DEVICE_STATE_JOIN:
  {
    LoRaWAN.join();
    break;
  }
  case DEVICE_STATE_SEND:
  {
    prepareTxFrame(appPort);
    LoRaWAN.send();
    digitalWrite(pVext, LOW);
    deviceState = DEVICE_STATE_CYCLE;
    break;
  }
  case DEVICE_STATE_CYCLE:
  {
    txDutyCycleTime = appTxDutyCycle + randr(-APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND);
    LoRaWAN.cycle(txDutyCycleTime);
    deviceState = DEVICE_STATE_SLEEP;
    break;
  }
  case DEVICE_STATE_SLEEP:
  {
    LoRaWAN.sleep(loraWanClass);
    break;
  }
  default:
  {
    deviceState = DEVICE_STATE_INIT;
    break;
  }
  }
}
static void prepareTxFrame(uint8_t port)
{
  Serial.println("Prepare Frame");
  int bytesRead;
  byte responseBuffer[256]; // Adjust the buffer size as needed

  rs485_init();
  delay(100);
  Serial.println("RS485 process");
  byte modbusQuery[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x0A, 0xC5, 0xCD};
  Serial.print("Sent:");
  printHex(modbusQuery, sizeof(modbusQuery));
  uint32_t startTime = millis();
  Serial1.setTimeout(1000);  // Set timeout to 1000 milliseconds (1 second)
  digitalWrite(RE_DE, HIGH); // Enable transmission
  Serial1.write(modbusQuery, sizeof(modbusQuery));
  Serial1.flush();
  digitalWrite(RE_DE, LOW); // Disable transmission
  bytesRead = Serial1.readBytes(responseBuffer, sizeof(responseBuffer));
  // Check if there is any response
  if (bytesRead > 0)
  {
    // Process the received bytes here
    Serial.print("Received:");
    printHex(responseBuffer, bytesRead);
  }
  else
  {
    Serial.println("timeout");
    appDataSize = 4;
    appData[0] = 0x00;
    appData[1] = 0x01;
    appData[2] = 0x02;
    appData[3] = 0x03;
  }
  digitalWrite(pVext, LOW);
  appDataSize = bytesRead;
  memcpy(appData, responseBuffer, bytesRead);
}