# MorningMessenger
Version `0.4.11 alpha`<br>
This commandline application allows you to send p2p encrypted messages using RSA and AES-256-CTR.

## Install Procedure
1. Install mysql server and make sure you have credentials you can use
2. To install and compile both client and server, run `make first_build`
3. Launch the application by running `./run` from the terminal and follow the instructions to setup the database.
4. From inside the application, go to the server menu via the `serverctrl` command or by entering `1`
5. Start the messenger service by then running `start`, which activates the service by using systemctrl.
6. You should be able to use the application from here, and can even configure the back end to start on system boot.

## Uninstall Procedure
1. Run the command `make full_uninstall` and all processes, folders, and users, and system configurations created by the installer will be removed. You can optionally remove the installed dependancies if you desire; but by default this is skipped.

### Changes in this version.
1. Refactored the locations of all the header files.
2. Refactored access requests, added "findFriends" menu option.
3. You can now update and view the config file directly in the application.
4. Added database-based user authentication.
5. Added server config table.
6. Added ability to reconfigure all service variables, key pairs, alias, hostname, and port name.
7. Service now runs independant of the client and can be configured to run on system startup.
8. Received untrusted keys are now stored in the database.
9. Updated key management to use the database.
10. Added menu for managing trusted keys.
11. overhauled the messenger inbox system.

### Messages From Morning Star
1. I'll be looking into creating a notification system so that you don't need to manually refresh the message cache.
2. Will test TOR compatibility soon..
3. Need to implement the live chat feature now that AF UNIX support was added.
4. Add mutual authentication.
5. Implement a gui ;)
