# MorningMessenger

## Install Procedure
1. Run the command `make install` and the requirements for the application will be installed and configured. Authentication may be requred.
2. Run the command `make` and the application will compile.
3. launch the application by running `./run` from the terminal.

## Uninstall Procedure
1. Run the command `make uninstall` and all processes, folders, and users created by the installer will be removed. You can optionally remove the installed dependancies if you desire; but by default this is skipped.

### Changes in this version.
1. Added full key exchange. "MLS" - Morning Layer Security
2. People who want to chat can now send their public keys to the server for the keys to be placed into the untrusted store for review and approval.
3. You can now review untrusted keys and approve or deny them for use in messageing by using the `manage` menu. Trusted keys are moved to the "trusted key folder"

### Messages From Morning Star
1. Version `0.2.2 alpha`
2. In the next update you will finally be able to send messages to the server in question. the standard chat, which is being implemented first, will function like an email server in the sense that you will receive a message and it will be stored for viewing later. 
3. Once communications are properly established, I will be modifying the key management to enable mutual authentication.
4. Before I can implement a live chat, which will allow the server hoster to join chats via the forked server connection, I need to update my netsnake code to support `AF_UNIX` clients and servers. 
