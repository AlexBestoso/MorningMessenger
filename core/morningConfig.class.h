struct morning_config_xml{
	string messages;
	string prikey;
	string pubkey;
	string serverhost;
	int serverport;
	string trustedkeys;
	
};
typedef struct morning_config_xml mornconf;

class MorningConfig{
	private:
		XmlSnake xml;
		mornconf config;
	public:
		bool newConfig(string fileName, mornconf config){
			this->config = config;

			if(!xml.openFileWriter(fileName))
				throw MorningException("Failed to open '%s' for writing.", fileName.c_str());
			if(!xml.startWritingFile())
				throw MorningException("Failed to start writing config file.");
			if(!xml.startWritingElement("root"))
				throw MorningException("Failed to create <root> element.");
			/* Begin creating config body*/
			
			if(!xml.writeElement("messages", config.messages))
				throw MorningException("Failed to write message storage to config file.");

			if(!xml.writeElement("prikey", config.prikey))
				throw MorningException("Failed to write private key location to config file.");

			if(!xml.writeElement("pubkey", config.pubkey))
				throw MorningException("Failed to write public key location to config file.");

			if(!xml.writeElement("serverhost", config.serverhost))
				throw MorningException("Failed to write server host to config file.");
			
			string port_str = to_string(config.serverport);
			if(!xml.writeElement("serverport", port_str))
				throw MorningException("Failed to write server port to config file.");

			if(!xml.writeElement("trustedkeys", config.trustedkeys))
				throw MorningException("Failed to write trusted keys storage location to config file.");
			
			/* Crafting done, level 99 skill*/
			if(!xml.stopWritingElement())
				throw MorningException("Failed to stop writing trusted keys storage location to config file.");
			if(!xml.stopWritingFile())
				throw MorningException("Failed to stop writing file.");
			xml.closeFileWriter();
			
			return true;
		}
};
