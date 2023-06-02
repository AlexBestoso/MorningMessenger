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

### Messages From Morning Star
1. Version `0.2.1 alpha`
2. Next I'll be building out a system for notifying you when an untrusted key is added as well as a system for apporving and denying the keys.
3. There's still no messaging/chat capability, lol, but it's comming soon. Before I can implement a live chat, which will allow the server hoster to join chats via the forked server connection, I need to update my netsnake code to support `AF_UNIX` clients and servers. 
