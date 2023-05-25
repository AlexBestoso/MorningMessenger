# MorningMessenger

## Install Procedure
1. Make the installer and uninstaller scrips executable with `chmod u+x install.sh` and `chmod u+x ./uninstall.sh` if they're not already executable.
2. Run the command `make install` and the requirements for the application will be installed and configured. Authentication may be requred.
3. Run the command `make` and the application will compile.
4. You can now launch the application by running `./MorningMessenger` from the terminal.

## Uninstall Procedure
1. Run the command `make uninstall` and all processes, folders, and users created by the installer will be removed. You can optionally remove the installed dependancies if you desire; but by default this is skipped.

### Message From Morning Star
Before I get to the code writing, I'm going to set up a brand new linux VM to test the install and properly document any unmentioned dependacies.
