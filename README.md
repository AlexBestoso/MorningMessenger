# MorningMessenger
You can now view and share messages with other people using this software, assuming that you've shared your public keys with each other. Which is done automatically by the system.

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

### Messages From Morning Star
1. Version `0.4.2 alpha` (Messaging temporarally disabled.)
2. I'll be looking into creating a notification system so that you don't need to manually refresh the message cache.
3. I'll also be looking into isolating the server code into a seperate binary so that it can be run as a service on system startup.
4. Will test TOR compatibility soon..
6. Need to refactor the config file, server code, and send/recv message code.
7. Need to implement the live chat feature now that AF UNIX support was added.
8. Add mutual authentication.
9. need to add system for managing and revoking trusted keys.
10. Implement a gui ;)
