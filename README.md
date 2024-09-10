---

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

---