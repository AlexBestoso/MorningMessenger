#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <exception>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <iostream>
#include <signal.h>
#include <time.h>
using namespace std;
#include "./core/snakeSuite/snakeSuite.h"

#include "./core/io/morningIO.class.h"

#include "./core/error/morningException.class.h"

#include "./core/algo/morningAlgorithms.class.h"

#include "./core/config/morningConfig.class.h"

#include "./core/managers/morningKeyManager.class.h"
#include "./core/managers/morningMessage.class.h"

#include "./core/network/morningServer.class.h"
#include "./core/network/morningClient.class.h"

#include "./core/cliMenu/morningMenu.class.h"
#include "./core/cliMenu/morningClientMenu.class.h"
#include "./core/cliMenu/morningManagerMenu.class.h"
#include "./core/cliMenu/morningInboxMenu.class.h"

#include "./core/morningMessenger.class.h"

int main(void){
	try{
		MorningMessenger MM;
		bool running = true;
		while(running){
			switch(MM.getMenuContext()){
				case MORNING_MENU_MAIN:
					MM.runMainMenu();
					break;
				case MORNING_MENU_LAUNCH:
					if(!MM.launchServer()){
						
					}else{
					
					}
					break;
				case MORNING_MENU_CONNECT:
					if(!MM.connectToServer()){
					
					}else{
					
					}
					break;
				case MORNING_MENU_CONFIG:
					if(!MM.manageConfigFile()){
					
					}else{
					
					}
					break;
				case MORNING_MENU_MANAGE:
					MM.keyManager();
					break;
				case MORNING_MENU_INBOX:
					MM.inbox();
					break;
				case MORNING_MENU_QUIT:
					running = false;
					MM.quitMessenger();
					break;
				default:
					throw MorningException("Illegal Menu Context.");
			}
		}
	}catch(exception& e){
		printf("caught in main() | %s\n", e.what());
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
