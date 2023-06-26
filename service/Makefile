all:
	g++ service.cc -lxml2 -lcrypto $(shell mysql_config --cflags) -o ./morningservice.out $(shell mysql_config --libs)
clean:
	rm ./morningservice.out
install:
	./installServiceFiles.sh
uninstall:
	./uninstallServiceFiles.sh
