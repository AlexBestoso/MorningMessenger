class MorningServer{
	private:
		NetSnake netSnake;
		FileSnake fileSnake;
		EncryptionSnake encryptionSnake;
		MorningConfig config;
		
	public:
	pid_t pid = -1;
	void setConfig(MorningConfig config){
		this->config = config;
	}
	void setEncryptionSnake(EncryptionSnake encryptionSnake){
		this->encryptionSnake = encryptionSnake;
	}
	void clearLockFile(void){
		string lockFile = config.getServerLockFile();
		fileSnake.removeFile(lockFile);
	}

	bool lockHeld(void){
		string lockFile = config.getServerLockFile();
		return fileSnake.fileExists(lockFile);
	}

	void killProcess(){
		kill(pid, SIGKILL);
                clearLockFile();
                pid = -1;
	}

	bool obtainLockFile(void){
		string lockFile = config.getServerLockFile();
		if(fileSnake.fileExists(lockFile))
			return false;
		if(!fileSnake.writeFileTrunc(lockFile, (char *)"1", 1))
			throw MorningException("Failed to obtain lock file.");
		return true;
	}

	void launchServer(){
		try{
			mornconf cfg = config.getConfig();
			if(!netSnake.createInetServer(cfg.serverport)){
				throw MorningException(netSnake.errorMessage());
			}
		
			bool running = true;
			while(running){
				if(!netSnake.listenAndConnect()){
					throw MorningException(netSnake.errorMessage());
				}
				if(fork() == 0){
					string msg = "My [def not a derogitory saying]! How are you?\n";
					netSnake.serverSend(msg.c_str(), msg.length());	
					char *falicSausage = new char[128];
					netSnake.serverRecv(falicSausage, 128, 0);
					netSnake.closeConnection();
					exit(EXIT_SUCCESS);
				}
				netSnake.closeConnection();
			}
		}catch(exception &e){
			string what = e.what();
			what = "MorningServer::launchServer() | Critical Failure.\n" + what;
			throw MorningException(what);
		}
	}	
};
