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

#include "../core/snakeSuite/snakeSuite.h"

#include "../core/io/morningIO.class.h"

#include "../core/error/morningException.class.h"

#include "../core/algo/morningAlgorithms.class.h"

#include "../core/config/morningConfig.class.h"

#include "../core/managers/morningKeyManager.class.h"
#include "../core/managers/morningMessage.class.h"

#include "../core/network/morningServer.class.h"
#include "../core/network/morningClient.class.h"

#include "../core/cliMenu/morningMenu.class.h"
#include "../core/cliMenu/morningClientMenu.class.h"
#include "../core/cliMenu/morningManagerMenu.class.h"
#include "../core/cliMenu/morningInboxMenu.class.h"
#include "../core/cliMenu/morningConfigMenu.class.h"

#include "../core/morningMessenger.class.h"

int main(int argc, char *argv[]){
	while(1){
		printf("hello\n");
		sleep(3);
	}
	exit(EXIT_SUCCESS);
}
