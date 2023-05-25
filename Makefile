BinaryName = "MorningMessenger"
all:
	g++ main.cc -lxml2 -lcrypto -o $(BinaryName)
clean:
	rm ./$(BinaryName)
install:
	./install.sh
uninstall:
	./uninstall.sh
