#!/bin/bash
ServiceUser="MorningNoLogin"
NoLoginPath=$(which nologin)
StorageLocation="/var/morningService/MMS_Storage"

mkdir ./bin 1>&2 > /dev/null

echo "[INFO] Checking for required libraries."
dpkg -l libxml2-dev >/dev/null
if [ "$?" == "0" ]; then
	echo "[+] libxml2-dev found!"
else
	echo "[-] libxml2-dev not found! Attempting to install."
	sudo apt-get install libxml2-dev -y
	if [ "$?" == "1" ]; then
		echo "[ERROR] Failed to install libxml2-dev. Aborting."
		exit 1
	fi
	echo "[INFO] Creating symlink for libxml2 source code files."
	sudo ln -s /usr/include/libxml2/libxml /usr/include/libxml
	if [ "$?" == "1" ];then
		echo "[ERROR] Failed to create libxml symlink. This MAY result in code files failing to compile."
	fi
	echo "[+] Successfully installed libxm2-dev"
fi

dpkg -l freeglut3-dev >/dev/null
if [ "$?" == "0" ]; then
        echo "[+] freeglut3-dev found!"
else
        echo "[-] freeglut3 not found! Attempting to install."
        sudo apt-get install freeglut3-dev -y
        if [ "$?" == "1" ]; then
                echo "[ERROR] Failed to install freeglut3-dev. Aborting."
                exit 1
        fi
        echo "[+] Successfully installed freeglut3-dev"
fi

dpkg -l libmysql++-dev > /dev/null
if [ "$?" == "0" ]; then
	echo "[+] libmysql++-dev found!"
else
	echo "[-] libmysql++-dev not found! Attempting to install."
	sudo apt-get install libmysql++-dev -y
	if [ "$?" == "1" ]; then
		echo "[ERROR] Failed to install libmysql++-dev. Aborting"
		exit 1
	fi
fi

dpkg -l libssl-dev >/dev/null
if [ "$?" == "0" ]; then
        echo "[+] libssl-dev found!"
else
        echo "[-] libssl-dev not found! Attempting to install."
        sudo apt-get install libssl-dev -y
        if [ "$?" == "1" ]; then
                echo "[ERROR] Failed to install libssl-dev. Aborting."
                exit 1
        fi
fi

dpkg -l g++ >/dev/null
if [ "$?" == "0" ]; then
	echo "[+] g++ compiler found!"
else 
	echo "[-] g++ compiler not found. Attempting to install."
	sudo apt-get install g++ -y
	if [ "$?" == "1" ]; then
                echo "[ERROR] Failed to install g++. Aborting."
                exit 1
        fi
	echo "[+] Successfully installed the g++ compiler."
fi

dpkg -l libclalsadrv-dev >/dev/null
if [ "$?" == "0" ]; then
	echo "[+] libclalsadrv-dev audio libs found!"
else
	echo "[-] libclalsadrv-dev not found. Attempting to install"
	sudo apt-get install libclalsadrv-dev -y
	if [ "$?" == "1" ]; then
		echo "[ERROR] Failed to install libclalsadrv-dev. Aborting."
		exit 1
	fi
	echo "[+] Successfully installed libclalsadrv-dev."
fi

dpkg -l libpulse-dev >/dev/null
if [ "$?" == "0" ]; then
        echo "[+] libpulse-dev audio libs found!"
else
        echo "[-] libpulse-dev not found. Attempting to install"
        sudo apt-get install libpulse-dev -y
        if [ "$?" == "1" ]; then
                echo "[ERROR] Failed to install libpulse-dev. Aborting."
                exit 1
        fi
        echo "[+] Successfully installed libpulse-dev."
fi

echo "[INFO] Checking if service user '$ServiceUser' exists."
if id "$ServiceUser" > /dev/null 2>&1; then
	echo "[+] '$ServiceUser' found! Not adding."
else
	echo "[-] '$ServiceUser' not found. Attempting to add service user."
	sudo useradd --system -s $NoLoginPath $ServiceUser
	if [ "$?" == "1" ]; then
		echo "[ERROR] Failed to create system user. Aborting."
		exit 1
	fi
	echo "[+] Successfully created system user '$ServiceUser'. They're using the shell '$NoLoginPath'"
	echo "[INFO] If the user's shell doesn't point to somthing similar to /sbin/nologin, please report an issue on github."
fi

echo "[INFO] Making storage directory '$StorageLocation'"
test -d $StorageLocation
if [ "$?" != "0" ]; then
	sudo mkdir -p $StorageLocation 2>/dev/null
	if [ "$?" != "0" ];then
		echo "[ERROR] Failed to create storage directory. Aborting."
		exit 1
	fi
	sudo chmod 770 $StorageLocation
else
	echo "[W] Storage location already exits!"
fi

echo "[INFO] Configuring ownership of the storage file."
sudo chown -R $ServiceUser:$ServiceUser /var/morningService
#sudo chown -R $(whoami):$(whoami) /var/morningService
if [ "$?" == "0" ];then
	echo "[+] Successfully configured storage ownership!"
else
	echo "[ERROR] Failed to configure storage ownership. Aborting."
	exit 1
fi

echo "[INFO] Configuring storage permissions."
sudo chmod 774 /var/morningService
if [ "$?" == "0" ];then
	echo "[+] Successfully configured storage permissions."
else
	echo "[ERROR] Failed to configure storage permissions. Aborting."
	exit 1
fi

sudo usermod -a -G $ServiceUser $(whoami) 
echo -en "\n\n\n\t\tYOU WILL NEED TO REBOOT BEFORE RUNNING FOR NEW PERMISSIONS TO BE PROPERLY APPLIED. tank you, that is all. :)\n\n"

exit 0

