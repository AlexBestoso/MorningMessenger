/*
 * The Algorithms in this file are specific to the Morning Messenger. They're not intended to be used elsewhere; nor are that standard.
 * Althogh they're not intended for other use, feel free to use them if you please.
 * */
class MorningAlgorithms{
	private:
		EncryptionSnake encryptionSnake;
	public:

	string deriveConfigEncryptionIv(string username, string password){
		string ret = "";
                string usernameHash = encryptionSnake.sha256(username, username.length(), false);
                if(encryptionSnake.didFail()){
                        throw MorningException("MorningAlgorithms::DeriveExportKey(): Failed to hash username.");
                }
                string passwordHash = encryptionSnake.sha256(password, password.length(), false);
                if(encryptionSnake.didFail()){
                        throw MorningException("MorningAlgorithms::DeriveExportKey(): Failed to hash password.");
                }

		string mashedUser = "";
                string mashedPass = "";
                for(int i=0; i<16; i++){
                        mashedUser += usernameHash[i] ^ passwordHash[16+i];
                        mashedPass += passwordHash[i] ^ usernameHash[16+i];
                }

                string mashedUnity = mashedUser + mashedPass;
                mashedUser = encryptionSnake.sha256(mashedUser, 16, false);
                if(encryptionSnake.didFail()){
                        throw MorningException("MorningAlgorithms::DeriveExportKey(): Failed to hash mashed user.");
                }
                mashedPass = encryptionSnake.sha256(mashedPass, 16, false);
                if(encryptionSnake.didFail()){
                        throw MorningException("MorningAlgorithms::DeriveExportKey(): Failed to hash mashed pass.");
                }
		int ctrA = 0;
                int ctrB = 0;
		string disoriented = "";
                for(int i=0; i<32; i++){
                        if((i%2) == 0){
                                disoriented += mashedUnity[i] ^ mashedUser[ctrA];
                                ctrA++;
                        }else{
                                disoriented += mashedUnity[i] ^ mashedPass[ctrB];
                                ctrB++;
                        }
                }

		for(int i=0; i<16; i++){
			ret += disoriented[i] ^ disoriented[15-i];
		}
		return ret;
	}
	
	string deriveConfigEncryptionKey(string username, string password){
		string ret = "";
                string usernameHash = encryptionSnake.sha256(username, username.length(), false);
                if(encryptionSnake.didFail()){
                        throw MorningException("MorningAlgorithms::DeriveExportKey(): Failed to hash username.");
                }
                string passwordHash = encryptionSnake.sha256(password, password.length(), false);
                if(encryptionSnake.didFail()){
                        throw MorningException("MorningAlgorithms::DeriveExportKey(): Failed to hash password.");
                }
		
		string mashedCreds = "";
		string mashedUser = "";
                string mashedPass = "";

                for(int i=0; i<32; i++)
                        mashedCreds += usernameHash[i] ^ passwordHash[i];
                for(int i=0; i<16; i++){
                        mashedUser += usernameHash[i] ^ usernameHash[16+i];
                        mashedPass += passwordHash[i] ^ passwordHash[16+i];
                }
		
		mashedCreds = encryptionSnake.sha256(mashedCreds, 32, false);
		int ctrA = 0;
		int ctrB = 0;
		for(int i=0; i<32; i++){
			if((i%2) == 0){
				ret += mashedCreds[i] ^ mashedUser[ctrA];
				ctrA++;
			}else{
				ret += mashedCreds[i] ^ mashedPass[ctrB];
				ctrB++;
			}
		}
		return ret;
	}

