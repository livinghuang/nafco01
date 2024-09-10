# **nafco01 Project Guide**

### **Description**  
The **nafco01** project collects data from a PLC via RS485 and transmits it to a LoRaWAN network using ChirpStack. The collected data is sent via LoRaWAN to an MQTT broker for further analysis and visualization.

### **System Overview**  
- **RS485 to LoRaWAN**: Collects data from the PLC and transmits it via LoRaWAN.
- **LoRaWAN Gateway**: Connects to the ChirpStack LoRaWAN server.
- **MQTT Broker**: Receives and stores the messages for data processing.

### **Prerequisites**
- **ChirpStack**: Installed and configured.
- **PLC with RS485 output**: Configured to send necessary data.
- **LoRaWAN Gateway**: Connected to the LoRaWAN network.
- **MQTT Broker**: Set up to receive LoRaWAN data from ChirpStack.

---

## **Instructions**

### 1. **Subscribing to the MQTT Topic**
To monitor incoming data from the PLC, subscribe to the following MQTT topic to receive uplink messages:

```bash
mosquitto_sub -h 192.168.1.170 -t "application/+/device/+/event/up" -v
```

### 2. **Interpreting Incoming Messages**
Each message from ChirpStack is in JSON format, and the target data is located in the `"object"` field under `"hexString"`.

Example JSON message:

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

- **hexString**: Represents the raw data collected from the PLC.

### 3. **Processing the Data**
The `"hexString"` contains the raw data received from the PLC, which can be converted into meaningful information. 

For example:
- `0103` may represent a specific function code.
- `1400df00e0...` could contain sensor values or status codes.

### 4. **Configuring Your Device in ChirpStack**
Ensure the device is configured correctly in ChirpStack:
- **Device Profile**: Use a suitable device profile such as `Base_ABP`.
- **Application**: Add the device under the "nafco" application in ChirpStack.
- **LoRaWAN Settings**: 
  - Device Class: `CLASS_A`
  - Data Rate: Adjust as needed for your project.

### 5. **Example LoRaWAN Uplink Message**
Here is an example of a LoRaWAN uplink message:

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

### 6. **Deployment and Testing Steps**
1. **Connect the PLC** to the RS485 interface.
2. **Ensure the LoRaWAN gateway** is connected to the network and ChirpStack.
3. **Start data collection** from the PLC and verify that the data is being published to the MQTT topic.

### 7. **Troubleshooting**
- **No Data in MQTT**: Check the device configuration in ChirpStack and ensure the LoRaWAN gateway is operational.
- **Incorrect Data Format**: Verify that the RS485 connection is correct and check the PLC’s data format.

---

## **Setting Up Linxdot LoRaWAN Gateway from DHCP to Static IP**

### Step 1: Connect to the Device via SSH

1. **Obtain the device's IP address** using your router's interface or a network scanning tool.
2. **Open a terminal** (Linux/macOS) or an SSH client like PuTTY (Windows).
3. **SSH into the device** by running the following command:

   ```bash
   ssh root@<ip.address>
   ```

4. **Enter the default password** when prompted:

   ```bash
   linxdot
   ```

You should now have access to the device's command-line interface.

### Step 2: Change Network Configuration from DHCP to Static IP

1. **Backup the current network configuration**:

   ```bash
   cp /etc/config/network /etc/config/network.backup
   ```

2. **Edit the network configuration file** using the `vi` editor:

   ```bash
   vi /etc/config/network
   ```

3. **Modify the LAN interface section** to use a static IP address. Replace the default settings with the following:

   ```bash
   config interface 'lan'
       option proto 'static'
       option ipaddr '192.168.1.100'    # Replace with desired static IP
       option netmask '255.255.255.0'   # Set appropriate subnet mask
       option gateway '192.168.1.1'     # Replace with your router's gateway IP
       option dns '8.8.8.8 8.8.4.4'     # Set DNS servers (e.g., Google DNS)
   ```

4. **Save and exit the editor**:  
   - Press `ESC` to exit insert mode.
   - Type `:wq` and press `Enter` to save the changes and exit.

5. **Restart the network service**:

   ```bash
   /etc/init.d/network restart
   ```

6. **Verify the changes** by running the following command:

   ```bash
   ifconfig
   ```

   The output should show the new static IP address for the `lan` interface.

---

### Additional Tips:
- **Troubleshooting**: If you lose connection, verify the static IP settings to ensure they match your network configuration.
- **Reverting changes**: If needed, restore the original DHCP configuration using the backup:

   ```bash
   cp /etc/config/network.backup /etc/config/network
   /etc/init.d/network restart
   ```

---

## **Support**

For further assistance or troubleshooting, please contact [Your Support Contact Information].

# **nafco01 專案指南**

### **專案描述**  
**nafco01** 專案旨在透過 RS485 介面從 PLC 收集數據，並將其傳輸到使用 ChirpStack 的 LoRaWAN 網絡。所收集的數據將通過 LoRaWAN 發送至 MQTT 中介服務器，進行進一步的分析和可視化。

