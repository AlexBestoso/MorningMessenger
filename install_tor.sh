#!/bin/bash
ServiceUser="MorningNoLogin"
NoLoginPath=$(which nologin)
StorageLocation="/var/morningService/MMS_Storage"

mkdir ./bin 1>&2 > /dev/null

echo "[INFO] Checking if tor is installed"
sudo apt-get install tor -y
if [ "$?" == "1" ]; then
	echo "[ERROR] Failed to install tor. Aborting."
	exit 1
else
       	echo "[+] Successfully installed tor"
fi
sudo service tor stop;
sudo service tor start;
sudo service tor stop;

TorRc="/etc/tor/torrc"

ServiceDir="HiddenServiceDir /var/lib/tor/morningService"
ServicePort="HiddenServicePort 21345 127.0.0.1:21345"
ServiceVersion="HiddenServiceVersion 3"

sudo mkdir /var/lib/tor/morningService
sudo chmod 700 /var/lib/tor/morningService
sudo chown debian-tor:debian-tor /var/lib/tor/morningService
#sudo usermod -a -G MorningNoLogin debian-tor
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
sudo killall -u debian-tor
sudo service tor start
sudo service tor status

echo -en "\nThe service appears to be up.\n"

sudo cat /var/lib/tor/morningService/hostname
echo -en "\n----^ set this as the host name in morning messenger. You can find it again in the file /var/lib/tor/morningService/hostname\n"
