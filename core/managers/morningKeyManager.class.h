struct morning_key_file{
	string publickey = "";
	string alias = "";
	string message = "";
};
typedef struct morning_key_file keyfile;

struct morning_key_record{
	int id = -1;
	bool trusted = false;
	string alias = "";
	string cbip = "";
	string cbhost = "";
	int cbport = -1;
	string date = "";
	string pubkey = "";
	string justification = "";
};
typedef struct morning_key_record friendkey_t;

class MorningKeyManager{
	private:
		string fileName = "";
		string publicKey = "";
		string alias = "";
		string message = "";

		string tableName = "morn_friends";
		string colNames[9] = {
			"id",
			"trusted",
			"alias",
			"cbip",
			"cbhost",
			"cbport",
			"date",
			"pubkey",
			"justification"
		};
		int sql_id = 0;
                int sql_trusted = 1;
                int sql_alias = 2;
		int sql_cbip = 3;
                int sql_cbhost = 4;
                int sql_cbport = 5;
                int sql_date = 6;
                int sql_key = 7;
		int sql_justification = 8;

		
		EncryptionSnake encryptionSnake;
		XmlSnake xmlSnake;
		FileSnake fileSnake;
		SqlSnake sqlSnake;

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

	bool isKeyTrusted(string pubkey){
		size_t pubkeySize = pubkey.length();
                if(pubkeySize > 1024)
                        pubkeySize = 1024;
		string keyHash = encryptionSnake.sha256(pubkey, pubkeySize, false);
		string dirLoc = config.getTrustedKeysLoc() + "/" +keyHash;
		if(fileSnake.fileExists(dirLoc) && fileSnake.getFileType(dirLoc) == FILE_SNAKE_DIR){
			return true;
		}
		return false;
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

	sqltable_t generateTable(void){
                sqltable_t ret;
                ret.name = tableName;
                ret.colCount = 9;
                ret.cols = new sqlcolumn_t[9];
                ret.cols[sql_id] = sqlSnake.generatePrimaryColumn(colNames[sql_id], "INT", "NOT NULL AUTO_INCREMENT");
		ret.cols[sql_trusted] = sqlSnake.generateColumn(colNames[sql_trusted], "INT(1)", "NOT NULL DEFAULT 0");
		ret.cols[sql_alias] = sqlSnake.generateColumn(colNames[sql_alias], "VARCHAR(50)", "NOT NULL");
		ret.cols[sql_cbip] = sqlSnake.generateColumn(colNames[sql_cbip], "VARCHAR(20)", "NOT NULL");
		ret.cols[sql_cbhost] = sqlSnake.generateColumn(colNames[sql_cbhost], "VARCHAR(300)", "NOT NULL");
		ret.cols[sql_cbport] = sqlSnake.generateColumn(colNames[sql_cbport], "VARCHAR(6)", "NOT NULL");
		ret.cols[sql_date] = sqlSnake.generateColumn(colNames[sql_date], "TIMESTAMP", "DEFAULT CURRENT_TIMESTAMP");
                ret.cols[sql_key] = sqlSnake.generateColumn(colNames[sql_key], "TEXT", "NOT NULL");
		ret.cols[sql_justification] = sqlSnake.generateColumn(colNames[sql_justification], "VARCHAR(200)", "");
                return ret;
        }

	void setupTable(void){
		sqlSnake = config.getSql();
		sqltable_t t = generateTable();
                if(!sqlSnake.createTable(t)){
                        throw MorningException(sqlSnake.getError());
                }
	}

	bool createUntrusted(friendkey_t friendKey){
		if(friendKey.pubkey == ""){
			io.out(MORNING_IO_ERROR, "No public key\n");
			return false;
		}
		sqlSnake = config.getSql();

		sqlselect_t select;
		select.table = tableName;
		select.colCount = 1;
		select.cols = new string[1]{
			colNames[sql_id]
		};	
		select.hasWhere = true;
		select.wheres = sqlSnake.addToWhere(
				select.wheres, 
				sqlSnake.generateWhere(colNames[sql_key], "=", friendKey.pubkey, true), 
				""
		);
		if(!sqlSnake.secureSelect(select)){
			throw MorningException("Failed to run key management integrity check : %s\n", sqlSnake.getError().c_str());
		}
		sqlresults_t result;
		result = sqlSnake.getResults();
		if(result.resultCount > 0){
			return false;
		}

		sqlinsert_t insert;
		insert.table = tableName;
		insert.count = 6;
		insert.cols = new string[6]{
			colNames[sql_key],
			colNames[sql_alias],
			colNames[sql_justification],
			colNames[sql_cbip],
			colNames[sql_cbhost],
			colNames[sql_cbport]
		};
		insert.values = new string[6]{
			friendKey.pubkey,
			friendKey.alias,
			friendKey.justification,
			friendKey.cbip,
			friendKey.cbhost,
			to_string(friendKey.cbport)
		};
		if(!sqlSnake.secureInsert(insert)){
			throw MorningException("Failed to store untrusted key.\n");
		}
		return true;
	}
};