### **系統概覽**  
- **RS485 到 LoRaWAN**：從 PLC 收集數據並透過 LoRaWAN 傳輸。
- **LoRaWAN 閘道器**：連接到 ChirpStack LoRaWAN 伺服器。
- **MQTT 中介服務器**：接收並存儲消息以進行數據處理。

### **先決條件**
- **ChirpStack**：已安裝並配置。
- **具備 RS485 輸出的 PLC**：已配置以發送所需數據。
- **LoRaWAN 閘道器**：已連接到 LoRaWAN 網絡。
- **MQTT 中介服務器**：已設定以接收來自 ChirpStack 的 LoRaWAN 數據。

---

## **操作說明**

### 1. **訂閱 MQTT 主題**
要監控從 PLC 傳入的數據，可以使用以下命令訂閱對應的 MQTT 主題，接收上行消息：

```bash
mosquitto_sub -h 192.168.1.170 -t "application/+/device/+/event/up" -v
```

### 2. **解讀接收到的消息**
每條來自 ChirpStack 的消息均以 JSON 格式發送，目標數據位於 `"object"` 欄位中的 `"hexString"`。

範例 JSON 消息：

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

- **hexString**：表示從 PLC 收集到的原始數據。

### 3. **處理數據**
`"hexString"` 包含從 PLC 接收到的原始數據，可以根據 PLC 的數據格式轉換為有意義的資訊。

例如：
- `0103` 可能表示特定的功能碼。
- `1400df00e0...` 可能包含感測器數值或狀態碼。

### 4. **在 ChirpStack 中配置您的設備**
確保設備在 ChirpStack 中配置正確：
- **設備檔案**：選擇適合的設備檔案，例如 `Base_ABP`。
- **應用程式**：在 ChirpStack 中將設備加入 "nafco" 應用程式。
- **LoRaWAN 設定**： 
  - 設備類別：`CLASS_A`
  - 數據速率：根據專案需求進行調整。

### 5. **LoRaWAN 上行消息範例**
以下是一個 LoRaWAN 上行消息的範例：

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

### 6. **部署與測試步驟**
1. **將 PLC 連接** 至 RS485 介面。
2. **確保 LoRaWAN 閘道器** 已連接到網絡並已註冊到 ChirpStack。
3. **開始從 PLC 收集數據**，並驗證數據是否成功發布至 MQTT 主題。

### 7. **故障排除**
- **MQTT 中無數據**：檢查設備在 ChirpStack 中的配置，並確認 LoRaWAN 閘道器是否正常工作。
- **數據格式錯誤**：確認 RS485 連接正確，並檢查 PLC 的數據格式。

---

## **將 Linxdot LoRaWAN 閘道器從 DHCP 設置為靜態 IP**

### 步驟 1：透過 SSH 連接設備

1. **透過路由器介面或網絡掃描工具獲取設備的 IP 位址**。
2. **開啟終端機**（Linux/macOS）或 SSH 客戶端（例如 PuTTY for Windows）。
3. **透過 SSH 連接設備**，執行以下命令：

   ```bash
   ssh root@<ip.address>
   ```

4. **輸入默認密碼**：

   ```bash
   linxdot
   ```

您現在應該已進入設備的命令行介面。

### 步驟 2：將網絡配置從 DHCP 更改為靜態 IP

1. **備份當前的網絡配置**：

   ```bash
   cp /etc/config/network /etc/config/network.backup
   ```

2. **使用 `vi` 編輯網絡配置檔案**：

   ```bash
   vi /etc/config/network
   ```

3. **修改 LAN 介面設置**，將其更改為靜態 IP。將默認設置替換為以下內容：

   ```bash
   config interface 'lan'
       option proto 'static'
       option ipaddr '192.168.1.100'    # 將其替換為您需要的靜態 IP
       option netmask '255.255.255.0'   # 設置正確的子網掩碼
       option gateway '192.168.1.1'     # 將其替換為您路由器的閘道 IP
       option dns '8.8.8.8 8.8.4.4'     # 設置 DNS 伺服器（例如 Google DNS）
   ```

4. **保存並退出編輯器**：  
   - 按 `ESC` 退出插入模式。
   - 輸入 `:wq` 並按下 `Enter` 以保存更改並退出。

5. **重新啟動網絡服務**：

   ```bash
   /etc/init.d/network restart
   ```

6. **驗證靜態 IP 設定**，執行以下命令：

   ```bash
   ifconfig
   ```

   輸出應顯示 `lan` 介面的靜態 IP 地址。

---

### 其他提示：
- **故障排除**：如果連接中斷，請檢查靜態 IP 設置是否與您的網絡配置相匹配。
- **還原配置**：如果有必要，可以使用備份還原原始 DHCP 配置：

   ```bash
   cp /etc/config/network.backup /etc/config/network
   /etc/init.d/network restart
   ```

---

## **技術支援**

若需進一步協助或技術支援，請聯繫 [您的支援聯絡資訊]。

