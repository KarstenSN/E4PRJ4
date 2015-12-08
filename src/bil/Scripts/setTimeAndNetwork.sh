#!/bin/bash

# Scan network
#sudo iwlist wlan0 scan

cat > /etc/wpa_supplicant/wpa_supplicant.conf <<END_TEXT
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1
network={
	ssid="Karstens iPhone"
 	psk="Silkeborg"
}
END_TEXT

ifdown wlan0
sleep 1
ifup wlan0
sleep 1
ifup wlan0
sleep 1
ifconfig wlan0
sleep 1
echo Done..