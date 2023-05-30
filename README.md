# MorningMessenger

## Install Procedure
1. Run the command `make install` and the requirements for the application will be installed and configured. Authentication may be requred.
2. Run the command `make` and the application will compile.
3. launch the application by running `./run` from the terminal.

## Uninstall Procedure
1. Run the command `make uninstall` and all processes, folders, and users created by the installer will be removed. You can optionally remove the installed dependancies if you desire; but by default this is skipped.

### Changes in this version.
1. Added multi process controlls for launching the server.
2. Added client side menu and menu controls.
3. Added networked communications between server and client code.
4. Added functions for implementing the public key exchange.

### Messages From Morning Star
There is currently no key exchange but the infrastructure for implementing it is now in place.
In the next update you can expect to see a fully functional networked key exhcnage and for the core key management system to be created.
There's still no messaging/chat capability, lol, but it's comming soon. Before I can implement a live chat, which will allow the server hoster to join chats via the forked server connection, I need to update my netsnake code to support `AF_UNIX` clients and servers. 
