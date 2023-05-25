#!/bin/bash
ServiceUser="MorningNoLogin"
StorageLocation="$(pwd)/MMS_Storage"

echo "[INFO] Deleting application storage directory."
sudo rm -rf $StorageLocation

echo "[INFO] Killing all processes belonging to service user '$ServiceUser'"
sudo killall -u $ServiceUser

echo "[INFO] Removing service user '$ServiceUser'"
sudo userdel -f $ServiceUser

# this one is redundant
echo "[INFO] Removing service user group."
sudo groupdel $ServiceUser 2>/dev/null

echo "Would you like to remove libxml2-dev, libssl-dev, and g++? (N/y)"
read UserInput

if [ "$UserInput" == "y" ]; then
	sudo apt-get remove libxml2-dev libssl-dev g++
fi
