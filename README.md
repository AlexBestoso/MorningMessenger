# MorningMessenger

## Install Procedure
1. Run the command `make install` and the requirements for the application will be installed and configured. Authentication may be requred.
2. Run the command `make` and the application will compile.
3. launch the application by running `./run` from the terminal.

## Uninstall Procedure
1. Run the command `make uninstall` and all processes, folders, and users created by the installer will be removed. You can optionally remove the installed dependancies if you desire; but by default this is skipped.

### Message From Morning Star
1. You can now generate your credentials from a fresh start and can decrypt and load your config file and key pairs. 
2. There's still no messenger; But that comes next, I swear! 
3. I've gotta figure out the best way to intialize the server and client. 
4. I'be also gotta implement a system for updating the config file. As the thing can't be modifyed without it being decyrpted by the program. 
5. Going to build out two types of messaging. "One-Way Session" chat and "Duel Server" chat. One way will be for enabling a one-way dialup scheme. IE, client calls server' but server cannot call client.
