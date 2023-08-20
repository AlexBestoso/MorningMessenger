#define __SERVER_MAIN__ -1
#define __SERVER_CONFIG__ 0
#define __SERVER_START__ 1
#define __SERVER_STOP__ 2
#define __SERVER_RESTART__ 3
#define __SERVER_STATUS__ 4
#define __SERVER_BACK__ 5

#define __SERVER_CONFIG_SERVNAME__ 0
#define __SERVER_CONFIG_HOSTNAME__ 1
#define __SERVER_CONFIG_PORT__ 2
#define __SERVER_CONFIG_NEWKEYS__ 3
#define __SERVER_CONFIG_TORMODE 4
#define __SERVER_CONFIG_BACK__ 5

#include "./server.form.h"
#include "./server.configForm.h"

class ServerView : public View{
	private:
		ServerViewForm serverForm;
		ServerConfigForm configForm;
		MorningIO io;

		void startService(){
                        io.out(MORNING_IO_GENERAL, "Starting Service...\n");
                        if(fork() == 0){
                                char *args[] = {
                                        (char *)"/usr/sbin/service",
                                        (char *)"morningService",
                                        (char *)"start",
                                        NULL
                                };
                                execvp(args[0], args);
                                exit(0);
                        }
                }
                void stopService(){
                        io.out(MORNING_IO_GENERAL, "Stopping Service...\n");
                        if(fork() == 0){
                                char *args[] = {
                                        (char *)"/usr/sbin/service",
                                        (char *)"morningService",
                                        (char *)"stop",
                                        NULL
                                };
                                execvp(args[0], args);
                                exit(0);
                        }
                }
                void statusService(){
                        io.out(MORNING_IO_GENERAL, "Getting Service Status...\n");
                        if(fork() == 0){
                                char *args[] = {
                                        (char *)"/usr/sbin/service",
                                        (char *)"morningService",
                                        (char *)"status",
                                        NULL
                                };
                                execvp(args[0], args);
                                exit(0);
                        }
                }

                void restartService(){
                        io.out(MORNING_IO_GENERAL, "Restarting Service...\n");
                        if(fork() == 0){
                                char *args[] = {
                                        (char *)"/usr/sbin/service",
                                        (char *)"morningService",
                                        (char *)"restart",
                                        NULL
                                };
                                execvp(args[0], args);
                                exit(0);
                        }
                }
	public:
		int viewContext = __SERVER_MAIN__;
		void init(int viewId, float bgColor[4]){
			View::init(viewId, bgColor);
		}

		int run(void){
			 View::run();
			 switch(viewContext){
				case __SERVER_CONFIG__:
					configForm.run();
					break;
				default:
					serverForm.run();
					break;
			 }
			 return getViewId();
		}

		int mousePassive(float x, float y){
			switch(viewContext){
				case __SERVER_CONFIG__:
					configForm.mousePassive(x, y);
					break;
				default:
					serverForm.mousePassive(x, y);
					break;
			}
                        return getViewId();
                }

		int mouseClick(int button, int state, float x, float y){
			int clickCode = -1;
			serverconfig_t cfg;
			MorningServer server;
			MorningIO io;
			MorningConfig conf;
			morningconfig_t c;
			switch(viewContext){
				case __SERVER_CONFIG__:
					clickCode = configForm.mouseClick(button, state, x, y);
					if(clickCode != -1 && (clickCode&0b1) == 1 && (clickCode>>1) == 0){
						switch(configForm.getClickedButton()){
							case __SERVER_CONFIG_BACK__:
								viewContext = __SERVER_MAIN__;
								break;
							case __SERVER_CONFIG_SERVNAME__:
								server.loadConfigs();
								cfg = server.getServerConfig();
								cfg.serverName = configForm.serverNameInput();
								server.updateConfig(cfg);
								io.outf(MORNING_IO_GENERAL, "Updated server name with '%s'\n", cfg.serverName.c_str());
								break;
							case __SERVER_CONFIG_HOSTNAME__:
								server.loadConfigs();
								cfg = server.getServerConfig();
								cfg.serverHost = configForm.serverHostInput();
								server.updateConfig(cfg);
								io.outf(MORNING_IO_GENERAL, "Updated host name with '%s'\n", cfg.serverHost.c_str());
								break;
							case __SERVER_CONFIG_PORT__:
								server.loadConfigs();
								cfg = server.getServerConfig();
								cfg.serverPort = atoi(configForm.serverPort().c_str());
								server.updateConfig(cfg);
								io.outf(MORNING_IO_GENERAL, "Updated port number with '%d'\n", cfg.serverPort);
								break;
							case __SERVER_CONFIG_NEWKEYS__:
								io.out(MORNING_IO_NONE, "Generating new key pair, please be patient...\n");
                        					server.updateConfig(server.generateNewKeyPair());
                        					io.outf(MORNING_IO_SUCCESS, "Updated server key pair\n");
								break;
							case __SERVER_CONFIG_TORMODE:
								c = conf.getConfig();
								c.torMode = (c.torMode)? false : true;
								conf.generateConfig(c);
								io.outf(MORNING_IO_GENERAL, "Toggled tor %s\n", (c.torMode ? "on" : "off"));
								break;
						}
					}
					break;
				default:
					clickCode = serverForm.mouseClick(button, state, x, y);
					if(clickCode != -1 && (clickCode&0b1) == 1 && (clickCode>>1) == 0){
						switch(serverForm.getClickedButton()){
							case __SERVER_BACK__:
								return __SERVER_BACK__;
							case __SERVER_START__:
								startService();
								break;
							case __SERVER_STOP__:
								stopService();
								break;
							case __SERVER_RESTART__:
								restartService();
								break;

							case __SERVER_STATUS__:
								statusService();
								break;
							case __SERVER_CONFIG__:
								viewContext = __SERVER_CONFIG__;
								break;
						}
					}
					break;
			}
			return -1;
		}

		int keyDown(unsigned char key, int mouseX, int mouseY){
			switch(viewContext){
				case __SERVER_CONFIG__:
					configForm.keyDown(key, mouseX, mouseY);
					break;
				default:
					break;
			}
			return getViewId();
		}

		int specialKeyDown(int key, int mouseX, int mouseY){return 0;}
		int specialKeyUp(int key, int mouseX, int mouseY){return 0;}
		int keyUp(unsigned char key, int mouseX, int mouseY){return 0;}
		void idle(void){}
		void mouseMovement(void){}
};
