# MorningMessenger
<p>
Version `0.4.9 alpha` (Messaging temporarally disabled.)<br>
This commandline application allows you to send p2p encrypted messages using RSA and AES-256-CTR.
</p>

## Install Procedure
1. Run the command `make install` and the requirements for the application will be installed and configured. Authentication may be requred.
2. Run the command `make` and the application will compile.
3. launch the application by running `./run` from the terminal.

## Uninstall Procedure
1. Run the command `make uninstall` and all processes, folders, and users created by the installer will be removed. You can optionally remove the installed dependancies if you desire; but by default this is skipped.

### Changes in this version.
1. Received Messages are now encrypted before being stored.
2. Refactored the locations of all the header files.
3. Refactored access requests, added "findFriends" menu option.
4. You can now update and view the config file directly in the application.
5. Added database-based user authentication.
6. Added server config table.
7. Added ability to reconfigure all service variables, key pairs, alias, hostname, and port name.
8. Service now runs independant of the client and can be configured to run on system startup.
9. received untrusted keys are now stored in the database.

### Messages From Morning Star
1. I'll be looking into creating a notification system so that you don't need to manually refresh the message cache.
2. Will test TOR compatibility soon..
3. Need to implement the live chat feature now that AF UNIX support was added.
4. Add mutual authentication.
5. need to add system for managing and revoking trusted keys.
6. Implement a gui ;)

### More super duper important information
Now that the server stores untrusted keys in the database, I need to update the key manager menu to allow you to properly manage your received keys.<br>
After this is done, I'll be finally getting to refactoring the actual messenger, which will operate much more smooth than version 0.3.x alpha.
