# MorningMessenger

## Install Procedure
1. Run the command `make install` and the requirements for the application will be installed and configured. Authentication may be requred.
2. Run the command `make` and the application will compile.
3. launch the application by running `./run` from the terminal.

## Uninstall Procedure
1. Run the command `make uninstall` and all processes, folders, and users created by the installer will be removed. You can optionally remove the installed dependancies if you desire; but by default this is skipped.

### Changes in this version.
1. You can now send messages to servers.

### Messages From Morning Star
1. Version `0.3.0 alpha`
2. I'm going to add base64 to encryption snake so that we can encrypt and store data within xml files themselves.
3. Once communications are properly established, I will be modifying the key management to enable mutual authentication.
4. Before I can implement a live chat, which will allow the server hoster to join chats via the forked server connection, I need to update my netsnake code to support `AF_UNIX` clients and servers. 
