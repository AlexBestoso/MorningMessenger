BinaryName = "./bin/MorningMessenger"
Flags = -lglut -lGL -lX11 -lasound -lpulse -lpulse-simple -ldl -lxml2 -lcrypto $(shell mysql_config --cflags) -o $(BinaryName) $(shell mysql_config --libs)

all:
	g++ main.cc $(Flags)
full:
	g++ main.cc $(Flags)
	make -C ./service/ all
full_uninstall:
	./uninstall.sh
	make -C ./service/ uninstall
clean:
	rm $(BinaryName)
	make -C ./service/ clean
server:
	make -C ./service/ all
	make -C ./service/ install
uninstall_server:
	make -C ./service/ uninstall
install:
	./install.sh
uninstall:
	./uninstall.sh
first_build:
	./install.sh
	g++ main.cc $(Flags)
	make -C ./service/ all
	make -C ./service/ install
	./run
tor:
	./install_tor.sh
