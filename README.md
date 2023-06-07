# MorningMessenger
You can now view and share messages with other people using this software, assuming that you've shared your public keys with each other. Which is done automatically by the system.

## Install Procedure
1. Run the command `make install` and the requirements for the application will be installed and configured. Authentication may be requred.
2. Run the command `make` and the application will compile.
3. launch the application by running `./run` from the terminal.

## Uninstall Procedure
1. Run the command `make uninstall` and all processes, folders, and users created by the installer will be removed. You can optionally remove the installed dependancies if you desire; but by default this is skipped.

### Changes in this version.
1. You can now send messages to servers.
2. Added inbox menu
3. You can now view, save, and delete messages that you receive from people.

### Messages From Morning Star
1. Version `0.3.3 alpha`
2. I'll be looking into creating a notification system so that you don't need to manually refresh the message cache.
3. I'll also be looking into isolating the server code into a seperate binary so that it can be run as a service on system startup.
4. Will test TOR compatibility soon..
5. I'm going to add base64 to encryption snake so that we can encrypt and store data within xml files themselves.
6. Once communications are properly established, I will be modifying the key management to enable mutual authentication.
7. Before I can implement a live chat, which will allow the server hoster to join chats via the forked server connection, I need to update my netsnake code to support `AF_UNIX` clients and servers. 
