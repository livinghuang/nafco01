### Guide to Connect to the Device and Configure Static IP

#### Step 1: Connecting to the Device via SSH

1. **Obtain the device's IP address.**  
   Ensure that the device is powered on and connected to your local network. You can find the device's current IP address through your router or by using a network scanning tool.

2. **Open a terminal (Linux/macOS) or an SSH client like PuTTY (Windows).**

3. **Connect to the device using SSH.**  
   In the terminal or SSH client, type the following command, replacing `ip.address` with the actual IP address of your device:

   ```
   ssh root@ip.address
   ```

4. **Enter the default password** when prompted:  
   ```
   linxdot
   ```

   You should now have access to the device's command-line interface.

#### Step 2: Changing the Network Configuration from DHCP to Static IP

1. **Backup the network configuration**  
   Before making changes, it is recommended to back up the existing network configuration. Run the following command:

   ```
   cp /etc/config/network /etc/config/network.backup
   ```

2. **Edit the network configuration file**  
   Open the network configuration file in the `vi` editor:

   ```
   vi /etc/config/network
   ```

3. **Modify the interface section** for the LAN to set a static IP address. Look for the section labeled `config interface 'lan'` and modify it as follows:

   ```
   config interface 'lan'
       option proto 'static'
       option ipaddr '192.168.1.100'    # Replace with the desired static IP address
       option netmask '255.255.255.0'   # Set the appropriate subnet mask
       option gateway '192.168.1.1'     # Replace with your router's IP address (gateway)
       option dns '8.8.8.8 8.8.4.4'     # Set the DNS servers (e.g., Google DNS)
   ```

4. **Save and exit the editor**  
   To save the changes in `vi`, follow these steps:
   - Press `ESC` to exit insert mode.
   - Type `:wq` and press `Enter` to save the file and exit.

5. **Restart the network service**  
   After making the changes, restart the network service to apply the new static IP configuration:

   ```
   /etc/init.d/network restart
   ```

6. **Verify the changes**  
   Check if the static IP address has been successfully applied by running the following command:

   ```
   ifconfig
   ```

   The output should display the new static IP address under the `lan` interface.

---

#### Step 3: Additional Tips

- **Troubleshooting:** If you lose connection after changing to a static IP, make sure the static IP is within the same range as your local network and that no other devices are using the same IP.
- **Reverting changes:** If something goes wrong, you can restore the original DHCP configuration using the backup:

  ```
  cp /etc/config/network.backup /etc/config/network
  /etc/init.d/network restart
  ```