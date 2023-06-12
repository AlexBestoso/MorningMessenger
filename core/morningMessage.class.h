struct morning_message{
	string clientHost = "";
	string messageDate = "";
	string messageBody = "";
	string messageLength = "";
};
typedef struct morning_message mornmsg;
class MorningMessage{
	private:
		MorningIO io;
		MorningConfig config;
		MorningAlgorithms algorithm;
		
		EncryptionSnake encryptionSnake;
		XmlSnake xmlSnake;
		FileSnake fileSnake;

		string storageLocation = "";

		size_t encryptionResultLen = 0;
		string encryptMessage(string msg, size_t msgLen){
			string _key = algorithm.deriveConfigEncryptionKey(config.username, config.password);
                        unsigned char aesKey[32];
                        for(int i=0; i<32; i++)
                                aesKey[i] = _key[i];

                        string _iv = algorithm.deriveConfigEncryptionIv(config.username, config.password);
                        unsigned char aesIv[16];
                        for(int i=0; i<16; i++)
                                aesIv[i] = _iv[i];

			string ret = encryptionSnake.aes256cbc(true, msg, msgLen, aesKey, aesIv);
                        if(encryptionSnake.didFail()){
                                encryptionSnake.printError();
                                throw MorningException("Failed to encrypt message.");
                        }
                        encryptionResultLen = encryptionSnake.getResultLen();
			return ret;
		}

		size_t decryptionResultLen = 0;
		string decryptMessage(string msg, size_t msgLen){
			string _key = algorithm.deriveConfigEncryptionKey(config.username, config.password);
                        unsigned char aesKey[32];
                        for(int i=0; i<32; i++)
                                aesKey[i] = _key[i];

                        string _iv = algorithm.deriveConfigEncryptionIv(config.username, config.password);
                        unsigned char aesIv[16];
                        for(int i=0; i<16; i++)
                                aesIv[i] = _iv[i];

			string ret = encryptionSnake.aes256cbc(false, msg, msgLen, aesKey, aesIv);
                        if(encryptionSnake.didFail()){
                                encryptionSnake.printError();
                                throw MorningException("Failed to decrypt message.");
                        }
                        decryptionResultLen = encryptionSnake.getResultLen();	
			return ret;
		}

		string encryptStoredMessage(string target, size_t targetSize){
			string ret = "";
			
			string _key = algorithm.deriveConfigEncryptionKey(config.username, config.password);
			unsigned char key[32];
			for(int i=0; i<32; i++)
				key[i] = _key[i];

			string _iv = algorithm.deriveConfigEncryptionIv(config.username, config.password);
			unsigned char iv[16];
			for(int i=0; i<16; i++)
				iv[i] = _iv[i];

			/*ret = encryptionSnake.aes256cbc(true, target, targetSize, key, iv);
			if(encryptionSnake.didFail())
				throw new MorningException("Failed to encrypt recived message for storage.");
			size_t eLen = encryptionSnake.getResultLen();
*/
			//ret = encryptionSnake.base64(true, ret, eLen);
			ret = encryptionSnake.base64(true, target, targetSize);
			if(encryptionSnake.didFail())
                                throw new MorningException("Failed to encode recived message for storage.");

			return ret;
		}

		string decryptStoredMessage(string target, size_t targetSize){
			string ret = "";
			ret = encryptionSnake.base64(false, target, targetSize);
			if(encryptionSnake.didFail())
                                throw new MorningException("Failed to decode recived message from storage.");

			string _key = algorithm.deriveConfigEncryptionKey(config.username, config.password);
                        unsigned char key[32];
                        for(int i=0; i<32; i++)
                                key[i] = _key[i];

                        string _iv = algorithm.deriveConfigEncryptionIv(config.username, config.password);
                        unsigned char iv[16];
                        for(int i=0; i<16; i++)
                                iv[i] = _iv[i];

			/*size_t eLen = encryptionSnake.getResultLen();
			ret = encryptionSnake.aes256cbc(false, ret, eLen, key, iv);
                        if(encryptionSnake.didFail())
                                throw new MorningException("Failed to encrypt recived message for storage.");
*/
			return ret;
		}
	public:
	
