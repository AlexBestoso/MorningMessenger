BinaryName = "./bin/MorningMessenger"
all:
	g++ main.cc -lxml2 -lcrypto (shell mysql_config --cflags) -o $(BinaryName) $(shell mysql_config --libs)
clean:
	rm $(BinaryName)
install:
	./install.sh
uninstall:
	./uninstall.sh