	string deriveKeyEncryptionIv(string username, string password){
		string ret = "";
		string usernameHash = encryptionSnake.sha256(username, username.length(), false);
                if(encryptionSnake.didFail()){
                        throw MorningException("MorningAlgorithms::DeriveExportKey(): Failed to hash username.");
                }
                string passwordHash = encryptionSnake.sha256(password, password.length(), false);
                if(encryptionSnake.didFail()){
                        throw MorningException("MorningAlgorithms::DeriveExportKey(): Failed to hash password.");
                }
		
		string mashedCreds = "";
		for(int i=0; i<32; i++)
			mashedCreds += usernameHash[i] ^ passwordHash[i];

		string ivRay[4];
		ivRay[0] = encryptionSnake.sha256(mashedCreds, 32, false);
		ivRay[1] = encryptionSnake.sha256(ivRay[0], 32, false);
		ivRay[2] = encryptionSnake.sha256(ivRay[1], 32, false);
		ivRay[3] = encryptionSnake.sha256(ivRay[2], 32, false);
		
		string resultRay[4]; 
		for(int i=0; i<4; i++)
			for(int j=0; j<16; j++)
				resultRay[i] += ivRay[i][j] ^ ivRay[i][j+16];

		for(int i=0; i<16; i++)
			ret += resultRay[0][i] ^ resultRay[2][i] ^ resultRay[1][i] ^ resultRay[3][i];

		return ret;
	}
	string deriveKeyEncryptionKey(string username, string password){
		string ret = "";
		string usernameHash = encryptionSnake.sha256(username, username.length(), false);
                if(encryptionSnake.didFail()){
                        throw MorningException("MorningAlgorithms::DeriveExportKey(): Failed to hash username.");
                }
                string passwordHash = encryptionSnake.sha256(password, password.length(), false);
                if(encryptionSnake.didFail()){
                        throw MorningException("MorningAlgorithms::DeriveExportKey(): Failed to hash password.");
                }
		
		string mashedUser = "";
		string mashedPass = "";
		for(int i=0; i<16; i++){
			mashedUser += usernameHash[i] ^ usernameHash[16+i];
			mashedPass += passwordHash[i] ^ passwordHash[16+i];
		}

		string mashedUnity = mashedUser + mashedPass;
		mashedUser = encryptionSnake.sha256(mashedUser, 16, false);
		if(encryptionSnake.didFail()){
                        throw MorningException("MorningAlgorithms::DeriveExportKey(): Failed to hash mashed user.");
                }
                mashedPass = encryptionSnake.sha256(mashedPass, 16, false);
                if(encryptionSnake.didFail()){
                        throw MorningException("MorningAlgorithms::DeriveExportKey(): Failed to hash mashed pass.");
                }

		for(int i=0; i<32; i++)
			ret += mashedUser[i] ^ mashedUnity[i] ^ mashedPass[i];
		return ret;
	}

	string deriveExportKey(string username, string password, string pin){
		string ret = "";
		string usernameHash = encryptionSnake.sha256(username, username.length(), false);
		if(encryptionSnake.didFail()){
			throw MorningException("MorningAlgorithms::DeriveExportKey(): Failed to hash username.");
		}
		string passwordHash = encryptionSnake.sha256(password, password.length(), false);
		if(encryptionSnake.didFail()){
			throw MorningException("MorningAlgorithms::DeriveExportKey(): Failed to hash password.");
		}
		string pinHash = encryptionSnake.sha256(pin, pin.length(), false);
		if(encryptionSnake.didFail()){
			throw MorningException("MorningAlgorithms::DeriveExportKey(): Failed to hash pin.");
		}

		size_t hashlen = 32; // SHA 256 byte count.
		string mashedHash = "";
		for(int i=0; i<hashlen; i++){
			char tmp = usernameHash[i] ^ pinHash[i] ^ 3;
			char tmp2 = passwordHash[i] ^ pinHash[i] ^ 7;
			mashedHash += tmp ^ tmp2 ^ 13;
		}

		ret = encryptionSnake.sha256(mashedHash, hashlen, false);
		if(encryptionSnake.didFail()){
			throw MorningException("MorningAlgorithms::DeriveExportKey(): Failed final hash.");
		}
		return ret;
	}
};
