#!/bin/bash
ServiceUser="MorningNoLogin"
NoLoginPath=$(which nologin)
StorageLocation="/var/morningService/MMS_Storage"

mkdir ./bin 1>&2 > /dev/null

echo "[INFO] Checking if tor is installed"
dpkg -l tor >/dev/null
if [ "$?" == "0" ]; then
        echo "[+] tor found!"
else
        echo "[-] tor not found! Attempting to install."
        sudo apt-get install tor -y
        if [ "$?" == "1" ]; then
                echo "[ERROR] Failed to install tor. Aborting."
                exit 1
        fi
        echo "[+] Successfully installed tor"
	sudo service stop tor;
	sudo service start tor;
	sudo service stop tor;
fi

TorRc="/etc/tor/torrc"

ServiceDir="HiddenServiceDir /var/morningService/hiddenService"
ServicePort="HiddenServicePort 21345 127.0.0.1:21345"
ServiceVersion="HiddenServiceVersion 3"

sudo mkdir /var/morningService/hiddenService
sudo chmod 700 /var/morningService/hiddenService
sudo chown debian-tor:debian-tor /var/morningService/hiddenService
sudo usermod -a -G MorningNoLogin debian-tor
echo -en "To get the service running you have to add the following three lines to the $TorRc file\n\n$ServiceDir\n$ServiceVersion\n$ServicePort\n\nWould you like me to do it for you? [y/n]\n"

read UserInput
if [ "$UserInput" == "y" ]; then
	sudo chmod o+w $TorRc
	echo "$ServiceDir" >> $TorRc
	echo "$ServicePort" >> $TorRc
	echo "$ServiceVersion" >> $TorRc
	sudo chmod o-w $TorRc
fi

sudo sed -i 's/#RunAsDaemon 1/RunAsDaemon 1/g' $TorRc

echo "Copying libtorsocks.so to the morningService dir."
SharedObjLoc=$(find / -name "libtorsocks.so" 2>/dev/null)
for shl in $SharedObjLoc
do
	sudo cp $shl /var/morningService/libtorsocks.so
	echo "copied shared object to /var/morningService/libtorsocks.so"
	break
done

sudo -u debian-tor tor

echo -en "\nThe service appears to be up.\n"

sudo cat /var/morningService/hiddenService/hostname
echo -en "\n----^ set this as the host name in morning messenger. You can find it again in the file /var/morningService/hiddenService/hostname\n"
