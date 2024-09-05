# nafco01 Project

**Description**:  
The nafco01 project is designed to collect data from a PLC using the RS485 interface and transmit it to a LoRaWAN network via ChirpStack. The system captures the data from the PLC, formats it, and sends it using LoRaWAN to an MQTT broker, where it can be processed and visualized.

## System Overview
- **RS485 to LoRaWAN**: Data is collected via the RS485 interface from the PLC, and the messages are sent via LoRaWAN.
- **LoRaWAN Gateway**: Communicates with the LoRaWAN server (ChirpStack).
- **MQTT Broker**: Messages are published to specific MQTT topics for data analysis.

## Prerequisites
- **ChirpStack setup**: Ensure ChirpStack is installed and configured.
- **PLC with RS485 output**: Configured to send the necessary data.
- **LoRaWAN Gateway**: Connected to the LoRaWAN network.
- **MQTT Broker**: Set up to receive data from ChirpStack.

## Instructions

### 1. Subscribing to the MQTT Topic
To monitor incoming data from the PLC, you can use the following command to subscribe to the appropriate topic:

```bash
mosquitto_sub -h 192.168.1.170 -t "application/+/device/+/event/up" -v
```

### 2. Interpreting the Message
Messages received from ChirpStack are formatted in JSON and contain several fields, including the target data in the `"object"` field under `"hexString"`. 

Example message:

```json
{
  "application": "4a881480-2ede-464a-94d0-f1958ae9a86d",
  "device": "88883c84279de948",
  "event": "up",
  "object": {
    "hexString": "01031400df00e0000200030004000500060007000800097af0"
  }
}
```

- **hexString**: This contains the data collected from the PLC via RS485.

### 3. Data Processing
The `"hexString"` value represents the raw data from the PLC. This data can be converted to meaningful information based on the PLC’s data format.

For example:

- `0103` might represent a specific function code.
- `1400df00e0...` might represent specific sensor values or status codes.

### 4. Device Configuration
Ensure that your device is properly configured in ChirpStack:

- **Device Profile**: Use a device profile like `Base_ABP` for your devices.
- **Application**: The device is part of the "nafco" application in ChirpStack.
- **LoRaWAN Settings**: 
  - Device Class: CLASS_A
  - Data Rate: Adjust based on the project needs.
  
### 5. LoRaWAN Uplink Data Example
Here is a detailed example of an uplink message:

```json
{
  "deviceName": "nafco9",
  "devEui": "88883c84279de948",
  "fCnt": 1294,
  "fPort": 2,
  "data": "AQMUAN8A4AACAAMABAAFAAYABwAIAAl68A==",
  "object": {
    "hexString": "01031400df00e0000200030004000500060007000800097af0"
  }
}
```

### 6. Deployment and Testing
- **Step 1**: Connect the PLC to the RS485 interface.
- **Step 2**: Ensure the LoRaWAN gateway is online and connected to ChirpStack.
- **Step 3**: Start data collection from the PLC and verify that data is being published to the MQTT topic.

### 7. Troubleshooting
- **No Data in MQTT**: Check that the device is correctly configured in ChirpStack and that the LoRaWAN gateway is operational.
- **Incorrect Data Format**: Verify the PLC's data format and ensure the RS485 connection is correct.

## Support
For any issues or additional assistance, please contact [Your Support Contact].

---
以下是針對 **nafco01 專案** 的說明文件範例，您可以提供給客戶使用此系統的指南：

---

# nafco01 專案

**描述**：  
nafco01 專案設計用於從 PLC 透過 RS485 接口收集資料，並通過 LoRaWAN 網路將其傳送到 ChirpStack 系統。系統將來自 PLC 的資料進行格式化處理，並透過 LoRaWAN 發送至 MQTT Broker，供進一步分析和顯示。

## 系統概覽
- **RS485 至 LoRaWAN**：系統透過 RS485 接口從 PLC 收集資料，並將訊息通過 LoRaWAN 發送。
- **LoRaWAN 閘道器**：與 LoRaWAN 伺服器（ChirpStack）進行通訊。
- **MQTT Broker**：訊息會發布至指定的 MQTT 主題，以供資料分析。

## 先決條件
- **ChirpStack 設置**：確保已安裝並配置好 ChirpStack 系統。
- **具備 RS485 輸出的 PLC**：配置為傳送必要的資料。
- **LoRaWAN 閘道器**：已連接至 LoRaWAN 網路。
- **MQTT Broker**：設置為接收來自 ChirpStack 的資料。

## 使用說明

### 1. 訂閱 MQTT 主題
若要監控從 PLC 傳送的資料，您可以使用以下指令來訂閱相關的主題：

```bash
mosquitto_sub -h 192.168.1.170 -t "application/+/device/+/event/up" -v
```

### 2. 解讀訊息
從 ChirpStack 收到的訊息是以 JSON 格式表示，並包含多個欄位，其中目標資料位於 `"object"` 欄位下的 `"hexString"`。

範例訊息：

```json
{
  "application": "4a881480-2ede-464a-94d0-f1958ae9a86d",
  "device": "88883c84279de948",
  "event": "up",
  "object": {
    "hexString": "01031400df00e0000200030004000500060007000800097af0"
  }
}
```

- **hexString**：這裡包含從 PLC 經由 RS485 收集到的資料。

### 3. 資料處理
`"hexString"` 值代表從 PLC 收集到的原始資料。這些資料可以根據 PLC 的資料格式轉換成有意義的資訊。

例如：

- `0103` 可能代表特定的功能代碼。
- `1400df00e0...` 可能代表特定的感測器數值或狀態碼。

### 4. 設備配置
確保設備已在 ChirpStack 中正確配置：

- **設備配置檔**：使用類似 `Base_ABP` 的設備配置檔。
- **應用程式**：設備屬於 ChirpStack 中的 "nafco" 應用程式。
- **LoRaWAN 設置**： 
  - 設備類別：CLASS_A
  - 資料傳輸速率：根據專案需求進行調整。

### 5. LoRaWAN 上行數據範例
以下是上行訊息的詳細範例：

```json
{
  "deviceName": "nafco9",
  "devEui": "88883c84279de948",
  "fCnt": 1294,
  "fPort": 2,
  "data": "AQMUAN8A4AACAAMABAAFAAYABwAIAAl68A==",
  "object": {
    "hexString": "01031400df00e0000200030004000500060007000800097af0"
  }
}
```

### 6. 部署與測試
- **步驟 1**：將 PLC 連接至 RS485 接口。
- **步驟 2**：確保 LoRaWAN 閘道器在線並已連接至 ChirpStack。
- **步驟 3**：開始從 PLC 收集資料，並驗證資料是否已發布到 MQTT 主題。

### 7. 疑難排解
- **MQTT 中無數據**：檢查設備是否在 ChirpStack 中正確配置，並確保 LoRaWAN 閘道器正常運作。
- **資料格式錯誤**：驗證 PLC 的資料格式，並確保 RS485 連接正常。