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
#include "./core/morningMessenger.class.h"

int main(void){
	try{
		MorningMessenger MM;
	}catch(exception& e){
		printf("caught in main() | %s\n", e.what());
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
