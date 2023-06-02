struct morning_key_file{
	string publickey = "";
	string alias = "";
	string message = "";
};
typedef struct morning_key_file keyfile;
class MorningKeyManager{
	private:
		string fileName = "";
		string publicKey = "";
		string alias = "";
		string message = "";
		
		EncryptionSnake encryptionSnake;
		XmlSnake xmlSnake;
		FileSnake fileSnake;

		MorningConfig config;
		MorningIO io;
	public:
	
	int untrustedKeyCount(void){
		int ret = 0;
		string *files = fileSnake.listDir(config.getUntrustedKeysLoc());
		for(int i=0; files[i] != ""; i++){
			if(files[i] != ""){
				ret++;
			}
		}
		delete[] files;
		return ret;
	}

	bool denyUntrustedKey(int id){
		int keyCount = untrustedKeyCount();
		if(keyCount <= 0 || id >= keyCount)
			return false;
		string *files = fileSnake.listDir(config.getUntrustedKeysLoc());
		string target = config.getUntrustedKeysLoc() + "/" + files[id];
		if(!fileSnake.removeFile(target)){
			return false;
		}
		delete[] files;
		return true;
	}

	bool approveUntrustedKey(int id){
		int keyCount = untrustedKeyCount();
                if(keyCount <= 0 || id >= keyCount)
                        return false;
                string *files = fileSnake.listDir(config.getUntrustedKeysLoc());
                string target = config.getUntrustedKeysLoc() + "/" + files[id];
		string newLoc = config.getTrustedKeysLoc() + "/" + files[id];
		delete[] files;

		if(!fileSnake.makeDir(newLoc)){
			fileSnake.removeDirRecursive(newLoc);
			io.outf(MORNING_IO_ERROR, "Failed to make directory '%s'\n", newLoc.c_str());
			return false;
		}

		newLoc = newLoc + "/user.xml";
		size_t fileSize = fileSnake.getFileSize(target);
		char *buf = new char[fileSize];
		if(!fileSnake.readFile(target, buf, fileSize)){
			io.outf(MORNING_IO_ERROR, "Failed to read file '%s'\n", target.c_str());
			return false;
		}

		if(!fileSnake.writeFileTrunc(newLoc, buf, fileSize)){
			io.outf(MORNING_IO_ERROR, "Failed to write file '%s'\n", newLoc.c_str());
			return false;
		}
		delete[] buf;

		if(!fileSnake.removeFile(target)){
                        return false;
                }

		return true;
	}

	keyfile *fetchUntrustedKeys(){
		keyfile *untrustedKeys = NULL;
		size_t keyCount = untrustedKeyCount();
		if(keyCount <= 0){
			return NULL;
		}
		untrustedKeys = new keyfile[keyCount];
		string *files = fileSnake.listDir(config.getUntrustedKeysLoc());
		for(int i=0; i<keyCount; i++){
			string fileName = config.getUntrustedKeysLoc() + "/" + files[i];
			if(!xmlSnake.openFileReader(fileName)){
                                throw MorningException("Failed to open key file '%s'", fileName.c_str());
                        }
                        string previous = "";
                        while(xmlSnake.readLineReader()){
                                if(xmlSnake.readResult.name == "#text" && previous == "alias"){
                                        untrustedKeys[i].alias = xmlSnake.readResult.value;
                                }
                                if(xmlSnake.readResult.name == "#text" && previous == "message"){
                                        untrustedKeys[i].message = xmlSnake.readResult.value;
                                }
                                if(xmlSnake.readResult.name == "#text" && previous == "publickey"){
                                        untrustedKeys[i].publickey = xmlSnake.readResult.value;
                                }

                                previous = xmlSnake.readResult.name;
                        }
                        xmlSnake.closeReader();
		}	
		delete[] files;
		return untrustedKeys;
	}
	bool createUntrusted(string pub, string name, string msg){
		fileName = "";
		if(pub == ""){
			io.out(MORNING_IO_ERROR, "No public key\n");
			return false;
		}
		size_t pubkeySize = pub.length();
		if(pubkeySize > 1024)
			pubkeySize = 1024;
		fileName = config.getUntrustedKeysLoc() + "/" + encryptionSnake.sha256(pub, pubkeySize, false);
		if(encryptionSnake.didFail()){
			return false;
		}
		if(fileSnake.fileExists(fileName)){
			io.out(MORNING_IO_ERROR, "Key is already in the untrusted store.\n");
			return false;
		}

		publicKey = pub;
		alias = name;
		message = msg;

		if(!xmlSnake.openFileWriter(fileName))
                        throw MorningException("Failed to open '%s' for writing.", fileName.c_str());
                if(!xmlSnake.startWritingFile())
                        throw MorningException("Failed to start writing config file.");
                if(!xmlSnake.startWritingElement("root"))
                        throw MorningException("Failed to create <root> element.");
		
		if(!xmlSnake.writeElement("alias", alias))
                	throw MorningException("Failed to write alias to key file.");
		if(!xmlSnake.writeElement("message", message))
			throw MorningException("Failed to write message to key file.");
		if(!xmlSnake.writeElement("publickey", publicKey))
			throw MorningException("Failed to write public key to key file.");

		if(!xmlSnake.stopWritingElement())
                        throw MorningException("Failed to stop writing trusted keys storage location to config file.");
                if(!xmlSnake.stopWritingFile())
                	throw MorningException("Failed to stop writing file.");
                xmlSnake.closeFileWriter();

		return true;
	}
};