	void setConfig(MorningConfig conf){
		config = conf;
	}

	int getMessageCount(void){
		string *files = fileSnake.listDir(storageLocation);
		if(files == NULL)
			return 0;

		int ret = 0;
		while(files[ret] != ""){
			ret++;
		}
		delete[] files;
		return ret;
	}

	void setStorageLocation(string loc){
		storageLocation = loc;
	}
	
	string getCurrentDateTime(void){
		time_t t = time(NULL);
		struct tm *tm = localtime(&t);
		char str[64];
		strftime(str, sizeof(str), "%c", tm);
		string ret = str;
		return ret;
	}

	mornmsg getMessageFromFile(string path){
		mornmsg ret;
		string previous = "";
		if(!xmlSnake.openFileReader(path)){
			return ret;
		}
		while(xmlSnake.readLineReader()){
			if(xmlSnake.readResult.name == "#text" && previous == "clientHost"){
				ret.clientHost = xmlSnake.readResult.value;
			}
			if(xmlSnake.readResult.name == "#text" && previous == "messageDate"){
				ret.messageDate = xmlSnake.readResult.value;
			}
			if(xmlSnake.readResult.name == "#text" && previous == "messageBody"){
				ret.messageBody = decryptStoredMessage(xmlSnake.readResult.value, xmlSnake.readResult.value.length());
			}
			if(xmlSnake.readResult.name == "#text" && previous == "messageLength"){
				ret.messageLength = xmlSnake.readResult.value;
			}
			previous = xmlSnake.readResult.name;
		}
		xmlSnake.closeReader();
		return ret;
	}

	bool storeClientMessage(mornmsg msg, string pubkey){
		size_t pubkeySize = pubkey.length();
                if(pubkeySize > 1024)
                        pubkeySize = 1024;
                string keyHash = encryptionSnake.sha256(pubkey, pubkeySize, false);
		storageLocation = config.getTrustedKeysLoc() + "/" + keyHash;
		if(!fileSnake.fileExists(storageLocation)){
			return false;
		}

		storageLocation = storageLocation + "/msg";
		if(!fileSnake.fileExists(storageLocation)){
			if(!fileSnake.makeDir(storageLocation))
				return false;
		}

		string dateHash = encryptionSnake.sha256(msg.messageDate, msg.messageDate.length(), false);
		string msgCountStr = to_string(getMessageCount()+1);
		string fileName = storageLocation + "/" + msgCountStr + "_" + dateHash;

		msg.messageBody = encryptStoredMessage(msg.messageBody, msg.messageBody.length());
		msg.messageLength = to_string(encryptionSnake.getResultLen()); 
		
		// Store data in xml file
		if(!xmlSnake.openFileWriter(fileName))
                        throw MorningException("Failed to open '%s' for writing.", fileName.c_str());
                if(!xmlSnake.startWritingFile())
                        throw MorningException("Failed to start writing config file.");
                if(!xmlSnake.startWritingElement("root"))
                        throw MorningException("Failed to create <root> element.");

		if(!xmlSnake.writeElement("clientHost", msg.clientHost))
                        throw MorningException("Failed to write clientHost to msg file.");
		if(!xmlSnake.writeElement("messageDate", msg.messageDate))
                        throw MorningException("Failed to write messageDate to msg file.");
		if(!xmlSnake.writeElement("messageBody", msg.messageBody))
                        throw MorningException("Failed to write messageBody to msg file.");
		if(!xmlSnake.writeElement("messageLength", msg.messageLength))
                        throw MorningException("Failed to write messageLength to msg file.");
		
		if(!xmlSnake.stopWritingElement())
                        throw MorningException("Failed to stop writing trusted keys storage location to config file.");
                if(!xmlSnake.stopWritingFile())
                        throw MorningException("Failed to stop writing file.");
                xmlSnake.closeFileWriter();
		return true;
	}
};
