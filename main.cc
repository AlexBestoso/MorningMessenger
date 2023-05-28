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
using namespace std;
#include "./snakeSuite/snakeSuite.h"
#include "./core/morningIO.class.h"
#include "./core/morningException.class.h"
#include "./core/morningAlgorithms.class.h"
#include "./core/morningConfig.class.h"
#include "./core/morningMenu.class.h"
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
				case MORNING_MENU_QUIT:
					running = false;
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
