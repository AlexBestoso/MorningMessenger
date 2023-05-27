# MorningMessenger

## Install Procedure
1. Run the command `make install` and the requirements for the application will be installed and configured. Authentication may be requred.
2. Run the command `make` and the application will compile.
3. launch the application by running `./run` from the terminal.

## Uninstall Procedure
1. Run the command `make uninstall` and all processes, folders, and users created by the installer will be removed. You can optionally remove the installed dependancies if you desire; but by default this is skipped.

### Message From Morning Star
1. The application now has a first start setup procedure. Can't do any messaging yet. But you can setup the required files and folders. As well as encrypt the generated private key and config file.
2. Next push will bring the authentication process full cirlce. You currently provide a username, password, and pin upon startup. Which then creates all the files needed to run the program. The sensitive files are encrypted using your credentials so that the only way to run the program without erasing the config file is to have the proper creds.
