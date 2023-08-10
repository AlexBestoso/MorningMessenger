# MorningMessenger
Version `0.5.7 alpha`<br>
This commandline application allows you to send p2p encrypted messages using RSA and AES-256-CTR.<br>
<b>Tested on Linux Mint</b>

## Install Procedure
1. Install mysql server and make sure you have credentials you can use
2. To install and compile both client and server, run `make first_build`. If this is your first time installing the libraries, you may need to run this twice
3. (optional) To set up TOR compatibility, run `make tor`
4. From inside the application, go to the server menu via the `serverctrl` command or by entering `1`
5. Start the messenger service by then running `start`, which activates the service by using systemctrl.
6. You should be able to use the application from here, and can even configure the back end to start on system boot.

## Uninstall Procedure
1. Run the command `make full_uninstall` and all processes, folders, and users, and system configurations created by the installer will be removed. You can optionally remove the installed dependancies if you desire; but by default this is skipped. Tor configurations won't be removed automatically.

### Changes in this version.
1. Added SoundSnake and TorSnake to the application.
2. Updated server config to allow you to run in tor mode.
3. SQL connection is no longer forced through tor when running in tormode.
4. Fixed bug in cli text input that resulted in the first char being cut off.
5. Fixed message subject overflow bug.
6. Fixed desanitization bug for received messages and message subjects.

### Messages From Morning Star
1. I'll be looking into creating a notification system so that you don't need to manually refresh the message cache.
2. Need to implement the live chat feature now that AF UNIX support was added. (maybe not)
3. Implement a gui ;)

