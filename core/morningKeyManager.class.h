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
