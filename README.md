# MorningMessenger
Version `0.6.6 alpha`<br>
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
1. Created first section of the GUI. The setup view has been added.
2. Added GUI login view
3. Added better; but experimental menu controls
4. You can no use the gui to configure the server configurations.
5. Added gui for the find frends feature.
6. Added gui for managing your untrusted and trusted friend keys.
7. Implemented remaining core functionalites in the GUI.
8. modified how the application runs so that we can get audio working.

### Messages From Morning Star
1. I'll be looking into creating a notification system so that you don't need to manually refresh the message cache.
2. Need to implement the live chat feature now that AF UNIX support was added. (maybe not)
3. Going to be adding the audio features in future updates.
4. the current textures in the application are 0 effort, and maybe offensive to some people...I'm not sorry.
5. Application requires X11.
